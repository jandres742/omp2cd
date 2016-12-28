#include "Allocator.h"

using namespace darts;

bool Allocator::handles(typing::ResourceType type)
{
    unsigned int handles = getTypeOfMyHandle();

    if (handles % type == 0)
        return true;
    else
        return false;
}
