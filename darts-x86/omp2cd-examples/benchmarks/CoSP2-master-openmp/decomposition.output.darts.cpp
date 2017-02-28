#include "decomposition.output.darts.h"
using namespace darts;
using namespace std;
/*Function: initDecomposition, ID: 14*/
Domain* initDecomposition(int nprocs, int nrows, int ncols)
{
    /*initDecomposition:14*/
    /*CompoundStmt:2940*/
    Domain* dd = (Domain*)malloc(sizeof(Domain));
    dd->totalProcs = nprocs;
    dd->totalRows = nrows;
    dd->totalCols = ncols;
    if (printRank() && debug == 1)
        printf("total procs = %d  total rows = %d  total cols = %d\n", dd->totalProcs,
            dd->totalRows, dd->totalCols);
    dd->globalRowMin = 0;
    dd->globalRowMax = nrows;
    dd->globalRowExtent = dd->globalRowMax - dd->globalRowMin;
    if (printRank() && debug == 1)
        printf("global row min = %d  row max = %d  row extent = %d\n", dd->globalRowMin,
            dd->globalRowMax, dd->globalRowExtent);
    int myRank = getMyRank();
    dd->localRowExtent = dd->globalRowExtent / dd->totalProcs;
    dd->localRowMin = dd->globalRowMin + myRank * dd->localRowExtent;
    dd->localRowMax = dd->globalRowMin + (myRank + 1) * dd->localRowExtent;
    if (debug == 1)
        printf("rank = %d local row min = %d  row max = %d  row extent = %d\n", getMyRank(),
            dd->localRowMin, dd->localRowMax, dd->localRowExtent);
    return dd;
}
/*Function: destroyDecomposition, ID: 15*/
void destroyDecomposition(struct DomainSt* domain)
{
    /*destroyDecomposition:15*/
    /*CompoundStmt:3003*/
    free(domain);
}
/*Function: processorNum, ID: 16*/
int processorNum(Domain* domain, int rnum)
{
    /*processorNum:16*/
    /*CompoundStmt:3005*/
    return rnum / domain->localRowExtent;
}
