#pragma once

#include "HardwareSpecifier.h"
#include "Resource.h"

namespace darts {

/*
	Class: HardwareResource

	An abstract class used to represent hardware resources. It uses the
	<HardwareSpecifier> class to indicate that all hardware resources
	need a method to poll.

	Inherits:
		<Resource>, <HardwareSpecifier>

	InstanceOf:
		<Resource>
*/
class HardwareResource : public HardwareSpecifier, public Resource {
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
};

} // namespace darts
