#include "MSchedPolicy.h"
#include "Codelet.h"
#include "MicroScheduler.h"
#include "TPSchedPolicy.h"
#include "TPScheduler.h"

#ifdef TRACE
#include "getClock.h"
#endif

uint64_t MCSleepTime;

namespace darts {
inline void cleanupOrSleep(bool scheduled, MScheduler* mSched,
    std::queue<ThreadedProcedure*>& pendingTPsToDel)
{
    /* If nothing was scheduled or there are too many closures or TPs
* to delete, do it. If there is nothing to delete, just go to
* sleep.*/
    if (pendingTPsToDel.size() > PENDINGTHRESHOLD) {
        while (pendingTPsToDel.empty() == false) {
            ThreadedProcedure* TPToDel = pendingTPsToDel.front();
            pendingTPsToDel.pop();
            delete TPToDel;
        }
    } else if (mSched->getNumSched() > mSched->getNumOfMaxUnitsPerCluster() * 2 && scheduled == false) {
        usleep(mSched->getSleepTime());
        if (mSched->getSleepTime() <= MCSleepTime)
            mSched->setSleepTime(mSched->getSleepTime() << 1);
    } else if (scheduled) {
        mSched->setSleepTime(1);
    }
}

bool MicroStandard(MScheduler* mSched)
{
    /*TPs are deleted on the run */
    while (mSched->alive()) {
        Codelet* tempCodelet = mSched->pop();
        while (tempCodelet && mSched->alive()) {
            ThreadedProcedure* checkTP = tempCodelet->getTP();
            bool deleteTP = (checkTP) ? checkTP->checkParent() : false;
            tempCodelet->fire();
            if (deleteTP)
                if (checkTP->decRef())
                    delete checkTP;

            tempCodelet = mSched->pop();
        }
    }
    return true;
}

bool MicroSteal(MScheduler* mSched)
{
    /*TPs are deleted on the run */
    TPScheduler* myTPSched = static_cast<TPScheduler*>(mSched->getParentScheduler());
    while (mSched->alive()) {
        Codelet* tempCodelet = myTPSched->popCodelet();
        if (tempCodelet) {
            tempCodelet->fire();
            ThreadedProcedure* checkTP = tempCodelet->getTP();
            bool deleteTP = (checkTP) ? checkTP->checkParent() : false;
            if (deleteTP)
                if (checkTP->decRef())
                    delete checkTP;
        }
    }
    return true;
}

bool MicroSteal_Sleep(MScheduler* mSched)
{
    /*TPs are deleted when scheduler is idle */
    std::queue<ThreadedProcedure*> pendingTPsToDel;
    TPScheduler* myTPSched = static_cast<TPScheduler*>(mSched->getParentScheduler());
    while (mSched->alive()) {
        bool scheduled = true;
        Codelet* tempCodelet = myTPSched->popCodelet();
        if (!tempCodelet)
            scheduled = false;
        while (tempCodelet && mSched->alive()) {
            tempCodelet->fire();
            ThreadedProcedure* checkTP = tempCodelet->getTP();
            bool deleteTP = (checkTP) ? checkTP->checkParent() : false;
            if (deleteTP)
                if (checkTP->decRef())
                    pendingTPsToDel.push(checkTP);
            tempCodelet = myTPSched->popCodelet();
        }
        cleanupOrSleep(scheduled, mSched, pendingTPsToDel);
    }
    cleanupOrSleep(false, mSched, pendingTPsToDel);
    return true;
}

bool MicroSteal_DeleteAtFinal(MScheduler* mSched)
{
    /*TPs are deleted when scheduler is idle */
    std::queue<ThreadedProcedure*> pendingTPsToDel;
    TPScheduler* myTPSched = static_cast<TPScheduler*>(mSched->getParentScheduler());
    while (mSched->alive()) {
        Codelet* tempCodelet = myTPSched->popCodelet();
        while (tempCodelet && mSched->alive()) {
            tempCodelet->fire();
            ThreadedProcedure* checkTP = tempCodelet->getTP();
            bool deleteTP = (checkTP) ? checkTP->checkParent() : false;
            if (deleteTP)
                if (checkTP->decRef())
                    pendingTPsToDel.push(checkTP);
            tempCodelet = myTPSched->popCodelet();
        }
    }
    cleanupOrSleep(false, mSched, pendingTPsToDel);
    return true;
}
} //namespace darts
