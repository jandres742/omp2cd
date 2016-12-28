/* 
 * File:	 MatrixVector.h
 * Author: jphalimi
 *
 * Created on May 16, 2012, 10:24 AM
 */

#pragma once
#ifndef MATRIXVECTORCD_H
#define	MATRIXVECTORCD_H

#include "ThreadedProcedure.h"
#include "Codelet.h"
#include "End.h"
#include "Compute.h"

class MatVector : public darts::ThreadedProcedure
{
public:
	//These are the frame variables
	double *vec_;
	double *mat_;
	double *res_;
	size_t matSize_;
	unsigned nbCodelets_;
	unsigned elemsPerCD_;
	Compute *compute;
	End end;

	//This is who to signal when complete
	darts::Codelet * toSignal;
	//Constructor
	MatVector(double *vec, double *mat, double *res, size_t matSize, unsigned elemsPerCD, darts::Codelet * toSig);
	~MatVector ();
};

#endif	/* MATRIXVECTOR_H */
