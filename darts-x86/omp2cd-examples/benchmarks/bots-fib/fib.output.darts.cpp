#include "fib.output.darts.h"
using namespace darts;
using namespace std;
long long fib_results[41]
    = { 0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610, 987, 1597, 2584, 4181, 6765,
        10946, 17711, 28657, 46368, 75025, 121393, 196418, 317811, 514229, 832040, 1346269, 2178309,
        3524578, 5702887, 9227465, 14930352, 24157817, 39088169, 63245986, 102334155 };
static long long par_res;
static long long seq_res;
/*Function: fib_seq, ID: 15*/
long long fib_seq(int n)
{
    /*fib_seq:15*/
    /*CompoundStmt:44*/
    int x, y;
    if (n < 2)
        return n;
    x = fib_seq(n - 1);
    y = fib_seq(n - 2);
    return x + y;
}
/*Function: fib0, ID: 17*/
void fib0(int n)
{
    /*fib0:17*/
    /*CompoundStmt:75*/
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP76>(
            ompNumThreads * DARTS_CODELETS_MULT, 0, RuntimeFinalCodelet, (int*)&((n))));
    }
    {
        /*CompoundStmt:80*/
        if (bots_verbose_mode >= BOTS_VERBOSE_DEFAULT) {
            fprintf(stdout, "Fibonacci result for %d is %lld\n", n, par_res);
        }
    }
}
/*Function: fib0_seq, ID: 18*/
void fib0_seq(int n)
{
    /*fib0_seq:18*/
    /*CompoundStmt:85*/
    seq_res = fib_seq(n);
    {
        /*CompoundStmt:88*/
        if (bots_verbose_mode >= BOTS_VERBOSE_DEFAULT) {
            fprintf(stdout, "Fibonacci result for %d is %lld\n", n, seq_res);
        }
    }
}
/*Function: fib_verify_value, ID: 19*/
long long fib_verify_value(int n)
{
    /*fib_verify_value:19*/
    /*CompoundStmt:93*/
    if (n < 41)
        return fib_results[n];
    return (fib_verify_value(n - 1) + fib_verify_value(n - 2));
}
/*Function: fib_verify, ID: 20*/
int fib_verify(int n)
{
    /*fib_verify:20*/
    /*CompoundStmt:105*/
    int result;
    if (bots_sequential_flag) {
        if (par_res == seq_res)
            result = 1;
        else
            result = 2;
    } else {
        seq_res = fib_verify_value(n);
        if (par_res == seq_res)
            result = 1;
        else
            result = 2;
    }
    return result;
}
/*TP16: TP_fib*/
void TP16::_checkInCodelets59::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    /*Init the vars for this region*/

    /*printing node 59: DeclStmt*/

    /*printing node 60: IfStmt*/
    if ((this->inputsTPParent->n_darts16[this->getID()]) < 2) {
        /*return*/
        *(this->inputsTPParent->functionResult[this->getID()])
            = (this->inputsTPParent->n_darts16[this->getID()]);
        myTP->controlTPParent->nextCodeletsfib[this->getID()]->decDep();
        return;
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 59 nextRegion: 64 */
    myTP->controlTPParent->checkInCodelets64[this->getID()].decDep();
}
void TP16::_checkInCodelets64::fire(void)
{

    /*printing node 64: OMPTaskDirective*/
    /*syncNode: OMPTaskwaitDirective 72*/
    /*Find the TP to which the nextRegion belongs*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->barrierCodelets72[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 64*/
    _task64Inputs* task64Inputs
        = new _task64Inputs(&((this->inputsTPParent->n_darts16[this->getID()])),
            &((this->inputsTPParent->x_darts16[this->getID()])), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task64Inputs[this->getID()] = task64Inputs;
    invoke<TP64>(myTP, 1, this->getID(), myTP, task64Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 64 nextRegion: 68 */
    myTP->controlTPParent->checkInCodelets68[this->getID()].decDep();
}
void TP16::_checkInCodelets68::fire(void)
{

    /*printing node 68: OMPTaskDirective*/
    /*syncNode: OMPTaskwaitDirective 72*/
    /*Find the TP to which the nextRegion belongs*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->barrierCodelets72[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 68*/
    _task68Inputs* task68Inputs
        = new _task68Inputs(&((this->inputsTPParent->n_darts16[this->getID()])),
            &((this->inputsTPParent->y_darts16[this->getID()])), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task68Inputs[this->getID()] = task68Inputs;
    invoke<TP68>(myTP, 1, this->getID(), myTP, task68Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 68 nextRegion: 72 */
    myTP->controlTPParent->barrierCodelets72[0].decDep();
}
void TP16::_barrierCodelets72::fire(void)
{
    TP16* myTP = static_cast<TP16*>(myTP_);
    {
        for (size_t codeletsCounter = 0; codeletsCounter < myTP->numThreads; codeletsCounter++) {
            myTP->checkInCodelets73[codeletsCounter].decDep();
        }
    }
}
void TP16::_checkInCodelets73::fire(void)
{

    /*printing node 73: ReturnStmt*/
    /*return*/
    *(this->inputsTPParent->functionResult[this->getID()])
        = (this->inputsTPParent->x_darts16[this->getID()])
        + (this->inputsTPParent->y_darts16[this->getID()]);
    myTP->controlTPParent->nextCodeletsfib[this->getID()]->decDep();
    return;
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/

    myTP->controlTPParent->nextCodeletsfib[this->getID()]->decDep();
}
TP16::TP16(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_mainNextCodelet,
    darts::Codelet* in_mainSyncCodelet, TP16** in_ptrToThisFunctionTP, int in_n,
    long long* in_functionResult)
    : ompTP(in_numThreads, in_mainCodeletID)
    , ptrToThisFunctionTP(in_ptrToThisFunctionTP)
    , inputsTPParent(this)
    , controlTPParent(this)
    , nextCodeletsfib(new Codelet*[in_numThreads])
    , nextSyncCodeletsfib(new Codelet*[in_numThreads])
    , n_darts16(new int[this->numThreads])
    , x_darts16(new long long[this->numThreads])
    , y_darts16(new long long[this->numThreads])
    , functionResult(new long long*[this->numThreads])
    , task64Inputs(new _task64Inputs*[this->numThreads])
    , task68Inputs(new _task68Inputs*[this->numThreads])
    , checkInCodelets59(new _checkInCodelets59[this->numThreads])
    , checkInCodelets64(new _checkInCodelets64[this->numThreads])
    , checkInCodelets68(new _checkInCodelets68[this->numThreads])
    , barrierCodelets72(new _barrierCodelets72[1])
    , checkInCodelets73(new _checkInCodelets73[this->numThreads])
{
    barrierCodelets72[0] = _barrierCodelets72(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets73* checkInCodelets73Ptr = (this->checkInCodelets73);
    _checkInCodelets68* checkInCodelets68Ptr = (this->checkInCodelets68);
    _checkInCodelets64* checkInCodelets64Ptr = (this->checkInCodelets64);
    _checkInCodelets59* checkInCodelets59Ptr = (this->checkInCodelets59);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets59);
#endif
    for (size_t codeletCounter = 0; codeletCounter < this->numThreads; codeletCounter++) {
        (*checkInCodelets73Ptr) = _checkInCodelets73(1, 1, this, codeletCounter);
        checkInCodelets73Ptr++;
        (*checkInCodelets68Ptr) = _checkInCodelets68(1, 1, this, codeletCounter);
        checkInCodelets68Ptr++;
        (*checkInCodelets64Ptr) = _checkInCodelets64(1, 1, this, codeletCounter);
        checkInCodelets64Ptr++;
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets59Ptr) = _checkInCodelets59(2, 1, this, codeletCounter);
#else
        (*checkInCodelets59Ptr) = _checkInCodelets59(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets59Ptr).decDep();
        checkInCodelets59Ptr++;
    }
    if (this->numThreads == 1) {
        this->nextCodeletsfib[0] = in_mainNextCodelet;
        this->nextSyncCodeletsfib[0] = in_mainSyncCodelet;
        this->n_darts16[0] = in_n;
        this->functionResult[0] = in_functionResult;
        this->availableCodelets[0] = 1;
    } else {
        this->n_darts16[this->mainCodeletID] = in_n;
        this->functionResult[this->mainCodeletID] = in_functionResult;
        this->nextCodeletsfib[in_mainCodeletID] = in_mainNextCodelet;
        this->nextSyncCodeletsfib[in_mainCodeletID] = in_mainSyncCodelet;
#if USE_SPIN_CODELETS == 0
        this->firstCodelet[this->mainCodeletID].decDep();
#else
        this->availableCodelets[this->mainCodeletID] = 1;
#endif
        *(this->ptrToThisFunctionTP) = this;
    }
}
TP16::~TP16()
{
    delete[] task64Inputs;
    delete[] task68Inputs;
    delete[] checkInCodelets73;
    delete[] barrierCodelets72;
    delete[] checkInCodelets68;
    delete[] checkInCodelets64;
    delete[] checkInCodelets59;
    delete[] nextCodeletsfib;
    delete[] nextSyncCodeletsfib;
    delete[] n_darts16;
    delete[] x_darts16;
    delete[] y_darts16;
    delete[] functionResult;
}
void TP16::setNewInputs(int in_n, long long* in_functionResult, size_t codeletID)
{
    this->n_darts16[codeletID] = in_n;
    this->functionResult[codeletID] = in_functionResult;
}
/*TP64: OMPTaskDirective*/
void TP64::_checkInCodelets66::fire(void)
{
/*Init the vars for this region*/

/*printing node 66: CallExpr*/
/*Finding the next codelet after the function call*/
#if 0
/*Signaling next codelet region: 66 nextRegion: 126 */
myTP->controlTPParent->checkInCodelets126.decDep();
#endif
/*Finding the next sync point before making the function call*/
#if 0
/*Find the sync point for a function call*/
/*syncNode: OMPTaskwaitDirective 72*/
/*No syncpoint found or outside task's boundaries, looking for syncCodelet*/
#endif
    /*Make the function call*/
    invoke<TP_fib>(myTP, myTP->numThreads, this->getID(),
        &(myTP->controlTPParent->checkInCodelets126),
        (myTP->controlTPParent->task64Inputs->nextSyncCodelet), nullptr,
        (this->taskInputs->n_darts64) - 1, &(*(this->taskInputs->x_darts64)));
}
void TP64::_checkInCodelets126::fire(void)
{

    /*printing node 126: NullStmt*/
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task64Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task64Inputs->nextSyncCodelet->decDep();
}
TP64::TP64(
    int in_numThreads, int in_mainCodeletID, TP16* in_TPParent, _task64Inputs* in_task64Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task64Inputs(in_task64Inputs)
    , TP66Ptr(nullptr)
    , TP66_alreadyLaunched(0)
    , checkInCodelets66(1, 1, this, this->mainCodeletID)
    , checkInCodelets126(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets66.decDep();
}
TP64::~TP64() { delete (task64Inputs); }
/*TP68: OMPTaskDirective*/
void TP68::_checkInCodelets70::fire(void)
{
/*Init the vars for this region*/

/*printing node 70: CallExpr*/
/*Finding the next codelet after the function call*/
#if 0
/*Signaling next codelet region: 70 nextRegion: 127 */
myTP->controlTPParent->checkInCodelets127.decDep();
#endif
/*Finding the next sync point before making the function call*/
#if 0
/*Find the sync point for a function call*/
/*syncNode: OMPTaskwaitDirective 72*/
/*No syncpoint found or outside task's boundaries, looking for syncCodelet*/
#endif
    /*Make the function call*/
    invoke<TP_fib>(myTP, myTP->numThreads, this->getID(),
        &(myTP->controlTPParent->checkInCodelets127),
        (myTP->controlTPParent->task68Inputs->nextSyncCodelet), nullptr,
        (this->taskInputs->n_darts68) - 2, &(*(this->taskInputs->y_darts68)));
}
void TP68::_checkInCodelets127::fire(void)
{

    /*printing node 127: NullStmt*/
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task68Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task68Inputs->nextSyncCodelet->decDep();
}
TP68::TP68(
    int in_numThreads, int in_mainCodeletID, TP16* in_TPParent, _task68Inputs* in_task68Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task68Inputs(in_task68Inputs)
    , TP70Ptr(nullptr)
    , TP70_alreadyLaunched(0)
    , checkInCodelets70(1, 1, this, this->mainCodeletID)
    , checkInCodelets127(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets70.decDep();
}
TP68::~TP68() { delete (task68Inputs); }
/*TP76: OMPParallelDirective*/
void TP76::_barrierCodelets76::fire(void)
{
    TP76* myTP = static_cast<TP76*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
void TP76::_checkInCodelets77::fire(void)
{
    /*Select the thread executing OMPSingleDirective 77*/
    if (!__sync_val_compare_and_swap(&(myTP->TP77_alreadyLaunched), 0, 1)) {
        invoke<TP77>(myTP, 1, this->getID(), myTP, &(*(this->inputsTPParent->n_darts76)));
    } else {
        myTP->barrierCodelets77[0].decDep();
    }
}
void TP76::_barrierCodelets77::fire(void)
{
    TP76* myTP = static_cast<TP76*>(myTP_);
    myTP->TPParent->barrierCodelets76[0].setDep(0);
    myTP->add(&(myTP->TPParent->barrierCodelets76[0]));
}
TP76::TP76(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet, int* in_n)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , n_darts76(in_n) /*OMP_SHARED - INPUT*/
    , TP77Ptr(nullptr)
    , TP77_alreadyLaunched(0)
    , barrierCodelets76(new _barrierCodelets76[1])
    , checkInCodelets77(new _checkInCodelets77[this->numThreads])
    , barrierCodelets77(new _barrierCodelets77[1])
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets76[0] = _barrierCodelets76(ompNumThreads, ompNumThreads, this, 0);
    barrierCodelets77[0] = _barrierCodelets77(this->numThreads, this->numThreads, this, 0);
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
    delete[] barrierCodelets77;
    delete[] checkInCodelets77;
}
/*TP77: OMPSingleDirective*/
void TP77::_checkInCodelets79::fire(void)
{
/*Init the vars for this region*/

/*printing node 79: CallExpr*/
/*Finding the next codelet after the function call*/
#if 0
/*Find and signal the next codelet*/
myTP->controlTPParent->TPParent->barrierCodelets77[0].decDep();
#endif
/*Finding the next sync point before making the function call*/
#if 0
/*Find the sync point for a function call*/
/*syncNode: OMPSingleDirective 77*/
/*Find the TP to which the nextRegion belongs*/
#endif
    /*Make the function call*/
    invoke<TP_fib>(myTP, myTP->numThreads, this->getID(),
        &(myTP->controlTPParent->TPParent->barrierCodelets77[0]),
        &(myTP->controlTPParent->TPParent->barrierCodelets77[0]), nullptr,
        (*(this->inputsTPParent->n_darts77)), &((par_res)));
}
TP77::TP77(int in_numThreads, int in_mainCodeletID, TP76* in_TPParent, int* in_n)
    : ompOMPSingleDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , n_darts77(in_n) /*OMP_SHARED - INPUT*/
    , TP79Ptr(nullptr)
    , TP79_alreadyLaunched(0)
    , checkInCodelets79(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets79.decDep();
}
TP77::~TP77() {}
