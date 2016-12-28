#include "utils.h"
#include "MatrixVector.h"
#include "darts.h"
#include "rdtsc.h"
#include <cstdio>
#include <cstdlib>
#include <AbstractMachine.h>

using namespace darts;

void feedData (double *vec, double *mat, size_t matSize) {
	unsigned i,j;

	for (i = 0; i < matSize; i++) {
		for (j = 0; j < matSize; j++) {
			mat[i*matSize+j] = drand48 ()*100;
		}
		vec[i] = drand48 ()*100;
	}
}

void printResult (double *res, size_t matSize) {
	unsigned j;
	
	std::cerr << "Print Result" << std::endl; 
	for (j = 0; j < matSize; j++) {
		fprintf (stderr, "%15.5f ", res[j]); 
	}
	std::cerr << std::endl;
}

void sequentialVersion (double *mat, double *vec, double *res, size_t size) {
	double resTmp;
	
	for (unsigned i = 0; i < size; ++i) {
		resTmp = 0.;
		for (unsigned j = 0; j < size; ++j) {
			resTmp += mat[i*size+j] * vec[j];
		}
		res[i] = resTmp;
	}
}

void openMPVersion (double *vec, double *mat, double *res, size_t matSize) {
	#pragma omp parallel for default (none) shared (matSize, mat,vec,res)
	for (unsigned i = 0; i < matSize; ++i) {
		double resTmp = 0.;
		for (unsigned j = 0; j < matSize; ++j) {
			resTmp += mat[i*matSize+j] * vec[j];
		}
		res[i] = resTmp;
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
    mainTime( unsigned repets, double *vec, double *mat, double *res,
							unsigned matSize, unsigned stride):
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
                      invoke<MatVector>(this, vec, mat, res, size, 12, &resu);     
		//start = rdtsc ();
		for (j = 0; j < repets; j++) {
			//dartsVersion (vec, mat, res, size);
                   //if(size < 3)
                      invoke<MatVector>(this, vec, mat, res, size, 12, &resu);      
		}
		//resC = rdtsc () - start;
		//fprintf (fd, "%u %lu\n", size, resC/repets);
	}
    }

};

void dartsVersion (double *vec, double *mat, double *res, size_t size) {
	if (size < 3) return;
        //invoke<MatVector>(vec, mat, res, size, 12, &res));
	//Runtime(1).run(launch<MatVector>(vec, mat, res, size, 12, &Runtime::finalSignal));
}

void launchSequential (FILE *fd, unsigned repets, double *vec, double *mat, double *res,
								unsigned matSize, unsigned stride) {
	unsigned j, size;
	uint64_t start, resC;

	std::cerr << "Sequential :" << std::endl;
	for (size = 0; size < matSize; size+=stride) {
		sequentialVersion (vec, mat, res, size);
		start = rdtsc ();
		for (j = 0; j < repets; j++) {
			sequentialVersion (vec, mat, res, size);
		}
		resC = rdtsc () - start;
		fprintf (fd, "%u %lu\n", size, resC/repets);
	}
	std::cerr << std::endl;
}

void launchOpenMP (FILE *fd, unsigned repets, double *vec, double *mat, double *res,
								unsigned matSize, unsigned stride) {
	unsigned j, size;
	uint64_t start, resC;

	std::cerr << "OpenMP :" << std::endl;
	for (size = 0; size < matSize; size+=stride) {
		openMPVersion (vec, mat, res, size);
		start = rdtsc ();
		for (j = 0; j < repets; j++) {
			openMPVersion (vec, mat, res, size);
		}
		resC = rdtsc () - start;
		fprintf (fd, "%u %lu\n", size, resC/repets);
	}
	std::cerr << std::endl;
}

void launchDarts (FILE *fd, unsigned repets, double *vec, double *mat, double *res,
							unsigned matSize, unsigned stride) {
	unsigned j, size;
	uint64_t start, resC;

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
        Runtime(&affin).run(launch<mainTime>(repets, vec, mat,res, matSize, stride));
	/*std::cerr << "DAR3TS :" << std::endl;
	for (size = 0; size < matSize; size+=stride) {
		dartsVersion (vec, mat, res, size);
		start = rdtsc ();
		for (j = 0; j < repets; j++) {
			dartsVersion (vec, mat, res, size);
		} 
		resC = rdtsc () - start;
		fprintf (fd, "%u %lu\n", size, resC/repets);
	}*/
	std::cerr << std::endl;
}
