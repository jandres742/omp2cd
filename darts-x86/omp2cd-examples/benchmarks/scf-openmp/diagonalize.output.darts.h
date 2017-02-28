#ifndef _diagonalize_output_darts_h_
#define _diagonalize_output_darts_h_
#ifndef __DARTS_
#define __DARTS_
#endif
#include "cscc.h"
#include "darts.h"
#include "ompTP.h"
#include "tbb/concurrent_vector.h"
#include "utils.h"
#include <limits.h>
#include <math.h>
#include <mutex>
#include <numa.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
void Eigen_std(double* g_a, double* g_v, double* evals);
void Eigen_gen(double* g_a, double* g_s, double* g_v, double* evals);
int my_rs(int nm, int n, double* a, double* w, double* z, double* fv1, double* fv2);
void my_ga_rrrebak(int nm, int n, double* b, double* dl, int m, double* z);
void my_ga_tred2(int nm, int n, double* a, double* d, double* e, double* z);
int my_ga_tql2(int nm, int n, double* d, double* e, double* z);
int my_ga_rrreduc(int nm, int n, double* a, double* b, double* fv2);
int my_rsg(int nm, int n, double* a, double* b, double* w, double* z, double* fv1, double* fv2);
double my_pythag(double a, double b);
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
