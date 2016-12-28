#ifndef MICROBENCH_H
#define	MICROBENCH_H
#include "Runtime.h"  
#include <string>
#include <iostream>
#include <iomanip>
#include <vector>

typedef uint64_t (*microbench) (darts::Runtime *,int, int, int);

uint64_t CDSerial      (darts::Runtime * rt, int, int depth, int);
uint64_t CDParallel    (darts::Runtime * rt, int, int, int fannout);
uint64_t CDSource      (darts::Runtime * rt, int, int, int fannout);
uint64_t CDSinc        (darts::Runtime * rt, int, int, int fannout);
uint64_t TPSerial      (darts::Runtime * rt, int, int depth, int);
uint64_t TPParallel    (darts::Runtime * rt, int, int, int);
uint64_t TPStrict      (darts::Runtime * rt, int, int depth, int fannout);
uint64_t TPNonStrict   (darts::Runtime * rt, int delay, int depth, int fannout);
uint64_t TPCDOverhead  (darts::Runtime * rt, int, int depth, int fannout);
uint64_t SerialOverhead(darts::Runtime * rt, int delay, int, int);
uint64_t Init          (darts::Runtime * rt, int, int, int);
uint64_t ParallelLoop  (darts::Runtime * rt, int delay, int, int fannout);
uint64_t SerialLoop  (darts::Runtime * rt, int delay, int, int fannout);
uint64_t CodeletLoop  (darts::Runtime * rt, int delay, int, int fannout);

struct microTest
{
    std::string name;
    microbench test;
    int delay;
    int depth;
    int fannout;
    microTest(std::string Name, microbench Test, int theDelay, int theDepth, int theFannout):
    name(Name),
    test(Test),
    delay(theDelay),
    depth(theDepth),
    fannout(theFannout) { }
};

class microBench
{
private:
    darts::Runtime * instance;
    int innerLoop;
    int outerLoop;
    int size;
    microTest * testers;
public:
    microBench(darts::Runtime * rt, int iLoop, int oLoop, int numTests, microTest * theTest):
    instance(rt),
    innerLoop(iLoop),
    outerLoop(oLoop),
    size(numTests), 
    testers(theTest) { }
    
    uint64_t runTest(std::string Name)
    {
        for(int i=0;i<size;i++)
        {
            if(Name==testers[i].name)
            {
                uint64_t oTime = 0;
                for(int j=0;j<outerLoop;j++)
                {
                    uint64_t iTime = 0;
                    testers[i].test(instance,testers[i].delay,testers[i].depth,testers[i].fannout);
                    for(int k=0;k<innerLoop;k++)
                    {
                        iTime+=testers[i].test(instance,testers[i].delay,testers[i].depth,testers[i].fannout);
                    }
                    oTime+=iTime/innerLoop;
                }
                
            std::cout << std::setw(15) << testers[i].name;
            std::cout << " Time " << std::setw(10) << oTime/outerLoop;
            std::cout << " Depth " << std::setw(5)  << testers[i].depth;
            std::cout << " Fannout " << std::setw(5)  << testers[i].fannout;
            std::cout << " Delay " << std::setw(5)  << testers[i].delay;
            std::cout << std::endl;             
                return oTime/outerLoop;
            }
                
        }
        return 0;
    }
    void runAllTests(void)
    {
        for(int i=0;i<size;i++)
        {
            std::cout << std::setw (15) << testers[i].name;           
            uint64_t oTime = 0;
            for(int j=0;j<outerLoop;j++)
            {
                uint64_t iTime = 0;
                testers[i].test(instance,testers[i].delay,testers[i].depth,testers[i].fannout);
                for(int k=0;k<innerLoop;k++)
                {
                    //std::cout << "J:" << j << " K:" << k << std::endl;
                    iTime+=testers[i].test(instance,testers[i].delay,testers[i].depth,testers[i].fannout);
                }
                oTime+=iTime/innerLoop;
            }

            std::cout << std::setw(15) << testers[i].name;
            std::cout << " Time " << std::setw(10) << oTime/outerLoop;
            std::cout << " Depth " << std::setw(5)  << testers[i].depth;
            std::cout << " Fannout " << std::setw(5)  << testers[i].fannout;
            std::cout << " Delay " << std::setw(5)  << testers[i].delay;
            std::cout << std::endl;
        }
    }
};

#endif	/* MICROBENCH_H */

