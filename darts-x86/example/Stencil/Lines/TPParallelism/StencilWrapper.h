#pragma once
#ifndef H_STENCILWRAPPERTP
#define H_STENCILWRAPPERTP

#include "Codelet.h"
#include "Swap.h"
#include "Compute.h"
#include "ThreadedProcedure.h"
#include "Stencil.h"
#include "Launch.h"
#include "SwapWrapper.h"
  /**
	 * Class: StencilWrapper_
	 * Wraps the TP creation and swap codelet
	*/
class StencilWrapper : public darts::ThreadedProcedure
{
public:
	/**
	 * Variable: oldA_
	 * Represents the array of the previous iteration (previous recursion call in this case)
	*/
  double *oldA_;
	
	/**
	 * Variable: newA_
	 * Represents the array that will be computed during this iteration
	*/
  double *newA_;
	
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
	 * Variable: elemsPerCD_
	 * Number of rows computed by a codelet
	*/\
	unsigned elemsPerCD_;
	
	/**
	 * Variable: launch
	 * Codelet which spawns all the TP children
	 */
   Launch launch;
	
	/**
	 * Variable: swap
	 * Is the codelet that waits for all the TP children to end before eventually
	 * spawn a new StencilWrapper TP swapping old and new arrays
	 */
  SwapWrapper swap;
	
	darts::Codelet *toSignal;

  StencilWrapper (double *oldA, double *newA, size_t matSize, unsigned recursionDepth,
						unsigned nbTPs, unsigned elemsPerCD, darts::Codelet* toSig);
  ~StencilWrapper ();
};

#endif
