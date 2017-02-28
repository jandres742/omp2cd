#include "performance.output.darts.h"
using namespace darts;
using namespace std;
char* counterName[3] = { "reduce", "send", "recv" };
static int oneMB = 1024 * 1024;
static Counter perfCounter[3];
static TimerGlobal perfGlobal;
static Timers perfTimer[14];
char* timerName[14]
    = { "total", "loop", "  pre", "    readh", "    sparse", "  sp2Loop", "    norm", "    x2",
        "    xadd", "    xset", "    exchange", "    reduceComm", "  post", "    dense" };
static double getTick();
static uint64_t getTime();
static void performanceStats();
/*Function: profileStart, ID: 75*/
void profileStart(const enum TimerHandle handle)
{
    /*profileStart:75*/
    /*CompoundStmt:1749*/
    perfTimer[handle].start = getTime();
}
/*Function: profileStop, ID: 76*/
void profileStop(const enum TimerHandle handle)
{
    /*profileStop:76*/
    /*CompoundStmt:1754*/
    perfTimer[handle].count += 1;
    uint64_t delta = getTime() - perfTimer[handle].start;
    perfTimer[handle].total += delta;
    perfTimer[handle].elapsed += delta;
}
/*Function: getElapsedTime, ID: 77*/
double getElapsedTime(const enum TimerHandle handle)
{
    /*getElapsedTime:77*/
    /*CompoundStmt:1769*/
    double etime = getTick() * (double)perfTimer[handle].elapsed;
    perfTimer[handle].elapsed = 0;
    return etime;
}
/*Function: printPerformanceResults, ID: 78*/
void printPerformanceResults(int nGlobalAtoms, int printRate)
{
    /*printPerformanceResults:78*/
    /*CompoundStmt:1780*/
    performanceStats();
    if (!printRank())
        return;
    fprintf(stdout, "\n\nCounters for Rank %d\n", getMyRank());
    fprintf(stdout, "       Counter          Calls    Avg/Call(MB)         Total(MB)   \n");
    fprintf(stdout, "_________________________________________________________________\n");
    for (int ii = 0; ii < numberOfCounters; ++ii) {
        double totalSize = (double)perfCounter[ii].total / (double)oneMB;
        if (perfCounter[ii].count > 0)
            fprintf(stdout, "%-16s%12lu     %12.4f      %12.4f\n", counterName[ii],
                perfCounter[ii].count, totalSize / (double)perfCounter[ii].count, totalSize);
    }
    fprintf(stdout, "\nCounter Statistics Across %d Ranks:\n", getNRanks());
    fprintf(stdout,
        "        Counter      Rank: Min(MB)        Rank: Max(MB)       Avg(MB)      Stdev(MB)\n");
    fprintf(stdout, "______________________________________________________________________________"
                    "_________\n");
    for (int ii = 0; ii < numberOfCounters; ++ii) {
        if (perfCounter[ii].count > 0)
            fprintf(stdout, "%-16s%6d:%12.4f  %6d:%12.4f  %12.4f  %12.4f\n", counterName[ii],
                perfCounter[ii].minRank, perfCounter[ii].minValue / oneMB, perfCounter[ii].maxRank,
                perfCounter[ii].maxValue / oneMB, perfCounter[ii].average / oneMB,
                perfCounter[ii].stdev / oneMB);
    }
    double tick = getTick();
    double loopTime = perfTimer[loopTimer].total * tick;
    fprintf(stdout, "\n\nTimings for Rank %d\n", getMyRank());
    fprintf(stdout, "        Timer        # Calls    Avg/Call (s)   Total (s)    %% Loop\n");
    fprintf(stdout, "___________________________________________________________________\n");
    for (int ii = 0; ii < numberOfTimers; ++ii) {
        double totalTime = perfTimer[ii].total * tick;
        if (perfTimer[ii].count > 0)
            fprintf(stdout, "%-16s%12lu     %8.4f      %8.4f    %8.2f\n", timerName[ii],
                perfTimer[ii].count, totalTime / (double)perfTimer[ii].count, totalTime,
                totalTime / loopTime * 100.);
    }
    fprintf(stdout, "\nTiming Statistics Across %d Ranks:\n", getNRanks());
    fprintf(
        stdout, "        Timer        Rank: Min(s)       Rank: Max(s)      Avg(s)    Stdev(s)\n");
    fprintf(
        stdout, "_____________________________________________________________________________\n");
    for (int ii = 0; ii < numberOfTimers; ++ii) {
        if (perfTimer[ii].count > 0)
            fprintf(stdout, "%-16s%6d:%10.4f  %6d:%10.4f  %10.4f  %10.4f\n", timerName[ii],
                perfTimer[ii].minRank, perfTimer[ii].minValue * tick, perfTimer[ii].maxRank,
                perfTimer[ii].maxValue * tick, perfTimer[ii].average * tick,
                perfTimer[ii].stdev * tick);
    }
}
/*Function: printPerformanceResultsYaml, ID: 79*/
void printPerformanceResultsYaml(FILE* file)
{
    /*printPerformanceResultsYaml:79*/
    /*CompoundStmt:1905*/
    if (!printRank())
        return;
    fprintf(file, "\nCounter Performance Results:\n");
    fprintf(file, "  TotalRanks: %d\n", getNRanks());
    fprintf(file, "  ReportingSizeUnits: MB\n");
    fprintf(file, "Counter Performance Results For Rank %d:\n", getMyRank());
    for (int ii = 0; ii < numberOfCounters; ii++) {
        if (perfCounter[ii].count > 0) {
            double totalSize = perfCounter[ii].total / oneMB;
            fprintf(file, "  Counter: %s\n", counterName[ii]);
            fprintf(file, "    CallCount:  %lu\n", perfCounter[ii].count);
            fprintf(file, "    AvgPerCall: %12.4f\n", totalSize / (double)perfCounter[ii].count);
            fprintf(file, "    Total:      %12.4f\n", totalSize);
        }
    }
    fprintf(file, "Counter Performance Results Across Ranks:\n");
    for (int ii = 0; ii < numberOfCounters; ii++) {
        if (perfCounter[ii].count > 0) {
            fprintf(file, "  Counter: %s\n", counterName[ii]);
            fprintf(file, "    MinRank: %d\n", perfCounter[ii].minRank);
            fprintf(file, "    MinTime: %12.4f\n", perfCounter[ii].minValue / oneMB);
            fprintf(file, "    MaxRank: %d\n", perfCounter[ii].maxRank);
            fprintf(file, "    MaxTime: %12.4f\n", perfCounter[ii].maxValue / oneMB);
            fprintf(file, "    AvgTime: %12.4f\n", perfCounter[ii].average / oneMB);
            fprintf(file, "    StdevTime: %12.4f\n", perfCounter[ii].stdev / oneMB);
        }
    }
    double tick = getTick();
    double loopTime = perfTimer[loopTimer].total * tick;
    fprintf(file, "\nTimer Performance Results:\n");
    fprintf(file, "  TotalRanks: %d\n", getNRanks());
    fprintf(file, "  ReportingTimeUnits: seconds\n");
    fprintf(file, "Timer Performance Results For Rank %d:\n", getMyRank());
    for (int ii = 0; ii < numberOfTimers; ii++) {
        if (perfTimer[ii].count > 0) {
            double totalTime = perfTimer[ii].total * tick;
            fprintf(file, "  Timer: %s\n", timerName[ii]);
            fprintf(file, "    CallCount:  %lu\n", perfTimer[ii].count);
            fprintf(file, "    AvgPerCall: %8.4f\n", totalTime / (double)perfTimer[ii].count);
            fprintf(file, "    Total:      %8.4f\n", totalTime);
            fprintf(file, "    PercentLoop: %8.2f\n", totalTime / loopTime * 100);
        }
    }
    fprintf(file, "Timer Performance Results Across Ranks:\n");
    for (int ii = 0; ii < numberOfTimers; ii++) {
        if (perfTimer[ii].count > 0) {
            fprintf(file, "  Timer: %s\n", timerName[ii]);
            fprintf(file, "    MinRank: %d\n", perfTimer[ii].minRank);
            fprintf(file, "    MinTime: %8.4f\n", perfTimer[ii].minValue * tick);
            fprintf(file, "    MaxRank: %d\n", perfTimer[ii].maxRank);
            fprintf(file, "    MaxTime: %8.4f\n", perfTimer[ii].maxValue * tick);
            fprintf(file, "    AvgTime: %8.4f\n", perfTimer[ii].average * tick);
            fprintf(file, "    StdevTime: %8.4f\n", perfTimer[ii].stdev * tick);
        }
    }
    fprintf(file, "\nPerformance Global Update Rates:\n");
    fprintf(file, "  AtomUpdateRate:\n");
    fprintf(file, "    AverageRate: %6.2f\n", perfGlobal.atomRate);
    fprintf(file, "    Units: us/atom/task\n");
    fprintf(file, "  AllAtomUpdateRate:\n");
    fprintf(file, "    AverageRate: %6.2f\n", perfGlobal.atomAllRate);
    fprintf(file, "    Units: us/atom\n");
    fprintf(file, "  AtomRate:\n");
    fprintf(file, "    AverageRate: %6.2f\n", perfGlobal.atomsPerUSec);
    fprintf(file, "    Units: atoms/us\n");
    fprintf(file, "\n");
}
/*Function: getTime, ID: 72*/
static uint64_t getTime()
{
    /*getTime:72*/
    /*CompoundStmt:2057*/
    struct timeval ptime;
    uint64_t t = 0;
    gettimeofday(&ptime, (struct timezone*)((void*)0));
    t = ((uint64_t)1000000) * (uint64_t)ptime.tv_sec + (uint64_t)ptime.tv_usec;
    return t;
}
/*Function: getTick, ID: 73*/
static double getTick()
{
    /*getTick:73*/
    /*CompoundStmt:2073*/
    double seconds_per_cycle = 9.9999999999999995E-7;
    return seconds_per_cycle;
}
/*Function: performanceStats, ID: 74*/
void performanceStats()
{
    /*performanceStats:74*/
    /*CompoundStmt:2077*/
    int numberOfBuffers = (numberOfTimers > numberOfCounters) ? numberOfTimers : numberOfCounters;
    double sendBuf[numberOfBuffers], recvBuf[numberOfBuffers];
    for (int ii = 0; ii < numberOfTimers; ii++)
        sendBuf[ii] = (double)perfTimer[ii].total;
    addDoubleParallel(sendBuf, recvBuf, numberOfTimers);
    for (int ii = 0; ii < numberOfTimers; ii++)
        perfTimer[ii].average = recvBuf[ii] / (double)getNRanks();
    RankReduceData reduceSendBuf[numberOfBuffers], reduceRecvBuf[numberOfBuffers];
    for (int ii = 0; ii < numberOfTimers; ii++) {
        reduceSendBuf[ii].val = (double)perfTimer[ii].total;
        reduceSendBuf[ii].rank = getMyRank();
    }
    minRankDoubleParallel(reduceSendBuf, reduceRecvBuf, numberOfTimers);
    for (int ii = 0; ii < numberOfTimers; ii++) {
        perfTimer[ii].minValue = reduceRecvBuf[ii].val;
        perfTimer[ii].minRank = reduceRecvBuf[ii].rank;
    }
    maxRankDoubleParallel(reduceSendBuf, reduceRecvBuf, numberOfTimers);
    for (int ii = 0; ii < numberOfTimers; ii++) {
        perfTimer[ii].maxValue = reduceRecvBuf[ii].val;
        perfTimer[ii].maxRank = reduceRecvBuf[ii].rank;
    }
    for (int ii = 0; ii < numberOfTimers; ii++) {
        double temp = (double)perfTimer[ii].total - perfTimer[ii].average;
        sendBuf[ii] = temp * temp;
    }
    addDoubleParallel(sendBuf, recvBuf, numberOfTimers);
    for (int ii = 0; ii < numberOfTimers; ii++) {
        perfTimer[ii].stdev = sqrt(recvBuf[ii] / (double)getNRanks());
    }
    for (int ii = 0; ii < numberOfCounters; ii++)
        sendBuf[ii] = (double)perfCounter[ii].total;
    addDoubleParallel(sendBuf, recvBuf, numberOfCounters);
    for (int ii = 0; ii < numberOfCounters; ii++)
        perfCounter[ii].average = recvBuf[ii] / (double)getNRanks();
    for (int ii = 0; ii < numberOfCounters; ii++) {
        reduceSendBuf[ii].val = (double)perfCounter[ii].total;
        reduceSendBuf[ii].rank = getMyRank();
    }
    minRankDoubleParallel(reduceSendBuf, reduceRecvBuf, numberOfCounters);
    for (int ii = 0; ii < numberOfCounters; ii++) {
        perfCounter[ii].minValue = reduceRecvBuf[ii].val;
        perfCounter[ii].minRank = reduceRecvBuf[ii].rank;
    }
    maxRankDoubleParallel(reduceSendBuf, reduceRecvBuf, numberOfCounters);
    for (int ii = 0; ii < numberOfCounters; ii++) {
        perfCounter[ii].maxValue = reduceRecvBuf[ii].val;
        perfCounter[ii].maxRank = reduceRecvBuf[ii].rank;
    }
    for (int ii = 0; ii < numberOfCounters; ii++) {
        double temp = (double)perfCounter[ii].total - perfCounter[ii].average;
        sendBuf[ii] = temp * temp;
    }
    addDoubleParallel(sendBuf, recvBuf, numberOfCounters);
    for (int ii = 0; ii < numberOfCounters; ii++) {
        perfCounter[ii].stdev = sqrt(recvBuf[ii] / (double)getNRanks());
    }
}
/*Function: addToCounter, ID: 83*/
void addToCounter(const enum CounterHandle handle, int size)
{
    /*addToCounter:83*/
    /*CompoundStmt:2277*/
    perfCounter[handle].count += 1;
    perfCounter[handle].total += size;
}
