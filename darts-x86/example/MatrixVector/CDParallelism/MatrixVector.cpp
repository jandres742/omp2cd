#include <cassert>
#include <cstdlib>
#include "Codelet.h"
#include "ThreadedProcedure.h"
#include "Runtime.h"
#include "MatrixVector.h"
#include "Compute.h"
#include "End.h"

using namespace darts;

MatVector::MatVector(double *vec, double *mat, double *res, size_t matSize,
										unsigned elemsPerCD, Codelet *toSig):
ThreadedProcedure(),
vec_(vec),
mat_(mat),
res_(res),
matSize_(matSize),
elemsPerCD_ (elemsPerCD),
toSignal(toSig) {
	assert (matSize != 0);
	assert (vec != 0);
	assert (mat != 0);
	assert (res != 0);
	assert (elemsPerCD != 0);
	assert (toSig != 0);
	
	nbCodelets_ = matSize / elemsPerCD;
	if (matSize % elemsPerCD != 0) {
		nbCodelets_++;
	}
	assert (nbCodelets_ > 0);
	
	compute = new Compute [nbCodelets_];
	assert (compute != 0);
	
	unsigned firstTaskId = 0;
	for (unsigned i = 0; i < nbCodelets_; ++i) {
		if (matSize % elemsPerCD != 0 && i == nbCodelets_-1) {
			elemsPerCD = matSize % elemsPerCD;
		}
		compute[i].initCompute (0, 0, this, SHORTWAIT, elemsPerCD, firstTaskId);
		firstTaskId += elemsPerCD;
    add(compute + i);
	}
	end.initCodelet(nbCodelets_, nbCodelets_, this, SHORTWAIT);
  add(&end);
}

MatVector::~MatVector () {
	delete [] compute, compute = 0;
}
