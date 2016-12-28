#include <iostream>
#include <stdlib.h>
#include <stdint.h>
#include <sstream>
#include "darts.h"
#include "Affinity.h"
#include "include/microBench.h"
#include <map>
#define NUMTESTS 14
#define TESTLOOP 10
#define DEPTH 2
#define FANNOUT 2
#define DELAY 10000000
using namespace darts;

void parseCommandLine(int * innerLoop, int * outerLoop, int * delay, int * depth, int * fannout, int * worker, int * cluster, std::vector<std::string> * vec, int argc, char * argv[]);
void getNumArg(int argc, char * argv[], int * count, int * set);

int
main(int argc, char *argv[])
{
    Runtime * rt;
    int innerLoop = TESTLOOP;
    int outerLoop = TESTLOOP;
    int delay = DELAY;
    int depth = DEPTH;
    int fannout = FANNOUT;
    int worker=0;
    int cluster=0;
    
    std::vector<std::string> vec;
    
    if(argc>1)
        parseCommandLine(&innerLoop,&outerLoop,&delay,&depth,&fannout,&worker,&cluster,&vec,argc,argv);
    
    microTest launch[NUMTESTS] =
    {
        microTest("CDSerial",       &CDSerial, 0,depth,0),
        microTest("CDParallel",     &CDParallel, 0,0,fannout),
        microTest("CDSource",       &CDSource, 0,0,fannout),
        microTest("CDSinc",         &CDSinc, 0,0,fannout),
        microTest("TPSerial",       &TPSerial, 0,depth,0),
        microTest("TPParallel",     &TPParallel, 0,0,fannout),
        microTest("TPStrict",       &TPStrict, 0,depth,fannout),
        microTest("TPNonStrict",    &TPNonStrict,0,depth,fannout),
        microTest("TPCDOverhead",   &TPCDOverhead,delay,depth,fannout),
        microTest("SerialOverhead", &SerialOverhead,delay,0,0),
        microTest("Init",           &Init,0,0,0),
        microTest("ParallelLoop",   &ParallelLoop,delay,0,fannout),
        microTest("SerialLoop",     &SerialLoop,delay,0,fannout),
        microTest("CodeletLoop",    &CodeletLoop,delay,0,fannout)
            
    };
    
    ThreadAffinity affin((unsigned int)worker, (unsigned int) cluster, COMPACT);
    if (!affin.generateMask())
    {
        rt = new Runtime;
    }
    else
    {
        std::cout << "***********Mask***********" << std::endl;
        affin.printMask();
        rt = new Runtime(&affin);
    }
    
    microBench MC(rt,innerLoop,outerLoop,NUMTESTS,launch);
    std::cout << "******Running  Tests******" << std::endl;
    if(vec.size())
    {
        std::vector<std::string>::iterator it;
        for(it=vec.begin();it!=vec.end();++it)
        {
            MC.runTest((*it));
        }
    }
    else
        MC.runAllTests();
    
    delete rt;
    return 0;
}

void getNumArg(int argc, char * argv[], int * count, int * set)
{
    if((*count)<argc)
    {
        (*set) = atoi(argv[(*count)]);
        (*count)=(*count)+1;
    }
    else
    {
        std::cout << "No option given" << std::endl;
    }
}

void parseCommandLine(int * innerLoop, int * outerLoop, int * delay, int * depth, int * fannout, int * worker, int * cluster, std::vector<std::string> * vec, int argc, char * argv[])
{
    std::map<std::string,int> options;
    options["-inner"] = 1;
    options["-outer"] = 2;
    options["-delay"] = 3;
    options["-depth"] = 4;
    options["-fannout"] = 5;
    options["-test"] = 6;
    options["-worker"] = 7;
    options["-cluster"] = 8;
    
    int i=1;
    while(i<argc)
    {
        std::stringstream tempArg;
        tempArg << argv[i];
        switch (options[tempArg.str()])
        {
            case 1:
                i++;
                getNumArg(argc,argv,&i,innerLoop);
                break;
            case 2:
                i++;
                getNumArg(argc,argv,&i,outerLoop);
                break;
            case 3:
                i++;
                getNumArg(argc,argv,&i,delay);
                break;
            case 4:
                i++;
                getNumArg(argc,argv,&i,depth);
                break;
            case 5:
                i++;
                getNumArg(argc,argv,&i,fannout);
                break;
            case 6:
            {
                i++;
                bool flag = true;
                while(flag&&i<argc)
                {
                    std::stringstream tempTest;
                    tempTest << argv[i];
                    if(tempTest.str().find('-')==std::string::npos)
                    {
                        vec->push_back(tempTest.str());
                        i++;
                    }
                    else
                    {
                        flag = false;
                    }
                }
                break;
            }
            case 7:
                i++;
                getNumArg(argc,argv,&i,worker);
                break;
            case 8:
                i++;
                getNumArg(argc,argv,&i,cluster);
                break;
            default:
                i++;
                std::cout << "Unsupported Option " << tempArg.str() << std::endl;
        }
    }
}