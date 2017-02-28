#ifndef _adc_output_darts_h_
#define _adc_output_darts_h_
#ifndef __DARTS_
#define __DARTS_
#endif
#include "adc.h"
#include "darts.h"
#include "ompTP.h"
#include "tbb/concurrent_vector.h"
#include "utils.h"
#include <limits.h>
#include <mutex>
#include <numa.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
void ShowADCPar(ADC_PAR* par);
int GenerateADC(ADC_PAR* par);
int GetNextTuple(int dcdim, int measnum, long long* attr, long long* meas, char clss);
int ParseParFile(char* parfname, ADC_PAR* par);
void swap8(void* num);
void initADCpar(ADC_PAR* par);
int CompareSizesByValue(const void* sz0, const void* sz1);
void ShowFactorization(Factorization* nmbfct);
int WriteADCPar(ADC_PAR* par, char* fname);
long ListFirstPrimes(long mpr, long* prlist);
void ExtendFactors(long nmb, long firstdiv, Factorization* nmbfct, Factorization** fctlist);
int CalculateVeiwSizes(ADC_PAR* par);
long long GetLCM(long long mask, Factorization** fctlist, long* adcexpons);
void GetFactorization(long prnum, long* prlist, Factorization** fctlist);
void swap4(void* num);
int CompareViewsBySize(const void* vw0, const void* vw1);
extern int DARTS_CODELETS_MULT;
extern int NUMTPS;
extern size_t numOfCUs;
extern darts::Codelet* RuntimeFinalCodelet;
extern darts::ThreadAffinity* affin;
extern bool affinMaskRes;
extern darts::Runtime* myDARTSRuntime;
extern std::vector<std::vector<void*> > threadFunctionStack;
extern size_t ompNumThreads;
extern int ompSchedulePolicy;
extern int ompScheduleChunk;
extern void omp_set_num_threads(unsigned long numThreadsToSet);
extern int omp_get_num_threads();
extern int omp_get_max_threads();
extern int omp_get_num_procs();
extern double omp_get_wtime();
extern void omp_init_lock(omp_lock_t* lock);
extern void omp_destroy_lock(omp_lock_t* lock);
extern void omp_set_lock(omp_lock_t* lock);
extern void omp_unset_lock(omp_lock_t* lock);
#endif
