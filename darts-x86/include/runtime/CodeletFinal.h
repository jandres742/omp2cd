#ifndef CODELETFINAL_H
#define CODELETFINAL_H
#include "Codelet.h"
namespace darts {
class CodeletFinal : public Codelet {
private:
    ABCScheduler** aliveSig;
    unsigned int numAliveSig;
    unsigned int numThreads;
    bool terminate;

public:
    CodeletFinal()
        : Codelet(1, 1)
        , numAliveSig(0)
        , numThreads(0)
        , terminate(true)
    {
    }

    void setTerminate(bool cond) { terminate = cond; }

    bool getTerminate(void) { return terminate; }

    unsigned int getNumThreads(void) { return numThreads; }

    void addAliveSignal(size_t index, ABCScheduler* toAdd)
    {
        assert(index < numThreads);

        aliveSig[index] = toAdd;
        numAliveSig++;
    }

    void setNumThreads(size_t num)
    {
        numThreads = num;
        aliveSig = new ABCScheduler*[num];
        for (unsigned int i = 0; i < numThreads; i++)
            aliveSig[i] = 0;
    }

    virtual void decDep(void)
    {
        if (terminate) {
            for (unsigned int i = 0; i < numThreads; i++) {
                aliveSig[i]->kill();
            }
            delete[] aliveSig;
        } else
            aliveSig[0]->kill();
    }

    virtual void fire(void){};
};
}
#endif /* CODELETFINAL_H */
