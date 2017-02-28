#ifndef _dc_output_darts_h_
#define _dc_output_darts_h_
#ifndef __DARTS_
#define __DARTS_
#endif
#include "adc.h"
#include "adc.output.darts.h"
#include "darts.h"
#include "macrodef.h"
#include "npbparams.h"
#include "ompTP.h"
#include "tbb/concurrent_vector.h"
#include "utils.h"
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <mutex>
#include <numa.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
int Verify(long long checksum, ADC_VIEW_PARS* adcpp);
int32 DC(ADC_VIEW_PARS* adcpp);
int main(int argc, char* argv[]);
class TP3440;
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
/*TP3440: OMPParallelDirective*/
class TP3440 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets3440 : public darts::Codelet {
    public:
        TP3440* inputsTPParent;
        _barrierCodelets3440()
            : darts::Codelet()
        {
        }
        _barrierCodelets3440(uint32_t dep, uint32_t res, TP3440* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets3442 : public darts::Codelet {
    public:
        TP3440* myTP;
        TP3440* inputsTPParent;
        _checkInCodelets3442()
            : darts::Codelet()
        {
        }
        _checkInCodelets3442(uint32_t dep, uint32_t res, TP3440* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP3440* TPParent;
    TP3440* controlTPParent;
    TP3440* inputsTPParent;
    ADC_VIEW_PARS** adcpp_darts3440; /*OMP_SHARED - INPUT*/
    int32* itsk_darts3440 /*OMP_PRIVATE - INPUT*/;
    PAR_VIEW_ST** pvstp_darts3440; /*OMP_SHARED - INPUT*/
    ADC_VIEW_CNTL** adccntlp_darts3440 /*VARIABLE*/;
    int* itimer_darts3440 /*VARIABLE*/;
    double* tm0_darts3440 /*VARIABLE*/;
    _barrierCodelets3440* barrierCodelets3440;
    _checkInCodelets3442* checkInCodelets3442;
    TP3440(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet,
        ADC_VIEW_PARS** in_adcpp, PAR_VIEW_ST** in_pvstp);
    ~TP3440();
};
#endif
