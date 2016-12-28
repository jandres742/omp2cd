#include "LimitSpecifier.h"

using namespace darts;

#ifndef _MSC_VER
const typing::ResourceType LimitSpecifier::classType;
#endif

int LimitSpecifier::getQuantity(void) const
{
    return quantity__;
}
