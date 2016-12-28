#include <stdint.h>
#include "darts.h"
#include "getClock.h"

using namespace darts;

class cdSource : public Codelet
{
public:
    cdSource(uint32_t dep, uint32_t res, ThreadedProcedure * myTP, uint32_t stat):
    Codelet(dep,res,myTP,stat) { }
    virtual void fire(void);
};

class cdSourceSincTester : public Codelet
{
private:
    int id;
public:
    cdSourceSincTester(void):
    Codelet() { }
    void initTester(int Id, uint32_t dep, uint32_t res, ThreadedProcedure * myTP, uint32_t stat)
    {
        id = Id;
        initCodelet(dep,res,myTP,stat);
    }
    virtual void fire(void);
};

class cdSinc : public Codelet
{
public:
    cdSinc(uint32_t dep, uint32_t res, ThreadedProcedure * myTP, uint32_t stat):
    Codelet(dep,res,myTP,stat) { }
    virtual void fire(void);
};

class cdSourceSinc : public ThreadedProcedure
{
public:
    //These are the frame variables
    int fanout;
    
    uint64_t * startTime;
    uint64_t * sourceResults;
    uint64_t * sincResults;
    uint64_t * endTime;
    
    cdSource             Source;
    cdSourceSincTester * Testers;
    cdSinc               Sinc;
    
    Codelet * toSignal;
    
    //Constructor
    cdSourceSinc(int theFanout, uint64_t * theStartTime, uint64_t * theSpawnTime, uint64_t * theJoinTime, uint64_t * theEndTime, Codelet * toSig):
    ThreadedProcedure(),
    fanout(theFanout),
    
    startTime(theStartTime),
    sourceResults(theSpawnTime),
    sincResults(theJoinTime),
    endTime(theEndTime),
    
    Source(0,0,this,SHORTWAIT),
    Testers(0),
    Sinc(theFanout,theFanout,this,SHORTWAIT),
    
    toSignal(toSig) 
    {
        add(&Source);
        Testers = new cdSourceSincTester[theFanout];
        for(int i=0;i<theFanout;i++)
        {
            Testers[i].initTester(i,1,1,this,SHORTWAIT);
            add(&Testers[i]);
        }
        add(&Sinc);
    }
};

void 
cdSource::fire(void)
{
    cdSourceSinc * myTestDep = static_cast<cdSourceSinc*>(myTP_);
    (*myTestDep->startTime) = getTime();
    for(int i=0;i<myTestDep->fanout;i++)
        myTestDep->Testers[i].decDep();
}

void 
cdSourceSincTester::fire(void)
{
    uint64_t time = getTime();
    cdSourceSinc * myTest = static_cast<cdSourceSinc*>(myTP_);
    myTest->sourceResults[id] = time;
    myTest->sincResults[id] = getTime();
    myTest->Sinc.decDep();
}
  
void 
cdSinc::fire(void)
{
    uint64_t time = getTime();
    cdSourceSinc * myTestDep = static_cast<cdSourceSinc*>(myTP_);
    (*myTestDep->endTime) = time;
    myTestDep->toSignal->decDep();
}

uint64_t CDSource(Runtime * rt, int, int, int fannout)
{
    uint64_t startTime;
    uint64_t * sourceTime = new uint64_t[fannout];
    uint64_t * sincTime   = new uint64_t[fannout];
    uint64_t endTime;
    
    rt->run(launch<cdSourceSinc>(fannout,&startTime,sourceTime,sincTime,&endTime,&Runtime::finalSignal));
    
    uint64_t tempSource = 0;
    for(int i=0;i<fannout;i++)
    {
        tempSource+=(sourceTime[i]-startTime);
    }
    
    delete [] sourceTime;
    delete [] sincTime;
    
    return tempSource/fannout;
}

uint64_t CDSinc(Runtime * rt, int, int, int fannout)
{
    uint64_t startTime;
    uint64_t * sourceTime = new uint64_t[fannout];
    uint64_t * sincTime   = new uint64_t[fannout];
    uint64_t endTime;
    
    rt->run(launch<cdSourceSinc>(fannout,&startTime,sourceTime,sincTime,&endTime,&Runtime::finalSignal));
    
    uint64_t tempSinc = 0;
    for(int i=0;i<fannout;i++)
    {
        tempSinc+=(endTime-sincTime[i]);
    }
    
    delete [] sourceTime;
    delete [] sincTime;
    
    return tempSinc/fannout;
}
