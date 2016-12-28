#include "Resource.h"
#include <utility>

using namespace darts;

#ifndef _MSC_VER
const typing::ResourceType Resource::classType;
#endif

Resource::Resource(MasterResource& masterToSet,
    Allocator& allocatorToSet)
    : ourMaster__(&masterToSet)
    , ourAllocator__(&allocatorToSet)
{
}

Resource::Resource()
{
}

bool Resource::instanceOf(typing::ResourceType typeCheck)
{
    unsigned int inherit = getInherit();

    if (inherit % typeCheck == 0)
        return true;
    else
        return false;
}

bool Resource::isType(typing::ResourceType typeCheck)
{
    typing::ResourceType type = getType();

    if (type == typeCheck)
        return true;
    else
        return false;
}

bool Resource::isValid(void)
{
    typing::ResourceType type = getType();
    typing::ResourceType typeCheck = typing::NullResource;

    if (type != typeCheck)
        return true;
    else
        return false;
}

IdT Resource::getId(void)
{
    return id__;
}

IdT Resource::getOwner(void)
{
    return owner__;
}

Allocator&
Resource::getAllocator(void)
{
    return *ourAllocator__;
}

void Resource::changeOwner(IdT ownerToSet)
{
    owner__ = ownerToSet;
}
