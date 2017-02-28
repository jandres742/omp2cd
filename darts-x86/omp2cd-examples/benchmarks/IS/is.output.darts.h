#ifndef _is_output_darts_h_
#define _is_output_darts_h_
#ifndef __DARTS_
#define __DARTS_
#endif
#include "darts.h"
#include "is.h"
#include "ompTP.h"
#include "tbb/concurrent_vector.h"
#include "utils.h"
#include <limits.h>
#include <mutex>
#include <numa.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
void full_verify();
void rank_is(int iteration);
void alloc_key_buff();
void* alloc_mem(size_t size);
void create_seq(double seed, double a);
double find_my_seed(int kn, int np, long nn, double s, double a);
int main(int argc, char** argv);
double randlc(double* X, double* A);
class TP183;
class TP268;
class TP303;
class TP352;
class TP416;
class TP432;
/*Number of TPs to be used for the OMPFor in region TP432*/
#define NUMTPS432 NUMTPS
class TP502;
/*Number of TPs to be used for the OMPFor in region TP502*/
#define NUMTPS502 NUMTPS
class TP555;
/*Number of TPs to be used for the OMPFor in region TP555*/
#define NUMTPS555 NUMTPS
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
/*TP183: OMPParallelDirective*/
class TP183 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets183 : public darts::Codelet {
    public:
        TP183* inputsTPParent;
        _barrierCodelets183()
            : darts::Codelet()
        {
        }
        _barrierCodelets183(uint32_t dep, uint32_t res, TP183* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets185 : public darts::Codelet {
    public:
        TP183* myTP;
        TP183* inputsTPParent;
        _checkInCodelets185()
            : darts::Codelet()
        {
        }
        _checkInCodelets185(uint32_t dep, uint32_t res, TP183* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP183* TPParent;
    TP183* controlTPParent;
    TP183* inputsTPParent;
    double* a_darts183; /*OMP_SHARED - INPUT*/
    INT_TYPE* i_darts183 /*OMP_PRIVATE - INPUT*/;
    INT_TYPE* k_darts183 /*OMP_PRIVATE - INPUT*/;
    double* s_darts183 /*OMP_PRIVATE - INPUT*/;
    double* seed_darts183; /*OMP_SHARED - INPUT*/
    double* x_darts183 /*OMP_PRIVATE - INPUT*/;
    double* an_darts183 /*VARIABLE*/;
    INT_TYPE* k1_darts183 /*VARIABLE*/;
    INT_TYPE* k2_darts183 /*VARIABLE*/;
    INT_TYPE* mq_darts183 /*VARIABLE*/;
    int* myid_darts183 /*VARIABLE*/;
    int* num_procs_darts183 /*VARIABLE*/;
    _barrierCodelets183* barrierCodelets183;
    _checkInCodelets185* checkInCodelets185;
    TP183(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet, double* in_a,
        double* in_seed);
    ~TP183();
};
/*TP268: OMPParallelForDirective*/
class TP268 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets268 : public darts::Codelet {
    public:
        TP268* inputsTPParent;
        _barrierCodelets268()
            : darts::Codelet()
        {
        }
        _barrierCodelets268(uint32_t dep, uint32_t res, TP268* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations268(INT_TYPE* endRange, uint32_t codeletID);
    class _checkInCodelets269 : public darts::Codelet {
    public:
        TP268* myTP;
        TP268* inputsTPParent;
        INT_TYPE endRange;
        _checkInCodelets269()
            : darts::Codelet()
        {
        }
        _checkInCodelets269(uint32_t dep, uint32_t res, TP268* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP268* TPParent;
    TP268* controlTPParent;
    TP268* inputsTPParent;
    INT_TYPE* i_darts268 /*OMP_PRIVATE - INPUT*/;
    INT_TYPE initIteration268;
    INT_TYPE lastIteration268;
    INT_TYPE range268;
    INT_TYPE rangePerCodelet268;
    INT_TYPE minIteration268;
    INT_TYPE remainderRange268;
    _barrierCodelets268* barrierCodelets268;
    _checkInCodelets269* checkInCodelets269;
    TP268(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet,
        INT_TYPE in_initIteration, INT_TYPE in_lastIteration);
    ~TP268();
};
/*TP303: OMPParallelForDirective*/
class TP303 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets303 : public darts::Codelet {
    public:
        TP303* inputsTPParent;
        _barrierCodelets303()
            : darts::Codelet()
        {
        }
        _barrierCodelets303(uint32_t dep, uint32_t res, TP303* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations303(INT_TYPE* endRange, uint32_t codeletID);
    class _checkInCodelets304 : public darts::Codelet {
    public:
        TP303* myTP;
        TP303* inputsTPParent;
        INT_TYPE endRange;
        _checkInCodelets304()
            : darts::Codelet()
        {
        }
        _checkInCodelets304(uint32_t dep, uint32_t res, TP303* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP303* TPParent;
    TP303* controlTPParent;
    TP303* inputsTPParent;
    INT_TYPE* i_darts303 /*OMP_PRIVATE - INPUT*/;
    INT_TYPE* j_darts303 /*OMP_PRIVATE - INPUT*/;
    INT_TYPE* k_darts303 /*OMP_PRIVATE - INPUT*/;
    INT_TYPE* k1_darts303 /*OMP_PRIVATE - INPUT*/;
    INT_TYPE initIteration303;
    INT_TYPE lastIteration303;
    INT_TYPE nextIteration303;
    int loop303alreadySetUp;
    _barrierCodelets303* barrierCodelets303;
    _checkInCodelets304* checkInCodelets304;
    TP303(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet,
        INT_TYPE in_initIteration, INT_TYPE in_lastIteration);
    ~TP303();
};
/*TP352: OMPParallelForDirective*/
class TP352 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets352 : public darts::Codelet {
    public:
        TP352* inputsTPParent;
        _barrierCodelets352()
            : darts::Codelet()
        {
        }
        _barrierCodelets352(uint32_t dep, uint32_t res, TP352* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations352(INT_TYPE* endRange, uint32_t codeletID);
    class _checkInCodelets353 : public darts::Codelet {
    public:
        TP352* myTP;
        TP352* inputsTPParent;
        INT_TYPE endRange;
        _checkInCodelets353()
            : darts::Codelet()
        {
        }
        _checkInCodelets353(uint32_t dep, uint32_t res, TP352* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP352* TPParent;
    TP352* controlTPParent;
    TP352* inputsTPParent;
    INT_TYPE* i_darts352 /*OMP_PRIVATE - INPUT*/;
    INT_TYPE* j_darts352; /*OMP_SHARED - INPUT*/
    INT_TYPE initIteration352;
    INT_TYPE lastIteration352;
    INT_TYPE range352;
    INT_TYPE rangePerCodelet352;
    INT_TYPE minIteration352;
    INT_TYPE remainderRange352;
    std::mutex j_darts352_mutex;
    _barrierCodelets352* barrierCodelets352;
    _checkInCodelets353* checkInCodelets353;
    TP352(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet,
        INT_TYPE in_initIteration, INT_TYPE in_lastIteration, INT_TYPE* in_j);
    ~TP352();
};
/*TP416: OMPParallelDirective*/
class TP416 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets416 : public darts::Codelet {
    public:
        TP416* inputsTPParent;
        _barrierCodelets416()
            : darts::Codelet()
        {
        }
        _barrierCodelets416(uint32_t dep, uint32_t res, TP416* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets418 : public darts::Codelet {
    public:
        TP416* myTP;
        TP416* inputsTPParent;
        _checkInCodelets418()
            : darts::Codelet()
        {
        }
        _checkInCodelets418(uint32_t dep, uint32_t res, TP416* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets432 : public darts::Codelet {
    public:
        TP416* myTP;
        TP416* inputsTPParent;
        _checkInCodelets432()
            : darts::Codelet()
        {
        }
        _checkInCodelets432(uint32_t dep, uint32_t res, TP416* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets432 : public darts::Codelet {
    public:
        TP416* inputsTPParent;
        _barrierCodelets432()
            : darts::Codelet()
        {
        }
        _barrierCodelets432(uint32_t dep, uint32_t res, TP416* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets466 : public darts::Codelet {
    public:
        TP416* myTP;
        TP416* inputsTPParent;
        _checkInCodelets466()
            : darts::Codelet()
        {
        }
        _checkInCodelets466(uint32_t dep, uint32_t res, TP416* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets502 : public darts::Codelet {
    public:
        TP416* myTP;
        TP416* inputsTPParent;
        _checkInCodelets502()
            : darts::Codelet()
        {
        }
        _checkInCodelets502(uint32_t dep, uint32_t res, TP416* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets502 : public darts::Codelet {
    public:
        TP416* inputsTPParent;
        _barrierCodelets502()
            : darts::Codelet()
        {
        }
        _barrierCodelets502(uint32_t dep, uint32_t res, TP416* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets539 : public darts::Codelet {
    public:
        TP416* myTP;
        TP416* inputsTPParent;
        _checkInCodelets539()
            : darts::Codelet()
        {
        }
        _checkInCodelets539(uint32_t dep, uint32_t res, TP416* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets555 : public darts::Codelet {
    public:
        TP416* myTP;
        TP416* inputsTPParent;
        _checkInCodelets555()
            : darts::Codelet()
        {
        }
        _checkInCodelets555(uint32_t dep, uint32_t res, TP416* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets555 : public darts::Codelet {
    public:
        TP416* inputsTPParent;
        _barrierCodelets555()
            : darts::Codelet()
        {
        }
        _barrierCodelets555(uint32_t dep, uint32_t res, TP416* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP416* TPParent;
    TP416* controlTPParent;
    TP416* inputsTPParent;
    INT_TYPE* i_darts416 /*OMP_PRIVATE - INPUT*/;
    INT_TYPE* k_darts416 /*OMP_PRIVATE - INPUT*/;
    INT_TYPE** key_buff_ptr_darts416; /*OMP_SHARED - INPUT*/
    INT_TYPE** key_buff_ptr2_darts416; /*OMP_SHARED - INPUT*/
    INT_TYPE* num_bucket_keys_darts416; /*OMP_SHARED - INPUT*/
    int* shift_darts416; /*OMP_SHARED - INPUT*/
    INT_TYPE* k1_darts416 /*VARIABLE*/;
    INT_TYPE* k2_darts416 /*VARIABLE*/;
    INT_TYPE* m_darts416 /*VARIABLE*/;
    int* myid_darts416 /*VARIABLE*/;
    int* num_procs_darts416 /*VARIABLE*/;
    INT_TYPE** work_buff_darts416 /*VARIABLE*/;
    TP432** TP432Ptr;
    size_t* TP432_alreadyLaunched;
    int numTPsSet432;
    int numTPsReady432;
    size_t TPsToUse432;
    size_t codeletsPerTP432;
    size_t totalCodelets432;
    TP502** TP502Ptr;
    size_t* TP502_alreadyLaunched;
    int numTPsSet502;
    int numTPsReady502;
    size_t TPsToUse502;
    size_t codeletsPerTP502;
    size_t totalCodelets502;
    TP555** TP555Ptr;
    size_t* TP555_alreadyLaunched;
    int numTPsSet555;
    int numTPsReady555;
    size_t TPsToUse555;
    size_t codeletsPerTP555;
    size_t totalCodelets555;
    _barrierCodelets416* barrierCodelets416;
    _checkInCodelets418* checkInCodelets418;
    _checkInCodelets432* checkInCodelets432;
    _barrierCodelets432* barrierCodelets432;
    _checkInCodelets466* checkInCodelets466;
    _checkInCodelets502* checkInCodelets502;
    _barrierCodelets502* barrierCodelets502;
    _checkInCodelets539* checkInCodelets539;
    _checkInCodelets555* checkInCodelets555;
    _barrierCodelets555* barrierCodelets555;
    TP416(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet,
        INT_TYPE** in_key_buff_ptr, INT_TYPE** in_key_buff_ptr2, INT_TYPE* in_num_bucket_keys,
        int* in_shift);
    ~TP416();
};
/*TP432: OMPForDirective*/
class TP432 : public ompTP {
public:
    class _barrierCodelets432 : public darts::Codelet {
    public:
        TP432* inputsTPParent;
        _barrierCodelets432()
            : darts::Codelet()
        {
        }
        _barrierCodelets432(uint32_t dep, uint32_t res, TP432* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations432(INT_TYPE* endRange, uint32_t codeletID);
    class _checkInCodelets433 : public darts::Codelet {
    public:
        TP432* myTP;
        TP432* inputsTPParent;
        INT_TYPE endRange;
        _checkInCodelets433()
            : darts::Codelet()
        {
        }
        _checkInCodelets433(uint32_t dep, uint32_t res, TP432* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP416* TPParent;
    TP432* controlTPParent;
    TP432* inputsTPParent;
    INT_TYPE* i_darts432 /*OMP_PRIVATE - INPUT*/;
    int* shift_darts432; /*OMP_SHARED - INPUT*/
    INT_TYPE*** work_buff_darts432 /*OMP_SHARED_PRIVATE - INPUT*/;
    INT_TYPE initIteration432;
    INT_TYPE lastIteration432;
    INT_TYPE range432;
    INT_TYPE rangePerCodelet432;
    INT_TYPE minIteration432;
    INT_TYPE remainderRange432;
    size_t readyCodelets;
    int baseNumThreads;
    _barrierCodelets432* barrierCodelets432;
    _checkInCodelets433* checkInCodelets433;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets433* firstCodelet;
#endif
    TP432(int in_numThreads, int in_mainCodeletID, TP416* in_TPParent, INT_TYPE in_initIteration,
        INT_TYPE in_lastIteration, int* in_shift, TP432** in_ptrToThisTP);
    void inline dispatchCodelet(size_t codeletID);
    ~TP432();
};
/*TP502: OMPForDirective*/
class TP502 : public ompTP {
public:
    class _barrierCodelets502 : public darts::Codelet {
    public:
        TP502* inputsTPParent;
        _barrierCodelets502()
            : darts::Codelet()
        {
        }
        _barrierCodelets502(uint32_t dep, uint32_t res, TP502* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations502(INT_TYPE* endRange, uint32_t codeletID);
    class _checkInCodelets503 : public darts::Codelet {
    public:
        TP502* myTP;
        TP502* inputsTPParent;
        INT_TYPE endRange;
        _checkInCodelets503()
            : darts::Codelet()
        {
        }
        _checkInCodelets503(uint32_t dep, uint32_t res, TP502* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP416* TPParent;
    TP502* controlTPParent;
    TP502* inputsTPParent;
    INT_TYPE* i_darts502 /*OMP_PRIVATE - INPUT*/;
    INT_TYPE** k_darts502 /*OMP_SHARED_PRIVATE - INPUT*/;
    int* shift_darts502; /*OMP_SHARED - INPUT*/
    INT_TYPE initIteration502;
    INT_TYPE lastIteration502;
    INT_TYPE range502;
    INT_TYPE rangePerCodelet502;
    INT_TYPE minIteration502;
    INT_TYPE remainderRange502;
    size_t readyCodelets;
    int baseNumThreads;
    _barrierCodelets502* barrierCodelets502;
    _checkInCodelets503* checkInCodelets503;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets503* firstCodelet;
#endif
    TP502(int in_numThreads, int in_mainCodeletID, TP416* in_TPParent, INT_TYPE in_initIteration,
        INT_TYPE in_lastIteration, int* in_shift, TP502** in_ptrToThisTP);
    void inline dispatchCodelet(size_t codeletID);
    ~TP502();
};
/*TP555: OMPForDirective*/
class TP555 : public ompTP {
public:
    class _barrierCodelets555 : public darts::Codelet {
    public:
        TP555* inputsTPParent;
        _barrierCodelets555()
            : darts::Codelet()
        {
        }
        _barrierCodelets555(uint32_t dep, uint32_t res, TP555* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations555(INT_TYPE* endRange, uint32_t codeletID);
    class _checkInCodelets556 : public darts::Codelet {
    public:
        TP555* myTP;
        TP555* inputsTPParent;
        INT_TYPE endRange;
        _checkInCodelets556()
            : darts::Codelet()
        {
        }
        _checkInCodelets556(uint32_t dep, uint32_t res, TP555* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP416* TPParent;
    TP555* controlTPParent;
    TP555* inputsTPParent;
    INT_TYPE* i_darts555 /*OMP_PRIVATE - INPUT*/;
    INT_TYPE* k_darts555 /*OMP_PRIVATE - INPUT*/;
    INT_TYPE** k1_darts555 /*OMP_SHARED_PRIVATE - INPUT*/;
    INT_TYPE** k2_darts555 /*OMP_SHARED_PRIVATE - INPUT*/;
    INT_TYPE** key_buff_ptr_darts555; /*OMP_SHARED - INPUT*/
    INT_TYPE** key_buff_ptr2_darts555; /*OMP_SHARED - INPUT*/
    INT_TYPE** m_darts555 /*OMP_SHARED_PRIVATE - INPUT*/;
    INT_TYPE* num_bucket_keys_darts555; /*OMP_SHARED - INPUT*/
    INT_TYPE initIteration555;
    INT_TYPE lastIteration555;
    INT_TYPE nextIteration555;
    int loop555alreadySetUp;
    size_t readyCodelets;
    int baseNumThreads;
    _barrierCodelets555* barrierCodelets555;
    _checkInCodelets556* checkInCodelets556;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets556* firstCodelet;
#endif
    TP555(int in_numThreads, int in_mainCodeletID, TP416* in_TPParent, INT_TYPE in_initIteration,
        INT_TYPE in_lastIteration, INT_TYPE** in_key_buff_ptr, INT_TYPE** in_key_buff_ptr2,
        INT_TYPE* in_num_bucket_keys, TP555** in_ptrToThisTP);
    void inline dispatchCodelet(size_t codeletID);
    ~TP555();
};
#endif
