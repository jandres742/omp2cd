#pragma once

#include "Resource.h"
#include "Specifier.h"

namespace darts {

/*
	Class: LimitSpecifier

	A class used to represent limited resources. It provides
	methods and guarantees for limiting a resource.

	InstanceOf:
		<Resource>
*/
class LimitSpecifier : public Specifier {
private:
    //Section: Protected ->

    /*
		Variable: quantity__

		The amount of this resource.
	*/
    unsigned int quantity__;

public:
    //Section: Public ->

    /*
		Constructor: LimitedResource

		Parameters:

			 quantityToSet - the quantity of this resource.
	*/
    LimitSpecifier(unsigned int quantityToSet)
        : quantity__(quantityToSet)
    {
    }

    LimitSpecifier()
    {
    }

    /*
		Variable: classType
			Type of this class.
	*/
    const static typing::ResourceType classType = typing::LimitedResource;
    /*
		Variable: classInherit
			Inheritance information for this class.
	*/
    const static unsigned int classInherit = classType * Resource::classInherit;

    /*
		Function: getQuantity

		Returns the quantity available of this resource.

		Returns:

			Amount of available for this resource.
	*/
    int
    getQuantity(void) const;
};

} // namespace darts
