#include "random.output.darts.h"
using namespace darts;
using namespace std;
double convertToDouble_darts166;
bool convertToDouble_darts166_initFlag;
/*Function: gasdev, ID: 165*/
real_t gasdev(uint64_t* seed)
{
    /*gasdev:165*/
    /*CompoundStmt:846*/
    real_t rsq, v1, v2;
    do {
        v1 = 2. * lcg61(seed) - 1.;
        v2 = 2. * lcg61(seed) - 1.;
        rsq = v1 * v1 + v2 * v2;
    } while (rsq >= 1. || rsq == 0.);
    return v2 * sqrt(-2. * log(rsq) / rsq);
}
/*Function: lcg61, ID: 166*/
double lcg61(uint64_t* seed)
{
    /*lcg61:166*/
    /*CompoundStmt:879*/
    static const double convertToDouble = 1. / 2305843009213693951UL;
    *seed *= 437799614237992725UL;
    *seed %= 2305843009213693951UL;
    return *seed * convertToDouble;
}
/*Function: mkSeed, ID: 167*/
uint64_t mkSeed(uint32_t id, uint32_t callSite)
{
    /*mkSeed:167*/
    /*CompoundStmt:890*/
    uint32_t s1 = id * 2654435761U;
    uint32_t s2 = (id + callSite) * 2654435761U;
    uint64_t iSeed = (4294967296UL * s1) + s2;
    for (unsigned int jj = 0; jj < 10; ++jj)
        lcg61(&iSeed);
    return iSeed;
}
