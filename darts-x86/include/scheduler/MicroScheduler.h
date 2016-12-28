#pragma once
//Containers
#include "ringbuffer.h"
#include <vector>
//Codelets and TPs
#include "Codelet.h"
#include "ThreadedProcedure.h"
#include "codeletDefines.h"
//Schedulers
#include "ABCScheduler.h"
#include "Atomics.h"
#include "MSchedPolicy.h"
#include "Scheduler.h"
#include <cassert>
#include <tbb/concurrent_queue.h>
#include <unistd.h>

#define THRESHOLD 4

#define USE_RING_BUFF 1

namespace darts {
static const unsigned DEFAULT_MC_POLICY = 0;

const mc_policy m_policies[] = {
    /* 0 */ MicroStandard,
    /* 1 */ MicroSteal,
    /* 2 */ MicroSteal_Sleep,
    /* 3 */ MicroSteal_DeleteAtFinal
};

class MScheduler : public ABCScheduler {
private:
#if USE_RING_BUFF == 1
    ringBuffer<Codelet*, THRESHOLD> buff;
#else
    tbb::concurrent_queue<Codelet*> buff;
#endif

    mc_policy myPolicy_;

public:
    //Constructors
    MScheduler(ABCScheduler* parent, std::vector<ABCScheduler*> child)
        : ABCScheduler(parent, child)
        ,
#if USE_RING_BUFF == 1
        buff()
        ,
#endif
        myPolicy_(m_policies[DEFAULT_MC_POLICY])
    {
    }

    MScheduler(void)
        : ABCScheduler()
        ,
#if USE_RING_BUFF == 1
        buff()
        ,
#endif
        myPolicy_(m_policies[DEFAULT_MC_POLICY])
    {
    }

    virtual ~MScheduler()
    {
    }

    void setPolicy(const mc_policy newPolicy)
    {
        assert(newPolicy != NULL);
        myPolicy_ = newPolicy;
    }

    /*This policy takes ready codelets from the waiting pool
        and puts them in the ready pool.  It then pushes codelets
        from the ready pool to a single micro scheduler.*/
    virtual void policy(void)
    {
        myPolicy_(this);
    }

    /*This is empty since we are scheduling only
         codelets, implying we have a TP scheduler
         who is responsible for this operation!!!*/
    virtual bool placeTP(tpClosure*) { return true; }
    virtual bool placeTP2(std::pair<ThreadedProcedure*, ThreadedProcedure*>*) { return true; }

    //This function puts a codelet into the waiting pool
    virtual bool placeCodelet(Codelet* input)
    {
        // return buff.push(input);
        buff.push(input);
        return true;
    }

    bool push(Codelet* const& toAdd)
    {
#if USE_RING_BUFF == 1
        return buff.push(toAdd);
#else
        buff.push(toAdd);
        return true;
#endif
    }

    Codelet* pop(void)
    {
#if USE_RING_BUFF == 1
        return buff.pull();
#else
        Codelet* temp = nullptr;
        buff.try_pop(temp);
        return temp;
#endif
    }
}; // class
} // namespace
