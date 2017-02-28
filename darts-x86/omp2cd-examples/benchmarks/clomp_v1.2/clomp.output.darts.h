#ifndef _clomp_output_darts_h_
#define _clomp_output_darts_h_
#ifndef __DARTS_
#define __DARTS_
#endif
#include "clomp.h"
#include "darts.h"
#include "ompTP.h"
#include "tbb/concurrent_vector.h"
#include "utils.h"
#include <limits.h>
#include <mutex>
#include <numa.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
int main(int argc, char* argv[]);
void addZone(Part* part, Zone* zone);
void do_bestcase_omp_version(long num_iterations);
void bestcase_omp_cycle(int startPidx, int endPidx, double deposit);
void bestcase_omp_module2(int startPidx, int endPidx, double deposit);
void bestcase_omp_module3(int startPidx, int endPidx, double deposit);
void bestcase_omp_module1(int startPidx, int endPidx, double deposit);
void dynamic_omp_module3();
void addPart(Part* part, long partId);
void dynamic_omp_module1();
void static_omp_cycle();
void dynamic_omp_module2();
void do_static_omp_version();
double print_timestats(const char* desc, struct timeval* start_ts, struct timeval* end_ts,
    double base_seconds, double bestcase_seconds);
void do_omp_barrier_only(long num_iterations);
void dynamic_omp_cycle();
void print_pseudocode(const char* desc, const char* pseudocode);
void dynamic_omp_module4();
void serial_ref_module2();
void print_start_message(const char* desc);
void do_dynamic_omp_version();
void reinitialize_parts();
void update_part(Part* part, double incoming_deposit);
void serial_ref_module3();
void static_omp_module3();
void bestcase_omp_module4(int startPidx, int endPidx, double deposit);
void print_data_stats(const char* desc);
long convert_to_positive_long(const char* parm_name, const char* parm_val);
void serial_ref_module4();
void print_usage();
void get_timestamp(struct timeval* ts);
void do_calc_deposit_only();
void do_manual_omp_version(long num_iterations);
void static_omp_module2();
double calc_deposit();
void serial_ref_module1();
void serial_ref_cycle();
void do_serial_ref_version();
void static_omp_module1();
void static_omp_module4();
class TP500;
class TP503;
class TP508;
class TP623;
class TP659;
class TP692;
class TP728;
class TP761;
class TP794;
class TP830;
class TP863;
class TP896;
class TP929;
class TP978;
class TP1014;
class TP1047;
class TP1083;
class TP1116;
class TP1149;
class TP1185;
class TP1218;
class TP1251;
class TP1284;
class TP31;
typedef TP31 TP_manual_omp_module1;
class TP32;
typedef TP32 TP_manual_omp_module2;
class TP33;
typedef TP33 TP_manual_omp_module3;
class TP34;
typedef TP34 TP_manual_omp_module4;
class TP1451;
class TP1490;
class TP1579;
class TP1860;
class TP1897;
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
/*TP500: OMPParallelDirective*/
class TP500 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets500 : public darts::Codelet {
    public:
        TP500* inputsTPParent;
        _barrierCodelets500()
            : darts::Codelet()
        {
        }
        _barrierCodelets500(uint32_t dep, uint32_t res, TP500* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets502 : public darts::Codelet {
    public:
        TP500* myTP;
        TP500* inputsTPParent;
        _checkInCodelets502()
            : darts::Codelet()
        {
        }
        _checkInCodelets502(uint32_t dep, uint32_t res, TP500* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets503 : public darts::Codelet {
    public:
        TP500* myTP;
        TP500* inputsTPParent;
        _checkInCodelets503()
            : darts::Codelet()
        {
        }
        _checkInCodelets503(uint32_t dep, uint32_t res, TP500* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets506 : public darts::Codelet {
    public:
        TP500* myTP;
        TP500* inputsTPParent;
        _checkInCodelets506()
            : darts::Codelet()
        {
        }
        _checkInCodelets506(uint32_t dep, uint32_t res, TP500* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP500* TPParent;
    TP500* controlTPParent;
    TP500* inputsTPParent;
    long* num_iterations_darts500; /*OMP_SHARED - INPUT*/
    long* iteration_darts500 /*VARIABLE*/;
    long* subcycle_darts500 /*VARIABLE*/;
    unsigned int TP503_LoopCounter;
    unsigned int* TP503_LoopCounterPerThread;
    tbb::concurrent_vector<TP503*> TP503PtrVec;
    _barrierCodelets500* barrierCodelets500;
    _checkInCodelets502* checkInCodelets502;
    _checkInCodelets503* checkInCodelets503;
    _checkInCodelets506* checkInCodelets506;
    TP500(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet,
        long* in_num_iterations);
    ~TP500();
};
/*TP503: ForStmt*/
class TP503 : public ompTP {
public:
    class _checkInCodelets509 : public darts::Codelet {
    public:
        TP503* myTP;
        TP500* inputsTPParent;
        _checkInCodelets509()
            : darts::Codelet()
        {
        }
        _checkInCodelets509(uint32_t dep, uint32_t res, TP503* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets508 : public darts::Codelet {
    public:
        TP503* myTP;
        TP500* inputsTPParent;
        _checkInCodelets508()
            : darts::Codelet()
        {
        }
        _checkInCodelets508(uint32_t dep, uint32_t res, TP503* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets511 : public darts::Codelet {
    public:
        TP503* myTP;
        TP500* inputsTPParent;
        _checkInCodelets511()
            : darts::Codelet()
        {
        }
        _checkInCodelets511(uint32_t dep, uint32_t res, TP503* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP500* TPParent;
    TP503* controlTPParent;
    TP500* inputsTPParent;
    TP503** ptrToThisTP;
    unsigned int TP508_LoopCounter;
    unsigned int* TP508_LoopCounterPerThread;
    tbb::concurrent_vector<TP508*> TP508PtrVec;
    _checkInCodelets509* checkInCodelets509;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets509* firstCodelet;
#endif
    _checkInCodelets508* checkInCodelets508;
    _checkInCodelets511* checkInCodelets511;
    TP503(int in_numThreads, int in_mainCodeletID, TP500* in_TPParent, TP500* in_inputsTPParent,
        TP503** in_ptrToThisTP);
    ~TP503();
};
/*TP508: ForStmt*/
class TP508 : public ompTP {
public:
    class _checkInCodelets513 : public darts::Codelet {
    public:
        TP508* myTP;
        TP500* inputsTPParent;
        _checkInCodelets513()
            : darts::Codelet()
        {
        }
        _checkInCodelets513(uint32_t dep, uint32_t res, TP508* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets513 : public darts::Codelet {
    public:
        TP500* inputsTPParent;
        _barrierCodelets513()
            : darts::Codelet()
        {
        }
        _barrierCodelets513(uint32_t dep, uint32_t res, TP508* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP503* TPParent;
    TP508* controlTPParent;
    TP500* inputsTPParent;
    TP508** ptrToThisTP;
    _checkInCodelets513* checkInCodelets513;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets513* firstCodelet;
#endif
    _barrierCodelets513* barrierCodelets513;
    TP508(int in_numThreads, int in_mainCodeletID, TP503* in_TPParent, TP500* in_inputsTPParent,
        TP508** in_ptrToThisTP);
    ~TP508();
};
/*TP623: OMPParallelForDirective*/
class TP623 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets623 : public darts::Codelet {
    public:
        TP623* inputsTPParent;
        _barrierCodelets623()
            : darts::Codelet()
        {
        }
        _barrierCodelets623(uint32_t dep, uint32_t res, TP623* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations623(long* endRange, uint32_t codeletID);
    class _checkInCodelets624 : public darts::Codelet {
    public:
        TP623* myTP;
        TP623* inputsTPParent;
        long endRange;
        _checkInCodelets624()
            : darts::Codelet()
        {
        }
        _checkInCodelets624(uint32_t dep, uint32_t res, TP623* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP623* TPParent;
    TP623* controlTPParent;
    TP623* inputsTPParent;
    double* deposit_darts623; /*OMP_SHARED - INPUT*/
    long* pidx_darts623 /*OMP_PRIVATE - INPUT*/;
    long initIteration623;
    long lastIteration623;
    long range623;
    long rangePerCodelet623;
    long minIteration623;
    long remainderRange623;
    _barrierCodelets623* barrierCodelets623;
    _checkInCodelets624* checkInCodelets624;
    TP623(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet,
        long in_initIteration, long in_lastIteration, double* in_deposit);
    ~TP623();
};
/*TP659: OMPParallelForDirective*/
class TP659 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets659 : public darts::Codelet {
    public:
        TP659* inputsTPParent;
        _barrierCodelets659()
            : darts::Codelet()
        {
        }
        _barrierCodelets659(uint32_t dep, uint32_t res, TP659* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations659(long* endRange, uint32_t codeletID);
    class _checkInCodelets660 : public darts::Codelet {
    public:
        TP659* myTP;
        TP659* inputsTPParent;
        long endRange;
        _checkInCodelets660()
            : darts::Codelet()
        {
        }
        _checkInCodelets660(uint32_t dep, uint32_t res, TP659* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP659* TPParent;
    TP659* controlTPParent;
    TP659* inputsTPParent;
    double* deposit_darts659; /*OMP_SHARED - INPUT*/
    long* pidx_darts659 /*OMP_PRIVATE - INPUT*/;
    long initIteration659;
    long lastIteration659;
    long range659;
    long rangePerCodelet659;
    long minIteration659;
    long remainderRange659;
    _barrierCodelets659* barrierCodelets659;
    _checkInCodelets660* checkInCodelets660;
    TP659(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet,
        long in_initIteration, long in_lastIteration, double* in_deposit);
    ~TP659();
};
/*TP692: OMPParallelForDirective*/
class TP692 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets692 : public darts::Codelet {
    public:
        TP692* inputsTPParent;
        _barrierCodelets692()
            : darts::Codelet()
        {
        }
        _barrierCodelets692(uint32_t dep, uint32_t res, TP692* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations692(long* endRange, uint32_t codeletID);
    class _checkInCodelets693 : public darts::Codelet {
    public:
        TP692* myTP;
        TP692* inputsTPParent;
        long endRange;
        _checkInCodelets693()
            : darts::Codelet()
        {
        }
        _checkInCodelets693(uint32_t dep, uint32_t res, TP692* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP692* TPParent;
    TP692* controlTPParent;
    TP692* inputsTPParent;
    double* deposit_darts692; /*OMP_SHARED - INPUT*/
    long* pidx_darts692 /*OMP_PRIVATE - INPUT*/;
    long initIteration692;
    long lastIteration692;
    long range692;
    long rangePerCodelet692;
    long minIteration692;
    long remainderRange692;
    _barrierCodelets692* barrierCodelets692;
    _checkInCodelets693* checkInCodelets693;
    TP692(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet,
        long in_initIteration, long in_lastIteration, double* in_deposit);
    ~TP692();
};
/*TP728: OMPParallelForDirective*/
class TP728 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets728 : public darts::Codelet {
    public:
        TP728* inputsTPParent;
        _barrierCodelets728()
            : darts::Codelet()
        {
        }
        _barrierCodelets728(uint32_t dep, uint32_t res, TP728* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations728(long* endRange, uint32_t codeletID);
    class _checkInCodelets729 : public darts::Codelet {
    public:
        TP728* myTP;
        TP728* inputsTPParent;
        long endRange;
        _checkInCodelets729()
            : darts::Codelet()
        {
        }
        _checkInCodelets729(uint32_t dep, uint32_t res, TP728* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP728* TPParent;
    TP728* controlTPParent;
    TP728* inputsTPParent;
    double* deposit_darts728; /*OMP_SHARED - INPUT*/
    long* pidx_darts728 /*OMP_PRIVATE - INPUT*/;
    long initIteration728;
    long lastIteration728;
    long range728;
    long rangePerCodelet728;
    long minIteration728;
    long remainderRange728;
    _barrierCodelets728* barrierCodelets728;
    _checkInCodelets729* checkInCodelets729;
    TP728(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet,
        long in_initIteration, long in_lastIteration, double* in_deposit);
    ~TP728();
};
/*TP761: OMPParallelForDirective*/
class TP761 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets761 : public darts::Codelet {
    public:
        TP761* inputsTPParent;
        _barrierCodelets761()
            : darts::Codelet()
        {
        }
        _barrierCodelets761(uint32_t dep, uint32_t res, TP761* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations761(long* endRange, uint32_t codeletID);
    class _checkInCodelets762 : public darts::Codelet {
    public:
        TP761* myTP;
        TP761* inputsTPParent;
        long endRange;
        _checkInCodelets762()
            : darts::Codelet()
        {
        }
        _checkInCodelets762(uint32_t dep, uint32_t res, TP761* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP761* TPParent;
    TP761* controlTPParent;
    TP761* inputsTPParent;
    double* deposit_darts761; /*OMP_SHARED - INPUT*/
    long* pidx_darts761 /*OMP_PRIVATE - INPUT*/;
    long initIteration761;
    long lastIteration761;
    long range761;
    long rangePerCodelet761;
    long minIteration761;
    long remainderRange761;
    _barrierCodelets761* barrierCodelets761;
    _checkInCodelets762* checkInCodelets762;
    TP761(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet,
        long in_initIteration, long in_lastIteration, double* in_deposit);
    ~TP761();
};
/*TP794: OMPParallelForDirective*/
class TP794 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets794 : public darts::Codelet {
    public:
        TP794* inputsTPParent;
        _barrierCodelets794()
            : darts::Codelet()
        {
        }
        _barrierCodelets794(uint32_t dep, uint32_t res, TP794* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations794(long* endRange, uint32_t codeletID);
    class _checkInCodelets795 : public darts::Codelet {
    public:
        TP794* myTP;
        TP794* inputsTPParent;
        long endRange;
        _checkInCodelets795()
            : darts::Codelet()
        {
        }
        _checkInCodelets795(uint32_t dep, uint32_t res, TP794* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP794* TPParent;
    TP794* controlTPParent;
    TP794* inputsTPParent;
    double* deposit_darts794; /*OMP_SHARED - INPUT*/
    long* pidx_darts794 /*OMP_PRIVATE - INPUT*/;
    long initIteration794;
    long lastIteration794;
    long range794;
    long rangePerCodelet794;
    long minIteration794;
    long remainderRange794;
    _barrierCodelets794* barrierCodelets794;
    _checkInCodelets795* checkInCodelets795;
    TP794(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet,
        long in_initIteration, long in_lastIteration, double* in_deposit);
    ~TP794();
};
/*TP830: OMPParallelForDirective*/
class TP830 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets830 : public darts::Codelet {
    public:
        TP830* inputsTPParent;
        _barrierCodelets830()
            : darts::Codelet()
        {
        }
        _barrierCodelets830(uint32_t dep, uint32_t res, TP830* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations830(long* endRange, uint32_t codeletID);
    class _checkInCodelets831 : public darts::Codelet {
    public:
        TP830* myTP;
        TP830* inputsTPParent;
        long endRange;
        _checkInCodelets831()
            : darts::Codelet()
        {
        }
        _checkInCodelets831(uint32_t dep, uint32_t res, TP830* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP830* TPParent;
    TP830* controlTPParent;
    TP830* inputsTPParent;
    double* deposit_darts830; /*OMP_SHARED - INPUT*/
    long* pidx_darts830 /*OMP_PRIVATE - INPUT*/;
    long initIteration830;
    long lastIteration830;
    long range830;
    long rangePerCodelet830;
    long minIteration830;
    long remainderRange830;
    _barrierCodelets830* barrierCodelets830;
    _checkInCodelets831* checkInCodelets831;
    TP830(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet,
        long in_initIteration, long in_lastIteration, double* in_deposit);
    ~TP830();
};
/*TP863: OMPParallelForDirective*/
class TP863 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets863 : public darts::Codelet {
    public:
        TP863* inputsTPParent;
        _barrierCodelets863()
            : darts::Codelet()
        {
        }
        _barrierCodelets863(uint32_t dep, uint32_t res, TP863* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations863(long* endRange, uint32_t codeletID);
    class _checkInCodelets864 : public darts::Codelet {
    public:
        TP863* myTP;
        TP863* inputsTPParent;
        long endRange;
        _checkInCodelets864()
            : darts::Codelet()
        {
        }
        _checkInCodelets864(uint32_t dep, uint32_t res, TP863* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP863* TPParent;
    TP863* controlTPParent;
    TP863* inputsTPParent;
    double* deposit_darts863; /*OMP_SHARED - INPUT*/
    long* pidx_darts863 /*OMP_PRIVATE - INPUT*/;
    long initIteration863;
    long lastIteration863;
    long range863;
    long rangePerCodelet863;
    long minIteration863;
    long remainderRange863;
    _barrierCodelets863* barrierCodelets863;
    _checkInCodelets864* checkInCodelets864;
    TP863(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet,
        long in_initIteration, long in_lastIteration, double* in_deposit);
    ~TP863();
};
/*TP896: OMPParallelForDirective*/
class TP896 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets896 : public darts::Codelet {
    public:
        TP896* inputsTPParent;
        _barrierCodelets896()
            : darts::Codelet()
        {
        }
        _barrierCodelets896(uint32_t dep, uint32_t res, TP896* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations896(long* endRange, uint32_t codeletID);
    class _checkInCodelets897 : public darts::Codelet {
    public:
        TP896* myTP;
        TP896* inputsTPParent;
        long endRange;
        _checkInCodelets897()
            : darts::Codelet()
        {
        }
        _checkInCodelets897(uint32_t dep, uint32_t res, TP896* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP896* TPParent;
    TP896* controlTPParent;
    TP896* inputsTPParent;
    double* deposit_darts896; /*OMP_SHARED - INPUT*/
    long* pidx_darts896 /*OMP_PRIVATE - INPUT*/;
    long initIteration896;
    long lastIteration896;
    long range896;
    long rangePerCodelet896;
    long minIteration896;
    long remainderRange896;
    _barrierCodelets896* barrierCodelets896;
    _checkInCodelets897* checkInCodelets897;
    TP896(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet,
        long in_initIteration, long in_lastIteration, double* in_deposit);
    ~TP896();
};
/*TP929: OMPParallelForDirective*/
class TP929 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets929 : public darts::Codelet {
    public:
        TP929* inputsTPParent;
        _barrierCodelets929()
            : darts::Codelet()
        {
        }
        _barrierCodelets929(uint32_t dep, uint32_t res, TP929* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations929(long* endRange, uint32_t codeletID);
    class _checkInCodelets930 : public darts::Codelet {
    public:
        TP929* myTP;
        TP929* inputsTPParent;
        long endRange;
        _checkInCodelets930()
            : darts::Codelet()
        {
        }
        _checkInCodelets930(uint32_t dep, uint32_t res, TP929* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP929* TPParent;
    TP929* controlTPParent;
    TP929* inputsTPParent;
    double* deposit_darts929; /*OMP_SHARED - INPUT*/
    long* pidx_darts929 /*OMP_PRIVATE - INPUT*/;
    long initIteration929;
    long lastIteration929;
    long range929;
    long rangePerCodelet929;
    long minIteration929;
    long remainderRange929;
    _barrierCodelets929* barrierCodelets929;
    _checkInCodelets930* checkInCodelets930;
    TP929(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet,
        long in_initIteration, long in_lastIteration, double* in_deposit);
    ~TP929();
};
/*TP978: OMPParallelForDirective*/
class TP978 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets978 : public darts::Codelet {
    public:
        TP978* inputsTPParent;
        _barrierCodelets978()
            : darts::Codelet()
        {
        }
        _barrierCodelets978(uint32_t dep, uint32_t res, TP978* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations978(long* endRange, uint32_t codeletID);
    class _checkInCodelets979 : public darts::Codelet {
    public:
        TP978* myTP;
        TP978* inputsTPParent;
        long endRange;
        _checkInCodelets979()
            : darts::Codelet()
        {
        }
        _checkInCodelets979(uint32_t dep, uint32_t res, TP978* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP978* TPParent;
    TP978* controlTPParent;
    TP978* inputsTPParent;
    double* deposit_darts978; /*OMP_SHARED - INPUT*/
    long* pidx_darts978 /*OMP_PRIVATE - INPUT*/;
    long initIteration978;
    long lastIteration978;
    long nextIteration978;
    int loop978alreadySetUp;
    _barrierCodelets978* barrierCodelets978;
    _checkInCodelets979* checkInCodelets979;
    TP978(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet,
        long in_initIteration, long in_lastIteration, double* in_deposit);
    ~TP978();
};
/*TP1014: OMPParallelForDirective*/
class TP1014 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets1014 : public darts::Codelet {
    public:
        TP1014* inputsTPParent;
        _barrierCodelets1014()
            : darts::Codelet()
        {
        }
        _barrierCodelets1014(uint32_t dep, uint32_t res, TP1014* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations1014(long* endRange, uint32_t codeletID);
    class _checkInCodelets1015 : public darts::Codelet {
    public:
        TP1014* myTP;
        TP1014* inputsTPParent;
        long endRange;
        _checkInCodelets1015()
            : darts::Codelet()
        {
        }
        _checkInCodelets1015(uint32_t dep, uint32_t res, TP1014* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP1014* TPParent;
    TP1014* controlTPParent;
    TP1014* inputsTPParent;
    double* deposit_darts1014; /*OMP_SHARED - INPUT*/
    long* pidx_darts1014 /*OMP_PRIVATE - INPUT*/;
    long initIteration1014;
    long lastIteration1014;
    long nextIteration1014;
    int loop1014alreadySetUp;
    _barrierCodelets1014* barrierCodelets1014;
    _checkInCodelets1015* checkInCodelets1015;
    TP1014(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet,
        long in_initIteration, long in_lastIteration, double* in_deposit);
    ~TP1014();
};
/*TP1047: OMPParallelForDirective*/
class TP1047 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets1047 : public darts::Codelet {
    public:
        TP1047* inputsTPParent;
        _barrierCodelets1047()
            : darts::Codelet()
        {
        }
        _barrierCodelets1047(uint32_t dep, uint32_t res, TP1047* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations1047(long* endRange, uint32_t codeletID);
    class _checkInCodelets1048 : public darts::Codelet {
    public:
        TP1047* myTP;
        TP1047* inputsTPParent;
        long endRange;
        _checkInCodelets1048()
            : darts::Codelet()
        {
        }
        _checkInCodelets1048(uint32_t dep, uint32_t res, TP1047* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP1047* TPParent;
    TP1047* controlTPParent;
    TP1047* inputsTPParent;
    double* deposit_darts1047; /*OMP_SHARED - INPUT*/
    long* pidx_darts1047 /*OMP_PRIVATE - INPUT*/;
    long initIteration1047;
    long lastIteration1047;
    long nextIteration1047;
    int loop1047alreadySetUp;
    _barrierCodelets1047* barrierCodelets1047;
    _checkInCodelets1048* checkInCodelets1048;
    TP1047(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet,
        long in_initIteration, long in_lastIteration, double* in_deposit);
    ~TP1047();
};
/*TP1083: OMPParallelForDirective*/
class TP1083 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets1083 : public darts::Codelet {
    public:
        TP1083* inputsTPParent;
        _barrierCodelets1083()
            : darts::Codelet()
        {
        }
        _barrierCodelets1083(uint32_t dep, uint32_t res, TP1083* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations1083(long* endRange, uint32_t codeletID);
    class _checkInCodelets1084 : public darts::Codelet {
    public:
        TP1083* myTP;
        TP1083* inputsTPParent;
        long endRange;
        _checkInCodelets1084()
            : darts::Codelet()
        {
        }
        _checkInCodelets1084(uint32_t dep, uint32_t res, TP1083* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP1083* TPParent;
    TP1083* controlTPParent;
    TP1083* inputsTPParent;
    double* deposit_darts1083; /*OMP_SHARED - INPUT*/
    long* pidx_darts1083 /*OMP_PRIVATE - INPUT*/;
    long initIteration1083;
    long lastIteration1083;
    long nextIteration1083;
    int loop1083alreadySetUp;
    _barrierCodelets1083* barrierCodelets1083;
    _checkInCodelets1084* checkInCodelets1084;
    TP1083(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet,
        long in_initIteration, long in_lastIteration, double* in_deposit);
    ~TP1083();
};
/*TP1116: OMPParallelForDirective*/
class TP1116 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets1116 : public darts::Codelet {
    public:
        TP1116* inputsTPParent;
        _barrierCodelets1116()
            : darts::Codelet()
        {
        }
        _barrierCodelets1116(uint32_t dep, uint32_t res, TP1116* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations1116(long* endRange, uint32_t codeletID);
    class _checkInCodelets1117 : public darts::Codelet {
    public:
        TP1116* myTP;
        TP1116* inputsTPParent;
        long endRange;
        _checkInCodelets1117()
            : darts::Codelet()
        {
        }
        _checkInCodelets1117(uint32_t dep, uint32_t res, TP1116* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP1116* TPParent;
    TP1116* controlTPParent;
    TP1116* inputsTPParent;
    double* deposit_darts1116; /*OMP_SHARED - INPUT*/
    long* pidx_darts1116 /*OMP_PRIVATE - INPUT*/;
    long initIteration1116;
    long lastIteration1116;
    long nextIteration1116;
    int loop1116alreadySetUp;
    _barrierCodelets1116* barrierCodelets1116;
    _checkInCodelets1117* checkInCodelets1117;
    TP1116(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet,
        long in_initIteration, long in_lastIteration, double* in_deposit);
    ~TP1116();
};
/*TP1149: OMPParallelForDirective*/
class TP1149 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets1149 : public darts::Codelet {
    public:
        TP1149* inputsTPParent;
        _barrierCodelets1149()
            : darts::Codelet()
        {
        }
        _barrierCodelets1149(uint32_t dep, uint32_t res, TP1149* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations1149(long* endRange, uint32_t codeletID);
    class _checkInCodelets1150 : public darts::Codelet {
    public:
        TP1149* myTP;
        TP1149* inputsTPParent;
        long endRange;
        _checkInCodelets1150()
            : darts::Codelet()
        {
        }
        _checkInCodelets1150(uint32_t dep, uint32_t res, TP1149* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP1149* TPParent;
    TP1149* controlTPParent;
    TP1149* inputsTPParent;
    double* deposit_darts1149; /*OMP_SHARED - INPUT*/
    long* pidx_darts1149 /*OMP_PRIVATE - INPUT*/;
    long initIteration1149;
    long lastIteration1149;
    long nextIteration1149;
    int loop1149alreadySetUp;
    _barrierCodelets1149* barrierCodelets1149;
    _checkInCodelets1150* checkInCodelets1150;
    TP1149(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet,
        long in_initIteration, long in_lastIteration, double* in_deposit);
    ~TP1149();
};
/*TP1185: OMPParallelForDirective*/
class TP1185 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets1185 : public darts::Codelet {
    public:
        TP1185* inputsTPParent;
        _barrierCodelets1185()
            : darts::Codelet()
        {
        }
        _barrierCodelets1185(uint32_t dep, uint32_t res, TP1185* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations1185(long* endRange, uint32_t codeletID);
    class _checkInCodelets1186 : public darts::Codelet {
    public:
        TP1185* myTP;
        TP1185* inputsTPParent;
        long endRange;
        _checkInCodelets1186()
            : darts::Codelet()
        {
        }
        _checkInCodelets1186(uint32_t dep, uint32_t res, TP1185* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP1185* TPParent;
    TP1185* controlTPParent;
    TP1185* inputsTPParent;
    double* deposit_darts1185; /*OMP_SHARED - INPUT*/
    long* pidx_darts1185 /*OMP_PRIVATE - INPUT*/;
    long initIteration1185;
    long lastIteration1185;
    long nextIteration1185;
    int loop1185alreadySetUp;
    _barrierCodelets1185* barrierCodelets1185;
    _checkInCodelets1186* checkInCodelets1186;
    TP1185(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet,
        long in_initIteration, long in_lastIteration, double* in_deposit);
    ~TP1185();
};
/*TP1218: OMPParallelForDirective*/
class TP1218 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets1218 : public darts::Codelet {
    public:
        TP1218* inputsTPParent;
        _barrierCodelets1218()
            : darts::Codelet()
        {
        }
        _barrierCodelets1218(uint32_t dep, uint32_t res, TP1218* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations1218(long* endRange, uint32_t codeletID);
    class _checkInCodelets1219 : public darts::Codelet {
    public:
        TP1218* myTP;
        TP1218* inputsTPParent;
        long endRange;
        _checkInCodelets1219()
            : darts::Codelet()
        {
        }
        _checkInCodelets1219(uint32_t dep, uint32_t res, TP1218* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP1218* TPParent;
    TP1218* controlTPParent;
    TP1218* inputsTPParent;
    double* deposit_darts1218; /*OMP_SHARED - INPUT*/
    long* pidx_darts1218 /*OMP_PRIVATE - INPUT*/;
    long initIteration1218;
    long lastIteration1218;
    long nextIteration1218;
    int loop1218alreadySetUp;
    _barrierCodelets1218* barrierCodelets1218;
    _checkInCodelets1219* checkInCodelets1219;
    TP1218(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet,
        long in_initIteration, long in_lastIteration, double* in_deposit);
    ~TP1218();
};
/*TP1251: OMPParallelForDirective*/
class TP1251 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets1251 : public darts::Codelet {
    public:
        TP1251* inputsTPParent;
        _barrierCodelets1251()
            : darts::Codelet()
        {
        }
        _barrierCodelets1251(uint32_t dep, uint32_t res, TP1251* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations1251(long* endRange, uint32_t codeletID);
    class _checkInCodelets1252 : public darts::Codelet {
    public:
        TP1251* myTP;
        TP1251* inputsTPParent;
        long endRange;
        _checkInCodelets1252()
            : darts::Codelet()
        {
        }
        _checkInCodelets1252(uint32_t dep, uint32_t res, TP1251* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP1251* TPParent;
    TP1251* controlTPParent;
    TP1251* inputsTPParent;
    double* deposit_darts1251; /*OMP_SHARED - INPUT*/
    long* pidx_darts1251 /*OMP_PRIVATE - INPUT*/;
    long initIteration1251;
    long lastIteration1251;
    long nextIteration1251;
    int loop1251alreadySetUp;
    _barrierCodelets1251* barrierCodelets1251;
    _checkInCodelets1252* checkInCodelets1252;
    TP1251(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet,
        long in_initIteration, long in_lastIteration, double* in_deposit);
    ~TP1251();
};
/*TP1284: OMPParallelForDirective*/
class TP1284 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets1284 : public darts::Codelet {
    public:
        TP1284* inputsTPParent;
        _barrierCodelets1284()
            : darts::Codelet()
        {
        }
        _barrierCodelets1284(uint32_t dep, uint32_t res, TP1284* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations1284(long* endRange, uint32_t codeletID);
    class _checkInCodelets1285 : public darts::Codelet {
    public:
        TP1284* myTP;
        TP1284* inputsTPParent;
        long endRange;
        _checkInCodelets1285()
            : darts::Codelet()
        {
        }
        _checkInCodelets1285(uint32_t dep, uint32_t res, TP1284* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP1284* TPParent;
    TP1284* controlTPParent;
    TP1284* inputsTPParent;
    double* deposit_darts1284; /*OMP_SHARED - INPUT*/
    long* pidx_darts1284 /*OMP_PRIVATE - INPUT*/;
    long initIteration1284;
    long lastIteration1284;
    long nextIteration1284;
    int loop1284alreadySetUp;
    _barrierCodelets1284* barrierCodelets1284;
    _checkInCodelets1285* checkInCodelets1285;
    TP1284(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet,
        long in_initIteration, long in_lastIteration, double* in_deposit);
    ~TP1284();
};
/*TP31: manual_omp_module1*/
class TP31 : public ompTP {
public:
    class _checkInCodelets1331 : public darts::Codelet {
    public:
        TP31* myTP;
        TP31* inputsTPParent;
        _checkInCodelets1331()
            : darts::Codelet()
        {
        }
        _checkInCodelets1331(uint32_t dep, uint32_t res, TP31* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets1331 : public darts::Codelet {
    public:
        TP31* inputsTPParent;
        _barrierCodelets1331()
            : darts::Codelet()
        {
        }
        _barrierCodelets1331(uint32_t dep, uint32_t res, TP31* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets1332 : public darts::Codelet {
    public:
        TP31* myTP;
        TP31* inputsTPParent;
        _checkInCodelets1332()
            : darts::Codelet()
        {
        }
        _checkInCodelets1332(uint32_t dep, uint32_t res, TP31* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets1332 : public darts::Codelet {
    public:
        TP31* inputsTPParent;
        _barrierCodelets1332()
            : darts::Codelet()
        {
        }
        _barrierCodelets1332(uint32_t dep, uint32_t res, TP31* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets1336 : public darts::Codelet {
    public:
        TP31* myTP;
        TP31* inputsTPParent;
        _checkInCodelets1336()
            : darts::Codelet()
        {
        }
        _checkInCodelets1336(uint32_t dep, uint32_t res, TP31* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP31** ptrToThisFunctionTP;
    TP31* inputsTPParent;
    TP31* controlTPParent;
    darts::Codelet** nextCodeletsmanual_omp_module1;
    darts::Codelet** nextSyncCodeletsmanual_omp_module1;
    int* startPidx_darts31 /*VARIABLE*/;
    int* endPidx_darts31 /*VARIABLE*/;
    long* pidx_darts31 /*VARIABLE*/;
    size_t TP1332_alreadyLaunched;
    _checkInCodelets1331* checkInCodelets1331;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets1331* firstCodelet;
#endif
    _barrierCodelets1331* barrierCodelets1331;
    _checkInCodelets1332* checkInCodelets1332;
    _barrierCodelets1332* barrierCodelets1332;
    _checkInCodelets1336* checkInCodelets1336;
    TP31(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_mainNextCodelet,
        darts::Codelet* in_mainSyncCodelet, TP31** in_ptrToThisFunctionTP, int in_startPidx,
        int in_endPidx);
    ~TP31();
    void setNewInputs(int in_startPidx, int in_endPidx, size_t codeletID);
};
/*TP32: manual_omp_module2*/
class TP32 : public ompTP {
public:
    class _checkInCodelets1345 : public darts::Codelet {
    public:
        TP32* myTP;
        TP32* inputsTPParent;
        _checkInCodelets1345()
            : darts::Codelet()
        {
        }
        _checkInCodelets1345(uint32_t dep, uint32_t res, TP32* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets1345 : public darts::Codelet {
    public:
        TP32* inputsTPParent;
        _barrierCodelets1345()
            : darts::Codelet()
        {
        }
        _barrierCodelets1345(uint32_t dep, uint32_t res, TP32* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets1346 : public darts::Codelet {
    public:
        TP32* myTP;
        TP32* inputsTPParent;
        _checkInCodelets1346()
            : darts::Codelet()
        {
        }
        _checkInCodelets1346(uint32_t dep, uint32_t res, TP32* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets1346 : public darts::Codelet {
    public:
        TP32* inputsTPParent;
        _barrierCodelets1346()
            : darts::Codelet()
        {
        }
        _barrierCodelets1346(uint32_t dep, uint32_t res, TP32* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets1350 : public darts::Codelet {
    public:
        TP32* myTP;
        TP32* inputsTPParent;
        _checkInCodelets1350()
            : darts::Codelet()
        {
        }
        _checkInCodelets1350(uint32_t dep, uint32_t res, TP32* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets1356 : public darts::Codelet {
    public:
        TP32* inputsTPParent;
        _barrierCodelets1356()
            : darts::Codelet()
        {
        }
        _barrierCodelets1356(uint32_t dep, uint32_t res, TP32* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets1357 : public darts::Codelet {
    public:
        TP32* myTP;
        TP32* inputsTPParent;
        _checkInCodelets1357()
            : darts::Codelet()
        {
        }
        _checkInCodelets1357(uint32_t dep, uint32_t res, TP32* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets1357 : public darts::Codelet {
    public:
        TP32* inputsTPParent;
        _barrierCodelets1357()
            : darts::Codelet()
        {
        }
        _barrierCodelets1357(uint32_t dep, uint32_t res, TP32* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets1361 : public darts::Codelet {
    public:
        TP32* myTP;
        TP32* inputsTPParent;
        _checkInCodelets1361()
            : darts::Codelet()
        {
        }
        _checkInCodelets1361(uint32_t dep, uint32_t res, TP32* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP32** ptrToThisFunctionTP;
    TP32* inputsTPParent;
    TP32* controlTPParent;
    darts::Codelet** nextCodeletsmanual_omp_module2;
    darts::Codelet** nextSyncCodeletsmanual_omp_module2;
    int* startPidx_darts32 /*VARIABLE*/;
    int* endPidx_darts32 /*VARIABLE*/;
    long* pidx_darts32 /*VARIABLE*/;
    size_t TP1346_alreadyLaunched;
    size_t TP1357_alreadyLaunched;
    _checkInCodelets1345* checkInCodelets1345;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets1345* firstCodelet;
#endif
    _barrierCodelets1345* barrierCodelets1345;
    _checkInCodelets1346* checkInCodelets1346;
    _barrierCodelets1346* barrierCodelets1346;
    _checkInCodelets1350* checkInCodelets1350;
    _barrierCodelets1356* barrierCodelets1356;
    _checkInCodelets1357* checkInCodelets1357;
    _barrierCodelets1357* barrierCodelets1357;
    _checkInCodelets1361* checkInCodelets1361;
    TP32(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_mainNextCodelet,
        darts::Codelet* in_mainSyncCodelet, TP32** in_ptrToThisFunctionTP, int in_startPidx,
        int in_endPidx);
    ~TP32();
    void setNewInputs(int in_startPidx, int in_endPidx, size_t codeletID);
};
/*TP33: manual_omp_module3*/
class TP33 : public ompTP {
public:
    class _checkInCodelets1370 : public darts::Codelet {
    public:
        TP33* myTP;
        TP33* inputsTPParent;
        _checkInCodelets1370()
            : darts::Codelet()
        {
        }
        _checkInCodelets1370(uint32_t dep, uint32_t res, TP33* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets1370 : public darts::Codelet {
    public:
        TP33* inputsTPParent;
        _barrierCodelets1370()
            : darts::Codelet()
        {
        }
        _barrierCodelets1370(uint32_t dep, uint32_t res, TP33* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets1371 : public darts::Codelet {
    public:
        TP33* myTP;
        TP33* inputsTPParent;
        _checkInCodelets1371()
            : darts::Codelet()
        {
        }
        _checkInCodelets1371(uint32_t dep, uint32_t res, TP33* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets1371 : public darts::Codelet {
    public:
        TP33* inputsTPParent;
        _barrierCodelets1371()
            : darts::Codelet()
        {
        }
        _barrierCodelets1371(uint32_t dep, uint32_t res, TP33* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets1375 : public darts::Codelet {
    public:
        TP33* myTP;
        TP33* inputsTPParent;
        _checkInCodelets1375()
            : darts::Codelet()
        {
        }
        _checkInCodelets1375(uint32_t dep, uint32_t res, TP33* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets1381 : public darts::Codelet {
    public:
        TP33* inputsTPParent;
        _barrierCodelets1381()
            : darts::Codelet()
        {
        }
        _barrierCodelets1381(uint32_t dep, uint32_t res, TP33* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets1382 : public darts::Codelet {
    public:
        TP33* myTP;
        TP33* inputsTPParent;
        _checkInCodelets1382()
            : darts::Codelet()
        {
        }
        _checkInCodelets1382(uint32_t dep, uint32_t res, TP33* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets1382 : public darts::Codelet {
    public:
        TP33* inputsTPParent;
        _barrierCodelets1382()
            : darts::Codelet()
        {
        }
        _barrierCodelets1382(uint32_t dep, uint32_t res, TP33* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets1386 : public darts::Codelet {
    public:
        TP33* myTP;
        TP33* inputsTPParent;
        _checkInCodelets1386()
            : darts::Codelet()
        {
        }
        _checkInCodelets1386(uint32_t dep, uint32_t res, TP33* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets1392 : public darts::Codelet {
    public:
        TP33* inputsTPParent;
        _barrierCodelets1392()
            : darts::Codelet()
        {
        }
        _barrierCodelets1392(uint32_t dep, uint32_t res, TP33* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets1393 : public darts::Codelet {
    public:
        TP33* myTP;
        TP33* inputsTPParent;
        _checkInCodelets1393()
            : darts::Codelet()
        {
        }
        _checkInCodelets1393(uint32_t dep, uint32_t res, TP33* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets1393 : public darts::Codelet {
    public:
        TP33* inputsTPParent;
        _barrierCodelets1393()
            : darts::Codelet()
        {
        }
        _barrierCodelets1393(uint32_t dep, uint32_t res, TP33* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets1397 : public darts::Codelet {
    public:
        TP33* myTP;
        TP33* inputsTPParent;
        _checkInCodelets1397()
            : darts::Codelet()
        {
        }
        _checkInCodelets1397(uint32_t dep, uint32_t res, TP33* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP33** ptrToThisFunctionTP;
    TP33* inputsTPParent;
    TP33* controlTPParent;
    darts::Codelet** nextCodeletsmanual_omp_module3;
    darts::Codelet** nextSyncCodeletsmanual_omp_module3;
    int* startPidx_darts33 /*VARIABLE*/;
    int* endPidx_darts33 /*VARIABLE*/;
    long* pidx_darts33 /*VARIABLE*/;
    size_t TP1371_alreadyLaunched;
    size_t TP1382_alreadyLaunched;
    size_t TP1393_alreadyLaunched;
    _checkInCodelets1370* checkInCodelets1370;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets1370* firstCodelet;
#endif
    _barrierCodelets1370* barrierCodelets1370;
    _checkInCodelets1371* checkInCodelets1371;
    _barrierCodelets1371* barrierCodelets1371;
    _checkInCodelets1375* checkInCodelets1375;
    _barrierCodelets1381* barrierCodelets1381;
    _checkInCodelets1382* checkInCodelets1382;
    _barrierCodelets1382* barrierCodelets1382;
    _checkInCodelets1386* checkInCodelets1386;
    _barrierCodelets1392* barrierCodelets1392;
    _checkInCodelets1393* checkInCodelets1393;
    _barrierCodelets1393* barrierCodelets1393;
    _checkInCodelets1397* checkInCodelets1397;
    TP33(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_mainNextCodelet,
        darts::Codelet* in_mainSyncCodelet, TP33** in_ptrToThisFunctionTP, int in_startPidx,
        int in_endPidx);
    ~TP33();
    void setNewInputs(int in_startPidx, int in_endPidx, size_t codeletID);
};
/*TP34: manual_omp_module4*/
class TP34 : public ompTP {
public:
    class _checkInCodelets1406 : public darts::Codelet {
    public:
        TP34* myTP;
        TP34* inputsTPParent;
        _checkInCodelets1406()
            : darts::Codelet()
        {
        }
        _checkInCodelets1406(uint32_t dep, uint32_t res, TP34* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets1406 : public darts::Codelet {
    public:
        TP34* inputsTPParent;
        _barrierCodelets1406()
            : darts::Codelet()
        {
        }
        _barrierCodelets1406(uint32_t dep, uint32_t res, TP34* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets1407 : public darts::Codelet {
    public:
        TP34* myTP;
        TP34* inputsTPParent;
        _checkInCodelets1407()
            : darts::Codelet()
        {
        }
        _checkInCodelets1407(uint32_t dep, uint32_t res, TP34* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets1407 : public darts::Codelet {
    public:
        TP34* inputsTPParent;
        _barrierCodelets1407()
            : darts::Codelet()
        {
        }
        _barrierCodelets1407(uint32_t dep, uint32_t res, TP34* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets1411 : public darts::Codelet {
    public:
        TP34* myTP;
        TP34* inputsTPParent;
        _checkInCodelets1411()
            : darts::Codelet()
        {
        }
        _checkInCodelets1411(uint32_t dep, uint32_t res, TP34* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets1417 : public darts::Codelet {
    public:
        TP34* inputsTPParent;
        _barrierCodelets1417()
            : darts::Codelet()
        {
        }
        _barrierCodelets1417(uint32_t dep, uint32_t res, TP34* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets1418 : public darts::Codelet {
    public:
        TP34* myTP;
        TP34* inputsTPParent;
        _checkInCodelets1418()
            : darts::Codelet()
        {
        }
        _checkInCodelets1418(uint32_t dep, uint32_t res, TP34* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets1418 : public darts::Codelet {
    public:
        TP34* inputsTPParent;
        _barrierCodelets1418()
            : darts::Codelet()
        {
        }
        _barrierCodelets1418(uint32_t dep, uint32_t res, TP34* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets1422 : public darts::Codelet {
    public:
        TP34* myTP;
        TP34* inputsTPParent;
        _checkInCodelets1422()
            : darts::Codelet()
        {
        }
        _checkInCodelets1422(uint32_t dep, uint32_t res, TP34* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets1428 : public darts::Codelet {
    public:
        TP34* inputsTPParent;
        _barrierCodelets1428()
            : darts::Codelet()
        {
        }
        _barrierCodelets1428(uint32_t dep, uint32_t res, TP34* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets1429 : public darts::Codelet {
    public:
        TP34* myTP;
        TP34* inputsTPParent;
        _checkInCodelets1429()
            : darts::Codelet()
        {
        }
        _checkInCodelets1429(uint32_t dep, uint32_t res, TP34* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets1429 : public darts::Codelet {
    public:
        TP34* inputsTPParent;
        _barrierCodelets1429()
            : darts::Codelet()
        {
        }
        _barrierCodelets1429(uint32_t dep, uint32_t res, TP34* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets1433 : public darts::Codelet {
    public:
        TP34* myTP;
        TP34* inputsTPParent;
        _checkInCodelets1433()
            : darts::Codelet()
        {
        }
        _checkInCodelets1433(uint32_t dep, uint32_t res, TP34* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets1439 : public darts::Codelet {
    public:
        TP34* inputsTPParent;
        _barrierCodelets1439()
            : darts::Codelet()
        {
        }
        _barrierCodelets1439(uint32_t dep, uint32_t res, TP34* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets1440 : public darts::Codelet {
    public:
        TP34* myTP;
        TP34* inputsTPParent;
        _checkInCodelets1440()
            : darts::Codelet()
        {
        }
        _checkInCodelets1440(uint32_t dep, uint32_t res, TP34* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets1440 : public darts::Codelet {
    public:
        TP34* inputsTPParent;
        _barrierCodelets1440()
            : darts::Codelet()
        {
        }
        _barrierCodelets1440(uint32_t dep, uint32_t res, TP34* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets1444 : public darts::Codelet {
    public:
        TP34* myTP;
        TP34* inputsTPParent;
        _checkInCodelets1444()
            : darts::Codelet()
        {
        }
        _checkInCodelets1444(uint32_t dep, uint32_t res, TP34* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP34** ptrToThisFunctionTP;
    TP34* inputsTPParent;
    TP34* controlTPParent;
    darts::Codelet** nextCodeletsmanual_omp_module4;
    darts::Codelet** nextSyncCodeletsmanual_omp_module4;
    int* startPidx_darts34 /*VARIABLE*/;
    int* endPidx_darts34 /*VARIABLE*/;
    long* pidx_darts34 /*VARIABLE*/;
    size_t TP1407_alreadyLaunched;
    size_t TP1418_alreadyLaunched;
    size_t TP1429_alreadyLaunched;
    size_t TP1440_alreadyLaunched;
    _checkInCodelets1406* checkInCodelets1406;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets1406* firstCodelet;
#endif
    _barrierCodelets1406* barrierCodelets1406;
    _checkInCodelets1407* checkInCodelets1407;
    _barrierCodelets1407* barrierCodelets1407;
    _checkInCodelets1411* checkInCodelets1411;
    _barrierCodelets1417* barrierCodelets1417;
    _checkInCodelets1418* checkInCodelets1418;
    _barrierCodelets1418* barrierCodelets1418;
    _checkInCodelets1422* checkInCodelets1422;
    _barrierCodelets1428* barrierCodelets1428;
    _checkInCodelets1429* checkInCodelets1429;
    _barrierCodelets1429* barrierCodelets1429;
    _checkInCodelets1433* checkInCodelets1433;
    _barrierCodelets1439* barrierCodelets1439;
    _checkInCodelets1440* checkInCodelets1440;
    _barrierCodelets1440* barrierCodelets1440;
    _checkInCodelets1444* checkInCodelets1444;
    TP34(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_mainNextCodelet,
        darts::Codelet* in_mainSyncCodelet, TP34** in_ptrToThisFunctionTP, int in_startPidx,
        int in_endPidx);
    ~TP34();
    void setNewInputs(int in_startPidx, int in_endPidx, size_t codeletID);
};
/*TP1451: OMPParallelDirective*/
class TP1451 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets1451 : public darts::Codelet {
    public:
        TP1451* inputsTPParent;
        _barrierCodelets1451()
            : darts::Codelet()
        {
        }
        _barrierCodelets1451(uint32_t dep, uint32_t res, TP1451* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets1453 : public darts::Codelet {
    public:
        TP1451* myTP;
        TP1451* inputsTPParent;
        _checkInCodelets1453()
            : darts::Codelet()
        {
        }
        _checkInCodelets1453(uint32_t dep, uint32_t res, TP1451* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets1490 : public darts::Codelet {
    public:
        TP1451* myTP;
        TP1451* inputsTPParent;
        _checkInCodelets1490()
            : darts::Codelet()
        {
        }
        _checkInCodelets1490(uint32_t dep, uint32_t res, TP1451* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets1493 : public darts::Codelet {
    public:
        TP1451* myTP;
        TP1451* inputsTPParent;
        _checkInCodelets1493()
            : darts::Codelet()
        {
        }
        _checkInCodelets1493(uint32_t dep, uint32_t res, TP1451* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP1451* TPParent;
    TP1451* controlTPParent;
    TP1451* inputsTPParent;
    long* num_iterations_darts1451; /*OMP_SHARED - INPUT*/
    double* dparts_per_thread_darts1451 /*VARIABLE*/;
    int* endPidx_darts1451 /*VARIABLE*/;
    long* iteration_darts1451 /*VARIABLE*/;
    int* numThreads_darts1451 /*VARIABLE*/;
    int* startPidx_darts1451 /*VARIABLE*/;
    int* thread_id_darts1451 /*VARIABLE*/;
    unsigned int TP1490_LoopCounter;
    unsigned int* TP1490_LoopCounterPerThread;
    tbb::concurrent_vector<TP1490*> TP1490PtrVec;
    _barrierCodelets1451* barrierCodelets1451;
    _checkInCodelets1453* checkInCodelets1453;
    _checkInCodelets1490* checkInCodelets1490;
    _checkInCodelets1493* checkInCodelets1493;
    TP1451(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet,
        long* in_num_iterations);
    ~TP1451();
};
/*TP1490: ForStmt*/
class TP1490 : public ompTP {
public:
    class _checkInCodelets1495 : public darts::Codelet {
    public:
        TP1490* myTP;
        TP1451* inputsTPParent;
        _checkInCodelets1495()
            : darts::Codelet()
        {
        }
        _checkInCodelets1495(uint32_t dep, uint32_t res, TP1490* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets1496 : public darts::Codelet {
    public:
        TP1490* myTP;
        TP1451* inputsTPParent;
        _checkInCodelets1496()
            : darts::Codelet()
        {
        }
        _checkInCodelets1496(uint32_t dep, uint32_t res, TP1490* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets1497 : public darts::Codelet {
    public:
        TP1490* myTP;
        TP1451* inputsTPParent;
        _checkInCodelets1497()
            : darts::Codelet()
        {
        }
        _checkInCodelets1497(uint32_t dep, uint32_t res, TP1490* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets1498 : public darts::Codelet {
    public:
        TP1490* myTP;
        TP1451* inputsTPParent;
        _checkInCodelets1498()
            : darts::Codelet()
        {
        }
        _checkInCodelets1498(uint32_t dep, uint32_t res, TP1490* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP1451* TPParent;
    TP1490* controlTPParent;
    TP1451* inputsTPParent;
    TP1490** ptrToThisTP;
    TP_manual_omp_module1* TP1495Ptr;
    int TP1495_alreadyLaunched;
    TP_manual_omp_module2* TP1496Ptr;
    int TP1496_alreadyLaunched;
    TP_manual_omp_module3* TP1497Ptr;
    int TP1497_alreadyLaunched;
    TP_manual_omp_module4* TP1498Ptr;
    int TP1498_alreadyLaunched;
    _checkInCodelets1495* checkInCodelets1495;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets1495* firstCodelet;
#endif
    _checkInCodelets1496* checkInCodelets1496;
    _checkInCodelets1497* checkInCodelets1497;
    _checkInCodelets1498* checkInCodelets1498;
    TP1490(int in_numThreads, int in_mainCodeletID, TP1451* in_TPParent, TP1451* in_inputsTPParent,
        TP1490** in_ptrToThisTP);
    ~TP1490();
};
/*TP1579: OMPParallelDirective*/
class TP1579 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets1579 : public darts::Codelet {
    public:
        TP1579* inputsTPParent;
        _barrierCodelets1579()
            : darts::Codelet()
        {
        }
        _barrierCodelets1579(uint32_t dep, uint32_t res, TP1579* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets1581 : public darts::Codelet {
    public:
        TP1579* myTP;
        TP1579* inputsTPParent;
        _checkInCodelets1581()
            : darts::Codelet()
        {
        }
        _checkInCodelets1581(uint32_t dep, uint32_t res, TP1579* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP1579* TPParent;
    TP1579* controlTPParent;
    TP1579* inputsTPParent;
    double* deposit_darts1579; /*OMP_SHARED - INPUT*/
    long* num_iterations_darts1579; /*OMP_SHARED - INPUT*/
    double* dparts_per_thread_darts1579 /*VARIABLE*/;
    int* endPidx_darts1579 /*VARIABLE*/;
    long* iteration_darts1579 /*VARIABLE*/;
    int* numThreads_darts1579 /*VARIABLE*/;
    int* startPidx_darts1579 /*VARIABLE*/;
    int* thread_id_darts1579 /*VARIABLE*/;
    _barrierCodelets1579* barrierCodelets1579;
    _checkInCodelets1581* checkInCodelets1581;
    TP1579(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet,
        double* in_deposit, long* in_num_iterations);
    ~TP1579();
};
/*TP1860: OMPParallelForDirective*/
class TP1860 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets1860 : public darts::Codelet {
    public:
        TP1860* inputsTPParent;
        _barrierCodelets1860()
            : darts::Codelet()
        {
        }
        _barrierCodelets1860(uint32_t dep, uint32_t res, TP1860* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations1860(long* endRange, uint32_t codeletID);
    class _checkInCodelets1861 : public darts::Codelet {
    public:
        TP1860* myTP;
        TP1860* inputsTPParent;
        long endRange;
        _checkInCodelets1861()
            : darts::Codelet()
        {
        }
        _checkInCodelets1861(uint32_t dep, uint32_t res, TP1860* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP1860* TPParent;
    TP1860* controlTPParent;
    TP1860* inputsTPParent;
    long* partId_darts1860 /*OMP_PRIVATE - INPUT*/;
    long initIteration1860;
    long lastIteration1860;
    long range1860;
    long rangePerCodelet1860;
    long minIteration1860;
    long remainderRange1860;
    _barrierCodelets1860* barrierCodelets1860;
    _checkInCodelets1861* checkInCodelets1861;
    TP1860(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet,
        long in_initIteration, long in_lastIteration);
    ~TP1860();
};
/*TP1897: OMPParallelForDirective*/
class TP1897 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets1897 : public darts::Codelet {
    public:
        TP1897* inputsTPParent;
        _barrierCodelets1897()
            : darts::Codelet()
        {
        }
        _barrierCodelets1897(uint32_t dep, uint32_t res, TP1897* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    bool requestNewRangeIterations1897(long* endRange, uint32_t codeletID);
    class _checkInCodelets1898 : public darts::Codelet {
    public:
        TP1897* myTP;
        TP1897* inputsTPParent;
        long endRange;
        _checkInCodelets1898()
            : darts::Codelet()
        {
        }
        _checkInCodelets1898(uint32_t dep, uint32_t res, TP1897* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP1897* TPParent;
    TP1897* controlTPParent;
    TP1897* inputsTPParent;
    long* partId_darts1897 /*OMP_PRIVATE - INPUT*/;
    long initIteration1897;
    long lastIteration1897;
    long range1897;
    long rangePerCodelet1897;
    long minIteration1897;
    long remainderRange1897;
    _barrierCodelets1897* barrierCodelets1897;
    _checkInCodelets1898* checkInCodelets1898;
    TP1897(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet,
        long in_initIteration, long in_lastIteration);
    ~TP1897();
};
#endif
