#ifndef _haloExchange_output_darts_h_
#define _haloExchange_output_darts_h_
#ifndef __DARTS_
#define __DARTS_
#endif
#include "constants.h"
#include "darts.h"
#include "decomposition.h"
#include "decomposition.output.darts.h"
#include "haloExchange.h"
#include "mytype.h"
#include "ompTP.h"
#include "parallel.h"
#include "parallel.output.darts.h"
#include "performance.h"
#include "performance.output.darts.h"
#include "sparseMatrix.h"
#include "sparseMatrix.output.darts.h"
#include "tbb/concurrent_vector.h"
#include "utils.h"
#include <cassert>
#include <limits.h>
#include <mutex>
#include <numa.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
void destroyHaloExchange(struct HaloExchangeSt* haloExchange);
void unloadBuffer(char* buf, int bufSize, struct SparseMatrixSt* xmatrix, struct DomainSt* domain);
void exchangeSetup(
    struct HaloExchangeSt* haloExchange, struct SparseMatrixSt* spmatrix, struct DomainSt* domain);
void exchangeData(
    struct HaloExchangeSt* haloExchange, struct SparseMatrixSt* spmatrix, struct DomainSt* domain);
void updateData(
    struct HaloExchangeSt* haloExchange, struct SparseMatrixSt* spmatrix, struct DomainSt* domain);
void gatherData(
    struct HaloExchangeSt* haloExchange, struct SparseMatrixSt* spmatrix, struct DomainSt* domain);
HaloExchange* initHaloExchange(struct DomainSt* domain);
void addHaloProc(struct HaloExchangeSt* haloExchange, int rproc);
void allGatherData(
    struct HaloExchangeSt* haloExchange, struct SparseMatrixSt* spmatrix, struct DomainSt* domain);
int isHaloProc(struct HaloExchangeSt* haloExchange, int rproc);
int loadBuffer(char* buf, struct SparseMatrixSt* xmatrix, struct DomainSt* domain);
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
