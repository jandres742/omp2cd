#include "fibtasks.output.darts.h"
using namespace darts;
using namespace std;
/*Function: seqFib, ID: 1*/
int seqFib(int n)
{
    /*seqFib:1*/
    /*CompoundStmt:9*/
    int z, x, y;
    if (n < 2) {
        z = n;
    } else {
        x = seqFib(n - 1);
        y = seqFib(n - 2);
        z = x + y;
    }
    return z;
}
/*Function: main, ID: 3*/
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
    /*main:3*/
    /*CompoundStmt:45*/
    int num = 0;
    if (argc <= 1) {
        num = 20;
    } else {
        num = atoi(argv[1]);
    }
    if (num <= 0) {
        printf("Num should be greater than 0\n");
        return 0;
    }
    int seq = seqFib(num);
    int parTaskWait = 0;
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP63>(ompNumThreads * DARTS_CODELETS_MULT, 0,
            RuntimeFinalCodelet, (int*)&((num)), (int*)&((parTaskWait))));
    }
    if (seq != parTaskWait) {
        printf("Error in fib test with taskwait: ");
        printf("seq = %d, ", seq);
        printf("parTaskWait = %d\n", parTaskWait);
    }
    return 0;
}
/*TP2: TP_parallelFibTaskWait*/
void TP2::_checkInCodelets27::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    /*Init the vars for this region*/
    /*Printing conditional branch node 27: inlining: 0*/
    if ((this->inputsTPParent->n_darts2[this->getID()]) < 2) {
        myTP->checkInCodelets30[this->getID()].decDep();
    } else {
        /*elseRegion: OMPTaskDirective 32*/
        myTP->checkInCodelets32[this->getID()].decDep();
    }
}
void TP2::_checkInCodelets30::fire(void)
{

    /*printing node 30: ReturnStmt*/
    /*return*/
    *(this->inputsTPParent->functionResult[this->getID()])
        = (this->inputsTPParent->n_darts2[this->getID()]);
    myTP->controlTPParent->nextCodeletsparallelFibTaskWait[this->getID()]->decDep();
    return;
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/

    myTP->controlTPParent->nextCodeletsparallelFibTaskWait[this->getID()]->decDep();
}
void TP2::_checkInCodelets32::fire(void)
{

    /*printing node 32: OMPTaskDirective*/
    /*syncNode: OMPTaskwaitDirective 42*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->barrierCodelets42[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 32*/
    _task32Inputs* task32Inputs
        = new _task32Inputs(&((this->inputsTPParent->n_darts2[this->getID()])),
            &((this->inputsTPParent->x_darts2[this->getID()])), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task32Inputs[this->getID()] = task32Inputs;
    invoke<TP32>(myTP, 1, this->getID(), myTP, task32Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 32 nextRegion: 37 */
    myTP->controlTPParent->checkInCodelets37[this->getID()].decDep();
}
void TP2::_checkInCodelets37::fire(void)
{

    /*printing node 37: OMPTaskDirective*/
    /*syncNode: OMPTaskwaitDirective 42*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->barrierCodelets42[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 37*/
    _task37Inputs* task37Inputs
        = new _task37Inputs(&((this->inputsTPParent->n_darts2[this->getID()])),
            &((this->inputsTPParent->y_darts2[this->getID()])), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task37Inputs[this->getID()] = task37Inputs;
    invoke<TP37>(myTP, 1, this->getID(), myTP, task37Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 37 nextRegion: 42 */
    myTP->controlTPParent->barrierCodelets42[0].decDep();
}
void TP2::_barrierCodelets42::fire(void)
{
    TP2* myTP = static_cast<TP2*>(myTP_);
    {
        for (size_t codeletsCounter = 0; codeletsCounter < myTP->numThreads; codeletsCounter++) {
            myTP->checkInCodelets43[codeletsCounter].decDep();
        }
    }
}
void TP2::_checkInCodelets43::fire(void)
{

    /*printing node 43: ReturnStmt*/
    /*return*/
    *(this->inputsTPParent->functionResult[this->getID()])
        = (this->inputsTPParent->x_darts2[this->getID()])
        + (this->inputsTPParent->y_darts2[this->getID()]);
    myTP->controlTPParent->nextCodeletsparallelFibTaskWait[this->getID()]->decDep();
    return;
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/

    myTP->controlTPParent->nextCodeletsparallelFibTaskWait[this->getID()]->decDep();
}
TP2::TP2(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_mainNextCodelet,
    darts::Codelet* in_mainSyncCodelet, TP2** in_ptrToThisFunctionTP, int in_n,
    int* in_functionResult)
    : ompTP(in_numThreads, in_mainCodeletID)
    , ptrToThisFunctionTP(in_ptrToThisFunctionTP)
    , inputsTPParent(this)
    , controlTPParent(this)
    , nextCodeletsparallelFibTaskWait(new Codelet*[in_numThreads])
    , nextSyncCodeletsparallelFibTaskWait(new Codelet*[in_numThreads])
    , n_darts2(new int[this->numThreads])
    , x_darts2(new int[this->numThreads])
    , y_darts2(new int[this->numThreads])
    , functionResult(new int*[this->numThreads])
    , task32Inputs(new _task32Inputs*[this->numThreads])
    , task37Inputs(new _task37Inputs*[this->numThreads])
    , checkInCodelets27(new _checkInCodelets27[this->numThreads])
    , checkInCodelets30(new _checkInCodelets30[this->numThreads])
    , checkInCodelets32(new _checkInCodelets32[this->numThreads])
    , checkInCodelets37(new _checkInCodelets37[this->numThreads])
    , barrierCodelets42(new _barrierCodelets42[1])
    , checkInCodelets43(new _checkInCodelets43[this->numThreads])
{
    barrierCodelets42[0] = _barrierCodelets42(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets43* checkInCodelets43Ptr = (this->checkInCodelets43);
    _checkInCodelets37* checkInCodelets37Ptr = (this->checkInCodelets37);
    _checkInCodelets32* checkInCodelets32Ptr = (this->checkInCodelets32);
    _checkInCodelets30* checkInCodelets30Ptr = (this->checkInCodelets30);
    _checkInCodelets27* checkInCodelets27Ptr = (this->checkInCodelets27);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets27);
#endif
    for (size_t codeletCounter = 0; codeletCounter < this->numThreads; codeletCounter++) {
        (*checkInCodelets43Ptr) = _checkInCodelets43(1, 1, this, codeletCounter);
        checkInCodelets43Ptr++;
        (*checkInCodelets37Ptr) = _checkInCodelets37(1, 1, this, codeletCounter);
        checkInCodelets37Ptr++;
        (*checkInCodelets32Ptr) = _checkInCodelets32(1, 1, this, codeletCounter);
        checkInCodelets32Ptr++;
        (*checkInCodelets30Ptr) = _checkInCodelets30(1, 1, this, codeletCounter);
        checkInCodelets30Ptr++;
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets27Ptr) = _checkInCodelets27(2, 1, this, codeletCounter);
#else
        (*checkInCodelets27Ptr) = _checkInCodelets27(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets27Ptr).decDep();
        checkInCodelets27Ptr++;
    }
    if (this->numThreads == 1) {
        this->nextCodeletsparallelFibTaskWait[0] = in_mainNextCodelet;
        this->nextSyncCodeletsparallelFibTaskWait[0] = in_mainSyncCodelet;
        this->n_darts2[0] = in_n;
        this->functionResult[0] = in_functionResult;
        this->availableCodelets[0] = 1;
    } else {
        this->n_darts2[this->mainCodeletID] = in_n;
        this->functionResult[this->mainCodeletID] = in_functionResult;
        this->nextCodeletsparallelFibTaskWait[in_mainCodeletID] = in_mainNextCodelet;
        this->nextSyncCodeletsparallelFibTaskWait[in_mainCodeletID] = in_mainSyncCodelet;
#if USE_SPIN_CODELETS == 0
        this->firstCodelet[this->mainCodeletID].decDep();
#else
        this->availableCodelets[this->mainCodeletID] = 1;
#endif
        *(this->ptrToThisFunctionTP) = this;
    }
}
TP2::~TP2()
{
    delete[] task32Inputs;
    delete[] task37Inputs;
    delete[] checkInCodelets43;
    delete[] barrierCodelets42;
    delete[] checkInCodelets37;
    delete[] checkInCodelets32;
    delete[] checkInCodelets30;
    delete[] checkInCodelets27;
    delete[] nextCodeletsparallelFibTaskWait;
    delete[] nextSyncCodeletsparallelFibTaskWait;
    delete[] n_darts2;
    delete[] x_darts2;
    delete[] y_darts2;
    delete[] functionResult;
}
void TP2::setNewInputs(int in_n, int* in_functionResult, size_t codeletID)
{
    this->n_darts2[codeletID] = in_n;
    this->functionResult[codeletID] = in_functionResult;
}
/*TP32: OMPTaskDirective*/
void TP32::_checkInCodelets35::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 35: CallExpr*/
    /*Signaling next codelet region: 35 nextRegion: 76 */
    myTP->controlTPParent->checkInCodelets76.decDep();
    /*No syncpoint found or outside task's boundaries, looking for syncCodelet*/
    /*Make the function call*/
    invoke<TP_parallelFibTaskWait>(myTP, myTP->numThreads, this->getID(),
        &(myTP->controlTPParent->checkInCodelets76),
        (myTP->controlTPParent->task32Inputs->nextSyncCodelet), nullptr,
        (this->taskInputs->n_darts32) - 1, &(*(this->taskInputs->x_darts32)));
}
void TP32::_checkInCodelets76::fire(void)
{

    /*printing node 76: NullStmt*/
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task32Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task32Inputs->nextSyncCodelet->decDep();
}
TP32::TP32(
    int in_numThreads, int in_mainCodeletID, TP2* in_TPParent, _task32Inputs* in_task32Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task32Inputs(in_task32Inputs)
    , TP35Ptr(nullptr)
    , TP35_alreadyLaunched(0)
    , checkInCodelets35(1, 1, this, this->mainCodeletID)
    , checkInCodelets76(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets35.decDep();
}
TP32::~TP32() { delete (task32Inputs); }
/*TP37: OMPTaskDirective*/
void TP37::_checkInCodelets40::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 40: CallExpr*/
    /*Signaling next codelet region: 40 nextRegion: 77 */
    myTP->controlTPParent->checkInCodelets77.decDep();
    /*No syncpoint found or outside task's boundaries, looking for syncCodelet*/
    /*Make the function call*/
    invoke<TP_parallelFibTaskWait>(myTP, myTP->numThreads, this->getID(),
        &(myTP->controlTPParent->checkInCodelets77),
        (myTP->controlTPParent->task37Inputs->nextSyncCodelet), nullptr,
        (this->taskInputs->n_darts37) - 2, &(*(this->taskInputs->y_darts37)));
}
void TP37::_checkInCodelets77::fire(void)
{

    /*printing node 77: NullStmt*/
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task37Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task37Inputs->nextSyncCodelet->decDep();
}
TP37::TP37(
    int in_numThreads, int in_mainCodeletID, TP2* in_TPParent, _task37Inputs* in_task37Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task37Inputs(in_task37Inputs)
    , TP40Ptr(nullptr)
    , TP40_alreadyLaunched(0)
    , checkInCodelets40(1, 1, this, this->mainCodeletID)
    , checkInCodelets77(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets40.decDep();
}
TP37::~TP37() { delete (task37Inputs); }
/*TP63: OMPParallelDirective*/
void TP63::_barrierCodelets63::fire(void)
{
    TP63* myTP = static_cast<TP63*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
void TP63::_checkInCodelets65::fire(void)
{
    /*Select the thread executing OMPSingleDirective 65*/
    if (!__sync_val_compare_and_swap(&(myTP->TP65_alreadyLaunched), 0, 1)) {
        invoke<TP65>(myTP, 1, this->getID(), myTP, &(*(this->inputsTPParent->num_darts63)),
            &(*(this->inputsTPParent->parTaskWait_darts63)));
    } else {
        myTP->barrierCodelets65[0].decDep();
    }
}
void TP63::_barrierCodelets65::fire(void)
{
    TP63* myTP = static_cast<TP63*>(myTP_);
    myTP->TPParent->barrierCodelets63[0].setDep(0);
    myTP->add(&(myTP->TPParent->barrierCodelets63[0]));
}
TP63::TP63(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet, int* in_num,
    int* in_parTaskWait)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , num_darts63(in_num) /*OMP_SHARED - INPUT*/
    , parTaskWait_darts63(in_parTaskWait) /*OMP_SHARED - INPUT*/
    , TP65Ptr(nullptr)
    , TP65_alreadyLaunched(0)
    , barrierCodelets63(new _barrierCodelets63[1])
    , checkInCodelets65(new _checkInCodelets65[this->numThreads])
    , barrierCodelets65(new _barrierCodelets65[1])
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets63[0] = _barrierCodelets63(ompNumThreads, ompNumThreads, this, 0);
    barrierCodelets65[0] = _barrierCodelets65(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets65* checkInCodelets65Ptr = (this->checkInCodelets65);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets65Ptr) = _checkInCodelets65(1, 1, this, codeletCounter);
        (*checkInCodelets65Ptr).decDep();
        checkInCodelets65Ptr++;
    }
}
TP63::~TP63()
{
    delete[] barrierCodelets63;
    delete[] barrierCodelets65;
    delete[] checkInCodelets65;
}
/*TP65: OMPSingleDirective*/
void TP65::_checkInCodelets68::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 68: CallExpr*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets65[0].decDep();
    /*Make the function call*/
    invoke<TP_parallelFibTaskWait>(myTP, myTP->numThreads, this->getID(),
        &(myTP->controlTPParent->TPParent->barrierCodelets65[0]),
        &(myTP->controlTPParent->TPParent->barrierCodelets65[0]), nullptr,
        (*(this->inputsTPParent->num_darts65)), &(*(this->inputsTPParent->parTaskWait_darts65)));
}
TP65::TP65(
    int in_numThreads, int in_mainCodeletID, TP63* in_TPParent, int* in_num, int* in_parTaskWait)
    : ompOMPSingleDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , num_darts65(in_num) /*OMP_SHARED - INPUT*/
    , parTaskWait_darts65(in_parTaskWait) /*OMP_SHARED - INPUT*/
    , TP68Ptr(nullptr)
    , TP68_alreadyLaunched(0)
    , checkInCodelets68(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets68.decDep();
}
TP65::~TP65() {}
