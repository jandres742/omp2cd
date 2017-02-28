/// \file
/// Parallel domain decomposition.

#ifndef _DECOMPOSITION_H_
#define _DECOMPOSITION_H_

#include "mytype.h"

/// Domain decomposition information.
typedef struct DomainSt
{
   // process-layout data
   int totalProcs;        //!< number of processors
   int totalRows;         //!< total number of rows
   int totalCols;         //!< number of non-zero columns per row

   // global bounds data
   int globalRowMin;      //!< minimum global row number
   int globalRowMax;      //!< maximum global row number
   int globalRowExtent;   //!< total global rows

   // local bounds data
   int localRowMin;       //!< minimum row on local processor
   int localRowMax;       //!< maximum row on local processor
   int localRowExtent;    //!< localRowMax - localRowMin
} Domain;

/// Initialize domain
Domain* initDecomposition(int nprocs, int nrows, int ncols);

/// Done with domain
void destroyDecomposition(struct DomainSt* domain);

/// Find the MPI rank of a neighbor domain from a row number
int processorNum(Domain* domain, int rnum);

#endif
