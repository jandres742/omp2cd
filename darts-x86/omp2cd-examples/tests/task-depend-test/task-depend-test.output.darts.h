#ifndef _task_depend_test_output_darts_h_
#define _task_depend_test_output_darts_h_
#ifndef __DARTS_
#define __DARTS_
#endif
#include "darts.h"
#include "ompTP.h"
#include "task-depend-test.h"
#include "tbb/concurrent_vector.h"
#include "utils.h"
#include <limits.h>
#include <mutex>
#include <numa.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
int main(int argc, char** argv);
class TP8;
class TP10;
/*Class containing the inputs passed to task12*/
class _task12Inputs {
public:
    int* x_darts12; /*OMP_SHARED*/
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task12Inputs() {}
    _task12Inputs(int* in_x, darts::Codelet* in_nextSyncCodelet)
        : x_darts12(in_x)
        , taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
    }
    ~_task12Inputs() {}
};
class TP12;
/*Class containing the inputs passed to task15*/
class _task15Inputs {
public:
    int* x_darts15; /*OMP_SHARED*/
    /*Tasks this task depends on*/
    bool* task12Completed;
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task15Inputs() {}
    _task15Inputs(int* in_x, bool* in_task12Completed, darts::Codelet* in_nextSyncCodelet)
        : x_darts15(in_x)
        , task12Completed(in_task12Completed)
        , taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
    }
    ~_task15Inputs() {}
};
class TP15;
class TP20;
class TP22;
/*Class containing the inputs passed to task24*/
class _task24Inputs {
public:
    int* x_darts24; /*OMP_SHARED*/
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task24Inputs() {}
    _task24Inputs(int* in_x, darts::Codelet* in_nextSyncCodelet)
        : x_darts24(in_x)
        , taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
    }
    ~_task24Inputs() {}
};
class TP24;
/*Class containing the inputs passed to task27*/
class _task27Inputs {
public:
    int* x_darts27; /*OMP_SHARED*/
    /*Tasks this task depends on*/
    bool* task24Completed;
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task27Inputs() {}
    _task27Inputs(int* in_x, bool* in_task24Completed, darts::Codelet* in_nextSyncCodelet)
        : x_darts27(in_x)
        , task24Completed(in_task24Completed)
        , taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
    }
    ~_task27Inputs() {}
};
class TP27;
class TP32;
class TP34;
/*Class containing the inputs passed to task36*/
class _task36Inputs {
public:
    int* x_darts36; /*OMP_SHARED*/
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task36Inputs() {}
    _task36Inputs(int* in_x, darts::Codelet* in_nextSyncCodelet)
        : x_darts36(in_x)
        , taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
    }
    ~_task36Inputs() {}
};
class TP36;
/*Class containing the inputs passed to task39*/
class _task39Inputs {
public:
    int* x_darts39; /*OMP_SHARED*/
    /*Tasks this task depends on*/
    bool* task36Completed;
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task39Inputs() {}
    _task39Inputs(int* in_x, bool* in_task36Completed, darts::Codelet* in_nextSyncCodelet)
        : x_darts39(in_x)
        , task36Completed(in_task36Completed)
        , taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
    }
    ~_task39Inputs() {}
};
class TP39;
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
/*TP8: OMPParallelDirective*/
class TP8 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets8 : public darts::Codelet {
    public:
        TP8* inputsTPParent;
        _barrierCodelets8()
            : darts::Codelet()
        {
        }
        _barrierCodelets8(uint32_t dep, uint32_t res, TP8* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets10 : public darts::Codelet {
    public:
        TP8* myTP;
        TP8* inputsTPParent;
        _checkInCodelets10()
            : darts::Codelet()
        {
        }
        _checkInCodelets10(uint32_t dep, uint32_t res, TP8* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP8* TPParent;
    TP8* controlTPParent;
    TP8* inputsTPParent;
    int* x_darts8; /*OMP_SHARED - INPUT*/
    TP10* TP10Ptr;
    size_t TP10_alreadyLaunched;
    _task12Inputs** task12Inputs;
    _task15Inputs** task15Inputs;
    _barrierCodelets8* barrierCodelets8;
    _checkInCodelets10* checkInCodelets10;
    TP8(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet, int* in_x);
    ~TP8();
};
/*TP10: OMPSingleDirective*/
class TP10 : public ompOMPSingleDirectiveTP {
public:
    class _checkInCodelets12 : public darts::Codelet {
    public:
        TP10* myTP;
        TP10* inputsTPParent;
        _checkInCodelets12()
            : darts::Codelet()
        {
        }
        _checkInCodelets12(uint32_t dep, uint32_t res, TP10* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets15 : public darts::Codelet {
    public:
        TP10* myTP;
        TP10* inputsTPParent;
        _checkInCodelets15()
            : darts::Codelet()
        {
        }
        _checkInCodelets15(uint32_t dep, uint32_t res, TP10* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP8* TPParent;
    TP10* controlTPParent;
    TP10* inputsTPParent;
    int* x_darts10; /*OMP_SHARED - INPUT*/
    _task12Inputs** task12Inputs;
    _task15Inputs** task15Inputs;
    _checkInCodelets12 checkInCodelets12;
    _checkInCodelets15 checkInCodelets15;
    TP10(int in_numThreads, int in_mainCodeletID, TP8* in_TPParent, int* in_x);
    ~TP10();
};
/*TP12: OMPTaskDirective*/
class TP12 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets14 : public darts::Codelet {
    public:
        TP12* myTP;
        TP12* inputsTPParent;
        _task12Inputs* taskInputs;
        _checkInCodelets14()
            : darts::Codelet()
        {
        }
        _checkInCodelets14(uint32_t dep, uint32_t res, TP12* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task12Inputs)
        {
        }
        void fire(void);
    };
    TP10* TPParent;
    TP12* controlTPParent;
    TP12* inputsTPParent;
    _task12Inputs* task12Inputs;
    _checkInCodelets14 checkInCodelets14;
    TP12(
        int in_numThreads, int in_mainCodeletID, TP10* in_TPParent, _task12Inputs* in_task12Inputs);
    ~TP12();
};
/*TP15: OMPTaskDirective*/
class TP15 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets17 : public darts::Codelet {
    public:
        TP15* myTP;
        TP15* inputsTPParent;
        _task15Inputs* taskInputs;
        _checkInCodelets17()
            : darts::Codelet()
        {
        }
        _checkInCodelets17(uint32_t dep, uint32_t res, TP15* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task15Inputs)
        {
        }
        void fire(void);
    };
    TP10* TPParent;
    TP15* controlTPParent;
    TP15* inputsTPParent;
    _task15Inputs* task15Inputs;
    _checkInCodelets17 checkInCodelets17;
    TP15(
        int in_numThreads, int in_mainCodeletID, TP10* in_TPParent, _task15Inputs* in_task15Inputs);
    ~TP15();
};
/*TP20: OMPParallelDirective*/
class TP20 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets20 : public darts::Codelet {
    public:
        TP20* inputsTPParent;
        _barrierCodelets20()
            : darts::Codelet()
        {
        }
        _barrierCodelets20(uint32_t dep, uint32_t res, TP20* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets22 : public darts::Codelet {
    public:
        TP20* myTP;
        TP20* inputsTPParent;
        _checkInCodelets22()
            : darts::Codelet()
        {
        }
        _checkInCodelets22(uint32_t dep, uint32_t res, TP20* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets22 : public darts::Codelet {
    public:
        TP20* inputsTPParent;
        _barrierCodelets22()
            : darts::Codelet()
        {
        }
        _barrierCodelets22(uint32_t dep, uint32_t res, TP20* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP20* TPParent;
    TP20* controlTPParent;
    TP20* inputsTPParent;
    int* x_darts20; /*OMP_SHARED - INPUT*/
    TP22* TP22Ptr;
    size_t TP22_alreadyLaunched;
    _task24Inputs** task24Inputs;
    _task27Inputs** task27Inputs;
    _barrierCodelets20* barrierCodelets20;
    _checkInCodelets22* checkInCodelets22;
    _barrierCodelets22* barrierCodelets22;
    TP20(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet, int* in_x);
    ~TP20();
};
/*TP22: OMPSingleDirective*/
class TP22 : public ompOMPSingleDirectiveTP {
public:
    class _checkInCodelets24 : public darts::Codelet {
    public:
        TP22* myTP;
        TP22* inputsTPParent;
        _checkInCodelets24()
            : darts::Codelet()
        {
        }
        _checkInCodelets24(uint32_t dep, uint32_t res, TP22* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets27 : public darts::Codelet {
    public:
        TP22* myTP;
        TP22* inputsTPParent;
        _checkInCodelets27()
            : darts::Codelet()
        {
        }
        _checkInCodelets27(uint32_t dep, uint32_t res, TP22* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP20* TPParent;
    TP22* controlTPParent;
    TP22* inputsTPParent;
    int* x_darts22; /*OMP_SHARED - INPUT*/
    _task24Inputs** task24Inputs;
    _task27Inputs** task27Inputs;
    _checkInCodelets24 checkInCodelets24;
    _checkInCodelets27 checkInCodelets27;
    TP22(int in_numThreads, int in_mainCodeletID, TP20* in_TPParent, int* in_x);
    ~TP22();
};
/*TP24: OMPTaskDirective*/
class TP24 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets26 : public darts::Codelet {
    public:
        TP24* myTP;
        TP24* inputsTPParent;
        _task24Inputs* taskInputs;
        _checkInCodelets26()
            : darts::Codelet()
        {
        }
        _checkInCodelets26(uint32_t dep, uint32_t res, TP24* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task24Inputs)
        {
        }
        void fire(void);
    };
    TP22* TPParent;
    TP24* controlTPParent;
    TP24* inputsTPParent;
    _task24Inputs* task24Inputs;
    _checkInCodelets26 checkInCodelets26;
    TP24(
        int in_numThreads, int in_mainCodeletID, TP22* in_TPParent, _task24Inputs* in_task24Inputs);
    ~TP24();
};
/*TP27: OMPTaskDirective*/
class TP27 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets29 : public darts::Codelet {
    public:
        TP27* myTP;
        TP27* inputsTPParent;
        _task27Inputs* taskInputs;
        _checkInCodelets29()
            : darts::Codelet()
        {
        }
        _checkInCodelets29(uint32_t dep, uint32_t res, TP27* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task27Inputs)
        {
        }
        void fire(void);
    };
    TP22* TPParent;
    TP27* controlTPParent;
    TP27* inputsTPParent;
    _task27Inputs* task27Inputs;
    _checkInCodelets29 checkInCodelets29;
    TP27(
        int in_numThreads, int in_mainCodeletID, TP22* in_TPParent, _task27Inputs* in_task27Inputs);
    ~TP27();
};
/*TP32: OMPParallelDirective*/
class TP32 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets32 : public darts::Codelet {
    public:
        TP32* inputsTPParent;
        _barrierCodelets32()
            : darts::Codelet()
        {
        }
        _barrierCodelets32(uint32_t dep, uint32_t res, TP32* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets34 : public darts::Codelet {
    public:
        TP32* myTP;
        TP32* inputsTPParent;
        _checkInCodelets34()
            : darts::Codelet()
        {
        }
        _checkInCodelets34(uint32_t dep, uint32_t res, TP32* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets34 : public darts::Codelet {
    public:
        TP32* inputsTPParent;
        _barrierCodelets34()
            : darts::Codelet()
        {
        }
        _barrierCodelets34(uint32_t dep, uint32_t res, TP32* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP32* TPParent;
    TP32* controlTPParent;
    TP32* inputsTPParent;
    int* x_darts32; /*OMP_SHARED - INPUT*/
    TP34* TP34Ptr;
    size_t TP34_alreadyLaunched;
    _task36Inputs** task36Inputs;
    _task39Inputs** task39Inputs;
    _barrierCodelets32* barrierCodelets32;
    _checkInCodelets34* checkInCodelets34;
    _barrierCodelets34* barrierCodelets34;
    TP32(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet, int* in_x);
    ~TP32();
};
/*TP34: OMPSingleDirective*/
class TP34 : public ompOMPSingleDirectiveTP {
public:
    class _checkInCodelets36 : public darts::Codelet {
    public:
        TP34* myTP;
        TP34* inputsTPParent;
        _checkInCodelets36()
            : darts::Codelet()
        {
        }
        _checkInCodelets36(uint32_t dep, uint32_t res, TP34* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets39 : public darts::Codelet {
    public:
        TP34* myTP;
        TP34* inputsTPParent;
        _checkInCodelets39()
            : darts::Codelet()
        {
        }
        _checkInCodelets39(uint32_t dep, uint32_t res, TP34* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets42 : public darts::Codelet {
    public:
        TP34* inputsTPParent;
        _barrierCodelets42()
            : darts::Codelet()
        {
        }
        _barrierCodelets42(uint32_t dep, uint32_t res, TP34* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets43 : public darts::Codelet {
    public:
        TP34* myTP;
        TP34* inputsTPParent;
        _checkInCodelets43()
            : darts::Codelet()
        {
        }
        _checkInCodelets43(uint32_t dep, uint32_t res, TP34* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP32* TPParent;
    TP34* controlTPParent;
    TP34* inputsTPParent;
    int* x_darts34; /*OMP_SHARED - INPUT*/
    _task36Inputs** task36Inputs;
    _task39Inputs** task39Inputs;
    _checkInCodelets36 checkInCodelets36;
    _checkInCodelets39 checkInCodelets39;
    _barrierCodelets42* barrierCodelets42;
    _checkInCodelets43 checkInCodelets43;
    TP34(int in_numThreads, int in_mainCodeletID, TP32* in_TPParent, int* in_x);
    ~TP34();
};
/*TP36: OMPTaskDirective*/
class TP36 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets38 : public darts::Codelet {
    public:
        TP36* myTP;
        TP36* inputsTPParent;
        _task36Inputs* taskInputs;
        _checkInCodelets38()
            : darts::Codelet()
        {
        }
        _checkInCodelets38(uint32_t dep, uint32_t res, TP36* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task36Inputs)
        {
        }
        void fire(void);
    };
    TP34* TPParent;
    TP36* controlTPParent;
    TP36* inputsTPParent;
    _task36Inputs* task36Inputs;
    _checkInCodelets38 checkInCodelets38;
    TP36(
        int in_numThreads, int in_mainCodeletID, TP34* in_TPParent, _task36Inputs* in_task36Inputs);
    ~TP36();
};
/*TP39: OMPTaskDirective*/
class TP39 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets41 : public darts::Codelet {
    public:
        TP39* myTP;
        TP39* inputsTPParent;
        _task39Inputs* taskInputs;
        _checkInCodelets41()
            : darts::Codelet()
        {
        }
        _checkInCodelets41(uint32_t dep, uint32_t res, TP39* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task39Inputs)
        {
        }
        void fire(void);
    };
    TP34* TPParent;
    TP39* controlTPParent;
    TP39* inputsTPParent;
    _task39Inputs* task39Inputs;
    _checkInCodelets41 checkInCodelets41;
    TP39(
        int in_numThreads, int in_mainCodeletID, TP34* in_TPParent, _task39Inputs* in_task39Inputs);
    ~TP39();
};
#endif
