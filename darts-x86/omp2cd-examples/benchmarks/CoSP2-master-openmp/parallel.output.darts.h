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
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <time.h>
#include <unistd.h>
void addIntReduce2(int* value0, int* value1);
void maxIntReduce2(int* value0, int* value1);
void maxRealReduce(real_t* value);
void minRealReduce(real_t* value);
void maxRankDoubleParallel(RankReduceData* sendBuf, RankReduceData* recvBuf, int count);
void addRealReduce2(real_t* value0, real_t* value1);
void minRankDoubleParallel(RankReduceData* sendBuf, RankReduceData* recvBuf, int count);
int testIrecv(int rind);
void minRealParallel(real_t* sendBuf, real_t* recvBuf, int count);
void maxIntParallel(int* sendBuf, int* recvBuf, int count);
int printRank();
void initParallel(int* argc, char*** argv);
void barrierParallel();
int isendParallel(void* sendBuf, int sendLen, int dest);
void timestampBarrier(const char* msg);
int getNRanks();
int sendParallel(void* sendBuf, int sendLen, int dest);
void addRealParallel(real_t* sendBuf, real_t* recvBuf, int count);
int builtWithMpi();
void maxRealParallel(real_t* sendBuf, real_t* recvBuf, int count);
int getMyRank();
void bcastParallel(void* buf, int count, int root);
int irecvAnyParallel(void* recvBuf, int recvLen);
int waitIrecv(int rind);
int sendReceiveParallel(
    void* sendBuf, int sendLen, int dest, void* recvBuf, int recvLen, int source);
int waitIsend(int rind);
void destroyParallel();
int recvAnyParallel(void* recvBuf, int recvLen);
int testIsend(int rind);
int recvParallel(void* recvBuf, int recvLen, int source);
int irecvParallel(void* recvBuf, int recvLen, int source);
void addIntParallel(int* sendBuf, int* recvBuf, int count);
void addDoubleParallel(double* sendBuf, double* recvBuf, int count);
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
