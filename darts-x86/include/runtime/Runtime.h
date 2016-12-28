#pragma once
#include "ABCScheduler.h"
#include "AbstractMachine.h"
#include "Affinity.h"
#include "CodeletFinal.h"
#include "MicroScheduler.h"
#include "TPScheduler.h"
#include "Thread.h"
#include "threadlocal.h"
#include <cassert>

extern uint64_t TPSleepTime;
extern uint64_t MCSleepTime;

namespace darts {
class Runtime;

struct tpRuntimeArgs {
    Runtime* rt;
    unsigned int threadId;
    unsigned int clusterId;
    unsigned int tpSched;
};

struct mcRuntimeArgs {
    Runtime* rt;
    unsigned int threadId;
    unsigned int clusterId;
    unsigned int unitId;
    unsigned int mcSched;
};

class Runtime {
private:
    hwloc::AbstractMachine AbsMac;
    hwloc::Cluster* clusterMap;
    unsigned int numTPSched_;
    unsigned int numMCSched_;
    unsigned int numThreads_;
    TPScheduler** TPSched_;
    MScheduler** MCSched_;
    Thread* localThreads_;
    tpRuntimeArgs* tpargs_;
    mcRuntimeArgs* mcargs_;
    volatile unsigned int tpcount_;
    volatile unsigned int mccount_;
    volatile unsigned int fullcount_;
    volatile bool spin_;

public:
    static CodeletFinal finalSignal;
    Runtime(unsigned int maxCluster = -1, unsigned int maxWorker = -1);
    Runtime(ThreadAffinity* affinity);

    void run(tpClosure* tpToStart);

    ~Runtime(void);
	
	void shutdownRuntime();

    inline unsigned int getNumTPS(void)
    {
        return numTPSched_;
    }

    inline unsigned int getNumMCS(void)
    {
        return numMCSched_;
    }

    inline TPScheduler* newTPSched(unsigned int id)
    {
        return TPSched_[id] = new TPScheduler;
    }

    inline MScheduler* newMCSched(unsigned int id)
    {
        return MCSched_[id] = new MScheduler;
    }

    inline void decTP(void)
    {
        Atomics::fetchSub(tpcount_, 1U);
    }

    inline void decMC(void)
    {
        Atomics::fetchSub(mccount_, 1U);
    }

    inline void decFull(void)
    {
        if (1 == __sync_fetch_and_sub(&fullcount_, 1))
            spin_ = false;
    }

    inline bool checkTP(void) { return (tpcount_); }
    bool checkMC(void) { return (mccount_); }
    void spin(void)
    {
        while (spin_)
            ;
    }
    void linkTPSched(void);
    void linkMCSched(void);
};

} // namespace darts
