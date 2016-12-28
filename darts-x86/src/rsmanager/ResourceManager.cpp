#include "ResourceManager.h"
#include <iostream>
using namespace darts;

ResourceManager::ResourceManager(InformationService& informationServiceToSet)
    : informationService__(informationServiceToSet)
{
}

ResourceManager::~ResourceManager(void)
{
}

std::unique_ptr<Resource>
ResourceManager::handleAllocateRequest(MasterResource& masterResource,
    const Specifier& specifier)
{
    std::list<Allocator*>::iterator allocateIt;

    typing::ResourceType type = masterResource.getType();

    std::unique_ptr<Resource> returnPtr;
    Allocator* allocator;
    //Search through allocator list
    for (allocateIt = allocators__.begin(); allocateIt != allocators__.end();
         ++allocateIt) {
        allocator = *allocateIt;

        //Handles this type?
        if (allocator->handles(type)) {
            //Yes -> try to allocate
            returnPtr = allocator->allocate(masterResource,
                specifier);

            if (returnPtr.get()->isValid()) {
                return returnPtr;
            }
        }
    }

    //No allocators could handle the request so return a null resource
    NullResource* error = new NullResource();

    returnPtr.reset(error);

    return returnPtr;
}

bool ResourceManager::handleAllocateRequest(MasterResource& masterResource,
    const Specifier& specifier, void* whereToAllocate)
{
    std::list<Allocator*>::iterator allocateIt;

    typing::ResourceType type = masterResource.getType();


    std::unique_ptr<Resource> returnPtr;

    Allocator* allocator;

    //Search through allocator list
    for (allocateIt = allocators__.begin(); allocateIt != allocators__.end();
         ++allocateIt) {
        allocator = *allocateIt;

        //Handles this type?
        if (allocator->handles(type)) {
            //Yes -> try to allocate
            if (allocator->allocate(masterResource,
                    specifier, whereToAllocate))
                return true;
        }
    }

    return false;
}

bool ResourceManager::handleDeallocateRequest(Resource& resource)
{
    Allocator& allocator = resource.getAllocator();

    return allocator.deallocate(resource);
}


std::unique_ptr<Resource>
ResourceManager::allocate(struct ResourcePlusItsNeeds resourcePlusNeeds)
{
    const Specifier* specifier = resourcePlusNeeds.setALimit;
    typing::ResourceType type = resourcePlusNeeds.typeOfResource;

    std::vector<struct ResourceNeed> needs = resourcePlusNeeds.needs;

    std::vector<MasterResource*> masterAvaiableList = informationService__.queryAvailable(type);

    std::vector<MasterResource*>::iterator searchMasterIt;

    std::vector<struct ResourceNeed>::iterator needIt;

    if (!masterAvaiableList.empty()) {
        MasterResource* masterResource = 0;

        *masterAvaiableList.begin();

        struct ResourceNeed need;

        bool needsMet = true;

        //Find first master resource who meets need
        //Searches through the whole master list
        for (searchMasterIt = masterAvaiableList.begin();
             searchMasterIt != masterAvaiableList.end(); ++searchMasterIt) {
            needsMet = true;

            masterResource = *searchMasterIt;

            //Check all the needs
            for (needIt = needs.begin(); needIt != needs.end(); ++needIt) {
                need = *needIt;

                //Check if the resource meets need
                if (!masterResource->doIMeetNeed(need)) {
                    //A need wasn't met so continue searching the loop
                    needsMet = false;
                    break;
                }
            }

            //All needs were met
            if (needsMet == true) {
                //So quit searching
                break;
            }
        }

        if (needsMet == false) {
            //Return a pointer to a null object if needs not met
            NullResource* error = new NullResource();

            std::unique_ptr<Resource> returnPtr(error);

            return returnPtr;
        }
        return handleAllocateRequest(*masterResource, *specifier);
    } else {
        //Return a pointer to a null object if needs not met
        NullResource* error = new NullResource();


        std::unique_ptr<Resource> returnPtr(error);

        return returnPtr;
    }
}

bool ResourceManager::allocateLazy(struct ResourcePlusItsNeeds resourcePlusNeeds)
{
    const Specifier* specifier = resourcePlusNeeds.setALimit;
    typing::ResourceType type = resourcePlusNeeds.typeOfResource;
    void* whereToAllocate = resourcePlusNeeds.address;

    std::vector<struct ResourceNeed> needs = resourcePlusNeeds.needs;

    std::vector<MasterResource*> masterAvaiableList = informationService__.queryAvailable(type);

    std::vector<MasterResource*>::iterator searchMasterIt;

    std::vector<struct ResourceNeed>::iterator needIt;

    if (!masterAvaiableList.empty()) {
        MasterResource* masterResource = 0;

        *masterAvaiableList.begin();

        struct ResourceNeed need;

        bool needsMet = true;

        //Find first master resource who meets need
        //Searches through the whole master list
        for (searchMasterIt = masterAvaiableList.begin();
             searchMasterIt != masterAvaiableList.end(); ++searchMasterIt) {
            needsMet = true;

            masterResource = *searchMasterIt;

            //Check all the needs
            for (needIt = needs.begin(); needIt != needs.end(); ++needIt) {
                need = *needIt;

                //Check if the resource meets need
                if (!masterResource->doIMeetNeed(need)) {
                    //A need wasn't met so continue searching the loop
                    needsMet = false;
                    break;
                }
            }

            //All needs were met
            if (needsMet == true) {
                //So quit searching
                break;
            }
        }

        if (needsMet == false) {
            return needsMet;
        }
        return handleAllocateRequest(*masterResource, *specifier, whereToAllocate);
    }

    return false;
}

bool ResourceManager::deallocate(
    std::vector<Resource*>& resources)
{
    std::vector<Resource*>::iterator deallocateIt;

    bool success = true;

    Resource* resourcePtr;

    //Loop through all resources to deallocate
    for (deallocateIt = resources.begin(); deallocateIt != resources.end();
         ++deallocateIt) {
        //Deallocate our resource
        resourcePtr = *deallocateIt;

        Resource& resource = *resourcePtr;

        if (handleDeallocateRequest(resource) == false) {
            success = false;
        }
    }

    return success;
}

bool ResourceManager::deallocate(Resource& resource)
{
    return handleDeallocateRequest(resource);
}

std::vector<Resource*>
ResourceManager::coallocate(
    std::vector<struct ResourcePlusItsNeeds> resourcesPlusNeeds)
{
    std::vector<struct ResourcePlusItsNeeds>::iterator createIt;

    bool success = true;

    std::vector<Resource*> resourcesCreated;

    //For all the resources sent in try to create them
    for (createIt = resourcesPlusNeeds.begin();
         createIt != resourcesPlusNeeds.end(); ++createIt) {
//Try to allocate the resource
#if 0
		std::auto_ptr< Resource > resourceAllocated = allocate(*createIt);
#else
        std::unique_ptr<Resource> resourceAllocated = allocate(*createIt);
#endif

        if (!resourceAllocated.get()->isValid()) {
            //Failure so time to rollback
            success = false;

            break;
        }

        resourcesCreated.push_back(resourceAllocated.get());
        resourceAllocated.release();
    }

    if (!success) {
        std::vector<Resource*>::iterator rewindIt;

        //Deallocate all resources created up to the failure
        for (rewindIt = resourcesCreated.begin();
             rewindIt != resourcesCreated.end(); ++rewindIt) {
            Resource& resource = *(*rewindIt);
            handleDeallocateRequest(resource);
        }

        resourcesCreated.clear();
    }

    return resourcesCreated;
}

bool ResourceManager::coallocateLazy(
    std::vector<struct ResourcePlusItsNeeds> resourcesPlusNeeds)
{
    std::vector<struct ResourcePlusItsNeeds>::iterator createIt;

    bool success = true;

    //For all the resources sent in try to create them
    for (createIt = resourcesPlusNeeds.begin();
         createIt != resourcesPlusNeeds.end(); ++createIt) {
        //Try to allocate the resource
        success = allocateLazy(*createIt);

        if (!success) {
            break;
        }
    }

    return success;
}

void ResourceManager::addAllocator(Allocator& allocatorToAdd)
{
    allocators__.push_back(&allocatorToAdd);
}
