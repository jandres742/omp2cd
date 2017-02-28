#ifndef _graph500_output_darts_h_
#define _graph500_output_darts_h_
#ifndef __DARTS_
#define __DARTS_
#endif
#include "compat.h"
#include "darts.h"
#include "generator/graph_generator.h"
#include "generator/make_graph.h"
#include "generator/splittable_mrg.h"
#include "graph500.h"
#include "kronecker.h"
#include "ompTP.h"
#include "options.h"
#include "prng.h"
#include "rmat.h"
#include "tbb/concurrent_vector.h"
#include "timer.h"
#include "utils.h"
#include "verify.h"
#include "xalloc.h"
#include <alloca.h>
#include <cassert>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <limits.h>
#include <math.h>
#include <mutex>
#include <numa.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/time.h>
#include <time.h>
#include <time.h>
#include <unistd.h>
#include <unistd.h>
void statistics(double* out, double* data, int64_t n);
int main(int argc, char** argv);
class TP869;
class TP871;
/*Number of TPs to be used for the OMPFor in region TP871*/
#define NUMTPS871 NUMTPS
class TP902;
/*Number of TPs to be used for the OMPFor in region TP902*/
#define NUMTPS902 NUMTPS
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
/*TP869: OMPParallelDirective*/
class TP869 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets869 : public darts::Codelet {
    public:
        TP869* inputsTPParent;
        _barrierCodelets869()
            : darts::Codelet()
        {
        }
        _barrierCodelets869(uint32_t dep, uint32_t res, TP869* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets871 : public darts::Codelet {
    public:
        TP869* myTP;
        TP869* inputsTPParent;
        _checkInCodelets871()
            : darts::Codelet()
        {
        }
        _checkInCodelets871(uint32_t dep, uint32_t res, TP869* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets871 : public darts::Codelet {
    public:
        TP869* inputsTPParent;
        _barrierCodelets871()
            : darts::Codelet()
        {
        }
        _barrierCodelets871(uint32_t dep, uint32_t res, TP869* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets902 : public darts::Codelet {
    public:
        TP869* myTP;
        TP869* inputsTPParent;
        _checkInCodelets902()
            : darts::Codelet()
        {
        }
        _checkInCodelets902(uint32_t dep, uint32_t res, TP869* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets902 : public darts::Codelet {
    public:
        TP869* inputsTPParent;
        _barrierCodelets902()
            : darts::Codelet()
        {
        }
        _barrierCodelets902(uint32_t dep, uint32_t res, TP869* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP869* TPParent;
    TP869* controlTPParent;
    TP869* inputsTPParent;
    int* __restrict* has_adj_darts869; /*OMP_SHARED - INPUT*/
    int64_t* k_darts869; /*OMP_SHARED - INPUT*/
    TP871** TP871Ptr;
    size_t* TP871_alreadyLaunched;
    int numTPsSet871;
    int numTPsReady871;
    size_t TPsToUse871;
    size_t codeletsPerTP871;
    size_t totalCodelets871;
    TP902** TP902Ptr;
    size_t* TP902_alreadyLaunched;
    int numTPsSet902;
    int numTPsReady902;
    size_t TPsToUse902;
    size_t codeletsPerTP902;
    size_t totalCodelets902;
    _barrierCodelets869* barrierCodelets869;
    _checkInCodelets871* checkInCodelets871;
    _barrierCodelets871* barrierCodelets871;
    _checkInCodelets902* checkInCodelets902;
    _barrierCodelets902* barrierCodelets902;
    TP869(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet,
        int* __restrict* in_has_adj, int64_t* in_k);
    ~TP869();
};
/*TP871: OMPForDirective*/
class TP871 : public ompTP {
public:
    class _barrierCodelets871 : public darts::Codelet {
    public:
        TP871* inputsTPParent;
        _barrierCodelets871()
            : darts::Codelet()
        {
        }
        _barrierCodelets871(uint32_t dep, uint32_t res, TP871* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations871(int64_t* endRange, uint32_t codeletID);
    class _checkInCodelets872 : public darts::Codelet {
    public:
        TP871* myTP;
        TP871* inputsTPParent;
        int64_t endRange;
        _checkInCodelets872()
            : darts::Codelet()
        {
        }
        _checkInCodelets872(uint32_t dep, uint32_t res, TP871* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP869* TPParent;
    TP871* controlTPParent;
    TP871* inputsTPParent;
    int* __restrict* has_adj_darts871; /*OMP_SHARED - INPUT*/
    int64_t* k_darts871 /*OMP_PRIVATE - INPUT*/;
    int64_t initIteration871;
    int64_t lastIteration871;
    int64_t range871;
    int64_t rangePerCodelet871;
    int64_t minIteration871;
    int64_t remainderRange871;
    size_t readyCodelets;
    int baseNumThreads;
    _barrierCodelets871* barrierCodelets871;
    _checkInCodelets872* checkInCodelets872;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets872* firstCodelet;
#endif
    TP871(int in_numThreads, int in_mainCodeletID, TP869* in_TPParent, int64_t in_initIteration,
        int64_t in_lastIteration, int* __restrict* in_has_adj, TP871** in_ptrToThisTP);
    void inline dispatchCodelet(size_t codeletID);
    ~TP871();
};
/*TP902: OMPForDirective*/
class TP902 : public ompTP {
public:
    class _barrierCodelets902 : public darts::Codelet {
    public:
        TP902* inputsTPParent;
        _barrierCodelets902()
            : darts::Codelet()
        {
        }
        _barrierCodelets902(uint32_t dep, uint32_t res, TP902* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations902(int64_t* endRange, uint32_t codeletID);
    class _checkInCodelets903 : public darts::Codelet {
    public:
        TP902* myTP;
        TP902* inputsTPParent;
        int64_t endRange;
        _checkInCodelets903()
            : darts::Codelet()
        {
        }
        _checkInCodelets903(uint32_t dep, uint32_t res, TP902* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP869* TPParent;
    TP902* controlTPParent;
    TP902* inputsTPParent;
    int* __restrict* has_adj_darts902; /*OMP_SHARED - INPUT*/
    int64_t* k_darts902 /*OMP_PRIVATE - INPUT*/;
    int64_t initIteration902;
    int64_t lastIteration902;
    int64_t range902;
    int64_t rangePerCodelet902;
    int64_t minIteration902;
    int64_t remainderRange902;
    size_t readyCodelets;
    int baseNumThreads;
    _barrierCodelets902* barrierCodelets902;
    _checkInCodelets903* checkInCodelets903;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets903* firstCodelet;
#endif
    TP902(int in_numThreads, int in_mainCodeletID, TP869* in_TPParent, int64_t in_initIteration,
        int64_t in_lastIteration, int* __restrict* in_has_adj, TP902** in_ptrToThisTP);
    void inline dispatchCodelet(size_t codeletID);
    ~TP902();
};
#endif
