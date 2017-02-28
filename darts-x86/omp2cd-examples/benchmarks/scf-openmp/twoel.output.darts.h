#ifndef _twoel_output_darts_h_
#define _twoel_output_darts_h_
#ifndef __DARTS_
#define __DARTS_
#endif
#include "darts.h"
#include "ompTP.h"
#include "tbb/concurrent_vector.h"
#include "twoel.h"
#include "utils.h"
#include <limits.h>
#include <mutex>
#include <numa.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
void twoel_i_eq_j_eq_k_eq_l(double tol2e_over_schwmax);
double twoel_fast(double schwmax);
void twoel_i_eq_j_and_k_eq_l(double tol2e_over_schwmax);
void twoel_ij_eq_kl(double tol2e_over_schwmax);
void twoel_k_eq_l(double tol2e_over_schwmax);
void twoel_i_eq_j(double tol2e_over_schwmax);
void twoel_i_j_k_l_all_different(double tol2e_over_schwmax);
class TP98;
class TP106;
/*Number of TPs to be used for the OMPFor in region TP106*/
#define NUMTPS106 NUMTPS
class TP276;
/*Number of TPs to be used for the OMPFor in region TP276*/
#define NUMTPS276 NUMTPS
class TP325;
/*Number of TPs to be used for the OMPFor in region TP325*/
#define NUMTPS325 NUMTPS
class TP406;
class TP414;
/*Number of TPs to be used for the OMPFor in region TP414*/
#define NUMTPS414 NUMTPS
class TP540;
class TP548;
/*Number of TPs to be used for the OMPFor in region TP548*/
#define NUMTPS548 NUMTPS
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
/*TP98: OMPParallelDirective*/
class TP98 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets98 : public darts::Codelet {
    public:
        TP98* inputsTPParent;
        _barrierCodelets98()
            : darts::Codelet()
        {
        }
        _barrierCodelets98(uint32_t dep, uint32_t res, TP98* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets100 : public darts::Codelet {
    public:
        TP98* myTP;
        TP98* inputsTPParent;
        _checkInCodelets100()
            : darts::Codelet()
        {
        }
        _checkInCodelets100(uint32_t dep, uint32_t res, TP98* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets106 : public darts::Codelet {
    public:
        TP98* myTP;
        TP98* inputsTPParent;
        _checkInCodelets106()
            : darts::Codelet()
        {
        }
        _checkInCodelets106(uint32_t dep, uint32_t res, TP98* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets106 : public darts::Codelet {
    public:
        TP98* inputsTPParent;
        _barrierCodelets106()
            : darts::Codelet()
        {
        }
        _barrierCodelets106(uint32_t dep, uint32_t res, TP98* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets271 : public darts::Codelet {
    public:
        TP98* myTP;
        TP98* inputsTPParent;
        _checkInCodelets271()
            : darts::Codelet()
        {
        }
        _checkInCodelets271(uint32_t dep, uint32_t res, TP98* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets276 : public darts::Codelet {
    public:
        TP98* myTP;
        TP98* inputsTPParent;
        _checkInCodelets276()
            : darts::Codelet()
        {
        }
        _checkInCodelets276(uint32_t dep, uint32_t res, TP98* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets276 : public darts::Codelet {
    public:
        TP98* inputsTPParent;
        _barrierCodelets276()
            : darts::Codelet()
        {
        }
        _barrierCodelets276(uint32_t dep, uint32_t res, TP98* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets325 : public darts::Codelet {
    public:
        TP98* myTP;
        TP98* inputsTPParent;
        _checkInCodelets325()
            : darts::Codelet()
        {
        }
        _checkInCodelets325(uint32_t dep, uint32_t res, TP98* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets325 : public darts::Codelet {
    public:
        TP98* inputsTPParent;
        _barrierCodelets325()
            : darts::Codelet()
        {
        }
        _barrierCodelets325(uint32_t dep, uint32_t res, TP98* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP98* TPParent;
    TP98* controlTPParent;
    TP98* inputsTPParent;
    long long* temp_icut1_darts98; /*OMP_SHARED - INPUT*/
    long long* temp_icut2_darts98; /*OMP_SHARED - INPUT*/
    long long* temp_icut3_darts98; /*OMP_SHARED - INPUT*/
    long long* temp_icut4_darts98; /*OMP_SHARED - INPUT*/
    double* tol2e_over_schwmax_darts98; /*OMP_SHARED - INPUT*/
    double** fock_darts98 /*VARIABLE*/;
    double** fock_2_darts98 /*VARIABLE*/;
    long long* nbfn_squared_darts98 /*VARIABLE*/;
    int* numthreads_darts98 /*VARIABLE*/;
    TP106** TP106Ptr;
    size_t* TP106_alreadyLaunched;
    int numTPsSet106;
    int numTPsReady106;
    size_t TPsToUse106;
    size_t codeletsPerTP106;
    size_t totalCodelets106;
    std::mutex temp_icut3_darts106_mutex;
    std::mutex temp_icut4_darts106_mutex;
    std::mutex temp_icut2_darts106_mutex;
    std::mutex temp_icut1_darts106_mutex;
    TP276** TP276Ptr;
    size_t* TP276_alreadyLaunched;
    int numTPsSet276;
    int numTPsReady276;
    size_t TPsToUse276;
    size_t codeletsPerTP276;
    size_t totalCodelets276;
    TP325** TP325Ptr;
    size_t* TP325_alreadyLaunched;
    int numTPsSet325;
    int numTPsReady325;
    size_t TPsToUse325;
    size_t codeletsPerTP325;
    size_t totalCodelets325;
    _barrierCodelets98* barrierCodelets98;
    _checkInCodelets100* checkInCodelets100;
    _checkInCodelets106* checkInCodelets106;
    _barrierCodelets106* barrierCodelets106;
    _checkInCodelets271* checkInCodelets271;
    _checkInCodelets276* checkInCodelets276;
    _barrierCodelets276* barrierCodelets276;
    _checkInCodelets325* checkInCodelets325;
    _barrierCodelets325* barrierCodelets325;
    TP98(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet,
        long long* in_temp_icut1, long long* in_temp_icut2, long long* in_temp_icut3,
        long long* in_temp_icut4, double* in_tol2e_over_schwmax);
    ~TP98();
};
/*TP106: OMPForDirective*/
class TP106 : public ompTP {
public:
    class _barrierCodelets106 : public darts::Codelet {
    public:
        TP106* inputsTPParent;
        _barrierCodelets106()
            : darts::Codelet()
        {
        }
        _barrierCodelets106(uint32_t dep, uint32_t res, TP106* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations106(int* endRange, uint32_t codeletID);
    class _checkInCodelets107 : public darts::Codelet {
    public:
        TP106* myTP;
        TP106* inputsTPParent;
        int endRange;
        _checkInCodelets107()
            : darts::Codelet()
        {
        }
        _checkInCodelets107(uint32_t dep, uint32_t res, TP106* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP98* TPParent;
    TP106* controlTPParent;
    TP106* inputsTPParent;
    double*** fock_darts106 /*OMP_SHARED_PRIVATE - INPUT*/;
    long long* temp_icut1_darts106; /*OMP_SHARED - INPUT*/
    long long* temp_icut2_darts106; /*OMP_SHARED - INPUT*/
    long long* temp_icut3_darts106; /*OMP_SHARED - INPUT*/
    long long* temp_icut4_darts106; /*OMP_SHARED - INPUT*/
    double* tol2e_over_schwmax_darts106; /*OMP_SHARED - INPUT*/
    int* i_darts106 /*VARIABLE*/;
    int initIteration106;
    int lastIteration106;
    size_t ompLoopSched106;
    size_t ompLoopChunk106;
    int range106;
    int rangePerCodelet106;
    int minIteration106;
    int remainderRange106;
    int nextIteration106;
    int loop106alreadySetUp;
    size_t readyCodelets;
    int baseNumThreads;
    std::mutex* temp_icut3_darts106_mutex;
    std::mutex* temp_icut4_darts106_mutex;
    std::mutex* temp_icut2_darts106_mutex;
    std::mutex* temp_icut1_darts106_mutex;
    _barrierCodelets106* barrierCodelets106;
    _checkInCodelets107* checkInCodelets107;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets107* firstCodelet;
#endif
    TP106(int in_numThreads, int in_mainCodeletID, TP98* in_TPParent, int in_initIteration,
        int in_lastIteration, long long* in_temp_icut1, long long* in_temp_icut2,
        long long* in_temp_icut3, long long* in_temp_icut4, double* in_tol2e_over_schwmax,
        TP106** in_ptrToThisTP);
    void inline dispatchCodelet(size_t codeletID);
    ~TP106();
};
/*TP276: OMPForDirective*/
class TP276 : public ompTP {
public:
    class _barrierCodelets276 : public darts::Codelet {
    public:
        TP276* inputsTPParent;
        _barrierCodelets276()
            : darts::Codelet()
        {
        }
        _barrierCodelets276(uint32_t dep, uint32_t res, TP276* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations276(int* endRange, uint32_t codeletID);
    class _checkInCodelets277 : public darts::Codelet {
    public:
        TP276* myTP;
        TP276* inputsTPParent;
        int endRange;
        _checkInCodelets277()
            : darts::Codelet()
        {
        }
        _checkInCodelets277(uint32_t dep, uint32_t res, TP276* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP98* TPParent;
    TP276* controlTPParent;
    TP276* inputsTPParent;
    double*** fock_2_darts276 /*OMP_SHARED_PRIVATE - INPUT*/;
    long long** nbfn_squared_darts276 /*OMP_SHARED_PRIVATE - INPUT*/;
    int** numthreads_darts276 /*OMP_SHARED_PRIVATE - INPUT*/;
    int* i_darts276 /*VARIABLE*/;
    int initIteration276;
    int lastIteration276;
    int range276;
    int rangePerCodelet276;
    int minIteration276;
    int remainderRange276;
    size_t readyCodelets;
    int baseNumThreads;
    _barrierCodelets276* barrierCodelets276;
    _checkInCodelets277* checkInCodelets277;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets277* firstCodelet;
#endif
    TP276(int in_numThreads, int in_mainCodeletID, TP98* in_TPParent, int in_initIteration,
        int in_lastIteration, TP276** in_ptrToThisTP);
    void inline dispatchCodelet(size_t codeletID);
    ~TP276();
};
/*TP325: OMPForDirective*/
class TP325 : public ompTP {
public:
    class _barrierCodelets325 : public darts::Codelet {
    public:
        TP325* inputsTPParent;
        _barrierCodelets325()
            : darts::Codelet()
        {
        }
        _barrierCodelets325(uint32_t dep, uint32_t res, TP325* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations325(int* endRange, uint32_t codeletID);
    class _checkInCodelets326 : public darts::Codelet {
    public:
        TP325* myTP;
        TP325* inputsTPParent;
        int endRange;
        _checkInCodelets326()
            : darts::Codelet()
        {
        }
        _checkInCodelets326(uint32_t dep, uint32_t res, TP325* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP98* TPParent;
    TP325* controlTPParent;
    TP325* inputsTPParent;
    int* i_darts325 /*VARIABLE*/;
    int initIteration325;
    int lastIteration325;
    size_t ompLoopSched325;
    size_t ompLoopChunk325;
    int range325;
    int rangePerCodelet325;
    int minIteration325;
    int remainderRange325;
    int nextIteration325;
    int loop325alreadySetUp;
    size_t readyCodelets;
    int baseNumThreads;
    _barrierCodelets325* barrierCodelets325;
    _checkInCodelets326* checkInCodelets326;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets326* firstCodelet;
#endif
    TP325(int in_numThreads, int in_mainCodeletID, TP98* in_TPParent, int in_initIteration,
        int in_lastIteration, TP325** in_ptrToThisTP);
    void inline dispatchCodelet(size_t codeletID);
    ~TP325();
};
/*TP406: OMPParallelDirective*/
class TP406 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets406 : public darts::Codelet {
    public:
        TP406* inputsTPParent;
        _barrierCodelets406()
            : darts::Codelet()
        {
        }
        _barrierCodelets406(uint32_t dep, uint32_t res, TP406* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets408 : public darts::Codelet {
    public:
        TP406* myTP;
        TP406* inputsTPParent;
        _checkInCodelets408()
            : darts::Codelet()
        {
        }
        _checkInCodelets408(uint32_t dep, uint32_t res, TP406* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets414 : public darts::Codelet {
    public:
        TP406* myTP;
        TP406* inputsTPParent;
        _checkInCodelets414()
            : darts::Codelet()
        {
        }
        _checkInCodelets414(uint32_t dep, uint32_t res, TP406* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP406* TPParent;
    TP406* controlTPParent;
    TP406* inputsTPParent;
    long long* temp_icut1_darts406; /*OMP_SHARED - INPUT*/
    long long* temp_icut2_darts406; /*OMP_SHARED - INPUT*/
    long long* temp_icut3_darts406; /*OMP_SHARED - INPUT*/
    long long* temp_icut4_darts406; /*OMP_SHARED - INPUT*/
    double* tol2e_over_schwmax_darts406; /*OMP_SHARED - INPUT*/
    double** fock_darts406 /*VARIABLE*/;
    TP414** TP414Ptr;
    size_t* TP414_alreadyLaunched;
    int numTPsSet414;
    int numTPsReady414;
    size_t TPsToUse414;
    size_t codeletsPerTP414;
    size_t totalCodelets414;
    std::mutex temp_icut3_darts414_mutex;
    std::mutex temp_icut4_darts414_mutex;
    std::mutex temp_icut2_darts414_mutex;
    std::mutex temp_icut1_darts414_mutex;
    _barrierCodelets406* barrierCodelets406;
    _checkInCodelets408* checkInCodelets408;
    _checkInCodelets414* checkInCodelets414;
    TP406(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet,
        long long* in_temp_icut1, long long* in_temp_icut2, long long* in_temp_icut3,
        long long* in_temp_icut4, double* in_tol2e_over_schwmax);
    ~TP406();
};
/*TP414: OMPForDirective*/
class TP414 : public ompTP {
public:
    bool requestNewRangeIterations414(int* endRange, uint32_t codeletID);
    class _checkInCodelets415 : public darts::Codelet {
    public:
        TP414* myTP;
        TP414* inputsTPParent;
        int endRange;
        _checkInCodelets415()
            : darts::Codelet()
        {
        }
        _checkInCodelets415(uint32_t dep, uint32_t res, TP414* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP406* TPParent;
    TP414* controlTPParent;
    TP414* inputsTPParent;
    double*** fock_darts414 /*OMP_SHARED_PRIVATE - INPUT*/;
    long long* temp_icut1_darts414; /*OMP_SHARED - INPUT*/
    long long* temp_icut2_darts414; /*OMP_SHARED - INPUT*/
    long long* temp_icut3_darts414; /*OMP_SHARED - INPUT*/
    long long* temp_icut4_darts414; /*OMP_SHARED - INPUT*/
    double* tol2e_over_schwmax_darts414; /*OMP_SHARED - INPUT*/
    int* i_darts414 /*VARIABLE*/;
    int initIteration414;
    int lastIteration414;
    size_t ompLoopSched414;
    size_t ompLoopChunk414;
    int range414;
    int rangePerCodelet414;
    int minIteration414;
    int remainderRange414;
    int nextIteration414;
    int loop414alreadySetUp;
    size_t readyCodelets;
    int baseNumThreads;
    int* signalNextReady;
    std::mutex* temp_icut3_darts414_mutex;
    std::mutex* temp_icut4_darts414_mutex;
    std::mutex* temp_icut2_darts414_mutex;
    std::mutex* temp_icut1_darts414_mutex;
    _checkInCodelets415* checkInCodelets415;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets415* firstCodelet;
#endif
    TP414(int in_numThreads, int in_mainCodeletID, TP406* in_TPParent, int in_initIteration,
        int in_lastIteration, long long* in_temp_icut1, long long* in_temp_icut2,
        long long* in_temp_icut3, long long* in_temp_icut4, double* in_tol2e_over_schwmax,
        TP414** in_ptrToThisTP);
    void inline dispatchCodelet(size_t codeletID);
    ~TP414();
};
/*TP540: OMPParallelDirective*/
class TP540 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets540 : public darts::Codelet {
    public:
        TP540* inputsTPParent;
        _barrierCodelets540()
            : darts::Codelet()
        {
        }
        _barrierCodelets540(uint32_t dep, uint32_t res, TP540* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets542 : public darts::Codelet {
    public:
        TP540* myTP;
        TP540* inputsTPParent;
        _checkInCodelets542()
            : darts::Codelet()
        {
        }
        _checkInCodelets542(uint32_t dep, uint32_t res, TP540* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets548 : public darts::Codelet {
    public:
        TP540* myTP;
        TP540* inputsTPParent;
        _checkInCodelets548()
            : darts::Codelet()
        {
        }
        _checkInCodelets548(uint32_t dep, uint32_t res, TP540* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP540* TPParent;
    TP540* controlTPParent;
    TP540* inputsTPParent;
    long long* temp_icut1_darts540; /*OMP_SHARED - INPUT*/
    long long* temp_icut2_darts540; /*OMP_SHARED - INPUT*/
    long long* temp_icut3_darts540; /*OMP_SHARED - INPUT*/
    long long* temp_icut4_darts540; /*OMP_SHARED - INPUT*/
    double* tol2e_over_schwmax_darts540; /*OMP_SHARED - INPUT*/
    double** fock_darts540 /*VARIABLE*/;
    TP548** TP548Ptr;
    size_t* TP548_alreadyLaunched;
    int numTPsSet548;
    int numTPsReady548;
    size_t TPsToUse548;
    size_t codeletsPerTP548;
    size_t totalCodelets548;
    std::mutex temp_icut3_darts548_mutex;
    std::mutex temp_icut4_darts548_mutex;
    std::mutex temp_icut2_darts548_mutex;
    std::mutex temp_icut1_darts548_mutex;
    _barrierCodelets540* barrierCodelets540;
    _checkInCodelets542* checkInCodelets542;
    _checkInCodelets548* checkInCodelets548;
    TP540(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet,
        long long* in_temp_icut1, long long* in_temp_icut2, long long* in_temp_icut3,
        long long* in_temp_icut4, double* in_tol2e_over_schwmax);
    ~TP540();
};
/*TP548: OMPForDirective*/
class TP548 : public ompTP {
public:
    bool requestNewRangeIterations548(int* endRange, uint32_t codeletID);
    class _checkInCodelets549 : public darts::Codelet {
    public:
        TP548* myTP;
        TP548* inputsTPParent;
        int endRange;
        _checkInCodelets549()
            : darts::Codelet()
        {
        }
        _checkInCodelets549(uint32_t dep, uint32_t res, TP548* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP540* TPParent;
    TP548* controlTPParent;
    TP548* inputsTPParent;
    double*** fock_darts548 /*OMP_SHARED_PRIVATE - INPUT*/;
    long long* temp_icut1_darts548; /*OMP_SHARED - INPUT*/
    long long* temp_icut2_darts548; /*OMP_SHARED - INPUT*/
    long long* temp_icut3_darts548; /*OMP_SHARED - INPUT*/
    long long* temp_icut4_darts548; /*OMP_SHARED - INPUT*/
    double* tol2e_over_schwmax_darts548; /*OMP_SHARED - INPUT*/
    int* i_darts548 /*VARIABLE*/;
    int initIteration548;
    int lastIteration548;
    size_t ompLoopSched548;
    size_t ompLoopChunk548;
    int range548;
    int rangePerCodelet548;
    int minIteration548;
    int remainderRange548;
    int nextIteration548;
    int loop548alreadySetUp;
    size_t readyCodelets;
    int baseNumThreads;
    int* signalNextReady;
    std::mutex* temp_icut3_darts548_mutex;
    std::mutex* temp_icut4_darts548_mutex;
    std::mutex* temp_icut2_darts548_mutex;
    std::mutex* temp_icut1_darts548_mutex;
    _checkInCodelets549* checkInCodelets549;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets549* firstCodelet;
#endif
    TP548(int in_numThreads, int in_mainCodeletID, TP540* in_TPParent, int in_initIteration,
        int in_lastIteration, long long* in_temp_icut1, long long* in_temp_icut2,
        long long* in_temp_icut3, long long* in_temp_icut4, double* in_tol2e_over_schwmax,
        TP548** in_ptrToThisTP);
    void inline dispatchCodelet(size_t codeletID);
    ~TP548();
};
#endif
