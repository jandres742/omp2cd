#ifndef _schedbench_output_darts_h_
#define _schedbench_output_darts_h_
#ifndef __DARTS_
#define __DARTS_
#endif
#include "common.h"
#include "common.output.darts.h"
#include "darts.h"
#include "ompTP.h"
#include "schedbench.h"
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
void testdynamicn();
void teststaticn();
void teststatic();
void testguidedn();
void refer();
int main(int argc, char** argv);
class TP447;
class TP449;
class TP454;
/*Number of TPs to be used for the OMPFor in region TP454*/
#define NUMTPS454 NUMTPS
class TP488;
class TP490;
class TP495;
/*Number of TPs to be used for the OMPFor in region TP495*/
#define NUMTPS495 NUMTPS
class TP529;
class TP531;
class TP536;
/*Number of TPs to be used for the OMPFor in region TP536*/
#define NUMTPS536 NUMTPS
class TP570;
class TP572;
class TP577;
/*Number of TPs to be used for the OMPFor in region TP577*/
#define NUMTPS577 NUMTPS
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
/*TP447: OMPParallelDirective*/
class TP447 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets447 : public darts::Codelet {
    public:
        TP447* inputsTPParent;
        _barrierCodelets447()
            : darts::Codelet()
        {
        }
        _barrierCodelets447(uint32_t dep, uint32_t res, TP447* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets450 : public darts::Codelet {
    public:
        TP447* myTP;
        TP447* inputsTPParent;
        _checkInCodelets450()
            : darts::Codelet()
        {
        }
        _checkInCodelets450(uint32_t dep, uint32_t res, TP447* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets449 : public darts::Codelet {
    public:
        TP447* myTP;
        TP447* inputsTPParent;
        _checkInCodelets449()
            : darts::Codelet()
        {
        }
        _checkInCodelets449(uint32_t dep, uint32_t res, TP447* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets452 : public darts::Codelet {
    public:
        TP447* myTP;
        TP447* inputsTPParent;
        _checkInCodelets452()
            : darts::Codelet()
        {
        }
        _checkInCodelets452(uint32_t dep, uint32_t res, TP447* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP447* TPParent;
    TP447* controlTPParent;
    TP447* inputsTPParent;
    int* i_darts447; /*OMP_SHARED - INPUT*/
    int* j_darts447 /*OMP_PRIVATE - INPUT*/;
    unsigned int TP449_LoopCounter;
    unsigned int* TP449_LoopCounterPerThread;
    tbb::concurrent_vector<TP449*> TP449PtrVec;
    _barrierCodelets447* barrierCodelets447;
    _checkInCodelets450* checkInCodelets450;
    _checkInCodelets449* checkInCodelets449;
    _checkInCodelets452* checkInCodelets452;
    TP447(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet, int* in_i);
    ~TP447();
};
/*TP449: ForStmt*/
class TP449 : public ompTP {
public:
    class _checkInCodelets454 : public darts::Codelet {
    public:
        TP449* myTP;
        TP447* inputsTPParent;
        _checkInCodelets454()
            : darts::Codelet()
        {
        }
        _checkInCodelets454(uint32_t dep, uint32_t res, TP449* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets454 : public darts::Codelet {
    public:
        TP447* inputsTPParent;
        _barrierCodelets454()
            : darts::Codelet()
        {
        }
        _barrierCodelets454(uint32_t dep, uint32_t res, TP449* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP447* TPParent;
    TP449* controlTPParent;
    TP447* inputsTPParent;
    TP449** ptrToThisTP;
    TP454** TP454Ptr;
    size_t* TP454_alreadyLaunched;
    int numTPsSet454;
    int numTPsReady454;
    size_t TPsToUse454;
    size_t codeletsPerTP454;
    size_t totalCodelets454;
    _checkInCodelets454* checkInCodelets454;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets454* firstCodelet;
#endif
    _barrierCodelets454* barrierCodelets454;
    TP449(int in_numThreads, int in_mainCodeletID, TP447* in_TPParent, TP447* in_inputsTPParent,
        TP449** in_ptrToThisTP);
    ~TP449();
};
/*TP454: OMPForDirective*/
class TP454 : public ompTP {
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
    bool requestNewRangeIterations454(int* endRange, uint32_t codeletID);
    class _checkInCodelets455 : public darts::Codelet {
    public:
        TP454* myTP;
        TP454* inputsTPParent;
        int endRange;
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
    TP449* TPParent;
    TP454* controlTPParent;
    TP454* inputsTPParent;
    int* i_darts454 /*OMP_PRIVATE - INPUT*/;
    int initIteration454;
    int lastIteration454;
    int range454;
    int rangePerCodelet454;
    int minIteration454;
    int remainderRange454;
    size_t readyCodelets;
    int baseNumThreads;
    _barrierCodelets454* barrierCodelets454;
    _checkInCodelets455* checkInCodelets455;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets455* firstCodelet;
#endif
    TP454(int in_numThreads, int in_mainCodeletID, TP449* in_TPParent, int in_initIteration,
        int in_lastIteration, TP454** in_ptrToThisTP);
    void inline dispatchCodelet(size_t codeletID);
    ~TP454();
};
/*TP488: OMPParallelDirective*/
class TP488 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets488 : public darts::Codelet {
    public:
        TP488* inputsTPParent;
        _barrierCodelets488()
            : darts::Codelet()
        {
        }
        _barrierCodelets488(uint32_t dep, uint32_t res, TP488* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets491 : public darts::Codelet {
    public:
        TP488* myTP;
        TP488* inputsTPParent;
        _checkInCodelets491()
            : darts::Codelet()
        {
        }
        _checkInCodelets491(uint32_t dep, uint32_t res, TP488* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets490 : public darts::Codelet {
    public:
        TP488* myTP;
        TP488* inputsTPParent;
        _checkInCodelets490()
            : darts::Codelet()
        {
        }
        _checkInCodelets490(uint32_t dep, uint32_t res, TP488* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets493 : public darts::Codelet {
    public:
        TP488* myTP;
        TP488* inputsTPParent;
        _checkInCodelets493()
            : darts::Codelet()
        {
        }
        _checkInCodelets493(uint32_t dep, uint32_t res, TP488* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP488* TPParent;
    TP488* controlTPParent;
    TP488* inputsTPParent;
    int* i_darts488; /*OMP_SHARED - INPUT*/
    int* j_darts488 /*OMP_PRIVATE - INPUT*/;
    unsigned int TP490_LoopCounter;
    unsigned int* TP490_LoopCounterPerThread;
    tbb::concurrent_vector<TP490*> TP490PtrVec;
    _barrierCodelets488* barrierCodelets488;
    _checkInCodelets491* checkInCodelets491;
    _checkInCodelets490* checkInCodelets490;
    _checkInCodelets493* checkInCodelets493;
    TP488(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet, int* in_i);
    ~TP488();
};
/*TP490: ForStmt*/
class TP490 : public ompTP {
public:
    class _checkInCodelets495 : public darts::Codelet {
    public:
        TP490* myTP;
        TP488* inputsTPParent;
        _checkInCodelets495()
            : darts::Codelet()
        {
        }
        _checkInCodelets495(uint32_t dep, uint32_t res, TP490* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets495 : public darts::Codelet {
    public:
        TP488* inputsTPParent;
        _barrierCodelets495()
            : darts::Codelet()
        {
        }
        _barrierCodelets495(uint32_t dep, uint32_t res, TP490* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP488* TPParent;
    TP490* controlTPParent;
    TP488* inputsTPParent;
    TP490** ptrToThisTP;
    TP495** TP495Ptr;
    size_t* TP495_alreadyLaunched;
    int numTPsSet495;
    int numTPsReady495;
    size_t TPsToUse495;
    size_t codeletsPerTP495;
    size_t totalCodelets495;
    _checkInCodelets495* checkInCodelets495;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets495* firstCodelet;
#endif
    _barrierCodelets495* barrierCodelets495;
    TP490(int in_numThreads, int in_mainCodeletID, TP488* in_TPParent, TP488* in_inputsTPParent,
        TP490** in_ptrToThisTP);
    ~TP490();
};
/*TP495: OMPForDirective*/
class TP495 : public ompTP {
public:
    class _barrierCodelets495 : public darts::Codelet {
    public:
        TP495* inputsTPParent;
        _barrierCodelets495()
            : darts::Codelet()
        {
        }
        _barrierCodelets495(uint32_t dep, uint32_t res, TP495* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations495(int* endRange, uint32_t codeletID);
    class _checkInCodelets496 : public darts::Codelet {
    public:
        TP495* myTP;
        TP495* inputsTPParent;
        int endRange;
        _checkInCodelets496()
            : darts::Codelet()
        {
        }
        _checkInCodelets496(uint32_t dep, uint32_t res, TP495* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP490* TPParent;
    TP495* controlTPParent;
    TP495* inputsTPParent;
    int* i_darts495 /*OMP_PRIVATE - INPUT*/;
    int initIteration495;
    int lastIteration495;
    int range495;
    int rangePerCodelet495;
    int minIteration495;
    int remainderRange495;
    size_t readyCodelets;
    int baseNumThreads;
    _barrierCodelets495* barrierCodelets495;
    _checkInCodelets496* checkInCodelets496;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets496* firstCodelet;
#endif
    TP495(int in_numThreads, int in_mainCodeletID, TP490* in_TPParent, int in_initIteration,
        int in_lastIteration, TP495** in_ptrToThisTP);
    void inline dispatchCodelet(size_t codeletID);
    ~TP495();
};
/*TP529: OMPParallelDirective*/
class TP529 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets529 : public darts::Codelet {
    public:
        TP529* inputsTPParent;
        _barrierCodelets529()
            : darts::Codelet()
        {
        }
        _barrierCodelets529(uint32_t dep, uint32_t res, TP529* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets532 : public darts::Codelet {
    public:
        TP529* myTP;
        TP529* inputsTPParent;
        _checkInCodelets532()
            : darts::Codelet()
        {
        }
        _checkInCodelets532(uint32_t dep, uint32_t res, TP529* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets531 : public darts::Codelet {
    public:
        TP529* myTP;
        TP529* inputsTPParent;
        _checkInCodelets531()
            : darts::Codelet()
        {
        }
        _checkInCodelets531(uint32_t dep, uint32_t res, TP529* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets534 : public darts::Codelet {
    public:
        TP529* myTP;
        TP529* inputsTPParent;
        _checkInCodelets534()
            : darts::Codelet()
        {
        }
        _checkInCodelets534(uint32_t dep, uint32_t res, TP529* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP529* TPParent;
    TP529* controlTPParent;
    TP529* inputsTPParent;
    int* i_darts529; /*OMP_SHARED - INPUT*/
    int* j_darts529 /*OMP_PRIVATE - INPUT*/;
    unsigned int TP531_LoopCounter;
    unsigned int* TP531_LoopCounterPerThread;
    tbb::concurrent_vector<TP531*> TP531PtrVec;
    _barrierCodelets529* barrierCodelets529;
    _checkInCodelets532* checkInCodelets532;
    _checkInCodelets531* checkInCodelets531;
    _checkInCodelets534* checkInCodelets534;
    TP529(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet, int* in_i);
    ~TP529();
};
/*TP531: ForStmt*/
class TP531 : public ompTP {
public:
    class _checkInCodelets536 : public darts::Codelet {
    public:
        TP531* myTP;
        TP529* inputsTPParent;
        _checkInCodelets536()
            : darts::Codelet()
        {
        }
        _checkInCodelets536(uint32_t dep, uint32_t res, TP531* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets536 : public darts::Codelet {
    public:
        TP529* inputsTPParent;
        _barrierCodelets536()
            : darts::Codelet()
        {
        }
        _barrierCodelets536(uint32_t dep, uint32_t res, TP531* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP529* TPParent;
    TP531* controlTPParent;
    TP529* inputsTPParent;
    TP531** ptrToThisTP;
    TP536** TP536Ptr;
    size_t* TP536_alreadyLaunched;
    int numTPsSet536;
    int numTPsReady536;
    size_t TPsToUse536;
    size_t codeletsPerTP536;
    size_t totalCodelets536;
    _checkInCodelets536* checkInCodelets536;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets536* firstCodelet;
#endif
    _barrierCodelets536* barrierCodelets536;
    TP531(int in_numThreads, int in_mainCodeletID, TP529* in_TPParent, TP529* in_inputsTPParent,
        TP531** in_ptrToThisTP);
    ~TP531();
};
/*TP536: OMPForDirective*/
class TP536 : public ompTP {
public:
    class _barrierCodelets536 : public darts::Codelet {
    public:
        TP536* inputsTPParent;
        _barrierCodelets536()
            : darts::Codelet()
        {
        }
        _barrierCodelets536(uint32_t dep, uint32_t res, TP536* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations536(int* endRange, uint32_t codeletID);
    class _checkInCodelets537 : public darts::Codelet {
    public:
        TP536* myTP;
        TP536* inputsTPParent;
        int endRange;
        _checkInCodelets537()
            : darts::Codelet()
        {
        }
        _checkInCodelets537(uint32_t dep, uint32_t res, TP536* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP531* TPParent;
    TP536* controlTPParent;
    TP536* inputsTPParent;
    int* i_darts536 /*OMP_PRIVATE - INPUT*/;
    int initIteration536;
    int lastIteration536;
    int nextIteration536;
    int loop536alreadySetUp;
    size_t readyCodelets;
    int baseNumThreads;
    _barrierCodelets536* barrierCodelets536;
    _checkInCodelets537* checkInCodelets537;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets537* firstCodelet;
#endif
    TP536(int in_numThreads, int in_mainCodeletID, TP531* in_TPParent, int in_initIteration,
        int in_lastIteration, TP536** in_ptrToThisTP);
    void inline dispatchCodelet(size_t codeletID);
    ~TP536();
};
/*TP570: OMPParallelDirective*/
class TP570 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets570 : public darts::Codelet {
    public:
        TP570* inputsTPParent;
        _barrierCodelets570()
            : darts::Codelet()
        {
        }
        _barrierCodelets570(uint32_t dep, uint32_t res, TP570* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets573 : public darts::Codelet {
    public:
        TP570* myTP;
        TP570* inputsTPParent;
        _checkInCodelets573()
            : darts::Codelet()
        {
        }
        _checkInCodelets573(uint32_t dep, uint32_t res, TP570* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets572 : public darts::Codelet {
    public:
        TP570* myTP;
        TP570* inputsTPParent;
        _checkInCodelets572()
            : darts::Codelet()
        {
        }
        _checkInCodelets572(uint32_t dep, uint32_t res, TP570* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets575 : public darts::Codelet {
    public:
        TP570* myTP;
        TP570* inputsTPParent;
        _checkInCodelets575()
            : darts::Codelet()
        {
        }
        _checkInCodelets575(uint32_t dep, uint32_t res, TP570* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP570* TPParent;
    TP570* controlTPParent;
    TP570* inputsTPParent;
    int* i_darts570; /*OMP_SHARED - INPUT*/
    int* j_darts570 /*OMP_PRIVATE - INPUT*/;
    unsigned int TP572_LoopCounter;
    unsigned int* TP572_LoopCounterPerThread;
    tbb::concurrent_vector<TP572*> TP572PtrVec;
    _barrierCodelets570* barrierCodelets570;
    _checkInCodelets573* checkInCodelets573;
    _checkInCodelets572* checkInCodelets572;
    _checkInCodelets575* checkInCodelets575;
    TP570(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet, int* in_i);
    ~TP570();
};
/*TP572: ForStmt*/
class TP572 : public ompTP {
public:
    class _checkInCodelets577 : public darts::Codelet {
    public:
        TP572* myTP;
        TP570* inputsTPParent;
        _checkInCodelets577()
            : darts::Codelet()
        {
        }
        _checkInCodelets577(uint32_t dep, uint32_t res, TP572* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets577 : public darts::Codelet {
    public:
        TP570* inputsTPParent;
        _barrierCodelets577()
            : darts::Codelet()
        {
        }
        _barrierCodelets577(uint32_t dep, uint32_t res, TP572* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP570* TPParent;
    TP572* controlTPParent;
    TP570* inputsTPParent;
    TP572** ptrToThisTP;
    TP577** TP577Ptr;
    size_t* TP577_alreadyLaunched;
    int numTPsSet577;
    int numTPsReady577;
    size_t TPsToUse577;
    size_t codeletsPerTP577;
    size_t totalCodelets577;
    _checkInCodelets577* checkInCodelets577;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets577* firstCodelet;
#endif
    _barrierCodelets577* barrierCodelets577;
    TP572(int in_numThreads, int in_mainCodeletID, TP570* in_TPParent, TP570* in_inputsTPParent,
        TP572** in_ptrToThisTP);
    ~TP572();
};
/*TP577: OMPForDirective*/
class TP577 : public ompTP {
public:
    class _barrierCodelets577 : public darts::Codelet {
    public:
        TP577* inputsTPParent;
        _barrierCodelets577()
            : darts::Codelet()
        {
        }
        _barrierCodelets577(uint32_t dep, uint32_t res, TP577* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations577(int* endRange, uint32_t codeletID);
    class _checkInCodelets578 : public darts::Codelet {
    public:
        TP577* myTP;
        TP577* inputsTPParent;
        int endRange;
        _checkInCodelets578()
            : darts::Codelet()
        {
        }
        _checkInCodelets578(uint32_t dep, uint32_t res, TP577* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP572* TPParent;
    TP577* controlTPParent;
    TP577* inputsTPParent;
    int* i_darts577 /*OMP_PRIVATE - INPUT*/;
    int initIteration577;
    int lastIteration577;
    int nextIteration577;
    int loop577alreadySetUp;
    size_t readyCodelets;
    int baseNumThreads;
    _barrierCodelets577* barrierCodelets577;
    _checkInCodelets578* checkInCodelets578;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets578* firstCodelet;
#endif
    TP577(int in_numThreads, int in_mainCodeletID, TP572* in_TPParent, int in_initIteration,
        int in_lastIteration, TP577** in_ptrToThisTP);
    void inline dispatchCodelet(size_t codeletID);
    ~TP577();
};
#endif
