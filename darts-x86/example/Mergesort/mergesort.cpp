/*CodeletMergesort.cpp 8/21/11
 This is a CodeletMergesort done using codelets and threaded procedures
 Josh Suetterlein
*/

//Still needs to be documented

#include <iostream>
#include <stdlib.h>
#include <Codelet.h>
#include "darts.h"

#ifdef _MSC_VER
	#include <Time.h>
#endif
using namespace darts;

typedef std::vector<int> vint;
typedef std::vector<int>* vintp;

class cd1 : public Codelet
{
public:
  cd1(uint32_t dep, uint32_t res, ThreadedProcedure * myTP, uint32_t stat):
  Codelet(dep,res,myTP,stat){ }
  
  virtual void fire(void);
};

class cd2 : public Codelet
{
public:
  cd2(uint32_t dep, uint32_t res, ThreadedProcedure * myTP, uint32_t stat):
  Codelet(dep,res,myTP,stat){ }
  
  virtual void fire(void);
};

class CodeletMergesort : public ThreadedProcedure
{
public:
    vintp toSort;
    vint left;
    vint right;
    cd1 divide;
    cd2 merge;
    Codelet * toSignal;
    
    CodeletMergesort(vintp tos, Codelet * toSig):
    ThreadedProcedure(),
    toSort(tos),
    divide(0,0,this,SHORTWAIT),
    merge(2,2,this,LONGWAIT),
    toSignal(toSig) 
    { 
        add(&divide);
        add(&merge);
    }
};

void 
cd1::fire(void)
{
    CodeletMergesort * myMerge = static_cast<CodeletMergesort*>(myTP_);

    if(myMerge->toSort->size()==1)
    {
        myMerge->toSignal->decDep();
    }
    else
    {
        myMerge->left.insert(myMerge->left.begin(),myMerge->toSort->begin(),myMerge->toSort->begin()+myMerge->toSort->size()/2);
        myMerge->right.insert(myMerge->right.begin(),myMerge->toSort->begin()+myMerge->toSort->size()/2,myMerge->toSort->end());

        invoke<CodeletMergesort>(myMerge,&myMerge->left,&myMerge->merge);
        invoke<CodeletMergesort>(myMerge,&myMerge->right,&myMerge->merge);
    }
}

void 
cd2::fire(void)
{
    CodeletMergesort * myMerge = static_cast<CodeletMergesort*>(myTP_);
    int j = 0;
    int k = 0;
    int length = myMerge->toSort->size();

    myMerge->toSort->clear();
    for(int i=0;i<length;i++)
    {
        if(j < length/2 && k < length/2 + length%2)
        {
            if(myMerge->left.at(j) < myMerge->right.at(k))
            {
                myMerge->toSort->push_back(myMerge->left.at(j));
                j++;
            }
            else
            {
                myMerge->toSort->push_back(myMerge->right.at(k));
                k++;
            }
        }
        else if(j < length/2)
        {
            myMerge->toSort->push_back(myMerge->left.at(j));
            j++;
        }
        else if(k < length/2 + length%2)
        {
            myMerge->toSort->push_back(myMerge->right.at(k));
            k++;
        }
    }
    myMerge->toSignal->decDep();
}

vint sortit;
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
        //std::cout << result << std::endl;
        Runtime::finalSignal.decDep();
         for(size_t i=1;i<sortit.size();i++)
	 {
	    if(sortit.at(i)<sortit.at(i-1))
	    {
		std::cout<<"FAILURE WILL ROBINSON!!!"<< std::endl;
		break;
	    }
	  }
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
        //invoke<fib >(this, fibnum, &result, &res);
        invoke<CodeletMergesort>(this, &sortit, &res);
    }

};

int
main(int argc, char *argv[])
{
  //Initial TP

  int amount;

  if(argc!=2)
  {
    std::cout << "error need an amount" << std::endl;
    return 0;
  }
  
  amount = atoi(argv[1]);
  
  srand(time(NULL));
  for(int i=0;i<amount;i++)
    sortit.push_back(rand());
   
  std::cout << "Sorting: " << amount << " ints" << std::endl;
  
  //for(size_t i=0;i<sortit.size();i++)
  //{
  //  std::cout << sortit.at(i) << " ";
  //}
  std::cout << std::endl;

  //Runtime().run( launch<CodeletMergesort>(&sortit,&Runtime::finalSignal) );
  ThreadAffinity affin(1U, 16U, COMPACT, 0, 0);
  if (affin.generateMask())
  Runtime(&affin).run(launch<mainTime>());
  
  
  std::cout << "done!" << std::endl;
  
  
  return 0;
}
