#ifndef _reduce_omp_output_darts_h_
#define _reduce_omp_output_darts_h_
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
class TP76;
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
/*TP76: OMPParallelForDirective*/
class TP76 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets76 : public darts::Codelet {
    public:
        TP76* inputsTPParent;
        _barrierCodelets76()
            : darts::Codelet()
        {
        }
        _barrierCodelets76(uint32_t dep, uint32_t res, TP76* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations76(int* endRange, uint32_t codeletID);
    class _checkInCodelets77 : public darts::Codelet {
    public:
        TP76* myTP;
        TP76* inputsTPParent;
        int endRange;
        _checkInCodelets77()
            : darts::Codelet()
        {
        }
        _checkInCodelets77(uint32_t dep, uint32_t res, TP76* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP76* TPParent;
    TP76* controlTPParent;
    TP76* inputsTPParent;
    double** a_darts76; /*OMP_SHARED - INPUT*/
    int* i_darts76 /*OMP_PRIVATE - INPUT*/;
    double* resa_par_darts76; /*OMP_SHARED - INPUT*/
    int initIteration76;
    int lastIteration76;
    int range76;
    int rangePerCodelet76;
    int minIteration76;
    int remainderRange76;
    std::mutex resa_par_darts76_mutex;
    _barrierCodelets76* barrierCodelets76;
    _checkInCodelets77* checkInCodelets77;
    TP76(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet,
        int in_initIteration, int in_lastIteration, double** in_a, double* in_resa_par);
    ~TP76();
};
#endif
