#include "c_timers.output.darts.h"
using namespace darts;
using namespace std;
double elapsed_dartsThreadPriv0[MAXNUMTHREADS][64];
double start_dartsThreadPriv0[MAXNUMTHREADS][64];
/*Function: elapsed_time, ID: 84*/
double elapsed_time()
{
    /*elapsed_time:84*/
    /*CompoundStmt:2107*/
    double t;
    wtime(&t);
    return (t);
}
/*Function: timer_clear, ID: 85*/
void timer_clear(int n)
{
    /*timer_clear:85*/
    /*CompoundStmt:2112*/
    ((elapsed_dartsThreadPriv0[0]))[n] = 0.;
}
/*Function: timer_start, ID: 86*/
void timer_start(int n)
{
    /*timer_start:86*/
    /*CompoundStmt:2116*/
    ((start_dartsThreadPriv0[0]))[n] = elapsed_time();
}
/*Function: timer_stop, ID: 87*/
void timer_stop(int n)
{
    /*timer_stop:87*/
    /*CompoundStmt:2120*/
    double t, now;
    now = elapsed_time();
    t = now - ((start_dartsThreadPriv0[0]))[n];
    ((elapsed_dartsThreadPriv0[0]))[n] += t;
}
/*Function: timer_read, ID: 88*/
double timer_read(int n)
{
    /*timer_read:88*/
    /*CompoundStmt:2129*/
    return (((elapsed_dartsThreadPriv0[0]))[n]);
}
