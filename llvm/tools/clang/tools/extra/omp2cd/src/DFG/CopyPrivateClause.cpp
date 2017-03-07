#include "include/DFG/CopyPrivateClause.h"
#include "include/DFG/DFG.h"
#include "include/DFG/DFGNode.h"
#include "include/DFG/TPRegion.h"

using namespace llvm;
using namespace clang;
using namespace clang::tooling;
using namespace std;

namespace omp2cd_space {

/* Copy the values calculated by the thread executing a single region
* to the other thread's vars. This function is called at the end of the
* single region*/
void CopyPrivateClause::copyValuesToVars(std::ostringstream& outputStream)
{
    if (this->vars.size() > 0) {
        outputStream << "/*Copy the values calculated by the single region\n"
                     << "to the other threads' vars specified in the\n"
                     << "copyprivate clause*/\n";

        int parentInputsRegionID = 0;
        TPRegion* parentRegion = this->ompNode->getRegion()->findOMPOrFunctionRegionParent();
        TPRegion* parentParentRegion = nullptr;
        if (parentRegion && parentRegion->parent
            && parentRegion->parent->findOMPOrFunctionRegionParent()) {
            parentParentRegion = parentRegion->parent->findOMPOrFunctionRegionParent();
            parentInputsRegionID = parentRegion->parent->findOMPOrFunctionRegionParent()->getID();
        }

        string prefixStr = "";
        if (DARTS_BACKEND)
            prefixStr = DARTS_PREFIXSTR_DEFAULT;

        ostringstream postfixStrTemp;
        if (DARTS_BACKEND)
            postfixStrTemp << "[" << prefixStr << "codeletIDexecutingSingle"
                           << this->ompNode->getID() << "]";
        else if (SWARM_BACKEND)
            postfixStrTemp << "[codeletIDexecutingSingle" << this->ompNode->getID() << "]";

        string postfixStr = postfixStrTemp.str();

        int sourceID = parentInputsRegionID;
        if (this->ompNode->getRegion()->isInlinedRegion())
            sourceID = this->ompNode->getID();

        DFGNode* sourceNode = parentParentRegion->mainNode;
        if (this->ompNode->getRegion()->isInlinedRegion())
            sourceNode = this->ompNode;

        if (DARTS_BACKEND)
            outputStream << "if(" << prefixStr << "codeletIDexecutingSingle"
                         << this->ompNode->getID() << " < ompNumThreads)\n"
                         << "{\n";
        else if (SWARM_BACKEND)
            outputStream << "if(codeletIDexecutingSingle" << this->ompNode->getID()
                         << " < ompNumThreads)\n"
                         << "{\n";

        vector<string> copyVarStrVec;
        /* Create temp vars for the single region's vars. */
        for (auto var : this->vars) {
            ostringstream copyVarStr;
            ostringstream tempVarStr;

            string typeStr = "";

            bool isThreadPrivate = false;
            int parentNodeID = 0;
            string varArray = "";
            string varSize = "";
            VarTableTuple_t varTuple;
            bool found
                = this->ompNode->getDFG()->getVarTupleFromTable(this->ompNode, var.first, varTuple);
            if (found) {
                typeStr = get<0>(varTuple);
                isThreadPrivate = get<4>(varTuple);
                parentNodeID = get<5>(varTuple);
                varArray = get<6>(varTuple);
                varSize = get<8>(varTuple);
            }

            /* Look for the input in the source node's inputs and vars */
            TPInputs_t::iterator srcIt = sourceNode->ompInputs.find(var.first);
            TPInputs_t::iterator srcItVar = sourceNode->ompVariables.find(var.first);
            if (srcIt != sourceNode->ompInputs.end()) {
                if (get<4>(srcIt->second).find("[") == string::npos) {

                    tempVarStr << typeStr << " temp_" << var.first << " = ";

                    if (get<1>(srcIt->second) == OMP_SHARED) {

                        outputStream << tempVarStr.str() << "*(" << prefixStr << var.first
                                     << suffixLang << sourceID << ");\n";

                    } else if (get<1>(srcIt->second) == OMP_SHARED_PRIVATE) {

                        if (sourceNode->getRegion()->singleThreaded) {

                            outputStream << tempVarStr.str() << "*(" << prefixStr << var.first
                                         << suffixLang << sourceID << ");\n";
                        } else {
                            outputStream << tempVarStr.str() << "*(" << prefixStr << var.first
                                         << suffixLang << sourceID << postfixStr << ");\n";
                        }

                    } else {
                        if (sourceNode->getRegion()->singleThreaded) {

                            outputStream << tempVarStr.str() << "(" << prefixStr << var.first
                                         << suffixLang << sourceID << ");\n";

                        } else {

                            outputStream << tempVarStr.str() << "(" << prefixStr << var.first
                                         << suffixLang << sourceID << postfixStr << ");\n";
                        }
                    }
                } else {

                    tempVarStr << typeStr << " temp_" << var.first << " = ";

                    if (get<1>(srcIt->second) == OMP_SHARED) {

                        copyVarStr << tempVarStr.str() << "*(" << prefixStr << var.first
                                   << suffixLang << sourceID << ")";

                    } else if (get<1>(srcIt->second) == OMP_SHARED_PRIVATE) {

                        if (sourceNode->getRegion()->singleThreaded) {

                            copyVarStr << tempVarStr.str() << "*(" << prefixStr << var.first
                                       << suffixLang << sourceID << ")";

                        } else {

                            copyVarStr << tempVarStr.str() << "*(" << prefixStr << var.first
                                       << suffixLang << sourceID << postfixStr << "[k])";
                        }
                    } else {
                        if (sourceNode->getRegion()->singleThreaded) {

                            copyVarStr << tempVarStr.str() << "(" << prefixStr << var.first
                                       << suffixLang << sourceID << ")";

                        } else {

                            copyVarStr << tempVarStr.str() << "(" << prefixStr << var.first
                                       << suffixLang << sourceID << postfixStr << "[k])";
                        }
                    }
                }
            }
            /* Check if it's in the vars */
            else if (srcItVar != sourceNode->ompVariables.end()) {
                tempVarStr << typeStr << " temp_" << var.first << " = ";
                if (get<4>(srcItVar->second).find("[") == string::npos) {
                    if (get<1>(srcItVar->second) == OMP_SHARED) {

                        outputStream << tempVarStr.str() << "*(" << prefixStr << var.first
                                     << suffixLang << sourceID << ");\n";

                    } else if (get<1>(srcItVar->second) == OMP_SHARED_PRIVATE) {

                        if (sourceNode->getRegion()->singleThreaded) {

                            outputStream << tempVarStr.str() << "*(" << prefixStr << var.first
                                         << suffixLang << sourceID << ");\n";

                        } else {

                            outputStream << tempVarStr.str() << "*(" << prefixStr << var.first
                                         << suffixLang << sourceID << postfixStr << ");\n";
                        }

                    } else if (get<1>(srcItVar->second) == OMP_PRIVATE) {

                        if (sourceNode->getRegion()->singleThreaded) {

                            outputStream << tempVarStr.str() << "(" << prefixStr << var.first
                                         << suffixLang << sourceID << ");\n";

                        } else {

                            outputStream << tempVarStr.str() << "(" << prefixStr << var.first
                                         << suffixLang << sourceID << postfixStr << ");\n";
                        }
                    }
                } else {
                    if (get<1>(srcItVar->second) == OMP_SHARED) {

                        copyVarStr << tempVarStr.str() << "*(" << prefixStr << var.first
                                   << suffixLang << sourceID << ")";

                    } else if (get<1>(srcItVar->second) == OMP_SHARED_PRIVATE) {

                        if (sourceNode->getRegion()->singleThreaded) {

                            copyVarStr << tempVarStr.str() << "*(" << prefixStr << var.first
                                       << suffixLang << sourceID << ")";

                        } else {

                            copyVarStr << tempVarStr.str() << "*(" << prefixStr << var.first
                                       << suffixLang << sourceID << postfixStr << ")[k]";
                        }

                    } else if (get<1>(srcItVar->second) == OMP_PRIVATE) {

                        if (sourceNode->getRegion()->singleThreaded) {

                            copyVarStr << tempVarStr.str() << "(" << prefixStr << var.first
                                       << suffixLang << sourceID << ")";

                        } else {

                            copyVarStr << tempVarStr.str() << "(" << prefixStr << var.first
                                       << suffixLang << sourceID << postfixStr << ")[k]";
                        }
                    }
                }
            }
            /* If in neither one, then it must be a global var or threadprivate */
            else {
                if (found && isThreadPrivate) {
                    if (varArray.compare("") == 0) {

                        outputStream << typeStr << " temp_" << var.first << " = "
                                     << "(" << var.first << suffixLang << "ThreadPriv"
                                     << parentNodeID << "[" << prefixStr
                                     << "codeletIDexecutingSingle" << this->ompNode->getID()
                                     << "]);\n";

                    } else {

                        copyVarStr << typeStr << " temp_" << var.first << " = "
                                   << "(" << var.first << suffixLang << "ThreadPriv" << parentNodeID
                                   << "[" << prefixStr << "codeletIDexecutingSingle"
                                   << this->ompNode->getID() << "][k])";
                    }
                }
            }
            copyVarStrVec.push_back(copyVarStr.str());
        }

        /* Copy the temp vars to the parent's vars. */
        if (DARTS_BACKEND)
            outputStream << "for(size_t codeletCounter = 0; codeletCounter < " << prefixStr
                         << "numThreads; codeletCounter++)\n";
        else if (SWARM_BACKEND)
            outputStream << "for(size_t codeletCounter = 0; codeletCounter < " << prefixStr
                         << "numThreads" << parentParentRegion->getID() << "; codeletCounter++)\n";

        outputStream << "{\n"
                     << "if(codeletCounter != " << prefixStr << "codeletIDexecutingSingle"
                     << this->ompNode->getID() << ")\n"
                     << "{\n";

        DFGNode* destNode = parentParentRegion->mainNode;

        /* To write the value in the other threads' vars, we dont check
         * for singleThreaded since the parent of the single region should
         * have several threads(true?)*/
        for (auto var : this->vars) {
            /* Look for the input in the destination node's inputs and vars */
            TPInputs_t::iterator destIt = destNode->ompInputs.find(var.first);
            TPInputs_t::iterator destItVar = destNode->ompVariables.find(var.first);

            if (destIt != destNode->ompInputs.end()) {
                if (get<4>(destIt->second).find("[") == string::npos) {
                    if (get<1>(destIt->second) == OMP_SHARED) {
                        outputStream << "*(" << prefixStr << var.first << suffixLang
                                     << parentInputsRegionID << ")= temp_" << var.first << ";\n";
                    } else if (get<1>(destIt->second) == OMP_SHARED_PRIVATE) {
                        outputStream << "*(" << prefixStr << var.first << suffixLang
                                     << parentInputsRegionID << "[codeletCounter])= temp_"
                                     << var.first << ";\n";
                    } else {
                        outputStream << "(" << prefixStr << var.first << suffixLang
                                     << parentInputsRegionID << "[codeletCounter])= temp_"
                                     << var.first << ";\n";
                    }
                } else {
                    size_t pos = get<4>(destIt->second).find("[");
                    size_t pos1 = get<4>(destIt->second).find("]");
                    string limit = get<4>(destIt->second).substr(pos + 1, pos1 - pos - 1);
                    outputStream << "for (int k = 0; k < " << limit << "; k++){\n";

                    for (string copyVarStr : copyVarStrVec) {

                        outputStream << copyVarStr << ";\n";

                        if (get<1>(destIt->second) == OMP_SHARED) {
                            outputStream << "*(" << prefixStr << var.first << suffixLang
                                         << parentInputsRegionID << ")= temp_" << var.first
                                         << ";\n";
                        } else if (get<1>(destIt->second) == OMP_SHARED_PRIVATE) {
                            outputStream << "*(" << prefixStr << var.first << suffixLang
                                         << parentInputsRegionID << "[codeletCounter][k])= temp_"
                                         << var.first << ";\n";
                        } else {
                            outputStream << "(" << prefixStr << var.first << suffixLang
                                         << parentInputsRegionID << "[codeletCounter][k])= temp_"
                                         << var.first << ";\n";
                        }
                    }
                    outputStream << "}\n";
                }
            }
            /* Check if it's in the vars */
            else if (destItVar != destNode->ompVariables.end()) {
                if (get<1>(destItVar->second) == OMP_SHARED) {
                    outputStream << "*(" << prefixStr << var.first << suffixLang
                                 << parentInputsRegionID << ")= temp_" << var.first << ";\n";
                }
                if (get<1>(destItVar->second) == OMP_PRIVATE) {
                    outputStream << "(" << prefixStr << var.first << suffixLang
                                 << parentInputsRegionID << "[codeletCounter])= temp_" << var.first
                                 << ";\n";
                } else if (get<1>(destItVar->second) == OMP_SHARED_PRIVATE) {
                    outputStream << "*(" << prefixStr << var.first << suffixLang
                                 << parentInputsRegionID << "[codeletCounter])= "
                                 << "temp_" << var.first << ";\n";
                }
            }
            /* If in neither one, then it must be a global var or threadprivate */
            else {
                bool isThreadPrivate = false;
                int parentNodeID = 0;
                string varArray = "";
                string varSize = "";
                VarTableTuple_t varTuple;
                bool found = this->ompNode->getDFG()->getVarTupleFromTable(
                    this->ompNode, var.first, varTuple);
                if (found) {
                    isThreadPrivate = get<4>(varTuple);
                    parentNodeID = get<5>(varTuple);
                    varArray = get<6>(varTuple);
                    varSize = get<8>(varTuple);
                }

                if (found && isThreadPrivate) {

                    if (varArray.compare("") == 0) {
                        outputStream << "(" << var.first << suffixLang << "ThreadPriv"
                                     << parentNodeID << "[codeletCounter])= temp_" << var.first
                                     << ";\n";
                    } else {
                        for (string copyVarStr : copyVarStrVec) {
                            outputStream << copyVarStr << ";\n"
                                         << "for (int k = 0; k < " << varSize << "; k++){\n"
                                         << "(" << var.first << suffixLang << "ThreadPriv"
                                         << parentNodeID << "[codeletCounter][k])= temp_"
                                         << var.first << ";\n"
                                         << "}\n";
                        }
                    }
                }
            }
        }
        outputStream << "}\n"
                     << "}\n"
                     << "}\n";
    }
}
}
