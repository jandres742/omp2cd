#include "c_timers.h"

double elapsed_time( void )
{
    double t;

#if 0
#if defined(_OPENMP) && (_OPENMP > 200010)
/*  Use the OpenMP timer if we can */
    t = omp_get_wtime();
#else
    wtime( &t );
#endif
#else
	wtime( &t );
#endif
    return( t );
}


static double start[64], elapsed[64];
#ifdef _OPENMP
#pragma omp threadprivate(start, elapsed)
#endif

void timer_clear( int n )
{
    elapsed[n] = 0.0;
}

void timer_start( int n )
{
    start[n] = elapsed_time();
}

void timer_stop( int n )
{
    double t, now;

    now = elapsed_time();
    t = now - start[n];
    elapsed[n] += t;

}

double timer_read( int n )
{
    return( elapsed[n] );
}