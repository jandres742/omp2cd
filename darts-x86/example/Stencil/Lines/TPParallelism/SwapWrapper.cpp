#include <cassert>
#include "darts.h"
#include "SwapWrapper.h"
#include "StencilWrapper.h"

using namespace darts;

SwapWrapper::SwapWrapper (uint32_t dep, uint32_t res, ThreadedProcedure * myTP, uint32_t stat):
Codelet(dep,res,myTP,stat) {
}

void SwapWrapper::fire () {
	StencilWrapper *tp = static_cast<StencilWrapper*>(myTP_);
	
	if (++tp->recursionDepth_ < MAX_REC_DEPTH) {
    invoke<StencilWrapper>(tp,tp->newA_, tp->oldA_, tp->matSize_,
						tp->recursionDepth_, tp->nbTPs_, tp->elemsPerCD_, tp->toSignal);
	} else {
		tp->toSignal->decDep();
	}
}
