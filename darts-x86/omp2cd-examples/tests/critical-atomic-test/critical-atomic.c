#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char** argv)
{
    volatile int count_int_atomic = 0;
    volatile int count_critical = 0;

    double count_double_atomic = 0;

    int num_threads = 16;

    int inc = 5;

    omp_set_num_threads(num_threads);
#pragma omp parallel
    {
// atomic test
#pragma omp atomic
        ++count_int_atomic;

#pragma omp atomic update
        count_int_atomic += 2;

#pragma omp atomic
        count_double_atomic += inc;

// critical test
#pragma omp critical
        {
            count_critical++;
            count_critical++;
        }
    }

    if (count_int_atomic != 3 * num_threads) {
        printf("Error int atomic, count_int_atomic = %d != 3*num_threads = %d\n", count_int_atomic,
            3 * num_threads);
    }
    if (count_double_atomic != 5 * num_threads) {
        printf("Error double atomic, count_double_atomic = %f != 5*num_threads = %d\n",
            count_double_atomic, 3 * num_threads);
    }
    if (count_critical != 2 * num_threads) {
        printf("Error critical, count_critical = %d != 2*num_threads = %d\n", count_critical,
            2 * num_threads);
    }

    return 0;
}
