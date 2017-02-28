#include "performanceTimers.output.darts.h"
using namespace darts;
using namespace std;
char* aaaa;
static TimerGlobal perfGlobal;
static Timers perfTimer[11];
char* timerName[11] = { "total", "loop", "timestep", "  position", "  velocity", "  redistribute",
    "    atomHalo", "  force", "    eamHalo", "commHalo", "commReduce" };
static void timerStats();
static double getTick();
static uint64_t getTime();
/*Function: profileStart, ID: 156*/
void profileStart(const enum TimerHandle handle)
{
    /*profileStart:156*/
    /*CompoundStmt:1953*/
    perfTimer[handle].start = getTime();
}
/*Function: profileStop, ID: 157*/
void profileStop(const enum TimerHandle handle)
{
    /*profileStop:157*/
    /*CompoundStmt:1958*/
    perfTimer[handle].count += 1;
    uint64_t delta = getTime() - perfTimer[handle].start;
    perfTimer[handle].total += delta;
    perfTimer[handle].elapsed += delta;
}
/*Function: getElapsedTime, ID: 158*/
double getElapsedTime(const enum TimerHandle handle)
{
    /*getElapsedTime:158*/
    /*CompoundStmt:1973*/
    double etime = getTick() * (double)perfTimer[handle].elapsed;
    perfTimer[handle].elapsed = 0;
    return etime;
}
/*Function: printPerformanceResults, ID: 159*/
void printPerformanceResults(int nGlobalAtoms, int printRate)
{
    /*printPerformanceResults:159*/
    /*CompoundStmt:1984*/
    timerStats();
    if (!printRank())
        return;
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
    double atomsPerTask = nGlobalAtoms / (real_t)getNRanks();
    perfGlobal.atomRate = perfTimer[timestepTimer].average * tick * 1.0E+6
        / (atomsPerTask * perfTimer[timestepTimer].count * printRate);
    perfGlobal.atomAllRate = perfTimer[timestepTimer].average * tick * 1.0E+6
        / (nGlobalAtoms * perfTimer[timestepTimer].count * printRate);
    perfGlobal.atomsPerUSec = 1. / perfGlobal.atomAllRate;
    fprintf(stdout, "\n---------------------------------------------------\n");
    fprintf(stdout, " Average atom update rate:     %6.2f us/atom/task\n", perfGlobal.atomRate);
    fprintf(stdout, "---------------------------------------------------\n\n");
    fprintf(stdout, "\n---------------------------------------------------\n");
    fprintf(stdout, " Average all atom update rate: %6.2f us/atom\n", perfGlobal.atomAllRate);
    fprintf(stdout, "---------------------------------------------------\n\n");
    fprintf(stdout, "\n---------------------------------------------------\n");
    fprintf(stdout, " Average atom rate:            %6.2f atoms/us\n", perfGlobal.atomsPerUSec);
    fprintf(stdout, "---------------------------------------------------\n\n");
}
/*Function: printPerformanceResultsYaml, ID: 160*/
void printPerformanceResultsYaml(FILE* file)
{
    /*printPerformanceResultsYaml:160*/
    /*CompoundStmt:2098*/
    if (!printRank())
        return;
    double tick = getTick();
    double loopTime = perfTimer[loopTimer].total * tick;
    fprintf(file, "\nPerformance Results:\n");
    fprintf(file, "  TotalRanks: %d\n", getNRanks());
    fprintf(file, "  ReportingTimeUnits: seconds\n");
    fprintf(file, "Performance Results For Rank %d:\n", getMyRank());
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
    fprintf(file, "Performance Results Across Ranks:\n");
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
    fprintf(file, "Performance Global Update Rates:\n");
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
/*Function: getTime, ID: 153*/
static uint64_t getTime()
{
    /*getTime:153*/
    /*CompoundStmt:2188*/
    struct timeval ptime;
    uint64_t t = 0;
    gettimeofday(&ptime, (struct timezone*)((void*)0));
    t = ((uint64_t)1000000) * (uint64_t)ptime.tv_sec + (uint64_t)ptime.tv_usec;
    return t;
}
/*Function: getTick, ID: 154*/
static double getTick()
{
    /*getTick:154*/
    /*CompoundStmt:2204*/
    double seconds_per_cycle = 9.9999999999999995E-7;
    return seconds_per_cycle;
}
/*Function: timerStats, ID: 155*/
void timerStats()
{
    /*timerStats:155*/
    /*CompoundStmt:2208*/
    double sendBuf[11], recvBuf[11];
    for (int ii = 0; ii < numberOfTimers; ii++)
        sendBuf[ii] = (double)perfTimer[ii].total;
    addDoubleParallel(sendBuf, recvBuf, numberOfTimers);
    for (int ii = 0; ii < numberOfTimers; ii++)
        perfTimer[ii].average = recvBuf[ii] / (double)getNRanks();
    RankReduceData reduceSendBuf[11], reduceRecvBuf[11];
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
}
