#pragma once

//keyword: allocator

#include "MasterResource.h"
#include "ResourceNeed.h"
#include "Specifier.h"
#include <memory>
#include <vector>

namespace darts {

class Resource;

/*
	Class: Allocator

	An abstract class to represent resource creation. It provides basic
	interfaces for allocating a given resource type.
*/
class Allocator {
public:
    //Section: Public ->

    /*
		Function: getTypeOfMyHandle

			Returns:

			The inheritance this allocator can handle
	*/
    virtual unsigned int
    getTypeOfMyHandle(void)
        = 0;

/*
		Function: allocate

		Allocates a given resource from a <MasterResource> given a
		specifier of information to determine how to allocate.

		Parameters:
			masterResource - resource to allocate from.
			specifier - how to handle the request.

		Returns:

			A auto pointer to the allocated resource.

		See Also:

			<ResourceNeed>
			<MemoryAllocator>

	*/

    virtual std::unique_ptr<Resource>
    allocate(MasterResource& masterResource,
        const Specifier& specifier)
        = 0;

    /*
		Function: allocate

		Allocates a given resource from a <MasterResource> given a
		specifier of information to determine how to allocate.

		Parameters:
			masterResource - resource to allocate from.
			specifier - how to handle the request.

		Returns:

			A auto pointer to the allocated resource.

		See Also:

			<ResourceNeed>
			<MemoryAllocator>

	*/
    virtual bool
    allocate(MasterResource& masterResource,
        const Specifier& specifier, void* whereToAllocate)
        = 0;

    /*
		Function: deallocate

		Remove a given resource given a reference.

		Parameters:
			resource - the resource to deallocate.

		Returns:

			True on success.

		See Also:

			<ResourceNeed>
			<MemoryAllocator>

	*/
    virtual bool
    deallocate(Resource& resource)
        = 0;

    /*
		Function: handles

		Returns true if the allocator can handle allocating the type.

		Parameters:
			type - the type we wish to know if we can allocate.

		Returns:

			True on success.

	*/
    bool
    handles(typing::ResourceType type);
};

} // namespace darts
