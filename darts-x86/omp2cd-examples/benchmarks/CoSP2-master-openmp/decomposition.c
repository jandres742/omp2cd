/// \file
/// Parallel domain decomposition.  

#include "decomposition.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "parallel.h"
#include "constants.h"

/// \param [in] nprocs total number of processors.
/// \param [in] nrows total number of rows.
/// \param [in] ncols maximum number of non-zero columns per row.
Domain* initDecomposition(int nprocs, int nrows, int ncols)
{
   // assert( nprocs == getNRanks());

   Domain* dd = (Domain*)malloc(sizeof(Domain));
   dd->totalProcs = nprocs;
   dd->totalRows = nrows;
   dd->totalCols = ncols;

   if (printRank() && debug == 1)
   printf("total procs = %d  total rows = %d  total cols = %d\n",
     dd->totalProcs, dd->totalRows, dd->totalCols);

   // initialialize global bounds
   dd->globalRowMin = 0;
   dd->globalRowMax = nrows;
   dd->globalRowExtent = dd->globalRowMax - dd->globalRowMin;

   if (printRank() && debug == 1)
   printf("global row min = %d  row max = %d  row extent = %d\n",
     dd->globalRowMin, dd->globalRowMax, dd->globalRowExtent);
   
   // initialize local bounds on this processor
   int myRank = getMyRank();
   dd->localRowExtent = dd->globalRowExtent / dd->totalProcs;
   dd->localRowMin = dd->globalRowMin +  myRank    * dd->localRowExtent;
   dd->localRowMax = dd->globalRowMin + (myRank+1) * dd->localRowExtent;

   if (debug == 1)
     printf("rank = %d local row min = %d  row max = %d  row extent = %d\n",
       getMyRank(), dd->localRowMin, dd->localRowMax, dd->localRowExtent);

   return dd;
}

/// \details
/// Deallocate decomposition
void destroyDecomposition(struct DomainSt* domain)
{
  free(domain);
}

/// \details
/// Calculates the rank of the processor with a given row.
int processorNum(Domain* domain, int rnum)
{
   return rnum / domain->localRowExtent;
}
