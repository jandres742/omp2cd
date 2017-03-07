#include "include/DFG/DFG.h"
#include "include/DFG/DFGNode.h"
#include "include/DFG/LoopInfo.h"
#include "include/DFG/TPRegion.h"

using namespace llvm;
using namespace clang;
using namespace clang::tooling;
using namespace std;

namespace omp2cd_space {

void TPRegion::printTPLoopCodeletClass(std::ostringstream& outputStream)
{
    TPRegion* parentRegion = this->parent;
    if (parentRegion == nullptr)
        parentRegion = this;

    int myTPID = parentRegion->getID();
    outputStream << "class _checkInCodelets" << this->getID() << ":"
                 << "public darts::Codelet\n"
                 << "{\n"
                 << "public:\n"
                 << "TP" << myTPID << "* myTP;\n";

    parentRegion->getMainNode()->printOMPInputAndVarDefsRaw(outputStream);

    outputStream << "_checkInCodelets" << this->getID() << "():\n"
                 << "darts::Codelet(){ }\n"
                 << "_checkInCodelets" << this->getID()
                 << "(uint32_t dep, uint32_t res, ThreadedProcedure* myTP";

    parentRegion->getMainNode()->printOMPInputAsParamsRaw(true, outputStream);

    outputStream << "):\n"
                 << "darts::Codelet(dep,res,myTP,LONGWAIT"
                 << ")";

    parentRegion->getMainNode()->printOMPInputAndVarInitsRaw(outputStream);

    outputStream << "{ }\n"
                 << "void fire(void);\n"
                 << "};\n";
}

void TPRegion::printTPLoopCodeletCoreClass(std::ostringstream& outputStream)
{
    TPRegion* parentRegion = this->parent;
    if (parentRegion == nullptr)
        parentRegion = this;

    string loopVarType = this->getLoopInfo()->loopVarType;

    outputStream << "class _checkInCodelets" << this->getID() << "_core:"
                 << "public darts::Codelet\n"
                 << "{\n"
                 << "public:\n"
                 << "Codelet *toSignal;\n"
                 << loopVarType << " initIteration;\n"
                 << loopVarType << " lastIteration;\n";

    this->getMainNode()->printOMPInputAndVarDefsRaw(outputStream);

    int myTPID = parentRegion->getID();

    outputStream << "_checkInCodelets" << this->getID() << "_core():\n"
                 << "darts::Codelet(){ }\n"
                 << "_checkInCodelets" << this->getID() << "_core"
                 << "(uint32_t dep, uint32_t res, TP" << myTPID << "* myTP,"
                 << "darts::Codelet * in_nextCodelet," << loopVarType << " in_initIteration,"
                 << loopVarType << " in_lastIteration";

    this->getMainNode()->printOMPInputAsParamsRaw(true, outputStream);

    outputStream << "):\n"
                 << "darts::Codelet(dep,res,myTP,LONGWAIT),"
                 << "toSignal(in_nextCodelet),"
                 << "initIteration(in_initIteration),"
                 << "lastIteration(in_lastIteration)";

    this->getMainNode()->printOMPInputAndVarInitsRaw(outputStream);

    outputStream << "{ }\n"
                 << "void fire(void);\n"
                 << "};\n";
}

void TPRegion::printTPLoopCodeletCoreFire(std::ostringstream& outputStream)
{
    TPRegion* parentRegion = this->getParent();
    int myTPID = parentRegion->getID();
    string loopVarType = this->getLoopInfo()->loopVarType;

    outputStream << "void TP" << myTPID << "::_checkInCodelets" << this->getID()
                 << "_core::fire(void)\n"
                 << "{\n"
                 << "TP" << myTPID << "* myTP = static_cast<TP" << myTPID << "*>(myTP_);\n"
                 << "(void)myTP;\n"
                 << loopVarType << " startRange = "
                 << "(this->initIteration);\n"
                 << loopVarType << " endRange = "
                 << "(this->lastIteration);\n";

    if (this->childRegions.size()) {
        TPRegion* childRegion = this->childRegions.front();
        if (childRegion && childRegion->myNodes.size() > 0) {
            DFGNode* node = childRegion->myNodes[0];
            if (node) {
                node->printNodeFireCode(outputStream);
            }
        }
    }
    outputStream << "this->toSignal->decDep();\n"
                 << "}\n";
}

/*This is the fire method for the codelet parallel for */
void TPRegion::printTPLoopCodeletForFire(std::ostringstream& outputStream)
{
    TPRegion* parentRegion = this->parent;
    int myTPID = parentRegion->getID();
    string loopVarType = parentRegion->getLoopInfo()->loopVarType;
    outputStream << "void TP" << myTPID << "_loop::_checkInCodelets" << this->getID()
                 << "::fire(void)\n"
                 << "{\n"
                 << "TP" << myTPID << "_loop* myTP = "
                 << "static_cast<TP" << myTPID << "_loop*>(myTP_);\n"
                 << "(void)myTP;\n"
                 << loopVarType << " startRange = "
                 << "(myTP->iter) * (myTP->tileSize) + (myTP->initIteration);\n"
                 << loopVarType << " endRange = "
                 << "min( (" << loopVarType
                 << ")(startRange + (myTP->tileSize)), (int64_t)(myTP->lastIteration));\n";

    if (this->myNodes.size() > 0) {
        DFGNode* node = this->myNodes[0];
        if (node)
            node->printNodeFireCode(outputStream);
    }

    outputStream << "myTP->toSignal->decDep();\n"
                 << "}\n";

    /*This is the parallel for */
    outputStream << "void TP" << myTPID << "_outer::TP" << myTPID << "_inner::"
                 << "_checkInCodelets" << this->getID() << "::fire(void)\n"
                 << "{\n"
                 << "TP" << myTPID << "_outer::TP" << myTPID << "_inner* myTP = "
                 << "static_cast<TP" << myTPID << "_outer::TP" << myTPID << "_inner*>(myTP_);\n"
                 << "(void)myTP;\n";

    outputStream
        << loopVarType << " startRange = "
        << "myTP->outerIteration * myTP->outerTile + myTP->innerIteration * myTP->innerTile "
        << "+ myTP->initIteration;\n"
        << loopVarType << " endRange = "
        << "min( min(startRange + myTP->innerTile, myTP->lastIteration ), "
        << "myTP->initIteration + (myTP->outerIteration+1) * myTP->outerTile );\n";

    if (this->myNodes.size() > 0) {
        DFGNode* node = this->myNodes[0];
        if (node) {
            node->printNodeFireCode(outputStream);
        }
    }
    outputStream << "myTP->toSignal->decDep();\n"
                 << "}\n";
    return;
}
}