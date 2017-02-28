#ifndef _arraybench_output_darts_h_
#define _arraybench_output_darts_h_
#ifndef __DARTS_
#define __DARTS_
#endif
#include "arraybench.h"
#include "common.h"
#include "common.output.darts.h"
#include "darts.h"
#include "ompTP.h"
#include "tbb/concurrent_vector.h"
#include "utils.h"
#include <limits.h>
#include <math.h>
#include <mutex>
#include <numa.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
void testthrprivnew();
void testcopyprivnew();
void testprivnew();
void testfirstprivnew();
void refer();
int main(int argc, char** argv);
class TP438;
class TP449;
class TP460;
class TP462;
class TP473;
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
/*TP438: OMPParallelDirective*/
class TP438 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets438 : public darts::Codelet {
    public:
        TP438* inputsTPParent;
        _barrierCodelets438()
            : darts::Codelet()
        {
        }
        _barrierCodelets438(uint32_t dep, uint32_t res, TP438* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets440 : public darts::Codelet {
    public:
        TP438* myTP;
        TP438* inputsTPParent;
        _checkInCodelets440()
            : darts::Codelet()
        {
        }
        _checkInCodelets440(uint32_t dep, uint32_t res, TP438* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP438* TPParent;
    TP438* controlTPParent;
    TP438* inputsTPParent;
    double** atest_darts438 /*OMP_FIRSTPRIVATE*/;
    uint64_t atest_outer438_size;
    double* atest_outer438_ptr;
    _barrierCodelets438* barrierCodelets438;
    _checkInCodelets440* checkInCodelets440;
    TP438(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet,
        double* in_atest_outer438_ptr, uint64_t in_atest_outer438_size);
    ~TP438();
};
/*TP449: OMPParallelDirective*/
class TP449 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets449 : public darts::Codelet {
    public:
        TP449* inputsTPParent;
        _barrierCodelets449()
            : darts::Codelet()
        {
        }
        _barrierCodelets449(uint32_t dep, uint32_t res, TP449* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets451 : public darts::Codelet {
    public:
        TP449* myTP;
        TP449* inputsTPParent;
        _checkInCodelets451()
            : darts::Codelet()
        {
        }
        _checkInCodelets451(uint32_t dep, uint32_t res, TP449* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP449* TPParent;
    TP449* controlTPParent;
    TP449* inputsTPParent;
    double** atest_darts449 /*OMP_PRIVATE - INPUT*/;
    uint64_t atest_outer449_size;
    _barrierCodelets449* barrierCodelets449;
    _checkInCodelets451* checkInCodelets451;
    TP449(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet,
        uint64_t in_atest_outer449_size);
    ~TP449();
};
/*TP460: OMPParallelDirective*/
class TP460 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets460 : public darts::Codelet {
    public:
        TP460* inputsTPParent;
        _barrierCodelets460()
            : darts::Codelet()
        {
        }
        _barrierCodelets460(uint32_t dep, uint32_t res, TP460* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets462 : public darts::Codelet {
    public:
        TP460* myTP;
        TP460* inputsTPParent;
        _checkInCodelets462()
            : darts::Codelet()
        {
        }
        _checkInCodelets462(uint32_t dep, uint32_t res, TP460* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets462 : public darts::Codelet {
    public:
        TP460* inputsTPParent;
        _barrierCodelets462()
            : darts::Codelet()
        {
        }
        _barrierCodelets462(uint32_t dep, uint32_t res, TP460* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP460* TPParent;
    TP460* controlTPParent;
    TP460* inputsTPParent;
    double** atest_darts460 /*OMP_PRIVATE - INPUT*/;
    uint64_t atest_outer460_size;
    TP462* TP462Ptr;
    size_t TP462_alreadyLaunched;
    size_t codeletIDexecutingSingle462;
    _barrierCodelets460* barrierCodelets460;
    _checkInCodelets462* checkInCodelets462;
    _barrierCodelets462* barrierCodelets462;
    TP460(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet,
        uint64_t in_atest_outer460_size);
    ~TP460();
};
/*TP462: OMPSingleDirective*/
class TP462 : public ompOMPSingleDirectiveTP {
public:
    class _checkInCodelets464 : public darts::Codelet {
    public:
        TP462* myTP;
        TP462* inputsTPParent;
        _checkInCodelets464()
            : darts::Codelet()
        {
        }
        _checkInCodelets464(uint32_t dep, uint32_t res, TP462* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP460* TPParent;
    TP462* controlTPParent;
    TP462* inputsTPParent;
    double* atest_darts462 /*OMP_SHARED_PRIVATE - INPUT*/;
    _checkInCodelets464 checkInCodelets464;
    TP462(int in_numThreads, int in_mainCodeletID, TP460* in_TPParent);
    ~TP462();
};
/*TP473: OMPParallelDirective*/
class TP473 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets473 : public darts::Codelet {
    public:
        TP473* inputsTPParent;
        _barrierCodelets473()
            : darts::Codelet()
        {
        }
        _barrierCodelets473(uint32_t dep, uint32_t res, TP473* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets475 : public darts::Codelet {
    public:
        TP473* myTP;
        TP473* inputsTPParent;
        _checkInCodelets475()
            : darts::Codelet()
        {
        }
        _checkInCodelets475(uint32_t dep, uint32_t res, TP473* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP473* TPParent;
    TP473* controlTPParent;
    TP473* inputsTPParent;
    _barrierCodelets473* barrierCodelets473;
    _checkInCodelets475* checkInCodelets475;
    TP473(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet);
    ~TP473();
};
#endif
