#ifndef _initAtoms_output_darts_h_
#define _initAtoms_output_darts_h_
#ifndef __DARTS_
#define __DARTS_
#endif
#include "constants.h"
#include "darts.h"
#include "decomposition.h"
#include "decomposition.output.darts.h"
#include "initAtoms.h"
#include "linkCells.h"
#include "linkCells.output.darts.h"
#include "memUtils.h"
#include "ompTP.h"
#include "parallel.h"
#include "parallel.output.darts.h"
#include "performanceTimers.h"
#include "performanceTimers.output.darts.h"
#include "random.h"
#include "random.output.darts.h"
#include "tbb/concurrent_vector.h"
#include "timestep.h"
#include "timestep.output.darts.h"
#include "utils.h"
#include <cassert>
#include <limits.h>
#include <math.h>
#include <mutex>
#include <numa.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
void randomDisplacements(SimFlat* s, real_t delta);
void setTemperature(SimFlat* s, real_t temperature);
void setVcm(SimFlat* s, real_t newVcm[3]);
void createFccLattice(int nx, int ny, int nz, real_t lat, SimFlat* s);
void destroyAtoms(Atoms* atoms);
Atoms* initAtoms(LinkCell* boxes);
class TP5150;
class TP5152;
/*Number of TPs to be used for the OMPFor in region TP5152*/
#define NUMTPS5152 NUMTPS
class TP5226;
class TP5228;
/*Number of TPs to be used for the OMPFor in region TP5228*/
#define NUMTPS5228 NUMTPS
class TP5340;
class TP5342;
/*Number of TPs to be used for the OMPFor in region TP5342*/
#define NUMTPS5342 NUMTPS
class TP5410;
class TP5412;
/*Number of TPs to be used for the OMPFor in region TP5412*/
#define NUMTPS5412 NUMTPS
class TP5525;
class TP5527;
/*Number of TPs to be used for the OMPFor in region TP5527*/
#define NUMTPS5527 NUMTPS
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
/*TP5150: OMPParallelDirective*/
class TP5150 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets5150 : public darts::Codelet {
    public:
        TP5150* inputsTPParent;
        _barrierCodelets5150()
            : darts::Codelet()
        {
        }
        _barrierCodelets5150(uint32_t dep, uint32_t res, TP5150* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets5152 : public darts::Codelet {
    public:
        TP5150* myTP;
        TP5150* inputsTPParent;
        _checkInCodelets5152()
            : darts::Codelet()
        {
        }
        _checkInCodelets5152(uint32_t dep, uint32_t res, TP5150* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets5152 : public darts::Codelet {
    public:
        TP5150* inputsTPParent;
        _barrierCodelets5152()
            : darts::Codelet()
        {
        }
        _barrierCodelets5152(uint32_t dep, uint32_t res, TP5150* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP5150* TPParent;
    TP5150* controlTPParent;
    TP5150* inputsTPParent;
    SimFlat** s_darts5150; /*OMP_SHARED - INPUT*/
    real_t** vShift_darts5150; /*OMP_SHARED - INPUT*/
    TP5152** TP5152Ptr;
    size_t* TP5152_alreadyLaunched;
    int numTPsSet5152;
    int numTPsReady5152;
    size_t TPsToUse5152;
    size_t codeletsPerTP5152;
    size_t totalCodelets5152;
    _barrierCodelets5150* barrierCodelets5150;
    _checkInCodelets5152* checkInCodelets5152;
    _barrierCodelets5152* barrierCodelets5152;
    TP5150(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet, SimFlat** in_s,
        real_t** in_vShift);
    ~TP5150();
};
/*TP5152: OMPForDirective*/
class TP5152 : public ompTP {
public:
    class _barrierCodelets5152 : public darts::Codelet {
    public:
        TP5152* inputsTPParent;
        _barrierCodelets5152()
            : darts::Codelet()
        {
        }
        _barrierCodelets5152(uint32_t dep, uint32_t res, TP5152* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations5152(int* endRange, uint32_t codeletID);
    class _checkInCodelets5153 : public darts::Codelet {
    public:
        TP5152* myTP;
        TP5152* inputsTPParent;
        int endRange;
        _checkInCodelets5153()
            : darts::Codelet()
        {
        }
        _checkInCodelets5153(uint32_t dep, uint32_t res, TP5152* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP5150* TPParent;
    TP5152* controlTPParent;
    TP5152* inputsTPParent;
    SimFlat** s_darts5152; /*OMP_SHARED - INPUT*/
    real_t** vShift_darts5152; /*OMP_SHARED - INPUT*/
    int* iBox_darts5152 /*VARIABLE*/;
    int initIteration5152;
    int lastIteration5152;
    int range5152;
    int rangePerCodelet5152;
    int minIteration5152;
    int remainderRange5152;
    size_t readyCodelets;
    int baseNumThreads;
    _barrierCodelets5152* barrierCodelets5152;
    _checkInCodelets5153* checkInCodelets5153;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets5153* firstCodelet;
#endif
    TP5152(int in_numThreads, int in_mainCodeletID, TP5150* in_TPParent, int in_initIteration,
        int in_lastIteration, SimFlat** in_s, real_t** in_vShift, TP5152** in_ptrToThisTP);
    void inline dispatchCodelet(size_t codeletID);
    ~TP5152();
};
/*TP5226: OMPParallelDirective*/
class TP5226 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets5226 : public darts::Codelet {
    public:
        TP5226* inputsTPParent;
        _barrierCodelets5226()
            : darts::Codelet()
        {
        }
        _barrierCodelets5226(uint32_t dep, uint32_t res, TP5226* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets5228 : public darts::Codelet {
    public:
        TP5226* myTP;
        TP5226* inputsTPParent;
        _checkInCodelets5228()
            : darts::Codelet()
        {
        }
        _checkInCodelets5228(uint32_t dep, uint32_t res, TP5226* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets5228 : public darts::Codelet {
    public:
        TP5226* inputsTPParent;
        _barrierCodelets5228()
            : darts::Codelet()
        {
        }
        _barrierCodelets5228(uint32_t dep, uint32_t res, TP5226* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP5226* TPParent;
    TP5226* controlTPParent;
    TP5226* inputsTPParent;
    SimFlat** s_darts5226; /*OMP_SHARED - INPUT*/
    real_t* temperature_darts5226; /*OMP_SHARED - INPUT*/
    TP5228** TP5228Ptr;
    size_t* TP5228_alreadyLaunched;
    int numTPsSet5228;
    int numTPsReady5228;
    size_t TPsToUse5228;
    size_t codeletsPerTP5228;
    size_t totalCodelets5228;
    _barrierCodelets5226* barrierCodelets5226;
    _checkInCodelets5228* checkInCodelets5228;
    _barrierCodelets5228* barrierCodelets5228;
    TP5226(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet, SimFlat** in_s,
        real_t* in_temperature);
    ~TP5226();
};
/*TP5228: OMPForDirective*/
class TP5228 : public ompTP {
public:
    class _barrierCodelets5228 : public darts::Codelet {
    public:
        TP5228* inputsTPParent;
        _barrierCodelets5228()
            : darts::Codelet()
        {
        }
        _barrierCodelets5228(uint32_t dep, uint32_t res, TP5228* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations5228(int* endRange, uint32_t codeletID);
    class _checkInCodelets5229 : public darts::Codelet {
    public:
        TP5228* myTP;
        TP5228* inputsTPParent;
        int endRange;
        _checkInCodelets5229()
            : darts::Codelet()
        {
        }
        _checkInCodelets5229(uint32_t dep, uint32_t res, TP5228* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP5226* TPParent;
    TP5228* controlTPParent;
    TP5228* inputsTPParent;
    SimFlat** s_darts5228; /*OMP_SHARED - INPUT*/
    real_t* temperature_darts5228; /*OMP_SHARED - INPUT*/
    int* iBox_darts5228 /*VARIABLE*/;
    int initIteration5228;
    int lastIteration5228;
    int range5228;
    int rangePerCodelet5228;
    int minIteration5228;
    int remainderRange5228;
    size_t readyCodelets;
    int baseNumThreads;
    _barrierCodelets5228* barrierCodelets5228;
    _checkInCodelets5229* checkInCodelets5229;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets5229* firstCodelet;
#endif
    TP5228(int in_numThreads, int in_mainCodeletID, TP5226* in_TPParent, int in_initIteration,
        int in_lastIteration, SimFlat** in_s, real_t* in_temperature, TP5228** in_ptrToThisTP);
    void inline dispatchCodelet(size_t codeletID);
    ~TP5228();
};
/*TP5340: OMPParallelDirective*/
class TP5340 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets5340 : public darts::Codelet {
    public:
        TP5340* inputsTPParent;
        _barrierCodelets5340()
            : darts::Codelet()
        {
        }
        _barrierCodelets5340(uint32_t dep, uint32_t res, TP5340* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets5342 : public darts::Codelet {
    public:
        TP5340* myTP;
        TP5340* inputsTPParent;
        _checkInCodelets5342()
            : darts::Codelet()
        {
        }
        _checkInCodelets5342(uint32_t dep, uint32_t res, TP5340* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets5342 : public darts::Codelet {
    public:
        TP5340* inputsTPParent;
        _barrierCodelets5342()
            : darts::Codelet()
        {
        }
        _barrierCodelets5342(uint32_t dep, uint32_t res, TP5340* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP5340* TPParent;
    TP5340* controlTPParent;
    TP5340* inputsTPParent;
    SimFlat** s_darts5340; /*OMP_SHARED - INPUT*/
    real_t* scaleFactor_darts5340; /*OMP_SHARED - INPUT*/
    TP5342** TP5342Ptr;
    size_t* TP5342_alreadyLaunched;
    int numTPsSet5342;
    int numTPsReady5342;
    size_t TPsToUse5342;
    size_t codeletsPerTP5342;
    size_t totalCodelets5342;
    _barrierCodelets5340* barrierCodelets5340;
    _checkInCodelets5342* checkInCodelets5342;
    _barrierCodelets5342* barrierCodelets5342;
    TP5340(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet, SimFlat** in_s,
        real_t* in_scaleFactor);
    ~TP5340();
};
/*TP5342: OMPForDirective*/
class TP5342 : public ompTP {
public:
    class _barrierCodelets5342 : public darts::Codelet {
    public:
        TP5342* inputsTPParent;
        _barrierCodelets5342()
            : darts::Codelet()
        {
        }
        _barrierCodelets5342(uint32_t dep, uint32_t res, TP5342* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations5342(int* endRange, uint32_t codeletID);
    class _checkInCodelets5343 : public darts::Codelet {
    public:
        TP5342* myTP;
        TP5342* inputsTPParent;
        int endRange;
        _checkInCodelets5343()
            : darts::Codelet()
        {
        }
        _checkInCodelets5343(uint32_t dep, uint32_t res, TP5342* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP5340* TPParent;
    TP5342* controlTPParent;
    TP5342* inputsTPParent;
    SimFlat** s_darts5342; /*OMP_SHARED - INPUT*/
    real_t* scaleFactor_darts5342; /*OMP_SHARED - INPUT*/
    int* iBox_darts5342 /*VARIABLE*/;
    int initIteration5342;
    int lastIteration5342;
    int range5342;
    int rangePerCodelet5342;
    int minIteration5342;
    int remainderRange5342;
    size_t readyCodelets;
    int baseNumThreads;
    _barrierCodelets5342* barrierCodelets5342;
    _checkInCodelets5343* checkInCodelets5343;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets5343* firstCodelet;
#endif
    TP5342(int in_numThreads, int in_mainCodeletID, TP5340* in_TPParent, int in_initIteration,
        int in_lastIteration, SimFlat** in_s, real_t* in_scaleFactor, TP5342** in_ptrToThisTP);
    void inline dispatchCodelet(size_t codeletID);
    ~TP5342();
};
/*TP5410: OMPParallelDirective*/
class TP5410 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets5410 : public darts::Codelet {
    public:
        TP5410* inputsTPParent;
        _barrierCodelets5410()
            : darts::Codelet()
        {
        }
        _barrierCodelets5410(uint32_t dep, uint32_t res, TP5410* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets5412 : public darts::Codelet {
    public:
        TP5410* myTP;
        TP5410* inputsTPParent;
        _checkInCodelets5412()
            : darts::Codelet()
        {
        }
        _checkInCodelets5412(uint32_t dep, uint32_t res, TP5410* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets5412 : public darts::Codelet {
    public:
        TP5410* inputsTPParent;
        _barrierCodelets5412()
            : darts::Codelet()
        {
        }
        _barrierCodelets5412(uint32_t dep, uint32_t res, TP5410* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP5410* TPParent;
    TP5410* controlTPParent;
    TP5410* inputsTPParent;
    real_t* delta_darts5410; /*OMP_SHARED - INPUT*/
    SimFlat** s_darts5410; /*OMP_SHARED - INPUT*/
    TP5412** TP5412Ptr;
    size_t* TP5412_alreadyLaunched;
    int numTPsSet5412;
    int numTPsReady5412;
    size_t TPsToUse5412;
    size_t codeletsPerTP5412;
    size_t totalCodelets5412;
    _barrierCodelets5410* barrierCodelets5410;
    _checkInCodelets5412* checkInCodelets5412;
    _barrierCodelets5412* barrierCodelets5412;
    TP5410(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet,
        real_t* in_delta, SimFlat** in_s);
    ~TP5410();
};
/*TP5412: OMPForDirective*/
class TP5412 : public ompTP {
public:
    class _barrierCodelets5412 : public darts::Codelet {
    public:
        TP5412* inputsTPParent;
        _barrierCodelets5412()
            : darts::Codelet()
        {
        }
        _barrierCodelets5412(uint32_t dep, uint32_t res, TP5412* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations5412(int* endRange, uint32_t codeletID);
    class _checkInCodelets5413 : public darts::Codelet {
    public:
        TP5412* myTP;
        TP5412* inputsTPParent;
        int endRange;
        _checkInCodelets5413()
            : darts::Codelet()
        {
        }
        _checkInCodelets5413(uint32_t dep, uint32_t res, TP5412* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP5410* TPParent;
    TP5412* controlTPParent;
    TP5412* inputsTPParent;
    real_t* delta_darts5412; /*OMP_SHARED - INPUT*/
    SimFlat** s_darts5412; /*OMP_SHARED - INPUT*/
    int* iBox_darts5412 /*VARIABLE*/;
    int initIteration5412;
    int lastIteration5412;
    int range5412;
    int rangePerCodelet5412;
    int minIteration5412;
    int remainderRange5412;
    size_t readyCodelets;
    int baseNumThreads;
    _barrierCodelets5412* barrierCodelets5412;
    _checkInCodelets5413* checkInCodelets5413;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets5413* firstCodelet;
#endif
    TP5412(int in_numThreads, int in_mainCodeletID, TP5410* in_TPParent, int in_initIteration,
        int in_lastIteration, real_t* in_delta, SimFlat** in_s, TP5412** in_ptrToThisTP);
    void inline dispatchCodelet(size_t codeletID);
    ~TP5412();
};
/*TP5525: OMPParallelDirective*/
class TP5525 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets5525 : public darts::Codelet {
    public:
        TP5525* inputsTPParent;
        _barrierCodelets5525()
            : darts::Codelet()
        {
        }
        _barrierCodelets5525(uint32_t dep, uint32_t res, TP5525* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets5527 : public darts::Codelet {
    public:
        TP5525* myTP;
        TP5525* inputsTPParent;
        _checkInCodelets5527()
            : darts::Codelet()
        {
        }
        _checkInCodelets5527(uint32_t dep, uint32_t res, TP5525* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets5527 : public darts::Codelet {
    public:
        TP5525* inputsTPParent;
        _barrierCodelets5527()
            : darts::Codelet()
        {
        }
        _barrierCodelets5527(uint32_t dep, uint32_t res, TP5525* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP5525* TPParent;
    TP5525* controlTPParent;
    TP5525* inputsTPParent;
    SimFlat** s_darts5525; /*OMP_SHARED - INPUT*/
    real_t* v0_darts5525; /*OMP_SHARED - INPUT*/
    real_t* v1_darts5525; /*OMP_SHARED - INPUT*/
    real_t* v2_darts5525; /*OMP_SHARED - INPUT*/
    real_t* v3_darts5525; /*OMP_SHARED - INPUT*/
    TP5527** TP5527Ptr;
    size_t* TP5527_alreadyLaunched;
    int numTPsSet5527;
    int numTPsReady5527;
    size_t TPsToUse5527;
    size_t codeletsPerTP5527;
    size_t totalCodelets5527;
    std::mutex v3_darts5527_mutex;
    std::mutex v2_darts5527_mutex;
    std::mutex v1_darts5527_mutex;
    std::mutex v0_darts5527_mutex;
    _barrierCodelets5525* barrierCodelets5525;
    _checkInCodelets5527* checkInCodelets5527;
    _barrierCodelets5527* barrierCodelets5527;
    TP5525(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet, SimFlat** in_s,
        real_t* in_v0, real_t* in_v1, real_t* in_v2, real_t* in_v3);
    ~TP5525();
};
/*TP5527: OMPForDirective*/
class TP5527 : public ompTP {
public:
    class _barrierCodelets5527 : public darts::Codelet {
    public:
        TP5527* inputsTPParent;
        _barrierCodelets5527()
            : darts::Codelet()
        {
        }
        _barrierCodelets5527(uint32_t dep, uint32_t res, TP5527* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations5527(int* endRange, uint32_t codeletID);
    class _checkInCodelets5528 : public darts::Codelet {
    public:
        TP5527* myTP;
        TP5527* inputsTPParent;
        int endRange;
        _checkInCodelets5528()
            : darts::Codelet()
        {
        }
        _checkInCodelets5528(uint32_t dep, uint32_t res, TP5527* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP5525* TPParent;
    TP5527* controlTPParent;
    TP5527* inputsTPParent;
    SimFlat** s_darts5527; /*OMP_SHARED - INPUT*/
    real_t* v0_darts5527; /*OMP_SHARED - INPUT*/
    real_t* v1_darts5527; /*OMP_SHARED - INPUT*/
    real_t* v2_darts5527; /*OMP_SHARED - INPUT*/
    real_t* v3_darts5527; /*OMP_SHARED - INPUT*/
    int* iBox_darts5527 /*VARIABLE*/;
    int initIteration5527;
    int lastIteration5527;
    int range5527;
    int rangePerCodelet5527;
    int minIteration5527;
    int remainderRange5527;
    size_t readyCodelets;
    int baseNumThreads;
    std::mutex* v3_darts5527_mutex;
    std::mutex* v2_darts5527_mutex;
    std::mutex* v1_darts5527_mutex;
    std::mutex* v0_darts5527_mutex;
    _barrierCodelets5527* barrierCodelets5527;
    _checkInCodelets5528* checkInCodelets5528;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets5528* firstCodelet;
#endif
    TP5527(int in_numThreads, int in_mainCodeletID, TP5525* in_TPParent, int in_initIteration,
        int in_lastIteration, SimFlat** in_s, real_t* in_v0, real_t* in_v1, real_t* in_v2,
        real_t* in_v3, TP5527** in_ptrToThisTP);
    void inline dispatchCodelet(size_t codeletID);
    ~TP5527();
};
#endif
