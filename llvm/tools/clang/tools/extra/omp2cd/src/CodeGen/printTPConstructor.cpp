#include "include/Compiler/ProcessVisitor.h"
#include "include/DFG/CopyinClause.h"
#include "include/DFG/LoopInfo.h"
#include "include/DFG/TPRegion.h"
#include "include/DFG/TaskInfo.h"

using namespace llvm;
using namespace clang;
using namespace clang::tooling;
using namespace std;

namespace omp2cd_space {

void ompASTVisitor::printTPConstructor(TPRegion* region, std::ostringstream& TPStream)
{
    /* Special case for omp regions that fit as TP Loops */
    if (region->isOMPForOnlyChildRegion() && region->getLoopInfo()) {
        this->printTPConstructorTPLoop(region, TPStream);
        return;
    }

    if (SWARM_BACKEND) {
        TPStream << "\n/*Init of Codelet*/\nCODELET_IMPL_BEGIN_NOCANCEL(" << region->getName()
                 << region->getID() << "Entry)\n"
                 << "{\n";

        TPRegion* childRegion = region->childRegions.front();
        if (childRegion) {

            if (region->isOMPParallel()) {
                TPStream << "/*Save the return codelet and its context.*/\n"
                         << "nextCodelets" << region->getID() << " = NEXT;\n"
                         << "nextCodeletContext" << region->getID() << " = NEXT_THIS;\n";
            } else {
                TPStream << "/*Check if the parameters for this region have been already set*/\n"
                         << "if(!(__sync_val_compare_and_swap(&(Codelet" << region->getID()
                         << "Entry_alreadySet), 0, 1))){\n";
            }

            if (check_isa<OMPExecutableDirective>(region->getStmt())) {

                TPStream << "countCodelets" << region->getID() << " = 0;\n";

                if (region->isOMPParallel() == false) {

                    TPStream << "numThreads" << region->getID() << " = "
                             << "numThreads" << region->parent->getID() << ";\n";
                }
                region->mainNode->printOMPInputAndVarInits_swarm(TPStream);
                /* inline functions */
                for (TPRegion* inlinedChildRegion : region->inlinedRegions) {
                    TPStream << "/*inlinedChildRegion: " << inlinedChildRegion->getName() << "\n*/";
                    inlinedChildRegion->mainNode->printOMPInputAndVarInits_swarm(TPStream);
                }
            }

            this->printRegionExtraVarsInits(region, TPStream);

            if (check_isa<OMPForDirective>(region->getStmt())
                || check_isa<OMPParallelForDirective>(region->getStmt())) {
                TPStream << "/*Initialize the loop parameters*/\n";
                region->getLoopInfo()->printLoopParamInitialization(TPStream, region);
            }

            this->printRegionCodeletsInitsInConstructor(region, TPStream);

            if (region->isOMPParallel() == false) {
                TPStream << "Codelet" << region->getID() << "Entry_alreadySet = 2;\n";

                TPStream << "}\n";

                TPStream << "/*Check if the parameters for this function have been already set*/\n"
                         << "if(Codelet" << region->getID() << "Entry_alreadySet != 2){\n"
                         << "swarm_schedule(&CODELET(" << region->getName() << region->getID()
                         << "Entry), "
                         << "THIS, NULL, NEXT, NEXT_THIS);\n"
                         << "return;\n"
                         << "}\n";

                TPStream << "uint64_t codeletID = (uint64_t)THIS;\n";
            }

            string nameFrontCodelet = "checkInCodelets" + to_string(childRegion->getID());
            TPStream << "/*Launch the first codelet in the region*/\n";
            if (region->isOMPParallel()) {
                TPStream << "swarm_Locale_scheduleToLeaves(swarm_getRootLocale(0), numThreads"
                         << region->getID() << ", &CODELET(" << nameFrontCodelet
                         << "), THIS, NULL, NULL, NULL, swarm_Scheduler_ORDER_FIFO);\n";
            } else {
                TPStream << "swarm_schedule(&CODELET(" << nameFrontCodelet << "), "
                         << "(void*)codeletID, NULL, NULL, NULL);\n";
            }
        }
        TPStream << "}CODELET_IMPL_END/*End of Codelet*/;\n";

        return;
    }

    TPStream << "TP" << region->getID() << "::TP" << region->getID()
             << "(int in_numThreads, int in_mainCodeletID";

    if (region->isOMPParallel()) {
        TPStream << ", Codelet* in_nextCodelet";
        if (check_isa<OMPParallelForDirective>(region->getStmt())
            || check_isa<OMPForDirective>(region->getStmt()))
            TPStream << ", " << region->getLoopInfo()->loopVarType << " in_initIteration, "
                     << region->getLoopInfo()->loopVarType << " in_lastIteration";

        region->mainNode->printOMPInputAsParams(true, TPStream);
        TPStream << "):ThreadedProcedure(in_numThreads, "
                 << "in_mainCodeletID), nextCodelet(in_nextCodelet)"
                 << ", TPParent(this), controlTPParent(this)"
                 << ", inputsTPParent(this)";

    } else {
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

        TPStream << "):omp";

        if (region->name.find("Directive") != string::npos && region->isBalancedOMPFor() == false)
            TPStream << region->name;

        if (check_isa<OMPParallelDirective>(region->getStmt())
            || check_isa<OMPParallelForDirective>(region->getStmt()))
            TPStream << "TP(in_numThreads, in_mainCodeletID, "
                     << "in_firstTPLaunched)";
        else
            TPStream << "TP(in_numThreads, in_mainCodeletID)";

        if (region->parent)
            TPStream << ", TPParent(in_TPParent)";

        if (region->parent) {
            if (check_isa<OMPExecutableDirective>(region->getStmt())
                || (region->isLoopRegion() && region->mainNode->isComplexStmt)) {
                TPStream << ", controlTPParent(this)";
            } else {
                TPStream << ", controlTPParent(in_TPParent)";
            }
        }

        if (inputsRegion) {
            if ((check_isa<OMPExecutableDirective>(region->getStmt())) == false) {
                TPStream << ", inputsTPParent(in_inputsTPParent)";
            } else if (check_isa<OMPExecutableDirective>(region->getStmt())) {
                TPStream << ", inputsTPParent(this)";
            }
        }

        if (region->isLoopRegion() && region->mainNode->isComplexStmt)
            TPStream << ", ptrToThisTP(in_ptrToThisTP)";
    }

    {
        if ((region->isLoopRegion() && region->mainNode->isComplexStmt) == false) {
            region->mainNode->printOMPInputAndVarInits(TPStream);
            /* inline functions */
            for (TPRegion* inlinedChildRegion : region->inlinedRegions)
                inlinedChildRegion->mainNode->printOMPInputAndVarInits(TPStream);
        }

        this->printRegionExtraVarsInits(region, TPStream);

        if (check_isa<OMPExecutableDirective>(region->getStmt()))
            region->mainNode->printTaskDataInits(TPStream);

        /* inline functions */
        for (TPRegion* inlinedChildRegion : region->inlinedRegions)
            this->printRegionExtraVarsInits(inlinedChildRegion, TPStream);

        this->printRegionCodeletsInits(region, TPStream);

        TPStream << "{\n";

        this->printRegionExtraVarsInitsInConstructor(region, TPStream);
        /* inline functions */
        for (TPRegion* inlinedChildRegion : region->inlinedRegions)
            this->printRegionExtraVarsInitsInConstructor(inlinedChildRegion, TPStream);

        if (check_isa<OMPForDirective>(region->getStmt())
            || check_isa<OMPParallelForDirective>(region->getStmt())) {
            TPStream << "/*Initialize the loop parameters*/\n";
            region->getLoopInfo()->printLoopParamInitialization(TPStream, region);
        }

        if (check_isa<OMPExecutableDirective>(region->getStmt())) {
            TPStream << "/*Initialize inputs and vars.*/\n";
            region->mainNode->printOMPInputAndVarInitsInConstructor(
                region->initLocalThreadsLoopStr, TPStream);
            if (region->getCopyinClause()) {
                region->getCopyinClause()->initVarsInConstructor(region->initLocalThreadsLoopStr);
            }
        }

        TPStream << "/*Initialize Codelets*/\n";
        this->printRegionCodeletsInitsInConstructor(region, TPStream);

        if (!region->initGlobalThreadsLoopStr.empty())
            TPStream << "for(size_t codeletCounter = 0; "
                     << "codeletCounter < (size_t)ompNumThreads; "
                     << "codeletCounter++)\n"
                     << "{\n"
                     << region->initGlobalThreadsLoopStr << "}\n";

        if (!region->initLocalThreadsLoopStr.empty()) {
            if (region->singleThreaded == false) {
                if (region->isBalancedOMPFor() && region->isOMPForOnlyChildRegion() == false) {
                    TPStream << "for(size_t codeletCounter = 0; "
                             << "codeletCounter < (size_t)this->baseNumThreads; "
                             << "codeletCounter++)\n";
                } else {
                    TPStream << "for(size_t codeletCounter = 0; "
                             << "codeletCounter < (size_t)this->numThreads; "
                             << "codeletCounter++)\n";
                }
            }
            TPStream << "{\n" << region->initLocalThreadsLoopStr << "}\n";
        }

        /*Dispatch first mainCodelet in case this is an OMPFor to be balanced */
        if (region->isBalancedOMPFor())
            TPStream << "this->dispatchCodelet(this->getID());\n";

        if (check_isa<OMPExecutableDirective>(region->getStmt())
            && region->isSingleThreaded() == false
            && !(check_isa<OMPParallelDirective>(region->getStmt())
                   || check_isa<OMPParallelForDirective>(region->getStmt())
                   || check_isa<OMPTaskDirective>(region->getStmt())))
            TPStream << "*(in_ptrToThisTP) = this;\n";

        if (region->parent) {
            if ((region->isLoopRegion() && region->mainNode->isComplexStmt)) {
                TPStream << "*(this->ptrToThisTP) = this;\n"
                         << "#if USE_SPIN_CODELETS == 0\n"
                         << "this->firstCodelet[this->getID()].decDep();\n"
                         << "#else\n"
                         << "this->availableCodelets[this->getID()] = 1;\n"
                         << "#endif\n";
            }
        }

        TPStream << "}\n";

        if (region->isBalancedOMPFor() && region->isOMPForOnlyChildRegion() == false) {
            TPStream << "void TP" << region->getID() << "::dispatchCodelet(size_t codeletID)\n"
                     << "{\n";

            if (region->getLoopInfo()
                && (get<0>(region->getLoopInfo()->schedulingPolicy) == STATIC_SCHED
                       || get<0>(region->getLoopInfo()->schedulingPolicy) == RUNTIME_SCHED)) {
                int frontID = region->childRegions.front()->getID();
                TPStream << "int idx = codeletID / this->baseNumThreads;\n"
                         << "int localID = codeletID - this->baseNumThreads * idx;\n"
                         << "this->checkInCodelets" << frontID << "[localID].setID (codeletID);\n"
                         << "this->checkInCodelets" << frontID
                         << "[localID].setLocalID (localID);\n"
                         << "/*Check if we want to replicate codelets*/\n"
                         << "for(size_t i = 1; i < (size_t)DARTS_CODELETS_MULT; i++){\n"
                         << "#if USE_SPIN_CODELETS == 0\n"
                         << "this->checkInCodelets" << frontID
                         << "[localID + this->baseNumThreads * i] = "
                         << "_checkInCodelets" << frontID
                         << "(2, 1, this, localID + this->baseNumThreads * i);\n"
                         << "#else\n"
                         << "this->checkInCodelets" << frontID
                         << "[localID + this->baseNumThreads * i] = "
                         << "_checkInCodelets" << frontID
                         << "(1, 1, this, localID + this->baseNumThreads * i);\n"
                         << "#endif\n"
                         << " this->checkInCodelets" << frontID
                         << "[localID + this->baseNumThreads * i].setID(codeletID);\n"
                         << " this->checkInCodelets" << frontID
                         << "[localID + this->baseNumThreads * i].decDep();\n"
                         << "#if USE_SPIN_CODELETS == 0\n"
                         << "this->firstCodelet[localID + this->baseNumThreads * i].decDep();\n"
                         << "#else\n"
                         << "this->availableCodelets[localID + this->baseNumThreads * i] = 1;\n"
                         << "#endif\n"
                         << "}\n"
                         << "#if USE_SPIN_CODELETS == 0\n"
                         << "this->firstCodelet[localID].decDep();\n"
                         << "#else\n"
                         << "this->availableCodelets[localID] = 1;\n"
                         << "#endif\n";
            } else {
                TPStream << "int currentCodelet = "
                         << "__sync_fetch_and_add (&(this->readyCodelets), 1);\n"
                         << "this->checkInCodelets" << region->childRegions.front()->getID()
                         << "[currentCodelet].setID(codeletID);\n"
                         << "#if USE_SPIN_CODELETS == 0\n"
                         << "this->firstCodelet[currentCodelet].decDep();\n"
                         << "#else\n"
                         << "this->availableCodelets[currentCodelet] = 1;\n"
                         << "#endif\n";
            }
            TPStream << "}\n";
        }
    }
}

void ompASTVisitor::printTPConstructorTPLoop(TPRegion* region, std::ostringstream& TPStream)
{
    /*This is the Codelet Parallel For */
    TPStream << "TP" << region->getID() << "_loop::TP" << region->getID()
             << "_loop(unsigned int it, "
             << "darts::Codelet * in_nextCodelet, unsigned int in_tileSize,"
             << region->getLoopInfo()->loopVarType << " in_initIteration, "
             << region->getLoopInfo()->loopVarType << " in_lastIteration";

    region->mainNode->printOMPInputAsParams(true, TPStream);

    TPStream << "):"
             << "loop(it, in_nextCodelet),"
             << "tileSize(in_tileSize),"
             << "initIteration(in_initIteration),"
             << "lastIteration(in_lastIteration)";

    this->printRegionCodeletsInits(region, TPStream);

    TPStream << "{\n";

    this->printRegionCodeletsInitsInConstructor(region, TPStream);

    TPStream << "}\n";

    /*This is the Parallel For */

    /*inner */
    TPStream << "TP" << region->getID() << "_outer::TP" << region->getID() << "_inner::TP"
             << region->getID() << "_inner(unsigned int it, "
             << "darts::Codelet * in_nextCodelet, "
             << "unsigned int in_outerIteration," << region->getLoopInfo()->loopVarType
             << " in_outerTile, " << region->getLoopInfo()->loopVarType << " in_innerTile, "
             << region->getLoopInfo()->loopVarType << " in_initIteration, "
             << region->getLoopInfo()->loopVarType << " in_lastIteration";

    region->mainNode->printOMPInputAsParams(true, TPStream);

    TPStream << "):"
             << "loop(it, in_nextCodelet),"
             << "outerIteration(in_outerIteration),"
             << "innerIteration(it),"
             << "outerTile(in_outerTile),"
             << "innerTile(in_innerTile),"
             << "initIteration(in_initIteration),"
             << "lastIteration(in_lastIteration)";

    this->printRegionCodeletsInits(region, TPStream);

    TPStream << "{\n";

    this->printRegionCodeletsInitsInConstructor(region, TPStream);

    TPStream << "}\n";

    /*outer */
    TPStream << "TP" << region->getID() << "_outer::TP" << region->getID()
             << "_outer(unsigned int it, darts::Codelet * in_nextCodelet,"
             << "int64_t in_outerTile, unsigned int in_workers,"
             << region->getLoopInfo()->loopVarType << " in_initIteration, "
             << region->getLoopInfo()->loopVarType << " in_lastIteration";

    region->mainNode->printOMPInputAsParams(true, TPStream);

    TPStream << "):"
             << "loop(it, in_nextCodelet),"
             << "outerTile(in_outerTile),"
             << "workers(in_workers),"
             << "initIteration(in_initIteration),"
             << "lastIteration(in_lastIteration),"
             << "masterCodeletTP" << region->getID() << "_inner(new codeletFor < TP"
             << region->getID() << "_inner >[1])";

    TPStream << "{\n";

    TPStream << "this->width = "
             << "min( ((this->initIteration) + it * "
             << "(this->outerTile)+(this->outerTile)), "
             << "this->lastIteration ) - "
             << "(it * (this->outerTile) + this->initIteration);\n"
             << "if(this->width/(this->workers * 2))\n"
             << "this->innerTile = "
             << "(this->width/(this->workers * 2));\n"
             << "else  if(this->width/this->workers)\n"
             << "this->innerTile = "
             << "(this->width/this->workers);\n"
             << "else\n"
             << "this->innerTile = "
             << "this->width;\n"
             << "uint64_t outerIteration = "
             << "(this->width/this->innerTile) + "
             << "( (this->width % this->innerTile) ? 1 : 0 );\n"
             << "codeletFor < TP" << region->getID() << "_outer::TP" << region->getID()
             << "_inner > *tempCodeletFor =\n"
             << "new codeletFor < TP" << region->getID() << "_outer::TP" << region->getID()
             << "_inner > (1, 1, this, LONGWAIT, "
             << "in_nextCodelet, outerIteration, it,"
             << "this->outerTile, this->innerTile, "
             << "this->initIteration, this->lastIteration";

    region->mainNode->printOMPInputAndVarInitsRawAsInputs(TPStream);

    TPStream << ");\n"
             << "this->masterCodeletTP" << region->getID() << "_inner = tempCodeletFor;\n"
             << "this->masterCodeletTP" << region->getID() << "_inner[0].decDep();\n"
             << "}\n";
}
}