//===- LazyValueInfo.cpp - Value constraint analysis ------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file defines the interface for lazy computation of value constraint
// information.
//
//===----------------------------------------------------------------------===//

#include "llvm/Analysis/LazyValueInfo.h"
#include "llvm/ADT/DenseSet.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/Analysis/AssumptionCache.h"
#include "llvm/Analysis/ConstantFolding.h"
#include "llvm/Analysis/TargetLibraryInfo.h"
#include "llvm/Analysis/ValueTracking.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/ConstantRange.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/Dominators.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/PatternMatch.h"
#include "llvm/IR/ValueHandle.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"
#include <map>
#include <stack>
using namespace llvm;
using namespace PatternMatch;

#define DEBUG_TYPE "lazy-value-info"

char LazyValueInfo::ID = 0;
INITIALIZE_PASS_BEGIN(LazyValueInfo, "lazy-value-info",
                "Lazy Value Information Analysis", false, true)
INITIALIZE_PASS_DEPENDENCY(AssumptionCacheTracker)
INITIALIZE_PASS_DEPENDENCY(TargetLibraryInfoWrapperPass)
INITIALIZE_PASS_END(LazyValueInfo, "lazy-value-info",
                "Lazy Value Information Analysis", false, true)

namespace llvm {
  FunctionPass *createLazyValueInfoPass() { return new LazyValueInfo(); }
}


//===----------------------------------------------------------------------===//
//                               LVILatticeVal
//===----------------------------------------------------------------------===//

/// This is the information tracked by LazyValueInfo for each value.
///
/// FIXME: This is basically just for bringup, this can be made a lot more rich
/// in the future.
///
namespace {
class LVILatticeVal {
  enum LatticeValueTy {
    /// This Value has no known value yet.  As a result, this implies the
    /// producing instruction is dead.  Caution: We use this as the starting
    /// state in our local meet rules.  In this usage, it's taken to mean
    /// "nothing known yet".  
    undefined,

    /// This Value has a specific constant value.  (For integers, constantrange
    /// is used instead.)
    constant,

    /// This Value is known to not have the specified value.  (For integers,
    /// constantrange is used instead.)
    notconstant,

    /// The Value falls within this range. (Used only for integer typed values.)
    constantrange,

    /// We can not precisely model the dynamic values this value might take.
    overdefined
  };

  /// Val: This stores the current lattice value along with the Constant* for
  /// the constant if this is a 'constant' or 'notconstant' value.
  LatticeValueTy Tag;
  Constant *Val;
  ConstantRange Range;

public:
  LVILatticeVal() : Tag(undefined), Val(nullptr), Range(1, true) {}

  static LVILatticeVal get(Constant *C) {
    LVILatticeVal Res;
    if (!isa<UndefValue>(C))
      Res.markConstant(C);
    return Res;
  }
  static LVILatticeVal getNot(Constant *C) {
    LVILatticeVal Res;
    if (!isa<UndefValue>(C))
      Res.markNotConstant(C);
    return Res;
  }
  static LVILatticeVal getRange(ConstantRange CR) {
    LVILatticeVal Res;
    Res.markConstantRange(std::move(CR));
    return Res;
  }
  static LVILatticeVal getOverdefined() {
    LVILatticeVal Res;
    Res.markOverdefined();
    return Res;
  }
  
  bool isUndefined() const     { return Tag == undefined; }
  bool isConstant() const      { return Tag == constant; }
  bool isNotConstant() const   { return Tag == notconstant; }
  bool isConstantRange() const { return Tag == constantrange; }
  bool isOverdefined() const   { return Tag == overdefined; }

  Constant *getConstant() const {
    assert(isConstant() && "Cannot get the constant of a non-constant!");
    return Val;
  }

  Constant *getNotConstant() const {
    assert(isNotConstant() && "Cannot get the constant of a non-notconstant!");
    return Val;
  }

  ConstantRange getConstantRange() const {
    assert(isConstantRange() &&
           "Cannot get the constant-range of a non-constant-range!");
    return Range;
  }

  /// Return true if this is a change in status.
  bool markOverdefined() {
    if (isOverdefined())
      return false;
    Tag = overdefined;
    return true;
  }

  /// Return true if this is a change in status.
  bool markConstant(Constant *V) {
    assert(V && "Marking constant with NULL");
    if (ConstantInt *CI = dyn_cast<ConstantInt>(V))
      return markConstantRange(ConstantRange(CI->getValue()));
    if (isa<UndefValue>(V))
      return false;

    assert((!isConstant() || getConstant() == V) &&
           "Marking constant with different value");
    assert(isUndefined());
    Tag = constant;
    Val = V;
    return true;
  }

  /// Return true if this is a change in status.
  bool markNotConstant(Constant *V) {
    assert(V && "Marking constant with NULL");
    if (ConstantInt *CI = dyn_cast<ConstantInt>(V))
      return markConstantRange(ConstantRange(CI->getValue()+1, CI->getValue()));
    if (isa<UndefValue>(V))
      return false;

    assert((!isConstant() || getConstant() != V) &&
           "Marking constant !constant with same value");
    assert((!isNotConstant() || getNotConstant() == V) &&
           "Marking !constant with different value");
    assert(isUndefined() || isConstant());
    Tag = notconstant;
    Val = V;
    return true;
  }

  /// Return true if this is a change in status.
  bool markConstantRange(ConstantRange NewR) {
    if (isConstantRange()) {
      if (NewR.isEmptySet())
        return markOverdefined();

      bool changed = Range != NewR;
      Range = std::move(NewR);
      return changed;
    }

    assert(isUndefined());
    if (NewR.isEmptySet())
      return markOverdefined();

    Tag = constantrange;
    Range = std::move(NewR);
    return true;
  }

  /// Merge the specified lattice value into this one, updating this
  /// one and returning true if anything changed.
  bool mergeIn(const LVILatticeVal &RHS, const DataLayout &DL) {
    if (RHS.isUndefined() || isOverdefined()) return false;
    if (RHS.isOverdefined()) return markOverdefined();

    if (isUndefined()) {
      Tag = RHS.Tag;
      Val = RHS.Val;
      Range = RHS.Range;
      return true;
    }

    if (isConstant()) {
      if (RHS.isConstant()) {
        if (Val == RHS.Val)
          return false;
        return markOverdefined();
      }

      if (RHS.isNotConstant()) {
        if (Val == RHS.Val)
          return markOverdefined();

        // Unless we can prove that the two Constants are different, we must
        // move to overdefined.
        if (ConstantInt *Res =
                dyn_cast<ConstantInt>(ConstantFoldCompareInstOperands(
                    CmpInst::ICMP_NE, getConstant(), RHS.getNotConstant(), DL)))
          if (Res->isOne())
            return markNotConstant(RHS.getNotConstant());

        return markOverdefined();
      }

      return markOverdefined();
    }

    if (isNotConstant()) {
      if (RHS.isConstant()) {
        if (Val == RHS.Val)
          return markOverdefined();

        // Unless we can prove that the two Constants are different, we must
        // move to overdefined.
        if (ConstantInt *Res =
                dyn_cast<ConstantInt>(ConstantFoldCompareInstOperands(
                    CmpInst::ICMP_NE, getNotConstant(), RHS.getConstant(), DL)))
          if (Res->isOne())
            return false;

        return markOverdefined();
      }

      if (RHS.isNotConstant()) {
        if (Val == RHS.Val)
          return false;
        return markOverdefined();
      }

      return markOverdefined();
    }

    assert(isConstantRange() && "New LVILattice type?");
    if (!RHS.isConstantRange())
      return markOverdefined();

    ConstantRange NewR = Range.unionWith(RHS.getConstantRange());
    if (NewR.isFullSet())
      return markOverdefined();
    return markConstantRange(NewR);
  }
};

} // end anonymous namespace.

namespace llvm {
raw_ostream &operator<<(raw_ostream &OS, const LVILatticeVal &Val)
    LLVM_ATTRIBUTE_USED;
raw_ostream &operator<<(raw_ostream &OS, const LVILatticeVal &Val) {
  if (Val.isUndefined())
    return OS << "undefined";
  if (Val.isOverdefined())
    return OS << "overdefined";

  if (Val.isNotConstant())
    return OS << "notconstant<" << *Val.getNotConstant() << '>';
  else if (Val.isConstantRange())
    return OS << "constantrange<" << Val.getConstantRange().getLower() << ", "
              << Val.getConstantRange().getUpper() << '>';
  return OS << "constant<" << *Val.getConstant() << '>';
}
}

/// Returns true if this lattice value represents at most one possible value.
/// This is as precise as any lattice value can get while still representing
/// reachable code.
static bool hasSingleValue(LVILatticeVal Val) {
  if (Val.isConstantRange() &&
      Val.getConstantRange().isSingleElement())
    // Integer constants are single element ranges
    return true;
  if (Val.isConstant())
    // Non integer constants
    return true;
  return false;
}

/// Combine two sets of facts about the same value into a single set of
/// facts.  Note that this method is not suitable for merging facts along
/// different paths in a CFG; that's what the mergeIn function is for.  This
/// is for merging facts gathered about the same value at the same location
/// through two independent means.
/// Notes:
/// * This method does not promise to return the most precise possible lattice
///   value implied by A and B.  It is allowed to return any lattice element
///   which is at least as strong as *either* A or B (unless our facts
///   conflict, see below).  
/// * Due to unreachable code, the intersection of two lattice values could be
///   contradictory.  If this happens, we return some valid lattice value so as
///   not confuse the rest of LVI.  Ideally, we'd always return Undefined, but
///   we do not make this guarantee.  TODO: This would be a useful enhancement.
static LVILatticeVal intersect(LVILatticeVal A, LVILatticeVal B) {
  // Undefined is the strongest state.  It means the value is known to be along
  // an unreachable path.
  if (A.isUndefined())
    return A;
  if (B.isUndefined())
    return B;

  // If we gave up for one, but got a useable fact from the other, use it.
  if (A.isOverdefined())
    return B;
  if (B.isOverdefined())
    return A;

  // Can't get any more precise than constants.
  if (hasSingleValue(A))
    return A;
  if (hasSingleValue(B))
    return B;

  // Could be either constant range or not constant here.
  if (!A.isConstantRange() || !B.isConstantRange()) {
    // TODO: Arbitrary choice, could be improved
    return A;
  }

  // Intersect two constant ranges
  ConstantRange Range =
    A.getConstantRange().intersectWith(B.getConstantRange());
  // Note: An empty range is implicitly converted to overdefined internally.
  // TODO: We could instead use Undefined here since we've proven a conflict
  // and thus know this path must be unreachable. 
  return LVILatticeVal::getRange(std::move(Range));
}

//===----------------------------------------------------------------------===//
//                          LazyValueInfoCache Decl
//===----------------------------------------------------------------------===//

namespace {
  /// A callback value handle updates the cache when values are erased.
  class LazyValueInfoCache;
  struct LVIValueHandle final : public CallbackVH {
    LazyValueInfoCache *Parent;

    LVIValueHandle(Value *V, LazyValueInfoCache *P)
      : CallbackVH(V), Parent(P) { }

    void deleted() override;
    void allUsesReplacedWith(Value *V) override {
      deleted();
    }
  };
}

namespace {
  /// This is the cache kept by LazyValueInfo which
  /// maintains information about queries across the clients' queries.
  class LazyValueInfoCache {
    /// This is all of the cached block information for exactly one Value*.
    /// The entries are sorted by the BasicBlock* of the
    /// entries, allowing us to do a lookup with a binary search.
    /// Over-defined lattice values are recorded in OverDefinedCache to reduce
    /// memory overhead.
    typedef SmallDenseMap<AssertingVH<BasicBlock>, LVILatticeVal, 4>
        ValueCacheEntryTy;

    /// This is all of the cached information for all values,
    /// mapped from Value* to key information.
    std::map<LVIValueHandle, ValueCacheEntryTy> ValueCache;

    /// This tracks, on a per-block basis, the set of values that are
    /// over-defined at the end of that block.
    typedef DenseMap<AssertingVH<BasicBlock>, SmallPtrSet<Value *, 4>>
        OverDefinedCacheTy;
    OverDefinedCacheTy OverDefinedCache;

    /// Keep track of all blocks that we have ever seen, so we
    /// don't spend time removing unused blocks from our caches.
    DenseSet<AssertingVH<BasicBlock> > SeenBlocks;

    /// This stack holds the state of the value solver during a query.
    /// It basically emulates the callstack of the naive
    /// recursive value lookup process.
    std::stack<std::pair<BasicBlock*, Value*> > BlockValueStack;

    /// Keeps track of which block-value pairs are in BlockValueStack.
    DenseSet<std::pair<BasicBlock*, Value*> > BlockValueSet;

    /// Push BV onto BlockValueStack unless it's already in there.
    /// Returns true on success.
    bool pushBlockValue(const std::pair<BasicBlock *, Value *> &BV) {
      if (!BlockValueSet.insert(BV).second)
        return false;  // It's already in the stack.

      DEBUG(dbgs() << "PUSH: " << *BV.second << " in " << BV.first->getName()
                   << "\n");
      BlockValueStack.push(BV);
      return true;
    }

    AssumptionCache *AC;  ///< A pointer to the cache of @llvm.assume calls.
    const DataLayout &DL; ///< A mandatory DataLayout
    DominatorTree *DT;    ///< An optional DT pointer.

    friend struct LVIValueHandle;

    void insertResult(Value *Val, BasicBlock *BB, const LVILatticeVal &Result) {
      SeenBlocks.insert(BB);

      // Insert over-defined values into their own cache to reduce memory
      // overhead.
      if (Result.isOverdefined())
        OverDefinedCache[BB].insert(Val);
      else
        lookup(Val)[BB] = Result;
    }

    LVILatticeVal getBlockValue(Value *Val, BasicBlock *BB);
    bool getEdgeValue(Value *V, BasicBlock *F, BasicBlock *T,
                      LVILatticeVal &Result,
                      Instruction *CxtI = nullptr);
    bool hasBlockValue(Value *Val, BasicBlock *BB);

    // These methods process one work item and may add more. A false value
    // returned means that the work item was not completely processed and must
    // be revisited after going through the new items.
    bool solveBlockValue(Value *Val, BasicBlock *BB);
    bool solveBlockValueNonLocal(LVILatticeVal &BBLV,
                                 Value *Val, BasicBlock *BB);
    bool solveBlockValuePHINode(LVILatticeVal &BBLV,
                                PHINode *PN, BasicBlock *BB);
    bool solveBlockValueSelect(LVILatticeVal &BBLV,
                               SelectInst *S, BasicBlock *BB); 
    bool solveBlockValueBinaryOp(LVILatticeVal &BBLV,
                              Instruction *BBI, BasicBlock *BB);
    bool solveBlockValueCast(LVILatticeVal &BBLV,
                             Instruction *BBI, BasicBlock *BB);
   void intersectAssumeBlockValueConstantRange(Value *Val, LVILatticeVal &BBLV,
                                            Instruction *BBI);

    void solve();

    ValueCacheEntryTy &lookup(Value *V) {
      return ValueCache[LVIValueHandle(V, this)];
    }

    bool isOverdefined(Value *V, BasicBlock *BB) const {
      auto ODI = OverDefinedCache.find(BB);

      if (ODI == OverDefinedCache.end())
        return false;

      return ODI->second.count(V);
    }

    bool hasCachedValueInfo(Value *V, BasicBlock *BB) {
      if (isOverdefined(V, BB))
        return true;

      LVIValueHandle ValHandle(V, this);
      auto I = ValueCache.find(ValHandle);
      if (I == ValueCache.end())
        return false;

      return I->second.count(BB);
    }

    LVILatticeVal getCachedValueInfo(Value *V, BasicBlock *BB) {
      if (isOverdefined(V, BB))
        return LVILatticeVal::getOverdefined();

      return lookup(V)[BB];
    }
    
  public:
    /// This is the query interface to determine the lattice
    /// value for the specified Value* at the end of the specified block.
    LVILatticeVal getValueInBlock(Value *V, BasicBlock *BB,
                                  Instruction *CxtI = nullptr);

    /// This is the query interface to determine the lattice
    /// value for the specified Value* at the specified instruction (generally
    /// from an assume intrinsic).
    LVILatticeVal getValueAt(Value *V, Instruction *CxtI);

    /// This is the query interface to determine the lattice
    /// value for the specified Value* that is true on the specified edge.
    LVILatticeVal getValueOnEdge(Value *V, BasicBlock *FromBB,BasicBlock *ToBB,
                                 Instruction *CxtI = nullptr);

    /// This is the update interface to inform the cache that an edge from
    /// PredBB to OldSucc has been threaded to be from PredBB to NewSucc.
    void threadEdge(BasicBlock *PredBB,BasicBlock *OldSucc,BasicBlock *NewSucc);

    /// This is part of the update interface to inform the cache
    /// that a block has been deleted.
    void eraseBlock(BasicBlock *BB);

    /// clear - Empty the cache.
    void clear() {
      SeenBlocks.clear();
      ValueCache.clear();
      OverDefinedCache.clear();
    }

    LazyValueInfoCache(AssumptionCache *AC, const DataLayout &DL,
                       DominatorTree *DT = nullptr)
        : AC(AC), DL(DL), DT(DT) {}
  };
} // end anonymous namespace

void LVIValueHandle::deleted() {
  SmallVector<AssertingVH<BasicBlock>, 4> ToErase;
  for (auto &I : Parent->OverDefinedCache) {
    SmallPtrSetImpl<Value *> &ValueSet = I.second;
    if (ValueSet.count(getValPtr()))
      ValueSet.erase(getValPtr());
    if (ValueSet.empty())
      ToErase.push_back(I.first);
  }
  for (auto &BB : ToErase)
    Parent->OverDefinedCache.erase(BB);

  // This erasure deallocates *this, so it MUST happen after we're done
  // using any and all members of *this.
  Parent->ValueCache.erase(*this);
}

void LazyValueInfoCache::eraseBlock(BasicBlock *BB) {
  // Shortcut if we have never seen this block.
  DenseSet<AssertingVH<BasicBlock> >::iterator I = SeenBlocks.find(BB);
  if (I == SeenBlocks.end())
    return;
  SeenBlocks.erase(I);

  auto ODI = OverDefinedCache.find(BB);
  if (ODI != OverDefinedCache.end())
    OverDefinedCache.erase(ODI);

  for (auto I = ValueCache.begin(), E = ValueCache.end(); I != E; ++I)
    I->second.erase(BB);
}

void LazyValueInfoCache::solve() {
  while (!BlockValueStack.empty()) {
    std::pair<BasicBlock*, Value*> &e = BlockValueStack.top();
    assert(BlockValueSet.count(e) && "Stack value should be in BlockValueSet!");

    if (solveBlockValue(e.second, e.first)) {
      // The work item was completely processed.
      assert(BlockValueStack.top() == e && "Nothing should have been pushed!");
      assert(hasCachedValueInfo(e.second, e.first) &&
             "Result should be in cache!");

      DEBUG(dbgs() << "POP " << *e.second << " in " << e.first->getName()
                   << " = " << getCachedValueInfo(e.second, e.first) << "\n");

      BlockValueStack.pop();
      BlockValueSet.erase(e);
    } else {
      // More work needs to be done before revisiting.
      assert(BlockValueStack.top() != e && "Stack should have been pushed!");
    }
  }
}

bool LazyValueInfoCache::hasBlockValue(Value *Val, BasicBlock *BB) {
  // If already a constant, there is nothing to compute.
  if (isa<Constant>(Val))
    return true;

  return hasCachedValueInfo(Val, BB);
}

LVILatticeVal LazyValueInfoCache::getBlockValue(Value *Val, BasicBlock *BB) {
  // If already a constant, there is nothing to compute.
  if (Constant *VC = dyn_cast<Constant>(Val))
    return LVILatticeVal::get(VC);

  SeenBlocks.insert(BB);
  return getCachedValueInfo(Val, BB);
}

static LVILatticeVal getFromRangeMetadata(Instruction *BBI) {
  switch (BBI->getOpcode()) {
  default: break;
  case Instruction::Load:
  case Instruction::Call:
  case Instruction::Invoke:
    if (MDNode *Ranges = BBI->getMetadata(LLVMContext::MD_range)) 
      if (isa<IntegerType>(BBI->getType())) {
        return LVILatticeVal::getRange(getConstantRangeFromMetadata(*Ranges));
      }
    break;
  };
  // Nothing known - will be intersected with other facts
  return LVILatticeVal::getOverdefined();
}

bool LazyValueInfoCache::solveBlockValue(Value *Val, BasicBlock *BB) {
  if (isa<Constant>(Val))
    return true;

  if (hasCachedValueInfo(Val, BB)) {
    // If we have a cached value, use that.
    DEBUG(dbgs() << "  reuse BB '" << BB->getName()
                 << "' val=" << getCachedValueInfo(Val, BB) << '\n');

    // Since we're reusing a cached value, we don't need to update the
    // OverDefinedCache. The cache will have been properly updated whenever the
    // cached value was inserted.
    return true;
  }

  // Hold off inserting this value into the Cache in case we have to return
  // false and come back later.
  LVILatticeVal Res;

  Instruction *BBI = dyn_cast<Instruction>(Val);
  if (!BBI || BBI->getParent() != BB) {
    if (!solveBlockValueNonLocal(Res, Val, BB))
      return false;
   insertResult(Val, BB, Res);
   return true;
  }

  if (PHINode *PN = dyn_cast<PHINode>(BBI)) {
    if (!solveBlockValuePHINode(Res, PN, BB))
      return false;
    insertResult(Val, BB, Res);
    return true;
  }

  if (auto *SI = dyn_cast<SelectInst>(BBI)) {
    if (!solveBlockValueSelect(Res, SI, BB))
      return false;
    insertResult(Val, BB, Res);
    return true;
  }

  // If this value is a nonnull pointer, record it's range and bailout.  Note
  // that for all other pointer typed values, we terminate the search at the
  // definition.  We could easily extend this to look through geps, bitcasts,
  // and the like to prove non-nullness, but it's not clear that's worth it
  // compile time wise.  The context-insensative value walk done inside
  // isKnownNonNull gets most of the profitable cases at much less expense.
  // This does mean that we have a sensativity to where the defining
  // instruction is placed, even if it could legally be hoisted much higher.
  // That is unfortunate.
  PointerType *PT = dyn_cast<PointerType>(BBI->getType());
  if (PT && isKnownNonNull(BBI)) {
    Res = LVILatticeVal::getNot(ConstantPointerNull::get(PT));
    insertResult(Val, BB, Res);
    return true;
  }
  else if (BBI->getType()->isIntegerTy()) {
    if (isa<CastInst>(BBI)) {
      if (!solveBlockValueCast(Res, BBI, BB))
        return false;
      insertResult(Val, BB, Res);
      return true;
    }
    BinaryOperator *BO = dyn_cast<BinaryOperator>(BBI);
    if (BO && isa<ConstantInt>(BO->getOperand(1))) { 
      if (!solveBlockValueBinaryOp(Res, BBI, BB))
        return false;
      insertResult(Val, BB, Res);
      return true;
    }
  }

  DEBUG(dbgs() << " compute BB '" << BB->getName()
                 << "' - unknown inst def found.\n");
  Res = getFromRangeMetadata(BBI);
  insertResult(Val, BB, Res);
  return true;
}

static bool InstructionDereferencesPointer(Instruction *I, Value *Ptr) {
  if (LoadInst *L = dyn_cast<LoadInst>(I)) {
    return L->getPointerAddressSpace() == 0 &&
           GetUnderlyingObject(L->getPointerOperand(),
                               L->getModule()->getDataLayout()) == Ptr;
  }
  if (StoreInst *S = dyn_cast<StoreInst>(I)) {
    return S->getPointerAddressSpace() == 0 &&
           GetUnderlyingObject(S->getPointerOperand(),
                               S->getModule()->getDataLayout()) == Ptr;
  }
  if (MemIntrinsic *MI = dyn_cast<MemIntrinsic>(I)) {
    if (MI->isVolatile()) return false;

    // FIXME: check whether it has a valuerange that excludes zero?
    ConstantInt *Len = dyn_cast<ConstantInt>(MI->getLength());
    if (!Len || Len->isZero()) return false;

    if (MI->getDestAddressSpace() == 0)
      if (GetUnderlyingObject(MI->getRawDest(),
                              MI->getModule()->getDataLayout()) == Ptr)
        return true;
    if (MemTransferInst *MTI = dyn_cast<MemTransferInst>(MI))
      if (MTI->getSourceAddressSpace() == 0)
        if (GetUnderlyingObject(MTI->getRawSource(),
                                MTI->getModule()->getDataLayout()) == Ptr)
          return true;
  }
  return false;
}

/// Return true if the allocation associated with Val is ever dereferenced
/// within the given basic block.  This establishes the fact Val is not null,
/// but does not imply that the memory at Val is dereferenceable.  (Val may
/// point off the end of the dereferenceable part of the object.)
static bool isObjectDereferencedInBlock(Value *Val, BasicBlock *BB) {
  assert(Val->getType()->isPointerTy());

  const DataLayout &DL = BB->getModule()->getDataLayout();
  Value *UnderlyingVal = GetUnderlyingObject(Val, DL);
  // If 'GetUnderlyingObject' didn't converge, skip it. It won't converge
  // inside InstructionDereferencesPointer either.
  if (UnderlyingVal == GetUnderlyingObject(UnderlyingVal, DL, 1))
    for (Instruction &I : *BB)
      if (InstructionDereferencesPointer(&I, UnderlyingVal))
        return true;
  return false;
}

bool LazyValueInfoCache::solveBlockValueNonLocal(LVILatticeVal &BBLV,
                                                 Value *Val, BasicBlock *BB) {
  LVILatticeVal Result;  // Start Undefined.

  // If this is the entry block, we must be asking about an argument.  The
  // value is overdefined.
  if (BB == &BB->getParent()->getEntryBlock()) {
    assert(isa<Argument>(Val) && "Unknown live-in to the entry block");
    // Bofore giving up, see if we can prove the pointer non-null local to
    // this particular block.
    if (Val->getType()->isPointerTy() &&
        (isKnownNonNull(Val) || isObjectDereferencedInBlock(Val, BB))) {
      PointerType *PTy = cast<PointerType>(Val->getType());
      Result = LVILatticeVal::getNot(ConstantPointerNull::get(PTy));
    } else {
      Result.markOverdefined();
    }
    BBLV = Result;
    return true;
  }

  // Loop over all of our predecessors, merging what we know from them into
  // result.
  bool EdgesMissing = false;
  for (pred_iterator PI = pred_begin(BB), E = pred_end(BB); PI != E; ++PI) {
    LVILatticeVal EdgeResult;
    EdgesMissing |= !getEdgeValue(Val, *PI, BB, EdgeResult);
    if (EdgesMissing)
      continue;

    Result.mergeIn(EdgeResult, DL);

    // If we hit overdefined, exit early.  The BlockVals entry is already set
    // to overdefined.
    if (Result.isOverdefined()) {
      DEBUG(dbgs() << " compute BB '" << BB->getName()
            << "' - overdefined because of pred (non local).\n");
      // Bofore giving up, see if we can prove the pointer non-null local to
      // this particular block.
      if (Val->getType()->isPointerTy() &&
          isObjectDereferencedInBlock(Val, BB)) {
        PointerType *PTy = cast<PointerType>(Val->getType());
        Result = LVILatticeVal::getNot(ConstantPointerNull::get(PTy));
      }

      BBLV = Result;
      return true;
    }
  }
  if (EdgesMissing)
    return false;

  // Return the merged value, which is more precise than 'overdefined'.
  assert(!Result.isOverdefined());
  BBLV = Result;
  return true;
}

bool LazyValueInfoCache::solveBlockValuePHINode(LVILatticeVal &BBLV,
                                                PHINode *PN, BasicBlock *BB) {
  LVILatticeVal Result;  // Start Undefined.

  // Loop over all of our predecessors, merging what we know from them into
  // result.
  bool EdgesMissing = false;
  for (unsigned i = 0, e = PN->getNumIncomingValues(); i != e; ++i) {
    BasicBlock *PhiBB = PN->getIncomingBlock(i);
    Value *PhiVal = PN->getIncomingValue(i);
    LVILatticeVal EdgeResult;
    // Note that we can provide PN as the context value to getEdgeValue, even
    // though the results will be cached, because PN is the value being used as
    // the cache key in the caller.
    EdgesMissing |= !getEdgeValue(PhiVal, PhiBB, BB, EdgeResult, PN);
    if (EdgesMissing)
      continue;

    Result.mergeIn(EdgeResult, DL);

    // If we hit overdefined, exit early.  The BlockVals entry is already set
    // to overdefined.
    if (Result.isOverdefined()) {
      DEBUG(dbgs() << " compute BB '" << BB->getName()
            << "' - overdefined because of pred (local).\n");

      BBLV = Result;
      return true;
    }
  }
  if (EdgesMissing)
    return false;

  // Return the merged value, which is more precise than 'overdefined'.
  assert(!Result.isOverdefined() && "Possible PHI in entry block?");
  BBLV = Result;
  return true;
}

static bool getValueFromFromCondition(Value *Val, ICmpInst *ICI,
                                      LVILatticeVal &Result,
                                      bool isTrueDest = true);

// If we can determine a constraint on the value given conditions assumed by
// the program, intersect those constraints with BBLV
void LazyValueInfoCache::intersectAssumeBlockValueConstantRange(Value *Val,
                                                            LVILatticeVal &BBLV,
                                                            Instruction *BBI) {
  BBI = BBI ? BBI : dyn_cast<Instruction>(Val);
  if (!BBI)
    return;

  for (auto &AssumeVH : AC->assumptions()) {
    if (!AssumeVH)
      continue;
    auto *I = cast<CallInst>(AssumeVH);
    if (!isValidAssumeForContext(I, BBI, DT))
      continue;

    Value *C = I->getArgOperand(0);
    if (ICmpInst *ICI = dyn_cast<ICmpInst>(C)) {
      LVILatticeVal Result;
      if (getValueFromFromCondition(Val, ICI, Result))
        BBLV = intersect(BBLV, Result);
    }
  }
}

bool LazyValueInfoCache::solveBlockValueSelect(LVILatticeVal &BBLV,
                                               SelectInst *SI, BasicBlock *BB) {

  // Recurse on our inputs if needed
  if (!hasBlockValue(SI->getTrueValue(), BB)) {
    if (pushBlockValue(std::make_pair(BB, SI->getTrueValue())))
      return false;
    BBLV.markOverdefined();
    return true;
  }
  LVILatticeVal TrueVal = getBlockValue(SI->getTrueValue(), BB);
  // If we hit overdefined, don't ask more queries.  We want to avoid poisoning
  // extra slots in the table if we can.
  if (TrueVal.isOverdefined()) {
    BBLV.markOverdefined();
    return true;
  }

  if (!hasBlockValue(SI->getFalseValue(), BB)) {
    if (pushBlockValue(std::make_pair(BB, SI->getFalseValue())))
      return false;
    BBLV.markOverdefined();
    return true;
  }
  LVILatticeVal FalseVal = getBlockValue(SI->getFalseValue(), BB);
  // If we hit overdefined, don't ask more queries.  We want to avoid poisoning
  // extra slots in the table if we can.
  if (FalseVal.isOverdefined()) {
    BBLV.markOverdefined();
    return true;
  }

  if (TrueVal.isConstantRange() && FalseVal.isConstantRange()) {
    ConstantRange TrueCR = TrueVal.getConstantRange();
    ConstantRange FalseCR = FalseVal.getConstantRange();
    Value *LHS = nullptr;
    Value *RHS = nullptr;
    SelectPatternResult SPR = matchSelectPattern(SI, LHS, RHS);
    // Is this a min specifically of our two inputs?  (Avoid the risk of
    // ValueTracking getting smarter looking back past our immediate inputs.)
    if (SelectPatternResult::isMinOrMax(SPR.Flavor) &&
        LHS == SI->getTrueValue() && RHS == SI->getFalseValue()) {
      switch (SPR.Flavor) {
      default:
        llvm_unreachable("unexpected minmax type!");
      case SPF_SMIN:                   /// Signed minimum
        BBLV.markConstantRange(TrueCR.smin(FalseCR));
        return true;
      case SPF_UMIN:                   /// Unsigned minimum
        BBLV.markConstantRange(TrueCR.umin(FalseCR));
        return true;
      case SPF_SMAX:                   /// Signed maximum
        BBLV.markConstantRange(TrueCR.smax(FalseCR));
        return true;
      case SPF_UMAX:                   /// Unsigned maximum        
        BBLV.markConstantRange(TrueCR.umax(FalseCR));
        return true;
      };
    }
    
    // TODO: ABS, NABS from the SelectPatternResult
  }

  // Can we constrain the facts about the true and false values by using the
  // condition itself?  This shows up with idioms like e.g. select(a > 5, a, 5).
  // TODO: We could potentially refine an overdefined true value above.
  if (auto *ICI = dyn_cast<ICmpInst>(SI->getCondition())) {
    LVILatticeVal TrueValTaken, FalseValTaken;
    if (!getValueFromFromCondition(SI->getTrueValue(), ICI,
                                   TrueValTaken, true))
      TrueValTaken.markOverdefined();
    if (!getValueFromFromCondition(SI->getFalseValue(), ICI,
                                   FalseValTaken, false))
      FalseValTaken.markOverdefined();

    TrueVal = intersect(TrueVal, TrueValTaken);
    FalseVal = intersect(FalseVal, FalseValTaken);


    // Handle clamp idioms such as:
    //   %24 = constantrange<0, 17>
    //   %39 = icmp eq i32 %24, 0
    //   %40 = add i32 %24, -1
    //   %siv.next = select i1 %39, i32 16, i32 %40
    //   %siv.next = constantrange<0, 17> not <-1, 17>
    // In general, this can handle any clamp idiom which tests the edge
    // condition via an equality or inequality.  
    ICmpInst::Predicate Pred = ICI->getPredicate();
    Value *A = ICI->getOperand(0);
    if (ConstantInt *CIBase = dyn_cast<ConstantInt>(ICI->getOperand(1))) {
      auto addConstants = [](ConstantInt *A, ConstantInt *B) {
        assert(A->getType() == B->getType());
        return ConstantInt::get(A->getType(), A->getValue() + B->getValue());
      };
      // See if either input is A + C2, subject to the constraint from the
      // condition that A != C when that input is used.  We can assume that
      // that input doesn't include C + C2.
      ConstantInt *CIAdded;
      switch (Pred) {
      default: break;
      case ICmpInst::ICMP_EQ:
        if (match(SI->getFalseValue(), m_Add(m_Specific(A),
                                             m_ConstantInt(CIAdded)))) {
          auto ResNot = addConstants(CIBase, CIAdded);
          FalseVal = intersect(FalseVal,
                               LVILatticeVal::getNot(ResNot));
        }
        break;
      case ICmpInst::ICMP_NE:
        if (match(SI->getTrueValue(), m_Add(m_Specific(A),
                                            m_ConstantInt(CIAdded)))) {
          auto ResNot = addConstants(CIBase, CIAdded);
          TrueVal = intersect(TrueVal,
                              LVILatticeVal::getNot(ResNot));
        }
        break;
      };
    }
  }
  
  LVILatticeVal Result;  // Start Undefined.
  Result.mergeIn(TrueVal, DL);
  Result.mergeIn(FalseVal, DL);
  BBLV = Result;
  return true;
}

bool LazyValueInfoCache::solveBlockValueCast(LVILatticeVal &BBLV,
                                             Instruction *BBI,
                                             BasicBlock *BB) {
  if (!BBI->getOperand(0)->getType()->isSized()) {
    // Without knowing how wide the input is, we can't analyze it in any useful
    // way.
    BBLV.markOverdefined();
    return true;
  }

  // Filter out casts we don't know how to reason about before attempting to
  // recurse on our operand.  This can cut a long search short if we know we're
  // not going to be able to get any useful information anways.
  switch (BBI->getOpcode()) {
  case Instruction::Trunc:
  case Instruction::SExt:
  case Instruction::ZExt:
  case Instruction::BitCast:
    break;
  default:
    // Unhandled instructions are overdefined.
    DEBUG(dbgs() << " compute BB '" << BB->getName()
                 << "' - overdefined (unknown cast).\n");
    BBLV.markOverdefined();
    return true;
  }

  
  // Figure out the range of the LHS.  If that fails, we still apply the
  // transfer rule on the full set since we may be able to locally infer
  // interesting facts.
  if (!hasBlockValue(BBI->getOperand(0), BB))
    if (pushBlockValue(std::make_pair(BB, BBI->getOperand(0))))
      // More work to do before applying this transfer rule.
      return false;

  const unsigned OperandBitWidth =
    DL.getTypeSizeInBits(BBI->getOperand(0)->getType());
  ConstantRange LHSRange = ConstantRange(OperandBitWidth);
  if (hasBlockValue(BBI->getOperand(0), BB)) {
    LVILatticeVal LHSVal = getBlockValue(BBI->getOperand(0), BB);
    intersectAssumeBlockValueConstantRange(BBI->getOperand(0), LHSVal, BBI);
    if (LHSVal.isConstantRange())
      LHSRange = LHSVal.getConstantRange();
  }

  const unsigned ResultBitWidth =
    cast<IntegerType>(BBI->getType())->getBitWidth();

  // NOTE: We're currently limited by the set of operations that ConstantRange
  // can evaluate symbolically.  Enhancing that set will allows us to analyze
  // more definitions.
  LVILatticeVal Result;
  switch (BBI->getOpcode()) {
  case Instruction::Trunc:
    Result.markConstantRange(LHSRange.truncate(ResultBitWidth));
    break;
  case Instruction::SExt:
    Result.markConstantRange(LHSRange.signExtend(ResultBitWidth));
    break;
  case Instruction::ZExt:
    Result.markConstantRange(LHSRange.zeroExtend(ResultBitWidth));
    break;
  case Instruction::BitCast:
    Result.markConstantRange(LHSRange);
    break;
  default:
    // Should be dead if the code above is correct
    llvm_unreachable("inconsistent with above");
    break;
  }

  BBLV = Result;
  return true;
}

bool LazyValueInfoCache::solveBlockValueBinaryOp(LVILatticeVal &BBLV,
                                                 Instruction *BBI,
                                                 BasicBlock *BB) {

  assert(BBI->getOperand(0)->getType()->isSized() &&
         "all operands to binary operators are sized");

  // Filter out operators we don't know how to reason about before attempting to
  // recurse on our operand(s).  This can cut a long search short if we know
  // we're not going to be able to get any useful information anways.
  switch (BBI->getOpcode()) {
  case Instruction::Add:
  case Instruction::Sub:
  case Instruction::Mul:
  case Instruction::UDiv:
  case Instruction::Shl:
  case Instruction::LShr:
  case Instruction::And:
  case Instruction::Or:
    // continue into the code below
    break;
  default:
    // Unhandled instructions are overdefined.
    DEBUG(dbgs() << " compute BB '" << BB->getName()
                 << "' - overdefined (unknown binary operator).\n");
    BBLV.markOverdefined();
    return true;
  };
  
  // Figure out the range of the LHS.  If that fails, use a conservative range,
  // but apply the transfer rule anyways.  This lets us pick up facts from
  // expressions like "and i32 (call i32 @foo()), 32"
  if (!hasBlockValue(BBI->getOperand(0), BB))
    if (pushBlockValue(std::make_pair(BB, BBI->getOperand(0))))
      // More work to do before applying this transfer rule.
      return false;

  const unsigned OperandBitWidth =
    DL.getTypeSizeInBits(BBI->getOperand(0)->getType());
  ConstantRange LHSRange = ConstantRange(OperandBitWidth);
  if (hasBlockValue(BBI->getOperand(0), BB)) {
    LVILatticeVal LHSVal = getBlockValue(BBI->getOperand(0), BB);
    intersectAssumeBlockValueConstantRange(BBI->getOperand(0), LHSVal, BBI);
    if (LHSVal.isConstantRange())
      LHSRange = LHSVal.getConstantRange();
  }

  ConstantInt *RHS = cast<ConstantInt>(BBI->getOperand(1));
  ConstantRange RHSRange = ConstantRange(RHS->getValue());

  // NOTE: We're currently limited by the set of operations that ConstantRange
  // can evaluate symbolically.  Enhancing that set will allows us to analyze
  // more definitions.
  LVILatticeVal Result;
  switch (BBI->getOpcode()) {
  case Instruction::Add:
    Result.markConstantRange(LHSRange.add(RHSRange));
    break;
  case Instruction::Sub:
    Result.markConstantRange(LHSRange.sub(RHSRange));
    break;
  case Instruction::Mul:
    Result.markConstantRange(LHSRange.multiply(RHSRange));
    break;
  case Instruction::UDiv:
    Result.markConstantRange(LHSRange.udiv(RHSRange));
    break;
  case Instruction::Shl:
    Result.markConstantRange(LHSRange.shl(RHSRange));
    break;
  case Instruction::LShr:
    Result.markConstantRange(LHSRange.lshr(RHSRange));
    break;
  case Instruction::And:
    Result.markConstantRange(LHSRange.binaryAnd(RHSRange));
    break;
  case Instruction::Or:
    Result.markConstantRange(LHSRange.binaryOr(RHSRange));
    break;
  default:
    // Should be dead if the code above is correct
    llvm_unreachable("inconsistent with above");
    break;
  }

  BBLV = Result;
  return true;
}

bool getValueFromFromCondition(Value *Val, ICmpInst *ICI,
                               LVILatticeVal &Result, bool isTrueDest) {
  assert(ICI && "precondition");
  if (isa<Constant>(ICI->getOperand(1))) {
    if (ICI->isEquality() && ICI->getOperand(0) == Val) {
      // We know that V has the RHS constant if this is a true SETEQ or
      // false SETNE. 
      if (isTrueDest == (ICI->getPredicate() == ICmpInst::ICMP_EQ))
        Result = LVILatticeVal::get(cast<Constant>(ICI->getOperand(1)));
      else
        Result = LVILatticeVal::getNot(cast<Constant>(ICI->getOperand(1)));
      return true;
    }

    // Recognize the range checking idiom that InstCombine produces.
    // (X-C1) u< C2 --> [C1, C1+C2)
    ConstantInt *NegOffset = nullptr;
    if (ICI->getPredicate() == ICmpInst::ICMP_ULT)
      match(ICI->getOperand(0), m_Add(m_Specific(Val),
                                      m_ConstantInt(NegOffset)));

    ConstantInt *CI = dyn_cast<ConstantInt>(ICI->getOperand(1));
    if (CI && (ICI->getOperand(0) == Val || NegOffset)) {
      // Calculate the range of values that are allowed by the comparison
      ConstantRange CmpRange(CI->getValue());
      ConstantRange TrueValues =
          ConstantRange::makeAllowedICmpRegion(ICI->getPredicate(), CmpRange);

      if (NegOffset) // Apply the offset from above.
        TrueValues = TrueValues.subtract(NegOffset->getValue());

      // If we're interested in the false dest, invert the condition.
      if (!isTrueDest) TrueValues = TrueValues.inverse();

      Result = LVILatticeVal::getRange(std::move(TrueValues));
      return true;
    }
  }
  
  return false;
}

/// \brief Compute the value of Val on the edge BBFrom -> BBTo. Returns false if
/// Val is not constrained on the edge.  Result is unspecified if return value
/// is false.
static bool getEdgeValueLocal(Value *Val, BasicBlock *BBFrom,
                              BasicBlock *BBTo, LVILatticeVal &Result) {
  // TODO: Handle more complex conditionals. If (v == 0 || v2 < 1) is false, we
  // know that v != 0.
  if (BranchInst *BI = dyn_cast<BranchInst>(BBFrom->getTerminator())) {
    // If this is a conditional branch and only one successor goes to BBTo, then
    // we may be able to infer something from the condition.
    if (BI->isConditional() &&
        BI->getSuccessor(0) != BI->getSuccessor(1)) {
      bool isTrueDest = BI->getSuccessor(0) == BBTo;
      assert(BI->getSuccessor(!isTrueDest) == BBTo &&
             "BBTo isn't a successor of BBFrom");

      // If V is the condition of the branch itself, then we know exactly what
      // it is.
      if (BI->getCondition() == Val) {
        Result = LVILatticeVal::get(ConstantInt::get(
                              Type::getInt1Ty(Val->getContext()), isTrueDest));
        return true;
      }

      // If the condition of the branch is an equality comparison, we may be
      // able to infer the value.
      if (ICmpInst *ICI = dyn_cast<ICmpInst>(BI->getCondition()))
        if (getValueFromFromCondition(Val, ICI, Result, isTrueDest))
          return true;
    }
  }

  // If the edge was formed by a switch on the value, then we may know exactly
  // what it is.
  if (SwitchInst *SI = dyn_cast<SwitchInst>(BBFrom->getTerminator())) {
    if (SI->getCondition() != Val)
      return false;

    bool DefaultCase = SI->getDefaultDest() == BBTo;
    unsigned BitWidth = Val->getType()->getIntegerBitWidth();
    ConstantRange EdgesVals(BitWidth, DefaultCase/*isFullSet*/);

    for (SwitchInst::CaseIt i : SI->cases()) {
      ConstantRange EdgeVal(i.getCaseValue()->getValue());
      if (DefaultCase) {
        // It is possible that the default destination is the destination of
        // some cases. There is no need to perform difference for those cases.
        if (i.getCaseSuccessor() != BBTo)
          EdgesVals = EdgesVals.difference(EdgeVal);
      } else if (i.getCaseSuccessor() == BBTo)
        EdgesVals = EdgesVals.unionWith(EdgeVal);
    }
    Result = LVILatticeVal::getRange(std::move(EdgesVals));
    return true;
  }
  return false;
}

/// \brief Compute the value of Val on the edge BBFrom -> BBTo or the value at
/// the basic block if the edge does not constrain Val.
bool LazyValueInfoCache::getEdgeValue(Value *Val, BasicBlock *BBFrom,
                                      BasicBlock *BBTo, LVILatticeVal &Result,
                                      Instruction *CxtI) {
  // If already a constant, there is nothing to compute.
  if (Constant *VC = dyn_cast<Constant>(Val)) {
    Result = LVILatticeVal::get(VC);
    return true;
  }

  LVILatticeVal LocalResult;
  if (!getEdgeValueLocal(Val, BBFrom, BBTo, LocalResult))
    // If we couldn't constrain the value on the edge, LocalResult doesn't
    // provide any information.
    LocalResult.markOverdefined();
  
  if (hasSingleValue(LocalResult)) {
    // Can't get any more precise here
    Result = LocalResult;
    return true;
  }

  if (!hasBlockValue(Val, BBFrom)) {
    if (pushBlockValue(std::make_pair(BBFrom, Val)))
      return false;
    // No new information.
    Result = LocalResult;
    return true;
  }

  // Try to intersect ranges of the BB and the constraint on the edge.
  LVILatticeVal InBlock = getBlockValue(Val, BBFrom);
  intersectAssumeBlockValueConstantRange(Val, InBlock, BBFrom->getTerminator());
  // We can use the context instruction (generically the ultimate instruction
  // the calling pass is trying to simplify) here, even though the result of
  // this function is generally cached when called from the solve* functions
  // (and that cached result might be used with queries using a different
  // context instruction), because when this function is called from the solve*
  // functions, the context instruction is not provided. When called from
  // LazyValueInfoCache::getValueOnEdge, the context instruction is provided,
  // but then the result is not cached.
  intersectAssumeBlockValueConstantRange(Val, InBlock, CxtI);

  Result = intersect(LocalResult, InBlock);
  return true;
}

LVILatticeVal LazyValueInfoCache::getValueInBlock(Value *V, BasicBlock *BB,
                                                  Instruction *CxtI) {
  DEBUG(dbgs() << "LVI Getting block end value " << *V << " at '"
        << BB->getName() << "'\n");

  assert(BlockValueStack.empty() && BlockValueSet.empty());
  if (!hasBlockValue(V, BB)) {
    pushBlockValue(std::make_pair(BB, V)); 
    solve();
  }
  LVILatticeVal Result = getBlockValue(V, BB);
  intersectAssumeBlockValueConstantRange(V, Result, CxtI);

  DEBUG(dbgs() << "  Result = " << Result << "\n");
  return Result;
}

LVILatticeVal LazyValueInfoCache::getValueAt(Value *V, Instruction *CxtI) {
  DEBUG(dbgs() << "LVI Getting value " << *V << " at '"
        << CxtI->getName() << "'\n");

  if (auto *C = dyn_cast<Constant>(V))
    return LVILatticeVal::get(C);

  LVILatticeVal Result = LVILatticeVal::getOverdefined();
  if (auto *I = dyn_cast<Instruction>(V))
    Result = getFromRangeMetadata(I);
  intersectAssumeBlockValueConstantRange(V, Result, CxtI);

  DEBUG(dbgs() << "  Result = " << Result << "\n");
  return Result;
}

LVILatticeVal LazyValueInfoCache::
getValueOnEdge(Value *V, BasicBlock *FromBB, BasicBlock *ToBB,
               Instruction *CxtI) {
  DEBUG(dbgs() << "LVI Getting edge value " << *V << " from '"
        << FromBB->getName() << "' to '" << ToBB->getName() << "'\n");

  LVILatticeVal Result;
  if (!getEdgeValue(V, FromBB, ToBB, Result, CxtI)) {
    solve();
    bool WasFastQuery = getEdgeValue(V, FromBB, ToBB, Result, CxtI);
    (void)WasFastQuery;
    assert(WasFastQuery && "More work to do after problem solved?");
  }

  DEBUG(dbgs() << "  Result = " << Result << "\n");
  return Result;
}

void LazyValueInfoCache::threadEdge(BasicBlock *PredBB, BasicBlock *OldSucc,
                                    BasicBlock *NewSucc) {
  // When an edge in the graph has been threaded, values that we could not
  // determine a value for before (i.e. were marked overdefined) may be
  // possible to solve now. We do NOT try to proactively update these values.
  // Instead, we clear their entries from the cache, and allow lazy updating to
  // recompute them when needed.

  // The updating process is fairly simple: we need to drop cached info
  // for all values that were marked overdefined in OldSucc, and for those same
  // values in any successor of OldSucc (except NewSucc) in which they were
  // also marked overdefined.
  std::vector<BasicBlock*> worklist;
  worklist.push_back(OldSucc);

  auto I = OverDefinedCache.find(OldSucc);
  if (I == OverDefinedCache.end())
    return; // Nothing to process here.
  SmallVector<Value *, 4> ValsToClear(I->second.begin(), I->second.end());

  // Use a worklist to perform a depth-first search of OldSucc's successors.
  // NOTE: We do not need a visited list since any blocks we have already
  // visited will have had their overdefined markers cleared already, and we
  // thus won't loop to their successors.
  while (!worklist.empty()) {
    BasicBlock *ToUpdate = worklist.back();
    worklist.pop_back();

    // Skip blocks only accessible through NewSucc.
    if (ToUpdate == NewSucc) continue;

    bool changed = false;
    for (Value *V : ValsToClear) {
      // If a value was marked overdefined in OldSucc, and is here too...
      auto OI = OverDefinedCache.find(ToUpdate);
      if (OI == OverDefinedCache.end())
        continue;
      SmallPtrSetImpl<Value *> &ValueSet = OI->second;
      if (!ValueSet.count(V))
        continue;

      ValueSet.erase(V);
      if (ValueSet.empty())
        OverDefinedCache.erase(OI);

      // If we removed anything, then we potentially need to update
      // blocks successors too.
      changed = true;
    }

    if (!changed) continue;

    worklist.insert(worklist.end(), succ_begin(ToUpdate), succ_end(ToUpdate));
  }
}

//===----------------------------------------------------------------------===//
//                            LazyValueInfo Impl
//===----------------------------------------------------------------------===//

/// This lazily constructs the LazyValueInfoCache.
static LazyValueInfoCache &getCache(void *&PImpl, AssumptionCache *AC,
                                    const DataLayout *DL,
                                    DominatorTree *DT = nullptr) {
  if (!PImpl) {
    assert(DL && "getCache() called with a null DataLayout");
    PImpl = new LazyValueInfoCache(AC, *DL, DT);
  }
  return *static_cast<LazyValueInfoCache*>(PImpl);
}

bool LazyValueInfo::runOnFunction(Function &F) {
  AC = &getAnalysis<AssumptionCacheTracker>().getAssumptionCache(F);
  const DataLayout &DL = F.getParent()->getDataLayout();

  DominatorTreeWrapperPass *DTWP =
      getAnalysisIfAvailable<DominatorTreeWrapperPass>();
  DT = DTWP ? &DTWP->getDomTree() : nullptr;

  TLI = &getAnalysis<TargetLibraryInfoWrapperPass>().getTLI();

  if (PImpl)
    getCache(PImpl, AC, &DL, DT).clear();

  // Fully lazy.
  return false;
}

void LazyValueInfo::getAnalysisUsage(AnalysisUsage &AU) const {
  AU.setPreservesAll();
  AU.addRequired<AssumptionCacheTracker>();
  AU.addRequired<TargetLibraryInfoWrapperPass>();
}

void LazyValueInfo::releaseMemory() {
  // If the cache was allocated, free it.
  if (PImpl) {
    delete &getCache(PImpl, AC, nullptr);
    PImpl = nullptr;
  }
}

Constant *LazyValueInfo::getConstant(Value *V, BasicBlock *BB,
                                     Instruction *CxtI) {
  const DataLayout &DL = BB->getModule()->getDataLayout();
  LVILatticeVal Result =
      getCache(PImpl, AC, &DL, DT).getValueInBlock(V, BB, CxtI);

  if (Result.isConstant())
    return Result.getConstant();
  if (Result.isConstantRange()) {
    ConstantRange CR = Result.getConstantRange();
    if (const APInt *SingleVal = CR.getSingleElement())
      return ConstantInt::get(V->getContext(), *SingleVal);
  }
  return nullptr;
}

ConstantRange LazyValueInfo::getConstantRange(Value *V, BasicBlock *BB,
					      Instruction *CxtI) {
  assert(V->getType()->isIntegerTy());
  unsigned Width = V->getType()->getIntegerBitWidth();
  const DataLayout &DL = BB->getModule()->getDataLayout();
  LVILatticeVal Result =
      getCache(PImpl, AC, &DL, DT).getValueInBlock(V, BB, CxtI);
  assert(!Result.isConstant());
  if (Result.isUndefined())
    return ConstantRange(Width, /*isFullSet=*/false);
  if (Result.isConstantRange())
    return Result.getConstantRange();
  else
    return ConstantRange(Width, /*isFullSet=*/true);
}

/// Determine whether the specified value is known to be a
/// constant on the specified edge. Return null if not.
Constant *LazyValueInfo::getConstantOnEdge(Value *V, BasicBlock *FromBB,
                                           BasicBlock *ToBB,
                                           Instruction *CxtI) {
  const DataLayout &DL = FromBB->getModule()->getDataLayout();
  LVILatticeVal Result =
      getCache(PImpl, AC, &DL, DT).getValueOnEdge(V, FromBB, ToBB, CxtI);

  if (Result.isConstant())
    return Result.getConstant();
  if (Result.isConstantRange()) {
    ConstantRange CR = Result.getConstantRange();
    if (const APInt *SingleVal = CR.getSingleElement())
      return ConstantInt::get(V->getContext(), *SingleVal);
  }
  return nullptr;
}

static LazyValueInfo::Tristate getPredicateResult(unsigned Pred, Constant *C,
                                                  LVILatticeVal &Result,
                                                  const DataLayout &DL,
                                                  TargetLibraryInfo *TLI) {

  // If we know the value is a constant, evaluate the conditional.
  Constant *Res = nullptr;
  if (Result.isConstant()) {
    Res = ConstantFoldCompareInstOperands(Pred, Result.getConstant(), C, DL,
                                          TLI);
    if (ConstantInt *ResCI = dyn_cast<ConstantInt>(Res))
      return ResCI->isZero() ? LazyValueInfo::False : LazyValueInfo::True;
    return LazyValueInfo::Unknown;
  }

  if (Result.isConstantRange()) {
    ConstantInt *CI = dyn_cast<ConstantInt>(C);
    if (!CI) return LazyValueInfo::Unknown;

    ConstantRange CR = Result.getConstantRange();
    if (Pred == ICmpInst::ICMP_EQ) {
      if (!CR.contains(CI->getValue()))
        return LazyValueInfo::False;

      if (CR.isSingleElement() && CR.contains(CI->getValue()))
        return LazyValueInfo::True;
    } else if (Pred == ICmpInst::ICMP_NE) {
      if (!CR.contains(CI->getValue()))
        return LazyValueInfo::True;

      if (CR.isSingleElement() && CR.contains(CI->getValue()))
        return LazyValueInfo::False;
    }

    // Handle more complex predicates.
    ConstantRange TrueValues =
        ICmpInst::makeConstantRange((ICmpInst::Predicate)Pred, CI->getValue());
    if (TrueValues.contains(CR))
      return LazyValueInfo::True;
    if (TrueValues.inverse().contains(CR))
      return LazyValueInfo::False;
    return LazyValueInfo::Unknown;
  }

  if (Result.isNotConstant()) {
    // If this is an equality comparison, we can try to fold it knowing that
    // "V != C1".
    if (Pred == ICmpInst::ICMP_EQ) {
      // !C1 == C -> false iff C1 == C.
      Res = ConstantFoldCompareInstOperands(ICmpInst::ICMP_NE,
                                            Result.getNotConstant(), C, DL,
                                            TLI);
      if (Res->isNullValue())
        return LazyValueInfo::False;
    } else if (Pred == ICmpInst::ICMP_NE) {
      // !C1 != C -> true iff C1 == C.
      Res = ConstantFoldCompareInstOperands(ICmpInst::ICMP_NE,
                                            Result.getNotConstant(), C, DL,
                                            TLI);
      if (Res->isNullValue())
        return LazyValueInfo::True;
    }
    return LazyValueInfo::Unknown;
  }

  return LazyValueInfo::Unknown;
}

/// Determine whether the specified value comparison with a constant is known to
/// be true or false on the specified CFG edge. Pred is a CmpInst predicate.
LazyValueInfo::Tristate
LazyValueInfo::getPredicateOnEdge(unsigned Pred, Value *V, Constant *C,
                                  BasicBlock *FromBB, BasicBlock *ToBB,
                                  Instruction *CxtI) {
  const DataLayout &DL = FromBB->getModule()->getDataLayout();
  LVILatticeVal Result =
      getCache(PImpl, AC, &DL, DT).getValueOnEdge(V, FromBB, ToBB, CxtI);

  return getPredicateResult(Pred, C, Result, DL, TLI);
}

LazyValueInfo::Tristate
LazyValueInfo::getPredicateAt(unsigned Pred, Value *V, Constant *C,
                              Instruction *CxtI) {
  const DataLayout &DL = CxtI->getModule()->getDataLayout();
  LVILatticeVal Result = getCache(PImpl, AC, &DL, DT).getValueAt(V, CxtI);
  Tristate Ret = getPredicateResult(Pred, C, Result, DL, TLI);
  if (Ret != Unknown)
    return Ret;

  // Note: The following bit of code is somewhat distinct from the rest of LVI;
  // LVI as a whole tries to compute a lattice value which is conservatively
  // correct at a given location.  In this case, we have a predicate which we
  // weren't able to prove about the merged result, and we're pushing that
  // predicate back along each incoming edge to see if we can prove it
  // separately for each input.  As a motivating example, consider:
  // bb1:
  //   %v1 = ... ; constantrange<1, 5>
  //   br label %merge
  // bb2:
  //   %v2 = ... ; constantrange<10, 20>
  //   br label %merge
  // merge:
  //   %phi = phi [%v1, %v2] ; constantrange<1,20>
  //   %pred = icmp eq i32 %phi, 8
  // We can't tell from the lattice value for '%phi' that '%pred' is false
  // along each path, but by checking the predicate over each input separately,
  // we can.
  // We limit the search to one step backwards from the current BB and value.
  // We could consider extending this to search further backwards through the
  // CFG and/or value graph, but there are non-obvious compile time vs quality
  // tradeoffs.  
  if (CxtI) {
    BasicBlock *BB = CxtI->getParent();

    // Function entry or an unreachable block.  Bail to avoid confusing
    // analysis below.
    pred_iterator PI = pred_begin(BB), PE = pred_end(BB);
    if (PI == PE)
      return Unknown;

    // If V is a PHI node in the same block as the context, we need to ask
    // questions about the predicate as applied to the incoming value along
    // each edge. This is useful for eliminating cases where the predicate is
    // known along all incoming edges.
    if (auto *PHI = dyn_cast<PHINode>(V))
      if (PHI->getParent() == BB) {
        Tristate Baseline = Unknown;
        for (unsigned i = 0, e = PHI->getNumIncomingValues(); i < e; i++) {
          Value *Incoming = PHI->getIncomingValue(i);
          BasicBlock *PredBB = PHI->getIncomingBlock(i);
          // Note that PredBB may be BB itself.        
          Tristate Result = getPredicateOnEdge(Pred, Incoming, C, PredBB, BB,
                                               CxtI);
          
          // Keep going as long as we've seen a consistent known result for
          // all inputs.
          Baseline = (i == 0) ? Result /* First iteration */
            : (Baseline == Result ? Baseline : Unknown); /* All others */
          if (Baseline == Unknown)
            break;
        }
        if (Baseline != Unknown)
          return Baseline;
      }    

    // For a comparison where the V is outside this block, it's possible
    // that we've branched on it before. Look to see if the value is known
    // on all incoming edges.
    if (!isa<Instruction>(V) ||
        cast<Instruction>(V)->getParent() != BB) {
      // For predecessor edge, determine if the comparison is true or false
      // on that edge. If they're all true or all false, we can conclude
      // the value of the comparison in this block.
      Tristate Baseline = getPredicateOnEdge(Pred, V, C, *PI, BB, CxtI);
      if (Baseline != Unknown) {
        // Check that all remaining incoming values match the first one.
        while (++PI != PE) {
          Tristate Ret = getPredicateOnEdge(Pred, V, C, *PI, BB, CxtI);
          if (Ret != Baseline) break;
        }
        // If we terminated early, then one of the values didn't match.
        if (PI == PE) {
          return Baseline;
        }
      }
    }
  }
  return Unknown;
}

void LazyValueInfo::threadEdge(BasicBlock *PredBB, BasicBlock *OldSucc,
                               BasicBlock *NewSucc) {
  if (PImpl) {
    const DataLayout &DL = PredBB->getModule()->getDataLayout();
    getCache(PImpl, AC, &DL, DT).threadEdge(PredBB, OldSucc, NewSucc);
  }
}

void LazyValueInfo::eraseBlock(BasicBlock *BB) {
  if (PImpl) {
    const DataLayout &DL = BB->getModule()->getDataLayout();
    getCache(PImpl, AC, &DL, DT).eraseBlock(BB);
  }
}
