#include "masterthread.output.darts.h"
using namespace darts;
using namespace std;
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
    int size, i;
    if (argc <= 1) {
        size = 31;
    } else {
        size = atoi(argv[1]);
    }
    if (size < 0) {
        printf("Size should be greater than 0\n");
        return 0;
    }
    int* inVector = (int*)malloc(size * sizeof(int));
    int* outVector = (int*)malloc(8 * sizeof(int));
    int threadID = -1;
    for (i = 0; i < size; i++) {
        inVector[i] = rand() % 100 - 50;
    }
    int sequentialSum = 0;
    for (i = 0; i < size; i++) {
        sequentialSum += inVector[i];
    }
    int parallelSum = 0;
    omp_set_num_threads(8);
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP56>(ompNumThreads * DARTS_CODELETS_MULT, 0,
            RuntimeFinalCodelet, (int*)&((i)), (int**)&((inVector)), (int**)&((outVector)),
            (int*)&((parallelSum)), (int*)&((size)), (int*)&((threadID))));
    }
    if (sequentialSum != parallelSum || threadID != 0) {
        printf("Error in master test\n");
    }
    for (i = 0; i < 8; i++) {
        if (outVector[i] != sequentialSum) {
            printf("Error: outVector[%d] =%d != %d\n", i, outVector[i], sequentialSum);
        }
    }
    free(inVector);
    free(outVector);
    return 0;
}
/*TP56: OMPParallelDirective*/
void TP56::_barrierCodelets56::fire(void)
{
    TP56* myTP = static_cast<TP56*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
void TP56::_checkInCodelets58::fire(void)
{
    if (this->getID() == 0) {
        /*Init the vars for this region*/
        /*Initialize the vars of the inlined region*/
        this->inputsTPParent->inVector_darts58
            = (this->inputsTPParent->inVector_darts56) /*OMP_SHARED - VAR INLINED*/;
        this->inputsTPParent->parallelSum_darts58
            = (this->inputsTPParent->parallelSum_darts56) /*OMP_SHARED - VAR INLINED*/;
        this->inputsTPParent->size_darts58
            = (this->inputsTPParent->size_darts56) /*OMP_SHARED - VAR INLINED*/;
        this->inputsTPParent->threadID_darts58
            = (this->inputsTPParent->threadID_darts56) /*OMP_SHARED - VAR INLINED*/;

        /*printing node 60: BinaryOperator*/
        (*(this->inputsTPParent->threadID_darts58)) = omp_get_thread_num();

        /*printing node 62: ForStmt*/
        {
            /*Loop's init*/
            (this->inputsTPParent->i_darts58) = 0;
            int i_darts_counter_temp58 = (this->inputsTPParent->i_darts58);
            for (; i_darts_counter_temp58 < (*(this->inputsTPParent->size_darts58));
                 i_darts_counter_temp58++) {
                (*(this->inputsTPParent->parallelSum_darts58))
                    += (*(this->inputsTPParent->inVector_darts58))[i_darts_counter_temp58];
            }
            (this->inputsTPParent->i_darts58) = i_darts_counter_temp58;
        }
        /*Signaling next codelet from last stmt in the codelet*/
        /*Signaling next codelet region: 58 nextRegion: 69 */
        myTP->controlTPParent->barrierCodelets69[0].decDep();
    } else {
        /*Signaling next codelet region: 58 nextRegion: 69 */
        myTP->barrierCodelets69[0].decDep();
    }
}
void TP56::_barrierCodelets69::fire(void)
{
    TP56* myTP = static_cast<TP56*>(myTP_);
    {
        for (size_t codeletsCounter = 0; codeletsCounter < myTP->numThreads; codeletsCounter++) {
            myTP->checkInCodelets70[codeletsCounter].decDep();
        }
    }
}
void TP56::_checkInCodelets70::fire(void)
{

    /*printing node 70: BinaryOperator*/
    (*(this->inputsTPParent->outVector_darts56))[omp_get_thread_num()]
        = (*(this->inputsTPParent->parallelSum_darts56));
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets56[0].decDep();
}
TP56::TP56(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet, int* in_i,
    int** in_inVector, int** in_outVector, int* in_parallelSum, int* in_size, int* in_threadID)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , i_darts56(in_i) /*OMP_SHARED - INPUT*/
    , inVector_darts56(in_inVector) /*OMP_SHARED - INPUT*/
    , outVector_darts56(in_outVector) /*OMP_SHARED - INPUT*/
    , parallelSum_darts56(in_parallelSum) /*OMP_SHARED - INPUT*/
    , size_darts56(in_size) /*OMP_SHARED - INPUT*/
    , threadID_darts56(in_threadID) /*OMP_SHARED - INPUT*/
    , TP58_alreadyLaunched(0)
    , barrierCodelets56(new _barrierCodelets56[1])
    , checkInCodelets58(new _checkInCodelets58[this->numThreads])
    , barrierCodelets69(new _barrierCodelets69[1])
    , checkInCodelets70(new _checkInCodelets70[this->numThreads])
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets56[0] = _barrierCodelets56(ompNumThreads, ompNumThreads, this, 0);
    barrierCodelets69[0] = _barrierCodelets69(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets70* checkInCodelets70Ptr = (this->checkInCodelets70);
    _checkInCodelets58* checkInCodelets58Ptr = (this->checkInCodelets58);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets70Ptr) = _checkInCodelets70(1, 1, this, codeletCounter);
        checkInCodelets70Ptr++;
        (*checkInCodelets58Ptr) = _checkInCodelets58(1, 1, this, codeletCounter);
        (*checkInCodelets58Ptr).decDep();
        checkInCodelets58Ptr++;
    }
}
TP56::~TP56()
{
    delete[] barrierCodelets56;
    delete[] checkInCodelets70;
    delete[] barrierCodelets69;
    delete[] checkInCodelets58;
}
