#pragma once

#include "Defines.h"
#include "Lock.h"
#include "ResourceNeed.h"
#include "Typing.h"
#include <list>
#include <map>
#include <vector>

namespace darts {

class Resource;

/*
	Class: MasterResource

	Used to keep a master list of resource and information.
	MasterResources are used to allocate <Resources>.

	The master resource class also provides fast inheritance
	relationship/typing checking via prime numbers.
*/
class MasterResource {
private:
    //Section: Private->

    /*
		Variable: type__
			Type of this resource
	*/
    typing::ResourceType type__;
    /*
		Variable: inherit__
			Inheritance information for this resource.
	*/
    unsigned int inherit__;

    /*
		Variable: linkedResources__
			Maps this resource to other resources using a goodness value.
	*/
    std::multimap<int, MasterResource const*> linkedResources__;

    /*
		Variable: dependencyList__
			List of resources that are dependent on me.
	*/
    std::list<MasterResource*> dependencyList__;

    /*
		Variable: alive__
			tells if the resource is usable.
	*/
    bool alive__;

    /*
		Variable: ourResourceStats__

		A link to the resource that defines us.
	*/
    Resource* ourResourceStats__;

public:
    //Section: Public ->

    /*
		Constructor: MasterResource

		Initializes the object.

		Parameters:
			typeToSet - the type of this resource.
			inheritToSet - inheritance of this resource.
			ourResourceStatsToSet - the resource to represent us.
	*/
    MasterResource(typing::ResourceType typeToSet, unsigned int inheritToSet,
        Resource* ourResourceStatsToSet)
        : type__(typeToSet)
        , inherit__(inheritToSet)
        , alive__(true)
        , ourResourceStats__(ourResourceStatsToSet)
    {
    }

    /*
		Function: getType

		Returns the type of this instantiation.

		Returns:

			The <typing::ResourceType> of this master.

		See Also:

			<typing>
	*/
    typing::ResourceType
    getType() const;

    /*
		Function: getInherit

		Returns the inheritance of this instantiation.

		Returns:

			The <typing::ResourceType> of this instantation.

		See Also:

			<typing>
	*/
    unsigned int
    getInherit(void) const;

    /*
		Function: instanceOf

		Returns true if this instantiation is the child of the type given.

		Returns:

			A bool to indicate inheritance.

		See Also:

			<typing>
	*/
    bool
    instanceOf(typing::ResourceType typeCheck) const;

    /*
		Function: isAlive

		Returns true if this resource is alive.

		Returns:

			True if the resource is alive.
	*/
    bool
    isAlive(void) const;

    /*
		Function: setAlive

		Says the resource is alive.
		All resources who depend on us are also set alive.
	*/
    void
    setAlive(void);

    /*
		Function: setDead

		Says the resource is dead.
		All resources who depend on us are also set dead.
	*/
    void
    setDead(void);

    /*
		Function:  getOurResourceStats

		Returns the resource info of this class.

		Returns:

			<Resource> * of this class.
	*/
    Resource*
    getOurResourceStats(void) const;

    /*
		Function: getResourcesWhoMeetNeed

		Returns a list of resources who meet the given need that are
		mapped to this resource.

		Parameters:

			need - A need is defined by: <ResourceNeed>; A want or desire...

		Returns:

			An array of resources that meet the need.
	*/
    std::vector<MasterResource const*>
    getResourcesWhoMeetNeed(struct ResourceNeed need);

    /*
		Function: doIMeetNeed

		Returns true if I meet the given need.

		Parameters:

			need - A need is defined by: <ResourceNeed>; A want or desire...

		Returns:

			True if the need is met.
	*/
    bool
    doIMeetNeed(struct ResourceNeed need);

    /*
		Function: addGoodness

		Adds the reference resource to our resource map.

		Parameters:

			resource - the reference we wish to add to our map.
			goodness - how close it is to this resource.
	*/
    void
    addGoodness(MasterResource& resource, int goodness);

    /*
		Function: linksGoodness

		Adds the reference resource to our resource map and adds our
		resource to their map.

		Parameters:

			resource - the reference we wish to add to our map.
			goodness - how close the resources are together.
	*/
    void
    linkGoodness(MasterResource& resource, int goodness);

    /*
		Function: addDependency

		Adds a resource that is dependent on us to our dependency list.

		Parameters:

			resource - the reference we wish to add to our list.
	*/
    void addDependency(MasterResource& resource);

    Lock lock;
};

} // namespace darts
