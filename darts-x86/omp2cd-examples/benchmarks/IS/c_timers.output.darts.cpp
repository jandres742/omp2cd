#include "c_timers.output.darts.h"
using namespace darts;
using namespace std;
double elapsed_dartsThreadPriv0[MAXNUMTHREADS][64];
double start_dartsThreadPriv0[MAXNUMTHREADS][64];
/*Function: elapsed_time, ID: 1*/
double elapsed_time()
{
    /*elapsed_time:1*/
    /*CompoundStmt:1109*/
    double t;
    wtime(&t);
    return (t);
}
/*Function: timer_clear, ID: 2*/
void timer_clear(int n)
{
    /*timer_clear:2*/
    /*CompoundStmt:1114*/
    ((elapsed_dartsThreadPriv0[0]))[n] = 0.;
}
/*Function: timer_start, ID: 3*/
void timer_start(int n)
{
    /*timer_start:3*/
    /*CompoundStmt:1118*/
    ((start_dartsThreadPriv0[0]))[n] = elapsed_time();
}
/*Function: timer_stop, ID: 4*/
void timer_stop(int n)
{
    /*timer_stop:4*/
    /*CompoundStmt:1122*/
    double t, now;
    now = elapsed_time();
    t = now - ((start_dartsThreadPriv0[0]))[n];
    ((elapsed_dartsThreadPriv0[0]))[n] += t;
}
/*Function: timer_read, ID: 5*/
double timer_read(int n)
{
    /*timer_read:5*/
    /*CompoundStmt:1131*/
    return (((elapsed_dartsThreadPriv0[0]))[n]);
}
