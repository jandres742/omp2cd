#ifndef _sequence_output_darts_h_
#define _sequence_output_darts_h_
#ifndef __DARTS_
#define __DARTS_
#endif
#include "alignment.h"
#include "alignment.output.darts.h"
#include "bots.h"
#include "darts.h"
#include "ompTP.h"
#include "sequence.h"
#include "sequence_extern.h"
#include "tbb/concurrent_vector.h"
#include "utils.h"
#include <ctype.h>
#include <limits.h>
#include <mutex>
#include <numa.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
char* get_seq(char* sname, int* len, char* chartab, FILE* fin);
void alloc_aln(int nseqs);
void encode(char* seq, char* naseq, int l);
void fill_chartab(char* chartab);
int readseqs(char* filename);
unsigned long strlcpy(char* dst, const char* src, size_t siz);
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
