#pragma once
#ifndef H_SWAPSTCD
#define H_SWAPSTCD

#include "Codelet.h"
#include "ThreadedProcedure.h"

#define MAX_REC_DEPTH 10


class Swap : public darts::Codelet
{
public:
  Swap () {}
  Swap (uint32_t dep, uint32_t res, darts::ThreadedProcedure * myTP, uint32_t stat);
  virtual void fire ();
};

#endif
