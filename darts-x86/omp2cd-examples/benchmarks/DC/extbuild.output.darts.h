#ifndef _extbuild_output_darts_h_
#define _extbuild_output_darts_h_
#ifndef __DARTS_
#define __DARTS_
#endif
#include "darts.h"
#include "extbuild.h"
#include "ompTP.h"
#include "tbb/concurrent_vector.h"
#include "utils.h"
#include <limits.h>
#include <mutex>
#include <numa.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
int32 ViewSizesVerification(ADC_VIEW_CNTL* adccntlp);
void AdcCntlLog(ADC_VIEW_CNTL* adccntlp);
ADC_VIEW_CNTL* NewAdcViewCntl(ADC_VIEW_PARS* adcpp, uint32 pnum);
void AdcFileName(char* adcFileName, const char* adcName, const char* fileName, uint32 taskNumber);
int32 ComputeGivenGroupbys(ADC_VIEW_CNTL* adccntlp);
int32 ReadWholeInputData(ADC_VIEW_CNTL* avp, FILE* inpf);
int32 ComputeMemoryFittedView(ADC_VIEW_CNTL* avp);
int32 SharedSortAggregate(ADC_VIEW_CNTL* avp);
void SeekAndReadNextSubChunk(uint32 multiChunkBuffer[], uint32 k, FILE* inFile, uint32 chunkRecSize,
    uint64 inFileOffs, uint32 subChunkNum);
void InitAdcViewCntl(
    ADC_VIEW_CNTL* adccntl, uint32 nSelectedDims, uint32* selection, uint32 fromParent);
int32 PrefixedAggregate(ADC_VIEW_CNTL* avp, FILE* iof);
void ReadSubChunk(uint32 chunkRecSize, uint32* multiChunkBuffer, uint32 mwBufRecSizeInInt,
    uint32 iChunk, uint32 regSubChunkSize, CHUNKS* chunks, FILE* fileOfChunks);
int32 CloseAdcView(ADC_VIEW_CNTL* adccntl);
int32 RunFormation(ADC_VIEW_CNTL* avp, FILE* inpf);
int32 MultiWayMerge(ADC_VIEW_CNTL* avp);
void SelectToView(uint32* ib, uint32* ix, uint32* viewBuf, uint32 nd, uint32 nm, uint32 nv);
FILE* AdcFileOpen(const char* fileName, const char* mode);
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
