#pragma once
#ifndef H_MATRIXVECTORWRAPPERTP
#define H_MATRIXVECTORWRAPPERTP

#include "Codelet.h"
#include "Swap.h"
#include "Compute.h"
#include "ThreadedProcedure.h"
#include "MatrixVector.h"
#include "Launch.h"
#include "SwapWrapper.h"
  /**
	 * Class: MatrixVectorWrapper_
	 * Wraps the TP creation and swap codelet
	*/
class MatrixVectorWrapper : public darts::ThreadedProcedure
{
public:
	/**
	 * Variable: vec_
	 * Vector to be multiplied
	*/
  double *vec_;
	
	/**
	 * Variable: mat_
	 * Matrix to be multiplied
	*/
  double *mat_;
	
	/**
	 * Variable: res_
	 * Result of computation
	*/
  double *res_;
	
	/**
	 * Variable: matSize_
	 * Size of the input matrix
	*/
  size_t matSize_;
	
	/**
	 * Variable: recursionDepth_
	 * Level of depth of the recursion
	*/
  unsigned recursionDepth_;
	
	/**
	 * Variable: nbTPs_
	 * Number of TP children we want to spawn
	*/
  unsigned nbTPs_;
	
	/**
	 * Variable: launch
	 * Codelet which spawns all the TP children
	 */
   Launch launch;
	
	/**
	 * Variable: swap
	 * Is the codelet that waits for all the TP children to end
	 */
  SwapWrapper swap;
	
	unsigned elemsPerCD_;
	
	darts::Codelet *toSignal;

  MatrixVectorWrapper (double *vec, double *mat, double *res, size_t matSize, unsigned nbTPs,
											unsigned elemsPerCD, darts::Codelet* toSig);
  ~MatrixVectorWrapper ();
};

#endif
