#include "HeapAllocator.h"

using namespace darts;

HeapAllocator::HeapAllocator(void)
{
}

HeapAllocator::~HeapAllocator(void)
{
}

unsigned int
HeapAllocator::getTypeOfMyHandle(void)
{
    return HeapResource::classInherit;
}


std::unique_ptr<Resource>
HeapAllocator::allocate(MasterResource& masterResource,
    const Specifier& specifier)
{
    const LimitSpecifier& limit = static_cast<const LimitSpecifier&>(specifier);

    unsigned int size = limit.getQuantity();

    //Allocate memory from the heap
    ByteT* ptr = static_cast<ByteT*>(operator new(size, std::nothrow));

    //If the allocation was successful
    if (ptr != 0) {
        //Create a resource to store the allocation size and ptr
        HeapResource* mResource = new HeapResource(masterResource, *this, size,
            ptr);

        std::unique_ptr<Resource> returnPtr(mResource);
        //And return it
        return returnPtr;
    } else //Allocation failed
    {
        //Create a null resource to indicate error
        NullResource* error = new NullResource();

        std::unique_ptr<Resource> returnPtr(error);

        //And return it
        return returnPtr;
    }
}

bool HeapAllocator::allocate(MasterResource& masterResource,
    const Specifier& specifier, void* whereToAllocate)
{
    const LimitSpecifier& limit = static_cast<const LimitSpecifier&>(specifier);

    unsigned int size = limit.getQuantity();

    //Allocate memory from the heap
    ByteT* ptr = static_cast<ByteT*>(operator new(size, std::nothrow));

    //If the allocation was successful
    if (ptr != 0) {
        //Create a resource to store the allocation size and ptr
        // Prevent comiler warning bout unused variable
        //FIXME: Why is this variable not used? Memory leake?
        //HeapResource * mResource =
        new (whereToAllocate) HeapResource(masterResource, *this, size,
            ptr);

        //And return it
        return true;
    } else //Allocation failed
        return false;
}

bool HeapAllocator::deallocate(Resource& resource)
{
    HeapResource& mr = static_cast<HeapResource&>(resource);

    ByteT* ptr = mr.getPointer();

    bool correctType = mr.isType(HeapResource::classType);

    //Do a basic check for validity
    if (correctType && ptr != 0) {
        delete ptr;
        return true;
    }

    return false;
}

std::unique_ptr<Resource>
HeapAllocator::allocate(MasterResource& masterResource,
    unsigned int sizeInBytes)
{
    unsigned int size = sizeInBytes;

    //Allocate memory from the heap
    ByteT* ptr = static_cast<ByteT*>(operator new(size, std::nothrow));

    //If the allocation was successful
    if (ptr != 0) {
        //Create a resource to store the allocation size
        HeapResource* mResource = new HeapResource(masterResource, *this, size,
            ptr);

        std::unique_ptr<Resource> returnPtr(mResource);

        //And return it
        return returnPtr;
    } else //Allocation failed
    {
        //Create a null resource to indicate error
        NullResource* error = new NullResource();

        std::unique_ptr<Resource> returnPtr(error);

        //And return it
        return returnPtr;
    }
}
