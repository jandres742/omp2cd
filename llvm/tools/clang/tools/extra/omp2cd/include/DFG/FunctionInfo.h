#ifndef _FUNCITONINFO_H_
#define _FUNCTIONINFO_H_

#include "include/DFG/DFG.h"

namespace omp2cd_space {
class DFG;
class DFGNode;

/*A class to store info about a function converted into TP*/
class FunctionInfo {
public:
    FunctionInfo(clang::FunctionDecl* f, DFGNode* node);

    void printInterface(std::ostringstream& outputStream, std::string extraArgs = "",
        bool printBody = true, std::string backEnd = "");
    void printArguments(std::ostringstream& outputStream, bool printTypes = true);
    std::string getParamStr(int numArg);
    void printSetNewInputsFunctionDecl(
        std::ostringstream& tempStream, omp2cd_space::FunctionTable_t functionTable);
    void printSetNewInputsFunction(
        std::ostringstream& tempStream, omp2cd_space::FunctionTable_t functionTable);
    bool isOMPDARTSFunction(omp2cd_space::FunctionTable_t functionTable);

    void printFunctionInputsAndVarsDefs(
        std::ostringstream& tempStream, omp2cd_space::FunctionTable_t functionTable);
    void printFunctionInputsAndVarsInits(
        std::ostringstream& tempStream, omp2cd_space::FunctionTable_t functionTable);
    void printFunctionInputsAndVarsInitsInConstructor(
        std::ostringstream& tempStream, omp2cd_space::FunctionTable_t functionTable);
    void printFunctionInputsAndVarsDelete(
        std::ostringstream& tempStream, omp2cd_space::FunctionTable_t functionTable);

    void printPassFuncParams_swarm(std::ostringstream& outputStream, DFGNode* srcNode,
        std::string postfixStr, clang::PrintingPolicy& Policy, clang::CallExpr* callExpr);

    DFGNode* node;
    clang::FunctionDecl* f;
    bool isOmp2dartsFunction;
    std::vector<std::string> allocaVars;
    int numOfArgs;
    std::string name;
    std::string returnType;
    VarTable2_t varTable2;

    clang::FunctionDecl* getFunctionDecl() { return this->f; }
};
}
#endif
