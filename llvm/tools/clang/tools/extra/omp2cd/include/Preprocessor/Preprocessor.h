#ifndef _PREPROCESSOR_H_
#define _PREPROCESSOR_H_

#include "include/BaseVisitor/BaseVisitor.h"
#include "include/DFG/DFG.h"
#include "include/DFG/DFGNode.h"
#include "include/omp2cd-utils.h"

extern std::string outputInclude;
extern std::ofstream outputFileH;
extern std::ofstream outputFileCpp;
extern std::string outputFileNameH;
extern std::string outputFileNameCpp;
extern std::unordered_map<std::string, std::string> outputFileNames;
extern int dfgNodesCount;

namespace omp2cd_space {

/*Traverse the AST and find the function calls.*/
class ompCallExprVisitor : public clang::RecursiveASTVisitor<ompCallExprVisitor>, BaseVisitor {
public:
    explicit ompCallExprVisitor(clang::ASTContext* context)
        : BaseVisitor(context)
    {
    }
    bool TraverseStmt(clang::Stmt* s);
    bool VisitFunctionDecl(clang::FunctionDecl* func);
    bool VisitCallExpr(clang::CallExpr* s);
    bool VisitOMPExecutableDirective(clang::OMPExecutableDirective* s);
    bool VisitOMPParallelDirective(clang::OMPParallelDirective* s);
    bool VisitOMPParallelForDirective(clang::OMPParallelForDirective* s);
};

class ompCallExprConsumer : public clang::ASTConsumer {
public:
    explicit ompCallExprConsumer(clang::ASTContext* context, clang::SourceManager* sourceManager)
        : visitor(context)
        , sourceManager(sourceManager)
    {
    }

    virtual void HandleTranslationUnit(clang::ASTContext& Context)
    {
        std::string fileName;
        auto Decls = Context.getTranslationUnitDecl()->decls();
        for (auto& Decl : Decls) {
            const auto& FileID = sourceManager->getFileID(Decl->getLocation());
            if (FileID != sourceManager->getMainFileID()) {
                continue;
            } else {
                // get file name and create output file name
                std::string mainFileName = sourceManager->getFilename(Decl->getLocation());
                std::string outputFileName = sourceManager->getFilename(Decl->getLocation());

                size_t pos1 = outputFileName.find_last_of(".");
                if (pos1 != std::string::npos) {
                    outputFileName = outputFileName.substr(0, pos1);
                }

                std::pair<std::string, std::string> outputNamePair(mainFileName, outputFileName);
                outputFileNames.insert(outputNamePair);
            }
            visitor.TraverseDecl(Decl);
        }
    }

private:
    ompCallExprVisitor visitor;
    clang::SourceManager* sourceManager;
};

class ompCallExprFrontEnd : public clang::ASTFrontendAction {
public:
    virtual std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(
        clang::CompilerInstance& Compiler, llvm::StringRef InFile)
    {
        return std::unique_ptr<clang::ASTConsumer>(
            new ompCallExprConsumer(&Compiler.getASTContext(), &Compiler.getSourceManager()));
    }
};

/*Identify the includes in the files*/
class PreprocessorCallbacks : public clang::PPCallbacks {
public:
    PreprocessorCallbacks(clang::PPCallbacks& findIncludeFilesCallback)
        : findIncludeFilesCallback(findIncludeFilesCallback)
    {
    }

    void InclusionDirective(clang::SourceLocation hashLoc, const clang::Token& includeTok,
        clang::StringRef fileName, bool isAngled, clang::CharSourceRange filenameRange,
        const clang::FileEntry* file, clang::StringRef searchPath, clang::StringRef relativePath,
        const clang::Module* imported)
    {
        findIncludeFilesCallback.InclusionDirective(hashLoc, includeTok, fileName, isAngled,
            filenameRange, file, searchPath, relativePath, imported);
    }

private:
    clang::PPCallbacks& findIncludeFilesCallback;
};

class FindIncludeFilesCallback : private clang::PPCallbacks {
public:
    FindIncludeFilesCallback(clang::CompilerInstance& compiler)
        : compiler(compiler)
    {
    }

    clang::PPCallbacks* createPreprocessorCallbacks() { return new PreprocessorCallbacks(*this); }

    void InclusionDirective(clang::SourceLocation hashLoc, const clang::Token& includeTok,
        clang::StringRef fileName, bool isAngled, clang::CharSourceRange filenameRange,
        const clang::FileEntry* file, clang::StringRef searchPath, clang::StringRef relativePath,
        const clang::Module* imported)
    {
        std::string fileNameStr = fileName;

        if (fileName.compare("assert.h") == 0)
            if (DARTS_BACKEND)
                fileName = "cassert";

        if (compiler.getSourceManager().isInMainFile(hashLoc)
            && fileNameStr.compare("omp.h") != 0) {
            std::string includeFileName = "#include ";
            if (isAngled) {
                includeFileName += "<";
                includeFileName += fileName;
                includeFileName += ">\n";
            } else {
                includeFileName += "\"";
                includeFileName += fileName;
                includeFileName += "\"\n";
            }

            std::string extension = "";
            size_t pos1 = fileNameStr.find_last_of(".");
            if (pos1 != std::string::npos) {
                fileNameStr = fileNameStr.substr(0, pos1);
            }

            std::unordered_map<std::string, std::string>::iterator it = outputFileNames.begin();
            for (; it != outputFileNames.end(); it++) {
                std::string source_name = "";
                size_t pos1 = it->first.find_last_of(".");
                if (pos1 != std::string::npos) {
                    source_name = it->first.substr(0, pos1);
                }

                pos1 = source_name.find_last_of("/");
                if (pos1 != std::string::npos) {
                    source_name = source_name.substr(pos1 + 1, source_name.length() - pos1 - 1);
                }
                if (source_name.compare(fileNameStr) == 0) {

                    if (DARTS_BACKEND)
                        fileNameStr += ".output.darts.h";
                    else if (SWARM_BACKEND)
                        fileNameStr += ".output.swarm.h";
#if 0
					includeFileName += "/*outputFileNameH: ";
					includeFileName += outputFileNameH;
					includeFileName += ", fileNameStr: ";
					includeFileName += fileNameStr;
					includeFileName += "*/\n";
#endif

                    if (outputFileNameH.find(fileNameStr) == std::string::npos) {
                        includeFileName += "#include ";
                        if (isAngled) {
                            includeFileName += "<";
                            includeFileName += fileNameStr;
                            includeFileName += ">\n";
                        } else {
                            includeFileName += "\"";
                            includeFileName += fileNameStr;
                            includeFileName += "\"\n";
                        }
                    }
                }
            }
            outputInclude += includeFileName;
        }
    }

private:
    const clang::CompilerInstance& compiler;
    std::vector<std::string> includeFileNamesVec;
};

class FindIncludeFilesAction : public clang::PreprocessOnlyAction {
protected:
    virtual void ExecuteAction()
    {
        FindIncludeFilesCallback includeFilesCallback(getCompilerInstance());
        getCompilerInstance().getPreprocessor().addPPCallbacks(std::unique_ptr<clang::PPCallbacks>(
            includeFilesCallback.createPreprocessorCallbacks()));
        clang::PreprocessOnlyAction::ExecuteAction();
    }
};

class IncludeFilesConsumer : public clang::DiagnosticConsumer {
public:
    bool IncludeInDiagnosticCounts() const { return false; }
};
}
#endif
