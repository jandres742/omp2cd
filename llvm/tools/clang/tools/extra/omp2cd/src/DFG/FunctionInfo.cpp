#include "include/DFG/FunctionInfo.h"
#include "include/DFG/DFGNode.h"
#include "include/DFG/TPRegion.h"

using namespace llvm;
using namespace clang;
using namespace clang::tooling;
using namespace std;

namespace omp2cd_space {

FunctionInfo::FunctionInfo(clang::FunctionDecl* f, DFGNode* node)
    : node(node)
    , f(f)
    , isOmp2dartsFunction(false)
    , numOfArgs(0)
{
    clang::QualType returnQualType = this->f->getReturnType();
    returnType = returnQualType.getAsString();
}

void FunctionInfo::printInterface(std::ostringstream& outputStream,
    std::string extraArgs, bool printBody, std::string backEnd)
{
    this->getFunctionDecl()->setPrintBodyFlag(printBody);
    this->getFunctionDecl()->setBackEnd(backEnd);
    string ostreamString;
    raw_string_ostream rawStringOStream(ostreamString);
    this->getFunctionDecl()->print(rawStringOStream);
    outputStream << rawStringOStream.str();
}

/* Print the function's args, either with types or not. By defaults, it prints them.*/
void FunctionInfo::printArguments(std::ostringstream& outputStream, bool printTypes)
{
    if (this->f->param_size() > 0) {
        clang::FunctionDecl::param_iterator paramIterator = this->f->param_begin();

        if (printTypes)
            outputStream << getTypeFromDecl(*paramIterator);

        outputStream << " " << getNameFromDecl(*paramIterator);

        paramIterator++;

        for (; paramIterator != this->f->param_end(); paramIterator++) {
            outputStream << ", ";
            if (printTypes)
                outputStream << getTypeFromDecl(*paramIterator);

            outputStream << " " << getNameFromDecl(*paramIterator);
        }
    }
}

/* Print an arg's name given its position*/
std::string FunctionInfo::getParamStr(int numArg)
{
    string paramStr = "";
    if (this->f->param_size() > 0) {
        ParmVarDecl* param = this->f->getParamDecl(numArg);
        paramStr = getNameFromDecl(param);
    }
    return paramStr;
}

/* Print the setNewInputs() function declaration. This function belongs
* to each function's TP and is used by each codelet to copy its own
* args values.*/
void FunctionInfo::printSetNewInputsFunctionDecl(std::ostringstream& tempStream,
    omp2cd_space::
        FunctionTable_t functionTable)
{
    if (this->isOMPDARTSFunction(functionTable) == false)
        return;

    DFGNode* functionNode = this->node;
    if (functionNode->getFunctionInfo() == nullptr)
        functionNode = functionNode->findFunctionParent();

    if (functionNode) {
        if (functionNode->ompVariables.size() > 0) {
            /* Since the function's args and the variables declared inside are 
			 * both treated as vars, we need to figure out how many of those 
			 * vars are actually args, using get<2>(var->second), which is not -1
			 * for all args.*/
            OMP_VARINFO initTuple("", OMP_SHARED, -1, "", "", false);
            pair<string, OMP_VARINFO> initPair("none", initTuple);
            vector<pair<string,
                OMP_VARINFO> >
                orderedVars(functionNode->ompVariables.size(),
                    initPair);
            for (auto var = functionNode->ompVariables.cbegin();
                 var != functionNode->ompVariables.cend(); ++var) {
                OMP_VARINFO newTuple(get<0>(var->second),
                    get<1>(var->second),
                    get<2>(var->second),
                    get<3>(var->second),
                    get<4>(var->second), false);
                std::pair<std::string,
                    omp2cd_space::OMP_VARINFO>
                    newPair(var->first,
                        newTuple);
                if (get<2>(var->second) >= 0) {
                    orderedVars[get<2>(var->second)] = newPair;
                } else {
                    orderedVars.push_back(newPair);
                }
            }

            int numOfInputs = 0;
            for (auto var : orderedVars) {
                if (var.first.compare("none") != 0
                    && get<2>(var.second) >= 0) {
                    numOfInputs++;
                }
            }

            /* If the num of args is greater than 0, print the function. */
            if (numOfInputs > 0) {
                tempStream << "void setNewInputs(";
                this->node->printFunctionInputsAsParams(false, tempStream);

                if (this->returnType.compare("void") != 0) {
                    tempStream << ", " << this->returnType << "* in_functionResult";
                }
                tempStream << ", size_t codeletID);";
            }
        }
    }
}

void FunctionInfo::printSetNewInputsFunction(std::ostringstream& tempStream,
    omp2cd_space::FunctionTable_t functionTable)
{
    if (this->isOMPDARTSFunction(functionTable) == false)
        return;

    DFGNode* functionNode = this->node;

    string inputsRegionID = "";
    TPRegion* parentRegion = functionNode->getRegion()->findOMPOrFunctionRegionParent();
    if (parentRegion)
        inputsRegionID += to_string(parentRegion->getID());

    if (functionNode->myFunctionInfo == nullptr)
        functionNode = functionNode->findFunctionParent();

    if (functionNode) {
        if (functionNode->ompVariables.size() > 0) {
            OMP_VARINFO initTuple("", OMP_SHARED, -1, "", "", false);
            pair<string, OMP_VARINFO> initPair("none", initTuple);
            vector<pair<string,
                OMP_VARINFO> >
                orderedVars(functionNode->ompVariables.size(),
                    initPair);
            for (auto var = functionNode->ompVariables.cbegin();
                 var != functionNode->ompVariables.cend(); ++var) {
                OMP_VARINFO newTuple(get<0>(var->second),
                    get<1>(var->second),
                    get<2>(var->second),
                    get<3>(var->second),
                    get<4>(var->second), false);
                std::pair<std::string,
                    omp2cd_space::OMP_VARINFO>
                    newPair(var->first,
                        newTuple);
                if (get<2>(var->second) >= 0) {
                    orderedVars[get<2>(var->second)] = newPair;
                } else {
                    orderedVars.push_back(newPair);
                }
            }

            int numOfInputs = 0;
            for (auto var : orderedVars) {
                if (var.first.compare("none") != 0
                    && get<2>(var.second) >= 0) {
                    numOfInputs++;
                }
            }

            if (numOfInputs > 0) {
                tempStream << "void TP" << this->node->getID() << "::setNewInputs(";
                this->node->printFunctionInputsAsParams(false, tempStream);
                if (this->returnType.compare("void") != 0) {
                    tempStream << ", " << this->returnType << "* in_functionResult";
                }
                tempStream << ", size_t codeletID){\n";

                for (auto var : orderedVars) {
                    if (var.first.compare("none") != 0
                        && get<2>(var.second) >= 0) {
                        tempStream << "this->" << var.first << "_darts" << inputsRegionID << "[codeletID]= in_" << var.first << ";\n";
                    }
                }

                if (this->returnType.compare("void") != 0) {
                    tempStream << "this->functionResult[codeletID] = in_functionResult;\n";
                }
                tempStream << "}\n";
            }
        }
    }
}

bool FunctionInfo::isOMPDARTSFunction(omp2cd_space::FunctionTable_t functionTable)
{
    bool ret = false;

    string funcName = this->node->getName();

    FunctionTable_t::iterator it = functionTable.find(funcName);
    if (it != functionTable.end())
        isOmp2dartsFunction = get<1>(it->second);

    if (isOmp2dartsFunction)
        ret = true;

    return ret;
}

void FunctionInfo::printFunctionInputsAndVarsDefs(std::ostringstream&
                                                      tempStream,
    omp2cd_space::
        FunctionTable_t functionTable)
{
    DFGNode* functionNode = this->node;

    if (DARTS_BACKEND) {
        if (functionNode->myRegion && functionNode->myRegion->singleThreaded)
            tempStream << "darts::Codelet* nextCodelets" << functionNode->getName() << ";\n"
                       << "darts::Codelet* nextSyncCodelets" << functionNode->getName() << ";\n";
        else
            tempStream << "darts::Codelet** nextCodelets" << functionNode->getName() << ";\n"
                       << "darts::Codelet** nextSyncCodelets" << functionNode->getName() << ";\n";
    } else if (SWARM_BACKEND) {
        tempStream << "const swarm_Codelet_t *nextCodelets" << this->node->getName() << "[MAXNUMTHREADS];\n";
        tempStream << "void *nextCodeletContext" << this->node->getName() << "[MAXNUMTHREADS];\n";
    }

    string inputsRegionID = "";
    TPRegion* parentRegion = functionNode->getRegion()->findOMPOrFunctionRegionParent();

    if (parentRegion)
        inputsRegionID += to_string(parentRegion->getID());

    if (functionNode) {
        if (functionNode->ompVariables.size() > 0) {
            OMP_VARINFO initTuple("", OMP_SHARED, -1, "", "", false);
            pair<string, OMP_VARINFO> initPair("none", initTuple);
            vector<pair<string, OMP_VARINFO> > orderedVars(functionNode->ompVariables.size(), initPair);

            /*First order the vars so they followed the same order as in the
			function's interface*/
            for (auto var = functionNode->ompVariables.cbegin();
                 var != functionNode->ompVariables.cend(); ++var) {
                OMP_VARINFO newTuple(get<0>(var->second), get<1>(var->second),
                    get<2>(var->second), get<3>(var->second),
                    get<4>(var->second), false);
                std::pair<std::string, omp2cd_space::OMP_VARINFO> newPair(var->first, newTuple);

                if (get<2>(var->second) >= 0)
                    orderedVars[get<2>(var->second)] = newPair;
                else
                    orderedVars.push_back(newPair);
            }

            for (auto var : orderedVars) {
                /* Dont declare threadprivate vars or static variables */
                VarTableTuple_t varTuple;
                bool found = this->node->myDFG->getVarTupleFromTable(this->node, var.first, varTuple);

                if (found)
                    if (get<4>(varTuple) || get<2>(varTuple).compare("static") == 0)
                        continue;

                string varType = get<0>(var.second);
                string varArray = get<6>(varTuple);
                if (var.first.compare("none") != 0) {
                    if (functionNode->myRegion && functionNode->myRegion->singleThreaded) {
                        tempStream << varType << " " << var.first << suffixLang << inputsRegionID << " " << varArray << "/*VARIABLE*/;\n";
                    } else {
                        if (varArray.compare("") == 0) {
                            if (DARTS_BACKEND) {
                                tempStream << varType << "* " << var.first << suffixLang << inputsRegionID << "/*VARIABLE*/;\n";
                            } else if (SWARM_BACKEND) {
                                tempStream << varType << " " << var.first << suffixLang << this->node->getID() << "[MAXNUMTHREADS]/*VARIABLE*/;\n";
                            }
                        } else {
                            if (DARTS_BACKEND) {
                                tempStream << varType << " **" << var.first << suffixLang << inputsRegionID << "/*VARIABLE*/;\n";
                                tempStream << "int " << var.first << "_outer" << inputsRegionID << "_size;\n";
                            } else if (SWARM_BACKEND) {
                                tempStream << varType << " *" << var.first << suffixLang << inputsRegionID << "[MAXNUMTHREADS]/*VARIABLE*/;\n";
                                tempStream << "int " << var.first << "_outer" << inputsRegionID << "_size;\n";
                            }
                        }
                    }
                }
            }
        }

        if (this->isOMPDARTSFunction(functionTable)
            && this->returnType.compare("void") != 0
            && this->node->getName().compare("main") != 0) {
            if (functionNode->myRegion && functionNode->myRegion->singleThreaded) {
                if (DARTS_BACKEND) {
                    tempStream << this->returnType << "* functionResult;\n";
                } else if (SWARM_BACKEND) {
                    tempStream << this->returnType << " functionResult" << this->node->getName() << "[MAXNUMTHREADS];\n";
                }
            } else {
                if (DARTS_BACKEND) {
                    tempStream << this->returnType << "** functionResult;\n";
                } else if (SWARM_BACKEND) {
                    tempStream << this->returnType << "* functionResult" << this->node->getName() << "[MAXNUMTHREADS];\n";
                }
            }
        }
    }
}

void FunctionInfo::printFunctionInputsAndVarsInits(std::ostringstream& tempStream,
    omp2cd_space::FunctionTable_t functionTable)
{
    DFGNode* functionNode = this->node;

    if (DARTS_BACKEND) {
        if (functionNode->myRegion && functionNode->myRegion->singleThreaded)
            tempStream << ", nextCodelets" << functionNode->getName() << "(in_mainNextCodelet)\n"
                       << ", nextSyncCodelets" << functionNode->getName() << "(in_mainSyncCodelet)\n";
        else
            tempStream << ", nextCodelets" << functionNode->getName() << "( new Codelet*[in_numThreads])\n"
                       << ", nextSyncCodelets" << functionNode->getName() << "( new Codelet*[in_numThreads])\n";
    }

    string inputsRegionID = "";
    TPRegion* parentRegion = functionNode->getRegion()->findOMPOrFunctionRegionParent();
    if (parentRegion)
        inputsRegionID += to_string(parentRegion->getID());

    if (functionNode) {
        if (functionNode->ompVariables.size() > 0) {
            /* Since the args and variables are stored in the same unordered map
			 * we need to order them first.*/
            OMP_VARINFO initTuple("", OMP_SHARED, -1, "", "", false);
            pair<string, OMP_VARINFO> initPair("none", initTuple);
            vector<pair<string,
                OMP_VARINFO> >
                orderedVars(functionNode->ompVariables.size(),
                    initPair);
            for (auto var = functionNode->ompVariables.cbegin();
                 var != functionNode->ompVariables.cend(); ++var) {
                OMP_VARINFO newTuple(get<0>(var->second), get<1>(var->second),
                    get<2>(var->second), get<3>(var->second),
                    get<4>(var->second), false);
                std::pair<std::string, omp2cd_space::OMP_VARINFO> newPair(var->first, newTuple);
                if (get<2>(var->second) >= 0) {
                    orderedVars[get<2>(var->second)] = newPair;
                } else {
                    orderedVars.push_back(newPair);
                }
            }

            for (auto var : orderedVars) {
                /* Dont initialize threadprivate vars or static */
                VarTableTuple_t varTuple;
                bool isGlobal = false;
                bool found = this->node->myDFG->getVarTupleFromTable(this->node, var.first, varTuple);
                if (found) {
                    isGlobal = get<3>(varTuple);
                    if (get<4>(varTuple) || get<2>(varTuple).compare("static") == 0)
                        continue;
                }

                string varType = get<0>(var.second);
                string varArray = get<6>(varTuple);

                if (var.first.compare("none") != 0) {
                    if (functionNode->myRegion && functionNode->myRegion->singleThreaded) {
                        /* If POSITION >=0, then it's an arg, so initialize the arg. */
                        if (get<2>(var.second) >= 0) {
                            tempStream << "," << var.first << suffixLang << inputsRegionID << "(in_" << var.first << ")\n";
                        }
                        /*Check if it's a global variable and initialize it */
                        else {
                            if (isGlobal) {
                                tempStream << "," << var.first << suffixLang << inputsRegionID << "(" << var.first << ")\n";
                            }
                        }
                    } else if (functionNode->myRegion == nullptr || functionNode->myRegion->singleThreaded == false) {
                        if (varArray.compare("") == 0) {
                            if (DARTS_BACKEND) {
                                tempStream << "," << var.first << suffixLang << inputsRegionID
                                           << "(new " << varType << "[this->numThreads])\n";
                            }
                        } else {
                            if (DARTS_BACKEND) {
                                tempStream << "," << var.first << suffixLang << inputsRegionID
                                           << "(new " << varType << "*[this->numThreads])\n";
                            }
                        }
                    }
                }
            }
        }

        if (this->isOMPDARTSFunction(functionTable)
            && this->returnType.compare("void") != 0
            && this->node->getName().compare("main") != 0) {
            if (functionNode->myRegion && functionNode->myRegion->singleThreaded) {
                if (DARTS_BACKEND)
                    tempStream << ", functionResult ( in_functionResult)";
            } else {
                if (DARTS_BACKEND)
                    tempStream << ", functionResult ( new " << this->returnType << "*[this->numThreads])";
#if 0
                else if (SWARM_BACKEND)
					tempStream << "functionResult" << functionNode->getID()
							   << " = (" << this->returnType << "**)malloc(sizeof("
							   << this->returnType << "*) * numThreads"
							   << functionNode->getID() << ");\n";
#endif
            }
        }
    }
}

void FunctionInfo::printPassFuncParams_swarm(std::ostringstream& outputStream,
    DFGNode* srcNode, std::string postfixStr, clang::PrintingPolicy& Policy, clang::CallExpr* callExpr)
{
    DFGNode* destNode = this->node;

    srcNode = srcNode->findParentNode();

#if FULL_DEBUG_COMMENTS == 1
    outputStream << "/*destNode: " << destNode->getID() << " " << destNode->getName() << "*/\n";
    outputStream << "/*srcNode: " << srcNode->getID() << " " << srcNode->getName() << "*/\n";
#endif

    Expr** args = callExpr->getArgs();
    int numArgs = callExpr->getNumArgs();
    if (numArgs > 0) {

        int argCounter = 0;
        clang::FunctionDecl::param_iterator paramIterator = this->f->param_begin();

        for (; argCounter < numArgs && paramIterator != this->f->param_end(); argCounter++, paramIterator++) {

            string destVarName = getNameFromDecl(*paramIterator);
#if FULL_DEBUG_COMMENTS == 1
            outputStream << "/*destVarName = " << destVarName << "*/\n";
#endif
            TPInputs_t::iterator destIt = destNode->ompInputs.find(destVarName);
            TPInputs_t::iterator destItVar = destNode->ompVariables.find(destVarName);

            OMP_VARSCOPE destVarScope = OMP_SHARED;
            string destVarArray = "";

            if (destIt != destNode->ompInputs.end()) {
                destVarScope = get<1>(destIt->second);
                destVarArray = get<4>(destIt->second);
            } else if (destItVar != destNode->ompVariables.end()) {
                destVarScope = get<1>(destItVar->second);
                destVarArray = get<4>(destItVar->second);
            }

            ostringstream destVarStream;

            if (destVarScope == OMP_SHARED) {
                if (destVarArray.compare("") == 0)
                    destVarStream << "*(" << destVarName << suffixLang << destNode->getID() << ")";
                else
                    destVarStream << "(" << destVarName << suffixLang << destNode->getID() << ")";
            } else {
                destVarStream << "(" << destVarName << suffixLang << destNode->getID() << postfixStr << ")";
            }

#if FULL_DEBUG_COMMENTS == 1
            outputStream << "/*destVarStream: " << destVarStream.str() << "*/\n";
#endif

            string ostreamString;
            raw_string_ostream rawStringOStream(ostreamString);
            args[argCounter]->printPretty(rawStringOStream, nullptr, Policy);

            string srcVarName = rawStringOStream.str();

            TPInputs_t::iterator srcIt = srcNode->ompInputs.find(srcVarName);
            TPInputs_t::iterator srcItVar = srcNode->ompVariables.find(srcVarName);

            OMP_VARSCOPE srcVarScope = OMP_SHARED;
            string srcVarArray = "";

            bool srcFound = false;
            if (srcIt != srcNode->ompInputs.end()) {
                srcVarScope = get<1>(srcIt->second);
                srcVarArray = get<4>(srcIt->second);
                srcFound = true;
            } else if (srcItVar != srcNode->ompVariables.end()) {
                srcVarScope = get<1>(srcItVar->second);
                srcVarArray = get<4>(srcItVar->second);
                srcFound = true;
            }

#if FULL_DEBUG_COMMENTS == 1
            outputStream << "/*srcVarName = " << srcVarName << ", found = " << srcFound
                         << ", scope = " << srcVarScope << "*/\n";
#endif

            if (srcFound) {
                if (srcVarScope == OMP_SHARED) {
                    if (srcVarArray.compare("") == 0) {
                        outputStream << destVarStream.str() << " = ";
                        outputStream << "*(" << srcVarName << suffixLang << srcNode->getID() << ");\n";
                    } else {
                        outputStream << destVarStream.str() << " = ";
                        outputStream << "(" << srcVarName << suffixLang << srcNode->getID() << ");\n";

                        std::ostringstream varSizeStr;
                        varSizeStr << destVarName << "_outer" << srcNode->getID() << "_size";

                        outputStream << destVarName << suffixLang << destNode->getID() << "_size = ";
                        outputStream << " (" << varSizeStr.str() << ");\n";
                    }
                } else if (srcVarScope == OMP_PRIVATE) {

                    if (srcVarArray.compare("") == 0) {
                        outputStream << destVarStream.str() << " = ";
                        outputStream << "(" << srcVarName << suffixLang << srcNode->getID() << postfixStr << ");\n";
                    } else {
                        std::ostringstream varSizeStr;
                        varSizeStr << destVarName << "_outer" << srcNode->getID() << "_size";

                        outputStream << destVarName << suffixLang << destNode->getID() << "_size = ";
                        outputStream << "(" << varSizeStr.str() << ");\n";
                    }
                } else if (srcVarScope == OMP_FIRSTPRIVATE) {

                    if (srcVarArray.compare("") == 0) {
                        outputStream << destVarStream.str() << " = ";
                        outputStream << "&(" << srcVarName << suffixLang << srcNode->getID() << postfixStr << ");\n";
                    } else {
                        outputStream << destVarStream.str() << " = ";
                        outputStream << "(" << srcVarName << suffixLang << srcNode->getID() << postfixStr << ");\n";

                        std::ostringstream varSizeStr;
                        varSizeStr << destVarName << "_outer" << srcNode->getID() << "_size";
                        outputStream << destVarName << suffixLang << destNode->getID() << "_size = ";
                        outputStream << " (" << varSizeStr.str() << ");\n";
                    }

                } else if (srcVarScope == OMP_LASTPRIVATE) {

                    if (srcVarArray.compare("") == 0) {
                        outputStream << destVarStream.str() << " = ";
                        outputStream << "&(" << srcVarName << suffixLang << srcNode->getID() << postfixStr << ");\n";
                    } else {
                        outputStream << destVarStream.str() << " = ";
                        outputStream << "(" << srcVarName << suffixLang << srcNode->getID() << postfixStr << ");\n";

                        std::ostringstream varSizeStr;
                        varSizeStr << destVarName << "_outer" << srcNode->getID() << "_size";
                        outputStream << destVarName << suffixLang << destNode->getID() << "_size = ";
                        outputStream << " (" << varSizeStr.str() << ");\n";
                    }
                } else if (srcVarScope == OMP_SHARED_PRIVATE) {

                    if (srcVarArray.compare("") == 0) {
                        outputStream << destVarStream.str() << " = ";
                        outputStream << "&(" << srcVarName << suffixLang << srcNode->getID() << postfixStr << ");\n";
                    } else {
                        outputStream << destVarStream.str() << " = ";
                        outputStream << "(" << srcVarName << suffixLang << srcNode->getID() << postfixStr << ");\n";

                        std::ostringstream varSizeStr;
                        varSizeStr << destVarName << "_outer" << srcNode->getID() << "_size";
                        outputStream << destVarName << suffixLang << destNode->getID() << "_size = ";
                        outputStream << " (" << varSizeStr.str() << ");\n";
                    }
                }
            } else {
                /* If in neither one, then it must be a global var */
                outputStream << destVarStream.str() << " = ";
                outputStream << "(" << destVarName << ");\n";
            }
        }
    }
}

void FunctionInfo::printFunctionInputsAndVarsInitsInConstructor(std::ostringstream& tempStream,
    omp2cd_space::FunctionTable_t functionTable)
{
    DFGNode* functionNode = this->node;

    string inputsRegionID = "";
    TPRegion* parentRegion = functionNode->getRegion()->findOMPOrFunctionRegionParent();
    if (parentRegion)
        inputsRegionID += to_string(parentRegion->getID());

    if (functionNode && functionNode->myRegion->singleThreaded == false) {
        ostringstream ifStr, elseStr;
        ifStr << "";
        elseStr << "";

        if (functionNode->ompVariables.size() > 0) {
            /* Since the args and variables are stored in the same unordered map
			 * we need to order them first.*/
            OMP_VARINFO initTuple("", OMP_SHARED, -1, "", "", false);
            pair<string, OMP_VARINFO> initPair("none", initTuple);
            vector<pair<string,
                OMP_VARINFO> >
                orderedVars(functionNode->ompVariables.size(),
                    initPair);
            for (auto var = functionNode->ompVariables.cbegin();
                 var != functionNode->ompVariables.cend(); ++var) {
                OMP_VARINFO newTuple(get<0>(var->second),
                    get<1>(var->second),
                    get<2>(var->second),
                    get<3>(var->second),
                    get<4>(var->second), false);
                std::pair<std::string,
                    omp2cd_space::OMP_VARINFO>
                    newPair(var->first,
                        newTuple);
                if (get<2>(var->second) >= 0)
                    orderedVars[get<2>(var->second)] = newPair;
                else
                    orderedVars.push_back(newPair);
            }

            for (auto var : orderedVars) {
                /* Dont initialize threadprivate vars */

                VarTableTuple_t varTuple;
                bool isGlobal = false;
                bool found = this->node->myDFG->getVarTupleFromTable(this->node, var.first, varTuple);
                if (found) {
                    isGlobal = get<3>(varTuple);
                    if (get<4>(varTuple) || get<2>(varTuple).compare("static") == 0)
                        continue;
                }

                if (var.first.compare("none") != 0) {
                    /* If POSITION >=0, then it's an arg, so initialize the arg. */
                    if (get<2>(var.second) >= 0) {
                        ifStr << "this->" << var.first << "_darts" << inputsRegionID
                              << "[0]= in_" << var.first << ";\n";

                        elseStr << "this->" << var.first << "_darts" << inputsRegionID
                                << "[this->mainCodeletID]= in_" << var.first << ";\n";
                    } else {
                        /*Check if it's a global variable and initialize it */
                        if (isGlobal) {
                            ifStr << "this->" << var.first << "_darts" << inputsRegionID
                                  << "[0]= " << var.first << ";\n";

                            elseStr << "this->" << var.first << "_darts" << inputsRegionID
                                    << "[this->mainCodeletID] = " << var.first << ";\n";
                        }
                    }
                }
            }
        }

        if (this->isOMPDARTSFunction(functionTable)
            && this->returnType.compare("void") != 0
            && this->node->getName().compare("main") != 0) {
            ifStr << "this->functionResult[0] = in_functionResult;\n";
            elseStr << "this->functionResult[this->mainCodeletID] = in_functionResult;\n";
        }

        tempStream << "if(this->numThreads == 1){\n"
                   << "this->nextCodelets" << functionNode->getName() << "[0] = in_mainNextCodelet;\n"
                   << "this->nextSyncCodelets" << functionNode->getName() << "[0] = in_mainSyncCodelet;\n"
					<< ifStr.str();
#if 0
		if(parentRegion && parentRegion->childRegions.front())
			tempStream << "this->checkInCodelets" << parentRegion->childRegions.front()->getID()
				<< "[0].setID(this->getID());\n";
#endif

		tempStream << "this->availableCodelets[0] = 1;\n";

        tempStream << "}\n"
                   << "else\n"
                   << "{\n"
                   << elseStr.str()
                   << "this->nextCodelets" << functionNode->getName() << "[in_mainCodeletID] = in_mainNextCodelet;\n"
                   << "this->nextSyncCodelets" << functionNode->getName() << "[in_mainCodeletID] = in_mainSyncCodelet;\n"
				   << "#if USE_SPIN_CODELETS == 0\n"
                   << "this->firstCodelet[this->mainCodeletID].decDep();\n"
                   << "#else\n"
                   << "this->availableCodelets[this->mainCodeletID] = 1;\n"
                   << "#endif\n"
                   << "*(this->ptrToThisFunctionTP) = this;\n"
                   << "}\n";
    }
}

void FunctionInfo::printFunctionInputsAndVarsDelete(std::ostringstream&
                                                        tempStream,
    omp2cd_space::
        FunctionTable_t functionTable)
{
    DFGNode* functionNode = this->node;

    if (functionNode->myRegion == nullptr
        || functionNode->myRegion->singleThreaded == false)
        tempStream << "delete [] nextCodelets" << functionNode->getName() << ";\n"
                   << "delete [] nextSyncCodelets" << functionNode->getName() << ";\n";

    string inputsRegionID = "";
    TPRegion* parentRegion = functionNode->getRegion()->findOMPOrFunctionRegionParent();
    if (parentRegion)
        inputsRegionID += to_string(parentRegion->getID());

    if (functionNode) {
        if (functionNode->ompVariables.size() > 0) {
            OMP_VARINFO initTuple("", OMP_SHARED, -1, "", "", false);
            pair<string, OMP_VARINFO> initPair("none", initTuple);
            vector<pair<string,
                OMP_VARINFO> >
                orderedVars(functionNode->ompVariables.size(),
                    initPair);
            for (auto var = functionNode->ompVariables.cbegin();
                 var != functionNode->ompVariables.cend(); ++var) {
                OMP_VARINFO newTuple(get<0>(var->second),
                    get<1>(var->second),
                    get<2>(var->second),
                    get<3>(var->second),
                    get<4>(var->second), false);
                std::pair<std::string,
                    omp2cd_space::OMP_VARINFO>
                    newPair(var->first,
                        newTuple);
                if (get<2>(var->second) >= 0)
                    orderedVars[get<2>(var->second)] = newPair;
                else
                    orderedVars.push_back(newPair);
            }

            if (functionNode->myRegion == nullptr || functionNode->myRegion->singleThreaded == false) {
                for (auto var : orderedVars) {
					/* Dont delete threadprivate vars */
                    VarTableTuple_t varTuple;
                    bool found = this->node->myDFG->getVarTupleFromTable(this->node,
                        var.first,
                        varTuple);
                    if (found)
                        if (get<4>(varTuple)
                            || get<2>(varTuple).compare("static") == 0)
                            continue;

                    if (var.first.compare("none") != 0)
                        tempStream << "delete [] "
                                   << var.first << "_darts"
                                   << inputsRegionID << ";\n";
                }
            }
        }
    }

    if (this->isOMPDARTSFunction(functionTable)
        && this->returnType.compare("void") != 0
        && this->node->getName().compare("main") != 0) {
        if (functionNode->myRegion->singleThreaded == false) {
            tempStream << "delete[]functionResult;\n";
        }
    }
}
}
