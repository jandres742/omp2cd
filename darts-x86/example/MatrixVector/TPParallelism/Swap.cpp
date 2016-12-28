#include "Swap.h"
#include "Codelet.h"
#include "ThreadedProcedure.h"
#include "MatrixVector.h"
#include "Runtime.h"
#include <cassert>

using namespace darts;

Swap::Swap (uint32_t dep, uint32_t res, ThreadedProcedure * myTP, uint32_t stat):
Codelet(dep,res,myTP,stat) {
}

void Swap::fire () {
  MatrixVector *tp = static_cast<MatrixVector*>(myTP_);
  tp->toSignal->decDep ();
}