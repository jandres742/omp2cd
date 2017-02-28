#ifndef _eam_output_darts_h_
#define _eam_output_darts_h_
#ifndef __DARTS_
#define __DARTS_
#endif
#include "constants.h"
#include "darts.h"
#include "eam.h"
#include "haloExchange.h"
#include "haloExchange.output.darts.h"
#include "linkCells.h"
#include "linkCells.output.darts.h"
#include "memUtils.h"
#include "ompTP.h"
#include "parallel.h"
#include "parallel.output.darts.h"
#include "performanceTimers.h"
#include "performanceTimers.output.darts.h"
#include "tbb/concurrent_vector.h"
#include "utils.h"
#include <cassert>
#include <limits.h>
#include <math.h>
#include <mutex>
#include <numa.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
struct BasePotentialSt* initEamPot(const char* dir, const char* file, const char* type);
class TP2529;
class TP2531;
/*Number of TPs to be used for the OMPFor in region TP2531*/
#define NUMTPS2531 NUMTPS
class TP2579;
class TP2581;
/*Number of TPs to be used for the OMPFor in region TP2581*/
#define NUMTPS2581 NUMTPS
class TP2709;
class TP2711;
/*Number of TPs to be used for the OMPFor in region TP2711*/
#define NUMTPS2711 NUMTPS
class TP2781;
class TP2783;
/*Number of TPs to be used for the OMPFor in region TP2783*/
#define NUMTPS2783 NUMTPS
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
/*TP2529: OMPParallelDirective*/
class TP2529 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets2529 : public darts::Codelet {
    public:
        TP2529* inputsTPParent;
        _barrierCodelets2529()
            : darts::Codelet()
        {
        }
        _barrierCodelets2529(uint32_t dep, uint32_t res, TP2529* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets2531 : public darts::Codelet {
    public:
        TP2529* myTP;
        TP2529* inputsTPParent;
        _checkInCodelets2531()
            : darts::Codelet()
        {
        }
        _checkInCodelets2531(uint32_t dep, uint32_t res, TP2529* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets2531 : public darts::Codelet {
    public:
        TP2529* inputsTPParent;
        _barrierCodelets2531()
            : darts::Codelet()
        {
        }
        _barrierCodelets2531(uint32_t dep, uint32_t res, TP2529* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP2529* TPParent;
    TP2529* controlTPParent;
    TP2529* inputsTPParent;
    int* fsize_darts2529; /*OMP_SHARED - INPUT*/
    EamPotential** pot_darts2529; /*OMP_SHARED - INPUT*/
    SimFlat** s_darts2529; /*OMP_SHARED - INPUT*/
    TP2531** TP2531Ptr;
    size_t* TP2531_alreadyLaunched;
    int numTPsSet2531;
    int numTPsReady2531;
    size_t TPsToUse2531;
    size_t codeletsPerTP2531;
    size_t totalCodelets2531;
    _barrierCodelets2529* barrierCodelets2529;
    _checkInCodelets2531* checkInCodelets2531;
    _barrierCodelets2531* barrierCodelets2531;
    TP2529(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet, int* in_fsize,
        EamPotential** in_pot, SimFlat** in_s);
    ~TP2529();
};
/*TP2531: OMPForDirective*/
class TP2531 : public ompTP {
public:
    class _barrierCodelets2531 : public darts::Codelet {
    public:
        TP2531* inputsTPParent;
        _barrierCodelets2531()
            : darts::Codelet()
        {
        }
        _barrierCodelets2531(uint32_t dep, uint32_t res, TP2531* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations2531(int* endRange, uint32_t codeletID);
    class _checkInCodelets2532 : public darts::Codelet {
    public:
        TP2531* myTP;
        TP2531* inputsTPParent;
        int endRange;
        _checkInCodelets2532()
            : darts::Codelet()
        {
        }
        _checkInCodelets2532(uint32_t dep, uint32_t res, TP2531* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP2529* TPParent;
    TP2531* controlTPParent;
    TP2531* inputsTPParent;
    int* fsize_darts2531; /*OMP_SHARED - INPUT*/
    EamPotential** pot_darts2531; /*OMP_SHARED - INPUT*/
    SimFlat** s_darts2531; /*OMP_SHARED - INPUT*/
    int* ii_darts2531 /*VARIABLE*/;
    int initIteration2531;
    int lastIteration2531;
    int range2531;
    int rangePerCodelet2531;
    int minIteration2531;
    int remainderRange2531;
    size_t readyCodelets;
    int baseNumThreads;
    _barrierCodelets2531* barrierCodelets2531;
    _checkInCodelets2532* checkInCodelets2532;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets2532* firstCodelet;
#endif
    TP2531(int in_numThreads, int in_mainCodeletID, TP2529* in_TPParent, int in_initIteration,
        int in_lastIteration, int* in_fsize, EamPotential** in_pot, SimFlat** in_s,
        TP2531** in_ptrToThisTP);
    void inline dispatchCodelet(size_t codeletID);
    ~TP2531();
};
/*TP2579: OMPParallelDirective*/
class TP2579 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets2579 : public darts::Codelet {
    public:
        TP2579* inputsTPParent;
        _barrierCodelets2579()
            : darts::Codelet()
        {
        }
        _barrierCodelets2579(uint32_t dep, uint32_t res, TP2579* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets2581 : public darts::Codelet {
    public:
        TP2579* myTP;
        TP2579* inputsTPParent;
        _checkInCodelets2581()
            : darts::Codelet()
        {
        }
        _checkInCodelets2581(uint32_t dep, uint32_t res, TP2579* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets2581 : public darts::Codelet {
    public:
        TP2579* inputsTPParent;
        _barrierCodelets2581()
            : darts::Codelet()
        {
        }
        _barrierCodelets2581(uint32_t dep, uint32_t res, TP2579* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP2579* TPParent;
    TP2579* controlTPParent;
    TP2579* inputsTPParent;
    real_t* etot_darts2579; /*OMP_SHARED - INPUT*/
    int* nNbrBoxes_darts2579; /*OMP_SHARED - INPUT*/
    EamPotential** pot_darts2579; /*OMP_SHARED - INPUT*/
    real_t* rCut2_darts2579; /*OMP_SHARED - INPUT*/
    SimFlat** s_darts2579; /*OMP_SHARED - INPUT*/
    TP2581** TP2581Ptr;
    size_t* TP2581_alreadyLaunched;
    int numTPsSet2581;
    int numTPsReady2581;
    size_t TPsToUse2581;
    size_t codeletsPerTP2581;
    size_t totalCodelets2581;
    std::mutex etot_darts2581_mutex;
    _barrierCodelets2579* barrierCodelets2579;
    _checkInCodelets2581* checkInCodelets2581;
    _barrierCodelets2581* barrierCodelets2581;
    TP2579(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet, real_t* in_etot,
        int* in_nNbrBoxes, EamPotential** in_pot, real_t* in_rCut2, SimFlat** in_s);
    ~TP2579();
};
/*TP2581: OMPForDirective*/
class TP2581 : public ompTP {
public:
    class _barrierCodelets2581 : public darts::Codelet {
    public:
        TP2581* inputsTPParent;
        _barrierCodelets2581()
            : darts::Codelet()
        {
        }
        _barrierCodelets2581(uint32_t dep, uint32_t res, TP2581* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations2581(int* endRange, uint32_t codeletID);
    class _checkInCodelets2582 : public darts::Codelet {
    public:
        TP2581* myTP;
        TP2581* inputsTPParent;
        int endRange;
        _checkInCodelets2582()
            : darts::Codelet()
        {
        }
        _checkInCodelets2582(uint32_t dep, uint32_t res, TP2581* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP2579* TPParent;
    TP2581* controlTPParent;
    TP2581* inputsTPParent;
    real_t* etot_darts2581; /*OMP_SHARED - INPUT*/
    int* nNbrBoxes_darts2581; /*OMP_SHARED - INPUT*/
    EamPotential** pot_darts2581; /*OMP_SHARED - INPUT*/
    real_t* rCut2_darts2581; /*OMP_SHARED - INPUT*/
    SimFlat** s_darts2581; /*OMP_SHARED - INPUT*/
    int* iBox_darts2581 /*VARIABLE*/;
    int initIteration2581;
    int lastIteration2581;
    int range2581;
    int rangePerCodelet2581;
    int minIteration2581;
    int remainderRange2581;
    size_t readyCodelets;
    int baseNumThreads;
    std::mutex* etot_darts2581_mutex;
    _barrierCodelets2581* barrierCodelets2581;
    _checkInCodelets2582* checkInCodelets2582;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets2582* firstCodelet;
#endif
    TP2581(int in_numThreads, int in_mainCodeletID, TP2579* in_TPParent, int in_initIteration,
        int in_lastIteration, real_t* in_etot, int* in_nNbrBoxes, EamPotential** in_pot,
        real_t* in_rCut2, SimFlat** in_s, TP2581** in_ptrToThisTP);
    void inline dispatchCodelet(size_t codeletID);
    ~TP2581();
};
/*TP2709: OMPParallelDirective*/
class TP2709 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets2709 : public darts::Codelet {
    public:
        TP2709* inputsTPParent;
        _barrierCodelets2709()
            : darts::Codelet()
        {
        }
        _barrierCodelets2709(uint32_t dep, uint32_t res, TP2709* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets2711 : public darts::Codelet {
    public:
        TP2709* myTP;
        TP2709* inputsTPParent;
        _checkInCodelets2711()
            : darts::Codelet()
        {
        }
        _checkInCodelets2711(uint32_t dep, uint32_t res, TP2709* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets2711 : public darts::Codelet {
    public:
        TP2709* inputsTPParent;
        _barrierCodelets2711()
            : darts::Codelet()
        {
        }
        _barrierCodelets2711(uint32_t dep, uint32_t res, TP2709* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP2709* TPParent;
    TP2709* controlTPParent;
    TP2709* inputsTPParent;
    real_t* etot_darts2709; /*OMP_SHARED - INPUT*/
    EamPotential** pot_darts2709; /*OMP_SHARED - INPUT*/
    SimFlat** s_darts2709; /*OMP_SHARED - INPUT*/
    TP2711** TP2711Ptr;
    size_t* TP2711_alreadyLaunched;
    int numTPsSet2711;
    int numTPsReady2711;
    size_t TPsToUse2711;
    size_t codeletsPerTP2711;
    size_t totalCodelets2711;
    std::mutex etot_darts2711_mutex;
    _barrierCodelets2709* barrierCodelets2709;
    _checkInCodelets2711* checkInCodelets2711;
    _barrierCodelets2711* barrierCodelets2711;
    TP2709(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet, real_t* in_etot,
        EamPotential** in_pot, SimFlat** in_s);
    ~TP2709();
};
/*TP2711: OMPForDirective*/
class TP2711 : public ompTP {
public:
    class _barrierCodelets2711 : public darts::Codelet {
    public:
        TP2711* inputsTPParent;
        _barrierCodelets2711()
            : darts::Codelet()
        {
        }
        _barrierCodelets2711(uint32_t dep, uint32_t res, TP2711* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations2711(int* endRange, uint32_t codeletID);
    class _checkInCodelets2712 : public darts::Codelet {
    public:
        TP2711* myTP;
        TP2711* inputsTPParent;
        int endRange;
        _checkInCodelets2712()
            : darts::Codelet()
        {
        }
        _checkInCodelets2712(uint32_t dep, uint32_t res, TP2711* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP2709* TPParent;
    TP2711* controlTPParent;
    TP2711* inputsTPParent;
    real_t* etot_darts2711; /*OMP_SHARED - INPUT*/
    EamPotential** pot_darts2711; /*OMP_SHARED - INPUT*/
    SimFlat** s_darts2711; /*OMP_SHARED - INPUT*/
    int* iBox_darts2711 /*VARIABLE*/;
    int initIteration2711;
    int lastIteration2711;
    int range2711;
    int rangePerCodelet2711;
    int minIteration2711;
    int remainderRange2711;
    size_t readyCodelets;
    int baseNumThreads;
    std::mutex* etot_darts2711_mutex;
    _barrierCodelets2711* barrierCodelets2711;
    _checkInCodelets2712* checkInCodelets2712;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets2712* firstCodelet;
#endif
    TP2711(int in_numThreads, int in_mainCodeletID, TP2709* in_TPParent, int in_initIteration,
        int in_lastIteration, real_t* in_etot, EamPotential** in_pot, SimFlat** in_s,
        TP2711** in_ptrToThisTP);
    void inline dispatchCodelet(size_t codeletID);
    ~TP2711();
};
/*TP2781: OMPParallelDirective*/
class TP2781 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets2781 : public darts::Codelet {
    public:
        TP2781* inputsTPParent;
        _barrierCodelets2781()
            : darts::Codelet()
        {
        }
        _barrierCodelets2781(uint32_t dep, uint32_t res, TP2781* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets2783 : public darts::Codelet {
    public:
        TP2781* myTP;
        TP2781* inputsTPParent;
        _checkInCodelets2783()
            : darts::Codelet()
        {
        }
        _checkInCodelets2783(uint32_t dep, uint32_t res, TP2781* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets2783 : public darts::Codelet {
    public:
        TP2781* inputsTPParent;
        _barrierCodelets2783()
            : darts::Codelet()
        {
        }
        _barrierCodelets2783(uint32_t dep, uint32_t res, TP2781* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP2781* TPParent;
    TP2781* controlTPParent;
    TP2781* inputsTPParent;
    int* nNbrBoxes_darts2781; /*OMP_SHARED - INPUT*/
    EamPotential** pot_darts2781; /*OMP_SHARED - INPUT*/
    real_t* rCut2_darts2781; /*OMP_SHARED - INPUT*/
    SimFlat** s_darts2781; /*OMP_SHARED - INPUT*/
    TP2783** TP2783Ptr;
    size_t* TP2783_alreadyLaunched;
    int numTPsSet2783;
    int numTPsReady2783;
    size_t TPsToUse2783;
    size_t codeletsPerTP2783;
    size_t totalCodelets2783;
    _barrierCodelets2781* barrierCodelets2781;
    _checkInCodelets2783* checkInCodelets2783;
    _barrierCodelets2783* barrierCodelets2783;
    TP2781(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet,
        int* in_nNbrBoxes, EamPotential** in_pot, real_t* in_rCut2, SimFlat** in_s);
    ~TP2781();
};
/*TP2783: OMPForDirective*/
class TP2783 : public ompTP {
public:
    class _barrierCodelets2783 : public darts::Codelet {
    public:
        TP2783* inputsTPParent;
        _barrierCodelets2783()
            : darts::Codelet()
        {
        }
        _barrierCodelets2783(uint32_t dep, uint32_t res, TP2783* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations2783(int* endRange, uint32_t codeletID);
    class _checkInCodelets2784 : public darts::Codelet {
    public:
        TP2783* myTP;
        TP2783* inputsTPParent;
        int endRange;
        _checkInCodelets2784()
            : darts::Codelet()
        {
        }
        _checkInCodelets2784(uint32_t dep, uint32_t res, TP2783* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP2781* TPParent;
    TP2783* controlTPParent;
    TP2783* inputsTPParent;
    int* nNbrBoxes_darts2783; /*OMP_SHARED - INPUT*/
    EamPotential** pot_darts2783; /*OMP_SHARED - INPUT*/
    real_t* rCut2_darts2783; /*OMP_SHARED - INPUT*/
    SimFlat** s_darts2783; /*OMP_SHARED - INPUT*/
    int* iBox_darts2783 /*VARIABLE*/;
    int initIteration2783;
    int lastIteration2783;
    int range2783;
    int rangePerCodelet2783;
    int minIteration2783;
    int remainderRange2783;
    size_t readyCodelets;
    int baseNumThreads;
    _barrierCodelets2783* barrierCodelets2783;
    _checkInCodelets2784* checkInCodelets2784;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets2784* firstCodelet;
#endif
    TP2783(int in_numThreads, int in_mainCodeletID, TP2781* in_TPParent, int in_initIteration,
        int in_lastIteration, int* in_nNbrBoxes, EamPotential** in_pot, real_t* in_rCut2,
        SimFlat** in_s, TP2783** in_ptrToThisTP);
    void inline dispatchCodelet(size_t codeletID);
    ~TP2783();
};
#endif
