#ifndef _masterthread_output_darts_h_
#define _masterthread_output_darts_h_
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
class TP56;
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
/*TP56: OMPParallelDirective*/
class TP56 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets56 : public darts::Codelet {
    public:
        TP56* inputsTPParent;
        _barrierCodelets56()
            : darts::Codelet()
        {
        }
        _barrierCodelets56(uint32_t dep, uint32_t res, TP56* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets58 : public darts::Codelet {
    public:
        TP56* myTP;
        TP56* inputsTPParent;
        _checkInCodelets58()
            : darts::Codelet()
        {
        }
        _checkInCodelets58(uint32_t dep, uint32_t res, TP56* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets69 : public darts::Codelet {
    public:
        TP56* inputsTPParent;
        _barrierCodelets69()
            : darts::Codelet()
        {
        }
        _barrierCodelets69(uint32_t dep, uint32_t res, TP56* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets70 : public darts::Codelet {
    public:
        TP56* myTP;
        TP56* inputsTPParent;
        _checkInCodelets70()
            : darts::Codelet()
        {
        }
        _checkInCodelets70(uint32_t dep, uint32_t res, TP56* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP56* TPParent;
    TP56* controlTPParent;
    TP56* inputsTPParent;
    int* i_darts56; /*OMP_SHARED - INPUT*/
    int** inVector_darts56; /*OMP_SHARED - INPUT*/
    int** outVector_darts56; /*OMP_SHARED - INPUT*/
    int* parallelSum_darts56; /*OMP_SHARED - INPUT*/
    int* size_darts56; /*OMP_SHARED - INPUT*/
    int* threadID_darts56; /*OMP_SHARED - INPUT*/
    int i_darts58;
    int** inVector_darts58; /*OMP_SHARED - INPUT*/
    int* parallelSum_darts58; /*OMP_SHARED - INPUT*/
    int* size_darts58; /*OMP_SHARED - INPUT*/
    int* threadID_darts58; /*OMP_SHARED - INPUT*/
    size_t TP58_alreadyLaunched;
    _barrierCodelets56* barrierCodelets56;
    _checkInCodelets58* checkInCodelets58;
    _barrierCodelets69* barrierCodelets69;
    _checkInCodelets70* checkInCodelets70;
    TP56(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet, int* in_i,
        int** in_inVector, int** in_outVector, int* in_parallelSum, int* in_size, int* in_threadID);
    ~TP56();
};
#endif
