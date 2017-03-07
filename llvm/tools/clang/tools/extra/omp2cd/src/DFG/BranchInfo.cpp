#include "include/DFG/BranchInfo.h"
#include "include/DFG/DFG.h"
#include "include/DFG/DFGNode.h"
#include "include/DFG/LoopInfo.h"
#include "include/DFG/TPRegion.h"

using namespace llvm;
using namespace clang;
using namespace clang::tooling;
using namespace std;

namespace omp2cd_space {

BranchInfo::BranchInfo()
    : ifNode(nullptr)
    , condNode(nullptr)
    , ifBodyNode(nullptr)
    , elseBodyNode(nullptr)
    , ifBodyRegion(nullptr)
    , elseBodyRegion(nullptr)
    , isSimpleIf(true)
    , printIfRegion(false)
    , inlineBranchCode(false)
{
}

BranchInfo::BranchInfo(DFGNode* in_ifNode)
    : ifNode(in_ifNode)
    , condNode(nullptr)
    , ifBodyNode(nullptr)
    , elseBodyNode(nullptr)
    , ifBodyRegion(nullptr)
    , elseBodyRegion(nullptr)
    , isSimpleIf(true)
    , printIfRegion(false)
    , inlineBranchCode(false)
{
}

void BranchInfo::printConditionalBranch(std::ostringstream& outputStream)
{
    outputStream << "/*Printing conditional branch node " << this->ifNode->getID()
                 << ": inlining: " << this->inlineBranchCode << "*/\n";

    outputStream << "if(" << this->conditionString << ")\n"
                 << "{\n";

    if (this->inlineBranchCode) {
        this->ifBodyRegion->printRegionFireCode(outputStream);
    } else {
        if (this->ifBodyRegion->parent && this->ifBodyRegion->parent->singleThreaded) {
            outputStream << "myTP->checkInCodelets" << this->ifBodyRegion->getID() << ".decDep();";
        } else {
            outputStream << "myTP->checkInCodelets" << this->ifBodyRegion->getID()
                         << "[this->getID()].decDep();";
        }
    }

    outputStream << "}\n";

    TPRegion* elseRegion = this->elseBodyRegion;

    /*If there are no else region, print one signaling the
    next region to avoid deadlocks.*/
    if (ifNode && elseRegion == nullptr) {
        outputStream << "else {\n";
        outputStream << "/*Signaling the region after the if stmt*/\n";
        this->ifNode->signalNext(outputStream);
        outputStream << "}\n";
        return;
    }

    while (elseRegion) {

        outputStream << "else ";

        if (elseRegion->getBranchInfo() && elseRegion->getBranchInfo()->ifBodyRegion) {

            outputStream << "if(" << elseRegion->getBranchInfo()->conditionString << ")\n"
                         << "{\n";

            TPRegion* bodyRegion = elseRegion->getBranchInfo()->ifBodyRegion;

            if (this->inlineBranchCode) {
                bodyRegion->printRegionFireCode(outputStream);
            } else {
                if (bodyRegion->parent && bodyRegion->parent->singleThreaded) {
                    outputStream << "myTP->checkInCodelets" << bodyRegion->getID() << ".decDep();";
                } else {
                    outputStream << "myTP->checkInCodelets" << bodyRegion->getID()
                                 << "[this->getID()].decDep();";
                }
            }
            outputStream << "}\n";
        } else {
            outputStream << "{\n";

            outputStream << "/*elseRegion: " << elseRegion->getName() << " " << elseRegion->getID()
                         << "*/\n";

            TPRegion* bodyRegion = elseRegion;

            if (this->inlineBranchCode) {
                bodyRegion->printRegionFireCode(outputStream);
            } else {
                if (bodyRegion->parent && bodyRegion->parent->singleThreaded) {
                    outputStream << "myTP->checkInCodelets" << bodyRegion->getID() << ".decDep();";
                } else {
                    outputStream << "myTP->checkInCodelets" << bodyRegion->getID()
                                 << "[this->getID()].decDep();";
                }
            }
            outputStream << "}\n";
        }
        if (elseRegion->getBranchInfo() && elseRegion->getBranchInfo()->elseBodyRegion) {
            elseRegion = elseRegion->getBranchInfo()->elseBodyRegion;
        } else {
            elseRegion = nullptr;
        }
    }
}

void BranchInfo::setBranchInfoNodes()
{
    TPRegion* ifRegion = this->ifNode->getRegion();

    /*inline the branch code if it does not contain omp regions*/
    bool ifNodeIsComplex = false;
    if (this->ifNode)
        ifNodeIsComplex = this->ifNode->containsOMPDirectivesOrCalls();

    if (ifNodeIsComplex != !this->isSimpleIf)
        abort();

    if (this->isSimpleIf)
        this->inlineBranchCode = true;

    /* an if node should have at most two child nodes, the if branch and the else branch.
    nested if stmts are handled as if-else(if-else)  */
    if (this->ifBodyNode) {
        /* if body */
        DFGNode* firstNode = this->ifBodyNode;

        /* consider an if branch within a compound stmt */
        while (firstNode && check_isa<CompoundStmt>(firstNode->getStmt()))
            firstNode = firstNode->myNodes.front();

        /*Consider an if branch with a complex loop*/
        if (firstNode->getLoopInfo() && firstNode->isComplexStmt)
            firstNode = firstNode->getLoopInfo()->initNode;

        this->ifBodyRegion = firstNode->getRegion();

        if (this->inlineBranchCode)
            this->ifBodyRegion->prevRegion = ifRegion->prevRegion;
        else
            this->ifBodyRegion->prevRegion = ifRegion;

        DFGNode* tempBackNode = firstNode;
        while (tempBackNode->next)
            tempBackNode = tempBackNode->next;
        tempBackNode->getRegion()->nextRegion = ifRegion->nextRegion;

        /* else body */
        if (this->elseBodyNode) {
            DFGNode* lastNode = this->elseBodyNode;

            /* consider an if branch within a compound stmt */
            while (lastNode && check_isa<CompoundStmt>(lastNode->getStmt()))
                lastNode = lastNode->myNodes.front();

            /*Consider an if branch with a complex loop*/
            if (lastNode->getLoopInfo() && lastNode->isComplexStmt)
                lastNode = lastNode->getLoopInfo()->initNode;

            this->elseBodyRegion = lastNode->getRegion();

            if (this->inlineBranchCode)
                this->elseBodyRegion->prevRegion = ifRegion->prevRegion;
            else
                this->elseBodyRegion->prevRegion = ifRegion;

            DFGNode* tempBackNode = lastNode;
            while (tempBackNode->next)
                tempBackNode = tempBackNode->next;
            tempBackNode->getRegion()->nextRegion = ifRegion->nextRegion;
        }
    }
    if (inlineBranchCode) {
        if (this->ifBodyRegion)
            this->ifBodyRegion->doNotPrint = true;
        if (this->elseBodyRegion)
            this->elseBodyRegion->doNotPrint = true;
    }
}
}
