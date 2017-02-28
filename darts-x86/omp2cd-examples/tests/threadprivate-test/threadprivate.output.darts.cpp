#include "threadprivate.output.darts.h"
using namespace darts;
using namespace std;
int b;
int tid;
int a_dartsThreadPriv0[MAXNUMTHREADS];
/*Function: main, ID: 1*/
int main(int argc, char* argv[])
{
    getOMPNumThreads();
    getOMPSchedulePolicy();
    getTPLoopThresholds();
    getNumTPs();
    affin = new ThreadAffinity(
        ompNumThreads / NUMTPS - 1, NUMTPS, COMPACT, getDARTSTPPolicy(), getDARTSMCPolicy());
    affinMaskRes = affin->generateMask();
    myDARTSRuntime = new Runtime(affin);
    RuntimeFinalCodelet = &(myDARTSRuntime->finalSignal);
    /*main:1*/
    /*CompoundStmt:5*/
    b = 0;
    if (affinMaskRes) {
        myDARTSRuntime->run(
            launch<TP7>(ompNumThreads * DARTS_CODELETS_MULT, 0, RuntimeFinalCodelet));
    }
    if (affinMaskRes) {
        myDARTSRuntime->run(
            launch<TP14>(ompNumThreads * DARTS_CODELETS_MULT, 0, RuntimeFinalCodelet));
    }
    return 0;
}
/*TP7: OMPParallelDirective*/
void TP7::_barrierCodelets7::fire(void)
{
    TP7* myTP = static_cast<TP7*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
void TP7::_checkInCodelets9::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 9: BinaryOperator*/
    (this->inputsTPParent->tid_darts7[this->getID()]) = omp_get_thread_num();

    /*printing node 11: BinaryOperator*/
    ((a_dartsThreadPriv0[this->getID()])) = (this->inputsTPParent->tid_darts7[this->getID()]) + 10;

    /*printing node 13: BinaryOperator*/
    (this->inputsTPParent->b_darts7[this->getID()])
        = (this->inputsTPParent->tid_darts7[this->getID()]);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets7[0].decDep();
}
TP7::TP7(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , b_darts7(new int[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , tid_darts7(new int[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , barrierCodelets7(new _barrierCodelets7[1])
    , checkInCodelets9(new _checkInCodelets9[this->numThreads])
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets7[0] = _barrierCodelets7(ompNumThreads, ompNumThreads, this, 0);
    _checkInCodelets9* checkInCodelets9Ptr = (this->checkInCodelets9);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets9Ptr) = _checkInCodelets9(1, 1, this, codeletCounter);
        (*checkInCodelets9Ptr).decDep();
        checkInCodelets9Ptr++;
    }
}
TP7::~TP7()
{
    delete[] barrierCodelets7;
    delete[] checkInCodelets9;
}
/*TP14: OMPParallelDirective*/
void TP14::_barrierCodelets14::fire(void)
{
    TP14* myTP = static_cast<TP14*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
void TP14::_checkInCodelets16::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 16: BinaryOperator*/
    (this->inputsTPParent->tid_darts14[this->getID()]) = omp_get_thread_num();

    /*printing node 18: IfStmt*/
    if (((a_dartsThreadPriv0[this->getID()]))
        != (this->inputsTPParent->tid_darts14[this->getID()]) + 10) {
        abort();
    }

    /*printing node 22: IfStmt*/
    if (b != 0) {
        abort();
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets14[0].decDep();
}
TP14::TP14(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , tid_darts14(new int[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , barrierCodelets14(new _barrierCodelets14[1])
    , checkInCodelets16(new _checkInCodelets16[this->numThreads])
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets14[0] = _barrierCodelets14(ompNumThreads, ompNumThreads, this, 0);
    _checkInCodelets16* checkInCodelets16Ptr = (this->checkInCodelets16);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets16Ptr) = _checkInCodelets16(1, 1, this, codeletCounter);
        (*checkInCodelets16Ptr).decDep();
        checkInCodelets16Ptr++;
    }
}
TP14::~TP14()
{
    delete[] barrierCodelets14;
    delete[] checkInCodelets16;
}
