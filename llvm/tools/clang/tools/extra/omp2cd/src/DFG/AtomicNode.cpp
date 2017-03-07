#include "include/DFG/AtomicNode.h"

using namespace llvm;
using namespace clang;
using namespace clang::tooling;
using namespace std;

namespace omp2cd_space {

bool AtomicNode::classof(const DFGNode* node) { return node->getKind() == AtomicNode_k; }

void AtomicNode::printCode(
    clang::ASTContext* context, clang::PrintingPolicy& Policy, ostringstream& outputStream)
{
    if (this->ompAtomicParentNode && this->ompAtomicParentNode->getOMPAtomicStmt()) {
        if (this->ompAtomicParentNode->getOMPClause().compare("update") == 0) {
            string xStr = "";
            Expr* x = this->ompAtomicParentNode->getOMPAtomicStmt()->getX();
            if (x) {
                string ostreamString;
                raw_string_ostream rawStringOStream(ostreamString);
                x->printPretty(rawStringOStream, nullptr, Policy);
                xStr = rawStringOStream.str();
            }

            string vStr = "";
            Expr* v = this->ompAtomicParentNode->getOMPAtomicStmt()->getV();
            if (v) {
                string ostreamString;
                raw_string_ostream rawStringOStream(ostreamString);
                v->printPretty(rawStringOStream, nullptr, Policy);
                vStr = rawStringOStream.str();
            }

            string exprStr = "";
            Expr* expr = this->ompAtomicParentNode->getOMPAtomicStmt()->getExpr();
            if (expr) {
                string ostreamString;
                raw_string_ostream rawStringOStream(ostreamString);
                expr->printPretty(rawStringOStream, nullptr, Policy);
                exprStr = rawStringOStream.str();
            }
            /*x++, x--, ++x, --x*/
            if (check_isa<UnaryOperator>(this->getStmt())) {
                UnaryOperator* unaryOp = dyn_cast<UnaryOperator>(this->getStmt());

                if (unaryOp->isIncrementOp()) {
                    if (unaryOp->isPrefix()) {
                        /*++x*/
                        outputStream << "__sync_add_and_fetch(&(" << xStr << "), 1);\n";
                    } else if (unaryOp->isPostfix()) {
                        /*x++*/
                        outputStream << "__sync_fetch_and_add(&(" << xStr << "), 1);\n";
                    }
                } else if (unaryOp->isDecrementOp()) {
                    if (unaryOp->isPrefix()) {
                        /*--x*/
                        outputStream << "__sync_sub_and_fetch(&(" << xStr << "), 1);\n";
                    } else if (unaryOp->isPostfix()) {
                        /*x++*/
                        outputStream << "__sync_fetch_and_sub(&(" << xStr << "), 1);\n";
                    }
                }
            }
            /*x binop= expr*/
            else if (check_isa<CompoundAssignOperator>(this->getStmt())) {
                CompoundAssignOperator* compAssignOp
                    = dyn_cast<CompoundAssignOperator>(this->getStmt());
                clang::BinaryOperatorKind opcode = compAssignOp->getOpcode();

                /*x += expr*/
                if (opcode == BO_AddAssign) {
                    outputStream << "__sync_fetch_and_add(&(" << xStr << "), " << exprStr << ");\n";
                }
                /*x -= expr*/
                else if (opcode == BO_SubAssign) {
                    outputStream << "__sync_fetch_and_sub(&(" << xStr << "), " << exprStr << ");\n";
                }
            }
        }
    }
}
}
