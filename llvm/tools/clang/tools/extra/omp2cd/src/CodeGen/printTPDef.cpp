#include "include/Compiler/ProcessVisitor.h"
#include "include/DFG/LoopInfo.h"
#include "include/DFG/TPRegion.h"
#include "include/DFG/TaskInfo.h"

using namespace llvm;
using namespace clang;
using namespace clang::tooling;
using namespace std;

namespace omp2cd_space {

void ompASTVisitor::printTPDef(TPRegion* region, std::ostringstream& TPStream)
{

    if (DARTS_BACKEND)
        TPStream << "/*TP" << region->getID() << ": " << region->getName() << "*/\n";
    else if (SWARM_BACKEND)
        TPStream << "/*Codelet" << region->getID() << ": " << region->getName() << "*/\n";

    if (region->isOMPForOnlyChildRegion() && region->getLoopInfo()) {
        this->printTPDefTPLoop(region, TPStream);
        return;
    }

    if (DARTS_BACKEND) {
        if (region->isOMPParallel()) {

            TPStream << "class TP" << region->getID() << " : public darts::ThreadedProcedure\n";

        } else {

            TPStream << "class TP" << region->getID() << " : public omp";

            if (region->name.find("Directive") != string::npos
                && region->isBalancedOMPFor() == false)
                TPStream << region->name;

            TPStream << "TP\n";
        }

        TPStream << "{\n public:\n";
    }

    /*print any functions the inlined regions need*/
    for (TPRegion* inlinedChildRegion : region->inlinedRegions) {
        /*if the inlined region is an ompfor, print the iterationrequest function implementation*/
        if (inlinedChildRegion->isOMPFor()) {
            inlinedChildRegion->getLoopInfo()->printRequestIterationsFunctionDef(
                inlinedChildRegion->getMainNode(), TPStream);
        }
    }

    /*Print the codelet's classes */
    this->printRegionCodeletsClass(region, TPStream);

    if (region->isOMPParallel()) {
        if (DARTS_BACKEND) {
            TPStream << "darts::Codelet* nextCodelet;\n";
        } else if (SWARM_BACKEND) {
            TPStream << "const swarm_Codelet_t* nextCodelets" << region->getID() << ";\n"
                     << "void *nextCodeletContext" << region->getID() << ";\n";
        }
    }

    if (region->isOMPForOnlyChildRegion() == false) {

        if (DARTS_BACKEND) {
            /*Pointer to the parent */

            if (region->isOMPParallel()) {
                TPStream << "TP" << region->getID() << " * TPParent;\n";
            } else if (region->parent) {
                TPStream << "TP" << region->parent->getID() << " * TPParent;\n";
            }

            /*Pointer to the TP that controls this TP (i.e. the one who spawns this TP, who has the
             * barriers for the regions
             * inside this TP, etc).*/
            if (check_isa<OMPExecutableDirective>(region->getStmt())
                || (region->isLoopRegion() && region->mainNode->isComplexStmt)) {
                TPStream << "TP" << region->getID() << " * controlTPParent;\n";
            } else if (region->parent) {
                TPStream << "TP" << region->parent->getID() << " * controlTPParent;\n";
            }

            /*Pointer to the TP where the inputs reside */
            TPRegion* inputsRegion = region->findOMPOrFunctionRegionParent();
            if (inputsRegion)
                TPStream << "TP" << inputsRegion->getID() << "* inputsTPParent;";

            if (region->childRegions.size() == 0)
                TPStream << "TP" << region->getID() << "** ptrToThisTP;\n";

            if (region->isLoopRegion() && region->mainNode->isComplexStmt)
                TPStream << "TP" << region->getID() << "** ptrToThisTP;\n";
        }

        /*Print the input and variables for this region */
        if (check_isa<OMPExecutableDirective>(region->getStmt())) {
            region->mainNode->printOMPInputAndVarDefs(TPStream);

            /*Print the inputs and vars for any inlined regions */
            for (TPRegion* inlinedChildRegion : region->inlinedRegions)
                inlinedChildRegion->mainNode->printOMPInputAndVarDefs(TPStream);
        }

        /*Print the variables needed for control */
        this->printRegionExtraVarsDefs(region, TPStream);

        if (check_isa<OMPExecutableDirective>(region->getStmt()))
            region->mainNode->printTaskDataDefs(TPStream);

        region->mainNode->printParallelReductionVarsDef(TPStream);

        /*Print the variables needed for control of the inlined regions */
        for (TPRegion* inlinedChildRegion : region->inlinedRegions)
            this->printRegionExtraVarsDefs(inlinedChildRegion, TPStream);
    }

    /*Print the defs of the codelets used on this region */
    this->printRegionCodeletsDefs(region, TPStream);

    if (DARTS_BACKEND) {
        /*Constructor Defintion */
        if (region->isOMPParallel()) {
            TPStream << "TP" << region->getID()
                     << "(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet";

            if (check_isa<OMPParallelForDirective>(region->getStmt())
                || check_isa<OMPForDirective>(region->getStmt()))
                TPStream << ", " << region->getLoopInfo()->loopVarType << " in_initIteration, "
                         << region->getLoopInfo()->loopVarType << " in_lastIteration";

            region->mainNode->printOMPInputAsParams(true, TPStream);
            TPStream << ");\n";
        } else {
            TPStream << "TP" << region->getID() << "(int in_numThreads, int in_mainCodeletID";
            if (region->parent)
                TPStream << ", TP" << region->parent->getID() << "* in_TPParent";

            TPRegion* inputsRegion = region->findOMPOrFunctionRegionParent();
            if (inputsRegion && !(check_isa<OMPExecutableDirective>(region->getStmt())))
                TPStream << ", TP" << inputsRegion->getID() << "* in_inputsTPParent";

            if (check_isa<OMPParallelForDirective>(region->getStmt())
                || check_isa<OMPForDirective>(region->getStmt()))
                TPStream << ", " << region->getLoopInfo()->loopVarType << " in_initIteration, "
                         << region->getLoopInfo()->loopVarType << " in_lastIteration";

            if (check_isa<OMPParallelDirective>(region->getStmt())
                || check_isa<OMPParallelForDirective>(region->getStmt()))
                TPStream << ", bool in_firstTPLaunched";

            if (check_isa<OMPExecutableDirective>(region->getStmt()))
                region->mainNode->printOMPInputAsParams(true, TPStream);

            if (region->childRegions.size() == 0)
                TPStream << ", TP" << region->getID() << "** in_ptrToThisTP";

            if (region->isLoopRegion() && region->mainNode->isComplexStmt)
                TPStream << ", TP" << region->getID() << "** in_ptrToThisTP";

            if (check_isa<OMPExecutableDirective>(region->getStmt())
                && region->isSingleThreaded() == false
                && !(check_isa<OMPParallelDirective>(region->getStmt())
                       || check_isa<OMPParallelForDirective>(region->getStmt())
                       || check_isa<OMPTaskDirective>(region->getStmt())))
                TPStream << ", TP" << region->getID() << "** in_ptrToThisTP";

            TPStream << ");\n";
        }

        /*interface for a codelet from another TP to signal the corresponding codelet
         * on this TP*/
        if (region->isBalancedOMPFor() && region->isOMPForOnlyChildRegion() == false)
            TPStream << "void inline dispatchCodelet(size_t codeletID);\n";

        /* destructor */
        TPStream << "~TP" << region->getID() << "();\n";

        TPStream << "};\n";
    }
}

void ompASTVisitor::printTPDefTPLoop(TPRegion* region, std::ostringstream& TPStream)
{
    /*This is the Codelet Parallel For */
    TPStream << "class TP" << region->getID() << "_loop:public darts::loop{\n"
             << "public:\n";

    this->printRegionCodeletsClass(region, TPStream);

    TPStream << "unsigned int tileSize;\n"
             << region->getLoopInfo()->loopVarType << " initIteration;\n"
             << region->getLoopInfo()->loopVarType << " lastIteration;\n";

    this->printRegionCodeletsDefs(region, TPStream);

    TPStream << "TP" << region->getID() << "_loop"
             << "(unsigned int it, darts::Codelet * in_nextCodelet, unsigned int in_tileSize, "
             << region->getLoopInfo()->loopVarType << " in_initIteration, "
             << region->getLoopInfo()->loopVarType << " in_lastIteration";

    region->mainNode->printOMPInputAsParams(true, TPStream);

    TPStream << ");\n"
             << "~TP" << region->getID() << "_loop();\n"
             << "};\n";

    /*This is the Parallel For */
    TPStream << "class TP" << region->getID() << "_outer:public darts::loop{\n"
             << "public:\n";

    /*inner */
    TPStream << "class TP" << region->getID() << "_inner:public darts::loop{\n"
             << "public:\n";

    this->printRegionCodeletsClass(region, TPStream);

    TPStream << "unsigned int outerIteration;\n"
             << "unsigned int innerIteration;\n"
             << region->getLoopInfo()->loopVarType << " outerTile;\n"
             << region->getLoopInfo()->loopVarType << " innerTile;\n"
             << region->getLoopInfo()->loopVarType << " initIteration;\n"
             << region->getLoopInfo()->loopVarType << " lastIteration;\n";

    this->printRegionCodeletsDefs(region, TPStream);

    TPStream << "TP" << region->getID() << "_inner"
             << "(unsigned int it, darts::Codelet * in_nextCodelet, "
             << "unsigned int in_outerIteration," << region->getLoopInfo()->loopVarType
             << " in_outerTile, " << region->getLoopInfo()->loopVarType << " in_innerTile, "
             << region->getLoopInfo()->loopVarType << " in_initIteration, "
             << region->getLoopInfo()->loopVarType << " in_lastIteration";

    region->mainNode->printOMPInputAsParams(true, TPStream);

    TPStream << ");\n"
             << "~TP" << region->getID() << "_inner();\n"
             << "};\n";

    /*outer */
    TPStream << region->getLoopInfo()->loopVarType << " outerTile;\n"
             << "unsigned int workers;\n"
             << region->getLoopInfo()->loopVarType << " initIteration;\n"
             << region->getLoopInfo()->loopVarType << " lastIteration;\n"
             << region->getLoopInfo()->loopVarType << " width;\n"
             << region->getLoopInfo()->loopVarType << " innerTile;\n"
             << "darts::codeletFor < TP" << region->getID() << "_inner > *masterCodeletTP"
             << region->getID() << "_inner;\n"
             << "TP" << region->getID() << "_outer"
             << "(unsigned int it, darts::Codelet * in_nextCodelet,"
             << "int64_t in_outerTile, unsigned int in_workers,"
             << region->getLoopInfo()->loopVarType << " in_initIteration, "
             << region->getLoopInfo()->loopVarType << " in_lastIteration";

    region->mainNode->printOMPInputAsParams(true, TPStream);

    TPStream << ");\n"
             << "~TP" << region->getID() << "_outer();\n"
             << "};\n";
}
}
