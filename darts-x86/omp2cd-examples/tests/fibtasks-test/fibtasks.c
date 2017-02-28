#ifndef __DARTS_
#include <omp.h>
#endif
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define CUTOFF 2

int seqFib(int n)
{
    int z, x, y;
    if (n < CUTOFF) {
        z = n;
    } else {
        x = seqFib(n - 1);
        y = seqFib(n - 2);
        z = x + y;
    }
    return z;
}

int parallelFibTaskWait(int n)
{
    int x, y;

    if (n < CUTOFF) {
        return n;
    } else {
#pragma omp task shared(x) firstprivate(n)
        {
            x = parallelFibTaskWait(n - 1);
        }
#pragma omp task shared(y) firstprivate(n)
        {
            y = parallelFibTaskWait(n - 2);
        }

#pragma omp taskwait
        return x + y;
    }
}

int main(int argc, char** argv)
{
    int num = 0;
    if (argc <= 1) {
        num = 20;
    } else {
        num = atoi(argv[1]);
    }

    if (num <= 0) {
        printf("Num should be greater than 0\n");
        return 0;
    }

    int seq = seqFib(num);

    int parTaskWait = 0;
#pragma omp parallel shared(num)
    {
#pragma omp single
        {
            parTaskWait = parallelFibTaskWait(num);
        }
    }

    if (seq != parTaskWait) {
        printf("Error in fib test with taskwait: ");
        printf("seq = %d, ", seq);
        printf("parTaskWait = %d\n", parTaskWait);
    }
    return 0;
}
