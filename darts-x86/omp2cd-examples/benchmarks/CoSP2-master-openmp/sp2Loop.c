/// \file
/// SP2 loop.

#include "sp2Loop.h"

/// \details
/// The secpnd order spectral projection algorithm.
void sp2Loop(struct SparseMatrixSt* xmatrix, struct DomainSt* domain)
{
  HaloExchange* haloExchange;

  startTimer(sp2LoopTimer);

#ifdef DO_MPI
  if (getNRanks() > 1)
    haloExchange = initHaloExchange(domain);
#endif

  int hsize = xmatrix->hsize;
  SparseMatrix* x2matrix = initSparseMatrix(xmatrix->hsize, xmatrix->msize);

  // Do gershgorin normalization
  startTimer(normTimer);
  normalize(xmatrix);
  stopTimer(normTimer);
  
  // Sparse SP2 algorithm
 
  real_t idempErr = ZERO;
  real_t idempErr1 = ZERO;
  real_t idempErr2 = ZERO;

  real_t occ = hsize*HALF;

  real_t trX = ZERO;
  real_t trX2 = ZERO;

  real_t tr2XX2, trXOLD, limDiff;

  int iter = 0;
  int breakLoop = 0;

  if (printRank() && debug == 1)
    printf("\nSP2Loop:\n");

  while ( breakLoop == 0 && iter < 100 )
  {
    trX = ZERO;
    trX2 = ZERO;

#ifdef DO_MPI
    if (getNRanks() > 1)
    {
      startTimer(exchangeTimer);
      exchangeSetup(haloExchange, xmatrix, domain);
      stopTimer(exchangeTimer);
    }
#endif

    // Matrix multiply X^2
    startTimer(x2Timer);
    sparseX2(&trX, &trX2, xmatrix, x2matrix, domain);
    stopTimer(x2Timer);

#ifdef DO_MPI
    // Reduce trace of X and X^2 across all processors
    if (getNRanks() > 1)
    {
      startTimer(reduceCommTimer);
      addRealReduce2(&trX, &trX2);
      stopTimer(reduceCommTimer);
      collectCounter(reduceCounter, 2 * sizeof(real_t));
    }
#endif

    if (printRank() && debug == 1) 
      printf("iter = %d  trX = %e  trX2 = %e\n", iter, trX, trX2);
 
    tr2XX2 = TWO*trX - trX2;
    trXOLD = trX;
    limDiff = ABS(trX2 - occ) - ABS(tr2XX2 - occ);

    if (limDiff > idemTol) 
    {
      // X = 2 * X - X^2
      trX = TWO * trX - trX2;

      startTimer(xaddTimer);
      sparseAdd(xmatrix, x2matrix, domain);
      stopTimer(xaddTimer);
    }
    else if (limDiff < -idemTol)
    {
      // X = X^2
      trX = trX2;

      startTimer(xsetTimer);
      sparseSetX2(xmatrix, x2matrix, domain);
      stopTimer(xsetTimer);
    }
    else 
    {
      trX = trXOLD;
      breakLoop = 1;
    }
         
    idempErr2 = idempErr1;
    idempErr1 = idempErr;
    idempErr = ABS(trX - trXOLD);    

    iter++;

    if (iter >= 25 && (idempErr >= idempErr2)) breakLoop = 1;

    // Exchange sparse matrix pieces across processors
#ifdef DO_MPI
    if (getNRanks() > 1)
    {
      startTimer(exchangeTimer);
      exchangeData(haloExchange, xmatrix, domain);
      stopTimer(exchangeTimer);
    }
#endif
  }

  stopTimer(sp2LoopTimer);

  // Multiply by 2
  sparseMultScalar(xmatrix, domain, TWO);
  
  // Report results
  reportResults(iter, xmatrix, x2matrix, domain);

#ifdef DO_MPI
  // Gather matrix to processor 0
  if (getNRanks() > 1)
    allGatherData(haloExchange, xmatrix, domain);
#endif

#ifdef DO_MPI
  if (getNRanks() > 1)
    destroyHaloExchange(haloExchange);
#endif
  destroySparseMatrix(x2matrix);
}

/// \details
/// Report density matrix results
void reportResults(int iter, struct SparseMatrixSt* xmatrix, struct SparseMatrixSt* x2matrix, struct DomainSt* domain)
{
  int hsize = xmatrix->hsize;

  int sumIIA= 0;
  int sumIIC= 0;
  int maxIIA= 0;
  int maxIIC= 0;

  // #pragma omp parallel for reduction(+:sumIIA,sumIIC) reduction(max:maxIIA,maxIIC)  
  for (int i = domain->localRowMin; i < domain->localRowMax; i++)
  {
    sumIIA += xmatrix->iia[i];
    sumIIC += x2matrix->iia[i];
	// if(i < 5)
		// fprintf(stderr, "report: x2matrix->iia[%d] = %d\n", i, x2matrix->iia[i]);
    maxIIA = MAX(maxIIA, xmatrix->iia[i]);
    maxIIC = MAX(maxIIC, x2matrix->iia[i]);
  }

#ifdef DO_MPI
  // Collect number of non-zeroes and max non-zeroes per row across ranks
  if (getNRanks() > 1)
  {
    startTimer(reduceCommTimer);
    addIntReduce2(&sumIIA, &sumIIC);
    stopTimer(reduceCommTimer);
    collectCounter(reduceCounter, 2 * sizeof(int));

    startTimer(reduceCommTimer);
    maxIntReduce2(&maxIIA, &maxIIC);
    stopTimer(reduceCommTimer);
    collectCounter(reduceCounter, 2 * sizeof(int));
  }
#endif

  if (printRank())
  {
    printf("\nResults:\n");
    printf("X2 Sparsity CCN = %d, fraction = %e avg = %g, max = %d\n", sumIIC, 
      (real_t)sumIIC/(real_t)(hsize*hsize), (real_t)sumIIC/(real_t)hsize, maxIIC);

    printf("D Sparsity AAN = %d, fraction = %e avg = %g, max = %d\n", sumIIA, 
      (real_t)sumIIA/(real_t)(hsize*hsize), (real_t)sumIIA/(real_t)hsize, maxIIA);

    printf("Number of iterations = %d\n", iter);
  }
  // exit(1);
}
