//===-- RuntimeDyldMachOX86_64.h ---- MachO/X86_64 specific code. -*- C++ -*-=//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_EXECUTIONENGINE_RUNTIMEDYLD_TARGETS_RUNTIMEDYLDMACHOX86_64_H
#define LLVM_LIB_EXECUTIONENGINE_RUNTIMEDYLD_TARGETS_RUNTIMEDYLDMACHOX86_64_H

#include "../RuntimeDyldMachO.h"
#include <string>

#define DEBUG_TYPE "dyld"

namespace llvm {

class RuntimeDyldMachOX86_64
    : public RuntimeDyldMachOCRTPBase<RuntimeDyldMachOX86_64> {
public:

  typedef uint64_t TargetPtrT;

  RuntimeDyldMachOX86_64(RuntimeDyld::MemoryManager &MM,
                         RuntimeDyld::SymbolResolver &Resolver)
      : RuntimeDyldMachOCRTPBase(MM, Resolver) {}

  unsigned getMaxStubSize() override { return 8; }

  unsigned getStubAlignment() override { return 1; }

  Expected<relocation_iterator>
  processRelocationRef(unsigned SectionID, relocation_iterator RelI,
                       const ObjectFile &BaseObjT,
                       ObjSectionToIDMap &ObjSectionToID,
                       StubMap &Stubs) override {
    const MachOObjectFile &Obj =
      static_cast<const MachOObjectFile &>(BaseObjT);
    MachO::any_relocation_info RelInfo =
        Obj.getRelocation(RelI->getRawDataRefImpl());
    uint32_t RelType = Obj.getAnyRelocationType(RelInfo);

    if (RelType == MachO::X86_64_RELOC_SUBTRACTOR)
      return processSubtractRelocation(SectionID, RelI, Obj, ObjSectionToID);

    assert(!Obj.isRelocationScattered(RelInfo) &&
           "Scattered relocations not supported on X86_64");

    RelocationEntry RE(getRelocationEntry(SectionID, Obj, RelI));
    RE.Addend = memcpyAddend(RE);
    RelocationValueRef Value;
    if (auto ValueOrErr = getRelocationValueRef(Obj, RelI, RE, ObjSectionToID))
      Value = *ValueOrErr;
    else
      return ValueOrErr.takeError();

    bool IsExtern = Obj.getPlainRelocationExternal(RelInfo);
    if (!IsExtern && RE.IsPCRel)
      makeValueAddendPCRel(Value, RelI, 1 << RE.Size);

    switch (RelType) {
    UNIMPLEMENTED_RELOC(MachO::X86_64_RELOC_TLV);
    default:
      if (RelType > MachO::X86_64_RELOC_TLV)
        return make_error<RuntimeDyldError>(("MachO X86_64 relocation type " +
                                             Twine(RelType) +
                                             " is out of range").str());
      break;
    }

    if (RE.RelType == MachO::X86_64_RELOC_GOT ||
        RE.RelType == MachO::X86_64_RELOC_GOT_LOAD)
      processGOTRelocation(RE, Value, Stubs);
    else {
      RE.Addend = Value.Offset;
      if (Value.SymbolName)
        addRelocationForSymbol(RE, Value.SymbolName);
      else
        addRelocationForSection(RE, Value.SectionID);
    }

    return ++RelI;
  }

  void resolveRelocation(const RelocationEntry &RE, uint64_t Value) override {
    DEBUG(dumpRelocationToResolve(RE, Value));
    const SectionEntry &Section = Sections[RE.SectionID];
    uint8_t *LocalAddress = Section.getAddressWithOffset(RE.Offset);

    // If the relocation is PC-relative, the value to be encoded is the
    // pointer difference.
    if (RE.IsPCRel) {
      // FIXME: It seems this value needs to be adjusted by 4 for an effective
      // PC address. Is that expected? Only for branches, perhaps?
      uint64_t FinalAddress = Section.getLoadAddressWithOffset(RE.Offset);
      Value -= FinalAddress + 4;
    }

    switch (RE.RelType) {
    default:
      llvm_unreachable("Invalid relocation type!");
    case MachO::X86_64_RELOC_SIGNED_1:
    case MachO::X86_64_RELOC_SIGNED_2:
    case MachO::X86_64_RELOC_SIGNED_4:
    case MachO::X86_64_RELOC_SIGNED:
    case MachO::X86_64_RELOC_UNSIGNED:
    case MachO::X86_64_RELOC_BRANCH:
      writeBytesUnaligned(Value + RE.Addend, LocalAddress, 1 << RE.Size);
      break;
    case MachO::X86_64_RELOC_SUBTRACTOR: {
      uint64_t SectionABase = Sections[RE.Sections.SectionA].getLoadAddress();
      uint64_t SectionBBase = Sections[RE.Sections.SectionB].getLoadAddress();
      assert((Value == SectionABase || Value == SectionBBase) &&
             "Unexpected SUBTRACTOR relocation value.");
      Value = SectionABase - SectionBBase + RE.Addend;
      writeBytesUnaligned(Value, LocalAddress, 1 << RE.Size);
      break;
    }
    }
  }

  Error finalizeSection(const ObjectFile &Obj, unsigned SectionID,
                        const SectionRef &Section) {
    return Error::success();
  }

private:
  void processGOTRelocation(const RelocationEntry &RE,
                            RelocationValueRef &Value, StubMap &Stubs) {
    SectionEntry &Section = Sections[RE.SectionID];
    assert(RE.IsPCRel);
    assert(RE.Size == 2);
    Value.Offset -= RE.Addend;
    RuntimeDyldMachO::StubMap::const_iterator i = Stubs.find(Value);
    uint8_t *Addr;
    if (i != Stubs.end()) {
      Addr = Section.getAddressWithOffset(i->second);
    } else {
      Stubs[Value] = Section.getStubOffset();
      uint8_t *GOTEntry = Section.getAddressWithOffset(Section.getStubOffset());
      RelocationEntry GOTRE(RE.SectionID, Section.getStubOffset(),
                            MachO::X86_64_RELOC_UNSIGNED, Value.Offset, false,
                            3);
      if (Value.SymbolName)
        addRelocationForSymbol(GOTRE, Value.SymbolName);
      else
        addRelocationForSection(GOTRE, Value.SectionID);
      Section.advanceStubOffset(8);
      Addr = GOTEntry;
    }
    RelocationEntry TargetRE(RE.SectionID, RE.Offset,
                             MachO::X86_64_RELOC_UNSIGNED, RE.Addend, true, 2);
    resolveRelocation(TargetRE, (uint64_t)Addr);
  }

  relocation_iterator
  processSubtractRelocation(unsigned SectionID, relocation_iterator RelI,
                            const ObjectFile &BaseObjT,
                            ObjSectionToIDMap &ObjSectionToID) {
    const MachOObjectFile &Obj =
        static_cast<const MachOObjectFile&>(BaseObjT);
    MachO::any_relocation_info RE =
        Obj.getRelocation(RelI->getRawDataRefImpl());

    unsigned Size = Obj.getAnyRelocationLength(RE);
    uint64_t Offset = RelI->getOffset();
    uint8_t *LocalAddress = Sections[SectionID].getAddressWithOffset(Offset);
    unsigned NumBytes = 1 << Size;

    Expected<StringRef> SubtrahendNameOrErr = RelI->getSymbol()->getName();
    if (!SubtrahendNameOrErr) {
      std::string Buf;
      raw_string_ostream OS(Buf);
      logAllUnhandledErrors(SubtrahendNameOrErr.takeError(), OS, "");
      OS.flush();
      report_fatal_error(Buf);
    }
    auto SubtrahendI = GlobalSymbolTable.find(*SubtrahendNameOrErr);
    unsigned SectionBID = SubtrahendI->second.getSectionID();
    uint64_t SectionBOffset = SubtrahendI->second.getOffset();
    int64_t Addend =
      SignExtend64(readBytesUnaligned(LocalAddress, NumBytes), NumBytes * 8);

    ++RelI;
    Expected<StringRef> MinuendNameOrErr = RelI->getSymbol()->getName();
    if (!MinuendNameOrErr) {
      std::string Buf;
      raw_string_ostream OS(Buf);
      logAllUnhandledErrors(MinuendNameOrErr.takeError(), OS, "");
      OS.flush();
      report_fatal_error(Buf);
    }
    auto MinuendI = GlobalSymbolTable.find(*MinuendNameOrErr);
    unsigned SectionAID = MinuendI->second.getSectionID();
    uint64_t SectionAOffset = MinuendI->second.getOffset();

    RelocationEntry R(SectionID, Offset, MachO::X86_64_RELOC_SUBTRACTOR, (uint64_t)Addend,
                      SectionAID, SectionAOffset, SectionBID, SectionBOffset,
                      false, Size);

    addRelocationForSection(R, SectionAID);

    return ++RelI;
  }

};
}

#undef DEBUG_TYPE

#endif