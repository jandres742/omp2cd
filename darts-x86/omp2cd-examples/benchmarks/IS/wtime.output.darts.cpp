#include "wtime.output.darts.h"
using namespace darts;
using namespace std;
int sec_darts7;
bool sec_darts7_initFlag;
/*Function: wtime, ID: 7*/
void wtime(double* t)
{
    /*wtime:7*/
    /*CompoundStmt:1088*/
    static int sec = -1;
    struct timeval tv;
    gettimeofday(&tv, (void*)0);
    if (sec < 0)
        sec = tv.tv_sec;
    *t = (tv.tv_sec - sec) + 9.9999999999999995E-7 * tv.tv_usec;
}
