#include <cassert>
#include <ThreadedProcedure.h>
#include <Codelet.h>
#include "StencilWrapper.h"
#include "SwapWrapper.h"
#include "Launch.h"

using namespace darts;

StencilWrapper::StencilWrapper (double *oldA, double *newA, size_t matSize, unsigned recursionDepth,
						unsigned nbTPs,unsigned elemsPerCD, Codelet* toSig):
  ThreadedProcedure(),
  oldA_(oldA),
  newA_(newA),
  matSize_(matSize),
  recursionDepth_(recursionDepth),
  nbTPs_(nbTPs),
	elemsPerCD_ (elemsPerCD),
  launch (0, 0, this, SHORTWAIT), // Static init of the launch codelet
	swap (),
  toSignal(toSig)
{
		
		assert(matSize > 2);
		assert (oldA != NULL);
		assert (newA != NULL);
		assert (recursionDepth <= MAX_REC_DEPTH);
		assert (nbTPs > 0);
		assert (elemsPerCD > 0);
		
    if (matSize < nbTPs)  nbTPs_ = matSize; /* Reduce the amount of TPs if matSize is too small */
		
		// Swap codelet init
    swap.initCodelet (nbTPs_, nbTPs_, this, SHORTWAIT);
    add(&launch);
		add(&swap);
}

StencilWrapper::~StencilWrapper () {}
