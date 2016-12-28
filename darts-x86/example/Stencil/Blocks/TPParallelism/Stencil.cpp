#include <cassert>
#include <Codelet.h>
#include <ThreadedProcedure.h>
#include "Stencil.h"
#include "Compute.h"

using namespace darts;

Stencil::Stencil (double *oldA, double *newA, size_t matSize,
									unsigned rowsNb, Codelet* toSig, unsigned elemsPerCD, unsigned firstTaskId):
ThreadedProcedure(),
oldA_(oldA),
newA_(newA),
matSize_(matSize),
rowsNb_ (rowsNb),
toSignal(toSig) {
	//std::cerr << "Stencil ctor (" << matSize << " matSize)" << std::endl;
	assert (rowsNb > 0); // The TP has to do at least one line
	assert (elemsPerCD > 0);
	assert (matSize > 0);

	nbCodelets_ = matSize_ / elemsPerCD;
	if (matSize % elemsPerCD != 0 || nbCodelets_ == 0) {
		nbCodelets_++;
	}
	
	//std::cerr << "NbCodelets = " << nbCodelets_ << std::endl;

	assert (nbCodelets_ > 0);

	compute = new Compute [nbCodelets_];
	assert (compute != 0);

	unsigned CDfirstTaskId = 0;
	for (unsigned i = 0; i < nbCodelets_; ++i) {
		if (matSize % elemsPerCD != 0 && i == nbCodelets_-1) {
			elemsPerCD  = matSize % elemsPerCD;
		}
		compute[i].initCompute (0, 0, this, SHORTWAIT, rowsNb_, elemsPerCD, i, firstTaskId, CDfirstTaskId);
		CDfirstTaskId += elemsPerCD;
    add(compute+i);
	}

	swap.initCodelet (nbCodelets_, nbCodelets_, this, SHORTWAIT);
  add(&swap);
}

Stencil::~Stencil () {
	delete [] compute, compute = 0;

}
