#define TSU_PRELOAD_FRAME
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <stdint.h>
#include <algorithm>
#include "darts.h"

using namespace darts;

uint64_t tt, t1;
uint64_t ts0[100],ts1[100];

template <class T>
int binSearch(const T arr[], int low, int high, T what)
{
  if(high+1<low)
	high = low;
  else
    high=high+1;
  while (low < high)
  {
    int mid = (low + high) / 2;
    if (arr[mid] < what)
      low = mid+1;
    else
      high = mid;
  }
  return low; // indicate not found 
}

class cd1 : public Codelet
{
public:
  cd1(uint64_t dep, uint64_t res, ThreadedProcedure * myTP, uint64_t stat):
  Codelet(dep, res, myTP, stat) { }
  
  virtual void fire(void);
};

//This codelet is the continuation of the intiated split phase
class cd2 : public Codelet
{
public:
  cd2(uint64_t dep, uint64_t res, ThreadedProcedure * myTP, uint64_t stat):
  Codelet(dep,res,myTP,stat) { }
  
  virtual void fire(void);
};

//This codelet is the continuation of the intiated split phase
class cd3 : public Codelet
{
public:
  cd3(uint64_t dep, uint64_t res, ThreadedProcedure * myTP, uint64_t stat):
  Codelet(dep,res,myTP,stat) { }
  
  virtual void fire(void);
};

//This codelet is the continuation of the intiated split phase
class cd4 : public Codelet
{
public:
  cd4(uint64_t dep, uint64_t res, ThreadedProcedure * myTP, uint64_t stat):
  Codelet(dep,res,myTP,stat) { }
  
  virtual void fire(void);
};

//This is the Fib threaded proceduer
class tree: public ThreadedProcedure
{
	public:
		Codelet * toSignal;
	
	tree(Codelet * toSig):
	ThreadedProcedure(),
	toSignal(toSig)
	{
		
	}
};

class mergesort : public tree
{
public:
    //These are the frame variables
	int *v; int * B; int p; int r; int q;
	
    int * result;
    cd1 check;
    cd2 * merge_step;
	cd4 * merge_final;
    //This is who to signal when complete
    //Codelet * toSignal;
    
    //Constructor

    mergesort(int *v_, int * B_, int p_, int r_, Codelet * toSig):
    v(v_),
	B(B_),
	p(p_),
	r(r_),
    check(0,0,this,SHORTWAIT),
    //merge_step(2,2,this,LONGWAIT),
    tree(toSig)
    {
       add(&check);
    }
};

class merge : public tree
{
public:
    //These are the frame variables
	int *A; int * T; int p1; int r1; int p2; int r2; int p3;
	
    int * result;
	cd3 check;
    cd4 * merge_step;
    //This is who to signal when complete
    //Codelet * toSignal;
    
    //Constructor

    merge(int *A_, int * T_, int p1_, int r1_, int p2_, int r2_, int p3_, Codelet * toSig):
    A(A_),
	T(T_),
	p1(p1_),
	r1(r1_),
	p2(p2_),
	r2(r2_),
	p3(p3_),
    check(0,0,this,SHORTWAIT),
    //merge_step(2,2,this,LONGWAIT),
    tree(toSig)
    {
       add(&check);
    }
};

void
cd1::fire(void)
{
    mergesort * myMerge = static_cast<mergesort*>(myTP_);
	
	if (myMerge->p < myMerge->r) 
	{
		myMerge->q = (myMerge->p + myMerge->r) / 2; 
		
		myMerge->merge_step = new cd2(2,2,myMerge,LONGWAIT);
		invoke<mergesort>(myMerge,myMerge->B,myMerge->v,myMerge->p,myMerge->q,myMerge->merge_step);
		invoke<mergesort>(myMerge,myMerge->B,myMerge->v,myMerge->q+1,myMerge->r,myMerge->merge_step);
	}
	else
	{
		myMerge->toSignal->decDep();
	}
}
 
void 
cd2::fire(void)
{ 
    mergesort * myMerge = static_cast<mergesort*>(myTP_);  
    //(*myMerge->result) = myMerge->x+myMerge->y;
	int *A=myMerge->v; int * T=myMerge->B; int p1=myMerge->p; int r1=myMerge->q; int p2=myMerge->q+1; int r2=myMerge->r; int p3=myMerge->p;
	
	int n1 = r1 - p1+1;
	int n2 = r2 - p2+1;
	
	
	
	if(n1 < n2)
	{
		std::swap(p1, p2);
		std::swap(r1, r2);
		std::swap(n1, n2);
	}
	if(n1 == 0)
	{
		myMerge->toSignal->decDep();
		delete myTP_;
	}
	else
	{
		int q1 =(p1+r1)/2;
		int q2 = binSearch(T,p2,r2, T[q1]);
		int q3 =p3 + (q1 - p1) +(q2-p2);
		A[q3]= T[q1];
		myMerge->merge_final = new cd4(2,2,myMerge,LONGWAIT);
		invoke<merge>(myMerge, A, T, p1, q1-1, p2,q2-1, p3,myMerge->merge_final);
		invoke<merge>(myMerge, A, T, q1+1, r1, q2,r2, q3+1,myMerge->merge_final);
	}
	delete this;
}

void 
cd3::fire(void)
{ 
    merge * myMerge = static_cast<merge*>(myTP_);  
    //(*myMerge->result) = myMerge->x+myMerge->y;
	int *A=myMerge->A; int * T=myMerge->T; int p1=myMerge->p1; int r1=myMerge->r1; int p2=myMerge->p2; int r2=myMerge->r2; int p3=myMerge->p3;
	
	int n1 = r1 - p1+1;
	int n2 = r2 - p2+1;
	
	if(n1 < n2)
	{
		std::swap(p1, p2);
		std::swap(r1, r2);
		std::swap(n1, n2);
	}
	if(n1 == 0)
	{
		myMerge->toSignal->decDep();
		//delete myTP_;
		//delete this;
	}
	else
	{
		int q1 =(p1+r1)/2;
		int q2 = binSearch(T,p2,r2, T[q1]);
		int q3 =p3 + (q1 - p1) +(q2-p2);
		A[q3]= T[q1];
		myMerge->merge_step = new cd4(2,2,myMerge,LONGWAIT);
		invoke<merge>(myMerge, A, T, p1, q1-1, p2,q2-1, p3,myMerge->merge_step);
		invoke<merge>(myMerge, A, T, q1+1, r1, q2,r2, q3+1,myMerge->merge_step);
	}
    
}

void 
cd4::fire(void)
{ 
    tree * myMerge = static_cast<mergesort*>(myTP_); 
	myMerge->toSignal->decDep();
	//delete myTP_;
	delete this;
    //(*myMerge->result) = myMerge->x+myMerge->y;   
}

//Codelet * finalSignal;
int SIZE_N;

class ResultCodelet : public Codelet
{
public:
  ResultCodelet(uint32_t dep, uint32_t res, ThreadedProcedure * myTP, uint32_t stat):
  Codelet(dep,res,myTP,stat)
  {}
  
  virtual void fire(void);
};


class mainTime : public ThreadedProcedure
{
public:
    ResultCodelet res;
	int *a; int *b;
    mainTime():
    res(1,1,this,LONGWAIT)
    {
        //getTime(&start);
		a = new int[SIZE_N];
		b = new int[SIZE_N];
		
		for (int i = 0; i < SIZE_N; ++i)
			a[i] = i;

		std::random_shuffle(a, a + SIZE_N);
		std::copy(a,a+SIZE_N, b);
		
        tt = df_tstamp(ts0);
        invoke<mergesort>(this, a,b,0, SIZE_N-1, &res);
    }

};

void
ResultCodelet::fire()
{
	mainTime * myRes = static_cast<mainTime*>(myTP_); 
    t1 = df_tstamp(ts1);
    double timeTaken = t1 - tt;
	std::cout <<" Done" << std::endl;
    std::cout <<" Time:" << timeTaken << " ns" << std::endl;
	for (int i = 0; i < SIZE_N - 1; ++i) {
        if (myRes->a[i] >= myRes->a[i + 1] || myRes->a[i] != i) {
            std::cout << "Sort failed at location i=" << i << " a[i] = "
                      << myRes->a[i] << " a[i+1] = " << myRes->a[i + 1] << std::endl;
        }
    }
	Runtime::finalSignal.decDep();
	delete[] myRes->a;
	delete[] myRes->b;
}

int
main(int argc, char **argv)
{
   
	if (argc != 2)
    {
        std::cout << "error need a num" << std::endl;
		return 0;
    }

    SIZE_N = atoi(argv[1]);
    
    std::cout << "mergesort(" << SIZE_N << ")" << std::endl;
	
	
	ThreadAffinity affin(3U, 1U, COMPACT, 0, 0);
	if (affin.generateMask())
	{
		  Runtime(&affin).run(launch<mainTime>());
	}

    return 0;
}
