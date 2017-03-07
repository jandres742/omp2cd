#include "include/DFG/LoopCondNode.h"
#include "include/DFG/DFGNode.h"
#include "include/DFG/LoopInfo.h"
#include "include/DFG/TPRegion.h"

using namespace llvm;
using namespace clang;
using namespace clang::tooling;
using namespace std;

namespace omp2cd_space {

bool LoopCondNode::classof(const DFGNode* node)
{
    return node->getKind() == LoopCondNode_k;
}

bool LoopCondNode::printJump(std::ostringstream& outputStream)
{
    bool nextPrinted = false;

    if (this->loopFirstNode) {
        int idToSignal = this->loopFirstNode->getID();

        /*dont print the condition for the condition node
		at the entrance of a do loop*/
        if ((check_isa<DoStmt>(this->loopStmtNode->getStmt()) && this->endCondLoopNode) == false)
            outputStream << "if(" << this->getLoopInfo()->condStr << ")";

        outputStream << "{\n";

        /*dont reset if it's the cond at the entrance of the loop*/
        if (this->endCondLoopNode == nullptr)
            outputStream << "this->resetCodelet();\n";

		outputStream << "/*Signal the first codelet in the loop*/\n";
        outputStream << "myTP->";

        if (this->loopFirstNode->getRegion()->parent->singleThreaded) {
            if (check_isa<OMPBarrierDirective>(this->loopFirstNode->getStmt()) || check_isa<OMPTaskwaitDirective>(this->loopFirstNode->getStmt())) {
                outputStream << "barrierCodelets" << idToSignal << "[0].decDep();\n";
            } else {
                outputStream << "checkInCodelets" << idToSignal << ".decDep();\n";
            }
        } else {
            if (check_isa<OMPBarrierDirective>(this->loopFirstNode->getStmt()) || check_isa<OMPTaskwaitDirective>(this->loopFirstNode->getStmt())) {
                outputStream << "barrierCodelets" << idToSignal << "[0].decDep();\n";
            } else {
                outputStream << "checkInCodelets" << idToSignal << "[this->getID()].decDep();\n";
            }
        }
        outputStream << "return;\n";
        outputStream << "}\n";
    }

    if (this->loopNextNode && ((check_isa<DoStmt>(this->loopStmtNode->getStmt()) && this->endCondLoopNode) == false)) {
        if (this->loopFirstNode) {
            outputStream << "else";
        }
        outputStream << "{\n";

		if ((check_isa < DoStmt >(this->loopStmtNode->getStmt()) && this->endCondLoopNode) ==false) {
			if (this->endCondLoopNode){
				outputStream << "/*Signal the codelet after the loop from the end condional node.*/\n";
				this->endCondLoopNode->getRegion()->signalNext(outputStream);
			}
			else {
				outputStream << "/*Signal the codelet after the loop from the condtional node.*/\n";
				this->getRegion()->signalNext(outputStream);
			}
		}
        nextPrinted = true;

        outputStream << "return;\n";

        outputStream << "}\n";
    }

    return nextPrinted;
}
}
