#ifndef _LOOPINCNODE_H_
#define _LOOPINCNODE_H_

#include "include/DFG/DFG.h"
#include "include/DFG/DFGNode.h"

namespace omp2cd_space {

class LoopIncNode : public DFGNode {
public:
    LoopIncNode(clang::Stmt* s, DFGNode* parent, int id)
        : DFGNode(s, parent, id)
        , loopCondNode(nullptr)
        , loopNextNode(nullptr)
    {
    }

    DFGNode* loopCondNode;
    DFGNode* loopNextNode;
};
}
#endif
