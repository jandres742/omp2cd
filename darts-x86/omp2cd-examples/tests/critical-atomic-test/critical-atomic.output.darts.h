#ifndef _critical_atomic_output_darts_h_
#define _critical_atomic_output_darts_h_
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
class TP12;
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
/*TP12: OMPParallelDirective*/
class TP12 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets12 : public darts::Codelet {
    public:
        TP12* inputsTPParent;
        _barrierCodelets12()
            : darts::Codelet()
        {
        }
        _barrierCodelets12(uint32_t dep, uint32_t res, TP12* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets15 : public darts::Codelet {
    public:
        TP12* myTP;
        TP12* inputsTPParent;
        _checkInCodelets15()
            : darts::Codelet()
        {
        }
        _checkInCodelets15(uint32_t dep, uint32_t res, TP12* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP12* TPParent;
    TP12* controlTPParent;
    TP12* inputsTPParent;
    volatile int* count_critical_darts12; /*OMP_SHARED - INPUT*/
    double* count_double_atomic_darts12; /*OMP_SHARED - INPUT*/
    volatile int* count_int_atomic_darts12; /*OMP_SHARED - INPUT*/
    int* inc_darts12; /*OMP_SHARED - INPUT*/
    _barrierCodelets12* barrierCodelets12;
    _checkInCodelets15* checkInCodelets15;
    TP12(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet,
        volatile int* in_count_critical, double* in_count_double_atomic,
        volatile int* in_count_int_atomic, int* in_inc);
    ~TP12();
};
#endif
