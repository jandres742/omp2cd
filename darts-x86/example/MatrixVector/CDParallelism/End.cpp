#include "End.h"
#include "Codelet.h"
#include "ThreadedProcedure.h"
#include "MatrixVector.h"

using namespace darts;

End::End () {
}

End::End(uint32_t dep, uint32_t res, ThreadedProcedure * myTP, uint32_t st):
	Codelet(dep,res,myTP,st) {
}

void End::fire(void)	{
	//std::cerr << "Entered codelet End" << std::endl;
	MatVector *tp = static_cast<MatVector*>(myTP_);

	tp->toSignal->decDep ();
	//std::cerr << "Finished codelet End" << std::endl;
	//ReturnTP ();
}