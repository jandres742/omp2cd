#ifndef _parallel_output_darts_h_
#define _parallel_output_darts_h_
#ifndef __DARTS_
#define __DARTS_
#endif
#include "darts.h"
#include "ompTP.h"
#include "parallel.h"
#include "tbb/concurrent_vector.h"
#include "utils.h"
#include <cassert>
#include <limits.h>
#include <mutex>
#include <numa.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <time.h>
#include <unistd.h>
void bcastParallel(void* buf, int count, int root);
void maxRankDoubleParallel(RankReduceData* sendBuf, RankReduceData* recvBuf, int count);
void minRankDoubleParallel(RankReduceData* sendBuf, RankReduceData* recvBuf, int count);
void addDoubleParallel(double* sendBuf, double* recvBuf, int count);
int getMyRank();
void timestampBarrier(const char* msg);
int builtWithMpi();
void initParallel(int* argc, char*** argv);
void addIntParallel(int* sendBuf, int* recvBuf, int count);
void destroyParallel();
void addRealParallel(real_t* sendBuf, real_t* recvBuf, int count);
int getNRanks();
int printRank();
void barrierParallel();
void maxIntParallel(int* sendBuf, int* recvBuf, int count);
int sendReceiveParallel(
    void* sendBuf, int sendLen, int dest, void* recvBuf, int recvLen, int source);
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
