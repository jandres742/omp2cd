#ifndef _ATOMICNODE_H_
#define _ATOMICNODE_H_

#include "include/DFG/DFG.h"
#include "include/DFG/DFGNode.h"

namespace omp2cd_space {

/*Class implementing an omp atomic node.*/
class AtomicNode : public DFGNode {
public:
    AtomicNode(clang::Stmt* s, DFGNode* parent, int id, DFGNodeKind k = AtomicNode_k)
        : DFGNode(s, parent, id, k)
        , ompAtomicParentNode(nullptr)
        , ompClause("update")
    {
    }

    AtomicNode(AtomicNode* originalNode, int id, DFGNodeKind k = AtomicNode_k)
        : DFGNode(originalNode, id, k)
        , ompAtomicParentNode(originalNode->ompAtomicParentNode)
        , ompClause(originalNode->ompClause)
    {
    }

public:
    AtomicNode* ompAtomicParentNode;
    std::string ompClause;

    clang::OMPAtomicDirective* getOMPAtomicStmt()
    {
        clang::OMPAtomicDirective* ompAtomicStmt = nullptr;
        if (check_isa<clang::OMPAtomicDirective>(this->s)) {
            ompAtomicStmt = clang::dyn_cast<clang::OMPAtomicDirective>(this->s);
        }
        return ompAtomicStmt;
    }

    std::string getOMPClause() { return this->ompClause; }

    void printCode(clang::ASTContext* context, clang::PrintingPolicy& Policy,
        std::ostringstream& outputStream);
    static bool classof(const DFGNode* node);
};
}
#endif
