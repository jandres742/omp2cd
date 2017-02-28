#ifndef _common_output_darts_h_
#define _common_output_darts_h_
#ifndef __DARTS_
#define __DARTS_
#endif
#include "common.h"
#include "darts.h"
#include "ompTP.h"
#include "tbb/concurrent_vector.h"
#include "utils.h"
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
double getclock();
void delay(int delaylength);
void array_delay(int delaylength, double* a);
void intitest(char* name);
void finalisetest(char* name);
void finalisereference(char* name);
void usage(char* argv[]);
unsigned long getinnerreps(void (*test)(void));
void printfooter(char* name, double testtime, double testsd, double referencetime, double refsd);
void reference(char* name, void (*refer)(void));
void parse_args(int argc, char* argv[]);
void printreferencefooter(char* name, double referencetime, double referencesd);
void printheader(char* name);
void benchmark(char* name, void (*test)(void));
void finalise();
int returnfalse();
void initreference(char* name);
int getdelaylengthfromtime(double delaytime);
void stats(double* mtp, double* sdp);
void init(int argc, char** argv);
class TP284;
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
/*TP284: OMPParallelDirective*/
class TP284 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets284 : public darts::Codelet {
    public:
        TP284* inputsTPParent;
        _barrierCodelets284()
            : darts::Codelet()
        {
        }
        _barrierCodelets284(uint32_t dep, uint32_t res, TP284* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets286 : public darts::Codelet {
    public:
        TP284* myTP;
        TP284* inputsTPParent;
        _checkInCodelets286()
            : darts::Codelet()
        {
        }
        _checkInCodelets286(uint32_t dep, uint32_t res, TP284* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP284* TPParent;
    TP284* controlTPParent;
    TP284* inputsTPParent;
    size_t TP286_alreadyLaunched;
    _barrierCodelets284* barrierCodelets284;
    _checkInCodelets286* checkInCodelets286;
    TP284(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet);
    ~TP284();
};
#endif
