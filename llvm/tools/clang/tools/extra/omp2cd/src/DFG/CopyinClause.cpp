#include "include/DFG/CopyinClause.h"
#include "include/DFG/DFG.h"
#include "include/DFG/DFGNode.h"
#include "include/DFG/TPRegion.h"

using namespace llvm;
using namespace clang;
using namespace clang::tooling;
using namespace std;

namespace omp2cd_space {

/* Copy the value of each threadprivate var in the master thread to all
* the other threads*/
void CopyinClause::initVarsInConstructor(std::string& outputStr)
{
    std::ostringstream outputStream;

    for (auto var : this->vars) {
        /* Confirm the var in the copyin clause is threadprivate */
        /* Dont initialize threadprivate vars */
        VarTableTuple_t varTuple;
        bool found
            = this->ompNode->getDFG()->getVarTupleFromTable(this->ompNode, var.first, varTuple);
        if (found == false)
            continue;
        if (get<4>(varTuple) == false)
            continue;
        int parentNodeID = get<5>(varTuple);
        string varArray = get<6>(varTuple);
        string varSize = get<8>(varTuple);

        string posStr = "codeletCounter";
        if (this->ompNode->getRegion()->singleThreaded)
            posStr = "mainCodeletID";

        /*Scalars */
        if (varArray.compare("") == 0) {
            outputStream << "if(codeletCounter != 0)"
                         << "{\n"
                         << var.first << "_dartsThreadPriv" << parentNodeID << "[ " << posStr
                         << "] = " << var.first << "_dartsThreadPriv" << parentNodeID << "[0];\n"
                         << "}\n";
        }
        /*Arrays */
        else {
            outputStream << "if(codeletCounter != 0)"
                         << "{\n"
                         << "for (int k = 0; k < " << varSize << "; k++){\n"
                         << var.first << "_dartsThreadPriv" << parentNodeID << "[ " << posStr
                         << "][k] = " << var.first << "_dartsThreadPriv" << parentNodeID
                         << "[0][k];\n"
                         << "}\n"
                         << "}\n";
        }
    }
    outputStr += outputStream.str();
}
}
