/*Based on examples 15.15-17 from:
OpenMP Application Programming Interface
Examples
Version 4.0.0 November 2013
*/

/*
depend clause enforces dependencies of types flow, anti, and output.

depend(out: x)
Indicates x is being written by the associated task.

depend(in: x)
Indicates x is being read by the associated task.

depend(inout: x)
Indicates x is being both written and read by the associated task.
*/

#include "task-depend-test.h"

int main(int argc, char** argv)
{
    printf("Example 15.15: x should be 2\n");
    int x = 1;
#pragma omp parallel
    {
#pragma omp single nowait
        {
#pragma omp task shared(x) depend(out : x)
            {
                x = 2;
            }

#pragma omp task shared(x) depend(in : x)
            {
                printf("x = %d\n", x);
            }
        }
    }

    printf("Example 15.16: x should be 1\n");
    x = 1;
#pragma omp parallel
    {
#pragma omp single
        {
#pragma omp task shared(x) depend(in : x)
            {
                printf("x = %d\n", x);
            }

#pragma omp task shared(x) depend(out : x)
            {
                x = 2;
            }
        }
    }

    printf("Example 15.17: x should be 2\n");
    x = 0;
#pragma omp parallel
    {
#pragma omp single
        {
#pragma omp task shared(x) depend(out : x)
            {
                x = 1;
            }

#pragma omp task shared(x) depend(out : x)
            {
                x = 2;
            }
#pragma omp taskwait
            printf("x = %d\n", x);
        }
    }

    return 0;
}
