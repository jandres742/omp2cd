#ifndef _LOOPCONDNODE_H_
#define _LOOPCONDNODE_H_

#include "include/DFG/DFG.h"
#include "include/DFG/DFGNode.h"

namespace omp2cd_space {

/*Class containing the condition of a loop.*/

/*TODO: Check if we can implement it in another way*/

class LoopCondNode : public DFGNode {
public:
    LoopCondNode(clang::Stmt* s, DFGNode* parent, int id, DFGNodeKind k = LoopCondNode_k)
        : DFGNode(s, parent, id, k)
        , loopFirstNode(nullptr)
        , loopNextNode(nullptr)
        , endCondLoopNode(nullptr)
        , loopStmtNode(parent)
    {
    }

    LoopCondNode(LoopCondNode* originalNode, int id, DFGNodeKind k = LoopCondNode_k)
        : DFGNode(originalNode, id, k)
        , loopFirstNode(originalNode->loopFirstNode)
        , loopNextNode(originalNode->loopNextNode)
        , endCondLoopNode(originalNode->endCondLoopNode)
        , loopStmtNode(originalNode->loopStmtNode)
    {
    }

    DFGNode* loopFirstNode; /* first node to execute in the loop */
    DFGNode* loopNextNode; /* the next node after the loop */
    DFGNode* endCondLoopNode; /* in a for loop, is the pointer to the node evaluating
     the condition at the end of the loop */
    DFGNode* loopStmtNode;

    bool printJump(std::ostringstream& outputStream);
    static bool classof(const DFGNode* node);
};
}
#endif
