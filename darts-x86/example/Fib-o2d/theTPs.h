#ifndef _THETPS_H_
#define _THETPS_H_

#include <iostream>
#include <stdlib.h>
#include <stdint.h>
#include "darts.h"

#include "theCodelets.h"

using namespace darts;

class mainTP : public ThreadedProcedure
{
public:
	
	int in0;
	
	int *out0;

    theMainWaitCodelet mainWaitCodelet;
	
	mainTP(int in0, int *out0);
};


class tp0 : public ThreadedProcedure
{
public:
    
    int in0;
	
	int *out0; 

	int inDep0;
    int inDep1;
     
	theActionCodelet actionCodelet;
    theWaitCodelet waitCodelet;
    
    Codelet * toSignal;
    
    tp0(int in0, int * out0, Codelet * toSig);
    
};

#endif