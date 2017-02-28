#include "timer.output.darts.h"
using namespace darts;
using namespace std;
int initted_darts8;
bool initted_darts8_initFlag;
struct timeval tvlast_darts8;
/*Function: timer, ID: 8*/
double timer()
{
    /*timer:8*/
    /*CompoundStmt:4009*/
    const double million = 1.0E+6;
    static struct timeval tvlast;
    static int initted = 0;
    struct timeval tv;
    struct timezone tz;
    double ret, t0, t1;
    gettimeofday(&tv, &tz);
    if (!initted) {
        tvlast = tv;
        initted = 1;
    }
    t0 = tvlast.tv_sec * million + tvlast.tv_usec;
    t1 = tv.tv_sec * million + tv.tv_usec;
    ret = (t1 - t0) / million;
    tvlast = tv;
    return ret;
}
/*Function: timer2, ID: 9*/
double timer2()
{
    /*timer2:9*/
    /*CompoundStmt:4040*/
    const double million = 1.0E+6;
    struct timeval tv;
    struct timezone tz;
    double ret, t1;
    gettimeofday(&tv, &tz);
    t1 = tv.tv_sec * million + tv.tv_usec;
    ret = t1 / million;
    return ret;
}
