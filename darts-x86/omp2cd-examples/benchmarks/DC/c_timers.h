#ifndef C_TIMERS
#define C_TIMERS

#include "wtime.h"
#include <stdlib.h>
#ifdef _OPENMP
#include <omp.h>
#endif

/*  Prototype  */
void wtime( double * );

double elapsed_time( void );
void timer_clear( int n );
void timer_start( int n );
void timer_stop( int n );
double timer_read( int n );

#endif