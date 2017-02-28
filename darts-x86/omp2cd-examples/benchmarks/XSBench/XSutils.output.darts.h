#ifndef _XSutils_output_darts_h_
#define _XSutils_output_darts_h_
#ifndef __DARTS_
#define __DARTS_
#endif
#include "XSbench_header.h"
#include "darts.h"
#include "ompTP.h"
#include "tbb/concurrent_vector.h"
#include "utils.h"
#include <limits.h>
#include <mutex>
#include <numa.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
void gpmatrix_free(NuclideGridPoint** M);
int NGP_compare(const void* a, const void* b);
Inputs read_CLI(int argc, char* argv[]);
void center_print(const char* s, int width);
double rn_v();
void border_print();
void fancy_int(int a);
double rn(unsigned long* seed);
unsigned int hash(unsigned char* str, int nbins);
void print_CLI_error();
NuclideGridPoint** gpmatrix(size_t m, size_t n);
void logo(int version);
int binary_search(NuclideGridPoint* A, double quarry, int n);
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
