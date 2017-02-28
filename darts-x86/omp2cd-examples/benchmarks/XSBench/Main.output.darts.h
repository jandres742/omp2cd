#ifndef _Main_output_darts_h_
#define _Main_output_darts_h_
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
int main(int argc, char* argv[]);
class TP1173;
class TP1186;
/*Number of TPs to be used for the OMPFor in region TP1186*/
#define NUMTPS1186 NUMTPS
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
/*TP1173: OMPParallelDirective*/
class TP1173 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets1173 : public darts::Codelet {
    public:
        TP1173* inputsTPParent;
        _barrierCodelets1173()
            : darts::Codelet()
        {
        }
        _barrierCodelets1173(uint32_t dep, uint32_t res, TP1173* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets1175 : public darts::Codelet {
    public:
        TP1173* myTP;
        TP1173* inputsTPParent;
        _checkInCodelets1175()
            : darts::Codelet()
        {
        }
        _checkInCodelets1175(uint32_t dep, uint32_t res, TP1173* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets1186 : public darts::Codelet {
    public:
        TP1173* myTP;
        TP1173* inputsTPParent;
        _checkInCodelets1186()
            : darts::Codelet()
        {
        }
        _checkInCodelets1186(uint32_t dep, uint32_t res, TP1173* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets1186 : public darts::Codelet {
    public:
        TP1173* inputsTPParent;
        _barrierCodelets1186()
            : darts::Codelet()
        {
        }
        _barrierCodelets1186(uint32_t dep, uint32_t res, TP1173* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP1173* TPParent;
    TP1173* controlTPParent;
    TP1173* inputsTPParent;
    double*** concs_darts1173; /*OMP_SHARED - INPUT*/
    GridPoint** energy_grid_darts1173; /*OMP_SHARED - INPUT*/
    int* i_darts1173 /*OMP_PRIVATE - INPUT*/;
    int* lookups_darts1173; /*OMP_SHARED - INPUT*/
    int* mat_darts1173 /*OMP_PRIVATE - INPUT*/;
    int*** mats_darts1173; /*OMP_SHARED - INPUT*/
    int* mype_darts1173; /*OMP_SHARED - INPUT*/
    int* n_gridpoints_darts1173; /*OMP_SHARED - INPUT*/
    int* n_isotopes_darts1173; /*OMP_SHARED - INPUT*/
    int* nthreads_darts1173; /*OMP_SHARED - INPUT*/
    NuclideGridPoint*** nuclide_grids_darts1173; /*OMP_SHARED - INPUT*/
    int** num_nucs_darts1173; /*OMP_SHARED - INPUT*/
    double* p_energy_darts1173 /*OMP_PRIVATE - INPUT*/;
    unsigned long* seed_darts1173 /*OMP_PRIVATE - INPUT*/;
    int* thread_darts1173 /*OMP_PRIVATE - INPUT*/;
    double** macro_xs_vector_darts1173 /*VARIABLE*/;
    TP1186** TP1186Ptr;
    size_t* TP1186_alreadyLaunched;
    int numTPsSet1186;
    int numTPsReady1186;
    size_t TPsToUse1186;
    size_t codeletsPerTP1186;
    size_t totalCodelets1186;
    _barrierCodelets1173* barrierCodelets1173;
    _checkInCodelets1175* checkInCodelets1175;
    _checkInCodelets1186* checkInCodelets1186;
    _barrierCodelets1186* barrierCodelets1186;
    TP1173(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet,
        double*** in_concs, GridPoint** in_energy_grid, int* in_lookups, int*** in_mats,
        int* in_mype, int* in_n_gridpoints, int* in_n_isotopes, int* in_nthreads,
        NuclideGridPoint*** in_nuclide_grids, int** in_num_nucs);
    ~TP1173();
};
/*TP1186: OMPForDirective*/
class TP1186 : public ompTP {
public:
    class _barrierCodelets1186 : public darts::Codelet {
    public:
        TP1186* inputsTPParent;
        _barrierCodelets1186()
            : darts::Codelet()
        {
        }
        _barrierCodelets1186(uint32_t dep, uint32_t res, TP1186* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations1186(int* endRange, uint32_t codeletID);
    class _checkInCodelets1187 : public darts::Codelet {
    public:
        TP1186* myTP;
        TP1186* inputsTPParent;
        int endRange;
        _checkInCodelets1187()
            : darts::Codelet()
        {
        }
        _checkInCodelets1187(uint32_t dep, uint32_t res, TP1186* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP1173* TPParent;
    TP1186* controlTPParent;
    TP1186* inputsTPParent;
    double*** concs_darts1186; /*OMP_SHARED - INPUT*/
    GridPoint** energy_grid_darts1186; /*OMP_SHARED - INPUT*/
    int* i_darts1186 /*OMP_PRIVATE - INPUT*/;
    int* lookups_darts1186; /*OMP_SHARED - INPUT*/
    double*** macro_xs_vector_darts1186 /*OMP_SHARED_PRIVATE - INPUT*/;
    int** mat_darts1186 /*OMP_SHARED_PRIVATE - INPUT*/;
    int*** mats_darts1186; /*OMP_SHARED - INPUT*/
    int* mype_darts1186; /*OMP_SHARED - INPUT*/
    int* n_gridpoints_darts1186; /*OMP_SHARED - INPUT*/
    int* n_isotopes_darts1186; /*OMP_SHARED - INPUT*/
    int* nthreads_darts1186; /*OMP_SHARED - INPUT*/
    NuclideGridPoint*** nuclide_grids_darts1186; /*OMP_SHARED - INPUT*/
    int** num_nucs_darts1186; /*OMP_SHARED - INPUT*/
    double** p_energy_darts1186 /*OMP_SHARED_PRIVATE - INPUT*/;
    unsigned long** seed_darts1186 /*OMP_SHARED_PRIVATE - INPUT*/;
    int** thread_darts1186 /*OMP_SHARED_PRIVATE - INPUT*/;
    int initIteration1186;
    int lastIteration1186;
    int nextIteration1186;
    int loop1186alreadySetUp;
    size_t readyCodelets;
    int baseNumThreads;
    _barrierCodelets1186* barrierCodelets1186;
    _checkInCodelets1187* checkInCodelets1187;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets1187* firstCodelet;
#endif
    TP1186(int in_numThreads, int in_mainCodeletID, TP1173* in_TPParent, int in_initIteration,
        int in_lastIteration, double*** in_concs, GridPoint** in_energy_grid, int* in_lookups,
        int*** in_mats, int* in_mype, int* in_n_gridpoints, int* in_n_isotopes, int* in_nthreads,
        NuclideGridPoint*** in_nuclide_grids, int** in_num_nucs, TP1186** in_ptrToThisTP);
    void inline dispatchCodelet(size_t codeletID);
    ~TP1186();
};
#endif
