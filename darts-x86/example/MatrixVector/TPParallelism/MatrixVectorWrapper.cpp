#include <cassert>
#include <ThreadedProcedure.h>
#include <Codelet.h>
#include "MatrixVectorWrapper.h"
#include "SwapWrapper.h"
#include "Launch.h"

using namespace darts;

MatrixVectorWrapper::MatrixVectorWrapper (double *vec, double *mat, double *res, size_t matSize,
																					unsigned nbTPs, unsigned elemsPerCD, Codelet* toSig):
ThreadedProcedure(),
vec_ (vec),
mat_ (mat),
res_ (res),
matSize_ (matSize),
nbTPs_ (nbTPs),
launch (0, 0, this, SHORTWAIT),
elemsPerCD_ (elemsPerCD),
toSignal (toSig) {
	assert(matSize > 2);
	assert (vec != 0);
	assert (mat != 0);
	assert (res != 0);
	assert (nbTPs > 0);

	if (matSize < nbTPs)  nbTPs_ = matSize; /* Reduce the amount of TPs if matSize is too small */

	// Swap codelet init
	swap.initCodelet (nbTPs_, nbTPs_, this, SHORTWAIT);
    add(&launch);
    add(&swap);
}

MatrixVectorWrapper::~MatrixVectorWrapper () {}
