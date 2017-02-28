#ifndef _omp_csr_output_darts_h_
#define _omp_csr_output_darts_h_
#ifndef __DARTS_
#define __DARTS_
#endif
#include "../compat.h"
#include "../generator/graph_generator.h"
#include "../graph500.h"
#include "../xalloc.h"
#include "darts.h"
#include "ompTP.h"
#include "tbb/concurrent_vector.h"
#include "utils.h"
#include <alloca.h>
#include <cassert>
#include <limits.h>
#include <math.h>
#include <mutex>
#include <numa.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/time.h>
#include <time.h>
#include <time.h>
#include <unistd.h>
int make_bfs_tree(int64_t* bfs_tree_out, int64_t* max_vtx_out, int64_t srcvtx);
void destroy_graph();
int create_graph_from_edgelist(struct packed_edge* IJ, int64_t nedge);
class TP60;
class TP64;
/*Number of TPs and codelets to be used for the OMPFor in region TP64*/
#define NUMTPS64 NUMTPS
#define NUMCODELETS64 (ompNumThreads / NUMTPS)
extern int dartsTPLoopTH0;
extern int dartsTPLoopTH1;
class _checkInCodelets64_core;
class TP64_loop;
class TP64_outer;
class TP15;
typedef TP15 TP_prefix_sum;
class TP211;
class TP214;
/*Number of TPs to be used for the OMPFor in region TP214*/
#define NUMTPS214 NUMTPS
class TP247;
/*Number of TPs to be used for the OMPFor in region TP247*/
#define NUMTPS247 NUMTPS
class TP326;
/*Number of TPs to be used for the OMPFor in region TP326*/
#define NUMTPS326 NUMTPS
class TP363;
/*Number of TPs to be used for the OMPFor in region TP363*/
#define NUMTPS363 NUMTPS
class TP20;
typedef TP20 TP_pack_edges;
class TP510;
/*Number of TPs to be used for the OMPFor in region TP510*/
#define NUMTPS510 NUMTPS
class TP541;
class TP544;
/*Number of TPs to be used for the OMPFor in region TP544*/
#define NUMTPS544 NUMTPS
class TP644;
class TP648;
/*Number of TPs and codelets to be used for the OMPFor in region TP648*/
#define NUMTPS648 NUMTPS
#define NUMCODELETS648 (ompNumThreads / NUMTPS)
extern int dartsTPLoopTH0;
extern int dartsTPLoopTH1;
class _checkInCodelets648_core;
class TP648_loop;
class TP648_outer;
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
/*TP60: OMPParallelDirective*/
class TP60 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets60 : public darts::Codelet {
    public:
        TP60* inputsTPParent;
        _barrierCodelets60()
            : darts::Codelet()
        {
        }
        _barrierCodelets60(uint32_t dep, uint32_t res, TP60* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets62 : public darts::Codelet {
    public:
        TP60* myTP;
        TP60* inputsTPParent;
        _checkInCodelets62()
            : darts::Codelet()
        {
        }
        _checkInCodelets62(uint32_t dep, uint32_t res, TP60* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets64_core : public darts::Codelet {
    public:
        Codelet* toSignal;
        int64_t initIteration;
        int64_t lastIteration;
        struct packed_edge* __restrict* IJ;
        int64_t* nedge;
        _checkInCodelets64_core()
            : darts::Codelet()
        {
        }
        _checkInCodelets64_core(uint32_t dep, uint32_t res, TP60* myTP,
            darts::Codelet* in_nextCodelet, int64_t in_initIteration, int64_t in_lastIteration,
            struct packed_edge* __restrict* in_IJ, int64_t* in_nedge)
            : darts::Codelet(dep, res, myTP, LONGWAIT)
            , toSignal(in_nextCodelet)
            , initIteration(in_initIteration)
            , lastIteration(in_lastIteration)
            , IJ(in_IJ)
            , nedge(in_nedge)
        {
        }
        void fire(void);
    };
    class _checkInCodelets64 : public darts::Codelet {
    public:
        TP60* myTP;
        TP60* inputsTPParent;
        _checkInCodelets64()
            : darts::Codelet()
        {
        }
        _checkInCodelets64(uint32_t dep, uint32_t res, TP60* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets64 : public darts::Codelet {
    public:
        TP60* inputsTPParent;
        _barrierCodelets64()
            : darts::Codelet()
        {
        }
        _barrierCodelets64(uint32_t dep, uint32_t res, TP60* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets108 : public darts::Codelet {
    public:
        TP60* myTP;
        TP60* inputsTPParent;
        _checkInCodelets108()
            : darts::Codelet()
        {
        }
        _checkInCodelets108(uint32_t dep, uint32_t res, TP60* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP60* TPParent;
    TP60* controlTPParent;
    TP60* inputsTPParent;
    struct packed_edge* __restrict* IJ_darts60; /*OMP_SHARED - INPUT*/
    int64_t* nedge_darts60; /*OMP_SHARED - INPUT*/
    TP64** TP64Ptr;
    size_t* TP64_alreadyLaunched;
    int numTPsSet64;
    int numTPsReady64;
    size_t TPsToUse64;
    size_t codeletsPerTP64;
    size_t totalCodelets64;
    _barrierCodelets60* barrierCodelets60;
    _checkInCodelets62* checkInCodelets62;
    _checkInCodelets64* checkInCodelets64;
    _checkInCodelets64_core* masterCodeletTP64_core;
    darts::codeletFor<TP64_loop>* masterCodeletTP64_loop;
    darts::paraFor<TP64_outer>* masterCodeletTP64_outer;
    _barrierCodelets64* barrierCodelets64;
    _checkInCodelets108* checkInCodelets108;
    TP60(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet,
        struct packed_edge* __restrict* in_IJ, int64_t* in_nedge);
    ~TP60();
};
/*TP64: OMPForDirective*/
class TP64_loop : public darts::loop {
public:
    class _checkInCodelets65 : public darts::Codelet {
    public:
        TP64* myTP;
        struct packed_edge* __restrict* IJ;
        int64_t* nedge;
        _checkInCodelets65()
            : darts::Codelet()
        {
        }
        _checkInCodelets65(uint32_t dep, uint32_t res, ThreadedProcedure* myTP,
            struct packed_edge* __restrict* in_IJ, int64_t* in_nedge)
            : darts::Codelet(dep, res, myTP, LONGWAIT)
            , IJ(in_IJ)
            , nedge(in_nedge)
        {
        }
        void fire(void);
    };
    unsigned int tileSize;
    int64_t initIteration;
    int64_t lastIteration;
    _checkInCodelets65* checkInCodelets65;
    TP64_loop(unsigned int it, darts::Codelet* in_nextCodelet, unsigned int in_tileSize,
        int64_t in_initIteration, int64_t in_lastIteration, struct packed_edge* __restrict* in_IJ,
        int64_t* in_nedge);
    ~TP64_loop();
};
class TP64_outer : public darts::loop {
public:
    class TP64_inner : public darts::loop {
    public:
        class _checkInCodelets65 : public darts::Codelet {
        public:
            TP64* myTP;
            struct packed_edge* __restrict* IJ;
            int64_t* nedge;
            _checkInCodelets65()
                : darts::Codelet()
            {
            }
            _checkInCodelets65(uint32_t dep, uint32_t res, ThreadedProcedure* myTP,
                struct packed_edge* __restrict* in_IJ, int64_t* in_nedge)
                : darts::Codelet(dep, res, myTP, LONGWAIT)
                , IJ(in_IJ)
                , nedge(in_nedge)
            {
            }
            void fire(void);
        };
        unsigned int outerIteration;
        unsigned int innerIteration;
        int64_t outerTile;
        int64_t innerTile;
        int64_t initIteration;
        int64_t lastIteration;
        _checkInCodelets65* checkInCodelets65;
        TP64_inner(unsigned int it, darts::Codelet* in_nextCodelet, unsigned int in_outerIteration,
            int64_t in_outerTile, int64_t in_innerTile, int64_t in_initIteration,
            int64_t in_lastIteration, struct packed_edge* __restrict* in_IJ, int64_t* in_nedge);
        ~TP64_inner();
    };
    int64_t outerTile;
    unsigned int workers;
    int64_t initIteration;
    int64_t lastIteration;
    int64_t width;
    int64_t innerTile;
    darts::codeletFor<TP64_inner>* masterCodeletTP64_inner;
    TP64_outer(unsigned int it, darts::Codelet* in_nextCodelet, int64_t in_outerTile,
        unsigned int in_workers, int64_t in_initIteration, int64_t in_lastIteration,
        struct packed_edge* __restrict* in_IJ, int64_t* in_nedge);
    ~TP64_outer();
};
/*TP15: prefix_sum*/
class TP15 : public ompTP {
public:
    class _checkInCodelets136 : public darts::Codelet {
    public:
        TP15* myTP;
        TP15* inputsTPParent;
        _checkInCodelets136()
            : darts::Codelet()
        {
        }
        _checkInCodelets136(uint32_t dep, uint32_t res, TP15* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets169 : public darts::Codelet {
    public:
        TP15* inputsTPParent;
        _barrierCodelets169()
            : darts::Codelet()
        {
        }
        _barrierCodelets169(uint32_t dep, uint32_t res, TP15* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets170 : public darts::Codelet {
    public:
        TP15* myTP;
        TP15* inputsTPParent;
        _checkInCodelets170()
            : darts::Codelet()
        {
        }
        _checkInCodelets170(uint32_t dep, uint32_t res, TP15* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets170 : public darts::Codelet {
    public:
        TP15* inputsTPParent;
        _barrierCodelets170()
            : darts::Codelet()
        {
        }
        _barrierCodelets170(uint32_t dep, uint32_t res, TP15* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets179 : public darts::Codelet {
    public:
        TP15* myTP;
        TP15* inputsTPParent;
        _checkInCodelets179()
            : darts::Codelet()
        {
        }
        _checkInCodelets179(uint32_t dep, uint32_t res, TP15* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets199 : public darts::Codelet {
    public:
        TP15* inputsTPParent;
        _barrierCodelets199()
            : darts::Codelet()
        {
        }
        _barrierCodelets199(uint32_t dep, uint32_t res, TP15* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets200 : public darts::Codelet {
    public:
        TP15* myTP;
        TP15* inputsTPParent;
        _checkInCodelets200()
            : darts::Codelet()
        {
        }
        _checkInCodelets200(uint32_t dep, uint32_t res, TP15* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP15** ptrToThisFunctionTP;
    TP15* inputsTPParent;
    TP15* controlTPParent;
    darts::Codelet** nextCodeletsprefix_sum;
    darts::Codelet** nextSyncCodeletsprefix_sum;
    int64_t** buf_darts15 /*VARIABLE*/;
    int64_t* k_darts15 /*VARIABLE*/;
    int* nt_darts15 /*VARIABLE*/;
    int64_t* slice_begin_darts15 /*VARIABLE*/;
    int64_t* slice_end_darts15 /*VARIABLE*/;
    int64_t* t1_darts15 /*VARIABLE*/;
    int64_t* t2_darts15 /*VARIABLE*/;
    int* tid_darts15 /*VARIABLE*/;
    int64_t* tmp_darts15 /*VARIABLE*/;
    int64_t** functionResult;
    int64_t** buf_darts170 /*OMP_SHARED_PRIVATE - INPUT*/;
    int64_t k_darts170;
    int* nt_darts170 /*OMP_SHARED_PRIVATE - INPUT*/;
    size_t TP170_alreadyLaunched;
    _checkInCodelets136* checkInCodelets136;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets136* firstCodelet;
#endif
    _barrierCodelets169* barrierCodelets169;
    _checkInCodelets170* checkInCodelets170;
    _barrierCodelets170* barrierCodelets170;
    _checkInCodelets179* checkInCodelets179;
    _barrierCodelets199* barrierCodelets199;
    _checkInCodelets200* checkInCodelets200;
    TP15(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_mainNextCodelet,
        darts::Codelet* in_mainSyncCodelet, TP15** in_ptrToThisFunctionTP, int64_t* in_buf,
        int64_t* in_functionResult);
    ~TP15();
    void setNewInputs(int64_t* in_buf, int64_t* in_functionResult, size_t codeletID);
};
/*TP211: OMPParallelDirective*/
class TP211 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets211 : public darts::Codelet {
    public:
        TP211* inputsTPParent;
        _barrierCodelets211()
            : darts::Codelet()
        {
        }
        _barrierCodelets211(uint32_t dep, uint32_t res, TP211* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets214 : public darts::Codelet {
    public:
        TP211* myTP;
        TP211* inputsTPParent;
        _checkInCodelets214()
            : darts::Codelet()
        {
        }
        _checkInCodelets214(uint32_t dep, uint32_t res, TP211* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets214 : public darts::Codelet {
    public:
        TP211* inputsTPParent;
        _barrierCodelets214()
            : darts::Codelet()
        {
        }
        _barrierCodelets214(uint32_t dep, uint32_t res, TP211* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets247 : public darts::Codelet {
    public:
        TP211* myTP;
        TP211* inputsTPParent;
        _checkInCodelets247()
            : darts::Codelet()
        {
        }
        _checkInCodelets247(uint32_t dep, uint32_t res, TP211* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets247 : public darts::Codelet {
    public:
        TP211* inputsTPParent;
        _barrierCodelets247()
            : darts::Codelet()
        {
        }
        _barrierCodelets247(uint32_t dep, uint32_t res, TP211* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets312 : public darts::Codelet {
    public:
        TP211* myTP;
        TP211* inputsTPParent;
        _checkInCodelets312()
            : darts::Codelet()
        {
        }
        _checkInCodelets312(uint32_t dep, uint32_t res, TP211* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets312 : public darts::Codelet {
    public:
        TP211* inputsTPParent;
        _barrierCodelets312()
            : darts::Codelet()
        {
        }
        _barrierCodelets312(uint32_t dep, uint32_t res, TP211* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets326 : public darts::Codelet {
    public:
        TP211* myTP;
        TP211* inputsTPParent;
        _checkInCodelets326()
            : darts::Codelet()
        {
        }
        _checkInCodelets326(uint32_t dep, uint32_t res, TP211* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets326 : public darts::Codelet {
    public:
        TP211* inputsTPParent;
        _barrierCodelets326()
            : darts::Codelet()
        {
        }
        _barrierCodelets326(uint32_t dep, uint32_t res, TP211* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets362 : public darts::Codelet {
    public:
        TP211* myTP;
        TP211* inputsTPParent;
        _checkInCodelets362()
            : darts::Codelet()
        {
        }
        _checkInCodelets362(uint32_t dep, uint32_t res, TP211* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets363 : public darts::Codelet {
    public:
        TP211* myTP;
        TP211* inputsTPParent;
        _checkInCodelets363()
            : darts::Codelet()
        {
        }
        _checkInCodelets363(uint32_t dep, uint32_t res, TP211* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets363 : public darts::Codelet {
    public:
        TP211* inputsTPParent;
        _barrierCodelets363()
            : darts::Codelet()
        {
        }
        _barrierCodelets363(uint32_t dep, uint32_t res, TP211* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets398 : public darts::Codelet {
    public:
        TP211* myTP;
        TP211* inputsTPParent;
        _checkInCodelets398()
            : darts::Codelet()
        {
        }
        _checkInCodelets398(uint32_t dep, uint32_t res, TP211* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets398 : public darts::Codelet {
    public:
        TP211* inputsTPParent;
        _barrierCodelets398()
            : darts::Codelet()
        {
        }
        _barrierCodelets398(uint32_t dep, uint32_t res, TP211* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP211* TPParent;
    TP211* controlTPParent;
    TP211* inputsTPParent;
    struct packed_edge* __restrict* IJ_darts211; /*OMP_SHARED - INPUT*/
    int64_t** buf_darts211; /*OMP_SHARED - INPUT*/
    int* err_darts211; /*OMP_SHARED - INPUT*/
    int64_t* nedge_darts211; /*OMP_SHARED - INPUT*/
    int64_t* accum_darts211 /*VARIABLE*/;
    int64_t* k_darts211 /*VARIABLE*/;
    int64_t* accum_darts398 /*OMP_SHARED_PRIVATE - INPUT*/;
    int* err_darts398; /*OMP_SHARED - INPUT*/
    int64_t k_darts398;
    int64_t** buf_darts312; /*OMP_SHARED - INPUT*/
    TP214** TP214Ptr;
    size_t* TP214_alreadyLaunched;
    int numTPsSet214;
    int numTPsReady214;
    size_t TPsToUse214;
    size_t codeletsPerTP214;
    size_t totalCodelets214;
    TP247** TP247Ptr;
    size_t* TP247_alreadyLaunched;
    int numTPsSet247;
    int numTPsReady247;
    size_t TPsToUse247;
    size_t codeletsPerTP247;
    size_t totalCodelets247;
    size_t TP312_alreadyLaunched;
    TP326** TP326Ptr;
    size_t* TP326_alreadyLaunched;
    int numTPsSet326;
    int numTPsReady326;
    size_t TPsToUse326;
    size_t codeletsPerTP326;
    size_t totalCodelets326;
    TP_prefix_sum* TP362Ptr;
    int TP362_alreadyLaunched;
    TP363** TP363Ptr;
    size_t* TP363_alreadyLaunched;
    int numTPsSet363;
    int numTPsReady363;
    size_t TPsToUse363;
    size_t codeletsPerTP363;
    size_t totalCodelets363;
    size_t TP398_alreadyLaunched;
    _barrierCodelets211* barrierCodelets211;
    _checkInCodelets214* checkInCodelets214;
    _barrierCodelets214* barrierCodelets214;
    _checkInCodelets247* checkInCodelets247;
    _barrierCodelets247* barrierCodelets247;
    _checkInCodelets312* checkInCodelets312;
    _barrierCodelets312* barrierCodelets312;
    _checkInCodelets326* checkInCodelets326;
    _barrierCodelets326* barrierCodelets326;
    _checkInCodelets362* checkInCodelets362;
    _checkInCodelets363* checkInCodelets363;
    _barrierCodelets363* barrierCodelets363;
    _checkInCodelets398* checkInCodelets398;
    _barrierCodelets398* barrierCodelets398;
    TP211(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet,
        struct packed_edge* __restrict* in_IJ, int64_t** in_buf, int* in_err, int64_t* in_nedge);
    ~TP211();
};
/*TP214: OMPForDirective*/
class TP214 : public ompTP {
public:
    class _barrierCodelets214 : public darts::Codelet {
    public:
        TP214* inputsTPParent;
        _barrierCodelets214()
            : darts::Codelet()
        {
        }
        _barrierCodelets214(uint32_t dep, uint32_t res, TP214* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations214(int64_t* endRange, uint32_t codeletID);
    class _checkInCodelets215 : public darts::Codelet {
    public:
        TP214* myTP;
        TP214* inputsTPParent;
        int64_t endRange;
        _checkInCodelets215()
            : darts::Codelet()
        {
        }
        _checkInCodelets215(uint32_t dep, uint32_t res, TP214* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP211* TPParent;
    TP214* controlTPParent;
    TP214* inputsTPParent;
    int64_t* k_darts214 /*OMP_PRIVATE - INPUT*/;
    int64_t initIteration214;
    int64_t lastIteration214;
    int64_t range214;
    int64_t rangePerCodelet214;
    int64_t minIteration214;
    int64_t remainderRange214;
    size_t readyCodelets;
    int baseNumThreads;
    _barrierCodelets214* barrierCodelets214;
    _checkInCodelets215* checkInCodelets215;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets215* firstCodelet;
#endif
    TP214(int in_numThreads, int in_mainCodeletID, TP211* in_TPParent, int64_t in_initIteration,
        int64_t in_lastIteration, TP214** in_ptrToThisTP);
    void inline dispatchCodelet(size_t codeletID);
    ~TP214();
};
/*TP247: OMPForDirective*/
class TP247 : public ompTP {
public:
    class _barrierCodelets247 : public darts::Codelet {
    public:
        TP247* inputsTPParent;
        _barrierCodelets247()
            : darts::Codelet()
        {
        }
        _barrierCodelets247(uint32_t dep, uint32_t res, TP247* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations247(int64_t* endRange, uint32_t codeletID);
    class _checkInCodelets248 : public darts::Codelet {
    public:
        TP247* myTP;
        TP247* inputsTPParent;
        int64_t endRange;
        _checkInCodelets248()
            : darts::Codelet()
        {
        }
        _checkInCodelets248(uint32_t dep, uint32_t res, TP247* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP211* TPParent;
    TP247* controlTPParent;
    TP247* inputsTPParent;
    struct packed_edge* __restrict* IJ_darts247; /*OMP_SHARED - INPUT*/
    int64_t* k_darts247 /*OMP_PRIVATE - INPUT*/;
    int64_t* nedge_darts247; /*OMP_SHARED - INPUT*/
    int64_t initIteration247;
    int64_t lastIteration247;
    int64_t range247;
    int64_t rangePerCodelet247;
    int64_t minIteration247;
    int64_t remainderRange247;
    size_t readyCodelets;
    int baseNumThreads;
    _barrierCodelets247* barrierCodelets247;
    _checkInCodelets248* checkInCodelets248;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets248* firstCodelet;
#endif
    TP247(int in_numThreads, int in_mainCodeletID, TP211* in_TPParent, int64_t in_initIteration,
        int64_t in_lastIteration, struct packed_edge* __restrict* in_IJ, int64_t* in_nedge,
        TP247** in_ptrToThisTP);
    void inline dispatchCodelet(size_t codeletID);
    ~TP247();
};
/*TP326: OMPForDirective*/
class TP326 : public ompTP {
public:
    class _barrierCodelets326 : public darts::Codelet {
    public:
        TP326* inputsTPParent;
        _barrierCodelets326()
            : darts::Codelet()
        {
        }
        _barrierCodelets326(uint32_t dep, uint32_t res, TP326* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations326(int64_t* endRange, uint32_t codeletID);
    class _checkInCodelets327 : public darts::Codelet {
    public:
        TP326* myTP;
        TP326* inputsTPParent;
        int64_t endRange;
        _checkInCodelets327()
            : darts::Codelet()
        {
        }
        _checkInCodelets327(uint32_t dep, uint32_t res, TP326* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP211* TPParent;
    TP326* controlTPParent;
    TP326* inputsTPParent;
    int64_t* k_darts326 /*OMP_PRIVATE - INPUT*/;
    int64_t initIteration326;
    int64_t lastIteration326;
    int64_t range326;
    int64_t rangePerCodelet326;
    int64_t minIteration326;
    int64_t remainderRange326;
    size_t readyCodelets;
    int baseNumThreads;
    _barrierCodelets326* barrierCodelets326;
    _checkInCodelets327* checkInCodelets327;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets327* firstCodelet;
#endif
    TP326(int in_numThreads, int in_mainCodeletID, TP211* in_TPParent, int64_t in_initIteration,
        int64_t in_lastIteration, TP326** in_ptrToThisTP);
    void inline dispatchCodelet(size_t codeletID);
    ~TP326();
};
/*TP363: OMPForDirective*/
class TP363 : public ompTP {
public:
    class _barrierCodelets363 : public darts::Codelet {
    public:
        TP363* inputsTPParent;
        _barrierCodelets363()
            : darts::Codelet()
        {
        }
        _barrierCodelets363(uint32_t dep, uint32_t res, TP363* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations363(int64_t* endRange, uint32_t codeletID);
    class _checkInCodelets364 : public darts::Codelet {
    public:
        TP363* myTP;
        TP363* inputsTPParent;
        int64_t endRange;
        _checkInCodelets364()
            : darts::Codelet()
        {
        }
        _checkInCodelets364(uint32_t dep, uint32_t res, TP363* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP211* TPParent;
    TP363* controlTPParent;
    TP363* inputsTPParent;
    int64_t* k_darts363 /*OMP_PRIVATE - INPUT*/;
    int64_t initIteration363;
    int64_t lastIteration363;
    int64_t range363;
    int64_t rangePerCodelet363;
    int64_t minIteration363;
    int64_t remainderRange363;
    size_t readyCodelets;
    int baseNumThreads;
    _barrierCodelets363* barrierCodelets363;
    _checkInCodelets364* checkInCodelets364;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets364* firstCodelet;
#endif
    TP363(int in_numThreads, int in_mainCodeletID, TP211* in_TPParent, int64_t in_initIteration,
        int64_t in_lastIteration, TP363** in_ptrToThisTP);
    void inline dispatchCodelet(size_t codeletID);
    ~TP363();
};
/*TP20: pack_edges*/
class TP20 : public ompTP {
public:
    class _checkInCodelets510 : public darts::Codelet {
    public:
        TP20* myTP;
        TP20* inputsTPParent;
        _checkInCodelets510()
            : darts::Codelet()
        {
        }
        _checkInCodelets510(uint32_t dep, uint32_t res, TP20* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets510 : public darts::Codelet {
    public:
        TP20* inputsTPParent;
        _barrierCodelets510()
            : darts::Codelet()
        {
        }
        _barrierCodelets510(uint32_t dep, uint32_t res, TP20* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP20** ptrToThisFunctionTP;
    TP20* inputsTPParent;
    TP20* controlTPParent;
    darts::Codelet** nextCodeletspack_edges;
    darts::Codelet** nextSyncCodeletspack_edges;
    int64_t* v_darts20 /*VARIABLE*/;
    TP510** TP510Ptr;
    size_t* TP510_alreadyLaunched;
    int numTPsSet510;
    int numTPsReady510;
    size_t TPsToUse510;
    size_t codeletsPerTP510;
    size_t totalCodelets510;
    _checkInCodelets510* checkInCodelets510;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets510* firstCodelet;
#endif
    _barrierCodelets510* barrierCodelets510;
    TP20(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_mainNextCodelet,
        darts::Codelet* in_mainSyncCodelet, TP20** in_ptrToThisFunctionTP);
    ~TP20();
};
/*TP510: OMPForDirective*/
class TP510 : public ompTP {
public:
    class _barrierCodelets510 : public darts::Codelet {
    public:
        TP510* inputsTPParent;
        _barrierCodelets510()
            : darts::Codelet()
        {
        }
        _barrierCodelets510(uint32_t dep, uint32_t res, TP510* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations510(int64_t* endRange, uint32_t codeletID);
    class _checkInCodelets511 : public darts::Codelet {
    public:
        TP510* myTP;
        TP510* inputsTPParent;
        int64_t endRange;
        _checkInCodelets511()
            : darts::Codelet()
        {
        }
        _checkInCodelets511(uint32_t dep, uint32_t res, TP510* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP20* TPParent;
    TP510* controlTPParent;
    TP510* inputsTPParent;
    int64_t* v_darts510 /*OMP_PRIVATE - INPUT*/;
    int64_t initIteration510;
    int64_t lastIteration510;
    int64_t range510;
    int64_t rangePerCodelet510;
    int64_t minIteration510;
    int64_t remainderRange510;
    size_t readyCodelets;
    int baseNumThreads;
    _barrierCodelets510* barrierCodelets510;
    _checkInCodelets511* checkInCodelets511;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets511* firstCodelet;
#endif
    TP510(int in_numThreads, int in_mainCodeletID, TP20* in_TPParent, int64_t in_initIteration,
        int64_t in_lastIteration, TP510** in_ptrToThisTP);
    void inline dispatchCodelet(size_t codeletID);
    ~TP510();
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
    class _barrierCodelets544 : public darts::Codelet {
    public:
        TP541* inputsTPParent;
        _barrierCodelets544()
            : darts::Codelet()
        {
        }
        _barrierCodelets544(uint32_t dep, uint32_t res, TP541* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets587 : public darts::Codelet {
    public:
        TP541* myTP;
        TP541* inputsTPParent;
        _checkInCodelets587()
            : darts::Codelet()
        {
        }
        _checkInCodelets587(uint32_t dep, uint32_t res, TP541* myTP, uint32_t id)
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
    struct packed_edge* __restrict* IJ_darts541; /*OMP_SHARED - INPUT*/
    int64_t* nedge_darts541; /*OMP_SHARED - INPUT*/
    int64_t* k_darts541 /*VARIABLE*/;
    TP544** TP544Ptr;
    size_t* TP544_alreadyLaunched;
    int numTPsSet544;
    int numTPsReady544;
    size_t TPsToUse544;
    size_t codeletsPerTP544;
    size_t totalCodelets544;
    TP_pack_edges* TP587Ptr;
    int TP587_alreadyLaunched;
    _barrierCodelets541* barrierCodelets541;
    _checkInCodelets544* checkInCodelets544;
    _barrierCodelets544* barrierCodelets544;
    _checkInCodelets587* checkInCodelets587;
    TP541(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet,
        struct packed_edge* __restrict* in_IJ, int64_t* in_nedge);
    ~TP541();
};
/*TP544: OMPForDirective*/
class TP544 : public ompTP {
public:
    class _barrierCodelets544 : public darts::Codelet {
    public:
        TP544* inputsTPParent;
        _barrierCodelets544()
            : darts::Codelet()
        {
        }
        _barrierCodelets544(uint32_t dep, uint32_t res, TP544* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations544(int64_t* endRange, uint32_t codeletID);
    class _checkInCodelets545 : public darts::Codelet {
    public:
        TP544* myTP;
        TP544* inputsTPParent;
        int64_t endRange;
        _checkInCodelets545()
            : darts::Codelet()
        {
        }
        _checkInCodelets545(uint32_t dep, uint32_t res, TP544* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP541* TPParent;
    TP544* controlTPParent;
    TP544* inputsTPParent;
    struct packed_edge* __restrict* IJ_darts544; /*OMP_SHARED - INPUT*/
    int64_t* k_darts544 /*OMP_PRIVATE - INPUT*/;
    int64_t* nedge_darts544; /*OMP_SHARED - INPUT*/
    int64_t initIteration544;
    int64_t lastIteration544;
    int64_t range544;
    int64_t rangePerCodelet544;
    int64_t minIteration544;
    int64_t remainderRange544;
    size_t readyCodelets;
    int baseNumThreads;
    _barrierCodelets544* barrierCodelets544;
    _checkInCodelets545* checkInCodelets545;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets545* firstCodelet;
#endif
    TP544(int in_numThreads, int in_mainCodeletID, TP541* in_TPParent, int64_t in_initIteration,
        int64_t in_lastIteration, struct packed_edge* __restrict* in_IJ, int64_t* in_nedge,
        TP544** in_ptrToThisTP);
    void inline dispatchCodelet(size_t codeletID);
    ~TP544();
};
/*TP644: OMPParallelDirective*/
class TP644 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets644 : public darts::Codelet {
    public:
        TP644* inputsTPParent;
        _barrierCodelets644()
            : darts::Codelet()
        {
        }
        _barrierCodelets644(uint32_t dep, uint32_t res, TP644* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets646 : public darts::Codelet {
    public:
        TP644* myTP;
        TP644* inputsTPParent;
        _checkInCodelets646()
            : darts::Codelet()
        {
        }
        _checkInCodelets646(uint32_t dep, uint32_t res, TP644* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets648_core : public darts::Codelet {
    public:
        Codelet* toSignal;
        int64_t initIteration;
        int64_t lastIteration;
        int64_t* __restrict* bfs_tree;
        int64_t* k1;
        int64_t* k2;
        int64_t* oldk2;
        int64_t* __restrict* vlist;
        _checkInCodelets648_core()
            : darts::Codelet()
        {
        }
        _checkInCodelets648_core(uint32_t dep, uint32_t res, TP644* myTP,
            darts::Codelet* in_nextCodelet, int64_t in_initIteration, int64_t in_lastIteration,
            int64_t* __restrict* in_bfs_tree, int64_t* in_k1, int64_t* in_k2, int64_t* in_oldk2,
            int64_t* __restrict* in_vlist)
            : darts::Codelet(dep, res, myTP, LONGWAIT)
            , toSignal(in_nextCodelet)
            , initIteration(in_initIteration)
            , lastIteration(in_lastIteration)
            , bfs_tree(in_bfs_tree)
            , k1(in_k1)
            , k2(in_k2)
            , oldk2(in_oldk2)
            , vlist(in_vlist)
        {
        }
        void fire(void);
    };
    class _checkInCodelets648 : public darts::Codelet {
    public:
        TP644* myTP;
        TP644* inputsTPParent;
        _checkInCodelets648()
            : darts::Codelet()
        {
        }
        _checkInCodelets648(uint32_t dep, uint32_t res, TP644* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets648 : public darts::Codelet {
    public:
        TP644* inputsTPParent;
        _barrierCodelets648()
            : darts::Codelet()
        {
        }
        _barrierCodelets648(uint32_t dep, uint32_t res, TP644* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets721 : public darts::Codelet {
    public:
        TP644* myTP;
        TP644* inputsTPParent;
        _checkInCodelets721()
            : darts::Codelet()
        {
        }
        _checkInCodelets721(uint32_t dep, uint32_t res, TP644* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP644* TPParent;
    TP644* controlTPParent;
    TP644* inputsTPParent;
    int64_t* __restrict* bfs_tree_darts644; /*OMP_SHARED - INPUT*/
    int64_t* k_darts644; /*OMP_SHARED - INPUT*/
    int64_t* k1_darts644; /*OMP_SHARED - INPUT*/
    int64_t* k2_darts644; /*OMP_SHARED - INPUT*/
    int64_t* oldk2_darts644; /*OMP_SHARED - INPUT*/
    int64_t* __restrict* vlist_darts644; /*OMP_SHARED - INPUT*/
    TP648** TP648Ptr;
    size_t* TP648_alreadyLaunched;
    int numTPsSet648;
    int numTPsReady648;
    size_t TPsToUse648;
    size_t codeletsPerTP648;
    size_t totalCodelets648;
    _barrierCodelets644* barrierCodelets644;
    _checkInCodelets646* checkInCodelets646;
    _checkInCodelets648* checkInCodelets648;
    _checkInCodelets648_core* masterCodeletTP648_core;
    darts::codeletFor<TP648_loop>* masterCodeletTP648_loop;
    darts::paraFor<TP648_outer>* masterCodeletTP648_outer;
    _barrierCodelets648* barrierCodelets648;
    _checkInCodelets721* checkInCodelets721;
    TP644(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet,
        int64_t* __restrict* in_bfs_tree, int64_t* in_k, int64_t* in_k1, int64_t* in_k2,
        int64_t* in_oldk2, int64_t* __restrict* in_vlist);
    ~TP644();
};
/*TP648: OMPForDirective*/
class TP648_loop : public darts::loop {
public:
    class _checkInCodelets649 : public darts::Codelet {
    public:
        TP648* myTP;
        int64_t* __restrict* bfs_tree;
        int64_t* k1;
        int64_t* k2;
        int64_t* oldk2;
        int64_t* __restrict* vlist;
        _checkInCodelets649()
            : darts::Codelet()
        {
        }
        _checkInCodelets649(uint32_t dep, uint32_t res, ThreadedProcedure* myTP,
            int64_t* __restrict* in_bfs_tree, int64_t* in_k1, int64_t* in_k2, int64_t* in_oldk2,
            int64_t* __restrict* in_vlist)
            : darts::Codelet(dep, res, myTP, LONGWAIT)
            , bfs_tree(in_bfs_tree)
            , k1(in_k1)
            , k2(in_k2)
            , oldk2(in_oldk2)
            , vlist(in_vlist)
        {
        }
        void fire(void);
    };
    unsigned int tileSize;
    int64_t initIteration;
    int64_t lastIteration;
    _checkInCodelets649* checkInCodelets649;
    TP648_loop(unsigned int it, darts::Codelet* in_nextCodelet, unsigned int in_tileSize,
        int64_t in_initIteration, int64_t in_lastIteration, int64_t* __restrict* in_bfs_tree,
        int64_t* in_k1, int64_t* in_k2, int64_t* in_oldk2, int64_t* __restrict* in_vlist);
    ~TP648_loop();
};
class TP648_outer : public darts::loop {
public:
    class TP648_inner : public darts::loop {
    public:
        class _checkInCodelets649 : public darts::Codelet {
        public:
            TP648* myTP;
            int64_t* __restrict* bfs_tree;
            int64_t* k1;
            int64_t* k2;
            int64_t* oldk2;
            int64_t* __restrict* vlist;
            _checkInCodelets649()
                : darts::Codelet()
            {
            }
            _checkInCodelets649(uint32_t dep, uint32_t res, ThreadedProcedure* myTP,
                int64_t* __restrict* in_bfs_tree, int64_t* in_k1, int64_t* in_k2, int64_t* in_oldk2,
                int64_t* __restrict* in_vlist)
                : darts::Codelet(dep, res, myTP, LONGWAIT)
                , bfs_tree(in_bfs_tree)
                , k1(in_k1)
                , k2(in_k2)
                , oldk2(in_oldk2)
                , vlist(in_vlist)
            {
            }
            void fire(void);
        };
        unsigned int outerIteration;
        unsigned int innerIteration;
        int64_t outerTile;
        int64_t innerTile;
        int64_t initIteration;
        int64_t lastIteration;
        _checkInCodelets649* checkInCodelets649;
        TP648_inner(unsigned int it, darts::Codelet* in_nextCodelet, unsigned int in_outerIteration,
            int64_t in_outerTile, int64_t in_innerTile, int64_t in_initIteration,
            int64_t in_lastIteration, int64_t* __restrict* in_bfs_tree, int64_t* in_k1,
            int64_t* in_k2, int64_t* in_oldk2, int64_t* __restrict* in_vlist);
        ~TP648_inner();
    };
    int64_t outerTile;
    unsigned int workers;
    int64_t initIteration;
    int64_t lastIteration;
    int64_t width;
    int64_t innerTile;
    darts::codeletFor<TP648_inner>* masterCodeletTP648_inner;
    TP648_outer(unsigned int it, darts::Codelet* in_nextCodelet, int64_t in_outerTile,
        unsigned int in_workers, int64_t in_initIteration, int64_t in_lastIteration,
        int64_t* __restrict* in_bfs_tree, int64_t* in_k1, int64_t* in_k2, int64_t* in_oldk2,
        int64_t* __restrict* in_vlist);
    ~TP648_outer();
};
#endif
