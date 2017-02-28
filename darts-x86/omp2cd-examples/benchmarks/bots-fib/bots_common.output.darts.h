#ifndef _bots_common_output_darts_h_
#define _bots_common_output_darts_h_
#ifndef __DARTS_
#define __DARTS_
#endif
#include "bots.h"
#include "bots_common.h"
#include "bots_main.h"
#include "bots_main.output.darts.h"
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
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/time.h>
#include <sys/utsname.h>
#include <time.h>
#include <time.h>
#include <unistd.h>
#include <unistd.h>
void bots_print_results();
void bots_get_load_average(char* str);
void bots_get_architecture(char* str);
long bots_usecs();
void bots_warning(int warning, char* message);
void bots_get_date(char* str);
void bots_error(int error, char* message);
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
