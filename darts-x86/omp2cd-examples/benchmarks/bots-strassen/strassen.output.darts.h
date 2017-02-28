#ifndef _strassen_output_darts_h_
#define _strassen_output_darts_h_
#ifndef __DARTS_
#define __DARTS_
#endif
#include "app-desc.h"
#include "bots.h"
#include "darts.h"
#include "ompTP.h"
#include "strassen.h"
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
void strassen_main_seq(REAL* A, REAL* B, REAL* C, int n);
void matrixmul(int n, REAL* A, int an, REAL* B, int bn, REAL* C, int cn);
void OptimizedStrassenMultiply_seq(REAL* C, REAL* A, REAL* B, unsigned int MatrixSize,
    unsigned int RowWidthC, unsigned int RowWidthA, unsigned int RowWidthB, int Depth);
void FastNaiveMatrixMultiply(REAL* C, REAL* A, REAL* B, unsigned int MatrixSize,
    unsigned int RowWidthC, unsigned int RowWidthA, unsigned int RowWidthB);
void FastAdditiveNaiveMatrixMultiply(REAL* C, REAL* A, REAL* B, unsigned int MatrixSize,
    unsigned int RowWidthC, unsigned int RowWidthA, unsigned int RowWidthB);
void MultiplyByDivideAndConquer(REAL* C, REAL* A, REAL* B, unsigned int MatrixSize,
    unsigned int RowWidthC, unsigned int RowWidthA, unsigned int RowWidthB, int AdditiveMode);
int compare_matrix(int n, REAL* A, int an, REAL* B, int bn);
void init_matrix(int n, REAL* A, int an);
REAL* alloc_matrix(int n);
void strassen_main_par(REAL* A, REAL* B, REAL* C, int n);
class TP20;
typedef TP20 TP_OptimizedStrassenMultiply_par;
/*Class containing the inputs passed to task998*/
class _task998Inputs {
public:
    REAL* A_darts998 /*OMP_FIRSTPRIVATE*/;
    REAL* B_darts998 /*OMP_FIRSTPRIVATE*/;
    int Depth_darts998 /*OMP_FIRSTPRIVATE*/;
    REAL* M2_darts998 /*OMP_FIRSTPRIVATE*/;
    unsigned int QuadrantSize_darts998 /*OMP_FIRSTPRIVATE*/;
    unsigned int RowWidthA_darts998 /*OMP_FIRSTPRIVATE*/;
    unsigned int RowWidthB_darts998 /*OMP_FIRSTPRIVATE*/;
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task998Inputs() {}
    _task998Inputs(REAL** in_A, REAL** in_B, int* in_Depth, REAL** in_M2,
        unsigned int* in_QuadrantSize, unsigned int* in_RowWidthA, unsigned int* in_RowWidthB,
        darts::Codelet* in_nextSyncCodelet)
        : taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
        this->A_darts998 = *in_A;
        this->B_darts998 = *in_B;
        this->Depth_darts998 = *in_Depth;
        this->M2_darts998 = *in_M2;
        this->QuadrantSize_darts998 = *in_QuadrantSize;
        this->RowWidthA_darts998 = *in_RowWidthA;
        this->RowWidthB_darts998 = *in_RowWidthB;
    }
    ~_task998Inputs() {}
};
class TP998;
/*Class containing the inputs passed to task1001*/
class _task1001Inputs {
public:
    int Depth_darts1001 /*OMP_FIRSTPRIVATE*/;
    REAL* M5_darts1001 /*OMP_FIRSTPRIVATE*/;
    unsigned int QuadrantSize_darts1001 /*OMP_FIRSTPRIVATE*/;
    REAL* S1_darts1001 /*OMP_FIRSTPRIVATE*/;
    REAL* S5_darts1001 /*OMP_FIRSTPRIVATE*/;
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task1001Inputs() {}
    _task1001Inputs(int* in_Depth, REAL** in_M5, unsigned int* in_QuadrantSize, REAL** in_S1,
        REAL** in_S5, darts::Codelet* in_nextSyncCodelet)
        : taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
        this->Depth_darts1001 = *in_Depth;
        this->M5_darts1001 = *in_M5;
        this->QuadrantSize_darts1001 = *in_QuadrantSize;
        this->S1_darts1001 = *in_S1;
        this->S5_darts1001 = *in_S5;
    }
    ~_task1001Inputs() {}
};
class TP1001;
/*Class containing the inputs passed to task1004*/
class _task1004Inputs {
public:
    int Depth_darts1004 /*OMP_FIRSTPRIVATE*/;
    unsigned int QuadrantSize_darts1004 /*OMP_FIRSTPRIVATE*/;
    REAL* S2_darts1004 /*OMP_FIRSTPRIVATE*/;
    REAL* S6_darts1004 /*OMP_FIRSTPRIVATE*/;
    REAL* T1sMULT_darts1004 /*OMP_FIRSTPRIVATE*/;
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task1004Inputs() {}
    _task1004Inputs(int* in_Depth, unsigned int* in_QuadrantSize, REAL** in_S2, REAL** in_S6,
        REAL** in_T1sMULT, darts::Codelet* in_nextSyncCodelet)
        : taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
        this->Depth_darts1004 = *in_Depth;
        this->QuadrantSize_darts1004 = *in_QuadrantSize;
        this->S2_darts1004 = *in_S2;
        this->S6_darts1004 = *in_S6;
        this->T1sMULT_darts1004 = *in_T1sMULT;
    }
    ~_task1004Inputs() {}
};
class TP1004;
/*Class containing the inputs passed to task1007*/
class _task1007Inputs {
public:
    REAL* C22_darts1007 /*OMP_FIRSTPRIVATE*/;
    int Depth_darts1007 /*OMP_FIRSTPRIVATE*/;
    unsigned int QuadrantSize_darts1007 /*OMP_FIRSTPRIVATE*/;
    unsigned int RowWidthC_darts1007 /*OMP_FIRSTPRIVATE*/;
    REAL* S3_darts1007 /*OMP_FIRSTPRIVATE*/;
    REAL* S7_darts1007 /*OMP_FIRSTPRIVATE*/;
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task1007Inputs() {}
    _task1007Inputs(REAL** in_C22, int* in_Depth, unsigned int* in_QuadrantSize,
        unsigned int* in_RowWidthC, REAL** in_S3, REAL** in_S7, darts::Codelet* in_nextSyncCodelet)
        : taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
        this->C22_darts1007 = *in_C22;
        this->Depth_darts1007 = *in_Depth;
        this->QuadrantSize_darts1007 = *in_QuadrantSize;
        this->RowWidthC_darts1007 = *in_RowWidthC;
        this->S3_darts1007 = *in_S3;
        this->S7_darts1007 = *in_S7;
    }
    ~_task1007Inputs() {}
};
class TP1007;
/*Class containing the inputs passed to task1010*/
class _task1010Inputs {
public:
    REAL* A12_darts1010 /*OMP_FIRSTPRIVATE*/;
    REAL* B21_darts1010 /*OMP_FIRSTPRIVATE*/;
    REAL* C_darts1010 /*OMP_FIRSTPRIVATE*/;
    int Depth_darts1010 /*OMP_FIRSTPRIVATE*/;
    unsigned int QuadrantSize_darts1010 /*OMP_FIRSTPRIVATE*/;
    unsigned int RowWidthA_darts1010 /*OMP_FIRSTPRIVATE*/;
    unsigned int RowWidthB_darts1010 /*OMP_FIRSTPRIVATE*/;
    unsigned int RowWidthC_darts1010 /*OMP_FIRSTPRIVATE*/;
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task1010Inputs() {}
    _task1010Inputs(REAL** in_A12, REAL** in_B21, REAL** in_C, int* in_Depth,
        unsigned int* in_QuadrantSize, unsigned int* in_RowWidthA, unsigned int* in_RowWidthB,
        unsigned int* in_RowWidthC, darts::Codelet* in_nextSyncCodelet)
        : taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
        this->A12_darts1010 = *in_A12;
        this->B21_darts1010 = *in_B21;
        this->C_darts1010 = *in_C;
        this->Depth_darts1010 = *in_Depth;
        this->QuadrantSize_darts1010 = *in_QuadrantSize;
        this->RowWidthA_darts1010 = *in_RowWidthA;
        this->RowWidthB_darts1010 = *in_RowWidthB;
        this->RowWidthC_darts1010 = *in_RowWidthC;
    }
    ~_task1010Inputs() {}
};
class TP1010;
/*Class containing the inputs passed to task1013*/
class _task1013Inputs {
public:
    REAL* B22_darts1013 /*OMP_FIRSTPRIVATE*/;
    REAL* C12_darts1013 /*OMP_FIRSTPRIVATE*/;
    int Depth_darts1013 /*OMP_FIRSTPRIVATE*/;
    unsigned int QuadrantSize_darts1013 /*OMP_FIRSTPRIVATE*/;
    unsigned int RowWidthB_darts1013 /*OMP_FIRSTPRIVATE*/;
    unsigned int RowWidthC_darts1013 /*OMP_FIRSTPRIVATE*/;
    REAL* S4_darts1013 /*OMP_FIRSTPRIVATE*/;
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task1013Inputs() {}
    _task1013Inputs(REAL** in_B22, REAL** in_C12, int* in_Depth, unsigned int* in_QuadrantSize,
        unsigned int* in_RowWidthB, unsigned int* in_RowWidthC, REAL** in_S4,
        darts::Codelet* in_nextSyncCodelet)
        : taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
        this->B22_darts1013 = *in_B22;
        this->C12_darts1013 = *in_C12;
        this->Depth_darts1013 = *in_Depth;
        this->QuadrantSize_darts1013 = *in_QuadrantSize;
        this->RowWidthB_darts1013 = *in_RowWidthB;
        this->RowWidthC_darts1013 = *in_RowWidthC;
        this->S4_darts1013 = *in_S4;
    }
    ~_task1013Inputs() {}
};
class TP1013;
/*Class containing the inputs passed to task1016*/
class _task1016Inputs {
public:
    REAL* A22_darts1016 /*OMP_FIRSTPRIVATE*/;
    REAL* C21_darts1016 /*OMP_FIRSTPRIVATE*/;
    int Depth_darts1016 /*OMP_FIRSTPRIVATE*/;
    unsigned int QuadrantSize_darts1016 /*OMP_FIRSTPRIVATE*/;
    unsigned int RowWidthA_darts1016 /*OMP_FIRSTPRIVATE*/;
    unsigned int RowWidthC_darts1016 /*OMP_FIRSTPRIVATE*/;
    REAL* S8_darts1016 /*OMP_FIRSTPRIVATE*/;
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task1016Inputs() {}
    _task1016Inputs(REAL** in_A22, REAL** in_C21, int* in_Depth, unsigned int* in_QuadrantSize,
        unsigned int* in_RowWidthA, unsigned int* in_RowWidthC, REAL** in_S8,
        darts::Codelet* in_nextSyncCodelet)
        : taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
        this->A22_darts1016 = *in_A22;
        this->C21_darts1016 = *in_C21;
        this->Depth_darts1016 = *in_Depth;
        this->QuadrantSize_darts1016 = *in_QuadrantSize;
        this->RowWidthA_darts1016 = *in_RowWidthA;
        this->RowWidthC_darts1016 = *in_RowWidthC;
        this->S8_darts1016 = *in_S8;
    }
    ~_task1016Inputs() {}
};
class TP1016;
class TP1244;
class TP1245;
/*Class containing the inputs passed to task1246*/
class _task1246Inputs {
public:
    REAL** A_darts1246; /*OMP_SHARED*/
    REAL** B_darts1246; /*OMP_SHARED*/
    REAL** C_darts1246; /*OMP_SHARED*/
    int* n_darts1246; /*OMP_SHARED*/
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task1246Inputs() {}
    _task1246Inputs(
        REAL** in_A, REAL** in_B, REAL** in_C, int* in_n, darts::Codelet* in_nextSyncCodelet)
        : A_darts1246(in_A)
        , B_darts1246(in_B)
        , C_darts1246(in_C)
        , n_darts1246(in_n)
        , taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
    }
    ~_task1246Inputs() {}
};
class TP1246;
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
/*TP20: OptimizedStrassenMultiply_par*/
class TP20 : public ompTP {
public:
    class _checkInCodelets785 : public darts::Codelet {
    public:
        TP20* myTP;
        TP20* inputsTPParent;
        _checkInCodelets785()
            : darts::Codelet()
        {
        }
        _checkInCodelets785(uint32_t dep, uint32_t res, TP20* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets998 : public darts::Codelet {
    public:
        TP20* myTP;
        TP20* inputsTPParent;
        _checkInCodelets998()
            : darts::Codelet()
        {
        }
        _checkInCodelets998(uint32_t dep, uint32_t res, TP20* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets1001 : public darts::Codelet {
    public:
        TP20* myTP;
        TP20* inputsTPParent;
        _checkInCodelets1001()
            : darts::Codelet()
        {
        }
        _checkInCodelets1001(uint32_t dep, uint32_t res, TP20* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets1004 : public darts::Codelet {
    public:
        TP20* myTP;
        TP20* inputsTPParent;
        _checkInCodelets1004()
            : darts::Codelet()
        {
        }
        _checkInCodelets1004(uint32_t dep, uint32_t res, TP20* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets1007 : public darts::Codelet {
    public:
        TP20* myTP;
        TP20* inputsTPParent;
        _checkInCodelets1007()
            : darts::Codelet()
        {
        }
        _checkInCodelets1007(uint32_t dep, uint32_t res, TP20* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets1010 : public darts::Codelet {
    public:
        TP20* myTP;
        TP20* inputsTPParent;
        _checkInCodelets1010()
            : darts::Codelet()
        {
        }
        _checkInCodelets1010(uint32_t dep, uint32_t res, TP20* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets1013 : public darts::Codelet {
    public:
        TP20* myTP;
        TP20* inputsTPParent;
        _checkInCodelets1013()
            : darts::Codelet()
        {
        }
        _checkInCodelets1013(uint32_t dep, uint32_t res, TP20* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets1016 : public darts::Codelet {
    public:
        TP20* myTP;
        TP20* inputsTPParent;
        _checkInCodelets1016()
            : darts::Codelet()
        {
        }
        _checkInCodelets1016(uint32_t dep, uint32_t res, TP20* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets1019 : public darts::Codelet {
    public:
        TP20* inputsTPParent;
        _barrierCodelets1019()
            : darts::Codelet()
        {
        }
        _barrierCodelets1019(uint32_t dep, uint32_t res, TP20* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets1020 : public darts::Codelet {
    public:
        TP20* myTP;
        TP20* inputsTPParent;
        _checkInCodelets1020()
            : darts::Codelet()
        {
        }
        _checkInCodelets1020(uint32_t dep, uint32_t res, TP20* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP20** ptrToThisFunctionTP;
    TP20* inputsTPParent;
    TP20* controlTPParent;
    darts::Codelet** nextCodeletsOptimizedStrassenMultiply_par;
    darts::Codelet** nextSyncCodeletsOptimizedStrassenMultiply_par;
    REAL** C_darts20 /*VARIABLE*/;
    REAL** A_darts20 /*VARIABLE*/;
    REAL** B_darts20 /*VARIABLE*/;
    unsigned int* MatrixSize_darts20 /*VARIABLE*/;
    unsigned int* RowWidthC_darts20 /*VARIABLE*/;
    unsigned int* RowWidthA_darts20 /*VARIABLE*/;
    unsigned int* RowWidthB_darts20 /*VARIABLE*/;
    int* Depth_darts20 /*VARIABLE*/;
    REAL** A12_darts20 /*VARIABLE*/;
    REAL** A21_darts20 /*VARIABLE*/;
    REAL** A22_darts20 /*VARIABLE*/;
    REAL** B12_darts20 /*VARIABLE*/;
    REAL** B21_darts20 /*VARIABLE*/;
    REAL** B22_darts20 /*VARIABLE*/;
    REAL** C12_darts20 /*VARIABLE*/;
    REAL** C21_darts20 /*VARIABLE*/;
    REAL** C22_darts20 /*VARIABLE*/;
    unsigned int* Column_darts20 /*VARIABLE*/;
    char** Heap_darts20 /*VARIABLE*/;
    REAL* LocalM2_0_darts20 /*VARIABLE*/;
    REAL* LocalM2_1_darts20 /*VARIABLE*/;
    REAL* LocalM2_2_darts20 /*VARIABLE*/;
    REAL* LocalM2_3_darts20 /*VARIABLE*/;
    REAL* LocalM5_0_darts20 /*VARIABLE*/;
    REAL* LocalM5_1_darts20 /*VARIABLE*/;
    REAL* LocalM5_2_darts20 /*VARIABLE*/;
    REAL* LocalM5_3_darts20 /*VARIABLE*/;
    REAL** M2_darts20 /*VARIABLE*/;
    REAL** M5_darts20 /*VARIABLE*/;
    PTR* MatrixOffsetA_darts20 /*VARIABLE*/;
    PTR* MatrixOffsetB_darts20 /*VARIABLE*/;
    unsigned int* QuadrantSize_darts20 /*VARIABLE*/;
    unsigned int* QuadrantSizeInBytes_darts20 /*VARIABLE*/;
    unsigned int* Row_darts20 /*VARIABLE*/;
    PTR* RowIncrementA_darts20 /*VARIABLE*/;
    PTR* RowIncrementB_darts20 /*VARIABLE*/;
    PTR* RowIncrementC_darts20 /*VARIABLE*/;
    REAL** S1_darts20 /*VARIABLE*/;
    REAL** S2_darts20 /*VARIABLE*/;
    REAL** S3_darts20 /*VARIABLE*/;
    REAL** S4_darts20 /*VARIABLE*/;
    REAL** S5_darts20 /*VARIABLE*/;
    REAL** S6_darts20 /*VARIABLE*/;
    REAL** S7_darts20 /*VARIABLE*/;
    REAL** S8_darts20 /*VARIABLE*/;
    void** StartHeap_darts20 /*VARIABLE*/;
    REAL* T1_0_darts20 /*VARIABLE*/;
    REAL* T1_1_darts20 /*VARIABLE*/;
    REAL* T1_2_darts20 /*VARIABLE*/;
    REAL* T1_3_darts20 /*VARIABLE*/;
    REAL** T1sMULT_darts20 /*VARIABLE*/;
    REAL* T2_0_darts20 /*VARIABLE*/;
    REAL* T2_1_darts20 /*VARIABLE*/;
    REAL* T2_2_darts20 /*VARIABLE*/;
    REAL* T2_3_darts20 /*VARIABLE*/;
    PTR* TempMatrixOffset_darts20 /*VARIABLE*/;
    _task998Inputs** task998Inputs;
    _task1001Inputs** task1001Inputs;
    _task1004Inputs** task1004Inputs;
    _task1007Inputs** task1007Inputs;
    _task1010Inputs** task1010Inputs;
    _task1013Inputs** task1013Inputs;
    _task1016Inputs** task1016Inputs;
    _checkInCodelets785* checkInCodelets785;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets785* firstCodelet;
#endif
    _checkInCodelets998* checkInCodelets998;
    _checkInCodelets1001* checkInCodelets1001;
    _checkInCodelets1004* checkInCodelets1004;
    _checkInCodelets1007* checkInCodelets1007;
    _checkInCodelets1010* checkInCodelets1010;
    _checkInCodelets1013* checkInCodelets1013;
    _checkInCodelets1016* checkInCodelets1016;
    _barrierCodelets1019* barrierCodelets1019;
    _checkInCodelets1020* checkInCodelets1020;
    TP20(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_mainNextCodelet,
        darts::Codelet* in_mainSyncCodelet, TP20** in_ptrToThisFunctionTP, REAL* in_C, REAL* in_A,
        REAL* in_B, unsigned int in_MatrixSize, unsigned int in_RowWidthC,
        unsigned int in_RowWidthA, unsigned int in_RowWidthB, int in_Depth);
    ~TP20();
    void setNewInputs(REAL* in_C, REAL* in_A, REAL* in_B, unsigned int in_MatrixSize,
        unsigned int in_RowWidthC, unsigned int in_RowWidthA, unsigned int in_RowWidthB,
        int in_Depth, size_t codeletID);
};
/*TP998: OMPTaskDirective*/
class TP998 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets999 : public darts::Codelet {
    public:
        TP998* myTP;
        TP998* inputsTPParent;
        _task998Inputs* taskInputs;
        _checkInCodelets999()
            : darts::Codelet()
        {
        }
        _checkInCodelets999(uint32_t dep, uint32_t res, TP998* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task998Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets1265 : public darts::Codelet {
    public:
        TP998* myTP;
        TP998* inputsTPParent;
        _task998Inputs* taskInputs;
        _checkInCodelets1265()
            : darts::Codelet()
        {
        }
        _checkInCodelets1265(uint32_t dep, uint32_t res, TP998* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task998Inputs)
        {
        }
        void fire(void);
    };
    TP20* TPParent;
    TP998* controlTPParent;
    TP998* inputsTPParent;
    _task998Inputs* task998Inputs;
    TP_OptimizedStrassenMultiply_par* TP999Ptr;
    int TP999_alreadyLaunched;
    _checkInCodelets999 checkInCodelets999;
    _checkInCodelets1265 checkInCodelets1265;
    TP998(int in_numThreads, int in_mainCodeletID, TP20* in_TPParent,
        _task998Inputs* in_task998Inputs);
    ~TP998();
};
/*TP1001: OMPTaskDirective*/
class TP1001 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets1002 : public darts::Codelet {
    public:
        TP1001* myTP;
        TP1001* inputsTPParent;
        _task1001Inputs* taskInputs;
        _checkInCodelets1002()
            : darts::Codelet()
        {
        }
        _checkInCodelets1002(uint32_t dep, uint32_t res, TP1001* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task1001Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets1266 : public darts::Codelet {
    public:
        TP1001* myTP;
        TP1001* inputsTPParent;
        _task1001Inputs* taskInputs;
        _checkInCodelets1266()
            : darts::Codelet()
        {
        }
        _checkInCodelets1266(uint32_t dep, uint32_t res, TP1001* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task1001Inputs)
        {
        }
        void fire(void);
    };
    TP20* TPParent;
    TP1001* controlTPParent;
    TP1001* inputsTPParent;
    _task1001Inputs* task1001Inputs;
    TP_OptimizedStrassenMultiply_par* TP1002Ptr;
    int TP1002_alreadyLaunched;
    _checkInCodelets1002 checkInCodelets1002;
    _checkInCodelets1266 checkInCodelets1266;
    TP1001(int in_numThreads, int in_mainCodeletID, TP20* in_TPParent,
        _task1001Inputs* in_task1001Inputs);
    ~TP1001();
};
/*TP1004: OMPTaskDirective*/
class TP1004 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets1005 : public darts::Codelet {
    public:
        TP1004* myTP;
        TP1004* inputsTPParent;
        _task1004Inputs* taskInputs;
        _checkInCodelets1005()
            : darts::Codelet()
        {
        }
        _checkInCodelets1005(uint32_t dep, uint32_t res, TP1004* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task1004Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets1267 : public darts::Codelet {
    public:
        TP1004* myTP;
        TP1004* inputsTPParent;
        _task1004Inputs* taskInputs;
        _checkInCodelets1267()
            : darts::Codelet()
        {
        }
        _checkInCodelets1267(uint32_t dep, uint32_t res, TP1004* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task1004Inputs)
        {
        }
        void fire(void);
    };
    TP20* TPParent;
    TP1004* controlTPParent;
    TP1004* inputsTPParent;
    _task1004Inputs* task1004Inputs;
    TP_OptimizedStrassenMultiply_par* TP1005Ptr;
    int TP1005_alreadyLaunched;
    _checkInCodelets1005 checkInCodelets1005;
    _checkInCodelets1267 checkInCodelets1267;
    TP1004(int in_numThreads, int in_mainCodeletID, TP20* in_TPParent,
        _task1004Inputs* in_task1004Inputs);
    ~TP1004();
};
/*TP1007: OMPTaskDirective*/
class TP1007 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets1008 : public darts::Codelet {
    public:
        TP1007* myTP;
        TP1007* inputsTPParent;
        _task1007Inputs* taskInputs;
        _checkInCodelets1008()
            : darts::Codelet()
        {
        }
        _checkInCodelets1008(uint32_t dep, uint32_t res, TP1007* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task1007Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets1268 : public darts::Codelet {
    public:
        TP1007* myTP;
        TP1007* inputsTPParent;
        _task1007Inputs* taskInputs;
        _checkInCodelets1268()
            : darts::Codelet()
        {
        }
        _checkInCodelets1268(uint32_t dep, uint32_t res, TP1007* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task1007Inputs)
        {
        }
        void fire(void);
    };
    TP20* TPParent;
    TP1007* controlTPParent;
    TP1007* inputsTPParent;
    _task1007Inputs* task1007Inputs;
    TP_OptimizedStrassenMultiply_par* TP1008Ptr;
    int TP1008_alreadyLaunched;
    _checkInCodelets1008 checkInCodelets1008;
    _checkInCodelets1268 checkInCodelets1268;
    TP1007(int in_numThreads, int in_mainCodeletID, TP20* in_TPParent,
        _task1007Inputs* in_task1007Inputs);
    ~TP1007();
};
/*TP1010: OMPTaskDirective*/
class TP1010 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets1011 : public darts::Codelet {
    public:
        TP1010* myTP;
        TP1010* inputsTPParent;
        _task1010Inputs* taskInputs;
        _checkInCodelets1011()
            : darts::Codelet()
        {
        }
        _checkInCodelets1011(uint32_t dep, uint32_t res, TP1010* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task1010Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets1269 : public darts::Codelet {
    public:
        TP1010* myTP;
        TP1010* inputsTPParent;
        _task1010Inputs* taskInputs;
        _checkInCodelets1269()
            : darts::Codelet()
        {
        }
        _checkInCodelets1269(uint32_t dep, uint32_t res, TP1010* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task1010Inputs)
        {
        }
        void fire(void);
    };
    TP20* TPParent;
    TP1010* controlTPParent;
    TP1010* inputsTPParent;
    _task1010Inputs* task1010Inputs;
    TP_OptimizedStrassenMultiply_par* TP1011Ptr;
    int TP1011_alreadyLaunched;
    _checkInCodelets1011 checkInCodelets1011;
    _checkInCodelets1269 checkInCodelets1269;
    TP1010(int in_numThreads, int in_mainCodeletID, TP20* in_TPParent,
        _task1010Inputs* in_task1010Inputs);
    ~TP1010();
};
/*TP1013: OMPTaskDirective*/
class TP1013 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets1014 : public darts::Codelet {
    public:
        TP1013* myTP;
        TP1013* inputsTPParent;
        _task1013Inputs* taskInputs;
        _checkInCodelets1014()
            : darts::Codelet()
        {
        }
        _checkInCodelets1014(uint32_t dep, uint32_t res, TP1013* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task1013Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets1270 : public darts::Codelet {
    public:
        TP1013* myTP;
        TP1013* inputsTPParent;
        _task1013Inputs* taskInputs;
        _checkInCodelets1270()
            : darts::Codelet()
        {
        }
        _checkInCodelets1270(uint32_t dep, uint32_t res, TP1013* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task1013Inputs)
        {
        }
        void fire(void);
    };
    TP20* TPParent;
    TP1013* controlTPParent;
    TP1013* inputsTPParent;
    _task1013Inputs* task1013Inputs;
    TP_OptimizedStrassenMultiply_par* TP1014Ptr;
    int TP1014_alreadyLaunched;
    _checkInCodelets1014 checkInCodelets1014;
    _checkInCodelets1270 checkInCodelets1270;
    TP1013(int in_numThreads, int in_mainCodeletID, TP20* in_TPParent,
        _task1013Inputs* in_task1013Inputs);
    ~TP1013();
};
/*TP1016: OMPTaskDirective*/
class TP1016 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets1017 : public darts::Codelet {
    public:
        TP1016* myTP;
        TP1016* inputsTPParent;
        _task1016Inputs* taskInputs;
        _checkInCodelets1017()
            : darts::Codelet()
        {
        }
        _checkInCodelets1017(uint32_t dep, uint32_t res, TP1016* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task1016Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets1271 : public darts::Codelet {
    public:
        TP1016* myTP;
        TP1016* inputsTPParent;
        _task1016Inputs* taskInputs;
        _checkInCodelets1271()
            : darts::Codelet()
        {
        }
        _checkInCodelets1271(uint32_t dep, uint32_t res, TP1016* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task1016Inputs)
        {
        }
        void fire(void);
    };
    TP20* TPParent;
    TP1016* controlTPParent;
    TP1016* inputsTPParent;
    _task1016Inputs* task1016Inputs;
    TP_OptimizedStrassenMultiply_par* TP1017Ptr;
    int TP1017_alreadyLaunched;
    _checkInCodelets1017 checkInCodelets1017;
    _checkInCodelets1271 checkInCodelets1271;
    TP1016(int in_numThreads, int in_mainCodeletID, TP20* in_TPParent,
        _task1016Inputs* in_task1016Inputs);
    ~TP1016();
};
/*TP1244: OMPParallelDirective*/
class TP1244 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets1244 : public darts::Codelet {
    public:
        TP1244* inputsTPParent;
        _barrierCodelets1244()
            : darts::Codelet()
        {
        }
        _barrierCodelets1244(uint32_t dep, uint32_t res, TP1244* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets1245 : public darts::Codelet {
    public:
        TP1244* myTP;
        TP1244* inputsTPParent;
        _checkInCodelets1245()
            : darts::Codelet()
        {
        }
        _checkInCodelets1245(uint32_t dep, uint32_t res, TP1244* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets1245 : public darts::Codelet {
    public:
        TP1244* inputsTPParent;
        _barrierCodelets1245()
            : darts::Codelet()
        {
        }
        _barrierCodelets1245(uint32_t dep, uint32_t res, TP1244* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP1244* TPParent;
    TP1244* controlTPParent;
    TP1244* inputsTPParent;
    REAL** A_darts1244; /*OMP_SHARED - INPUT*/
    REAL** B_darts1244; /*OMP_SHARED - INPUT*/
    REAL** C_darts1244; /*OMP_SHARED - INPUT*/
    int* n_darts1244; /*OMP_SHARED - INPUT*/
    TP1245* TP1245Ptr;
    size_t TP1245_alreadyLaunched;
    _task1246Inputs** task1246Inputs;
    _barrierCodelets1244* barrierCodelets1244;
    _checkInCodelets1245* checkInCodelets1245;
    _barrierCodelets1245* barrierCodelets1245;
    TP1244(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet, REAL** in_A,
        REAL** in_B, REAL** in_C, int* in_n);
    ~TP1244();
};
/*TP1245: OMPSingleDirective*/
class TP1245 : public ompOMPSingleDirectiveTP {
public:
    class _checkInCodelets1246 : public darts::Codelet {
    public:
        TP1245* myTP;
        TP1245* inputsTPParent;
        _checkInCodelets1246()
            : darts::Codelet()
        {
        }
        _checkInCodelets1246(uint32_t dep, uint32_t res, TP1245* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP1244* TPParent;
    TP1245* controlTPParent;
    TP1245* inputsTPParent;
    REAL** A_darts1245; /*OMP_SHARED - INPUT*/
    REAL** B_darts1245; /*OMP_SHARED - INPUT*/
    REAL** C_darts1245; /*OMP_SHARED - INPUT*/
    int* n_darts1245; /*OMP_SHARED - INPUT*/
    _task1246Inputs** task1246Inputs;
    _checkInCodelets1246 checkInCodelets1246;
    TP1245(int in_numThreads, int in_mainCodeletID, TP1244* in_TPParent, REAL** in_A, REAL** in_B,
        REAL** in_C, int* in_n);
    ~TP1245();
};
/*TP1246: OMPTaskDirective*/
class TP1246 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets1247 : public darts::Codelet {
    public:
        TP1246* myTP;
        TP1246* inputsTPParent;
        _task1246Inputs* taskInputs;
        _checkInCodelets1247()
            : darts::Codelet()
        {
        }
        _checkInCodelets1247(uint32_t dep, uint32_t res, TP1246* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task1246Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets1272 : public darts::Codelet {
    public:
        TP1246* myTP;
        TP1246* inputsTPParent;
        _task1246Inputs* taskInputs;
        _checkInCodelets1272()
            : darts::Codelet()
        {
        }
        _checkInCodelets1272(uint32_t dep, uint32_t res, TP1246* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task1246Inputs)
        {
        }
        void fire(void);
    };
    TP1245* TPParent;
    TP1246* controlTPParent;
    TP1246* inputsTPParent;
    _task1246Inputs* task1246Inputs;
    TP_OptimizedStrassenMultiply_par* TP1247Ptr;
    int TP1247_alreadyLaunched;
    _checkInCodelets1247 checkInCodelets1247;
    _checkInCodelets1272 checkInCodelets1272;
    TP1246(int in_numThreads, int in_mainCodeletID, TP1245* in_TPParent,
        _task1246Inputs* in_task1246Inputs);
    ~TP1246();
};
#endif
