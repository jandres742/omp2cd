/* 
 * File:   End.h
 * Author: jphalimi
 *
 * Created on May 16, 2012, 10:34 AM
 */

#pragma once
#ifndef ENDMVCD_H
#define	ENDMVCD_H

#include "Codelet.h"
#include "ThreadedProcedure.h"

class End : public darts::Codelet
{
public:
	End ();
	End(uint32_t dep, uint32_t res, darts::ThreadedProcedure * myTP, uint32_t stat);
	virtual void fire(void);
};

#endif	/* END_H */

