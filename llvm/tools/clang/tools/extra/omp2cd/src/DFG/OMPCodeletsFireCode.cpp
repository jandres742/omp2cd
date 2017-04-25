#include "include/DFG/DFG.h"
#include "include/DFG/DFGNode.h"
#include "include/DFG/LoopInfo.h"
#include "include/DFG/TPRegion.h"
#include "include/DFG/TaskInfo.h"

using namespace llvm;
using namespace clang;
using namespace clang::tooling;
using namespace std;

namespace omp2cd_space {

void TPRegion::printOMPCodeletFireCodeInlined(std::ostringstream& outputStream)
{
    this->childRegions.front()->printRegionFireCode(outputStream);
}

void TPRegion::printOMPCodeletFireCodeInvokeTPOneThread(std::ostringstream& outputStream)
{
    TPRegion* parentRegion = this->getParent();

    if (DARTS_BACKEND) {
        outputStream << "invoke < TP" << this->getID() << " > (myTP, 1, this->getID(), myTP";

        if (this->parent == nullptr || this->parent->singleThreaded == false) {
            outputStream << this->getMainNode()->printOMPInputNamesInFunctionCall(
                parentRegion->mainNode, DARTS_PREFIXSTR_DEFAULT, true, "[this->getID()]");
        } else {
            outputStream << this->getMainNode()->printOMPInputNamesInFunctionCall(
                parentRegion->mainNode, DARTS_PREFIXSTR_DEFAULT, true, "");
        }
        outputStream << ");\n";
    } else if (SWARM_BACKEND) {
        outputStream << this->getMainNode()->printCodeletParamPassing_swarm(
            parentRegion->mainNode, "[codeletID]");
        ostringstream targetRegionName;
        targetRegionName << this->getName() << this->getID() << "Entry";
        outputStream << "swarm_schedule(&CODELET(" << targetRegionName.str()
                     << "), (void *) codeletID, NULL, NULL, NULL);\n";
    }
}

void TPRegion::printOMPCodeletFireCodeInvokeTPMultipleThread(std::ostringstream& outputStream)
{
    TPRegion* parentRegion = this->getParent();

    outputStream << "invoke < TP" << this->getID() << " > (myTP";

    /*Set the number of threads for the TP */
    if (this->isOMPParallel())
        outputStream << ", ompNumThreads";
    else
        outputStream << ", myTP->numThreads";

    outputStream << ", this->getID(), myTP";

    /*Set the init and last iteration for an OMPFor */
    if (this->isOMPFor()) {
        LoopInfo* myLoopInfo = this->getLoopInfo();

        DFGNode* srcNode = parentRegion->mainNode;
        if (!(check_isa<OMPExecutableDirective>(srcNode->getStmt())))
            srcNode = srcNode->findParentNode();

        string loopInit
            = myLoopInfo->getForStmtInit(srcNode, DARTS_PREFIXSTR_DEFAULT, "[this->getID()]");
        string loopLimit
            = myLoopInfo->getForStmtLimit(srcNode, DARTS_PREFIXSTR_DEFAULT, "[this->getID()]");

        outputStream << ", " << loopInit << ", " << loopLimit;
    }

    /*If it's a ParallelDirective, it can start right away. */
    /*FIXME: Check if it works for nested parallel regions. */
    if (check_isa<OMPParallelDirective>(this->getStmt())
        || check_isa<OMPParallelForDirective>(this->getStmt())) {
        outputStream << ", true ";
    }

    /*Print the arguments */
    if (this->parent == nullptr || this->parent->singleThreaded == false) {
        outputStream << this->getMainNode()->printOMPInputNamesInFunctionCall(
            parentRegion->mainNode, DARTS_PREFIXSTR_DEFAULT, true, "[this->getID()]");
    } else {
        outputStream << this->getMainNode()->printOMPInputNamesInFunctionCall(
            parentRegion->mainNode, DARTS_PREFIXSTR_DEFAULT, true, "");
    }

    if (check_isa<OMPExecutableDirective>(this->getStmt()) && this->isSingleThreaded() == false) {
        if (!(this->isOMPParallel() || check_isa<OMPTaskDirective>(this->getStmt())))
            outputStream << ", &(myTP->TP" << this->getID() << "Ptr)";
    }
    outputStream << ");\n";
}

void TPRegion::printOMPSingleCodeletFireCode(std::ostringstream& outputStream)
{
    /*If the parent is singlethread, make sure only one codelet launches the TP. */
    if (this->parent == nullptr || this->parent->singleThreaded == false) {
        outputStream << "/*Select the thread executing " << this->getName() << " " << this->getID()
                     << "*/\n";

        if (DARTS_BACKEND)
            outputStream << "if (! __sync_val_compare_and_swap(&(myTP->TP" << this->getID()
                         << "_alreadyLaunched), 0, 1))\n"
                         << "{\n";
        else if (SWARM_BACKEND)
            outputStream << "if (! __sync_val_compare_and_swap(&(Codelet" << this->getID()
                         << "_alreadyLaunched), 0, 1))\n"
                         << "{\n";
    }

    /*If the region has been inlined, just print the code here and don't
     * invoke the TP.*/
    if (this->isInlinedRegion() && this->childRegions.front()) {
        this->printOMPCodeletFireCodeInlined(outputStream);
        outputStream << "}\n"
                     << "else\n"
                     << "{\n";
        this->getMainNode()->signalNext(outputStream, "myTP->", "this->getID()");
        outputStream << "}\n";
    } else {
        this->printOMPCodeletFireCodeInvokeTPOneThread(outputStream);
        if (DARTS_BACKEND) {
            if (this->parent == nullptr || this->parent->singleThreaded == false) {
                outputStream << "}\n";
                outputStream << "else\n";
                outputStream << "{\n";
                ostringstream tempStream;
                string nextCodelet
                    = this->getMainNode()->signalNext(tempStream, "myTP->", "this->getID()");

                if (nextCodelet.find("myTP->") == string::npos)
                    outputStream << "myTP->";
                outputStream << nextCodelet << ".decDep();\n";
                outputStream << "}\n";
            }
        } else if (SWARM_BACKEND) {
            if (this->parent == nullptr || this->parent->singleThreaded == false) {
                outputStream << "}\n";
                outputStream << "else\n";
                outputStream << "{\n";
                ostringstream tempStream;
                string nextCodelet = this->getMainNode()->signalNext(tempStream, "", "codeletID");
                if (nextCodelet.find("barrierCodelets") == string::npos)
                    outputStream << "swarm_schedule(&CODELET(" << nextCodelet << "), "
                                 << "(void*)codeletID, NULL, NULL, NULL);\n";
                else
                    outputStream << "swarm_Dep_satisfyOnce(&" << nextCodelet << "Dep);\n";
                outputStream << "}\n";
            }
        }
    }
}

void TPRegion::printOMPSectionCodeletFireCode(std::ostringstream& outputStream)
{
    this->printOMPSingleCodeletFireCode(outputStream);
}

void TPRegion::printOMPMasterCodeletFireCode(std::ostringstream& outputStream)
{
    /*If the parent is singlethread, make sure only one
                    codelet launches the TP. */
    if (this->parent == nullptr || this->parent->singleThreaded == false) {
        if (DARTS_BACKEND)
            outputStream << "if (this->getID() == 0)\n"
                         << "{\n";
        else if (SWARM_BACKEND)
            outputStream << "if (codeletID == 0)\n"
                         << "{\n";
    }

    /*If the region has been inlined, just print the code here and don't
     * invoke the TP.*/
    if (this->isInlinedRegion() && this->childRegions.front()) {
        this->printOMPCodeletFireCodeInlined(outputStream);
        outputStream << "}\n"
                     << "else\n"
                     << "{\n";
        this->getMainNode()->signalNext(outputStream, "myTP->", "this->getID()");
        outputStream << "}\n";
    } else {
        this->printOMPCodeletFireCodeInvokeTPOneThread(outputStream);
        if (DARTS_BACKEND) {
            if (this->parent == nullptr || this->parent->singleThreaded == false) {
                outputStream << "}\n";
                outputStream << "else\n";
                outputStream << "{\n";
                ostringstream tempStream;
                string nextCodelet
                    = this->getMainNode()->signalNext(tempStream, "myTP->", "this->getID()");

                if (nextCodelet.find("myTP->") == string::npos)
                    outputStream << "myTP->";
                outputStream << nextCodelet << ".decDep();\n";
                outputStream << "}\n";
            }
        } else if (SWARM_BACKEND) {
            if (this->parent == nullptr || this->parent->singleThreaded == false) {
                outputStream << "}\n";
                outputStream << "else\n";
                outputStream << "{\n";
                ostringstream tempStream;
                string nextCodelet = this->getMainNode()->signalNext(tempStream, "", "codeletID");
                if (nextCodelet.find("barrierCodelets") == string::npos)
                    outputStream << "swarm_schedule(&CODELET(" << nextCodelet << "), "
                                 << "(void*)codeletID, NULL, NULL, NULL);\n";
                else
                    outputStream << "swarm_Dep_satisfyOnce(&" << nextCodelet << "Dep);\n";
                outputStream << "}\n";
            }
        }
    }
}

void TPRegion::printOMPForCodeletFireCode_darts(std::ostringstream& outputStream)
{
    TPRegion* parentRegion = this->getParent();

    outputStream << "/*Determine the TP to which this codelet "
                 << "belongs and check if this codelet spawns the TP or "
                 << "if it signals it using dispatchCodelet()*/\n";

    outputStream << "size_t idx = this->getID() / myTP->codeletsPerTP" << this->getID() << ";\n"
                 << "if(idx < myTP->TPsToUse" << this->getID() << "){\n"
                 << "if (!__sync_val_compare_and_swap (&(myTP->TP" << this->getID()
                 << "_alreadyLaunched[idx]), 0, 1)){\n";

    this->getLoopInfo()->printCalcOfParamsForBalancedOMPFor(this, outputStream);

    ostringstream tempInvokeStream;

    tempInvokeStream << "myTP, myTP->codeletsPerTP" << this->getID() << " * DARTS_CODELETS_MULT "
                     << ", this->getID(), myTP, "
                     << "initIteration, lastIteration";

    /*If it's a ParallelDirective, it can start right away. */
    /*FIXME: Check if it works for nested parallel regions. */
    if (this->isOMPParallel())
        tempInvokeStream << ", true ";

    /*Print the arguments */
    if (this->parent == nullptr || this->parent->singleThreaded == false)
        tempInvokeStream << this->getMainNode()->printOMPInputNamesInFunctionCall(
            parentRegion->mainNode, DARTS_PREFIXSTR_DEFAULT, true, "[this->getID()]");
    else
        tempInvokeStream << this->getMainNode()->printOMPInputNamesInFunctionCall(
            parentRegion->mainNode, DARTS_PREFIXSTR_DEFAULT, true, "");

    if (this->isSingleThreaded() == false)
        if (!(this->isOMPParallel() || check_isa<OMPTaskDirective>(this->getStmt())))
            tempInvokeStream << ", &(myTP->TP" << this->getID() << "Ptr[idx])";

    outputStream << "#if USEINVOKE == 1\n"
                 << "invoke < TP" << this->getID() << " > (" << tempInvokeStream.str() << ");\n"
                 << "#else\n"
                 << "place < TP" << this->getID() << " > (idx, " << tempInvokeStream.str() << ");\n"
                 << "#endif\n"
                 << "}else{\n"
                 << "if (myTP->TP" << this->getID() << "Ptr[idx] != nullptr){\n"
                 << "myTP->TP" << this->getID() << "Ptr[idx]->dispatchCodelet(this->getID ());\n"
                 << "}else{\n"
                 << "this->resetCodelet ();\n"
                 << "this->decDep ();\n"
                 << "}\n"
                 << "}\n"
                 << "}\n";

    if (this->hasBarrier() == false) {
        outputStream << "else\n";
        outputStream << "{\n";
        this->getMainNode()->signalNext(outputStream);
        outputStream << "}\n";
    }
}

void TPRegion::printOMPForCodeletFireCode_swarm(std::ostringstream& outputStream)
{
    TPRegion* parentRegion = this->getParent();
    TPRegion* childRegion = this->childRegions.front();
    if (childRegion) {
        outputStream << this->getMainNode()->printCodeletParamPassing_swarm(
            parentRegion->mainNode, "[codeletID]");

        string loopInit = "";
        string loopLimit = "";
        if (this->isOMPFor()) {
            TPRegion* parentRegion = this->parent;
            DFGNode* srcNode = parentRegion->mainNode;
            if (!(check_isa<OMPExecutableDirective>(srcNode->getStmt()))) {
                srcNode = srcNode->findParentNode();
            }
            loopInit = this->getLoopInfo()->getForStmtInit(srcNode, "", "[codeletID]");
            loopLimit = this->getLoopInfo()->getForStmtLimit(srcNode, "", "[codeletID]");
        }

        outputStream << "initIteration" << this->getID() << " = " << loopInit << ";\n"
                     << "lastIteration" << this->getID() << " = " << loopLimit << ";\n";

        ostringstream targetRegionName;
        targetRegionName << this->getName() << this->getID() << "Entry";

        outputStream << "swarm_schedule(&CODELET(" << targetRegionName.str()
                     << "), (void *) codeletID, NULL, NULL, NULL);\n";
    }
}

void TPRegion::printOMPForCodeletFireCode(std::ostringstream& outputStream)
{
    outputStream << "/*region " << this->getID() << " "
                 << this->getMainNode()->ompExtensionClause->codelet << "*/\n";

    TPRegion* parentRegion = this->getParent();
    if (this->isOMPForOnlyChildRegion()) {
        this->getLoopInfo()->implementDARTSLoops(this, parentRegion, outputStream);
    } else if (this->isInlinedRegion() && this->childRegions.front()) {
		if (DARTS_BACKEND) {
			this->printOMPCodeletFireCodeInlined(outputStream);
		}
    } else if (this->isBalancedOMPFor()) {
        if (DARTS_BACKEND)
            this->printOMPForCodeletFireCode_darts(outputStream);
        else if (SWARM_BACKEND)
            this->printOMPForCodeletFireCode_swarm(outputStream);
    }
}

void TPRegion::printOMPCodeletFireCode(std::ostringstream& outputStream)
{
    /*If it's a barrier, just signal it. */
    if (check_isa<OMPBarrierDirective>(this->getStmt()) && this->prevRegion == nullptr) {
        outputStream << "myTP->barrierCodelets" << this->getID() << "[0].decDep();\n";
    } else if (check_isa<OMPSingleDirective>(this->getStmt())) {
        this->printOMPSingleCodeletFireCode(outputStream);
    } else if (check_isa<OMPSectionDirective>(this->getStmt())) {
        this->printOMPSectionCodeletFireCode(outputStream);
    } else if (check_isa<OMPMasterDirective>(this->getStmt())) {
        this->printOMPMasterCodeletFireCode(outputStream);
    } else if (check_isa<OMPForDirective>(this->getStmt())) {
        this->printOMPForCodeletFireCode(outputStream);
    }
    /*Print other omp directives */
    else {
        /*Print the code for an inlined region */
        if (this->isInlinedRegion() && this->childRegions.front()) {
            this->printOMPCodeletFireCodeInlined(outputStream);
        }
        /*Or invoke the TP */
        else {
            /*Use CAS in case more than one codelet tries to invoke the TP */
            if (this->parent == nullptr || this->parent->singleThreaded == false) {
                outputStream << "if (! __sync_val_compare_and_swap(&(myTP->TP" << this->getID()
                             << "_alreadyLaunched), 0, 1))\n"
                             << "{\n";
            }

            this->printOMPCodeletFireCodeInvokeTPMultipleThread(outputStream);

            if (this->parent == nullptr || this->parent->singleThreaded == false) {
                outputStream << "}\n"
                             << "else{\n"
                             << "if(myTP->TP" << this->getID() << "Ptr == nullptr){\n"
                             << "this->resetCodelet(); \n"
                             << "this->decDep();\n"
                             << "}else{\n"
                             << "#if USE_SPIN_CODELETS == 0\n"
                             << "myTP->TP" << this->getID() << "Ptr->firstCodelet"
                             << "[this->getID()].decDep();\n"
                             << "#else\n"
                             << "myTP->TP" << this->getID() << "Ptr->availableCodelets"
                             << "[this->getID()] = 1;\n"
                             << "#endif\n"
                             << "}\n"
                             << "}\n";
            }
        }
    }
}
}
