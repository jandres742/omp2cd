#ifndef __DARTS_
#include <omp.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void myFunction(double* inVec, double* outVec, int idx)
{
    *(outVec + idx) = (*(inVec + idx)) * (*(inVec + idx)) + 1;
}

int main(int argc, char** argv)
{
    srand(time((time_t*)NULL));

    int totalThreads = atoi(getenv("OMP_NUM_THREADS"));

    double* inVector = (double*)malloc(totalThreads * sizeof(double));
    double* outVectorSeq = (double*)malloc(totalThreads * sizeof(double));
    double* outVectorPar = (double*)malloc(totalThreads * sizeof(double));

    int i;
    for (i = 0; i < totalThreads; i++) {
        inVector[i] = (rand() % 100 - 50) / 0.5;
    }

    for (i = 0; i < totalThreads; i++) {
        outVectorSeq[i] = inVector[i] * inVector[i] + 1;
    }

#pragma omp parallel
    {
        int id = omp_get_thread_num();
#pragma omp task firstprivate(id) shared(inVector, outVectorPar)
        {
            myFunction(inVector, outVectorPar, id);
        }

        (void)id;

#pragma omp taskwait

        (void)id;

#pragma omp barrier
    }

    int good = 1;
    for (i = 0; i < totalThreads && good == 1; i++) {
        if (*(outVectorSeq + i) != *(outVectorPar + i)) {
            printf("Error in tasks test\n");
            printf("*(outVectorSeq + %d) = %f\n", i, *(outVectorSeq + i));
            printf("*(outVectorPar + %d) = %f\n", i, *(outVectorPar + i));
            good = 0;
        }
    }

    free(inVector);
    free(outVectorPar);
    free(outVectorSeq);

    return 0;
}
