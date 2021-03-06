#include "stdint.h"
#include "darts.h"
#include "getClock.h"

using namespace darts;

void serialDelay(int delaylength)
{

    int  i; 
    float a=0.; 
    for (i=0; i<delaylength; i++) a+=i; 
    if (a < 0) std::cout << a; 
} 

class Loop2 : public loop
{
public:
    
    class empty : public Codelet
    {
    private:
        int delay;
        Loop2 * myLoop;
    public:
        empty(uint32_t dep, uint32_t res, ThreadedProcedure * myTP, uint32_t stat, Loop2 * lp, int theDelay):
        Codelet(dep,res,myTP,stat),
        delay(theDelay),
        myLoop(lp) { }

        void fire(void)
        {
            serialDelay(delay);
            myLoop->toSignal->decDep();
        }
    };
    
    empty aCD;
    Loop2(unsigned int it, Codelet * toSig, int delay, int reps):
    loop(it,toSig),
    aCD(0,0,this,SHORTWAIT,this,
       (!it) ? delay/reps + delay%reps : delay/reps)
    { 
        add(&aCD);
    }
};

class serialLoop : public ThreadedProcedure
{
private:
    class timer : public Codelet
    {
    private: 
        uint64_t * tm;
        Codelet * toSignal;
    public:
        timer(uint32_t dep, uint32_t res, ThreadedProcedure * myTP, uint32_t stat, uint64_t * theTime, Codelet * toSig) :
        Codelet(dep, res, myTP, stat),
        tm(theTime),
        toSignal(toSig) { }
        
        virtual void fire(void)
        {
            (*tm) = getTime();
            toSignal->decDep();
        }
    };
public:
    timer ST;
    serialFor<Loop2> ML;
    timer ET;
    
    serialLoop(int theDelay, int theFanout, uint64_t * theStartTime, uint64_t * theEndTime, Codelet * toSig) :
    ThreadedProcedure(),
    ST(0,0,this,SHORTWAIT,theStartTime,&ML),
    ML(1,1,this,SHORTWAIT,&ET,theFanout,theDelay,theFanout),
    ET(1,1,this,SHORTWAIT,theEndTime,toSig)
    {
        add(&ST);
    }

};

uint64_t SerialLoop(Runtime * rt, int delay, int, int fannout)
{
    uint64_t startTime;
    uint64_t endTime;
    rt->run(launch<serialLoop>(delay, fannout, &startTime, &endTime, &Runtime::finalSignal));
    return endTime-startTime;
}
