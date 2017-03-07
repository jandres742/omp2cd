#ifndef _COPYPRIVATECLAUSE_H_
#define _COPYPRIVATECLAUSE_H_

#include "include/DFG/DFG.h"

namespace omp2cd_space {

class DFGNode;

class CopyPrivateClause {
public:
    CopyPrivateClause(DFGNode* in_ompNode)
        : ompNode(in_ompNode)
    {
    }

    DFGNode* ompNode;
    CopyprivateVars_t vars;

    void copyValuesToVars(std::ostringstream& outputStream);
};
}
#endif
