/*The all methods are separated because there is a circular dependency
 between codelet and threaded procedure classes*/

//Don't Mess with the order of these includes
#include "Codelet.h"
#include "SyncSlot.h"
#include "ThreadedProcedure.h"
#include "codeletDefines.h"
#include "threadlocal.h"

#include "TPScheduler.h"

#include <cassert>

namespace darts {

Codelet::Codelet(uint32_t dep, uint32_t res, ThreadedProcedure* theTp, uint32_t stat)
    : sync_(dep, res)
    , status_(stat)
    , myTP_(theTp)
    , myLocalID(0)
    , myGlobalID(0)
{
}

Codelet::Codelet(uint32_t dep, uint32_t res, ThreadedProcedure* theTp,
    uint32_t stat, uint32_t _localID)
    : sync_(dep, res)
    , status_(stat)
    , myTP_(theTp)
    , myLocalID(_localID)
    , myGlobalID(_localID)
{
}

Codelet::Codelet(uint32_t dep, uint32_t res, ThreadedProcedure* theTp,
    uint32_t stat, uint32_t _localID, uint32_t _globalID)
    : sync_(dep, res)
    , status_(stat)
    , myTP_(theTp)
    , myLocalID(_localID)
    , myGlobalID(_globalID)
{
}

Codelet::Codelet(void)
    : sync_(0U, 0U)
    , status_(NIL)
    , myTP_(0)
    , myLocalID(-1)
    , myGlobalID(-1)
{
}

Codelet::Codelet(Codelet& original_codelet)
    : sync_(original_codelet.sync_)
    , status_(original_codelet.status_)
    , myTP_(original_codelet.myTP_)
    , myLocalID(original_codelet.myLocalID)
    , myGlobalID(original_codelet.myGlobalID)
{
}

Codelet& Codelet::operator=(const Codelet& rhs_codelet)
{
    if (this != &rhs_codelet) {
        this->sync_ = rhs_codelet.sync_;
        this->status_ = rhs_codelet.status_;
        this->myTP_ = rhs_codelet.myTP_;
        this->myLocalID = rhs_codelet.myLocalID;
        this->myGlobalID = rhs_codelet.myGlobalID;
    }
    return *this;
}

void Codelet::initCodelet(uint32_t dep, uint32_t res, ThreadedProcedure* theTp,
    uint32_t stat)
{
    sync_.initSyncSlot(dep, res);
    status_ = stat;
    myTP_ = theTp;
}

void Codelet::decDep(void)
{
    if (sync_.decCounter()) {
        myTP_->incRef();
        myThread.threadTPsched->placeCodelet(this);
    }
}

void Codelet::setDep(uint32_t dep)
{
    sync_.initSyncSlot(dep, this->sync_.getReset());
}

void Codelet::resetCodelet(void)
{
    sync_.resetCounter();
#if 0
        //The check is just in case we are reseting the final codelet
        if(sync_.ready()){
           myTP_->incRef();
           myThread.threadTPsched->placeCodelet(this);
        }
#endif
}

bool Codelet::casStatus(uint32_t oldval, uint32_t newval)
{
    return Atomics::boolcompareAndSwap(status_, oldval, newval);
}

void Codelet::setTP(ThreadedProcedure* aTP)
{
    myTP_ = aTP;
}

void Codelet::add(Codelet* aCodelet)
{
    if (aCodelet->codeletReady()) {
        myTP_->incRef();
        myThread.threadTPsched->placeCodelet(aCodelet);
    }
}

uint32_t Codelet::getCounter(void) const
{
    return sync_.getCounter();
}

SyncSlot* Codelet::getSyncSlot(void)
{
    return &sync_;
}

#ifdef TRACE
void* Codelet::returnFunct(void)
{
    void (Codelet::*f)(void) = &Codelet::fire;
    return (void*)f;
}
#endif

} // namespace darts
