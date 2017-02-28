#ifndef _rbt_output_darts_h_
#define _rbt_output_darts_h_
#ifndef __DARTS_
#define __DARTS_
#endif
#include "adc.h"
#include "adc.output.darts.h"
#include "darts.h"
#include "macrodef.h"
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
int32 DestroyTree(RBTree* tree);
void InitializeTree(RBTree* tree, uint32 nd, uint32 nm);
RBTree* CreateEmptyTree(uint32 nd, uint32 nm, uint32 memoryLimit, unsigned char* memPool);
int32 computeChecksum(ADC_VIEW_CNTL* avp, treeNode* t, uint64* ordern);
int32 WriteViewToDisk(ADC_VIEW_CNTL* avp, treeNode* t);
int32 TreeInsert(RBTree* tree, uint32* attrs);
int32 WriteChunkToDisk(uint32 recordSize, FILE* fileOfChunks, treeNode* t, FILE* logFile);
int32 WriteViewToDiskCS(ADC_VIEW_CNTL* avp, treeNode* t, uint64* ordern);
int32 KeyComp(const uint32* a, const uint32* b, uint32 n);
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
