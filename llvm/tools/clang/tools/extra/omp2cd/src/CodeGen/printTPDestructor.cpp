#include "include/Compiler/ProcessVisitor.h"
#include "include/DFG/TPRegion.h"

using namespace llvm;
using namespace clang;
using namespace clang::tooling;
using namespace std;

namespace omp2cd_space {

void ompASTVisitor::printTPDestructor(TPRegion* region, std::ostringstream& TPStream)
{
    if (region->isOMPForOnlyChildRegion()) {

        TPStream << "TP" << region->getID() << "_loop::~TP" << region->getID() << "_loop()"
                 << "{\n";

        this->printRegionCodeletsDelete(region, TPStream);
        TPStream << "}\n";

        TPStream << "TP" << region->getID() << "_outer::TP" << region->getID() << "_inner::~TP"
                 << region->getID() << "_inner()\n"
                 << "{\n";

        this->printRegionCodeletsDelete(region, TPStream);

        TPStream << "}\n"
                 << "TP" << region->getID() << "_outer::~TP" << region->getID() << "_outer()\n"
                 << "{\n"
                 << "delete masterCodeletTP" << region->getID() << "_inner;\n"
                 << "}\n";

    } else {

        TPStream << "TP" << region->getID() << "::~TP" << region->getID() << "()"
                 << "{\n";

        this->printRegionExtraVarsFree(region, TPStream);

        if (check_isa<OMPExecutableDirective>(region->getStmt()))
            region->mainNode->printTaskDataFree(TPStream);

        for (TPRegion* inlinedChildRegion : region->inlinedRegions)
            this->printRegionExtraVarsFree(inlinedChildRegion, TPStream);

        if ((region->isLoopRegion() && region->mainNode->isComplexStmt) == false) {

            region->mainNode->printOMPInputAndVarFreeInDestructor(TPStream);
            /* inline functions */
            for (TPRegion* inlinedChildRegion : region->inlinedRegions)
                inlinedChildRegion->mainNode->printOMPInputAndVarFreeInDestructor(TPStream);
        }
        this->printRegionCodeletsDelete(region, TPStream);

        TPStream << "}\n";
    }
}
}
