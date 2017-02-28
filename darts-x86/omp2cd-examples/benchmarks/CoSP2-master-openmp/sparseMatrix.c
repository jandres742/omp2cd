/// \File
/// Routines for creating and manipulating sparse matrices.

#include "sparseMatrix.h"

/// \details
/// Adjust number of non-zeroes
int nnzStart(int hsize, int msize)
{
  int M = msize;
  if (M == 0) M = hsize;
  if ((M % 32) > 0) M += (32 - (M % 32));
  if (M > hsize) M = hsize;
  if (printRank()) printf("Adjusted M = %d\n", M);

  return M;
}

/// \details
/// Allocate space for sparse matrix
SparseMatrix* initSparseMatrix(int hsize, int msize)
{
  SparseMatrix* spmatrix = (SparseMatrix*)malloc(sizeof(SparseMatrix));

  // hsize is number of rows and msize is the max number of non-zeroes per row
  spmatrix->hsize = hsize;
  spmatrix->msize = msize;
  
  // iia holds the number of non-zeroes in each row
  spmatrix->iia = (int*)malloc(hsize*sizeof(int));

#ifdef CONTIG_MATRIX
  spmatrix->jjcontig = (int*)malloc(hsize*msize*sizeof(int));
  spmatrix->jja = (int**)malloc(hsize*sizeof(int*));
  #pragma omp parallel for
  for (int i = 0; i < hsize; i++)
  {
    spmatrix->jja[i] = &(spmatrix->jjcontig[i*msize]);
  }

  // Zero counts of non-zeroes per row and indices
  memset(spmatrix->jjcontig, 0, hsize*msize*sizeof(int));
  
  spmatrix->valcontig = (real_t*)malloc(hsize*msize*sizeof(real_t));
  spmatrix->val = (real_t**)malloc(hsize*sizeof(real_t*));
  #pragma omp parallel for
  for (int i = 0; i < hsize; i++)
  {
    spmatrix->val[i] = &(spmatrix->valcontig[i*msize]);
  }
  
  // Zero non-zero values
  memset(spmatrix->valcontig, ZERO, hsize*msize*sizeof(real_t));
 
#else

  // jja contains the column index for each non-zero value
  spmatrix->jja = (int**)malloc(hsize*sizeof(int*));
  for (int i = 0; i < hsize; i++)
  {
    spmatrix->jja[i] = (int*)malloc(msize*sizeof(int)); 
  }

  // val contains the non-zeroes
  spmatrix->val = (real_t**)malloc(hsize*sizeof(real_t*));
  for (int i = 0; i < hsize; i++)
  {
    spmatrix->val[i] = (real_t*)malloc(msize*sizeof(real_t));
  }
#endif

  // Zero counts of non-zeroes per row
  memset(spmatrix->iia, 0, hsize*sizeof(int));

  // Used for normalization
  spmatrix->maxEval = ZERO;
  spmatrix->minEval = ZERO;
  spmatrix->maxMinusMin = ZERO;

  // Matrix bandwidth
  spmatrix->bandwidth = 0;

  return spmatrix;
}

/// \details
/// Deallocate space for sparse matrix
void destroySparseMatrix(struct SparseMatrixSt* spmatrix)
{
  int hsize = spmatrix->hsize;

  free(spmatrix->iia);

#ifdef CONTIG_MATRIX
  free(spmatrix->jjcontig);
  free(spmatrix->jja);

  free(spmatrix->valcontig);
  free(spmatrix->val);
#else
  for (int i = 0; i < hsize; i++)
  {
    //free(spmatrix->jja[i]);
  }
  free(spmatrix->jja);

  for (int i = 0; i < hsize; i++)
  {
    free(spmatrix->val[i]);
  }
  free(spmatrix->val);
#endif

  spmatrix->hsize = 0;
  spmatrix->msize = 0;
  spmatrix->bandwidth = 0;

  spmatrix->minEval = ZERO;
  spmatrix->maxEval = ZERO;
  spmatrix->maxMinusMin = ZERO;
}

/// \details
/// Calculate sparcity statistics for a sparse matrix
void sparsity(struct SparseMatrixSt* spmatrix)
{
  int hsize = spmatrix->hsize;
  int hValCount=0;
  int hDist[hsize];

  memset(hDist, 0, hsize*sizeof(int));

  for (int i = 0; i < hsize; i++)
  {
    hValCount += spmatrix->iia[i];
    if (spmatrix->iia[i] > 0)
      hDist[spmatrix->iia[i]] += 1;
  }

  if (printRank())
  {
    printf("\nSparsity:\nInitial sparsity = %d, fraction = %e, Avg per row = %f\n", hValCount,
       (real_t)hValCount/(real_t)(hsize*hsize), (real_t)hValCount/(real_t)hsize);
    int maxRowCount = 0;
    for (int i = 0; i < hsize; i++)
    {
       maxRowCount = MAX(maxRowCount, spmatrix->iia[i]);
    }
    printf("Max per row = %d\n", maxRowCount);
    for (int i = 0; i < hsize; i++)
    {
      if (hDist[i] > 0)
         printf("I = %d, count = %d, fraction = %f\n", 
           i, hDist[i], (real_t)hDist[i]/(real_t)hsize);
    }
  }
}

/// \details
/// Calculate gershgorin bounds for sparse matrix
void gershgorin(struct SparseMatrixSt* spmatrix, struct DomainSt* domain)
{
  int hsize = spmatrix->hsize;
  real_t eMin = 10000;
  real_t eMax = -10000;

  real_t sumP, sumM, maxMinusMin;

  #pragma omp parallel for private(sumM,sumP) reduction(max:eMax) reduction(min:eMin)
  for(int i = 0; i < hsize; i++)
  {
    sumM = 0.0;

    for(int j = 0; j < spmatrix->iia[i]; j++) {
      real_t hx = ABS(spmatrix->val[i][j]);
      sumM += hx;
      if (spmatrix->jja[i][j] == i)
      {
        sumP = spmatrix->val[i][j];
        sumM -= hx;
      }
    }
    eMax = ((eMax < (sumP + sumM)) ? sumP + sumM : eMax);
    eMin = ((eMin > (sumP - sumM)) ? sumP - sumM : eMin);

  }

  // Determine eMax and eMin across ranks
#ifdef DO_MPI
  if (getNRanks() > 1)
  {
    startTimer(reduceCommTimer);
    minRealReduce(&eMin);
    stopTimer(reduceCommTimer);
    collectCounter(reduceCounter, sizeof(real_t));
   
    startTimer(reduceCommTimer);
    maxRealReduce(&eMax);
    stopTimer(reduceCommTimer);
    collectCounter(reduceCounter, sizeof(real_t));
  }
#endif
    
  maxMinusMin = eMax-eMin;

  if (printRank()) 
    printf("\nGershgorin:\nNew  eMax, eMin = %e, %e\n", eMax, eMin); // GERSGORIN BOUNDS;

  spmatrix->maxEval = eMax;
  spmatrix->minEval = eMin;
  spmatrix->maxMinusMin = maxMinusMin;
}

/// \details
/// Normalize a matrix in sparse format using the gershgorin estimates
void normalize(struct SparseMatrixSt* spmatrix)
{
  int hsize = spmatrix->hsize;
  int sumIia = 0;
  int maxIia = 0;

  #pragma omp parallel for reduction(+:sumIia) reduction(max:maxIia)
  for(int i = 0; i < hsize; i++)
  {
    for(int j = 0; j < spmatrix->iia[i]; j++)
    {
      if (spmatrix->jja[i][j] == i) {
        spmatrix->val[i][j] = (spmatrix->maxEval - spmatrix->val[i][j])/spmatrix->maxMinusMin;
      } else {
        spmatrix->val[i][j] = -spmatrix->val[i][j]/spmatrix->maxMinusMin;
      }
    }
    sumIia += spmatrix->iia[i];
    maxIia = MAX(maxIia, spmatrix->iia[i]);
 }

 // WE NOW HAVE X = (eMax*I-H)/(eMax-eMin)
 if (printRank() && debug == 1)
   printf("Initial sparsity normalized = %d, fraction = %e,  avg = %g, max = %d\n",
       sumIia, (real_t)sumIia/(real_t)(hsize*hsize), (real_t)sumIia/(real_t)hsize, maxIia);
}

/// \details
/// Calculate trace and trace^2 for a sparse matrix.
void trace(struct SparseMatrixSt* spmatrix, struct DomainSt* domain, real_t* tr, real_t* tr2)
{
  int hsize = spmatrix->hsize;
  real_t trace = ZERO;
  real_t trace2 = ZERO;

  #pragma omp parallel for reduction(+:trace, trace2)
  for(int i = domain->localRowMin; i < domain->localRowMax; i++)
  {
#ifdef POS1
    // Diagonal values are in first position
    trace += spmatrix->val[i][0];
    trace2 += spmatrix->val[i][0] * spmatrix->val[i][0];
#else
    for(int j = 0; j < spmatrix->iia[i]; j++)
    {
      if (i == spmatrix->jja[i][j])
      {
        trace += spmatrix->val[i][j];
        trace2 += spmatrix->val[i][j] * spmatrix->val[i][j];
      }
    }
#endif
  }
  
  *tr = trace;
  *tr2 = trace2;
}
