#ifndef _haloExchange_output_darts_h_
#define _haloExchange_output_darts_h_
#ifndef __DARTS_
#define __DARTS_
#endif
#include "CoMDTypes.h"
#include "darts.h"
#include "decomposition.h"
#include "decomposition.output.darts.h"
#include "eam.h"
#include "eam.output.darts.h"
#include "haloExchange.h"
#include "linkCells.h"
#include "linkCells.output.darts.h"
#include "memUtils.h"
#include "ompTP.h"
#include "parallel.h"
#include "parallel.output.darts.h"
#include "performanceTimers.h"
#include "performanceTimers.output.darts.h"
#include "tbb/concurrent_vector.h"
#include "utils.h"
#include <cassert>
#include <limits.h>
#include <mutex>
#include <numa.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
HaloExchange* initAtomHaloExchange(Domain* domain, LinkCell* boxes);
HaloExchange* initForceHaloExchange(Domain* domain, LinkCell* boxes);
void destroyHaloExchange(HaloExchange** haloExchange);
void sortAtomsInCell(Atoms* atoms, LinkCell* boxes, int iBox);
void haloExchange(HaloExchange* haloExchange, void* data);
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
