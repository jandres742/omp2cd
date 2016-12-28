using namespace darts;

//These are forward declarations
class outerSource : public Codelet
{
public:
  outerSource(uint32_t dep, uint32_t res, ThreadedProcedure * myTP, uint32_t stat):
  Codelet(dep,res,myTP,stat) { }
  virtual void funct(void);
};

class outerSinc : public Codelet
{
public:
  outerSinc(uint32_t dep, uint32_t res, ThreadedProcedure * myTP, uint32_t stat):
  Codelet(dep,res,myTP,stat) { }
  virtual void funct(void);
};

//This class calls the serial test
class tpWrapper : public ThreadedProcedure
{
public:
    //These are the frame variables
    int fanout;
    int depth;
    
    int warmUp;
    int outerLoop;
    int innerLoop;
    int tempInnerLoop;
    
    uint64_t startTime;
    uint64_t endTime;
    
    thread_safe::vector<uint64_t> * aves;
    thread_safe::vector<uint64_t> results;
    
    outerSource source;
    outerSinc sinc;
    
    Codelet * toSignal;
    //Constructor
    tpWrapper(int theFanout, int theDepth, int theOuterLoop, int theInnerLoop, 
              thread_safe::vector<uint64_t> * res, Codelet * toSig):
    ThreadedProcedure(),
    fanout(theFanout),
    depth(theDepth),
    warmUp(0),
    outerLoop(theOuterLoop),
    innerLoop(theInnerLoop),
    tempInnerLoop(theInnerLoop),
    aves(res),
    source(0,1,this,SHORTWAIT),
    sinc(1,1,this,LONGWAIT),
    toSignal(toSig){ }
    //This function creates the codelets
    void buildCDG(void)
    {
        addCodelet(&source);
        addCodelet(&sinc);
    }
};

//This is the beginning of the loop
void 
outerSource::funct(void)
{
    tpWrapper * myTpWrapper = static_cast<tpWrapper*>(myTP_);
    if(myTpWrapper->warmUp)
        myTpWrapper->sinc.resetCodelet();
    //std::cout << "source" << std::endl;
    //Spawn the test call
    test * newTest = new test(myTpWrapper->fanout, myTpWrapper->depth, &myTpWrapper->startTime, &myTpWrapper->endTime, &myTpWrapper->sinc);
    add(newTest);
}

//This is the second half of the loop
void 
outerSinc::funct(void)
{ 
    tpWrapper * myTpWrapper = static_cast<tpWrapper*>(myTP_);
    //std::cout << "sinc" << std::endl;
    //Check the loop
    if(myTpWrapper->outerLoop)
    {
        if(myTpWrapper->innerLoop)
        {
            if(myTpWrapper->warmUp)
            {
                //dec the loop and create the second half and give it to the runtime
                myTpWrapper->innerLoop--;
                myTpWrapper->results.push_back(myTpWrapper->endTime - myTpWrapper->startTime);
            }
            else
            {
                myTpWrapper->warmUp = 1;
            }
            myTpWrapper->source.resetCodelet();
            myTpWrapper->source.decDep();
        }
        if(!myTpWrapper->innerLoop)
        {
            //std::cout << "doing aves size: " << myTpWrapper->results.size() << std::endl;
            myTpWrapper->aves->push_back(ave(myTpWrapper->results));
            myTpWrapper->outerLoop--;
            myTpWrapper->innerLoop = myTpWrapper->tempInnerLoop;
            myTpWrapper->results.clear();
        }       
    }
    //end the TP
    else
    {
        myTpWrapper->toSignal->decDep();
    }   
}


