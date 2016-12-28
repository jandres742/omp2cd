#ifndef _THECODELETS_H_
#define _THECODELETS_H_

#include <iostream>
#include <stdlib.h>
#include <stdint.h>
#include "darts.h"

using namespace darts;

class theActionCodelet : public Codelet
{
public:
	theActionCodelet(uint32_t dep, uint32_t res, ThreadedProcedure * myTP, uint32_t stat):	Codelet(dep,res,myTP,stat) { }
  
	virtual void fire(void);
};

class theWaitCodelet : public Codelet
{
public:
	
	theWaitCodelet(uint32_t dep, uint32_t res, ThreadedProcedure * myTP, uint32_t stat):	Codelet(dep,res,myTP,stat) { }
  
	virtual void fire(void);
};

class theMainWaitCodelet : public Codelet
{
public:
	theMainWaitCodelet(uint32_t dep, uint32_t res, ThreadedProcedure * myTP, uint32_t stat): Codelet(dep,res,myTP,stat) { }
  
	virtual void fire(void);
};

#endif