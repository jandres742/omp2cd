#pragma once

#include "Specifier.h"
#include "Typing.h"
#include <vector>

namespace darts {

/*
	Enum: GoodnessOper

	EQ  - Goodness is equal to.
	N   - Goodness is not equal to.
	L   - Goodness less than.
	LEQ - Goodness is less than or equal to.
	G   - Goodness is greater than.
	GEQ - Goodness is greater than or equal to.
*/
enum GoodnessOper { EQ,
    N,
    L,
    LEQ,
    G,
    GEQ };

/*
	Structure: ResourceNeed

	A tuple of the <typing::ResourceType> of resource needed, the goodness
	needed, and an operator to bound the goodness level of the needs.
*/
struct ResourceNeed {
    typing::ResourceType typeOfNeed;
    int goodnessLevel;
    GoodnessOper howToBound;
};

/*
	Structure: ResourcePlusItsNeed

	A  tuple of <typing::ResourceType>, <Specifier>, and a vector of its
	needed resources.

	Specifiers may set limits etc for the resource type.
*/
struct ResourcePlusItsNeeds {
    const Specifier* setALimit;
    typing::ResourceType typeOfResource;
    std::vector<struct ResourceNeed> needs;
    void* address;

    ResourcePlusItsNeeds(const Specifier* setLimitToMe,
        typing::ResourceType setTypeOfResourceToMe,
        std::vector<struct ResourceNeed> setNeedsToMe)
        : setALimit(setLimitToMe)
        , typeOfResource(setTypeOfResourceToMe)
        , needs(setNeedsToMe)
        , address(0)
    {
    }

    ResourcePlusItsNeeds(const Specifier* setLimitToMe,
        typing::ResourceType setTypeOfResourceToMe,
        std::vector<struct ResourceNeed> setNeedsToMe,
        void* setAddressToMe)
        : setALimit(setLimitToMe)
        , typeOfResource(setTypeOfResourceToMe)
        , needs(setNeedsToMe)
        , address(setAddressToMe)
    {
    }

    ResourcePlusItsNeeds(const Specifier* setLimitToMe,
        typing::ResourceType setTypeOfResourceToMe)
        : setALimit(setLimitToMe)
        , typeOfResource(setTypeOfResourceToMe)
        , address(0)
    {
    }

    ResourcePlusItsNeeds(const Specifier* setLimitToMe,
        typing::ResourceType setTypeOfResourceToMe,
        void* setAddressToMe)
        : setALimit(setLimitToMe)
        , typeOfResource(setTypeOfResourceToMe)
        , address(setAddressToMe)
    {
    }
};

} // namespace darts
