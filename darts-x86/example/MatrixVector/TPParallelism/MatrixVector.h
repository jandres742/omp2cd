#pragma once
#ifndef H_MATRIXVECTORTP
#define H_MATRIXVECTORTP

#include "Codelet.h"
#include "Swap.h"
#include "Compute.h"
#include "ThreadedProcedure.h"

class MatrixVector : public darts::ThreadedProcedure
{
public:
  double *vec_;
  double *mat_;
	double *res_;
  size_t matSize_;
	unsigned nbCodelets_;
	unsigned rowsNb_;

  //This is who to signal when complete
  darts::Codelet *toSignal;
	
  /* Local Swap codelet */
  Swap swap;
  /* Array of compute codelets */
  Compute *compute;
	
  //Constructor
  MatrixVector (double *vec, double *mat, double *res, size_t matSize,
					unsigned rowsNb, darts::Codelet* toSig, unsigned elemsPerCD, unsigned first_task_id);
  ~MatrixVector ();
};

#endif
