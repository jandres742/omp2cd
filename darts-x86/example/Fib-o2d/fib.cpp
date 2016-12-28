#include <iostream>
#include <stdlib.h>
#include <stdint.h>
#include "getClock.h"
#include "darts.h"

using namespace darts;

class cd1 : public Codelet
{
public:
  cd1(uint32_t dep, uint32_t res, ThreadedProcedure * myTP, uint32_t stat):
  Codelet(dep,res,myTP,stat) { }
  
  virtual void fire(void);
};

//This codelet is the continuation of the intiated split phase
class cd2 : public Codelet
{
public:
  cd2(uint32_t dep, uint32_t res, ThreadedProcedure * myTP, uint32_t stat):
  Codelet(dep,res,myTP,stat) { }
  
  virtual void fire(void);
};

//This is the Fib threaded proceduer
class fib : public ThreadedProcedure
{
public:
    //These are the frame variables
    int num;
    int x;
    int y;
    int * result;
    cd1 check;
    cd2 adder;
    //This is who to signal when complete
    Codelet * toSignal;
    
    //Constructor
    fib(int n, int * res, Codelet * toSig):
    ThreadedProcedure(),
    num(n),
    x(0),
    y(0),
    result(res),
    check(0,0,this,SHORTWAIT),
    adder(2,2,this,LONGWAIT),
    toSignal(toSig) 
    { 
        add(&check); 
    }
    
    /*~fib(void)
    {
        std::cout << "dieing "<< num << std::endl;
    }*/
    
};


void
cd1::fire(void)
{
    fib * myFib = static_cast<fib*>(myTP_);
    
    //std::cout << "check " << myFib->num << std::endl;
    
    if(myFib->num<2)
    {
        (*myFib->result) = myFib->num;
        myFib->toSignal->decDep();
    }
    else
    {
        invoke<fib>(myFib,myFib->num-1,&myFib->x,&myFib->adder);
        invoke<fib>(myFib,myFib->num-2,&myFib->y,&myFib->adder);
    }
}
 
void 
cd2::fire(void)
{ 
    fib * myFib = static_cast<fib*>(myTP_);    
    //std::cout << "add " << myFib->num << std::endl;
    (*myFib->result) = myFib->x + myFib->y;
    myFib->toSignal->decDep();
}

timespec start, end;
int fibnum;
int result;
uint64_t tt, t1;
uint64_t ts0[100],ts1[100];

class ResultCodelet : public Codelet
{
public:
  ResultCodelet(uint32_t dep, uint32_t res, ThreadedProcedure * myTP, uint32_t stat):
  Codelet(dep,res,myTP,stat) { }
  
  void fire(void)
  {
        t1 = df_tstamp(ts1);
        std::cout << result << std::endl;
        Runtime::finalSignal.decDep();
        //std::cout << "Total Time Taken: " << seconds(end - start) << " sec" << std::endl;
        std::cout << "Total Time Taken: " << t1 - tt << " ns" << std::endl;
  }
};

class mainTime : public ThreadedProcedure
{
public:
    ResultCodelet res;
    mainTime():
    res(1,1,this,LONGWAIT)
    {
        //getTime(&start);
        tt = df_tstamp(ts0);
        invoke<fib >(this, fibnum, &result, &res);
    }

};

int
main(int argc, char *argv[])
{
    //finalSignal = new ResultCodelet;
    //ResultCodelet finalSignal;
    result = 0;
    if (argc != 2)
    {
        std::cout << "error need a num" << std::endl;
        return 0;
    }

    fibnum = atoi(argv[1]);

    ThreadAffinity affin(3U, 1U, COMPACT, 0, 0);
    if (affin.generateMask())
    {
        std::cout << "fib(" << fibnum << ")" << std::endl;

        //Runtime(&affin).run(launch<fib >(fibnum, &result, &Runtime::finalSignal));
        Runtime(&affin).run(launch<mainTime>());
    }

    return 0;
}
