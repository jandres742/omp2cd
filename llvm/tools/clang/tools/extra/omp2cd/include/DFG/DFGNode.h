#ifndef _DFGNODE_H
#define _DFGNODE_H

#include "include/DFG/DFG.h"

namespace omp2cd_space {

class DFG;
class LoopInfo;
class TPRegion;
class FunctionInfo;
class TaskInfo;
class BranchInfo;

class OMPExtensionClause {
public:
    bool codelet;

    OMPExtensionClause()
        : codelet(false)
    {
    }
};

/*The class for a node in the DFG*/
class DFGNode {
public:
    DFGNode(int id, std::string name, DFGNodeKind k = DFGNode_k);
    DFGNode(DFGNode* originalNode, int id, DFGNodeKind k = DFGNode_k);
    DFGNode(clang::Stmt* s, DFGNodeKind k = DFGNode_k);
    DFGNode(clang::Stmt* s, DFGNode* parent, int id, DFGNodeKind k = DFGNode_k);
    DFGNode(clang::Decl* d, DFGNode* parent, int id, DFGNodeKind k = DFGNode_k);
    DFGNode(clang::FunctionDecl* f, DFGNode* parent, int id, DFGNodeKind k = DFGNode_k);
    ~DFGNode();

    void addChild(DFGNode* childNode);
    void addID(int id);
    void addNewInput(clang::Decl* decl, OMP_VARSCOPE varScope, int varPos = -1);
    void addNewInput(clang::Decl* decl, int varPos = -1);
    void addNewVar(clang::Decl* decl, int varPos = -1, std::string initValStr = "");
    void addNewVar(std::string varType, std::string varName);

    DFGNode* findFunctionParent();
    TPRegion* findMyRegion();
    DFGNode* findMySyncPoint();
    DFGNode* findNextBarrier();
    DFGNode* findOMPParent();
    DFGNode* findOMPParallelParent();
    DFGNode* findParentNode();

    void getParallelNodes(std::vector<DFGNode*>& ompParallelNodesVec);
    DFG* getDFG();
    void getClangName(std::string& clangName);
    clang::Stmt* getStmt();
    int getID();
    std::string getName();
    DFGNode* getParent();
    TPRegion* getRegion();
    LoopInfo* getLoopInfo();
    FunctionInfo* getFunctionInfo();
    TaskInfo* getTaskInfo();
    BranchInfo* getBranchInfo();
    CopyinClause* getCopyinClause();
    CopyPrivateClause* getCopyPrivateClause();
    DFGNode* getMyNodesHead();
    OMP_VARSCOPE getVariableScope(std::string varName);

    void printNodeFireCode(std::ostringstream& outputStream);

    void printExtraVarsInit(std::ostringstream& outputStream);
    void printExtraVarsInit_swarm(std::ostringstream& outputStream);
    void printExtraVarsInitInConstructor(std::ostringstream& outputStream);
    void printExtraVarsDef(std::ostringstream& outputStream, std::ostringstream& globalVarsStream);
    void printExtraVarsFree(std::ostringstream& outputStream);
    std::string printOMPInputNamesInFunctionCall(
        DFGNode* srcNode, std::string prefixStr, bool startWithComma, std::string postfixStr = "");

    std::string printCodeletParamPassing_swarm(DFGNode* srcNode, std::string postfixStr = "");

    std::string printOMPInputNamesFromSequential(bool startWithComma);
    std::string printOMPInputNamesFromSequential_swarm();

    std::string packOMPInputContextFromSequential_swarm();
    void unpackOMPInputContext_swarm(std::ostringstream& outputStream);

    bool printOMPInputAsParams(bool startWithComma, std::ostringstream& outputStream);

    void printOMPInputAndVarDefs(std::ostringstream& outputStream);
    void printOMPInputAndVarDefs_swarm(std::ostringstream& outputStream);

    void printOMPInputAndVarInits(std::ostringstream& outputStream);
    void printOMPInputAndVarInits_swarm(std::ostringstream& outputStream);
    void printOMPInputAndVarFree_swarm(std::ostringstream& outputStream);

    void printOMPInputAndVarInitsFromTPLooptoCodelet(
        bool startWithComma, std::ostringstream& outputStream);

    bool printOMPInputAsParamsRaw(bool startWithComma, std::ostringstream& outputStream);
    void printOMPInputAndVarDefsRaw(std::ostringstream& outputStream);
    void printOMPInputAndVarInitsRaw(std::ostringstream& outputStream);
    void printOMPInputAndVarInitsRawAsInputs(std::ostringstream& outputStream);

    void printOMPInputAndVarInitsInConstructor(
        std::string& outputStr, std::ostringstream& TPStream);
    void printOMPInputAndVarInitsInFirstCodelet(std::ostringstream& outputStream);
    void printOMPArrayVarInitsInFirstCodelet(std::ostringstream& outputStream, TPRegion* region);
    void printOMPArrayInputInitsInFirstCodelet(std::ostringstream& outputStream, TPRegion* region);
    void printOMPInputAndVarInitsInlinedRegion(std::ostringstream& outputStream);
    void printOMPInputAndVarFreeInDestructor(std::ostringstream& outputStream);
    void printOMPInputGetFromParent(std::ostringstream& outputStream);
    std::string printOMPParallelInputNames(bool startWithComma);
    std::string printOMPInputNames(bool startWithComma);
    bool printFunctionInputsAsParams(bool startWithComma, std::ostringstream& outputStream);
    void printOMPTaskInputsInInvoke(bool startWithComma, std::ostringstream& outputStream);
    void printUnpackOMPTaskInputs(std::ostringstream& outputStream);
    void printFunctionCall(std::ostringstream& outputStream);

    void printPrivateVarsStructDef(std::ostringstream& outputStream);
    void printPrivateVarsDef(std::ostringstream& outputStream);

    void findTPParent(std::ostringstream& outputStream, std::string& TPParentStr,
        TPRegion* currentRegion, TPRegion* regionToSignal);

    bool containsOMPDirectivesOrCalls();

    bool checkIfNodeIsChild(DFGNode* targetNode);

    std::string signalNextCodelet(TPRegion* parentRegion, std::ostringstream& outputStream,
        std::string prefixStr = "myTP->controlTPParent->", std::string postfixStr = "this->getID()",
        bool signalFromBarrier = false);

    std::string signalTaskSyncPoint(std::ostringstream& outputStream,
        std::string prefixStr = "myTP->controlTPParent->",
        std::string postfixStr = "this->getID()");

    std::string signalNext(std::ostringstream& outputStream,
        std::string prefixStr = "myTP->controlTPParent->", std::string postfixStr = "this->getID()",
        bool signalFromBarrier = false, bool signalFromCallExpr = false,
        bool getTaskSyncPoint = false);

    void printReductionVarPtrDefs(std::ostringstream& outputStream, TPRegion* parentRegion);
    void updateLocalReductionVars(std::ostringstream& outputStream, TPRegion* parentRegion);
    void updateGlobalReductionVars(std::ostringstream& outputStream, TPRegion* parentRegion);
    void replaceReductionVarInFireCode(std::ostringstream& changedStream, TPRegion* parentRegion);

    void printParallelReductionVarPtrDefs(std::ostringstream& outputStream, TPRegion* parentRegion);
    void updateLocalParallelReductionVars(std::ostringstream& outputStream, TPRegion* parentRegion);
    void updateGlobalParallelReductionVars(
        std::ostringstream& outputStream, TPRegion* parentRegion);
    void replaceParallelReductionVarInFireCode(std::string& codeStr, TPRegion* parentRegion);
    std::string getParallelReductionVarInitVal(std::string opCode);
    void printParallelReductionVarsDef(std::ostringstream& outputStream);

    void declareCriticalAtomicMutexes(std::ostringstream& globalVarsStream);
    void showNodeInfo();
    void reduceBarriersInRegions(TPRegion* parentRegion);
    void insertNewNodeBeforeChildNode(DFGNode* oldChild, DFGNode* newChild);
    bool checkIfComplexStmt();

    void printTaskDataDefs(std::ostringstream& outputStream);
    void printTaskDataInits(std::ostringstream& outputStream);
    void printTaskDataFree(std::ostringstream& outputStream);

    bool isCallerNode();
    bool isLoopNode();
    bool isOMPParallel();
    bool isOMPFor();

    bool isFuncCallInsideBranch();

    void getVarSize(std::string varSize, DFGNode* parentNode, std::string& varSizeStr,
        std::string prefixStr = DARTS_PREFIXSTR_DEFAULT,
        std::string postfixStr = "[this->getID()]");

public:
    clang::Decl* d;
    clang::Stmt* s;
    int id;
    std::string name;
    DFGNode* parent;
    DFG* myDFG;

    std::string fireCodeStr;
    std::ostringstream originalCodeStr;

    DFGNode* next;
    DFGNode* prev;

    bool noWait;
    OMPExtensionClause* ompExtensionClause;

    LoopInfo* myLoopInfo;
    TPRegion* myRegion;
    FunctionInfo* myFunctionInfo;
    TaskInfo* myTaskInfo;

    CopyinClause* myCopyinClause;
    CopyPrivateClause* myCopyPrivateClause;

    std::string calleeFunctionNodeName;
    int calleeFunctionNodeID;
    std::string calleeFunctionNodeType;
    bool calleeFunctionNodeIsSingleThreaded;

    std::string calleeTmpVarName;
    bool useTmpVarInsteadOfCall;

    BranchInfo* myBranchInfo;
    DFGNode* myIfNode;
    bool isRegionMainNode;
    bool isRegionNode;

    std::string criticalDirectiveName;
    bool criticalDirectiveLock;
    bool criticalDirectiveUnlock;

    bool isComplexStmt;

    TPInputs_t ompInputs;
    TPInputs_t ompVariables;
    TPCallees_t calleeNames;

    NodeList myNodes;
    std::vector<DFGNode*> tasksToExecute;
    std::vector<DFGNode*> nodesToCopyprivate;

    ReductionVar_t parallelReductionVars;

    DFGNodeNullStmtType_t nullStmtAction;
    std::string nullStmtActionVar;

private:
    const DFGNodeKind kind;

public:
    DFGNodeKind getKind() const { return this->kind; }
};
}
#endif
