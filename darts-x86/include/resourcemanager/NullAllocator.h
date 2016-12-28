#pragma once

//keyword: allocator

#include "Allocator.h"
#include "Specifier.h"
#include <vector>

namespace darts {

/*
	Class: NullAllocator

	A class to represent null resource creation. It provides basic
	interfaces for allocating a given resource type.
*/
class NullAllocator : public Allocator {
public:
    //Section: Public ->

    /*
		Function: getTypeOfMyHandle

		Returns:

			The type inheritance this can allocate.
	*/
    unsigned int
    getTypeOfMyHandle(void)
    {
        return typing::NullResource;
    }

/*
		Function: allocate

		Returns an empty pointer.

		Parameters:

			masterResource - resource to allocate from.
			specifier - how to handle the request.

		Returns:

			Null pointer.

	*/

    std::unique_ptr<Resource>
    allocate(MasterResource& masterResource, const Specifier& specifier)
    {
        // Prevent compiler warning about unused arguments
        (void)masterResource;
        (void)specifier;

        std::unique_ptr<Resource> dummy;

        return dummy;
    }

    /*
		Function: allocate

		Returns false always.

		Parameters:

			masterResource - resource to allocate from.
			specifier - how to handle the request.
			whereToAllocate - allocates resource there...

		Returns:

			false.

	*/
    bool
    allocate(MasterResource& masterResource, const Specifier& specifier,
        void* whereToAllocate)
    {
        // Prevent compiler warning about unused arguments
        (void)masterResource;
        (void)specifier;
        (void)whereToAllocate;

        return false;
    }

    /*
		Function: deallocate

		Does nothing.

		Parameters:
			resource - the resource to deallocate.

		Returns:

			True always.
	*/
    bool
    deallocate(Resource& resource)
    {
        // Prevent compiler warning about unused arguments
        (void)resource;

        return true;
    }
};

} // namespace darts
