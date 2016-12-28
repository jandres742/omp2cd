#pragma once
#ifndef H_STENCILSTTP
#define H_STENCILSTTP

#include "Codelet.h"
#include "Swap.h"
#include "Compute.h"
#include "ThreadedProcedure.h"

class Stencil : public darts::ThreadedProcedure
{
public:
  double *oldA_;
  double *newA_;
  size_t matSize_;
	unsigned nbCodelets_;
	unsigned elemsPerCD;
	unsigned rowsNb_;

  //This is who to signal when complete
  darts::Codelet *toSignal;
	
  /* Local Swap codelet */
  Swap swap;
  /* Array of compute codelets */
  Compute *compute;
	
  //Constructor
  Stencil (double *oldA, double *newA, size_t matSize,
					unsigned rowsNb, darts::Codelet* toSig, unsigned elemsPerCD, unsigned firstTaskId);
  ~Stencil ();
};

#endif
