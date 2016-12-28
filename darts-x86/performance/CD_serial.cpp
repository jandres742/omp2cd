#include "stdint.h"
#include "darts.h"
#include "getClock.h"

using namespace darts;

class cdSerialStart : public Codelet
{
public:

    cdSerialStart(uint32_t dep, uint32_t res, ThreadedProcedure * myTP, uint32_t stat) :
    Codelet(dep, res, myTP, stat) { }
    virtual void fire(void);
};

class cdSerialEmpty : public Codelet
{
private:
    Codelet * sig;
public:

    cdSerialEmpty(void) :
    Codelet() { }

    void
    initEmpty(Codelet * toSig, uint32_t dep, uint32_t res, ThreadedProcedure * myTP, uint32_t stat)
    {
        sig = toSig;
        initCodelet(dep, res, myTP, stat);
    }
    virtual void fire(void);
};

class cdSerialEnd : public Codelet
{
public:

    cdSerialEnd(uint32_t dep, uint32_t res, ThreadedProcedure * myTP, uint32_t stat) :
    Codelet(dep, res, myTP, stat) { }
    virtual void fire(void);
};

class cdSerial : public ThreadedProcedure
{
public:
    int depth;

    uint64_t * startTime;
    uint64_t * endTime;

    cdSerialStart startCD;
    cdSerialEmpty * empty;
    cdSerialEnd endCD;
    Codelet * toSignal;

    cdSerial(int theDepth, uint64_t * theStartTime, uint64_t * theEndTime, Codelet * toSig) :
    ThreadedProcedure(),
    depth(theDepth),
    startTime(theStartTime),
    endTime(theEndTime),
    startCD(0, 0, this, SHORTWAIT),
    empty(0),
    endCD(1, 1, this, SHORTWAIT),
    toSignal(toSig)
    {
        add(&startCD);
        empty = new cdSerialEmpty[theDepth];
        for (int i = 0; i < theDepth - 1; i++)
        {
            empty[i].initEmpty(&empty[i + 1], 1, 1, this, SHORTWAIT);
            add(&empty[i]);
        }
        empty[theDepth - 1].initEmpty(&endCD, 1, 1, this, SHORTWAIT);
        add(&empty[theDepth - 1]);
    }
    ~cdSerial(void)
    {
        delete [] empty;
    }
};

void
cdSerialStart::fire(void)
{
    cdSerial * myTest = static_cast<cdSerial*> (myTP_);
    (*myTest->startTime) = getTime();
    myTest->empty[0].decDep();
}

void
cdSerialEmpty::fire(void)
{
    sig->decDep();
}

void
cdSerialEnd::fire(void)
{
    cdSerial * myTest = static_cast<cdSerial*> (myTP_);
    (*myTest->endTime) = getTime();
    myTest->toSignal->decDep();
}

uint64_t CDSerial(Runtime * rt, int, int depth, int)
{
    uint64_t startTime;
    uint64_t endTime;
    rt->run(launch<cdSerial > (depth, &startTime, &endTime, &Runtime::finalSignal));
    return endTime - startTime;
}
