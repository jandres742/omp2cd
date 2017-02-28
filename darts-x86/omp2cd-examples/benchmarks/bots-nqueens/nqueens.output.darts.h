#ifndef _nqueens_output_darts_h_
#define _nqueens_output_darts_h_
#ifndef __DARTS_
#define __DARTS_
#endif
#include "app-desc.h"
#include "bots.h"
#include "darts.h"
#include "ompTP.h"
#include "tbb/concurrent_vector.h"
#include "utils.h"
#include <alloca.h>
#include <limits.h>
#include <memory.h>
#include <mutex>
#include <numa.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
void find_queens(int size);
int verify_queens(int size);
void nqueens_ser(int n, int j, char* a, int* solutions);
int ok(int n, char* a);
class TP17;
typedef TP17 TP_nqueens;
class TP113;
/*Class containing the inputs passed to task118*/
class _task118Inputs {
public:
    char* a_darts118 /*OMP_FIRSTPRIVATE*/;
    int* csols_darts118 /*OMP_FIRSTPRIVATE*/;
    int depth_darts118 /*OMP_FIRSTPRIVATE*/;
    int i_darts118 /*OMP_FIRSTPRIVATE*/;
    int j_darts118 /*OMP_FIRSTPRIVATE*/;
    int n_darts118 /*OMP_FIRSTPRIVATE*/;
    char* b_darts118; /*VARIABLE*/
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task118Inputs() {}
    _task118Inputs(char** in_a, int** in_csols, int* in_depth, int* in_i, int* in_j, int* in_n,
        darts::Codelet* in_nextSyncCodelet)
        : taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
        this->a_darts118 = *in_a;
        this->csols_darts118 = *in_csols;
        this->depth_darts118 = *in_depth;
        this->i_darts118 = *in_i;
        this->j_darts118 = *in_j;
        this->n_darts118 = *in_n;
    }
    ~_task118Inputs() {}
};
class TP118;
class TP154;
class TP156;
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
/*TP17: nqueens*/
class TP17 : public ompTP {
public:
    class _checkInCodelets94 : public darts::Codelet {
    public:
        TP17* myTP;
        TP17* inputsTPParent;
        _checkInCodelets94()
            : darts::Codelet()
        {
        }
        _checkInCodelets94(uint32_t dep, uint32_t res, TP17* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets113 : public darts::Codelet {
    public:
        TP17* myTP;
        TP17* inputsTPParent;
        _checkInCodelets113()
            : darts::Codelet()
        {
        }
        _checkInCodelets113(uint32_t dep, uint32_t res, TP17* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets116 : public darts::Codelet {
    public:
        TP17* myTP;
        TP17* inputsTPParent;
        _checkInCodelets116()
            : darts::Codelet()
        {
        }
        _checkInCodelets116(uint32_t dep, uint32_t res, TP17* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets139 : public darts::Codelet {
    public:
        TP17* inputsTPParent;
        _barrierCodelets139()
            : darts::Codelet()
        {
        }
        _barrierCodelets139(uint32_t dep, uint32_t res, TP17* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets140 : public darts::Codelet {
    public:
        TP17* myTP;
        TP17* inputsTPParent;
        _checkInCodelets140()
            : darts::Codelet()
        {
        }
        _checkInCodelets140(uint32_t dep, uint32_t res, TP17* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP17** ptrToThisFunctionTP;
    TP17* inputsTPParent;
    TP17* controlTPParent;
    darts::Codelet** nextCodeletsnqueens;
    darts::Codelet** nextSyncCodeletsnqueens;
    int* n_darts17 /*VARIABLE*/;
    int* j_darts17 /*VARIABLE*/;
    char** a_darts17 /*VARIABLE*/;
    int** solutions_darts17 /*VARIABLE*/;
    int* depth_darts17 /*VARIABLE*/;
    int** csols_darts17 /*VARIABLE*/;
    int* i_darts17 /*VARIABLE*/;
    unsigned int TP113_LoopCounter;
    unsigned int* TP113_LoopCounterPerThread;
    tbb::concurrent_vector<TP113*> TP113PtrVec;
    _task118Inputs** task118Inputs;
    tbb::concurrent_vector<void*> allocaVars;
    _checkInCodelets94* checkInCodelets94;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets94* firstCodelet;
#endif
    _checkInCodelets113* checkInCodelets113;
    _checkInCodelets116* checkInCodelets116;
    _barrierCodelets139* barrierCodelets139;
    _checkInCodelets140* checkInCodelets140;
    TP17(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_mainNextCodelet,
        darts::Codelet* in_mainSyncCodelet, TP17** in_ptrToThisFunctionTP, int in_n, int in_j,
        char* in_a, int* in_solutions, int in_depth);
    ~TP17();
    void setNewInputs(
        int in_n, int in_j, char* in_a, int* in_solutions, int in_depth, size_t codeletID);
};
/*TP113: ForStmt*/
class TP113 : public ompTP {
public:
    class _checkInCodelets118 : public darts::Codelet {
    public:
        TP113* myTP;
        TP17* inputsTPParent;
        _checkInCodelets118()
            : darts::Codelet()
        {
        }
        _checkInCodelets118(uint32_t dep, uint32_t res, TP113* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP17* TPParent;
    TP113* controlTPParent;
    TP17* inputsTPParent;
    TP113** ptrToThisTP;
    _checkInCodelets118* checkInCodelets118;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets118* firstCodelet;
#endif
    TP113(int in_numThreads, int in_mainCodeletID, TP17* in_TPParent, TP17* in_inputsTPParent,
        TP113** in_ptrToThisTP);
    ~TP113();
};
/*TP118: OMPTaskDirective*/
class TP118 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets120 : public darts::Codelet {
    public:
        TP118* myTP;
        TP118* inputsTPParent;
        _task118Inputs* taskInputs;
        _checkInCodelets120()
            : darts::Codelet()
        {
        }
        _checkInCodelets120(uint32_t dep, uint32_t res, TP118* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task118Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets132 : public darts::Codelet {
    public:
        TP118* myTP;
        TP118* inputsTPParent;
        _task118Inputs* taskInputs;
        _checkInCodelets132()
            : darts::Codelet()
        {
        }
        _checkInCodelets132(uint32_t dep, uint32_t res, TP118* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task118Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets185 : public darts::Codelet {
    public:
        TP118* myTP;
        TP118* inputsTPParent;
        _task118Inputs* taskInputs;
        _checkInCodelets185()
            : darts::Codelet()
        {
        }
        _checkInCodelets185(uint32_t dep, uint32_t res, TP118* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task118Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets134 : public darts::Codelet {
    public:
        TP118* myTP;
        TP118* inputsTPParent;
        _task118Inputs* taskInputs;
        _checkInCodelets134()
            : darts::Codelet()
        {
        }
        _checkInCodelets134(uint32_t dep, uint32_t res, TP118* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task118Inputs)
        {
        }
        void fire(void);
    };
    TP113* TPParent;
    TP118* controlTPParent;
    TP118* inputsTPParent;
    _task118Inputs* task118Inputs;
    char* b_darts118 /*VARIABLE*/;
    TP_nqueens* TP134Ptr;
    int TP134_alreadyLaunched;
    _checkInCodelets120 checkInCodelets120;
    _checkInCodelets132 checkInCodelets132;
    _checkInCodelets185 checkInCodelets185;
    _checkInCodelets134 checkInCodelets134;
    TP118(int in_numThreads, int in_mainCodeletID, TP113* in_TPParent,
        _task118Inputs* in_task118Inputs);
    ~TP118();
};
/*TP154: OMPParallelDirective*/
class TP154 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets154 : public darts::Codelet {
    public:
        TP154* inputsTPParent;
        _barrierCodelets154()
            : darts::Codelet()
        {
        }
        _barrierCodelets154(uint32_t dep, uint32_t res, TP154* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets156 : public darts::Codelet {
    public:
        TP154* myTP;
        TP154* inputsTPParent;
        _checkInCodelets156()
            : darts::Codelet()
        {
        }
        _checkInCodelets156(uint32_t dep, uint32_t res, TP154* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets156 : public darts::Codelet {
    public:
        TP154* inputsTPParent;
        _barrierCodelets156()
            : darts::Codelet()
        {
        }
        _barrierCodelets156(uint32_t dep, uint32_t res, TP154* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP154* TPParent;
    TP154* controlTPParent;
    TP154* inputsTPParent;
    int* size_darts154; /*OMP_SHARED - INPUT*/
    TP156* TP156Ptr;
    size_t TP156_alreadyLaunched;
    _barrierCodelets154* barrierCodelets154;
    _checkInCodelets156* checkInCodelets156;
    _barrierCodelets156* barrierCodelets156;
    TP154(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet, int* in_size);
    ~TP154();
};
/*TP156: OMPSingleDirective*/
class TP156 : public ompOMPSingleDirectiveTP {
public:
    class _checkInCodelets158 : public darts::Codelet {
    public:
        TP156* myTP;
        TP156* inputsTPParent;
        _checkInCodelets158()
            : darts::Codelet()
        {
        }
        _checkInCodelets158(uint32_t dep, uint32_t res, TP156* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets164 : public darts::Codelet {
    public:
        TP156* myTP;
        TP156* inputsTPParent;
        _checkInCodelets164()
            : darts::Codelet()
        {
        }
        _checkInCodelets164(uint32_t dep, uint32_t res, TP156* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP154* TPParent;
    TP156* controlTPParent;
    TP156* inputsTPParent;
    int* size_darts156; /*OMP_SHARED - INPUT*/
    char* a_darts156 /*VARIABLE*/;
    TP_nqueens* TP164Ptr;
    int TP164_alreadyLaunched;
    _checkInCodelets158 checkInCodelets158;
    _checkInCodelets164 checkInCodelets164;
    TP156(int in_numThreads, int in_mainCodeletID, TP154* in_TPParent, int* in_size);
    ~TP156();
};
#endif
