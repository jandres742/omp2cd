#include "arraybench.output.darts.h"
using namespace darts;
using namespace std;
double atest[1024];
double btest_dartsThreadPriv0[MAXNUMTHREADS][1024];
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
    /*CompoundStmt:400*/
    init(argc, argv);
    char reference_str[] = "reference time 1";
    reference(reference_str, &refer);
    char testName[32];
    sprintf(testName, "PRIVATE %d", 1024);
    benchmark(testName, &testprivnew);
    sprintf(testName, "FIRSTPRIVATE %d", 1024);
    benchmark(testName, &testfirstprivnew);
    sprintf(testName, "COPYPRIVATE %d", 1024);
    benchmark(testName, &testcopyprivnew);
    sprintf(testName, "COPYIN %d", 1024);
    benchmark(testName, &testthrprivnew);
    finalise();
    return 0;
}
/*Function: refer, ID: 2*/
void refer()
{
    /*refer:2*/
    /*CompoundStmt:420*/
    int j;
    double a[1];
    for (j = 0; j < (int)innerreps; j++) {
        array_delay(delaylength, a);
    }
}
/*Function: testfirstprivnew, ID: 3*/
void testfirstprivnew()
{
    /*testfirstprivnew:3*/
    /*CompoundStmt:430*/
    int j;
    for (j = 0; j < (int)innerreps; j++) {
        /*CompoundStmt:437*/
        if (affinMaskRes) {
            myDARTSRuntime->run(launch<TP438>(ompNumThreads * DARTS_CODELETS_MULT, 0,
                RuntimeFinalCodelet, (double*)(atest), (1024)));
        }
    }
}
/*Function: testprivnew, ID: 4*/
void testprivnew()
{
    /*testprivnew:4*/
    /*CompoundStmt:441*/
    int j;
    for (j = 0; j < (int)innerreps; j++) {
        /*CompoundStmt:448*/
        if (affinMaskRes) {
            myDARTSRuntime->run(
                launch<TP449>(ompNumThreads * DARTS_CODELETS_MULT, 0, RuntimeFinalCodelet, (1024)));
        }
    }
}
/*Function: testcopyprivnew, ID: 5*/
void testcopyprivnew()
{
    /*testcopyprivnew:5*/
    /*CompoundStmt:452*/
    int j;
    for (j = 0; j < (int)innerreps; j++) {
        /*CompoundStmt:459*/
        if (affinMaskRes) {
            myDARTSRuntime->run(
                launch<TP460>(ompNumThreads * DARTS_CODELETS_MULT, 0, RuntimeFinalCodelet, (1024)));
        }
    }
}
/*Function: testthrprivnew, ID: 6*/
void testthrprivnew()
{
    /*testthrprivnew:6*/
    /*CompoundStmt:465*/
    int j;
    for (j = 0; j < (int)innerreps; j++) {
        /*CompoundStmt:472*/
        if (affinMaskRes) {
            myDARTSRuntime->run(
                launch<TP473>(ompNumThreads * DARTS_CODELETS_MULT, 0, RuntimeFinalCodelet));
        }
    }
}
/*TP438: OMPParallelDirective*/
void TP438::_barrierCodelets438::fire(void)
{
    TP438* myTP = static_cast<TP438*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
void TP438::_checkInCodelets440::fire(void)
{
    /*Init the vars for this region*/
    this->inputsTPParent->atest_darts438[this->getID()]
        = (double*)malloc(sizeof(double) * this->inputsTPParent->atest_outer438_size);
    double* temp_atest_darts_ptr = (double*)(this->inputsTPParent->atest_darts438[this->getID()]);
    double* temp_atest_outer_ptr = (double*)(this->inputsTPParent->atest_outer438_ptr);
    for (uint64_t k_outer438 = 0; k_outer438 < this->inputsTPParent->atest_outer438_size;
         k_outer438++) {
        temp_atest_darts_ptr[k_outer438] = temp_atest_outer_ptr[k_outer438];
    }

    /*printing node 440: CallExpr*/
    array_delay(delaylength, (this->inputsTPParent->atest_darts438[this->getID()]));
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets438[0].decDep();
}
TP438::TP438(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet,
    double* in_atest_outer438_ptr, uint64_t in_atest_outer438_size)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , atest_outer438_size(in_atest_outer438_size)
    , atest_outer438_ptr(in_atest_outer438_ptr)
    , barrierCodelets438(new _barrierCodelets438[1])
    , checkInCodelets440(new _checkInCodelets440[this->numThreads])
{
    /*Initialize inputs and vars.*/
    this->atest_darts438
        = (double**)malloc(sizeof(double*) * this->numThreads); /*OMP_FIRSTPRIVATE*/
    /*Initialize Codelets*/
    barrierCodelets438[0] = _barrierCodelets438(ompNumThreads, ompNumThreads, this, 0);
    _checkInCodelets440* checkInCodelets440Ptr = (this->checkInCodelets440);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets440Ptr) = _checkInCodelets440(1, 1, this, codeletCounter);
        (*checkInCodelets440Ptr).decDep();
        checkInCodelets440Ptr++;
    }
}
TP438::~TP438()
{
    delete[] atest_darts438;
    delete[] barrierCodelets438;
    delete[] checkInCodelets440;
}
/*TP449: OMPParallelDirective*/
void TP449::_barrierCodelets449::fire(void)
{
    TP449* myTP = static_cast<TP449*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
void TP449::_checkInCodelets451::fire(void)
{
    /*Init the vars for this region*/
    this->inputsTPParent->atest_darts449[this->getID()]
        = (double*)malloc(sizeof(double) * this->inputsTPParent->atest_outer449_size);

    /*printing node 451: CallExpr*/
    array_delay(delaylength, (this->inputsTPParent->atest_darts449[this->getID()]));
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets449[0].decDep();
}
TP449::TP449(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet,
    uint64_t in_atest_outer449_size)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , atest_outer449_size(in_atest_outer449_size)
    , barrierCodelets449(new _barrierCodelets449[1])
    , checkInCodelets451(new _checkInCodelets451[this->numThreads])
{
    /*Initialize inputs and vars.*/
    this->atest_darts449 = (double**)malloc(sizeof(double*) * this->numThreads) /*OMP_PRIVATE*/;
    /*Initialize Codelets*/
    barrierCodelets449[0] = _barrierCodelets449(ompNumThreads, ompNumThreads, this, 0);
    _checkInCodelets451* checkInCodelets451Ptr = (this->checkInCodelets451);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets451Ptr) = _checkInCodelets451(1, 1, this, codeletCounter);
        (*checkInCodelets451Ptr).decDep();
        checkInCodelets451Ptr++;
    }
}
TP449::~TP449()
{
    delete[] atest_darts449;
    delete[] barrierCodelets449;
    delete[] checkInCodelets451;
}
/*TP460: OMPParallelDirective*/
void TP460::_barrierCodelets460::fire(void)
{
    TP460* myTP = static_cast<TP460*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
void TP460::_checkInCodelets462::fire(void)
{
    this->inputsTPParent->atest_darts460[this->getID()]
        = (double*)malloc(sizeof(double) * this->inputsTPParent->atest_outer460_size);
    /*Select the thread executing OMPSingleDirective 462*/
    if (!__sync_val_compare_and_swap(&(myTP->TP462_alreadyLaunched), 0, 1)) {
        invoke<TP462>(myTP, 1, this->getID(), myTP);
    } else {
        myTP->barrierCodelets462[0].decDep();
    }
}
void TP460::_barrierCodelets462::fire(void)
{
    TP460* myTP = static_cast<TP460*>(myTP_);
    /*Copy the values calculated by the single region
    to the other threads' vars specified in the
    copyprivate clause*/
    if (this->inputsTPParent->codeletIDexecutingSingle462 < ompNumThreads) {
        for (size_t codeletCounter = 0; codeletCounter < this->inputsTPParent->numThreads;
             codeletCounter++) {
            if (codeletCounter != this->inputsTPParent->codeletIDexecutingSingle462) {
                for (int k = 0; k < 1024; k++) {
                    double temp_atest
                        = (this->inputsTPParent
                                ->atest_darts460[this->inputsTPParent->codeletIDexecutingSingle462]
                                                [k]);
                    (this->inputsTPParent->atest_darts460[codeletCounter][k]) = temp_atest;
                }
            }
        }
    }
    myTP->TPParent->barrierCodelets460[0].setDep(0);
    myTP->add(&(myTP->TPParent->barrierCodelets460[0]));
}
TP460::TP460(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet,
    uint64_t in_atest_outer460_size)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , atest_outer460_size(in_atest_outer460_size)
    , TP462Ptr(nullptr)
    , TP462_alreadyLaunched(0)
    , codeletIDexecutingSingle462(ompNumThreads + 1)
    , barrierCodelets460(new _barrierCodelets460[1])
    , checkInCodelets462(new _checkInCodelets462[this->numThreads])
    , barrierCodelets462(new _barrierCodelets462[1])
{
    /*Initialize inputs and vars.*/
    this->atest_darts460 = (double**)malloc(sizeof(double*) * this->numThreads) /*OMP_PRIVATE*/;
    /*Initialize Codelets*/
    barrierCodelets460[0] = _barrierCodelets460(ompNumThreads, ompNumThreads, this, 0);
    barrierCodelets462[0] = _barrierCodelets462(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets462* checkInCodelets462Ptr = (this->checkInCodelets462);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets462Ptr) = _checkInCodelets462(1, 1, this, codeletCounter);
        (*checkInCodelets462Ptr).decDep();
        checkInCodelets462Ptr++;
    }
}
TP460::~TP460()
{
    delete[] atest_darts460;
    delete[] barrierCodelets460;
    delete[] barrierCodelets462;
    delete[] checkInCodelets462;
}
/*TP462: OMPSingleDirective*/
void TP462::_checkInCodelets464::fire(void)
{
    /*Init the vars for this region*/
    /*Get pointer from parent for variable
     with shared scope in this region but private
     in the enclosing one*/
    this->inputsTPParent->atest_darts462
        = (double*)&(myTP->TPParent->inputsTPParent->atest_darts460[this->getID()][1024]);

    /*printing node 464: CallExpr*/
    array_delay(delaylength, ((this->inputsTPParent->atest_darts462)));
    this->inputsTPParent->TPParent->codeletIDexecutingSingle462 = this->getID();
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets462[0].decDep();
}
TP462::TP462(int in_numThreads, int in_mainCodeletID, TP460* in_TPParent)
    : ompOMPSingleDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , checkInCodelets464(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets464.decDep();
}
TP462::~TP462() {}
/*TP473: OMPParallelDirective*/
void TP473::_barrierCodelets473::fire(void)
{
    TP473* myTP = static_cast<TP473*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
void TP473::_checkInCodelets475::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 475: CallExpr*/
    array_delay(delaylength, ((btest_dartsThreadPriv0[this->getID()])));
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets473[0].decDep();
}
TP473::TP473(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , barrierCodelets473(new _barrierCodelets473[1])
    , checkInCodelets475(new _checkInCodelets475[this->numThreads])
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets473[0] = _barrierCodelets473(ompNumThreads, ompNumThreads, this, 0);
    _checkInCodelets475* checkInCodelets475Ptr = (this->checkInCodelets475);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        if (codeletCounter != 0) {
            for (int k = 0; k < 1024; k++) {
                btest_dartsThreadPriv0[codeletCounter][k] = btest_dartsThreadPriv0[0][k];
            }
        }
        (*checkInCodelets475Ptr) = _checkInCodelets475(1, 1, this, codeletCounter);
        (*checkInCodelets475Ptr).decDep();
        checkInCodelets475Ptr++;
    }
}
TP473::~TP473()
{
    delete[] barrierCodelets473;
    delete[] checkInCodelets475;
}
