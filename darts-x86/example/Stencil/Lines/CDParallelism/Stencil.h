#pragma once
#ifndef H_STENCILCD
#define H_STENCILCD

#include "Codelet.h"
#include "Swap.h"
#include "Compute.h"
#include "ThreadedProcedure.h"

class Stencil : public darts::ThreadedProcedure
{
public:
  //These are the frame variables
  
  double *oldA_;
  double *newA_;
  size_t matSize_;
  unsigned recursionDepth_;
  unsigned elemsPerCD_;
	unsigned nbCodelets_;

  //This is who to signal when complete
  darts::Codelet * toSignal;
  /* Statically allocated swap codelet */
  Swap swap;
  /* Array of compute codelets */
  Compute *compute;
  //Constructor
  Stencil (double *oldA, double *newA, size_t matSize, unsigned recursionDepth, unsigned elemsPerCD, darts::Codelet* toSig);
  ~Stencil ();
};

#endif
