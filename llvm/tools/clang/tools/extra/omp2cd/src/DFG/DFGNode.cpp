#include "include/DFG/DFGNode.h"
#include "include/DFG/BranchInfo.h"
#include "include/DFG/FunctionInfo.h"
#include "include/DFG/LoopInfo.h"
#include "include/DFG/TPRegion.h"
#include "include/DFG/TaskInfo.h"

using namespace llvm;
using namespace clang;
using namespace clang::tooling;
using namespace std;

namespace omp2cd_space {

DFGNode::DFGNode(int id, std::string name, DFGNodeKind k)
    : d(nullptr)
    , s(nullptr)
    , id(id)
    , parent(nullptr)
    , myDFG(parent ? parent->myDFG : nullptr)
    , fireCodeStr("")
    , next(nullptr)
    , prev(nullptr)
    , noWait(false)
    , myLoopInfo(nullptr)
    , myRegion(nullptr)
    , myFunctionInfo(nullptr)
    , myTaskInfo(nullptr)
    , myCopyinClause(nullptr)
    , myCopyPrivateClause(nullptr)
    , calleeFunctionNodeName("")
    , calleeTmpVarName("")
    , useTmpVarInsteadOfCall(false)
    , myBranchInfo(nullptr)
    , myIfNode(nullptr)
    , isRegionMainNode(false)
    , isRegionNode(false)
    , criticalDirectiveName("")
    , criticalDirectiveLock(false)
    , criticalDirectiveUnlock(false)
    , isComplexStmt(false)
    , nullStmtAction(NULLSTMT_EMPTY)
    , nullStmtActionVar("")
    , kind(k)
{
    this->name = name;
    ompExtensionClause = new OMPExtensionClause();
}

DFGNode::DFGNode(DFGNode* originalNode, int id, DFGNodeKind k)
    : d(nullptr)
    , s(nullptr)
    , id(id)
    , parent(nullptr)
    , fireCodeStr("")
    , next(nullptr)
    , prev(nullptr)
    , noWait(false)
    , myLoopInfo(nullptr)
    , myRegion(nullptr)
    , myFunctionInfo(nullptr)
    , myTaskInfo(nullptr)
    , myCopyinClause(nullptr)
    , myCopyPrivateClause(nullptr)
    , calleeFunctionNodeName("")
    , calleeTmpVarName("")
    , useTmpVarInsteadOfCall(false)
    , myBranchInfo(nullptr)
    , myIfNode(nullptr)
    , isRegionMainNode(false)
    , isRegionNode(false)
    , criticalDirectiveName("")
    , criticalDirectiveLock(false)
    , criticalDirectiveUnlock(false)
    , isComplexStmt(false)
    , nullStmtAction(originalNode->nullStmtAction)
    , nullStmtActionVar(originalNode->nullStmtActionVar)
    , kind(k)
{
    this->name = originalNode->name;
    this->d = originalNode->d;
    this->s = originalNode->s;
    this->parent = originalNode->parent;
    this->myDFG = originalNode->myDFG;
    this->fireCodeStr = originalNode->fireCodeStr;
    this->next = originalNode->next;
    this->prev = originalNode->prev;
    this->noWait = originalNode->noWait;
    this->myLoopInfo = originalNode->myLoopInfo;
    this->myRegion = originalNode->myRegion;
    this->myFunctionInfo = originalNode->myFunctionInfo;
    this->myTaskInfo = originalNode->myTaskInfo;
    this->myCopyinClause = originalNode->myCopyinClause;
    this->myCopyPrivateClause = originalNode->myCopyPrivateClause;
    this->calleeFunctionNodeName = originalNode->calleeFunctionNodeName;
    this->calleeTmpVarName = originalNode->calleeTmpVarName;
    this->useTmpVarInsteadOfCall = originalNode->useTmpVarInsteadOfCall;
    this->myBranchInfo = originalNode->myBranchInfo;
    this->myIfNode = originalNode->myIfNode;
    this->isRegionMainNode = originalNode->isRegionMainNode;
    this->isRegionNode = originalNode->isRegionNode;
    this->criticalDirectiveName = originalNode->criticalDirectiveName;
    this->criticalDirectiveLock = originalNode->criticalDirectiveLock;
    this->criticalDirectiveUnlock = originalNode->criticalDirectiveUnlock;
    this->isComplexStmt = originalNode->isComplexStmt;

    this->ompExtensionClause = originalNode->ompExtensionClause;
}

DFGNode::DFGNode(clang::Stmt* s, DFGNodeKind k)
    : d(nullptr)
    , s(s)
    , id(-1)
    , parent(nullptr)
    , myDFG(parent ? parent->myDFG : nullptr)
    , fireCodeStr("")
    , next(nullptr)
    , prev(nullptr)
    , noWait(false)
    , myLoopInfo(nullptr)
    , myRegion(nullptr)
    , myFunctionInfo(nullptr)
    , myTaskInfo(nullptr)
    , myCopyinClause(nullptr)
    , myCopyPrivateClause(nullptr)
    , calleeFunctionNodeName("")
    , calleeTmpVarName("")
    , useTmpVarInsteadOfCall(false)
    , myBranchInfo(nullptr)
    , myIfNode(nullptr)
    , isRegionMainNode(false)
    , isRegionNode(false)
    , criticalDirectiveName("")
    , criticalDirectiveLock(false)
    , criticalDirectiveUnlock(false)
    , isComplexStmt(false)
    , nullStmtAction(NULLSTMT_EMPTY)
    , nullStmtActionVar("")
    , kind(k)
{
    ompExtensionClause = new OMPExtensionClause();
}

DFGNode::DFGNode(clang::Stmt* s, DFGNode* parent, int id, DFGNodeKind k)
    : d(nullptr)
    , s(s)
    , id(id)
    , parent(parent)
    , myDFG(parent ? parent->myDFG : nullptr)
    , fireCodeStr("")
    , next(nullptr)
    , prev(nullptr)
    , noWait(false)
    , myLoopInfo(nullptr)
    , myRegion(nullptr)
    , myFunctionInfo(nullptr)
    , myTaskInfo(nullptr)
    , myCopyinClause(nullptr)
    , myCopyPrivateClause(nullptr)
    , calleeFunctionNodeName("")
    , calleeTmpVarName("")
    , useTmpVarInsteadOfCall(false)
    , myBranchInfo(nullptr)
    , myIfNode(nullptr)
    , isRegionMainNode(false)
    , isRegionNode(false)
    , criticalDirectiveName("")
    , criticalDirectiveLock(false)
    , criticalDirectiveUnlock(false)
    , isComplexStmt(false)
    , nullStmtAction(NULLSTMT_EMPTY)
    , nullStmtActionVar("")
    , kind(k)
{
    this->getClangName(this->name);
    ompExtensionClause = new OMPExtensionClause();
}

DFGNode::DFGNode(clang::Decl* d, DFGNode* parent, int id, DFGNodeKind k)
    : d(d)
    , s(nullptr)
    , id(id)
    , parent(parent)
    , myDFG(parent ? parent->myDFG : nullptr)
    , fireCodeStr("")
    , next(nullptr)
    , prev(nullptr)
    , noWait(false)
    , myLoopInfo(nullptr)
    , myRegion(nullptr)
    , myFunctionInfo(nullptr)
    , myTaskInfo(nullptr)
    , myCopyinClause(nullptr)
    , myCopyPrivateClause(nullptr)
    , calleeFunctionNodeName("")
    , calleeTmpVarName("")
    , useTmpVarInsteadOfCall(false)
    , myBranchInfo(nullptr)
    , myIfNode(nullptr)
    , isRegionMainNode(false)
    , isRegionNode(false)
    , criticalDirectiveName("")
    , criticalDirectiveLock(false)
    , criticalDirectiveUnlock(false)
    , isComplexStmt(false)
    , nullStmtAction(NULLSTMT_EMPTY)
    , nullStmtActionVar("")
    , kind(k)
{
    this->getClangName(this->name);
    ompExtensionClause = new OMPExtensionClause();
}

DFGNode::DFGNode(clang::FunctionDecl* f, DFGNode* parent, int id, DFGNodeKind k)
    : d(nullptr)
    , s(nullptr)
    , id(id)
    , parent(parent)
    , myDFG(parent ? parent->myDFG : nullptr)
    , fireCodeStr("")
    , next(nullptr)
    , prev(nullptr)
    , noWait(false)
    , myLoopInfo(nullptr)
    , myRegion(nullptr)
    , myFunctionInfo(nullptr)
    , myTaskInfo(nullptr)
    , myCopyinClause(nullptr)
    , myCopyPrivateClause(nullptr)
    , calleeFunctionNodeName("")
    , calleeTmpVarName("")
    , useTmpVarInsteadOfCall(false)
    , myBranchInfo(nullptr)
    , myIfNode(nullptr)
    , isRegionMainNode(false)
    , isRegionNode(false)
    , criticalDirectiveName("")
    , criticalDirectiveLock(false)
    , criticalDirectiveUnlock(false)
    , isComplexStmt(false)
    , nullStmtAction(NULLSTMT_EMPTY)
    , nullStmtActionVar("")
    , kind(k)
{
    this->myFunctionInfo = new FunctionInfo(f, this);
    this->getClangName(this->name);
    this->myFunctionInfo->name = this->name;
    ompExtensionClause = new OMPExtensionClause();
}

DFGNode::~DFGNode()
{
    if (this->myLoopInfo) {
        delete (this->myLoopInfo);
        this->myLoopInfo = nullptr;
    }

    if (this->ompExtensionClause)
        delete (this->ompExtensionClause);
}

void DFGNode::addChild(DFGNode* childNode)
{
	this->myNodes.insert(childNode);
}

clang::Stmt* DFGNode::getStmt()
{ 
	return this->s;
}

bool DFGNode::isOMPParallel()
{
    bool res = check_isa<OMPParallelDirective>(this->getStmt())
        || check_isa<OMPParallelForDirective>(this->getStmt())
		|| check_isa<OMPParallelSectionsDirective>(this->getStmt());
    return res;
}

bool DFGNode::isOMPFor()
{
    bool res = check_isa<OMPForDirective>(this->getStmt())
        || check_isa<OMPParallelForDirective>(this->getStmt());
    return res;
}

void DFGNode::insertNewNodeBeforeChildNode(DFGNode* oldChild, DFGNode* newChild)
{
    if (oldChild->prev)
        oldChild->prev->next = newChild;

    newChild->prev = oldChild->prev;

    oldChild->prev = newChild;
    newChild->next = oldChild;

    if (oldChild == this->myNodes.front())
        this->myNodes.head = newChild;

    if (oldChild == this->myNodes.back())
        this->myNodes.tail = newChild;

    this->myNodes.length += 1;
}

TPRegion* DFGNode::getRegion()
{
    TPRegion* region = this->myRegion;
    if (region == nullptr) {
        DFGNode* parentNode = this->parent;
        while (region == nullptr && parentNode) {
            region = parentNode->myRegion;
            parentNode = parentNode->parent;
        }
    }
    return region;
}

bool DFGNode::isLoopNode()
{
    return (check_isa<ForStmt>(this->getStmt()) || check_isa<WhileStmt>(this->getStmt())
        || check_isa<DoStmt>(this->getStmt()));
}

DFG* DFGNode::getDFG() 
{
	return this->myDFG;
}

DFGNode* DFGNode::getParent() 
{
	return this->parent;
}

int DFGNode::getID() 
{
	return this->id;
}

std::string DFGNode::getName()
{ 
	return this->name;
}

bool DFGNode::isCallerNode()
{
    bool res = false;
    if (!(this->calleeFunctionNodeName.empty()))
        res = true;

    return res;
}

CopyPrivateClause* DFGNode::getCopyPrivateClause()
{
	return this->myCopyPrivateClause;
}

CopyinClause* DFGNode::getCopyinClause()
{
	return this->myCopyinClause;
}

BranchInfo* DFGNode::getBranchInfo()
{
	return this->myBranchInfo;
}

LoopInfo* DFGNode::getLoopInfo()
{
	return this->myLoopInfo;
}

FunctionInfo* DFGNode::getFunctionInfo()
{
	return this->myFunctionInfo;
}

TaskInfo* DFGNode::getTaskInfo()
{
	return this->myTaskInfo; 
}

void DFGNode::getClangName(std::string& clangName)
{
    if (this->d) {
        clangName = this->d->clang::Decl::getDeclKindName();
    } else if (this->s) {
        clangName = this->s->clang::Stmt::getStmtClassName();
    } else if (this->myFunctionInfo) {
        NamedDecl* tempNamedDecl = dyn_cast<NamedDecl>(this->myFunctionInfo->f);
        if (tempNamedDecl != nullptr) {
            clangName = tempNamedDecl->getNameAsString();
        }
    }
}

void DFGNode::addNewInput(Decl* decl, int varPos)
{
    string varName = getNameFromDecl(decl);

    string varType = getTypeFromDecl(decl);

    size_t pos = varType.find("const");
    if (pos != string::npos)
        varType.erase(pos, 5);

    string varArray = "";
    string varTypeTemp = varType;
    size_t pos0 = varTypeTemp.find("[");
    size_t pos1 = varTypeTemp.find_last_of("]");
    if (pos0 != string::npos && pos1 != string::npos) {
        varType = varTypeTemp.substr(0, pos0);
        varArray = varTypeTemp.substr(pos0, pos1 - 1);
    }

    TPInputs_t::iterator it = ompInputs.find(varName);
    if (it != ompInputs.end()) {
        get<0>(it->second) = varType;
        get<4>(it->second) = varArray;
    } else {
        string initValStr = "";
        OMP_VARINFO newTuple(varType, OMP_SHARED, varPos, initValStr, varArray, false);
        pair<string, OMP_VARINFO> newInput(varName, newTuple);
        ompInputs.insert(newInput);
    }
}

void DFGNode::addNewInput(Decl* decl, OMP_VARSCOPE varScope, int varPos)
{
    string varName = getNameFromDecl(decl);

    string varType = getTypeFromDecl(decl);

    size_t pos = varType.find("const");
    if (pos != string::npos)
        varType.erase(pos, 5);

    string varTypeTemp = varType;
    string varArray = "";
    size_t pos0 = varTypeTemp.find("[");
    size_t pos1 = varTypeTemp.find_last_of("]");
    if (pos0 != string::npos && pos1 != string::npos) {
        varType = varTypeTemp.substr(0, pos0);
        varArray = varTypeTemp.substr(pos0, pos1 - 1);
    }

    TPInputs_t::iterator it = ompInputs.find(varName);
    if (it != ompInputs.end()) {
        get<0>(it->second) = varType;
        get<4>(it->second) = varArray;
    } else {
        string initValStr = "";
        OMP_VARINFO newTuple(varType, varScope, varPos, initValStr, varArray, false);
        pair<string, OMP_VARINFO> newInput(varName, newTuple);
        ompInputs.insert(newInput);
    }
}

void DFGNode::addNewVar(Decl* decl, int varPos, string initValStr)
{
    /*Get the type */
    string varType = getTypeFromDecl(decl);

    size_t pos = varType.find("const");
    if (pos != string::npos)
        varType.erase(pos, 5);

    string varTypeTemp = varType;
    string varArray = "";
    size_t pos0 = varTypeTemp.find("[");
    size_t pos1 = varTypeTemp.find_last_of("]");
    if (pos0 != string::npos && pos1 != string::npos) {
        varType = varTypeTemp.substr(0, pos0);
        varArray = varTypeTemp.substr(pos0, pos1 - 1);
    }

    /*Get the name */
    string varName = getNameFromDecl(decl);

    /*Get the storage specifier */
    if (isa<VarDecl>(decl)) {
        VarDecl* d = dyn_cast<VarDecl>(decl);
        if (d->hasExternalStorage()) {
            initValStr = "extern";
        } else if (d->isExternC() == 0) {
            initValStr = "static";
        }
    }

    bool declaredLocally = false;
    if (check_isa<OMPForDirective>(this->getStmt())
        || check_isa<OMPParallelForDirective>(this->getStmt())) {
        declaredLocally = true;
    } else {
        DFGNode* ompParent = this->findOMPParent();
        if (ompParent && (check_isa<OMPForDirective>(ompParent->getStmt())
                             || check_isa<OMPParallelForDirective>(ompParent->getStmt()))) {
            declaredLocally = true;
        }
    }

    TPInputs_t::iterator it = ompVariables.find(varName);
    if (it == ompVariables.end()) {

        OMP_VARINFO newTuple(varType, OMP_PRIVATE, varPos, initValStr, varArray, declaredLocally);
        pair<string, OMP_VARINFO> newInput(varName, newTuple);
        ompVariables.insert(newInput);
    }
}

void DFGNode::addNewVar(std::string varType, std::string varName)
{
    bool declaredLocally = false;
    if (check_isa<OMPForDirective>(this->getStmt())
        || check_isa<OMPParallelForDirective>(this->getStmt())) {
        declaredLocally = true;
    } else {
        DFGNode* ompParent = this->findOMPParent();
        if (ompParent && (check_isa<OMPForDirective>(ompParent->getStmt())
                             || check_isa<OMPParallelForDirective>(ompParent->getStmt()))) {
            declaredLocally = true;
        }
    }

    TPInputs_t::iterator it = ompVariables.find(varName);
    if (it == ompVariables.end()) {
        OMP_VARINFO newTuple(varType, OMP_PRIVATE, -1, "", "", declaredLocally);
        pair<string, OMP_VARINFO> newInput(varName, newTuple);
        ompVariables.insert(newInput);
    }
}

bool DFGNode::containsOMPDirectivesOrCalls()
{
    bool res = false;
    DFGNode* tempNode = this->myNodes.front();
    while (tempNode && res == false) {
        if (check_isa<OMPExecutableDirective>(tempNode->getStmt()) || tempNode->isCallerNode())
            res = true;
        else
            res = tempNode->containsOMPDirectivesOrCalls();
        tempNode = tempNode->next;
    }
    return res;
}

void DFGNode::getParallelNodes(std::vector<DFGNode*>& ompParallelNodesVec)
{
    DFGNode* child = this->myNodes.front();
    while (child) {
        if (child->isOMPParallel())
            ompParallelNodesVec.push_back(child);

        child->getParallelNodes(ompParallelNodesVec);
        child = child->next;
    }
}

/*Check if the node contains a stmt that has a call to a function with
* omp regions or contains an omp region*/
bool DFGNode::checkIfComplexStmt()
{
    bool found = false;
    DFGNode* child = this->myNodes.front();
    while (child && found == false) {
        if ((check_isa<OMPExecutableDirective>(child->getStmt())
                && check_isa<OMPCriticalDirective>(child->getStmt()) == false
                && check_isa<OMPAtomicDirective>(child->getStmt()) == false)
            ) {
            found = true;
        }
        /* If the call is to a function that contains parallel regions, do
         * not call this loop as complex since we will print the loop
         * in sequential mode*/
        else if (check_isa<CallExpr>(child->getStmt()) && child->isCallerNode()) {
            CallExpr* s = dyn_cast<CallExpr>(child->getStmt());
            string calleeFunctionName = "";
            Decl* calleeDecl = s->getCalleeDecl();
            if (calleeDecl) {
                calleeFunctionName = getNameFromDecl(calleeDecl);
            }
            FunctionTable_t::iterator it = functionTable.find(calleeFunctionName);
            if (it != functionTable.end()) {
                /* Loop is complex if the call is to a function with
                 * zero parallel regions.*/
                if (get<5>(it->second) == 0) {
                    found = true;
                }
            }
        } else {
            found = child->checkIfComplexStmt();
        }

        child = child->next;
    }
    this->isComplexStmt = found;
    return found;
}

DFGNode* DFGNode::findFunctionParent()
{
    DFGNode* functionParent = this->parent;
    while (functionParent != nullptr && functionParent->myFunctionInfo == nullptr)
        functionParent = functionParent->parent;

    return functionParent;
}

TPRegion* DFGNode::findMyRegion()
{
    TPRegion* region = this->myRegion;
    DFGNode* parentNode = this->parent;
    while (region == nullptr && parentNode) {
        region = parentNode->myRegion;
        parentNode = parentNode->parent;
    }
    return region;
}

bool DFGNode::isFuncCallInsideBranch()
{
    bool res = false;
    DFGNode* tempParent = this->parent;
    while (tempParent) {
        if (check_isa<IfStmt>(tempParent->getStmt())) {
            res = true;
            break;
        }
        tempParent = tempParent->parent;
    }
    return res;
}

DFGNode* DFGNode::findOMPParent()
{
    DFGNode* ompParent = this->parent;
    while (ompParent && check_isa<OMPExecutableDirective>(ompParent->getStmt()) == false) {
        ompParent = ompParent->parent;
    }
    return ompParent;
}

DFGNode* DFGNode::findParentNode()
{
    DFGNode* parentNode = this->findOMPParent();
    if (parentNode == nullptr) {
        parentNode = this->findFunctionParent();
        if (parentNode == nullptr) {
            parentNode = this;
        }
    }
    return parentNode;
}

DFGNode* DFGNode::findOMPParallelParent()
{
    DFGNode* ompParent = this->parent;
    while (ompParent != nullptr && !(check_isa<OMPParallelDirective>(ompParent->getStmt()))) {
        ompParent = ompParent->parent;
    }
    return ompParent;
}

std::string DFGNode::printOMPInputNames(bool startWithComma)
{
    std::ostringstream outputStream;

    DFGNode* ompNode = this;
    if (this->myRegion && this->myRegion->parent)
        ompNode = this->myRegion->parent->mainNode;

	bool printComma = startWithComma;
	TPInputs_t::iterator it = ompNode->ompInputs.begin();
	for (; it != ompNode->ompInputs.end(); it++) {
		if(printComma)
			outputStream << ", ";
		printComma = true;

		if (get<1>(it->second) == OMP_SHARED) {
			outputStream << "&";
		}
		outputStream << it->first;
	}
    return outputStream.str();
}

std::string DFGNode::printOMPParallelInputNames(bool startWithComma)
{
    std::ostringstream outputStream;

    DFGNode* ompNode = this;
    if (this->myRegion && this->myRegion->parent) {
        ompNode = this->myRegion->parent->mainNode;
    }

	bool printComma = startWithComma;
	TPInputs_t::iterator it = ompNode->ompInputs.begin();
	for (; it != ompNode->ompInputs.end(); it++) {
		if (get<1>(it->second) == OMP_SHARED) {
			if(printComma)
				outputStream << ", ";
			printComma = true;
			outputStream << "&" << it->first;
		} else if (get<1>(it->second) == OMP_FIRSTPRIVATE) {
			if(printComma)
				outputStream << ", ";
			printComma = true;
			outputStream << it->first;
		}
	}
    return outputStream.str();
}

std::string DFGNode::printOMPInputNamesInFunctionCall(
    DFGNode* srcNode, std::string prefixStr, bool startWithComma, std::string postfixStr)
{
    std::ostringstream outputStream;

    if (!(check_isa<OMPExecutableDirective>(srcNode->getStmt())))
        srcNode = srcNode->findParentNode();

    int srcID = srcNode->getID();

    if (!(check_isa<OMPExecutableDirective>(srcNode->getStmt())))
        srcNode = srcNode->findParentNode();

    DFGNode* destNode = this;
    if (!(check_isa<OMPExecutableDirective>(destNode->getStmt())))
        destNode = destNode->findParentNode();

    if (destNode && destNode->ompInputs.size() > 0 && srcNode) {
        bool printComma = startWithComma;
        for (TPInputs_t::iterator destVar = destNode->ompInputs.begin();
             destVar != destNode->ompInputs.end(); destVar++) {

            string destVarName = destVar->first;
            OMP_VARSCOPE destVarScope = get<1>(destVar->second);

            ostringstream inputVar;

            /* Look for the input in the source node's inputs and vars */
            TPInputs_t::iterator srcIt = srcNode->ompInputs.find(destVarName);
            TPInputs_t::iterator srcItVar = srcNode->ompVariables.find(destVarName);

            string varArray = "";
            VarTableTuple_t varTuple;
            bool found = this->myDFG->getVarTupleFromTable(this, destVarName, varTuple);
            if (found)
                varArray = get<6>(varTuple);

            /* Check if it's in the inputs */
            if (srcIt != srcNode->ompInputs.end()) {
                if (get<1>(srcIt->second) == OMP_SHARED)
                    if (varArray.compare("") == 0)
                        inputVar << "*(" << prefixStr << destVarName << "_darts" << srcID << ")";
                    else
                        inputVar << "(" << prefixStr << destVarName << "_darts" << srcID << ")";
                else
                    inputVar << "(" << prefixStr << destVarName << "_darts" << srcID << postfixStr
                             << ")";
            }
            /* Check if it's in the vars */
            else if (srcItVar != srcNode->ompVariables.end()) {
                if (get<1>(srcItVar->second) == OMP_PRIVATE)
                    inputVar << "(" << prefixStr << destVarName << "_darts" << srcID << postfixStr
                             << ")";
            }
            /* If in neither one, then it must be a global var */
            else {
                inputVar << "(" << destVarName << ")";
            }

            if (destVarScope == OMP_SHARED) {

                if (printComma)
                    outputStream << ", ";
                printComma = true;

                if (varArray.compare("") == 0) {
                    outputStream << "&(" << inputVar.str() << ")";
                } else {
                    outputStream << "(" << inputVar.str() << ")";

                    std::ostringstream varSizeStr;
                    varSizeStr << DARTS_PREFIXSTR_DEFAULT << destVarName << "_outer" << srcID
                               << "_size";
                    outputStream << ", (" << varSizeStr.str() << ")";
                }
            } else if (destVarScope == OMP_PRIVATE) {

                if (varArray.compare("") != 0) {
                    std::ostringstream varSizeStr;
                    varSizeStr << DARTS_PREFIXSTR_DEFAULT << destVarName << "_outer" << srcID
                               << "_size";

                    if (printComma)
                        outputStream << ", ";
                    printComma = true;

                    outputStream << "(" << varSizeStr.str() << ")";
                }
            } else if (destVarScope == OMP_FIRSTPRIVATE) {

                if (printComma)
                    outputStream << ", ";
                printComma = true;

                if (varArray.compare("") == 0) {
                    outputStream << "&(" << inputVar.str() << ")";
                } else {
                    outputStream << "(" << inputVar.str() << ")";
                    std::ostringstream varSizeStr;
                    varSizeStr << DARTS_PREFIXSTR_DEFAULT << destVarName << "_outer" << srcID
                               << "_size";
                    outputStream << ", (" << varSizeStr.str() << ")";
                }

            } else if (destVarScope == OMP_LASTPRIVATE) {

                if (printComma)
                    outputStream << ", ";
                printComma = true;

                if (varArray.compare("") == 0) {
                    outputStream << "&(" << inputVar.str() << ")";
                } else {
                    outputStream << "(" << inputVar.str() << ")";
                    std::ostringstream varSizeStr;
                    varSizeStr << DARTS_PREFIXSTR_DEFAULT << destVarName << "_outer" << srcID
                               << "_size";
                    outputStream << ", (" << varSizeStr.str() << ")";
                }
            }
        }
    }
    return outputStream.str();
}

std::string DFGNode::printCodeletParamPassing_swarm(DFGNode* srcNode, std::string postfixStr)
{
    std::ostringstream outputStream;

    if (!(check_isa<OMPExecutableDirective>(srcNode->getStmt())))
        srcNode = srcNode->findParentNode();

    DFGNode* destNode = this;
    if (!(check_isa<OMPExecutableDirective>(destNode->getStmt())))
        destNode = destNode->findParentNode();

    if (destNode && destNode->ompInputs.size() > 0 && srcNode) {
        /* Print the first destintion node's input, which may or may not have comma */
        TPInputs_t::iterator destVar = destNode->ompInputs.begin();
        for (; destVar != destNode->ompInputs.end(); destVar++) {
            ostringstream inputVar;
            string destVarName = destVar->first;
            OMP_VARSCOPE destVarScope = get<1>(destVar->second);
            string varArray = "";

            VarTableTuple_t varTuple;
            bool found = this->myDFG->getVarTupleFromTable(this, destVarName, varTuple);
            if (found)
                varArray = get<6>(varTuple);

            /* Look for the input in the source node's inputs and vars */
            TPInputs_t::iterator srcIt = srcNode->ompInputs.find(destVarName);
            TPInputs_t::iterator srcItVar = srcNode->ompVariables.find(destVarName);

            /* Check if it's in the inputs */
            if (srcIt != srcNode->ompInputs.end()) {
                if (get<1>(srcIt->second) == OMP_SHARED) {
                    if (varArray.compare("") == 0)
                        inputVar << "*(" << destVarName << suffixLang << srcNode->getID() << ")";
                    else
                        inputVar << "(" << destVarName << suffixLang << srcNode->getID() << ")";
                } else {
                    inputVar << "(" << destVarName << suffixLang << srcNode->getID() << postfixStr
                             << ")";
                }
            }
            /* Check if it's in the vars */
            else if (srcItVar != srcNode->ompVariables.end()) {
                if (get<1>(srcItVar->second) == OMP_PRIVATE)
                    inputVar << "(" << destVarName << suffixLang << srcNode->getID() << postfixStr
                             << ")";
            }
            /* If in neither one, then it must be a global var */
            else {
                inputVar << "(" << destVarName << ")";
            }

            /* Now check the destination node's input's scope,
            whether a pointer or value must be passed)*/
            if (destVarScope == OMP_SHARED) {
                if (varArray.compare("") == 0) {
                    outputStream << destVarName << suffixLang << destNode->getID() << " = ";
                    outputStream << "&(" << inputVar.str() << ");\n";
                } else {
                    outputStream << destVarName << suffixLang << destNode->getID() << " = ";
                    outputStream << "(" << inputVar.str() << ");\n";
                    std::ostringstream varSizeStr;
                    varSizeStr << destVarName << "_outer" << srcNode->getID() << "_size";

                    outputStream << destVarName << suffixLang << destNode->getID() << "_size = ";
                    outputStream << " (" << varSizeStr.str() << ");\n";
                }
            } else if (destVarScope == OMP_PRIVATE) {
                if (varArray.compare("") != 0) {

                    std::ostringstream varSizeStr;
                    varSizeStr << destVarName << "_outer" << srcNode->getID() << "_size";

                    outputStream << destVarName << suffixLang << destNode->getID() << "_size = ";
                    outputStream << "(" << varSizeStr.str() << ");\n";
                }
            } else if (destVarScope == OMP_FIRSTPRIVATE) {

                if (varArray.compare("") == 0) {
                    outputStream << destVarName << "_outer" << destNode->getID() << "_ptr = ";
                    outputStream << "&(" << inputVar.str() << ");\n";

                    outputStream << destVarName << suffixLang << destNode->getID() << "[codeletID] "
                                 << "= (*" << destVarName << "_outer" << destNode->getID()
                                 << "_ptr);\n";

                } else {
                    outputStream << destVarName << suffixLang << destNode->getID() << " = ";
                    outputStream << "(" << inputVar.str() << ");\n";
                    std::ostringstream varSizeStr;
                    varSizeStr << destVarName << "_outer" << srcNode->getID() << "_size";
                    outputStream << destVarName << suffixLang << destNode->getID() << "_size = ";
                    outputStream << " (" << varSizeStr.str() << ");\n";
                }

            } else if (destVarScope == OMP_LASTPRIVATE) {

                if (varArray.compare("") == 0) {
                    outputStream << destVarName << suffixLang << destNode->getID() << " = ";
                    outputStream << "&(" << inputVar.str() << ");\n";
                } else {
                    outputStream << destVarName << suffixLang << destNode->getID() << " = ";
                    outputStream << "(" << inputVar.str() << ");\n";
                    std::ostringstream varSizeStr;
                    varSizeStr << destVarName << "_outer" << srcNode->getID() << "_size";
                    outputStream << destVarName << suffixLang << destNode->getID() << "_size = ";
                    outputStream << " (" << varSizeStr.str() << ");\n";
                }
            }
        }
    }
    return outputStream.str();
}

std::string DFGNode::printOMPInputNamesFromSequential(bool startWithComma)
{
    std::ostringstream outputStream;

    DFGNode* destNode = this;
    if (!(check_isa<OMPExecutableDirective>(destNode->s)))
        destNode = destNode->findParentNode();

    if (destNode && destNode->ompInputs.size() > 0) {
        /* Print the rest of the destination node's inputs, all with commas */
		bool printComma = startWithComma;
		TPInputs_t::iterator destVar = destNode->ompInputs.begin();
        for (; destVar != destNode->ompInputs.end(); destVar++) {

            ostringstream inputVar;
            VarTableTuple_t varTuple;
            bool found = this->myDFG->getVarTupleFromTable(this, destVar->first, varTuple);

            if (found == false)
                continue;

            if (get<4>(varTuple)) {
                inputVar << "";
                continue;
            } else {
                inputVar << "(" << destVar->first << ")";
            }
            string varType = get<0>(varTuple);
            string varArray = get<6>(varTuple);
            if (varArray.compare("") != 0)
                varType += "*";
            string varSize = get<8>(varTuple);

            if (get<1>(destVar->second) == OMP_SHARED) {
				if(printComma)
					outputStream << ", ";
				printComma = true;
                if (varArray.compare("") == 0)
                    outputStream << "(" << varType << "*)&";
                else
                    outputStream << "(" << varType << ")";

                outputStream << "(" << inputVar.str() << ")";

                if (varArray.compare("") != 0)
                    outputStream << ", (" << varSize << ")";

            } else if (get<1>(destVar->second) == OMP_PRIVATE) {
                if (varArray.compare("") != 0) {
					if(printComma)
						outputStream << ", ";
					printComma = true;
                    outputStream << "(" << varSize << ")";
                }
            } else if (get<1>(destVar->second) == OMP_FIRSTPRIVATE) {
				if(printComma)
					outputStream << ", ";
				printComma = true;
                if (varArray.compare("") == 0) {
                    outputStream << "(" << varType << "*)&(" << inputVar.str() << ")";
                } else {
                    outputStream << "(" << varType << ")(" << destVar->first << ")";
                    outputStream << ", (" << varSize << ")";
                }
            } else if (get<1>(destVar->second) == OMP_LASTPRIVATE) {
				if(printComma)
					outputStream << ", ";
				printComma = true;
                if (varArray.compare("") == 0) {
                    outputStream << "(" << varType << "*)&(" << destVar->first << ")";
                } else {
                    outputStream << "(" << varType << ")(" << destVar->first << ")";
                    outputStream << ", (" << varSize << ")";
                }
            }
        }
    }
    return outputStream.str();
}

std::string DFGNode::packOMPInputContextFromSequential_swarm()
{
    std::ostringstream outputStream;

    DFGNode* destNode = this;
    if (!(check_isa<OMPExecutableDirective>(destNode->s)))
        destNode = destNode->findParentNode();

    int destNodeID = destNode->getID();

    if (destNode && destNode->ompInputs.size() > 0) {
        /* Print the first destination node's input, which may or may not have comma */
        TPInputs_t::iterator destVar = destNode->ompInputs.begin();
        for (; destVar != destNode->ompInputs.end(); destVar++) {
            ostringstream inputVar;

            string varName = destVar->first;

            VarTableTuple_t varTuple;
            bool found = this->myDFG->getVarTupleFromTable(this, varName, varTuple);
            if (found) {
                if (get<4>(varTuple)) {
                    inputVar << "";
                } else {
                    inputVar << "(" << destVar->first << ")";
                    string varType = get<0>(varTuple);
                    string varArray = get<6>(varTuple);
                    if (varArray.compare("") != 0)
                        varType += "*";
                    string varSize = get<8>(varTuple);

                    /* Now check the destination node's input's scope (if a pointer or value
                     * must be passed)*/
                    if (get<1>(destVar->second) == OMP_SHARED) {

                        ostringstream varParam;

                        if (varArray.compare("") == 0)
                            varParam << "(" << varType << "*)&";
                        else
                            varParam << "(" << varType << ")";

                        varParam << "(" << inputVar.str() << ")";

                        outputStream << varName << suffixLang << destNodeID << " = "
                                     << varParam.str() << ";\n";

                        if (varArray.compare("") != 0) {
                            outputStream << varName << suffixLang << "_size = " << varSize << ";\n";
                        }

                    } else if (get<1>(destVar->second) == OMP_PRIVATE) {
                        if (varArray.compare("") != 0) {
                            outputStream << varName << "_outer" << destNodeID
                                         << "_size = " << varSize << ";\n";
                        }
                    } else if (get<1>(destVar->second) == OMP_FIRSTPRIVATE) {
                        if (varArray.compare("") == 0) {
                            outputStream << destVar->first << suffixLang << destNodeID << " = "
                                         << "(" << varType << "*)&(" << inputVar.str() << ");\n";
                        } else {
                            outputStream << destVar->first << "_outer" << destNodeID << "_ptr = "
                                         << "(" << varType << ")" << destVar->first << ";\n";
                            outputStream << varName << "_outer" << destNodeID
                                         << "_size = " << varSize << ";\n";
                        }
                    } else if (get<1>(destVar->second) == OMP_LASTPRIVATE) {
                        if (varArray.compare("") == 0) {
                            outputStream << destVar->first << suffixLang << destNodeID << " = "
                                         << "(" << varType << "*)&(" << inputVar.str() << ");\n";
                        } else {
                            outputStream << destVar->first << "_outer" << destNodeID << "_ptr = "
                                         << "(" << varType << ")" << destVar->first << ";\n";
                            outputStream << varName << "_outer" << destNodeID
                                         << "_size = " << varSize << ";\n";
                        }
                    }
                }
            }
        }
    }
    return outputStream.str();
}

std::string DFGNode::printOMPInputNamesFromSequential_swarm()
{
    std::ostringstream outputStream;

    DFGNode* destNode = this;
    if (!(check_isa<OMPExecutableDirective>(destNode->s)))
        destNode = destNode->findParentNode();

    int destNodeID = destNode->getID();

    if (destNode && destNode->ompInputs.size() > 0) {
        /* Print the first destination node's input, which may or may not have comma */
        TPInputs_t::iterator destVar = destNode->ompInputs.begin();
        for (; destVar != destNode->ompInputs.end(); destVar++) {
            ostringstream inputVar;

            string varName = destVar->first;

            VarTableTuple_t varTuple;
            bool found = this->myDFG->getVarTupleFromTable(this, varName, varTuple);
            if (found) {
                if (get<4>(varTuple)) {
                    inputVar << "";
                } else {
                    inputVar << "(" << destVar->first << ")";
                    string varType = get<0>(varTuple);
                    string varArray = get<6>(varTuple);
                    if (varArray.compare("") != 0)
                        varType += "*";
                    string varSize = get<8>(varTuple);

                    /* Now check the destination node's input's scope (if a pointer or value
                     * must be passed)*/
                    if (get<1>(destVar->second) == OMP_SHARED) {

                        ostringstream varParam;

                        if (varArray.compare("") == 0)
                            varParam << "(" << varType << "*)&";
                        else
                            varParam << "(" << varType << ")";

                        varParam << "(" << inputVar.str() << ")";

                        outputStream << varName << suffixLang << destNodeID << " = "
                                     << varParam.str() << ";\n";

                        if (varArray.compare("") != 0) {
                            outputStream << varName << suffixLang << "_size = " << varSize << ";\n";
                        }

                    } else if (get<1>(destVar->second) == OMP_PRIVATE) {
                        if (varArray.compare("") != 0) {
                            outputStream << varName << suffixLang << destNodeID
                                         << "_size = " << varSize << ";\n";
                        }
                    } else if (get<1>(destVar->second) == OMP_FIRSTPRIVATE) {
                        if (varArray.compare("") == 0) {
                            outputStream << destVar->first << suffixLang << destNodeID << " = "
                                         << "(" << varType << "*)&(" << inputVar.str() << ");\n";
                        } else {
                            outputStream << destVar->first << suffixLang << destNodeID << " = "
                                         << "(" << varType << ")" << destVar->first << ";\n";
                            outputStream << varName << suffixLang << destNodeID
                                         << "_size = " << varSize << ";\n";
                        }
                    } else if (get<1>(destVar->second) == OMP_LASTPRIVATE) {
                        if (varArray.compare("") == 0) {
                            outputStream << destVar->first << suffixLang << destNodeID << " = "
                                         << "(" << varType << "*)&(" << inputVar.str() << ");\n";
                        } else {
                            outputStream << destVar->first << suffixLang << destNodeID << " = "
                                         << "(" << varType << ")" << destVar->first << ";\n";
                            outputStream << varName << suffixLang << destNodeID
                                         << "_size = " << varSize << ";\n";
                        }
                    } else if (get<1>(destVar->second) == OMP_SHARED_PRIVATE) {
                        if (varArray.compare("") == 0) {
                            outputStream << destVar->first << suffixLang << destNodeID
                                         << "[codeletID] = "
                                         << "(" << varType << "*)&(" << inputVar.str() << ");\n";
                        } else {
                            outputStream << destVar->first << suffixLang << destNodeID
                                         << "[codeletID] = "
                                         << "(" << varType << ")" << destVar->first << ";\n";
                            outputStream << varName << suffixLang << destNodeID
                                         << "_size = " << varSize << ";\n";
                        }
                    }
                }
            }
        }
    }
    return outputStream.str();
}

OMP_VARSCOPE DFGNode::getVariableScope(std::string varName)
{
    OMP_VARSCOPE varScope = OMP_PRIVATE;
    DFGNode* parentNode = this->findParentNode();
    if (parentNode) {
        TPInputs_t::iterator it = parentNode->ompInputs.find(varName);
        if (it != parentNode->ompInputs.end()) {
            varScope = get<1>(it->second);
        }
    }
    return varScope;
}

void DFGNode::printPrivateVarsStructDef(std::ostringstream& tempStream)
{
    DFGNode* parentNode = this;
    if (!(check_isa<OMPExecutableDirective>(parentNode->getStmt())))
        parentNode = this->findOMPParent();

    if (parentNode && parentNode->ompInputs.size() > 0) {
        for (auto var : parentNode->ompInputs) {
			/* Dont declare threadprivate vars */
            VarTableTuple_t varTuple;
            bool found = this->myDFG->getVarTupleFromTable(this, var.first, varTuple);
            if (found == false)
                continue;
            if (get<4>(varTuple))
                continue;

            string varType = get<0>(var.second);
            string varArray = get<4>(var.second);

            if (get<1>(var.second) == OMP_SHARED || get<1>(var.second) == OMP_SHARED_PRIVATE) {
                continue;
            } else if (varArray.compare("") != 0 && parentNode->myRegion->singleThreaded == false) {

                if (get<1>(var.second) == OMP_FIRSTPRIVATE
                    || get<1>(var.second) == OMP_LASTPRIVATE) {
                    tempStream << "typedef struct {\n";
                    tempStream << "uint64_t *ptr;\n";
                    tempStream << "uint64_t size;\n";
                    tempStream << "}" << var.first << "_omp" << parentNode->getID() << "_st;\n";
                }
                if (get<1>(var.second) == OMP_PRIVATE) {
                    tempStream << "typedef struct {\n";
                    tempStream << "uint64_t size;\n";
                    tempStream << "}" << var.first << "_omp" << parentNode->getID() << "_st;\n";
                }
            }
        }
    }
}

void DFGNode::printPrivateVarsDef(std::ostringstream& tempStream)
{
    DFGNode* parentNode = this;
    if (!(check_isa<OMPExecutableDirective>(parentNode->getStmt())))
        parentNode = this->findOMPParent();

    if (parentNode && parentNode->ompInputs.size() > 0) {
        for (auto var : parentNode->ompInputs) {
			/* Dont declare threadprivate vars */
            VarTableTuple_t varTuple;
            bool found = this->myDFG->getVarTupleFromTable(this, var.first, varTuple);
            if (found == false)
                continue;
            if (get<4>(varTuple))
                continue;
            string varSize = get<8>(varTuple);

            string varType = get<0>(var.second);
            string varArray = get<4>(var.second);

            if (get<1>(var.second) == OMP_SHARED || get<1>(var.second) == OMP_SHARED_PRIVATE) {
                continue;
            } else if (varArray.compare("") != 0 && parentNode->myRegion->singleThreaded == false) {

                tempStream << var.first << "_omp" << parentNode->getID() << "_st " << var.first
                           << "_omp" << parentNode->getID() << ";\n";

                if (get<1>(var.second) == OMP_FIRSTPRIVATE || get<1>(var.second) == OMP_LASTPRIVATE)
                    tempStream << var.first << "_omp" << parentNode->getID() << ".ptr "
                               << " = (uint64_t *)(" << var.first << ");\n";

                tempStream << var.first << "_omp" << parentNode->getID() << ".size "
                           << " = (" << varSize << ");\n";
            }
        }
    }
}

void DFGNode::printOMPInputAndVarDefs(std::ostringstream& tempStream)
{
    DFGNode* parentNode = this;
    if (!(check_isa<OMPExecutableDirective>(parentNode->getStmt())))
        parentNode = this->findOMPParent();

    if (check_isa<OMPTaskDirective>(parentNode->getStmt())) {
        tempStream << "_task" << parentNode->getID() << "Inputs* task" << parentNode->getID()
                   << "Inputs;\n";
    } else if (parentNode && parentNode->ompInputs.size() > 0) {
        for (auto var : parentNode->ompInputs) {
            /* Dont declare threadprivate vars or local variables*/
            bool declaredLocally = get<5>(var.second);
            if (declaredLocally)
                continue;

            VarTableTuple_t varTuple;
            bool found = this->myDFG->getVarTupleFromTable(this, var.first, varTuple);
            if (found == false)
                continue;
            if (get<4>(varTuple))
                continue;
            string varArray = get<6>(varTuple);

            string varType = get<0>(var.second);

            if (get<1>(var.second) == OMP_SHARED) {
                tempStream << varType << " *" << var.first << suffixLang << parentNode->getID()
                           << ";/*OMP_SHARED - INPUT*/\n";

                if (varArray.compare("") != 0)
                    tempStream << "uint64_t " << var.first << "_outer" << parentNode->getID()
                               << "_size;\n";

            } else if (get<1>(var.second) == OMP_SHARED_PRIVATE) {
                if (DARTS_BACKEND) {
                    if (parentNode->myRegion->singleThreaded == false) {
                        if (varArray.compare("") == 0)
                            tempStream << varType << " **" << var.first << suffixLang
                                       << parentNode->getID() << " "
                                       << "/*OMP_SHARED_PRIVATE - INPUT*/;\n";
                        else
                            /*TODO: Check why there are two stars for varArray empty and not.*/
                            tempStream << varType << " **" << var.first << suffixLang
                                       << parentNode->getID() << " "
                                       << "/*OMP_SHARED_PRIVATE - INPUT*/;\n";
                    } else {
                        tempStream << varType << "* " << var.first << suffixLang
                                   << parentNode->getID() << "/*OMP_SHARED_PRIVATE - INPUT*/;\n";
                    }
                } else if (SWARM_BACKEND) {
                    if (parentNode->myRegion->singleThreaded == false) {
                        if (varArray.compare("") == 0)
                            tempStream << varType << " *" << var.first << suffixLang
                                       << parentNode->getID()
                                       << "[MAXNUMTHREADS]/*OMP_SHARED_PRIVATE - INPUT*/;\n";
                        else
                            tempStream << varType << " *" << var.first << suffixLang
                                       << parentNode->getID()
                                       << "[MAXNUMTHREADS]/*OMP_SHARED_PRIVATE - INPUT*/;\n";
                    } else {
                        tempStream << varType << " *" << var.first << suffixLang
                                   << parentNode->getID() << "/*OMP_SHARED_PRIVATE - INPUT*/;\n";
                    }
                }
            } else {
                if (parentNode->myRegion->singleThreaded) {
                    tempStream << varType << " " << var.first << suffixLang << parentNode->getID()
                               << " " << varArray << ";\n";
                } else {

                    if (get<1>(var.second) == OMP_PRIVATE) {
                        if (varArray.compare("") == 0) {
                            if (DARTS_BACKEND)
                                tempStream << varType << " *" << var.first << suffixLang
                                           << parentNode->getID() << "/*OMP_PRIVATE - INPUT*/;\n";
                            else if (SWARM_BACKEND)
                                tempStream << varType << " " << var.first << suffixLang
                                           << parentNode->getID()
                                           << "[MAXNUMTHREADS]/*OMP_PRIVATE - INPUT*/;\n";
                        } else {
                            tempStream << varType << " **" << var.first << suffixLang
                                       << parentNode->getID() << "/*OMP_PRIVATE - INPUT*/;\n";
                            tempStream << "uint64_t " << var.first << "_outer"
                                       << parentNode->getID() << "_size;\n";
                        }
                    } else if (get<1>(var.second) == OMP_FIRSTPRIVATE) {
                        if (varArray.compare("") == 0) {
                            if (DARTS_BACKEND)
                                tempStream << varType << " *" << var.first << suffixLang
                                           << parentNode->getID()
                                           << "/*OMP_FIRSTPRIVATE - INPUT*/;\n";
                            else if (SWARM_BACKEND)
                                tempStream << varType << " " << var.first << suffixLang
                                           << parentNode->getID()
                                           << "[MAXNUMTHREADS]/*OMP_FIRSTPRIVATE - INPUT*/;\n";
                        } else {
                            tempStream << varType << " **" << var.first << suffixLang
                                       << parentNode->getID() << "/*OMP_FIRSTPRIVATE*/;\n";
                            tempStream << "uint64_t " << var.first << "_outer"
                                       << parentNode->getID() << "_size;\n";
                        }
                        tempStream << varType << " *" << var.first << "_outer"
                                   << parentNode->getID() << "_ptr;\n";
                    } else if (get<1>(var.second) == OMP_LASTPRIVATE) {
                        if (varArray.compare("") == 0) {
                            if (DARTS_BACKEND)
                                tempStream << varType << " *" << var.first << suffixLang
                                           << parentNode->getID()
                                           << "/*OMP_LASTPRIVATE - INPUT*/;\n";
                            else if (SWARM_BACKEND)
                                tempStream << varType << " " << var.first << suffixLang
                                           << parentNode->getID()
                                           << "[MAXNUMTHREADS]/*OMP_LASTPRIVATE - INPUT*/;\n";
                        } else {
                            tempStream << varType << " **" << var.first << suffixLang
                                       << parentNode->getID() << "/*OMP_LASTPRIVATE - INPUT*/;\n";
                            tempStream << "uint64_t " << var.first << "_outer"
                                       << parentNode->getID() << "_size;\n";
                        }
                        tempStream << varType << " *" << var.first << "_outer"
                                   << parentNode->getID() << "_ptr;\n";
                    }
                }
            }
        }
    }

    if ((parentNode && check_isa<OMPParallelDirective>(parentNode->getStmt())
            && parentNode->getRegion()->hasOnlyOneChildOMPForRegion())
        == false) {
        if (parentNode && parentNode->ompVariables.size() > 0) {
            for (auto var : parentNode->ompVariables) {
                /* Dont declare threadprivate or local vars */
                bool declaredLocally = get<5>(var.second);
                if (declaredLocally)
                    continue;

                VarTableTuple_t varTuple;
                bool found = this->myDFG->getVarTupleFromTable(this, var.first, varTuple);

                if (found == false)
                    continue;

                if (get<4>(varTuple))
                    continue;

                string varType = get<0>(var.second);
                string varArray = get<6>(varTuple);

                if (parentNode->myRegion->singleThreaded) {
                    tempStream << varType << " " << var.first << suffixLang << parentNode->getID()
                               << " " << varArray << "/*VARIABLE*/;\n";
                } else {

                    if (varArray.compare("") == 0) {
                        if (DARTS_BACKEND)
                            tempStream << varType << " *" << var.first << suffixLang
                                       << parentNode->getID() << "/*VARIABLE*/;\n";
                        else if (SWARM_BACKEND)
                            tempStream << varType << " " << var.first << suffixLang
                                       << parentNode->getID() << "[MAXNUMTHREADS]/*VAR*/;\n";
                    } else {
                        if (DARTS_BACKEND) {
                            tempStream << varType << " **" << var.first << suffixLang
                                       << parentNode->getID() << "/*VARIABLE*/;\n";
                            tempStream << "uint64_t " << var.first << "_outer"
                                       << parentNode->getID() << "_size;\n";
                        } else if (SWARM_BACKEND) {
                            tempStream << varType << " *" << var.first << suffixLang
                                       << parentNode->getID() << "[MAXNUMTHREADS]/*VARIABLE*/;\n";
                            tempStream << "uint64_t " << var.first << "_outer"
                                       << parentNode->getID() << "_size;\n";
                        }
                    }
                }
            }
        }
    }
}

void DFGNode::printOMPInputAndVarDefsRaw(std::ostringstream& tempStream)
{
    DFGNode* parentNode = this;
    if (!(check_isa<OMPExecutableDirective>(parentNode->getStmt())))
        parentNode = this->findOMPParent();

    if (parentNode && parentNode->ompInputs.size() > 0) {
        for (auto var : parentNode->ompInputs) {
			/* Dont declare threadprivate vars */
            VarTableTuple_t varTuple;
            bool found = this->myDFG->getVarTupleFromTable(this, var.first, varTuple);
            if (found == false)
                continue;
            if (get<4>(varTuple))
                continue;

            string varType = get<0>(var.second);
            string varArray = get<4>(var.second);

            if (get<1>(var.second) == OMP_SHARED)
                tempStream << varType << " * " << var.first << " " << varArray << ";\n";
            else if (get<1>(var.second) == OMP_FIRSTPRIVATE)
                tempStream << varType << " " << var.first << " " << varArray << ";\n";
        }
    }
}

void DFGNode::unpackOMPInputContext_swarm(std::ostringstream& outputStream)
{
    DFGNode* parentNode = this;
    if (!(check_isa<OMPExecutableDirective>(parentNode->getStmt())))
        parentNode = parentNode->findParentNode();

    string contextStr = parentNode->getName();
    contextStr += to_string(parentNode->getID());
    contextStr += "_Context";

    /* Print the first destination node's input, which may or may not have comma */
    TPInputs_t::iterator var = parentNode->ompInputs.begin();
    for (; var != parentNode->ompInputs.end(); var++) {
        ostringstream inputVar;
        string varName = var->first;
        VarTableTuple_t varTuple;
        bool found = this->myDFG->getVarTupleFromTable(this, varName, varTuple);
        if (found) {
            if (get<4>(varTuple)) {
                inputVar << "";
            } else {
                inputVar << "(" << var->first << ")";
                string varType = get<0>(varTuple);
                string varArray = get<6>(varTuple);
                if (varArray.compare("") != 0)
                    varType += "*";
                string varSize = get<8>(varTuple);

                /* Now check the destination node's input's scope (if a pointer or value
                 * must be passed)*/
                if (get<1>(var->second) == OMP_SHARED) {

                    outputStream << varType << " *" << varName << " = " << contextStr << "."
                                 << varName << ";\n";

                } else if (get<1>(var->second) == OMP_PRIVATE) {
                    if (varArray.compare("") != 0) {
                        outputStream << "uint64_t " << varName << "_size = " << contextStr << "."
                                     << varName << "_size;\n";
                    }
                } else if (get<1>(var->second) == OMP_FIRSTPRIVATE) {
                    if (varArray.compare("") == 0) {
                        outputStream << varType << " *" << varName << " = " << contextStr << "."
                                     << varName << ";\n";
                    } else {
                        outputStream << varType << " " << varName << " = " << contextStr << "."
                                     << varName << ";\n";
                        outputStream << "uint64_t " << varName << "_size = " << contextStr << "."
                                     << varName << "_size;\n";
                    }
                } else if (get<1>(var->second) == OMP_LASTPRIVATE) {
                    if (varArray.compare("") == 0) {
                        outputStream << varType << " *" << varName << " = " << contextStr << "."
                                     << varName << ";\n";
                    } else {
                        outputStream << varType << " " << varName << " = " << contextStr << "."
                                     << varName << ";\n";
                        outputStream << "uint64_t " << varName << "_size = " << contextStr << "."
                                     << varName << "_size;\n";
                    }
                }
            }
        }
    }
}

void DFGNode::printOMPInputAndVarFree_swarm(std::ostringstream& outputStream)
{
    DFGNode* parentNode = this;
    if (!(check_isa<OMPExecutableDirective>(parentNode->getStmt()))) {
        parentNode = this->findOMPParent();
    }
    for (auto var : parentNode->ompInputs) {
        /* Dont initialize threadprivate vars */
        bool declaredLocally = get<5>(var.second);
        if (declaredLocally)
            continue;

        VarTableTuple_t varTuple;
        bool found = this->myDFG->getVarTupleFromTable(this, var.first, varTuple);
        if (found == false)
            continue;
        if (get<4>(varTuple))
            continue;
        string varArray = get<6>(varTuple);

        OMP_VARSCOPE varScope = get<1>(var.second);
        string varType = get<0>(var.second);

        if (varScope == OMP_SHARED) {
            asm("nop");
        } else if (varScope == OMP_SHARED_PRIVATE) {
            asm("nop");
        } else if (varScope == OMP_PRIVATE) {
            if (parentNode->myRegion->singleThreaded == false) {
                if (varArray.compare("") == 0) {
                    outputStream << "free(" << var.first << suffixLang << parentNode->getID()
                                 << ");\n";
                }
            }
        } else if (varScope == OMP_FIRSTPRIVATE) {
            if (parentNode->myRegion->singleThreaded == false) {
                if (varArray.compare("") == 0) {
                    outputStream << "free(" << var.first << suffixLang << parentNode->getID()
                                 << ");\n";
                }
            }
        } else if (varScope == OMP_LASTPRIVATE) {
            if (parentNode->myRegion->singleThreaded == false) {
                if (varArray.compare("") == 0) {
                    outputStream << "free(" << var.first << suffixLang << parentNode->getID()
                                 << ");\n";
                }
            }
        }
    }

    if ((parentNode && check_isa<OMPParallelDirective>(parentNode->getStmt())
            && parentNode->getRegion()->hasOnlyOneChildOMPForRegion())
        == false) {
        for (auto var : parentNode->ompVariables) {
            /* Dont initialize threadprivate vars */

            bool declaredLocally = get<5>(var.second);
            if (declaredLocally)
                continue;

            VarTableTuple_t varTuple;
            bool found = this->myDFG->getVarTupleFromTable(this, var.first, varTuple);
            if (found == false)
                continue;
            if (get<4>(varTuple))
                continue;
            string varType = get<0>(varTuple);
            string varArray = get<6>(varTuple);

            if (parentNode->myRegion->singleThreaded == false) {
                if (varArray.compare("") == 0)
                    outputStream << "free(" << var.first << suffixLang << parentNode->getID()
                                 << ");\n";
            }
        }
    }
}

void DFGNode::printOMPInputAndVarInits_swarm(std::ostringstream& outputStream)
{
    DFGNode* parentNode = this;
    if (!(check_isa<OMPExecutableDirective>(parentNode->getStmt()))) {
        parentNode = this->findOMPParent();
    }

    for (auto var : parentNode->ompInputs) {
        /* Dont initialize threadprivate vars */
        bool declaredLocally = get<5>(var.second);
        if (declaredLocally)
            continue;

        VarTableTuple_t varTuple;
        bool found = this->myDFG->getVarTupleFromTable(this, var.first, varTuple);
        if (found == false)
            continue;
        if (get<4>(varTuple))
            continue;
        string varArray = get<6>(varTuple);

        OMP_VARSCOPE varScope = get<1>(var.second);
        string varType = get<0>(var.second);

        if (varScope == OMP_SHARED) {
            asm("nop");
        } else if (varScope == OMP_SHARED_PRIVATE) {
            asm("nop");
        } else if (varScope == OMP_PRIVATE) {
            if (parentNode->myRegion->singleThreaded == false) {
                if (varArray.compare("") == 0) {
#if 0
                    outputStream << var.first << suffixLang << parentNode->getID()
                    << " = (" << varType << "*)malloc(numThreads" << parentNode->getID()
                    << " * sizeof(" << varType << "));\n";
#else
                    asm("nop");
#endif
                }
            }
        } else if (varScope == OMP_FIRSTPRIVATE) {
            if (parentNode->myRegion->singleThreaded == false) {
                if (varArray.compare("") != 0) {
                    outputStream << var.first << suffixLang << parentNode->getID() << " = ("
                                 << varType << "**)malloc(numThreads" << parentNode->getID()
                                 << " * sizeof(" << varType << "*));/*OMP_FIRSTPRIVATE*/\n";
                }
            }
        } else if (varScope == OMP_LASTPRIVATE) {
            if (parentNode->myRegion->singleThreaded == false) {
                if (varArray.compare("") != 0) {
                    outputStream << var.first << suffixLang << parentNode->getID() << " = ("
                                 << varType << "**)malloc(numThreads" << parentNode->getID()
                                 << " * sizeof(" << varType << "*));/*OMP_LASTPRIVATE*/\n";
                }
            }
        }
    }

    if ((parentNode && check_isa<OMPParallelDirective>(parentNode->getStmt())
            && parentNode->getRegion()->hasOnlyOneChildOMPForRegion())
        == false) {
        for (auto var : parentNode->ompVariables) {
            /* Dont initialize threadprivate vars */
            bool declaredLocally = get<5>(var.second);
            if (declaredLocally)
                continue;

            VarTableTuple_t varTuple;
            bool found = this->myDFG->getVarTupleFromTable(this, var.first, varTuple);
            if (found == false)
                continue;
            if (get<4>(varTuple))
                continue;
            string varType = get<0>(varTuple);
            string varArray = get<6>(varTuple);

            if (parentNode->myRegion->singleThreaded == false) {
                if (varArray.compare("") == 0)
#if 0
                    outputStream << var.first << suffixLang << parentNode->getID()
                    << " = (" << varType << "*)malloc(numThreads" << parentNode->getID()
                    << " * sizeof(" << varType << "));\n";
#else
                    asm("nop");
#endif
            }
        }
    }
}

void DFGNode::printOMPInputAndVarInits(std::ostringstream& outputStream)
{
    DFGNode* parentNode = this;
    if (!(check_isa<OMPExecutableDirective>(parentNode->getStmt()))) {
        parentNode = this->findOMPParent();
    }

    TPRegion* parentRegion = parentNode->getRegion()->findOMPOrFunctionRegionParent();

    if (check_isa<OMPTaskDirective>(parentNode->getStmt())) {
        outputStream << ", task" << parentNode->getID() << "Inputs(in_task" << parentNode->getID()
                     << "Inputs)";
    } else {
        for (auto var : parentNode->ompInputs) {
            /* Dont initialize threadprivate vars */
            bool declaredLocally = get<5>(var.second);
            if (declaredLocally)
                continue;

            VarTableTuple_t varTuple;
            bool found = this->myDFG->getVarTupleFromTable(this, var.first, varTuple);
            if (found == false)
                continue;
            if (get<4>(varTuple))
                continue;
            string varArray = get<6>(varTuple);

            OMP_VARSCOPE varScope = get<1>(var.second);
            string varType = get<0>(var.second);

            if (varScope == OMP_SHARED) {
                if (parentRegion->isInlinedRegion() == false) {
                    outputStream << "," << var.first << "_darts" << parentNode->getID() << "(in_"
                                 << var.first << ")/*OMP_SHARED - INPUT*/";

                    if (varArray.compare("") != 0)
                        outputStream
                            << "," << var.first << "_outer" << parentNode->getID() << "_size"
                            << "(in_" << var.first << "_outer" << parentNode->getID() << "_size)";
                }
            } else if (varScope == OMP_SHARED_PRIVATE) {
                if (parentNode->myRegion->singleThreaded == false) {
                    outputStream << "," << var.first << "_darts" << parentNode->getID() << "(new "
                                 << get<0>(var.second) << "*[this->numThreads]"
                                 << get<4>(var.second) << ")";
                }
            } else if (varScope == OMP_PRIVATE) {
                if (parentNode->myRegion->singleThreaded == false) {
                    if (varArray.compare("") == 0)
                        outputStream << "," << var.first << "_darts" << parentNode->getID()
                                     << "(new " << varType << "[this->numThreads]"
                                     << ")/*OMP_PRIVATE - INPUT*/";
                    else
                        outputStream
                            << "," << var.first << "_outer" << parentNode->getID() << "_size"
                            << "(in_" << var.first << "_outer" << parentNode->getID() << "_size)";
                }
            } else if (varScope == OMP_FIRSTPRIVATE) {
                if (parentNode->myRegion->singleThreaded == false) {
                    if (varArray.compare("") == 0) {
                        outputStream << "," << var.first << "_darts" << parentNode->getID()
                                     << "(new " << varType << "[this->numThreads]"
                                     << ")/*OMP_FIRSTPRIVATE - INPUT*/";
                    } else {
                        outputStream
                            << "," << var.first << "_outer" << parentNode->getID() << "_size"
                            << "(in_" << var.first << "_outer" << parentNode->getID() << "_size)";
                    }
                    outputStream << "," << var.first << "_outer" << parentNode->getID() << "_ptr"
                                 << "(in_" << var.first << "_outer" << parentNode->getID()
                                 << "_ptr)";

                } else if (parentRegion->isInlinedRegion() == false) {
                    outputStream << "," << var.first << "_darts" << parentNode->getID() << "(in_"
                                 << var.first << ")";
                }
            } else if (varScope == OMP_LASTPRIVATE) {
                if (parentNode->myRegion->singleThreaded == false) {
                    if (varArray.compare("") == 0) {
                        outputStream << "," << var.first << "_darts" << parentNode->getID()
                                     << "(new " << varType << "[this->numThreads]"
                                     << ")/*OMP_LASTPRIVATE - INPUT*/";

                    } else {
                        outputStream
                            << "," << var.first << "_outer" << parentNode->getID() << "_size"
                            << "(in_" << var.first << "_outer" << parentNode->getID() << "_size)";
                    }
                    outputStream << "," << var.first << "_outer" << parentNode->getID() << "_ptr"
                                 << "(in_" << var.first << "_outer" << parentNode->getID()
                                 << "_ptr)";
                }
            }
        }
    }

    if ((parentNode && check_isa<OMPParallelDirective>(parentNode->getStmt())
            && parentNode->getRegion()->hasOnlyOneChildOMPForRegion())
        == false) {
        for (auto var : parentNode->ompVariables) {
            /* Dont initialize threadprivate vars */
            bool declaredLocally = get<5>(var.second);
            if (declaredLocally)
                continue;

            VarTableTuple_t varTuple;
            bool found = this->myDFG->getVarTupleFromTable(this, var.first, varTuple);
            if (found == false)
                continue;
            if (get<4>(varTuple))
                continue;
            string varArray = get<6>(varTuple);

            if (parentNode->myRegion->singleThreaded == false) {
                if (varArray.compare("") == 0)
                    outputStream << "," << var.first << "_darts" << parentNode->getID() << "(new "
                                 << get<0>(var.second) << "[this->numThreads]"
                                 << ")/*VARIABLE*/";
            }
        }
    }
}

void DFGNode::printOMPInputAndVarInitsRaw(std::ostringstream& outputStream)
{
    DFGNode* parentNode = this;
    if (!(check_isa<OMPExecutableDirective>(parentNode->getStmt())))
        parentNode = this->findOMPParent();

    TPRegion* parentRegion = parentNode->getRegion()->findOMPOrFunctionRegionParent();

    for (auto var : parentNode->ompInputs) {
		/* Dont initialize threadprivate vars */
        VarTableTuple_t varTuple;
        bool found = this->myDFG->getVarTupleFromTable(this, var.first, varTuple);
        if (found == false)
            continue;
        if (get<4>(varTuple))
            continue;

        if (parentRegion->isInlinedRegion() == false) {
            if (get<1>(var.second) == OMP_SHARED) {
                outputStream << "," << var.first << "(in_" << var.first << ")";
            } else if (get<1>(var.second) == OMP_FIRSTPRIVATE) {
                outputStream << "," << var.first << "(in_" << var.first << ")";
            }
        }
    }
}

void DFGNode::printOMPInputAndVarInitsRawAsInputs(std::ostringstream& outputStream)
{
    DFGNode* parentNode = this;
    if (!(check_isa<OMPExecutableDirective>(parentNode->getStmt())))
        parentNode = this->findOMPParent();

    TPRegion* parentRegion = parentNode->getRegion()->findOMPOrFunctionRegionParent();

    for (auto var : parentNode->ompInputs) {
		/* Dont initialize threadprivate vars */
        VarTableTuple_t varTuple;
        bool found = this->myDFG->getVarTupleFromTable(this, var.first, varTuple);
        if (found == false)
            continue;
        if (get<4>(varTuple))
            continue;

        if (parentRegion->isInlinedRegion() == false) {
            if (get<1>(var.second) == OMP_SHARED) {
                outputStream << ", in_" << var.first;
            } else if (get<1>(var.second) == OMP_FIRSTPRIVATE) {
                outputStream << ", in_" << var.first;
            }
        }
    }
}

void DFGNode::printOMPInputAndVarInitsFromTPLooptoCodelet(
    bool startWithComma, std::ostringstream& outputStream)
{
    DFGNode* parentNode = this;
    if (!(check_isa<OMPExecutableDirective>(parentNode->getStmt()))) {
        parentNode = this->findOMPParent();
    }

    TPRegion* parentRegion = parentNode->getRegion()->findOMPOrFunctionRegionParent();

    for (auto var : parentNode->ompInputs) {
		/* Dont initialize threadprivate vars */
        VarTableTuple_t varTuple;
        bool found = this->myDFG->getVarTupleFromTable(this, var.first, varTuple);
        if (found == false)
            continue;
        if (get<4>(varTuple))
            continue;

        if (get<1>(var.second) == OMP_SHARED) {
            if (parentRegion->isInlinedRegion() == false) {
                if (startWithComma) {
                    outputStream << ", ";
                }
                outputStream << "(in_" << var.first << ")";
            }
        } else if (get<1>(var.second) == OMP_FIRSTPRIVATE) {
            if (parentRegion->isInlinedRegion() == false) {
                if (startWithComma) {
                    outputStream << ", ";
                }
                outputStream << "(in_" << var.first << ")";
            }
        }
    }
}

void DFGNode::printOMPInputAndVarInitsInConstructor(
    std::string& outputStr, std::ostringstream& TPStream)
{
    std::ostringstream outputStream;

    DFGNode* parentNode = this;
    if (!(check_isa<OMPExecutableDirective>(parentNode->getStmt())))
        parentNode = this->findParentNode();

    string parentInputsRegionID = "";
    TPRegion* parentRegion = parentNode->getRegion()->findOMPOrFunctionRegionParent();
    if (parentRegion && parentRegion->parent
        && parentRegion->parent->findOMPOrFunctionRegionParent())
        parentInputsRegionID
            = to_string(parentRegion->parent->findOMPOrFunctionRegionParent()->getID());

    if (parentRegion->isBalancedOMPFor() && parentRegion->isOMPForOnlyChildRegion() == false
        && parentRegion->hasBarrier() == false) {
        outputStream << "this->signalNextReady[codeletCounter] = 0;\n";
    }

    if (parentNode->myRegion->singleThreaded == false
        && parentNode->getRegion()->hasOnlyOneChildOMPForRegion() == false) {
        for (auto var : parentNode->ompInputs) {
            VarTableTuple_t varTuple;
            bool found = this->myDFG->getVarTupleFromTable(this, var.first, varTuple);
            if (found == false)
                continue;
            /* Dont initialize threadprivate vars */
            if (get<4>(varTuple))
                continue;
            string varArray = get<6>(varTuple);
            string varType = get<0>(varTuple);

            OMP_VARSCOPE varScope = get<1>(var.second);

            if (varScope == OMP_SHARED_PRIVATE) {
                if (varArray.compare("") == 0) {
                    TPStream << "this->" << var.first << "_darts" << parentNode->getID() << " = ("
                             << varType << "**)malloc(sizeof(" << varType
                             << "*) * this->numThreads)/*OMP_SHARED_PRIVATE*/;\n";
                } else {
                    TPStream << "this->" << var.first << "_darts" << parentNode->getID() << " = ("
                             << varType << "**)malloc(sizeof(" << varType
                             << "*) * this->numThreads)/*OMP_SHARED_PRIVATE*/;\n";
                }
            } else if (varScope == OMP_PRIVATE) {
                if (varArray.compare("") != 0) {
                    TPStream << "this->" << var.first << "_darts" << parentNode->getID() << " = ("
                             << varType << "**)malloc(sizeof(" << varType
                             << "*) * this->numThreads)/*OMP_PRIVATE*/;\n";
                }
            } else if (varScope == OMP_FIRSTPRIVATE) {
                if (varArray.compare("") == 0) {
                    outputStream << var.first << "_darts" << parentNode->getID()
                                 << "[codeletCounter]="
                                 << " *in_" << var.first << "_outer" << parentNode->getID()
                                 << "_ptr;/*OMP_FIRSTPRIVATE*/\n";
                    if (parentRegion->isBalancedOMPFor()
                        && parentRegion->isOMPForOnlyChildRegion() == false) {
                        outputStream << "for(size_t i = 1; i < (size_t)DARTS_CODELETS_MULT; i++){\n"
                                     << var.first << "_darts" << parentNode->getID()
                                     << "[codeletCounter + this->baseNumThreads * i] = "
                                     << " *in_" << var.first << "_outer" << parentNode->getID()
                                     << "_ptr;\n"
                                     << "}\n";
                    }
                } else {
                    TPStream << "this->" << var.first << "_darts" << parentNode->getID() << " = ("
                             << varType << "**)malloc(sizeof(" << varType
                             << "*) * this->numThreads);/*OMP_FIRSTPRIVATE*/\n";
                }
            } else if (varScope == OMP_LASTPRIVATE) {
                if (varArray.compare("") != 0) {
                    TPStream << "this->" << var.first << "_darts" << parentNode->getID() << " = ("
                             << varType << "**)malloc(sizeof(" << varType
                             << "*) * this->numThreads);/*OMP_LASTPRIVATE*/\n";
                }
            }
        }
        for (auto var : parentNode->ompVariables) {
            VarTableTuple_t varTuple;
            bool found = this->myDFG->getVarTupleFromTable(this, var.first, varTuple);
            if (found == false)
                continue;
            if (get<4>(varTuple))
                continue;
            string varArray = get<6>(varTuple);
            string varType = get<0>(varTuple);

            if (varArray.compare("") != 0) {
                TPStream << "this->" << var.first << "_darts" << parentNode->getID() << " = ("
                         << varType << "**)malloc(sizeof(" << varType
                         << "*) * this->numThreads);/*VARIABLE*/\n";
            }
        }
    } else {
        for (auto var : parentNode->ompInputs) {
            /* Dont initialize threadprivate vars */
            VarTableTuple_t varTuple;
            bool found = this->myDFG->getVarTupleFromTable(this, var.first, varTuple);
            if (found == false)
                continue;
            if (get<4>(varTuple))
                continue;
#if 0
			if (get < 1 > (var.second) == OMP_SHARED_PRIVATE) {
				outputStream << var.first << "_darts" << parentNode->getID()
					<< "= &(this->TPParent->inputsTPParent->" << var.first
					<< "_darts" << parentInputsRegionID <<
					"[this->mainCodeletID]);\n";
			}
#endif
        }
    }
    outputStr += outputStream.str();
}

void DFGNode::printOMPArrayInputInitsInFirstCodelet(
    std::ostringstream& outputStream, TPRegion* region)
{
    DFGNode* parentNode = this;
    for (auto var : parentNode->ompInputs) {
        VarTableTuple_t varTuple;
        bool found
            = region->getDFG()->getVarTupleFromTable(region->getMainNode(), var.first, varTuple);
        if (found == false)
            continue;
        if (get<4>(varTuple))
            continue;
        string varType = get<0>(varTuple);
        string varArray = get<6>(varTuple);
        string varSize = get<8>(varTuple);

        OMP_VARSCOPE varScope = get<1>(var.second);

        /*Allocate each arrray var in the codelet */
        if (varScope == OMP_PRIVATE || varScope == OMP_FIRSTPRIVATE
            || varScope == OMP_LASTPRIVATE) {
            if (varArray.compare("") != 0) {

                if (DARTS_BACKEND) {
                    std::ostringstream varSizeStr;
                    varSizeStr << DARTS_PREFIXSTR_DEFAULT << var.first << "_outer"
                               << parentNode->getID() << "_size";

                    outputStream << DARTS_PREFIXSTR_DEFAULT << var.first << suffixLang
                                 << parentNode->getID() << "[this->getID()] = "
                                 << "(" << varType << "*)malloc(sizeof(" << varType << ") * "
                                 << varSizeStr.str() << ");\n";
                } else if (SWARM_BACKEND) {
                    std::ostringstream varSizeStr;
                    varSizeStr << var.first << "_outer" << parentNode->getID() << "_size";

                    outputStream << var.first << suffixLang << parentNode->getID()
                                 << "[codeletID] = "
                                 << "(" << varType << "*)malloc(sizeof(" << varType << ") * "
                                 << varSizeStr.str() << ");\n";
                }
            }
        }

        /*Copy the values from a firstprivate array var to the TP var. */
        if (varScope == OMP_FIRSTPRIVATE) {
            if (varArray.compare("") != 0) {

                if (DARTS_BACKEND) {
                    outputStream << varType << " *temp_" << var.first << "_darts_ptr = "
                                 << "(" << varType << " *)(" << DARTS_PREFIXSTR_DEFAULT << var.first
                                 << "_darts" << parentNode->getID() << "[this->getID()]);\n";

                    outputStream << varType << " *temp_" << var.first << "_outer_ptr = "
                                 << "(" << varType << " *)(" << DARTS_PREFIXSTR_DEFAULT << var.first
                                 << "_outer" << parentNode->getID() << "_ptr);\n";

                    std::ostringstream varSizeStr;
                    varSizeStr << DARTS_PREFIXSTR_DEFAULT << var.first << "_outer"
                               << parentNode->getID() << "_size";

                    outputStream << "for (uint64_t k_outer" << parentNode->getID() << " = 0;"
                                 << " k_outer" << parentNode->getID() << " < " << varSizeStr.str()
                                 << "; k_outer" << parentNode->getID() << "++){\n"
                                 << "temp_" << var.first << "_darts_ptr[k_outer"
                                 << parentNode->getID() << "] = "
                                 << "temp_" << var.first << "_outer_ptr[k_outer"
                                 << parentNode->getID() << "];\n"
                                 << "}\n";
                } else if (SWARM_BACKEND) {
                    outputStream << varType << " *temp_" << var.first << suffixLang << "_ptr = "
                                 << "(" << varType << " *)(" << var.first << suffixLang
                                 << parentNode->getID() << "[codeletID]);\n";

                    outputStream << varType << " *temp_" << var.first << "_outer_ptr = "
                                 << "(" << varType << " *)(" << var.first << "_outer"
                                 << parentNode->getID() << "_ptr);\n";

                    std::ostringstream varSizeStr;
                    varSizeStr << var.first << "_outer" << parentNode->getID() << "_size";

                    outputStream << "for (uint64_t k_outer" << parentNode->getID() << " = 0;"
                                 << " k_outer" << parentNode->getID() << " < " << varSizeStr.str()
                                 << "; k_outer" << parentNode->getID() << "++){\n"
                                 << "temp_" << var.first << suffixLang << "_ptr[k_outer"
                                 << parentNode->getID() << "] = "
                                 << "temp_" << var.first << "_outer_ptr[k_outer"
                                 << parentNode->getID() << "];\n"
                                 << "}\n";
                }
            }
        } else if (varScope == OMP_LASTPRIVATE) {
            if (varArray.compare("") != 0) {
                if (DARTS_BACKEND) {
                    outputStream << varType << " *temp_" << var.first << "_darts_ptr = (" << varType
                                 << " *)(" << DARTS_PREFIXSTR_DEFAULT << var.first << "_darts"
                                 << parentNode->getID() << "[this->getID()]);\n";

                    outputStream << varType << " *temp_" << var.first << "_outer_ptr = (" << varType
                                 << " *)(" << DARTS_PREFIXSTR_DEFAULT << var.first << "_outer"
                                 << parentNode->getID() << "_ptr);\n";
                } else if (SWARM_BACKEND) {
                    outputStream << varType << " *temp_" << var.first << suffixLang << "_ptr = ("
                                 << varType << " *)(" << var.first << suffixLang
                                 << parentNode->getID() << "[this->getID()]);\n";

                    outputStream << varType << " *temp_" << var.first << "_outer_ptr = (" << varType
                                 << " *)(" << var.first << "_outer" << parentNode->getID()
                                 << "_ptr);\n";
                }
            }
        }
    }
}

void DFGNode::printOMPArrayVarInitsInFirstCodelet(
    std::ostringstream& outputStream, TPRegion* region)
{
    DFGNode* parentNode = this;

    for (auto var : parentNode->ompVariables) {
        VarTableTuple_t varTuple;
        bool found
            = region->getDFG()->getVarTupleFromTable(region->getMainNode(), var.first, varTuple);
        if (found == false)
            continue;
        if (get<4>(varTuple))
            continue;
        string varType = get<0>(varTuple);
        string varArray = get<6>(varTuple);
        string varSize = get<8>(varTuple);

        if (varArray.compare("") != 0) {

            outputStream << "/*Allocate each array var in the codelet */\n";

            if (DARTS_BACKEND) {
                /*Init the array's var's size */
                std::string varSizeStr;
                parentNode->getVarSize(varSize, parentNode, varSizeStr, DARTS_PREFIXSTR_DEFAULT,
                    "[this->getLocalID()]");
                outputStream << DARTS_PREFIXSTR_DEFAULT << var.first << "_outer"
                             << parentNode->getID() << "_size = " << varSizeStr << ";\n";

                /*Init the array's var' */
                outputStream << DARTS_PREFIXSTR_DEFAULT << var.first << suffixLang
                             << parentNode->getID() << "[this->getID()] = (" << varType
                             << "*)malloc(sizeof(" << varType << ") * " << DARTS_PREFIXSTR_DEFAULT
                             << var.first << "_outer" << parentNode->getID() << "_size"
                             << ");\n";
            } else if (SWARM_BACKEND) {
                /*Init the array's var's size */
                std::string varSizeStr;
                parentNode->getVarSize(varSize, parentNode, varSizeStr, "", "[codeletID]");
                outputStream << var.first << "_outer" << parentNode->getID()
                             << "_size = " << varSizeStr << ";\n";

                /*Init the array's var' */
                outputStream << var.first << suffixLang << parentNode->getID() << "[codeletID] = ("
                             << varType << "*)malloc(sizeof(" << varType << ") * " << var.first
                             << "_outer" << parentNode->getID() << "_size"
                             << ");\n";
            }
        }
    }
}

void DFGNode::printOMPInputAndVarInitsInFirstCodelet(std::ostringstream& outputStream)
{
    DFGNode* parentNode = this;
    if (!(check_isa<OMPExecutableDirective>(parentNode->getStmt())))
        parentNode = this->findParentNode();

    string parentInputsRegionID = "";
    TPRegion* parentRegion = parentNode->getRegion()->findOMPOrFunctionRegionParent();
    if (parentRegion && parentRegion->parent
        && parentRegion->parent->findOMPOrFunctionRegionParent()) {
        parentInputsRegionID
            = to_string(parentRegion->parent->findOMPOrFunctionRegionParent()->getID());
    }

    if (parentNode->getRegion()->singleThreaded == false) {
        for (auto var : parentNode->ompInputs) {
            /* Dont initialize threadprivate vars */

            VarTableTuple_t varTuple;
            bool found = this->myDFG->getVarTupleFromTable(this, var.first, varTuple);
            if (found == false)
                continue;
            if (get<4>(varTuple))
                continue;
            string varArray = get<6>(varTuple);
            string varType = get<0>(varTuple);

            if (get<1>(var.second) == OMP_SHARED_PRIVATE) {
                outputStream << "/*Get pointer from parent for variable\n"
                             << " with shared scope in this region but private\n"
                             << " in the enclosing one.*/\n";
                if (parentRegion->isInlinedRegion() == false) {
                    if (DARTS_BACKEND) {
                        if (varArray.compare("") != 0) {
                            outputStream << DARTS_PREFIXSTR_DEFAULT << var.first << suffixLang
                                         << parentNode->getID() << "[this->getLocalID()] = ("
                                         << varType << "*)"
                                         << "&(myTP->TPParent->inputsTPParent->" << var.first
                                         << suffixLang << parentInputsRegionID
                                         << "[this->getID()][0]);\n";
                        } else {
                            outputStream << DARTS_PREFIXSTR_DEFAULT << var.first << suffixLang
                                         << parentNode->getID() << "[this->getLocalID()] = ("
                                         << varType << "*)"
                                         << "&(myTP->TPParent->inputsTPParent->" << var.first
                                         << suffixLang << parentInputsRegionID << "[this->getID()]"
                                         << varArray << ");\n";
                        }
                    } else if (SWARM_BACKEND) {
                        if (varArray.compare("") != 0) {
                            outputStream << var.first << suffixLang << parentNode->getID()
                                         << "[codeletID] = (" << varType << "*)&(" << var.first
                                         << suffixLang << parentInputsRegionID
                                         << "[codeletID][0]);\n";
                        } else {
                            outputStream << var.first << suffixLang << parentNode->getID()
                                         << "[codeletID] = (" << varType << "*)&(" << var.first
                                         << suffixLang << parentInputsRegionID << "[codeletID]"
                                         << varArray << ");\n";
                        }
                    }
                } else {
                    if (DARTS_BACKEND) {
                        outputStream
                            << DARTS_PREFIXSTR_DEFAULT << var.first << suffixLang
                            << parentNode->getID() << "[this->getLocalID()] = (" << varType << "*)"
                            << "&(" << DARTS_PREFIXSTR_DEFAULT << var.first << suffixLang
                            << parentInputsRegionID << "[this->getLocalID()]" << varArray << ");\n";
                    } else if (SWARM_BACKEND) {
                        if (varArray.compare("") != 0) {
                            outputStream << var.first << suffixLang << parentNode->getID()
                                         << "[codeletID] = (" << varType << "*)&(" << var.first
                                         << suffixLang << parentInputsRegionID
                                         << "[codeletID][0]);\n";
                        } else {
                            outputStream << var.first << suffixLang << parentNode->getID()
                                         << "[codeletID] = (" << varType << "*)&(" << var.first
                                         << suffixLang << parentInputsRegionID << "[codeletID]"
                                         << varArray << ");\n";
                        }
                    }
                }
            }
        }
    } else {
        for (auto var : parentNode->ompInputs) {
            /* Dont initialize threadprivate vars */

            VarTableTuple_t varTuple;
            bool found = this->myDFG->getVarTupleFromTable(this, var.first, varTuple);
            if (found == false)
                continue;
            if (get<4>(varTuple))
                continue;
            string varArray = get<6>(varTuple);
            string varType = get<0>(varTuple);

            if (get<1>(var.second) == OMP_SHARED_PRIVATE) {
                outputStream << "/*Get pointer from parent for variable\n"
                             << " with shared scope in this region but private\n"
                             << " in the enclosing one*/\n";
                if (parentRegion->isInlinedRegion() == false) {
                    if (DARTS_BACKEND)
                        outputStream << DARTS_PREFIXSTR_DEFAULT << var.first << suffixLang
                                     << parentNode->getID() << " = (" << varType << "*)"
                                     << "&(myTP->TPParent->inputsTPParent->" << var.first
                                     << suffixLang << parentInputsRegionID << "[this->getID()]"
                                     << varArray << ");\n";
                    else if (SWARM_BACKEND)
                        outputStream << var.first << suffixLang << parentNode->getID() << " = ("
                                     << varType << "*)"
                                     << "&(" << var.first << suffixLang << parentInputsRegionID
                                     << "[codeletID]" << varArray << ");\n";
                } else {
                    if (DARTS_BACKEND)
                        outputStream << DARTS_PREFIXSTR_DEFAULT << var.first << suffixLang
                                     << parentNode->getID() << " = (" << varType << "*)"
                                     << "&(" << DARTS_PREFIXSTR_DEFAULT << var.first << suffixLang
                                     << parentInputsRegionID << "[this->getID()]" << varArray
                                     << ");\n";
                    else if (SWARM_BACKEND)
                        outputStream << var.first << suffixLang << parentNode->getID() << " = ("
                                     << varType << "*)"
                                     << "&(" << var.first << suffixLang << parentInputsRegionID
                                     << "[codeletID]" << varArray << ");\n";
                }
            }
        }
    }
}

void DFGNode::printOMPInputAndVarInitsInlinedRegion(std::ostringstream& outputStream)
{
    DFGNode* parentNode = this;
    if (!(check_isa<OMPExecutableDirective>(parentNode->getStmt()))) {
        parentNode = this->findParentNode();
    }

    string parentInputsRegionID = "";
    TPRegion* parentRegion = parentNode->getRegion()->findOMPOrFunctionRegionParent();
    if (parentRegion && parentRegion->parent
        && parentRegion->parent->findOMPOrFunctionRegionParent()) {
        parentInputsRegionID
            = to_string(parentRegion->parent->findOMPOrFunctionRegionParent()->getID());
    }

    if (parentNode->ompInputs.size() > 0) {
        outputStream << "/*Initialize the vars of the inlined region*/\n";
    }

    string prefixStr = "";
    string postfixStr = "";

    if (DARTS_BACKEND) {
        prefixStr = DARTS_PREFIXSTR_DEFAULT;
        postfixStr = "[this->getLocalID()]";
    } else if (SWARM_BACKEND) {
        prefixStr = "";
        postfixStr = "[codeletID]";
    }

    if (parentNode->getRegion()->singleThreaded == false) {
        for (auto var : parentNode->ompInputs) {
            /* Dont initialize threadprivate vars */
            VarTableTuple_t varTuple;
            bool found = this->myDFG->getVarTupleFromTable(this, var.first, varTuple);
            if (found == false)
                continue;
            if (get<4>(varTuple))
                continue;

            string varType = get<0>(var.second);

            if (get<1>(var.second) == OMP_SHARED) {

                if (DARTS_BACKEND)
                    outputStream << prefixStr << var.first << suffixLang << parentNode->getID()
                                 << " = (" << prefixStr << var.first << suffixLang
                                 << parentInputsRegionID << ")/*OMP_SHARED - INPUT INLINED*/;\n";
                else if (SWARM_BACKEND)
                    outputStream << prefixStr << var.first << suffixLang << parentNode->getID()
                                 << " = (" << prefixStr << var.first << suffixLang
                                 << parentInputsRegionID << ")/*OMP_SHARED - INPUT INLINED*/;\n";

            } else if (get<1>(var.second) == OMP_PRIVATE) {

                if (DARTS_BACKEND)
#if 0
                    outputStream << prefixStr << var.first << suffixLang << parentNode->getID()
                                 << "= new " << varType
                                 << "[myTP->numThreads]/*OMP_PRIVATE - INPUT INLINED*/;\n";
#else
					asm("nop");
#endif
                else if (SWARM_BACKEND)
                    outputStream << prefixStr << var.first << suffixLang << parentNode->getID()
                                 << " = (" << varType << "*)malloc(sizeof(" << varType
                                 << ") * numThreads" << parentInputsRegionID
                                 << ")/*OMP_PRIVATE - INPUT INLINED*/;\n";

            } else if (get<1>(var.second) == OMP_FIRSTPRIVATE) {

                if (DARTS_BACKEND)
                    outputStream << prefixStr << var.first << suffixLang << parentNode->getID()
                                 << postfixStr << " = (" << prefixStr << var.first << suffixLang
                                 << parentInputsRegionID << postfixStr
                                 << ")/*OMP_FIRSTPRIVATE - INPUT INLINED*/;\n";
                else if (SWARM_BACKEND)
                    outputStream << prefixStr << var.first << suffixLang << parentNode->getID()
                                 << postfixStr << " = (" << prefixStr << var.first << suffixLang
                                 << parentInputsRegionID << postfixStr
                                 << ")/*OMP_FIRSTPRIVATE - INPUT INLINED*/;\n";

            } else if (get<1>(var.second) == OMP_SHARED_PRIVATE) {

                if (DARTS_BACKEND)
                    outputStream << prefixStr << var.first << suffixLang << parentNode->getID()
                                 << postfixStr << "= &(" << prefixStr << var.first << suffixLang
                                 << parentInputsRegionID << postfixStr
                                 << ")/*OMP_SHARED_PRIVATE - INPUT INLINED*/;\n";
                else if (SWARM_BACKEND)
                    outputStream << prefixStr << var.first << suffixLang << parentNode->getID()
                                 << postfixStr << "= &(" << prefixStr << var.first << suffixLang
                                 << parentInputsRegionID << postfixStr
                                 << ")/*OMP_SHARED_PRIVATE - INPUT INLINED*/;\n";
            }
        }
    } else {
        for (auto var : parentNode->ompInputs) {
            /* Dont initialize threadprivate vars */
            VarTableTuple_t varTuple;
            bool found = this->myDFG->getVarTupleFromTable(this, var.first, varTuple);
            if (found == false)
                continue;
            if (get<4>(varTuple))
                continue;

            string varType = get<0>(var.second);

            if (get<1>(var.second) == OMP_SHARED) {
                outputStream << prefixStr << var.first << suffixLang << parentNode->getID()
                             << " = (" << prefixStr << var.first << suffixLang
                             << parentInputsRegionID << ")/*OMP_SHARED - VAR INLINED*/;\n";
            }
/*TODO: Apparently, we haven't used this in any of the benchmarks we have tried
so far, but let's leave this code here in case we need it in some point*/
#if 0
            else if(get<1>(var.second) == OMP_PRIVATE) {
				outputStream  << DARTS_PREFIXSTR_DEFAULT << var.first
							  <<  suffixLang << parentNode->getID()
							  << "= new " <<  get<0>(var.second)
							  <<  "[myTP->numThreads]/*OMP_PRIVATE - VAR INLINED*/;\n";
            }
#endif
            else if (get<1>(var.second) == OMP_FIRSTPRIVATE) {
                outputStream << prefixStr << var.first << suffixLang << parentNode->getID()
                             << postfixStr << " = (" << prefixStr << var.first << suffixLang
                             << parentInputsRegionID << postfixStr
                             << ")/*OMP_FIRSTPRIVATE - VAR INLINED*/;\n";
            } else if (get<1>(var.second) == OMP_SHARED_PRIVATE) {
                outputStream << prefixStr << var.first << suffixLang << parentNode->getID()
                             << "= &(" << prefixStr << var.first << suffixLang
                             << parentInputsRegionID << postfixStr
                             << ")/*OMP_SHARED_PRIVATE - VAR INLINED*/;\n";
            }
        }
    }
}

void DFGNode::printOMPInputAndVarFreeInDestructor(std::ostringstream& outputStream)
{
    DFGNode* parentNode = this;
    if (!(check_isa<OMPExecutableDirective>(parentNode->getStmt())))
        parentNode = this->findOMPParent();

    if (check_isa<OMPTaskDirective>(parentNode->getStmt())) {
        outputStream << "delete (task" << parentNode->getID() << "Inputs);\n";
    } else if (parentNode->myRegion->singleThreaded == false) {

        for (auto var : parentNode->ompInputs) {
            bool declaredLocally = get<5>(var.second);
            if (declaredLocally)
                continue;
            /* Dont delete threadprivate vars */
            VarTableTuple_t varTuple;
            bool found = this->myDFG->getVarTupleFromTable(this, var.first, varTuple);
            if (found == false)
                continue;
            if (get<4>(varTuple))
                continue;
            string varArray = get<6>(varTuple);
            OMP_VARSCOPE varScope = get<1>(var.second);

            if (varScope == OMP_PRIVATE
				|| varScope == OMP_FIRSTPRIVATE
                || varScope == OMP_LASTPRIVATE) {
                if (varArray.compare("") != 0) {
                    outputStream << "delete [] " << var.first << "_darts" << parentNode->getID()
                                 << ";\n";
                }
            } else if (varScope == OMP_SHARED_PRIVATE) {
                outputStream << "delete [] " << var.first << "_darts" << parentNode->getID()
                             << ";\n";
            }
        }

        if ((parentNode && check_isa<OMPParallelDirective>(parentNode->getStmt())
                && parentNode->getRegion()->hasOnlyOneChildOMPForRegion())
            == false) {
            for (auto var : parentNode->ompVariables) {

                bool declaredLocally = get<5>(var.second);
                if (declaredLocally)
                    continue;

                /* Dont delete threadprivate vars */
                VarTableTuple_t varTuple;
                bool found = this->myDFG->getVarTupleFromTable(this, var.first, varTuple);
                if (found == false)
                    continue;
                if (get<4>(varTuple))
                    continue;
                string varArray = get<6>(varTuple);
                if (varArray.find("[") == string::npos)
                    outputStream << "delete []" << var.first << "_darts" << parentNode->getID()
                                 << varArray << ";\n";
            }
        }
    }
}

bool DFGNode::printOMPInputAsParams(bool startWithComma, std::ostringstream& outputStream)
{
    bool res = false;
    DFGNode* parentNode = this;
    if (!(check_isa<OMPExecutableDirective>(parentNode->getStmt())))
        parentNode = this->findOMPParent();

    if (check_isa<OMPTaskDirective>(parentNode->getStmt())) {
        if (startWithComma)
            outputStream << ", ";
        outputStream << "_task" << parentNode->getID() << "Inputs* in_task" << parentNode->getID()
                     << "Inputs";
    } else if (parentNode && parentNode->ompInputs.size() > 0) {

        TPInputs_t::iterator var = parentNode->ompInputs.begin();
        for (; var != parentNode->ompInputs.end(); var++) {

            VarTableTuple_t varTuple;
            bool found = this->myDFG->getVarTupleFromTable(this, var->first, varTuple);
            if (found == false)
                continue;
            if (get<4>(varTuple))
                continue;

            string varType = get<0>(var->second);
            string varArray = get<4>(var->second);

            if (get<1>(var->second) == OMP_SHARED) {

                if (startWithComma)
                    outputStream << ", ";
                startWithComma = true;

                outputStream << varType << " *in_" << var->first;

                if (varArray.compare("") != 0)
                    outputStream << ", int in_" << var->first << "_outer" << parentNode->getID()
                                 << "_size";

            } else if (get<1>(var->second) == OMP_PRIVATE) {
                if (varArray.compare("") != 0) {
                    if (startWithComma)
                        outputStream << ", ";
                    startWithComma = true;

                    outputStream << "uint64_t in_" << var->first << "_outer" << parentNode->getID()
                                 << "_size";
                }

            } else if (get<1>(var->second) == OMP_FIRSTPRIVATE) {

                if (startWithComma)
                    outputStream << ", ";
                startWithComma = true;

                outputStream << varType << " *in_" << var->first << "_outer" << parentNode->getID()
                             << "_ptr";

                if (varArray.compare("") != 0)
                    outputStream << ", uint64_t in_" << var->first << "_outer"
                                 << parentNode->getID() << "_size";

            } else if (get<1>(var->second) == OMP_LASTPRIVATE) {

                if (startWithComma)
                    outputStream << ", ";
                startWithComma = true;

                outputStream << varType << " *in_" << var->first << "_outer" << parentNode->getID()
                             << "_ptr";
                if (varArray.compare("") != 0)
                    outputStream << ", uint64_t in_" << var->first << "_outer"
                                 << parentNode->getID() << "_size";
            }
        }
    }
    return res;
}

bool DFGNode::printOMPInputAsParamsRaw(bool startWithComma, std::ostringstream& outputStream)
{
    bool res = false;
    DFGNode* parentNode = this;
    if (!(check_isa<OMPExecutableDirective>(parentNode->getStmt()))) {
        parentNode = this->findOMPParent();
    }
    if (parentNode && parentNode->ompInputs.size() > 0) {
		bool printComma = startWithComma;
		TPInputs_t::iterator var = parentNode->ompInputs.begin();
        for (; var != parentNode->ompInputs.end(); var++) {
            if (get<1>(var->second) == OMP_SHARED) {
				if(printComma)
					outputStream << ", ";
				printComma = true;
                outputStream << get<0>(var->second) << "* in_" << var->first;
            } else if (get<1>(var->second) == OMP_FIRSTPRIVATE) {
				if(printComma)
					outputStream << ", ";
				printComma = true;
                outputStream << get<0>(var->second) << " in_" << var->first;
            }
        }
    }
    return res;
}

bool DFGNode::printFunctionInputsAsParams(bool startWithComma, std::ostringstream& outputStream)
{
    bool printed = false;
    DFGNode* functionNode = this;
    if (functionNode->myFunctionInfo == nullptr) {
        functionNode = functionNode->findFunctionParent();
    }

    if (functionNode && functionNode->ompVariables.size() > 0) {
        printed = true;

        OMP_VARINFO initTuple("", OMP_SHARED, -1, "", "", false);
        pair<string, OMP_VARINFO> initPair("none", initTuple);
        vector<pair<string, OMP_VARINFO> > orderedVars(functionNode->ompVariables.size(), initPair);
        for (auto var = functionNode->ompVariables.cbegin();
             var != functionNode->ompVariables.cend(); ++var) {
            if (get<2>(var->second) >= 0) {
                OMP_VARINFO newTuple(get<0>(var->second), get<1>(var->second), get<2>(var->second),
                    get<3>(var->second), get<4>(var->second), false);
                std::pair<std::string, omp2cd_space::OMP_VARINFO> newPair(var->first, newTuple);
                orderedVars[get<2>(var->second)] = newPair;
            }
        }
		vector<pair<string, OMP_VARINFO> >::iterator var = orderedVars.begin();
		bool printComma = startWithComma;
        for (; var != orderedVars.end(); var++) {
            if (var->first.compare("none") != 0) {
				if(printComma)
					outputStream << ", ";
				printComma = true;
                outputStream << get<0>(var->second) << " in_" << var->first;
            }
        }
    }
    return printed;
}

void DFGNode::printUnpackOMPTaskInputs(std::ostringstream& outputStream)
{
    DFGNode* ompNode = this;
    if (!(check_isa<OMPTaskDirective>(ompNode->getStmt())))
        return;

    if (ompNode && ompNode->ompInputs.size() > 0) {
        for (auto var : ompNode->ompInputs) {
            if (get<1>(var.second) == OMP_SHARED) {
                outputStream << "myTP->task" << this->getID() << "_" << var.first << " = "
                             << "task" << ompNode->id << "Inputs->in_" << var.first << ";\n";
            } else {
                outputStream << "myTP->task" << this->getID() << "_" << var.first
                             << "[this->getID()] = task" << ompNode->id << "Inputs->in_"
                             << var.first << ";\n";
            }
        }
    }
}

void DFGNode::printOMPTaskInputsInInvoke(bool startWithComma, std::ostringstream& outputStream)
{
    DFGNode* ompNode = this;
    if (!(check_isa<OMPTaskDirective>(ompNode->s)))
        return;

    if (ompNode && ompNode->ompInputs.size() > 0) {
        if (startWithComma)
            outputStream << ", ";
        auto var = ompNode->ompInputs.begin();
        outputStream << "temp_" << var->first;
        var++;
        for (; var != ompNode->ompInputs.end(); var++)
            outputStream << ", temp_" << var->first;
    }
}

void DFGNode::printOMPInputGetFromParent(std::ostringstream& outputStream)
{
    DFGNode* ompNode = this;
    if (!(ompNode->s && isa<OMPExecutableDirective>(ompNode->s))) {
        ompNode = this->findOMPParent();
    }

    DFGNode* ompParentOmpNode = ompNode->parent;
    if (ompParentOmpNode && !(check_isa<OMPExecutableDirective>(ompParentOmpNode->s))) {
        ompParentOmpNode = ompParentOmpNode->findOMPParent();
    }

    for (auto var : ompNode->ompInputs) {
        if (get<1>(var.second) == OMP_FIRSTPRIVATE) {
            TPInputs_t::iterator ompParentOmpNodeIt = ompParentOmpNode->ompInputs.find(var.first);

            if (ompParentOmpNodeIt != ompParentOmpNode->ompInputs.end()) {
                outputStream << "myTP->" << var.first << "[this->getID()] = ";

                if (get<1>((*ompParentOmpNodeIt).second) == OMP_SHARED) {
                    outputStream << " *(myTP->TPParent->" << var.first << ");\n";
                } else {
                    outputStream << " myTP->TPParent->" << var.first << "[this->getID()];\n";
                }
            }
        }
    }
}

void DFGNode::declareCriticalAtomicMutexes(std::ostringstream& globalVarsStream)
{
    if (this->criticalDirectiveName.empty() == false && this->criticalDirectiveLock == true) {
        ostringstream mutexName;

        if (DARTS_BACKEND)
            mutexName << "TP" << this->criticalDirectiveName << "mutex";
        else if (SWARM_BACKEND)
            mutexName << "volatile int Codelet" << this->criticalDirectiveName
                      << "mutex  = swarm_spinlock_INITIALIZER";
        /*make sure the mutex is not declared twice*/
        if (globalVarsStream.str().find(mutexName.str()) == string::npos) {
            if (DARTS_BACKEND)
                globalVarsStream << "std::mutex " << mutexName.str() << ";\n";
            else if (SWARM_BACKEND)
                globalVarsStream << mutexName.str() << ";\n";
        }
    } else if (this->myNodes.size() > 0) {
        DFGNode* childNode = this->myNodes.front();
        while (childNode) {
            childNode->declareCriticalAtomicMutexes(globalVarsStream);
            childNode = childNode->next;
        }
    }
}

void DFGNode::printParallelReductionVarsDef(std::ostringstream& outputStream)
{
    if (this->parallelReductionVars.size() > 0) {
        for (auto& myPair : this->parallelReductionVars) {
            string varName = myPair.first;
            string varType = "";
            auto inputsIt = this->ompInputs.find(varName);
            if (inputsIt != this->ompInputs.end()) {
                varType = get<0>(inputsIt->second);
            } else {
                auto varsIt = this->ompVariables.find(varName);
                if (varsIt != this->ompVariables.end())
                    varType = get<0>(varsIt->second);
            }

            ostringstream newVarName;
            newVarName << varName << suffixLang << this->getID();

            ostringstream localName;
            localName << newVarName.str() << "_reductionVar";

            if (DARTS_BACKEND) {
                if (this->isOMPParallel())
                    outputStream << "std::mutex " << newVarName.str() << "_mutex;\n";
                else
                    outputStream << "std::mutex * " << newVarName.str() << "_mutex;\n";

                outputStream << "std::mutex " << localName.str() << "_mutex;\n";

            } else if (SWARM_BACKEND) {
                outputStream << "volatile int " << newVarName.str() << "_mutex;\n";

                outputStream << "volatile int " << localName.str() << "_mutex;\n";
            }

            outputStream << varType << "* " << localName.str() << ";\n";
            outputStream << "size_t " << localName.str() << "_counter;\n";

            get<1>(myPair.second) = varType;
            get<2>(myPair.second) = localName.str();
        }
    }
}

void DFGNode::printExtraVarsDef(
    std::ostringstream& outputStream, std::ostringstream& globalVarsStream)
{
    DFGNode* parentNode = this;
    if (!(check_isa<OMPExecutableDirective>(parentNode->getStmt())))
        parentNode = this->findOMPParent();

	/*Variable counting the number of codelets already in the region*/
    if (SWARM_BACKEND) {
        if (this->isRegionMainNode && this->getRegion()->getParent()) {
            if (check_isa<OMPExecutableDirective>(this->getRegion()->getParent()->getStmt())) {
                outputStream << "uint32_t countCodelets" << this->getRegion()->getParent()->getID()
                             << ";\n";
                outputStream << "uint32_t numThreads" << this->getRegion()->getParent()->getID() << ";\n";
            }
        }
    }

	/*Variables to control the calls to functions*/
    if (this->isCallerNode()) {
        if (DARTS_BACKEND) {
            if (this->calleeFunctionNodeIsSingleThreaded == false) {
                outputStream << "TP_" << this->calleeFunctionNodeName << "* TP"
							 << this->getID() << "Ptr;\n"
							 << "int TP" << this->getID() << "_alreadyLaunched;\n";
            }
        } else if (SWARM_BACKEND) {
            if (this->calleeTmpVarName.compare("") != 0) {
                if (this->calleeFunctionNodeIsSingleThreaded) {
                    outputStream << this->calleeFunctionNodeType << " " << this->calleeTmpVarName
                                 << ";\n";
                } else {
                    outputStream << this->calleeFunctionNodeType << " " << this->calleeTmpVarName
                                 << "[MAXNUMTHREADS];\n";
                }
            }
            outputStream << "size_t Codelet" << this->calleeFunctionNodeName
                         << "Entry_alreadySetFrom_Codelet" << this->getID() << ";\n";
        }
    } 
	
	/*Variables for omp regions*/
	else if (this->getRegion()->getParent() == nullptr
        || this->getRegion()->getParent()->isSingleThreaded() == false) {

        if (check_isa<OMPExecutableDirective>(this->getStmt())
            && !(check_isa<OMPBarrierDirective>(this->getStmt())
                 || check_isa<OMPTaskwaitDirective>(this->getStmt())
                 || check_isa<OMPTaskDirective>(this->getStmt()))) {

            if (this->getRegion()->isInlinedRegion() == false) {
                if (this->getRegion()->isBalancedOMPFor()) {
                    if (DARTS_BACKEND)
                        outputStream << "TP" << this->getID() << "** TP" << this->getID()
                                     << "Ptr;\n";
                } else {
                    if (DARTS_BACKEND)
                        outputStream << "TP" << this->getID() << "* TP" << this->getID()
                                     << "Ptr;\n";
                }
            }

            if (this->getRegion()->isBalancedOMPFor()) {
                if (DARTS_BACKEND)
                    outputStream << "size_t *TP" << this->getID() << "_alreadyLaunched;\n"
                                 << "int numTPsSet" << this->getID() << ";\n"
                                 << "int numTPsReady" << this->getID() << ";\n"
                                 << "size_t TPsToUse" << this->getID() << ";\n"
                                 << "size_t codeletsPerTP" << this->getID() << ";\n"
                                 << "size_t totalCodelets" << this->getID() << ";\n";
                else if (SWARM_BACKEND)
                    outputStream << "size_t Codelet" << this->getID() << "Entry_alreadySet;\n";
            } else {
                if (DARTS_BACKEND) {
                    outputStream << "size_t TP" << this->getID() << "_alreadyLaunched;\n";
                } else if (SWARM_BACKEND) {
                    outputStream << "size_t Codelet" << this->getID() << "_alreadyLaunched;\n";
                    outputStream << "size_t Codelet" << this->getID() << "Entry_alreadySet;\n";
                }
            }

			/*Mutexes for reduction variable updates*/
            if (this->getLoopInfo() && this->getLoopInfo()->reductionVars.size() > 0) {
                for (auto& myPair : this->getLoopInfo()->reductionVars) {
                    string varName = myPair.first;
                    if (DARTS_BACKEND)
                        outputStream << "std::mutex " << varName << suffixLang << this->getID()
                                     << "_mutex;\n";
                    else if (SWARM_BACKEND)
                        outputStream << "volatile int " << varName << suffixLang << this->getID()
                                     << "_mutex;\n";
                }
            }
        }
    }

	/*Variable to get the ID of the thread executing a single region and perform the copy
	of the copyprivate variable from there*/
    if (this->getCopyPrivateClause()) {
        outputStream << "size_t codeletIDexecutingSingle" << this->getID() << ";\n";
    }

	/*Variables for loops containing omp regions or TP invocations*/
    if (this->isLoopNode() && this->isComplexStmt) {
        outputStream << "unsigned int TP" << this->getID() << "_LoopCounter;\n"
					 << "unsigned int *TP" << this->getID() << "_LoopCounterPerThread;\n"
					 << "tbb::concurrent_vector<TP" << this->getID() << "*> TP" << this->getID()
                     << "PtrVec;\n";
    }

	/*Variables to handle the scheduling and chunks in a parallel loop*/
    if (this->isRegionMainNode && check_isa<ForStmt>(this->getRegion()->getStmt())
        && this->getRegion()->getParent()
        && (check_isa<OMPForDirective>(this->getRegion()->getParent()->getStmt())
               || check_isa<OMPParallelForDirective>(this->getRegion()->getParent()->getStmt()))) {

        LoopInfo* myLoopInfo = this->getRegion()->getParent()->getLoopInfo();
		string loopVarType = myLoopInfo->loopVarType;
		int parentID = this->getRegion()->getParent()->getID();

		outputStream << loopVarType << " initIteration" << parentID << ";\n"
					 << loopVarType << " lastIteration" << parentID << ";\n";

		if (get<0>(myLoopInfo->schedulingPolicy) == RUNTIME_SCHED) {
			outputStream << "size_t ompLoopSched" << parentID << ";\n";
			outputStream << "size_t ompLoopChunk" << parentID << ";\n";
		}

		if (get<0>(myLoopInfo->schedulingPolicy) == STATIC_SCHED
			|| get<0>(myLoopInfo->schedulingPolicy) == RUNTIME_SCHED) {
			outputStream << loopVarType << " range" << parentID << ";\n"
						 << loopVarType << " rangePerCodelet" << parentID << ";\n"
						 << loopVarType << " minIteration" << parentID << ";\n"
						 << loopVarType << " remainderRange" << parentID << ";\n";
		}

		if (get<0>(myLoopInfo->schedulingPolicy) != STATIC_SCHED) {
			outputStream << loopVarType << " nextIteration" << parentID << ";\n"
						 << "int loop" << parentID << "alreadySetUp;\n";
		}

        if (this->getRegion()->getParent()->isBalancedOMPFor()) {
            outputStream << "size_t readyCodelets;\n";

            if (this->getRegion()->getParent()->isOMPForOnlyChildRegion() == false) {
                outputStream << "int baseNumThreads;\n";
                if (this->getRegion()->getParent()->hasBarrier() == false)
                    outputStream << "int *signalNextReady;\n";
            }
        }

        /*Print reduction variables for an omp for */
        if (myLoopInfo->reductionVars.size() > 0) {
            DFGNode* parentNode = this;
            if (!(check_isa<OMPExecutableDirective>(parentNode->getStmt()))) {
                parentNode = this->findOMPParent();
            }

            for (auto& myPair : myLoopInfo->reductionVars) {
                string varName = myPair.first;
                string varType = "";
                auto inputsIt = parentNode->ompInputs.find(varName);
                if (inputsIt != parentNode->ompInputs.end()) {
                    varType = get<0>(inputsIt->second);
                } else {
                    auto varsIt = parentNode->ompVariables.find(varName);
                    if (varsIt != parentNode->ompVariables.end()) {
                        varType = get<0>(varsIt->second);
                    }
                }

                ostringstream newVarName;
                newVarName << varName << suffixLang << parentNode->getID();

                ostringstream localName;
                localName << newVarName.str() << "_reductionVar";

                if (DARTS_BACKEND) {
                    if (parentNode->isOMPParallel())
                        outputStream << "std::mutex " << newVarName.str() << "_mutex;\n";
                    else
                        outputStream << "std::mutex * " << newVarName.str() << "_mutex;\n";
                } else if (SWARM_BACKEND) {
                    outputStream << "volatile int  " << newVarName.str() << "_mutex;\n";
                }
				/*Store the mutex's local name we just defined for future references*/
                get<1>(myPair.second) = varType;
                get<2>(myPair.second) = localName.str();
            }
        }
    }
    this->declareCriticalAtomicMutexes(globalVarsStream);
}

void DFGNode::printTaskDataDefs(std::ostringstream& outputStream)
{
    DFGNode* childNode = this->myNodes.front();
    while (childNode) {
        if (check_isa<OMPTaskDirective>(childNode->getStmt())
            && childNode->getTaskInfo()->encapsulateTaskData) {
            outputStream << "_task" << childNode->getID() << "Inputs** task" << childNode->getID()
                         << "Inputs;\n";
        }
        childNode->printTaskDataDefs(outputStream);
        childNode = childNode->next;
    }
}

void DFGNode::printExtraVarsInit_swarm(std::ostringstream& outputStream)
{
    if (this->getRegion()->getParent() == nullptr ||
		this->getRegion()->getParent()->isSingleThreaded() == false) {

        if (check_isa<OMPExecutableDirective>(this->getStmt())
            && !(check_isa<OMPBarrierDirective>(this->getStmt())
                || check_isa<OMPTaskwaitDirective>(this->getStmt())
                || check_isa<OMPTaskDirective>(this->getStmt()))) {

            if (this->getRegion()->isInlinedRegion() == false) {
                if (this->getRegion()->isBalancedOMPFor()) {
                    if (DARTS_BACKEND)
                        outputStream << ", TP" << this->getID() << "Ptr(new TP" << this->getID()
                                     << " *[NUMTPS" << this->getID() << "])";
                } else {
                    if (DARTS_BACKEND)
                        outputStream << ", TP" << this->getID() << "Ptr(nullptr)";
                }
            }

            if (this->getRegion()->isBalancedOMPFor()) {
                if (DARTS_BACKEND)
                    outputStream << ", TP" << this->getID() << "_alreadyLaunched(new size_t [NUMTPS"
                                 << this->getID() << "])"
                                 << ", numTPsSet" << this->getID() << "(0)"
                                 << ", numTPsReady" << this->getID() << "(0)"
                                 << ", TPsToUse" << this->getID() << "(NUMTPS" << this->getID()
                                 << ")"
                                 << ", codeletsPerTP" << this->getID() << "(this->numThreads/NUMTPS"
                                 << this->getID() << ")"
                                 << ", totalCodelets" << this->getID() << "(this->TPsToUse"
                                 << this->getID() << "*this->codeletsPerTP" << this->getID() << ")";
                else if (SWARM_BACKEND)
                    outputStream << "Codelet" << this->getID() << "Entry_alreadySet"
                                 << " = 0;\n";
            } else {
                if (DARTS_BACKEND) {
                    outputStream << ", TP" << this->getID() << "_alreadyLaunched(0)";
                } else if (SWARM_BACKEND) {
                    outputStream << "Codelet" << this->getID() << "_alreadyLaunched = 0;\n";
                    outputStream << "Codelet" << this->getID() << "Entry_alreadySet = 0;\n";
                    if (this->getRegion()->hasBarrier() && this->getRegion()->getParent())
                        outputStream << "swarm_Dep_init(&(barrierCodelets" << this->getID()
                                     << "Dep), numThreads" << this->getRegion()->getParent()->getID()
                                     << ", "
                                     << "&CODELET(barrierCodelets" << this->getID()
                                     << "), (void*)NULL, (void*)NULL);\n";
                }
            }
        } else if (this->isCallerNode()) {
            if (DARTS_BACKEND) {
                outputStream << ", TP" << this->getID() << "Ptr(nullptr)"
                             << ", TP" << this->getID() << "_alreadyLaunched(0)";
            } else if (SWARM_BACKEND) {
                outputStream << "Codelet" << this->calleeFunctionNodeName
                             << "Entry_alreadySetFrom_Codelet" << this->getID() << " = 0;\n";
            }
        }
    }

    if (this->getCopyPrivateClause()) {
        if (DARTS_BACKEND)
            outputStream << ", codeletIDexecutingSingle" << this->getID()
                         << "(ompNumThreads + 1)\n";
        else if (SWARM_BACKEND)
            outputStream << "codeletIDexecutingSingle" << this->getID()
                         << " = (ompNumThreads + 1);\n";
    }

    if (this->isLoopNode() && this->isComplexStmt) {
        if (DARTS_BACKEND)
            outputStream << ", TP" << this->getID() << "_LoopCounter(0)"
                         << ", TP" << this->getID()
                         << "_LoopCounterPerThread(new unsigned int[this->numThreads])";
    }

    if (this->isRegionMainNode && check_isa<ForStmt>(this->getRegion()->getStmt())
        && this->getRegion()->getParent()
        && (check_isa<OMPForDirective>(this->getRegion()->getParent()->getStmt())
            || check_isa<OMPParallelForDirective>(this->getRegion()->getParent()->getStmt()))) {
				
        LoopInfo* myLoopInfo = this->getRegion()->getParent()->getLoopInfo();
		int parentID = this->getRegion()->getParent()->getID();

        if (DARTS_BACKEND)
            outputStream << ", initIteration" << parentID << "(in_initIteration)"
                         << ", lastIteration" << parentID << "(in_lastIteration)";

        if (get<0>(myLoopInfo->schedulingPolicy) == RUNTIME_SCHED) {
            if (DARTS_BACKEND) {
                outputStream << ", ompLoopSched(1)/*Static by default with runtime scheduling*/"
                             << ", ompLoopChunk(1)/*Chunk =1 by default with runtime scheduling*/";
            }
        }

        if (get<0>(myLoopInfo->schedulingPolicy) != STATIC_SCHED) {
            if (DARTS_BACKEND) {
                outputStream << ", nextIteration" << parentID << "(INT_MAX)"
                             << ", loop" << parentID << "alreadySetUp(0)";
            }
        }

        if (this->getRegion()->getParent()->isBalancedOMPFor()) {
            if (DARTS_BACKEND) {
                outputStream << ", readyCodelets(0)";
            }

            if (this->getRegion()->getParent()->isOMPForOnlyChildRegion() == false) {
                if (DARTS_BACKEND) {
                    outputStream << ", baseNumThreads(this->numThreads / DARTS_CODELETS_MULT)";
                }
                if (this->getRegion()->getParent()->hasBarrier() == false) {
                    if (DARTS_BACKEND) {
                        outputStream << ", signalNextReady(new int[baseNumThreads])";
                    }
                }
            }
        }

        /*Initialize the reduction variables */
        if (myLoopInfo->reductionVars.size() > 0) {
            DFGNode* parentNode = this;
            if (!(check_isa<OMPExecutableDirective>(parentNode->getStmt()))) {
                parentNode = this->findOMPParent();
            }

            for (auto& myPair : myLoopInfo->reductionVars) {
                string varName = myPair.first;
                ostringstream newVarName;
                newVarName << varName << suffixLang << parentNode->getID();

                if (parentNode->isOMPParallel() == 0) {
                    if (DARTS_BACKEND)
                        outputStream << ", " << newVarName.str() << "_mutex (&(this->TPParent->"
                                     << newVarName.str() << "_mutex))";
                }

                if (SWARM_BACKEND)
                    outputStream << newVarName.str() << "_mutex = swarm_spinlock_INITIALIZER;\n";
            }
        }
    }
}

void DFGNode::printTaskDataInits(std::ostringstream& outputStream)
{
    DFGNode* childNode = this->myNodes.front();
    while (childNode) {
        if (check_isa<OMPTaskDirective>(childNode->getStmt())
            && childNode->getTaskInfo()->encapsulateTaskData) {
            outputStream << ", task" << childNode->getID() << "Inputs(new _task"
                         << childNode->getID() << "Inputs*[this->numThreads])";
        }
        childNode->printTaskDataInits(outputStream);
        childNode = childNode->next;
    }
}

void DFGNode::printExtraVarsInit(std::ostringstream& outputStream)
{
    if (this->isCallerNode()) {
        DFGNode* parentNode = this;
        if (!(check_isa<OMPExecutableDirective>(parentNode->getStmt())))
            parentNode = this->findOMPParent();

        if (DARTS_BACKEND) {
            if (this->calleeFunctionNodeIsSingleThreaded == false) {
                outputStream << ", TP" << this->getID() << "Ptr(nullptr)";
                outputStream << ", TP" << this->getID() << "_alreadyLaunched(0)";
/*TODO: Apparently we don't need to initialize the temp var created for a function call with
return value, but keep this code here in case we find that we actually need it.*/
#if 0
                if(this->calleeTmpVarName.compare("") != 0)
					outputStream << ", " << this->calleeTmpVarName << suffixLang
								 << parentNode->getID()
								 << "(new " << this->calleeFunctionNodeType
								 << "[this->numThreads])\n";
#endif
            }
        }
    } else if (this->getRegion()->getParent() == nullptr
        || this->getRegion()->getParent()->isSingleThreaded() == false) {

        if (check_isa<OMPExecutableDirective>(this->getStmt())
            && !(check_isa<OMPBarrierDirective>(this->getStmt())
                 || check_isa<OMPTaskwaitDirective>(this->getStmt())
                 || check_isa<OMPTaskDirective>(this->getStmt()))) {

			int valID = this->getID();
				 
            if (this->getRegion()->isInlinedRegion() == false) {
                if (this->getRegion()->isBalancedOMPFor()) {
                    outputStream << ", TP" << valID << "Ptr(new TP" << valID
                                 << " *[NUMTPS" << valID << "])";
                } else {
                    outputStream << ", TP" << valID << "Ptr(nullptr)";
                }
            }

            if (this->getRegion()->isBalancedOMPFor()) {
                outputStream << ", TP" << valID << "_alreadyLaunched(new size_t [NUMTPS"
                             << valID << "])"
							 << ", numTPsSet" << valID << "(0)"
							 << ", numTPsReady" << valID << "(0)"
							 << ", TPsToUse" << valID << "(NUMTPS" << valID << ")"
							 << ", codeletsPerTP" << valID << "(this->numThreads/NUMTPS"
                             << valID << ")"
							 << ", totalCodelets" << valID << "(this->TPsToUse"
                             << valID << "*this->codeletsPerTP" << valID << ")";
            } else {
                outputStream << ", TP" << valID << "_alreadyLaunched(0)";
            }
        }
    }

    if (this->getCopyPrivateClause()) {
        outputStream << ", codeletIDexecutingSingle" << this->getID() << "(ompNumThreads + 1)\n";
    }

    if (this->isLoopNode() && this->isComplexStmt) {
        outputStream << ", TP" << this->getID() << "_LoopCounter(0)"
					 << ", TP" << this->getID()
                     << "_LoopCounterPerThread(new unsigned int[this->numThreads])";
    }

    if (this->isRegionMainNode && check_isa<ForStmt>(this->myRegion->getStmt())
        && this->myRegion->parent
        && (check_isa<OMPForDirective>(this->myRegion->parent->getStmt())
               || check_isa<OMPParallelForDirective>(this->myRegion->parent->getStmt()))) {

        LoopInfo* myLoopInfo = this->myRegion->parent->getLoopInfo();
		int parentID = this->myRegion->parent->getID();

        if (this->getRegion()->getParent()->isInlinedRegion() == false) {
            outputStream << ", initIteration" << parentID << "(in_initIteration)"
						 << ", lastIteration" << parentID << "(in_lastIteration)";

            if (get<0>(myLoopInfo->schedulingPolicy) == RUNTIME_SCHED) {
                outputStream << ", ompLoopSched" << parentID
							 << "(1)/*Static by default with runtime scheduling*/"
							 << ", ompLoopChunk" << parentID 
							 << "(1)/*Chunk =1 by default with runtime scheduling*/";
            }

            if (get<0>(myLoopInfo->schedulingPolicy) != STATIC_SCHED) {
                outputStream << ", nextIteration" << parentID << "(INT_MAX)"
							 << ", loop" << parentID << "alreadySetUp(0)";
            }
        } else {
            outputStream << ", initIteration" << parentID << "(-1)"
						 << ", lastIteration" << parentID << "(-1)";

            if (get<0>(myLoopInfo->schedulingPolicy) == RUNTIME_SCHED) {
                outputStream << ", ompLoopSched" << parentID
							 << "(1)/*Static by default with runtime scheduling*/"
							 << ", ompLoopChunk" << parentID 
							 << "(1)/*Chunk =1 by default with runtime scheduling*/";
            }

            if (get<0>(myLoopInfo->schedulingPolicy) != STATIC_SCHED) {
                outputStream << ", nextIteration" << parentID << "(INT_MAX * (-1))"
							 << ", loop" << parentID << "alreadySetUp(0)";
            }
        }

        if (this->getRegion()->getParent()->isBalancedOMPFor()) {
            outputStream << ", readyCodelets(0)";

            if (this->myRegion->parent->isOMPForOnlyChildRegion() == false) {
                outputStream << ", baseNumThreads(this->numThreads / DARTS_CODELETS_MULT)";
                if (this->myRegion->parent->hasBarrier() == false)
                    outputStream << ", signalNextReady(new int[baseNumThreads])";
            }
        }

        /*Initialize the reduction variables */
        if (myLoopInfo->reductionVars.size() > 0) {
            DFGNode* parentNode = this;
            if (!(check_isa<OMPExecutableDirective>(parentNode->getStmt()))) {
                parentNode = this->findOMPParent();
            }

            for (auto& myPair : myLoopInfo->reductionVars) {
                string varName = myPair.first;
                ostringstream newVarName;
                newVarName << varName << "_darts" << parentNode->getID();

                if (parentNode->isOMPParallel() == 0)
                    outputStream << ", " << newVarName.str() << "_mutex (&(this->TPParent->"
                                 << newVarName.str() << "_mutex))";
            }
        }
    }
}

void DFGNode::printExtraVarsInitInConstructor(std::ostringstream& outputStream)
{
    if (this->isLoopNode() && this->isComplexStmt)
        outputStream << "memset((void*)TP" << this->getID()
                     << "_LoopCounterPerThread, 0, this->numThreads*sizeof(unsigned int));\n";
}

void DFGNode::printExtraVarsFree(std::ostringstream& outputStream)
{
    if (this->isLoopNode() && this->isComplexStmt)
        outputStream << "delete [] TP" << this->getID() << "_LoopCounterPerThread;";
}

void DFGNode::printTaskDataFree(std::ostringstream& outputStream)
{
    DFGNode* childNode = this->myNodes.front();
    while (childNode) {
        if (check_isa<OMPTaskDirective>(childNode->getStmt())
            && childNode->getTaskInfo()->encapsulateTaskData) {
            outputStream << "delete [] task" << childNode->getID() << "Inputs;";
        }
        childNode->printTaskDataFree(outputStream);
        childNode = childNode->next;
    }
}

void DFGNode::printReductionVarPtrDefs(std::ostringstream& outputStream, TPRegion* parentRegion)
{
    if (parentRegion->getLoopInfo()->reductionVars.size() > 0)
        outputStream << "/*Declare local reduction vars*/\n";

    for (auto& it : parentRegion->getLoopInfo()->reductionVars) {
        string varName = it.first;
        string varType = get<1>(it.second);

        string ptrName = varName;
        ptrName += "_LocalReductionVar";
        get<3>(it.second) = ptrName;

        /*Initialize the reduction variable local copy according to the OpenMP 3.1 standard, pp 103
         */
        string operatorStr = get<0>(it.second);
        string initValue
            = parentRegion->getLoopInfo()->getReductionVarInitVal(operatorStr, varType);
        outputStream << varType << " " << ptrName << " = "
                     << "(" << varType << ")" << initValue << ";\n";
    }
}

void DFGNode::printParallelReductionVarPtrDefs(
    std::ostringstream& outputStream, TPRegion* parentRegion)
{
    for (auto& it : parentRegion->mainNode->parallelReductionVars) {
        /* Dont declare threadprivate vars */

        VarTableTuple_t varTuple;
        bool found = this->myDFG->getVarTupleFromTable(this, it.first, varTuple);
        if (found == false)
            continue;
        if (get<4>(varTuple))
            continue;

        string varType = get<0>(varTuple);
        string varArray = get<6>(varTuple);

        string varName = it.first;
        string ptrName = varName;
        ptrName += "_LocalReductionVar";
        get<3>(it.second) = ptrName;

        string operatorStr = get<0>(it.second);
        string initValue = parentRegion->mainNode->getParallelReductionVarInitVal(operatorStr);
        outputStream << varType << " " << ptrName << varArray << " = "
                     << "(" << varType << ")" << initValue << ";";
    }
}

void DFGNode::replaceReductionVarInFireCode(
    std::ostringstream& changedStream, TPRegion* parentRegion)
{
    std::string codeStr = changedStream.str();
    for (ReductionVarPair_t it : parentRegion->getLoopInfo()->reductionVars) {
        string varName = it.first;
        string varType = get<1>(it.second);
        string localName = get<2>(it.second);
        string ptrName = "&(";
        ptrName += get<3>(it.second);
        ptrName += ")";

        /*We are gonna look for the full variable's name to replace with the local we just created
         */
        ostringstream fullVarName;

        if (DARTS_BACKEND)
            fullVarName << DARTS_PREFIXSTR_DEFAULT << varName << suffixLang
                        << parentRegion->getID();
        else if (SWARM_BACKEND)
            fullVarName << varName << suffixLang << parentRegion->getID();

        size_t newPos = 0;
        do {
            newPos = codeStr.find(fullVarName.str(), newPos);
            if (newPos != string::npos) {
                codeStr.replace(newPos, fullVarName.str().length(), ptrName);
                newPos = newPos + ptrName.length();
            }
        } while ((newPos != string::npos) && (newPos < codeStr.length()));
    }
    changedStream.str("");
    changedStream.clear();
    changedStream << codeStr;
}

void DFGNode::replaceParallelReductionVarInFireCode(std::string& codeStr, TPRegion* parentRegion)
{
    for (ReductionVarPair_t it : parentRegion->mainNode->parallelReductionVars) {
        string varName = it.first;
        string varType = get<1>(it.second);
        string localName = get<2>(it.second);
        string ptrName = "&(";
        ptrName += get<3>(it.second);
        ptrName += ")";

        /*We are gonna look for the full variable's name to replace with the local we just created
         */
        ostringstream fullVarName;

        if (DARTS_BACKEND)
            fullVarName << DARTS_PREFIXSTR_DEFAULT << varName << suffixLang
                        << parentRegion->getID();
        else if (SWARM_BACKEND)
            fullVarName << varName << suffixLang << parentRegion->getID();

        size_t newPos = 0;
        do {
            newPos = codeStr.find(fullVarName.str(), newPos);
            if (newPos != string::npos) {
                codeStr.replace(newPos, fullVarName.str().length(), ptrName);
                newPos = newPos + ptrName.length();
            }
        } while ((newPos != string::npos) && (newPos < codeStr.length()));
    }
}

void DFGNode::updateLocalReductionVars(std::ostringstream& outputStream, TPRegion* parentRegion)
{
    outputStream << "/*Update the TP's local copy of each reduction variable*/\n";
    for (ReductionVarPair_t it : parentRegion->getLoopInfo()->reductionVars) {
        string varName = it.first;
        string varType = get<1>(it.second);
        string localName = get<2>(it.second);
        string ptrName = get<3>(it.second);

        ostringstream fullVarName;
        fullVarName << DARTS_PREFIXSTR_DEFAULT << varName << suffixLang << parentRegion->getID();
        ostringstream fullLocalName;
        fullLocalName << DARTS_PREFIXSTR_DEFAULT << localName;

        string operatorStr = get<0>(it.second);

        outputStream << fullLocalName.str() << "_mutex.lock();\n"
                     << "{\n"
                     << fullLocalName.str() << "[myTP->numThreads] " << operatorStr << " = *("
                     << ptrName << ");\n"
                     << "}\n"
                     << fullLocalName.str() << "_mutex.unlock();\n";
    }
}

void DFGNode::updateLocalParallelReductionVars(
    std::ostringstream& outputStream, TPRegion* parentRegion)
{
    outputStream << "/*Update the TP's local copy of each reduction variable*/\n";
    for (ReductionVarPair_t it : parentRegion->mainNode->parallelReductionVars) {
        string varName = it.first;
        string varType = get<1>(it.second);
        string localName = get<2>(it.second);
        string ptrName = get<3>(it.second);

        ostringstream fullVarName;
        fullVarName << DARTS_PREFIXSTR_DEFAULT << varName << "_darts" << parentRegion->getID();
        ostringstream fullLocalName;
        fullLocalName << DARTS_PREFIXSTR_DEFAULT << localName;

        string operatorStr = get<0>(it.second);

        outputStream << fullLocalName.str() << "_mutex.lock();\n"
                     << "{\n"
                     << fullLocalName.str() << "[myTP->numThreads] " << operatorStr << " = *("
                     << ptrName << ");\n"
                     << "}\n"
                     << fullLocalName.str() << "_mutex.unlock();\n";
    }
}

void DFGNode::updateGlobalReductionVars(std::ostringstream& outputStream, TPRegion* parentRegion)
{
    outputStream << "updateReduceVars:\n"
                 << "/*Before signaling next codelet, check if all the threads in the TP\n"
                 << "* performed the reduction locally and if so, update the unique \n"
                 << "* reduction variable.*/\n";

    DFGNode* parentNode = this;
    if (!(check_isa<OMPExecutableDirective>(parentNode->getStmt())))
        parentNode = this->findOMPParent();

    for (ReductionVarPair_t it : parentRegion->getLoopInfo()->reductionVars) {
        string varName = it.first;
        string varType = get<1>(it.second);
        string localName = get<2>(it.second);
        string ptrName = get<3>(it.second);

        ostringstream fullVarName;

        if (DARTS_BACKEND)
            fullVarName << DARTS_PREFIXSTR_DEFAULT << varName << suffixLang
                        << parentRegion->getID();
        else if (SWARM_BACKEND)
            fullVarName << varName << suffixLang << parentRegion->getID();

        string operatorStr = get<0>(it.second);

        outputStream << "/*Reducing for operator '" << operatorStr << "'*/\n";

        if (DARTS_BACKEND) {
            if (parentNode->isOMPParallel())
                outputStream << fullVarName.str() << "_mutex.lock();\n";
            else
                outputStream << fullVarName.str() << "_mutex->lock();\n";
        } else if (SWARM_BACKEND) {
            outputStream << "swarm_spinlock_acquire(" << fullVarName.str() << "_mutex);\n";
        }

        /*Reduce for each type of operation */
        if (strcmp(operatorStr.c_str(), "+") == 0) {
            outputStream << "*( " << fullVarName.str() << ") += (" << ptrName << ");\n";
        } else if (strcmp(operatorStr.c_str(), "-") == 0) {
            outputStream << "*( " << fullVarName.str() << ") += (" << ptrName << ");\n";
        } else if (strcmp(operatorStr.c_str(), "*") == 0) {
            outputStream << "*( " << fullVarName.str() << ") *= (" << ptrName << ");\n";
        } else if (strcmp(operatorStr.c_str(), "&") == 0) {
            outputStream << "*( " << fullVarName.str() << ") &= (" << ptrName << ");\n";
        } else if (strcmp(operatorStr.c_str(), "|") == 0) {
            outputStream << "*( " << fullVarName.str() << ") |= (" << ptrName << ");\n";
        } else if (strcmp(operatorStr.c_str(), "^") == 0) {
            outputStream << "*( " << fullVarName.str() << ") ^= (" << ptrName << ");\n";
        } else if (strcmp(operatorStr.c_str(), "&&") == 0) {
            outputStream << "*( " << fullVarName.str() << ") = (" << ptrName << ") && ( "
                         << fullVarName.str() << ");\n";
        } else if (strcmp(operatorStr.c_str(), "||") == 0) {
            outputStream << "*( " << fullVarName.str() << ") = (" << ptrName << ") || ( "
                         << fullVarName.str() << ");\n";
        } else if (strcmp(operatorStr.c_str(), "max") == 0) {
            outputStream << "if (*( " << fullVarName.str() << ") < (" << ptrName << "))\n"
                         << "*( " << fullVarName.str() << ") = (" << ptrName << ");\n";
        } else if (strcmp(operatorStr.c_str(), "min") == 0) {
            outputStream << "if (*( " << fullVarName.str() << ") > (" << ptrName << "))\n"
                         << "*( " << fullVarName.str() << ") = (" << ptrName << ");\n";
        }

        if (DARTS_BACKEND) {
            if (parentNode->isOMPParallel())
                outputStream << fullVarName.str() << "_mutex.unlock();\n";
            else
                outputStream << fullVarName.str() << "_mutex->unlock();\n";
        } else if (SWARM_BACKEND) {
            outputStream << "swarm_spinlock_release(" << fullVarName.str() << "_mutex);\n";
        }
    }
}

void DFGNode::updateGlobalParallelReductionVars(
    std::ostringstream& outputStream, TPRegion* parentRegion)
{
    outputStream << "updateReduceVars:\n"
                 << "/*Before signaling next codelet, check if all the threads in the TP\n"
                 << "* performed the reduction locally and if so, update the unique \n"
                 << "* reduction variable.*/\n";

    DFGNode* parentNode = this;
    if (!(check_isa<OMPExecutableDirective>(parentNode->getStmt())))
        parentNode = this->findOMPParent();

    for (ReductionVarPair_t it : parentRegion->mainNode->parallelReductionVars) {
        string varName = it.first;
        string varType = get<1>(it.second);
        string localName = get<2>(it.second);
        string ptrName = get<3>(it.second);

        ostringstream fullVarName;
        fullVarName << DARTS_PREFIXSTR_DEFAULT << varName << "_darts" << parentRegion->getID();

        string operatorStr = get<0>(it.second);

        if (parentNode->isOMPParallel())
            outputStream << fullVarName.str() << "_mutex.lock();\n";
        else
            outputStream << fullVarName.str() << "_mutex->lock();\n";

        /*Reduce for each type of operation */
        if (strcmp(operatorStr.c_str(), "+") == 0) {
            outputStream << "*( " << fullVarName.str() << ") += (" << ptrName << ");\n";
        } else if (strcmp(operatorStr.c_str(), "-") == 0) {
            outputStream << "*( " << fullVarName.str() << ") += (" << ptrName << ");\n";
        } else if (strcmp(operatorStr.c_str(), "*") == 0) {
            outputStream << "*( " << fullVarName.str() << ") *= (" << ptrName << ");\n";
        } else if (strcmp(operatorStr.c_str(), "&") == 0) {
            outputStream << "*( " << fullVarName.str() << ") &= (" << ptrName << ");\n";
        } else if (strcmp(operatorStr.c_str(), "|") == 0) {
            outputStream << "*( " << fullVarName.str() << ") |= (" << ptrName << ");\n";
        } else if (strcmp(operatorStr.c_str(), "^") == 0) {
            outputStream << "*( " << fullVarName.str() << ") ^= (" << ptrName << ");\n";
        } else if (strcmp(operatorStr.c_str(), "&&") == 0) {
            outputStream << "*( " << fullVarName.str() << ") = (" << ptrName << ") && ( "
                         << fullVarName.str() << ");\n";
        } else if (strcmp(operatorStr.c_str(), "||") == 0) {
            outputStream << "*( " << fullVarName.str() << ") = (" << ptrName << ") || ( "
                         << fullVarName.str() << ");\n";
        } else if (strcmp(operatorStr.c_str(), "max") == 0) {
            outputStream << "if (*( " << fullVarName.str() << ") < (" << ptrName << "))\n"
                         << "*( " << fullVarName.str() << ") = (" << ptrName << ");\n";
        } else if (strcmp(operatorStr.c_str(), "min") == 0) {
            outputStream << "if (*( " << fullVarName.str() << ") > (" << ptrName << "))\n"
                         << "*( " << fullVarName.str() << ") = (" << ptrName << ");\n";
        }

        if (parentNode->isOMPParallel())
            outputStream << fullVarName.str() << "_mutex.unlock();\n";
        else
            outputStream << fullVarName.str() << "_mutex->unlock();\n";
    }
}

std::string DFGNode::getParallelReductionVarInitVal(string opCode)
{
    /*Reduction variable local copy according to the OpenMP 3.1 standard, pp 103 */
    string initValue = "";

    if (strcmp(opCode.c_str(), "+") == 0)
        initValue = "0";
    else if (strcmp(opCode.c_str(), "*") == 0)
        initValue = "1";
    else if (strcmp(opCode.c_str(), "&") == 0)
        initValue = "~0";
    else if (strcmp(opCode.c_str(), "|") == 0)
        initValue = "0";
    else if (strcmp(opCode.c_str(), "^") == 0)
        initValue = "0";
    else if (strcmp(opCode.c_str(), "&&") == 0)
        initValue = "1";
    else if (strcmp(opCode.c_str(), "||") == 0)
        initValue = "0";
    else if (strcmp(opCode.c_str(), "max") == 0)
        initValue = "(std::numeric_limits<double>::infinity() * (-1))";
    else if (strcmp(opCode.c_str(), "min") == 0)
        initValue = "(std::numeric_limits<double>::infinity())";

    return initValue;
}

void DFGNode::findTPParent(std::ostringstream& outputStream, std::string& TPParentStr,
    TPRegion* currentRegion, TPRegion* regionToSignal)
{
    /*Find the TP to which the nextRegion belongs*/
#if FULL_DEBUG_COMMENTS == 1
    outputStream << "/*Find the TP to which the nextRegion belongs*/\n";
#endif

    TPRegion* currentParent = currentRegion;
    while (currentParent) {
        if (currentParent->isThreadedProcedure) {
            if (currentParent->getLoopInfo() && currentParent->getMainNode()->isComplexStmt)
                break;
            if (check_isa<OMPExecutableDirective>(currentParent->getStmt()))
                break;
            if (currentParent->getFunctionInfo())
                break;
        }
        currentParent = currentParent->getParent();
    }
    if (currentParent == nullptr)
        currentParent = currentRegion;

    TPRegion* parentToSignal = regionToSignal->getParent();
    while (parentToSignal) {
        if (parentToSignal->isThreadedProcedure) {
            if (currentParent->getLoopInfo() && currentParent->getMainNode()->isComplexStmt)
                break;
            if (check_isa<OMPExecutableDirective>(parentToSignal->getStmt()))
                break;
            if (parentToSignal->getFunctionInfo())
                break;
        }
        parentToSignal = parentToSignal->getParent();
    }
    if (parentToSignal == nullptr)
        parentToSignal = regionToSignal;

#if FULL_DEBUG_COMMENTS == 1
    outputStream << "/*currentRegion: " << currentRegion->getName() << " " << currentRegion->getID()
                 << ", ";
    outputStream << "currentParent: " << currentParent->getName() << " " << currentParent->getID()
                 << "*/\n";
    outputStream << "/*regionToSignal: " << regionToSignal->getName() << " "
                 << regionToSignal->getID() << ", ";
    outputStream << "parentToSignal: " << parentToSignal->getName() << " "
                 << parentToSignal->getID() << "*/\n";
#endif

    while (currentParent) {
#if FULL_DEBUG_COMMENTS == 1
        outputStream << "/*currentParent: " << currentParent->getName() << " "
                     << currentParent->getID() << "*/\n";
#endif
        if (currentParent->getID() == parentToSignal->getID()) {
            break;
        } else {
            if (currentParent->isThreadedProcedure) {
                TPParentStr += "TPParent";
#if FULL_DEBUG_COMMENTS == 1
                TPParentStr += "/*Parent of ";
                TPParentStr += to_string(currentParent->getID());
                TPParentStr += "*/";
#endif
				TPParentStr += "->";
            }
        }
        currentParent = currentParent->getParent();
    }
}

std::string DFGNode::signalTaskSyncPoint(
    std::ostringstream& outputStream, std::string prefixStr, std::string postfixStr)
{
    /*Signal the task's synchronization point*/

    if (SWARM_BACKEND) {
        prefixStr = "";
        postfixStr = "";
    }

    ostringstream nextCodeletName;

    TPRegion* parentRegion = this->getRegion()->parent;
    if (parentRegion == nullptr)
        parentRegion = this->getRegion();

#if FULL_DEBUG_COMMENTS == 1
	DFGNode* parentNode = this->findParentNode();
    outputStream << "/*signalTaskSyncPoint: node: " << this->getName() << " " << this->getID()
                 << ", "
				 << "parentNode: " << parentNode->getName() << " " << parentNode->getID() << ", "
				 << "region: " << this->getRegion()->getName() << " " << this->getRegion()->getID()
                 << ", "
				 << "parentRegion: " << parentRegion->getName() << " " << parentRegion->getID()
                 << "*/\n";
#endif

    if (check_isa<OMPTaskDirective>(this->getRegion()->getStmt())) {
        ostringstream tempStream;
        TaskInfo* taskInfo = this->getRegion()->getTaskInfo();
        DFGNode* syncNode = taskInfo->mySyncPoint;
        if (syncNode) {
            outputStream << "/*syncNode: " << syncNode->getName() << " " << syncNode->getID()
                         << "*/\n";
            string TPParentStr = "";
            findTPParent(
                outputStream, TPParentStr, this->getRegion()->getParent(), syncNode->getRegion());
            nextCodeletName << prefixStr << TPParentStr << "barrierCodelets" << syncNode->getID()
                            << "[0]";
        } else {
            DFGNode* functionParentNode = this->findFunctionParent();
            if (functionParentNode) {
                outputStream << "/*No syncnode, using the function's ("
                             << functionParentNode->getName() << ") sync codelet*/\n";
                string TPParentStr = "";
                findTPParent(outputStream, TPParentStr, this->getRegion()->getParent(),
                    functionParentNode->getRegion());
                if (functionParentNode->getRegion()->singleThreaded)
                    nextCodeletName << prefixStr << TPParentStr << "nextSyncCodelets"
                                    << functionParentNode->getName();
                else
                    nextCodeletName << prefixStr << TPParentStr << "nextSyncCodelets"
                                    << functionParentNode->getName() << "[" << postfixStr << "]";
            }
        }

        if (nextCodeletName.str().empty())
            outputStream << "/*No codelet found*/\n";
    }
    return nextCodeletName.str();
}

bool DFGNode::checkIfNodeIsChild(DFGNode* targetNode)
{
	/*Check if this node is a targetnode's descendent*/
    bool res = false;
    DFGNode* parentNode = this;
    DFGNode* childNode = parentNode->myNodes.front();
    while (childNode && res == false) {
        if (childNode->getID() == targetNode->getID()) {
            res = true;
        } else {
            res = childNode->checkIfNodeIsChild(targetNode);
        }
        childNode = childNode->next;
    }
    return res;
}

std::string DFGNode::signalNext(std::ostringstream& outputStream, std::string prefixStr,
    std::string postfixStr, bool signalFromBarrier, bool signalFromCallExpr, bool getTaskSyncPoint)
{
    if (SWARM_BACKEND) {
        prefixStr = "";
        postfixStr = "";
    }

    ostringstream nextCodeletName;

    TPRegion* parentRegion = this->getRegion()->parent;
    if (parentRegion == nullptr)
        parentRegion = this->getRegion();

    DFGNode* parentNode = this->findParentNode();

    DFGNode* taskOmpParentNode = parentNode;
    if (check_isa<OMPTaskDirective>(taskOmpParentNode->getStmt()))
        taskOmpParentNode = parentNode->findParentNode();

#if FULL_DEBUG_COMMENTS == 1
    outputStream << "/*signalNext: node: " << this->getName() << " " << this->getID() << ", ";
    outputStream << "parentNode: " << parentNode->getName() << " " << parentNode->getID() << ", ";
    outputStream << "region: " << this->getRegion()->getName() << " " << this->getRegion()->getID()
                 << ", ";
    outputStream << "parentRegion: " << parentRegion->getName() << " " << parentRegion->getID()
                 << "*/\n";
#endif

    /*If this is a for stmt and the parent is a balanced OMP For, just signal
     * the barrier on this region and that barrier will signal the next region*/
    if (check_isa<ForStmt>(this->getStmt()) && parentRegion->isBalancedOMPFor()
        && parentRegion->hasBarrier()) {

        outputStream << "/*Signaling omp for stmt's barrier*/\n";
        if (DARTS_BACKEND) {
            nextCodeletName << prefixStr << "barrierCodelets" << parentRegion->getID()
                            << "[0].decDep();\n";
            outputStream << nextCodeletName.str();
        } else if (SWARM_BACKEND) {
            nextCodeletName << "barrierCodelets" << parentRegion->getID();
            outputStream << "swarm_Dep_satisfy(&" << nextCodeletName.str() << "Dep, 1);\n";
        }
        return nextCodeletName.str();
    }

    /*The current region is an omp region with barrier.*/
    if (check_isa<OMPExecutableDirective>(this->getStmt()) && !this->noWait
        && !check_isa<OMPTaskwaitDirective>(this->getStmt())) {

        outputStream << "/*Signaling omp region's barrier*/\n";

        outputStream << prefixStr;
        if (this->getRegion()->isInlinedRegion() == false)
            outputStream << "TPParent->";

        if (DARTS_BACKEND) {
            outputStream << "barrierCodelets" << this->getID() << "[0].decDep();\n";
            nextCodeletName << "barrierCodelets" << this->getID() << "[0]";
        } else if (SWARM_BACKEND) {
            nextCodeletName << "barrierCodelets" << this->getID();
            outputStream << "swarm_Dep_satisfy(&barrierCodelets" << this->getID() << "Dep, 1);\n";
        }
    }

	/*Find the next region but we are not doing it from a task region*/
    else if (this->getRegion()->nextRegion && getTaskSyncPoint == false) {

        /*Just signal the next region.*/
        TPRegion* nextRegion = this->getRegion()->nextRegion;
        int idToSignal = nextRegion->mainNode->getID();

        outputStream << "/*Signaling next codelet region: " << this->getRegion()->getID()
                     << " nextRegion: " << nextRegion->getID() << " */\n";

        string indexID = postfixStr;

        if (check_isa<OMPBarrierDirective>(nextRegion->getStmt())
            || check_isa<OMPTaskwaitDirective>(nextRegion->getStmt())) {

            if (DARTS_BACKEND)
                nextCodeletName << prefixStr << "barrierCodelets" << idToSignal << "[0]";
            else if (SWARM_BACKEND)
                nextCodeletName << prefixStr << "barrierCodelets" << idToSignal;

        }
        else if (nextRegion->parent && nextRegion->parent->singleThreaded) {

            nextCodeletName << prefixStr << "checkInCodelets" << idToSignal;

        } else {

            if (DARTS_BACKEND)
                nextCodeletName << prefixStr << "checkInCodelets" << idToSignal << "[" << indexID
                                << "]";
            else if (SWARM_BACKEND)
                nextCodeletName << prefixStr << "checkInCodelets" << idToSignal;
        }

        if (DARTS_BACKEND) {
            outputStream << nextCodeletName.str() << ".decDep();\n";
        } else if (SWARM_BACKEND) {
            if (nextCodeletName.str().find("barrierCodelets") == string::npos) {
                outputStream << "swarm_schedule(&CODELET(" << nextCodeletName.str() << "), "
                             << "(void*)codeletID, NULL, NULL, NULL);\n";
            } else {
                outputStream << "swarm_Dep_satisfyOnce(&" << nextCodeletName.str() << "Dep);\n";
            }
        }
    }

	/*Find the next region but we are doing it from a task region but we don't need the syncpoint*/
    else if (check_isa<OMPTaskDirective>(parentNode->getStmt()) && signalFromCallExpr == false
        && getTaskSyncPoint == false && parentRegion->getID() == parentNode->getID()) {
        outputStream << "/*Mark this task as completed*/\n"
					 << prefixStr << "task" << parentNode->getID()
                     << "Inputs->taskCompleted = true;\n"
					 << "/*Signal the task's synchronization point*/\n"
					 << prefixStr << "task" << parentNode->getID()
                     << "Inputs->nextSyncCodelet->decDep();\n";
    }
    /*This is for when we need to pass a sync point to a function.*/
    else if (signalFromCallExpr && getTaskSyncPoint) {
#if FULL_DEBUG_COMMENTS == 1
        outputStream << "/*Find the sync point for a function call*/\n";
#endif
        bool withinTaskBoundary = true;
        DFGNode* syncNode = this->findMySyncPoint();
        if (syncNode) {
            DFGNode* funcOrTaskParentNode = this;
            while (funcOrTaskParentNode) {
                if (check_isa<OMPTaskDirective>(funcOrTaskParentNode->getStmt())) {
                    /*Make sure the syncnode is within the current task*/
                    withinTaskBoundary = funcOrTaskParentNode->checkIfNodeIsChild(syncNode);
                }
                if (funcOrTaskParentNode->getFunctionInfo()) {
                    break;
                }
                funcOrTaskParentNode = funcOrTaskParentNode->getParent();
            }
#if FULL_DEBUG_COMMENTS == 1
            outputStream << "/*syncNode: " << syncNode->getName() << " " << syncNode->getID()
                         << "*/\n";
            outputStream << "/*withinTaskBoundary: " << withinTaskBoundary << "*/\n";
#endif
        }
        if (syncNode && withinTaskBoundary) {
            string TPParentStr = "";
            findTPParent(
                outputStream, TPParentStr, this->getRegion()->getParent(), syncNode->getRegion());
            nextCodeletName << prefixStr << TPParentStr << "barrierCodelets" << syncNode->getID()
                            << "[0]";

        } else {
            outputStream
                << "/*No syncpoint found or outside task's boundaries, looking for syncCodelet*/\n";
            DFGNode* funcOrTaskParentNode = this;
            while (funcOrTaskParentNode) {
                if (check_isa<OMPTaskDirective>(funcOrTaskParentNode->getStmt())) {
                    string TPParentStr = "";
                    nextCodeletName << prefixStr << TPParentStr << "task"
                                    << funcOrTaskParentNode->getID() << "Inputs->nextSyncCodelet";
                    break;
                }
                if (funcOrTaskParentNode->getFunctionInfo()) {
                    string TPParentStr = "";
                    if (funcOrTaskParentNode->getRegion()->singleThreaded)
                        nextCodeletName << prefixStr << TPParentStr << "nextSyncCodelets"
                                        << funcOrTaskParentNode->getName();
                    else
                        nextCodeletName << prefixStr << TPParentStr << "nextSyncCodelets"
                                        << funcOrTaskParentNode->getName() << "[" << postfixStr
                                        << "]";

                    break;
                }
                funcOrTaskParentNode = funcOrTaskParentNode->getParent();
            }
            if (nextCodeletName.str().empty())
                outputStream << "/*Error: Not able to find node with parent task or function "
                             << "for the syncodelet*/\n";
        }
    }
    /*The node is the last one in a complex loop, so signal the inc node*/
    else if (parentRegion->getLoopInfo() && parentRegion->getMainNode()->isComplexStmt) {
        outputStream << "/*The node is the last one in a complex loop, so signal the inc node*/\n";
        int idToSignal = parentRegion->getLoopInfo()->incNode->getID();

        /*Find the TP to which the nextRegion belongs*/
        string TPParentStr = "";
        findTPParent(outputStream, TPParentStr, parentRegion,
            parentRegion->getLoopInfo()->incNode->getRegion());

        if (parentRegion->singleThreaded)
            nextCodeletName << prefixStr << TPParentStr << "checkInCodelets" << idToSignal;
        else
            nextCodeletName << prefixStr << TPParentStr << "checkInCodelets" << idToSignal << "["
                            << postfixStr << "]";
        outputStream << nextCodeletName.str() << ".decDep();\n";
    }

    else {
        /*Find which region we need to signal.*/
        outputStream << "/*Find and signal the next codelet*/\n";
        nextCodeletName << this->signalNextCodelet(parentRegion, outputStream, prefixStr);

        if (this->myDFG->useFunctionStack) {
            if (outputStream.str().find("nextCodelets") != string::npos) {
                outputStream << "/*Remove pointer to current function from thread's stack*/\n";
                outputStream << "popFunctionStack(this->getLocalID());\n";
            }
        }
    }
    return nextCodeletName.str();
}

std::string DFGNode::signalNextCodelet(TPRegion* parentRegion, std::ostringstream& outputStream,
    std::string prefixStr, std::string postfixStr, bool signalFromBarrier)
{
    if (SWARM_BACKEND) {
        prefixStr = "";
        postfixStr = "";
    }

    ostringstream tempStream;

    ostringstream nextCodeletName;

    ostringstream signalStream;
    signalStream << prefixStr;

    if (parentRegion->getMainNode()->myFunctionInfo && parentRegion->getName().compare("main") != 0) {

        tempStream << "\n";

        if (signalFromBarrier && (this->getRegion()->parent == nullptr
                                     || this->getRegion()->parent->singleThreaded == false)) {

            if (DARTS_BACKEND) {
                tempStream << "for(size_t codeletsCounter=0; "
                           << "codeletsCounter < (size_t)myTP->numThreads; "
                           << "codeletsCounter++)\n"
                           << "{\n"
                           << prefixStr << "nextCodelets" << parentRegion->getName()
                           << "[codeletsCounter]->decDep();\n"
                           << "}\n";
            } else if (SWARM_BACKEND) {

                DFGNode* parentNode = this;
                while (parentNode && parentNode->isOMPParallel() == false) {
                    parentNode = parentNode->parent;
                }

                tempStream << "for(size_t codeletsCounter=0; ";

                if (parentNode)
                    tempStream << "codeletsCounter < (size_t)numThreads" << parentNode->getID()
                               << "; ";
                else
                    tempStream << "codeletsCounter < (size_t)ompNumThreads; ";

                tempStream << "codeletsCounter++)\n"
                           << "{\n";

                tempStream << "if(nextCodeletContext" << parentRegion->getName()
                           << "[codeletsCounter] == NULL)\n"
                           << "swarm_dispatch("
                           << "nextCodelets" << parentRegion->getName() << "[codeletsCounter], "
                           << "nextCodeletContext" << parentRegion->getName()
                           << "[codeletsCounter], "
                           << "(void*)NULL, (void*)NULL, (void*)NULL);\n"
                           << "else\n"
                           << "swarm_Dep_satisfy(((swarm_Dep_t *)nextCodeletContext"
                           << parentRegion->getName() << "[codeletsCounter]), 1);\n";

                tempStream << "}\n";
            }

        } else if (this->getRegion()->parent && this->getRegion()->parent->singleThreaded) {
            nextCodeletName << prefixStr << "nextCodelets" << parentRegion->getName() << "";
            tempStream << nextCodeletName.str() << "->decDep();\n";
        } else {
            if (DARTS_BACKEND) {
                nextCodeletName << prefixStr << "nextCodelets" << parentRegion->getName() << "["
                                << postfixStr << "]";
                tempStream << nextCodeletName.str() << "->decDep();\n";
            } else if (SWARM_BACKEND) {

                tempStream << "/*Signal the codelet after this function.*/\n";

                tempStream << "if(nextCodeletContext" << parentRegion->getName()
                           << "[codeletID] == NULL)\n"
                           << "swarm_dispatch("
                           << "nextCodelets" << parentRegion->getName() << "[codeletID], "
                           << "(void *)codeletID, (void*)NULL, (void*)NULL, (void*)NULL);\n"
                           << "else\n"
                           << "swarm_Dep_satisfy(((swarm_Dep_t *)nextCodeletContext"
                           << parentRegion->getName() << "[codeletID]), 1);\n";
            }
        }
    } else {
        bool found = false;
        while (!found && parentRegion) {
            if (check_isa<OMPExecutableDirective>(parentRegion->getStmt())
                && !parentRegion->mainNode->noWait) {

                if (DARTS_BACKEND)
                    signalStream << "TPParent->";

                if (this->getRegion()->nextRegion == nullptr) {
                    signalStream << "barrierCodelets" << parentRegion->getID();

                    if (signalFromBarrier) {

                        if (DARTS_BACKEND)
#if 0
                            tempStream << "for(size_t codeletsCounter=0; "
                                       << "codeletsCounter<myTP->numThreads; "
                                       << "codeletsCounter++)\n"
                                       << "{\n"
                                       << signalStream.str() << "[0].decDep();\n"
                                       << "}\n";
#else
							tempStream << signalStream.str() << "[0].setDep(0);\n"
                                       << "myTP->add(&(" << signalStream.str() << "[0]));\n";
#endif
                        else if (SWARM_BACKEND)
                            tempStream
                                << "for(size_t codeletsCounter=0; "
                                << "codeletsCounter < numThreads" << parentRegion->getID() << "; "
                                << "codeletsCounter++)\n"
                                << "{\n"
                                << "swarm_Dep_satisfy(&" << signalStream.str() << "Dep, 1);\n"
                                << "}\n";
                    } else {

                        if (DARTS_BACKEND) {
                            nextCodeletName << signalStream.str() << "[0]";
                            tempStream << nextCodeletName.str() << ".decDep();\n";
                        } else if (SWARM_BACKEND) {
                            nextCodeletName << signalStream.str();
                            tempStream << "swarm_Dep_satisfy(&" << nextCodeletName.str()
                                       << "Dep, 1);\n";
                        }
                    }
                } else if (parentRegion->mainNode->tasksToExecute.size() > 0) {

                    if (DARTS_BACKEND) {
                        nextCodeletName << signalStream.str() << "barrierCodelets"
                                        << parentRegion->getID() << "[0]";
                        tempStream << nextCodeletName.str() << ".decDep();\n";
                    } else if (SWARM_BACKEND) {
                        nextCodeletName << signalStream.str() << "barrierCodelets"
                                        << parentRegion->getID();
                        tempStream << "swarm_Dep_satisfy(&" << nextCodeletName.str()
                                   << "Dep, 1);\n";
                    }

                } else {

                    if (DARTS_BACKEND) {
                        nextCodeletName << signalStream.str() << "barrierCodelets"
                                        << parentRegion->getID() << "[0]";
                        tempStream << nextCodeletName.str() << ".decDep();\n";
                    } else if (SWARM_BACKEND) {
                        nextCodeletName << signalStream.str() << "barrierCodelets"
                                        << parentRegion->getID();
                        tempStream << "swarm_Dep_satisfy(&" << nextCodeletName.str()
                                   << "Dep, 1);\n";
                    }
                }
                found = true;
            } else if (parentRegion->nextRegion) {
                int idToSignal = parentRegion->nextRegion->mainNode->getID();

                if (check_isa<OMPBarrierDirective>(parentRegion->nextRegion->getStmt())
                    || check_isa<OMPTaskwaitDirective>(parentRegion->nextRegion->getStmt())) {
                    signalStream << "TPParent->barrierCodelets" << idToSignal;
                    if (parentRegion->nextRegion->parent
                        && parentRegion->nextRegion->parent->singleThreaded) {
                        nextCodeletName << signalStream.str();
                    } else {
                        nextCodeletName << signalStream.str() << "[0]";
                    }
                } else {
                    signalStream << "TPParent->checkInCodelets" << idToSignal;
                    if (parentRegion->nextRegion->parent
                        && parentRegion->nextRegion->parent->singleThreaded) {
                        nextCodeletName << signalStream.str();
                    } else {
                        nextCodeletName << signalStream.str() << "[" << postfixStr << "]";
                    }
                }

                if (signalFromBarrier) {
                    tempStream << "for(size_t codeletsCounter=0; "
                               << "codeletsCounter<myTP->numThreads; "
                               << "codeletsCounter++)\n"
                               << "{\n";

                    if (parentRegion->nextRegion->parent
                        && parentRegion->nextRegion->parent->singleThreaded) {
                        tempStream << signalStream.str() << ".decDep();\n";
                    } else {
                        tempStream << signalStream.str() << "[codeletsCounter].decDep();\n";
                    }

                    tempStream << "}\n";
                } else {

                    if (DARTS_BACKEND)
                        tempStream << nextCodeletName.str() << ".decDep();\n";
                    else if (SWARM_BACKEND)
                        tempStream << "swarm_Dep_satisfy(&" << nextCodeletName.str()
                                   << "Dep, 1);\n";
                }
                found = true;
            }

            else {
                if (DARTS_BACKEND)
                    signalStream << "TPParent->";

                parentRegion = parentRegion->parent;
            }
        }
    }

    outputStream << tempStream.str();

    return nextCodeletName.str();
}

void DFGNode::showNodeInfo()
{
    debugFile << "----\n";
    debugFile << this->name;
    debugFile << ", id: " << this->getID();

    debugFile << ", num children: " << this->myNodes.size();

    if (this->parent) {
        debugFile << ", parent " << this->parent->getID() << ": " << this->parent->name;
    }

    if (this->getRegion()) {
        debugFile << ", myRegionID: " << this->getRegion()->getID();
        if (this->getRegion()->parent) {
            debugFile << ", myParentRegionID: " << this->getRegion()->parent->getID();
        }

        if (this->getRegion()->prevRegion) {
            debugFile << ", myPrevRegionID: " << this->getRegion()->prevRegion->getID();
        }

        if (this->getRegion()->nextRegion) {
            debugFile << ", myNextRegionID: " << this->getRegion()->nextRegion->getID();
        }

        if (this->getRegion()->getID() == this->getID()) {
            debugFile << ", Region nodes: ";
            for (DFGNode* tempNode : this->getRegion()->myNodes) {
                debugFile << tempNode->getID() << " ,";
            }
        }
    }

    if (this->prev) {
        debugFile << ", prev: " << this->prev->getID();
    }
    if (this->next) {
        debugFile << ", next: " << this->next->getID();
    }
    debugFile << "\n";

    if (check_isa<CallExpr>(this->getStmt())) {
        debugFile << "calleeFunctionNodeName: " << calleeFunctionNodeName << "\n";
        debugFile << "calleeTmpVarName: " << calleeTmpVarName << "\n";
        debugFile << "useTmpVarInsteadOfCall: " << useTmpVarInsteadOfCall << "\n";
    }

    debugFile << "calleeNames:\n";
    for (pair<string, string> tmpPair : this->calleeNames) {
        debugFile << tmpPair.first << " " << tmpPair.second << "\n";
    }

    if (this->myFunctionInfo) {
        debugFile << "Function: " << this->myFunctionInfo->name
                  << " return type: " << this->myFunctionInfo->returnType << " ";

        std::ostringstream tempOutputStream;
        this->myFunctionInfo->printInterface(tempOutputStream);
        debugFile << tempOutputStream.str() << "\n";
    }

    if (this->ompInputs.size() > 0) {
        debugFile << "Inputs: ";
        for (auto var : this->ompInputs) {
            debugFile << get<0>(var.second) << " " << var.first << " " << get<1>(var.second)
                      << "; ";
        }
        debugFile << "\n";
    }

    if (this->ompVariables.size() > 0) {
        debugFile << "Variables: ";
        for (auto var : this->ompVariables) {
            debugFile << get<0>(var.second) << " " << var.first << " " << get<1>(var.second) << ";";
        }
        debugFile << "\n";
    }

    if (check_isa<OMPExecutableDirective>(this->s)) {
        if (this->noWait) {
            debugFile << "NoWait\n";
        } else {
            debugFile << "Implicit Barrier\n";
        }

        if (this->myLoopInfo) {
            debugFile << "\nScheduling : " << this->myLoopInfo->schedulingPolicy.first;
        }

        debugFile << "\n";
    }

    if (criticalDirectiveName.empty() == false) {
        debugFile << "criticalDirectiveName: " << criticalDirectiveName
                  << ", criticalDirectiveLock: " << criticalDirectiveLock
                  << ", criticalDirectiveUnlock: " << criticalDirectiveUnlock << "\n";
    }

    if (this->myBranchInfo) {
        debugFile << "isSimpleIf: " << this->myBranchInfo->isSimpleIf << "\n";
    }
    debugFile << "isComplex: " << this->isComplexStmt << "\n";

    DFGNode* childNode = this->myNodes.front();
    while (childNode) {
        childNode->showNodeInfo();
        childNode = childNode->next;
    }
}

void DFGNode::printFunctionCall(std::ostringstream& outputStream)
{
    outputStream << this->fireCodeStr;
}

DFGNode* DFGNode::findMySyncPoint()
{
    if (check_isa<OMPExecutableDirective>(this->getStmt()) && !this->noWait)
        return this;

    DFGNode* syncNode = nullptr;
    DFGNode* nextNode = this->next;

    while (nextNode) {
        debugFile << "nextNode " << nextNode->getName() << " " << nextNode->getID() << "\n";

        if (check_isa<OMPTaskwaitDirective>(nextNode->getStmt())
            || (check_isa<OMPExecutableDirective>(nextNode->getStmt()) && !nextNode->noWait)) {
            syncNode = nextNode;
            break;
        }
        nextNode = nextNode->next;
    }
    if (syncNode == nullptr && this->parent) {
        debugFile << "finding SyncPoint in parent\n";
        syncNode = this->parent->findMySyncPoint();
    }
    return syncNode;
}

DFGNode* DFGNode::findNextBarrier()
{
    if (check_isa<OMPExecutableDirective>(this->getStmt()) && this->noWait == false)
        return this;

    DFGNode* barrier = nullptr;
    DFGNode* nextNode = this->next;

    while (nextNode) {
        if (check_isa<OMPExecutableDirective>(nextNode->getStmt()) && nextNode->noWait == false) {
            barrier = nextNode;
            break;
        }
        nextNode = nextNode->next;
    }
    if (barrier == nullptr && this->parent) {
        barrier = this->parent->findNextBarrier();
    }
    return barrier;
}

void DFGNode::getVarSize(std::string varSize, DFGNode* parentNode, std::string& varSizeStr,
    std::string prefixStr, std::string postfixStr)
{
    ostringstream tempStream;
    TPInputs_t::iterator itInputs = parentNode->ompInputs.find(varSize);
    TPInputs_t::iterator itVars = parentNode->ompVariables.find(varSize);
    if (itInputs != parentNode->ompInputs.end()) {
        OMP_VARSCOPE varScope = get<1>(itInputs->second);

        if (varScope == OMP_SHARED)
            tempStream << "*"
                       << "(" << prefixStr << varSize << "_darts" << parentNode->getID() << ")";
        else
            tempStream << "(" << prefixStr << varSize << "_darts" << parentNode->getID()
                       << postfixStr << ")";

    } else if (itVars != parentNode->ompVariables.end()) {
        OMP_VARSCOPE varScope = get<1>(itVars->second);

        if (varScope == OMP_SHARED)
            tempStream << "*"
                       << "(" << prefixStr << varSize << "_darts" << parentNode->getID() << ")";
        else
            tempStream << "(" << prefixStr << varSize << "_darts" << parentNode->getID()
                       << postfixStr << ")";

    } else {
        tempStream << varSize;
    }

    varSizeStr = tempStream.str();
}
}
