#include "reduce-omp.output.darts.h"
using namespace darts;
using namespace std;
int ss = 8;
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
    srand(time((time_t*)((void*)0)));
    int N;
    if (argc <= 1)
        N = 1000;
    else
        N = atoi(argv[1]);
    if (N <= 0) {
        printf("Size should be greater than 0\n");
        return 0;
    }
    double* a = (double*)malloc(N * sizeof(double));
    double* A = (double*)malloc(N * sizeof(double));
    double* B = (double*)malloc(N * sizeof(double));
    int i;
    for (i = 0; i < N; i++) {
        *(a + i) = rand() % 10 - 5;
        *(A + i) = rand() % 10 - 5;
        *(B + i) = rand() % 10 - 5;
    }
    printf("Testing reduction\n");
    double resa_seq = 1;
    for (i = 0; i < 8; i++) {
        if (a[i] < resa_seq)
            resa_seq = a[i];
    }
    double resa_par = 1;
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP76>(ompNumThreads * DARTS_CODELETS_MULT, 0,
            RuntimeFinalCodelet, 0, 8, (double**)&((a)), (double*)&((resa_par))));
    }
    if (resa_par != resa_seq)
        printf("Error: resa_seq = %f, resa_par = %f\n", resa_seq, resa_par);
    else
        printf("Reduction successful resa_seq = %f, resa_par = %f\n", resa_seq, resa_par);
    return 0;
}
/*TP76: OMPParallelForDirective*/
void TP76::_barrierCodelets76::fire(void)
{
    TP76* myTP = static_cast<TP76*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
bool TP76::requestNewRangeIterations76(int* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Static */
    /*Chunk = 0*/
    bool isThereNewIteration = false;
    {
        /*Static Scheduling*/
        int tempStartRange = rangePerCodelet76 * codeletID;
        int tempEndRange = rangePerCodelet76 * (codeletID + 1);
        if (remainderRange76 != 0) {
            if (codeletID < (uint32_t)remainderRange76) {
                tempStartRange += codeletID;
                tempEndRange += (codeletID + 1);
            } else {
                tempStartRange += remainderRange76;
                tempEndRange += remainderRange76;
            }
        }
        tempStartRange = tempStartRange * 1 + minIteration76;
        tempEndRange = tempEndRange * 1 + minIteration76;
        if (tempStartRange != tempEndRange) {
            isThereNewIteration = true;
        }
        if (initIteration76 < lastIteration76) {
            (this->inputsTPParent->i_darts76[codeletID]) = min(tempStartRange, tempEndRange);
            *endRange = max(tempStartRange, tempEndRange);
        } else {
            (this->inputsTPParent->i_darts76[codeletID]) = max(tempStartRange, tempEndRange);
            *endRange = min(tempStartRange, tempEndRange);
        }
        if (codeletID == this->numThreads - 1) {
            *endRange = lastIteration76;
        }
    }
    return isThereNewIteration;
}
void TP76::_checkInCodelets77::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 77: ForStmt*/
    /*var: a*/
    /*var: i*/
    /*var: resa_par*/
    /*Declare local reduction vars*/
    double resa_par_LocalReductionVar = (double)(std::numeric_limits<double>::infinity());
    double** a = (this->inputsTPParent->a_darts76);
    (void)a /*OMP_SHARED*/;
    int* i = &(this->inputsTPParent->i_darts76[this->getLocalID()]);
    (void)i /*OMP_PRIVATE*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations76(
        (int*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        goto updateReduceVars;
    }
    for (int i_darts_counter_temp76 = (*i); i_darts_counter_temp76 < endRange
         && i_darts_counter_temp76 < this->inputsTPParent->lastIteration76;
         i_darts_counter_temp76++) {
        {
            if ((*(a))[(i_darts_counter_temp76)] < (*(&(resa_par_LocalReductionVar)))) {
                (*(&(resa_par_LocalReductionVar))) = (*(a))[(i_darts_counter_temp76)];
            }
        }
    }
updateReduceVars:
    /*Before signaling next codelet, check if all the threads in the TP
    * performed the reduction locally and if so, update the unique
    * reduction variable.*/
    /*Reducing for operator 'min'*/
    this->inputsTPParent->resa_par_darts76_mutex.lock();
    if (*(this->inputsTPParent->resa_par_darts76) > (resa_par_LocalReductionVar))
        *(this->inputsTPParent->resa_par_darts76) = (resa_par_LocalReductionVar);
    this->inputsTPParent->resa_par_darts76_mutex.unlock();
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets76[0].decDep();
}
TP76::TP76(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet, int in_initIteration,
    int in_lastIteration, double** in_a, double* in_resa_par)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , a_darts76(in_a) /*OMP_SHARED - INPUT*/
    , i_darts76(new int[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , resa_par_darts76(in_resa_par) /*OMP_SHARED - INPUT*/
    , initIteration76(in_initIteration)
    , lastIteration76(in_lastIteration)
    , barrierCodelets76(new _barrierCodelets76[1])
    , checkInCodelets77(new _checkInCodelets77[this->numThreads])
{
    /*Initialize the loop parameters*/
    range76 = abs(lastIteration76 - initIteration76) / 1;
    rangePerCodelet76 = range76 / numThreads;
    minIteration76 = min<int>(lastIteration76, initIteration76);
    remainderRange76 = range76 % numThreads;
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets76[0] = _barrierCodelets76(ompNumThreads, ompNumThreads, this, 0);
    _checkInCodelets77* checkInCodelets77Ptr = (this->checkInCodelets77);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets77Ptr) = _checkInCodelets77(1, 1, this, codeletCounter);
        (*checkInCodelets77Ptr).decDep();
        checkInCodelets77Ptr++;
    }
}
TP76::~TP76()
{
    delete[] barrierCodelets76;
    delete[] checkInCodelets77;
}
