#pragma once

#include "HardwareResource.h"
#include "MasterResource.h"
#include "Resource.h"
#include <list>
#include <vector>

namespace darts {

/*
	Class: InformationService

	A class used to keep track of all types of resources.
	Keeps a tab on hardware and software resources.
*/
class InformationService {
private:
    //Section: Private->

    /*
		Variable: hardwareResources__

		List of all software resources on the system
	*/
    std::list<MasterResource*> resources__;

    /*
		Variable: hardwareResources__

		List of all hardware resources on the system
	*/
    std::list<MasterResource*> hardwareResources__;

    /*
		Function: updateLiveness

		Polls all hardware resources and sets them dead if
		they cannot be reached.

	*/
    void updateLiveness(void);

public:
    //Section: Public->

    /*
		Constructor: InformationService

		Initializes the object. I'm assuming the constructor will get
		create initial list for now. For instance, I will use a config
		file for this purpose when I do the start up code.
	*/
    InformationService(void);
    /*
		Destructor: InformationService

		Destroys the object.
	*/
    ~InformationService(void);

    //These add methods should be private but for testing purposes...

    /*
		Function: addSoftwareResource

		Adds a resource to the alive software list.

		Parameters:

			resource - the resource to add.
	*/
    void
    addSoftwareResource(MasterResource& resource);

    /*
		Function: addHardwareResource

		Adds a resource to the alive hardware list.

		Parameters:

			resource - the resource to add.
	*/
    void
    addHardwareResource(MasterResource& resource);

    /*
		Function: queryAvailable

		Checks to see if the type of resource is available and returns
		an array of available resources meeting that type.

		Parameters:

			type - the type of resource to check if available.

		Returns:

			<MasterResource> * array of found resources.
	*/
    std::vector<MasterResource*>
    queryAvailable(typing::ResourceType type);

    /*
		Function: signalDead

		Indicates the resource was fond to be dead.
		The information service should check to see if this is true/
		Then, set the resource dead.

		Parameters:

			masterResource - the resource to set dead.

	*/
    void signalDead(MasterResource& masterResource);
};

} // namespace darts
