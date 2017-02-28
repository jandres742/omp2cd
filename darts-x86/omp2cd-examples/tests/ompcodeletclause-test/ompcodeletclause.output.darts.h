#ifndef _ompcodeletclause_output_darts_h_
#define _ompcodeletclause_output_darts_h_
#ifndef __DARTS_
#define __DARTS_
#endif
#include "darts.h"
#include "ompTP.h"
#include "tbb/concurrent_vector.h"
#include "utils.h"
#include <limits.h>
#include <mutex>
#include <numa.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <time.h>
#include <unistd.h>
int main(int argc, char** argv);
class TP57;
class TP59;
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
/*TP57: OMPParallelDirective*/
class TP57 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets57 : public darts::Codelet {
    public:
        TP57* inputsTPParent;
        _barrierCodelets57()
            : darts::Codelet()
        {
        }
        _barrierCodelets57(uint32_t dep, uint32_t res, TP57* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets59 : public darts::Codelet {
    public:
        TP57* myTP;
        TP57* inputsTPParent;
        _checkInCodelets59()
            : darts::Codelet()
        {
        }
        _checkInCodelets59(uint32_t dep, uint32_t res, TP57* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets70 : public darts::Codelet {
    public:
        TP57* myTP;
        TP57* inputsTPParent;
        _checkInCodelets70()
            : darts::Codelet()
        {
        }
        _checkInCodelets70(uint32_t dep, uint32_t res, TP57* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets72 : public darts::Codelet {
    public:
        TP57* myTP;
        TP57* inputsTPParent;
        int endRange;
        _checkInCodelets72()
            : darts::Codelet()
        {
        }
        _checkInCodelets72(uint32_t dep, uint32_t res, TP57* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets72 : public darts::Codelet {
    public:
        TP57* inputsTPParent;
        _barrierCodelets72()
            : darts::Codelet()
        {
        }
        _barrierCodelets72(uint32_t dep, uint32_t res, TP57* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets104 : public darts::Codelet {
    public:
        TP57* myTP;
        TP57* inputsTPParent;
        _checkInCodelets104()
            : darts::Codelet()
        {
        }
        _checkInCodelets104(uint32_t dep, uint32_t res, TP57* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets104 : public darts::Codelet {
    public:
        TP57* inputsTPParent;
        _barrierCodelets104()
            : darts::Codelet()
        {
        }
        _barrierCodelets104(uint32_t dep, uint32_t res, TP57* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP57* TPParent;
    TP57* controlTPParent;
    TP57* inputsTPParent;
    int* i_darts57; /*OMP_SHARED - INPUT*/
    int** inVector_darts57; /*OMP_SHARED - INPUT*/
    int** outVector_darts57; /*OMP_SHARED - INPUT*/
    int* parallelSum_darts57; /*OMP_SHARED - INPUT*/
    int* size_darts57; /*OMP_SHARED - INPUT*/
    int* threadID_darts57; /*OMP_SHARED - INPUT*/
    int* endLoop_darts57 /*VARIABLE*/;
    int* initLoop_darts57 /*VARIABLE*/;
    int i_darts104;
    int** outVector_darts104; /*OMP_SHARED - INPUT*/
    int* parallelSum_darts104; /*OMP_SHARED - INPUT*/
    int** endLoop_darts72 /*OMP_SHARED_PRIVATE - INPUT*/;
    int* i_darts72 /*OMP_PRIVATE - INPUT*/;
    int** initLoop_darts72 /*OMP_SHARED_PRIVATE - INPUT*/;
    int** outVector_darts72; /*OMP_SHARED - INPUT*/
    TP59* TP59Ptr;
    size_t TP59_alreadyLaunched;
    size_t TP72_alreadyLaunched;
    size_t TP104_alreadyLaunched;
    _barrierCodelets57* barrierCodelets57;
    _checkInCodelets59* checkInCodelets59;
    _checkInCodelets70* checkInCodelets70;
    _checkInCodelets72* checkInCodelets72;
    _barrierCodelets72* barrierCodelets72;
    _checkInCodelets104* checkInCodelets104;
    _barrierCodelets104* barrierCodelets104;
    TP57(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet, int* in_i,
        int** in_inVector, int** in_outVector, int* in_parallelSum, int* in_size, int* in_threadID);
    ~TP57();
};
/*TP59: OMPMasterDirective*/
class TP59 : public ompOMPMasterDirectiveTP {
public:
    class _checkInCodelets61 : public darts::Codelet {
    public:
        TP59* myTP;
        TP59* inputsTPParent;
        _checkInCodelets61()
            : darts::Codelet()
        {
        }
        _checkInCodelets61(uint32_t dep, uint32_t res, TP59* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP57* TPParent;
    TP59* controlTPParent;
    TP59* inputsTPParent;
    int i_darts59;
    int** inVector_darts59; /*OMP_SHARED - INPUT*/
    int* parallelSum_darts59; /*OMP_SHARED - INPUT*/
    int* size_darts59; /*OMP_SHARED - INPUT*/
    int* threadID_darts59; /*OMP_SHARED - INPUT*/
    _checkInCodelets61 checkInCodelets61;
    TP59(int in_numThreads, int in_mainCodeletID, TP57* in_TPParent, int** in_inVector,
        int* in_parallelSum, int* in_size, int* in_threadID);
    ~TP59();
};
#endif
