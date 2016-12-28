#include <iostream>
#include <iomanip>
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
        add(&adder);
    }
    
    //~fib(void)
    //{
    //    std::cout << "dieing "<< num << std::endl;
    //}
    
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

int
main(int argc, char *argv[])
{
  int fibnum;
  int result = 0;
  uint64_t start, end;

  if(argc!=2)
  {
    std::cout << "error need a num" << std::endl;
    return 0;
  }
  
  fibnum = atoi(argv[1]);
  Runtime rt;
  for(int i = 0; i<fibnum+1; i++)
  {
    result = 0;
    std::cout << "fib(" << std::setw (2) << i << ") ";
    start = getTime();
    rt.run(launch<fib>(i,&result, &Runtime::finalSignal));
    end = getTime();
    std::cout << std::setw (6) << result;
    std::cout << " Total Time Taken: " << std::setw (12) << (end-start)/1000000 << " ms" << std::endl;
  }
  return 0;
}
