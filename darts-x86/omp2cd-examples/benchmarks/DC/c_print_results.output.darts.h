#ifndef _c_print_results_output_darts_h_
#define _c_print_results_output_darts_h_
#ifndef __DARTS_
#define __DARTS_
#endif
#include "c_print_results.h"
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
void c_print_results(char* name, char class_is, int n1, int n2, int n3, int niter, double t,
    double mops, char* optype, int passed_verification, char* npbversion, char* compiletime,
    char* cc, char* clink, char* c_lib, char* c_inc, char* cflags, char* clinkflags);
class TP189;
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
/*TP189: OMPParallelDirective*/
class TP189 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets189 : public darts::Codelet {
    public:
        TP189* inputsTPParent;
        _barrierCodelets189()
            : darts::Codelet()
        {
        }
        _barrierCodelets189(uint32_t dep, uint32_t res, TP189* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets191 : public darts::Codelet {
    public:
        TP189* myTP;
        TP189* inputsTPParent;
        _checkInCodelets191()
            : darts::Codelet()
        {
        }
        _checkInCodelets191(uint32_t dep, uint32_t res, TP189* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP189* TPParent;
    TP189* controlTPParent;
    TP189* inputsTPParent;
    int* num_threads_darts189; /*OMP_SHARED - INPUT*/
    int* num_threads_darts191; /*OMP_SHARED - INPUT*/
    size_t TP191_alreadyLaunched;
    _barrierCodelets189* barrierCodelets189;
    _checkInCodelets191* checkInCodelets191;
    TP189(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet,
        int* in_num_threads);
    ~TP189();
};
#endif
