#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int NUM_THREADS;

int main(int argc, char** argv)
{
    srand(time((time_t*)NULL));

    int size, i;

    if (argc <= 1) {
        size = 31;
    } else {
        size = atoi(argv[1]);
    }

    if (size < 0) {
        printf("Size should be greater than 0\n");
        return 0;
    }

    NUM_THREADS = omp_get_max_threads();
    int* inVector = (int*)malloc(size * sizeof(int));
    int* outVector = (int*)malloc(NUM_THREADS * sizeof(int));
    int threadID = -1;

    for (i = 0; i < size; i++) {
        inVector[i] = rand() % 100 - 50;
    }

    int sequentialSum = 0;
    for (i = 0; i < size; i++) {
        sequentialSum += inVector[i];
    }

    int parallelSum = 0;

#pragma omp parallel
    {
#pragma omp master
        {
            threadID = omp_get_thread_num();
            for (i = 0; i < size; i++) {
                parallelSum += inVector[i];
            }
        }

        int initLoop = 0;
        int endLoop = NUM_THREADS;

#if USECODELETS
#pragma omp for codelet schedule(runtime)
#else
#pragma omp for schedule(runtime)
#endif
        for (i = initLoop; i < endLoop; i++) {
            outVector[i] = 10;
        }
#if USECODELETS
#pragma omp single codelet
#else
#pragma omp single
#endif
        {
            for (i = 0; i < NUM_THREADS; i++) {
                outVector[i] += parallelSum;
            }
        }
    }

    if (sequentialSum != parallelSum || threadID != 0) {
        printf("Error in master test\n");
    }

    for (i = 0; i < NUM_THREADS; i++) {
        if (outVector[i] != (sequentialSum + 10)) {
            printf("Error: outVector[%d] =%d != %d\n", i, outVector[i], sequentialSum + 10);
        }
    }

    free(inVector);
    free(outVector);

    return 0;
}
