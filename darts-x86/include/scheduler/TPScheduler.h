#pragma once
//Containers
#ifdef TBB
#include <tbb/concurrent_queue.h>
#else
#include "thread_safe_vector.h"
#endif

//Codelets and TPs
#include "Codelet.h"
#include "ThreadedProcedure.h"
#include "codeletDefines.h"
//Schedulers
#include "ABCScheduler.h"
#include "MicroScheduler.h"
#include "Scheduler.h"
//Resource Manager
#include "ResourceManager.h"
#include "TPSchedPolicy.h"
#include <mutex>
#include <queue>
#include <unistd.h>

#define USE_TBB_QUEUE 1

#if USE_TBB_QUEUE == 0
#include <boost/lockfree/queue.hpp>
#endif

static const unsigned DEFAULT_TP_POLICY = 0;

namespace darts {
const tp_policy tp_policies[] = {
    /* 0 */ TPPushFull,
    /* 1 */ TPRoundRobin,
    /* 2 */ TPSingleLevel,
    /* 3 */ TPTwoLevelSteal,
    /* 4 */ TPTwoLevelSteal_Sleep,
    /* 5 */ TPTwoLevelSteal_DeleteAtFinal
};

/*
    * Class: TPScheduler
    * 
    * This class instantiates a scheduler_t class.
    * It takes TPs and schedules them to execute.
    * A default policy is provided.	This policy
    * breaks TPs and pushes them to a codelet scheduler.
    * 
    * See Also:
    * <Scheduler>
    * <ThreadedProcedure>
    */

class TPScheduler : public Scheduler<tpClosure*> {
private:
    /*
        * Variable: peers_
        * This is tpschedulers we may want to steal from 
        */
    std::vector<TPScheduler*> peers_;

/*
        * Variable: Codelets_
        * This is for the serial and single level policy balancing is not at the codelet level
        * but it must be thread safe since it has multiple writes... Do(codelet);
        */
#ifdef TBB

#if USE_TBB_QUEUE == 1
    tbb::concurrent_queue<Codelet*> Codelets_;
#else
    boost::lockfree::queue<Codelet*> Codelets_{ 4096 };
#endif

#else
    thread_safe::vector<Codelet*> Codelets_;
#endif

    /*
        * Variable: whichSub_
        * This is for the round robin policy
        */
    size_t whichSub_;

    /*
        * Variable: myPolicy_
        * Function pointer to the policy the user wishes to use
        */
    //tp_policy myPolicy_;

public:
    tp_policy myPolicy_;

    //Constructors
    TPScheduler(ABCScheduler* parent, std::vector<ABCScheduler*> child)
        : Scheduler<tpClosure*>::Scheduler(parent, child)
        , whichSub_(0)
        , myPolicy_(tp_policies[DEFAULT_TP_POLICY])
    {
    }

    TPScheduler(void)
        : Scheduler<tpClosure*>::Scheduler()
        , whichSub_(0)
        , myPolicy_(tp_policies[DEFAULT_TP_POLICY])
    {
    }

    virtual ~TPScheduler()
    {
    }

    void setPolicy(const tp_policy newPolicy)
    {
        assert(newPolicy != NULL);
        myPolicy_ = newPolicy;
    }

    /*
        * Method: addPeer
        * Adds a peer to the list to use for stealing
        */
    inline void addPeer(TPScheduler* toAdd)
    {
        peers_.push_back(toAdd);
    }

    /*
        * Method: getNumPeers
        * Returns:
        * The number of TPS peers
        */
    inline size_t getNumPeers(void) const
    {
        return peers_.size();
    }

    /*
        * Method: getPeer
        * Get the ThreadedProcedure scheduler peer at the position pos
        * 
        * Parameters:
        *	pos - The position of the peer in the peers array
        * 
        * Returns:
        * The pointer to the peer
        */
    inline TPScheduler* getPeer(size_t pos) const
    {
        return peers_[pos % peers_.size()];
    }

    /*
        * Method: getSubIndexInc
        * This returns who is the next sub scheduler and inc the counter
        * This is used for the round robin policy
        * 
        * Returns:
        * A static index (last call's value + 1)
        */
    size_t getSubIndexInc(void)
    {
        size_t temp = whichSub_;
        whichSub_ = (whichSub_ + 1) % getNumSub();
        return temp;
    }

    inline size_t getSubIndex(void)
    {
        return whichSub_;
    }

    inline void setSubIndex(size_t index)
    {
        whichSub_ = index;
    }

    /*
        * Method: policy
        * Run the scheduling policy selected by the user
        * 
        * Parameters:
        * input - ???
        */
    virtual void
    policy(void)
    {
        myPolicy_(this);
    }

    /*
        * Method: placeTP
        * Adds a TP to the ready deque
        * 
        * Parameters:
        * input - ???
        */
    virtual bool placeTP(tpClosure* input)
    {
        pushReadyTail(input);

        return true;
    }

    inline tp_policy getPolicy(void)
    {
        return myPolicy_;
    }

    /*
        * Method: popCodelet
        * Pops a codelet from the codelet vector
        * 
        * Returns:
        * The pointer to the popped codelet
        */
    //Return pops and returns the last element of Codelets_
    Codelet*
    popCodelet(void)
    {
#ifdef TBB
        Codelet* temp = nullptr;

#if USE_TBB_QUEUE == 1
        Codelets_.try_pop(temp);
        return temp;
#else
        bool success = Codelets_.pop(temp);
        if (success)
            return temp;
        else
            return nullptr;
#endif

#else
        return Codelets_.popBack();
#endif
    }

    /*
        * Method: placeCodelet
        * This is for single level or serial scheduling policies
        * it pushes the codelet to the codelet vector
        * 
        * Parameters:
        * input - ???
        */
    virtual bool placeCodelet(Codelet* input)
    {
#ifdef TBB

#if USE_TBB_QUEUE == 1
        Codelets_.push(input);
#else
        Codelets_.push(input);
#endif

#else
        Codelets_.push_back(input);
#endif
        return true;
    }
}; //class
} //namespace
