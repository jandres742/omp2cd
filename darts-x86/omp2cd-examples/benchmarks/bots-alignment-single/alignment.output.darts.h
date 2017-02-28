#ifndef _alignment_output_darts_h_
#define _alignment_output_darts_h_
#ifndef __DARTS_
#define __DARTS_
#endif
#include "alignment.h"
#include "bots.h"
#include "darts.h"
#include "ompTP.h"
#include "param.h"
#include "sequence.h"
#include "sequence.output.darts.h"
#include "tbb/concurrent_vector.h"
#include "utils.h"
#include <libgen.h>
#include <limits.h>
#include <math.h>
#include <mutex>
#include <numa.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
void align_seq_init();
void align();
int align_verify();
void align_init();
void pairalign_init(char* filename);
void del(int k, int* print_ptr, int* last_print, int* displ);
int pairalign();
void align_seq();
int calc_score(int iat, int jat, int v1, int v2, int seq1, int seq2);
void align_end();
void init_matrix();
int pairalign_seq();
void forward_pass(
    char* ia, char* ib, int n, int m, int* se1, int* se2, int* maxscore, int g, int gh);
int get_matrix(int* matptr, int* xref, int scale);
void reverse_pass(
    char* ia, char* ib, int se1, int se2, int* sb1, int* sb2, int maxscore, int g, int gh);
int diff(int A, int B, int M, int N, int tb, int te, int* print_ptr, int* last_print, int* displ,
    int seq1, int seq2, int g, int gh);
void add(int v, int* print_ptr, int* last_print, int* displ);
double tracepath(int tsb1, int tsb2, int* print_ptr, int* displ, int seq1, int seq2);
class TP1104;
class TP1106;
class TP1107;
class TP1129;
/*Class containing the inputs passed to task1148*/
class _task1148Inputs {
public:
    double gg_darts1148 /*OMP_PRIVATE*/;
    int i_darts1148 /*OMP_FIRSTPRIVATE*/;
    int len1_darts1148 /*OMP_FIRSTPRIVATE*/;
    int len2_darts1148 /*OMP_PRIVATE*/;
    int m_darts1148 /*OMP_FIRSTPRIVATE*/;
    double mm_score_darts1148 /*OMP_PRIVATE*/;
    int n_darts1148 /*OMP_FIRSTPRIVATE*/;
    int si_darts1148 /*OMP_FIRSTPRIVATE*/;
    int sj_darts1148 /*OMP_FIRSTPRIVATE*/;
    char c_darts1148; /*VARIABLE*/
    int displ_darts1148[10001]; /*VARIABLE*/
    int g_darts1148; /*VARIABLE*/
    int gh_darts1148; /*VARIABLE*/
    int last_print_darts1148; /*VARIABLE*/
    int maxscore_darts1148; /*VARIABLE*/
    int print_ptr_darts1148; /*VARIABLE*/
    int sb1_darts1148; /*VARIABLE*/
    int sb2_darts1148; /*VARIABLE*/
    int se1_darts1148; /*VARIABLE*/
    int se2_darts1148; /*VARIABLE*/
    int seq1_darts1148; /*VARIABLE*/
    int seq2_darts1148; /*VARIABLE*/
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task1148Inputs() {}
    _task1148Inputs(int* in_i, int* in_len1, int* in_m, int* in_n, int* in_si, int* in_sj,
        darts::Codelet* in_nextSyncCodelet)
        : taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
        this->i_darts1148 = *in_i;
        this->len1_darts1148 = *in_len1;
        this->m_darts1148 = *in_m;
        this->n_darts1148 = *in_n;
        this->si_darts1148 = *in_si;
        this->sj_darts1148 = *in_sj;
    }
    ~_task1148Inputs() {}
};
class TP1148;
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
/*TP1104: OMPParallelDirective*/
class TP1104 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets1104 : public darts::Codelet {
    public:
        TP1104* inputsTPParent;
        _barrierCodelets1104()
            : darts::Codelet()
        {
        }
        _barrierCodelets1104(uint32_t dep, uint32_t res, TP1104* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets1106 : public darts::Codelet {
    public:
        TP1104* myTP;
        TP1104* inputsTPParent;
        _checkInCodelets1106()
            : darts::Codelet()
        {
        }
        _checkInCodelets1106(uint32_t dep, uint32_t res, TP1104* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets1106 : public darts::Codelet {
    public:
        TP1104* inputsTPParent;
        _barrierCodelets1106()
            : darts::Codelet()
        {
        }
        _barrierCodelets1106(uint32_t dep, uint32_t res, TP1104* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP1104* TPParent;
    TP1104* controlTPParent;
    TP1104* inputsTPParent;
    TP1106* TP1106Ptr;
    size_t TP1106_alreadyLaunched;
    _task1148Inputs** task1148Inputs;
    _barrierCodelets1104* barrierCodelets1104;
    _checkInCodelets1106* checkInCodelets1106;
    _barrierCodelets1106* barrierCodelets1106;
    TP1104(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet);
    ~TP1104();
};
/*TP1106: OMPSingleDirective*/
class TP1106 : public ompOMPSingleDirectiveTP {
public:
    class _checkInCodelets1108 : public darts::Codelet {
    public:
        TP1106* myTP;
        TP1106* inputsTPParent;
        _checkInCodelets1108()
            : darts::Codelet()
        {
        }
        _checkInCodelets1108(uint32_t dep, uint32_t res, TP1106* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets1107 : public darts::Codelet {
    public:
        TP1106* myTP;
        TP1106* inputsTPParent;
        _checkInCodelets1107()
            : darts::Codelet()
        {
        }
        _checkInCodelets1107(uint32_t dep, uint32_t res, TP1106* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets1110 : public darts::Codelet {
    public:
        TP1106* myTP;
        TP1106* inputsTPParent;
        _checkInCodelets1110()
            : darts::Codelet()
        {
        }
        _checkInCodelets1110(uint32_t dep, uint32_t res, TP1106* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP1104* TPParent;
    TP1106* controlTPParent;
    TP1106* inputsTPParent;
    int i_darts1106;
    int len1_darts1106;
    int m_darts1106;
    int n_darts1106;
    int si_darts1106;
    int sj_darts1106;
    char c_darts1106 /*VARIABLE*/;
    unsigned int TP1107_LoopCounter;
    unsigned int* TP1107_LoopCounterPerThread;
    tbb::concurrent_vector<TP1107*> TP1107PtrVec;
    _task1148Inputs** task1148Inputs;
    _checkInCodelets1108 checkInCodelets1108;
    _checkInCodelets1107 checkInCodelets1107;
    _checkInCodelets1110 checkInCodelets1110;
    TP1106(int in_numThreads, int in_mainCodeletID, TP1104* in_TPParent);
    ~TP1106();
};
/*TP1107: ForStmt*/
class TP1107 : public ompTP {
public:
    class _checkInCodelets1112 : public darts::Codelet {
    public:
        TP1107* myTP;
        TP1106* inputsTPParent;
        _checkInCodelets1112()
            : darts::Codelet()
        {
        }
        _checkInCodelets1112(uint32_t dep, uint32_t res, TP1107* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets1129 : public darts::Codelet {
    public:
        TP1107* myTP;
        TP1106* inputsTPParent;
        _checkInCodelets1129()
            : darts::Codelet()
        {
        }
        _checkInCodelets1129(uint32_t dep, uint32_t res, TP1107* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets1133 : public darts::Codelet {
    public:
        TP1107* myTP;
        TP1106* inputsTPParent;
        _checkInCodelets1133()
            : darts::Codelet()
        {
        }
        _checkInCodelets1133(uint32_t dep, uint32_t res, TP1107* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP1106* TPParent;
    TP1107* controlTPParent;
    TP1106* inputsTPParent;
    TP1107** ptrToThisTP;
    unsigned int TP1129_LoopCounter;
    unsigned int* TP1129_LoopCounterPerThread;
    tbb::concurrent_vector<TP1129*> TP1129PtrVec;
    _checkInCodelets1112 checkInCodelets1112;
    _checkInCodelets1129 checkInCodelets1129;
    _checkInCodelets1133 checkInCodelets1133;
    TP1107(int in_numThreads, int in_mainCodeletID, TP1106* in_TPParent, TP1106* in_inputsTPParent,
        TP1107** in_ptrToThisTP);
    ~TP1107();
};
/*TP1129: ForStmt*/
class TP1129 : public ompTP {
public:
    class _checkInCodelets1135 : public darts::Codelet {
    public:
        TP1129* myTP;
        TP1106* inputsTPParent;
        _checkInCodelets1135()
            : darts::Codelet()
        {
        }
        _checkInCodelets1135(uint32_t dep, uint32_t res, TP1129* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets1138 : public darts::Codelet {
    public:
        TP1129* myTP;
        TP1106* inputsTPParent;
        _checkInCodelets1138()
            : darts::Codelet()
        {
        }
        _checkInCodelets1138(uint32_t dep, uint32_t res, TP1129* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets1141 : public darts::Codelet {
    public:
        TP1129* myTP;
        TP1106* inputsTPParent;
        _checkInCodelets1141()
            : darts::Codelet()
        {
        }
        _checkInCodelets1141(uint32_t dep, uint32_t res, TP1129* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets1148 : public darts::Codelet {
    public:
        TP1129* myTP;
        TP1106* inputsTPParent;
        _checkInCodelets1148()
            : darts::Codelet()
        {
        }
        _checkInCodelets1148(uint32_t dep, uint32_t res, TP1129* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP1107* TPParent;
    TP1129* controlTPParent;
    TP1106* inputsTPParent;
    TP1129** ptrToThisTP;
    _checkInCodelets1135 checkInCodelets1135;
    _checkInCodelets1138 checkInCodelets1138;
    _checkInCodelets1141 checkInCodelets1141;
    _checkInCodelets1148 checkInCodelets1148;
    TP1129(int in_numThreads, int in_mainCodeletID, TP1107* in_TPParent, TP1106* in_inputsTPParent,
        TP1129** in_ptrToThisTP);
    ~TP1129();
};
/*TP1148: OMPTaskDirective*/
class TP1148 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets1150 : public darts::Codelet {
    public:
        TP1148* myTP;
        TP1148* inputsTPParent;
        _task1148Inputs* taskInputs;
        _checkInCodelets1150()
            : darts::Codelet()
        {
        }
        _checkInCodelets1150(uint32_t dep, uint32_t res, TP1148* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task1148Inputs)
        {
        }
        void fire(void);
    };
    TP1129* TPParent;
    TP1148* controlTPParent;
    TP1148* inputsTPParent;
    _task1148Inputs* task1148Inputs;
    char c_darts1148 /*VARIABLE*/;
    int displ_darts1148[10001] /*VARIABLE*/;
    int g_darts1148 /*VARIABLE*/;
    int gh_darts1148 /*VARIABLE*/;
    int last_print_darts1148 /*VARIABLE*/;
    int maxscore_darts1148 /*VARIABLE*/;
    int print_ptr_darts1148 /*VARIABLE*/;
    int sb1_darts1148 /*VARIABLE*/;
    int sb2_darts1148 /*VARIABLE*/;
    int se1_darts1148 /*VARIABLE*/;
    int se2_darts1148 /*VARIABLE*/;
    int seq1_darts1148 /*VARIABLE*/;
    int seq2_darts1148 /*VARIABLE*/;
    _checkInCodelets1150 checkInCodelets1150;
    TP1148(int in_numThreads, int in_mainCodeletID, TP1129* in_TPParent,
        _task1148Inputs* in_task1148Inputs);
    ~TP1148();
};
#endif
