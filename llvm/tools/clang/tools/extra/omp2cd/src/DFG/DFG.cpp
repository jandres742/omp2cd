#include "include/DFG/DFG.h"
#include "include/DFG/DFGNode.h"
#include "include/DFG/TPRegion.h"

using namespace llvm;
using namespace clang;
using namespace clang::tooling;
using namespace std;

namespace omp2cd_space {

void DFG::addRoot(DFGNode* rootNode) { this->root = rootNode; }

void DFG::addFunctionDecl(DFGNode* functionNode)
{
    std::tuple<DFGNode*, TPRegion*> newTuple(functionNode, nullptr);
    myFunctions_Elem_t newFunction(functionNode->name, newTuple);
    myFunctionsDecl.insert(newFunction);
}

void DFG::addFunction(DFGNode* functionNode)
{
    std::tuple<DFGNode*, TPRegion*> newTuple(functionNode, nullptr);
    myFunctions_Elem_t newFunction(functionNode->name, newTuple);
    myFunctions.insert(newFunction);

    myFunctionsVec.push_back(functionNode->name);
}

void DFG::removeFunction(DFGNode* functionNode)
{
    myFunctions_t::iterator it = myFunctions.find(functionNode->name);
    if (it != myFunctions.end()) {
        myFunctions.erase(it);
    }

    myFunctionsVec.erase(myFunctionsVec.end());
}

/* Add global vars as inputs to a function's TP because
those vars may have private data-sharing
attributes in omp regions that are within
the function*/
void DFG::addGlobalVarsAsInputsToFunction(DFGNode* functionNode)
{
    if (this->root && this->root->ompVariables.size() > 0) {
        for (auto var : this->root->ompVariables) {
            string varType = get<0>(var.second);
            string varArray = get<4>(var.second);
            /* FIXME: We are not adding arrays for now. */
            if (varArray.empty())
                functionNode->addNewVar(varType, var.first);
        }
    }
}

/* Print the global variables, which have all been added to the DFG's root node*/
void DFG::printGlobalVars(std::ostringstream& tempStream, clang::ASTContext* context)
{
    if (this->root) {
        for (auto var : this->root->ompVariables) {
            bool isThreadPrivate = false;

            string varType = get<0>(var.second);
            string varStorage = get<3>(var.second);
            string varArray = get<4>(var.second);

            /* Don't print global vars that are threadprivate, they are
             * handled in printThreadPrivateVars(...)*/
            ostringstream initValStr;
            VarTableTuple_t varTuple;
            bool found = this->getVarTupleFromTable(nullptr, var.first, varTuple);

            if (found) {
                if (get<1>(varTuple).empty() == false)
                    initValStr << " = " << get<1>(varTuple);
                isThreadPrivate = get<4>(varTuple);
            }
            if (isThreadPrivate == false) {
                tempStream << varStorage << " " << varType << " " << var.first << " " << varArray
                           << " " << initValStr.str() << ";\n";
            }
        }
    }
}

/* Print funtions' static varibles as global variables with the node ID as suffix*/
void DFG::printFunctionStaticVars(std::ostringstream& tempStream, clang::ASTContext* context)
{
    for (VarTablePair2_t varTuple : this->varTable2) {
        string varName = varTuple.first;

        for (size_t i = 0; i < get<0>(varTuple.second).size(); i++) {

            bool isThreadPrivate = get<4>(varTuple.second)[i];

            if (isThreadPrivate == false && get<2>(varTuple.second)[i].compare("static") == 0) {

                string varType = get<0>(varTuple.second)[i];
                int parentNodeID = get<5>(varTuple.second)[i];
                string varArray = get<6>(varTuple.second)[i];
                string varInit = get<1>(varTuple.second)[i];

                tempStream << varType << " " << varName << "_darts" << parentNodeID << " "
                           << varArray << ";\n";

                if (varInit.empty() == false)
                    tempStream << "bool " << varName << "_darts" << parentNodeID << "_initFlag "
                               << varArray << ";\n";
            }
        }
    }
}

void DFG::printFunctionStaticVarFlagsInits(
    std::ostringstream& tempStream, clang::ASTContext* context)
{
    std::ostringstream initStream;
    for (VarTablePair2_t varTuple : this->varTable2) {
        string varName = varTuple.first;
        for (size_t i = 0; i < get<0>(varTuple.second).size(); i++) {
            bool isThreadPrivate = get<4>(varTuple.second)[i];
            if (isThreadPrivate == false && get<2>(varTuple.second)[i].compare("static") == 0) {

                int parentNodeID = get<5>(varTuple.second)[i];

                initStream << varName << "_darts" << parentNodeID << " = ("
                           << get<0>(varTuple.second)[i] << ") 0;\n";

                string varInit = get<1>(varTuple.second)[i];

                if (varInit.empty() == false)
                    initStream << varName << "_darts" << parentNodeID << "_initFlag = false;\n";
            }
        }
    }
    tempStream << initStream.str();
}

/* Print any threadprivate vars in the form of vector of omp_get_max_threads() size.
* With that vector, each thread will have a copy of the threadprivate var that will
* be preserved across parallel regions.*/
void DFG::printThreadPrivateVars(std::ostringstream& tempStream, clang::ASTContext* context)
{
    ostringstream varName;
    for (auto var : this->varTable2) {
        for (size_t i = 0; i < get<0>(var.second).size(); i++) {
            bool isThreadPrivate = get<4>(var.second)[i];
            if (isThreadPrivate) {
                ostringstream initValStr;
                if (get<1>(var.second)[i].empty() == false)
                    initValStr << " = {" << get<1>(var.second)[i] << "}";

                varName << get<0>(var.second)[i] << " " << var.first << suffixLang << "ThreadPriv"
                        << get<5>(var.second)[i] << "[MAXNUMTHREADS]" << get<6>(var.second)[i]
                        << initValStr.str() << ";\n";
            }
        }
    }
    tempStream << varName.str();
}

void DFG::getVarInfoFromTable(
    DFGNode* node, const std::string varName, std::string& varType, std::string& varArray)
{
    DFGNode* parentNode = node->findFunctionParent();
    VarTable2_t::iterator varIt2 = this->varTable2.find(varName);
    if (varIt2 != this->varTable2.end()) {
        for (size_t i = 0; i < get<5>(varIt2->second).size(); i++) {
            if (get<5>(varIt2->second)[i] == parentNode->getID()) {
                varType = get<0>(varIt2->second)[i];
                varArray = get<6>(varIt2->second)[i];
            }
        }
    }
}

bool DFG::getVarTupleFromTable(DFGNode* node, const std::string varName, VarTableTuple_t& varTuple)
{
    bool found = false;

    /*If node is passed, then use it to match the ID and find the correct variable.
       If node is nullptr, then we are using the root node. */
    if (node == nullptr) {
        VarTable2_t::iterator varIt2 = this->varTable2.find(varName);
        if (varIt2 != this->varTable2.end()) {
            found = true;
            get<0>(varTuple) = get<0>(varIt2->second)[0];
            get<1>(varTuple) = get<1>(varIt2->second)[0];
            get<2>(varTuple) = get<2>(varIt2->second)[0];
            get<3>(varTuple) = get<3>(varIt2->second)[0];
            get<4>(varTuple) = get<4>(varIt2->second)[0];
            get<5>(varTuple) = get<5>(varIt2->second)[0];
            get<6>(varTuple) = get<6>(varIt2->second)[0];
            get<7>(varTuple) = get<7>(varIt2->second)[0];
            get<8>(varTuple) = get<8>(varIt2->second)[0];
            get<9>(varTuple) = get<9>(varIt2->second)[0];
        }
    } else {
        DFGNode* parentNode = node;
        VarTable2_t::iterator varIt2 = this->varTable2.find(varName);
        if (varIt2 != this->varTable2.end()) {
            /*We assume if it's a global or threadprivate var, there's only one copy of it. */
            if (get<3>(varIt2->second)[0] || get<4>(varIt2->second)[0]) {
                found = true;
                get<0>(varTuple) = get<0>(varIt2->second)[0];
                get<1>(varTuple) = get<1>(varIt2->second)[0];
                get<2>(varTuple) = get<2>(varIt2->second)[0];
                get<3>(varTuple) = get<3>(varIt2->second)[0];
                get<4>(varTuple) = get<4>(varIt2->second)[0];
                get<5>(varTuple) = get<5>(varIt2->second)[0];
                get<6>(varTuple) = get<6>(varIt2->second)[0];
                get<7>(varTuple) = get<7>(varIt2->second)[0];
                get<8>(varTuple) = get<8>(varIt2->second)[0];
                get<9>(varTuple) = get<9>(varIt2->second)[0];
            } else {
                while (parentNode && found == false) {
                    for (size_t i = 0; i < get<5>(varIt2->second).size(); i++) {
                        if (get<5>(varIt2->second)[i] == parentNode->getID()) {
                            found = true;
                            get<0>(varTuple) = get<0>(varIt2->second)[i];
                            get<1>(varTuple) = get<1>(varIt2->second)[i];
                            get<2>(varTuple) = get<2>(varIt2->second)[i];
                            get<3>(varTuple) = get<3>(varIt2->second)[i];
                            get<4>(varTuple) = get<4>(varIt2->second)[i];
                            get<5>(varTuple) = get<5>(varIt2->second)[i];
                            get<6>(varTuple) = get<6>(varIt2->second)[i];
                            get<7>(varTuple) = get<7>(varIt2->second)[i];
                            get<8>(varTuple) = get<8>(varIt2->second)[i];
                            get<9>(varTuple) = get<9>(varIt2->second)[i];
                            break;
                        }
                    }
                    parentNode = parentNode->parent;
                }
            }
        }
    }
    return found;
}

void DFG::insertVarTupleToTable(std::string varName, VarTableTuple_t& varTuple)
{
    /*
    typedef std::tuple <
            std::string      0 = type
            std::string      1 = init
            std::string      2 = storage: extern or static
            bool             3 = is it global?
            bool             4 = is is threadprivate?
            int              5 = ParentNode id
            std::string      6 = array specifier
            std::string      7 = original name
            std::string      8 = array size
            bool		     9 = needs to be freed()
    > VarTableTuple_t;
    */
    VarTable2_t::iterator varIt2 = this->varTable2.find(varName);
    if (varIt2 != this->varTable2.end()) {

        bool found = false;

        size_t i = 0;
        for (; i < get<5>(varIt2->second).size(); i++) {
            if (get<5>(varIt2->second)[i] == get<5>(varTuple)) {
                found = true;
                break;
            }
        }

        if (found == false) {
            get<0>(varIt2->second).push_back(get<0>(varTuple));
            get<1>(varIt2->second).push_back(get<1>(varTuple));
            get<2>(varIt2->second).push_back(get<2>(varTuple));
            get<3>(varIt2->second).push_back(get<3>(varTuple));
            get<4>(varIt2->second).push_back(get<4>(varTuple));
            get<5>(varIt2->second).push_back(get<5>(varTuple));
            get<6>(varIt2->second).push_back(get<6>(varTuple));
            get<7>(varIt2->second).push_back(get<7>(varTuple));
            get<8>(varIt2->second).push_back(get<8>(varTuple));
            get<9>(varIt2->second).push_back(get<9>(varTuple));
        } else {
            get<0>(varIt2->second)[i] = get<0>(varTuple);
            get<1>(varIt2->second)[i] = get<1>(varTuple);
            get<2>(varIt2->second)[i] = get<2>(varTuple);
            get<3>(varIt2->second)[i] = get<3>(varTuple);
            get<4>(varIt2->second)[i] = get<4>(varTuple);
            get<5>(varIt2->second)[i] = get<5>(varTuple);
            get<6>(varIt2->second)[i] = get<6>(varTuple);
            get<7>(varIt2->second)[i] = get<7>(varTuple);
            get<8>(varIt2->second)[i] = get<8>(varTuple);
            get<9>(varIt2->second)[i] = get<9>(varTuple);
        }
    } else {
        VarTableTuple2_t newTuple2;
        get<0>(newTuple2).push_back(get<0>(varTuple));
        get<1>(newTuple2).push_back(get<1>(varTuple));
        get<2>(newTuple2).push_back(get<2>(varTuple));
        get<3>(newTuple2).push_back(get<3>(varTuple));
        get<4>(newTuple2).push_back(get<4>(varTuple));
        get<5>(newTuple2).push_back(get<5>(varTuple));
        get<6>(newTuple2).push_back(get<6>(varTuple));
        get<7>(newTuple2).push_back(get<7>(varTuple));
        get<8>(newTuple2).push_back(get<8>(varTuple));
        get<9>(newTuple2).push_back(get<9>(varTuple));
        VarTablePair2_t newVar(varName, newTuple2);
        this->varTable2.insert(newVar);
    }
}
}
