#include "critical-atomic.output.darts.h"
using namespace darts;
using namespace std;
std::mutex TP24mutex;
std::mutex TP29mutex;
/*Function: main, ID: 1*/
int main(int argc, char** argv)
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
    volatile int count_int_atomic = 0;
    volatile int count_critical = 0;
    double count_double_atomic = 0;
    int num_threads = 16;
    int inc = 5;
    omp_set_num_threads(num_threads);
    if (affinMaskRes) {
        myDARTSRuntime->run(
            launch<TP12>(ompNumThreads * DARTS_CODELETS_MULT, 0, RuntimeFinalCodelet,
                (volatile int*)&((count_critical)), (double*)&((count_double_atomic)),
                (volatile int*)&((count_int_atomic)), (int*)&((inc))));
    }
    if (count_int_atomic != 3 * num_threads) {
        printf("Error int atomic, count_int_atomic = %d != 3*num_threads = %d\n", count_int_atomic,
            3 * num_threads);
    }
    if (count_double_atomic != 5 * num_threads) {
        printf("Error double atomic, count_double_atomic = %f != 5*num_threads = %d\n",
            count_double_atomic, 3 * num_threads);
    }
    if (count_critical != 2 * num_threads) {
        printf("Error critical, count_critical = %d != 2*num_threads = %d\n", count_critical,
            2 * num_threads);
    }
    return 0;
}
/*TP12: OMPParallelDirective*/
void TP12::_barrierCodelets12::fire(void)
{
    TP12* myTP = static_cast<TP12*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
void TP12::_checkInCodelets15::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 15: UnaryOperator*/
    __sync_add_and_fetch(&((*(this->inputsTPParent->count_int_atomic_darts12))), 1);

    /*printing node 20: CompoundAssignOperator*/
    __sync_fetch_and_add(&((*(this->inputsTPParent->count_int_atomic_darts12))), 2);

    /*printing node 25: CompoundAssignOperator*/
    TP24mutex.lock();
    (*(this->inputsTPParent->count_double_atomic_darts12))
        += (*(this->inputsTPParent->inc_darts12));
    TP24mutex.unlock();

    /*printing node 31: UnaryOperator*/
    TP29mutex.lock();
    (*(this->inputsTPParent->count_critical_darts12))++;

    /*printing node 32: UnaryOperator*/
    (*(this->inputsTPParent->count_critical_darts12))++;
    TP29mutex.unlock();
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets12[0].decDep();
}
TP12::TP12(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet,
    volatile int* in_count_critical, double* in_count_double_atomic,
    volatile int* in_count_int_atomic, int* in_inc)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , count_critical_darts12(in_count_critical) /*OMP_SHARED - INPUT*/
    , count_double_atomic_darts12(in_count_double_atomic) /*OMP_SHARED - INPUT*/
    , count_int_atomic_darts12(in_count_int_atomic) /*OMP_SHARED - INPUT*/
    , inc_darts12(in_inc) /*OMP_SHARED - INPUT*/
    , barrierCodelets12(new _barrierCodelets12[1])
    , checkInCodelets15(new _checkInCodelets15[this->numThreads])
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets12[0] = _barrierCodelets12(ompNumThreads, ompNumThreads, this, 0);
    _checkInCodelets15* checkInCodelets15Ptr = (this->checkInCodelets15);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets15Ptr) = _checkInCodelets15(1, 1, this, codeletCounter);
        (*checkInCodelets15Ptr).decDep();
        checkInCodelets15Ptr++;
    }
}
TP12::~TP12()
{
    delete[] barrierCodelets12;
    delete[] checkInCodelets15;
}
