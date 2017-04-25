#include "include/Compiler/ProcessVisitor.h"
#include "include/DFG/TPRegion.h"
#include "include/DFG/FunctionInfo.h"
#include "include/DFG/LoopInfo.h"
#include "include/DFG/BranchInfo.h"
#include "include/DFG/TaskInfo.h"
#include "include/DFG/LoopCondNode.h"
#include "include/DFG/AtomicNode.h"

using namespace llvm;
using namespace clang;
using namespace clang::tooling;
using namespace std;

namespace omp2cd_space {

/* Print a sequential function implementation for a functionNode.*/
void ompASTVisitor::printSequentialFunction(omp2cd_space::DFGNode * functionNode)
{
	/*print function definition */
	FunctionTable_t::iterator it = functionTable.find(functionNode->getName());
	string extraArgs = "";

	/*print function implementation */
	this->codeStream << "/*Function: " << functionNode->getName()
		<< ", ID: " << functionNode->getID() << "*/\n";

	functionNode->myFunctionInfo->printInterface(this->codeStream, extraArgs, false);
	this->codeStream << "{\n";

	PrintingPolicy Policy = context->getPrintingPolicy();
	if (it != functionTable.end()) {
		map < string, string > calleeNames;
		for (std::pair < std::string, ExtraArgsTuple_t > extraArgsPair:get < 3 > (it->second)){
			string cause = extraArgsPair.first;
			string replacementVar = get < 0 > (extraArgsPair.second).second;
			pair < string, string > pairNames(cause, replacementVar);
			calleeNames.insert(pairNames);
		}
		Policy.darts_TPCallees = calleeNames;
	}
	Policy.darts_varTable2 = &(this->myDFG.varTable2);
	Policy.backEnd = backEnd;

	string tempString("");
	/* Check if this is main() */
	if (functionNode->myFunctionInfo->f->isMain()) {
		if(DARTS_BACKEND){
			this->codeStream << "getOMPNumThreads();\n"
				<< "getOMPSchedulePolicy();\n"
				<< "getTPLoopThresholds();\n"
				<< "getNumTPs();\n"
				<< "affin  = new ThreadAffinity"
				<< "(ompNumThreads/NUMTPS - 1, NUMTPS, "
				<< "COMPACT, getDARTSTPPolicy(), "
				<< "getDARTSMCPolicy());"
				<< "affinMaskRes = affin->generateMask ();\n"
				<< "myDARTSRuntime = new Runtime(affin);\n"
				<< "RuntimeFinalCodelet = &(myDARTSRuntime->finalSignal);\n";
		} else if (SWARM_BACKEND){
			this->codeStream << "getOMPNumThreads();\n"
			<< "getOMPSchedulePolicy();\n"
			<< "mySWARMruntime = swarm_posix_startRuntime(NULL, NULL, NULL, NULL);\n";
		}
		this->myDFG.printFunctionStaticVarFlagsInits(this->codeStream,this->context);
	}

	/*For other functions, just print it */
	this->printSequentialDFG(functionNode, tempString, Policy);

	/*Comment out any remaining pragmas */
	size_t pos = tempString.find("#pragma");
	while (pos != string::npos) {
		tempString.replace(pos, 7, "//#pragma");
		pos = tempString.find("#pragma", pos + 9);
	}
	this->codeStream << tempString
		<< "}\n";
}

/* Print the DFG of a sequential function.*/
void ompASTVisitor::printSequentialDFG(DFGNode * node, string & codeStr, PrintingPolicy & Policy)
{
	/*We print the node's number and name for debugging purposes.*/
	codeStr += "/*" + node->getName() + ":" + to_string(node->getID()) + "*/\n";

	DFGNode *child = node->myNodes.front();
	while (child) {
		if (child->isOMPParallel()) {
			
			ostringstream tempStream;
			
			if(DARTS_BACKEND){
				string numOfThreadsStr = "ompNumThreads * DARTS_CODELETS_MULT";

				/* If the parallel region has only one omp region and that is an omp
				 * for region, then a TPLoop will be used.*/
				if (child->getRegion()->hasOnlyOneChildOMPForRegion())
					numOfThreadsStr = "1";

				tempStream << "if (affinMaskRes)\n"
					<< "{\n"
					<< "myDARTSRuntime->run(launch<TP"<< child->getRegion()->getID()
					<< ">(" << numOfThreadsStr<< ", 0, RuntimeFinalCodelet";

				if (child->isOMPFor()) {
					string loopInit = child->getLoopInfo()->getForStmtInitRaw();
					string loopLimit = child->getLoopInfo()->getForStmtLimitRaw();
					tempStream << ", " << loopInit << ", " << loopLimit;
				}

				tempStream
					<< child->getRegion()->mainNode->printOMPInputNamesFromSequential(true)<< "));\n"
					<< "}\n";

			} else if (SWARM_BACKEND){
				tempStream << child->getRegion()->mainNode->packOMPInputContextFromSequential_swarm();
			
				if(child->getLoopInfo()){
					tempStream << "initIteration" << child->getRegion()->getID() << " = "
						<< child->getLoopInfo()->getForStmtInitRaw() << ";\n";
					
					tempStream << "lastIteration" << child->getRegion()->getID() << " = "
						<< child->getLoopInfo()->getForStmtLimitRaw() << ";\n";
				}
				
				tempStream 	<< "numThreads" << child->getRegion()->getID() << " = ompNumThreads;\n"
					<< "swarm_callInto(mySWARMruntime, "
					<<"&CODELET(" << child->getRegion()->getName()
					<< child->getRegion()->getID() << "Entry), "
					<< "NULL, NULL);\n";
			}

			codeStr += tempStream.str();
		}
		else if (isStmtPrintable(child->getStmt())) {
			/* for stmt (not an ompfor stmt) */
			if (check_isa < ForStmt > (child->getStmt())
				&& child->isComplexStmt) {
				/* print the for() string */
				codeStr += "for(";

				if (child->myLoopInfo->initNode)
					printSequentialNode(child->myLoopInfo->initNode,
								codeStr, Policy);
				else
					codeStr += ";";

				if (child->myLoopInfo->condNode)
					printSequentialNode(child->myLoopInfo->condNode,
								codeStr, Policy);
				else
					codeStr += ";";

				if (child->myLoopInfo->incNode)
					printSequentialNode(child->myLoopInfo->incNode,
								codeStr, Policy, false);

				codeStr += ")";

				/* check if there's a body */
				if (child->myNodes.size() > 0) {
					codeStr += "{\n";
					printSequentialDFG(child->myNodes.front(), codeStr,
							   Policy);
					codeStr += "}\n";
				} else {
					codeStr += ";";
				}
			}
			/* while stmt */
			else if (check_isa < WhileStmt > (child->getStmt())
				 && child->isComplexStmt) {
				/* print the while() string */
				codeStr += "while(";
				if (child->myLoopInfo->condNode) {
					printSequentialNode(child->myLoopInfo->condNode,
								codeStr, Policy, false);
				}
				codeStr += ")";

				/* check if there's a body */
				if (child->myNodes.size() > 0) {
					codeStr += "{\n";
					printSequentialDFG(child->myNodes.front(), codeStr,
							   Policy);
					codeStr += "}\n";
				} else {
					codeStr += ";";
				}
			}
			/* do stmt */
			else if (check_isa < DoStmt > (child->getStmt())
				 && child->isComplexStmt) {
				codeStr += "do{";

				/* check if there's a body */
				if (child->myNodes.size() > 0) {
					/* print the body */
					printSequentialNode(child->myNodes.front(), codeStr,
								Policy);
				}
				codeStr += "}";

				/* cond */
				codeStr += "while(";
				if (child->myLoopInfo->condNode) {
					printSequentialNode(child->myLoopInfo->condNode,
								codeStr, Policy, false);
				}
				codeStr += ");";
			}
			/*FIXME: We should verify we should change node for child here */
			else if (check_isa < ForStmt > (node->getStmt()) ||
				 check_isa < OMPParallelForDirective > (node->getStmt()) ||
				 check_isa < OMPForDirective > (node->getStmt())
				) {
				break;
			} else if (check_isa < CompoundStmt > (child->getStmt())) {
				/* Dont print brackets for the function's body, they are printed
				 * by default.*/
				if (node->getFunctionInfo() == nullptr) {
					codeStr += "{\n";
				}

				printSequentialDFG(child, codeStr, Policy);

				if (node->getFunctionInfo() == nullptr) {
					codeStr += "}\n";
				}
			} else if (check_isa < ReturnStmt > (child->getStmt()) && SWARM_BACKEND){

				DFGNode *parentNode = node->findFunctionParent();
				if(parentNode && parentNode->getFunctionInfo() &&
					parentNode->getFunctionInfo()->f->isMain()){
					codeStr += "/*Shutdown the runtime*/\n";
					codeStr += "swarm_shutdownRuntime(mySWARMruntime);\n";
				}

				printSequentialNode(child, codeStr, Policy);

			} else {
				printSequentialNode(child, codeStr, Policy);
			}
		} else {
			printSequentialDFG(child, codeStr, Policy);
		}
		child = child->next;
	}
}

/* Print a sequential DFG's node, i.e. being printed inside a sequential
* function instead of being printed in a TP.*/
void ompASTVisitor::printSequentialNode(DFGNode * node, string & codeStr,
					PrintingPolicy & Policy, bool printSemicolon)
{
	string ostreamString;
	raw_string_ostream rawStringOStream(ostreamString);

	node->getStmt()->printPretty(rawStringOStream, nullptr, Policy);

	codeStr += rawStringOStream.str();

	if (check_isa < Expr > (node->getStmt()) && printSemicolon)
		codeStr += ";\n";
}

void ompASTVisitor::printForStmtToFireString(DFGNode * node,
		DFGNode *parentNode, TPRegion *parentRegion,
		std::string & codeStr, bool printSemiColon)
{
	string initStr = "";		
	string loopStmtStr = "";
	string loopBodyStr = "";

	bool declaredLocally = false;
	if ((node->parent
		 && (check_isa < OMPParallelForDirective > (node->parent->getStmt())
		 || check_isa < OMPForDirective > (node->parent->getStmt()))) ==
		false){
	
		/*Let's print the init outside the for stmt to support loops with more
		than one declaration or init.*/
		/* var init */
		if (node->myLoopInfo->initNode){
			initStr += "/*Loop's init*/\n";
			this->printNodeToFireString(node->getLoopInfo()->initNode, initStr, true);
		}
						
		/*To improve performance, let's create a temp loop var if the loop var
		is not declared locally.*/
		TPInputs_t::iterator it = parentNode->ompVariables.find(node->getLoopInfo()->loopVar);
		if(it != parentNode->ompVariables.end())
			declaredLocally = get<5>(it->second);
		
		if(node->getLoopInfo()->transformedLoopVarStr.find("inputsTPParent") != string::npos ||
			node->getLoopInfo()->transformedLoopVarStr.find(suffixLang) != string::npos){
			if(declaredLocally == false){
				node->getLoopInfo()->localLoopVarStr = node->getLoopInfo()->loopVar;
				node->getLoopInfo()->localLoopVarStr += suffixLang;
				node->getLoopInfo()->localLoopVarStr += "_counter_temp";
				node->getLoopInfo()->localLoopVarStr += to_string(parentRegion->getID());
			
				ostringstream tempInitStream;
				tempInitStream << node->getLoopInfo()->loopVarType << " "
					<< node->getLoopInfo()->localLoopVarStr << " = "
					<< node->getLoopInfo()->transformedLoopVarStr << ";\n";
				initStr += tempInitStream.str();
			}
		}

		// print the for() string
		loopStmtStr += "for(;";

		/* cond */
		if (node->myLoopInfo->condNode)
			this->printNodeToFireString(node->myLoopInfo->condNode,
						loopStmtStr, false);

		loopStmtStr += ";";

		/* incr */
		if (node->myLoopInfo->incNode)
			this->printNodeToFireString(node->myLoopInfo->incNode,
						loopStmtStr, false);

		loopStmtStr += ")";
	}
	
	if (node->myNodes.size() > 0) {
		if(check_isa<CompoundStmt>(node->myNodes.front()->getStmt()) == false)
			loopBodyStr += "{\n";
		this->printNodeToFireString(node->myNodes.front(), loopBodyStr);
		if(check_isa<CompoundStmt>(node->myNodes.front()->getStmt()) == false)
			loopBodyStr += "}\n";
	} else {
		loopBodyStr += ";";
	}

	/*Organize the loop*/
	if ((node->parent && 
		(check_isa < OMPParallelForDirective > (node->parent->getStmt()) ||
		check_isa < OMPForDirective > (node->parent->getStmt()))) == false){

		/*To avoid problems with contiguous loops
		declaring the same loop variables, we are enclosing the loop in
		a compound stmt. */
		if(check_isa<CompoundStmt>(node->myNodes.front()->getStmt()) == false)
			codeStr += "{\n";
		
		/*Create pointers for the variables used inside the loop*/
		std::ostringstream ptrsStream;
		ostringstream changedStmtStream;
		changedStmtStream << loopStmtStr;
		ostringstream changedBodyStream;
		changedBodyStream << loopBodyStr;
		
		DFGNode *parentStmt = node->parent;
		while(parentStmt && check_isa<CompoundStmt>(parentStmt->getStmt())){
			parentStmt = parentStmt->parent;
		}

		if(parentRegion->isInlinedRegion() == false && node->isRegionNode){
			vector< std::string > varNamesToDeclare;
			vector < std::string > varNamesToChange;
			vector < std::string > varTypesToChange;
			vector < std::string > dereferenceSymbol;

			string stmtStr = changedStmtStream.str();
			string bodyStr = changedBodyStream.str();
			node->getLoopInfo()->findVarsToChange(varNamesToDeclare, varNamesToChange,
					varTypesToChange, dereferenceSymbol,
					parentRegion, parentNode, ptrsStream, stmtStr, bodyStr);

			node->getLoopInfo()->replaceVarsForPtrs(varNamesToDeclare, varNamesToChange,
						dereferenceSymbol, node, changedStmtStream);

			node->getLoopInfo()->replaceVarsForPtrs(varNamesToDeclare, varNamesToChange,
						dereferenceSymbol, node, changedBodyStream);
		} 
		else {
			if(node->getLoopInfo()->localLoopVarStr.compare("") != 0 && 
				node->getLoopInfo()->transformedLoopVarStr.find(suffixLang) != string::npos){
				node->getLoopInfo()->replaceLoopVarForLocal(node, changedStmtStream);
				node->getLoopInfo()->replaceLoopVarForLocal(node, changedBodyStream);
			}
		}
		
		codeStr += ptrsStream.str();
		codeStr += initStr;
		codeStr += changedStmtStream.str();
		codeStr += changedBodyStream.str();
	}
	else 
	{
		codeStr += loopBodyStr;
	}

	/*Copy the value in the local var back to the loop var in case is needed
	after the loop.*/
	if ((node->parent && 
		(check_isa < OMPParallelForDirective > (node->parent->getStmt()) ||
		check_isa < OMPForDirective > (node->parent->getStmt()))) == false){

		if(node->getLoopInfo()->transformedLoopVarStr.find("inputsTPParent") != string::npos ||
			node->getLoopInfo()->transformedLoopVarStr.find("darts") != string::npos){
			if(declaredLocally == false){
				ostringstream tempCopyBackStream;
				tempCopyBackStream << node->getLoopInfo()->transformedLoopVarStr
					<< " = " << node->getLoopInfo()->localLoopVarStr << ";\n";
				codeStr += tempCopyBackStream.str();
			}
		}

		if(check_isa<CompoundStmt>(node->myNodes.front()->getStmt()) == false)
			codeStr += "}\n"; 
	}
}

/* Print a DFG's node inside a TP.*/
void ompASTVisitor::printNodeToFireString(DFGNode * node, std::string & codeStr, bool printSemiColon)
{
	DFGNode *parentNode = node;
	if (check_isa < OMPExecutableDirective > (parentNode->getStmt()) == false)
		parentNode = node->findParentNode();

	TPRegion *parentRegion = node->getRegion()->findOMPOrFunctionRegionParent();

	/* create the for() loop for an OMPFor*/
	if (node->isRegionMainNode){
		if(node->getRegion() && node->getRegion()->getParent()){
			TPRegion *tmpParentRegion = node->getRegion()->getParent();
			if (tmpParentRegion->isOMPFor() && tmpParentRegion->getLoopInfo()){
				PrintingPolicy Policy = context->getPrintingPolicy();
				if (parentNode && parentRegion)
					this->setPrintingPolicyParameters(node, Policy);
				tmpParentRegion->getLoopInfo()->getForStmtStrings(
					tmpParentRegion->getMainNode(), Policy.prefixStr, Policy.postfixStr);
			}
		}
	} else if (node->getLoopInfo()) {
		PrintingPolicy Policy = context->getPrintingPolicy();
		if (parentNode && parentRegion)
			this->setPrintingPolicyParameters(node, Policy);
		node->getLoopInfo()->getForStmtStrings(node, Policy.prefixStr, Policy.postfixStr);
	}

	/*Print according to the type of stmt */
	
	if (node->criticalDirectiveName.empty() == false
		&& node->criticalDirectiveLock == true) {
		ostringstream extraStream;

		if(DARTS_BACKEND)
			extraStream << "TP" << node->criticalDirectiveName << "mutex.lock();\n";
		else if (SWARM_BACKEND)
			extraStream << "swarm_spinlock_acquire(Codelet" << node->criticalDirectiveName << "mutex);\n";

		codeStr += extraStream.str();
	}

	/* Compound stmt */
	if (check_isa < CompoundStmt > (node->getStmt())) {
		codeStr += "{\n";
		DFGNode *childNode = node->myNodes.front();
		while (childNode) {
			this->printNodeToFireString(childNode, codeStr);
			childNode = childNode->next;
		}
		codeStr += "}\n";
		goto exit_fn;
	}
	/* atomics */
	else if (isa < AtomicNode > (node)) {
		PrintingPolicy Policy = context->getPrintingPolicy();

		if (parentNode && parentRegion) {
			this->setPrintingPolicyParameters(node, Policy);
		}

		AtomicNode *atomicNode = dyn_cast < AtomicNode > (node);
		if (atomicNode) {
			ostringstream outputStream;
			atomicNode->printCode(this->context, Policy, outputStream);
			codeStr += outputStream.str();
		}
		goto exit_fn;
	}
	/* nullstmt */
	else if (check_isa < NullStmt > (node->getStmt())) {
		ostringstream tempStream;
		if(node->nullStmtAction == NULLSTMT_ALLOCA){
			DFGNode* parentFunction = node->findFunctionParent();
			if(parentFunction){
				string varName = node->nullStmtActionVar;
				tempStream << "/*Variable " << varName << " originally allocated using alloca(). "
					"Store its pointer to delete it when function "
					<< parentFunction->getName() << " is destroyed.*/\n";
				
				PrintingPolicy Policy = context->getPrintingPolicy();

				if (parentNode && parentRegion)
					this->setPrintingPolicyParameters(node, Policy);
				
				tempStream << "void* topFunctionPtr = topFunctionStack(this->getLocalID());\n"
					<< "if(topFunctionPtr){\n"
					<< "TP_" << parentFunction->getName() << " *functionPtr = (TP_"
					<< parentFunction->getName() << "*)topFunctionPtr;\n"
					<< "functionPtr->allocaVars.push_back((void*)"
					<< Policy.prefixStr << varName << suffixLang
					<< parentRegion->getID() << Policy.postfixStr
					<< ");\n"
					<< "}\n";
			}
		}
		codeStr += tempStream.str();
		goto exit_fn;
	}
	/* break stmt */
	else if (check_isa < BreakStmt > (node->getStmt())) {
		DFGNode *breakParentNode = node->parent;
		while (breakParentNode != nullptr &&
			   check_isa < WhileStmt > (breakParentNode->getStmt()) == false &&
			   check_isa < DoStmt > (breakParentNode->getStmt()) == false &&
			   check_isa < ForStmt > (breakParentNode->getStmt()) == false &&
			   check_isa < SwitchStmt > (breakParentNode->getStmt()) == false) {
			breakParentNode = breakParentNode->parent;
		}

		/*If the break stmt belongs to a complex while/do/for/switch stmt, then
		 * signal the next region after that stmt. Otherwise, just print the break;*/
		if (breakParentNode && breakParentNode->getLoopInfo()
			&& breakParentNode->isLoopNode() && breakParentNode->isComplexStmt) {
			LoopCondNode *condNode = breakParentNode->getLoopInfo()->condNode;
			ostringstream tempStream;

			tempStream << "//break\n";
			condNode->endCondLoopNode->signalNext(tempStream,
								  "myTP->controlTPParent->TPParent->");
			tempStream << "return;\n";
			codeStr += tempStream.str();
			goto exit_fn;
		}
	}
	/* if stmt */
	else if (check_isa < IfStmt > (node->getStmt()) && node->myBranchInfo->isSimpleIf
		 && node->myNodes.size() >= 1) {
		/* an if stmt should have only two children: the if body and the else body.
		if the else is an else-if, then the second child (the else) is an if stmt */

		codeStr += "if(";
		this->printNodeToFireString(node->myBranchInfo->condNode, codeStr, false);
		codeStr += ")\n";

		/* the if body */
		DFGNode *childNode = node->myNodes.front();
		if(check_isa<CompoundStmt>(childNode->getStmt()) == false)
			codeStr += "{\n";
		this->printNodeToFireString(childNode, codeStr);
		if(check_isa<CompoundStmt>(childNode->getStmt()) == false)
			codeStr += "}\n";

		childNode = childNode->next;

		/* the else body */
		if (childNode) {
			if (check_isa < IfStmt > (childNode->getStmt())) {
				codeStr += "else\n";
				this->printNodeToFireString(childNode, codeStr);
			} else {
				codeStr += "else\n";
				if(check_isa<CompoundStmt>(childNode->getStmt()) == false)
					codeStr += "{\n";
				this->printNodeToFireString(childNode, codeStr);
				if(check_isa<CompoundStmt>(childNode->getStmt()) == false)
					codeStr += "}\n";
			}
		}
		goto exit_fn;
	}
	/*for stmt:
	This generates the code for a simple for stmt and the body of an OMP for.
	TODO: This should be written differently so both types of for stmts are
	handled separately.
	*/
	else if (check_isa < ForStmt > (node->getStmt()) && node->isComplexStmt == false) {
		/*don't print the for stmt associated with the omp for directive since it is
		   printed in the checkincodelet */
		if ((node->parent &&
			(check_isa < OMPParallelForDirective > (node->parent->getStmt()) ||
			check_isa < OMPForDirective > (node->parent->getStmt()))) == false)
			codeStr += "{\n";	

		this->printForStmtToFireString(node, parentNode, parentRegion,
			codeStr, printSemiColon);
			
		if ((node->parent &&
			(check_isa < OMPParallelForDirective > (node->parent->getStmt()) ||
			check_isa < OMPForDirective > (node->parent->getStmt()))) == false)
			codeStr += "}\n";
		goto exit_fn;
	}
	/* while stmt */
	else if (check_isa < WhileStmt > (node->getStmt()) && node->isComplexStmt == false) {
		/* print the while() string */
		codeStr += "while(";

		/* cond */
		if (node->myLoopInfo->condNode) {
			this->printNodeToFireString(node->myLoopInfo->condNode, codeStr,
							false);
		}
		codeStr += ")";

		/* check if there's a body */
		if (node->myNodes.size() > 0) {
			/* print the body */
			this->printNodeToFireString(node->myNodes.front(), codeStr);
		} else {
			codeStr += ";";
		}
		goto exit_fn;
	}
	/* do stmt */
	else if (check_isa < DoStmt > (node->getStmt()) && node->isComplexStmt == false) {
		codeStr += "do{";

		/* check if there's a body */
		if (node->myNodes.size() > 0) {
			/* print the body */
			this->printNodeToFireString(node->myNodes.front(), codeStr);
		}
		codeStr += "}";

		/* cond */
		codeStr += "while(";
		if (node->myLoopInfo->condNode) {
			this->printNodeToFireString(node->myLoopInfo->condNode, codeStr,
							false);
		}
		codeStr += ");";
		goto exit_fn;
	}
	/* loopnode - complex */
	else if (node->isLoopNode() && node->isRegionMainNode && node->isComplexStmt) {
		ostringstream outputStream;

		outputStream << "bool haveToLaunch = "
			<< "__sync_bool_compare_and_swap(&(myTP->controlTPParent->TP" << node->getID()
			<< "_LoopCounter), myTP->controlTPParent->TP" << node->getID()
			<< "_LoopCounterPerThread[this->getID ()], "
			<< "myTP->controlTPParent->TP" << node->getID()
			<< "_LoopCounterPerThread[this->getID ()] + 1);\n"
			<< "unsigned int iterIdx = myTP->controlTPParent->TP" << node->getID()
			<< "_LoopCounterPerThread[this->getID ()];\n"
			<< "if (haveToLaunch)\n"
			<< "{\n"
			<< "this->resetCodelet(); \n"
			<< "myTP->controlTPParent->TP" << node->getID()
			<< "PtrVec.push_back(nullptr);\n"
			<< "myTP->controlTPParent->TP" << node->getID()
			<< "_LoopCounterPerThread[this->getID ()] += 1;\n"
			<< "invoke < TP" << node->getID()
			<< " > (myTP, myTP->numThreads, this->getID(), myTP, "
			<< "myTP->inputsTPParent, &(myTP->controlTPParent->TP" << node->getID()
			<< "PtrVec.back()));\n"
			<< "}\n"
			<< "else\n"
			<< "{\n"
			<< "if(myTP->controlTPParent->TP" << node->getID()
			<<"PtrVec.size () == 0)\n"
			<< "{\n"
			<< "this->resetCodelet ();\n"
			<< "this->decDep ();\n"
			<< "return;\n"
			<< "} else if (myTP->controlTPParent->TP" << node->getID()
			<< "PtrVec.size () < (iterIdx + 1))\n"
			<< "{\n"
			<< "this->resetCodelet ();\n"
			<< "this->decDep ();\n"
			<< "return;\n"
			<< "} else if (myTP->controlTPParent->TP" << node->getID()
			<< "PtrVec[iterIdx] == nullptr)\n"
			<< "{\n"
			<< "this->resetCodelet ();\n"
			<< "this->decDep ();\n"
			<< "return;\n"
			<< "}\n"
			<< "else\n"
			<< "{\n"
			<< "this->resetCodelet ();\n"
			<< "#if USE_SPIN_CODELETS == 0\n"
			<< "myTP->controlTPParent->TP" << node->getID()
			<< "PtrVec[iterIdx]->firstCodelet[this->getID ()].decDep();\n"
			<< "#else\n"
			<< "myTP->controlTPParent->TP" << node->getID()
			<< "PtrVec[iterIdx]->availableCodelets[this->getID ()] = 1;\n"
			<< "#endif\n"
			<< "myTP->controlTPParent->TP" << node->getID()
			<< "_LoopCounterPerThread[this->getID ()] += 1;\n"
			<< "}\n"
			<< "}\n";

		node->fireCodeStr += outputStream.str();
		goto exit_fn;
	}
	/* decl stmt with init */
	else if (check_isa < DeclStmt > (node->getStmt())) {
		PrintingPolicy Policy = context->getPrintingPolicy();

		if (parentNode && parentRegion)
			this->setPrintingPolicyParameters(node, Policy);

		string declInitString = this->getInitValueForDeclStmt(node, Policy);
		codeStr += declInitString;
		goto exit_fn;
	}
	/* return stmt */
	else if (check_isa < ReturnStmt > (node->getStmt())) {
		codeStr += "/*return*/\n";
		DFGNode *parentFunction = node->findFunctionParent();
		
		if (parentFunction && parentFunction->getName().compare("main") == 0) {
			codeStr += "RuntimeFinalCodelet->decDep();\n";
		} else {
			ReturnStmt *retStmt = dyn_cast < ReturnStmt > (node->getStmt());
			Expr *retValue = retStmt->getRetValue();
			DFGNode *functionParentNode = node->findFunctionParent();
			if (retValue && functionParentNode &&
				functionParentNode->myFunctionInfo->isOMPDARTSFunction(functionTable)) {

				ostringstream tempStream;
				string ostreamString;

				raw_string_ostream rawStringOStream(ostreamString);
				PrintingPolicy Policy = context->getPrintingPolicy();

				if (parentNode && parentRegion) {
					this->setPrintingPolicyParameters(node, Policy);
				}

				retValue->printPretty(rawStringOStream, nullptr, Policy);

				if(DARTS_BACKEND)
					tempStream << "*(" << Policy.prefixStr << "functionResult"
						<< Policy.postfixStr << ")= " << rawStringOStream.str() << ";\n";
				else if (SWARM_BACKEND)
					tempStream << "*(" << Policy.prefixStr << "functionResult"
						<< parentFunction->getName() << Policy.postfixStr << ") = "
						<< rawStringOStream.str() << ";\n";
				codeStr += tempStream.str();
			}
		}
		
		if (parentFunction && parentFunction->getName().compare("main") != 0) {
			
			string ostreamString;
			raw_string_ostream rawStringOStream(ostreamString);
			PrintingPolicy Policy = context->getPrintingPolicy();
			if (parentNode && parentRegion)
				this->setPrintingPolicyParameters(node, Policy);
			
			ostringstream tempStream;
			if(DARTS_BACKEND){
				if (parentRegion->singleThreaded) {
					tempStream << "myTP->controlTPParent->nextCodelets" << parentFunction->getName()
						<< "->decDep();\n";
					tempStream << "return;\n";
				} else {
					tempStream  << "myTP->controlTPParent->nextCodelets" << parentFunction->getName()
						<< Policy.postfixStr << "->decDep();\n";
					tempStream  << "return;\n";
				}
			} else if (SWARM_BACKEND){
				asm("nop");
			}
			codeStr += tempStream.str();
		}
		goto exit_fn;
	}
	/* calling a function */
	else if (node->isCallerNode()) {
		CallExpr *callExpr = dyn_cast < CallExpr > (node->getStmt());
		if (callExpr) {
			
			ostringstream outputStream;
			
			if(DARTS_BACKEND){
				if (parentRegion->singleThreaded == false)
					outputStream << "if (! __sync_val_compare_and_swap(&(myTP->controlTPParent->TP"
						<< node->getID() << "_alreadyLaunched), 0, 1))\n"
						<< "{\n";

				string nextCodeletName;
				string nextSyncCodeletName;
				ostringstream tempOutputStream;

				tempOutputStream << "/*Finding the next codelet after the function call*/\n";
				tempOutputStream << "#if 0\n";
				nextCodeletName = node->signalNext(tempOutputStream, "myTP->controlTPParent->",
				"this->getID()", false, true, false);
				tempOutputStream << "#endif\n";
				
				tempOutputStream << "/*Finding the next sync point before making the function call*/\n";
				tempOutputStream << "#if 0\n";
				nextSyncCodeletName = node->signalNext(tempOutputStream, "myTP->controlTPParent->",
				"this->getID()", false, true, true);
				tempOutputStream << "#endif\n";
				
#if FULL_DEBUG_COMMENTS == 1
				outputStream << tempOutputStream.str();
#endif
				outputStream << "/*Make the function call*/\n";
				outputStream << "invoke < TP_" << node->calleeFunctionNodeName
					<< " > (myTP, myTP->numThreads, this->getID()";
					
				outputStream << ", ";
				if (nextCodeletName.find("nextCodelets") == string::npos)
					outputStream << "&";
				outputStream << " (" << nextCodeletName << ") ";

				outputStream << ", ";
				if (nextSyncCodeletName.find("nextSyncCodelet") == string::npos)
					outputStream << "&";
				outputStream << " (" << nextSyncCodeletName << ") ";

				if (parentRegion->singleThreaded == false)
					outputStream << " , &(myTP->controlTPParent->TP" << node->getID() << "Ptr)";
				else
					outputStream << " , nullptr";

				string ostreamString;
				raw_string_ostream rawStringOStream(ostreamString);
				PrintingPolicy Policy = context->getPrintingPolicy();

				if (parentNode && parentRegion)
					this->setPrintingPolicyParameters(node, Policy);

				/* print arguments */
				Expr **args = callExpr->getArgs();
				int numArgs = callExpr->getNumArgs();
				for (int argCounter = 0; argCounter < numArgs; argCounter++) {
					rawStringOStream.str() += ", ";
					args[argCounter]->printPretty(rawStringOStream, nullptr, Policy);
				}
				outputStream << rawStringOStream.str();

				/* add tmp var if the callexpr was originally moved to this
				position by VisitCallExpr */
				FunctionTable_t::iterator it = functionTable.find(node->calleeFunctionNodeName);
				if (it != functionTable.end()
					&& get < 0 > (it->second).compare("void") != 0
					&& get < 1 > (it->second)) {
					outputStream << ", &(";
					OMP_VARSCOPE varScope = node->getVariableScope(node->calleeTmpVarName);
					if (varScope == OMP_SHARED) {
						outputStream << "*";
					}
					string tmpVarName = node->calleeTmpVarName;
					string tmpVarPostfix = "";
					size_t pos = tmpVarName.find("[");
					if(pos != string::npos){
						size_t pos1 = tmpVarName.find("]");
						tmpVarPostfix = tmpVarName.substr(pos + 1, pos1 - pos - 1);
						tmpVarName = tmpVarName.substr(0, pos);
					}

					ostringstream tmpVarNameStream;
					tmpVarNameStream << "";
					VarTableTuple_t varTuple;
					bool found = this->myDFG.getVarTupleFromTable(node, tmpVarName, varTuple);
					if (found){	
						/*Check if the variable is global*/
						if (get < 3 > (varTuple)){
							tmpVarNameStream << tmpVarName;
						} else {
							tmpVarNameStream << Policy.prefixStr << tmpVarName << suffixLang
								<< Policy.darts_TPID << Policy.postfixStr;
						}
					} else {
						tmpVarNameStream << tmpVarName;
					}
					
					ostringstream tmpVarPostfixStream;
					tmpVarPostfixStream << "";
					if(tmpVarPostfix.compare("") != 0){
						VarTableTuple_t varTuplePostfix;
						bool foundPostfix = this->myDFG.getVarTupleFromTable(node, tmpVarPostfix, varTuplePostfix);
						if(foundPostfix){
							if (get < 3 > (varTuple)){
							tmpVarPostfixStream << "[" << tmpVarPostfix << "]";
							} else {
								tmpVarPostfixStream << "[" << Policy.prefixStr << tmpVarPostfix << suffixLang
									<< Policy.darts_TPID << Policy.postfixStr << "]";
							}
						} else {
							tmpVarPostfixStream << "[" << tmpVarPostfix << "]";
						}
					}
					
					outputStream << "(" << tmpVarNameStream.str() << tmpVarPostfixStream.str() << ")";
					outputStream << ")";
				}

				outputStream << ");\n";
				if (parentRegion->singleThreaded == false) {
					outputStream << "}\n"
						<< "else\n"
						<< "{\n"
						<< "if(myTP->controlTPParent->TP" << node->getID() << "Ptr == nullptr)\n"
						<< "{\n"
						<< "myTP->add(this); \n"
						<< "return;\n"
						<< "}\n"
						<< "else\n"
						<< "{\n";

					string ostreamStringNewInputs;
					raw_string_ostream rawStringOStreamNewInputs(ostreamStringNewInputs);

					/* pass parameters and result pointer for current codelet */
					if ((numArgs > 0)
						|| (it != functionTable.end()
						&& get < 0 > (it->second).compare("void") != 0
						&& get < 1 > (it->second))) {
						outputStream << "myTP->controlTPParent->TP" << node->getID() << "Ptr->setNewInputs(\n";

						/* print arguments */
						Expr **args = callExpr->getArgs();
						int numArgs = callExpr->getNumArgs();
						if (numArgs > 0) {
							int argCounter = 0;
							args[argCounter]->printPretty(rawStringOStreamNewInputs, nullptr, Policy);
							argCounter++;

							for (; argCounter < numArgs; argCounter++) {
								rawStringOStreamNewInputs.str() += ", ";
								args[argCounter]->printPretty(rawStringOStreamNewInputs, nullptr, Policy);
							}
							outputStream << rawStringOStreamNewInputs.str();
						}
						/* add tmp var if the callexpr was originally moved to this
						position by VisitCallExpr */
						if (it != functionTable.end()
							&& get < 0 > (it->second).compare("void") != 0
							&& get < 1 > (it->second)) {
							outputStream << ", &(";
							OMP_VARSCOPE varScope = node->getVariableScope(node->calleeTmpVarName);
							if (varScope == OMP_SHARED) {
								outputStream << "*";
							}
							outputStream << "(" << Policy.prefixStr <<
								node->calleeTmpVarName << "_darts"
									<< Policy.darts_TPID << Policy. postfixStr << "))";
						}

						outputStream << ", this->getID());\n";
					}

					outputStream << "myTP->controlTPParent->TP" << node->getID()
						<< "Ptr->nextCodelets" << node->calleeFunctionNodeName
						<< "[this->getID()] = ";
					if (nextCodeletName.find("nextCodelets") == string::npos)
						outputStream << "&";
					outputStream << "(" << nextCodeletName << ");\n";
					
					outputStream << "myTP->controlTPParent->TP" << node->getID()
						<< "Ptr->nextSyncCodelets" << node->calleeFunctionNodeName
						<< "[this->getID()] = ";
					if (nextSyncCodeletName.find("nextSyncCodelet") == string::npos)
						outputStream << "&";
					outputStream << "(" << nextSyncCodeletName << ");\n";
					
					if(this->myDFG.useFunctionStack)
						outputStream << "pushFunctionStack(this->getLocalID(), "
							<< "(void*)myTP->controlTPParent->TP" << node->getID() << "Ptr);\n";

					outputStream << "#if USE_SPIN_CODELETS == 0\n"
						<< "myTP->controlTPParent->TP" << node->getID()
						<< "Ptr->firstCodelet[this->getID()].decDep();\n"
						<< "#else\n"
						<< "myTP->controlTPParent->TP" << node->getID()
						<< "Ptr->availableCodelets[this->getID()] = 1;\n"
						<< "#endif\n"
						<< "}\n"
						<< "}\n";
				}
			} else if (SWARM_BACKEND){

				PrintingPolicy Policy = context->getPrintingPolicy();

				if (parentNode && parentRegion)
					this->setPrintingPolicyParameters(node, Policy);
				ostringstream targetRegionName;
				targetRegionName << node->calleeFunctionNodeName << "Entry";

				/* add tmp var if the callexpr was originally moved to this
				position by VisitCallExpr */
				FunctionTable_t::iterator it = functionTable.find(node->calleeFunctionNodeName);
				if (it != functionTable.end()
					&& get < 0 > (it->second).compare("void") != 0
					&& get < 1 > (it->second)) {
						
					if (node->calleeFunctionNodeIsSingleThreaded){
						outputStream << "functionResult" << node->calleeFunctionNodeName
							<< "[codeletID] = ";
					} else {
						outputStream << "functionResult" << node->calleeFunctionNodeName
							<< "[codeletID] = ";
					}
					
					outputStream << "&(";
					OMP_VARSCOPE varScope = node->getVariableScope(node->calleeTmpVarName);
					if (varScope == OMP_SHARED) {
						outputStream << "*";
					}
					
					outputStream << "(" << Policy.prefixStr << node->calleeTmpVarName
						<< suffixLang << Policy.darts_TPID << Policy.postfixStr << "));\n";
				}
				
				outputStream << "currentNumThreads[codeletID] = "
					<< "numThreads" <<  parentRegion->getID() << ";\n";
					
				outputStream << "Codelet" << node->calleeFunctionNodeName
					<< "Entry_alreadySet = &(Codelet" << node->calleeFunctionNodeName
					<< "Entry_alreadySetFrom_Codelet" << node->getID() << ");\n";

				myFunctions_t::iterator itCalleeName = this->myDFG.myFunctions.find(node->calleeFunctionNodeName);
				if(itCalleeName != this->myDFG.myFunctions.end()){
					
					DFGNode *functionNode = get < 0 > (itCalleeName->second);
					
					outputStream << "/*Calling " << node->calleeFunctionNodeName
						<< ", functionNode: " << functionNode->getID()
						<< " " << functionNode->getName() << "*/\n";
					
					if(functionNode){
						PrintingPolicy PolicyArg = context->getPrintingPolicy();
						functionNode->getFunctionInfo()->printPassFuncParams_swarm(outputStream, 
							node, "[codeletID]", PolicyArg, callExpr);
					}
				}
				
				string nextCodeletName;
				ostringstream tempStream;
				nextCodeletName = node->signalNext(tempStream, "", "codeletID", false, true);
				
				ostringstream nextCodeletContext;
				
				/*If we are signaling a barrier after the function call, put it in the context
				so we know we need to decrease deps instead of scheduling a codelet.*/
				if(nextCodeletName.find("barrier") != string::npos){
					nextCodeletContext << "&" << nextCodeletName << "Dep";
				} else {
					nextCodeletContext << "NULL";
				}
			
				outputStream << "swarm_schedule(&CODELET(" << targetRegionName.str() << "), "
					<< "(void *) codeletID, NULL, &CODELET(" << nextCodeletName << "), "
					<< "(void *)" << nextCodeletContext.str() << ");\n";
			}
			node->fireCodeStr += outputStream.str();
		}
		goto exit_fn;
	}

	{
		string ostreamString;
		raw_string_ostream rawStringOStream(ostreamString);
		PrintingPolicy Policy = context->getPrintingPolicy();

		if (parentNode && parentRegion) {
			this->setPrintingPolicyParameters(node, Policy);
		}

		if(node->getStmt())
			node->getStmt()->printPretty(rawStringOStream, nullptr, Policy);

		if (rawStringOStream.str().find(".copyprivate.dst") != string::npos) {
			codeStr += "/*copyprivate here*/";
		}

		if (printSemiColon && check_isa < Expr > (node->getStmt())
			&& !rawStringOStream.str().empty()) {
			rawStringOStream << ";\n";
		}

		codeStr += rawStringOStream.str();

		/* check if we just printed a call to omp_set_num_threads(int)
		 * and change the num of dependencies of the subsequent parallel
		 * regions, since that was set in the TP's constructor*/
		if (check_isa < CallExpr > (node->getStmt())) {
			CallExpr *s = dyn_cast < CallExpr > (node->getStmt());
			string calleeFunctionName = this->getCalleeFunctionName(s);

			if (calleeFunctionName.compare("omp_set_num_threads") == 0) {
				ostringstream outputStream;

				TPRegion *childRegion = parentRegion->childRegions.front();
				while (childRegion) {
					if (childRegion->isOMPParallel()) {
						outputStream << "myTP->barrierCodelets" <<
							childRegion->getID() <<
							"[0].setDep(ompNumThreads);\n";
					}
					childRegion = childRegion->next;
				}
				codeStr += outputStream.str();
			}
		}
	}

	exit_fn:

	if (node->criticalDirectiveName.empty() == false
		&& node->criticalDirectiveUnlock == true) {
		ostringstream extraStream;
		
		if(DARTS_BACKEND)
			extraStream << "TP" << node->criticalDirectiveName << "mutex.unlock();\n";
		else if (SWARM_BACKEND)
			extraStream << "swarm_spinlock_release(Codelet" << node->criticalDirectiveName << "mutex);\n";
		codeStr += extraStream.str();
	}

	return;
}


void ompASTVisitor::createRegionsNodeCode(TPRegion * region)
{
	for (DFGNode * node:region->myNodes)
		if (!check_isa < OMPExecutableDirective > (node->getStmt()))
			/*Create a string with the DARTS code */
			printNodeToFireString(node, node->fireCodeStr);
}

void ompASTVisitor::printRegionCodeletsClass(TPRegion * region,
						 std::ostringstream & tempStream)
{
	if (region->isBalancedOMPFor() && region->hasBarrier()
		&& region->isOMPForOnlyChildRegion() == false) {
		region->printBarrierCodeletClass(tempStream, true);
	}

	if (region->isOMPParallel()) {
		region->printBarrierCodeletClass(tempStream, true);
	}

	TPRegion *childRegion = region->childRegions.front();
	while (childRegion) {
		if (childRegion->doNotPrint == false) {
			childRegion->printCheckInCodeletClass(tempStream);
		}

		if (check_isa < OMPExecutableDirective > (childRegion->getStmt())
			&& childRegion->hasBarrier()) {
			childRegion->printBarrierCodeletClass(tempStream);
		}

		childRegion = childRegion->next;
	}
}

void ompASTVisitor::printRegionCodeletsFire(TPRegion * region, std::ostringstream & tempStream)
{
	if (region->isBalancedOMPFor() && region->hasBarrier()
		&& region->isOMPForOnlyChildRegion() == false) {
			
		if(DARTS_BACKEND)
			region->printBarrierCodeletFire(tempStream, true);
	}

	if (region->isOMPParallel()) {
		region->printBarrierCodeletFire(tempStream, true);
	}

	TPRegion *childRegion = region->childRegions.front();
	while (childRegion) {
		if (childRegion->doNotPrint == false) {
			childRegion->printCheckInCodeletFire(tempStream);
		}

		if (check_isa < OMPExecutableDirective > (childRegion->getStmt())
			&& childRegion->hasBarrier()) {
			childRegion->printBarrierCodeletFire(tempStream);
		}
		childRegion = childRegion->next;
	}
}


void ompASTVisitor::printRegionCodeletsDefs(TPRegion * region,
						std::ostringstream & tempStream)
{
	if (region->isBalancedOMPFor() && region->hasBarrier()
		&& region->isOMPForOnlyChildRegion() == false) {
			
		if(DARTS_BACKEND)
			region->printBarrierCodeletDef(tempStream);
	}

	if (region->isOMPParallel()) {
		region->printBarrierCodeletDef(tempStream);
	}

	TPRegion *childRegion = region->childRegions.front();
	while (childRegion) {
		if (childRegion->doNotPrint == false) {
			childRegion->printCheckInCodeletDef(tempStream);
		}

		if (check_isa < OMPExecutableDirective > (childRegion->getStmt())
			&& childRegion->hasBarrier()) {
			childRegion->printBarrierCodeletDef(tempStream);
		}
		childRegion = childRegion->next;
	}
}

void ompASTVisitor::printRegionCodeletsInits(TPRegion * region,
						 std::ostringstream & tempStream)
{
	bool printComma = true;

	if (region->isBalancedOMPFor() && region->hasBarrier()
		&& region->isOMPForOnlyChildRegion() == false) {
		region->printBarrierCodeletInit(tempStream, printComma);
	}

	if (region->isOMPParallel()) {
		region->printBarrierCodeletInit(tempStream, printComma);
	}

	TPRegion *childRegion = region->childRegions.front();
	while (childRegion) {
		if (childRegion->doNotPrint == false) {
			childRegion->printCheckInCodeletInit(tempStream,
								 printComma);
			printComma = true;
		}

		if (check_isa < OMPExecutableDirective > (childRegion->getStmt())
			&& childRegion->hasBarrier()) {
			childRegion->printBarrierCodeletInit(tempStream,
								 printComma);
			printComma = true;
		}
		childRegion = childRegion->next;
	}
}

void ompASTVisitor::printRegionCodeletsInitsInConstructor( TPRegion * region, std::ostringstream & tempStream)
{
	if (region->isBalancedOMPFor() && region->hasBarrier()
		&& region->isOMPForOnlyChildRegion() == false) {
		region->printBarrierCodeletCreation(tempStream, true);
	}

	if (region->isOMPParallel()) {
		region->printBarrierCodeletCreation(tempStream, true);
	}

	TPRegion *childRegion = region->childRegions.back();
	while (childRegion) {		
		if (check_isa < OMPExecutableDirective > (childRegion->getStmt())
			&& childRegion->hasBarrier()) {
			childRegion->printBarrierCodeletCreation(tempStream);
		}

		if (childRegion->doNotPrint == false) {
			childRegion->printCheckInCodeletDecDep(tempStream);
		}
		childRegion = childRegion->prev;
	}

	int res = -1;

	childRegion = region->childRegions.back();
	while (childRegion) {
		if (childRegion->doNotPrint == false) {
			childRegion->printCheckInCodeletPtr(tempStream);

			std::ostringstream checkStream;
			res = childRegion->printCheckInCodeletCreation(checkStream);
			assignPerElementToCorrectInitLoop(res, checkStream, region);
		}
		childRegion = childRegion->prev;
	}
}

void ompASTVisitor::printRegionCodeletsDelete(TPRegion * region,
						  std::ostringstream & tempStream)
{
	if (region->isBalancedOMPFor() && region->hasBarrier()
		&& region->isOMPForOnlyChildRegion() == false) {
		region->printBarrierCodeletDelete(tempStream);
	}

	if (region->isOMPParallel()) {
		region->printBarrierCodeletDelete(tempStream);
	}

	TPRegion *childRegion = region->childRegions.back();
	while (childRegion) {
		if (check_isa < OMPExecutableDirective > (childRegion->getStmt())
			&& childRegion->hasBarrier()) {
			childRegion->printBarrierCodeletDelete(tempStream);
		}

		if (childRegion->doNotPrint == false) {
			childRegion->printCheckInCodeletDelete(tempStream);
		}

		childRegion = childRegion->prev;
	}
}

void ompASTVisitor::assignPerElementToCorrectInitLoop(int res,
							  std::ostringstream & inStream,
							  TPRegion * region)
{
	if (res == initInLocalThreadsLoop) {
		region->initLocalThreadsLoopStr += inStream.str();
	} else if (res == initInGlobalThreadsLoop) {
		region->initGlobalThreadsLoopStr += inStream.str();
	}
}

void ompASTVisitor::assignPerElementToCorrectFreeLoop(int res,
							  std::ostringstream & inStream,
							  TPRegion * region)
{
	if (res == initInLocalThreadsLoop) {
		region->freeLocalThreadsLoopStr += inStream.str();
	} else if (res == initInGlobalThreadsLoop) {
		region->freeGlobalThreadsLoopStr += inStream.str();
	}
}

void ompASTVisitor::printRegionExtraVarsDefs(TPRegion * region,
						 std::ostringstream & tempStream)
{
	TPRegion *childRegion = region->childRegions.front();
	while (childRegion) {
		for (DFGNode * node:	childRegion->myNodes) {
			node->printExtraVarsDef(tempStream, this->globalVarsStream);
		}
		childRegion = childRegion->next;
	}
}

void ompASTVisitor::printRegionExtraVarsInits(TPRegion * region, std::ostringstream & tempStream)
{
	TPRegion *childRegion = region->childRegions.front();
	while (childRegion) {
		for (DFGNode * node:	childRegion->myNodes) {
			if(DARTS_BACKEND)
				node->printExtraVarsInit(tempStream);
			else if(SWARM_BACKEND)
				node->printExtraVarsInit_swarm(tempStream);
		}
		childRegion = childRegion->next;
	}
}

void ompASTVisitor::printRegionExtraVarsInitsInConstructor(TPRegion * region, std::ostringstream & tempStream)
{
	TPRegion *childRegion = region->childRegions.front();
	while (childRegion) {
		for (DFGNode * node:	childRegion->myNodes) {
			node->printExtraVarsInitInConstructor(tempStream);
		}
		childRegion = childRegion->next;
	}
}

void ompASTVisitor::printRegionExtraVarsFree(TPRegion * region, std::ostringstream & tempStream)
{
	TPRegion *childRegion = region->childRegions.front();
	while (childRegion) {
		for (DFGNode * node:	childRegion->myNodes) {
			node->printExtraVarsFree(tempStream);
		}
		childRegion = childRegion->next;
	}
}


void ompASTVisitor::printRegion(TPRegion * region, std::string & codeStr)
{
	/*Check if this region is converted into TP and mark it*/
	if (region->getFunctionInfo()) {
		region->isThreadedProcedure = true;
	} else if (
			  (region->childRegions.size() == 0)
			  ||
			  /* dont create TPs for barriers */
			  check_isa < OMPBarrierDirective > (region->getStmt()) ||
			  check_isa < OMPTaskwaitDirective > (region->getStmt()) ||
			  /* dont create TPs for ifStmts */
			  (region->name.compare("IfStmt") == 0
			   && !region->mainNode->myBranchInfo->isSimpleIf)
		) {
		/* do nothing */
		asm("nop");
	} else if (region->isInlinedRegion() == false) {
		region->isThreadedProcedure = true;
	}
	
	/* create the code for the branch */
	if (check_isa < IfStmt > (region->getStmt())
		&& region->getBranchInfo()->isSimpleIf == false) {
		if (region->getBranchInfo()->condNode) {
			this->printNodeToFireString(region->getBranchInfo()->condNode,
							region->getBranchInfo()->conditionString, false);
		}
	}
	/* create the code for the codelets of this region */
	TPRegion *childRegion = region->childRegions.front();
	while (childRegion) {
		createRegionsNodeCode(childRegion);
		childRegion = childRegion->next;
	}

	/* create the task data classes for any node that is a task */
	for (DFGNode * node:region->myNodes) {
		if (check_isa < OMPTaskDirective > (node->getStmt())) {
			node->myTaskInfo->printTaskDataClass(includesStream);
		}
	}

	if (region->getFunctionInfo()) {
		this->printFunctionRegion(region, codeStr);
	} else if (
			  (region->childRegions.size() == 0)
			  ||
			  /* dont create TPs for barriers */
			  check_isa < OMPBarrierDirective > (region->getStmt()) ||
			  check_isa < OMPTaskwaitDirective > (region->getStmt()) ||
			  /* dont create TPs for ifStmts */
			  (region->name.compare("IfStmt") == 0
			   && !region->mainNode->myBranchInfo->isSimpleIf)
		) {
		/* do nothing */
		asm("nop");
	} else if (region->isInlinedRegion() == false) {
		
		if(DARTS_BACKEND){
			includesStream << "class TP" << region->getID() << ";\n";
		} else if (SWARM_BACKEND){
			includesStream << "CODELET_DECL(" << region->getName() << region->getID() << "Entry);\n";
		}

		if(DARTS_BACKEND){
			if (region->isOMPForOnlyChildRegion()) {
				includesStream << "/*Number of TPs and codelets to be used "
					<< "for the OMPFor in region TP" << region->getID() << "*/\n"
					<< "#define NUMTPS" << region->getID() << " NUMTPS\n"
					<< "#define NUMCODELETS" << region->getID() << " (ompNumThreads/NUMTPS)\n"
					<< "extern int dartsTPLoopTH0;\n"
					<< "extern int dartsTPLoopTH1;\n"
					<< "class _checkInCodelets" << region->getID() << "_core;\n"
					<< "class TP" << region->getID() << "_loop;\n"
					<< "class TP" << region->getID() << "_outer;\n";
					
			} else if (region->isBalancedOMPFor()) {
				includesStream << "/*Number of TPs to be used for the OMPFor " << "in region TP"
					<< region->getID() << "*/\n"
					<< "#define NUMTPS" << region->getID() << " NUMTPS\n";
			}
		}

		std::ostringstream TPStream;

		this->printTPDef(region, TPStream);

		codeStr += TPStream.str();
	}

	childRegion = region->childRegions.front();
	while (childRegion) {
		printRegion(childRegion, codeStr);
		childRegion = childRegion->next;
	}
}

void ompASTVisitor::printRegionCode(TPRegion * region, std::string & codeStr)
{
	if (region->getFunctionInfo()) {
		this->printFunctionRegionCode(region, codeStr);
	} else if (
			  (region->childRegions.size() == 0)
			  ||
			  /* dont create TPs for barriers */
			  check_isa < OMPBarrierDirective > (region->getStmt()) ||
			  check_isa < OMPTaskwaitDirective > (region->getStmt()) ||
			  /* dont create TPs for ifStmts */
			  (region->name.compare("IfStmt") == 0
			   && !region->mainNode->myBranchInfo->isSimpleIf)
		) {
		/* do nothing */
	} else if (region->isInlinedRegion() == false) {
		std::ostringstream TPStream;

		if(DARTS_BACKEND){
			TPStream << "/*TP" << region->getID() << ": " << region->getName() << "*/\n";
		} else if(SWARM_BACKEND){
			TPStream << "/*Codelet" << region->getID() << ": " << region->getName() << "*/\n";
		}

		this->printRegionCodeletsFire(region, TPStream);

		/* inline functions */
		/*print any functions the inlined regions need*/
		for (TPRegion * inlinedChildRegion:region->inlinedRegions) {
			/*if the inlined region is an ompfor,
			print the iterationrequest function implementation*/
			if (inlinedChildRegion->isOMPFor()) {
				inlinedChildRegion->getLoopInfo()->printRequestIterationsFunctionImpl(
					inlinedChildRegion->getMainNode(), TPStream, region->getID(), inlinedChildRegion);
			}
		}

		this->printTPConstructor(region, TPStream);
		
		if(DARTS_BACKEND){
			this->printTPDestructor(region, TPStream);
		}
		
		codeStr += TPStream.str();
	}

	TPRegion *childRegion = region->childRegions.front();
	while (childRegion) {
		printRegionCode(childRegion, codeStr);
		childRegion = childRegion->next;
	}
}

void ompASTVisitor::printFunctionRegion(TPRegion * region, std::string & codeStr)
{
	std::ostringstream TPStream;

	if(DARTS_BACKEND){
		includesStream << "class TP" << region->getID() << ";\n"
			<< "typedef TP" << region->getID() << " TP_" << region->getName() << ";\n";		
	} else if (SWARM_BACKEND){
		TPStream << "\n/*Function : " <<  region->getName() << ", ID: " << region->getID() << "*/\n"
				 << "CODELET_DECL(" << region->getName() << "Entry);\n"
				 << "size_t *Codelet" << region->getID() << "Entry_alreadySet;\n"
				 << "#define Codelet" << region->getName() << "Entry_alreadySet Codelet" << region->getID() << "Entry_alreadySet\n"
				 << "uint32_t numThreads" << region->getID() << ";\n"
				 << "#define numThreads" << region->getName() << " numThreads" << region->getID() << "\n";
	}

	if(DARTS_BACKEND){
		TPStream << "/*TP" << region->getID() << ": " << region->getName() << "*/\n"
			<< "class TP" << region->getID() << ":";

		if (region->singleThreaded) {
			TPStream << "public darts::ThreadedProcedure\n";
		} else {
			TPStream << "public ompTP\n";
		}
		
		TPStream << "{\n public:\n";

		printRegionCodeletsClass(region, TPStream);

		/* inline functions */
		//print any functions the inlined regions need
		for (TPRegion * inlinedChildRegion:region->inlinedRegions) {
			/* if the inlined region is an ompfor, print the iterationrequest function implementation */
			if (check_isa < OMPForDirective > (inlinedChildRegion->getStmt())
				|| check_isa < OMPParallelForDirective >
				(inlinedChildRegion->getStmt())) {
				inlinedChildRegion->
					getLoopInfo()->printRequestIterationsFunctionDef
					(inlinedChildRegion->mainNode, TPStream);
			}
		}

		TPStream << "TP" << region->getID() << "** ptrToThisFunctionTP;\n"
			<< "TP" << region->getID() << "* inputsTPParent;\n"
			<< "TP" << region->getID() << "* controlTPParent;\n";

	}

	if (region->getFunctionInfo()) {
		region->getFunctionInfo()->printFunctionInputsAndVarsDefs(TPStream, functionTable);

		/* print the inputs and vars for any inlined regions */
		/* inline functions */
		for (TPRegion * inlinedChildRegion:region->inlinedRegions) {
			inlinedChildRegion->mainNode->printOMPInputAndVarDefs(TPStream);
		}
	}

	this->printRegionExtraVarsDefs(region, TPStream);
	
	region->mainNode->printTaskDataDefs(TPStream);
	
	if(region->getFunctionInfo()->allocaVars.size() > 0)
		TPStream << "tbb::concurrent_vector<void*> allocaVars;\n";
	
	/* inline functions */
	for (TPRegion * inlinedChildRegion:region->inlinedRegions) {
		this->printRegionExtraVarsDefs(inlinedChildRegion, TPStream);
	}

	this->printRegionCodeletsDefs(region, TPStream);

	if(DARTS_BACKEND){
		TPStream << "TP" << region->getID() << "(int in_numThreads"
			<< ", int in_mainCodeletID"
			<< ", darts::Codelet* in_mainNextCodelet"
			<< ", darts::Codelet* in_mainSyncCodelet"
			<< ", TP" << region->getID() << "** in_ptrToThisFunctionTP";

		region->mainNode->printFunctionInputsAsParams(true, TPStream);

		if (region->getFunctionInfo()->isOMPDARTSFunction(functionTable)
			&& region->getFunctionInfo()->returnType.compare("void") != 0
			&& region->getName().compare("main") != 0) {
			TPStream << ", " << region->
				getFunctionInfo()->returnType << "* in_functionResult";
		}

		TPStream << ");\n"
			<< "~TP" << region->getID() << "();\n";

		if (region->singleThreaded == false) {
			region->getFunctionInfo()->printSetNewInputsFunctionDecl(TPStream,
										 functionTable);
		}

		TPStream << "};\n";
	}

	codeStr += TPStream.str();
}

void ompASTVisitor::printFunctionRegionCode(TPRegion * region, std::string & codeStr)
{
	std::ostringstream TPStream;

	for (auto var:region->getFunctionInfo()->varTable2)
		for (size_t i = 0; i < get < 0 > (var.second).size(); i++) {
			if (get < 2 > (var.second)[i].compare("static") == 0) {
				string varType = get < 0 > (var.second)[i];
				string varArray = get < 6 > (var.second)[i];
				TPStream << varType << " " << var.first << "_darts" <<
					region->getID() << "[MAXNUMTHREADS];\n";
			}
		}

	if(DARTS_BACKEND){
		TPStream << "/*TP" << region->getID() << ": TP_" << region->getName() << "*/\n";
	} else if (SWARM_BACKEND){
		TPStream << "/*Function : " <<  region->getName() << ", ID: " << region->getID() << "*/\n";
	}	

	printRegionCodeletsFire(region, TPStream);

	/* inline functions */
	/* print any functions the inlined regions need */
	for (TPRegion * inlinedChildRegion:region->inlinedRegions) {
		/* if the inlined region is an ompfor, print the iterationrequest function implementation */
		if (check_isa < OMPForDirective > (inlinedChildRegion->getStmt())
			|| check_isa < OMPParallelForDirective >
			(inlinedChildRegion->getStmt())) {
			inlinedChildRegion->
				getLoopInfo()->printRequestIterationsFunctionImpl
				(inlinedChildRegion->mainNode, TPStream, region->getID());
		}
	}

	if(DARTS_BACKEND){
		TPStream << "TP" << region->getID() << "::TP" << region->getID()
			<< "("
			<< "int in_numThreads, int in_mainCodeletID, "
			<< "darts::Codelet* in_mainNextCodelet, darts::Codelet* in_mainSyncCodelet, "
			<< "TP" << region->getID() << "** in_ptrToThisFunctionTP";

		region->mainNode->printFunctionInputsAsParams(true, TPStream);

		if (region->getFunctionInfo()->isOMPDARTSFunction(functionTable)
			&& region->getFunctionInfo()->returnType.compare("void") != 0
			&& region->getName().compare("main") != 0) {
			TPStream << ", " << region->
				getFunctionInfo()->returnType << "* in_functionResult";
		}

		TPStream << "):";
		if (region->singleThreaded) {
			TPStream << "ThreadedProcedure(in_numThreads, in_mainCodeletID)";
		} else {
			TPStream << "ompTP(in_numThreads, in_mainCodeletID)";
		}

		TPStream << ", ptrToThisFunctionTP(in_ptrToThisFunctionTP), "
			<< "inputsTPParent(this), "
			<< "controlTPParent(this)";

		if (region->getFunctionInfo()) {
			region->getFunctionInfo()->printFunctionInputsAndVarsInits(TPStream, functionTable);

			/* inline functions */
			for (TPRegion * inlinedChildRegion:region->inlinedRegions) {
				inlinedChildRegion->mainNode->printOMPInputAndVarInits(TPStream);
			}
		}

		this->printRegionExtraVarsInits(region, TPStream);
		
		region->mainNode->printTaskDataInits(TPStream);
		
		/* inline functions */
		for (TPRegion * inlinedChildRegion:region->inlinedRegions) {
			this->printRegionExtraVarsInits(inlinedChildRegion, TPStream);
		}

		this->printRegionCodeletsInits(region, TPStream);

		TPStream << "{\n";
		
		if(this->myDFG.useFunctionStack)
			TPStream << "pushFunctionStack(this->getID(), (void*)this);\n";
		
	} else if(SWARM_BACKEND){
		TPStream << "/*Init of Codelet*/\nCODELET_IMPL_BEGIN_NOCANCEL(" << region->getName() << "Entry)\n"
			<< "{\n";
			
		TPStream << "uint64_t codeletID = (uint64_t)(swarm_natP_t)THIS;\n";

		TPStream << "/*Check if the parameters for this function have been already set*/\n"
			<< "if(!(__sync_val_compare_and_swap(Codelet" <<  region->getName() << "Entry_alreadySet, 0, 1))){\n";
		TPStream << "numThreads" << region->getName() << " = currentNumThreads[codeletID];\n";

		if (region->getFunctionInfo()) {
			region->getFunctionInfo()->printFunctionInputsAndVarsInits(TPStream, functionTable);

			/* inline functions */
			for (TPRegion * inlinedChildRegion:region->inlinedRegions) {
				inlinedChildRegion->mainNode->printOMPInputAndVarInits(TPStream);
			}
		}
	
		this->printRegionExtraVarsInits(region, TPStream);
		/* inline functions */
		for (TPRegion * inlinedChildRegion:region->inlinedRegions) {
			this->printRegionExtraVarsInits(inlinedChildRegion, TPStream);
		}

		this->printRegionCodeletsInitsInConstructor(region, TPStream);

		TPStream << "*(Codelet" <<  region->getName() << "Entry_alreadySet) = 2;\n"
				 << "}\n"
				 << "/*Check if the parameters for this function have been already set*/\n"
				 << "if(*(Codelet" <<  region->getName() << "Entry_alreadySet) != 2){\n"
				 << "swarm_schedule(&CODELET(" << region->getName() << "Entry), "
				 << "THIS, NULL, NEXT, NEXT_THIS);\n"
				 << "return;\n"
				 << "}\n";
		
		TPStream << "/*Save the return codelet and its context.*/\n"
			<< "nextCodelets" << region->getName() << "[codeletID] = NEXT;\n"
			<< "nextCodeletContext" << region->getName() << "[codeletID] = NEXT_THIS;\n";
			
		TPRegion *childRegion = region->childRegions.front();
		if(childRegion){
			string nameFrontCodelet = "checkInCodelets" + to_string(childRegion->getID());
			TPStream << "/*Launch the first codelet in the region*/\n"
					 << "swarm_schedule(&CODELET(" << nameFrontCodelet << "), "
					 << "(void*)codeletID, NULL, NULL, NULL);\n";
		}
	}

	this->printRegionExtraVarsInitsInConstructor(region, TPStream);
	/* inline functions */
	for (TPRegion * inlinedChildRegion:region->inlinedRegions) {
		this->printRegionExtraVarsInitsInConstructor(inlinedChildRegion, TPStream);
	}

	if(DARTS_BACKEND){
		
		this->printRegionCodeletsInitsInConstructor(region, TPStream);

		if (!region->initGlobalThreadsLoopStr.empty())
			TPStream << "for(size_t codeletCounter = 0; "
				<< "codeletCounter < ompNumThreads; "
				<< "codeletCounter++)\n"
				<< "{\n"
				<< region->initGlobalThreadsLoopStr
				<< "}\n";

		if (!region->initLocalThreadsLoopStr.empty())
			TPStream << "for(size_t codeletCounter = 0; "
				<< "codeletCounter < this->numThreads; "
				<< "codeletCounter++)\n"
				<< "{\n"
				<< region->initLocalThreadsLoopStr
				<< "}\n";

		if (region->getFunctionInfo())
			region->getFunctionInfo()->printFunctionInputsAndVarsInitsInConstructor(TPStream, functionTable);
	}

	if(DARTS_BACKEND)
		TPStream << "}\n";
	else if (SWARM_BACKEND)
		TPStream << "}CODELET_IMPL_END/*End of Codelet*/;\n";
	
	if(DARTS_BACKEND){
		TPStream << "TP" << region->getID() << "::~TP" << region->getID() << "(){\n";
	
		if(region->getFunctionInfo()->allocaVars.size() > 0)
			TPStream << "/*Delete any variables allocated using alloca() in the original code*/\n"
				<< "for(void* allocaPtr: this->allocaVars)\n"
				<< "free(allocaPtr);\n";
	
		this->printRegionExtraVarsFree(region, TPStream);
		
		region->mainNode->printTaskDataFree(TPStream);
		
		this->printRegionCodeletsDelete(region, TPStream);

		if (region->getFunctionInfo()) {
			region->getFunctionInfo()->printFunctionInputsAndVarsDelete(TPStream, functionTable);
		}

		TPStream << "}\n";

		if (region->singleThreaded == false) {
			region->getFunctionInfo()->printSetNewInputsFunction(TPStream, functionTable);
		}
	}

	codeStr += TPStream.str();
}

std::string ompASTVisitor::getInitValueForDeclStmt(DFGNode * node, clang::PrintingPolicy & Policy)
{
	ostringstream declStream;

	DeclStmt *declStmt = dyn_cast < DeclStmt > (node->getStmt());

	if (declStmt) {
		for (Decl **declIt = declStmt->decl_begin(); declIt != declStmt->decl_end(); declIt++) {
			string declName = getNameFromDecl(*declIt);
			
			bool isStatic = false;
			VarTableTuple_t varTuple;
			bool found = this->myDFG.getVarTupleFromTable(node, declName, varTuple);
			if (found && get < 2 > (varTuple).compare("static") == 0)
					isStatic = true;

			bool declaredLocally = false;
			string typeStr = "";
			string arrayStr = "";
			DFGNode *ompParent = node->findOMPParent();
			if (ompParent) {
				TPInputs_t::iterator it = ompParent->ompVariables.find(declName);
				if (it != ompParent->ompVariables.end()) {
					declaredLocally = get < 5 > (it->second);
					typeStr = get < 0 > (it->second);
					arrayStr = get < 4 > (it->second);
				}
			}

			VarDecl *varDecl = dyn_cast < VarDecl > (*declIt);
			if (varDecl) {
				std::string prefixStr = "";
				if(isStatic == false)
					prefixStr = Policy.prefixStr;
				std::string postfixStr = Policy.postfixStr;
				
				ostringstream initStr;
				initStr << "";
				if (varDecl->hasInit()) {
					Expr *initExpr = varDecl->getInit();
					if (initExpr) {
						string ostreamString;
						raw_string_ostream rawStringOStream(ostreamString);
						initExpr->printPretty(rawStringOStream, nullptr, Policy);
						initStr << " = " << rawStringOStream.str();
					}
					if(isStatic)
						declStream << "if(" << declName << suffixLang
								<< Policy.darts_TPID << "_initFlag"<< postfixStr
								<< " == false)\n";
				}

				if (declaredLocally){
					declStream << typeStr << " " << declName << " " << arrayStr
						<< initStr.str() << ";\n";
				}
				else if (varDecl->hasInit()) {
					
					declStream << prefixStr << declName << suffixLang << Policy.darts_TPID
						<< postfixStr << initStr.str() << ";\n";
					
					if(isStatic)
						declStream << declName << suffixLang
							<< Policy.darts_TPID << "_initFlag"<< postfixStr << " = true;\n";
				}
			}
		}
	}

	return declStream.str();
}

}
