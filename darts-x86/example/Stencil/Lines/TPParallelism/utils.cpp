#include <cstdlib>
#include <cstdio>
#include <iostream>
#include "utils.h"
#include "Stencil.h"
#include "StencilWrapper.h"
#include "SwapWrapper.h"
#include "rdtsc.h"
#include "darts.h"


using namespace darts;

static inline void printPercentage (unsigned currentSize) {
	std::cerr << "\r                                              ";
	std::cerr << "\rProblem Size : " << currentSize << " (" << ((double)currentSize/(MAX_MATSIZE+1))*100 << "%)";
}

void feedData (double *oldA, size_t matSize) {
	unsigned i,j;

	for (i = 0; i < matSize; i++) {
		for (j = 0; j < matSize; j++) {
			oldA [i*matSize+j] = drand48 ()*100;
		}
	}
}

void printResult (double *oldA, size_t matSize) {
	unsigned i,j;
	
	std::cerr << "Print Matrix" << std::endl;
	for (i = 0; i < matSize; i++) {
		for (j = 0; j < matSize; j++) {
			fprintf (stderr, "%15.5f ", oldA[i*matSize+j]);
		}
		std::cerr << std::endl;
	}
}

void sequentialVersion (double *oldA, double *newA, size_t matSize) {
	size_t i,j,k;
	double *tmpA;

	if (matSize < 3) return;

	for (k = 0; k < MAX_REC_DEPTH; k++) {
		for (i = 1; i < matSize-1; i++) {
			for (j = 1; j < matSize-1; j++) {
				newA[i*matSize+j] = oldA[(i-1)*matSize+j] + oldA [(i+1)*matSize+j] + oldA [i*matSize + j - 1] + oldA [i*matSize + j + 1]; 
			}
		}
	 tmpA = oldA; /* Swap */
	 oldA = newA;
	 newA = tmpA;
	}
}

void openMPVersion (double *oldA, double *newA, size_t matSize) {
	size_t k;

	if (matSize < 3) return;

	for (k = 0; k < MAX_REC_DEPTH; k++) {
			#pragma omp parallel for default(none) shared(newA,oldA,matSize)
			for (size_t i = 1; i < matSize-1; i++) {
				for (size_t j = 1; j < matSize-1; j++) {
					newA[i*matSize+j] = oldA[(i-1)*matSize+j] + oldA [(i+1)*matSize+j] + oldA [i*matSize + j - 1] + oldA [i*matSize + j + 1]; 
				}
			}
			//#pragma omp single nowait
			{
				double *tmpA = oldA; /* Swap */
				oldA = newA;
				newA = tmpA;
			}
	}
}

timespec start, end;
int fibnum;
int result;
uint64_t signals;
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
        //std::cout << "Total Time Taken: " << seconds(end - start) << " sec" << std::endl;
        std::cout << "Total Time Taken: " << t1 - tt << " ns" << std::endl;
  }
};

class mainTime : public ThreadedProcedure
{
public:
    ResultCodelet resu;
    mainTime( unsigned repets, double *oldA, double *newA, unsigned matSize, unsigned stride):
    resu(signals,signals,this,LONGWAIT)
    {
        //getTime(&start);
        
        //invoke<fib >(this, fibnum, &result, &res);
        unsigned j, size;
	uint64_t start, resC;
        std::cerr << "DAR3TS :" << std::endl;
        //timer.start();
        tt = df_tstamp(ts0);
	for (size = 3; size < matSize; size+=stride) {
		//dartsVersion (vec, mat, res, size);
                //if(size < 3)
                      invoke<StencilWrapper>(this, oldA, newA, size, 0, 7, 12, &resu);     
		//start = rdtsc ();
		for (j = 0; j < repets; j++) {
			//dartsVersion (vec, mat, res, size);
                   //if(size < 3)
                      invoke<StencilWrapper>(this, oldA, newA, size, 0, 7, 12, &resu);      
		}
		//resC = rdtsc () - start;
		//fprintf (fd, "%u %lu\n", size, resC/repets);
	}
    }

};

void dartsVersion (double *oldA, double *newA, size_t size) {
	if (size < 3) return;
	//std::cerr << am.getNbClusters () << "," << 1 << "," << nbMuScheds << std::endl;
	Runtime().run(launch<StencilWrapper>(oldA, newA, size, 0, 7, 12, &Runtime::finalSignal));
}

void launchSequential (FILE *fd, unsigned repets, double *oldA, double *newA, unsigned matSize, unsigned stride) {
	unsigned j, size;
	uint64_t start, res;

	std::cerr << "Sequential :" << std::endl;
	for (size = 3; size < matSize; size+=stride) {
		//printPercentage (size);
		sequentialVersion (oldA, newA, size);
		start = rdtsc ();
		for (j = 0; j < repets; j++) {
			sequentialVersion (oldA, newA, size);
		}
		res = rdtsc () - start;
		fprintf (fd, "%u %lu\n", size, res/repets);
	}
	//printPercentage (MAX_MATSIZE+1);
	std::cerr << std::endl;
	//exit (EXIT_FAILURE);
}

void launchOpenMP (FILE *fd, unsigned repets, double *oldA, double *newA, unsigned matSize, unsigned stride) {
	unsigned j, size;
	uint64_t start, res;

	std::cerr << "OpenMP :" << std::endl;
	for (size = 3; size < matSize; size+=stride) {
		//printPercentage (size);
		openMPVersion (oldA, newA, size);
		//std::cerr << "Computing OpenMP size " << size << std::endl;
		start = rdtsc ();
		for (j = 0; j < repets; j++) {
			openMPVersion (oldA, newA, size);
		}
		//std::cerr << "Finished Repetitions..." << std::endl;
		res = rdtsc () - start;
		fprintf (fd, "%u %lu\n", size, res/repets);
	}
	//printPercentage (MAX_MATSIZE+1);
	std::cerr << std::endl;
}

void launchDarts (FILE *fd, unsigned repets, double *oldA, double *newA, unsigned matSize, unsigned stride) {
	unsigned j, size;
	uint64_t start, res;

        int add =1;
        int sub=1;
        if(matSize%stride == 0)
	  add=0;
        if(stride == 2)
          sub = 2;
        else if (stride == 1 )
          sub = 3;
        
        signals = (matSize/stride+add-sub)+(matSize/stride+add-sub)*repets;
        ThreadAffinity affin(1U, 16U, COMPACT, 0, 0);
        if (affin.generateMask())
        Runtime(&affin).run(launch<mainTime>(repets, oldA, newA, matSize, stride));

	/*std::cerr << "DAR3TS :" << std::endl;
	for (size = 3; size < matSize; size+=stride) {
		//printPercentage (size);
		dartsVersion (oldA, newA, size);
		start = rdtsc ();
		for (j = 0; j < repets; j++) {
			dartsVersion (oldA, newA, size);
		}
		res = rdtsc () - start;
		fprintf (fd, "%u %lu\n", size, res/repets);
	}*/
	//printPercentage (MAX_MATSIZE+1);
	std::cerr << std::endl;
}
