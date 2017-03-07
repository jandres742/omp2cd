#include "include/Preprocessor/Preprocessor.h"
#include "include/DFG/FunctionInfo.h"

using namespace llvm;
using namespace clang;
using namespace clang::tooling;
using namespace std;

namespace omp2cd_space {

bool ompCallExprVisitor::VisitFunctionDecl(clang::FunctionDecl* func)
{
    DFGNode* tempNode = new DFGNode(func, this->currentNode, this->getNewNodeID());
    this->currentNode->addChild(tempNode);
    this->currentNode = tempNode;

    string funcName = "";
    this->currentNode->getClangName(funcName);
    FunctionTable_t::iterator it = functionTable.find(funcName);
    if (it == functionTable.end()) {
        string returnType = this->currentNode->myFunctionInfo->returnType;
        ExtraArgsMap_t newExtraArgsMap;
        FunctionTableTuple_t newFunctionTuple(
            returnType, false, this->currentNode->getID(), newExtraArgsMap, false, 0);
        FunctionTablePair_t newFunctionPair(funcName, newFunctionTuple);
        functionTable.insert(newFunctionPair);
    }
    return true;
}

bool ompCallExprVisitor::TraverseStmt(Stmt* s)
{
    DFGNode* previousNode;

    if (this->isTraversableStmt(s)) {
        DFGNode* tempNode = new DFGNode(s, this->currentNode, -1);
        this->currentNode->addChild(tempNode);
        previousNode = this->currentNode;
        this->currentNode = tempNode;
    }

    RecursiveASTVisitor::TraverseStmt(s);

    if (this->isTraversableStmt(s)) {
        this->currentNode = previousNode;
    }
    return true;
}

bool ompCallExprVisitor::VisitCallExpr(clang::CallExpr* s)
{
    /*Check if the call is to a function marked as omp2darts and make the
    caller also an omp2darts function ONLY if the callee does not have a
    OMP parallel region*/
    string calleeFunctionName = this->getCalleeFunctionName(s);
    FunctionTable_t::iterator itCallee = functionTable.find(calleeFunctionName);
    if (itCallee != functionTable.end() && get<1>(itCallee->second)
        && get<5>(itCallee->second) == 0) {
        DFGNode* functionParent = this->currentNode->findFunctionParent();
        if (functionParent) {
            string funcName = functionParent->name;
            FunctionTable_t::iterator itCaller = functionTable.find(funcName);
            if (itCaller == functionTable.end()) {
                string returnType = functionParent->myFunctionInfo->returnType;
                ExtraArgsMap_t newExtraArgsMap;
                FunctionTableTuple_t newFunctionTuple(
                    returnType, true, functionParent->getID(), newExtraArgsMap, false, 0);
                FunctionTablePair_t newFunctionPair(funcName, newFunctionTuple);
                functionTable.insert(newFunctionPair);
            } else {
                get<1>(itCaller->second) = true;
            }
        }
    }
    return true;
}

bool ompCallExprVisitor::VisitOMPExecutableDirective(OMPExecutableDirective* s)
{
    /*Mark as omp2darts function the one containing an OMP executable directive.*/
    DFGNode* functionParent = this->currentNode->findFunctionParent();
    if (functionParent) {
        string funcName = functionParent->name;
        FunctionTable_t::iterator it = functionTable.find(funcName);
        if (it == functionTable.end()) {
            string returnType = functionParent->myFunctionInfo->returnType;
            ExtraArgsMap_t newExtraArgsMap;
            FunctionTableTuple_t newFunctionTuple(
                returnType, true, functionParent->getID(), newExtraArgsMap, false, 0);
            FunctionTablePair_t newFunctionPair(funcName, newFunctionTuple);
            functionTable.insert(newFunctionPair);
        } else {
            get<1>(it->second) = true;
        }
    }
    return true;
}

bool ompCallExprVisitor::VisitOMPParallelDirective(OMPParallelDirective* s)
{
    DFGNode* functionParent = this->currentNode->findFunctionParent();
    if (functionParent) {
        functionParent->myFunctionInfo->isOmp2dartsFunction = true;
        string funcName = functionParent->name;
        FunctionTable_t::iterator it = functionTable.find(funcName);
        if (it != functionTable.end()) {
            /*Increment the number of parallel regions found */
            get<5>(it->second) += 1;
        }
    }
    return true;
}

bool ompCallExprVisitor::VisitOMPParallelForDirective(OMPParallelForDirective* s)
{
    DFGNode* functionParent = this->currentNode->findFunctionParent();
    if (functionParent) {
        functionParent->myFunctionInfo->isOmp2dartsFunction = true;
        string funcName = functionParent->name;
        FunctionTable_t::iterator it = functionTable.find(funcName);
        if (it != functionTable.end()) {
            /*Increment the number of parallel regions found */
            get<5>(it->second) += 1;
        }
    }
    return true;
}
}
