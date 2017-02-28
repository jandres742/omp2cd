#ifndef _sparselu_output_darts_h_
#define _sparselu_output_darts_h_
#ifndef __DARTS_
#define __DARTS_
#endif
#include "bots.h"
#include "darts.h"
#include "ompTP.h"
#include "sparselu.h"
#include "tbb/concurrent_vector.h"
#include "utils.h"
#include <libgen.h>
#include <limits.h>
#include <math.h>
#include <mutex>
#include <numa.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
int sparselu_check(float** SEQ, float** BENCH);
int checkmat(float* M, float* N);
void print_structure(char* name, float* M[]);
void sparselu_seq_call(float** BENCH);
void sparselu_init(float*** pBENCH, char* pass);
void sparselu_fini(float** BENCH, char* pass);
float* allocate_clean_block();
void lu0(float* diag);
void bdiv(float* diag, float* row);
void fwd(float* diag, float* col);
void genmat(float* M[]);
void sparselu_par_call(float** BENCH);
void bmod(float* row, float* col, float* inner);
class TP454;
class TP455;
/*Class containing the inputs passed to task456*/
class _task456Inputs {
public:
    float*** BENCH_darts456; /*OMP_SHARED*/
    int ii_darts456 /*OMP_FIRSTPRIVATE*/;
    int jj_darts456 /*OMP_FIRSTPRIVATE*/;
    int kk_darts456 /*OMP_FIRSTPRIVATE*/;
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task456Inputs() {}
    _task456Inputs(
        float*** in_BENCH, int* in_ii, int* in_jj, int* in_kk, darts::Codelet* in_nextSyncCodelet)
        : BENCH_darts456(in_BENCH)
        , taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
        this->ii_darts456 = *in_ii;
        this->jj_darts456 = *in_jj;
        this->kk_darts456 = *in_kk;
    }
    ~_task456Inputs() {}
};
class TP456;
class TP457;
class TP466;
/*Class containing the inputs passed to task473*/
class _task473Inputs {
public:
    float*** BENCH_darts473; /*OMP_SHARED*/
    int jj_darts473 /*OMP_FIRSTPRIVATE*/;
    int kk_darts473 /*OMP_FIRSTPRIVATE*/;
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task473Inputs() {}
    _task473Inputs(float*** in_BENCH, int* in_jj, int* in_kk, darts::Codelet* in_nextSyncCodelet)
        : BENCH_darts473(in_BENCH)
        , taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
        this->jj_darts473 = *in_jj;
        this->kk_darts473 = *in_kk;
    }
    ~_task473Inputs() {}
};
class TP473;
class TP483;
/*Class containing the inputs passed to task490*/
class _task490Inputs {
public:
    float*** BENCH_darts490; /*OMP_SHARED*/
    int ii_darts490 /*OMP_FIRSTPRIVATE*/;
    int kk_darts490 /*OMP_FIRSTPRIVATE*/;
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task490Inputs() {}
    _task490Inputs(float*** in_BENCH, int* in_ii, int* in_kk, darts::Codelet* in_nextSyncCodelet)
        : BENCH_darts490(in_BENCH)
        , taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
        this->ii_darts490 = *in_ii;
        this->kk_darts490 = *in_kk;
    }
    ~_task490Inputs() {}
};
class TP490;
class TP501;
class TP508;
/*Class containing the inputs passed to task515*/
class _task515Inputs {
public:
    float*** BENCH_darts515; /*OMP_SHARED*/
    int ii_darts515 /*OMP_FIRSTPRIVATE*/;
    int jj_darts515 /*OMP_FIRSTPRIVATE*/;
    int kk_darts515 /*OMP_FIRSTPRIVATE*/;
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task515Inputs() {}
    _task515Inputs(
        float*** in_BENCH, int* in_ii, int* in_jj, int* in_kk, darts::Codelet* in_nextSyncCodelet)
        : BENCH_darts515(in_BENCH)
        , taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
        this->ii_darts515 = *in_ii;
        this->jj_darts515 = *in_jj;
        this->kk_darts515 = *in_kk;
    }
    ~_task515Inputs() {}
};
class TP515;
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
/*TP454: OMPParallelDirective*/
class TP454 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets454 : public darts::Codelet {
    public:
        TP454* inputsTPParent;
        _barrierCodelets454()
            : darts::Codelet()
        {
        }
        _barrierCodelets454(uint32_t dep, uint32_t res, TP454* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets455 : public darts::Codelet {
    public:
        TP454* myTP;
        TP454* inputsTPParent;
        _checkInCodelets455()
            : darts::Codelet()
        {
        }
        _checkInCodelets455(uint32_t dep, uint32_t res, TP454* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP454* TPParent;
    TP454* controlTPParent;
    TP454* inputsTPParent;
    float*** BENCH_darts454; /*OMP_SHARED - INPUT*/
    int* ii_darts454; /*OMP_SHARED - INPUT*/
    int* jj_darts454; /*OMP_SHARED - INPUT*/
    int* kk_darts454; /*OMP_SHARED - INPUT*/
    TP455* TP455Ptr;
    size_t TP455_alreadyLaunched;
    _task456Inputs** task456Inputs;
    _task473Inputs** task473Inputs;
    _task490Inputs** task490Inputs;
    _task515Inputs** task515Inputs;
    _barrierCodelets454* barrierCodelets454;
    _checkInCodelets455* checkInCodelets455;
    TP454(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet,
        float*** in_BENCH, int* in_ii, int* in_jj, int* in_kk);
    ~TP454();
};
/*TP455: OMPSingleDirective*/
class TP455 : public ompOMPSingleDirectiveTP {
public:
    class _checkInCodelets456 : public darts::Codelet {
    public:
        TP455* myTP;
        TP455* inputsTPParent;
        _checkInCodelets456()
            : darts::Codelet()
        {
        }
        _checkInCodelets456(uint32_t dep, uint32_t res, TP455* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP454* TPParent;
    TP455* controlTPParent;
    TP455* inputsTPParent;
    float*** BENCH_darts455; /*OMP_SHARED - INPUT*/
    int* ii_darts455; /*OMP_SHARED - INPUT*/
    int* jj_darts455; /*OMP_SHARED - INPUT*/
    int* kk_darts455; /*OMP_SHARED - INPUT*/
    _task456Inputs** task456Inputs;
    _task473Inputs** task473Inputs;
    _task490Inputs** task490Inputs;
    _task515Inputs** task515Inputs;
    _checkInCodelets456 checkInCodelets456;
    TP455(int in_numThreads, int in_mainCodeletID, TP454* in_TPParent, float*** in_BENCH,
        int* in_ii, int* in_jj, int* in_kk);
    ~TP455();
};
/*TP456: OMPTaskDirective*/
class TP456 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets458 : public darts::Codelet {
    public:
        TP456* myTP;
        TP456* inputsTPParent;
        _task456Inputs* taskInputs;
        _checkInCodelets458()
            : darts::Codelet()
        {
        }
        _checkInCodelets458(uint32_t dep, uint32_t res, TP456* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task456Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets457 : public darts::Codelet {
    public:
        TP456* myTP;
        TP456* inputsTPParent;
        _task456Inputs* taskInputs;
        _checkInCodelets457()
            : darts::Codelet()
        {
        }
        _checkInCodelets457(uint32_t dep, uint32_t res, TP456* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task456Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets460 : public darts::Codelet {
    public:
        TP456* myTP;
        TP456* inputsTPParent;
        _task456Inputs* taskInputs;
        _checkInCodelets460()
            : darts::Codelet()
        {
        }
        _checkInCodelets460(uint32_t dep, uint32_t res, TP456* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task456Inputs)
        {
        }
        void fire(void);
    };
    TP455* TPParent;
    TP456* controlTPParent;
    TP456* inputsTPParent;
    _task456Inputs* task456Inputs;
    unsigned int TP457_LoopCounter;
    unsigned int* TP457_LoopCounterPerThread;
    tbb::concurrent_vector<TP457*> TP457PtrVec;
    _task473Inputs** task473Inputs;
    _task490Inputs** task490Inputs;
    _task515Inputs** task515Inputs;
    _checkInCodelets458 checkInCodelets458;
    _checkInCodelets457 checkInCodelets457;
    _checkInCodelets460 checkInCodelets460;
    TP456(int in_numThreads, int in_mainCodeletID, TP455* in_TPParent,
        _task456Inputs* in_task456Inputs);
    ~TP456();
};
/*TP457: ForStmt*/
class TP457 : public ompTP {
public:
    class _checkInCodelets462 : public darts::Codelet {
    public:
        TP457* myTP;
        TP456* inputsTPParent;
        _task456Inputs* taskInputs;
        _checkInCodelets462()
            : darts::Codelet()
        {
        }
        _checkInCodelets462(uint32_t dep, uint32_t res, TP457* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task456Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets466 : public darts::Codelet {
    public:
        TP457* myTP;
        TP456* inputsTPParent;
        _task456Inputs* taskInputs;
        _checkInCodelets466()
            : darts::Codelet()
        {
        }
        _checkInCodelets466(uint32_t dep, uint32_t res, TP457* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task456Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets470 : public darts::Codelet {
    public:
        TP457* myTP;
        TP456* inputsTPParent;
        _task456Inputs* taskInputs;
        _checkInCodelets470()
            : darts::Codelet()
        {
        }
        _checkInCodelets470(uint32_t dep, uint32_t res, TP457* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task456Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets484 : public darts::Codelet {
    public:
        TP457* myTP;
        TP456* inputsTPParent;
        _task456Inputs* taskInputs;
        _checkInCodelets484()
            : darts::Codelet()
        {
        }
        _checkInCodelets484(uint32_t dep, uint32_t res, TP457* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task456Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets483 : public darts::Codelet {
    public:
        TP457* myTP;
        TP456* inputsTPParent;
        _task456Inputs* taskInputs;
        _checkInCodelets483()
            : darts::Codelet()
        {
        }
        _checkInCodelets483(uint32_t dep, uint32_t res, TP457* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task456Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets487 : public darts::Codelet {
    public:
        TP457* myTP;
        TP456* inputsTPParent;
        _task456Inputs* taskInputs;
        _checkInCodelets487()
            : darts::Codelet()
        {
        }
        _checkInCodelets487(uint32_t dep, uint32_t res, TP457* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task456Inputs)
        {
        }
        void fire(void);
    };
    class _barrierCodelets500 : public darts::Codelet {
    public:
        TP456* inputsTPParent;
        _task456Inputs* taskInputs;
        _barrierCodelets500()
            : darts::Codelet()
        {
        }
        _barrierCodelets500(uint32_t dep, uint32_t res, TP457* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets502 : public darts::Codelet {
    public:
        TP457* myTP;
        TP456* inputsTPParent;
        _task456Inputs* taskInputs;
        _checkInCodelets502()
            : darts::Codelet()
        {
        }
        _checkInCodelets502(uint32_t dep, uint32_t res, TP457* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task456Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets501 : public darts::Codelet {
    public:
        TP457* myTP;
        TP456* inputsTPParent;
        _task456Inputs* taskInputs;
        _checkInCodelets501()
            : darts::Codelet()
        {
        }
        _checkInCodelets501(uint32_t dep, uint32_t res, TP457* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task456Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets505 : public darts::Codelet {
    public:
        TP457* myTP;
        TP456* inputsTPParent;
        _task456Inputs* taskInputs;
        _checkInCodelets505()
            : darts::Codelet()
        {
        }
        _checkInCodelets505(uint32_t dep, uint32_t res, TP457* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task456Inputs)
        {
        }
        void fire(void);
    };
    class _barrierCodelets537 : public darts::Codelet {
    public:
        TP456* inputsTPParent;
        _task456Inputs* taskInputs;
        _barrierCodelets537()
            : darts::Codelet()
        {
        }
        _barrierCodelets537(uint32_t dep, uint32_t res, TP457* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP456* TPParent;
    TP457* controlTPParent;
    TP456* inputsTPParent;
    TP457** ptrToThisTP;
    unsigned int TP466_LoopCounter;
    unsigned int* TP466_LoopCounterPerThread;
    tbb::concurrent_vector<TP466*> TP466PtrVec;
    unsigned int TP483_LoopCounter;
    unsigned int* TP483_LoopCounterPerThread;
    tbb::concurrent_vector<TP483*> TP483PtrVec;
    unsigned int TP501_LoopCounter;
    unsigned int* TP501_LoopCounterPerThread;
    tbb::concurrent_vector<TP501*> TP501PtrVec;
    _checkInCodelets462 checkInCodelets462;
    _checkInCodelets466 checkInCodelets466;
    _checkInCodelets470 checkInCodelets470;
    _checkInCodelets484 checkInCodelets484;
    _checkInCodelets483 checkInCodelets483;
    _checkInCodelets487 checkInCodelets487;
    _barrierCodelets500* barrierCodelets500;
    _checkInCodelets502 checkInCodelets502;
    _checkInCodelets501 checkInCodelets501;
    _checkInCodelets505 checkInCodelets505;
    _barrierCodelets537* barrierCodelets537;
    TP457(int in_numThreads, int in_mainCodeletID, TP456* in_TPParent, TP456* in_inputsTPParent,
        TP457** in_ptrToThisTP);
    ~TP457();
};
/*TP466: ForStmt*/
class TP466 : public ompTP {
public:
    class _checkInCodelets471 : public darts::Codelet {
    public:
        TP466* myTP;
        TP456* inputsTPParent;
        _task456Inputs* taskInputs;
        _checkInCodelets471()
            : darts::Codelet()
        {
        }
        _checkInCodelets471(uint32_t dep, uint32_t res, TP466* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task456Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets473 : public darts::Codelet {
    public:
        TP466* myTP;
        TP456* inputsTPParent;
        _task456Inputs* taskInputs;
        _checkInCodelets473()
            : darts::Codelet()
        {
        }
        _checkInCodelets473(uint32_t dep, uint32_t res, TP466* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task456Inputs)
        {
        }
        void fire(void);
    };
    TP457* TPParent;
    TP466* controlTPParent;
    TP456* inputsTPParent;
    TP466** ptrToThisTP;
    _checkInCodelets471 checkInCodelets471;
    _checkInCodelets473 checkInCodelets473;
    TP466(int in_numThreads, int in_mainCodeletID, TP457* in_TPParent, TP456* in_inputsTPParent,
        TP466** in_ptrToThisTP);
    ~TP466();
};
/*TP473: OMPTaskDirective*/
class TP473 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets475 : public darts::Codelet {
    public:
        TP473* myTP;
        TP473* inputsTPParent;
        _task473Inputs* taskInputs;
        _checkInCodelets475()
            : darts::Codelet()
        {
        }
        _checkInCodelets475(uint32_t dep, uint32_t res, TP473* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task473Inputs)
        {
        }
        void fire(void);
    };
    TP466* TPParent;
    TP473* controlTPParent;
    TP473* inputsTPParent;
    _task473Inputs* task473Inputs;
    _checkInCodelets475 checkInCodelets475;
    TP473(int in_numThreads, int in_mainCodeletID, TP466* in_TPParent,
        _task473Inputs* in_task473Inputs);
    ~TP473();
};
/*TP483: ForStmt*/
class TP483 : public ompTP {
public:
    class _checkInCodelets488 : public darts::Codelet {
    public:
        TP483* myTP;
        TP456* inputsTPParent;
        _task456Inputs* taskInputs;
        _checkInCodelets488()
            : darts::Codelet()
        {
        }
        _checkInCodelets488(uint32_t dep, uint32_t res, TP483* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task456Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets490 : public darts::Codelet {
    public:
        TP483* myTP;
        TP456* inputsTPParent;
        _task456Inputs* taskInputs;
        _checkInCodelets490()
            : darts::Codelet()
        {
        }
        _checkInCodelets490(uint32_t dep, uint32_t res, TP483* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task456Inputs)
        {
        }
        void fire(void);
    };
    TP457* TPParent;
    TP483* controlTPParent;
    TP456* inputsTPParent;
    TP483** ptrToThisTP;
    _checkInCodelets488 checkInCodelets488;
    _checkInCodelets490 checkInCodelets490;
    TP483(int in_numThreads, int in_mainCodeletID, TP457* in_TPParent, TP456* in_inputsTPParent,
        TP483** in_ptrToThisTP);
    ~TP483();
};
/*TP490: OMPTaskDirective*/
class TP490 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets492 : public darts::Codelet {
    public:
        TP490* myTP;
        TP490* inputsTPParent;
        _task490Inputs* taskInputs;
        _checkInCodelets492()
            : darts::Codelet()
        {
        }
        _checkInCodelets492(uint32_t dep, uint32_t res, TP490* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task490Inputs)
        {
        }
        void fire(void);
    };
    TP483* TPParent;
    TP490* controlTPParent;
    TP490* inputsTPParent;
    _task490Inputs* task490Inputs;
    _checkInCodelets492 checkInCodelets492;
    TP490(int in_numThreads, int in_mainCodeletID, TP483* in_TPParent,
        _task490Inputs* in_task490Inputs);
    ~TP490();
};
/*TP501: ForStmt*/
class TP501 : public ompTP {
public:
    class _checkInCodelets506 : public darts::Codelet {
    public:
        TP501* myTP;
        TP456* inputsTPParent;
        _task456Inputs* taskInputs;
        _checkInCodelets506()
            : darts::Codelet()
        {
        }
        _checkInCodelets506(uint32_t dep, uint32_t res, TP501* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task456Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets509 : public darts::Codelet {
    public:
        TP501* myTP;
        TP456* inputsTPParent;
        _task456Inputs* taskInputs;
        _checkInCodelets509()
            : darts::Codelet()
        {
        }
        _checkInCodelets509(uint32_t dep, uint32_t res, TP501* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task456Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets508 : public darts::Codelet {
    public:
        TP501* myTP;
        TP456* inputsTPParent;
        _task456Inputs* taskInputs;
        _checkInCodelets508()
            : darts::Codelet()
        {
        }
        _checkInCodelets508(uint32_t dep, uint32_t res, TP501* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task456Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets512 : public darts::Codelet {
    public:
        TP501* myTP;
        TP456* inputsTPParent;
        _task456Inputs* taskInputs;
        _checkInCodelets512()
            : darts::Codelet()
        {
        }
        _checkInCodelets512(uint32_t dep, uint32_t res, TP501* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task456Inputs)
        {
        }
        void fire(void);
    };
    TP457* TPParent;
    TP501* controlTPParent;
    TP456* inputsTPParent;
    TP501** ptrToThisTP;
    unsigned int TP508_LoopCounter;
    unsigned int* TP508_LoopCounterPerThread;
    tbb::concurrent_vector<TP508*> TP508PtrVec;
    _checkInCodelets506 checkInCodelets506;
    _checkInCodelets509 checkInCodelets509;
    _checkInCodelets508 checkInCodelets508;
    _checkInCodelets512 checkInCodelets512;
    TP501(int in_numThreads, int in_mainCodeletID, TP457* in_TPParent, TP456* in_inputsTPParent,
        TP501** in_ptrToThisTP);
    ~TP501();
};
/*TP508: ForStmt*/
class TP508 : public ompTP {
public:
    class _checkInCodelets513 : public darts::Codelet {
    public:
        TP508* myTP;
        TP456* inputsTPParent;
        _task456Inputs* taskInputs;
        _checkInCodelets513()
            : darts::Codelet()
        {
        }
        _checkInCodelets513(uint32_t dep, uint32_t res, TP508* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task456Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets515 : public darts::Codelet {
    public:
        TP508* myTP;
        TP456* inputsTPParent;
        _task456Inputs* taskInputs;
        _checkInCodelets515()
            : darts::Codelet()
        {
        }
        _checkInCodelets515(uint32_t dep, uint32_t res, TP508* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task456Inputs)
        {
        }
        void fire(void);
    };
    TP501* TPParent;
    TP508* controlTPParent;
    TP456* inputsTPParent;
    TP508** ptrToThisTP;
    _checkInCodelets513 checkInCodelets513;
    _checkInCodelets515 checkInCodelets515;
    TP508(int in_numThreads, int in_mainCodeletID, TP501* in_TPParent, TP456* in_inputsTPParent,
        TP508** in_ptrToThisTP);
    ~TP508();
};
/*TP515: OMPTaskDirective*/
class TP515 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets517 : public darts::Codelet {
    public:
        TP515* myTP;
        TP515* inputsTPParent;
        _task515Inputs* taskInputs;
        _checkInCodelets517()
            : darts::Codelet()
        {
        }
        _checkInCodelets517(uint32_t dep, uint32_t res, TP515* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task515Inputs)
        {
        }
        void fire(void);
    };
    TP508* TPParent;
    TP515* controlTPParent;
    TP515* inputsTPParent;
    _task515Inputs* task515Inputs;
    _checkInCodelets517 checkInCodelets517;
    TP515(int in_numThreads, int in_mainCodeletID, TP508* in_TPParent,
        _task515Inputs* in_task515Inputs);
    ~TP515();
};
#endif
