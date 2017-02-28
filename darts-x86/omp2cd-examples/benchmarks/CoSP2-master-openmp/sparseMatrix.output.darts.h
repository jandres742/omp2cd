#ifndef _sparseMatrix_output_darts_h_
#define _sparseMatrix_output_darts_h_
#ifndef __DARTS_
#define __DARTS_
#endif
#include "darts.h"
#include "ompTP.h"
#include "sparseMatrix.h"
#include "tbb/concurrent_vector.h"
#include "utils.h"
#include <limits.h>
#include <mutex>
#include <numa.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
void trace(struct SparseMatrixSt* spmatrix, struct DomainSt* domain, real_t* tr, real_t* tr2);
void normalize(struct SparseMatrixSt* spmatrix);
void gershgorin(struct SparseMatrixSt* spmatrix, struct DomainSt* domain);
void sparsity(struct SparseMatrixSt* spmatrix);
void destroySparseMatrix(struct SparseMatrixSt* spmatrix);
SparseMatrix* initSparseMatrix(int hsize, int msize);
int nnzStart(int hsize, int msize);
class TP397;
class TP477;
class TP565;
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
/*TP397: OMPParallelForDirective*/
class TP397 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets397 : public darts::Codelet {
    public:
        TP397* inputsTPParent;
        _barrierCodelets397()
            : darts::Codelet()
        {
        }
        _barrierCodelets397(uint32_t dep, uint32_t res, TP397* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations397(int* endRange, uint32_t codeletID);
    class _checkInCodelets398 : public darts::Codelet {
    public:
        TP397* myTP;
        TP397* inputsTPParent;
        int endRange;
        _checkInCodelets398()
            : darts::Codelet()
        {
        }
        _checkInCodelets398(uint32_t dep, uint32_t res, TP397* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP397* TPParent;
    TP397* controlTPParent;
    TP397* inputsTPParent;
    real_t* eMax_darts397; /*OMP_SHARED - INPUT*/
    real_t* eMin_darts397; /*OMP_SHARED - INPUT*/
    int* hsize_darts397; /*OMP_SHARED - INPUT*/
    struct SparseMatrixSt** spmatrix_darts397; /*OMP_SHARED - INPUT*/
    real_t* sumM_darts397 /*OMP_PRIVATE - INPUT*/;
    real_t* sumP_darts397 /*OMP_PRIVATE - INPUT*/;
    int* i_darts397 /*VARIABLE*/;
    int initIteration397;
    int lastIteration397;
    int range397;
    int rangePerCodelet397;
    int minIteration397;
    int remainderRange397;
    std::mutex eMin_darts397_mutex;
    std::mutex eMax_darts397_mutex;
    _barrierCodelets397* barrierCodelets397;
    _checkInCodelets398* checkInCodelets398;
    TP397(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet,
        int in_initIteration, int in_lastIteration, real_t* in_eMax, real_t* in_eMin, int* in_hsize,
        struct SparseMatrixSt** in_spmatrix);
    ~TP397();
};
/*TP477: OMPParallelForDirective*/
class TP477 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets477 : public darts::Codelet {
    public:
        TP477* inputsTPParent;
        _barrierCodelets477()
            : darts::Codelet()
        {
        }
        _barrierCodelets477(uint32_t dep, uint32_t res, TP477* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations477(int* endRange, uint32_t codeletID);
    class _checkInCodelets478 : public darts::Codelet {
    public:
        TP477* myTP;
        TP477* inputsTPParent;
        int endRange;
        _checkInCodelets478()
            : darts::Codelet()
        {
        }
        _checkInCodelets478(uint32_t dep, uint32_t res, TP477* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP477* TPParent;
    TP477* controlTPParent;
    TP477* inputsTPParent;
    int* hsize_darts477; /*OMP_SHARED - INPUT*/
    int* maxIia_darts477; /*OMP_SHARED - INPUT*/
    struct SparseMatrixSt** spmatrix_darts477; /*OMP_SHARED - INPUT*/
    int* sumIia_darts477; /*OMP_SHARED - INPUT*/
    int* i_darts477 /*VARIABLE*/;
    int initIteration477;
    int lastIteration477;
    int range477;
    int rangePerCodelet477;
    int minIteration477;
    int remainderRange477;
    std::mutex maxIia_darts477_mutex;
    std::mutex sumIia_darts477_mutex;
    _barrierCodelets477* barrierCodelets477;
    _checkInCodelets478* checkInCodelets478;
    TP477(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet,
        int in_initIteration, int in_lastIteration, int* in_hsize, int* in_maxIia,
        struct SparseMatrixSt** in_spmatrix, int* in_sumIia);
    ~TP477();
};
/*TP565: OMPParallelForDirective*/
class TP565 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets565 : public darts::Codelet {
    public:
        TP565* inputsTPParent;
        _barrierCodelets565()
            : darts::Codelet()
        {
        }
        _barrierCodelets565(uint32_t dep, uint32_t res, TP565* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations565(int* endRange, uint32_t codeletID);
    class _checkInCodelets566 : public darts::Codelet {
    public:
        TP565* myTP;
        TP565* inputsTPParent;
        int endRange;
        _checkInCodelets566()
            : darts::Codelet()
        {
        }
        _checkInCodelets566(uint32_t dep, uint32_t res, TP565* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP565* TPParent;
    TP565* controlTPParent;
    TP565* inputsTPParent;
    struct DomainSt** domain_darts565; /*OMP_SHARED - INPUT*/
    struct SparseMatrixSt** spmatrix_darts565; /*OMP_SHARED - INPUT*/
    real_t* trace_darts565; /*OMP_SHARED - INPUT*/
    real_t* trace2_darts565; /*OMP_SHARED - INPUT*/
    int* i_darts565 /*VARIABLE*/;
    int initIteration565;
    int lastIteration565;
    int range565;
    int rangePerCodelet565;
    int minIteration565;
    int remainderRange565;
    std::mutex trace2_darts565_mutex;
    std::mutex trace_darts565_mutex;
    _barrierCodelets565* barrierCodelets565;
    _checkInCodelets566* checkInCodelets566;
    TP565(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet,
        int in_initIteration, int in_lastIteration, struct DomainSt** in_domain,
        struct SparseMatrixSt** in_spmatrix, real_t* in_trace, real_t* in_trace2);
    ~TP565();
};
#endif
