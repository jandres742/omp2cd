//Don't Mess with the order of these includes
#include "ThreadedProcedure.h"
#include "Atomics.h"
#include "Codelet.h"
#include "codeletDefines.h"
#include "threadlocal.h"
#include <cassert>

namespace darts {
ThreadedProcedure::ThreadedProcedure(void)
    : ref_(0)
    , parentTP_(myThread.tempParent)
    , numThreads(0)
    , mainCodeletID(0)
{
}

ThreadedProcedure::ThreadedProcedure(unsigned int num, ThreadedProcedure* parentTP)
    : ref_(num)
    , parentTP_(parentTP)
    , numThreads(0)
    , mainCodeletID(0)
{
}

ThreadedProcedure::ThreadedProcedure(unsigned int in_numThreads,
    unsigned int in_mainCodeletID)
    : ref_(0)
    , parentTP_(myThread.tempParent)
    , numThreads(in_numThreads)
    , mainCodeletID(in_mainCodeletID)
{
}

ThreadedProcedure::~ThreadedProcedure(void)
{
    //We are deleting the TP
    //Does the TP have a parent
    if (parentTP_) {
        //Make sure he isn't a serial loop by checking if the parent has a parent
        if (parentTP_->checkParent()) {
            //If he does lets dec the parent's counter and see if the parent has no more outstanding objects
            if (parentTP_->decRef()) {
                delete parentTP_;
            }
        }
    }
}

void ThreadedProcedure::add(Codelet* toAdd)
{
    //check on where to set
    if (toAdd->codeletReady()) {
        Atomics::fetchAdd(ref_, 1U);
        myThread.threadTPsched->placeCodelet(toAdd);
    }
}

unsigned int ThreadedProcedure::getID()
{
    return this->mainCodeletID;
}
}
