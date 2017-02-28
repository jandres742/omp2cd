#ifndef _CalculateXS_output_darts_h_
#define _CalculateXS_output_darts_h_
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
int grid_search(int n, double quarry, GridPoint* A);
void calculate_macro_xs(double p_energy, int mat, int n_isotopes, int n_gridpoints,
    int* restrict num_nucs, double** restrict concs, GridPoint* restrict energy_grid,
    NuclideGridPoint** restrict nuclide_grids, int** restrict mats,
    double* restrict macro_xs_vector);
void calculate_micro_xs(double p_energy, int nuc, int n_isotopes, int n_gridpoints,
    GridPoint* restrict energy_grid, NuclideGridPoint** restrict nuclide_grids, int idx,
    double* restrict xs_vector);
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
