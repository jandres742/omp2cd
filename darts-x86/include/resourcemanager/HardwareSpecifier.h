#pragma once

#include "Resource.h"
#include "Specifier.h"

namespace darts {

/*
	Class: HardwareSpecifier

	An abstract class used to represent hardware resources. It provides
	methods for polling hardware and interfacing with it.

	Adding this specifier to a any resource requires a polling method
	to be implemented.

	InstanceOf:
		<Resource>
*/
class HardwareSpecifier : public Specifier {
public:
    //Section: Public ->

    /*
		Variable: classType
			Type of this class.
	*/
    const static typing::ResourceType classType = typing::HardwareResource;
    /*
		Variable: classInherit
			Inheritance information for this class.
	*/
    const static unsigned int classInherit = classType * Resource::classInherit;

    /*
		Function: poll

		Does a hardware check for the resource to see if it is available.

		Returns:

			True if the hardware is available.
	*/
    virtual bool
    poll(void)
        = 0;
};

} // namespace darts
