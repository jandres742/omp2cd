#ifndef _BASEVISITOR_H_
#define _BASEVISITOR_H_

#include "include/DFG/DFG.h"
#include "include/DFG/DFGNode.h"

extern std::string outputInclude;
extern std::ofstream outputFileH;
extern std::ofstream outputFileCpp;
extern std::string outputFileNameH;
extern std::string outputFileNameCpp;
extern std::unordered_map<std::string, std::string> outputFileNames;
extern int dfgNodesCount;

namespace omp2cd_space {

/*This is the base class for the preprocess and process visitors.*/
class BaseVisitor {
public:
    explicit BaseVisitor(clang::ASTContext* context)
        : context(context)
        , currentNode(nullptr)
        , numNodes(0)
    {
        DFGNode* tempNode = new DFGNode(this->getNewNodeID(), "rootNode");
        this->currentNode = tempNode;
        this->myDFG.addRoot(tempNode);
        tempNode->myDFG = &(this->myDFG);
    }

    int getNewNodeID();
    std::string getCalleeFunctionName(clang::CallExpr* s);
    bool isTraversableStmt(clang::Stmt* s);

public:
    clang::ASTContext* context;
    DFGNode* currentNode;
    int numNodes;
    DFG myDFG;

    ~BaseVisitor() {}
};
}
#endif
