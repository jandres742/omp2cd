/* 
 * File:   Launch.h
 * Author: jphalimi
 *
 * Created on May 31, 2012, 3:25 PM
 */

#pragma once
#ifndef LAUNCHMVTP_H
#define	LAUNCHMVTP_H

#include <Codelet.h>
#include <ThreadedProcedure.h>

class Launch : public darts::Codelet {
	public:
		Launch (uint32_t dep, uint32_t res, darts::ThreadedProcedure * myTP, uint32_t stat);
		
		virtual void fire ();
};

#endif	/* LAUNCH_H */

