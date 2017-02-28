#ifndef _threadprivate_output_darts_h_
#define _threadprivate_output_darts_h_
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
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
int main(int argc, char* argv[]);
class TP7;
class TP14;
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
/*TP7: OMPParallelDirective*/
class TP7 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets7 : public darts::Codelet {
    public:
        TP7* inputsTPParent;
        _barrierCodelets7()
            : darts::Codelet()
        {
        }
        _barrierCodelets7(uint32_t dep, uint32_t res, TP7* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets9 : public darts::Codelet {
    public:
        TP7* myTP;
        TP7* inputsTPParent;
        _checkInCodelets9()
            : darts::Codelet()
        {
        }
        _checkInCodelets9(uint32_t dep, uint32_t res, TP7* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP7* TPParent;
    TP7* controlTPParent;
    TP7* inputsTPParent;
    int* b_darts7 /*OMP_PRIVATE - INPUT*/;
    int* tid_darts7 /*OMP_PRIVATE - INPUT*/;
    _barrierCodelets7* barrierCodelets7;
    _checkInCodelets9* checkInCodelets9;
    TP7(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet);
    ~TP7();
};
/*TP14: OMPParallelDirective*/
class TP14 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets14 : public darts::Codelet {
    public:
        TP14* inputsTPParent;
        _barrierCodelets14()
            : darts::Codelet()
        {
        }
        _barrierCodelets14(uint32_t dep, uint32_t res, TP14* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets16 : public darts::Codelet {
    public:
        TP14* myTP;
        TP14* inputsTPParent;
        _checkInCodelets16()
            : darts::Codelet()
        {
        }
        _checkInCodelets16(uint32_t dep, uint32_t res, TP14* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP14* TPParent;
    TP14* controlTPParent;
    TP14* inputsTPParent;
    int* tid_darts14 /*OMP_PRIVATE - INPUT*/;
    _barrierCodelets14* barrierCodelets14;
    _checkInCodelets16* checkInCodelets16;
    TP14(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet);
    ~TP14();
};
#endif
