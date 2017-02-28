#ifndef _UTILS_H_
#define _UTILS_H_

#include "darts.h"
#include "ompTP.h"
#include "tbb/concurrent_vector.h"
#include "tbb/scalable_allocator.h"
#include <algorithm>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <utility>
#include <vector>

#define USE_SPIN_CODELETS 1
#define USEINVOKE 1
#define MAXNUMTHREADS 48

#ifndef restrict
#define restrict __restrict
#endif

extern size_t ompNumThreads;

inline void omp_set_num_threads(unsigned long numThreadsToSet)
{
    ompNumThreads = numThreadsToSet;
}

inline int omp_get_num_threads() { return ompNumThreads; }

inline int omp_get_max_threads() { return ompNumThreads; }

inline int omp_get_num_procs() { return sysconf(_SC_NPROCESSORS_ONLN); }

inline double omp_get_wtime()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return ((double)(t.tv_sec) + ((double)(t.tv_usec) / 1000000));
}

typedef pthread_spinlock_t omp_lock_t;
inline void omp_init_lock(omp_lock_t* lock)
{
    pthread_spin_init(lock, PTHREAD_PROCESS_PRIVATE);
}

inline void omp_destroy_lock(omp_lock_t* lock) { pthread_spin_destroy(lock); }

inline void omp_set_lock(omp_lock_t* lock) { pthread_spin_lock(lock); }

inline void omp_unset_lock(omp_lock_t* lock) { pthread_spin_unlock(lock); }

void getOMPSchedulePolicy();

void getOMPNumThreads();

void getTPLoopThresholds();

void getNumTPs();

uint32_t getDARTSTPPolicy();

uint32_t getDARTSMCPolicy();

inline size_t getNumOfSUs() { return 1; }

inline size_t getNumOfCUs() { return numOfCUs; }

inline size_t getNumOfPUs() { return getNumOfCUs() + getNumOfSUs(); }

size_t getNumOfTPChildren(int numThreads);

size_t codeletsPerTP(size_t numThreads, size_t startCodeletID);

/*These vector<vector> and functions are used to keep track of the current C/C++
function where
the current codelet is. It's mostly used when an application has alloca() calls.
TODO: This is very bad in performance terms because a mutex is being used
instead of a concurrent
container. However, there are no LIFO containers in TBB. The other issue to have
in mind is that
codeletID is the codelet's ID in the DARTS application, not the thread ID or the
scheduler ID.
Therefore two different threads/schedulers may be executing different codelets
but with the same
ID; consequently, these functions need to be thread-safe*/
extern std::vector<std::vector<void*> > threadFunctionStack;
void pushFunctionStack(uint32_t codeletID, void* funcPtr);
void popFunctionStack(uint32_t codeletID);
void* topFunctionStack(uint32_t codeletID);

#endif
