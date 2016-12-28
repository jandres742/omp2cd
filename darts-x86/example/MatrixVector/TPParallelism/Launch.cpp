#include <cassert>
#include "darts.h"
#include "MatrixVector.h"
#include "MatrixVectorWrapper.h"
#include "Launch.h"

using namespace darts;

Launch::Launch (uint32_t dep, uint32_t res, ThreadedProcedure * myTP, uint32_t stat):
Codelet(dep,res,myTP,stat) {
}

void Launch::fire() {
	MatrixVectorWrapper *tp = static_cast<MatrixVectorWrapper*>(myTP_);
	assert (tp != NULL);
	
	unsigned nbTPs = tp->nbTPs_;
	unsigned rowsPerTP;
	unsigned first_task_id = 0;
	unsigned matSize = tp->matSize_;
	double *vec = tp->vec_;
	double *mat = tp->mat_;
	double *res = tp->res_;
	Codelet *swap = &tp->swap;
	unsigned elemsPerCD = tp->elemsPerCD_;
	
	assert (nbTPs > 0);
	
	for (unsigned i = 0; i < nbTPs; ++i) {
		rowsPerTP = matSize / nbTPs;

		if (i < matSize % nbTPs) {
			rowsPerTP++;
		}
		
		//std::cerr << "MatrixVectorWrapper::launch --> Create TP #" << i << ", with rowsPerTP = " << rowsPerTP << std::endl;

		/* Creates and spawns the new TPs */
    invoke<MatrixVector>(tp,vec, mat, res, matSize, rowsPerTP, swap, elemsPerCD, first_task_id);

		first_task_id += rowsPerTP;
	}
}