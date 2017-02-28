#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_THREADS 8

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

    omp_set_num_threads(NUM_THREADS);
#pragma omp parallel
    {
#pragma omp master
        {
            threadID = omp_get_thread_num();
            for (i = 0; i < size; i++) {
                parallelSum += inVector[i];
            }
        }

#pragma omp barrier

        outVector[omp_get_thread_num()] = parallelSum;
    }

    if (sequentialSum != parallelSum || threadID != 0) {
        printf("Error in master test\n");
    }

    for (i = 0; i < NUM_THREADS; i++) {
        if (outVector[i] != sequentialSum) {
            printf("Error: outVector[%d] =%d != %d\n", i, outVector[i], sequentialSum);
        }
    }

    free(inVector);
    free(outVector);

    return 0;
}
