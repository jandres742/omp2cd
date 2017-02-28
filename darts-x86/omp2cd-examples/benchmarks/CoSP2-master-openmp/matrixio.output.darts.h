#ifndef _matrixio_output_darts_h_
#define _matrixio_output_darts_h_
#ifndef __DARTS_
#define __DARTS_
#endif
#include "constants.h"
#include "darts.h"
#include "matrixio.h"
#include "ompTP.h"
#include "sparseMatrix.h"
#include "sparseMatrix.output.darts.h"
#include "tbb/concurrent_vector.h"
#include "utils.h"
#include <limits.h>
#include <math.h>
#include <mutex>
#include <numa.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
void writeMTX(char* fname, struct SparseMatrixSt* spmatrix);
void readMTX(char* fname, struct SparseMatrixSt* hmatrix);
void writeSparsePattern(char* fname, struct SparseMatrixSt* spmatrix, real_t hthresh);
class TP2516;
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
/*TP2516: OMPParallelForDirective*/
class TP2516 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets2516 : public darts::Codelet {
    public:
        TP2516* inputsTPParent;
        _barrierCodelets2516()
            : darts::Codelet()
        {
        }
        _barrierCodelets2516(uint32_t dep, uint32_t res, TP2516* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations2516(int* endRange, uint32_t codeletID);
    class _checkInCodelets2517 : public darts::Codelet {
    public:
        TP2516* myTP;
        TP2516* inputsTPParent;
        int endRange;
        _checkInCodelets2517()
            : darts::Codelet()
        {
        }
        _checkInCodelets2517(uint32_t dep, uint32_t res, TP2516* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP2516* TPParent;
    TP2516* controlTPParent;
    TP2516* inputsTPParent;
    char** hrow_darts2516; /*OMP_SHARED - INPUT*/
    real_t* hthresh_darts2516; /*OMP_SHARED - INPUT*/
    FILE** sFile_darts2516; /*OMP_SHARED - INPUT*/
    struct SparseMatrixSt** spmatrix_darts2516; /*OMP_SHARED - INPUT*/
    int* spmatrix_hsize_darts2516; /*OMP_SHARED - INPUT*/
    int* i_darts2516 /*VARIABLE*/;
    int initIteration2516;
    int lastIteration2516;
    int range2516;
    int rangePerCodelet2516;
    int minIteration2516;
    int remainderRange2516;
    _barrierCodelets2516* barrierCodelets2516;
    _checkInCodelets2517* checkInCodelets2517;
    TP2516(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet,
        int in_initIteration, int in_lastIteration, char** in_hrow, real_t* in_hthresh,
        FILE** in_sFile, struct SparseMatrixSt** in_spmatrix, int* in_spmatrix_hsize);
    ~TP2516();
};
#endif
