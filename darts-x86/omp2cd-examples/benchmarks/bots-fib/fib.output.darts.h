#ifndef _fib_output_darts_h_
#define _fib_output_darts_h_
#ifndef __DARTS_
#define __DARTS_
#endif
#include "bots.h"
#include "darts.h"
#include "fib.h"
#include "ompTP.h"
#include "tbb/concurrent_vector.h"
#include "utils.h"
#include <limits.h>
#include <mutex>
#include <numa.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
int fib_verify(int n);
long long fib_verify_value(int n);
void fib0_seq(int n);
void fib0(int n);
long long fib_seq(int n);
class TP16;
typedef TP16 TP_fib;
/*Class containing the inputs passed to task64*/
class _task64Inputs {
public:
    int n_darts64 /*OMP_FIRSTPRIVATE*/;
    long long* x_darts64; /*OMP_SHARED*/
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task64Inputs() {}
    _task64Inputs(int* in_n, long long* in_x, darts::Codelet* in_nextSyncCodelet)
        : x_darts64(in_x)
        , taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
        this->n_darts64 = *in_n;
    }
    ~_task64Inputs() {}
};
class TP64;
/*Class containing the inputs passed to task68*/
class _task68Inputs {
public:
    int n_darts68 /*OMP_FIRSTPRIVATE*/;
    long long* y_darts68; /*OMP_SHARED*/
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task68Inputs() {}
    _task68Inputs(int* in_n, long long* in_y, darts::Codelet* in_nextSyncCodelet)
        : y_darts68(in_y)
        , taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
        this->n_darts68 = *in_n;
    }
    ~_task68Inputs() {}
};
class TP68;
class TP76;
class TP77;
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
/*TP16: fib*/
class TP16 : public ompTP {
public:
    class _checkInCodelets59 : public darts::Codelet {
    public:
        TP16* myTP;
        TP16* inputsTPParent;
        _checkInCodelets59()
            : darts::Codelet()
        {
        }
        _checkInCodelets59(uint32_t dep, uint32_t res, TP16* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets64 : public darts::Codelet {
    public:
        TP16* myTP;
        TP16* inputsTPParent;
        _checkInCodelets64()
            : darts::Codelet()
        {
        }
        _checkInCodelets64(uint32_t dep, uint32_t res, TP16* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets68 : public darts::Codelet {
    public:
        TP16* myTP;
        TP16* inputsTPParent;
        _checkInCodelets68()
            : darts::Codelet()
        {
        }
        _checkInCodelets68(uint32_t dep, uint32_t res, TP16* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets72 : public darts::Codelet {
    public:
        TP16* inputsTPParent;
        _barrierCodelets72()
            : darts::Codelet()
        {
        }
        _barrierCodelets72(uint32_t dep, uint32_t res, TP16* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets73 : public darts::Codelet {
    public:
        TP16* myTP;
        TP16* inputsTPParent;
        _checkInCodelets73()
            : darts::Codelet()
        {
        }
        _checkInCodelets73(uint32_t dep, uint32_t res, TP16* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP16** ptrToThisFunctionTP;
    TP16* inputsTPParent;
    TP16* controlTPParent;
    darts::Codelet** nextCodeletsfib;
    darts::Codelet** nextSyncCodeletsfib;
    int* n_darts16 /*VARIABLE*/;
    long long* x_darts16 /*VARIABLE*/;
    long long* y_darts16 /*VARIABLE*/;
    long long** functionResult;
    _task64Inputs** task64Inputs;
    _task68Inputs** task68Inputs;
    _checkInCodelets59* checkInCodelets59;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets59* firstCodelet;
#endif
    _checkInCodelets64* checkInCodelets64;
    _checkInCodelets68* checkInCodelets68;
    _barrierCodelets72* barrierCodelets72;
    _checkInCodelets73* checkInCodelets73;
    TP16(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_mainNextCodelet,
        darts::Codelet* in_mainSyncCodelet, TP16** in_ptrToThisFunctionTP, int in_n,
        long long* in_functionResult);
    ~TP16();
    void setNewInputs(int in_n, long long* in_functionResult, size_t codeletID);
};
/*TP64: OMPTaskDirective*/
class TP64 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets66 : public darts::Codelet {
    public:
        TP64* myTP;
        TP64* inputsTPParent;
        _task64Inputs* taskInputs;
        _checkInCodelets66()
            : darts::Codelet()
        {
        }
        _checkInCodelets66(uint32_t dep, uint32_t res, TP64* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task64Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets126 : public darts::Codelet {
    public:
        TP64* myTP;
        TP64* inputsTPParent;
        _task64Inputs* taskInputs;
        _checkInCodelets126()
            : darts::Codelet()
        {
        }
        _checkInCodelets126(uint32_t dep, uint32_t res, TP64* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task64Inputs)
        {
        }
        void fire(void);
    };
    TP16* TPParent;
    TP64* controlTPParent;
    TP64* inputsTPParent;
    _task64Inputs* task64Inputs;
    TP_fib* TP66Ptr;
    int TP66_alreadyLaunched;
    _checkInCodelets66 checkInCodelets66;
    _checkInCodelets126 checkInCodelets126;
    TP64(
        int in_numThreads, int in_mainCodeletID, TP16* in_TPParent, _task64Inputs* in_task64Inputs);
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
    class _checkInCodelets127 : public darts::Codelet {
    public:
        TP68* myTP;
        TP68* inputsTPParent;
        _task68Inputs* taskInputs;
        _checkInCodelets127()
            : darts::Codelet()
        {
        }
        _checkInCodelets127(uint32_t dep, uint32_t res, TP68* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task68Inputs)
        {
        }
        void fire(void);
    };
    TP16* TPParent;
    TP68* controlTPParent;
    TP68* inputsTPParent;
    _task68Inputs* task68Inputs;
    TP_fib* TP70Ptr;
    int TP70_alreadyLaunched;
    _checkInCodelets70 checkInCodelets70;
    _checkInCodelets127 checkInCodelets127;
    TP68(
        int in_numThreads, int in_mainCodeletID, TP16* in_TPParent, _task68Inputs* in_task68Inputs);
    ~TP68();
};
/*TP76: OMPParallelDirective*/
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
    class _checkInCodelets77 : public darts::Codelet {
    public:
        TP76* myTP;
        TP76* inputsTPParent;
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
    class _barrierCodelets77 : public darts::Codelet {
    public:
        TP76* inputsTPParent;
        _barrierCodelets77()
            : darts::Codelet()
        {
        }
        _barrierCodelets77(uint32_t dep, uint32_t res, TP76* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP76* TPParent;
    TP76* controlTPParent;
    TP76* inputsTPParent;
    int* n_darts76; /*OMP_SHARED - INPUT*/
    TP77* TP77Ptr;
    size_t TP77_alreadyLaunched;
    _barrierCodelets76* barrierCodelets76;
    _checkInCodelets77* checkInCodelets77;
    _barrierCodelets77* barrierCodelets77;
    TP76(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet, int* in_n);
    ~TP76();
};
/*TP77: OMPSingleDirective*/
class TP77 : public ompOMPSingleDirectiveTP {
public:
    class _checkInCodelets79 : public darts::Codelet {
    public:
        TP77* myTP;
        TP77* inputsTPParent;
        _checkInCodelets79()
            : darts::Codelet()
        {
        }
        _checkInCodelets79(uint32_t dep, uint32_t res, TP77* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP76* TPParent;
    TP77* controlTPParent;
    TP77* inputsTPParent;
    int* n_darts77; /*OMP_SHARED - INPUT*/
    TP_fib* TP79Ptr;
    int TP79_alreadyLaunched;
    _checkInCodelets79 checkInCodelets79;
    TP77(int in_numThreads, int in_mainCodeletID, TP76* in_TPParent, int* in_n);
    ~TP77();
};
#endif
