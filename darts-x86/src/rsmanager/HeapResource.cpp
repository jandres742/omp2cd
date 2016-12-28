#include "HeapResource.h"

using namespace darts;

#ifndef _MSC_VER
const typing::ResourceType HeapResource::classType;
#endif

HeapResource::HeapResource(MasterResource& masterToSet,
    Allocator& allocatorToSet, int sizeInBytes,
    ByteT* pointerToAssign)
    : MemoryResource(masterToSet, allocatorToSet, sizeInBytes, pointerToAssign)
{
}

HeapResource::~HeapResource(void)
{
}

typing::ResourceType
HeapResource::getType(void)
{
    return classType;
}

unsigned int
HeapResource::getInherit(void)
{
    return classInherit;
}
