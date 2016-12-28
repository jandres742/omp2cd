#include "theTPs.h"

mainTP::mainTP(int in0, int *out0):in0(in0), out0(out0), mainWaitCodelet(1,1,this,LONGWAIT)
{
	invoke<tp0>(this, in0, out0, &mainWaitCodelet);
}

tp0::tp0(int in0, int * out0, Codelet * toSig): in0(in0), out0(out0), actionCodelet(0,0,this,SHORTWAIT), waitCodelet(2,2,this,LONGWAIT), toSignal(toSig) 
{ 
	add(&actionCodelet); 
}

