#pragma once
#include "Atomics.h"
#include <iostream>
#include <stdint.h>

namespace darts {

/*
	 * Class: SyncSlot
   * This class contains to ints which are used as a counter and reset. This is to allow
   * a codelet to run.
	 * 
	 * See Also:
	 * <Atomics>
	*/

class SyncSlot {
private:
    unsigned int counter_;
    unsigned int reset_;

public:
    SyncSlot(uint32_t dep, uint32_t res)
        : counter_(dep)
        , reset_(res)
    {
    }

    SyncSlot(SyncSlot* original_sync)
        : counter_(original_sync->counter_)
        , reset_(original_sync->reset_)
    {
    }

    SyncSlot& operator=(const SyncSlot& rhs_sync)
    {
        if (this != &rhs_sync) {
            this->counter_ = rhs_sync.counter_;
            this->reset_ = rhs_sync.reset_;
        }
        return *this;
    }

    inline void initSyncSlot(uint32_t dep, uint32_t res)
    {
        counter_ = dep;
        reset_ = res;
    }

    //dec the counter
    inline bool decCounter(void)
    {
        return (1 == __sync_fetch_and_sub(&counter_, 1U));
    }

    //inc the counter
    inline void incCounter(void)
    {
        __sync_fetch_and_add(&counter_, 1U);
        __sync_fetch_and_add(&reset_, 1U);
    }

    inline void incOnlyCounter(void)
    {
        __sync_fetch_and_add(&counter_, 1U);
    }

    //resets the counter
    inline void resetCounter(void)
    {
        counter_ = reset_;
        //return Atomics::boolcompareAndSwap(counter_,0U,reset_);
    }

    //retrieves the counter's current value
    inline int getReset(void)
    {
        return reset_;
    }

    inline uint32_t getCounter(void) const
    {
        return counter_;
    }

    //returns the if the counter has reached zero
    inline bool ready(void) const
    {
        return (counter_ == 0);
    }
};

} // namespace darts
