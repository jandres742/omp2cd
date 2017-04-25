#ifndef _PROCESSVISITOR_H_
#define _PROCESSVISITOR_H_

#include "include/BaseVisitor/BaseVisitor.h"
#include "include/DFG/DFG.h"
#include "include/DFG/DFGNode.h"

extern std::string outputInclude;
extern std::ofstream outputFileH;
extern std::ofstream outputFileCpp;
extern std::string outputFileNameCpp;
extern std::string outputFileNameH;
extern std::ofstream debugFile;
extern omp2cd_space::VarTable_t varTable;
extern int dfgNodesCount;
extern std::string backEnd;

namespace omp2cd_space {

/*Traverses ASTs and annotates them*/
class ompASTVisitor : public clang::RecursiveASTVisitor<ompASTVisitor>, BaseVisitor {
public:
    explicit ompASTVisitor(clang::ASTContext* context, clang::SourceManager* sourceManager)
        : BaseVisitor(context)
        , sourceManager(sourceManager)
        , sourceFileName("")
    {
    }

    void printRegionContext_swarm(TPRegion* region, std::string& codeStr);
    void printRegionCodelets_swarm(TPRegion* region, std::string& codeStr);

    void printSequentialFunction(omp2cd_space::DFGNode* functionNode);
    void printSequentialDFG(DFGNode* node, std::string& codeStr, clang::PrintingPolicy& Policy);
    void printSequentialNode(DFGNode* node, std::string& codeStr, clang::PrintingPolicy& Policy,
        bool printSemicolon = true);
    void printForStmtWithTaskToFireString(DFGNode* node, DFGNode* parentNode,
        TPRegion* parentRegion, std::string& codeStr, bool printSemiColon);
    void printForStmtToFireString(DFGNode* node, DFGNode* parentNode, TPRegion* parentRegion,
        std::string& codeStr, bool printSemiColon);
    void printNodeToFireString(DFGNode* node, std::string& codeStr, bool printSemiColon = true);
    void printTPDef(TPRegion* region, std::ostringstream& TPStream);
    void printTPDefTPLoop(TPRegion* region, std::ostringstream& TPStream);
    void printTPConstructor(TPRegion* region, std::ostringstream& TPStream);
    void printTPConstructorTPLoop(TPRegion* region, std::ostringstream& TPStream);
    void printTPDestructor(TPRegion* region, std::ostringstream& TPStream);
    void printTPDestructorTPLoop(TPRegion* region, std::ostringstream& TPStream);

    void printRegion(TPRegion* region, std::string& codeStr);
    void printFunctionRegion(TPRegion* region, std::string& codeStr);
    void printRegionCode(TPRegion* region, std::string& codeStr);
    void printFunctionRegionCode(TPRegion* region, std::string& codeStr);
    void printRegionBarriers(TPRegion* region, std::string& codeStr);
    void printRegionBarriersDefs(TPRegion* region, std::string& codeStr);
    void printRegionBarriersInits(TPRegion* region, std::string& codeStr);
    void printRegionCodeletsClass(TPRegion* region, std::ostringstream& tempStream);

    void printRegionCodeletsFire_swarm(TPRegion* region, std::ostringstream& tempStream);

    void printRegionCodeletsFire(TPRegion* region, std::ostringstream& tempStream);

    void printRegionCodeletsDefs(TPRegion* region, std::ostringstream& tempStream);
    void printRegionCodeletsInits(TPRegion* region, std::ostringstream& tempStream);
    void printRegionCodeletsInitsInConstructor(TPRegion* region, std::ostringstream& tempStream);
    void printRegionCodeletsDelete(TPRegion* region, std::ostringstream& tempStream);
    void printRegionExtraVarsDefs(TPRegion* region, std::ostringstream& tempStream);
    void printRegionExtraVarsInits(TPRegion* region, std::ostringstream& tempStream);
    void printRegionExtraVarsFree(TPRegion* region, std::ostringstream& tempStream);
    void printRegionExtraVarsInitsInConstructor(TPRegion* region, std::ostringstream& tempStream);

    void createRegionsNodeCode(TPRegion* region);
    void createRegionsNodeCode_swarm(TPRegion* region);

    bool TraverseStmt(clang::Stmt* s);
    bool TraverseDecl(clang::Decl* decl);

    bool VisitDecl(clang::Decl* d);
    bool VisitVarDecl(clang::VarDecl* d);
    bool VisitTagDecl(clang::TagDecl* d);
    bool VisitEnumConstantDecl(clang::EnumConstantDecl* d);
    bool VisitParmVarDecl(clang::ParmVarDecl* func);
    bool VisitTypedefDecl(clang::TypedefDecl* d);

    bool VisitOMPExecutableDirective(clang::OMPExecutableDirective* s);
    bool VisitOMPAtomicDirective(clang::OMPAtomicDirective* s);
    bool VisitOMPTaskDirective(clang::OMPTaskDirective* s);
    bool VisitCapturedStmt(clang::CapturedStmt* s);
    bool VisitStmt(clang::Stmt* s);
    bool VisitIfStmt(clang::IfStmt* ifStmt);
    bool VisitForStmt(clang::ForStmt* forStmt);
    bool VisitWhileStmt(clang::WhileStmt* whileStmt);
    bool VisitDoStmt(clang::DoStmt* whileStmt);
    bool VisitCallExpr(clang::CallExpr* s);
    bool VisitDeclRefExpr(clang::DeclRefExpr* s);
    bool VisitOMPThreadPrivateDecl(clang::OMPThreadPrivateDecl* d);

    void addNewVarToTable(clang::VarDecl* d);

    void findDependencyInPreviousTask(DFGNode *node, std::string varName, unsigned int depType, std::vector<DFGNode*> &taskVector);

    void generateFineGrainCode();
    void generateRegions(TPRegion* parentRegion, DFGNode* node);
    void optimizeDFG(DFGNode* node);
    void optimizeCallExpr(DFGNode* node);
    void optimizeForStmt(DFGNode* node);
    void optimizeWhileStmt(DFGNode* node);
    void optimizeDoStmt(DFGNode* node);
    void optimizeCriticalAtomic(DFGNode* node);
    void optimizeOMPFor(DFGNode* node);
    void optimizeOMPTask(DFGNode* node);

    void insertNullNodeAfter(DFGNode* prevNode,
        DFGNodeNullStmtType_t nullStmtAction = NULLSTMT_EMPTY, std::string nullStmtActionVar = "");

    void writeHeaderFile(std::string& includeHFileStr);
    void writeImplemFile(std::string& includeHFileStr);

    void setPrintingPolicyParameters(DFGNode* node, clang::PrintingPolicy& Policy);
    void setMyIfNode(DFGNode* node);
    void assignPerElementToCorrectInitLoop(int res, std::ostringstream& inStream, TPRegion* region);
    void assignPerElementToCorrectFreeLoop(int res, std::ostringstream& inStream, TPRegion* region);

    DFGNode* getCalleeFunctionNode(clang::CallExpr* s);
    int getCalleeFunctionNodeID(std::string functName);
    std::string getInitValueForDeclStmt(DFGNode* node, clang::PrintingPolicy& Policy);

    void findCallsWithExtraArgsInNode(
        DFGNode* node, std::vector<std::pair<std::string, std::string> >& extraArgPairs);

    inline void clearDependTaskNodes() { this->dependTaskNodes.clear(); }

    inline void insertDependTaskNode(DFGNode* taskNode)
    {
        this->dependTaskNodes.push_back(taskNode);
    }

private:
    clang::SourceManager* sourceManager;
    std::string sourceFileName;
    std::ostringstream includesStream;
    std::ostringstream constantsDeclStream;
    std::ostringstream typedefDeclStream;
    std::ostringstream codeStream;
    std::ostringstream globalVarsStream;
    std::vector<DFGNode*> dependTaskNodes;
};

/*AST Consumer*/
class ompASTConsumer : public clang::ASTConsumer {
public:
    explicit ompASTConsumer(clang::ASTContext* context, clang::SourceManager* sourceManager)
        : visitor(context, sourceManager)
        , sourceManager(sourceManager)
    {
    }

    virtual void HandleTranslationUnit(clang::ASTContext& Context)
    {
        /*Visit the AST */
        auto decls = Context.getTranslationUnitDecl()->decls();
        for (auto& decl : decls) {
            /*Get any variable declaration in header files (extern decls mostly)*/
            clang::VarDecl* varDecl = llvm::dyn_cast<clang::VarDecl>(decl);
            if (varDecl != nullptr)
                visitor.addNewVarToTable(varDecl);

            const auto& fileID = sourceManager->getFileID(decl->getLocation());
            if (fileID != sourceManager->getMainFileID())
                continue;

            visitor.TraverseDecl(decl);
        }

        debugFile << "AST traverse ended, generating fine-grain application..."
                  << "\n";

        visitor.generateFineGrainCode();
    }

private:
    ompASTVisitor visitor;
    clang::SourceManager* sourceManager;
};

/*Front end for the compiler*/
class ompASTFrontEnd : public clang::ASTFrontendAction {
public:
    virtual std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(
        clang::CompilerInstance& Compiler, llvm::StringRef InFile)
    {
        return std::unique_ptr<clang::ASTConsumer>(
            new ompASTConsumer(&Compiler.getASTContext(), &Compiler.getSourceManager()));
    }
};
}
#endif
