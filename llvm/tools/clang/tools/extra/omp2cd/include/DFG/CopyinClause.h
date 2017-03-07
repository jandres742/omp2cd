#ifndef _COPYINCLAUSE_H_
#define _COPYINCLAUSE_H_

#include "include/DFG/DFG.h"

namespace omp2cd_space {

class DFGNode;

class CopyinClause {
public:
    CopyinClause(DFGNode* in_ompNode)
        : ompNode(in_ompNode)
    {
    }

    DFGNode* ompNode;
    CopyinVars_t vars;

    void initVarsInConstructor(std::string& outputStr);
};
}
#endif
