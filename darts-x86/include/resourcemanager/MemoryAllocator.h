#pragma once

#include "Allocator.h"
#include "LimitSpecifier.h"
#include "MemoryResource.h"

namespace darts {

/*
	Class: MemoryAllocator

	An abstarct class to represent memory resource creation. It provides
	basic interfaces for allocating a given memory resource type.

	Inherits:
		<Allocator>
*/
class MemoryAllocator : public Allocator {
    // Import all "allocate" methods from the base class,
    // otherwise they will be hidden by our new "allocate" method
    using Allocator::allocate;

public:
    //Section: Public ->
    /*
		Constructor: MemoryAllocator

		Initializes the object.
	*/
    MemoryAllocator(void);

    /*
		Destructor: MemoryAllocator

		Destroys the object.
	*/
    ~MemoryAllocator(void);

/*
		Function: allocate

		Allocates a memory resource using the master resource
		given a size in bytes.

		Parameters:
			masterResource - resource to allocate from.
			sizeInBytes - allocate byte amount of space.

		Returns:

			An auto pointer to the memory resource of size allocated.

		See Also:

			<MemoryResource>
			<Allocator>
	*/

    virtual std::unique_ptr<Resource>
    allocate(MasterResource& masterResource, unsigned int sizeInBytes) = 0;
};

} // namespace darts
