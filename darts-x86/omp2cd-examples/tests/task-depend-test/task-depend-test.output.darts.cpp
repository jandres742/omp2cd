#include "task-depend-test.output.darts.h"
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
    printf("Example 15.15: x should be 2\n");
    int x = 1;
    if (affinMaskRes) {
        myDARTSRuntime->run(
            launch<TP8>(ompNumThreads * DARTS_CODELETS_MULT, 0, RuntimeFinalCodelet, (int*)&((x))));
    }
    printf("Example 15.16: x should be 1\n");
    x = 1;
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP20>(
            ompNumThreads * DARTS_CODELETS_MULT, 0, RuntimeFinalCodelet, (int*)&((x))));
    }
    printf("Example 15.17: x should be 2\n");
    x = 0;
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP32>(
            ompNumThreads * DARTS_CODELETS_MULT, 0, RuntimeFinalCodelet, (int*)&((x))));
    }
    return 0;
}
/*TP8: OMPParallelDirective*/
void TP8::_barrierCodelets8::fire(void)
{
    TP8* myTP = static_cast<TP8*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
void TP8::_checkInCodelets10::fire(void)
{
    /*Select the thread executing OMPSingleDirective 10*/
    if (!__sync_val_compare_and_swap(&(myTP->TP10_alreadyLaunched), 0, 1)) {
        invoke<TP10>(myTP, 1, this->getID(), myTP, &(*(this->inputsTPParent->x_darts8)));
    } else {
        myTP->TPParent->barrierCodelets8[0].decDep();
    }
}
TP8::TP8(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet, int* in_x)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , x_darts8(in_x) /*OMP_SHARED - INPUT*/
    , TP10Ptr(nullptr)
    , TP10_alreadyLaunched(0)
    , task12Inputs(new _task12Inputs*[this->numThreads])
    , task15Inputs(new _task15Inputs*[this->numThreads])
    , barrierCodelets8(new _barrierCodelets8[1])
    , checkInCodelets10(new _checkInCodelets10[this->numThreads])
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets8[0] = _barrierCodelets8(ompNumThreads, ompNumThreads, this, 0);
    _checkInCodelets10* checkInCodelets10Ptr = (this->checkInCodelets10);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets10Ptr) = _checkInCodelets10(1, 1, this, codeletCounter);
        (*checkInCodelets10Ptr).decDep();
        checkInCodelets10Ptr++;
    }
}
TP8::~TP8()
{
    delete[] task12Inputs;
    delete[] task15Inputs;
    delete[] barrierCodelets8;
    delete[] checkInCodelets10;
}
/*TP10: OMPSingleDirective*/
void TP10::_checkInCodelets12::fire(void)
{

    /*printing node 12: OMPTaskDirective*/
    /*syncNode: OMPParallelDirective 8*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->TPParent->barrierCodelets8[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 12*/
    _task12Inputs* task12Inputs
        = new _task12Inputs(&(*(this->inputsTPParent->x_darts10)), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task12Inputs[0] = task12Inputs;
    invoke<TP12>(myTP, 1, this->getID(), myTP, task12Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 12 nextRegion: 15 */
    myTP->controlTPParent->checkInCodelets15.decDep();
}
void TP10::_checkInCodelets15::fire(void)
{

    /*printing node 15: OMPTaskDirective*/
    /*syncNode: OMPParallelDirective 8*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->TPParent->barrierCodelets8[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 15*/
    _task15Inputs* task15Inputs = new _task15Inputs(&(*(this->inputsTPParent->x_darts10)),
        &(this->inputsTPParent->task12Inputs[0]->taskCompleted), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task15Inputs[0] = task15Inputs;
    invoke<TP15>(myTP, 1, this->getID(), myTP, task15Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->TPParent->barrierCodelets8[0].decDep();
}
TP10::TP10(int in_numThreads, int in_mainCodeletID, TP8* in_TPParent, int* in_x)
    : ompOMPSingleDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , x_darts10(in_x) /*OMP_SHARED - INPUT*/
    , task12Inputs(new _task12Inputs*[this->numThreads])
    , task15Inputs(new _task15Inputs*[this->numThreads])
    , checkInCodelets12(1, 1, this, this->mainCodeletID)
    , checkInCodelets15(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets12.decDep();
}
TP10::~TP10()
{
    delete[] task12Inputs;
    delete[] task15Inputs;
}
/*TP12: OMPTaskDirective*/
void TP12::_checkInCodelets14::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 14: BinaryOperator*/
    (*(this->taskInputs->x_darts12)) = 2;
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task12Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task12Inputs->nextSyncCodelet->decDep();
}
TP12::TP12(
    int in_numThreads, int in_mainCodeletID, TP10* in_TPParent, _task12Inputs* in_task12Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task12Inputs(in_task12Inputs)
    , checkInCodelets14(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets14.decDep();
}
TP12::~TP12() { delete (task12Inputs); }
/*TP15: OMPTaskDirective*/
void TP15::_checkInCodelets17::fire(void)
{
    /*Wait until the tasks this task depends on have completed*/
    if (*(myTP->task15Inputs->task12Completed) == false) {
        myTP->add(this);
        return;
    }
    /*Init the vars for this region*/

    /*printing node 17: CallExpr*/
    printf("x = %d\n", (*(this->taskInputs->x_darts15)));
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task15Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task15Inputs->nextSyncCodelet->decDep();
}
TP15::TP15(
    int in_numThreads, int in_mainCodeletID, TP10* in_TPParent, _task15Inputs* in_task15Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task15Inputs(in_task15Inputs)
    , checkInCodelets17(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets17.decDep();
}
TP15::~TP15() { delete (task15Inputs); }
/*TP20: OMPParallelDirective*/
void TP20::_barrierCodelets20::fire(void)
{
    TP20* myTP = static_cast<TP20*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
void TP20::_checkInCodelets22::fire(void)
{
    /*Select the thread executing OMPSingleDirective 22*/
    if (!__sync_val_compare_and_swap(&(myTP->TP22_alreadyLaunched), 0, 1)) {
        invoke<TP22>(myTP, 1, this->getID(), myTP, &(*(this->inputsTPParent->x_darts20)));
    } else {
        myTP->barrierCodelets22[0].decDep();
    }
}
void TP20::_barrierCodelets22::fire(void)
{
    TP20* myTP = static_cast<TP20*>(myTP_);
    myTP->TPParent->barrierCodelets20[0].setDep(0);
    myTP->add(&(myTP->TPParent->barrierCodelets20[0]));
}
TP20::TP20(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet, int* in_x)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , x_darts20(in_x) /*OMP_SHARED - INPUT*/
    , TP22Ptr(nullptr)
    , TP22_alreadyLaunched(0)
    , task24Inputs(new _task24Inputs*[this->numThreads])
    , task27Inputs(new _task27Inputs*[this->numThreads])
    , barrierCodelets20(new _barrierCodelets20[1])
    , checkInCodelets22(new _checkInCodelets22[this->numThreads])
    , barrierCodelets22(new _barrierCodelets22[1])
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets20[0] = _barrierCodelets20(ompNumThreads, ompNumThreads, this, 0);
    barrierCodelets22[0] = _barrierCodelets22(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets22* checkInCodelets22Ptr = (this->checkInCodelets22);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets22Ptr) = _checkInCodelets22(1, 1, this, codeletCounter);
        (*checkInCodelets22Ptr).decDep();
        checkInCodelets22Ptr++;
    }
}
TP20::~TP20()
{
    delete[] task24Inputs;
    delete[] task27Inputs;
    delete[] barrierCodelets20;
    delete[] barrierCodelets22;
    delete[] checkInCodelets22;
}
/*TP22: OMPSingleDirective*/
void TP22::_checkInCodelets24::fire(void)
{

    /*printing node 24: OMPTaskDirective*/
    /*syncNode: OMPSingleDirective 22*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->TPParent->barrierCodelets22[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 24*/
    _task24Inputs* task24Inputs
        = new _task24Inputs(&(*(this->inputsTPParent->x_darts22)), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task24Inputs[0] = task24Inputs;
    invoke<TP24>(myTP, 1, this->getID(), myTP, task24Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 24 nextRegion: 27 */
    myTP->controlTPParent->checkInCodelets27.decDep();
}
void TP22::_checkInCodelets27::fire(void)
{

    /*printing node 27: OMPTaskDirective*/
    /*syncNode: OMPSingleDirective 22*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->TPParent->barrierCodelets22[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 27*/
    _task27Inputs* task27Inputs = new _task27Inputs(&(*(this->inputsTPParent->x_darts22)),
        &(this->inputsTPParent->task24Inputs[0]->taskCompleted), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task27Inputs[0] = task27Inputs;
    invoke<TP27>(myTP, 1, this->getID(), myTP, task27Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets22[0].decDep();
}
TP22::TP22(int in_numThreads, int in_mainCodeletID, TP20* in_TPParent, int* in_x)
    : ompOMPSingleDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , x_darts22(in_x) /*OMP_SHARED - INPUT*/
    , task24Inputs(new _task24Inputs*[this->numThreads])
    , task27Inputs(new _task27Inputs*[this->numThreads])
    , checkInCodelets24(1, 1, this, this->mainCodeletID)
    , checkInCodelets27(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets24.decDep();
}
TP22::~TP22()
{
    delete[] task24Inputs;
    delete[] task27Inputs;
}
/*TP24: OMPTaskDirective*/
void TP24::_checkInCodelets26::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 26: CallExpr*/
    printf("x = %d\n", (*(this->taskInputs->x_darts24)));
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task24Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task24Inputs->nextSyncCodelet->decDep();
}
TP24::TP24(
    int in_numThreads, int in_mainCodeletID, TP22* in_TPParent, _task24Inputs* in_task24Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task24Inputs(in_task24Inputs)
    , checkInCodelets26(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets26.decDep();
}
TP24::~TP24() { delete (task24Inputs); }
/*TP27: OMPTaskDirective*/
void TP27::_checkInCodelets29::fire(void)
{
    /*Wait until the tasks this task depends on have completed*/
    if (*(myTP->task27Inputs->task24Completed) == false) {
        myTP->add(this);
        return;
    }
    /*Init the vars for this region*/

    /*printing node 29: BinaryOperator*/
    (*(this->taskInputs->x_darts27)) = 2;
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task27Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task27Inputs->nextSyncCodelet->decDep();
}
TP27::TP27(
    int in_numThreads, int in_mainCodeletID, TP22* in_TPParent, _task27Inputs* in_task27Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task27Inputs(in_task27Inputs)
    , checkInCodelets29(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets29.decDep();
}
TP27::~TP27() { delete (task27Inputs); }
/*TP32: OMPParallelDirective*/
void TP32::_barrierCodelets32::fire(void)
{
    TP32* myTP = static_cast<TP32*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
void TP32::_checkInCodelets34::fire(void)
{
    /*Select the thread executing OMPSingleDirective 34*/
    if (!__sync_val_compare_and_swap(&(myTP->TP34_alreadyLaunched), 0, 1)) {
        invoke<TP34>(myTP, 1, this->getID(), myTP, &(*(this->inputsTPParent->x_darts32)));
    } else {
        myTP->barrierCodelets34[0].decDep();
    }
}
void TP32::_barrierCodelets34::fire(void)
{
    TP32* myTP = static_cast<TP32*>(myTP_);
    myTP->TPParent->barrierCodelets32[0].setDep(0);
    myTP->add(&(myTP->TPParent->barrierCodelets32[0]));
}
TP32::TP32(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet, int* in_x)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , x_darts32(in_x) /*OMP_SHARED - INPUT*/
    , TP34Ptr(nullptr)
    , TP34_alreadyLaunched(0)
    , task36Inputs(new _task36Inputs*[this->numThreads])
    , task39Inputs(new _task39Inputs*[this->numThreads])
    , barrierCodelets32(new _barrierCodelets32[1])
    , checkInCodelets34(new _checkInCodelets34[this->numThreads])
    , barrierCodelets34(new _barrierCodelets34[1])
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets32[0] = _barrierCodelets32(ompNumThreads, ompNumThreads, this, 0);
    barrierCodelets34[0] = _barrierCodelets34(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets34* checkInCodelets34Ptr = (this->checkInCodelets34);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets34Ptr) = _checkInCodelets34(1, 1, this, codeletCounter);
        (*checkInCodelets34Ptr).decDep();
        checkInCodelets34Ptr++;
    }
}
TP32::~TP32()
{
    delete[] task36Inputs;
    delete[] task39Inputs;
    delete[] barrierCodelets32;
    delete[] barrierCodelets34;
    delete[] checkInCodelets34;
}
/*TP34: OMPSingleDirective*/
void TP34::_checkInCodelets36::fire(void)
{

    /*printing node 36: OMPTaskDirective*/
    /*syncNode: OMPTaskwaitDirective 42*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->barrierCodelets42[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 36*/
    _task36Inputs* task36Inputs
        = new _task36Inputs(&(*(this->inputsTPParent->x_darts34)), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task36Inputs[0] = task36Inputs;
    invoke<TP36>(myTP, 1, this->getID(), myTP, task36Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 36 nextRegion: 39 */
    myTP->controlTPParent->checkInCodelets39.decDep();
}
void TP34::_checkInCodelets39::fire(void)
{

    /*printing node 39: OMPTaskDirective*/
    /*syncNode: OMPTaskwaitDirective 42*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->barrierCodelets42[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 39*/
    _task39Inputs* task39Inputs = new _task39Inputs(&(*(this->inputsTPParent->x_darts34)),
        &(this->inputsTPParent->task36Inputs[0]->taskCompleted), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task39Inputs[0] = task39Inputs;
    invoke<TP39>(myTP, 1, this->getID(), myTP, task39Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 39 nextRegion: 42 */
    myTP->controlTPParent->barrierCodelets42[0].decDep();
}
void TP34::_barrierCodelets42::fire(void)
{
    TP34* myTP = static_cast<TP34*>(myTP_);
    {
        myTP->checkInCodelets43.decDep();
    }
}
void TP34::_checkInCodelets43::fire(void)
{

    /*printing node 43: CallExpr*/
    printf("x = %d\n", (*(this->inputsTPParent->x_darts34)));
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets34[0].decDep();
}
TP34::TP34(int in_numThreads, int in_mainCodeletID, TP32* in_TPParent, int* in_x)
    : ompOMPSingleDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , x_darts34(in_x) /*OMP_SHARED - INPUT*/
    , task36Inputs(new _task36Inputs*[this->numThreads])
    , task39Inputs(new _task39Inputs*[this->numThreads])
    , checkInCodelets36(1, 1, this, this->mainCodeletID)
    , checkInCodelets39(1, 1, this, this->mainCodeletID)
    , barrierCodelets42(new _barrierCodelets42[1])
    , checkInCodelets43(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets42[0] = _barrierCodelets42(this->numThreads, this->numThreads, this, 0);
    checkInCodelets36.decDep();
}
TP34::~TP34()
{
    delete[] task36Inputs;
    delete[] task39Inputs;
    delete[] barrierCodelets42;
}
/*TP36: OMPTaskDirective*/
void TP36::_checkInCodelets38::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 38: BinaryOperator*/
    (*(this->taskInputs->x_darts36)) = 1;
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task36Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task36Inputs->nextSyncCodelet->decDep();
}
TP36::TP36(
    int in_numThreads, int in_mainCodeletID, TP34* in_TPParent, _task36Inputs* in_task36Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task36Inputs(in_task36Inputs)
    , checkInCodelets38(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets38.decDep();
}
TP36::~TP36() { delete (task36Inputs); }
/*TP39: OMPTaskDirective*/
void TP39::_checkInCodelets41::fire(void)
{
    /*Wait until the tasks this task depends on have completed*/
    if (*(myTP->task39Inputs->task36Completed) == false) {
        myTP->add(this);
        return;
    }
    /*Init the vars for this region*/

    /*printing node 41: BinaryOperator*/
    (*(this->taskInputs->x_darts39)) = 2;
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task39Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task39Inputs->nextSyncCodelet->decDep();
}
TP39::TP39(
    int in_numThreads, int in_mainCodeletID, TP34* in_TPParent, _task39Inputs* in_task39Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task39Inputs(in_task39Inputs)
    , checkInCodelets41(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets41.decDep();
}
TP39::~TP39() { delete (task39Inputs); }
