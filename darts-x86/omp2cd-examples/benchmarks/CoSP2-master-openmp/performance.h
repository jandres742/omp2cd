/// \file
/// Performance timer and counter functions.
#ifndef __PERFORMANCE_H_
#define __PERFORMANCE_H_

#include <stdio.h>

#include <inttypes.h>

/// Timer handles
enum TimerHandle{
   totalTimer, 
   loopTimer,
   preTimer,
   readhTimer,
   dense2sparseTimer,
   sp2LoopTimer, 
   normTimer,
   x2Timer,
   xaddTimer,
   xsetTimer,
   exchangeTimer,
   reduceCommTimer,
   postTimer,
   sparse2denseTimer,
   numberOfTimers};

/// Use the startTimer and stopTimer macros for timers in code regions
/// that may be performance sensitive.  These can be compiled away by
/// defining NTIMING.  If you are placing a timer anywere outside of a
/// tight loop, consider calling profileStart and profileStop instead.
///
/// Place calls as follows to collect time for code pieces.
/// Time is collected everytime this portion of code is executed. 
///
///     ...
///     startTimer(computeForceTimer);
///     computeForce(sim);
///     stopTimer(computeForceTimer);
///     ...
///
#ifndef NTIMING
#define startTimer(handle)    \
   do                         \
{                          \
   profileStart(handle);   \
} while(0)
#define stopTimer(handle)     \
   do                         \
{                          \
   profileStop(handle);    \
} while(0)
#else
#define startTimer(handle)
#define stopTimer(handle)
#endif

/// Use profileStart and profileStop only for timers that should *never*
/// be turned off.  Typically this means they are outside the main
/// simulation loop.  If the timer is inside the main loop use
/// startTimer and stopTimer instead.
void profileStart(const enum TimerHandle handle);
void profileStop(const enum TimerHandle handle);

/// Use to get elapsed time (lap timer).
double getElapsedTime(const enum TimerHandle handle);

/// Print timing results.
void printPerformanceResults(int nGlobalAtoms, int printRate);

/// Print timing results to Yaml file
void printPerformanceResultsYaml(FILE* file);


/// Counter handles
enum CounterHandle {
   reduceCounter,
   sendCounter,
   recvCounter,
   numberOfCounters};
   
typedef struct TimersSt
{
   uint64_t start;     //!< call start time
   uint64_t total;     //!< current total time
   uint64_t count;     //!< current call count
   uint64_t elapsed;   //!< lap time
 
   int minRank;        //!< rank with min value
   int maxRank;        //!< rank with max value

   double minValue;    //!< min over ranks
   double maxValue;    //!< max over ranks
   double average;     //!< average over ranks
   double stdev;       //!< stdev across ranks
} Timers;


typedef struct TimerGlobalSt
{
   double atomRate;       //!< average time (us) per atom per rank 
   double atomAllRate;    //!< average time (us) per atom
   double atomsPerUSec;   //!< average atoms per time (us)
} TimerGlobal;

typedef struct CounterSt
{
   uint64_t total;     //!< sum of values
   uint64_t count;     //!< number of values

   int minRank;        //!< rank with min value
   int maxRank;        //!< rank with max value

   double minValue;    //!< min over ranks
   double maxValue;    //!< max over ranks
   double average;     //!< average over ranks
   double stdev;       //!< stdev across ranks
} Counter;

/// Use the collectCounter macro to collect counts and sizes for messages.
/// Place call as follows to collect counts and size for messaging.
///
///     ...
///     nRecvP = sendReceiveParallel(sendBufM, nSendM, nbrRankM, recvBufP, 
///                                  haloExchange->bufCapacity, nbrRankP);
///     collectCounter(haloSendCounter, nSendM);
///     collectCounter(haloRecvCounter, nRecvP);
///     ...
///
#ifndef NCOUNTING
#define collectCounter(handle, size)    \
   do                         \
{                          \
   addToCounter(handle, size);   \
} while(0)
#else
#define collectCounter(handle, size)
#endif

/// Add number and size to counter
void addToCounter(const enum CounterHandle handle, int size);

#endif
