#pragma once
#ifndef H_COMPUTESTTP
#define H_COMPUTESTTP

#include "Codelet.h"
#include "ThreadedProcedure.h"

class Compute : public darts::Codelet {
private:
	unsigned rowsNb_;
  unsigned id_;
	unsigned firstTaskId_;
public:
  /* Constructors */
  Compute ();
  Compute (uint32_t dep, uint32_t res, darts::ThreadedProcedure * myTP, uint32_t stat, unsigned rowsNb,
					unsigned id, unsigned firstTaskId);
  
  /* Initializes the codelet after its construction */
  void initCompute (uint32_t dep, uint32_t res, darts::ThreadedProcedure * myTP, uint32_t stat,
								unsigned rowsNb, unsigned id, unsigned firstTaskId);
  
  virtual void fire(void);
};

#endif
