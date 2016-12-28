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

	nbCodelets_ = rowsNb / elemsPerCD;
	if (rowsNb % elemsPerCD != 0 || nbCodelets_ == 0) {
		nbCodelets_++;
	}

	assert (nbCodelets_ > 0);

	compute = new Compute [nbCodelets_];
	assert (compute != 0);

	unsigned CDfirstTaskId = firstTaskId;
	for (unsigned i = 0; i < nbCodelets_; ++i) {
		if (rowsNb % elemsPerCD != 0 && i == nbCodelets_-1) {
			elemsPerCD  = rowsNb % elemsPerCD;
		}
		//std::cerr << "#" << i << ": " << elemsPerCD << " elems per CD" << std::endl;
		//std::cerr << "Stencil --> Create codelet #" << i << ", with rowsPerCD = " << rowsPerCD << std::endl;
		compute[i].initCompute (0, 0, this, SHORTWAIT, elemsPerCD, i, CDfirstTaskId);
		CDfirstTaskId += elemsPerCD;
    add(compute+i);
	}

	swap.initCodelet (nbCodelets_, nbCodelets_, this, SHORTWAIT);
  add(&swap);
}

Stencil::~Stencil () {
	delete [] compute, compute = 0;

}

