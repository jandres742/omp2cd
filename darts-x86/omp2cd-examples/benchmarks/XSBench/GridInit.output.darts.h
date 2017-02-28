#ifndef _GridInit_output_darts_h_
#define _GridInit_output_darts_h_
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
void set_grid_ptrs(
    GridPoint* energy_grid, NuclideGridPoint** nuclide_grids, int n_isotopes, int n_gridpoints);
GridPoint* generate_energy_grid(int n_isotopes, int n_gridpoints, NuclideGridPoint** nuclide_grids);
void sort_nuclide_grids(NuclideGridPoint** nuclide_grids, int n_isotopes, int n_gridpoints);
void generate_grids_v(NuclideGridPoint** nuclide_grids, int n_isotopes, int n_gridpoints);
void generate_grids(NuclideGridPoint** nuclide_grids, int n_isotopes, int n_gridpoints);
class TP869;
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
/*TP869: OMPParallelForDirective*/
class TP869 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets869 : public darts::Codelet {
    public:
        TP869* inputsTPParent;
        _barrierCodelets869()
            : darts::Codelet()
        {
        }
        _barrierCodelets869(uint32_t dep, uint32_t res, TP869* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations869(int* endRange, uint32_t codeletID);
    class _checkInCodelets870 : public darts::Codelet {
    public:
        TP869* myTP;
        TP869* inputsTPParent;
        int endRange;
        _checkInCodelets870()
            : darts::Codelet()
        {
        }
        _checkInCodelets870(uint32_t dep, uint32_t res, TP869* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP869* TPParent;
    TP869* controlTPParent;
    TP869* inputsTPParent;
    GridPoint** energy_grid_darts869; /*OMP_SHARED - INPUT*/
    int* mype_darts869; /*OMP_SHARED - INPUT*/
    int* n_gridpoints_darts869; /*OMP_SHARED - INPUT*/
    int* n_isotopes_darts869; /*OMP_SHARED - INPUT*/
    NuclideGridPoint*** nuclide_grids_darts869; /*OMP_SHARED - INPUT*/
    int* i_darts869 /*VARIABLE*/;
    int initIteration869;
    int lastIteration869;
    int range869;
    int rangePerCodelet869;
    int minIteration869;
    int remainderRange869;
    _barrierCodelets869* barrierCodelets869;
    _checkInCodelets870* checkInCodelets870;
    TP869(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet,
        int in_initIteration, int in_lastIteration, GridPoint** in_energy_grid, int* in_mype,
        int* in_n_gridpoints, int* in_n_isotopes, NuclideGridPoint*** in_nuclide_grids);
    ~TP869();
};
#endif
