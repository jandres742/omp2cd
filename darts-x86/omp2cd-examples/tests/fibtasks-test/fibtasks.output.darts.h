#ifndef _fibtasks_output_darts_h_
#define _fibtasks_output_darts_h_
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
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
int main(int argc, char** argv);
int seqFib(int n);
class TP2;
typedef TP2 TP_parallelFibTaskWait;
/*Class containing the inputs passed to task32*/
class _task32Inputs {
public:
    int n_darts32 /*OMP_FIRSTPRIVATE*/;
    int* x_darts32; /*OMP_SHARED*/
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task32Inputs() {}
    _task32Inputs(int* in_n, int* in_x, darts::Codelet* in_nextSyncCodelet)
        : x_darts32(in_x)
        , taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
        this->n_darts32 = *in_n;
    }
    ~_task32Inputs() {}
};
class TP32;
/*Class containing the inputs passed to task37*/
class _task37Inputs {
public:
    int n_darts37 /*OMP_FIRSTPRIVATE*/;
    int* y_darts37; /*OMP_SHARED*/
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task37Inputs() {}
    _task37Inputs(int* in_n, int* in_y, darts::Codelet* in_nextSyncCodelet)
        : y_darts37(in_y)
        , taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
        this->n_darts37 = *in_n;
    }
    ~_task37Inputs() {}
};
class TP37;
class TP63;
class TP65;
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
/*TP2: parallelFibTaskWait*/
class TP2 : public ompTP {
public:
    class _checkInCodelets27 : public darts::Codelet {
    public:
        TP2* myTP;
        TP2* inputsTPParent;
        _checkInCodelets27()
            : darts::Codelet()
        {
        }
        _checkInCodelets27(uint32_t dep, uint32_t res, TP2* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets30 : public darts::Codelet {
    public:
        TP2* myTP;
        TP2* inputsTPParent;
        _checkInCodelets30()
            : darts::Codelet()
        {
        }
        _checkInCodelets30(uint32_t dep, uint32_t res, TP2* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets32 : public darts::Codelet {
    public:
        TP2* myTP;
        TP2* inputsTPParent;
        _checkInCodelets32()
            : darts::Codelet()
        {
        }
        _checkInCodelets32(uint32_t dep, uint32_t res, TP2* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets37 : public darts::Codelet {
    public:
        TP2* myTP;
        TP2* inputsTPParent;
        _checkInCodelets37()
            : darts::Codelet()
        {
        }
        _checkInCodelets37(uint32_t dep, uint32_t res, TP2* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets42 : public darts::Codelet {
    public:
        TP2* inputsTPParent;
        _barrierCodelets42()
            : darts::Codelet()
        {
        }
        _barrierCodelets42(uint32_t dep, uint32_t res, TP2* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets43 : public darts::Codelet {
    public:
        TP2* myTP;
        TP2* inputsTPParent;
        _checkInCodelets43()
            : darts::Codelet()
        {
        }
        _checkInCodelets43(uint32_t dep, uint32_t res, TP2* myTP, uint32_t id)
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
    darts::Codelet** nextCodeletsparallelFibTaskWait;
    darts::Codelet** nextSyncCodeletsparallelFibTaskWait;
    int* n_darts2 /*VARIABLE*/;
    int* x_darts2 /*VARIABLE*/;
    int* y_darts2 /*VARIABLE*/;
    int** functionResult;
    _task32Inputs** task32Inputs;
    _task37Inputs** task37Inputs;
    _checkInCodelets27* checkInCodelets27;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets27* firstCodelet;
#endif
    _checkInCodelets30* checkInCodelets30;
    _checkInCodelets32* checkInCodelets32;
    _checkInCodelets37* checkInCodelets37;
    _barrierCodelets42* barrierCodelets42;
    _checkInCodelets43* checkInCodelets43;
    TP2(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_mainNextCodelet,
        darts::Codelet* in_mainSyncCodelet, TP2** in_ptrToThisFunctionTP, int in_n,
        int* in_functionResult);
    ~TP2();
    void setNewInputs(int in_n, int* in_functionResult, size_t codeletID);
};
/*TP32: OMPTaskDirective*/
class TP32 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets35 : public darts::Codelet {
    public:
        TP32* myTP;
        TP32* inputsTPParent;
        _task32Inputs* taskInputs;
        _checkInCodelets35()
            : darts::Codelet()
        {
        }
        _checkInCodelets35(uint32_t dep, uint32_t res, TP32* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task32Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets76 : public darts::Codelet {
    public:
        TP32* myTP;
        TP32* inputsTPParent;
        _task32Inputs* taskInputs;
        _checkInCodelets76()
            : darts::Codelet()
        {
        }
        _checkInCodelets76(uint32_t dep, uint32_t res, TP32* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task32Inputs)
        {
        }
        void fire(void);
    };
    TP2* TPParent;
    TP32* controlTPParent;
    TP32* inputsTPParent;
    _task32Inputs* task32Inputs;
    TP_parallelFibTaskWait* TP35Ptr;
    int TP35_alreadyLaunched;
    _checkInCodelets35 checkInCodelets35;
    _checkInCodelets76 checkInCodelets76;
    TP32(int in_numThreads, int in_mainCodeletID, TP2* in_TPParent, _task32Inputs* in_task32Inputs);
    ~TP32();
};
/*TP37: OMPTaskDirective*/
class TP37 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets40 : public darts::Codelet {
    public:
        TP37* myTP;
        TP37* inputsTPParent;
        _task37Inputs* taskInputs;
        _checkInCodelets40()
            : darts::Codelet()
        {
        }
        _checkInCodelets40(uint32_t dep, uint32_t res, TP37* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task37Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets77 : public darts::Codelet {
    public:
        TP37* myTP;
        TP37* inputsTPParent;
        _task37Inputs* taskInputs;
        _checkInCodelets77()
            : darts::Codelet()
        {
        }
        _checkInCodelets77(uint32_t dep, uint32_t res, TP37* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task37Inputs)
        {
        }
        void fire(void);
    };
    TP2* TPParent;
    TP37* controlTPParent;
    TP37* inputsTPParent;
    _task37Inputs* task37Inputs;
    TP_parallelFibTaskWait* TP40Ptr;
    int TP40_alreadyLaunched;
    _checkInCodelets40 checkInCodelets40;
    _checkInCodelets77 checkInCodelets77;
    TP37(int in_numThreads, int in_mainCodeletID, TP2* in_TPParent, _task37Inputs* in_task37Inputs);
    ~TP37();
};
/*TP63: OMPParallelDirective*/
class TP63 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets63 : public darts::Codelet {
    public:
        TP63* inputsTPParent;
        _barrierCodelets63()
            : darts::Codelet()
        {
        }
        _barrierCodelets63(uint32_t dep, uint32_t res, TP63* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets65 : public darts::Codelet {
    public:
        TP63* myTP;
        TP63* inputsTPParent;
        _checkInCodelets65()
            : darts::Codelet()
        {
        }
        _checkInCodelets65(uint32_t dep, uint32_t res, TP63* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets65 : public darts::Codelet {
    public:
        TP63* inputsTPParent;
        _barrierCodelets65()
            : darts::Codelet()
        {
        }
        _barrierCodelets65(uint32_t dep, uint32_t res, TP63* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP63* TPParent;
    TP63* controlTPParent;
    TP63* inputsTPParent;
    int* num_darts63; /*OMP_SHARED - INPUT*/
    int* parTaskWait_darts63; /*OMP_SHARED - INPUT*/
    TP65* TP65Ptr;
    size_t TP65_alreadyLaunched;
    _barrierCodelets63* barrierCodelets63;
    _checkInCodelets65* checkInCodelets65;
    _barrierCodelets65* barrierCodelets65;
    TP63(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet, int* in_num,
        int* in_parTaskWait);
    ~TP63();
};
/*TP65: OMPSingleDirective*/
class TP65 : public ompOMPSingleDirectiveTP {
public:
    class _checkInCodelets68 : public darts::Codelet {
    public:
        TP65* myTP;
        TP65* inputsTPParent;
        _checkInCodelets68()
            : darts::Codelet()
        {
        }
        _checkInCodelets68(uint32_t dep, uint32_t res, TP65* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP63* TPParent;
    TP65* controlTPParent;
    TP65* inputsTPParent;
    int* num_darts65; /*OMP_SHARED - INPUT*/
    int* parTaskWait_darts65; /*OMP_SHARED - INPUT*/
    TP_parallelFibTaskWait* TP68Ptr;
    int TP68_alreadyLaunched;
    _checkInCodelets68 checkInCodelets68;
    TP65(int in_numThreads, int in_mainCodeletID, TP63* in_TPParent, int* in_num,
        int* in_parTaskWait);
    ~TP65();
};
#endif
