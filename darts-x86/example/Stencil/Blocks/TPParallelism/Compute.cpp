#include "Compute.h"
#include "Codelet.h"
#include "ThreadedProcedure.h"
#include "Stencil.h"
#include <cassert>
#include <iostream>

using namespace darts;

Compute::Compute () {
}

Compute::Compute (uint32_t dep, uint32_t res, ThreadedProcedure * myTP, uint32_t stat,
									unsigned rowsNb, unsigned colsNb, unsigned id, unsigned firstTaskId, unsigned firstTaskIdx):
  Codelet(dep,res,myTP,stat),
	rowsNb_ (rowsNb),
	colsNb_ (colsNb),
	id_(id),
	firstTaskId_ (firstTaskId),
	firstTaskIdx_ (firstTaskIdx) {
}

void Compute::initCompute (uint32_t dep, uint32_t res, ThreadedProcedure * myTP, uint32_t stat,
											unsigned rowsNb, unsigned colsNb, unsigned id, unsigned firstTaskId, unsigned firstTaskIdx) {
	initCodelet (dep, res, myTP, stat);
	rowsNb_ = rowsNb;
	colsNb_ = colsNb;
	id_ = id;
	firstTaskId_ = firstTaskId;
	firstTaskIdx_ = firstTaskIdx;
}
 
void Compute::fire () {
  Stencil *tp = static_cast<Stencil*>(myTP_);
  size_t matSize = tp->matSize_;
  size_t i, j;
  double *newA = tp->newA_;
  double *oldA = tp->oldA_;
  unsigned startRow = firstTaskId_;
  unsigned stopRow = startRow + rowsNb_;
  unsigned startCol = firstTaskIdx_;
  unsigned stopCol = startCol + colsNb_;

  /* Upsets the load balancing a bit for the first and last thread */
  /* Because we don't want to use the first and last row */
  if (startRow == 0)  startRow = 1;
  if (stopRow == matSize)  stopRow--;
  if (startCol == 0)  startCol = 1;
  if (stopCol == matSize)  stopCol--;
	
	//std::cerr << "Codelet Id #" << id_ << ": {" << firstTaskId_ << "," << startRow << "," << stopRow << "," << firstTaskIdx_ << "," << startCol << "," << stopCol << "}" << std::endl;

  /* Effective computation */
  for (i = startRow; i < stopRow; i++) {
    for (j = startCol; j < stopCol; j++) {
      newA[i*matSize+j] = oldA[(i-1)*matSize+j] + oldA [(i+1)*matSize+j] + oldA [i*matSize + j - 1] + oldA [i*matSize + j + 1];
    }
  }

  tp->swap.decDep ();
}
