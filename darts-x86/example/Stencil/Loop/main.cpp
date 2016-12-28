#include <iostream>
#include <stdlib.h>
#include "darts.h"
#include "stencilTP.h"
#include "getClock.h"

#define MAX_REC_DEPTH 10
#define STARTTILE 2
#define INNER 10
#define OUTER 10

void feedData (double *oldA, size_t matSize) 
{
	unsigned i,j;
  srand48(4884);
  memset(oldA, 0, sizeof (double) * matSize * matSize * 2);
	for (i = 0; i < matSize; i++) {
		for (j = 0; j < matSize; j++) {
			oldA [i*matSize+j] = drand48 ()*100;
		}
	}
}

void printResult (double *oldA, size_t matSize) {
	unsigned i,j;
	
	std::cerr << "Print Matrix" << std::endl;
	for (i = 0; i < matSize; i++) {
		for (j = 0; j < matSize; j++) {
			fprintf (stderr, "%15.5f ", oldA[i*matSize+j]);
		}
		std::cerr << std::endl;
	}
}

uint64_t dartTile(size_t matSize, size_t tile)
{
    uint64_t innerTime = 0;
    uint64_t outerTime = 0;
    
    double * oldA = new double[matSize*matSize*2];
    double * newA = oldA + matSize * matSize;
    
    Runtime rt;
    
    for (int i = 0; i < OUTER; i++)
    {
        feedData(oldA, matSize);
        rt.run(launch<stencil>(matSize,tile,oldA,newA,MAX_REC_DEPTH,&Runtime::finalSignal));
        
        for (int j = 0; j < INNER; j++)
        {
            feedData(oldA, matSize);
            uint64_t start = getTime();
            rt.run(launch<stencil>(matSize,tile,oldA,newA,MAX_REC_DEPTH,&Runtime::finalSignal));
            uint64_t end = getTime();
            innerTime+=end-start;
        }
        outerTime+=innerTime/INNER;
        innerTime = 0;
    }
    //printResult(oldA,matSize);
    delete oldA;
    return outerTime/OUTER;
}

void tileSearch(size_t * tileResult, uint64_t * timeResult, size_t size, uint64_t lowTime, uint64_t highTime, size_t minTile, size_t maxTile )
{
    size_t lowRes = minTile;
    size_t highRes = maxTile;
    uint64_t lowTimeRes = lowTime;
    uint64_t highTimeRes = highTime;
    size_t midTile = minTile+(maxTile-minTile)/2;
    uint64_t mid = 0;
    if(midTile!=minTile)
    {
        std::cout << "Tile: " << midTile << std::endl;
        mid = dartTile(size,midTile);
        std::cout << "Time: " << mid << std::endl;
        if(mid < lowTime && mid < highTime)
        {
            tileSearch(&lowRes,&lowTimeRes,size,lowTime,mid,minTile,midTile);
            tileSearch(&highRes,&highTimeRes,size,mid,highTime,midTile,maxTile);
        }
    }    
    else
    {
        mid = lowTimeRes;
        midTile = lowRes;
    }
    
    if(lowTimeRes < highTimeRes)
    {
        if(lowTimeRes < mid)
        {
            (*timeResult) = lowTimeRes;
            (*tileResult) = lowRes;
        }
        else
        {
            (*timeResult) = mid;
            (*tileResult) = midTile;
        }
    }
    else
    {
        if(highTimeRes < mid)
        {
            (*timeResult) = highTimeRes;
            (*tileResult) = highRes;
        }
        else
        {
            (*timeResult) = mid;
            (*tileResult) = midTile;
        }
    }   
    return;
}

void tileCheck(size_t size, uint64_t * time, size_t * tile)
{
    uint64_t tempTime;
    (*time) = -1;
    for(size_t i=STARTTILE;i<size+1;i*=2)
    {
        std::cout << "Tile: " << i << std::endl;
        tempTime = dartTile(size,i);
        std::cout << "Time: " << tempTime << std::endl;
        if(tempTime<(*time))
        {
            (*time) = tempTime;
            (*tile) = i;
        }
    }
}

void tileWrapper(size_t size, uint64_t * time, size_t * tile)
{
    uint64_t tempTime;
    size_t guess;
    tileCheck(size,&tempTime,&guess);
    if(guess==STARTTILE || guess==size)
    {
        (*time) = tempTime;
        (*tile) = guess;
        return;
    }
    tileSearch(tile, time, size, 
               dartTile(size,guess/2), 
               dartTile(size,guess*2), 
               guess/2, guess*2 );
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "./stencilLoop <size> " << std::endl;
        return 0;
    }

    unsigned matSize = strtol(argv[1], 0, 10);
    
    uint64_t time;
    uint64_t tile;
    
    tileWrapper(matSize,&time,&tile);
    std::cout << "Tile: " << time << " Tile: " << tile << std::endl;

    return 0;
}
