#include "include/DFG/DFG.h"
#include "include/DFG/DFGNode.h"
#include "include/DFG/TPRegion.h"

using namespace llvm;
using namespace clang;
using namespace clang::tooling;
using namespace std;

namespace omp2cd_space {

size_t RegionList::size() { return this->length; }

TPRegion* RegionList::front() { return this->head; }

TPRegion* RegionList::back() { return this->tail; }

void RegionList::replace(TPRegion* oldRegion, TPRegion* newRegion)
{
    newRegion->prev = oldRegion->prev;
    if (oldRegion->prev)
        oldRegion->prev->next = newRegion;

    newRegion->next = oldRegion->next;
    if (oldRegion->next)
        oldRegion->next->prev = newRegion;

    newRegion->parent = oldRegion->parent;

    if (this->head == oldRegion)
        this->head = newRegion;

    if (this->tail == oldRegion)
        this->tail = newRegion;
}

void RegionList::insertBefore(TPRegion* region, TPRegion* newRegion)
{
    newRegion->prev = region->prev;
    if (region->prev)
        region->prev->next = newRegion;

    newRegion->next = region;

    region->prev = newRegion;

    if (this->head == region)
        this->head = newRegion;

    newRegion->parent = region->parent;

    this->length += 1;
}

void RegionList::insertAfter(TPRegion* region, TPRegion* newRegion)
{
    newRegion->next = region->next;
    if (region->next)
        region->next->prev = newRegion;

    newRegion->prev = region;

    region->next = newRegion;

    if (this->tail == region)
        this->tail = newRegion;

    newRegion->parent = region->parent;

    this->length += 1;
}

void RegionList::insert(TPRegion* newRegion)
{
    if (this->head == nullptr)
        this->head = newRegion;

    if (this->tail == nullptr) {
        this->tail = newRegion;
    } else {
        this->tail->next = newRegion;
        newRegion->prev = this->tail;
        this->tail = newRegion;
    }
    this->length += 1;
}

void RegionList::push_back(TPRegion* newRegion) { this->insert(newRegion); }

void RegionList::remove(TPRegion* region)
{
    if (region->prev)
        region->prev->next = region->next;

    if (region->next)
        region->next->prev = region->prev;

    if (region == this->head)
        this->head = region->next;

    if (region == this->tail)
        this->tail = region->prev;

    this->length -= 1;
}

void RegionList::clear()
{
    TPRegion* childRegion = this->tail;
    while (childRegion) {
        TPRegion* tempRegion = childRegion->prev;
        this->remove(childRegion);
        childRegion = tempRegion;
    }
}
}
