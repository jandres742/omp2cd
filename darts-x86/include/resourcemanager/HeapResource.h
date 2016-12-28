#pragma once

#include "Defines.h"
#include "MemoryResource.h"

namespace darts {

/*
	Class: HeapResource

	A class used to represent a heap memory resource. It provides
	methods for reading and writing to memory.

	InstanceOf:
		<Resource>, <LimitedResource>, <SharedResource>, <HardwareResource>,
		<MemoryResource>

	Inherits:
		<MemoryResource>
*/
class HeapResource : public MemoryResource {
public:
    /*
		Variable: classType
			Type of this class.
	*/
    const static typing::ResourceType classType = typing::HeapResource;
    /*
		Variable: classInherit
			Inheritance information for this class.
	*/
    const static unsigned int classInherit = classType * MemoryResource::classInherit;

    /*
		Constructor: HeapResource

		Initializes the object.

		Parameters:

			masterToSet - the master resource we were allocated from.
			allocatorToSet - the allocator we were allocated from.
			pointerToAssign - assigns the memory pointer to this.
			sizeInBytes - sets the quantity to this memory to this byte size.
	*/
    HeapResource(MasterResource& masterToSet, Allocator& allocatorToSet,
        int sizeInBytes, ByteT* pointerToAssign);

    /*
		Destructor: MemoryResource

		Destroys the object.
	*/
    ~HeapResource(void);

    /*
		Function: getType

		Returns the type of this instantiation.

		Returns:

			The <Typing::ResourceType> of this instantation.

		See Also:

			<Typing>
	*/
    typing::ResourceType
    getType(void);

    /*
		Function: getInherit

		Returns the inheritance of this instantiation.

		Returns:

			The inheritance value of this instantiation.

	*/
    unsigned int
    getInherit(void);
};

} // namespace darts
