#ifndef _LOOPINFO_H_
#define _LOOPINFO_H_

#include "include/DFG/DFG.h"

namespace omp2cd_space {

class DFGNode;
class LoopCondNode;
class LoopIncNode;

/*A class to store info about an omp for*/

class LoopInfo {
public:
    LoopInfo(clang::ASTContext* Context);
    ~LoopInfo();

    DFGNode* initNode;
    LoopCondNode* condNode;
    DFGNode* incNode;

    clang::Stmt* initStmt;
    clang::Expr* condExpr;
    clang::Expr* incrExpr;
    clang::Expr* loopVarExpr;

    clang::ForStmt* s;
    std::string loopVarType;
    std::string loopVar;

    std::string loopCondOpcode;
    std::string loopIncrOpcode;
    std::string loopInitOpcode;

    std::string localLoopVarStr;
    std::string transformedLoopVarStr;
    std::string initStr;
    std::string condStr;
    std::string incrStr;
    std::string incrVal;

    std::string chunkStr;

    /*Reduction variables used in the loop if any: unordered_map < variable's name, operator > */
    ReductionVar_t reductionVars;
    clang::ASTContext* context;

    /*Scheduling policy used on this loop: pair < scheduling policy, chunk size > */
    std::pair<omp2cd_space::OMP_SCHEDULE, omp2cd_space::chunkTuple_t> schedulingPolicy;

    void initForStmtInfo(clang::ForStmt* s, omp2cd_space::DFGNode* node, std::string prefixStr = "",
        std::string postfixStr = "");
    void initWhileStmtInfo(clang::WhileStmt* s, omp2cd_space::DFGNode* node,
        std::string prefixStr = "", std::string postfixStr = "");
    void initDoStmtInfo(clang::DoStmt* s, omp2cd_space::DFGNode* node, std::string prefixStr = "",
        std::string postfixStr = "");

    std::string getForStmtVarRaw();
    std::string getForStmtInitRaw();
    std::string getForStmtLimitRaw();
    std::string getForStmtCondRaw();
    std::string getForStmtIncrRaw();
    void getForStmtStrings(DFGNode* node, std::string& prefixStr, std::string& postfixStr);
    std::string getForStmtVar(
        omp2cd_space::DFGNode* node, std::string prefixStr, std::string postfixStr);
    std::string getForStmtInit(
        omp2cd_space::DFGNode* node, std::string prefixStr, std::string postfixStr);
    std::string getForStmtLimit(
        omp2cd_space::DFGNode* node, std::string prefixStr, std::string postfixStr);
    std::string getForStmtLimitOnInvoke(
        omp2cd_space::DFGNode* node, std::string& prefixStr, std::string& postfixStr);
    std::string getForStmtCond(
        omp2cd_space::DFGNode* node, std::string& prefixStr, std::string& postfixStr);
    std::string getForStmtCondWhile(
        omp2cd_space::DFGNode* node, std::string& prefixStr, std::string& postfixStr);
    void getForStmtCondOpCode();
    std::string getForStmtIncr(
        omp2cd_space::DFGNode* node, std::string& prefixStr, std::string& postfixStr);
    std::string getForStmtChunk(
        omp2cd_space::DFGNode* node, std::string prefixStr, std::string postfixStr);
    void printRequestIterationsFunctionDef(
        omp2cd_space::DFGNode* node, std::ostringstream& outputStream);
    void printRequestIterationsFunctionImpl(
        omp2cd_space::DFGNode* node, std::ostringstream& outputStream, int parentRegionID,
		TPRegion* region = nullptr);
    void printRequestIterationsFunctionDefTPLoops(
        omp2cd_space::DFGNode* node, std::ostringstream& outputStream);
    void printRequestIterationsFunctionImplTPLoops(
        omp2cd_space::DFGNode* node, std::ostringstream& outputStream, int parentRegionID);
    void printCalcOfParamsForBalancedOMPFor(TPRegion* region, std::ostringstream& outputStream);
    void findVarsToChange(std::vector<std::string>& varNamesToDeclare,
        std::vector<std::string>& varNamesToChange, std::vector<std::string>& varTypesToChange,
        std::vector<std::string>& dereferenceSymbol, omp2cd_space::TPRegion* parentRegion,
        DFGNode* parentNode, std::ostringstream& ptrsStream, std::string& forStmtString,
        std::string& forBodyString);
    void updateLastPrivateVars(omp2cd_space::TPRegion* parentRegion, DFGNode* parentNode,
        std::ostringstream& outputStream);
    bool checkIfVarisUsedInLoop(
        std::string& varName, std::string& forStmtString, std::string& forBodyString);
    void replaceVarsForPtrs(std::vector<std::string>& varNamesToDeclare,
        std::vector<std::string>& varNamesToChange, std::vector<std::string>& dereferenceSymbol,
        DFGNode* node, std::ostringstream& changedStream, bool useLocalLoopVar = true);
    void replaceLoopVarForLocal(DFGNode* node, std::ostringstream& changedStream);
    void createPrologue(std::vector<std::string>& varNamesToDeclare,
        std::vector<std::string>& varNamesToChange, std::vector<std::string>& varTypesToChange,
        std::vector<std::string>& dereferenceSymbol, omp2cd_space::TPRegion* parentRegion,
        DFGNode* parentNode, DFGNode* node, std::ostringstream& prologueStream,
        std::string& fireStmtStream, std::string& fireCodeStream);
    void createForStmt(omp2cd_space::TPRegion* parentRegion, std::ostringstream& fireStmtStream);
    void implementDARTSLoops(omp2cd_space::TPRegion* region, omp2cd_space::TPRegion* parentRegion,
        std::ostringstream& fireStmtStream);
    void printFireCode(std::ostringstream& outputStream, TPRegion* parentRegion, DFGNode* node,
        DFGNode* parentNode, bool atLeastOneLastPrivate);
    void printTPLoopFireCode(
        std::ostringstream& outputStream, TPRegion* parentRegion, DFGNode* node);
    std::string getReductionVarInitVal(std::string opCode, std::string& varType);
    void printLoopParamInitialization(std::ostringstream& outputStream, TPRegion* region);
};
}
#endif
