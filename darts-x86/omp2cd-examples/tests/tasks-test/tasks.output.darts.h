#ifndef _tasks_output_darts_h_
#define _tasks_output_darts_h_
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
void myFunction(double* inVec, double* outVec, int idx);
class TP64;
/*Class containing the inputs passed to task68*/
class _task68Inputs {
public:
    int id_darts68 /*OMP_FIRSTPRIVATE*/;
    double** inVector_darts68; /*OMP_SHARED*/
    double** outVectorPar_darts68; /*OMP_SHARED*/
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task68Inputs() {}
    _task68Inputs(int* in_id, double** in_inVector, double** in_outVectorPar,
        darts::Codelet* in_nextSyncCodelet)
        : inVector_darts68(in_inVector)
        , outVectorPar_darts68(in_outVectorPar)
        , taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
        this->id_darts68 = *in_id;
    }
    ~_task68Inputs() {}
};
class TP68;
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
/*TP64: OMPParallelDirective*/
class TP64 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets64 : public darts::Codelet {
    public:
        TP64* inputsTPParent;
        _barrierCodelets64()
            : darts::Codelet()
        {
        }
        _barrierCodelets64(uint32_t dep, uint32_t res, TP64* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets66 : public darts::Codelet {
    public:
        TP64* myTP;
        TP64* inputsTPParent;
        _checkInCodelets66()
            : darts::Codelet()
        {
        }
        _checkInCodelets66(uint32_t dep, uint32_t res, TP64* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets68 : public darts::Codelet {
    public:
        TP64* myTP;
        TP64* inputsTPParent;
        _checkInCodelets68()
            : darts::Codelet()
        {
        }
        _checkInCodelets68(uint32_t dep, uint32_t res, TP64* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets71 : public darts::Codelet {
    public:
        TP64* myTP;
        TP64* inputsTPParent;
        _checkInCodelets71()
            : darts::Codelet()
        {
        }
        _checkInCodelets71(uint32_t dep, uint32_t res, TP64* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets72 : public darts::Codelet {
    public:
        TP64* inputsTPParent;
        _barrierCodelets72()
            : darts::Codelet()
        {
        }
        _barrierCodelets72(uint32_t dep, uint32_t res, TP64* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets73 : public darts::Codelet {
    public:
        TP64* myTP;
        TP64* inputsTPParent;
        _checkInCodelets73()
            : darts::Codelet()
        {
        }
        _checkInCodelets73(uint32_t dep, uint32_t res, TP64* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets74 : public darts::Codelet {
    public:
        TP64* inputsTPParent;
        _barrierCodelets74()
            : darts::Codelet()
        {
        }
        _barrierCodelets74(uint32_t dep, uint32_t res, TP64* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP64* TPParent;
    TP64* controlTPParent;
    TP64* inputsTPParent;
    double** inVector_darts64; /*OMP_SHARED - INPUT*/
    double** outVectorPar_darts64; /*OMP_SHARED - INPUT*/
    int* id_darts64 /*VARIABLE*/;
    _task68Inputs** task68Inputs;
    _barrierCodelets64* barrierCodelets64;
    _checkInCodelets66* checkInCodelets66;
    _checkInCodelets68* checkInCodelets68;
    _checkInCodelets71* checkInCodelets71;
    _barrierCodelets72* barrierCodelets72;
    _checkInCodelets73* checkInCodelets73;
    _barrierCodelets74* barrierCodelets74;
    TP64(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet,
        double** in_inVector, double** in_outVectorPar);
    ~TP64();
};
/*TP68: OMPTaskDirective*/
class TP68 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets70 : public darts::Codelet {
    public:
        TP68* myTP;
        TP68* inputsTPParent;
        _task68Inputs* taskInputs;
        _checkInCodelets70()
            : darts::Codelet()
        {
        }
        _checkInCodelets70(uint32_t dep, uint32_t res, TP68* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task68Inputs)
        {
        }
        void fire(void);
    };
    TP64* TPParent;
    TP68* controlTPParent;
    TP68* inputsTPParent;
    _task68Inputs* task68Inputs;
    _checkInCodelets70 checkInCodelets70;
    TP68(
        int in_numThreads, int in_mainCodeletID, TP64* in_TPParent, _task68Inputs* in_task68Inputs);
    ~TP68();
};
#endif
