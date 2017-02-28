#ifndef _brg_sha1_output_darts_h_
#define _brg_sha1_output_darts_h_
#ifndef __DARTS_
#define __DARTS_
#endif
#include "bots.h"
#include "brg_endian.h"
#include "brg_sha1.h"
#include "darts.h"
#include "ompTP.h"
#include "tbb/concurrent_vector.h"
#include "utils.h"
#include <limits.h>
#include <mutex>
#include <numa.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
void sha1(unsigned char hval[], const unsigned char data[], unsigned long len);
void rng_init(RNG_state* newstate, int seed);
void rng_spawn(RNG_state* mystate, RNG_state* newstate, int spawnnumber);
int rng_rand(RNG_state* mystate);
void sha1_hash(const unsigned char data[], unsigned long len, sha1_ctx ctx[1]);
char* rng_showstate(RNG_state* state, char* s);
int rng_nextrand(RNG_state* mystate);
void sha1_compile(sha1_ctx ctx[1]);
void sha1_begin(sha1_ctx ctx[1]);
void rng_showtype();
void sha1_end(unsigned char hval[], sha1_ctx ctx[1]);
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
#endif
