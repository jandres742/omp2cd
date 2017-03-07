#ifndef _DFG_H
#define _DFG_H

#include <clang/AST/ASTConsumer.h>
#include <clang/AST/ASTContext.h>
#include <clang/AST/Decl.h>
#include <clang/AST/OpenMPClause.h>
#include <clang/AST/ParentMap.h>
#include <clang/AST/PrettyPrinter.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/AST/StmtOpenMP.h>
#include <clang/AST/omp2cd-typedefs.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/FrontendAction.h>
#include <clang/Frontend/FrontendActions.h>
#include <clang/Lex/PPCallbacks.h>
#include <clang/Lex/Preprocessor.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>
#include <llvm/Support/Casting.h>
#include <llvm/Support/CommandLine.h>
#include <llvm/Support/raw_ostream.h>

#include "include/omp2cd-utils.h"

#include <fstream>
#include <list>
#include <queue>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

#define initInLocalThreadsLoop 0
#define initInGlobalThreadsLoop 1

/*Enable this for debugging purposes*/
#define FULL_DEBUG_COMMENTS 0

#define DARTS_BACKEND backEnd.compare("darts") == 0
#define SWARM_BACKEND backEnd.compare("swarm") == 0

#define DARTS_PREFIXSTR_DEFAULT "this->inputsTPParent->"

extern std::ofstream debugFile;
extern bool useTPLoops;
extern bool inlineOMP;
extern bool inlineOMPFor;
extern bool balanceOMPFor;
extern size_t instrGranularityThr;
extern bool changePtrs;
extern std::string backEnd;
extern std::string suffixLang;

namespace omp2cd_space {
}

extern omp2cd_space::FunctionTable_t functionTable;

namespace omp2cd_space {

class DFG;
/* class DFGNode; */
class LoopInfo;
class TPRegion;
class TaskInfo;
class FunctionInfo;
class BranchInfo;
class NodeList;
class CopyPrivateClause;
class CopyinClause;
class LoopCondNode;
class LoopIncNode;
class AtomicNode;

/* Typedefs for the function table in a single decl unit*/
typedef std::pair<std::string, std::tuple<DFGNode*, TPRegion*> > myFunctions_Elem_t;
typedef std::unordered_map<std::string, std::tuple<DFGNode*, TPRegion*> > myFunctions_t;
typedef std::vector<std::string> myFunctionsVec_t;

/* Typedef for the data container used for a region's children*/
typedef std::vector<TPRegion*> myRegions_t;

/* Typedefs for the reduction var's table*/
typedef std::tuple<std::string, /*0 = reduction operator */
    std::string, /*1 = type */
    std::string, /*2 = name of the local reduction variable in the TP */
    std::string /*3 = name of the local reduction variable inside the codelet
(a ptr to avoid address calculation) */
    >
    ReductionVarTuple_t;
typedef std::pair<std::string, ReductionVarTuple_t> ReductionVarPair_t;
/* key: reduction variable's name*/
typedef std::unordered_map<std::string, ReductionVarTuple_t> ReductionVar_t;

typedef std::pair<std::string, std::string> CopyinVarPair_t;
typedef std::unordered_map<std::string, std::string> CopyinVars_t;

typedef std::pair<std::string, std::string> CopyprivateVarPair_t;
typedef std::unordered_map<std::string, std::string> CopyprivateVars_t;

enum DFGNodeNullStmtType_t { NULLSTMT_EMPTY = 0, NULLSTMT_ALLOCA };

enum DFGNodeKind { DFGNode_k, LoopCondNode_k, AtomicNode_k, ReturnNode_k };

class NodeList {
public:
    NodeList()
        : head(nullptr)
        , tail(nullptr)
        , length(0)
    {
    }
    size_t size();
    void insert(DFGNode* newNode);
    void insertBefore(DFGNode* node, DFGNode* newNode);
    void insertAfter(DFGNode* node, DFGNode* newNode);
    void swap(DFGNode* oldNode, DFGNode* newNode);
    void remove(DFGNode* node);
    void replace(DFGNode* oldNode, DFGNode* newNode);
    DFGNode* front();
    DFGNode* back();
    void clear();

    DFGNode* head;
    DFGNode* tail;
    size_t length;
};

class RegionList {
public:
    RegionList()
        : head(nullptr)
        , tail(nullptr)
        , length(0)
    {
    }

    size_t size();
    void insert(TPRegion* newRegion);
    void push_back(TPRegion* newRegion);
    void insertBefore(TPRegion* region, TPRegion* newRegion);
    void insertAfter(TPRegion* region, TPRegion* newRegion);
    void remove(TPRegion* region);
    void replace(TPRegion* oldRegion, TPRegion* newRegion);
    TPRegion* front();
    TPRegion* back();
    void clear();

    TPRegion* head;
    TPRegion* tail;
    size_t length;
};

/*The class for the Codelet Graph, here called DFG = DataFlow Graph*/
class DFG {
public:
    DFG()
        : root(nullptr)
        , numEnumTypesPrinted(0)
        , useFunctionStack(false)
    {
        if (backEnd.compare("darts") == 0) {
            suffixLang = "_darts";
        } else if (backEnd.compare("swarm") == 0) {
            suffixLang = "_swarm";
        }
    }

    void addRoot(DFGNode* rootNode);
    void addFunction(DFGNode* functionNode);
    void addFunctionDecl(DFGNode* functionNode);
    void removeFunction(DFGNode* functionNode);
    void printGlobalVars(std::ostringstream& tempStream, clang::ASTContext* context);
    void printFunctionStaticVars(std::ostringstream& tempStream, clang::ASTContext* context);
    void printFunctionStaticVarFlagsInits(
        std::ostringstream& tempStream, clang::ASTContext* context);
    void addGlobalVarsAsInputsToFunction(DFGNode* functionNode);
    void printThreadPrivateVars(std::ostringstream& tempStream, clang::ASTContext* context);
    void getVarInfoFromTable(
        DFGNode* node, const std::string varName, std::string& varType, std::string& varArray);
    bool getVarTupleFromTable(DFGNode* node, const std::string varName, VarTableTuple_t& varTuple);
    void insertVarTupleToTable(std::string varName, VarTableTuple_t& varTuple);

    DFGNode* root;
    int numEnumTypesPrinted;
    myFunctions_t myFunctions;
    myFunctions_t myFunctionsDecl;
    myFunctionsVec_t myFunctionsVec;
    bool useFunctionStack;

    std::vector<std::string> TPImplementations;
    std::vector<std::string> TPDefinitions;
    VarTable2_t varTable2;
};
}
#endif
