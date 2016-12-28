#include <cassert>
#include "darts.h"
#include "Stencil.h"
#include "StencilWrapper.h"
#include "Launch.h"

using namespace darts;

Launch::Launch (uint32_t dep, uint32_t res, ThreadedProcedure * myTP, uint32_t stat):
Codelet(dep,res,myTP,stat) {
}

void Launch::fire() {
	StencilWrapper *tp = static_cast<StencilWrapper*>(myTP_);
	assert (tp != 0);
	
	unsigned nbTPs = tp->nbTPs_;
	unsigned rowsPerTP;
	unsigned matSize = tp->matSize_;
	double *oldA = tp->oldA_;
	double *newA = tp->newA_;
	Codelet *swap = &tp->swap;
	unsigned elemsPerCD = tp->elemsPerCD_;
	
	//std::cerr << "Launch ctor (" << matSize << " matSize, nbTPs = " << nbTPs << ")" << std::endl;
	assert (nbTPs > 0);
	
	unsigned firstTaskId = 0;
	for (unsigned i = 0; i < nbTPs; ++i) {
		rowsPerTP = matSize / nbTPs;

		if (i < matSize % nbTPs) {
			rowsPerTP++;
		}
		
		/* Creates and spawns the new TPs */
    invoke<Stencil>(tp,oldA, newA, matSize, rowsPerTP, swap, elemsPerCD, firstTaskId);
		firstTaskId += rowsPerTP;
	}
}