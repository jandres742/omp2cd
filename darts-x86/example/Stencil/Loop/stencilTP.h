#pragma once
#include "darts.h"

using namespace darts;

class stencil : public ThreadedProcedure
{
public:
   
    class rec: public loop
    {
    public:
        
        class iloop: public loop
        {
        public:
            
            class jloop: public loop
            {
            public:
                
                class compute : public Codelet
                {
                    double * oldA;
                    double * newA;
                    jloop * myLoop;
                public:
                    compute(uint32_t dep, uint32_t res, ThreadedProcedure * myTP, uint32_t stat, double * oa, double * na, jloop * lp):
                    Codelet(dep,res,myTP,stat),
                    oldA(oa),
                    newA(na),
                    myLoop(lp) { }

                    virtual void fire(void)
                    {   
                        double * oldB;
                        double * newB;
                        if(myLoop->which%2)
                        {
                            oldB = newA;
                            newB = oldA;
                            //std::cout << "0";
                        }
                        else
                        {
                            oldB = oldA;
                            newB = newA;
                            //std::cout << "1";
                        }
                        
                        int size = myLoop->size;
                        int tile = myLoop->tile;
                        int i = myLoop->i;
                        int j = myLoop->j;
                        int startRow = i * tile;
                        int endRow = startRow + ( (i!=size/tile - 1) ? tile : (tile - 1 + size%tile) );
                        int startCol = j * tile;
                        int endCol = startCol + ( (j!=size/tile - 1) ? tile : (tile - 1 + size%tile) );
                        
                        if(!startRow)
                            startRow++;
                        if(!startCol)
                            startCol++;
                        //std::cout << " SR: " << startRow << " ER: " << endRow << " SC: " << startCol << " EC: " << endCol << std::endl; 
                        for (int I = startRow; I < endRow; I++) 
                        {
                            for (int J = startCol; J < endCol; J++) 
                            {
                                newB[I*size+J] = oldB[(I-1)*size+J] + oldB [(I+1)*size+J] + oldB [I*size + J - 1] + oldB [I*size + J + 1]; 
                            }
                        }
                        myLoop->toSignal->decDep();
                    }
                };
                
                int i;
                int j;
                int which;
                int size;
                int tile;
                
                compute comp;
                
                jloop(unsigned int it, Codelet * toSig, int sz, int tl, double * oa, double * na, int I, int wh):
                loop(it,toSig),
                i(I),
                j(it),
                which(wh),
                size(sz),
                tile(tl),
                comp(0,0,this,SHORTWAIT,oa,na,this)
                {                           
                    add(&comp);
                }
            };
            
            codeletFor<jloop> jl;
            iloop(unsigned int it, Codelet * toSig, int sz, int tl, double * oa, double * na, int wh):
            loop(it,toSig),
            jl(0,1,this,SHORTWAIT,toSig,sz/tl,sz,tl,oa,na,it,wh)
            { 
                add(&jl);
            }
        };
        
        paraFor<iloop> il;
        
        rec(unsigned int it, Codelet * toSig, int sz, int tl, double * oa, double * na):
        loop(it,toSig),
        il(0,1,this,SHORTWAIT,toSig,sz/tl,sz,tl,oa,na,it)        
        {
            add(&il);
        }        
    };
    
    int size;
    int tile;
    
    double * oldA;
    double * newA;
    
    Codelet * toSignal;
    
    serialFor<rec> rc;
    
    stencil(int sz, int tl, double * oa, double * na, int dp, Codelet * toSig):
    size(sz),
    tile(tl),
    oldA(oa),
    newA(na),
    toSignal(toSig),
    rc(0,1,this,SHORTWAIT,toSig,dp,sz,tl,oldA,newA)
    {
        add(&rc);
    }   
};


