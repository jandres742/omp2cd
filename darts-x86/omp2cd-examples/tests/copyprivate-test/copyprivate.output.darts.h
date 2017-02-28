#ifndef _copyprivate_output_darts_h_
#define _copyprivate_output_darts_h_
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
int main();
float get_float();
class TP2;
typedef TP2 TP_CopyPrivate;
class TP36;
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
/*TP2: CopyPrivate*/
class TP2 : public ompTP {
public:
    class _checkInCodelets13 : public darts::Codelet {
    public:
        TP2* myTP;
        TP2* inputsTPParent;
        _checkInCodelets13()
            : darts::Codelet()
        {
        }
        _checkInCodelets13(uint32_t dep, uint32_t res, TP2* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets13 : public darts::Codelet {
    public:
        TP2* inputsTPParent;
        _barrierCodelets13()
            : darts::Codelet()
        {
        }
        _barrierCodelets13(uint32_t dep, uint32_t res, TP2* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets19 : public darts::Codelet {
    public:
        TP2* myTP;
        TP2* inputsTPParent;
        _checkInCodelets19()
            : darts::Codelet()
        {
        }
        _checkInCodelets19(uint32_t dep, uint32_t res, TP2* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP2** ptrToThisFunctionTP;
    TP2* inputsTPParent;
    TP2* controlTPParent;
    darts::Codelet** nextCodeletsCopyPrivate;
    darts::Codelet** nextSyncCodeletsCopyPrivate;
    float* a_darts2 /*VARIABLE*/;
    float* b_darts2 /*VARIABLE*/;
    float* a_darts13 /*OMP_SHARED_PRIVATE - INPUT*/;
    float* b_darts13 /*OMP_SHARED_PRIVATE - INPUT*/;
    size_t TP13_alreadyLaunched;
    size_t codeletIDexecutingSingle13;
    _checkInCodelets13* checkInCodelets13;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets13* firstCodelet;
#endif
    _barrierCodelets13* barrierCodelets13;
    _checkInCodelets19* checkInCodelets19;
    TP2(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_mainNextCodelet,
        darts::Codelet* in_mainSyncCodelet, TP2** in_ptrToThisFunctionTP, float in_a, float in_b);
    ~TP2();
    void setNewInputs(float in_a, float in_b, size_t codeletID);
};
/*TP36: OMPParallelDirective*/
class TP36 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets36 : public darts::Codelet {
    public:
        TP36* inputsTPParent;
        _barrierCodelets36()
            : darts::Codelet()
        {
        }
        _barrierCodelets36(uint32_t dep, uint32_t res, TP36* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets38 : public darts::Codelet {
    public:
        TP36* myTP;
        TP36* inputsTPParent;
        _checkInCodelets38()
            : darts::Codelet()
        {
        }
        _checkInCodelets38(uint32_t dep, uint32_t res, TP36* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP36* TPParent;
    TP36* controlTPParent;
    TP36* inputsTPParent;
    float* a_darts36; /*OMP_SHARED - INPUT*/
    float* b_darts36; /*OMP_SHARED - INPUT*/
    TP_CopyPrivate* TP38Ptr;
    int TP38_alreadyLaunched;
    _barrierCodelets36* barrierCodelets36;
    _checkInCodelets38* checkInCodelets38;
    TP36(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet, float* in_a,
        float* in_b);
    ~TP36();
};
#endif
