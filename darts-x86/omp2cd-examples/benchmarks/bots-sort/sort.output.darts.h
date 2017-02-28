#ifndef _sort_output_darts_h_
#define _sort_output_darts_h_
#ifndef __DARTS_
#define __DARTS_
#endif
#include "app-desc.h"
#include "bots.h"
#include "darts.h"
#include "ompTP.h"
#include "tbb/concurrent_vector.h"
#include "utils.h"
#include <limits.h>
#include <mutex>
#include <numa.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
int sort_verify();
void sort_par();
void scramble_array(ELM* in_array);
void fill_array(ELM* in_array);
void seqquick(ELM* low, ELM* high);
void seqmerge(ELM* low1, ELM* high1, ELM* low2, ELM* high2, ELM* lowdest);
ELM* binsplit(ELM val, ELM* low, ELM* high);
void sort_init();
class TP24;
typedef TP24 TP_cilkmerge_par;
/*Class containing the inputs passed to task330*/
class _task330Inputs {
public:
    ELM* low1_darts330 /*OMP_FIRSTPRIVATE*/;
    ELM* low2_darts330 /*OMP_FIRSTPRIVATE*/;
    ELM* lowdest_darts330 /*OMP_FIRSTPRIVATE*/;
    ELM* split1_darts330 /*OMP_FIRSTPRIVATE*/;
    ELM* split2_darts330 /*OMP_FIRSTPRIVATE*/;
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task330Inputs() {}
    _task330Inputs(ELM** in_low1, ELM** in_low2, ELM** in_lowdest, ELM** in_split1, ELM** in_split2,
        darts::Codelet* in_nextSyncCodelet)
        : taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
        this->low1_darts330 = *in_low1;
        this->low2_darts330 = *in_low2;
        this->lowdest_darts330 = *in_lowdest;
        this->split1_darts330 = *in_split1;
        this->split2_darts330 = *in_split2;
    }
    ~_task330Inputs() {}
};
class TP330;
/*Class containing the inputs passed to task333*/
class _task333Inputs {
public:
    ELM* high1_darts333 /*OMP_FIRSTPRIVATE*/;
    ELM* high2_darts333 /*OMP_FIRSTPRIVATE*/;
    ELM* lowdest_darts333 /*OMP_FIRSTPRIVATE*/;
    long lowsize_darts333 /*OMP_FIRSTPRIVATE*/;
    ELM* split1_darts333 /*OMP_FIRSTPRIVATE*/;
    ELM* split2_darts333 /*OMP_FIRSTPRIVATE*/;
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task333Inputs() {}
    _task333Inputs(ELM** in_high1, ELM** in_high2, ELM** in_lowdest, long* in_lowsize,
        ELM** in_split1, ELM** in_split2, darts::Codelet* in_nextSyncCodelet)
        : taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
        this->high1_darts333 = *in_high1;
        this->high2_darts333 = *in_high2;
        this->lowdest_darts333 = *in_lowdest;
        this->lowsize_darts333 = *in_lowsize;
        this->split1_darts333 = *in_split1;
        this->split2_darts333 = *in_split2;
    }
    ~_task333Inputs() {}
};
class TP333;
class TP25;
typedef TP25 TP_cilksort_par;
/*Class containing the inputs passed to task366*/
class _task366Inputs {
public:
    ELM* A_darts366 /*OMP_FIRSTPRIVATE*/;
    long quarter_darts366 /*OMP_FIRSTPRIVATE*/;
    ELM* tmpA_darts366 /*OMP_FIRSTPRIVATE*/;
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task366Inputs() {}
    _task366Inputs(ELM** in_A, long* in_quarter, ELM** in_tmpA, darts::Codelet* in_nextSyncCodelet)
        : taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
        this->A_darts366 = *in_A;
        this->quarter_darts366 = *in_quarter;
        this->tmpA_darts366 = *in_tmpA;
    }
    ~_task366Inputs() {}
};
class TP366;
/*Class containing the inputs passed to task368*/
class _task368Inputs {
public:
    ELM* B_darts368 /*OMP_FIRSTPRIVATE*/;
    long quarter_darts368 /*OMP_FIRSTPRIVATE*/;
    ELM* tmpB_darts368 /*OMP_FIRSTPRIVATE*/;
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task368Inputs() {}
    _task368Inputs(ELM** in_B, long* in_quarter, ELM** in_tmpB, darts::Codelet* in_nextSyncCodelet)
        : taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
        this->B_darts368 = *in_B;
        this->quarter_darts368 = *in_quarter;
        this->tmpB_darts368 = *in_tmpB;
    }
    ~_task368Inputs() {}
};
class TP368;
/*Class containing the inputs passed to task370*/
class _task370Inputs {
public:
    ELM* C_darts370 /*OMP_FIRSTPRIVATE*/;
    long quarter_darts370 /*OMP_FIRSTPRIVATE*/;
    ELM* tmpC_darts370 /*OMP_FIRSTPRIVATE*/;
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task370Inputs() {}
    _task370Inputs(ELM** in_C, long* in_quarter, ELM** in_tmpC, darts::Codelet* in_nextSyncCodelet)
        : taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
        this->C_darts370 = *in_C;
        this->quarter_darts370 = *in_quarter;
        this->tmpC_darts370 = *in_tmpC;
    }
    ~_task370Inputs() {}
};
class TP370;
/*Class containing the inputs passed to task372*/
class _task372Inputs {
public:
    ELM* D_darts372 /*OMP_FIRSTPRIVATE*/;
    long quarter_darts372 /*OMP_FIRSTPRIVATE*/;
    long size_darts372 /*OMP_FIRSTPRIVATE*/;
    ELM* tmpD_darts372 /*OMP_FIRSTPRIVATE*/;
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task372Inputs() {}
    _task372Inputs(ELM** in_D, long* in_quarter, long* in_size, ELM** in_tmpD,
        darts::Codelet* in_nextSyncCodelet)
        : taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
        this->D_darts372 = *in_D;
        this->quarter_darts372 = *in_quarter;
        this->size_darts372 = *in_size;
        this->tmpD_darts372 = *in_tmpD;
    }
    ~_task372Inputs() {}
};
class TP372;
/*Class containing the inputs passed to task377*/
class _task377Inputs {
public:
    ELM* A_darts377 /*OMP_FIRSTPRIVATE*/;
    ELM* B_darts377 /*OMP_FIRSTPRIVATE*/;
    long quarter_darts377 /*OMP_FIRSTPRIVATE*/;
    ELM* tmpA_darts377 /*OMP_FIRSTPRIVATE*/;
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task377Inputs() {}
    _task377Inputs(
        ELM** in_A, ELM** in_B, long* in_quarter, ELM** in_tmpA, darts::Codelet* in_nextSyncCodelet)
        : taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
        this->A_darts377 = *in_A;
        this->B_darts377 = *in_B;
        this->quarter_darts377 = *in_quarter;
        this->tmpA_darts377 = *in_tmpA;
    }
    ~_task377Inputs() {}
};
class TP377;
/*Class containing the inputs passed to task383*/
class _task383Inputs {
public:
    ELM* C_darts383 /*OMP_FIRSTPRIVATE*/;
    ELM* D_darts383 /*OMP_FIRSTPRIVATE*/;
    ELM* low_darts383 /*OMP_FIRSTPRIVATE*/;
    long quarter_darts383 /*OMP_FIRSTPRIVATE*/;
    long size_darts383 /*OMP_FIRSTPRIVATE*/;
    ELM* tmpC_darts383 /*OMP_FIRSTPRIVATE*/;
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task383Inputs() {}
    _task383Inputs(ELM** in_C, ELM** in_D, ELM** in_low, long* in_quarter, long* in_size,
        ELM** in_tmpC, darts::Codelet* in_nextSyncCodelet)
        : taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
        this->C_darts383 = *in_C;
        this->D_darts383 = *in_D;
        this->low_darts383 = *in_low;
        this->quarter_darts383 = *in_quarter;
        this->size_darts383 = *in_size;
        this->tmpC_darts383 = *in_tmpC;
    }
    ~_task383Inputs() {}
};
class TP383;
class TP501;
class TP502;
/*Class containing the inputs passed to task503*/
class _task503Inputs {
public:
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task503Inputs() {}
    _task503Inputs(darts::Codelet* in_nextSyncCodelet)
        : taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
    }
    ~_task503Inputs() {}
};
class TP503;
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
/*TP24: cilkmerge_par*/
class TP24 : public ompTP {
public:
    class _checkInCodelets285 : public darts::Codelet {
    public:
        TP24* myTP;
        TP24* inputsTPParent;
        _checkInCodelets285()
            : darts::Codelet()
        {
        }
        _checkInCodelets285(uint32_t dep, uint32_t res, TP24* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets330 : public darts::Codelet {
    public:
        TP24* myTP;
        TP24* inputsTPParent;
        _checkInCodelets330()
            : darts::Codelet()
        {
        }
        _checkInCodelets330(uint32_t dep, uint32_t res, TP24* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets333 : public darts::Codelet {
    public:
        TP24* myTP;
        TP24* inputsTPParent;
        _checkInCodelets333()
            : darts::Codelet()
        {
        }
        _checkInCodelets333(uint32_t dep, uint32_t res, TP24* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets339 : public darts::Codelet {
    public:
        TP24* inputsTPParent;
        _barrierCodelets339()
            : darts::Codelet()
        {
        }
        _barrierCodelets339(uint32_t dep, uint32_t res, TP24* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets340 : public darts::Codelet {
    public:
        TP24* myTP;
        TP24* inputsTPParent;
        _checkInCodelets340()
            : darts::Codelet()
        {
        }
        _checkInCodelets340(uint32_t dep, uint32_t res, TP24* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP24** ptrToThisFunctionTP;
    TP24* inputsTPParent;
    TP24* controlTPParent;
    darts::Codelet** nextCodeletscilkmerge_par;
    darts::Codelet** nextSyncCodeletscilkmerge_par;
    ELM** low1_darts24 /*VARIABLE*/;
    ELM** high1_darts24 /*VARIABLE*/;
    ELM** low2_darts24 /*VARIABLE*/;
    ELM** high2_darts24 /*VARIABLE*/;
    ELM** lowdest_darts24 /*VARIABLE*/;
    long* lowsize_darts24 /*VARIABLE*/;
    ELM** split1_darts24 /*VARIABLE*/;
    ELM** split2_darts24 /*VARIABLE*/;
    ELM** tmp_darts24 /*VARIABLE*/;
    _task330Inputs** task330Inputs;
    _task333Inputs** task333Inputs;
    _checkInCodelets285* checkInCodelets285;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets285* firstCodelet;
#endif
    _checkInCodelets330* checkInCodelets330;
    _checkInCodelets333* checkInCodelets333;
    _barrierCodelets339* barrierCodelets339;
    _checkInCodelets340* checkInCodelets340;
    TP24(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_mainNextCodelet,
        darts::Codelet* in_mainSyncCodelet, TP24** in_ptrToThisFunctionTP, ELM* in_low1,
        ELM* in_high1, ELM* in_low2, ELM* in_high2, ELM* in_lowdest);
    ~TP24();
    void setNewInputs(ELM* in_low1, ELM* in_high1, ELM* in_low2, ELM* in_high2, ELM* in_lowdest,
        size_t codeletID);
};
/*TP330: OMPTaskDirective*/
class TP330 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets331 : public darts::Codelet {
    public:
        TP330* myTP;
        TP330* inputsTPParent;
        _task330Inputs* taskInputs;
        _checkInCodelets331()
            : darts::Codelet()
        {
        }
        _checkInCodelets331(uint32_t dep, uint32_t res, TP330* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task330Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets530 : public darts::Codelet {
    public:
        TP330* myTP;
        TP330* inputsTPParent;
        _task330Inputs* taskInputs;
        _checkInCodelets530()
            : darts::Codelet()
        {
        }
        _checkInCodelets530(uint32_t dep, uint32_t res, TP330* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task330Inputs)
        {
        }
        void fire(void);
    };
    TP24* TPParent;
    TP330* controlTPParent;
    TP330* inputsTPParent;
    _task330Inputs* task330Inputs;
    TP_cilkmerge_par* TP331Ptr;
    int TP331_alreadyLaunched;
    _checkInCodelets331 checkInCodelets331;
    _checkInCodelets530 checkInCodelets530;
    TP330(int in_numThreads, int in_mainCodeletID, TP24* in_TPParent,
        _task330Inputs* in_task330Inputs);
    ~TP330();
};
/*TP333: OMPTaskDirective*/
class TP333 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets334 : public darts::Codelet {
    public:
        TP333* myTP;
        TP333* inputsTPParent;
        _task333Inputs* taskInputs;
        _checkInCodelets334()
            : darts::Codelet()
        {
        }
        _checkInCodelets334(uint32_t dep, uint32_t res, TP333* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task333Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets531 : public darts::Codelet {
    public:
        TP333* myTP;
        TP333* inputsTPParent;
        _task333Inputs* taskInputs;
        _checkInCodelets531()
            : darts::Codelet()
        {
        }
        _checkInCodelets531(uint32_t dep, uint32_t res, TP333* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task333Inputs)
        {
        }
        void fire(void);
    };
    TP24* TPParent;
    TP333* controlTPParent;
    TP333* inputsTPParent;
    _task333Inputs* task333Inputs;
    TP_cilkmerge_par* TP334Ptr;
    int TP334_alreadyLaunched;
    _checkInCodelets334 checkInCodelets334;
    _checkInCodelets531 checkInCodelets531;
    TP333(int in_numThreads, int in_mainCodeletID, TP24* in_TPParent,
        _task333Inputs* in_task333Inputs);
    ~TP333();
};
/*TP25: cilksort_par*/
class TP25 : public ompTP {
public:
    class _checkInCodelets342 : public darts::Codelet {
    public:
        TP25* myTP;
        TP25* inputsTPParent;
        _checkInCodelets342()
            : darts::Codelet()
        {
        }
        _checkInCodelets342(uint32_t dep, uint32_t res, TP25* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets366 : public darts::Codelet {
    public:
        TP25* myTP;
        TP25* inputsTPParent;
        _checkInCodelets366()
            : darts::Codelet()
        {
        }
        _checkInCodelets366(uint32_t dep, uint32_t res, TP25* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets368 : public darts::Codelet {
    public:
        TP25* myTP;
        TP25* inputsTPParent;
        _checkInCodelets368()
            : darts::Codelet()
        {
        }
        _checkInCodelets368(uint32_t dep, uint32_t res, TP25* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets370 : public darts::Codelet {
    public:
        TP25* myTP;
        TP25* inputsTPParent;
        _checkInCodelets370()
            : darts::Codelet()
        {
        }
        _checkInCodelets370(uint32_t dep, uint32_t res, TP25* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets372 : public darts::Codelet {
    public:
        TP25* myTP;
        TP25* inputsTPParent;
        _checkInCodelets372()
            : darts::Codelet()
        {
        }
        _checkInCodelets372(uint32_t dep, uint32_t res, TP25* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets376 : public darts::Codelet {
    public:
        TP25* inputsTPParent;
        _barrierCodelets376()
            : darts::Codelet()
        {
        }
        _barrierCodelets376(uint32_t dep, uint32_t res, TP25* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets377 : public darts::Codelet {
    public:
        TP25* myTP;
        TP25* inputsTPParent;
        _checkInCodelets377()
            : darts::Codelet()
        {
        }
        _checkInCodelets377(uint32_t dep, uint32_t res, TP25* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets383 : public darts::Codelet {
    public:
        TP25* myTP;
        TP25* inputsTPParent;
        _checkInCodelets383()
            : darts::Codelet()
        {
        }
        _checkInCodelets383(uint32_t dep, uint32_t res, TP25* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets389 : public darts::Codelet {
    public:
        TP25* inputsTPParent;
        _barrierCodelets389()
            : darts::Codelet()
        {
        }
        _barrierCodelets389(uint32_t dep, uint32_t res, TP25* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets390 : public darts::Codelet {
    public:
        TP25* myTP;
        TP25* inputsTPParent;
        _checkInCodelets390()
            : darts::Codelet()
        {
        }
        _checkInCodelets390(uint32_t dep, uint32_t res, TP25* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP25** ptrToThisFunctionTP;
    TP25* inputsTPParent;
    TP25* controlTPParent;
    darts::Codelet** nextCodeletscilksort_par;
    darts::Codelet** nextSyncCodeletscilksort_par;
    ELM** low_darts25 /*VARIABLE*/;
    ELM** tmp_darts25 /*VARIABLE*/;
    long* size_darts25 /*VARIABLE*/;
    ELM** A_darts25 /*VARIABLE*/;
    ELM** B_darts25 /*VARIABLE*/;
    ELM** C_darts25 /*VARIABLE*/;
    ELM** D_darts25 /*VARIABLE*/;
    long* quarter_darts25 /*VARIABLE*/;
    ELM** tmpA_darts25 /*VARIABLE*/;
    ELM** tmpB_darts25 /*VARIABLE*/;
    ELM** tmpC_darts25 /*VARIABLE*/;
    ELM** tmpD_darts25 /*VARIABLE*/;
    TP_cilkmerge_par* TP390Ptr;
    int TP390_alreadyLaunched;
    _task366Inputs** task366Inputs;
    _task368Inputs** task368Inputs;
    _task370Inputs** task370Inputs;
    _task372Inputs** task372Inputs;
    _task377Inputs** task377Inputs;
    _task383Inputs** task383Inputs;
    _checkInCodelets342* checkInCodelets342;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets342* firstCodelet;
#endif
    _checkInCodelets366* checkInCodelets366;
    _checkInCodelets368* checkInCodelets368;
    _checkInCodelets370* checkInCodelets370;
    _checkInCodelets372* checkInCodelets372;
    _barrierCodelets376* barrierCodelets376;
    _checkInCodelets377* checkInCodelets377;
    _checkInCodelets383* checkInCodelets383;
    _barrierCodelets389* barrierCodelets389;
    _checkInCodelets390* checkInCodelets390;
    TP25(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_mainNextCodelet,
        darts::Codelet* in_mainSyncCodelet, TP25** in_ptrToThisFunctionTP, ELM* in_low, ELM* in_tmp,
        long in_size);
    ~TP25();
    void setNewInputs(ELM* in_low, ELM* in_tmp, long in_size, size_t codeletID);
};
/*TP366: OMPTaskDirective*/
class TP366 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets367 : public darts::Codelet {
    public:
        TP366* myTP;
        TP366* inputsTPParent;
        _task366Inputs* taskInputs;
        _checkInCodelets367()
            : darts::Codelet()
        {
        }
        _checkInCodelets367(uint32_t dep, uint32_t res, TP366* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task366Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets524 : public darts::Codelet {
    public:
        TP366* myTP;
        TP366* inputsTPParent;
        _task366Inputs* taskInputs;
        _checkInCodelets524()
            : darts::Codelet()
        {
        }
        _checkInCodelets524(uint32_t dep, uint32_t res, TP366* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task366Inputs)
        {
        }
        void fire(void);
    };
    TP25* TPParent;
    TP366* controlTPParent;
    TP366* inputsTPParent;
    _task366Inputs* task366Inputs;
    TP_cilksort_par* TP367Ptr;
    int TP367_alreadyLaunched;
    _checkInCodelets367 checkInCodelets367;
    _checkInCodelets524 checkInCodelets524;
    TP366(int in_numThreads, int in_mainCodeletID, TP25* in_TPParent,
        _task366Inputs* in_task366Inputs);
    ~TP366();
};
/*TP368: OMPTaskDirective*/
class TP368 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets369 : public darts::Codelet {
    public:
        TP368* myTP;
        TP368* inputsTPParent;
        _task368Inputs* taskInputs;
        _checkInCodelets369()
            : darts::Codelet()
        {
        }
        _checkInCodelets369(uint32_t dep, uint32_t res, TP368* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task368Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets525 : public darts::Codelet {
    public:
        TP368* myTP;
        TP368* inputsTPParent;
        _task368Inputs* taskInputs;
        _checkInCodelets525()
            : darts::Codelet()
        {
        }
        _checkInCodelets525(uint32_t dep, uint32_t res, TP368* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task368Inputs)
        {
        }
        void fire(void);
    };
    TP25* TPParent;
    TP368* controlTPParent;
    TP368* inputsTPParent;
    _task368Inputs* task368Inputs;
    TP_cilksort_par* TP369Ptr;
    int TP369_alreadyLaunched;
    _checkInCodelets369 checkInCodelets369;
    _checkInCodelets525 checkInCodelets525;
    TP368(int in_numThreads, int in_mainCodeletID, TP25* in_TPParent,
        _task368Inputs* in_task368Inputs);
    ~TP368();
};
/*TP370: OMPTaskDirective*/
class TP370 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets371 : public darts::Codelet {
    public:
        TP370* myTP;
        TP370* inputsTPParent;
        _task370Inputs* taskInputs;
        _checkInCodelets371()
            : darts::Codelet()
        {
        }
        _checkInCodelets371(uint32_t dep, uint32_t res, TP370* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task370Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets526 : public darts::Codelet {
    public:
        TP370* myTP;
        TP370* inputsTPParent;
        _task370Inputs* taskInputs;
        _checkInCodelets526()
            : darts::Codelet()
        {
        }
        _checkInCodelets526(uint32_t dep, uint32_t res, TP370* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task370Inputs)
        {
        }
        void fire(void);
    };
    TP25* TPParent;
    TP370* controlTPParent;
    TP370* inputsTPParent;
    _task370Inputs* task370Inputs;
    TP_cilksort_par* TP371Ptr;
    int TP371_alreadyLaunched;
    _checkInCodelets371 checkInCodelets371;
    _checkInCodelets526 checkInCodelets526;
    TP370(int in_numThreads, int in_mainCodeletID, TP25* in_TPParent,
        _task370Inputs* in_task370Inputs);
    ~TP370();
};
/*TP372: OMPTaskDirective*/
class TP372 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets373 : public darts::Codelet {
    public:
        TP372* myTP;
        TP372* inputsTPParent;
        _task372Inputs* taskInputs;
        _checkInCodelets373()
            : darts::Codelet()
        {
        }
        _checkInCodelets373(uint32_t dep, uint32_t res, TP372* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task372Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets527 : public darts::Codelet {
    public:
        TP372* myTP;
        TP372* inputsTPParent;
        _task372Inputs* taskInputs;
        _checkInCodelets527()
            : darts::Codelet()
        {
        }
        _checkInCodelets527(uint32_t dep, uint32_t res, TP372* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task372Inputs)
        {
        }
        void fire(void);
    };
    TP25* TPParent;
    TP372* controlTPParent;
    TP372* inputsTPParent;
    _task372Inputs* task372Inputs;
    TP_cilksort_par* TP373Ptr;
    int TP373_alreadyLaunched;
    _checkInCodelets373 checkInCodelets373;
    _checkInCodelets527 checkInCodelets527;
    TP372(int in_numThreads, int in_mainCodeletID, TP25* in_TPParent,
        _task372Inputs* in_task372Inputs);
    ~TP372();
};
/*TP377: OMPTaskDirective*/
class TP377 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets378 : public darts::Codelet {
    public:
        TP377* myTP;
        TP377* inputsTPParent;
        _task377Inputs* taskInputs;
        _checkInCodelets378()
            : darts::Codelet()
        {
        }
        _checkInCodelets378(uint32_t dep, uint32_t res, TP377* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task377Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets528 : public darts::Codelet {
    public:
        TP377* myTP;
        TP377* inputsTPParent;
        _task377Inputs* taskInputs;
        _checkInCodelets528()
            : darts::Codelet()
        {
        }
        _checkInCodelets528(uint32_t dep, uint32_t res, TP377* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task377Inputs)
        {
        }
        void fire(void);
    };
    TP25* TPParent;
    TP377* controlTPParent;
    TP377* inputsTPParent;
    _task377Inputs* task377Inputs;
    TP_cilkmerge_par* TP378Ptr;
    int TP378_alreadyLaunched;
    _checkInCodelets378 checkInCodelets378;
    _checkInCodelets528 checkInCodelets528;
    TP377(int in_numThreads, int in_mainCodeletID, TP25* in_TPParent,
        _task377Inputs* in_task377Inputs);
    ~TP377();
};
/*TP383: OMPTaskDirective*/
class TP383 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets384 : public darts::Codelet {
    public:
        TP383* myTP;
        TP383* inputsTPParent;
        _task383Inputs* taskInputs;
        _checkInCodelets384()
            : darts::Codelet()
        {
        }
        _checkInCodelets384(uint32_t dep, uint32_t res, TP383* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task383Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets529 : public darts::Codelet {
    public:
        TP383* myTP;
        TP383* inputsTPParent;
        _task383Inputs* taskInputs;
        _checkInCodelets529()
            : darts::Codelet()
        {
        }
        _checkInCodelets529(uint32_t dep, uint32_t res, TP383* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task383Inputs)
        {
        }
        void fire(void);
    };
    TP25* TPParent;
    TP383* controlTPParent;
    TP383* inputsTPParent;
    _task383Inputs* task383Inputs;
    TP_cilkmerge_par* TP384Ptr;
    int TP384_alreadyLaunched;
    _checkInCodelets384 checkInCodelets384;
    _checkInCodelets529 checkInCodelets529;
    TP383(int in_numThreads, int in_mainCodeletID, TP25* in_TPParent,
        _task383Inputs* in_task383Inputs);
    ~TP383();
};
/*TP501: OMPParallelDirective*/
class TP501 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets501 : public darts::Codelet {
    public:
        TP501* inputsTPParent;
        _barrierCodelets501()
            : darts::Codelet()
        {
        }
        _barrierCodelets501(uint32_t dep, uint32_t res, TP501* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets502 : public darts::Codelet {
    public:
        TP501* myTP;
        TP501* inputsTPParent;
        _checkInCodelets502()
            : darts::Codelet()
        {
        }
        _checkInCodelets502(uint32_t dep, uint32_t res, TP501* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP501* TPParent;
    TP501* controlTPParent;
    TP501* inputsTPParent;
    TP502* TP502Ptr;
    size_t TP502_alreadyLaunched;
    _task503Inputs** task503Inputs;
    _barrierCodelets501* barrierCodelets501;
    _checkInCodelets502* checkInCodelets502;
    TP501(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet);
    ~TP501();
};
/*TP502: OMPSingleDirective*/
class TP502 : public ompOMPSingleDirectiveTP {
public:
    class _checkInCodelets503 : public darts::Codelet {
    public:
        TP502* myTP;
        TP502* inputsTPParent;
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
    TP501* TPParent;
    TP502* controlTPParent;
    TP502* inputsTPParent;
    _task503Inputs** task503Inputs;
    _checkInCodelets503 checkInCodelets503;
    TP502(int in_numThreads, int in_mainCodeletID, TP501* in_TPParent);
    ~TP502();
};
/*TP503: OMPTaskDirective*/
class TP503 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets504 : public darts::Codelet {
    public:
        TP503* myTP;
        TP503* inputsTPParent;
        _task503Inputs* taskInputs;
        _checkInCodelets504()
            : darts::Codelet()
        {
        }
        _checkInCodelets504(uint32_t dep, uint32_t res, TP503* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task503Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets523 : public darts::Codelet {
    public:
        TP503* myTP;
        TP503* inputsTPParent;
        _task503Inputs* taskInputs;
        _checkInCodelets523()
            : darts::Codelet()
        {
        }
        _checkInCodelets523(uint32_t dep, uint32_t res, TP503* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task503Inputs)
        {
        }
        void fire(void);
    };
    TP502* TPParent;
    TP503* controlTPParent;
    TP503* inputsTPParent;
    _task503Inputs* task503Inputs;
    TP_cilksort_par* TP504Ptr;
    int TP504_alreadyLaunched;
    _checkInCodelets504 checkInCodelets504;
    _checkInCodelets523 checkInCodelets523;
    TP503(int in_numThreads, int in_mainCodeletID, TP502* in_TPParent,
        _task503Inputs* in_task503Inputs);
    ~TP503();
};
#endif
