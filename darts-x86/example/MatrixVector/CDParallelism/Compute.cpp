#include <cstdio>
#include "Compute.h"
#include "Codelet.h"
#include "MatrixVector.h"
#include "ThreadedProcedure.h"

using namespace darts;

Compute::Compute () {
}

Compute::Compute(uint32_t dep, uint32_t res, ThreadedProcedure * myTP, uint32_t st,
								unsigned nbElems, unsigned firstTaskId):
Codelet(dep,res,myTP,st),
nbElems_ (nbElems),
firstTaskId_ (firstTaskId) {
}

void Compute::initCompute (uint32_t dep, uint32_t res, ThreadedProcedure * myTP, uint32_t stat,
								unsigned nbElems, unsigned firstTaskId) {
	initCodelet (dep, res, myTP, stat);
	nbElems_ = nbElems;
	firstTaskId_ = firstTaskId;
}

void Compute::fire(void) {
	//std::cerr << "Entered codelet " << id << " Compute" << std::endl;

	MatVector *tp = static_cast<MatVector*>(myTP_);
	size_t matSize = tp->matSize_;
	unsigned i, j;
	double *mat = tp->mat_;
	double *vec = tp->vec_;
	double *res = tp->res_;
	double tmpRes;
	unsigned rowStart = firstTaskId_;
	unsigned rowStop = rowStart + nbElems_;
	
	//fprintf (stderr, "#%u: row = [%u,%u], col = [%u,%u], resTmpStartIdx = %u\n", id, rowStart, rowStop, colStart, colStop, resTmpStartIdx);

	for (i = rowStart; i < rowStop; i++) {
		tmpRes = 0.;
		for (j = 0; j < matSize; ++j) {
			tmpRes += mat[i*matSize + j] * vec[j];
		}
		res[i] = tmpRes;
	}

	tp->end.decDep ();
	//std::cerr << "Finished codelet Compute" << std::endl;
}