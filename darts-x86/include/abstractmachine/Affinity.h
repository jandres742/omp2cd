#ifndef AFFINITY_H
#define AFFINITY_H

namespace darts {

struct AffinityMask {
    unsigned int size;
    unsigned int* clusterID;
    unsigned int* unitID;
    AffinityMask(unsigned int num)
        : size(num)
    {
        clusterID = new unsigned int[size];
        unitID = new unsigned int[size];
    }
    ~AffinityMask(void)
    {
        delete[] clusterID;
        delete[] unitID;
    }
};

enum AffinityMode { SPREAD = 0,
    COMPACT = 1,
    SCATTER };

class ThreadAffinity {
private:
    unsigned int mcPerTp;
    unsigned int numTPS;
    unsigned int numMCS;
    unsigned int TPpolicy;
    unsigned int MCpolicy;
    AffinityMode mode;
    AffinityMask TPMask;
    AffinityMask MCMask;

public:
    ThreadAffinity(unsigned int mcpertp, unsigned int numbase, AffinityMode choice, unsigned int tpSched = 0, unsigned int mcSched = 0)
        : mcPerTp(mcpertp)
        , numTPS(numbase)
        , numMCS(numbase * (mcpertp))
        , TPpolicy(tpSched)
        , MCpolicy(mcSched)
        , mode(choice)
        , TPMask(numTPS)
        , MCMask(numMCS)
    {
    }
    unsigned int getNumTPS(void)
    {
        return numTPS;
    }
    unsigned int getNumMCS(void)
    {
        return numMCS;
    }
    unsigned int getNumMcPerTp(void)
    {
        return mcPerTp;
    }
    AffinityMask* getTPMask(void)
    {
        return &TPMask;
    }
    AffinityMask* getMCMask(void)
    {
        return &MCMask;
    }
    unsigned int getTPpolicy(void)
    {
        return TPpolicy;
    }
    unsigned int getMCpolicy(void)
    {
        return MCpolicy;
    }
    bool generateMask(void);
    void printMask(void);
};
}

#endif /* AFFINITY_H */
