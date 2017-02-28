#include "ompcodeletclause.output.darts.h"
using namespace darts;
using namespace std;
int NUM_THREADS;
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
    NUM_THREADS = omp_get_max_threads();
    int* inVector = (int*)malloc(size * sizeof(int));
    int* outVector = (int*)malloc(NUM_THREADS * sizeof(int));
    int threadID = -1;
    for (i = 0; i < size; i++) {
        inVector[i] = rand() % 100 - 50;
    }
    int sequentialSum = 0;
    for (i = 0; i < size; i++) {
        sequentialSum += inVector[i];
    }
    int parallelSum = 0;
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP57>(ompNumThreads * DARTS_CODELETS_MULT, 0,
            RuntimeFinalCodelet, (int*)&((i)), (int**)&((inVector)), (int**)&((outVector)),
            (int*)&((parallelSum)), (int*)&((size)), (int*)&((threadID))));
    }
    if (sequentialSum != parallelSum || threadID != 0) {
        printf("Error in master test\n");
    }
    for (i = 0; i < NUM_THREADS; i++) {
        if (outVector[i] != (sequentialSum + 10)) {
            printf("Error: outVector[%d] =%d != %d\n", i, outVector[i], sequentialSum + 10);
        }
    }
    free(inVector);
    free(outVector);
    return 0;
}
/*TP57: OMPParallelDirective*/
void TP57::_barrierCodelets57::fire(void)
{
    TP57* myTP = static_cast<TP57*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
void TP57::_checkInCodelets59::fire(void)
{
    if (this->getID() == 0) {
        invoke<TP59>(myTP, 1, this->getID(), myTP, &(*(this->inputsTPParent->inVector_darts57)),
            &(*(this->inputsTPParent->parallelSum_darts57)),
            &(*(this->inputsTPParent->size_darts57)), &(*(this->inputsTPParent->threadID_darts57)));
    } else {
        myTP->checkInCodelets70[this->getID()].decDep();
    }
}
void TP57::_checkInCodelets70::fire(void)
{

    /*printing node 70: DeclStmt*/
    this->inputsTPParent->initLoop_darts57[this->getID()] = 0;

    /*printing node 71: DeclStmt*/
    this->inputsTPParent->endLoop_darts57[this->getID()] = NUM_THREADS;
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 70 nextRegion: 72 */
    myTP->controlTPParent->checkInCodelets72[this->getID()].decDep();
}
void TP57::_checkInCodelets72::fire(void)
{
    /*printOMPForCodeletFireCode, inlined: 1*/
    /*region 72 1*/
    /*Init the vars for this region*/
    /*Initialize the vars of the inlined region*/
    this->inputsTPParent->endLoop_darts72[this->getLocalID()]
        = &(this->inputsTPParent
                ->endLoop_darts57[this->getLocalID()]) /*OMP_SHARED_PRIVATE - INPUT INLINED*/;
    this->inputsTPParent->i_darts72 = new int[myTP->numThreads] /*OMP_PRIVATE - INPUT INLINED*/;
    this->inputsTPParent->initLoop_darts72[this->getLocalID()]
        = &(this->inputsTPParent
                ->initLoop_darts57[this->getLocalID()]) /*OMP_SHARED_PRIVATE - INPUT INLINED*/;
    this->inputsTPParent->outVector_darts72
        = (this->inputsTPParent->outVector_darts57) /*OMP_SHARED - INPUT INLINED*/;

    /*printing node 73: ForStmt*/
    /*var: endLoop*/
    /*var: i*/
    /*var: initLoop*/
    /*var: outVector*/
    int* i = &(this->inputsTPParent->i_darts72[this->getLocalID()]);
    (void)i /*OMP_PRIVATE*/;
    int** outVector = (this->inputsTPParent->outVector_darts72);
    (void)outVector /*OMP_SHARED*/;
    for (int i_darts_counter_temp72 = (*(myTP->initLoop_darts72[this->getID()]));
         i_darts_counter_temp72 < (*(myTP->endLoop_darts72[this->getID()]));
         i_darts_counter_temp72++) {
        {
            (*(outVector))[(i_darts_counter_temp72)] = 10;
        }
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling omp region's barrier*/
    myTP->controlTPParent->barrierCodelets72[0].decDep();
}
void TP57::_barrierCodelets72::fire(void)
{
    TP57* myTP = static_cast<TP57*>(myTP_);
    {
        for (size_t codeletsCounter = 0; codeletsCounter < myTP->numThreads; codeletsCounter++) {
            myTP->checkInCodelets104[codeletsCounter].decDep();
        }
    }
}
void TP57::_checkInCodelets104::fire(void)
{
    /*Select the thread executing OMPSingleDirective 104*/
    if (!__sync_val_compare_and_swap(&(myTP->TP104_alreadyLaunched), 0, 1)) {
        /*Init the vars for this region*/
        /*Initialize the vars of the inlined region*/
        this->inputsTPParent->outVector_darts104
            = (this->inputsTPParent->outVector_darts57) /*OMP_SHARED - VAR INLINED*/;
        this->inputsTPParent->parallelSum_darts104
            = (this->inputsTPParent->parallelSum_darts57) /*OMP_SHARED - VAR INLINED*/;

        /*printing node 106: ForStmt*/
        {
            /*Loop's init*/
            (this->inputsTPParent->i_darts104) = 0;
            int i_darts_counter_temp104 = (this->inputsTPParent->i_darts104);
            for (; i_darts_counter_temp104 < NUM_THREADS; i_darts_counter_temp104++) {
                (*(this->inputsTPParent->outVector_darts104))[i_darts_counter_temp104]
                    += (*(this->inputsTPParent->parallelSum_darts104));
            }
            (this->inputsTPParent->i_darts104) = i_darts_counter_temp104;
        }
        /*Signaling next codelet from last stmt in the codelet*/
        /*Signaling omp region's barrier*/
        myTP->controlTPParent->barrierCodelets104[0].decDep();
    } else {
        /*Signaling omp region's barrier*/
        myTP->barrierCodelets104[0].decDep();
    }
}
void TP57::_barrierCodelets104::fire(void)
{
    TP57* myTP = static_cast<TP57*>(myTP_);
    myTP->TPParent->barrierCodelets57[0].setDep(0);
    myTP->add(&(myTP->TPParent->barrierCodelets57[0]));
}
TP57::TP57(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet, int* in_i,
    int** in_inVector, int** in_outVector, int* in_parallelSum, int* in_size, int* in_threadID)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , i_darts57(in_i) /*OMP_SHARED - INPUT*/
    , inVector_darts57(in_inVector) /*OMP_SHARED - INPUT*/
    , outVector_darts57(in_outVector) /*OMP_SHARED - INPUT*/
    , parallelSum_darts57(in_parallelSum) /*OMP_SHARED - INPUT*/
    , size_darts57(in_size) /*OMP_SHARED - INPUT*/
    , threadID_darts57(in_threadID) /*OMP_SHARED - INPUT*/
    , endLoop_darts57(new int[this->numThreads]) /*VARIABLE*/
    , initLoop_darts57(new int[this->numThreads]) /*VARIABLE*/
    , endLoop_darts72(new int*[this->numThreads])
    , i_darts72(new int[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , initLoop_darts72(new int*[this->numThreads])
    , TP59Ptr(nullptr)
    , TP59_alreadyLaunched(0)
    , TP72_alreadyLaunched(0)
    , TP104_alreadyLaunched(0)
    , barrierCodelets57(new _barrierCodelets57[1])
    , checkInCodelets59(new _checkInCodelets59[this->numThreads])
    , checkInCodelets70(new _checkInCodelets70[this->numThreads])
    , checkInCodelets72(new _checkInCodelets72[this->numThreads])
    , barrierCodelets72(new _barrierCodelets72[1])
    , checkInCodelets104(new _checkInCodelets104[this->numThreads])
    , barrierCodelets104(new _barrierCodelets104[1])
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets57[0] = _barrierCodelets57(ompNumThreads, ompNumThreads, this, 0);
    barrierCodelets104[0] = _barrierCodelets104(this->numThreads, this->numThreads, this, 0);
    barrierCodelets72[0] = _barrierCodelets72(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets104* checkInCodelets104Ptr = (this->checkInCodelets104);
    _checkInCodelets72* checkInCodelets72Ptr = (this->checkInCodelets72);
    _checkInCodelets70* checkInCodelets70Ptr = (this->checkInCodelets70);
    _checkInCodelets59* checkInCodelets59Ptr = (this->checkInCodelets59);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets104Ptr) = _checkInCodelets104(1, 1, this, codeletCounter);
        checkInCodelets104Ptr++;
        (*checkInCodelets72Ptr) = _checkInCodelets72(1, 1, this, codeletCounter);
        checkInCodelets72Ptr++;
        (*checkInCodelets70Ptr) = _checkInCodelets70(1, 1, this, codeletCounter);
        checkInCodelets70Ptr++;
        (*checkInCodelets59Ptr) = _checkInCodelets59(1, 1, this, codeletCounter);
        (*checkInCodelets59Ptr).decDep();
        checkInCodelets59Ptr++;
    }
}
TP57::~TP57()
{
    delete[] endLoop_darts57;
    delete[] initLoop_darts57;
    delete[] endLoop_darts72;
    delete[] initLoop_darts72;
    delete[] barrierCodelets57;
    delete[] barrierCodelets104;
    delete[] checkInCodelets104;
    delete[] barrierCodelets72;
    delete[] checkInCodelets72;
    delete[] checkInCodelets70;
    delete[] checkInCodelets59;
}
/*TP59: OMPMasterDirective*/
void TP59::_checkInCodelets61::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 61: BinaryOperator*/
    (*(this->inputsTPParent->threadID_darts59)) = omp_get_thread_num();

    /*printing node 63: ForStmt*/
    {
        int* i = &(this->inputsTPParent->i_darts59);
        (void)i /*OMP_PRIVATE*/;
        int** inVector = (this->inputsTPParent->inVector_darts59);
        (void)inVector /*OMP_SHARED*/;
        int* parallelSum = (this->inputsTPParent->parallelSum_darts59);
        (void)parallelSum /*OMP_SHARED*/;
        int* size = (this->inputsTPParent->size_darts59);
        (void)size /*OMP_SHARED*/;
        /*Loop's init*/
        (this->inputsTPParent->i_darts59) = 0;
        int i_darts_counter_temp59 = (this->inputsTPParent->i_darts59);
        for (; (i_darts_counter_temp59) < (*(size)); (i_darts_counter_temp59)++) {
            (*(parallelSum)) += (*(inVector))[(i_darts_counter_temp59)];
        }
        (this->inputsTPParent->i_darts59) = i_darts_counter_temp59;
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->checkInCodelets70[this->getID()].decDep();
}
TP59::TP59(int in_numThreads, int in_mainCodeletID, TP57* in_TPParent, int** in_inVector,
    int* in_parallelSum, int* in_size, int* in_threadID)
    : ompOMPMasterDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , inVector_darts59(in_inVector) /*OMP_SHARED - INPUT*/
    , parallelSum_darts59(in_parallelSum) /*OMP_SHARED - INPUT*/
    , size_darts59(in_size) /*OMP_SHARED - INPUT*/
    , threadID_darts59(in_threadID) /*OMP_SHARED - INPUT*/
    , checkInCodelets61(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets61.decDep();
}
TP59::~TP59() {}
