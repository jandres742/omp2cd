#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"

#include <fstream>
#include <stdio.h>
#include <unordered_map>

#include "include/Compiler/ProcessVisitor.h"
#include "include/Preprocessor/Preprocessor.h"
#include "include/omp2cd-utils.h"

using namespace llvm;
using namespace clang;
using namespace clang::tooling;
using namespace std;

using namespace omp2cd_space;

static cl::OptionCategory LibToolingCat("omp2cd options");

/*Command Line Options*/
static cl::opt<bool> useTPLoopsOption("useTPLoops", cl::cat(LibToolingCat), cl::init(false));
bool useTPLoops;

static cl::opt<bool> inlineOMPOption("inlineOMP", cl::cat(LibToolingCat), cl::init(false));
bool inlineOMP;

static cl::opt<bool> inlineOMPForOption("inlineOMPFor", cl::cat(LibToolingCat), cl::init(false));
bool inlineOMPFor;

static cl::opt<string> debugFileOption("d", cl::cat(LibToolingCat));
std::ofstream debugFile;

static cl::opt<string> backEndOption("t", cl::cat(LibToolingCat));
std::string backEnd;

cl::opt<int> instrGranularityThrOption("IG", cl::cat(LibToolingCat), cl::init(0));
size_t instrGranularityThr;

static cl::opt<bool> balanceOMPForOption("balanceOMPFor", cl::cat(LibToolingCat), cl::init(0));
bool balanceOMPFor;
bool changePtrs;

/*Help message*/
static cl::extrahelp MoreHelp("\nUniversity of Delaware, 2017\n");

std::string outputInclude;
std::ofstream outputFileH;
std::ofstream outputFileCpp;
std::string outputFileNameH;
std::string outputFileNameCpp;
std::string suffixLang;

/*Some globals*/
omp2cd_space::FunctionTable_t functionTable;
unordered_map<std::string, std::string> outputFileNames;
int dfgNodesCount;

int main(int argc, const char** argv)
{
    dfgNodesCount = 0;

    /*Help messages for options */
    backEndOption.HelpStr = "Output fine-grain runtime selected: darts or swarm.\n";
    instrGranularityThrOption.HelpStr = "Maximum number of statements in a region allowed for \
inlining.\n";
    inlineOMPOption.HelpStr = "Inline OMP regions if the number of statements is less than IG.\n";
    inlineOMPForOption.HelpStr = "Inline OMP for regions if the number of statements is less \
than IG.\n";
    balanceOMPForOption.HelpStr = "Decompose an OMP for into several TPs to be distributed \
across NUMA nodes (disables inlineOMPForOption).\n";
    useTPLoopsOption.HelpStr = "Activate DARTS TPLoops for applications with parallel \
loops (Experimental).\n";
    debugFileOption.HelpStr = "File where debug information about the DFG is dumped.\n";

    /*create compilation database */
    CommonOptionsParser optionsParser(argc, argv, LibToolingCat);

    /*Get debug file */
    string debugFileName = debugFileOption;
    if (!debugFileOption.empty())
        debugFile.open(debugFileName);

    /*Check which back end was chosen*/
    backEnd = "darts";
    if (!backEndOption.empty())
        backEnd = backEndOption;

    /*Get inline options */
	inlineOMPFor = inlineOMPForOption;
    inlineOMP = inlineOMPOption;
	if(inlineOMP)
		inlineOMPFor = true;
    useTPLoops = useTPLoopsOption;

    /*Get instruction granularity threshold */
    instrGranularityThr = instrGranularityThrOption;

    /*To balance OMPFor regions, we need to spawn several TPs and therefore,
       cannot inline those regions, so cancel the inlineOMPFor option.
       Also, activate changePtrs since with balanceOMPFor, we need to use
       localID to get the arrays. */
    balanceOMPFor = balanceOMPForOption;
    if (balanceOMPFor) {
        inlineOMPFor = false;
        changePtrs = true;
    }

    for (string sourceFileName : optionsParser.getSourcePathList()) {
        debugFile << "sourceFileName : " << sourceFileName << "\n";
    }

    /*Create tool with compilation database */
    ClangTool preprocessTool(optionsParser.getCompilations(), optionsParser.getSourcePathList());

    /*Find functions with omp pragmas and mark those functions and the one calling them
    so they are converted into codelets. We do it twice to make sure we identify all
    the callers and callees.*/
    /*TODO: Since we do it only twice, we only identify omp regions in functions having
    calls with 2-level depth. There should be a better way of doing it to handle
    n-level depth*/
    preprocessTool.run(newFrontendActionFactory<ompCallExprFrontEnd>().get());
    preprocessTool.run(newFrontendActionFactory<ompCallExprFrontEnd>().get());

    debugFile << "-----------------------------\n";
    debugFile << "Function Table (size = " << functionTable.size() << "):\n";
    for (auto it : functionTable) {
        debugFile << "-----------------------------\n"
                  << "funcName              = " << it.first << "\n"
                  << "type                  = " << std::get<0>(it.second) << "\n"
                  << "isOmp2dartsFunction   = " << std::get<1>(it.second) << "\n"
                  << "id                    = " << std::get<2>(it.second) << "\n"
                  << "Num of Parallel Reg   = " << std::get<5>(it.second) << "\n";
        for (std::pair<std::string, ExtraArgsTuple_t> extraArgs : std::get<3>(it.second)) {
            debugFile << "extra args cause      = " << extraArgs.first << "\n"
                      << "extra args(interface) = " << (std::get<0>(extraArgs.second)).first << " "
                      << (std::get<0>(extraArgs.second)).second << "\n"
                      << "extra args(call)      = " << std::get<1>(extraArgs.second) << "\n";
        }
    }
    debugFile << "-----------------------------\n";
    debugFile << "Compiling\n";

    for (pair<std::string, std::string> namePair : outputFileNames) {
        /*open outputfile for writing */
        string fileName = namePair.first;
        debugFile << "fileName : " << fileName << "\n";

        outputInclude = "";

        outputFileNameH = namePair.second;
        if (DARTS_BACKEND)
            outputFileNameH += ".output.darts.h";
        else if (SWARM_BACKEND)
            outputFileNameH += ".output.swarm.h";

        outputFileNameCpp = namePair.second;
        if (DARTS_BACKEND)
            outputFileNameCpp += ".output.darts.cpp";
        else if (SWARM_BACKEND)
            outputFileNameCpp += ".output.swarm.c";

        std::vector<std::string> sourceFile;
        sourceFile.push_back(fileName);

        ClangTool compilerTool(optionsParser.getCompilations(), sourceFile);

        IncludeFilesConsumer includeFilesConsumer;
        compilerTool.setDiagnosticConsumer(&includeFilesConsumer);
        compilerTool.run(newFrontendActionFactory<FindIncludeFilesAction>().get());

        /*compile*/
        compilerTool.run(newFrontendActionFactory<ompASTFrontEnd>().get());
    }

    if (!debugFileOption.empty())
        debugFile.close();

    return 0;
}