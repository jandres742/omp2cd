#pragma once

#include "HeapResource.h"
#include "MemoryAllocator.h"
#include "MemoryResource.h"
#include "NullResource.h"
#include "Resource.h"
#include "ResourceNeed.h"

namespace darts {

/*
	Class: HeapAllocator

	A class to represent heap memory resource creation. It provides
	basic interfaces for allocating a heap resource type.

	Inherits:
		<MemoryAllocator>
*/
class HeapAllocator : public MemoryAllocator {
private:
public:
    //Section: Public ->

    /*
		Constructor: HeapAllocator

		Initializes the object.
	*/
    HeapAllocator(void);

    /*
		Destructor: HeapAllocator

		Destroys the object.
	*/
    ~HeapAllocator(void);

    /*
		Function: getTypeOfMyHandle

		This allocator can handle memory types.

			Returns:

			The heap memory resource inheritance.
	*/
    unsigned int
    getTypeOfMyHandle(void);

/*
		Function: allocate

		Allocates a memory resource given a place to store and
		quantity in bytes defined by the reference.

		Parameters:
			masterResource - resource to allocate from.
			specifier - a <LimitSpecifier> to set the size.

		Returns:

			A auto pointer to a memory resource on success.
			Otherwise, a NullResource is returned.

		See Also:

			<MemoryResourceNeed>
			<Allocator>

	*/

    std::unique_ptr<Resource>
    allocate(MasterResource& masterResource, const Specifier& specifier);

    /*
		Function: allocate

		Allocates a memory resource given a place to store and
		quantity in bytes defined by the reference.

		Parameters:
			masterResource - resource to allocate from.
			specifier - a <LimitSpecifier> to set the size.

		Returns:

			A auto pointer to a memory resource on success.
			Otherwise, a NullResource is returned.

		See Also:

			<MemoryResourceNeed>
			<Allocator>

	*/
    bool
    allocate(MasterResource& masterResource, const Specifier& specifier,
        void* whereToAllocate);

    /*
		Function: deallocate

		Remove a heap resource given a reference.

		Parameters:
			resource - the heap resource to deallocate.

		Returns:

			True on success.
	*/
    bool
    deallocate(Resource& resource);

/*
		Function: allocate

		Allocates a memory resource on the heap using the master resource
		given a size in bytes.

		Parameters:
			masterResource - resource to allocate from.
			sizeInBytes - allocate byte amount of space.

		Returns:

			Memory resource of size allocated.

		See Also:

			<MemoryResourceNeed>
			<Allocator>

	*/

    std::unique_ptr<Resource>
    allocate(MasterResource& masterResource, unsigned int sizeInBytes);
};

} // namespace darts
