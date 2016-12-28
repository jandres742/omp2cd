/* 
 * File:	 Compute.h
 * Author: jphalimi
 *
 * Created on May 16, 2012, 10:26 AM
 */

#pragma once
#ifndef COMPUTEMVCD_H
#define	COMPUTEMVCD_H

#include "Codelet.h"
#include "ThreadedProcedure.h"

class Compute : public darts::Codelet
{
private:
	unsigned nbElems_;
	unsigned firstTaskId_;
public:
	Compute ();
	Compute(uint32_t dep, uint32_t res, darts::ThreadedProcedure * myTP, uint32_t stat,
										unsigned nbElems, unsigned firstTaskId);
	void initCompute (uint32_t dep, uint32_t res, darts::ThreadedProcedure * myTP, uint32_t stat,
										unsigned nbElems, unsigned firstTaskId);
	
	virtual void fire(void);
};

#endif	/* COMPUTE_H */

