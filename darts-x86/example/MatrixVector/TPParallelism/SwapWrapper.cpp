#include <cassert>
#include <ThreadedProcedure.h>
#include <Codelet.h>
#include "SwapWrapper.h"
#include "MatrixVectorWrapper.h"

using namespace darts;

SwapWrapper::SwapWrapper (uint32_t dep, uint32_t res, ThreadedProcedure * myTP, uint32_t stat):
Codelet(dep,res,myTP,stat) {
}

void SwapWrapper::fire () {
	MatrixVectorWrapper *tp = static_cast<MatrixVectorWrapper*>(myTP_);
	tp->toSignal->decDep();
}
