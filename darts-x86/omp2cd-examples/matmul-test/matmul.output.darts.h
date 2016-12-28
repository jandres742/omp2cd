#ifndef _matmul_output_darts_h_
#define _matmul_output_darts_h_
#ifndef __DARTS_
#define __DARTS_
#endif
#include "darts.h"
#include "matmul.h"
#include "ompTP.h"
#include "tbb/concurrent_vector.h"
#include "utils.h"
#include <limits.h>
#include <mutex>
#include <numa.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
int main(int argc, char** argv);
int functB();
class TP150;
class TP154;
/*Number of TPs to be used for the OMPFor in region TP154*/
#define NUMTPS154 NUMTPS
/*Limit for which only one TP154 region is launched with only one codelet*/
#define RANGELIMIT154_0 10
/*Limit for which half of TP154 regions are launched with half of codelets*/
#define RANGELIMIT154_1 100
extern int DARTS_CODELETS_MULT;
extern int NUMTPS;
extern size_t numOfCUs;
extern darts::Codelet* RuntimeFinalCodelet;
extern darts::ThreadAffinity* affin;
extern bool affinMaskRes;
extern darts::Runtime* myDARTSRuntime;
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
/*TP150: OMPParallelDirective*/
class TP150 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets150 : public darts::Codelet {
    public:
        _barrierCodelets150()
            : darts::Codelet()
        {
        }
        _barrierCodelets150(uint32_t dep, uint32_t res, TP150* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
        {
        }
        void fire(void);
    };
    class _checkInCodelets154 : public darts::Codelet {
    public:
        _checkInCodelets154()
            : darts::Codelet()
        {
        }
        _checkInCodelets154(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
        {
        }
        void fire(void);
    };
    class _barrierCodelets154 : public darts::Codelet {
    public:
        _barrierCodelets154()
            : darts::Codelet()
        {
        }
        _barrierCodelets154(uint32_t dep, uint32_t res, TP150* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
        {
        }
        void fire(void);
    };
    class _checkInCodelets210 : public darts::Codelet {
    public:
        _checkInCodelets210()
            : darts::Codelet()
        {
        }
        _checkInCodelets210(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP150* TPParent;
    TP150* controlTPParent;
    TP150* inputsTPParent;
    MYTYPE** A_darts150; /*OMP_SHARED - INPUT*/
    double** B_darts150; /*OMP_SHARED - INPUT*/
    double** Cp_darts150; /*OMP_SHARED - INPUT*/
    int* M_darts150; /*OMP_SHARED - INPUT*/
    int* N_darts150; /*OMP_SHARED - INPUT*/
    int* P_darts150; /*OMP_SHARED - INPUT*/
    double* zero_darts150; /*OMP_SHARED - INPUT*/
    int* i_darts150 /*VAR*/;
    int* j_darts150 /*VAR*/;
    int* k_darts150 /*VAR*/;
    double* one_darts150 /*VAR*/;
    double* xx_darts150 /*VAR*/;
    double* yy_darts150 /*VAR*/;
    TP154** TP154Ptr;
    size_t* TP154_alreadyLaunched;
    int numTPsSet154;
    int numTPsReady154;
    size_t TPsToUse154;
    size_t codeletsPerTP154;
    size_t totalCodelets154;
    _barrierCodelets150* barrierCodelets150;
    _checkInCodelets154* checkInCodelets154;
    _barrierCodelets154* barrierCodelets154;
    _checkInCodelets210* checkInCodelets210;
    TP150(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet, MYTYPE** in_A, double** in_B, double** in_Cp, int* in_M, int* in_N, int* in_P, double* in_zero);
    ~TP150();
};
/*TP154: OMPForDirective*/
class TP154 : public ompTP {
public:
    class _barrierCodelets154 : public darts::Codelet {
    public:
        _barrierCodelets154()
            : darts::Codelet()
        {
        }
        _barrierCodelets154(uint32_t dep, uint32_t res, TP154* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations154(int* endRange, uint32_t codeletID);
    class _checkInCodelets155 : public darts::Codelet {
    public:
        int endRange;
        _checkInCodelets155()
            : darts::Codelet()
        {
        }
        _checkInCodelets155(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
        {
        }
        void fire(void);
    };
    TP150* TPParent;
    TP154* controlTPParent;
    TP154* inputsTPParent;
    MYTYPE** A_darts154; /*OMP_SHARED - INPUT*/
    double** B_darts154; /*OMP_SHARED - INPUT*/
    double** Cp_darts154; /*OMP_SHARED - INPUT*/
    int* M_darts154; /*OMP_SHARED - INPUT*/
    int* N_darts154; /*OMP_SHARED - INPUT*/
    int* P_darts154; /*OMP_SHARED - INPUT*/
    int* i_darts154 /*OMP_PRIVATE - INPUT*/;
    int* j_darts154 /*OMP_PRIVATE - INPUT*/;
    int* k_darts154 /*OMP_PRIVATE - INPUT*/;
    double* zero_darts154 /*OMP_FIRSTPRIVATE - INPUT*/;
    double* zero_outer154_ptr;
    int initIteration154;
    int lastIteration154;
    int range154;
    int rangePerCodelet154;
    int minIteration154;
    int remainderRange154;
    size_t readyCodelets;
    int baseNumThreads;
    _barrierCodelets154* barrierCodelets154;
    _checkInCodelets155* checkInCodelets155;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets155* firstCodelet;
#endif
    TP154(int in_numThreads, int in_mainCodeletID, TP150* in_TPParent, int in_initIteration, int in_lastIteration, MYTYPE** in_A, double** in_B, double** in_Cp, int* in_M, int* in_N, int* in_P, double* in_zero_outer154_ptr, TP154** in_ptrToThisTP);
    void inline dispatchCodelet(size_t codeletID);
    ~TP154();
};
#endif
