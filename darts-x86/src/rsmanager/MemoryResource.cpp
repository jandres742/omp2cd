#include "MemoryResource.h"

using namespace darts;

#ifndef _MSC_VER
const typing::ResourceType MemoryResource::classType;
#endif

MemoryResource::MemoryResource(MasterResource& masterToSet,
    Allocator& allocatorToSet, int sizeInBytes,
    ByteT* pointerToAssign)
    : Resource(masterToSet, allocatorToSet)
    , LimitSpecifier(sizeInBytes)
    , pointer__(pointerToAssign)
{
}

MemoryResource::MemoryResource()
{
}

MemoryResource::~MemoryResource(void)
{
}

typing::ResourceType
MemoryResource::getType(void)
{
    return classType;
}

unsigned int
MemoryResource::getInherit(void)
{
    return classInherit;
}

//Generic and not so useful

std::vector<ByteT>
MemoryResource::read(void)
{
    //Read 1 byte and return it
    //TODO: Stream it
    std::vector<ByteT> byteStore;

    if (getQuantity() > 0)
        byteStore.push_back(*pointer__);

    return byteStore;
}

bool MemoryResource::write(std::vector<ByteT> const& stuffToWrite)
{
    unsigned int quantity = getQuantity();

    //Don't write past the bounds
    if (stuffToWrite.size() < quantity) {
        //Read location
        std::vector<ByteT>::const_iterator readIt;

        //Write location
        ByteT* whereToWrite = pointer__;

        //...read a byte...write a byte...move 1 byte forward...
        for (readIt = stuffToWrite.begin(); readIt != stuffToWrite.end();
             readIt++) {
            *whereToWrite = *readIt;

            ++whereToWrite;
        }

        return true;
    }

    return false;
}

//Our special interfaces

ByteT*
MemoryResource::getPointer(void)
{
    return pointer__;
}

bool MemoryResource::read(std::vector<ByteT>& stuffToStore,
    unsigned int initialOffset)
{
    unsigned int quantity = getQuantity();

    unsigned int lastReadLocation = stuffToStore.size() - 1 + initialOffset;

    //Don't read pass the boundary
    if (lastReadLocation < quantity) {
        //Store Location
        std::vector<ByteT>::iterator storeIt;

        //Read location
        ByteT* whereToRead = pointer__ + initialOffset;

        //Read a byte....store a byte...move forward a byte...
        for (storeIt = stuffToStore.begin(); storeIt != stuffToStore.end();
             storeIt++) {
            *storeIt = *whereToRead;

            ++whereToRead;
        }

        return true;
    }

    return false;
}

bool MemoryResource::write(std::vector<ByteT> const& stuffToWrite,
    unsigned int initialOffset)
{
    unsigned int quantity = getQuantity();

    unsigned int lastWriteLocation = stuffToWrite.size() - 1 + initialOffset;

    //Don't write pass the boundary
    if (lastWriteLocation < quantity) {
        //Read location
        std::vector<ByteT>::const_iterator readIt;

        //Write location
        ByteT* whereToWrite = pointer__ + initialOffset;

        //...read a byte...write a byte...move 1 byte forward..
        for (readIt = stuffToWrite.begin(); readIt != stuffToWrite.end();
             readIt++) {
            *whereToWrite = *readIt;

            ++whereToWrite;
        }

        return true;
    }

    return false;
}

bool MemoryResource::poll(void)
{
    return false;
}
