#pragma once

namespace darts {

//	Namespace: typing
//	Delegates resource typing information so type names don't
//  don't conflict with their classes. Must be prime numbers.
namespace typing {

    /*
	Enum: ResourceType

	Resource         - All resources are this type.
	SharedResource   - Can be shared among users.
	LimitedResource  - Have a given limit of use.
	HardwareResource - Tied to hardware.
	MemoryResource   - Basic volatile computer storage.
	NullResource     - Error signaling resource.
	HeapResource     - Memory stored on the heap.
*/
    enum ResourceType {
        Resource = 2,
        SharedResource = 3,
        LimitedResource = 5,
        HardwareResource = 7,
        MemoryResource = 11,
        NullResource = 13,
        HeapResource = 17
    };

} // namespace typing

} // namespace darts
