#ifndef _sparseMath_output_darts_h_
#define _sparseMath_output_darts_h_
#ifndef __DARTS_
#define __DARTS_
#endif
#include "darts.h"
#include "ompTP.h"
#include "sparseMath.h"
#include "tbb/concurrent_vector.h"
#include "utils.h"
#include <limits.h>
#include <mutex>
#include <numa.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
void sparseMultScalar(struct SparseMatrixSt* xmatrix, struct DomainSt* domain, real_t scalar);
void sparseSetX2(
    struct SparseMatrixSt* xmatrix, struct SparseMatrixSt* x2matrix, struct DomainSt* domain);
void sparseAdd(
    struct SparseMatrixSt* xmatrix, struct SparseMatrixSt* x2matrix, struct DomainSt* domain);
void sparseX2(real_t* trX, real_t* trX2, struct SparseMatrixSt* xmatrix,
    struct SparseMatrixSt* x2matrix, struct DomainSt* domain);
class TP641;
class TP787;
class TP930;
class TP996;
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
/*TP641: OMPParallelForDirective*/
class TP641 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets641 : public darts::Codelet {
    public:
        TP641* inputsTPParent;
        _barrierCodelets641()
            : darts::Codelet()
        {
        }
        _barrierCodelets641(uint32_t dep, uint32_t res, TP641* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations641(int* endRange, uint32_t codeletID);
    class _checkInCodelets642 : public darts::Codelet {
    public:
        TP641* myTP;
        TP641* inputsTPParent;
        int endRange;
        _checkInCodelets642()
            : darts::Codelet()
        {
        }
        _checkInCodelets642(uint32_t dep, uint32_t res, TP641* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP641* TPParent;
    TP641* controlTPParent;
    TP641* inputsTPParent;
    int* domain_localRowMax_darts641; /*OMP_SHARED - INPUT*/
    int* domain_localRowMin_darts641; /*OMP_SHARED - INPUT*/
    int** ix_darts641 /*OMP_FIRSTPRIVATE*/;
    uint64_t ix_outer641_size;
    int* ix_outer641_ptr;
    real_t* traceX_darts641; /*OMP_SHARED - INPUT*/
    real_t* traceX2_darts641; /*OMP_SHARED - INPUT*/
    real_t** x_darts641 /*OMP_FIRSTPRIVATE*/;
    uint64_t x_outer641_size;
    real_t* x_outer641_ptr;
    struct SparseMatrixSt** x2matrix_darts641; /*OMP_SHARED - INPUT*/
    struct SparseMatrixSt** xmatrix_darts641; /*OMP_SHARED - INPUT*/
    int* i_darts641 /*VARIABLE*/;
    int initIteration641;
    int lastIteration641;
    int range641;
    int rangePerCodelet641;
    int minIteration641;
    int remainderRange641;
    std::mutex traceX2_darts641_mutex;
    std::mutex traceX_darts641_mutex;
    _barrierCodelets641* barrierCodelets641;
    _checkInCodelets642* checkInCodelets642;
    TP641(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet,
        int in_initIteration, int in_lastIteration, int* in_domain_localRowMax,
        int* in_domain_localRowMin, int* in_ix_outer641_ptr, uint64_t in_ix_outer641_size,
        real_t* in_traceX, real_t* in_traceX2, real_t* in_x_outer641_ptr,
        uint64_t in_x_outer641_size, struct SparseMatrixSt** in_x2matrix,
        struct SparseMatrixSt** in_xmatrix);
    ~TP641();
};
/*TP787: OMPParallelForDirective*/
class TP787 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets787 : public darts::Codelet {
    public:
        TP787* inputsTPParent;
        _barrierCodelets787()
            : darts::Codelet()
        {
        }
        _barrierCodelets787(uint32_t dep, uint32_t res, TP787* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations787(int* endRange, uint32_t codeletID);
    class _checkInCodelets788 : public darts::Codelet {
    public:
        TP787* myTP;
        TP787* inputsTPParent;
        int endRange;
        _checkInCodelets788()
            : darts::Codelet()
        {
        }
        _checkInCodelets788(uint32_t dep, uint32_t res, TP787* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP787* TPParent;
    TP787* controlTPParent;
    TP787* inputsTPParent;
    int* domain_localRowMax_darts787; /*OMP_SHARED - INPUT*/
    int* domain_localRowMin_darts787; /*OMP_SHARED - INPUT*/
    int** ix_darts787 /*OMP_FIRSTPRIVATE*/;
    uint64_t ix_outer787_size;
    int* ix_outer787_ptr;
    real_t** x_darts787 /*OMP_FIRSTPRIVATE*/;
    uint64_t x_outer787_size;
    real_t* x_outer787_ptr;
    struct SparseMatrixSt** x2matrix_darts787; /*OMP_SHARED - INPUT*/
    struct SparseMatrixSt** xmatrix_darts787; /*OMP_SHARED - INPUT*/
    int* i_darts787 /*VARIABLE*/;
    int initIteration787;
    int lastIteration787;
    int range787;
    int rangePerCodelet787;
    int minIteration787;
    int remainderRange787;
    _barrierCodelets787* barrierCodelets787;
    _checkInCodelets788* checkInCodelets788;
    TP787(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet,
        int in_initIteration, int in_lastIteration, int* in_domain_localRowMax,
        int* in_domain_localRowMin, int* in_ix_outer787_ptr, uint64_t in_ix_outer787_size,
        real_t* in_x_outer787_ptr, uint64_t in_x_outer787_size, struct SparseMatrixSt** in_x2matrix,
        struct SparseMatrixSt** in_xmatrix);
    ~TP787();
};
/*TP930: OMPParallelForDirective*/
class TP930 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets930 : public darts::Codelet {
    public:
        TP930* inputsTPParent;
        _barrierCodelets930()
            : darts::Codelet()
        {
        }
        _barrierCodelets930(uint32_t dep, uint32_t res, TP930* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations930(int* endRange, uint32_t codeletID);
    class _checkInCodelets931 : public darts::Codelet {
    public:
        TP930* myTP;
        TP930* inputsTPParent;
        int endRange;
        _checkInCodelets931()
            : darts::Codelet()
        {
        }
        _checkInCodelets931(uint32_t dep, uint32_t res, TP930* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP930* TPParent;
    TP930* controlTPParent;
    TP930* inputsTPParent;
    int* domain_localRowMax_darts930; /*OMP_SHARED - INPUT*/
    int* domain_localRowMin_darts930; /*OMP_SHARED - INPUT*/
    struct SparseMatrixSt** x2matrix_darts930; /*OMP_SHARED - INPUT*/
    struct SparseMatrixSt** xmatrix_darts930; /*OMP_SHARED - INPUT*/
    int* i_darts930 /*VARIABLE*/;
    int initIteration930;
    int lastIteration930;
    int range930;
    int rangePerCodelet930;
    int minIteration930;
    int remainderRange930;
    _barrierCodelets930* barrierCodelets930;
    _checkInCodelets931* checkInCodelets931;
    TP930(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet,
        int in_initIteration, int in_lastIteration, int* in_domain_localRowMax,
        int* in_domain_localRowMin, struct SparseMatrixSt** in_x2matrix,
        struct SparseMatrixSt** in_xmatrix);
    ~TP930();
};
/*TP996: OMPParallelForDirective*/
class TP996 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets996 : public darts::Codelet {
    public:
        TP996* inputsTPParent;
        _barrierCodelets996()
            : darts::Codelet()
        {
        }
        _barrierCodelets996(uint32_t dep, uint32_t res, TP996* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations996(int* endRange, uint32_t codeletID);
    class _checkInCodelets997 : public darts::Codelet {
    public:
        TP996* myTP;
        TP996* inputsTPParent;
        int endRange;
        _checkInCodelets997()
            : darts::Codelet()
        {
        }
        _checkInCodelets997(uint32_t dep, uint32_t res, TP996* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP996* TPParent;
    TP996* controlTPParent;
    TP996* inputsTPParent;
    int* domain_localRowMax_darts996; /*OMP_SHARED - INPUT*/
    int* domain_localRowMin_darts996; /*OMP_SHARED - INPUT*/
    real_t* scalar_darts996; /*OMP_SHARED - INPUT*/
    struct SparseMatrixSt** xmatrix_darts996; /*OMP_SHARED - INPUT*/
    int* i_darts996 /*VARIABLE*/;
    int initIteration996;
    int lastIteration996;
    int range996;
    int rangePerCodelet996;
    int minIteration996;
    int remainderRange996;
    _barrierCodelets996* barrierCodelets996;
    _checkInCodelets997* checkInCodelets997;
    TP996(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet,
        int in_initIteration, int in_lastIteration, int* in_domain_localRowMax,
        int* in_domain_localRowMin, real_t* in_scalar, struct SparseMatrixSt** in_xmatrix);
    ~TP996();
};
#endif
