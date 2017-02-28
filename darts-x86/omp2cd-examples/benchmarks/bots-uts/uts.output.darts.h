#ifndef _uts_output_darts_h_
#define _uts_output_darts_h_
#ifndef __DARTS_
#define __DARTS_
#endif
#include "app-desc.h"
#include "bots.h"
#include "darts.h"
#include "ompTP.h"
#include "tbb/concurrent_vector.h"
#include "utils.h"
#include "uts.h"
#include <limits.h>
#include <math.h>
#include <mutex>
#include <numa.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
void uts_show_stats();
void uts_read_file(char* filename);
unsigned long long parallel_uts(Node* root);
int uts_check_result();
int uts_numChildren(Node* parent);
int uts_numChildren_bin(Node* parent);
void uts_initRoot(Node* root);
double rng_toProb(int n);
class TP151;
class TP152;
/*Class containing the inputs passed to task153*/
class _task153Inputs {
public:
    unsigned long long* num_nodes_darts153; /*OMP_SHARED*/
    Node** root_darts153; /*OMP_SHARED*/
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task153Inputs() {}
    _task153Inputs(
        unsigned long long* in_num_nodes, Node** in_root, darts::Codelet* in_nextSyncCodelet)
        : num_nodes_darts153(in_num_nodes)
        , root_darts153(in_root)
        , taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
    }
    ~_task153Inputs() {}
};
class TP153;
class TP32;
typedef TP32 TP_parTreeSearch;
class TP167;
/*Class containing the inputs passed to task192*/
class _task192Inputs {
public:
    int depth_darts192 /*OMP_FIRSTPRIVATE*/;
    int i_darts192 /*OMP_FIRSTPRIVATE*/;
    Node* nodePtr_darts192 /*OMP_FIRSTPRIVATE*/;
    unsigned long long* partialCount_darts192; /*OMP_SHARED*/
    int partialCount_outer192_size;
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task192Inputs() {}
    _task192Inputs(int* in_depth, int* in_i, Node** in_nodePtr, unsigned long long* in_partialCount,
        int in_partialCount_outer_size, darts::Codelet* in_nextSyncCodelet)
        : partialCount_darts192(in_partialCount)
        , partialCount_outer192_size(in_partialCount_outer_size)
        , taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
        this->depth_darts192 = *in_depth;
        this->i_darts192 = *in_i;
        this->nodePtr_darts192 = *in_nodePtr;
    }
    ~_task192Inputs() {}
};
class TP192;
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
/*TP151: OMPParallelDirective*/
class TP151 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets151 : public darts::Codelet {
    public:
        TP151* inputsTPParent;
        _barrierCodelets151()
            : darts::Codelet()
        {
        }
        _barrierCodelets151(uint32_t dep, uint32_t res, TP151* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets152 : public darts::Codelet {
    public:
        TP151* myTP;
        TP151* inputsTPParent;
        _checkInCodelets152()
            : darts::Codelet()
        {
        }
        _checkInCodelets152(uint32_t dep, uint32_t res, TP151* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP151* TPParent;
    TP151* controlTPParent;
    TP151* inputsTPParent;
    unsigned long long* num_nodes_darts151; /*OMP_SHARED - INPUT*/
    Node** root_darts151; /*OMP_SHARED - INPUT*/
    TP152* TP152Ptr;
    size_t TP152_alreadyLaunched;
    _task153Inputs** task153Inputs;
    _barrierCodelets151* barrierCodelets151;
    _checkInCodelets152* checkInCodelets152;
    TP151(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet,
        unsigned long long* in_num_nodes, Node** in_root);
    ~TP151();
};
/*TP152: OMPSingleDirective*/
class TP152 : public ompOMPSingleDirectiveTP {
public:
    class _checkInCodelets153 : public darts::Codelet {
    public:
        TP152* myTP;
        TP152* inputsTPParent;
        _checkInCodelets153()
            : darts::Codelet()
        {
        }
        _checkInCodelets153(uint32_t dep, uint32_t res, TP152* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP151* TPParent;
    TP152* controlTPParent;
    TP152* inputsTPParent;
    unsigned long long* num_nodes_darts152; /*OMP_SHARED - INPUT*/
    Node** root_darts152; /*OMP_SHARED - INPUT*/
    _task153Inputs** task153Inputs;
    _checkInCodelets153 checkInCodelets153;
    TP152(int in_numThreads, int in_mainCodeletID, TP151* in_TPParent,
        unsigned long long* in_num_nodes, Node** in_root);
    ~TP152();
};
/*TP153: OMPTaskDirective*/
class TP153 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets155 : public darts::Codelet {
    public:
        TP153* myTP;
        TP153* inputsTPParent;
        _task153Inputs* taskInputs;
        _checkInCodelets155()
            : darts::Codelet()
        {
        }
        _checkInCodelets155(uint32_t dep, uint32_t res, TP153* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task153Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets357 : public darts::Codelet {
    public:
        TP153* myTP;
        TP153* inputsTPParent;
        _task153Inputs* taskInputs;
        _checkInCodelets357()
            : darts::Codelet()
        {
        }
        _checkInCodelets357(uint32_t dep, uint32_t res, TP153* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task153Inputs)
        {
        }
        void fire(void);
    };
    TP152* TPParent;
    TP153* controlTPParent;
    TP153* inputsTPParent;
    _task153Inputs* task153Inputs;
    TP_parTreeSearch* TP155Ptr;
    int TP155_alreadyLaunched;
    _checkInCodelets155 checkInCodelets155;
    _checkInCodelets357 checkInCodelets357;
    TP153(int in_numThreads, int in_mainCodeletID, TP152* in_TPParent,
        _task153Inputs* in_task153Inputs);
    ~TP153();
};
/*TP32: parTreeSearch*/
class TP32 : public ompTP {
public:
    class _checkInCodelets164 : public darts::Codelet {
    public:
        TP32* myTP;
        TP32* inputsTPParent;
        _checkInCodelets164()
            : darts::Codelet()
        {
        }
        _checkInCodelets164(uint32_t dep, uint32_t res, TP32* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets167 : public darts::Codelet {
    public:
        TP32* myTP;
        TP32* inputsTPParent;
        _checkInCodelets167()
            : darts::Codelet()
        {
        }
        _checkInCodelets167(uint32_t dep, uint32_t res, TP32* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets170 : public darts::Codelet {
    public:
        TP32* myTP;
        TP32* inputsTPParent;
        _checkInCodelets170()
            : darts::Codelet()
        {
        }
        _checkInCodelets170(uint32_t dep, uint32_t res, TP32* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets198 : public darts::Codelet {
    public:
        TP32* inputsTPParent;
        _barrierCodelets198()
            : darts::Codelet()
        {
        }
        _barrierCodelets198(uint32_t dep, uint32_t res, TP32* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets199 : public darts::Codelet {
    public:
        TP32* myTP;
        TP32* inputsTPParent;
        _checkInCodelets199()
            : darts::Codelet()
        {
        }
        _checkInCodelets199(uint32_t dep, uint32_t res, TP32* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP32** ptrToThisFunctionTP;
    TP32* inputsTPParent;
    TP32* controlTPParent;
    darts::Codelet** nextCodeletsparTreeSearch;
    darts::Codelet** nextSyncCodeletsparTreeSearch;
    int* depth_darts32 /*VARIABLE*/;
    Node** parent_darts32 /*VARIABLE*/;
    int* numChildren_darts32 /*VARIABLE*/;
    int* i_darts32 /*VARIABLE*/;
    int* j_darts32 /*VARIABLE*/;
    Node** n_darts32 /*VARIABLE*/;
    int n_outer32_size;
    Node** nodePtr_darts32 /*VARIABLE*/;
    unsigned long long** partialCount_darts32 /*VARIABLE*/;
    int partialCount_outer32_size;
    unsigned long long* subtreesize_darts32 /*VARIABLE*/;
    unsigned long long** functionResult;
    unsigned int TP167_LoopCounter;
    unsigned int* TP167_LoopCounterPerThread;
    tbb::concurrent_vector<TP167*> TP167PtrVec;
    _task192Inputs** task192Inputs;
    _checkInCodelets164* checkInCodelets164;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets164* firstCodelet;
#endif
    _checkInCodelets167* checkInCodelets167;
    _checkInCodelets170* checkInCodelets170;
    _barrierCodelets198* barrierCodelets198;
    _checkInCodelets199* checkInCodelets199;
    TP32(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_mainNextCodelet,
        darts::Codelet* in_mainSyncCodelet, TP32** in_ptrToThisFunctionTP, int in_depth,
        Node* in_parent, int in_numChildren, unsigned long long* in_functionResult);
    ~TP32();
    void setNewInputs(int in_depth, Node* in_parent, int in_numChildren,
        unsigned long long* in_functionResult, size_t codeletID);
};
/*TP167: ForStmt*/
class TP167 : public ompTP {
public:
    class _checkInCodelets172 : public darts::Codelet {
    public:
        TP167* myTP;
        TP32* inputsTPParent;
        _checkInCodelets172()
            : darts::Codelet()
        {
        }
        _checkInCodelets172(uint32_t dep, uint32_t res, TP167* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets192 : public darts::Codelet {
    public:
        TP167* myTP;
        TP32* inputsTPParent;
        _checkInCodelets192()
            : darts::Codelet()
        {
        }
        _checkInCodelets192(uint32_t dep, uint32_t res, TP167* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP32* TPParent;
    TP167* controlTPParent;
    TP32* inputsTPParent;
    TP167** ptrToThisTP;
    _checkInCodelets172* checkInCodelets172;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets172* firstCodelet;
#endif
    _checkInCodelets192* checkInCodelets192;
    TP167(int in_numThreads, int in_mainCodeletID, TP32* in_TPParent, TP32* in_inputsTPParent,
        TP167** in_ptrToThisTP);
    ~TP167();
};
/*TP192: OMPTaskDirective*/
class TP192 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets195 : public darts::Codelet {
    public:
        TP192* myTP;
        TP192* inputsTPParent;
        _task192Inputs* taskInputs;
        _checkInCodelets195()
            : darts::Codelet()
        {
        }
        _checkInCodelets195(uint32_t dep, uint32_t res, TP192* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task192Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets355 : public darts::Codelet {
    public:
        TP192* myTP;
        TP192* inputsTPParent;
        _task192Inputs* taskInputs;
        _checkInCodelets355()
            : darts::Codelet()
        {
        }
        _checkInCodelets355(uint32_t dep, uint32_t res, TP192* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task192Inputs)
        {
        }
        void fire(void);
    };
    TP167* TPParent;
    TP192* controlTPParent;
    TP192* inputsTPParent;
    _task192Inputs* task192Inputs;
    TP_parTreeSearch* TP195Ptr;
    int TP195_alreadyLaunched;
    _checkInCodelets195 checkInCodelets195;
    _checkInCodelets355 checkInCodelets355;
    TP192(int in_numThreads, int in_mainCodeletID, TP167* in_TPParent,
        _task192Inputs* in_task192Inputs);
    ~TP192();
};
#endif
