#include <stdint.h>
#include "darts.h"
#include "getClock.h"

using namespace darts;

class tpParallelStart : public Codelet
{
public:
  tpParallelStart(uint32_t dep, uint32_t res, ThreadedProcedure * myTP, uint32_t stat):
  Codelet(dep,res,myTP,stat) { }  
  virtual void fire(void);
};

class tpParallelEnd : public Codelet
{
public:
  tpParallelEnd(uint32_t dep, uint32_t res, ThreadedProcedure * myTP, uint32_t stat):
  Codelet(dep,res,myTP,stat) { }  
  virtual void fire(void);
};

class tpParallelLauncher : public ThreadedProcedure
{
public:
    int fannout;
    uint64_t * startTime;
    uint64_t * endTime;
    
    tpParallelStart startCD;
    tpParallelEnd   endCD;
    //This is who to signal when complete
    Codelet * toSignal;
    //Constructor
    tpParallelLauncher(int theFannout, uint64_t * theStartTime, uint64_t * theEndTime, Codelet * toSig):
    ThreadedProcedure(),
    fannout(theFannout),
    startTime(theStartTime),
    endTime(theEndTime),
    startCD(0,0,this,SHORTWAIT),
    endCD(theFannout,theFannout,this,SHORTWAIT),
    toSignal(toSig) 
    { 
        add(&startCD);
        add(&endCD);
    }
};

class tpParallelEmpty : public Codelet
{
public:
  tpParallelEmpty(uint32_t dep, uint32_t res, ThreadedProcedure * myTP, uint32_t stat):
  Codelet(dep,res,myTP,stat) { }  
  virtual void fire(void);
};

class tpParallel : public ThreadedProcedure
{
public:
    tpParallelEmpty empty;
    //This is who to signal when complete
    Codelet * toSignal;
    //Constructor
    tpParallel(Codelet * toSig):
    ThreadedProcedure(),
    empty(0,0,this,SHORTWAIT),
    toSignal(toSig) 
    { 
        add(&empty);
    }
};

void 
tpParallelStart::fire(void)
{
    tpParallelLauncher * myStart = static_cast<tpParallelLauncher*>(myTP_);
    (*myStart->startTime) = getTime();
    for(int i=0;i<myStart->fannout;i++)
        invoke<tpParallel>(myStart,&myStart->endCD);
}

void 
tpParallelEnd::fire(void)
{
    tpParallelLauncher * myEnd = static_cast<tpParallelLauncher*>(myTP_);
    (*myEnd->endTime) = getTime();
    myEnd->toSignal->decDep();
}

void 
tpParallelEmpty::fire(void)
{
    tpParallel * myEmptyTP = static_cast<tpParallel*>(myTP_);
    myEmptyTP->toSignal->decDep();
}

uint64_t TPParallel(Runtime * rt, int, int, int fannout)
{
    uint64_t startTime;
    uint64_t endTime;
    rt->run(launch<tpParallelLauncher>(fannout, &startTime, &endTime, &Runtime::finalSignal));
    return endTime-startTime;
}
