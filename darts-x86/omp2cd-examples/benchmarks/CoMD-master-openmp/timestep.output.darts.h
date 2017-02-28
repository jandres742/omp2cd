#ifndef _timestep_output_darts_h_
#define _timestep_output_darts_h_
#ifndef __DARTS_
#define __DARTS_
#endif
#include "CoMDTypes.h"
#include "darts.h"
#include "linkCells.h"
#include "linkCells.output.darts.h"
#include "ompTP.h"
#include "parallel.h"
#include "parallel.output.darts.h"
#include "performanceTimers.h"
#include "performanceTimers.output.darts.h"
#include "tbb/concurrent_vector.h"
#include "timestep.h"
#include "utils.h"
#include <limits.h>
#include <mutex>
#include <numa.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
void redistributeAtoms(SimFlat* sim);
void computeForce(SimFlat* s);
void kineticEnergy(SimFlat* s);
double timestep(SimFlat* s, int nSteps, real_t dt);
class TP436;
class TP438;
/*Number of TPs to be used for the OMPFor in region TP438*/
#define NUMTPS438 NUMTPS
class TP510;
class TP512;
/*Number of TPs to be used for the OMPFor in region TP512*/
#define NUMTPS512 NUMTPS
class TP605;
class TP607;
/*Number of TPs to be used for the OMPFor in region TP607*/
#define NUMTPS607 NUMTPS
class TP723;
class TP725;
/*Number of TPs to be used for the OMPFor in region TP725*/
#define NUMTPS725 NUMTPS
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
/*TP436: OMPParallelDirective*/
class TP436 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets436 : public darts::Codelet {
    public:
        TP436* inputsTPParent;
        _barrierCodelets436()
            : darts::Codelet()
        {
        }
        _barrierCodelets436(uint32_t dep, uint32_t res, TP436* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets438 : public darts::Codelet {
    public:
        TP436* myTP;
        TP436* inputsTPParent;
        _checkInCodelets438()
            : darts::Codelet()
        {
        }
        _checkInCodelets438(uint32_t dep, uint32_t res, TP436* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets438 : public darts::Codelet {
    public:
        TP436* inputsTPParent;
        _barrierCodelets438()
            : darts::Codelet()
        {
        }
        _barrierCodelets438(uint32_t dep, uint32_t res, TP436* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP436* TPParent;
    TP436* controlTPParent;
    TP436* inputsTPParent;
    real_t* dt_darts436; /*OMP_SHARED - INPUT*/
    int* nBoxes_darts436; /*OMP_SHARED - INPUT*/
    SimFlat** s_darts436; /*OMP_SHARED - INPUT*/
    TP438** TP438Ptr;
    size_t* TP438_alreadyLaunched;
    int numTPsSet438;
    int numTPsReady438;
    size_t TPsToUse438;
    size_t codeletsPerTP438;
    size_t totalCodelets438;
    _barrierCodelets436* barrierCodelets436;
    _checkInCodelets438* checkInCodelets438;
    _barrierCodelets438* barrierCodelets438;
    TP436(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet, real_t* in_dt,
        int* in_nBoxes, SimFlat** in_s);
    ~TP436();
};
/*TP438: OMPForDirective*/
class TP438 : public ompTP {
public:
    class _barrierCodelets438 : public darts::Codelet {
    public:
        TP438* inputsTPParent;
        _barrierCodelets438()
            : darts::Codelet()
        {
        }
        _barrierCodelets438(uint32_t dep, uint32_t res, TP438* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations438(int* endRange, uint32_t codeletID);
    class _checkInCodelets439 : public darts::Codelet {
    public:
        TP438* myTP;
        TP438* inputsTPParent;
        int endRange;
        _checkInCodelets439()
            : darts::Codelet()
        {
        }
        _checkInCodelets439(uint32_t dep, uint32_t res, TP438* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP436* TPParent;
    TP438* controlTPParent;
    TP438* inputsTPParent;
    real_t* dt_darts438; /*OMP_SHARED - INPUT*/
    int* nBoxes_darts438; /*OMP_SHARED - INPUT*/
    SimFlat** s_darts438; /*OMP_SHARED - INPUT*/
    int* iBox_darts438 /*VARIABLE*/;
    int initIteration438;
    int lastIteration438;
    int range438;
    int rangePerCodelet438;
    int minIteration438;
    int remainderRange438;
    size_t readyCodelets;
    int baseNumThreads;
    _barrierCodelets438* barrierCodelets438;
    _checkInCodelets439* checkInCodelets439;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets439* firstCodelet;
#endif
    TP438(int in_numThreads, int in_mainCodeletID, TP436* in_TPParent, int in_initIteration,
        int in_lastIteration, real_t* in_dt, int* in_nBoxes, SimFlat** in_s,
        TP438** in_ptrToThisTP);
    void inline dispatchCodelet(size_t codeletID);
    ~TP438();
};
/*TP510: OMPParallelDirective*/
class TP510 : public darts::ThreadedProcedure {
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
    class _checkInCodelets512 : public darts::Codelet {
    public:
        TP510* myTP;
        TP510* inputsTPParent;
        _checkInCodelets512()
            : darts::Codelet()
        {
        }
        _checkInCodelets512(uint32_t dep, uint32_t res, TP510* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets512 : public darts::Codelet {
    public:
        TP510* inputsTPParent;
        _barrierCodelets512()
            : darts::Codelet()
        {
        }
        _barrierCodelets512(uint32_t dep, uint32_t res, TP510* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP510* TPParent;
    TP510* controlTPParent;
    TP510* inputsTPParent;
    real_t* dt_darts510; /*OMP_SHARED - INPUT*/
    int* nBoxes_darts510; /*OMP_SHARED - INPUT*/
    SimFlat** s_darts510; /*OMP_SHARED - INPUT*/
    TP512** TP512Ptr;
    size_t* TP512_alreadyLaunched;
    int numTPsSet512;
    int numTPsReady512;
    size_t TPsToUse512;
    size_t codeletsPerTP512;
    size_t totalCodelets512;
    _barrierCodelets510* barrierCodelets510;
    _checkInCodelets512* checkInCodelets512;
    _barrierCodelets512* barrierCodelets512;
    TP510(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet, real_t* in_dt,
        int* in_nBoxes, SimFlat** in_s);
    ~TP510();
};
/*TP512: OMPForDirective*/
class TP512 : public ompTP {
public:
    class _barrierCodelets512 : public darts::Codelet {
    public:
        TP512* inputsTPParent;
        _barrierCodelets512()
            : darts::Codelet()
        {
        }
        _barrierCodelets512(uint32_t dep, uint32_t res, TP512* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations512(int* endRange, uint32_t codeletID);
    class _checkInCodelets513 : public darts::Codelet {
    public:
        TP512* myTP;
        TP512* inputsTPParent;
        int endRange;
        _checkInCodelets513()
            : darts::Codelet()
        {
        }
        _checkInCodelets513(uint32_t dep, uint32_t res, TP512* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP510* TPParent;
    TP512* controlTPParent;
    TP512* inputsTPParent;
    real_t* dt_darts512; /*OMP_SHARED - INPUT*/
    int* nBoxes_darts512; /*OMP_SHARED - INPUT*/
    SimFlat** s_darts512; /*OMP_SHARED - INPUT*/
    int* iBox_darts512 /*VARIABLE*/;
    int initIteration512;
    int lastIteration512;
    int range512;
    int rangePerCodelet512;
    int minIteration512;
    int remainderRange512;
    size_t readyCodelets;
    int baseNumThreads;
    _barrierCodelets512* barrierCodelets512;
    _checkInCodelets513* checkInCodelets513;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets513* firstCodelet;
#endif
    TP512(int in_numThreads, int in_mainCodeletID, TP510* in_TPParent, int in_initIteration,
        int in_lastIteration, real_t* in_dt, int* in_nBoxes, SimFlat** in_s,
        TP512** in_ptrToThisTP);
    void inline dispatchCodelet(size_t codeletID);
    ~TP512();
};
/*TP605: OMPParallelDirective*/
class TP605 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets605 : public darts::Codelet {
    public:
        TP605* inputsTPParent;
        _barrierCodelets605()
            : darts::Codelet()
        {
        }
        _barrierCodelets605(uint32_t dep, uint32_t res, TP605* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets607 : public darts::Codelet {
    public:
        TP605* myTP;
        TP605* inputsTPParent;
        _checkInCodelets607()
            : darts::Codelet()
        {
        }
        _checkInCodelets607(uint32_t dep, uint32_t res, TP605* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets607 : public darts::Codelet {
    public:
        TP605* inputsTPParent;
        _barrierCodelets607()
            : darts::Codelet()
        {
        }
        _barrierCodelets607(uint32_t dep, uint32_t res, TP605* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP605* TPParent;
    TP605* controlTPParent;
    TP605* inputsTPParent;
    real_t* kenergy_darts605; /*OMP_SHARED - INPUT*/
    SimFlat** s_darts605; /*OMP_SHARED - INPUT*/
    TP607** TP607Ptr;
    size_t* TP607_alreadyLaunched;
    int numTPsSet607;
    int numTPsReady607;
    size_t TPsToUse607;
    size_t codeletsPerTP607;
    size_t totalCodelets607;
    std::mutex kenergy_darts607_mutex;
    _barrierCodelets605* barrierCodelets605;
    _checkInCodelets607* checkInCodelets607;
    _barrierCodelets607* barrierCodelets607;
    TP605(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet,
        real_t* in_kenergy, SimFlat** in_s);
    ~TP605();
};
/*TP607: OMPForDirective*/
class TP607 : public ompTP {
public:
    class _barrierCodelets607 : public darts::Codelet {
    public:
        TP607* inputsTPParent;
        _barrierCodelets607()
            : darts::Codelet()
        {
        }
        _barrierCodelets607(uint32_t dep, uint32_t res, TP607* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations607(int* endRange, uint32_t codeletID);
    class _checkInCodelets608 : public darts::Codelet {
    public:
        TP607* myTP;
        TP607* inputsTPParent;
        int endRange;
        _checkInCodelets608()
            : darts::Codelet()
        {
        }
        _checkInCodelets608(uint32_t dep, uint32_t res, TP607* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP605* TPParent;
    TP607* controlTPParent;
    TP607* inputsTPParent;
    real_t* kenergy_darts607; /*OMP_SHARED - INPUT*/
    SimFlat** s_darts607; /*OMP_SHARED - INPUT*/
    int* iBox_darts607 /*VARIABLE*/;
    int initIteration607;
    int lastIteration607;
    int range607;
    int rangePerCodelet607;
    int minIteration607;
    int remainderRange607;
    size_t readyCodelets;
    int baseNumThreads;
    std::mutex* kenergy_darts607_mutex;
    _barrierCodelets607* barrierCodelets607;
    _checkInCodelets608* checkInCodelets608;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets608* firstCodelet;
#endif
    TP607(int in_numThreads, int in_mainCodeletID, TP605* in_TPParent, int in_initIteration,
        int in_lastIteration, real_t* in_kenergy, SimFlat** in_s, TP607** in_ptrToThisTP);
    void inline dispatchCodelet(size_t codeletID);
    ~TP607();
};
/*TP723: OMPParallelDirective*/
class TP723 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets723 : public darts::Codelet {
    public:
        TP723* inputsTPParent;
        _barrierCodelets723()
            : darts::Codelet()
        {
        }
        _barrierCodelets723(uint32_t dep, uint32_t res, TP723* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets725 : public darts::Codelet {
    public:
        TP723* myTP;
        TP723* inputsTPParent;
        _checkInCodelets725()
            : darts::Codelet()
        {
        }
        _checkInCodelets725(uint32_t dep, uint32_t res, TP723* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets725 : public darts::Codelet {
    public:
        TP723* inputsTPParent;
        _barrierCodelets725()
            : darts::Codelet()
        {
        }
        _barrierCodelets725(uint32_t dep, uint32_t res, TP723* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP723* TPParent;
    TP723* controlTPParent;
    TP723* inputsTPParent;
    SimFlat** sim_darts723; /*OMP_SHARED - INPUT*/
    TP725** TP725Ptr;
    size_t* TP725_alreadyLaunched;
    int numTPsSet725;
    int numTPsReady725;
    size_t TPsToUse725;
    size_t codeletsPerTP725;
    size_t totalCodelets725;
    _barrierCodelets723* barrierCodelets723;
    _checkInCodelets725* checkInCodelets725;
    _barrierCodelets725* barrierCodelets725;
    TP723(
        int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet, SimFlat** in_sim);
    ~TP723();
};
/*TP725: OMPForDirective*/
class TP725 : public ompTP {
public:
    class _barrierCodelets725 : public darts::Codelet {
    public:
        TP725* inputsTPParent;
        _barrierCodelets725()
            : darts::Codelet()
        {
        }
        _barrierCodelets725(uint32_t dep, uint32_t res, TP725* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations725(int* endRange, uint32_t codeletID);
    class _checkInCodelets726 : public darts::Codelet {
    public:
        TP725* myTP;
        TP725* inputsTPParent;
        int endRange;
        _checkInCodelets726()
            : darts::Codelet()
        {
        }
        _checkInCodelets726(uint32_t dep, uint32_t res, TP725* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP723* TPParent;
    TP725* controlTPParent;
    TP725* inputsTPParent;
    SimFlat** sim_darts725; /*OMP_SHARED - INPUT*/
    int* ii_darts725 /*VARIABLE*/;
    int initIteration725;
    int lastIteration725;
    int range725;
    int rangePerCodelet725;
    int minIteration725;
    int remainderRange725;
    size_t readyCodelets;
    int baseNumThreads;
    _barrierCodelets725* barrierCodelets725;
    _checkInCodelets726* checkInCodelets726;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets726* firstCodelet;
#endif
    TP725(int in_numThreads, int in_mainCodeletID, TP723* in_TPParent, int in_initIteration,
        int in_lastIteration, SimFlat** in_sim, TP725** in_ptrToThisTP);
    void inline dispatchCodelet(size_t codeletID);
    ~TP725();
};
#endif
