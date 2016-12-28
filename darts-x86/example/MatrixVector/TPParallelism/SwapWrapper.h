/* 
 * File:   SwapWrapper.h
 * Author: jphalimi
 *
 * Created on May 31, 2012, 5:26 PM
 */

#ifndef SWAPWRAPPERMVTP_H
#define	SWAPWRAPPERMVTP_H

#include <Codelet.h>
#include <ThreadedProcedure.h>

#define MAX_REC_DEPTH 10

class SwapWrapper : public darts::Codelet {
	public:
		SwapWrapper () {}
		SwapWrapper (uint32_t dep, uint32_t res, darts::ThreadedProcedure *myTP, uint32_t stat);
		
		virtual void fire ();
};

#endif	/* SWAPWRAPPER_H */

