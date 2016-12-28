#include "Compute.h"
#include "Codelet.h"
#include "ThreadedProcedure.h"
#include "MatrixVector.h"
#include <cassert>
#include <iostream>

using namespace darts;

Compute::Compute () {
}

Compute::Compute (uint32_t dep, uint32_t res, ThreadedProcedure * myTP, uint32_t stat,
									unsigned rowsNb, unsigned first_task_id):
  Codelet(dep,res,myTP,stat),
	rowsNb_ (rowsNb),
	firstTaskId (first_task_id) {
}

void Compute::initCompute (uint32_t dep, uint32_t res, ThreadedProcedure * myTP, uint32_t stat,
													 unsigned rowsNb, unsigned first_task_id) {
    initCodelet (dep, res, myTP, stat);
		rowsNb_ = rowsNb;
		firstTaskId = first_task_id;
}
 
void Compute::fire () {
  MatrixVector *tp = static_cast<MatrixVector*>(myTP_);
  size_t matSize = tp->matSize_;
  size_t i, j;
	double *vec = tp->vec_;
  double *mat = tp->mat_;
  double *res = tp->res_;
  unsigned startRow = firstTaskId;
  unsigned stopRow = startRow + rowsNb_;
	double tmpRes;
	
	//std::cerr << "Codelet Id #" << id_ << "{firstTaskId,startRow, stopRow} = {" << firstTaskId << "," << startRow << "," << stopRow << "}" << std::endl;
	
	for (i = startRow; i < stopRow; i++) {
		tmpRes = 0.;
		for (j = 0; j < matSize; ++j) {
			tmpRes += mat[i*matSize + j] * vec[j];
		}
		res[i] = tmpRes;
	}

  tp->swap.decDep ();
}

