#include "theCodelets.h"
#include "theTPs.h"

using namespace darts;

void theActionCodelet::fire(void)
{
    tp0 * myFib = static_cast<tp0*>(myTP_);
    
    if(myFib->in0<2)
    {
        (*myFib->out0) = myFib->in0;
		
        myFib->toSignal->decDep();
    }
    else
    {
        invoke<tp0>(myFib,myFib->in0-1,&myFib->inDep0,&myFib->waitCodelet);
		
        invoke<tp0>(myFib,myFib->in0-2,&myFib->inDep1,&myFib->waitCodelet);
    }
}
 
void theWaitCodelet::fire(void)
{ 
    tp0 * myFib = static_cast<tp0*>(myTP_);    
    
    (*myFib->out0) = myFib->inDep0 + myFib->inDep1;
	
    myFib->toSignal->decDep();
}


void theMainWaitCodelet::fire(void)
{
	std::cout << "Done!!" << std::endl;
	
	Runtime::finalSignal.decDep();
}

