#include "MasterResource.h"
#include <utility>

using namespace darts;

typing::ResourceType
MasterResource::getType(void) const
{
    return type__;
}

unsigned int
MasterResource::getInherit(void) const
{
    return inherit__;
}

bool MasterResource::instanceOf(typing::ResourceType typeCheck) const
{
    if (inherit__ % typeCheck == 0)
        return true;
    else
        return false;
}

bool MasterResource::isAlive(void) const
{
    return alive__;
}

void MasterResource::setAlive(void)
{
    alive__ = true;

    std::list<MasterResource*>::iterator reviveIt;

    for (reviveIt = dependencyList__.begin();
         reviveIt != dependencyList__.end(); ++reviveIt) {
        (*reviveIt)->setAlive();
    }
}

void MasterResource::setDead(void)
{
    alive__ = false;

    std::list<MasterResource*>::iterator killIt;

    for (killIt = dependencyList__.begin();
         killIt != dependencyList__.end(); ++killIt) {
        (*killIt)->setDead();
    }
}

Resource*
MasterResource::getOurResourceStats(void) const
{
    return ourResourceStats__;
}

std::vector<MasterResource const*>
MasterResource::getResourcesWhoMeetNeed(struct ResourceNeed need)
{
    typing::ResourceType rType;
    int goodness;
    GoodnessOper oper;

    std::vector<MasterResource const*> resourcesFound;

    std::multimap<int, MasterResource const*>::iterator masterIt;

    rType = need.typeOfNeed;
    goodness = need.goodnessLevel;
    oper = need.howToBound;

    if (oper == EQ) {
        std::pair<std::multimap<int, MasterResource const*>::iterator,
            std::multimap<int, MasterResource const*>::iterator>
            range;

        range = linkedResources__.equal_range(goodness);

        for (masterIt = range.first; masterIt != range.second;
             ++masterIt) {
            if ((*masterIt).second->instanceOf(rType)) {
                resourcesFound.push_back((*masterIt).second);
            }
        }
    } else if (oper == N) {
        std::pair<std::multimap<int, MasterResource const*>::iterator,
            std::multimap<int, MasterResource const*>::iterator>
            range;

        range = linkedResources__.equal_range(goodness);

        for (masterIt = linkedResources__.begin();
             masterIt != range.first; ++masterIt) {
            if ((*masterIt).second->instanceOf(rType)) {
                resourcesFound.push_back((*masterIt).second);
            }
        }
        for (masterIt = range.second;
             masterIt != linkedResources__.end(); ++masterIt) {
            if ((*masterIt).second->instanceOf(rType)) {
                resourcesFound.push_back((*masterIt).second);
            }
        }
    } else if (oper == L) {
        std::multimap<int, MasterResource const*>::iterator lessBound;

        lessBound = linkedResources__.lower_bound(goodness);

        for (masterIt = linkedResources__.begin();
             masterIt != lessBound; ++masterIt) {
            if ((*masterIt).second->instanceOf(rType)) {
                resourcesFound.push_back((*masterIt).second);
            }
        }
    } else if (oper == LEQ) {
        std::multimap<int, MasterResource const*>::iterator greaterBound;

        greaterBound = linkedResources__.upper_bound(goodness);

        for (masterIt = linkedResources__.begin();
             masterIt != greaterBound; ++masterIt) {
            if ((*masterIt).second->instanceOf(rType)) {
                resourcesFound.push_back((*masterIt).second);
            }
        }
    } else if (oper == G) {
        std::multimap<int, MasterResource const*>::iterator greaterBound;

        greaterBound = linkedResources__.upper_bound(goodness);

        for (masterIt = greaterBound;
             masterIt != linkedResources__.end(); ++masterIt) {
            if ((*masterIt).second->instanceOf(rType)) {
                resourcesFound.push_back((*masterIt).second);
            }
        }
    } else if (oper == GEQ) {
        std::multimap<int, MasterResource const*>::iterator lessBound;

        lessBound = linkedResources__.lower_bound(goodness);

        for (masterIt = lessBound; masterIt != linkedResources__.end();
             ++masterIt) {
            if ((*masterIt).second->instanceOf(rType)) {
                resourcesFound.push_back((*masterIt).second);
            }
        }
    }

    return resourcesFound;
}

bool MasterResource::doIMeetNeed(struct ResourceNeed need)
{
    typing::ResourceType rType;
    int goodness;
    GoodnessOper oper;

    std::multimap<int, MasterResource const*>::iterator it;

    rType = need.typeOfNeed;
    goodness = need.goodnessLevel;
    oper = need.howToBound;

    if (oper == EQ) {
        std::pair<std::multimap<int, MasterResource const*>::iterator,
            std::multimap<int, MasterResource const*>::iterator>
            range;

        range = linkedResources__.equal_range(goodness);

        for (it = range.first; it != range.second; ++it) {
            if ((*it).second->instanceOf(rType)) {
                return true;
            }
        }
    } else if (oper == N) {
        std::pair<std::multimap<int, MasterResource const*>::iterator,
            std::multimap<int, MasterResource const*>::iterator>
            range;

        range = linkedResources__.equal_range(goodness);

        for (it = linkedResources__.begin(); it != range.first; ++it) {
            if ((*it).second->instanceOf(rType)) {
                return true;
            }
        }
        for (it = range.second; it != linkedResources__.end(); ++it) {
            if ((*it).second->instanceOf(rType)) {
                return true;
            }
        }
    } else if (oper == L) {
        std::multimap<int, MasterResource const*>::iterator lessBound;

        lessBound = linkedResources__.lower_bound(goodness);

        for (it = linkedResources__.begin(); it != lessBound; ++it) {
            if ((*it).second->instanceOf(rType)) {
                return true;
            }
        }
    } else if (oper == LEQ) {
        std::multimap<int, MasterResource const*>::iterator greaterBound;

        greaterBound = linkedResources__.upper_bound(goodness);

        for (it = linkedResources__.begin(); it != greaterBound; ++it) {
            if ((*it).second->instanceOf(rType)) {
                return true;
            }
        }
    } else if (oper == G) {
        std::multimap<int, MasterResource const*>::iterator greaterBound;

        greaterBound = linkedResources__.upper_bound(goodness);

        for (it = greaterBound; it != linkedResources__.end(); ++it) {
            if ((*it).second->instanceOf(rType)) {
                return true;
            }
        }
    } else if (oper == GEQ) {
        std::multimap<int, MasterResource const*>::iterator lessBound;

        lessBound = linkedResources__.lower_bound(goodness);

        for (it = lessBound; it != linkedResources__.end(); ++it) {
            if ((*it).second->instanceOf(rType)) {
                return true;
            }
        }
    }

    return false;
}

void MasterResource::addGoodness(MasterResource& resource, int goodness)
{
    linkedResources__.insert(std::make_pair(goodness, &resource));
}

void MasterResource::linkGoodness(MasterResource& resource, int goodness)
{
    linkedResources__.insert(std::make_pair(goodness, &resource));
    resource.addGoodness(*this, goodness);
}

void MasterResource::addDependency(MasterResource& resource)
{
    dependencyList__.push_back(&resource);
}
