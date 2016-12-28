#include <iostream>
#include <stdlib.h>
#include <stdint.h>
#include "getClock.h"
#include "darts.h"
#include "rdtsc.h"

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
    cd2  adder;
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
    
    ~fib(void)
    {
    }
};

void
cd1::fire(void)
{
    fib * myFib = static_cast<fib*>(myTP_);
    
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
    (*myFib->result) = myFib->x + myFib->y;
    myFib->toSignal->decDep();
}

int
main(int argc, char *argv[])
{
  int fibnum;
  int result = 0;
  timespec start, end;

  if(argc!=2)
  {
    std::cout << "error need a num" << std::endl;
    return 0;
  }
  
  fibnum = atoi(argv[1]);
 
  std::cout << "fib(" << fibnum << ")" << std::endl;

  ThreadAffinity *affin;
  Runtime *myDARTSRuntime;
  affin = new ThreadAffinity (3, 3, COMPACT, 0, 0);
  bool affinMaskRes = affin->generateMask ();
  myDARTSRuntime = new Runtime (affin);
  
  getTime(&start);
  //~ Runtime(8,0).run(launch<fib>(fibnum,&result, &Runtime::finalSignal));
  myDARTSRuntime->run(launch<fib>(fibnum,&result, &Runtime::finalSignal));
  getTime(&end);
  
  //~ std::cout << result << std::endl;  
  std::cout << seconds(end-start) << std::endl;
  //~ std::cout << "Total Time Taken: " <<  seconds(end-start) << " sec" << std::endl;
  return 0;
}

