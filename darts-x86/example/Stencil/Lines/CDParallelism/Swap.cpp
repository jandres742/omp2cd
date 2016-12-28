#include "Swap.h"
#include "darts.h"
#include "Stencil.h"
#include <cassert>

using namespace darts;

Swap::Swap (uint32_t dep, uint32_t res, ThreadedProcedure * myTP, uint32_t stat):
Codelet(dep,res,myTP,stat) {
}

void Swap::fire () {
  Stencil *tp = static_cast<Stencil*>(myTP_);

  if (++tp->recursionDepth_ < MAX_REC_DEPTH) {
		//std::cerr << "constructing next stencil iteration " << tp->recursionDepth_ << std::endl;
    invoke<Stencil>(tp,tp->newA_, tp->oldA_,
                      tp->matSize_, tp->recursionDepth_, tp->elemsPerCD_, tp->toSignal);
  } else { /* We finished the MAX_REC_DEPTH recursion calls */
    tp->toSignal->decDep ();
    //ReturnTP ();
  }
}
