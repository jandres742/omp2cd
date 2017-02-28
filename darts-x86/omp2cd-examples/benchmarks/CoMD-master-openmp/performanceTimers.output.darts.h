#ifndef _performanceTimers_output_darts_h_
#define _performanceTimers_output_darts_h_
#ifndef __DARTS_
#define __DARTS_
#endif
#include "darts.h"
#include "mytype.h"
#include "ompTP.h"
#include "parallel.h"
#include "parallel.output.darts.h"
#include "performanceTimers.h"
#include "performanceTimers.h"
#include "tbb/concurrent_vector.h"
#include "utils.h"
#include "yamlOutput.h"
#include "yamlOutput.output.darts.h"
#include <limits.h>
#include <math.h>
#include <mutex>
#include <numa.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
void printPerformanceResultsYaml(FILE* file);
double getElapsedTime(const enum TimerHandle handle);
void printPerformanceResults(int nGlobalAtoms, int printRate);
void profileStop(const enum TimerHandle handle);
void profileStart(const enum TimerHandle handle);
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
