#ifndef _jobcntl_output_darts_h_
#define _jobcntl_output_darts_h_
#ifndef __DARTS_
#define __DARTS_
#endif
#include "darts.h"
#include "jobcntl.h"
#include "ompTP.h"
#include "tbb/concurrent_vector.h"
#include "utils.h"
#include <limits.h>
#include <mutex>
#include <numa.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
int32 PartitionCube(ADC_VIEW_CNTL* avp);
uint32 MultiFileProcJobs(TUPLE_VIEWSIZE* tuplesAndSizes, uint32 nViews, ADC_VIEW_CNTL* avp);
void onessort(TUPLE_ONES x[], uint32 n);
void restoreo(TUPLE_ONES x[], uint32 f, uint32 l);
uint32 countTupleOnes(uint64 binRepTuple, uint32 numDims);
void vszsort(TUPLE_VIEWSIZE x[], uint32 n);
uint32 NumOfCombsFromNbyK(uint32 n, uint32 k);
void WriteOne32Tuple(char* t, uint32 s, uint32 l, FILE* logf);
uint32 Mlo32(uint32 x);
uint32 GetSmallestParent(ADC_VIEW_CNTL* avp, uint32 binRepTuple);
uint32 setLeadingOnes32(uint32 n);
void SetOneBit32(uint32* s, uint32 pos);
int32 GetPrefixedParent(ADC_VIEW_CNTL* avp, uint32 binRepTuple);
void WriteOne64Tuple(char* t, uint64 s, uint32 l, FILE* logf);
void JobPoolUpdate(ADC_VIEW_CNTL* avp);
void SetOneBit(uint64* s, int32 pos);
int32 GetParent(ADC_VIEW_CNTL* avp, uint32 binRepTuple);
uint32 NumberOfOnes(uint64 s);
int32 DeleteOneFile(const char* file_name);
void JobPoolInit(JOB_POOL* jpp, uint32 n, uint32 nd);
void d32v(char* t, uint32* v, uint32 n);
int32 mro32(uint32 x);
void GetRegTupleFromBin64(uint64 binRepTuple, uint32* selTuple, uint32 numDims, uint32* numOfUnits);
void CreateBinTuple(uint64* binRepTuple, uint32* selTuple, uint32 numDims);
void getRegTupleFromBin32(uint32 binRepTuple, uint32* selTuple, uint32 numDims, uint32* numOfUnits);
void GetRegTupleFromParent(uint64 bin64RepTuple, uint32 bin32RepTuple, uint32* selTuple, uint32 nd);
int32 Comp8gbuf(const void* a, const void* b);
void restore(TUPLE_VIEWSIZE x[], uint32 f, uint32 l);
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
