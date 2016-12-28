#include "darts.h"
#include "getClock.h"

using namespace darts;

int pow(int base, int power)
{
    int temp = 1;
    for(int i=0;i<power;i++)
    {
        temp*=base;
    }
    return temp;
}

class tpNonStrictStart : public Codelet
{
public:
  tpNonStrictStart(uint32_t dep, uint32_t res, ThreadedProcedure * myTP, uint32_t stat):
  Codelet(dep,res,myTP,stat) { }  
  virtual void fire(void);
};

class tpNonStrictEnd : public Codelet
{
public:
  tpNonStrictEnd(uint32_t dep, uint32_t res, ThreadedProcedure * myTP, uint32_t stat):
  Codelet(dep,res,myTP,stat) { }  
  virtual void fire(void);
};

class tpNonStrictLauncher : public ThreadedProcedure
{
public:
    int depth;
    int fannout;
    
    uint64_t * startTime;
    uint64_t * endTime;
    
    tpNonStrictStart startCD;
    tpNonStrictEnd   endCD;
    //This is who to signal when complete
    Codelet * toSignal;
    //Constructor
    tpNonStrictLauncher(int theDepth, int theFannout, uint64_t * theStartTime, uint64_t * theEndTime, Codelet * toSig):
    ThreadedProcedure(),
    depth(theDepth),
    fannout(theFannout),
    startTime(theStartTime),
    endTime(theEndTime),
    startCD(0,0,this,SHORTWAIT),
    endCD(pow(theFannout,theDepth),pow(theFannout,theDepth),this,SHORTWAIT),
    toSignal(toSig) 
    { 
        add(&startCD);
        add(&endCD);
    }
};

class cd1NonStrict : public Codelet
{
public:
  cd1NonStrict(uint32_t dep, uint32_t res, ThreadedProcedure * myTP, uint32_t stat):
  Codelet(dep,res,myTP,stat) { }
  
  virtual void fire(void);
};

class tpNonStrict : public ThreadedProcedure
{
public:
    //These are the frame variables
    int depth;
    int fanout;
    cd1NonStrict CD1;
    //This is who to signal when complete
    Codelet * toSignal;
    //Constructor
    tpNonStrict(int theDepth, int theFanout, Codelet * toSig):
    ThreadedProcedure(),
    depth(theDepth),
    fanout(theFanout),
    CD1(0,0,this,SHORTWAIT),
    toSignal(toSig) 
    {
        add(&CD1);
    }
};

void 
cd1NonStrict::fire(void)
{
    tpNonStrict * myNonStrict = static_cast<tpNonStrict*>(myTP_);

    if(myNonStrict->depth)
    {
        for(int i = 0; i < myNonStrict->fanout; i++)
        {
            invoke<tpNonStrict>(myNonStrict, myNonStrict->depth-1, myNonStrict->fanout, myNonStrict->toSignal);
        }
    }
    myNonStrict->toSignal->decDep();
}

void
tpNonStrictStart::fire(void)
{
    tpNonStrictLauncher * myLauncher = static_cast<tpNonStrictLauncher*> (myTP_);
    (*myLauncher->startTime) = getTime();
    invoke<tpNonStrict>(myLauncher,myLauncher->depth,myLauncher->fannout,&myLauncher->endCD);
}

void
tpNonStrictEnd::fire(void)
{
    tpNonStrictLauncher * myLauncher = static_cast<tpNonStrictLauncher*> (myTP_);
    (*myLauncher->endTime) = getTime();
    myLauncher->toSignal->decDep();
}

uint64_t TPNonStrict(Runtime * rt, int, int depth, int fannout)
{
    uint64_t startTime;
    uint64_t endTime;
    rt->run(launch<tpNonStrictLauncher>(depth, fannout, &startTime, &endTime, &Runtime::finalSignal));
    return endTime-startTime;
}
