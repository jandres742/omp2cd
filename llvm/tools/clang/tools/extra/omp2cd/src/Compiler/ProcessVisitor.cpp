#include "include/Compiler/ProcessVisitor.h"
#include "include/DFG/LoopCondNode.h"
#include "include/DFG/TPRegion.h"
#include "include/DFG/FunctionInfo.h"
#include "include/DFG/TaskInfo.h"
#include "include/DFG/LoopInfo.h"
#include "include/DFG/BranchInfo.h"
#include "include/DFG/AtomicNode.h"

using namespace llvm;
using namespace clang;
using namespace clang::tooling;
using namespace std;

namespace omp2cd_space {
void ompASTVisitor::generateFineGrainCode() {
	
	this->includesStream << "#include <unistd.h>\n"
		<< "#include <time.h>\n"
		<< "#include <sys/time.h>\n"
		<< "#include <limits.h>\n"
		<< "#include \"utils.h\"\n"
		<< "#include <numa.h>\n";
		
	if(DARTS_BACKEND){
		this->includesStream << "#include \"darts.h\"\n"
			<< "#include \"ompTP.h\"\n"
			<< "#include \"tbb/concurrent_vector.h\"\n"
			<< "#include <mutex>\n";
	} else if(SWARM_BACKEND){
		this->includesStream << "#include <swarm/Runtime.h>\n"
			<< "#include <swarm/Codelet.h>\n"
			<< "#include <swarm/construct.h>\n"
			<< "#include <swarm/Locale.h>\n"
			<< "#include <swarm/util/atomic.h>\n"
			<< "#include <swarm/Barrier.h>\n"
			<< "#include <eti/swarm_convenience.h>\n";
	}

	/*For each function, optimize its DFG and generate its regions.*/
	for (myFunctions_Elem_t functionPair:this->myDFG.myFunctions) {
		DFGNode *functionNode = get < 0 > (functionPair.second);
		if (functionNode->myFunctionInfo->isOMPDARTSFunction(functionTable)) {
			this->optimizeDFG(functionNode);
			this->generateRegions(nullptr, functionNode);
		}
	}
	
	/*Print the declaration of any function declared as static.*/
	for (myFunctions_Elem_t functionPair:this->myDFG.myFunctions) {
		DFGNode *functionNode = get < 0 > (functionPair.second);
		
		bool numParallelRegions = 0;
		FunctionTable_t::iterator it = functionTable.find(functionNode->getName());
		if (it != functionTable.end())
			numParallelRegions = get < 5 > (it->second);
		
		if (functionNode->myFunctionInfo->isOMPDARTSFunction(functionTable) == false || 
			(functionNode->myFunctionInfo->isOMPDARTSFunction(functionTable) && numParallelRegions > 0) ||
			functionNode->getName().compare("main") == 0) {
		
			myFunctions_t::iterator it = this->myDFG.myFunctionsDecl.find(functionNode->getName());
			if(it != this->myDFG.myFunctionsDecl.end()){				
				ostringstream tempStream;
				get<0>(it->second)->getFunctionInfo()->printInterface(tempStream, "", false);
				if(tempStream.str().find("static ") != string::npos){
					this->codeStream << tempStream.str() << ";\n";
				}
				else{
					this->includesStream << tempStream.str() << ";\n";
				}
			}
		}
	}

	/*Generate the code for each function.*/
	/*Use the vector to print the functions in the same order as in the original code.*/
	for (string functionName:this->myDFG.myFunctionsVec){
		
		myFunctions_t::iterator itFunctionVec = this->myDFG.myFunctions.find(functionName);
		if(itFunctionVec == this->myDFG.myFunctions.end())
			continue;
		
		myFunctions_Elem_t functionPair = *itFunctionVec;

		DFGNode *functionNode = get < 0 > (functionPair.second);
		
		if (functionNode->myFunctionInfo->isOMPDARTSFunction(functionTable)) {
			TPRegion *functionRegion = get < 1 > (functionPair.second);
			if (functionRegion) {
				functionRegion->makeSingleThreaded();
			}
		}
		
		int numOfParallelRegions = 0;
		FunctionTable_t::iterator it = functionTable.find(functionNode->getName());
		if (it != functionTable.end())
			numOfParallelRegions = get < 5 > (it->second);

		/*Print a function that does not contain omp regions*/
		if (functionNode->myFunctionInfo->isOMPDARTSFunction(functionTable) ==false) {
			this->printSequentialFunction(functionNode);
		} else {
			/*Print functions with parallel regions*/
			if (numOfParallelRegions > 0) {
				this->printSequentialFunction(functionNode);

				TPRegion *functionRegion = get < 1 > (functionPair.second);
				vector < TPRegion * >ompParallelRegionsVec;
				functionRegion->getParallelRegions(ompParallelRegionsVec);

				for (TPRegion * &parallelRegion: ompParallelRegionsVec) {
					parallelRegion->preProcess();

					string regionDefinitionStr;
					printRegion(parallelRegion, regionDefinitionStr);
					this->myDFG.TPDefinitions.push_back(regionDefinitionStr);

					string regionImplementationStr;
					printRegionCode(parallelRegion,regionImplementationStr);
					this->myDFG.TPImplementations.push_back(regionImplementationStr);
				}
			} else {
				/*Print functions without parallel regions*/
				TPRegion *functionRegion = get < 1 > (functionPair.second);

				functionRegion->preProcess();

				string regionDefinitionStr;
				printRegion(functionRegion,regionDefinitionStr);
				this->myDFG.TPDefinitions.push_back(regionDefinitionStr);

				string regionImplementationStr;
				printRegionCode(functionRegion,regionImplementationStr);
				this->myDFG.TPImplementations.push_back(regionImplementationStr);
			}
		}
	}

	/*Write down the files*/		
	string includeHFileStr;
	
	this->writeHeaderFile(includeHFileStr);
	this->writeImplemFile(includeHFileStr);

}

void ompASTVisitor::writeHeaderFile(std::string &includeHFileStr)
{
	this->myDFG.printGlobalVars(this->globalVarsStream, this->context);
	this->myDFG.printFunctionStaticVars(this->globalVarsStream, this->context);
	this->myDFG.printThreadPrivateVars(this->globalVarsStream, this->context);

	string guardNameTemp = outputFileNameH;
	string guardName = "";

	size_t pos0 = guardNameTemp.find_last_of("/");
	if (pos0 != std::string::npos)
		guardName =
			guardNameTemp.substr(pos0 + 1, guardNameTemp.length() - pos0);

	includeHFileStr = guardName;

	size_t pos = guardName.find_first_of(".");
	string underscoreStr = "_";
	while (pos != string::npos) {
		guardName.replace(pos, 1, underscoreStr);
		pos = guardName.find_first_of(".");
	}

	pos = guardName.find_first_of("-");
	while (pos != string::npos) {
		guardName.replace(pos, 1, underscoreStr);
		pos = guardName.find_first_of("-");
	}

	if(DARTS_BACKEND)
		this->includesStream << "extern int DARTS_CODELETS_MULT;\n"
			<< "extern int NUMTPS;\n"
			<< "extern size_t numOfCUs;\n"
			<< "extern darts::Codelet* RuntimeFinalCodelet;\n"
			<< "extern darts::ThreadAffinity *affin;\n"
			<< "extern bool affinMaskRes;\n"
			<< "extern darts::Runtime *myDARTSRuntime;\n"
			<< "extern std::vector< std::vector < void* > > threadFunctionStack;\n";
	else if(SWARM_BACKEND)
			this->includesStream << "extern swarm_Runtime_t* mySWARMruntime;\n";

	this->includesStream << "extern size_t ompNumThreads;\n"
		<< "extern int ompSchedulePolicy;\n"
		<< "extern int ompScheduleChunk;\n"
		<< "extern void omp_set_num_threads(unsigned long numThreadsToSet);\n"
		<< "extern int omp_get_num_threads();\n"
		<< "extern int omp_get_max_threads();\n"
		<< "extern int omp_get_num_procs();\n"
		<< "extern double omp_get_wtime();\n" 
		<< "extern void omp_init_lock(omp_lock_t * lock);\n"
		<< "extern void omp_destroy_lock(omp_lock_t * lock);\n"
		<< "extern void omp_set_lock(omp_lock_t * lock);\n"
		<< "extern void omp_unset_lock(omp_lock_t * lock);\n";
		
	if(SWARM_BACKEND)
		this->includesStream << "extern uint64_t currentNumThreads[MAXNUMTHREADS];\n";

	/*H file */
	outputFileH.open(outputFileNameH);
	outputFileH << "#ifndef _" << guardName << "_\n"
		<< "#define _" << guardName << "_\n";
	
	if(DARTS_BACKEND){
		outputFileH	<< "#ifndef __DARTS_\n"
			<< "#define __DARTS_\n"
			<< "#endif\n";
	} else if(SWARM_BACKEND){
		outputFileH	<< "#ifndef __OMP2CD_SWARM_\n"
			<< "#define __OMP2CD_SWARM_\n"
			<< "#endif\n";
	}
		
	outputFileH	<< outputInclude
		<< this->typedefDeclStream.str()
		<< this->includesStream.str()
		<< this->constantsDeclStream.str();
	for (string regionStr:this->myDFG.TPDefinitions)
		outputFileH << regionStr;

	outputFileH << "#endif\n";
	outputFileH.close();
}

void ompASTVisitor::writeImplemFile(std::string &includeHFileStr)
{
	for (string regionStr:this->myDFG.TPImplementations)
		this->codeStream << regionStr;

	outputFileCpp.open(outputFileNameCpp);
	outputFileCpp << "#include \"" << includeHFileStr << "\"\n";
	
	if(DARTS_BACKEND){
		outputFileCpp << "using namespace darts;\n"
			<< "using namespace std;\n";
	}
	
	outputFileCpp << this->globalVarsStream.str()
		<< this->codeStream.str();

	outputFileCpp.close();
}

void ompASTVisitor::optimizeDFG(DFGNode * node)
{
	if (node) {
		if (check_isa < IfStmt > (node->getStmt())) {
			if (node->parent && check_isa < IfStmt > (node->parent->getStmt())) {
				node->myBranchInfo->isSimpleIf = node->parent->myBranchInfo->isSimpleIf;
			} else {
				node->myBranchInfo->isSimpleIf = !(node->checkIfComplexStmt());
			}
		} else if (check_isa < CallExpr > (node->getStmt())) {
			this->optimizeCallExpr(node);
		}

		DFGNode *child = node->myNodes.front();
		while (child) {
			DFGNode *nextChild = child->next;
			this->optimizeDFG(child);
			child = nextChild;
		}

		if (check_isa < OMPAtomicDirective > (node->getStmt()))
			this->optimizeCriticalAtomic(node);
		else if (check_isa < OMPCriticalDirective > (node->getStmt()))
			this->optimizeCriticalAtomic(node);
		else if (node->isOMPFor())
			this->optimizeOMPFor(node);
		else if (check_isa < ForStmt > (node->getStmt()))
			this->optimizeForStmt(node);
		else if (check_isa < WhileStmt > (node->getStmt()) && node->isComplexStmt)
			this->optimizeWhileStmt(node);
		else if (check_isa < DoStmt > (node->getStmt()) && node->isComplexStmt)
			this->optimizeDoStmt(node);
		else if (check_isa < OMPTaskDirective > (node->getStmt()))
			this->optimizeOMPTask(node);
	}
}

void ompASTVisitor::optimizeOMPTask(DFGNode * node)
{
	/*Check if the last stmt in the task is a function call, in which case,
	add a new node so that node can mark the task as complete*/
	DFGNode *backNode = node->myNodes.back();	
	while(check_isa<CompoundStmt>(backNode->getStmt()))
		backNode = backNode->myNodes.back();

	if(backNode){
		if(check_isa<CallExpr>(backNode->getStmt())){
			CallExpr *callExpr = dyn_cast<CallExpr>(backNode->getStmt());
			string calleeFunctionName = this->getCalleeFunctionName(callExpr);
			FunctionTable_t::iterator it = functionTable.find(calleeFunctionName);
			if (it != functionTable.end() && get < 1 > (it->second))
				this->insertNullNodeAfter(backNode);
		} else if(check_isa<IfStmt>(backNode->getStmt()) && backNode->isComplexStmt){
			this->insertNullNodeAfter(backNode);
		} else if(check_isa<ForStmt>(backNode->getStmt()) && backNode->isComplexStmt){
			this->insertNullNodeAfter(backNode);
		}
	}
}

void ompASTVisitor::insertNullNodeAfter(DFGNode* prevNode, DFGNodeNullStmtType_t nullStmtAction,
	std::string nullStmtActionVar)
{
	uint8_t *nullStmtPtr = (uint8_t*) this->context->Allocate(sizeof(NullStmt));
	NullStmt *nullStmt = nullptr;
	if(nullStmtPtr)
		nullStmt = new(nullStmtPtr) NullStmt(prevNode->getStmt()->getLocEnd());
	
	DFGNode *newNode = new DFGNode(nullStmt, prevNode->parent, getNewNodeID());
	if (newNode){
		newNode->nullStmtAction = nullStmtAction;
		newNode->nullStmtActionVar = nullStmtActionVar;
		prevNode->parent->myNodes.insertAfter(prevNode, newNode);
	}
}

void ompASTVisitor::optimizeOMPFor(DFGNode * node)
{
	if(node->ompExtensionClause->codelet) {
		if(node->getLoopInfo()->reductionVars.size() > 0)
			node->ompExtensionClause->codelet = false;
		
		if(useTPLoops)
			node->ompExtensionClause->codelet = false;
		
		bool atLeastOneLastPrivate = false;
		for (auto var:node->ompInputs) {
			if (get < 1 > (var.second) == OMP_LASTPRIVATE) {
				atLeastOneLastPrivate = true;
				break;
			}
		}
		if(atLeastOneLastPrivate)
			node->ompExtensionClause->codelet = false;
	}

	/*make sure the loop var is private inside the omp for */
	string rawLoopVar = node->getLoopInfo()->getForStmtVarRaw();
	TPInputs_t::iterator it = node->ompInputs.find(rawLoopVar);
	if (it != node->ompInputs.end()){
		if(get < 1 > (it->second) != OMP_LASTPRIVATE)
			get < 1 > (it->second) = OMP_PRIVATE;
	}

	/*make sure the loop var is not optimized as local variable */
	it = node->ompVariables.find(rawLoopVar);
	if (it != node->ompVariables.end())
		get < 5 > (it->second) = false;

	/* delete all the children of an OMPFor, except for the for stmt */
	DFGNode *childNode = node->myNodes.front();
	while (childNode) {
		DFGNode *nextNode = childNode->next;
		if (check_isa < ForStmt > (childNode->getStmt()) == false) {
			node->myNodes.remove(childNode);
			delete(childNode);
		}
		childNode = nextNode;
	}

/*We have deactivated this for performance/correctness reasons, but we have to put it
back to have parallel for loops as balanced*/
#if 0
	/*If this is an omp parallel for, insert a new parent node to have balanceOMPFor */
	if (check_isa < OMPParallelForDirective > (node->getStmt())) {
		DFGNode *parentNode =
			new DFGNode(node->getStmt(), node->parent, this->getNewNodeID());
		parentNode->myLoopInfo = node->myLoopInfo;
		node->parent->myNodes.replace(node, parentNode);
		node->prev = nullptr;
		node->next = nullptr;
		node->parent = parentNode;
	}
#endif
}



void ompASTVisitor::optimizeCriticalAtomic(DFGNode * node)
{
	/* Make sure an atomic has only one child */
	if (check_isa < OMPAtomicDirective > (node->getStmt())) {
		if (node->myNodes.size() > 1) {
			DFGNode *tempNode = node->myNodes.back();
			while (tempNode && tempNode != node->myNodes.front()) {
				DFGNode *tempPrev = tempNode->prev;
				node->myNodes.remove(tempNode);
				tempNode = tempPrev;
			}
		}

		/*Check if the variable on the left hand of expresion under the atomic
		 * has a type valid for a gcc builtin. If not, implement it using
		 * as a critical region, with locks*/
		if (node->myNodes.front()) {
			BinaryOperator *binOp =
				dyn_cast < BinaryOperator > (node->myNodes.front()->getStmt());
			if (binOp) {
				Expr *lhs = binOp->getLHS();
				if (lhs) {
					string typeString = lhs->getType().getAsString();
					if (typeString.compare("double") == 0
						|| typeString.compare("float") == 0) {
						DFGNode *frontNode = node->myNodes.front();
						frontNode->criticalDirectiveName =
							node->criticalDirectiveName;
						frontNode->criticalDirectiveName +=
							to_string(node->getID());
						frontNode->criticalDirectiveLock = true;

						frontNode->criticalDirectiveName =
							node->criticalDirectiveName;
						frontNode->criticalDirectiveName +=
							to_string(node->getID());
						frontNode->criticalDirectiveUnlock = true;
					} else {
						/* change the remaining child to atomic node */
						AtomicNode *newAtomicNode =
							new AtomicNode(node->myNodes.
								   front()->getStmt(), node,
								   node->myNodes.
								   front()->getID());
						newAtomicNode->ompAtomicParentNode =
							dyn_cast < AtomicNode > (node);
						node->myNodes.replace(node->myNodes.front(),
									  newAtomicNode);
					}
				}
			} else {
				/* change the remaining child to atomic node */
				AtomicNode *newAtomicNode =
					new AtomicNode(node->myNodes.front()->getStmt(), node,
						   node->myNodes.front()->getID());
				newAtomicNode->ompAtomicParentNode =
					dyn_cast < AtomicNode > (node);
				node->myNodes.replace(node->myNodes.front(), newAtomicNode);
			}
		}
	}
	/* attach the children of the critical/atomic node to the node's parent
	we don't want to create a TP for critical/atomic nodes */
	DFGNode *frontNode = node->myNodes.front();
	DFGNode *backNode = node->myNodes.back();
	if (check_isa < CompoundStmt > (frontNode->getStmt())) {
		node->parent->myNodes.length += frontNode->myNodes.size();
		backNode = frontNode->myNodes.back();
		frontNode = frontNode->myNodes.front();
	} else {
		node->parent->myNodes.length += node->myNodes.size();
	}

	if (check_isa < OMPCriticalDirective > (node->getStmt())) {
		frontNode->criticalDirectiveName = node->criticalDirectiveName;
		frontNode->criticalDirectiveName += to_string(node->getID());
		frontNode->criticalDirectiveLock = true;

		backNode->criticalDirectiveName = node->criticalDirectiveName;
		backNode->criticalDirectiveName += to_string(node->getID());
		backNode->criticalDirectiveUnlock = true;
	}

	frontNode->parent = node->parent;
	if (frontNode != backNode) {
		DFGNode *childNode = frontNode->next;
		while (childNode) {
			childNode->parent = node->parent;
			childNode = childNode->next;
		}
	}

	frontNode->prev = node->prev;
	if (node->prev)
		node->prev->next = frontNode;

	if (node->parent->myNodes.head == node)
		node->parent->myNodes.head = frontNode;

	backNode->next = node->next;
	if (node->next)
		node->next->prev = backNode;

	if (node->parent->myNodes.tail == node)
		node->parent->myNodes.tail = backNode;

	node->parent->myNodes.length -= 1;

	/* move inputs and vars from this node to its parent */
	DFGNode *parentNode = node->findParentNode();
	if (parentNode)
		for (auto var:	node->ompVariables)
			parentNode->ompVariables.insert(var);
}


void ompASTVisitor::optimizeDoStmt(DFGNode * node)
{
	if (node->myLoopInfo && node->parent) {
		LoopCondNode *condNode = node->myLoopInfo->condNode;
		if (condNode)
			node->parent->myNodes.insertBefore(node, condNode);

		condNode->loopFirstNode = node;
		condNode->loopNextNode = node->next;

		LoopCondNode *condNodeCopy =
			new LoopCondNode(condNode, this->getNewNodeID());
		node->parent->myNodes.insertAfter(node, condNodeCopy);
		condNode->endCondLoopNode = condNodeCopy;
	}
}


void ompASTVisitor::optimizeWhileStmt(DFGNode * node)
{
	/*Don't move nodes if this for includes an omp parallel region */
	bool moveNodes = true;

	vector < DFGNode * >ompParallelNodesVec;
	node->getParallelNodes(ompParallelNodesVec);
	if (ompParallelNodesVec.size() > 0)
		moveNodes = false;

	if (node->myLoopInfo && node->parent) {
		LoopCondNode *condNode = node->myLoopInfo->condNode;
		if (condNode && moveNodes)
			node->parent->myNodes.insertBefore(node, condNode);

		condNode->loopFirstNode = node;
		condNode->loopNextNode = node->next;

		LoopCondNode *condNodeCopy =
			new LoopCondNode(condNode, this->getNewNodeID());
		if (moveNodes)
			node->parent->myNodes.insertAfter(node, condNodeCopy);

		condNode->endCondLoopNode = condNodeCopy;
	}
}


void ompASTVisitor::optimizeForStmt(DFGNode * node)
{
	/*Don't move nodes if this for includes an omp parallel region */
	bool moveNodes = true;

	vector < DFGNode * >ompParallelNodesVec;
	node->getParallelNodes(ompParallelNodesVec);
	if (ompParallelNodesVec.size() > 0)
		moveNodes = false;

	/*make sure the loop var is private inside the omp for */
	string rawLoopVar = node->getLoopInfo()->getForStmtVarRaw();
	DFGNode *ompParentNode = node->findOMPParent();
	if (ompParentNode) {
		TPInputs_t::iterator it = ompParentNode->ompInputs.find(rawLoopVar);
		if (it != ompParentNode->ompInputs.end()) {
			if(check_isa<OMPTaskDirective>(ompParentNode->getStmt())){
				get < 1 > (it->second) = OMP_FIRSTPRIVATE;
			} else {
				if(get < 1 > (it->second) != OMP_LASTPRIVATE)
					get < 1 > (it->second) = OMP_PRIVATE;
			}
		}
	}

	if (node->myLoopInfo && node->parent && node->isComplexStmt) {
		DFGNode *initNode = node->myLoopInfo->initNode;

		if (initNode && moveNodes)
			node->parent->myNodes.insertBefore(node, initNode);

		LoopCondNode *condNode = node->myLoopInfo->condNode;
		if (condNode && moveNodes)
			node->parent->myNodes.insertAfter(initNode, condNode);

		condNode->loopFirstNode = node;
		condNode->loopNextNode = node->next;

		DFGNode *incNode = node->myLoopInfo->incNode;
		if (incNode && moveNodes)
			node->parent->myNodes.insertAfter(node, incNode);

		LoopCondNode *condNodeCopy = new LoopCondNode(condNode, this->getNewNodeID());
		node->parent->myNodes.insertAfter(incNode, condNodeCopy);
		condNode->endCondLoopNode = condNodeCopy;
	}
}

/*Make sure a function call is void and the return value is returned to the pointer
to a temporary value.
*/
void ompASTVisitor::optimizeCallExpr(DFGNode * node)
{
	string calleeFunctionName = node->calleeFunctionNodeName;

	FunctionTable_t::iterator it = functionTable.find(calleeFunctionName);
	if (it != functionTable.end() && get < 1 > (it->second)) {
		
		string returnType = get < 0 > (it->second);

		/*Find the declstmt, binoperator, or functioncall that has the callexpr*/
		DFGNode *parentNode = node->getParent();
		while(parentNode){
			if(check_isa<DeclStmt>(parentNode->getStmt()))
				break;
			else if(check_isa<BinaryOperator>(parentNode->getStmt()))
				break;
			else if(check_isa<CallExpr>(parentNode->getStmt()))
				break;
			else
				parentNode = parentNode->getParent();
		}

		/*Create temporary variable for non-void functions.*/
		if (returnType.compare("void") != 0 && parentNode && parentNode->parent) {
			if (check_isa < DeclStmt > (parentNode->getStmt())) {
				DeclStmt *declStmt = dyn_cast < DeclStmt > (parentNode->getStmt());
				
				/*TODO: We have tested this with only one declaration per line.*/
				Decl **declIt = declStmt->decl_begin();
				string declName = getNameFromDecl(*declIt);

				node->calleeTmpVarName = declName;
				node->prev = parentNode->prev;
				if (parentNode->prev)
					parentNode->prev->next = node;

				node->next = parentNode->next;
				if (parentNode->next)
					parentNode->next->prev = node;

				if (parentNode == parentNode->parent->myNodes.front())
					parentNode->parent->myNodes.head = node;

				if (parentNode == parentNode->parent->myNodes.back())
					parentNode->parent->myNodes.tail = node;
				parentNode->myNodes.swap(parentNode, node);
			

			} else if (check_isa < BinaryOperator > (parentNode->getStmt())) {

				Expr *lhs = (dyn_cast < BinaryOperator > (parentNode->getStmt()))->getLHS();

				string ostreamString;
				raw_string_ostream rawStringOStream(ostreamString);
				PrintingPolicy Policy = context->getPrintingPolicy();
				lhs->printPretty(rawStringOStream, nullptr, Policy);
				string lhsStr = rawStringOStream.str();

				node->calleeTmpVarName = lhsStr;

				node->prev = parentNode->prev;
				if (parentNode->prev)
					parentNode->prev->next = node;

				node->next = parentNode->next;
				if (parentNode->next)
					parentNode->next->prev = node;

				if (parentNode == parentNode->parent->myNodes.front())
					parentNode->parent->myNodes.head = node;

				if (parentNode == parentNode->parent->myNodes.back())
					parentNode->parent->myNodes.tail = node;

				node->parent = parentNode->parent;

			} else {
				ostringstream newVarName;
				newVarName << calleeFunctionName << "_" << node->getID() << "_var";
				node->calleeTmpVarName = newVarName.str();

				DFGNode *newNode = new DFGNode(node, getNewNodeID());

				parentNode->parent->insertNewNodeBeforeChildNode(parentNode, newNode);

				node->useTmpVarInsteadOfCall = true;

				if (node->parent) {
					pair < string, string > newPair(calleeFunctionName, newVarName.str());
					node->parent->calleeNames.insert(newPair);

					if (node->findParentNode())
						node->findParentNode()->addNewVar(returnType, newVarName.str());
				}
			}
		}
	}
}

DFGNode *ompASTVisitor::getCalleeFunctionNode(CallExpr * s)
{
	DFGNode *calleeFunctionNode = nullptr;
	Decl *calleeDecl = s->getCalleeDecl();
	if (calleeDecl) {
		string calleName = getNameFromDecl(calleeDecl);
		myFunctions_t::iterator it = this->myDFG.myFunctions.find(calleName);
		if (it != this->myDFG.myFunctions.end())
			calleeFunctionNode = get < 0 > (it->second);
	}
	return calleeFunctionNode;
}



int ompASTVisitor::getCalleeFunctionNodeID(std::string functName)
{
	int id = -1;
	FunctionTable_t::iterator it = functionTable.find(functName);
	if (it != functionTable.end())
		id = get < 2 > (it->second);
	return id;
}

/*This is one of the most important functions since it's the one creating
TP regions from AST nodes.
A TP region is basically a CBB or Codelet Basic Block, which is to be
executed by a codelet.
This function implements the rules to define a CBB leader.*/
void ompASTVisitor::generateRegions(TPRegion * parentRegion, DFGNode * node)
{
	bool isFunctionDecl = node->myFunctionInfo != nullptr;

	bool isOMPDirective = check_isa < OMPExecutableDirective > (node->getStmt());

	debugFile << "========================================================\n";
	debugFile << "Processing node " << node->name << " " << node->getID() << "\n";

	if(node->myNodes.size() > 0){
		debugFile << "children: ";
		DFGNode* tempChildNode = node->myNodes.front();
		while(tempChildNode){
			debugFile << tempChildNode->getName() << " " << tempChildNode->getID() << ", ";
			tempChildNode = tempChildNode->next;
		}
		debugFile << "\n";
	}

	if(node->name.compare("IfStmt") == 0){
		if(node->getBranchInfo()->ifBodyNode)
			debugFile << "ifBodyNode: " << node->getBranchInfo()->ifBodyNode->getName()
				<< " " << node->getBranchInfo()->ifBodyNode->getID() << ", ";
		if(node->getBranchInfo()->elseBodyNode)
			debugFile << "elseBodyNode: " << node->getBranchInfo()->elseBodyNode->getName()
				<< " " << node->getBranchInfo()->elseBodyNode->getID() << "\n";
	}

	if (parentRegion)
		debugFile << "parentRegion = " << parentRegion->getName() << " "
			<< parentRegion->getID() << " ";

	if (node->parent)
		debugFile << "node->parent = " << node->parent->getName() << " "
			<< node->parent->getID() << " ";

	debugFile << "\n";

	debugFile << "isFunctionDecl = " << isFunctionDecl << "\n";
	debugFile << "isOMPDirective = " << isOMPDirective << "\n";
	
	if (check_isa < IfStmt > (node->getStmt()))
		debugFile << "node->myBranchInfo->isSimpleIf = " << node->myBranchInfo->isSimpleIf << "\n";
	
	debugFile << "isCallerNode = " << node->isCallerNode() << "\n";
	if (node->isCallerNode()) 
		debugFile << "isCallerNode = " << node->useTmpVarInsteadOfCall << "\n";

	debugFile << "isLoopNode = " << node-> isLoopNode() << " isComplexStmt = " << node->isComplexStmt << "\n";	

	if (isFunctionDecl ||
		isOMPDirective ||
		(check_isa < IfStmt > (node->getStmt()) && node->myBranchInfo->isSimpleIf == false)
		|| (node->isLoopNode() && node->isComplexStmt)) {
		debugFile << "Creating region\n";

		TPRegion *tempRegion = new TPRegion(node, parentRegion);

		/* check if node is a function */
		myFunctions_t::iterator it = this->myDFG.myFunctions.find(node->getName());
		if (it != this->myDFG.myFunctions.end()) {
			get < 1 > (it->second) = tempRegion;
		}

		parentRegion = tempRegion;

		debugFile << "parentRegion = " << parentRegion->getName() << " " << parentRegion->getID() << "\n"
			<< "node->myNodes.size() = " << node->myNodes.size() << "\n";

		if (node->myNodes.size() > 0) {
			DFGNode *childNode = node->myNodes.front();
			TPRegion *currentRegion = nullptr;
			while (childNode) {
				
				debugFile << "---------------------\n"
					<< "In while-loop: childNode: " << childNode->getName() << " " << childNode->getID() << " "
					<< "childNode->myNodes.size() = " << childNode->myNodes.size() << "\n";
					if(childNode->myNodes.size() > 0){
						debugFile << "children: ";
						DFGNode* tempChildNode = childNode->myNodes.front();
						while(tempChildNode){
							debugFile << tempChildNode->getName() << " " << tempChildNode->getID() << ", ";
							tempChildNode = tempChildNode->next;
						}
						debugFile << "\n";
					}

				DFGNode *tempNode = childNode;
				bool insideCompound = false;
				if (check_isa < CompoundStmt > (tempNode->getStmt())) {
					insideCompound = true;
					currentRegion = nullptr;
					if (tempNode->myNodes.size() > 0) {
						tempNode = tempNode->myNodes.front();
					} else {
						tempNode = nullptr;
					}
				}

				if (tempNode == nullptr) {
					childNode = childNode->next;
					continue;
				}

				do {
					debugFile << "-------------\n"
					 << "In do-loop: tempNode: " << tempNode->getName() << " " << tempNode->getID() << " "
						<< "tempNode->myNodes.size() = " << tempNode->myNodes.size() << "\n";
					if(currentRegion)
						debugFile << "currentRegion: " << currentRegion->getName() << " " << currentRegion->getID() << "\n";
					
					if (check_isa < IfStmt > (tempNode->getStmt())) {

						if (tempNode->parent && check_isa < IfStmt > (tempNode->parent->getStmt())) {
							tempNode->myBranchInfo->isSimpleIf = tempNode->parent->myBranchInfo->isSimpleIf;
						} else {
							tempNode->myBranchInfo->isSimpleIf = !(tempNode->checkIfComplexStmt());
						}
					}
					
					/*Check if the previous node is a function call inside a task.
					In such case, that function call is signaling the syncpoint and
					not the next node, so we must keep the function call and the current
					node in the same region.
					If the previous node is a function call, then we check if we are
					inside an omp region or a function converted into TP*/
					bool prevNodeIsFunctCall = false;				
					if(tempNode->prev && tempNode->prev->isCallerNode()){

						string calleeFunctionName = tempNode->prev->calleeFunctionNodeName;
							
						debugFile << "tempNode->prev->calleeFunctionNodeName: " << calleeFunctionName << "\n";
							
						FunctionTable_t::iterator it = functionTable.find(calleeFunctionName);
						if (it != functionTable.end() &&
							get<1>(it->second) == true /*It's an OMP2DARTS function*/){
							prevNodeIsFunctCall = true;
						}
					}
					
					if(tempNode->prev)
						debugFile << "tempNode->prev " << tempNode->prev->getID() << " prevNodeIsFunctCall: " << prevNodeIsFunctCall
						<< " tempNode->prev->calleeTmpVarName.empty(): " << tempNode->prev->calleeTmpVarName.empty()
						<< " tempNode->prev->useTmpVarInsteadOfCall: " << tempNode->prev->useTmpVarInsteadOfCall
						<< "\n";
					
					bool tempNodeIsFunctCall = false;
					if(tempNode->isCallerNode()){
						string calleeFunctionName = tempNode->calleeFunctionNodeName;
						
						debugFile << "calleeFunctionName: " << calleeFunctionName << "\n";
						
						FunctionTable_t::iterator it = functionTable.find(calleeFunctionName);
						if (it != functionTable.end() &&
							get<1>(it->second) == true /*It's an OMP2DARTS function*/){
							tempNodeIsFunctCall = true;
						}
					}
					
					debugFile << "tempNodeIsFunctCall: " << tempNodeIsFunctCall
						<< ", calleeTmpVarName: " << tempNode->calleeTmpVarName << "\n";

					if ((check_isa < OMPExecutableDirective >(tempNode->getStmt())) ||
						(check_isa < IfStmt > (tempNode->getStmt()) &&
						tempNode->myBranchInfo->isSimpleIf == false) ||
						(tempNode->isLoopNode() && tempNode->isComplexStmt)) {
						debugFile << "calling generateRegions for tempNode : "
							<< tempNode->getID() << "\n";

						this->generateRegions(parentRegion, tempNode);

						debugFile << "coming back from calling generateRegions for tempNode : "
							<< tempNode->getID() << "\n";

						currentRegion = nullptr;
					}
					else if (tempNodeIsFunctCall){
						debugFile << "(1)creating new Region for tempNode : " << tempNode->getID() << "\n";
						TPRegion *tempChildRegion = new TPRegion(tempNode, parentRegion);
						
						if(currentRegion && currentRegion->getTaskInfo()){
							currentRegion->getTaskInfo()->nextRegionAfterTask = tempChildRegion;
							outs() << "currentRegion " << currentRegion->getID() << "\n";
						}

						currentRegion = tempChildRegion;
						debugFile << "currentRegion = " << currentRegion->getName() << " " << currentRegion->getID() << "\n";
					}
					else if (tempNode->prev && ((prevNodeIsFunctCall)
							|| (isa < LoopCondNode > (tempNode->prev)))) {

						debugFile << "(2)creating new Region for tempNode : " << tempNode->getID() << "\n";

						TPRegion *tempChildRegion = new TPRegion(tempNode, parentRegion);
						
						if(currentRegion && currentRegion->getTaskInfo()){
							outs() << "currentRegion " << currentRegion->getID() << "\n";
							currentRegion->getTaskInfo()->nextRegionAfterTask = tempChildRegion;
						}
						
						currentRegion = tempChildRegion;

						debugFile << "currentRegion = " << currentRegion->getName() << " " << currentRegion->getID() << "\n";

					} else {
						if (currentRegion == nullptr) {
							debugFile << "(3)creating new Region for tempNode : " << tempNode->getID() << "\n";
							currentRegion = new TPRegion(tempNode, parentRegion);
							debugFile << "currentRegion = " << currentRegion->getName()
								<< " " << currentRegion->getID() << "\n";
						} else {
							debugFile << "adding  tempNode : " << tempNode->getID()
								<< " to current region " << currentRegion->getName()
								<< " " << currentRegion->getID() << "\n";
							currentRegion->addNode(tempNode);
							tempNode->myRegion = currentRegion;
						}
					}
					tempNode = tempNode->next;
				}
				while (tempNode && insideCompound);
				childNode = childNode->next;
			}
		}
	}
}

void ompASTVisitor::setMyIfNode(DFGNode * node)
{
	/* consider if stmts without a compound stmt */
	if (check_isa < CompoundStmt > (node->getStmt()) == false &&
		node->parent && check_isa < IfStmt > (node->parent->getStmt())
		) {
		node->myIfNode = node->parent;
	}
	/* consider if stmts with a compound stmt */
	else if
		(node->parent &&
		 check_isa < CompoundStmt > (node->parent->getStmt()) &&
		 node->parent->parent && check_isa < IfStmt > (node->parent->parent->getStmt())
		) {
		node->myIfNode = node->parent->parent;
	}
}


void ompASTVisitor::findCallsWithExtraArgsInNode(DFGNode * node,
						 std::vector < std::pair < std::string,
						 std::string > >&extraArgPairs)
{
	DFGNode *childNode = node->myNodes.front();
	while (childNode) {
		if (check_isa < CallExpr > (childNode->getStmt())) {
			string calleeName = "";
			CallExpr *call = dyn_cast < CallExpr > (childNode->getStmt());
			if (call) {
				Decl *calleeDecl = call->getCalleeDecl();
				if (calleeDecl) {
					calleeName = getNameFromDecl(calleeDecl);
				}
			}
			FunctionTable_t::iterator it = functionTable.find(calleeName);
			if (it != functionTable.end() && get < 1 > (it->second) == false) {
				for (std::pair < std::string, ExtraArgsTuple_t >
						extraArgsPair:get < 3 > (it->second)){
					string replacementExpr = get < 1 > (extraArgsPair.second);
					std::pair < std::string,std::string > newPair(calleeName,replacementExpr);
					extraArgPairs.push_back(newPair);
				}
			}
		} else {
			this->findCallsWithExtraArgsInNode(childNode, extraArgPairs);
		}
		childNode = childNode->next;
	}
}


void ompASTVisitor::setPrintingPolicyParameters(DFGNode * node,
						clang::PrintingPolicy & Policy)
{
	DFGNode *parentNode = node;
	if (check_isa < OMPExecutableDirective > (parentNode->getStmt()) == false)
		parentNode = node->findParentNode();

	DFGNode *functionNode = node->findFunctionParent();

	TPRegion *parentRegion = node->getRegion()->findOMPOrFunctionRegionParent();

	/*darts_TPID comes from the parentRegion, while the TPInputs and TPVars come
	 * from the parentNode because an OMP node might have different data-sharing
	 * attributes for variables, but all of them are declared in the parentRegion.
	 */
	Policy.darts_printingInTP = true;
	Policy.darts_functionNodeID = functionNode->getID();
	Policy.darts_TPID = parentRegion->getID();
	Policy.darts_parentNodeID = parentNode->getID();
	Policy.darts_TPInputs = parentNode->ompInputs;
	Policy.darts_TPVars = parentNode->ompVariables;
	Policy.darts_TPCallees = node->calleeNames;
	Policy.darts_varTable2 = &(this->myDFG.varTable2);
	Policy.backEnd = backEnd;

	vector<int> darts_TPIDTree;
	TPRegion *tempParentRegion = parentRegion;
	while (tempParentRegion) {
		darts_TPIDTree.push_back(tempParentRegion->getID());
		tempParentRegion = tempParentRegion->parent;
	}
	Policy.darts_TPIDTree = darts_TPIDTree;

	/*Take the extraArgs from the node for function calls that need an extra
	 * arg, for instance, to avoid calls to omp_get_num_thread().
	 * */
	std::vector < std::pair < std::string, std::string > >extraArgPairs(0);
	this->findCallsWithExtraArgsInNode(node, extraArgPairs);
	for (std::pair < std::string, std::string > extraArgPair:extraArgPairs)
		Policy.darts_TPExtraArgs.insert(extraArgPair);

	/*Prefix*/
	if(DARTS_BACKEND){
		if (check_isa < OMPTaskDirective > (parentNode->getStmt())
			&& parentNode->myTaskInfo->encapsulateTaskData) {
			string prefix = "this->taskInputs->";
			Policy.prefixStr = prefix;
		} else {
			Policy.prefixStr = DARTS_PREFIXSTR_DEFAULT;
		}
	} else if(SWARM_BACKEND){
		Policy.prefixStr = "";
	}

	/*Postfix*/	
	if (check_isa < OMPTaskDirective > (parentNode->getStmt())
		&& parentNode->myTaskInfo->encapsulateTaskData) {
		Policy.postfixStr = "";
	} else if (parentRegion->singleThreaded) {
		Policy.postfixStr = "";
	} else {
		if(DARTS_BACKEND)
			Policy.postfixStr = "[this->getID()]";
		else if(SWARM_BACKEND)
			Policy.postfixStr = "[codeletID]";
	}
}
}