#pragma once
#include "ABCScheduler.h"
#include "SyncSlot.h"
#include "thread_safe_vector.h"
#include <stdint.h>

#include <mutex>

namespace darts {
//This is a forward declaration since there is a circular dependence
class Codelet;
class ABCScheduler;

/*
     * Class: ThreadedProcedure
     * The TP class is a virtual class. An instatiation requires a buildCDG function which is
     * the codelets put together
     * 
     * See Also:
     * <Codelet>
     * <TPScheduler>
    */

class ThreadedProcedure {
protected:
    /*
	 * Variable: ref_
	 * ref_ says when the TP is done
	*/
    // unsigned int ref_;

public:
    unsigned int ref_;

    ThreadedProcedure* parentTP_;
    unsigned int numThreads;
    unsigned int mainCodeletID;
    bool schedulingBeyondCluster;

    ThreadedProcedure(void);
    ThreadedProcedure(unsigned int num, ThreadedProcedure* parentTP);
    ThreadedProcedure(unsigned int in_numThreads, unsigned int in_mainCodeletID);

    virtual ~ThreadedProcedure();

    inline bool decRef(void)
    {
        return (1 == __sync_fetch_and_sub(&ref_, 1U));
    }

    inline void incRef(void)
    {
        __sync_fetch_and_add(&ref_, 1U);
    }

    inline bool zeroRef(void)
    {
        return (!ref_);
    }

    inline bool checkParent(void)
    {
        return (parentTP_ != this);
    }
    /*
        * Method: addCodelet
        * Adds a codelet to the TP's list
        */
    void add(Codelet* toAdd);

    unsigned int getID();
};
} // namespace darts
