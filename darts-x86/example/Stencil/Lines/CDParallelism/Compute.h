#pragma once
#ifndef H_COMPUTESTCD
#define H_COMPUTESTCD

#include "Codelet.h"
#include "ThreadedProcedure.h"

class Compute : public darts::Codelet {
private:
	unsigned firstTaskId_;
  unsigned id_;
	unsigned nbElems_;
public:
  /* Constructors */
  Compute ();
  Compute(uint32_t dep, uint32_t res, darts::ThreadedProcedure * myTP, uint32_t stat, unsigned elemsPerCD, unsigned firstTaskId);
  
  /* Initializes the codelet after its construction */
  void initCompute (uint32_t dep, uint32_t res, darts::ThreadedProcedure * myTP, uint32_t stat, unsigned elemsPerCD, unsigned firstTaskId);
  
  virtual void fire(void);
};


#endif
