#include "include/DFG/TaskInfo.h"
#include "include/DFG/DFG.h"
#include "include/DFG/DFGNode.h"
#include "include/DFG/TPRegion.h"

using namespace llvm;
using namespace clang;
using namespace clang::tooling;
using namespace std;

namespace omp2cd_space {

TaskInfo::TaskInfo(DFGNode* _taskNode)
    : taskNode(_taskNode)
    , myTaskRegionStart(nullptr)
    , myTaskRegionNext(nullptr)
    , encapsulateTaskData(true)
    , mySyncPoint(nullptr)
    , mySyncPointRegion(nullptr)
    , nextRegionAfterTask(nullptr)
    , nextNodeAfterTask(nullptr)
    , containsFunctionCall(false)
{
}

void TaskInfo::printTaskCode(std::ostringstream& outputStream, size_t nodeCount)
{
    int taskID = this->taskNode->getID();

#if FULL_DEBUG_COMMENTS == 1
    outputStream << "#if 0\n";
    outputStream << "/*Finding task's sync point*/\n";
#endif
    string nextCodeletName = this->taskNode->signalTaskSyncPoint(
        outputStream, "myTP->controlTPParent->", "this->getID()");
#if FULL_DEBUG_COMMENTS == 1
    outputStream << "#endif\n";
#endif

    outputStream << "Codelet *nextSyncCodelet = ";
    size_t pos = nextCodeletName.find("nextCodelets");
    if (pos != string::npos) {
        nextCodeletName.replace(pos, 12 /*length of "nextCodelets"*/, "nextSyncCodelets");
    } else {
        pos = nextCodeletName.find("nextSyncCodelets");
        if (pos == string::npos)
            outputStream << "&";
    }

    outputStream << "(" << nextCodeletName << ");\n";
    outputStream
        << "/*Increment sync point's dependency to account for the task to be launched*/\n";
    if (DARTS_BACKEND) {
        outputStream << "nextSyncCodelet->incDep();\n";
    } else if (SWARM_BACKEND) {
        outputStream << "swarm_Dep_unsatisfy(";
        pos = nextCodeletName.find("nextSyncCodelets");
        if (pos == string::npos) {
            outputStream << "&";
        } else {
            pos = nextCodeletName.find("nextCodelets");
            if (pos == string::npos)
                outputStream << "&";
        }
        outputStream << nextCodeletName << "Dep, 1);\n";
    }

    if (this->encapsulateTaskData)
        this->printEncapsulateTaskData(outputStream);

    outputStream << "invoke < TP" << taskID << " > (myTP, 1, this->getID(), myTP, task"
                 << this->taskNode->getID() << "Inputs"
                 << ");\n";

    return;

    DFGNode* taskOmpParentNode = this->taskNode->findParentNode();
    if (check_isa<OMPTaskDirective>(taskOmpParentNode->getStmt()))
        taskOmpParentNode = taskOmpParentNode->findParentNode();

    /*increment the sync point's dependencies to account for the recently launched task.
    Only do it for the first node in the codelet, so we don't do it several times
    in case we have merged tasks, unless the task makes recursive calls.*/
    if (nodeCount == 0 || containsFunctionCall) {
        if (this->mySyncPoint) {
            /*Check if the syncpoint is in the same TP or in the parent*/
            DFGNode* syncPointParentNode = this->mySyncPoint->findParentNode();
            if (check_isa<OMPTaskDirective>(syncPointParentNode->getStmt()))
                syncPointParentNode = syncPointParentNode->findParentNode();

            bool isSyncPointAnOMPExec
                = check_isa<OMPExecutableDirective>(this->mySyncPoint->getStmt());

            string TPParentStr = "";

            bool insideFor = false;
            {
                DFGNode* tempParentNode = this->taskNode->parent;
                while (tempParentNode && check_isa<ForStmt>(tempParentNode->getStmt()) == false
                    && check_isa<WhileStmt>(tempParentNode->getStmt()) == false)
                    tempParentNode = tempParentNode->parent;

                if (tempParentNode && (check_isa<ForStmt>(tempParentNode->getStmt())
                                          || check_isa<WhileStmt>(tempParentNode->getStmt())))
                    insideFor = true;
            }

            if (insideFor) {
                TPParentStr = "TPParent->";
            } else if (taskOmpParentNode && syncPointParentNode
                && taskOmpParentNode->getID() != syncPointParentNode->getID()) {
                if (isSyncPointAnOMPExec) {
                    if (this->mySyncPoint->myRegion->isInlinedRegion() == false) {
                        TPParentStr = "TPParent->";
                    }
                }
            }

            outputStream << "/*Increment the sync point's dependencies to "
                         << "account for the recently launched task.*/\n";
            outputStream << "myTP->controlTPParent->" << TPParentStr << "barrierCodelets"
                         << this->mySyncPoint->getID() << "[0].incDep();\n";

        } else {

            ostringstream tempStream;
            string tempSyncNodeName = taskNode->signalNext(tempStream);

            outputStream << "/*increment the sync point dependencies to "
                         << "account for the recently launched task.*/\n";

            if (tempSyncNodeName.find("nextCodelets") != std::string::npos) {
                if (taskNode->myRegion->singleThreaded)
                    outputStream << tempSyncNodeName << "->incDep();\n";
                else
                    outputStream << tempSyncNodeName << ".incDep();\n";
            } else {
                outputStream << tempSyncNodeName << ".incDep();\n";
            }
        }
    }
}

void TaskInfo::printEncapsulateTaskData(std::ostringstream& outputStream)
{
    TPRegion* parentRegion = taskNode->getRegion()->getParent();
    if (check_isa<OMPExecutableDirective>(parentRegion->getStmt()) == false) {
        DFGNode* parentNode = parentRegion->getMainNode()->findParentNode();
        if (parentNode)
            parentRegion = parentNode->getRegion();
    }

    string prefixStrTask = DARTS_PREFIXSTR_DEFAULT;
    string posfixStrTask = "[this->getID()]";

    outputStream << "/*Encapsulating data for task " << taskNode->getID() << "*/\n";
#if FULL_DEBUG_COMMENTS == 1
    outputStream << "/*parentRegion " << parentRegion->getName() << " " << parentRegion->getID()
                 << "*/\n";
#endif

    if (DARTS_BACKEND) {

		/*Print pointers to the completed flag of tasks this task depends on*/
        for (DFGNode* inDepNode : this->inputDependencyNodes) {

			ostringstream depTaskStream;
			depTaskStream << DARTS_PREFIXSTR_DEFAULT << "task" << inDepNode->getID()
                         << "Inputs[";
            if (parentRegion->singleThreaded == false)
                depTaskStream << "this->getID()";
            else
                depTaskStream << "0";
            depTaskStream << "]";

			outputStream << "bool *dep" << inDepNode->getID() << " = nullptr;\n"
						 << "if(" << depTaskStream.str() << " != nullptr)\n"
						 << "dep" << inDepNode->getID() << " = &(" << depTaskStream.str()
						 << "->taskCompleted);\n";
        }

        outputStream << "_task" << taskNode->getID() << "Inputs *"
                     << "task" << taskNode->getID() << "Inputs ";
        outputStream << " = new _task" << taskNode->getID() << "Inputs(";

        if (check_isa<OMPTaskDirective>(parentRegion->getStmt())
            && parentRegion->getTaskInfo()->encapsulateTaskData) {

            prefixStrTask = DARTS_PREFIXSTR_DEFAULT;
            prefixStrTask += "task";
            prefixStrTask += to_string(parentRegion->getID());
            if (parentRegion->singleThreaded == false) {
                prefixStrTask += "Inputs[this->getID()].";
            } else {
                prefixStrTask += "Inputs[0].";
            }

            posfixStrTask = "";
        }

        if (taskNode->myRegion && taskNode->myRegion->parent
            && taskNode->myRegion->parent->singleThreaded)
            outputStream << taskNode->printOMPInputNamesInFunctionCall(
                parentRegion->getMainNode(), prefixStrTask, false, "");
        else
            outputStream << taskNode->printOMPInputNamesInFunctionCall(
                parentRegion->getMainNode(), prefixStrTask, false, posfixStrTask);

        /*Print pointers to the completed flag of tasks this task depends on*/
        bool printComma = false;
        for (DFGNode* inDepNode : this->inputDependencyNodes) {
            if (printComma || taskNode->ompInputs.size() > 0)
                outputStream << ", ";
            printComma = true;
            outputStream << "dep" << inDepNode->getID();
        }

        if (printComma || taskNode->ompInputs.size() > 0)
            outputStream << ", ";
        outputStream << "nextSyncCodelet";

        outputStream << ");\n";

        outputStream << "/*Save the pointer to the recently created task's data*/\n";
        outputStream << DARTS_PREFIXSTR_DEFAULT << "task" << taskNode->getID() << "Inputs[";
        if (parentRegion->singleThreaded == false)
            outputStream << "this->getID()";
        else
            outputStream << "0";
        outputStream << "] = task" << taskNode->getID() << "Inputs;\n";
    } else if (SWARM_BACKEND) {

        outputStream << "_task" << taskNode->getID() << "Inputs *"
                     << "task" << taskNode->getID() << "InputsTemp "
                     << " = (_task" << taskNode->getID() << "Inputs*)malloc"
                     << "(sizeof(_task" << taskNode->getID() << "Inputs);\n";

        if (check_isa<OMPTaskDirective>(parentRegion->getStmt())
            && parentRegion->getTaskInfo()->encapsulateTaskData) {

            prefixStrTask = "task";
            prefixStrTask += to_string(parentRegion->getID());
            if (parentRegion->singleThreaded == false) {
                prefixStrTask += "Inputs[codeletID].";
            } else {
                prefixStrTask += "Inputs[0].";
            }

            posfixStrTask = "";
        }

        if (taskNode->myRegion && taskNode->myRegion->parent
            && taskNode->myRegion->parent->singleThreaded)
            outputStream << taskNode->printOMPInputNamesInFunctionCall(
                parentRegion->getMainNode(), prefixStrTask, false, "");
        else
            outputStream << taskNode->printOMPInputNamesInFunctionCall(
                parentRegion->getMainNode(), prefixStrTask, false, posfixStrTask);

        /*Print pointers to the completed flag of tasks this task depends on*/
        bool printComma = false;
        for (DFGNode* inDepNode : this->inputDependencyNodes) {
            if (printComma || taskNode->ompInputs.size() > 0)
                outputStream << ", ";
            printComma = true;
            outputStream << "&(task" << inDepNode->getID() << "Inputs[";
            if (parentRegion->singleThreaded == false)
                outputStream << "codeletID";
            else
                outputStream << "0";

            outputStream << "]->taskCompleted)";
        }

        if (printComma || taskNode->ompInputs.size() > 0)
            outputStream << ", ";
        outputStream << "nextSyncCodelet";

        outputStream << ");\n";

        outputStream << "/*Save the pointer to the recently created task's data*/\n";
        outputStream << DARTS_PREFIXSTR_DEFAULT << "task" << taskNode->getID() << "Inputs[";
        if (parentRegion->singleThreaded == false)
            outputStream << "this->getID()";
        else
            outputStream << "0";
        outputStream << "] = task" << taskNode->getID() << "Inputs;\n";
    }
}

void TaskInfo::printTaskDataClass(std::ostringstream& outputStream)
{
    DFGNode* parentNode = taskNode;
    if (check_isa<OMPExecutableDirective>(parentNode->getStmt()) == false)
        parentNode = taskNode->findParentNode();

    TPRegion* parentRegion = taskNode->getRegion()->findOMPOrFunctionRegionParent();
    (void)parentRegion;

    outputStream << "/*Class containing the inputs passed to task" << taskNode->getID() << "*/\n";

    if (DARTS_BACKEND) {
        outputStream << "class _task" << taskNode->getID() << "Inputs{";
        outputStream << "public:\n";

        TPInputs_t::iterator var;
        string varName = "";
        string varType = "";
        string varArray = "";
        OMP_VARSCOPE varScope = OMP_SHARED;

        for (var = taskNode->ompInputs.begin(); var != taskNode->ompInputs.end(); var++) {

            varName = var->first;
            varType = get<0>(var->second);
            varScope = get<1>(var->second);
            varArray = get<4>(var->second);

            if (varScope == OMP_SHARED) {
                if (varArray.compare("") == 0) {
                    outputStream << varType << " *" << varName << "_darts" << parentRegion->getID()
                                 << ";/*OMP_SHARED*/\n";
                } else {
                    outputStream << varType << " *" << varName << "_darts" << parentRegion->getID()
                                 << ";/*OMP_SHARED*/\n";
                    outputStream << "int " << varName << "_outer" << parentRegion->getID()
                                 << "_size;\n";
                }
            } else if (varScope == OMP_FIRSTPRIVATE) {
                if (varArray.compare("") == 0) {
                    outputStream << varType << " " << varName << "_darts" << parentRegion->getID()
                                 << "/*OMP_FIRSTPRIVATE*/;\n";
                } else {
                    outputStream << varType << " *" << varName << "_darts" << parentRegion->getID()
                                 << ";/*OMP_FIRSTPRIVATE*/\n";
                    outputStream << "int " << varName << "_outer" << parentRegion->getID()
                                 << "_size;\n";
                }
            } else if (varScope == OMP_PRIVATE) {
                if (varArray.compare("") == 0) {
                    outputStream << varType << " " << varName << "_darts" << parentRegion->getID()
                                 << "/*OMP_PRIVATE*/;\n";
                } else {
                    outputStream << varType << " *" << varName << "_darts" << parentRegion->getID()
                                 << ";/*OMP_PRIVATE*/\n";
                    outputStream << "int " << varName << "_outer" << parentRegion->getID()
                                 << "_size;\n";
                }
            } else {
                outputStream << varType << " " << varName << "_darts" << parentRegion->getID()
                             << ";\n";
            }
        }

        for (var = taskNode->ompVariables.begin(); var != taskNode->ompVariables.end(); var++) {

            varName = var->first;
            varType = get<0>(var->second);
            varScope = get<1>(var->second);
            varArray = get<4>(var->second);
            outputStream << varType << " " << varName << "_darts" << parentRegion->getID()
                         << varArray << ";/*VARIABLE*/\n";
        }

        if (this->inputDependencyNodes.size() > 0) {
            outputStream << "/*Tasks this task depends on*/\n";
            for (DFGNode* inDepNode : this->inputDependencyNodes) {
                outputStream << "bool *task" << inDepNode->getID() << "Completed;\n";
            }
        }

        outputStream << "/*Termination flag to be used by in the depend clauses*/\n"
                     << "bool taskCompleted;\n";
        outputStream << "/*Synchronization codelet for this task*/\n"
                     << "darts::Codelet *nextSyncCodelet;\n";

        outputStream << "_task" << taskNode->getID() << "Inputs(){}\n";

        outputStream << "_task" << taskNode->getID() << "Inputs(";

        bool printComma = false;
        for (var = taskNode->ompInputs.begin(); var != taskNode->ompInputs.end(); var++) {

            varName = var->first;
            varType = get<0>(var->second);
            varScope = get<1>(var->second);
            varArray = get<4>(var->second);

            if (varScope == OMP_SHARED) {

                if (printComma)
                    outputStream << ", ";
                printComma = true;

                if (varArray.compare("") == 0) {
                    outputStream << varType << " *in_" << varName;
                } else {
                    outputStream << varType << " *in_" << varName;
                    outputStream << ", int in_" << varName << "_outer_size";
                }
            } else if (varScope == OMP_FIRSTPRIVATE) {

                if (printComma)
                    outputStream << ", ";
                printComma = true;

                if (varArray.compare("") == 0) {
                    outputStream << varType << " *in_" << varName;
                } else {
                    outputStream << varType << " *in_" << varName;
                    outputStream << ", int in_" << varName << "_outer_size";
                }
            }
#if 0
			else {
				
				if(printComma)
					outputStream << ", ";
				printComma = true;
				
				if (varArray.compare("") == 0) {
					outputStream << varType << " in_" << varName;
				} else {
					outputStream << varType << " *in_" << varName;
					outputStream << ", int in_" << varName << "_outer_size";
				}
			}
#endif
        }

        for (DFGNode* inDepNode : this->inputDependencyNodes) {
            if (printComma)
                outputStream << ", ";
            printComma = true;
            outputStream << "bool *in_task" << inDepNode->getID() << "Completed";
        }

        if (printComma)
            outputStream << ", ";
        outputStream << "darts::Codelet *in_nextSyncCodelet";
        outputStream << ")";

        printComma = false;
        bool printSemiColon = true;
        for (var = taskNode->ompInputs.begin(); var != taskNode->ompInputs.end(); var++) {

            varName = var->first;
            varScope = get<1>(var->second);
            varArray = get<4>(var->second);
            if (varScope != OMP_FIRSTPRIVATE && varScope != OMP_PRIVATE) {

                if (printSemiColon) {
                    outputStream << ":";
                    printSemiColon = false;
                }

                if (printComma)
                    outputStream << ", ";
                printComma = true;

                outputStream << varName << "_darts" << parentRegion->getID() << "(in_" << varName
                             << ")";
                if (varArray.compare("") != 0)
                    outputStream << ", " << varName << "_outer" << parentRegion->getID()
                                 << "_size(in_" << varName << "_outer_size)";
            }
        }

        for (DFGNode* inDepNode : this->inputDependencyNodes) {

            if (printSemiColon) {
                outputStream << ":";
                printSemiColon = false;
            }

            if (printComma)
                outputStream << ", ";
            printComma = true;

            outputStream << "task" << inDepNode->getID() << "Completed(in_task"
                         << inDepNode->getID() << "Completed)";
        }

        if (printSemiColon) {
            outputStream << ":";
            printSemiColon = false;
        }
        if (printComma)
            outputStream << ", ";
        outputStream << "taskCompleted(false), nextSyncCodelet(in_nextSyncCodelet)";
        outputStream << "\n{\n";

        for (var = taskNode->ompInputs.begin(); var != taskNode->ompInputs.end(); var++) {
            varName = var->first;
            varType = get<0>(var->second);
            varScope = get<1>(var->second);
            varArray = get<4>(var->second);

            if (varScope == OMP_FIRSTPRIVATE) {
                if (varArray.compare("") == 0) {
                    outputStream << "this->" << varName << "_darts" << parentRegion->getID()
                                 << " = "
                                 << " *in_" << varName << ";\n";
                } else {
                    outputStream << "this->" << varName << "_darts" << parentRegion->getID()
                                 << " = "
                                 << "(" << varType << "*)malloc("
                                 << "sizeof(" << varType << ") * in_" << varName
                                 << "_outer_size);\n"
                                 << "for(int i = 0; i < in_" << varName << "_outer_size; i++){\n"
                                 << "this->" << varName << "_darts" << parentRegion->getID()
                                 << "[i] = "
                                 << " in_" << varName << "[i];\n"
                                 << "};\n";
                }
            }
        }
        outputStream << "}\n";

        outputStream << "~_task" << taskNode->getID() << "Inputs(){\n";
        for (var = taskNode->ompInputs.begin(); var != taskNode->ompInputs.end(); var++) {
            varName = var->first;
            varType = get<0>(var->second);
            varScope = get<1>(var->second);
            varArray = get<4>(var->second);

            if (varScope == OMP_FIRSTPRIVATE) {
                if (varArray.compare("") != 0) {
                    outputStream << "free(" << varName << "_darts" << parentRegion->getID()
                                 << ");\n";
                }
            }
        }
        outputStream << "}\n";

        outputStream << "};\n";
    } else if (SWARM_BACKEND) {
        outputStream << "typedef struct _task" << taskNode->getID() << "Inputs_st{";

        TPInputs_t::iterator var;
        string varName = "";
        string varType = "";
        string varArray = "";
        OMP_VARSCOPE varScope = OMP_SHARED;

        for (var = taskNode->ompInputs.begin(); var != taskNode->ompInputs.end(); var++) {

            varName = var->first;
            varType = get<0>(var->second);
            varScope = get<1>(var->second);
            varArray = get<4>(var->second);

            if (varScope == OMP_SHARED) {
                if (varArray.compare("") == 0) {
                    outputStream << varType << " *" << varName << suffixLang
                                 << parentRegion->getID() << ";/*OMP_SHARED*/\n";
                } else {
                    outputStream << varType << " *" << varName << suffixLang
                                 << parentRegion->getID() << ";/*OMP_SHARED*/\n";
                    outputStream << "int " << varName << "_outer" << parentRegion->getID()
                                 << "_size;\n";
                }
            } else if (varScope == OMP_FIRSTPRIVATE) {
                if (varArray.compare("") == 0) {
                    outputStream << varType << " " << varName << suffixLang << parentRegion->getID()
                                 << "/*OMP_FIRSTPRIVATE*/;\n";
                } else {
                    outputStream << varType << " *" << varName << suffixLang
                                 << parentRegion->getID() << ";/*OMP_FIRSTPRIVATE*/\n";
                    outputStream << "int " << varName << "_outer" << parentRegion->getID()
                                 << "_size;\n";
                }
            } else {
                outputStream << varType << " " << varName << suffixLang << parentRegion->getID()
                             << ";\n";
            }
        }

        for (var = taskNode->ompVariables.begin(); var != taskNode->ompVariables.end(); var++) {

            varName = var->first;
            varType = get<0>(var->second);
            varScope = get<1>(var->second);

            outputStream << varType << " " << varName << suffixLang << parentRegion->getID()
                         << ";\n";
        }

        if (this->inputDependencyNodes.size() > 0) {
            outputStream << "/*Tasks this task depends on*/\n";
            for (DFGNode* inDepNode : this->inputDependencyNodes) {
                outputStream << "bool *task" << inDepNode->getID() << "Completed;\n";
            }
        }
        outputStream << "/*Termination flag to be used by in the depend clauses*/\n"
                     << "bool taskCompleted;\n";
        outputStream << "/*Synchronization codelet for this task*/\n"
                     << "swarm_Codelet_t *nextSyncCodelet;\n";

        outputStream << "}_task" << taskNode->getID() << "Inputs_t;\n";

        outputStream << "void task" << taskNode->getID() << "InputsInit("
                     << "_task" << taskNode->getID() << "Inputs_t *taskInputs";

        for (var = taskNode->ompInputs.begin(); var != taskNode->ompInputs.end(); var++) {

            varName = var->first;
            varType = get<0>(var->second);
            varScope = get<1>(var->second);
            varArray = get<4>(var->second);

            outputStream << ", ";

            if (varScope == OMP_SHARED) {
                if (varArray.compare("") == 0) {
                    outputStream << varType << " *in_" << varName;
                } else {
                    outputStream << varType << " *in_" << varName;
                    outputStream << ", int in_" << varName << "_outer_size";
                }
            } else if (varScope == OMP_FIRSTPRIVATE) {
                if (varArray.compare("") == 0) {
                    outputStream << varType << " *in_" << varName;
                } else {
                    outputStream << varType << " *in_" << varName;
                    outputStream << ", int in_" << varName << "_outer_size";
                }
            } else {
                if (varArray.compare("") == 0) {
                    outputStream << varType << " in_" << varName;
                } else {
                    outputStream << varType << " *in_" << varName;
                    outputStream << ", int in_" << varName << "_outer_size";
                }
            }
        }

        if (this->inputDependencyNodes.size() > 0) {
            if (taskNode->ompInputs.size() > 0)
                outputStream << ", ";
            bool printComma = false;
            for (DFGNode* inDepNode : this->inputDependencyNodes) {
                if (printComma)
                    outputStream << ", ";
                printComma = true;
                outputStream << "bool *in_task" << inDepNode->getID() << "Completed";
            }
        }

        if (this->inputDependencyNodes.size() > 0 || taskNode->ompInputs.size() > 0)
            outputStream << ", ";

        outputStream << "swarm_Codelet_t *in_nextSyncCodelet";

        outputStream << ")";
        outputStream << "\n{\n";

        for (var = taskNode->ompInputs.begin(); var != taskNode->ompInputs.end(); var++) {
            varName = var->first;
            varScope = get<1>(var->second);
            varArray = get<4>(var->second);
            if (varScope != OMP_FIRSTPRIVATE) {
                outputStream << "taskInputs->" << varName << suffixLang << parentRegion->getID()
                             << " = in_" << varName << ";\n";
                if (varArray.compare("") != 0)
                    outputStream << "taskInputs->" << varName << "_outer" << parentRegion->getID()
                                 << "_size = in_" << varName << "_outer_size;\n";
            }
        }

        if (this->inputDependencyNodes.size() > 0) {
            for (DFGNode* inDepNode : this->inputDependencyNodes) {
                outputStream << "taskInputs->"
                             << "task" << inDepNode->getID() << "Completed = in_task"
                             << inDepNode->getID() << "Completed;\n";
            }
        }

        outputStream << "taskInputs->"
                     << "taskCompleted = false;\n"
                     << "taskInputs->"
                     << "nextSyncCodelet = in_nextSyncCodelet;\n";

        for (var = taskNode->ompInputs.begin(); var != taskNode->ompInputs.end(); var++) {
            varName = var->first;
            varType = get<0>(var->second);
            varScope = get<1>(var->second);
            varArray = get<4>(var->second);

            if (varScope == OMP_FIRSTPRIVATE) {
                if (varArray.compare("") == 0) {
                    outputStream << "taskInputs->" << varName << suffixLang << parentRegion->getID()
                                 << " = "
                                 << " *in_" << varName << ";\n";
                } else {
                    outputStream << "taskInputs->" << varName << suffixLang << parentRegion->getID()
                                 << " = "
                                 << "(" << varType << "*)malloc("
                                 << "sizeof(" << varType << ") * in_" << varName
                                 << "_outer_size);\n"
                                 << "for(int i = 0; i < in_" << varName << "_outer_size; i++){\n"
                                 << "taskInputs->" << varName << suffixLang << parentRegion->getID()
                                 << "[i] = "
                                 << " in_" << varName << "[i];\n"
                                 << "};\n";
                }
            }
        }
        outputStream << "}\n";
    }
}

void TaskInfo::setSyncPoints()
{
    /*Get the syncpoint node*/
    DFGNode* syncPoint = this->taskNode->findMySyncPoint();

    /*Now get the syncpoint node's region*/
    if (syncPoint) {
        this->taskNode->myTaskInfo->mySyncPoint = syncPoint;
        syncPoint->tasksToExecute.push_back(taskNode);

        if (check_isa<OMPParallelDirective>(syncPoint->getStmt()) == false) {
            DFGNode* parallelParent = syncPoint->findOMPParallelParent();
            if (parallelParent == nullptr) {
                parallelParent = syncPoint->findFunctionParent();
            }
            taskNode->myTaskInfo->mySyncPointRegion = parallelParent->myRegion;
        } else {
            taskNode->myTaskInfo->mySyncPointRegion = syncPoint->myRegion;
        }
    }
    return;
}

void TaskInfo::insertDependClauseVar(std::string varName, unsigned int depType)
{
    if (depType == 0)
        this->dependClauseInVars.push_back(varName);
    else if (depType == 1)
        this->dependClauseOutVars.push_back(varName);
    else if (depType == 2)
        this->dependClauseInOutVars.push_back(varName);
}

bool TaskInfo::findDependClauseVar(std::string varName, unsigned int depType)
{
    bool found = false;
    if (depType == 0) {
        for (string var : dependClauseInVars) {
            if (var.compare(varName) == 0) {
                found = true;
                break;
            }
        }
    } else if (depType == 1) {
        for (string var : dependClauseOutVars) {
            if (var.compare(varName) == 0) {
                found = true;
                break;
            }
        }
    }
    if (depType == 2) {
        for (string var : dependClauseInOutVars) {
            if (var.compare(varName) == 0) {
                found = true;
                break;
            }
        }
    }
    return found;
}

void TaskInfo::insertInputDependencyNode(DFGNode* node)
{
    this->inputDependencyNodes.push_back(node);
}

void TaskInfo::insertOutputDependencyNode(DFGNode* node)
{
    this->outputDependencyNodes.push_back(node);
}

void TaskInfo::printInputDependencyNodes(std::ostringstream& outputStream, std::string prefixStr,
    std::string postfixStr, std::string frontDelimiter, std::string backDelimiter)
{
    if (this->inputDependencyNodes.size() > 0) {
        outputStream << frontDelimiter;
        for (DFGNode* inDepNode : this->inputDependencyNodes) {
            outputStream << prefixStr << inDepNode->getID() << postfixStr << backDelimiter;
        }
    }
}

void TaskInfo::printOutputDependencyNodes(std::ostringstream& outputStream, std::string prefixStr,
    std::string postfixStr, std::string frontDelimiter, std::string backDelimiter)
{
    if (this->outputDependencyNodes.size() > 0) {
        outputStream << frontDelimiter;
        for (DFGNode* outDepNode : this->outputDependencyNodes) {
            outputStream << prefixStr << outDepNode->getID() << postfixStr << backDelimiter;
        }
    }
}
}
