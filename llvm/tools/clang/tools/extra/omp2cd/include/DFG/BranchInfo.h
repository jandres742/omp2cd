#ifndef _BRANCHINFO_H_
#define _BRANCHINFO_H_

#include "include/DFG/DFG.h"

namespace omp2cd_space {
class DFG;
class DFGNode;

/*A class to store the info of a branch containing a TP invocation*/

class BranchInfo {
public:
    BranchInfo();
    BranchInfo(DFGNode* in_ifNode);

    void setBranchInfoNodes();
    void printConditionalBranch(std::ostringstream& outputStream);

    DFGNode* ifNode;
    DFGNode* condNode;
    DFGNode* ifBodyNode;
    DFGNode* elseBodyNode;
    TPRegion* ifBodyRegion;
    TPRegion* elseBodyRegion;
    bool isSimpleIf;
    bool printIfRegion;
    bool inlineBranchCode;
    std::string conditionString;
};
}
#endif
