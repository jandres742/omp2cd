#include "darts.h"
#include "getClock.h"

using namespace darts;

class tpStrictStart : public Codelet
{
public:
  tpStrictStart(uint32_t dep, uint32_t res, ThreadedProcedure * myTP, uint32_t stat):
  Codelet(dep,res,myTP,stat) { }  
  virtual void fire(void);
};

class tpStrictEnd : public Codelet
{
public:
  tpStrictEnd(uint32_t dep, uint32_t res, ThreadedProcedure * myTP, uint32_t stat):
  Codelet(dep,res,myTP,stat) { }  
  virtual void fire(void);
};

class tpStrictLauncher : public ThreadedProcedure
{
public:
    int depth;
    int fannout;
    
    uint64_t * startTime;
    uint64_t * endTime;
    
    tpStrictStart startCD;
    tpStrictEnd   endCD;
    //This is who to signal when complete
    Codelet * toSignal;
    //Constructor
    tpStrictLauncher(int theDepth, int theFannout, uint64_t * theStartTime, uint64_t * theEndTime, Codelet * toSig):
    ThreadedProcedure(),
    depth(theDepth),
    fannout(theFannout),
    startTime(theStartTime),
    endTime(theEndTime),
    startCD(0,0,this,SHORTWAIT),
    endCD(1,1,this,SHORTWAIT),
    toSignal(toSig) 
    { 
        add(&startCD);
        add(&endCD);
    }
};

class cd1Strict : public Codelet
{
public:

    cd1Strict(uint32_t dep, uint32_t res, ThreadedProcedure * myTP, uint32_t stat) :
    Codelet(dep, res, myTP, stat){ }

    virtual void fire(void);
};

class cd2Strict : public Codelet
{
public:

    cd2Strict(uint32_t dep, uint32_t res, ThreadedProcedure * myTP, uint32_t stat) :
    Codelet(dep, res, myTP, stat){ }

    virtual void fire(void);
};

class tpStrict : public ThreadedProcedure
{
public:
    //These are the frame variables
    int depth;
    int fanout;
    cd1Strict CD1;
    cd2Strict CD2;
    Codelet * toSignal;
    //Constructor

    tpStrict(int theDepth, int theFanout, Codelet * toSig) :
    ThreadedProcedure(),
    depth(theDepth),
    fanout(theFanout),
    CD1(0, 0, this, SHORTWAIT),
    CD2(theFanout, theFanout, this, LONGWAIT),
    toSignal(toSig)
    {
        add(&CD1);
        add(&CD2);
    }
};

void
cd1Strict::fire(void)
{
    tpStrict * myStrict = static_cast<tpStrict*> (myTP_);
    if (!myStrict->depth)
    {
        myStrict->toSignal->decDep();
    } else
    {
        for (int i = 0; i < myStrict->fanout; i++)
        {
            invoke<tpStrict > (myStrict, myStrict->depth - 1, myStrict->fanout, &myStrict->CD2);
        }
    }
}

void
cd2Strict::fire(void)
{
    tpStrict * myStrict = static_cast<tpStrict*> (myTP_);
    myStrict->toSignal->decDep();
}

void
tpStrictStart::fire(void)
{
    tpStrictLauncher * myLauncher = static_cast<tpStrictLauncher*> (myTP_);
    (*myLauncher->startTime) = getTime();
    invoke<tpStrict>(myLauncher,myLauncher->depth,myLauncher->fannout,&myLauncher->endCD);
}

void
tpStrictEnd::fire(void)
{
    tpStrictLauncher * myLauncher = static_cast<tpStrictLauncher*> (myTP_);
    (*myLauncher->endTime) = getTime();
    myLauncher->toSignal->decDep();
}

uint64_t TPStrict(Runtime * rt, int, int depth, int fannout)
{
    uint64_t startTime;
    uint64_t endTime;
    rt->run(launch<tpStrictLauncher>(depth, fannout, &startTime, &endTime, &Runtime::finalSignal));
    return endTime-startTime;
}
