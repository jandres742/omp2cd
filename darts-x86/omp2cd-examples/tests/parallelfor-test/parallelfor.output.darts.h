#ifndef _parallelfor_output_darts_h_
#define _parallelfor_output_darts_h_
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
int applyFunction(int x);
class TP75;
class TP80;
/*Number of TPs to be used for the OMPFor in region TP80*/
#define NUMTPS80 NUMTPS
class TP123;
/*Number of TPs to be used for the OMPFor in region TP123*/
#define NUMTPS123 NUMTPS
class TP168;
/*Number of TPs to be used for the OMPFor in region TP168*/
#define NUMTPS168 NUMTPS
class TP215;
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
/*TP75: OMPParallelDirective*/
class TP75 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets75 : public darts::Codelet {
    public:
        TP75* inputsTPParent;
        _barrierCodelets75()
            : darts::Codelet()
        {
        }
        _barrierCodelets75(uint32_t dep, uint32_t res, TP75* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets77 : public darts::Codelet {
    public:
        TP75* myTP;
        TP75* inputsTPParent;
        _checkInCodelets77()
            : darts::Codelet()
        {
        }
        _checkInCodelets77(uint32_t dep, uint32_t res, TP75* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets80 : public darts::Codelet {
    public:
        TP75* myTP;
        TP75* inputsTPParent;
        _checkInCodelets80()
            : darts::Codelet()
        {
        }
        _checkInCodelets80(uint32_t dep, uint32_t res, TP75* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets80 : public darts::Codelet {
    public:
        TP75* inputsTPParent;
        _barrierCodelets80()
            : darts::Codelet()
        {
        }
        _barrierCodelets80(uint32_t dep, uint32_t res, TP75* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets123 : public darts::Codelet {
    public:
        TP75* myTP;
        TP75* inputsTPParent;
        _checkInCodelets123()
            : darts::Codelet()
        {
        }
        _checkInCodelets123(uint32_t dep, uint32_t res, TP75* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets123 : public darts::Codelet {
    public:
        TP75* inputsTPParent;
        _barrierCodelets123()
            : darts::Codelet()
        {
        }
        _barrierCodelets123(uint32_t dep, uint32_t res, TP75* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets168 : public darts::Codelet {
    public:
        TP75* myTP;
        TP75* inputsTPParent;
        _checkInCodelets168()
            : darts::Codelet()
        {
        }
        _checkInCodelets168(uint32_t dep, uint32_t res, TP75* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets168 : public darts::Codelet {
    public:
        TP75* inputsTPParent;
        _barrierCodelets168()
            : darts::Codelet()
        {
        }
        _barrierCodelets168(uint32_t dep, uint32_t res, TP75* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP75* TPParent;
    TP75* controlTPParent;
    TP75* inputsTPParent;
    int* i_darts75; /*OMP_SHARED - INPUT*/
    int** inVector_darts75; /*OMP_SHARED - INPUT*/
    int** outVectorPar_darts75; /*OMP_SHARED - INPUT*/
    int* size_darts75; /*OMP_SHARED - INPUT*/
    int* xx_darts75 /*OMP_PRIVATE - INPUT*/;
    int* yy_darts75; /*OMP_SHARED - INPUT*/
    TP80** TP80Ptr;
    size_t* TP80_alreadyLaunched;
    int numTPsSet80;
    int numTPsReady80;
    size_t TPsToUse80;
    size_t codeletsPerTP80;
    size_t totalCodelets80;
    TP123** TP123Ptr;
    size_t* TP123_alreadyLaunched;
    int numTPsSet123;
    int numTPsReady123;
    size_t TPsToUse123;
    size_t codeletsPerTP123;
    size_t totalCodelets123;
    TP168** TP168Ptr;
    size_t* TP168_alreadyLaunched;
    int numTPsSet168;
    int numTPsReady168;
    size_t TPsToUse168;
    size_t codeletsPerTP168;
    size_t totalCodelets168;
    _barrierCodelets75* barrierCodelets75;
    _checkInCodelets77* checkInCodelets77;
    _checkInCodelets80* checkInCodelets80;
    _barrierCodelets80* barrierCodelets80;
    _checkInCodelets123* checkInCodelets123;
    _barrierCodelets123* barrierCodelets123;
    _checkInCodelets168* checkInCodelets168;
    _barrierCodelets168* barrierCodelets168;
    TP75(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet, int* in_i,
        int** in_inVector, int** in_outVectorPar, int* in_size, int* in_yy);
    ~TP75();
};
/*TP80: OMPForDirective*/
class TP80 : public ompTP {
public:
    class _barrierCodelets80 : public darts::Codelet {
    public:
        TP80* inputsTPParent;
        _barrierCodelets80()
            : darts::Codelet()
        {
        }
        _barrierCodelets80(uint32_t dep, uint32_t res, TP80* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations80(int* endRange, uint32_t codeletID);
    class _checkInCodelets81 : public darts::Codelet {
    public:
        TP80* myTP;
        TP80* inputsTPParent;
        int endRange;
        _checkInCodelets81()
            : darts::Codelet()
        {
        }
        _checkInCodelets81(uint32_t dep, uint32_t res, TP80* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP75* TPParent;
    TP80* controlTPParent;
    TP80* inputsTPParent;
    int* i_darts80 /*OMP_PRIVATE - INPUT*/;
    int** inVector_darts80; /*OMP_SHARED - INPUT*/
    int** outVectorPar_darts80; /*OMP_SHARED - INPUT*/
    int* size_darts80; /*OMP_SHARED - INPUT*/
    int initIteration80;
    int lastIteration80;
    size_t ompLoopSched80;
    size_t ompLoopChunk80;
    int range80;
    int rangePerCodelet80;
    int minIteration80;
    int remainderRange80;
    int nextIteration80;
    int loop80alreadySetUp;
    size_t readyCodelets;
    int baseNumThreads;
    _barrierCodelets80* barrierCodelets80;
    _checkInCodelets81* checkInCodelets81;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets81* firstCodelet;
#endif
    TP80(int in_numThreads, int in_mainCodeletID, TP75* in_TPParent, int in_initIteration,
        int in_lastIteration, int** in_inVector, int** in_outVectorPar, int* in_size,
        TP80** in_ptrToThisTP);
    void inline dispatchCodelet(size_t codeletID);
    ~TP80();
};
/*TP123: OMPForDirective*/
class TP123 : public ompTP {
public:
    class _barrierCodelets123 : public darts::Codelet {
    public:
        TP123* inputsTPParent;
        _barrierCodelets123()
            : darts::Codelet()
        {
        }
        _barrierCodelets123(uint32_t dep, uint32_t res, TP123* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations123(int* endRange, uint32_t codeletID);
    class _checkInCodelets124 : public darts::Codelet {
    public:
        TP123* myTP;
        TP123* inputsTPParent;
        int endRange;
        _checkInCodelets124()
            : darts::Codelet()
        {
        }
        _checkInCodelets124(uint32_t dep, uint32_t res, TP123* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP75* TPParent;
    TP123* controlTPParent;
    TP123* inputsTPParent;
    int* i_darts123 /*OMP_PRIVATE - INPUT*/;
    int** inVector_darts123; /*OMP_SHARED - INPUT*/
    int** outVectorPar_darts123; /*OMP_SHARED - INPUT*/
    int* size_darts123; /*OMP_SHARED - INPUT*/
    int initIteration123;
    int lastIteration123;
    int range123;
    int rangePerCodelet123;
    int minIteration123;
    int remainderRange123;
    size_t readyCodelets;
    int baseNumThreads;
    _barrierCodelets123* barrierCodelets123;
    _checkInCodelets124* checkInCodelets124;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets124* firstCodelet;
#endif
    TP123(int in_numThreads, int in_mainCodeletID, TP75* in_TPParent, int in_initIteration,
        int in_lastIteration, int** in_inVector, int** in_outVectorPar, int* in_size,
        TP123** in_ptrToThisTP);
    void inline dispatchCodelet(size_t codeletID);
    ~TP123();
};
/*TP168: OMPForDirective*/
class TP168 : public ompTP {
public:
    class _barrierCodelets168 : public darts::Codelet {
    public:
        TP168* inputsTPParent;
        _barrierCodelets168()
            : darts::Codelet()
        {
        }
        _barrierCodelets168(uint32_t dep, uint32_t res, TP168* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations168(int* endRange, uint32_t codeletID);
    class _checkInCodelets169 : public darts::Codelet {
    public:
        TP168* myTP;
        TP168* inputsTPParent;
        int endRange;
        _checkInCodelets169()
            : darts::Codelet()
        {
        }
        _checkInCodelets169(uint32_t dep, uint32_t res, TP168* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP75* TPParent;
    TP168* controlTPParent;
    TP168* inputsTPParent;
    int* i_darts168 /*OMP_PRIVATE - INPUT*/;
    int** inVector_darts168; /*OMP_SHARED - INPUT*/
    int** outVectorPar_darts168; /*OMP_SHARED - INPUT*/
    int* size_darts168; /*OMP_SHARED - INPUT*/
    int initIteration168;
    int lastIteration168;
    int nextIteration168;
    int loop168alreadySetUp;
    size_t readyCodelets;
    int baseNumThreads;
    _barrierCodelets168* barrierCodelets168;
    _checkInCodelets169* checkInCodelets169;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets169* firstCodelet;
#endif
    TP168(int in_numThreads, int in_mainCodeletID, TP75* in_TPParent, int in_initIteration,
        int in_lastIteration, int** in_inVector, int** in_outVectorPar, int* in_size,
        TP168** in_ptrToThisTP);
    void inline dispatchCodelet(size_t codeletID);
    ~TP168();
};
/*TP215: OMPParallelForDirective*/
class TP215 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets215 : public darts::Codelet {
    public:
        TP215* inputsTPParent;
        _barrierCodelets215()
            : darts::Codelet()
        {
        }
        _barrierCodelets215(uint32_t dep, uint32_t res, TP215* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations215(int* endRange, uint32_t codeletID);
    class _checkInCodelets216 : public darts::Codelet {
    public:
        TP215* myTP;
        TP215* inputsTPParent;
        int endRange;
        _checkInCodelets216()
            : darts::Codelet()
        {
        }
        _checkInCodelets216(uint32_t dep, uint32_t res, TP215* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP215* TPParent;
    TP215* controlTPParent;
    TP215* inputsTPParent;
    int* i_darts215 /*OMP_PRIVATE - INPUT*/;
    int** inVector_darts215; /*OMP_SHARED - INPUT*/
    int** outVectorPar_darts215; /*OMP_SHARED - INPUT*/
    int* size_darts215; /*OMP_SHARED - INPUT*/
    int initIteration215;
    int lastIteration215;
    int nextIteration215;
    int loop215alreadySetUp;
    _barrierCodelets215* barrierCodelets215;
    _checkInCodelets216* checkInCodelets216;
    TP215(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet,
        int in_initIteration, int in_lastIteration, int** in_inVector, int** in_outVectorPar,
        int* in_size);
    ~TP215();
};
#endif
