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
void c_print_results(char* name, char class_name, int n1, int n2, int n3, int niter, double t,
    double mops, char* optype, int passed_verification, char* npbversion, char* compiletime,
    char* cc, char* clink, char* c_lib, char* c_inc, char* cflags, char* clinkflags);
class TP1032;
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
/*TP1032: OMPParallelDirective*/
class TP1032 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets1032 : public darts::Codelet {
    public:
        TP1032* inputsTPParent;
        _barrierCodelets1032()
            : darts::Codelet()
        {
        }
        _barrierCodelets1032(uint32_t dep, uint32_t res, TP1032* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets1034 : public darts::Codelet {
    public:
        TP1032* myTP;
        TP1032* inputsTPParent;
        _checkInCodelets1034()
            : darts::Codelet()
        {
        }
        _checkInCodelets1034(uint32_t dep, uint32_t res, TP1032* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP1032* TPParent;
    TP1032* controlTPParent;
    TP1032* inputsTPParent;
    int* num_threads_darts1032; /*OMP_SHARED - INPUT*/
    int* num_threads_darts1034; /*OMP_SHARED - INPUT*/
    size_t TP1034_alreadyLaunched;
    _barrierCodelets1032* barrierCodelets1032;
    _checkInCodelets1034* checkInCodelets1034;
    TP1032(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet,
        int* in_num_threads);
    ~TP1032();
};
#endif
