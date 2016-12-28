#include "utils.h"

size_t ompNumThreads;
size_t DARTS_CODELETS_MULT;
size_t numOfCUs;
darts::Codelet* RuntimeFinalCodelet;
darts::ThreadAffinity* affin;
bool affinMaskRes;
darts::Runtime* myDARTSRuntime;
int ompSchedulePolicy;
int ompScheduleChunk;
int NUMTPS;
int dartsTPLoopTH0;
int dartsTPLoopTH1;

using namespace std;

void getOMPSchedulePolicy()
{
    ompSchedulePolicy = 1; /*Static*/
    ompScheduleChunk = 1;
    char* ompSchedule = getenv("OMP_SCHEDULE");
    if (ompSchedule != NULL) {
        string ompSchedStr = ompSchedule;
        /*Extract policy and chunk size*/
        size_t pos = ompSchedStr.find(",");
        if (pos != string::npos) {
            string tempChunk = ompSchedStr.substr(pos + 1, (ompSchedStr.length() - pos));
            ompScheduleChunk = std::stoi(tempChunk, nullptr, 10);
            ompSchedStr = ompSchedStr.substr(0, pos);
        }
        if (ompSchedStr.compare("static") == 0)
            ompSchedulePolicy = 1; /*Static*/
        else
            ompSchedulePolicy = 2; /*Dynamic*/
    }
}

void getTPLoopThresholds()
{
    char* tpLoopTh0String = getenv("DARTS_TPLOOP_TH0");
    if (tpLoopTh0String == NULL)
        dartsTPLoopTH0 = 4;
    else
        dartsTPLoopTH0 = atoi(tpLoopTh0String);

    char* tpLoopTh1String = getenv("DARTS_TPLOOP_TH1");
    if (tpLoopTh1String == NULL)
        dartsTPLoopTH1 = 32;
    else
        dartsTPLoopTH1 = atoi(tpLoopTh1String);
}

void getOMPNumThreads()
{
    char* ompNumThreadsString = getenv("OMP_NUM_THREADS");
    if (ompNumThreadsString == NULL)
        ompNumThreads = 1;
    else
        ompNumThreads = atoi(ompNumThreadsString);

    numOfCUs = 11;
    if (ompNumThreads < 12)
        numOfCUs = ompNumThreads - 1;

    char* DARTS_CODELETS_MULTString = getenv("DARTS_CODELETS_MULT");
    if (DARTS_CODELETS_MULTString == NULL)
        DARTS_CODELETS_MULT = 1;
    else
        DARTS_CODELETS_MULT = atoi(DARTS_CODELETS_MULTString);
}

void getNumTPs()
{
    char* NUMTPSstring = getenv("DARTS_NUMTPS");
    if (NUMTPSstring == NULL)
        NUMTPS = sysconf(_SC_NPROCESSORS_ONLN);
    else
        NUMTPS = atoi(NUMTPSstring);
}

uint32_t getDARTSTPPolicy()
{
    uint32_t policy = 3; /*TP Stealing*/
    char* policyString = getenv("DARTS_TPPOLICY");
    if (policyString != NULL)
        policy = atoi(policyString);
    return policy;
}

uint32_t getDARTSMCPolicy()
{
    uint32_t policy = 1; /*MC Stealing*/
    char* policyString = getenv("DARTS_MCPOLICY");
    if (policyString != NULL)
        policy = atoi(policyString);
    return policy;
}

size_t getNumOfTPChildren(int numThreads)
{
    size_t res = numThreads / getNumOfPUs();
    if (numThreads % getNumOfPUs() > 0)
        res++;
    return res;
}

size_t codeletsPerTP(size_t numThreads, size_t startCodeletID)
{
    size_t res = getNumOfPUs();
    if (numThreads % getNumOfPUs() > 0 && startCodeletID == getNumOfTPChildren(numThreads) - 1)
        res = numThreads % getNumOfPUs();
    return res;
}
