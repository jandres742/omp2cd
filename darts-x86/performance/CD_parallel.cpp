#include "stdint.h"
#include "darts.h"
#include "getClock.h"

using namespace darts;

class cdParallelSource : public Codelet
{
public:
    cdParallelSource(uint32_t dep, uint32_t res, ThreadedProcedure * myTP, uint32_t stat) :
    Codelet(dep, res, myTP, stat){ }
    virtual void fire(void);
};

class cdParallelTester : public Codelet
{
public:
    cdParallelTester(void) :
    Codelet(){}
    void
    initTester(uint32_t dep, uint32_t res, ThreadedProcedure * myTP, uint32_t stat)
    {
        initCodelet(dep, res, myTP, stat);
    }
    virtual void fire(void);
};

class cdParallelSinc : public Codelet
{
public:

    cdParallelSinc(uint32_t dep, uint32_t res, ThreadedProcedure * myTP, uint32_t stat) :
    Codelet(dep, res, myTP, stat){ }
    virtual void fire(void);
};

class cdParallel : public ThreadedProcedure
{
public:
    //These are the frame variables
    int fanout;

    uint64_t * startTime;
    uint64_t * endTime;

    cdParallelSource aSource;
    cdParallelTester * testers;
    cdParallelSinc aSinc;
    Codelet * toSignal;

    cdParallel(int theFanout, uint64_t * theStartTime, uint64_t * theEndTime, Codelet * toSig) :
    ThreadedProcedure(),
    fanout(theFanout),
    startTime(theStartTime),
    endTime(theEndTime),
    aSource(0, 0, this, SHORTWAIT),
    testers(0),
    aSinc(theFanout, theFanout, this, SHORTWAIT),
    toSignal(toSig)
    {
        add(&aSource);
        testers = new cdParallelTester[theFanout];
        for (int i = 0; i < theFanout; i++)
        {
            testers[i].initCodelet(1, 1, this, SHORTWAIT);
            add(&testers[i]);
        }
        add(&aSinc);
    }

    ~cdParallel(void)
    {
        if (testers)
            delete [] testers;
    }
};

void
cdParallelSource::fire(void)
{
    cdParallel * myTest = static_cast<cdParallel*> (myTP_);
    (*myTest->startTime) = getTime();
    for (int i = 0; i < myTest->fanout; i++)
        myTest->testers[i].decDep();
}

void
cdParallelTester::fire(void)
{
    cdParallel * myTest = static_cast<cdParallel*> (myTP_);
    //std::cout << "hello" << std::endl;
    myTest->aSinc.decDep();
}

void
cdParallelSinc::fire(void)
{
    cdParallel * myTest = static_cast<cdParallel*> (myTP_);
    (*myTest->endTime) = getTime();
    myTest->toSignal->decDep();
}

uint64_t CDParallel(Runtime * rt, int, int, int fannout)
{
    uint64_t startTime;
    uint64_t endTime;
    rt->run(launch<cdParallel>(fannout, &startTime, &endTime, &Runtime::finalSignal));
    return endTime-startTime;
}