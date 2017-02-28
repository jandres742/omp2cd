/* Example taken from
 * https://msdn.microsoft.com/en-us/library/bc1k0739.aspx
 * Jan 2016
 * */

#include <omp.h>
#include <stdio.h>

#define NUM_THREADS 16

float fGlobal = 10;
float get_float()
{
    fGlobal += 1;
    return fGlobal;
}

void CopyPrivate(float a, float b)
{
#pragma omp single copyprivate(a, b)
    {
        a = get_float();
        b = get_float();
    }

    if (a - 11 != 0) {
        fprintf(stderr, "thread = %d, a != 11, a = %f\n", omp_get_thread_num(), a);
        abort();
    }
    if (b != 12) {
        fprintf(stderr, "thread = %d, b != 12, b = %f\n", omp_get_thread_num(), b);
        abort();
    }
}

int main()
{
    float a = 9.99, b = 123.456;

    omp_set_num_threads(NUM_THREADS);

#pragma omp parallel
    {
        CopyPrivate(a, b);
    }

    return 0;
}
