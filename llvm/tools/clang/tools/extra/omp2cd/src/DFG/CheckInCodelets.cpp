#include "include/DFG/BranchInfo.h"
#include "include/DFG/DFG.h"
#include "include/DFG/DFGNode.h"
#include "include/DFG/LoopCondNode.h"
#include "include/DFG/LoopInfo.h"
#include "include/DFG/TPRegion.h"
#include "include/DFG/TaskInfo.h"

using namespace llvm;
using namespace clang;
using namespace clang::tooling;
using namespace std;

namespace omp2cd_space {

void TPRegion::printCheckInCodeletDef(std::ostringstream& outputStream)
{
    if (check_isa<OMPBarrierDirective>(this->getStmt()) && this->prevRegion)
        return;

    if (check_isa<OMPTaskwaitDirective>(this->getStmt()))
        return;

    this->isCodelet = true;

    TPRegion* parentRegion = this->parent;

    if (parentRegion == nullptr)
        parentRegion = this;

    if (backEnd.compare("swarm") == 0) {
        outputStream << "CODELET_DECL(checkInCodelets" << this->getID() << ");\n";
        return;
    }

    if (this->parent && this->parent->singleThreaded) {
        outputStream << "_checkInCodelets" << this->getID() << " checkInCodelets" << this->getID()
                     << ";\n";
    } else {
        outputStream << "_checkInCodelets" << this->getID() << "* checkInCodelets" << this->getID()
                     << ";\n";

        if (parentRegion->isOMPForOnlyChildRegion() == false) {
            if (this->prevRegion == nullptr && parentRegion->isOMPParallel() == false) {
                outputStream << "#if USE_SPIN_CODELETS == 0\n"
                             << "_checkInCodelets" << this->getID() << "* firstCodelet"
                             << ";\n"
                             << "#endif\n";
            }
        }
    }

    if (this->isOMPForOnlyChildRegion())
        outputStream << "_checkInCodelets" << this->getID() << "_core "
                     << " *masterCodeletTP" << this->getID() << "_core;\n"
                     << "darts::codeletFor < TP" << this->getID() << "_loop > "
                     << "*masterCodeletTP" << this->getID() << "_loop;\n"
                     << "darts::paraFor < TP" << this->getID() << "_outer > "
                     << "*masterCodeletTP" << this->getID() << "_outer;\n";
}

void TPRegion::printCheckInCodeletInit(std::ostringstream& outputStream, bool startWithComma)
{
    if (backEnd.compare("swarm") == 0)
        return;

    if (check_isa<OMPBarrierDirective>(this->getStmt()) && this->prevRegion)
        return;

    if (check_isa<OMPTaskwaitDirective>(this->getStmt()))
        return;

    DFGNode* parentNode = this->getMainNode()->findParentNode();

    if (startWithComma)
        outputStream << " ,";

    if (this->parent && this->parent->singleThreaded) {
        outputStream << "checkInCodelets" << this->getID() << "(1,1,this,this->mainCodeletID)";
    } else if (check_isa<OMPTaskDirective>(parentNode->getStmt())) {
        outputStream << "checkInCodelets" << this->getID() << "(new _checkInCodelets"
                     << this->getID() << "[ompNumThreads])";
    } else {
        outputStream << "checkInCodelets" << this->getID() << "(new _checkInCodelets"
                     << this->getID();

        TPRegion* parentRegion = this->parent;
        if (parentRegion && parentRegion->isOMPForOnlyChildRegion())
            outputStream << "[1])";
        else
            outputStream << "[this->numThreads])";
    }

    if (this->isOMPForOnlyChildRegion()) {
        if (startWithComma)
            outputStream << " ,";

        outputStream << "masterCodeletTP" << this->getID() << "_core(new _checkInCodelets"
                     << this->getID() << "_core[1]),"
                     << "masterCodeletTP" << this->getID() << "_loop"
                     << "(new codeletFor < TP" << this->getID() << "_loop >[1]),"
                     << "masterCodeletTP" << this->getID() << "_outer"
                     << "(new paraFor < TP" << this->getID() << "_outer >[1])";
    }
}

void TPRegion::printCheckInCodeletDecDep(std::ostringstream& outputStream)
{
    if (check_isa<OMPBarrierDirective>(this->getStmt()) && this->prevRegion)
        return;

    if (check_isa<OMPTaskwaitDirective>(this->getStmt()))
        return;

    TPRegion* parentRegion = this->parent;
    if (parentRegion == nullptr)
        parentRegion = this;

    if (this->parent && this->parent->singleThreaded)
        if (this->prevRegion == nullptr && this->getMainNode()->myIfNode == nullptr)
            if (DARTS_BACKEND)
                outputStream << "checkInCodelets" << this->getID() << ".decDep();\n";
}

void TPRegion::printCheckInCodeletPtr(std::ostringstream& outputStream)
{
    if (SWARM_BACKEND)
        return;

    if (check_isa<OMPBarrierDirective>(this->getStmt()) && this->prevRegion)
        return;

    if (check_isa<OMPTaskwaitDirective>(this->getStmt()))
        return;

    TPRegion* parentRegion = this->parent;
    if (parentRegion == nullptr)
        parentRegion = this;

    if (parentRegion->isOMPForOnlyChildRegion()) {
        outputStream << "this->checkInCodelets" << this->getID() << "[0] = "
                     << "_checkInCodelets" << this->getID() << "(1, 1, this";

        parentRegion->mainNode->printOMPInputAndVarInitsRawAsInputs(outputStream);

        outputStream << ");\n"
                     << "this->checkInCodelets" << this->getID() << "[0].decDep();\n";
        return;
    }

    if (this->parent->singleThreaded == false) {
        outputStream << "_checkInCodelets" << this->getID() << " * checkInCodelets" << this->getID()
                     << "Ptr = (this->checkInCodelets" << this->getID() << ");\n";
        if (this->prevRegion == nullptr && parentRegion->isOMPParallel() == false) {
            outputStream << "#if USE_SPIN_CODELETS == 0\n"
                         << "firstCodelet = (this->checkInCodelets" << this->getID() << ");\n"
                         << "#endif\n";
        }
    }

    /*Init the control vars for a balanced OMPFor */
    /*FIXME: Move it to a function with a more fitting name */
    if (this->isBalancedOMPFor() && this->isOMPForOnlyChildRegion() == false) {
        outputStream << "for(int i=0; i<NUMTPS" << this->getID() << "; i++)\n"
                     << "{\n"
                     << "TP" << this->getID() << "Ptr[i] = nullptr;\n"
                     << "TP" << this->getID() << "_alreadyLaunched[i] = 0;\n"
                     << "}\n";
    }
}

int TPRegion::printCheckInCodeletCreation(std::ostringstream& outputStream)
{
    if (SWARM_BACKEND)
        return -1;

    if (check_isa<OMPBarrierDirective>(this->getStmt()) && this->prevRegion)
        return -1;

    if (check_isa<OMPTaskwaitDirective>(this->getStmt()))
        return -1;

    int res = 0;

    TPRegion* parentRegion = this->parent;
    if (parentRegion == nullptr)
        parentRegion = this;

    DFGNode* parentNode = this->getMainNode()->findParentNode();

    if (this->parent->singleThreaded == false) {
        if (check_isa<OMPTaskDirective>(parentNode->getStmt())) {
            outputStream << "(*checkInCodelets" << this->getID() << "Ptr) = "
                         << "_checkInCodelets" << this->getID() << "(1,1,this,codeletCounter);\n"
                         << "checkInCodelets" << this->getID() << "Ptr++;\n";
            res = initInGlobalThreadsLoop;
        } else {
            if (this->prevRegion == nullptr && parentRegion->isOMPParallel() == false
                && parentRegion->isOMPForOnlyChildRegion() == false) {
                outputStream << "#if USE_SPIN_CODELETS == 0\n"
                             << "(*checkInCodelets" << this->getID() << "Ptr) = _checkInCodelets"
                             << this->getID() << "(2,1,this,codeletCounter);\n"
                             << "#else\n"
                             << "(*checkInCodelets" << this->getID() << "Ptr) = _checkInCodelets"
                             << this->getID() << "(1,1,this,codeletCounter);\n"
                             << "#endif\n";
            } else {
                outputStream << "(*checkInCodelets" << this->getID() << "Ptr) = _checkInCodelets"
                             << this->getID() << "(1,1,this,codeletCounter);\n";
            }

            if (this->prevRegion == nullptr && this->getMainNode()->myIfNode == nullptr)
                outputStream << "(*checkInCodelets" << this->getID() << "Ptr).decDep();\n";

            outputStream << "checkInCodelets" << this->getID() << "Ptr++;\n";
            res = initInLocalThreadsLoop;
        }
    }

    return res;
}

void TPRegion::printCheckInCodeletDelete(std::ostringstream& outputStream)
{
    if ((check_isa<OMPBarrierDirective>(this->getStmt()) && this->prevRegion)
        || check_isa<OMPTaskwaitDirective>(this->getStmt()))
        return;

    if (this->parent == nullptr || this->parent->singleThreaded == false)
        outputStream << "delete [] checkInCodelets" << this->getID() << ";\n";
}

void TPRegion::printCheckInCodeletClass(std::ostringstream& outputStream)
{
    if (SWARM_BACKEND)
        return;

    if ((check_isa<OMPBarrierDirective>(this->getStmt()) && this->prevRegion)
        || check_isa<OMPTaskwaitDirective>(this->getStmt()))
        return;

    TPRegion* parentRegion = this->parent;
    if (parentRegion == nullptr)
        parentRegion = this;

    if (this->isOMPForOnlyChildRegion())
        this->printTPLoopCodeletCoreClass(outputStream);

    if (parentRegion->isOMPForOnlyChildRegion()) {
        this->printTPLoopCodeletClass(outputStream);
        return;
    }

    if (parentRegion->isOMPFor()
        && parentRegion->getMainNode()->ompExtensionClause->codelet == false)
        parentRegion->getLoopInfo()->printRequestIterationsFunctionDef(
            parentRegion->mainNode, outputStream);

    int myTPID = parentRegion->getID();
    outputStream << "class _checkInCodelets" << this->getID() << ":"
                 << "public darts::Codelet\n"
                 << "{\n"
                 << "public:\n"
                 << "TP" << myTPID << "* myTP;\n";

    TPRegion* inputsParentRegion = parentRegion;
    while (inputsParentRegion) {
        if (inputsParentRegion->isThreadedProcedure) {
            if (inputsParentRegion->getFunctionInfo()) {
                break;
            } else if (check_isa<OMPExecutableDirective>(inputsParentRegion->getStmt())) {
                break;
            } else if (check_isa<ForStmt>(inputsParentRegion->getStmt())
                && inputsParentRegion->getMainNode()->isComplexStmt) {
                inputsParentRegion = inputsParentRegion->getParent();
                continue;
            } else {
                break;
            }
        } else {
            inputsParentRegion = inputsParentRegion->getParent();
        }
    }

    outputStream << "TP" << inputsParentRegion->getID() << "* inputsTPParent;\n";
    if (check_isa<OMPTaskDirective>(inputsParentRegion->getStmt()))
        outputStream << "_task" << inputsParentRegion->getID() << "Inputs* taskInputs;\n";

    if (parentRegion->isOMPFor())
        outputStream << parentRegion->getLoopInfo()->loopVarType << " endRange;\n";
    else if (check_isa<OMPForDirective>(this->getStmt()) && this->isInlinedRegion())
        outputStream << this->getLoopInfo()->loopVarType << " endRange;\n";

    outputStream << "_checkInCodelets" << this->getID() << "():\n"
                 << "darts::Codelet(){ }\n"
                 << "_checkInCodelets" << this->getID() << "(uint32_t dep, uint32_t res, TP"
                 << myTPID << "* myTP, uint32_t id):\n"
                 << "darts::Codelet(dep,res,myTP,LONGWAIT, id)"
                 << ", myTP(myTP)"
                 << ", inputsTPParent(myTP->inputsTPParent)";
    if (check_isa<OMPTaskDirective>(inputsParentRegion->getStmt()))
        outputStream << ", taskInputs(inputsTPParent->task" << inputsParentRegion->getID()
                     << "Inputs)";

    outputStream << "{ }\n"
                 << "void fire(void);\n"
                 << "};\n";
}

void TPRegion::printCheckInCodeletFire(std::ostringstream& outputStream)
{
    if (check_isa<OMPBarrierDirective>(this->getStmt()) && this->prevRegion)
        return;

    if (check_isa<OMPTaskwaitDirective>(this->getStmt()))
        return;

    TPRegion* parentRegion = this->getParent();

    if (this->isOMPForOnlyChildRegion())
        this->printTPLoopCodeletCoreFire(outputStream);

    if (parentRegion->isOMPForOnlyChildRegion()) {
        this->printTPLoopCodeletForFire(outputStream);
        return;
    }

    int myTPID = parentRegion->getID();

    if (check_isa<ForStmt>(this->getStmt()) && (parentRegion->isOMPFor())
        && parentRegion->getMainNode()->ompExtensionClause->codelet == false){
			parentRegion->getLoopInfo()->printRequestIterationsFunctionImpl(
				parentRegion->getMainNode(), outputStream, myTPID, this);
	}

    if (DARTS_BACKEND) {
        outputStream << "void TP" << myTPID << "::_checkInCodelets" << this->getID()
                     << "::fire(void)\n"
                     << "{\n";

    } else if (SWARM_BACKEND) {
        outputStream << "/*Init of Codelet*/\nCODELET_IMPL_BEGIN_NOCANCEL(checkInCodelets"
                     << this->getID() << ")\n"
                     << "{\n"
                     << "uint64_t codeletID;\n";

        if (this->prevRegion == nullptr && parentRegion->getFunctionInfo() == nullptr)
            outputStream << "if(THIS == (void *)NULL)\n"
                         << "codeletID = swarm_atomic_getAndAdd(countCodelets"
                         << parentRegion->getID() << ", 1);\n"
                         << "else\n"
                         << "codeletID = (uint64_t)THIS;\n";
        else
            outputStream << "codeletID = (uint64_t)THIS;\n";

        outputStream << "(void)codeletID;\n";
    }

    if (this->getParent()) {
        if (this->getParent()->isOMPParallel() == false && this->prevRegion == nullptr
            && this->getParent()->singleThreaded == false
            && this->getMainNode()->myIfNode == nullptr) {
            if (DARTS_BACKEND) {
                outputStream
                    << "#if USE_SPIN_CODELETS == 1\n"
                    << "/*Wait until the codelet with the same ID finishes in the previous TP*/\n"
                    << "if(myTP->availableCodelets[this->getLocalID()] == 0)\n"
                    << "{\n"
                    << "myTP->add(this);\n"
                    << "return;\n"
                    << "}\n"
                    << "#endif\n";
            }
        } else if (check_isa<OMPTaskDirective>(this->getParent()->getStmt())) {
            if (DARTS_BACKEND) {
                TaskInfo* taskInfo = this->getParent()->getTaskInfo();
                if (taskInfo->inputDependencyNodes.size()) {
                    outputStream
                        << "/*Wait until the tasks this task depends on have completed*/\n";
                    outputStream << "if((";
                    bool printDelimiter = false;
                    for (DFGNode* inDepNode : taskInfo->inputDependencyNodes) {
                        if (printDelimiter)
                            outputStream << " && ";
                        printDelimiter = true;
						outputStream << " ( "
									 << "myTP->task" << this->getParent()->getID()
                                     << "Inputs->task" << inDepNode->getID()
                                     << "Completed == nullptr || "
									 << "*(myTP->task" << this->getParent()->getID()
                                     << "Inputs->task" << inDepNode->getID()
                                     << "Completed) == true)";
                    }
                    outputStream << " )== false){\n"
                                 << "myTP->add(this);\n"
                                 << "return;\n"
                                 << "}\n";
                }
            }
        }
    }

    this->printRegionFireCode(outputStream);

    if (DARTS_BACKEND)
        outputStream << "}\n";
    else if (SWARM_BACKEND)
        outputStream << "}CODELET_IMPL_END/*End of Codelet*/;\n";
}

void TPRegion::printRegionFireCode(std::ostringstream& outputStream)
{
    TPRegion* parentRegion = this->getParent();
    DFGNode* parentNode = this->getMainNode()->findParentNode();

    /*init the vars for this region */
    if (parentRegion->isOMPForOnlyChildRegion() == false) {
        if (this->prevRegion == nullptr && this->getMainNode()->myIfNode == nullptr) {
            if (check_isa<OMPExecutableDirective>(parentRegion->getStmt())
                || parentRegion->getFunctionInfo()) {
                if (check_isa<OMPExecutableDirective>(this->getStmt()) == false
                    && this->getFunctionInfo() == nullptr) {
                    outputStream << "/*Init the vars for this region*/\n";
                    if (parentNode->getRegion()->isInlinedRegion() == false)
                        this->getMainNode()->printOMPInputAndVarInitsInFirstCodelet(outputStream);
                    else
                        this->getMainNode()->printOMPInputAndVarInitsInlinedRegion(outputStream);
                }
            }
        }
    }

    /*Allocate each array var in the codelet */
    if (this->prevRegion == nullptr && parentNode->getRegion()->singleThreaded == false) {
        if (parentNode->getRegion()->hasOnlyOneChildOMPForRegion() == false)
            if ((check_isa<ForStmt>(parentRegion->getStmt())
                    && parentRegion->getMainNode()->isComplexStmt)
                == false)
                parentNode->printOMPArrayVarInitsInFirstCodelet(outputStream, this);

        if (check_isa<OMPExecutableDirective>(parentNode->getStmt()))
            parentNode->printOMPArrayInputInitsInFirstCodelet(outputStream, this);
    }

    /*Print code for an executable directive */
    if (!check_isa<OMPTaskDirective>(this->getStmt())
        && !check_isa<OMPTaskwaitDirective>(this->getStmt())
        && check_isa<OMPExecutableDirective>(this->getStmt())) {
        this->printOMPCodeletFireCode(outputStream);
    } else {
        if (check_isa<IfStmt>(this->getStmt()) && !this->getBranchInfo()->isSimpleIf) {
            this->getBranchInfo()->printConditionalBranch(outputStream);
        } else {
            this->printAllNodesFireCode(outputStream);
        }
    }
}

void TPRegion::printAllNodesFireCode(std::ostringstream& outputStream)
{
    TPRegion* parentRegion = this->parent;
    for (size_t i = 0; i < this->myNodes.size(); i++) {

        DFGNode* node = this->myNodes[i];

        outputStream << "\n/*printing node " << node->getID();
        if (node->getStmt())
            outputStream << ": " << node->getStmt()->getStmtClassName();
        outputStream << "*/\n";

        /*Let's first take care of control nodes.*/

        /*Taskwait*/
        if (check_isa<OMPTaskwaitDirective>(node->getStmt())) {
            continue;
        }
        /*Node calling a function*/
        else if (node->isCallerNode()) {
            node->printFunctionCall(outputStream);
        }
        /*loop containing a function call or omp directive*/
        else if (node->isLoopNode() && node->isRegionMainNode && node->isComplexStmt) {
            node->printNodeFireCode(outputStream);
        }
        /*Print the code for a condition node in a complex while/do/for stmt */
        else if (isa<LoopCondNode>(node)) {
            LoopCondNode* loopCondNode = dyn_cast<LoopCondNode>(node);
            outputStream << "/*Print the code for a condition node in a complex loop stmt */\n";
            bool nextPrinted = loopCondNode->printJump(outputStream);
            if (nextPrinted == false) {
                /*dont jump from the condition node at the entrance of a do loop */
                if ((check_isa<DoStmt>(loopCondNode->loopStmtNode->getStmt())
                        && loopCondNode->endCondLoopNode)
                    == false) {
                    if (loopCondNode->endCondLoopNode) {
                        outputStream << "/*Signal the codelet after the loop from the end's "
                                        "condional node.*/\n";
                        loopCondNode->endCondLoopNode->getRegion()->signalNext(outputStream);
                    } else {
                        outputStream
                            << "/*Signal the codelet after the loop from the condtional node.*/\n";
                        loopCondNode->getRegion()->signalNext(outputStream);
                    }
                }
            }
        }
        /*Now let's take care of computation nodes.*/
        else {
            if (check_isa<OMPTaskDirective>(node->getStmt())) {
                node->getTaskInfo()->printTaskCode(outputStream, i);
            } else {
                node->printNodeFireCode(outputStream);
            }

            /*First node in the region */
            if (i == 0)
                if (parentRegion->mainNode->parallelReductionVars.size() > 0)
                    this->getMainNode()->printParallelReductionVarPtrDefs(
                        outputStream, parentRegion);

            /*Last node in the region. */
            if (i == this->myNodes.size() - 1) {
                if (parentRegion->next == nullptr)
                    /*Update the reduction variables if any */
                    if (parentRegion->mainNode->parallelReductionVars.size() > 0)
                        node->updateGlobalParallelReductionVars(outputStream, parentRegion);

                /* Check if there are any copyprivate vars to copy before
                 * signaling the next codelet.*/
                if (parentRegion->getCopyPrivateClause()) {

                    if (DARTS_BACKEND) {
                        if (parentRegion->isInlinedRegion()) {
                            outputStream << DARTS_PREFIXSTR_DEFAULT;
                        } else {
                            outputStream << DARTS_PREFIXSTR_DEFAULT << "TPParent->";
                        }

                        outputStream << "codeletIDexecutingSingle" << parentRegion->getID()
                                     << " = this->getID();\n";
                    } else if (SWARM_BACKEND) {
                        outputStream << "codeletIDexecutingSingle" << parentRegion->getID()
                                     << " = codeletID;\n";
                    }
                }
                /*If this is a ompFor with balancedOMPFor option but no barriers, use the
                signalNextReady vector to know when to signal the next codelet*/
                if (DARTS_BACKEND) {
                    if (parentRegion->isBalancedOMPFor()
                        && parentRegion->isOMPForOnlyChildRegion() == false
                        && parentRegion->hasBarrier() == false) {
                        outputStream
                            << "/*If this omp for has no barrier, \n"
                            << "check if all the codelets \n"
                            << "replicated from the same \n"
                            << "global ID has finished and \n"
                            << "signal the next codelet. \n"
                            << "Otherwise, return.*/\n"
                            << "uint32_t completedMultCodelet = "
                            << "__sync_fetch_and_add("
                            << "&(myTP->signalNextReady["
                            << "this->getLocalID() % myTP->baseNumThreads]), 1);\n"
                            << "if(completedMultCodelet < (uint32_t)(DARTS_CODELETS_MULT - 1))\n"
                            << "return;\n";
                    }
                }
                outputStream << "/*Signaling next codelet from last stmt in the codelet*/\n";
                if (parentRegion->isOMPForOnlyChildRegion())
                    outputStream << "myTP->toSignal->decDep();\n";
                else if (parentRegion->isInlinedRegion() == false)
                    node->signalNext(outputStream);
                else
                    parentRegion->mainNode->signalNext(outputStream);
            }
        }
    }
}

void DFGNode::printNodeFireCode(std::ostringstream& outputStream)
{
    TPRegion* parentRegion = this->myRegion->parent;
    if (parentRegion == nullptr)
        parentRegion = this->myRegion;

    DFGNode* parentNode = this->parent;
    while (check_isa<CompoundStmt>(parentNode->getStmt()))
        parentNode = parentNode->parent;

    /*Dont print the code on this codelet if the parent is ompparallel
     * and will optimized as TP Loop*/
    if (check_isa<OMPParallelDirective>(parentRegion->getStmt())
        && parentRegion->hasOnlyOneChildOMPForRegion()
        && check_isa<OMPForDirective>(this->getStmt()) == false) {
        return;
    }

    if (parentRegion->mainNode->parallelReductionVars.size() > 0)
        this->replaceParallelReductionVarInFireCode(this->fireCodeStr, parentRegion);

    if (parentRegion->isOMPFor() == false) {
        outputStream << this->fireCodeStr;
    } else {

        /*If there is at least one lastprivate variable, we will have two for loops. One that keeps
           updating the lastprivate variables and one that doesn't. This is better than having an if
           inside the for loop because in this way we are having the if only once, instead of having
           it
           on every iteration */
        bool atLeastOneLastPrivate = false;
        for (auto var : parentRegion->getMainNode()->ompInputs) {
            outputStream << "/*var: " << var.first << "*/\n";
            if (get<1>(var.second) == OMP_LASTPRIVATE) {
                atLeastOneLastPrivate = true;
                break;
            }
        }

        if (parentRegion->isOMPForOnlyChildRegion() == false) {
            this->getLoopInfo()->printFireCode(
                outputStream, parentRegion, this, parentNode, atLeastOneLastPrivate);
        } else {
            this->getLoopInfo()->printTPLoopFireCode(outputStream, parentRegion, this);
        }
    }
}
}
