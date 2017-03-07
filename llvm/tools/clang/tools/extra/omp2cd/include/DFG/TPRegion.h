#ifndef _TPREGION_H_
#define _TPREGION_H_

#include "include/DFG/DFG.h"

namespace omp2cd_space {

class DFG;
class DFGNode;

class TPRegion {
public:
    TPRegion();
    TPRegion(DFGNode* _newNode);
    TPRegion(DFGNode* _newNode, TPRegion* _parent);

    void addNode(DFGNode* newNode);
    void addRegion(TPRegion* newRegion);
    void showRegionInfo();
    void printRegionFireCode(std::ostringstream& outputStream);
    size_t getNumNodes();
    DFGNode* getMainNode();
    clang::Stmt* getStmt();
    int getID();
    std::string getName();
    TPRegion* getParent();
    std::string signalNext(std::ostringstream& outputStream,
        std::string prefixStr = "myTP->controlTPParent->", std::string postfixStr = "this->getID()",
        bool signalFromBarrier = false, bool signalFromCallExpr = false);
    TaskInfo* getTaskInfo();
    LoopInfo* getLoopInfo();
    FunctionInfo* getFunctionInfo();
    TPRegion* getParentRegion();
    BranchInfo* getBranchInfo();
    CopyinClause* getCopyinClause();
    CopyPrivateClause* getCopyPrivateClause();
    DFG* getDFG();
    TPInputs_t getOMPInputs();
    TPInputs_t getOMPVariables();

    bool hasBarrier();
    TPRegion* findOMPOrFunctionRegionParent();
    bool isCaller();
    void makeChildrenSingleThreaded();
    bool hasLoopCondNode();
    bool isLoopRegion();
    bool isSingleThreaded();
    void getParallelRegions(std::vector<TPRegion*>& ompParallelRegionsVec);
    uint64_t getNumOMPRegions();
    void printRegions();
    void preProcess();
    void reduceBarriersInRegions();
    void makeSingleThreaded();
    void deleteDeclRegionsWithNoInit();
    void transformIfStmts();
    void transformTaskRegion();
    void transformLoops();
    void deleteExtraOMPForChildren();
    void mergeTaskWait();
    void setUpBranchRegions();
    void inlineOMPRegions();
    void findBarrierForCopyprivate();

    void printRegionCodeletFire_swarm(std::ostringstream& outputStream);

    void printCheckInCodeletClass(std::ostringstream& outputStream);
    void printCheckInCodeletFire(std::ostringstream& outputStream);
    void printCheckInCodeletDef(std::ostringstream& outputStream);
    void printCheckInCodeletInit(std::ostringstream& outputStream, bool startWithComma);
    void printCheckInCodeletDecDep(std::ostringstream& outputStream);
    void printCheckInCodeletPtr(std::ostringstream& outputStream);
    int printCheckInCodeletCreation(std::ostringstream& outputStream);
    void printCheckInCodeletDelete(std::ostringstream& outputStream);

    void printOMPCodeletFireCode(std::ostringstream& outputStream);
    void printOMPCodeletFireCodeInlined(std::ostringstream& outputStream);
    void printOMPCodeletFireCodeInvokeTPOneThread(std::ostringstream& outputStream);
    void printOMPCodeletFireCodeInvokeTPMultipleThread(std::ostringstream& outputStream);
    void printOMPSingleCodeletFireCode(std::ostringstream& outputStream);
    void printOMPSectionCodeletFireCode(std::ostringstream& outputStream);
    void printOMPMasterCodeletFireCode(std::ostringstream& outputStream);
    void printOMPForCodeletFireCode(std::ostringstream& outputStream);
    void printOMPForCodeletFireCode_swarm(std::ostringstream& outputStream);
    void printOMPForCodeletFireCode_darts(std::ostringstream& outputStream);

    void printTPLoopCodeletClass(std::ostringstream& outputStream);
    void printTPLoopCodeletCoreClass(std::ostringstream& outputStream);
    void printTPLoopCodeletCoreFire(std::ostringstream& outputStream);
    void printTPLoopCodeletForFire(std::ostringstream& outputStream);

    void printBarrierCodeletClass(std::ostringstream& outputStream, bool printingInRegion = false);
    void printBarrierCodeletDef(std::ostringstream& outputStream);
    void printBarrierCodeletInit(std::ostringstream& outputStream, bool startWithComma);
    int printBarrierCodeletCreation(
        std::ostringstream& outputStream, bool printingInRegion = false);
    void printBarrierCodeletFire(std::ostringstream& outputStream, bool printingInRegion = false);
    void printBarrierCodeletDelete(std::ostringstream& outputStream);

    void printAllNodesFireCode(std::ostringstream& outputStream);

    bool isSimpleRegion();
    bool isInlinedRegion();
    bool isBalancedOMPFor();
    bool isOMPForOnlyChildRegion();
    bool hasOnlyOneChildOMPForRegion();

    bool isOMPParallel();
    bool isOMPFor();

    TPRegion* parent;
    TPRegion* prevRegion;
    TPRegion* nextRegion;
    TPRegion* prev;
    TPRegion* next;

    DFGNode* mainNode;
    int id;
    std::string name;
    bool shouldBeDeleted;
    bool singleThreaded;
    bool doNotPrint;
    bool isThreadedProcedure;
    bool isCodelet;

    RegionList childRegions;
    std::vector<DFGNode*> myNodes;
    std::vector<TPRegion*> inlinedRegions;

    std::string initLocalThreadsLoopStr;
    std::string initGlobalThreadsLoopStr;
    std::string freeLocalThreadsLoopStr;
    std::string freeGlobalThreadsLoopStr;

    ~TPRegion();
};
}
#endif
