#include "InformationService.h"

using namespace darts;

void InformationService::addSoftwareResource(MasterResource& resource)
{
    resources__.push_back(&resource);
}

void InformationService::addHardwareResource(MasterResource& resource)
{
    hardwareResources__.push_back(&resource);
}

void InformationService::updateLiveness(void)
{
    std::list<MasterResource*>::iterator updateIt;

    //Poll are hardware resources to see if they are alive
    for (updateIt = hardwareResources__.begin();
         updateIt != hardwareResources__.end(); ++updateIt) {
        HardwareResource* hw;
        Resource* currentResource = (*updateIt)->getOurResourceStats();

        hw = static_cast<HardwareResource*>(currentResource);

        if (!hw->poll()) {
            (*updateIt)->setDead();
        }
    }
}

void InformationService::signalDead(MasterResource& masterResource)
{
    //Needs to double check hardware dependencies...poll them to see if alive
    masterResource.setDead();
}

InformationService::InformationService(void)
{
}

InformationService::~InformationService(void)
{
}

std::vector<MasterResource*>
InformationService::queryAvailable(typing::ResourceType type)
{
    std::vector<MasterResource*> foundResourceList;
    std::list<MasterResource*>::iterator queryIt;

    //Check all the resources alive to see if we are type of them
    for (queryIt = resources__.begin();
         queryIt != resources__.end(); ++queryIt) {
        //Add to the found list if successful
        if ((*queryIt)->instanceOf(type))
            foundResourceList.push_back(*queryIt);
    }

    return foundResourceList;
}
