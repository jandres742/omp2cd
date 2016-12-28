/*Joshua Suetterlein
 This application tests how long it takes
 to initialize the runtime and run a TP with
 one empty codelet*/

#include <iostream>
#include "darts.h"
#include "getClock.h"

using namespace darts;

class emptyCD : public Codelet
{
public:
  emptyCD(uint32_t dep, uint32_t res, ThreadedProcedure * myTP, uint32_t stat):
  Codelet(dep,res,myTP,stat) { }
  
  virtual void fire(void);
};

class emptyTP : public ThreadedProcedure
{
public:
    emptyCD empty;
    //This is who to signal when complete
    Codelet * toSignal;
    //Constructor
    emptyTP(Codelet * toSig):
    ThreadedProcedure(),
    empty(0,0,this,SHORTWAIT),
    toSignal(toSig) 
    {
        add(&empty);
    }
};

void 
emptyCD::fire(void)
{
    emptyTP * myEmptyTP = static_cast<emptyTP*>(myTP_);
    myEmptyTP->toSignal->decDep();
}

uint64_t Init(Runtime * rt, int, int, int)
{
    uint64_t startTime;
    uint64_t endTime;
    startTime = getTime();
    rt->run(launch<emptyTP>(&Runtime::finalSignal));
    endTime = getTime();
    return endTime-startTime;
}
