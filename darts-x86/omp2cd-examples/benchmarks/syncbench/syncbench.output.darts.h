#ifndef _syncbench_output_darts_h_
#define _syncbench_output_darts_h_
#ifndef __DARTS_
#define __DARTS_
#endif
#include "common.h"
#include "common.output.darts.h"
#include "darts.h"
#include "ompTP.h"
#include "syncbench.h"
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
void referred();
int main(int argc, char** argv);
void testcrit();
void refer();
void testpr();
void testatom();
void testpfor();
void testlock();
void testfor();
void testred();
void referatom();
void testbar();
void testsing();
class TP458;
class TP463;
class TP465;
class TP470;
/*Number of TPs to be used for the OMPFor in region TP470*/
#define NUMTPS470 NUMTPS
class TP508;
class TP541;
class TP543;
class TP552;
class TP554;
class TP563;
class TP576;
class TP634;
class TP660;
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
/*TP458: OMPParallelDirective*/
class TP458 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets458 : public darts::Codelet {
    public:
        TP458* inputsTPParent;
        _barrierCodelets458()
            : darts::Codelet()
        {
        }
        _barrierCodelets458(uint32_t dep, uint32_t res, TP458* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets460 : public darts::Codelet {
    public:
        TP458* myTP;
        TP458* inputsTPParent;
        _checkInCodelets460()
            : darts::Codelet()
        {
        }
        _checkInCodelets460(uint32_t dep, uint32_t res, TP458* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP458* TPParent;
    TP458* controlTPParent;
    TP458* inputsTPParent;
    _barrierCodelets458* barrierCodelets458;
    _checkInCodelets460* checkInCodelets460;
    TP458(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet);
    ~TP458();
};
/*TP463: OMPParallelDirective*/
class TP463 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets463 : public darts::Codelet {
    public:
        TP463* inputsTPParent;
        _barrierCodelets463()
            : darts::Codelet()
        {
        }
        _barrierCodelets463(uint32_t dep, uint32_t res, TP463* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets466 : public darts::Codelet {
    public:
        TP463* myTP;
        TP463* inputsTPParent;
        _checkInCodelets466()
            : darts::Codelet()
        {
        }
        _checkInCodelets466(uint32_t dep, uint32_t res, TP463* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets465 : public darts::Codelet {
    public:
        TP463* myTP;
        TP463* inputsTPParent;
        _checkInCodelets465()
            : darts::Codelet()
        {
        }
        _checkInCodelets465(uint32_t dep, uint32_t res, TP463* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets468 : public darts::Codelet {
    public:
        TP463* myTP;
        TP463* inputsTPParent;
        _checkInCodelets468()
            : darts::Codelet()
        {
        }
        _checkInCodelets468(uint32_t dep, uint32_t res, TP463* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP463* TPParent;
    TP463* controlTPParent;
    TP463* inputsTPParent;
    int* i_darts463; /*OMP_SHARED - INPUT*/
    int* j_darts463 /*OMP_PRIVATE - INPUT*/;
    unsigned int TP465_LoopCounter;
    unsigned int* TP465_LoopCounterPerThread;
    tbb::concurrent_vector<TP465*> TP465PtrVec;
    _barrierCodelets463* barrierCodelets463;
    _checkInCodelets466* checkInCodelets466;
    _checkInCodelets465* checkInCodelets465;
    _checkInCodelets468* checkInCodelets468;
    TP463(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet, int* in_i);
    ~TP463();
};
/*TP465: ForStmt*/
class TP465 : public ompTP {
public:
    class _checkInCodelets470 : public darts::Codelet {
    public:
        TP465* myTP;
        TP463* inputsTPParent;
        _checkInCodelets470()
            : darts::Codelet()
        {
        }
        _checkInCodelets470(uint32_t dep, uint32_t res, TP465* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets470 : public darts::Codelet {
    public:
        TP463* inputsTPParent;
        _barrierCodelets470()
            : darts::Codelet()
        {
        }
        _barrierCodelets470(uint32_t dep, uint32_t res, TP465* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP463* TPParent;
    TP465* controlTPParent;
    TP463* inputsTPParent;
    TP465** ptrToThisTP;
    TP470** TP470Ptr;
    size_t* TP470_alreadyLaunched;
    int numTPsSet470;
    int numTPsReady470;
    size_t TPsToUse470;
    size_t codeletsPerTP470;
    size_t totalCodelets470;
    _checkInCodelets470* checkInCodelets470;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets470* firstCodelet;
#endif
    _barrierCodelets470* barrierCodelets470;
    TP465(int in_numThreads, int in_mainCodeletID, TP463* in_TPParent, TP463* in_inputsTPParent,
        TP465** in_ptrToThisTP);
    ~TP465();
};
/*TP470: OMPForDirective*/
class TP470 : public ompTP {
public:
    class _barrierCodelets470 : public darts::Codelet {
    public:
        TP470* inputsTPParent;
        _barrierCodelets470()
            : darts::Codelet()
        {
        }
        _barrierCodelets470(uint32_t dep, uint32_t res, TP470* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations470(int* endRange, uint32_t codeletID);
    class _checkInCodelets471 : public darts::Codelet {
    public:
        TP470* myTP;
        TP470* inputsTPParent;
        int endRange;
        _checkInCodelets471()
            : darts::Codelet()
        {
        }
        _checkInCodelets471(uint32_t dep, uint32_t res, TP470* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP465* TPParent;
    TP470* controlTPParent;
    TP470* inputsTPParent;
    int* i_darts470 /*OMP_PRIVATE - INPUT*/;
    int initIteration470;
    int lastIteration470;
    int range470;
    int rangePerCodelet470;
    int minIteration470;
    int remainderRange470;
    size_t readyCodelets;
    int baseNumThreads;
    _barrierCodelets470* barrierCodelets470;
    _checkInCodelets471* checkInCodelets471;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets471* firstCodelet;
#endif
    TP470(int in_numThreads, int in_mainCodeletID, TP465* in_TPParent, int in_initIteration,
        int in_lastIteration, TP470** in_ptrToThisTP);
    void inline dispatchCodelet(size_t codeletID);
    ~TP470();
};
/*TP508: OMPParallelForDirective*/
class TP508 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets508 : public darts::Codelet {
    public:
        TP508* inputsTPParent;
        _barrierCodelets508()
            : darts::Codelet()
        {
        }
        _barrierCodelets508(uint32_t dep, uint32_t res, TP508* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations508(int* endRange, uint32_t codeletID);
    class _checkInCodelets509 : public darts::Codelet {
    public:
        TP508* myTP;
        TP508* inputsTPParent;
        int endRange;
        _checkInCodelets509()
            : darts::Codelet()
        {
        }
        _checkInCodelets509(uint32_t dep, uint32_t res, TP508* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP508* TPParent;
    TP508* controlTPParent;
    TP508* inputsTPParent;
    int* i_darts508 /*OMP_PRIVATE - INPUT*/;
    int initIteration508;
    int lastIteration508;
    int range508;
    int rangePerCodelet508;
    int minIteration508;
    int remainderRange508;
    _barrierCodelets508* barrierCodelets508;
    _checkInCodelets509* checkInCodelets509;
    TP508(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet,
        int in_initIteration, int in_lastIteration);
    ~TP508();
};
/*TP541: OMPParallelDirective*/
class TP541 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets541 : public darts::Codelet {
    public:
        TP541* inputsTPParent;
        _barrierCodelets541()
            : darts::Codelet()
        {
        }
        _barrierCodelets541(uint32_t dep, uint32_t res, TP541* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets544 : public darts::Codelet {
    public:
        TP541* myTP;
        TP541* inputsTPParent;
        _checkInCodelets544()
            : darts::Codelet()
        {
        }
        _checkInCodelets544(uint32_t dep, uint32_t res, TP541* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets543 : public darts::Codelet {
    public:
        TP541* myTP;
        TP541* inputsTPParent;
        _checkInCodelets543()
            : darts::Codelet()
        {
        }
        _checkInCodelets543(uint32_t dep, uint32_t res, TP541* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets546 : public darts::Codelet {
    public:
        TP541* myTP;
        TP541* inputsTPParent;
        _checkInCodelets546()
            : darts::Codelet()
        {
        }
        _checkInCodelets546(uint32_t dep, uint32_t res, TP541* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP541* TPParent;
    TP541* controlTPParent;
    TP541* inputsTPParent;
    int* j_darts541 /*OMP_PRIVATE - INPUT*/;
    unsigned int TP543_LoopCounter;
    unsigned int* TP543_LoopCounterPerThread;
    tbb::concurrent_vector<TP543*> TP543PtrVec;
    _barrierCodelets541* barrierCodelets541;
    _checkInCodelets544* checkInCodelets544;
    _checkInCodelets543* checkInCodelets543;
    _checkInCodelets546* checkInCodelets546;
    TP541(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet);
    ~TP541();
};
/*TP543: ForStmt*/
class TP543 : public ompTP {
public:
    class _checkInCodelets548 : public darts::Codelet {
    public:
        TP543* myTP;
        TP541* inputsTPParent;
        _checkInCodelets548()
            : darts::Codelet()
        {
        }
        _checkInCodelets548(uint32_t dep, uint32_t res, TP543* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets549 : public darts::Codelet {
    public:
        TP541* inputsTPParent;
        _barrierCodelets549()
            : darts::Codelet()
        {
        }
        _barrierCodelets549(uint32_t dep, uint32_t res, TP543* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP541* TPParent;
    TP543* controlTPParent;
    TP541* inputsTPParent;
    TP543** ptrToThisTP;
    _checkInCodelets548* checkInCodelets548;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets548* firstCodelet;
#endif
    _barrierCodelets549* barrierCodelets549;
    TP543(int in_numThreads, int in_mainCodeletID, TP541* in_TPParent, TP541* in_inputsTPParent,
        TP543** in_ptrToThisTP);
    ~TP543();
};
/*TP552: OMPParallelDirective*/
class TP552 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets552 : public darts::Codelet {
    public:
        TP552* inputsTPParent;
        _barrierCodelets552()
            : darts::Codelet()
        {
        }
        _barrierCodelets552(uint32_t dep, uint32_t res, TP552* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets555 : public darts::Codelet {
    public:
        TP552* myTP;
        TP552* inputsTPParent;
        _checkInCodelets555()
            : darts::Codelet()
        {
        }
        _checkInCodelets555(uint32_t dep, uint32_t res, TP552* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets554 : public darts::Codelet {
    public:
        TP552* myTP;
        TP552* inputsTPParent;
        _checkInCodelets554()
            : darts::Codelet()
        {
        }
        _checkInCodelets554(uint32_t dep, uint32_t res, TP552* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets557 : public darts::Codelet {
    public:
        TP552* myTP;
        TP552* inputsTPParent;
        _checkInCodelets557()
            : darts::Codelet()
        {
        }
        _checkInCodelets557(uint32_t dep, uint32_t res, TP552* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP552* TPParent;
    TP552* controlTPParent;
    TP552* inputsTPParent;
    int* j_darts552 /*OMP_PRIVATE - INPUT*/;
    unsigned int TP554_LoopCounter;
    unsigned int* TP554_LoopCounterPerThread;
    tbb::concurrent_vector<TP554*> TP554PtrVec;
    _barrierCodelets552* barrierCodelets552;
    _checkInCodelets555* checkInCodelets555;
    _checkInCodelets554* checkInCodelets554;
    _checkInCodelets557* checkInCodelets557;
    TP552(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet);
    ~TP552();
};
/*TP554: ForStmt*/
class TP554 : public ompTP {
public:
    class _checkInCodelets559 : public darts::Codelet {
    public:
        TP554* myTP;
        TP552* inputsTPParent;
        _checkInCodelets559()
            : darts::Codelet()
        {
        }
        _checkInCodelets559(uint32_t dep, uint32_t res, TP554* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets559 : public darts::Codelet {
    public:
        TP552* inputsTPParent;
        _barrierCodelets559()
            : darts::Codelet()
        {
        }
        _barrierCodelets559(uint32_t dep, uint32_t res, TP554* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP552* TPParent;
    TP554* controlTPParent;
    TP552* inputsTPParent;
    TP554** ptrToThisTP;
    size_t TP559_alreadyLaunched;
    _checkInCodelets559* checkInCodelets559;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets559* firstCodelet;
#endif
    _barrierCodelets559* barrierCodelets559;
    TP554(int in_numThreads, int in_mainCodeletID, TP552* in_TPParent, TP552* in_inputsTPParent,
        TP554** in_ptrToThisTP);
    ~TP554();
};
/*TP563: OMPParallelDirective*/
class TP563 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets563 : public darts::Codelet {
    public:
        TP563* inputsTPParent;
        _barrierCodelets563()
            : darts::Codelet()
        {
        }
        _barrierCodelets563(uint32_t dep, uint32_t res, TP563* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets565 : public darts::Codelet {
    public:
        TP563* myTP;
        TP563* inputsTPParent;
        _checkInCodelets565()
            : darts::Codelet()
        {
        }
        _checkInCodelets565(uint32_t dep, uint32_t res, TP563* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP563* TPParent;
    TP563* controlTPParent;
    TP563* inputsTPParent;
    int* j_darts563 /*OMP_PRIVATE - INPUT*/;
    _barrierCodelets563* barrierCodelets563;
    _checkInCodelets565* checkInCodelets565;
    TP563(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet);
    ~TP563();
};
/*TP576: OMPParallelDirective*/
class TP576 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets576 : public darts::Codelet {
    public:
        TP576* inputsTPParent;
        _barrierCodelets576()
            : darts::Codelet()
        {
        }
        _barrierCodelets576(uint32_t dep, uint32_t res, TP576* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets578 : public darts::Codelet {
    public:
        TP576* myTP;
        TP576* inputsTPParent;
        _checkInCodelets578()
            : darts::Codelet()
        {
        }
        _checkInCodelets578(uint32_t dep, uint32_t res, TP576* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP576* TPParent;
    TP576* controlTPParent;
    TP576* inputsTPParent;
    int* j_darts576 /*OMP_PRIVATE - INPUT*/;
    _barrierCodelets576* barrierCodelets576;
    _checkInCodelets578* checkInCodelets578;
    TP576(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet);
    ~TP576();
};
/*TP634: OMPParallelDirective*/
class TP634 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets634 : public darts::Codelet {
    public:
        TP634* inputsTPParent;
        _barrierCodelets634()
            : darts::Codelet()
        {
        }
        _barrierCodelets634(uint32_t dep, uint32_t res, TP634* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets636 : public darts::Codelet {
    public:
        TP634* myTP;
        TP634* inputsTPParent;
        _checkInCodelets636()
            : darts::Codelet()
        {
        }
        _checkInCodelets636(uint32_t dep, uint32_t res, TP634* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP634* TPParent;
    TP634* controlTPParent;
    TP634* inputsTPParent;
    double* aaaa_darts634; /*OMP_SHARED - INPUT*/
    double* b_darts634 /*OMP_FIRSTPRIVATE - INPUT*/;
    double* b_outer634_ptr;
    double* c_darts634; /*OMP_SHARED - INPUT*/
    int* j_darts634 /*OMP_PRIVATE - INPUT*/;
    _barrierCodelets634* barrierCodelets634;
    _checkInCodelets636* checkInCodelets636;
    TP634(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet, double* in_aaaa,
        double* in_b_outer634_ptr, double* in_c);
    ~TP634();
};
/*TP660: OMPParallelDirective*/
class TP660 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets660 : public darts::Codelet {
    public:
        TP660* inputsTPParent;
        _barrierCodelets660()
            : darts::Codelet()
        {
        }
        _barrierCodelets660(uint32_t dep, uint32_t res, TP660* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets662 : public darts::Codelet {
    public:
        TP660* myTP;
        TP660* inputsTPParent;
        _checkInCodelets662()
            : darts::Codelet()
        {
        }
        _checkInCodelets662(uint32_t dep, uint32_t res, TP660* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP660* TPParent;
    TP660* controlTPParent;
    TP660* inputsTPParent;
    int* aaaa_darts660; /*OMP_SHARED - INPUT*/
    std::mutex aaaa_darts660_mutex;
    std::mutex aaaa_darts660_reductionVar_mutex;
    int* aaaa_darts660_reductionVar;
    size_t aaaa_darts660_reductionVar_counter;
    _barrierCodelets660* barrierCodelets660;
    _checkInCodelets662* checkInCodelets662;
    TP660(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet, int* in_aaaa);
    ~TP660();
};
#endif
