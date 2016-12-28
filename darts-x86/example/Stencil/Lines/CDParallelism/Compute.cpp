#include "Compute.h"
#include "Codelet.h"
#include "ThreadedProcedure.h"
#include "Stencil.h"
#include <cassert>
#include <iostream>

using namespace darts;

Compute::Compute () {
}

Compute::Compute(uint32_t dep, uint32_t res, ThreadedProcedure * myTP, uint32_t stat,
					unsigned elemsPerCD, unsigned firstTaskId):
  Codelet(dep,res,myTP,stat),
	firstTaskId_ (firstTaskId),
	nbElems_ (elemsPerCD)
{}

void Compute::initCompute (uint32_t dep, uint32_t res, ThreadedProcedure * myTP, uint32_t stat,
					unsigned elemsPerCD, unsigned firstTaskId) {
	initCodelet (dep, res, myTP, stat);
	firstTaskId_ = firstTaskId;
	nbElems_ = elemsPerCD;
}
 
void Compute::fire () {
  Stencil *tp = static_cast<Stencil*>(myTP_);
  size_t matSize = tp->matSize_;
  size_t i,j;
  double *newA = tp->newA_;
  double *oldA = tp->oldA_;
  double startRow = firstTaskId_;
  double stopRow = startRow + nbElems_;

  /* Upsets the load balancing a bit for the first and last codelets */
  /* Because we don't want to use the first and last row */
  if (startRow == 0)  startRow = 1;
  if (stopRow == matSize)  stopRow--;
	
	//std::cerr << "start,stop" << startRow << "," << stopRow << std::endl;

  /* Effective computation */
  for (i = startRow; i < stopRow; i++) {
    for (j = 1; j < matSize-1; j++) {
      newA[i*matSize+j] = oldA[(i-1)*matSize+j] + oldA [(i+1)*matSize+j] + oldA [i*matSize + j - 1] + oldA [i*matSize + j + 1];
    }
  }

  tp->swap.decDep ();
}

