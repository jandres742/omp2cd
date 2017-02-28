#ifndef _ljForce_output_darts_h_
#define _ljForce_output_darts_h_
#ifndef __DARTS_
#define __DARTS_
#endif
#include "darts.h"
#include "ljForce.h"
#include "ompTP.h"
#include "tbb/concurrent_vector.h"
#include "utils.h"
#include <limits.h>
#include <mutex>
#include <numa.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
struct BasePotentialSt* initLjPot();
void ljDestroy(BasePotential** inppot);
class TP3548;
class TP3550;
/*Number of TPs to be used for the OMPFor in region TP3550*/
#define NUMTPS3550 NUMTPS
class TP3606;
class TP3608;
/*Number of TPs to be used for the OMPFor in region TP3608*/
#define NUMTPS3608 NUMTPS
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
/*TP3548: OMPParallelDirective*/
class TP3548 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets3548 : public darts::Codelet {
    public:
        TP3548* inputsTPParent;
        _barrierCodelets3548()
            : darts::Codelet()
        {
        }
        _barrierCodelets3548(uint32_t dep, uint32_t res, TP3548* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets3550 : public darts::Codelet {
    public:
        TP3548* myTP;
        TP3548* inputsTPParent;
        _checkInCodelets3550()
            : darts::Codelet()
        {
        }
        _checkInCodelets3550(uint32_t dep, uint32_t res, TP3548* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets3550 : public darts::Codelet {
    public:
        TP3548* inputsTPParent;
        _barrierCodelets3550()
            : darts::Codelet()
        {
        }
        _barrierCodelets3550(uint32_t dep, uint32_t res, TP3548* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP3548* TPParent;
    TP3548* controlTPParent;
    TP3548* inputsTPParent;
    int* fSize_darts3548; /*OMP_SHARED - INPUT*/
    SimFlat** s_darts3548; /*OMP_SHARED - INPUT*/
    TP3550** TP3550Ptr;
    size_t* TP3550_alreadyLaunched;
    int numTPsSet3550;
    int numTPsReady3550;
    size_t TPsToUse3550;
    size_t codeletsPerTP3550;
    size_t totalCodelets3550;
    _barrierCodelets3548* barrierCodelets3548;
    _checkInCodelets3550* checkInCodelets3550;
    _barrierCodelets3550* barrierCodelets3550;
    TP3548(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet, int* in_fSize,
        SimFlat** in_s);
    ~TP3548();
};
/*TP3550: OMPForDirective*/
class TP3550 : public ompTP {
public:
    class _barrierCodelets3550 : public darts::Codelet {
    public:
        TP3550* inputsTPParent;
        _barrierCodelets3550()
            : darts::Codelet()
        {
        }
        _barrierCodelets3550(uint32_t dep, uint32_t res, TP3550* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations3550(int* endRange, uint32_t codeletID);
    class _checkInCodelets3551 : public darts::Codelet {
    public:
        TP3550* myTP;
        TP3550* inputsTPParent;
        int endRange;
        _checkInCodelets3551()
            : darts::Codelet()
        {
        }
        _checkInCodelets3551(uint32_t dep, uint32_t res, TP3550* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP3548* TPParent;
    TP3550* controlTPParent;
    TP3550* inputsTPParent;
    int* fSize_darts3550; /*OMP_SHARED - INPUT*/
    SimFlat** s_darts3550; /*OMP_SHARED - INPUT*/
    int* ii_darts3550 /*VARIABLE*/;
    int initIteration3550;
    int lastIteration3550;
    int range3550;
    int rangePerCodelet3550;
    int minIteration3550;
    int remainderRange3550;
    size_t readyCodelets;
    int baseNumThreads;
    _barrierCodelets3550* barrierCodelets3550;
    _checkInCodelets3551* checkInCodelets3551;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets3551* firstCodelet;
#endif
    TP3550(int in_numThreads, int in_mainCodeletID, TP3548* in_TPParent, int in_initIteration,
        int in_lastIteration, int* in_fSize, SimFlat** in_s, TP3550** in_ptrToThisTP);
    void inline dispatchCodelet(size_t codeletID);
    ~TP3550();
};
/*TP3606: OMPParallelDirective*/
class TP3606 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets3606 : public darts::Codelet {
    public:
        TP3606* inputsTPParent;
        _barrierCodelets3606()
            : darts::Codelet()
        {
        }
        _barrierCodelets3606(uint32_t dep, uint32_t res, TP3606* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets3608 : public darts::Codelet {
    public:
        TP3606* myTP;
        TP3606* inputsTPParent;
        _checkInCodelets3608()
            : darts::Codelet()
        {
        }
        _checkInCodelets3608(uint32_t dep, uint32_t res, TP3606* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets3608 : public darts::Codelet {
    public:
        TP3606* inputsTPParent;
        _barrierCodelets3608()
            : darts::Codelet()
        {
        }
        _barrierCodelets3608(uint32_t dep, uint32_t res, TP3606* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP3606* TPParent;
    TP3606* controlTPParent;
    TP3606* inputsTPParent;
    real_t* ePot_darts3606; /*OMP_SHARED - INPUT*/
    real_t* eShift_darts3606; /*OMP_SHARED - INPUT*/
    real_t* epsilon_darts3606; /*OMP_SHARED - INPUT*/
    real_t* rCut2_darts3606; /*OMP_SHARED - INPUT*/
    SimFlat** s_darts3606; /*OMP_SHARED - INPUT*/
    real_t* s6_darts3606; /*OMP_SHARED - INPUT*/
    TP3608** TP3608Ptr;
    size_t* TP3608_alreadyLaunched;
    int numTPsSet3608;
    int numTPsReady3608;
    size_t TPsToUse3608;
    size_t codeletsPerTP3608;
    size_t totalCodelets3608;
    std::mutex ePot_darts3608_mutex;
    _barrierCodelets3606* barrierCodelets3606;
    _checkInCodelets3608* checkInCodelets3608;
    _barrierCodelets3608* barrierCodelets3608;
    TP3606(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet, real_t* in_ePot,
        real_t* in_eShift, real_t* in_epsilon, real_t* in_rCut2, SimFlat** in_s, real_t* in_s6);
    ~TP3606();
};
/*TP3608: OMPForDirective*/
class TP3608 : public ompTP {
public:
    class _barrierCodelets3608 : public darts::Codelet {
    public:
        TP3608* inputsTPParent;
        _barrierCodelets3608()
            : darts::Codelet()
        {
        }
        _barrierCodelets3608(uint32_t dep, uint32_t res, TP3608* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations3608(int* endRange, uint32_t codeletID);
    class _checkInCodelets3609 : public darts::Codelet {
    public:
        TP3608* myTP;
        TP3608* inputsTPParent;
        int endRange;
        _checkInCodelets3609()
            : darts::Codelet()
        {
        }
        _checkInCodelets3609(uint32_t dep, uint32_t res, TP3608* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP3606* TPParent;
    TP3608* controlTPParent;
    TP3608* inputsTPParent;
    real_t* ePot_darts3608; /*OMP_SHARED - INPUT*/
    real_t* eShift_darts3608; /*OMP_SHARED - INPUT*/
    real_t* epsilon_darts3608; /*OMP_SHARED - INPUT*/
    real_t* rCut2_darts3608; /*OMP_SHARED - INPUT*/
    SimFlat** s_darts3608; /*OMP_SHARED - INPUT*/
    real_t* s6_darts3608; /*OMP_SHARED - INPUT*/
    int* iBox_darts3608 /*VARIABLE*/;
    int initIteration3608;
    int lastIteration3608;
    int range3608;
    int rangePerCodelet3608;
    int minIteration3608;
    int remainderRange3608;
    size_t readyCodelets;
    int baseNumThreads;
    std::mutex* ePot_darts3608_mutex;
    _barrierCodelets3608* barrierCodelets3608;
    _checkInCodelets3609* checkInCodelets3609;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets3609* firstCodelet;
#endif
    TP3608(int in_numThreads, int in_mainCodeletID, TP3606* in_TPParent, int in_initIteration,
        int in_lastIteration, real_t* in_ePot, real_t* in_eShift, real_t* in_epsilon,
        real_t* in_rCut2, SimFlat** in_s, real_t* in_s6, TP3608** in_ptrToThisTP);
    void inline dispatchCodelet(size_t codeletID);
    ~TP3608();
};
#endif
