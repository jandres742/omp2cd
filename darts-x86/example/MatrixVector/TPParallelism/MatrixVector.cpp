#include <cassert>
#include <Codelet.h>
#include <ThreadedProcedure.h>
#include "MatrixVector.h"
#include "Compute.h"

using namespace darts;

MatrixVector::MatrixVector (double *vec, double *mat, double *res, size_t matSize,
					unsigned rowsNb, Codelet* toSig, unsigned elemsPerCD, unsigned first_task_id):
ThreadedProcedure(),
vec_(vec),
mat_(mat),
res_(res),
matSize_(matSize),
rowsNb_ (rowsNb),
toSignal(toSig) {
	assert (rowsNb > 0); // The TP has to do at least one line

	nbCodelets_ = rowsNb / elemsPerCD;
	if (rowsNb % elemsPerCD != 0) {
		nbCodelets_++;
	}
	assert (nbCodelets_ > 0);

	compute = new Compute [nbCodelets_];
	assert (compute != 0);

	unsigned CDfirstTaskId = first_task_id;
	for (unsigned i = 0; i < nbCodelets_; ++i) {
		if (rowsNb % elemsPerCD != 0 && i == nbCodelets_-1) {
			elemsPerCD = rowsNb % elemsPerCD;
		}

		compute[i].initCompute (0, 0, this, SHORTWAIT, elemsPerCD, CDfirstTaskId);
		CDfirstTaskId += elemsPerCD;
    add(compute+i);
	}

	swap.initCodelet (nbCodelets_, nbCodelets_, this, SHORTWAIT);
  add(&swap);
}

MatrixVector::~MatrixVector () {
	delete [] compute, compute = 0;
}

