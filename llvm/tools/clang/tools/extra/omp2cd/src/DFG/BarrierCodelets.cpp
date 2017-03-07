#include "include/DFG/CopyPrivateClause.h"
#include "include/DFG/DFG.h"
#include "include/DFG/DFGNode.h"
#include "include/DFG/TPRegion.h"

using namespace llvm;
using namespace clang;
using namespace clang::tooling;
using namespace std;

namespace omp2cd_space {

void TPRegion::printBarrierCodeletDef(std::ostringstream& outputStream)
{
    this->isCodelet = true;

    if (DARTS_BACKEND) {
        outputStream << "_barrierCodelets" << this->getID() << "* barrierCodelets" << this->getID()
                     << ";\n";
    } else if (SWARM_BACKEND) {
        outputStream << "swarm_Dep_t barrierCodelets" << this->getID() << "Dep;\n";
        outputStream << "CODELET_DECL(barrierCodelets" << this->getID() << ");\n";
    }
}

void TPRegion::printBarrierCodeletInit(std::ostringstream& outputStream, bool startWithComma)
{
    string numDependencies;

    if (this->mainNode->tasksToExecute.size() > 0) {
        numDependencies += to_string(this->mainNode->tasksToExecute.size());
        numDependencies += "*";
    }

    if (check_isa<OMPParallelDirective>(this->getStmt())
        || check_isa<OMPParallelForDirective>(this->getStmt()))
        numDependencies += "ompNumThreads";
    else
        numDependencies += "this->numThreads";

    if (startWithComma)
        outputStream << " ,";

    outputStream << "barrierCodelets" << this->getID() << "(new _barrierCodelets" << this->getID()
                 << "[1])";
}

/* bool printingInRegion: True if the barrier is being printed on its same region, not on its
* parent.
* this is done when a balancedOMPFor is being printed and there is a barrier on each spawned TP.
* */
int TPRegion::printBarrierCodeletCreation(std::ostringstream& outputStream, bool printingInRegion)
{
    int res = -1;

    string numDependencies = "";

    if (this->isOMPParallel()) {
        if (this->hasOnlyOneChildOMPForRegion()) {
            numDependencies += "1";
        } else {
            numDependencies += "ompNumThreads";
        }
        res = initInGlobalThreadsLoop;
    } else if (this->isOMPForOnlyChildRegion()) {
        numDependencies += "1";
    } else if (this->isBalancedOMPFor() && this->hasBarrier() && printingInRegion == false) {
        numDependencies += "NUMTPS";
        numDependencies += to_string(this->getID());
    } else {
        numDependencies += "this->numThreads";
        res = initInLocalThreadsLoop;
    }

    if (DARTS_BACKEND) {
        outputStream << "barrierCodelets" << this->getID() << "[0] = _barrierCodelets"
                     << this->getID() << "(" << numDependencies << "," << numDependencies
                     << ",this, 0);\n";
    } else if (SWARM_BACKEND) {
        DFGNode* parentNode = this->mainNode;
        while (parentNode && parentNode->isOMPParallel() == false
            && parentNode->getFunctionInfo() == nullptr) {
            parentNode = parentNode->parent;
        }

        if (parentNode)
            outputStream << "swarm_Dep_init(&(barrierCodelets" << this->getID()
                         << "Dep), numThreads" << parentNode->getID()
                         << ", &CODELET(barrierCodelets" << this->getID()
                         << "), (void *) NULL, (void *) NULL);\n";
        else
            outputStream << "swarm_Dep_init(&(barrierCodelets" << this->getID()
                         << "Dep), ompNumThreads, &CODELET(barrierCodelets" << this->getID()
                         << "), (void *) NULL, (void *) NULL);\n";
    }
    return res;
}

void TPRegion::printBarrierCodeletDelete(std::ostringstream& outputStream)
{
    outputStream << "delete [] barrierCodelets" << this->getID() << ";\n";
}

void TPRegion::printBarrierCodeletClass(std::ostringstream& outputStream, bool printingInRegion)
{
    if (backEnd.compare("swarm") == 0)
        return;

    outputStream << "class _barrierCodelets" << this->getID() << " : "
                 << "public darts::Codelet\n"
                 << "{\n"
                 << "public:\n";

    TPRegion* parentRegion = this->parent;
    if (parentRegion == nullptr)
        parentRegion = this;

    int myTPID = parentRegion->getID();
    TPRegion* inputsParentRegion = parentRegion;
    if (printingInRegion) {
        myTPID = this->getID();
        inputsParentRegion = this;
    }

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

    outputStream << "_barrierCodelets" << this->getID() << "():\n"
                 << "darts::Codelet(){ }\n";

    outputStream << "_barrierCodelets" << this->getID() << "(uint32_t dep, uint32_t res, TP"
                 << myTPID << "* myTP, uint32_t id):\n"
                 << "darts::Codelet(dep,res,myTP,LONGWAIT, id)"
                 << ", inputsTPParent(myTP->inputsTPParent)"
                 << "{ }\n"
                 << "void fire(void);\n"
                 << "};\n";
}

void TPRegion::printBarrierCodeletFire(std::ostringstream& outputStream, bool printingInRegion)
{
    TPRegion* parentRegion = this->parent;
    if (parentRegion == nullptr)
        parentRegion = this;

    int myTPID = parentRegion->getID();

    if (printingInRegion) {
        if (DARTS_BACKEND) {
            outputStream << "void TP" << this->getID() << "::_barrierCodelets" << this->getID()
                         << "::fire(void)\n"
                         << "{\n"
                         << "TP" << this->getID() << "* myTP = static_cast<TP" << this->getID()
                         << "*>(myTP_);\n";
        } else if (SWARM_BACKEND) {
            outputStream << "/*Init of Codelet*/\nCODELET_IMPL_BEGIN_NOCANCEL(barrierCodelets"
                         << this->getID() << ")\n"
                         << "{\n"
                         << "countCodelets" << this->getID() << " = 0;\n";
        }

        if (this->isOMPParallel()) {
            if (DARTS_BACKEND) {
                outputStream << "myTP->controlTPParent->nextCodelet->decDep ();\n";
            } else if (SWARM_BACKEND) {
                outputStream << "/*Signal the runtime we are done here.*/\n"
                             << "swarm_dispatch("
                             << "nextCodelets" << this->getID() << ", "
                             << "nextCodeletContext" << this->getID() << ", "
                             << "(void*)NULL, (void*)NULL, (void*)NULL);\n";
            }
        } else {
            outputStream << "myTP->controlTPParent->TPParent->barrierCodelets" << this->getID()
                         << "[0].decDep ();\n";
        }

        if (backEnd.compare("darts") == 0) {
            outputStream << "}\n";
        } else if (backEnd.compare("swarm") == 0) {
            outputStream << "}CODELET_IMPL_END/*End of Codelet*/;\n";
        }
        return;
    }

    if (DARTS_BACKEND) {
        outputStream << "void TP" << myTPID << "::_barrierCodelets" << this->getID()
                     << "::fire(void)\n"
                     << "{\n"
                     << "TP" << myTPID << "* myTP = "
                     << " static_cast<TP" << myTPID << "*>(myTP_);\n";
    } else if (SWARM_BACKEND) {
        outputStream << "/*Init of Codelet*/\nCODELET_IMPL_BEGIN_NOCANCEL(barrierCodelets"
                     << this->getID() << ")\n"
                     << "{\n";
    }

    /* Check if there are any copyprivate clauses that need to be
     * copied upon this point.*/
    for (DFGNode* copyprivateNode : this->mainNode->nodesToCopyprivate)
        if (copyprivateNode->getCopyPrivateClause())
            copyprivateNode->getCopyPrivateClause()->copyValuesToVars(outputStream);

    if (check_isa<OMPTaskwaitDirective>(this->getStmt()) && this->myNodes.size() > 1) {
        this->printRegionFireCode(outputStream);
    } else if (this->nextRegion) {
        outputStream << "{\n";
        if (check_isa<OMPBarrierDirective>(this->nextRegion->getStmt())) {
            if (DARTS_BACKEND)
#if 0
				outputStream << "for(size_t codeletsCounter=0; codeletsCounter < myTP->numThreads; "
					<< "codeletsCounter++)\n"
					<< "{\n"
					<< "myTP->barrierCodelets" << this->nextRegion->getID() << "[0].decDep();\n"
					<< "}\n";
#else
                outputStream << "myTP->barrierCodelets" << this->nextRegion->getID()
                             << "[0].setDep(0);\n"
                             << "myTP->add(&(myTP->barrierCodelets" << this->nextRegion->getID()
                             << "[0]));\n";
#endif
                else if (SWARM_BACKEND) outputStream
                    << "for(size_t codeletsCounter=0; codeletsCounter < numThreads"
                    << this->parent->getID() << "; codeletsCounter++)\n"
                    << "{\n"
                    << "swarm_Dep_satisfy(&barrierCodelets" << this->nextRegion->getID()
                    << "Dep, 1);\n"
                    << "}\n";
        } else {
            if (this->parent && this->parent->singleThreaded) {
                outputStream << "myTP->checkInCodelets" << this->nextRegion->getID()
                             << ".decDep();\n";
            } else {
                if (DARTS_BACKEND)
                    outputStream << "for(size_t codeletsCounter=0; "
                                 << "codeletsCounter < myTP->numThreads;"
                                 << "codeletsCounter++)\n"
                                 << "{\n"
                                 << "myTP->checkInCodelets" << this->nextRegion->getID()
                                 << "[codeletsCounter].decDep();\n"
                                 << "}\n";
                else if (SWARM_BACKEND) {
                    outputStream << "for(size_t codeletsCounter=0; codeletsCounter < numThreads"
                                 << this->parent->getID() << "; codeletsCounter++)\n"
                                 << "{\n"
                                 << "swarm_schedule(&CODELET(checkInCodelets"
                                 << this->nextRegion->getID() << "), "
                                 << "(void *)codeletsCounter, NULL, NULL, NULL);\n"
                                 << "}\n";
                }
            }
        }
        outputStream << "}\n";
    } else {
        if (this->isOMPParallel()) {
            outputStream << "myTP->controlTPParent->nextCodelet->decDep ();\n";
        } else {
            this->mainNode->signalNextCodelet(parentRegion, outputStream, "myTP->", "", true);
        }
    }

    if (backEnd.compare("darts") == 0) {
        outputStream << "}\n";
    } else if (backEnd.compare("swarm") == 0) {
        outputStream << "}CODELET_IMPL_END/*End of Codelet*/;\n";
    }
}
}
