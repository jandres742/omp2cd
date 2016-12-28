#include <cassert>
#include <iostream>
#include "Stencil.h"
#include "Codelet.h"
#include "Compute.h"
#include "ThreadedProcedure.h"

using namespace darts;

Stencil::Stencil (double *oldA, double *newA, size_t matSize, unsigned recursionDepth, unsigned elemsPerCD, Codelet* toSig):
ThreadedProcedure(),
oldA_(oldA),
newA_(newA),
matSize_(matSize),
recursionDepth_(recursionDepth),
elemsPerCD_(elemsPerCD),
toSignal(toSig),
compute (0) {
	nbCodelets_ = matSize_ / elemsPerCD_;
	if (matSize % elemsPerCD != 0) {
		nbCodelets_++;
	}
	assert (nbCodelets_ > 0);

	swap.initCodelet (nbCodelets_, nbCodelets_, this, SHORTWAIT);
	compute = new Compute [nbCodelets_];
	assert (compute != 0);
  add(&swap);

	unsigned firstTaskId = 0;
	for (unsigned i = 0; i < nbCodelets_; ++i) {
		if (matSize % elemsPerCD != 0 && i == nbCodelets_-1) {
			elemsPerCD  = matSize % elemsPerCD;
		}
		compute[i].initCompute (0, 0, this, SHORTWAIT, elemsPerCD, firstTaskId);
		firstTaskId += elemsPerCD;
    add(compute+i);
	}
}

Stencil::~Stencil () {
    delete [] compute, compute = 0;
}
