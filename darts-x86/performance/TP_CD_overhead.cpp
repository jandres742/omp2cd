#include <iostream>
#include "darts.h"
#include "getClock.h"
#include "include/delay.h"

using namespace darts;

class launchCD : public Codelet
{
public:
    launchCD(uint32_t dep, uint32_t res, ThreadedProcedure * myTP, uint32_t stat):
    Codelet(dep,res,myTP,stat) { }
    virtual void fire(void);
};

class delayCD : public Codelet
{
public:
    delayCD(void): Codelet() { }
  delayCD(uint32_t dep, uint32_t res, ThreadedProcedure * myTP, uint32_t stat):
  Codelet(dep,res,myTP,stat) { }  
  virtual void fire(void);
};

class endCD : public Codelet
{
public:
  endCD(uint32_t dep, uint32_t res, ThreadedProcedure * myTP, uint32_t stat):
  Codelet(dep,res,myTP,stat) { }  
  virtual void fire(void);
};

class tpOverhead : public ThreadedProcedure
{
public:
    int delay;
    int depth;
    int fannout;
    launchCD   launcher;
    delayCD  * delayer;
    endCD      ender;
    Codelet * toSignal;
    tpOverhead(int theDelay, int theDepth, int theFannout, Codelet * toSig):
    ThreadedProcedure(),
    delay(theDelay),
    depth(theDepth),
    fannout(theFannout),
    launcher(0,0,this,SHORTWAIT),
    delayer(0),
    ender((theDelay%theFannout) ? theFannout+1 : theFannout,
          (theDelay%theFannout) ? theFannout+1 : theFannout,
          this,SHORTWAIT),
    toSignal(toSig) 
    {
        if(theDepth)
        {
            add(&launcher);
        }
        else
        {
            int tempCDS = (theDelay%theFannout) ? theFannout+1 : theFannout;
            delayer = new delayCD[tempCDS];
            for(int i=0;i<tempCDS;i++)
            {
                delayer[i].initCodelet(0,0,this,SHORTWAIT);
                add(&delayer[i]);
            }
        }
        add(&ender);
    }
    
    ~tpOverhead(void)
    {
        delete [] delayer;
    }
};

void
launchCD::fire(void)
{
    tpOverhead * myOverhead = static_cast<tpOverhead*> (myTP_);
    for(int i=0;i<myOverhead->fannout-1;i++)
    {
      invoke<tpOverhead>(myOverhead,myOverhead->delay/myOverhead->fannout,myOverhead->depth-1,myOverhead->fannout,&myOverhead->ender);
    }
    invoke<tpOverhead>(myOverhead,myOverhead->delay/myOverhead->fannout+myOverhead->delay%myOverhead->fannout,myOverhead->depth-1,myOverhead->fannout,&myOverhead->ender);
}

void
delayCD::fire(void)
{
    tpOverhead * myOverhead = static_cast<tpOverhead*> (myTP_);
    delayLoop(myOverhead->delay/myOverhead->fannout);
    myOverhead->ender.decDep();
}

void
endCD::fire(void)
{
    tpOverhead * myOverhead = static_cast<tpOverhead*> (myTP_);
    myOverhead->toSignal->decDep();
}

uint64_t TPCDOverhead(Runtime * rt, int delay, int depth, int fannout)
{
    uint64_t startTime;
    uint64_t endTime;
    startTime = getTime();
    rt->run(launch<tpOverhead>(delay, depth, fannout, &Runtime::finalSignal));
    endTime = getTime();
    return endTime-startTime;
}

uint64_t SerialOverhead(Runtime *, int delay, int, int)
{
    uint64_t startTime;
    uint64_t endTime;
    startTime = getTime();
    delayLoop(delay);
    endTime = getTime();
    return endTime-startTime;
}