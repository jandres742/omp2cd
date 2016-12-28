#pragma once

#include "MasterResource.h"
#include "NullAllocator.h"
#include "Resource.h"

namespace darts {

/*
	Class: NullResource

	A resource that hass no value. Used for error checking.

*/
class NullResource : public Resource {
private:
    template <typename T>
    inline T&
    getLvalue(void)
    {
        static T t;
        return t;
    }

    MasterResource&
    getMasterResourceLvalue(void)
    {
        static MasterResource t(typing::NullResource, 0, 0);

        return t;
    }

public:
    //Section: Public ->

    /*
		Constructor: NullResource

		Initializes the object.
	*/
    NullResource()
        : Resource(getMasterResourceLvalue(), getLvalue<NullAllocator>())
    {
    }

    /*
		Variable: classType
			Type of this class.
	*/
    const static typing::ResourceType classType = typing::NullResource;
    /*
		Variable: classInherit
			Inheritance information for this class.
	*/
    const static unsigned int classInherit = classType;

    /*
		Function: getType

		Returns the type of this instantiation.

		Returns:

			The <typing::ResourceType> of this instantation.

		See Also:

			<typing>
	*/
    typing::ResourceType
    getType(void)
    {
        return classType;
    }

    /*
		Function: getInherit

		Returns the inheritance of this instantiation.

		Returns:

			The inheritance value of this instantiation.

	*/
    unsigned int getInherit(void)
    {
        return classInherit;
    }

    /*
		Function: read

		Returns an empty <ByteT> vector.
	*/
    std::vector<ByteT> read()
    {
        std::vector<ByteT> vec;
        return vec;
    };

    /*
		Function: write

		Doesn't write anything.

		Parameters:

			stuffToWrite - bytes to write to the resource.

		Returns:

			True if written.
	*/
    bool write(std::vector<ByteT> const& stuffToWrite)
    {
        // Prevent compiler warning about unused arguments
        (void)stuffToWrite;

        return false;
    }
};

} // namespace darts
