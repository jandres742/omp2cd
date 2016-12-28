#include "Affinity.h"
#include "AbstractMachine.h"
#include <iostream>
using namespace darts;

bool ThreadAffinity::generateMask(void)
{
    hwloc::AbstractMachine AbsMac;
    hwloc::Cluster* clusterMap = AbsMac.getClusterMap();
    unsigned int totalUnits = AbsMac.getTotalNbUnits();
    unsigned int maxCluster = AbsMac.getNbClusters();
    unsigned int maxUnit = clusterMap[0].getNbUnits();
    unsigned int totalRequestedUnits = numTPS + numMCS;
    unsigned int RequestedUnits = mcPerTp + 1;
#if 0
    std::cout << "totalUnits = " << totalUnits << "\n";
    std::cout << "maxCluster = AbsMac.getNbClusters() = " << maxCluster << "\n";
    std::cout << "maxUnit = " << maxUnit << "\n";    
    std::cout << "numTPS = " << numTPS << "\n";
    std::cout << "numMCS = " << numMCS << "\n";
    std::cout << "mcPerTp = " << mcPerTp << "\n";
    std::cout << "totalRequestedUnits = " << totalRequestedUnits << "\n";
#endif

    if (numTPS == 0)
        return false;

    if (totalRequestedUnits > totalUnits) {
        std::cout << "Requested " << totalRequestedUnits << " only "
                  << totalUnits << " available." << std::endl;
        return false;
    }

    unsigned int check = RequestedUnits / maxUnit;
    if (RequestedUnits % maxUnit)
        check++;

    unsigned int count;

    switch (mode) {
    case SPREAD:
        if (check * maxUnit * numTPS > totalUnits) {
            std::cout << "Size will not fit in SPREAD mode"
                      << ", check("
                      << check << ")*maxUnit(" << maxUnit << ")*numTPS(" << numTPS
                      << ") = " << check * maxUnit * numTPS << " > totalUnits("
                      << totalUnits << ")" << std::endl;
            return false;
        }
#if 0
            else if(numTPS > maxCluster)
            {
                std::cout << "Size will not fit in SPREAD mode" << std::endl;
                return false;
            }
#endif
        if (RequestedUnits < maxUnit) {
            unsigned int* clusterCount = new unsigned int[maxCluster];
            for (unsigned int i = 0; i < maxCluster; i++)
                clusterCount[i] = 0;
            for (unsigned int i = 0; i < numTPS; i++) {
                TPMask.clusterID[i] = i % maxCluster;
                TPMask.unitID[i] = clusterCount[i] = clusterCount[i] + 1;
                for (unsigned int j = 0; j < mcPerTp; j++) {
                    MCMask.clusterID[i * mcPerTp + j] = i % maxCluster;
                    MCMask.unitID[i * mcPerTp + j] = clusterCount[i] = clusterCount[i] + 1;
                }
                delete[] clusterCount;
            }
        } else {
            count = 0;
            for (unsigned int i = 0; i < numTPS; i++) {
                TPMask.clusterID[i] = count / maxUnit;
                TPMask.unitID[i] = count % maxUnit;
                count++;
                for (unsigned int j = 0; j < mcPerTp; j++) {
                    MCMask.clusterID[i * mcPerTp + j] = count / maxUnit;
                    MCMask.unitID[i * mcPerTp + j] = count % maxUnit;
                    count++;
                }
                if (count % maxUnit) {
                    unsigned int temp = count / maxUnit;
                    count = (temp + 1) * maxUnit;
                }
            }
        }

        break;

    case COMPACT:
        count = 0;
        for (unsigned int i = 0; i < numTPS; i++) {
            TPMask.clusterID[i] = count / maxUnit;
            TPMask.unitID[i] = count % maxUnit;
            count++;
            for (unsigned int j = 0; j < mcPerTp; j++) {
                MCMask.clusterID[i * mcPerTp + j] = count / maxUnit;
                MCMask.unitID[i * mcPerTp + j] = count % maxUnit;
                count++;
            }
        }

        break;

    case SCATTER:

        if (mcPerTp * numTPS > totalUnits) {
            std::cout << "Size will not fit in SCATTER mode because the "
                      << "combined number of TPSchedulers(" << numTPS
                      << ") and MicroSchedulers(" << mcPerTp
                      << ") is more than the number of total processing units("
                      << maxUnit << ") on this machine\n";
        } else {
            unsigned int* clusterCounter = new unsigned int[totalUnits];
            for (unsigned int i = 0; i < totalUnits; i++) {
                clusterCounter[i] = 0;
            }

            unsigned int offset = 0;
            for (unsigned int i = 0; i < numTPS; i++) {
                TPMask.clusterID[i] = i % maxCluster;
                TPMask.unitID[i] = clusterCounter[i] + offset;

                int offsetCluster = 0;
                for (unsigned int j = 0; j < mcPerTp; j++) {
                    clusterCounter[i] = clusterCounter[i] + 2;
                    if (clusterCounter[i] >= maxUnit) {
                        clusterCounter[i] = ((clusterCounter[i] % maxUnit) + 1) % 2;
                        offsetCluster++;
                    }

                    MCMask.clusterID[i * mcPerTp + j] = TPMask.clusterID[i] + offsetCluster;
                    MCMask.unitID[i * mcPerTp + j] = clusterCounter[i] + offset;
                }

                if (TPMask.clusterID[i] >= maxCluster - 1) {
                    offset += (mcPerTp + 1) * 2;
                    if (offset >= maxUnit) {
                        offset = 1;
                    }
                }
            }
            delete[] clusterCounter;
        }
        break;

    default:
        return false;
    }
    return true;
}

void ThreadAffinity::printMask(void)
{
    for (unsigned int i = 0; i < TPMask.size; i++)
        std::cout << "TPSched " << TPMask.clusterID[i] << " "
                  << TPMask.unitID[i] << " Policy " << TPpolicy << std::endl;

    for (unsigned int i = 0; i < MCMask.size; i++)
        std::cout << "MCSched " << MCMask.clusterID[i] << " "
                  << MCMask.unitID[i] << std::endl;
}
