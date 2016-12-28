#pragma once
#ifndef H_COMPUTESTTP
#define H_COMPUTESTTP

#include "Codelet.h"
#include "ThreadedProcedure.h"

class Compute : public darts::Codelet {
private:
	unsigned rowsNb_;
	unsigned colsNb_;
  unsigned id_;
	unsigned firstTaskId_;
	unsigned firstTaskIdx_;
public:
  /* Constructors */
  Compute ();
  Compute (uint32_t dep, uint32_t res, darts::ThreadedProcedure * myTP, uint32_t stat, unsigned rowsNb, unsigned colsNb,
					unsigned id, unsigned firstTaskId, unsigned firstTaskIdx);
  
  /* Initializes the codelet after its construction */
  void initCompute (uint32_t dep, uint32_t res, darts::ThreadedProcedure * myTP, uint32_t stat,
								unsigned rowsNb, unsigned colsNb, unsigned id, unsigned firstTaskId, unsigned firstTaskIdx);
  
  virtual void fire(void);
};

#endif
