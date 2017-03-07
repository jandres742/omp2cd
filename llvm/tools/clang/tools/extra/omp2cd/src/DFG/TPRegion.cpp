#include "include/DFG/TPRegion.h"
#include "include/DFG/DFGNode.h"
#include "include/DFG/LoopInfo.h"
#include "include/DFG/FunctionInfo.h"
#include "include/DFG/LoopCondNode.h"
#include "include/DFG/BranchInfo.h"
#include "include/DFG/TaskInfo.h"

using namespace llvm;
using namespace clang;
using namespace clang::tooling;
using namespace std;

namespace omp2cd_space {

TPRegion::TPRegion():parent(nullptr),
	prevRegion(nullptr),
	nextRegion(nullptr),
	prev(nullptr),
	next(nullptr),
	mainNode(nullptr),
	id(-1),
	name(""),
	shouldBeDeleted(false),
	singleThreaded(false),
	doNotPrint(false),
	isThreadedProcedure(false),
	isCodelet(false)
{
	initLocalThreadsLoopStr = "";
	initGlobalThreadsLoopStr = "";
	freeLocalThreadsLoopStr = "";
	freeGlobalThreadsLoopStr = "";
}

TPRegion::TPRegion(DFGNode * _newNode):parent(nullptr),
	prevRegion(nullptr),
	nextRegion(nullptr),
	prev(nullptr),
	next(nullptr),
	mainNode(nullptr),
	id(-1), name(""),
	shouldBeDeleted(false),
	singleThreaded(false),
	doNotPrint(false),
	isThreadedProcedure(false),
	isCodelet(false)
{
	this->addNode(_newNode);

	initLocalThreadsLoopStr = "";
	initGlobalThreadsLoopStr = "";
	freeLocalThreadsLoopStr = "";
	freeGlobalThreadsLoopStr = "";
}

TPRegion::TPRegion(DFGNode * _newNode, TPRegion * _parent):
	parent(_parent),
	prevRegion(nullptr),
	nextRegion(nullptr),
	prev(nullptr),
	next(nullptr),
	mainNode(nullptr),
	id(-1), name(""),
	shouldBeDeleted(false),
	singleThreaded(false),
	doNotPrint(false),
	isThreadedProcedure(false),
	isCodelet(false)
{
	if (this->parent)
		this->parent->addRegion(this);

	this->addNode(_newNode);

	initLocalThreadsLoopStr = "";
	initGlobalThreadsLoopStr = "";
	freeLocalThreadsLoopStr = "";
	freeGlobalThreadsLoopStr = "";
}

TPRegion::~TPRegion()
{
	
}

DFG *TPRegion::getDFG()
{
	return this->mainNode->getDFG();
}

clang::Stmt * TPRegion::getStmt()
{
	return this->mainNode->s;
}

bool TPRegion::hasBarrier()
{
	bool withBarrier = true;
	if (this->mainNode->noWait)
		withBarrier = false;

	return withBarrier;
}

TPRegion *TPRegion::getParentRegion()
{
	return this->parent;
}

TPInputs_t TPRegion::getOMPInputs()
{
	return this->mainNode->ompInputs;
}

TPInputs_t TPRegion::getOMPVariables()
{
	return this->mainNode->ompVariables;
}

int TPRegion::getID()
{
	return this->id;
}

CopyPrivateClause *TPRegion::getCopyPrivateClause()
{
	return this->mainNode->myCopyPrivateClause;
}

CopyinClause *TPRegion::getCopyinClause()
{
	return this->mainNode->myCopyinClause;
}

TaskInfo *TPRegion::getTaskInfo()
{
	return this->mainNode->myTaskInfo;
}

LoopInfo *TPRegion::getLoopInfo()
{
	return this->mainNode->myLoopInfo;
}

BranchInfo *TPRegion::getBranchInfo()
{
	return this->mainNode->myBranchInfo;
}

FunctionInfo *TPRegion::getFunctionInfo()
{
	return this->mainNode->myFunctionInfo;
}

size_t TPRegion::getNumNodes()
{
	return this->myNodes.size();
}

std::string TPRegion::getName()
{
	return this->name;
}

bool TPRegion::hasLoopCondNode()
{
	bool res = false;
	for (DFGNode * node:this->myNodes) {
		if (isa < LoopCondNode > (node)) {
			res = true;
			break;
		}
	}
	return res;
}

bool TPRegion::isOMPParallel()
{
	return this->mainNode->isOMPParallel();
}

bool TPRegion::isOMPFor()
{
	return this->mainNode->isOMPFor();
}

bool TPRegion::isSingleThreaded()
{
	return this->singleThreaded;
}

bool TPRegion::isBalancedOMPFor()
{
	bool res = false;

	if (balanceOMPFor)
		if (check_isa < OMPForDirective > (this->getStmt()))
			if (this->getMainNode()->ompExtensionClause->codelet == false)
				res = true;

	return res;
}

/* Check if an omp for region is the solely region inside an omp parallel region
* in order to use a DARTS TP loop. Should be called from the omp for region*/
bool TPRegion::isOMPForOnlyChildRegion()
{
	if (useTPLoops == false)
		return false;
	bool res = false;

	if (check_isa < OMPForDirective > (this->getStmt()) &&
		this->getLoopInfo() && this->getLoopInfo()->reductionVars.size() == 0
		&& this->parent) {
		int numOfOMPChildRegions = 0;
		TPRegion *parentRegion = this->parent->findOMPOrFunctionRegionParent();
		if (parentRegion
			&& check_isa < OMPParallelDirective > (parentRegion->getStmt())) {
			TPRegion *childRegion = parentRegion->childRegions.front();
			while (childRegion) {
				if (check_isa < OMPExecutableDirective >
					(childRegion->getStmt())) {
					numOfOMPChildRegions++;
				} else if (childRegion->isCaller()) {
					numOfOMPChildRegions++;
				}
				childRegion = childRegion->next;
			}
		}
		if (numOfOMPChildRegions == 1)
			res = true;
	}

	return res;
}

/* Check if an omp parallel region has only one omp for region
* in order to use a DARTS TP loop.*/
bool TPRegion::hasOnlyOneChildOMPForRegion()
{
	if (useTPLoops == false)
		return false;

	bool res = false;

	if (check_isa < OMPParallelForDirective > (this->getStmt())) {
		res = true;
	} else if (check_isa < OMPParallelDirective > (this->getStmt())) {
		int numOfOMPChildRegions = 0;
		bool isChildOMPFor = false;
		TPRegion *childRegion = this->childRegions.front();
		while (childRegion) {
			if (check_isa < OMPForDirective > (childRegion->getStmt()) &&
				childRegion->getLoopInfo() &&
				childRegion->getLoopInfo()->reductionVars.size() == 0) {
				isChildOMPFor = true;
				numOfOMPChildRegions++;
			} else if (check_isa < OMPExecutableDirective >
				   (childRegion->getStmt())) {
				isChildOMPFor = false;
				numOfOMPChildRegions++;
			} else if (childRegion->isCaller()) {
				isChildOMPFor = false;
				numOfOMPChildRegions++;
			}
			childRegion = childRegion->next;
		}
		if (isChildOMPFor && numOfOMPChildRegions == 1)
			res = true;
	}
	return res;
}

bool TPRegion::isCaller()
{
	bool res = false;

	for (DFGNode * node:this->myNodes) {
		if (node->isCallerNode()) {
			res = true;
			break;
		}
	}
	return res;
}

TPRegion *TPRegion::findOMPOrFunctionRegionParent()
{
	TPRegion *parentRegion = this;
	while (parentRegion &&
		   (check_isa < OMPExecutableDirective > (parentRegion->getStmt()) == false ||
			(
			check_isa < OMPTaskwaitDirective > (parentRegion->getStmt()) ||
			check_isa < OMPBarrierDirective > (parentRegion->getStmt()))
			) && parentRegion->getFunctionInfo() == nullptr){
		parentRegion = parentRegion->parent;
	}
	return parentRegion;
}

void TPRegion::addNode(DFGNode * newNode)
{
	if (this->myNodes.size() == 0) {
		this->id = newNode->getID();
		this->name = newNode->getName();
		this->mainNode = newNode;
		newNode->isRegionMainNode = true;
	}
	newNode->myRegion = this;
	newNode->isRegionNode = true;
	
	this->myNodes.push_back(newNode);
}

void TPRegion::addRegion(TPRegion * newRegion)
{
	if (this->childRegions.size() > 0) {
		newRegion->prevRegion = this->childRegions.back();
		this->childRegions.back()->nextRegion = newRegion;
	}

	this->childRegions.push_back(newRegion);

}

bool TPRegion::isLoopRegion()
{
	return this->mainNode->isLoopNode();
}

void TPRegion::getParallelRegions(std::vector < TPRegion * >&ompParallelRegionsVec)
{
	TPRegion *childRegion = this->childRegions.front();
	while (childRegion) {
		if (childRegion->isOMPParallel())
			ompParallelRegionsVec.push_back(childRegion);

		childRegion->getParallelRegions(ompParallelRegionsVec);

		childRegion = childRegion->next;
	}
}

TPRegion *TPRegion::getParent()
{
	return this->parent;
}

DFGNode *TPRegion::getMainNode()
{
	return this->mainNode;
}

std::string TPRegion::signalNext(std::ostringstream & outputStream, std::string prefixStr,
		std::string postfixStr, bool signalFromBarrier, bool signalFromCallExpr)
{
	string nextCodeletName =
		this->mainNode->signalNext(outputStream, prefixStr, postfixStr,
			signalFromBarrier, signalFromCallExpr);
	return nextCodeletName;
}

uint64_t TPRegion::getNumOMPRegions()
{
	uint64_t num = 0;
	TPRegion *childRegion = this->childRegions.front();
	while (childRegion) {
		if (check_isa<OMPExecutableDirective>(childRegion->getStmt()))
			num++;

		childRegion->getNumOMPRegions();

		childRegion = childRegion->next;
	}
	return num;
}

void TPRegion::reduceBarriersInRegions()
{
	/*Traverse backwards since we might merge regions upwards*/
	TPRegion *childRegion = this->childRegions.back();
	while (childRegion) {
		TPRegion *childRegionPrev = childRegion->prev;
		childRegion->reduceBarriersInRegions();
		childRegion = childRegionPrev;
	}
	if (this->isOMPParallel()){
		return;
	}
	/*Check if the current region is a sync point and the previous one has/is already a barrier or
	the parent in case the sync point is the last child.*/
	else if (check_isa < OMPBarrierDirective > (this->getStmt()) ||
		check_isa < OMPTaskwaitDirective > (this->getStmt())){
			
		if(this->prevRegion){
			if (check_isa < OMPBarrierDirective > (this->prevRegion->getStmt()) ||
				check_isa < OMPTaskwaitDirective > (this->prevRegion->getStmt())){

				this->prevRegion->nextRegion = this->nextRegion;
				if (this->nextRegion)
					this->nextRegion->prevRegion = this->prevRegion;
				this->getParent()->childRegions.remove(this);
					
			} else if (check_isa < OMPExecutableDirective > (this->prevRegion->getStmt()) &&
				this->prevRegion->hasBarrier()){
					
				this->prevRegion->nextRegion = this->nextRegion;
				if (this->nextRegion)
					this->nextRegion->prevRegion = this->prevRegion;
				this->getParent()->childRegions.remove(this);
					
			}
		} 
	}
	return;
}

bool TPRegion::isSimpleRegion()
{
	bool res = false;
	if (this->childRegions.size() == 0)
		if(this->isLoopRegion() == false || (this->isLoopRegion() && this->mainNode->isComplexStmt ==false))
			if (check_isa < OMPExecutableDirective > (this->getStmt()) == false)
				if(this->isCaller() == false)
					if(this->getNumNodes() < instrGranularityThr ||
						this->getParent()->getMainNode()->ompExtensionClause->codelet)

						res = true;

	return res;
}

bool TPRegion::isInlinedRegion()
{
	bool res = false;
	if (inlineOMP == true &&
		(check_isa < OMPSingleDirective > (this->getStmt()) ||
		 check_isa < OMPMasterDirective > (this->getStmt()) ||
		(check_isa < OMPForDirective > (this->getStmt()) && (inlineOMPFor || 
		this->getMainNode()->ompExtensionClause->codelet))) &&
		this->childRegions.size() == 1 && this->childRegions.front()->isSimpleRegion()
		) {
		res = true;
	}

	return res;
}

void TPRegion::preProcess()
{
	debugFile << "Preprocessing region " << this->getName() << " " << this->getID() << "\n";

	this->reduceBarriersInRegions();

	this->deleteDeclRegionsWithNoInit();

	this->deleteExtraOMPForChildren();

	if (inlineOMP)
		this->inlineOMPRegions();

	this->transformIfStmts();

	this->transformTaskRegion();

	this->setUpBranchRegions();

	this->findBarrierForCopyprivate();
}

void TPRegion::transformLoops()
{
	TPRegion *childRegion = this->childRegions.back();
	while (childRegion) {
		TPRegion *childRegionPrev = childRegion->prev;
		childRegion->transformLoops();
		childRegion = childRegionPrev;
	}

	if (this->isLoopRegion() && this->mainNode->isComplexStmt) {
		/* append children to the this region's parent */
		TPRegion *childRegion = this->childRegions.front();
		while (childRegion) {
			TPRegion *childRegionNext = childRegion->next;
			childRegion->parent = this->parent;
			this->childRegions.remove(childRegion);
			this->parent->childRegions.push_back(childRegion);
			childRegion = childRegionNext;
		}
	}

}

void TPRegion::inlineOMPRegions()
{
	TPRegion *childRegion = this->childRegions.back();
	while (childRegion) {
		TPRegion *childRegionPrev = childRegion->prev;
		childRegion->inlineOMPRegions();
		childRegion = childRegionPrev;
	}

	if (this->isInlinedRegion() && this->parent) {
		TPRegion *parentRegion = this;
		if (this->parent) {
			parentRegion = this->parent->findOMPOrFunctionRegionParent();
		}
		parentRegion->inlinedRegions.push_back(this);
	}
}

void TPRegion::findBarrierForCopyprivate()
{
	TPRegion *childRegion = this->childRegions.back();
	while (childRegion) {
		TPRegion *childRegionPrev = childRegion->prev;
		childRegion->findBarrierForCopyprivate();
		childRegion = childRegionPrev;
	}

	if (this->getCopyPrivateClause()) {
		DFGNode *barrierNode = this->mainNode->findNextBarrier();
		if (barrierNode) {
			barrierNode->nodesToCopyprivate.push_back(this->mainNode);
		}
	}
}

void TPRegion::transformTaskRegion()
{
	TPRegion *childRegion = this->childRegions.front();
	while (childRegion) {
		TPRegion *childRegionNext = childRegion->next;
		childRegion->transformTaskRegion();
		childRegion = childRegionNext;
	}

	if (check_isa < OMPTaskDirective > (this->getStmt()) && this->parent) {

		/* Find the sync point for this task */
		this->getTaskInfo()->setSyncPoints();

		TPRegion *nextReg = this->nextRegion;
		TPRegion *parentRegion = this->parent;

		while(nextReg == nullptr && parentRegion){
			nextReg = parentRegion->nextRegion;
			parentRegion = parentRegion->parent;
		}

		/*fix nextregions to signal*/
		if (this->getTaskInfo() && this->getTaskInfo()->mySyncPoint) {
			/*Keep the next region as the next region to signal after
			spawning the task, so the nextregion
			and the task can be executed concurrently*/
			if (nextReg &&
				this->getTaskInfo()->mySyncPoint->getID() != nextReg->getID()) {
				this->getTaskInfo()->myTaskRegionNext = nextReg;
			}
		}
	}
}

void TPRegion::deleteExtraOMPForChildren()
{
	TPRegion *childRegion = this->childRegions.front();
	while (childRegion) {
		TPRegion *nextChildRegion = childRegion->next;
		childRegion->deleteExtraOMPForChildren();
		childRegion = nextChildRegion;
	}

	/* delete all the regions inside an omp for directive except the for stmt */
	if (this->parent && (check_isa < OMPParallelForDirective > (this->parent->getStmt())
				 || check_isa < OMPForDirective > (this->parent->getStmt()))) {
		if (check_isa < ForStmt > (this->getStmt()) == false) {
			if (this->prevRegion) {
				this->prevRegion->nextRegion = this->nextRegion;
			}
			if (this->nextRegion) {
				this->nextRegion->prevRegion = this->prevRegion;
			}
			if (this->parent) {
				this->parent->childRegions.remove(this);
				delete(this);
			}
		}
	}
}

void TPRegion::transformIfStmts()
{

	TPRegion *childRegion = this->childRegions.front();
	while (childRegion) {
		TPRegion *nextChildRegion = childRegion->next;
		childRegion->transformIfStmts();
		childRegion = nextChildRegion;
	}

	/* Append if-node's chidren to if-node's parent since 
	the if-stmt is only for control flow.
	An if stmt should have only two child regions max, 
	the if branch and the else branch. */
	if (check_isa < IfStmt > (this->getStmt())) {
		/* append child regions to if parent. */
		debugFile << "region " << this->getID()
			<< " isSimpleIf " << this->getBranchInfo()->isSimpleIf
			<< " childRegions->size() = " << childRegions.size()
			<< "\n";
		
		if (this->getBranchInfo()->isSimpleIf == 0) {
			TPRegion *childRegion = this->childRegions.front();
			
			while (childRegion) {
				TPRegion *nextChildRegion = childRegion->next;
				
				debugFile << "attaching childRegion " << childRegion->getID() << " to "
					<< " this->parent " << this->parent->getID() << "\n";

				childRegion->parent = this->parent;
				this->childRegions.remove(childRegion);
				this->parent->childRegions.push_back(childRegion);

				childRegion = nextChildRegion;
			}
		}
	}
	return;
}

void TPRegion::setUpBranchRegions()
{
	/* set the correct prev and next regions for each region inside the if region */
	if (check_isa < IfStmt > (this->getStmt()) && this->getBranchInfo()
		&& this->getBranchInfo()->isSimpleIf == false) {
		this->getBranchInfo()->setBranchInfoNodes();
	}

	TPRegion *childRegion = this->childRegions.front();
	while (childRegion) {
		childRegion->setUpBranchRegions();
		childRegion = childRegion->next;
	}
}

void TPRegion::deleteDeclRegionsWithNoInit()
{
	TPRegion *childRegion = this->childRegions.front();
	while (childRegion) {
		TPRegion *nextChildRegion = childRegion->next;
		childRegion->deleteDeclRegionsWithNoInit();
		childRegion = nextChildRegion;
	}

	bool mustBeDeleted = false;

	/*delete regions with only decl stmts and no init*/
	if (check_isa < DeclStmt > (this->getStmt())) {
		/*If this decl stmt is part of an omp parallel region with only
		 * one omp for, then the omp for will optimize with a TP Loop
		 * and this decl needs to be printed, even if it does not have
		 * init*/
		bool hasOnlyOMPFor = false;
		TPRegion *parentRegion = this->findOMPOrFunctionRegionParent();
		if (parentRegion && check_isa < OMPParallelDirective > (parentRegion->getStmt())) {
			hasOnlyOMPFor = parentRegion->hasOnlyOneChildOMPForRegion();
		}

		if (hasOnlyOMPFor == false) {
			mustBeDeleted = true;
			for (size_t i = 0; i < this->myNodes.size(); i++) {
				DFGNode *node = this->myNodes[i];
				/* if there's something else than decl stmts in the region, do not delete it */
				if (check_isa < DeclStmt > (node->getStmt()) == false) {
					mustBeDeleted = false;
					break;
				} else {
					DeclStmt *declStmt = dyn_cast < DeclStmt > (node->getStmt());
					/* look for decl stmts with inits */
					for (clang::DeclStmt::decl_iterator it = declStmt->decl_begin(); it < declStmt->decl_end(); it++) {
						VarDecl *varDecl = dyn_cast < VarDecl > (*it);
						if (varDecl && varDecl->hasInit()) {
							mustBeDeleted = false;
							break;
						}
					}
				}
			}
		}
	}

	if (mustBeDeleted) {
		if (this->prevRegion) {
			this->prevRegion->nextRegion = this->nextRegion;
		}
		if (this->nextRegion) {
			this->nextRegion->prevRegion = this->prevRegion;
		}
		if (this->parent) {
			this->parent->childRegions.remove(this);
			delete(this);
		}
	}
}

void TPRegion::makeSingleThreaded()
{
	/* Mark as single-threaded the child this of a this executed by a single thread */
	if (check_isa < OMPSingleDirective > (this->getStmt()) ||
		check_isa < OMPSectionDirective > (this->getStmt()) ||
		check_isa < OMPTaskDirective > (this->getStmt()) ||
		check_isa < OMPMasterDirective > (this->getStmt())
		) {
		this->singleThreaded = true;
		this->makeChildrenSingleThreaded();
	}

/*Deactivating this because it was making any function with an omp task directive as singlethreaded,
which is usually true, but it's still an assumption.*/
#if 0
	if (check_isa < OMPTaskDirective > (this->getStmt())) {
		DFGNode *parentNode = this->mainNode->findParentNode();
		if (parentNode->myFunctionInfo && check_isa < OMPExecutableDirective > (parentNode->getStmt()) == false) {
			if (parentNode->myRegion) {
				parentNode->myRegion->singleThreaded = true;
				FunctionTable_t::iterator it = functionTable.find(parentNode->myRegion->getName());
				if (it != functionTable.end()) {
					get < 4 > (it->second) = true;
				}
				parentNode->myRegion->makeChildrenSingleThreaded();
			}
		}
	}
#endif
	
	/* make main single threaded */
	if (this->getName().compare("main") == 0) {
		this->singleThreaded = true;
		this->makeChildrenSingleThreaded();
	}

	TPRegion *childRegion = this->childRegions.front();
	while (childRegion) {
		childRegion->makeSingleThreaded();
		childRegion = childRegion->next;
	}

	return;
}

void TPRegion::makeChildrenSingleThreaded()
{
	TPRegion *childRegion = this->childRegions.front();
	while (childRegion) {
		if (check_isa < OMPParallelDirective > (childRegion->getStmt()) == false &&
			check_isa < OMPParallelForDirective > (childRegion->getStmt()) ==
			false) {
			childRegion->singleThreaded = true;
			childRegion->makeChildrenSingleThreaded();
		}
		childRegion = childRegion->next;
	}
}

void TPRegion::printRegions()
{
	outs() << "Region: " << this->getName() << ":" << this->getID()
		<< ", childRegions: " << this->childRegions.size() << "\n";

	TPRegion *childRegion = this->childRegions.front();
	while (childRegion) {
		outs() << childRegion->getName() << ":" << childRegion->getID() << " , ";
		childRegion = childRegion->next;
	}
	outs() << "\n";
}

void TPRegion::showRegionInfo() {
	debugFile << "++++++++++++++++++++++++++\n";
	debugFile << "Region " << this->id << " : " << this->name;

	if (this->parent) {
		debugFile << ", parent = "
			<< this->parent->id
			<< " : "
			<< this->parent->name;
	}
	debugFile << "\n";

	if (this->prevRegion) {
		debugFile << "prevRegion = "
			<< this->prevRegion->id
			<< " : "
			<< this->prevRegion->name;
	}
	if (this->nextRegion) {
		debugFile << ", nextRegion = "
			<< this->nextRegion->id
			<< " : "
			<< this->nextRegion->name;
	}
	debugFile << "\n";
	if (this->prev) {
		debugFile << "prev = "
			<< this->prev->id
			<< " : "
			<< this->prev->name;
	}
	if (this->next) {
		debugFile << ", next = "
			<< this->next->id
			<< " : "
			<< this->next->name;
	}
	debugFile << "\n";

	debugFile << "Size() = " << this->childRegions.size() << "\n";

	debugFile << "doNotPrint = " << this->doNotPrint << "\n";

	debugFile << "hasLoopCondNode = " << this->hasLoopCondNode() << "\n";

	debugFile << "isLoopRegion = " << this->isLoopRegion() << "\n";

	debugFile << "isComplexStmt = " << this->mainNode->isComplexStmt << "\n";

	if (this->mainNode->noWait) {
		debugFile << "No Wait\n";
	} else {
		debugFile << "Barrier\n";
		for (DFGNode * child:	this->mainNode->tasksToExecute) {
			debugFile << "Executes task " << child->id << "\n";
		}
	}

	debugFile << "singleThreaded: " << this->singleThreaded << "\n";

	if (this->mainNode->ompInputs.size() > 0) {
		debugFile << "Inputs: ";
		for (auto var:	this->mainNode->ompInputs) {
			debugFile << get < 0 >
				(var.second) << " " << var.first << " " << get < 1 >
				(var.second) << "; ";
		}
		debugFile << "\n";
	}

	if (this->mainNode->ompVariables.size() > 0) {
		debugFile << "Variables: ";
		for (auto var:	this->mainNode->ompVariables) {
			debugFile << get < 0 >
				(var.second) << " " << var.first << " " << get < 1 >
				(var.second) << ";";
		}
		debugFile << "\n";
	}

	if (check_isa < OMPCriticalDirective > (this->mainNode->getStmt())) {
		debugFile << "Critical Directive name: " << this->mainNode->
			criticalDirectiveName << "\n";
	}

	if (check_isa < OMPTaskDirective > (this->mainNode->getStmt())
		&& this->mainNode->myTaskInfo->mySyncPoint) {
		debugFile << "SyncNode: " << this->mainNode->myTaskInfo->mySyncPoint->
			getID() << "\n";
		debugFile << "SyncPointRegion: " << this->mainNode->myTaskInfo->
			mySyncPointRegion->getID() << "\n";
		if (this->mainNode->myTaskInfo->myTaskRegionStart) {
			debugFile << "TaskRegionStart: " << this->mainNode->myTaskInfo->
				myTaskRegionStart->getID() << "\n";
		}
		if (this->mainNode->myTaskInfo->myTaskRegionNext) {
			debugFile << "TaskRegionNext: " << this->mainNode->myTaskInfo->
				myTaskRegionNext->getID() << "\n";
		}
	}

	if (check_isa < IfStmt > (this->getStmt()) && this->getBranchInfo()) {
		debugFile << "node->myBranchInfo->isSimpleIf  = " << this->getBranchInfo()->
			isSimpleIf << "\n";

		debugFile << "Condition: " << this->getBranchInfo()->
			conditionString << "\n";

		if (this->getBranchInfo()->ifBodyRegion) {
			debugFile << "IfBodyRegion: " << this->getBranchInfo()->
				ifBodyRegion->getID() << "\n";
		}
		if (this->getBranchInfo()->elseBodyRegion) {
			debugFile << "ElseBodyRegion: " << this->getBranchInfo()->
				elseBodyRegion->getID() << "\n";
		}
	}

	if (this->mainNode->myIfNode && this->getBranchInfo()) {
		debugFile << "myIfNode = " << this->mainNode->myIfNode->getID() << "\n";

		if (this->getBranchInfo()->ifBodyRegion &&
			this->getID() == this->getBranchInfo()->ifBodyRegion->getID()
			) {
			debugFile << "If Body\n";
		} else
			if (this->getBranchInfo()->elseBodyRegion &&
			this->getID() == this->getBranchInfo()->elseBodyRegion->getID()
			) {
			debugFile << "Else Body\n";
		}
	}

	if (this->mainNode->myLoopInfo) {
		debugFile << "\nScheduling : "
			<< this->mainNode->myLoopInfo->schedulingPolicy.first;
	}

	if(this->myNodes.size() > 0){
		debugFile << "\nNodes :\n";
		for (DFGNode * tempNode:this->myNodes) {
			debugFile << tempNode->getID() << " : ";
			debugFile << tempNode->getName() << "\n";
			if (check_isa < CallExpr > (tempNode->getStmt())) {
				debugFile << "isCallerNode: "
					<< tempNode->isCallerNode()
					<< "\n"
					<< "calleeFunctionNodeName: "
					<< tempNode->calleeFunctionNodeName
					<< "\n"
					<< "calleeFunctionNodeName.empty(): "
					<< tempNode->calleeFunctionNodeName.empty()
					<< "\n";
			}
		}
		debugFile << "\n\n";
	}

	debugFile << "childRegions "
		<< this->childRegions.size()
		<< "\n";
	TPRegion *childRegion = this->childRegions.front();
	while (childRegion) {
		debugFile << childRegion->getName()
			<< ":"
			<< childRegion->getID()
			<< " , ";

		childRegion = childRegion->next;
	}
	debugFile << "\n";

	debugFile << "inlinedRegions "
		<< this->inlinedRegions.size()
		<< "\n";

	for (TPRegion * inlinedChildRegion:this->inlinedRegions) {
		debugFile << inlinedChildRegion->getName()
			<< ":"
			<< inlinedChildRegion->getID()
			<< " , ";
	}
	debugFile << "\n";

	childRegion = this->childRegions.front();
	while (childRegion) {
		childRegion->showRegionInfo();
		childRegion = childRegion->next;
	}

}

}