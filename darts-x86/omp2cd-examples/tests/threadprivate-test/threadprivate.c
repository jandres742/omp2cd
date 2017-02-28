#include <omp.h>
#include <stdio.h>

int a, b, tid;

#pragma omp threadprivate(a)

int main(int argc, char* argv[])
{
    b = 0;

#pragma omp parallel private(b, tid)
    {
        tid = omp_get_thread_num();
        a = tid + 10;
        b = tid;
    }

#pragma omp parallel private(tid)
    {
        tid = omp_get_thread_num();
        if (a != tid + 10)
            abort();
        if (b != 0)
            abort();
    }
    return 0;
}
