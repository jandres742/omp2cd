#ifndef _lastprivate_output_darts_h_
#define _lastprivate_output_darts_h_
#ifndef __DARTS_
#define __DARTS_
#endif
#include "darts.h"
#include "lastprivate.h"
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
class TP30;
class TP32;
/*Number of TPs to be used for the OMPFor in region TP32*/
#define NUMTPS32 NUMTPS
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
/*TP30: OMPParallelDirective*/
class TP30 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets30 : public darts::Codelet {
    public:
        TP30* inputsTPParent;
        _barrierCodelets30()
            : darts::Codelet()
        {
        }
        _barrierCodelets30(uint32_t dep, uint32_t res, TP30* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets32 : public darts::Codelet {
    public:
        TP30* myTP;
        TP30* inputsTPParent;
        _checkInCodelets32()
            : darts::Codelet()
        {
        }
        _checkInCodelets32(uint32_t dep, uint32_t res, TP30* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets32 : public darts::Codelet {
    public:
        TP30* inputsTPParent;
        _barrierCodelets32()
            : darts::Codelet()
        {
        }
        _barrierCodelets32(uint32_t dep, uint32_t res, TP30* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets78 : public darts::Codelet {
    public:
        TP30* myTP;
        TP30* inputsTPParent;
        _checkInCodelets78()
            : darts::Codelet()
        {
        }
        _checkInCodelets78(uint32_t dep, uint32_t res, TP30* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP30* TPParent;
    TP30* controlTPParent;
    TP30* inputsTPParent;
    MYTYPE** A_darts30; /*OMP_SHARED - INPUT*/
    int* N_darts30; /*OMP_SHARED - INPUT*/
    int* i_darts30; /*OMP_SHARED - INPUT*/
    int* w_darts30; /*OMP_SHARED - INPUT*/
    TP32** TP32Ptr;
    size_t* TP32_alreadyLaunched;
    int numTPsSet32;
    int numTPsReady32;
    size_t TPsToUse32;
    size_t codeletsPerTP32;
    size_t totalCodelets32;
    _barrierCodelets30* barrierCodelets30;
    _checkInCodelets32* checkInCodelets32;
    _barrierCodelets32* barrierCodelets32;
    _checkInCodelets78* checkInCodelets78;
    TP30(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet, MYTYPE** in_A,
        int* in_N, int* in_i, int* in_w);
    ~TP30();
};
/*TP32: OMPForDirective*/
class TP32 : public ompTP {
public:
    class _barrierCodelets32 : public darts::Codelet {
    public:
        TP32* inputsTPParent;
        _barrierCodelets32()
            : darts::Codelet()
        {
        }
        _barrierCodelets32(uint32_t dep, uint32_t res, TP32* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations32(
        int* endRange, uint32_t codeletID, bool* containsLastIteration);
    class _checkInCodelets34 : public darts::Codelet {
    public:
        TP32* myTP;
        TP32* inputsTPParent;
        int endRange;
        _checkInCodelets34()
            : darts::Codelet()
        {
        }
        _checkInCodelets34(uint32_t dep, uint32_t res, TP32* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP30* TPParent;
    TP32* controlTPParent;
    TP32* inputsTPParent;
    MYTYPE** A_darts32; /*OMP_SHARED - INPUT*/
    int* N_darts32; /*OMP_SHARED - INPUT*/
    int* i_darts32 /*OMP_LASTPRIVATE - INPUT*/;
    int* i_outer32_ptr;
    int* w_darts32 /*OMP_FIRSTPRIVATE - INPUT*/;
    int* w_outer32_ptr;
    int initIteration32;
    int lastIteration32;
    int range32;
    int rangePerCodelet32;
    int minIteration32;
    int remainderRange32;
    size_t readyCodelets;
    int baseNumThreads;
    _barrierCodelets32* barrierCodelets32;
    _checkInCodelets34* checkInCodelets34;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets34* firstCodelet;
#endif
    TP32(int in_numThreads, int in_mainCodeletID, TP30* in_TPParent, int in_initIteration,
        int in_lastIteration, MYTYPE** in_A, int* in_N, int* in_i_outer32_ptr,
        int* in_w_outer32_ptr, TP32** in_ptrToThisTP);
    void inline dispatchCodelet(size_t codeletID);
    ~TP32();
};
#endif
