#include "Runtime.h"
#include "Atomics.h"
#include <cassert>
#include <stdlib.h>
#include <time.h>

#ifdef TRACE
#include "getClock.h"
#endif
#ifndef _MSC_VER
#include <unistd.h>
#endif

using namespace darts;

CodeletFinal Runtime::finalSignal;

void* MCThread(void* args)
{
    mcRuntimeArgs* mcargs = static_cast<mcRuntimeArgs*>(args);
    Runtime* rt = mcargs->rt;
    unsigned int numMCS = rt->getNumMCS();
    unsigned int threadId = mcargs->threadId;
    unsigned int clusterId = mcargs->clusterId;
    unsigned int unitId = mcargs->unitId;

    MScheduler* myMCSched = rt->newMCSched(clusterId * numMCS + unitId);
    myMCSched->setID(threadId);
    myMCSched->setPolicy(m_policies[mcargs->mcSched]);
    rt->finalSignal.addAliveSignal(threadId, myMCSched);

    rt->decMC();
    rt->decFull();
    rt->spin();

    myThread.threadTPsched = myMCSched->getParentScheduler();
    myThread.threadMCsched = myMCSched;

    myMCSched->policy();
    return 0;
}

void* TPThread(void* args)
{
    tpRuntimeArgs* tpargs = static_cast<tpRuntimeArgs*>(args);
    Runtime* rt = tpargs->rt;
    unsigned int threadId = tpargs->threadId;
    unsigned int clusterId = tpargs->clusterId;

    TPScheduler* myTPSched = rt->newTPSched(clusterId);
    myTPSched->setID(threadId);
    myTPSched->setPolicy(tp_policies[tpargs->tpSched]);
    rt->finalSignal.addAliveSignal(threadId, myTPSched);

    rt->decTP();
    rt->decFull();
    rt->spin();

    myThread.threadTPsched = myTPSched;
    myThread.threadMCsched = NULL;

    myTPSched->policy();
    return 0;
}

void TPThread0(Runtime* rt, unsigned int tpPolicy)
{
    TPScheduler* myTPSched = rt->newTPSched(0);
    myTPSched->setID(0);
    myTPSched->setPolicy(tp_policies[tpPolicy]);
    rt->finalSignal.addAliveSignal(0, myTPSched);

    rt->decTP();
    while (rt->checkTP())
        ;
    rt->linkTPSched();

    while (rt->checkMC())
        ;
    rt->linkMCSched();

    myThread.threadTPsched = myTPSched;
    myThread.threadMCsched = NULL;

    rt->decFull();
}

void Runtime::linkTPSched()
{
    for (unsigned int i = 0; i < numTPSched_; i++)
        for (unsigned int j = 0; j < numTPSched_; j++)
            if (TPSched_[i] != TPSched_[j])
                TPSched_[i]->addPeer(TPSched_[j]);
}

void Runtime::linkMCSched()
{
    for (unsigned int i = 0; i < numTPSched_; i++) {
        for (unsigned int j = 0; j < numMCSched_; j++) {
            TPSched_[i]->setSubScheduler(MCSched_[i * numMCSched_ + j]);
            MCSched_[i * numMCSched_ + j]->setParentScheduler(TPSched_[i]);
        }
    }
}

void Runtime::run(tpClosure* tpToStart)
{
    finalSignal.resetCodelet();
    TPSched_[0]->resurrect();
    TPSched_[0]->pushReadyHead(tpToStart);
    TPSched_[0]->policy();
}

Runtime::Runtime(unsigned int maxCluster, unsigned int maxWorker)
    : AbsMac(false)
    , clusterMap(AbsMac.getClusterMap())
    , numTPSched_((AbsMac.getNbClusters() < maxCluster) ? AbsMac.getNbClusters() : maxCluster)
    , numMCSched_(((clusterMap[0].getNbUnits() - 1) < maxWorker) ? (clusterMap[0].getNbUnits() - 1) : maxWorker)
    , numThreads_(numTPSched_ + numTPSched_ * numMCSched_)
    , TPSched_(new TPScheduler*[numTPSched_])
    , MCSched_(new MScheduler*[numMCSched_ * numTPSched_])
    , localThreads_(new Thread[numThreads_])
    , tpargs_(new tpRuntimeArgs[numTPSched_])
    , mcargs_(new mcRuntimeArgs[numMCSched_ * numTPSched_])
    , tpcount_(numTPSched_)
    , mccount_(numTPSched_ * numMCSched_)
    , fullcount_(numTPSched_ + numTPSched_ * numMCSched_)
    , spin_(true)
{

    if (maxCluster > AbsMac.getNbClusters() && maxCluster != (unsigned int)-1)
        std::cerr << "maxCluster is greater than the number of available cluster"
                  << std::endl;

    if (maxWorker > AbsMac.getNbClusters() && maxWorker != (unsigned int)-1)
        std::cerr << "maxWorker is greater than the number of available worker"
                  << std::endl;

    finalSignal.setNumThreads(numThreads_);

    finalSignal.setTerminate(false);

    for (unsigned int i = 0; i < numTPSched_; i++) {
        unsigned tid = i * (1 + numMCSched_);
        tpargs_[i].rt = this;
        tpargs_[i].threadId = tid;
        tpargs_[i].clusterId = i;
        tpargs_[i].tpSched = DEFAULT_TP_POLICY;
        localThreads_[tid].resetArgument(&tpargs_[i]);
        localThreads_[tid].resetFunction(TPThread);
        localThreads_[tid].setAffinity(clusterMap[i].getUnits()[0].getId());
    }

    for (unsigned int i = 0; i < numTPSched_; i++) {
        int idx = i * numMCSched_;
        for (unsigned int j = 0; j < numMCSched_; j++) {
            unsigned int tid = i * (1 + numMCSched_) + j + 1;
            mcargs_[idx + j].rt = this;
            mcargs_[idx + j].threadId = tid;
            mcargs_[idx + j].clusterId = i;
            mcargs_[idx + j].unitId = j;
            mcargs_[idx + j].mcSched = DEFAULT_MC_POLICY;
            localThreads_[tid].resetArgument(&mcargs_[idx + j]);
            localThreads_[tid].resetFunction(MCThread);
            localThreads_[tid].setAffinity(clusterMap[i].getUnits()[j + 1].getId());
        }
    }
    for (unsigned int i = 1; i < numThreads_; i++)
        localThreads_[i].run();

    TPThread0(this, DEFAULT_TP_POLICY);
}

Runtime::Runtime(ThreadAffinity* affinity)
    : AbsMac(false)
    , clusterMap(AbsMac.getClusterMap())
    , numTPSched_(affinity->getNumTPS())
    , numMCSched_(affinity->getNumMcPerTp())
    , numThreads_(numTPSched_ + numTPSched_ * numMCSched_)
    , TPSched_(new TPScheduler*[numTPSched_])
    , MCSched_(new MScheduler*[numMCSched_ * numTPSched_])
    , localThreads_(new Thread[numThreads_])
    , tpargs_(new tpRuntimeArgs[numTPSched_])
    , mcargs_(new mcRuntimeArgs[numMCSched_ * numTPSched_])
    , tpcount_(numTPSched_)
    , mccount_(numTPSched_ * numMCSched_)
    , fullcount_(numTPSched_ + numTPSched_ * numMCSched_)
    , spin_(true)
{
    finalSignal.setNumThreads(numThreads_);

    finalSignal.setTerminate(false);

    AffinityMask* TPaffinity = affinity->getTPMask();
    AffinityMask* MCaffinity = affinity->getMCMask();

    /*Read the schedulers' sleep times if given*/
    TPSleepTime = 512;
    char* TPSleepString = getenv("DARTS_TPSLEEP");
    if (TPSleepString != NULL)
        TPSleepTime = (uint64_t)(atoi(TPSleepString));

    MCSleepTime = 16384 / 4;
    char* MCSleepString = getenv("DARTS_MCSLEEP");
    if (MCSleepString != NULL)
        MCSleepTime = uint64_t(atoi(MCSleepString));

    /* Check if user set DARTS_AFFINITY on its env. following 
 * GOMP_CPU_AFFINITY as specified in
 * https://gcc.gnu.org/onlinedocs/gcc-4.8.5/libgomp/GOMP_005fCPU_005fAFFINITY.html#GOMP_005fCPU_005fAFFINITY:
 * "The variable should contain a space-separated or comma-separated
 * list of CPUs. This list may contain different kinds of entries: 
 * either single CPU numbers in any order, a range of CPUs (M-N) or
 * a range with some stride (M-N:S). CPU numbers are zero based"
 * 
 * Example: DARTS_AFFINITY="0-47"
 * 
 * verbose will output the affinity information:
 * 
 * Example: DARTS_AFFINITY="0-47, verbose"
 * */

    bool isVerbose = false;
    std::vector<int> dartsAffinityPosVec;
    char* dartsAffinity = getenv("DARTS_AFFINITY");
    if (dartsAffinity != NULL) {
        std::string dartsAffinityStr = dartsAffinity;
        if (dartsAffinityStr.find("verbose") != std::string::npos)
            isVerbose = true;

        if (dartsAffinityStr.empty() == false) {
            size_t i = 0;
            std::string newNumberStr = "";
            int lowerNumber = -1;
            int upperNumber = -1;
            int strideNumber = 1;
            bool isRange = false;
            bool hasStride = false;
            while (i < dartsAffinityStr.length()) {
                char literal = dartsAffinityStr[i];
                if (std::isdigit(literal))
                    newNumberStr += literal;

                if ((std::isdigit(literal) == false || i == dartsAffinityStr.length() - 1) && newNumberStr.empty() == false) {
                    int newNumber = stoi(newNumberStr);
                    if (literal == '-') {
                        isRange = true;
                        lowerNumber = newNumber;
                        newNumberStr = "";
                    } else if (literal == ':') {
                        hasStride = true;
                        upperNumber = newNumber;
                        newNumberStr = "";
                    } else {
                        if (isRange) {

                            if (hasStride)
                                strideNumber = newNumber;
                            else
                                upperNumber = newNumber;

                            for (int p = lowerNumber; p <= upperNumber; p += strideNumber)
                                dartsAffinityPosVec.push_back(p);
                        } else {
                            dartsAffinityPosVec.push_back(newNumber);
                        }
                        newNumberStr = "";
                        strideNumber = 1;
                        isRange = false;
                        hasStride = false;
                    }
                }
                i++;
            }

            if (isVerbose) {
                fprintf(stderr, "Pinning threads to: ");
                for (int& i : dartsAffinityPosVec)
                    fprintf(stderr, "%d ", i);
                fprintf(stderr, "\n");
            }
        }
    }

    /*Pin down the master*/
    if (dartsAffinityPosVec.size() > 0) {
        cpu_set_t cpuset;
        CPU_ZERO(&cpuset);
        CPU_SET(dartsAffinityPosVec[0], &cpuset);
        pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
    }

    int dartsAffinityPosVecSize = dartsAffinityPosVec.size();
    int nextAffinityPos = 0;
    for (unsigned int i = 0; i < numTPSched_; i++) {
        unsigned tid = i * (1 + numMCSched_);
        tpargs_[i].rt = this;
        tpargs_[i].threadId = tid;
        tpargs_[i].clusterId = i;
        tpargs_[i].tpSched = affinity->getTPpolicy();
        // tpargs_[i].tpSched.setGlobalID(i);
        localThreads_[tid].resetArgument(&tpargs_[i]);
        localThreads_[tid].resetFunction(TPThread);

        uint16_t affinCore = clusterMap[TPaffinity->clusterID[i]].getUnits()[TPaffinity->unitID[i]].getId();

        /*Override the affinity mask*/
        if (dartsAffinityPosVecSize > 0) {
            affinCore = dartsAffinityPosVec[nextAffinityPos];
            nextAffinityPos = (nextAffinityPos + numMCSched_ + 1) % dartsAffinityPosVecSize;
        }

        localThreads_[tid].setAffinity(affinCore);

        if (isVerbose)
            fprintf(stderr, "TPSched %d(tid = %d) pinned to %d\n", i, tid, affinCore);
    }

    nextAffinityPos = 0;
    for (unsigned int i = 0; i < numTPSched_; i++) {
        int idx = i * numMCSched_;

        if (dartsAffinityPosVecSize > 0)
            nextAffinityPos = (nextAffinityPos + 1) % dartsAffinityPosVecSize;

        for (unsigned int j = 0; j < numMCSched_; j++) {
            unsigned int tid = i * (1 + numMCSched_) + j + 1;
            mcargs_[idx + j].rt = this;
            mcargs_[idx + j].threadId = tid;
            mcargs_[idx + j].clusterId = i;
            mcargs_[idx + j].unitId = j;
            mcargs_[idx + j].mcSched = affinity->getMCpolicy();
            localThreads_[tid].resetArgument(&mcargs_[idx + j]);
            localThreads_[tid].resetFunction(MCThread);

            uint16_t affinCore = clusterMap[MCaffinity->clusterID[idx + j]].getUnits()[MCaffinity->unitID[idx + j]].getId();

            /*Override the affinity mask*/
            if (dartsAffinityPosVecSize > 0) {
                affinCore = dartsAffinityPosVec[nextAffinityPos];
                nextAffinityPos = (nextAffinityPos + 1) % dartsAffinityPosVecSize;
            }

            localThreads_[tid].setAffinity(affinCore);

            if (isVerbose)
                fprintf(stderr, "\tMCSched %d:%d (tid=%d) pinned to %d\n", i, j, tid, affinCore);
        }
    }

    for (unsigned int i = 1; i < numThreads_; i++)
        localThreads_[i].run();

    TPThread0(this, affinity->getTPpolicy());

    /*Store information about the schedulers on each one*/
    for (unsigned int i = 0; i < numTPSched_; i++) {
        TPSched_[i]->setRuntimeTPScheds((ABCScheduler**)TPSched_);
        TPSched_[i]->setNumOfMaxUnitsPerCluster(clusterMap[0].getNbUnits() / 2);
        TPSched_[i]->setNumTPSched(numTPSched_);
        TPSched_[i]->setNumMCSched(numMCSched_);
        TPSched_[i]->setNumSched(numTPSched_ * (numMCSched_ + 1));
        for (unsigned int j = 0; j < numMCSched_; j++) {
            MScheduler* myCDS = static_cast<MScheduler*>(TPSched_[i]->getSubScheduler(j));
            myCDS->setRuntimeTPScheds((ABCScheduler**)TPSched_);
            myCDS->setNumOfMaxUnitsPerCluster(clusterMap[0].getNbUnits() / 2);
            myCDS->setNumTPSched(numTPSched_);
            myCDS->setNumMCSched(numMCSched_);
            myCDS->setNumSched(numTPSched_ * (numMCSched_ + 1));
        }
    }

    if (isVerbose) {
        fprintf(stderr, "Total Units: %lu\n", TPSched_[0]->getNumOfMaxUnitsPerCluster());
        fprintf(stderr, "Total Schedulers: %lu\n", TPSched_[0]->getNumSched());
        fprintf(stderr, "Total TPSchedulers: %lu\n", TPSched_[0]->getNumTPSched());
        fprintf(stderr, "Total MCSchedulers: %lu\n", TPSched_[0]->getNumMCSched());

        for (unsigned int i = 0; i < numTPSched_; i++) {
            fprintf(stderr, "TPSched_[%d] : id   = %lu\n", i, TPSched_[i]->getID());
            for (unsigned int j = 0; j < numMCSched_; j++) {
                MScheduler* myCDS = static_cast<MScheduler*>(TPSched_[i]->getSubScheduler(j));
                fprintf(stderr, "\tMCSched_[%d] : id = %lu\n", j, myCDS->getID());
            }
        }
    }
}

void Runtime::shutdownRuntime()
{
    for (unsigned int i = 0; i < numTPSched_; i++) {
        for (unsigned int j = 0; j < numMCSched_; j++) {
            MScheduler* myCDS = static_cast<MScheduler*>(TPSched_[i]->getSubScheduler(j));
            myCDS->kill();
        }
        TPSched_[i]->kill();
    }
}

Runtime::~Runtime(void)
{
    if (!finalSignal.getTerminate()) {
        finalSignal.setTerminate(true);
        finalSignal.resetCodelet();
        finalSignal.decDep();
        for (unsigned int i = 1; i < numThreads_; i++)
            localThreads_[i].join();
    }
    for (unsigned int i = 0; i < numTPSched_; i++)
        delete TPSched_[i];
    for (unsigned int i = 0; i < numTPSched_ * numMCSched_; i++)
        delete MCSched_[i];
    delete[] TPSched_;
    delete[] MCSched_;
    delete[] localThreads_;
    delete[] tpargs_;
    delete[] mcargs_;
}