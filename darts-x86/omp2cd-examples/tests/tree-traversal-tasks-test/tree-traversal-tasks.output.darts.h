#ifndef _tree_traversal_tasks_output_darts_h_
#define _tree_traversal_tasks_output_darts_h_
#ifndef __DARTS_
#define __DARTS_
#endif
#include "darts.h"
#include "ompTP.h"
#include "tbb/concurrent_vector.h"
#include "tree-traversal-tasks.h"
#include "utils.h"
#include <limits.h>
#include <mutex>
#include <numa.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
int main(int argc, char** argv);
void getDistanceToRoot(Node_t* node);
void deleteGraph(Node_t* node);
void displayGraph(Node_t* node);
void printSpaces(int numSpaces);
void constructGraph(Node_t* node, int maxNodes, int* countNodes);
Node_t* createNode(int* countNodes);
class TP7;
typedef TP7 TP_getDistanceToRootTasks;
/*Class containing the inputs passed to task202*/
class _task202Inputs {
public:
    Node_t* node_darts202 /*OMP_FIRSTPRIVATE*/;
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task202Inputs() {}
    _task202Inputs(Node_t** in_node, darts::Codelet* in_nextSyncCodelet)
        : taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
        this->node_darts202 = *in_node;
    }
    ~_task202Inputs() {}
};
class TP202;
/*Class containing the inputs passed to task211*/
class _task211Inputs {
public:
    Node_t** node_darts211; /*OMP_SHARED*/
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task211Inputs() {}
    _task211Inputs(Node_t** in_node, darts::Codelet* in_nextSyncCodelet)
        : node_darts211(in_node)
        , taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
    }
    ~_task211Inputs() {}
};
class TP211;
class TP235;
class TP237;
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
/*TP7: getDistanceToRootTasks*/
class TP7 : public ompTP {
public:
    class _checkInCodelets191 : public darts::Codelet {
    public:
        TP7* myTP;
        TP7* inputsTPParent;
        _checkInCodelets191()
            : darts::Codelet()
        {
        }
        _checkInCodelets191(uint32_t dep, uint32_t res, TP7* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets199 : public darts::Codelet {
    public:
        TP7* myTP;
        TP7* inputsTPParent;
        _checkInCodelets199()
            : darts::Codelet()
        {
        }
        _checkInCodelets199(uint32_t dep, uint32_t res, TP7* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets207 : public darts::Codelet {
    public:
        TP7* inputsTPParent;
        _barrierCodelets207()
            : darts::Codelet()
        {
        }
        _barrierCodelets207(uint32_t dep, uint32_t res, TP7* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets208 : public darts::Codelet {
    public:
        TP7* myTP;
        TP7* inputsTPParent;
        _checkInCodelets208()
            : darts::Codelet()
        {
        }
        _checkInCodelets208(uint32_t dep, uint32_t res, TP7* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets202 : public darts::Codelet {
    public:
        TP7* myTP;
        TP7* inputsTPParent;
        _checkInCodelets202()
            : darts::Codelet()
        {
        }
        _checkInCodelets202(uint32_t dep, uint32_t res, TP7* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets211 : public darts::Codelet {
    public:
        TP7* myTP;
        TP7* inputsTPParent;
        _checkInCodelets211()
            : darts::Codelet()
        {
        }
        _checkInCodelets211(uint32_t dep, uint32_t res, TP7* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP7** ptrToThisFunctionTP;
    TP7* inputsTPParent;
    TP7* controlTPParent;
    darts::Codelet** nextCodeletsgetDistanceToRootTasks;
    darts::Codelet** nextSyncCodeletsgetDistanceToRootTasks;
    Node_t** node_darts7 /*VARIABLE*/;
    _task202Inputs** task202Inputs;
    _task211Inputs** task211Inputs;
    _checkInCodelets191* checkInCodelets191;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets191* firstCodelet;
#endif
    _checkInCodelets199* checkInCodelets199;
    _barrierCodelets207* barrierCodelets207;
    _checkInCodelets208* checkInCodelets208;
    _checkInCodelets202* checkInCodelets202;
    _checkInCodelets211* checkInCodelets211;
    TP7(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_mainNextCodelet,
        darts::Codelet* in_mainSyncCodelet, TP7** in_ptrToThisFunctionTP, Node_t* in_node);
    ~TP7();
    void setNewInputs(Node_t* in_node, size_t codeletID);
};
/*TP202: OMPTaskDirective*/
class TP202 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets204 : public darts::Codelet {
    public:
        TP202* myTP;
        TP202* inputsTPParent;
        _task202Inputs* taskInputs;
        _checkInCodelets204()
            : darts::Codelet()
        {
        }
        _checkInCodelets204(uint32_t dep, uint32_t res, TP202* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task202Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets206 : public darts::Codelet {
    public:
        TP202* myTP;
        TP202* inputsTPParent;
        _task202Inputs* taskInputs;
        _checkInCodelets206()
            : darts::Codelet()
        {
        }
        _checkInCodelets206(uint32_t dep, uint32_t res, TP202* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task202Inputs)
        {
        }
        void fire(void);
    };
    TP7* TPParent;
    TP202* controlTPParent;
    TP202* inputsTPParent;
    _task202Inputs* task202Inputs;
    TP_getDistanceToRootTasks* TP204Ptr;
    int TP204_alreadyLaunched;
    _checkInCodelets204 checkInCodelets204;
    _checkInCodelets206 checkInCodelets206;
    TP202(int in_numThreads, int in_mainCodeletID, TP7* in_TPParent,
        _task202Inputs* in_task202Inputs);
    ~TP202();
};
/*TP211: OMPTaskDirective*/
class TP211 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets212 : public darts::Codelet {
    public:
        TP211* myTP;
        TP211* inputsTPParent;
        _task211Inputs* taskInputs;
        _checkInCodelets212()
            : darts::Codelet()
        {
        }
        _checkInCodelets212(uint32_t dep, uint32_t res, TP211* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task211Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets244 : public darts::Codelet {
    public:
        TP211* myTP;
        TP211* inputsTPParent;
        _task211Inputs* taskInputs;
        _checkInCodelets244()
            : darts::Codelet()
        {
        }
        _checkInCodelets244(uint32_t dep, uint32_t res, TP211* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task211Inputs)
        {
        }
        void fire(void);
    };
    TP7* TPParent;
    TP211* controlTPParent;
    TP211* inputsTPParent;
    _task211Inputs* task211Inputs;
    TP_getDistanceToRootTasks* TP212Ptr;
    int TP212_alreadyLaunched;
    _checkInCodelets212 checkInCodelets212;
    _checkInCodelets244 checkInCodelets244;
    TP211(int in_numThreads, int in_mainCodeletID, TP7* in_TPParent,
        _task211Inputs* in_task211Inputs);
    ~TP211();
};
/*TP235: OMPParallelDirective*/
class TP235 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets235 : public darts::Codelet {
    public:
        TP235* inputsTPParent;
        _barrierCodelets235()
            : darts::Codelet()
        {
        }
        _barrierCodelets235(uint32_t dep, uint32_t res, TP235* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets237 : public darts::Codelet {
    public:
        TP235* myTP;
        TP235* inputsTPParent;
        _checkInCodelets237()
            : darts::Codelet()
        {
        }
        _checkInCodelets237(uint32_t dep, uint32_t res, TP235* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP235* TPParent;
    TP235* controlTPParent;
    TP235* inputsTPParent;
    Node_t** root_darts235; /*OMP_SHARED - INPUT*/
    TP237* TP237Ptr;
    size_t TP237_alreadyLaunched;
    _barrierCodelets235* barrierCodelets235;
    _checkInCodelets237* checkInCodelets237;
    TP235(
        int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet, Node_t** in_root);
    ~TP235();
};
/*TP237: OMPSingleDirective*/
class TP237 : public ompOMPSingleDirectiveTP {
public:
    class _checkInCodelets239 : public darts::Codelet {
    public:
        TP237* myTP;
        TP237* inputsTPParent;
        _checkInCodelets239()
            : darts::Codelet()
        {
        }
        _checkInCodelets239(uint32_t dep, uint32_t res, TP237* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets240 : public darts::Codelet {
    public:
        TP237* myTP;
        TP237* inputsTPParent;
        _checkInCodelets240()
            : darts::Codelet()
        {
        }
        _checkInCodelets240(uint32_t dep, uint32_t res, TP237* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP235* TPParent;
    TP237* controlTPParent;
    TP237* inputsTPParent;
    Node_t** root_darts237; /*OMP_SHARED - INPUT*/
    TP_getDistanceToRootTasks* TP239Ptr;
    int TP239_alreadyLaunched;
    _checkInCodelets239 checkInCodelets239;
    _checkInCodelets240 checkInCodelets240;
    TP237(int in_numThreads, int in_mainCodeletID, TP235* in_TPParent, Node_t** in_root);
    ~TP237();
};
#endif
