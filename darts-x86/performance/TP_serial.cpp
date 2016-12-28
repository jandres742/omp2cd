#include <stdlib.h>
#include "darts.h"
#include "getClock.h"

using namespace darts;

class tpSerialStart : public Codelet
{
public:
  tpSerialStart(uint32_t dep, uint32_t res, ThreadedProcedure * myTP, uint32_t stat):
  Codelet(dep,res,myTP,stat) { }  
  virtual void fire(void);
};

class tpSerialEnd : public Codelet
{
public:
  tpSerialEnd(uint32_t dep, uint32_t res, ThreadedProcedure * myTP, uint32_t stat):
  Codelet(dep,res,myTP,stat) { }  
  virtual void fire(void);
};

class tpSerialLauncher : public ThreadedProcedure
{
public:
    int depth;
    uint64_t * startTime;
    uint64_t * endTime;
    
    tpSerialStart startCD;
    tpSerialEnd   endCD;
    //This is who to signal when complete
    Codelet * toSignal;
    //Constructor
    tpSerialLauncher(int theDepth, uint64_t * theStartTime, uint64_t * theEndTime, Codelet * toSig):
    ThreadedProcedure(),
    depth(theDepth),
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

class tpSerialEmpty : public Codelet
{
public:
  tpSerialEmpty(uint32_t dep, uint32_t res, ThreadedProcedure * myTP, uint32_t stat):
  Codelet(dep,res,myTP,stat) { }
  virtual void fire(void);
};

class tpSerial : public ThreadedProcedure
{
public:
    int depth;
    tpSerialEmpty empty;
    Codelet * toSignal;
    tpSerial(int theDepth, Codelet * toSig):
    ThreadedProcedure(),
    depth(theDepth),
    empty(0,0,this,SHORTWAIT),
    toSignal(toSig) 
    { 
        add(&empty);
    }
};

void 
tpSerialEmpty::fire(void)
{
    tpSerial * mySerial = static_cast<tpSerial*>(myTP_);
    if(!mySerial->depth)
    {
        mySerial->toSignal->decDep();
    }
    else
    {
        invoke<tpSerial>(mySerial,mySerial->depth-1,mySerial->toSignal);
    }
}

void
tpSerialStart::fire(void)
{
    tpSerialLauncher * myLauncher = static_cast<tpSerialLauncher*> (myTP_);
    (*myLauncher->startTime) = getTime();
    invoke<tpSerial>(myLauncher,myLauncher->depth,&myLauncher->endCD);
}

void
tpSerialEnd::fire(void)
{
    tpSerialLauncher * myLauncher = static_cast<tpSerialLauncher*> (myTP_);
    (*myLauncher->endTime) = getTime();
    myLauncher->toSignal->decDep();
}

uint64_t TPSerial(Runtime * rt, int, int depth, int)
{
    uint64_t startTime;
    uint64_t endTime;
    rt->run(launch<tpSerialLauncher>(depth, &startTime, &endTime, &Runtime::finalSignal));
    return endTime-startTime;
}