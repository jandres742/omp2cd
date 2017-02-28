#include "wtime.output.darts.h"
using namespace darts;
using namespace std;
int sec_darts90;
bool sec_darts90_initFlag;
/*Function: wtime, ID: 90*/
void wtime(double* t)
{
    /*wtime:90*/
    /*CompoundStmt:2086*/
    static int sec = -1;
    struct timeval tv;
    gettimeofday(&tv, (void*)0);
    if (sec < 0)
        sec = tv.tv_sec;
    *t = (tv.tv_sec - sec) + 9.9999999999999995E-7 * tv.tv_usec;
}
