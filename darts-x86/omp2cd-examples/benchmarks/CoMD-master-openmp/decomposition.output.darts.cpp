#include "decomposition.output.darts.h"
using namespace darts;
using namespace std;
/*Function: initDecomposition, ID: 37*/
struct DomainSt* initDecomposition(int xproc, int yproc, int zproc, real3 globalExtent)
{
    /*initDecomposition:37*/
    /*CompoundStmt:2309*/
    Domain* dd = comdMalloc(sizeof(Domain));
    dd->procGrid[0] = xproc;
    dd->procGrid[1] = yproc;
    dd->procGrid[2] = zproc;
    int myRank = getMyRank();
    dd->procCoord[0] = myRank % dd->procGrid[0];
    myRank /= dd->procGrid[0];
    dd->procCoord[1] = myRank % dd->procGrid[1];
    dd->procCoord[2] = myRank / dd->procGrid[1];
    for (int i = 0; i < 3; i++) {
        dd->globalMin[i] = 0;
        dd->globalMax[i] = globalExtent[i];
        dd->globalExtent[i] = dd->globalMax[i] - dd->globalMin[i];
    }
    for (int i = 0; i < 3; i++) {
        dd->localExtent[i] = dd->globalExtent[i] / dd->procGrid[i];
        dd->localMin[i] = dd->globalMin[i] + dd->procCoord[i] * dd->localExtent[i];
        dd->localMax[i] = dd->globalMin[i] + (dd->procCoord[i] + 1) * dd->localExtent[i];
    }
    return dd;
}
/*Function: processorNum, ID: 38*/
int processorNum(Domain* domain, int dix, int diy, int diz)
{
    /*processorNum:38*/
    /*CompoundStmt:2402*/
    const int* procCoord = domain->procCoord;
    const int* procGrid = domain->procGrid;
    int ix = (procCoord[0] + dix + procGrid[0]) % procGrid[0];
    int iy = (procCoord[1] + diy + procGrid[1]) % procGrid[1];
    int iz = (procCoord[2] + diz + procGrid[2]) % procGrid[2];
    return ix + procGrid[0] * (iy + procGrid[1] * iz);
}
