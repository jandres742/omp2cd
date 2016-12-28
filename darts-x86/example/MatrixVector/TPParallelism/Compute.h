#pragma once
#ifndef H_COMPUTEMVTP
#define H_COMPUTEMVTP

#include "Codelet.h"
#include "ThreadedProcedure.h"

class Compute : public darts::Codelet {
private:
	unsigned rowsNb_;
	unsigned firstTaskId;
public:
  /* Constructors */
  Compute ();
  Compute (uint32_t dep, uint32_t res, darts::ThreadedProcedure * myTP, uint32_t stat, unsigned rowsNb, unsigned first_task_id);
  
  /* Initializes the codelet after its construction */
  void initCompute (uint32_t dep, uint32_t res, darts::ThreadedProcedure * myTP, uint32_t stat, unsigned rowsNb, unsigned first_task_id);
  
  virtual void fire(void);
};


#endif
