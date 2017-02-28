#ifndef _linkCells_output_darts_h_
#define _linkCells_output_darts_h_
#ifndef __DARTS_
#define __DARTS_
#endif
#include "CoMDTypes.h"
#include "darts.h"
#include "decomposition.h"
#include "decomposition.output.darts.h"
#include "linkCells.h"
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
#include <math.h>
#include <mutex>
#include <numa.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
LinkCell* initLinkCells(const Domain* domain, real_t cutoff);
void destroyLinkCells(LinkCell** boxes);
int getBoxFromTuple(LinkCell* boxes, int ix, int iy, int iz);
void moveAtom(LinkCell* boxes, Atoms* atoms, int iId, int iBox, int jBox);
int maxOccupancy(LinkCell* boxes);
void updateLinkCells(LinkCell* boxes, Atoms* atoms);
int getNeighborBoxes(LinkCell* boxes, int iBox, int* nbrBoxes);
void putAtomInBox(LinkCell* boxes, Atoms* atoms, const int gid, const int iType, const real_t x,
    const real_t y, const real_t z, const real_t px, const real_t py, const real_t pz);
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
