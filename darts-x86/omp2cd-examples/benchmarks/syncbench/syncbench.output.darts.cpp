#include "syncbench.output.darts.h"
using namespace darts;
using namespace std;
std::mutex TP571mutex;
std::mutex TP642mutex;
omp_lock_t omp_lock;
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
    /*CompoundStmt:413*/
    init(argc, argv);
    omp_init_lock(&omp_lock);
    reference("reference time 1", &refer);
    benchmark("PARALLEL", &testpr);
    benchmark("FOR", &testfor);
    benchmark("PARALLEL FOR", &testpfor);
    benchmark("BARRIER", &testbar);
    benchmark("SINGLE", &testsing);
    benchmark("CRITICAL", &testcrit);
    benchmark("LOCK/UNLOCK", &testlock);
    reference("reference time 2", &referatom);
    benchmark("ATOMIC", &testatom);
    reference("reference time 3", &referred);
    benchmark("REDUCTION", &testred);
    finalise();
    return 0;
}
/*Function: refer, ID: 2*/
void refer()
{
    /*refer:2*/
    /*CompoundStmt:443*/
    int j;
    for (j = 0; j < innerreps; j++) {
        delay(delaylength);
    }
}
/*Function: testpr, ID: 3*/
void testpr()
{
    /*testpr:3*/
    /*CompoundStmt:451*/
    int j;
    for (j = 0; j < innerreps; j++) {
        /*CompoundStmt:457*/
        if (affinMaskRes) {
            myDARTSRuntime->run(
                launch<TP458>(ompNumThreads * DARTS_CODELETS_MULT, 0, RuntimeFinalCodelet));
        }
    }
}
/*Function: testfor, ID: 4*/
void testfor()
{
    /*testfor:4*/
    /*CompoundStmt:461*/
    int i, j;
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP463>(
            ompNumThreads * DARTS_CODELETS_MULT, 0, RuntimeFinalCodelet, (int*)&((i))));
    }
}
/*Function: testpfor, ID: 5*/
void testpfor()
{
    /*testpfor:5*/
    /*CompoundStmt:501*/
    int i, j;
    for (j = 0; j < innerreps; j++) {
        /*CompoundStmt:507*/
        if (affinMaskRes) {
            myDARTSRuntime->run(launch<TP508>(
                ompNumThreads * DARTS_CODELETS_MULT, 0, RuntimeFinalCodelet, 0, nthreads));
        }
    }
}
/*Function: testbar, ID: 6*/
void testbar()
{
    /*testbar:6*/
    /*CompoundStmt:539*/
    int j;
    if (affinMaskRes) {
        myDARTSRuntime->run(
            launch<TP541>(ompNumThreads * DARTS_CODELETS_MULT, 0, RuntimeFinalCodelet));
    }
}
/*Function: testsing, ID: 7*/
void testsing()
{
    /*testsing:7*/
    /*CompoundStmt:550*/
    int j;
    if (affinMaskRes) {
        myDARTSRuntime->run(
            launch<TP552>(ompNumThreads * DARTS_CODELETS_MULT, 0, RuntimeFinalCodelet));
    }
}
/*Function: testcrit, ID: 8*/
void testcrit()
{
    /*testcrit:8*/
    /*CompoundStmt:561*/
    int j;
    if (affinMaskRes) {
        myDARTSRuntime->run(
            launch<TP563>(ompNumThreads * DARTS_CODELETS_MULT, 0, RuntimeFinalCodelet));
    }
}
/*Function: testlock, ID: 9*/
void testlock()
{
    /*testlock:9*/
    /*CompoundStmt:574*/
    int j;
    if (affinMaskRes) {
        myDARTSRuntime->run(
            launch<TP576>(ompNumThreads * DARTS_CODELETS_MULT, 0, RuntimeFinalCodelet));
    }
}
/*Function: referatom, ID: 10*/
void referatom()
{
    /*referatom:10*/
    /*CompoundStmt:589*/
    int j;
    double aaaa = 0.;
    double epsilon = 1.0000000000000001E-15;
    double b, c;
    b = 1.;
    c = (1. + epsilon);
    for (j = 0; j < innerreps; j++) {
        aaaa += b;
        b *= c;
    }
    if (aaaa < 0.)
        printf("%f\n", aaaa);
}
/*Function: referred, ID: 11*/
void referred()
{
    /*referred:11*/
    /*CompoundStmt:612*/
    int j;
    int aaaa = 0;
    for (j = 0; j < innerreps; j++) {
        delay(delaylength);
        aaaa += 1;
    }
}
/*Function: testatom, ID: 12*/
void testatom()
{
    /*testatom:12*/
    /*CompoundStmt:622*/
    int j;
    double aaaa = 0.;
    double epsilon = 1.0000000000000001E-15;
    double b, c;
    b = 1.;
    c = (1. + epsilon);
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP634>(ompNumThreads * DARTS_CODELETS_MULT, 0,
            RuntimeFinalCodelet, (double*)&((aaaa)), (double*)&((b)), (double*)&((c))));
    }
    if (aaaa < 0.)
        printf("%f\n", aaaa);
}
/*Function: testred, ID: 13*/
void testred()
{
    /*testred:13*/
    /*CompoundStmt:652*/
    int j;
    int aaaa = 0;
    for (j = 0; j < innerreps; j++) {
        /*CompoundStmt:659*/
        if (affinMaskRes) {
            myDARTSRuntime->run(launch<TP660>(
                ompNumThreads * DARTS_CODELETS_MULT, 0, RuntimeFinalCodelet, (int*)&((aaaa))));
        }
    }
}
/*TP458: OMPParallelDirective*/
void TP458::_barrierCodelets458::fire(void)
{
    TP458* myTP = static_cast<TP458*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
void TP458::_checkInCodelets460::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 460: CallExpr*/
    delay(delaylength);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets458[0].decDep();
}
TP458::TP458(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , barrierCodelets458(new _barrierCodelets458[1])
    , checkInCodelets460(new _checkInCodelets460[this->numThreads])
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets458[0] = _barrierCodelets458(ompNumThreads, ompNumThreads, this, 0);
    _checkInCodelets460* checkInCodelets460Ptr = (this->checkInCodelets460);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets460Ptr) = _checkInCodelets460(1, 1, this, codeletCounter);
        (*checkInCodelets460Ptr).decDep();
        checkInCodelets460Ptr++;
    }
}
TP458::~TP458()
{
    delete[] barrierCodelets458;
    delete[] checkInCodelets460;
}
/*TP463: OMPParallelDirective*/
void TP463::_barrierCodelets463::fire(void)
{
    TP463* myTP = static_cast<TP463*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
void TP463::_checkInCodelets466::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 466: BinaryOperator*/
    (this->inputsTPParent->j_darts463[this->getID()]) = 0;

    /*printing node 467: BinaryOperator*/
    /*Print the code for a condition node in a complex loop stmt */
    if ((this->inputsTPParent->j_darts463[this->getID()]) < innerreps) {
        /*Signal the first codelet in the loop*/
        myTP->checkInCodelets465[this->getID()].decDep();
        return;
    }
    /*Signal the codelet after the loop from the end's condional node.*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets463[0].decDep();
}
void TP463::_checkInCodelets465::fire(void)
{

    /*printing node 465: ForStmt*/
    bool haveToLaunch = __sync_bool_compare_and_swap(&(myTP->controlTPParent->TP465_LoopCounter),
        myTP->controlTPParent->TP465_LoopCounterPerThread[this->getID()],
        myTP->controlTPParent->TP465_LoopCounterPerThread[this->getID()] + 1);
    unsigned int iterIdx = myTP->controlTPParent->TP465_LoopCounterPerThread[this->getID()];
    if (haveToLaunch) {
        this->resetCodelet();
        myTP->controlTPParent->TP465PtrVec.push_back(nullptr);
        myTP->controlTPParent->TP465_LoopCounterPerThread[this->getID()] += 1;
        invoke<TP465>(myTP, myTP->numThreads, this->getID(), myTP, myTP->inputsTPParent,
            &(myTP->controlTPParent->TP465PtrVec.back()));
    } else {
        if (myTP->controlTPParent->TP465PtrVec.size() == 0) {
            this->resetCodelet();
            this->decDep();
            return;
        } else if (myTP->controlTPParent->TP465PtrVec.size() < (iterIdx + 1)) {
            this->resetCodelet();
            this->decDep();
            return;
        } else if (myTP->controlTPParent->TP465PtrVec[iterIdx] == nullptr) {
            this->resetCodelet();
            this->decDep();
            return;
        } else {
            this->resetCodelet();
#if USE_SPIN_CODELETS == 0
            myTP->controlTPParent->TP465PtrVec[iterIdx]->firstCodelet[this->getID()].decDep();
#else
            myTP->controlTPParent->TP465PtrVec[iterIdx]->availableCodelets[this->getID()] = 1;
#endif
            myTP->controlTPParent->TP465_LoopCounterPerThread[this->getID()] += 1;
        }
    }
}
void TP463::_checkInCodelets468::fire(void)
{

    /*printing node 468: UnaryOperator*/
    (this->inputsTPParent->j_darts463[this->getID()])++;

    /*printing node 666: BinaryOperator*/
    /*Print the code for a condition node in a complex loop stmt */
    if ((this->inputsTPParent->j_darts463[this->getID()]) < innerreps) {
        this->resetCodelet();
        /*Signal the first codelet in the loop*/
        myTP->checkInCodelets465[this->getID()].decDep();
        return;
    }
    /*Signal the codelet after the loop from the condtional node.*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets463[0].decDep();
}
TP463::TP463(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet, int* in_i)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , i_darts463(in_i) /*OMP_SHARED - INPUT*/
    , j_darts463(new int[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , TP465_LoopCounter(0)
    , TP465_LoopCounterPerThread(new unsigned int[this->numThreads])
    , barrierCodelets463(new _barrierCodelets463[1])
    , checkInCodelets466(new _checkInCodelets466[this->numThreads])
    , checkInCodelets465(new _checkInCodelets465[this->numThreads])
    , checkInCodelets468(new _checkInCodelets468[this->numThreads])
{
    memset((void*)TP465_LoopCounterPerThread, 0, this->numThreads * sizeof(unsigned int));
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets463[0] = _barrierCodelets463(ompNumThreads, ompNumThreads, this, 0);
    _checkInCodelets468* checkInCodelets468Ptr = (this->checkInCodelets468);
    _checkInCodelets465* checkInCodelets465Ptr = (this->checkInCodelets465);
    _checkInCodelets466* checkInCodelets466Ptr = (this->checkInCodelets466);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets468Ptr) = _checkInCodelets468(1, 1, this, codeletCounter);
        checkInCodelets468Ptr++;
        (*checkInCodelets465Ptr) = _checkInCodelets465(1, 1, this, codeletCounter);
        checkInCodelets465Ptr++;
        (*checkInCodelets466Ptr) = _checkInCodelets466(1, 1, this, codeletCounter);
        (*checkInCodelets466Ptr).decDep();
        checkInCodelets466Ptr++;
    }
}
TP463::~TP463()
{
    delete[] TP465_LoopCounterPerThread;
    delete[] barrierCodelets463;
    delete[] checkInCodelets468;
    delete[] checkInCodelets465;
    delete[] checkInCodelets466;
}
/*TP465: ForStmt*/
void TP465::_checkInCodelets470::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    /*printOMPForCodeletFireCode, inlined: 0*/
    /*region 470 0*/
    /*Determine the TP to which this codelet belongs and check if this codelet spawns the TP or if
     * it signals it using dispatchCodelet()*/
    size_t idx = this->getID() / myTP->codeletsPerTP470;
    if (idx < myTP->TPsToUse470) {
        if (!__sync_val_compare_and_swap(&(myTP->TP470_alreadyLaunched[idx]), 0, 1)) {
            int range = abs(nthreads - 0) / 1;
            int rangePerCodelet = range / myTP->TPsToUse470;
            int minIteration = min<int>(nthreads, 0);
            int remainderRange = range % myTP->TPsToUse470;
            int initIteration = rangePerCodelet * idx;
            int lastIteration = rangePerCodelet * (idx + 1);
            if (remainderRange != 0) {
                if (idx < (uint32_t)remainderRange) {
                    initIteration += idx;
                    lastIteration += (idx + 1);
                } else {
                    initIteration += remainderRange;
                    lastIteration += remainderRange;
                }
            }
            initIteration = initIteration * 1 + minIteration;
            lastIteration = lastIteration * 1 + minIteration;
            if (0 < nthreads) {
                initIteration = min(initIteration, lastIteration);
                lastIteration = max(initIteration, lastIteration);
            } else {
                initIteration = max(initIteration, lastIteration);
                lastIteration = min(initIteration, lastIteration);
            }
            if (idx == myTP->TPsToUse470 - 1) {
                lastIteration = nthreads;
            }
#if USEINVOKE == 1
            invoke<TP470>(myTP, myTP->codeletsPerTP470 * DARTS_CODELETS_MULT, this->getID(), myTP,
                initIteration, lastIteration, &(myTP->TP470Ptr[idx]));
#else
            place<TP470>(idx, myTP, myTP->codeletsPerTP470 * DARTS_CODELETS_MULT, this->getID(),
                myTP, initIteration, lastIteration, &(myTP->TP470Ptr[idx]));
#endif
        } else {
            if (myTP->TP470Ptr[idx] != nullptr) {
                myTP->TP470Ptr[idx]->dispatchCodelet(this->getID());
            } else {
                this->resetCodelet();
                this->decDep();
            }
        }
    }
}
void TP465::_barrierCodelets470::fire(void)
{
    TP465* myTP = static_cast<TP465*>(myTP_);
    for (size_t codeletsCounter = 0; codeletsCounter < myTP->numThreads; codeletsCounter++) {
        myTP->TPParent->checkInCodelets468[codeletsCounter].decDep();
    }
}
TP465::TP465(int in_numThreads, int in_mainCodeletID, TP463* in_TPParent, TP463* in_inputsTPParent,
    TP465** in_ptrToThisTP)
    : ompTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(in_inputsTPParent)
    , ptrToThisTP(in_ptrToThisTP)
    , TP470Ptr(new TP470*[NUMTPS470])
    , TP470_alreadyLaunched(new size_t[NUMTPS470])
    , numTPsSet470(0)
    , numTPsReady470(0)
    , TPsToUse470(NUMTPS470)
    , codeletsPerTP470(this->numThreads / NUMTPS470)
    , totalCodelets470(this->TPsToUse470 * this->codeletsPerTP470)
    , checkInCodelets470(new _checkInCodelets470[this->numThreads])
    , barrierCodelets470(new _barrierCodelets470[1])
{
    /*Initialize Codelets*/
    barrierCodelets470[0] = _barrierCodelets470(NUMTPS470, NUMTPS470, this, 0);
    _checkInCodelets470* checkInCodelets470Ptr = (this->checkInCodelets470);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets470);
#endif
    for (int i = 0; i < NUMTPS470; i++) {
        TP470Ptr[i] = nullptr;
        TP470_alreadyLaunched[i] = 0;
    }
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets470Ptr) = _checkInCodelets470(2, 1, this, codeletCounter);
#else
        (*checkInCodelets470Ptr) = _checkInCodelets470(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets470Ptr).decDep();
        checkInCodelets470Ptr++;
    }
    *(this->ptrToThisTP) = this;
#if USE_SPIN_CODELETS == 0
    this->firstCodelet[this->getID()].decDep();
#else
    this->availableCodelets[this->getID()] = 1;
#endif
}
TP465::~TP465()
{
    delete[] barrierCodelets470;
    delete[] checkInCodelets470;
}
/*TP470: OMPForDirective*/
void TP470::_barrierCodelets470::fire(void)
{
    TP470* myTP = static_cast<TP470*>(myTP_);
    myTP->controlTPParent->TPParent->barrierCodelets470[0].decDep();
}
bool TP470::requestNewRangeIterations470(int* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Static */
    /*Chunk = 0*/
    bool isThereNewIteration = false;
    {
        /*Static Scheduling*/
        int tempStartRange = rangePerCodelet470 * codeletID;
        int tempEndRange = rangePerCodelet470 * (codeletID + 1);
        if (remainderRange470 != 0) {
            if (codeletID < (uint32_t)remainderRange470) {
                tempStartRange += codeletID;
                tempEndRange += (codeletID + 1);
            } else {
                tempStartRange += remainderRange470;
                tempEndRange += remainderRange470;
            }
        }
        tempStartRange = tempStartRange * 1 + minIteration470;
        tempEndRange = tempEndRange * 1 + minIteration470;
        if (tempStartRange != tempEndRange) {
            isThereNewIteration = true;
        }
        if (initIteration470 < lastIteration470) {
            (this->inputsTPParent->i_darts470[codeletID]) = min(tempStartRange, tempEndRange);
            *endRange = max(tempStartRange, tempEndRange);
        } else {
            (this->inputsTPParent->i_darts470[codeletID]) = max(tempStartRange, tempEndRange);
            *endRange = min(tempStartRange, tempEndRange);
        }
        if (codeletID == this->numThreads - 1) {
            *endRange = lastIteration470;
        }
    }
    return isThereNewIteration;
}
void TP470::_checkInCodelets471::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    /*Init the vars for this region*/

    /*printing node 471: ForStmt*/
    /*var: i*/
    int* i = &(this->inputsTPParent->i_darts470[this->getLocalID()]);
    (void)i /*OMP_PRIVATE*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations470(
        (int*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        /*Signaling omp for stmt's barrier*/
        myTP->controlTPParent->barrierCodelets470[0].decDep();
        return;
    }
    for (int i_darts_counter_temp470 = (*i); i_darts_counter_temp470 < endRange
         && i_darts_counter_temp470 < this->inputsTPParent->lastIteration470;
         i_darts_counter_temp470++) {
        {
            delay(delaylength);
        }
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling omp for stmt's barrier*/
    myTP->controlTPParent->barrierCodelets470[0].decDep();
}
TP470::TP470(int in_numThreads, int in_mainCodeletID, TP465* in_TPParent, int in_initIteration,
    int in_lastIteration, TP470** in_ptrToThisTP)
    : ompTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , i_darts470(new int[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , initIteration470(in_initIteration)
    , lastIteration470(in_lastIteration)
    , readyCodelets(0)
    , baseNumThreads(this->numThreads / DARTS_CODELETS_MULT)
    , barrierCodelets470(new _barrierCodelets470[1])
    , checkInCodelets471(new _checkInCodelets471[this->numThreads])
{
    /*Initialize the loop parameters*/
    range470 = abs(lastIteration470 - initIteration470) / 1;
    rangePerCodelet470 = range470 / numThreads;
    minIteration470 = min<int>(lastIteration470, initIteration470);
    remainderRange470 = range470 % numThreads;
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets470[0] = _barrierCodelets470(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets471* checkInCodelets471Ptr = (this->checkInCodelets471);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets471);
#endif
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->baseNumThreads;
         codeletCounter++) {
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets471Ptr) = _checkInCodelets471(2, 1, this, codeletCounter);
#else
        (*checkInCodelets471Ptr) = _checkInCodelets471(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets471Ptr).decDep();
        checkInCodelets471Ptr++;
    }
    this->dispatchCodelet(this->getID());
    *(in_ptrToThisTP) = this;
}
void TP470::dispatchCodelet(size_t codeletID)
{
    int idx = codeletID / this->baseNumThreads;
    int localID = codeletID - this->baseNumThreads * idx;
    this->checkInCodelets471[localID].setID(codeletID);
    this->checkInCodelets471[localID].setLocalID(localID);
    /*Check if we want to replicate codelets*/
    for (size_t i = 1; i < (size_t)DARTS_CODELETS_MULT; i++) {
#if USE_SPIN_CODELETS == 0
        this->checkInCodelets471[localID + this->baseNumThreads * i]
            = _checkInCodelets471(2, 1, this, localID + this->baseNumThreads * i);
#else
        this->checkInCodelets471[localID + this->baseNumThreads * i]
            = _checkInCodelets471(1, 1, this, localID + this->baseNumThreads * i);
#endif
        this->checkInCodelets471[localID + this->baseNumThreads * i].setID(codeletID);
        this->checkInCodelets471[localID + this->baseNumThreads * i].decDep();
#if USE_SPIN_CODELETS == 0
        this->firstCodelet[localID + this->baseNumThreads * i].decDep();
#else
        this->availableCodelets[localID + this->baseNumThreads * i] = 1;
#endif
    }
#if USE_SPIN_CODELETS == 0
    this->firstCodelet[localID].decDep();
#else
    this->availableCodelets[localID] = 1;
#endif
}
TP470::~TP470()
{
    delete[] barrierCodelets470;
    delete[] checkInCodelets471;
}
/*TP508: OMPParallelForDirective*/
void TP508::_barrierCodelets508::fire(void)
{
    TP508* myTP = static_cast<TP508*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
bool TP508::requestNewRangeIterations508(int* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Static */
    /*Chunk = 0*/
    bool isThereNewIteration = false;
    {
        /*Static Scheduling*/
        int tempStartRange = rangePerCodelet508 * codeletID;
        int tempEndRange = rangePerCodelet508 * (codeletID + 1);
        if (remainderRange508 != 0) {
            if (codeletID < (uint32_t)remainderRange508) {
                tempStartRange += codeletID;
                tempEndRange += (codeletID + 1);
            } else {
                tempStartRange += remainderRange508;
                tempEndRange += remainderRange508;
            }
        }
        tempStartRange = tempStartRange * 1 + minIteration508;
        tempEndRange = tempEndRange * 1 + minIteration508;
        if (tempStartRange != tempEndRange) {
            isThereNewIteration = true;
        }
        if (initIteration508 < lastIteration508) {
            (this->inputsTPParent->i_darts508[codeletID]) = min(tempStartRange, tempEndRange);
            *endRange = max(tempStartRange, tempEndRange);
        } else {
            (this->inputsTPParent->i_darts508[codeletID]) = max(tempStartRange, tempEndRange);
            *endRange = min(tempStartRange, tempEndRange);
        }
        if (codeletID == this->numThreads - 1) {
            *endRange = lastIteration508;
        }
    }
    return isThereNewIteration;
}
void TP508::_checkInCodelets509::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 509: ForStmt*/
    /*var: i*/
    int* i = &(this->inputsTPParent->i_darts508[this->getLocalID()]);
    (void)i /*OMP_PRIVATE*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations508(
        (int*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        /*Find and signal the next codelet*/
        myTP->controlTPParent->TPParent->barrierCodelets508[0].decDep();
        return;
    }
    for (int i_darts_counter_temp508 = (*i); i_darts_counter_temp508 < endRange
         && i_darts_counter_temp508 < this->inputsTPParent->lastIteration508;
         i_darts_counter_temp508++) {
        {
            delay(delaylength);
        }
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets508[0].decDep();
}
TP508::TP508(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet, int in_initIteration,
    int in_lastIteration)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , i_darts508(new int[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , initIteration508(in_initIteration)
    , lastIteration508(in_lastIteration)
    , barrierCodelets508(new _barrierCodelets508[1])
    , checkInCodelets509(new _checkInCodelets509[this->numThreads])
{
    /*Initialize the loop parameters*/
    range508 = abs(lastIteration508 - initIteration508) / 1;
    rangePerCodelet508 = range508 / numThreads;
    minIteration508 = min<int>(lastIteration508, initIteration508);
    remainderRange508 = range508 % numThreads;
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets508[0] = _barrierCodelets508(ompNumThreads, ompNumThreads, this, 0);
    _checkInCodelets509* checkInCodelets509Ptr = (this->checkInCodelets509);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets509Ptr) = _checkInCodelets509(1, 1, this, codeletCounter);
        (*checkInCodelets509Ptr).decDep();
        checkInCodelets509Ptr++;
    }
}
TP508::~TP508()
{
    delete[] barrierCodelets508;
    delete[] checkInCodelets509;
}
/*TP541: OMPParallelDirective*/
void TP541::_barrierCodelets541::fire(void)
{
    TP541* myTP = static_cast<TP541*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
void TP541::_checkInCodelets544::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 544: BinaryOperator*/
    (this->inputsTPParent->j_darts541[this->getID()]) = 0;

    /*printing node 545: BinaryOperator*/
    /*Print the code for a condition node in a complex loop stmt */
    if ((this->inputsTPParent->j_darts541[this->getID()]) < innerreps) {
        /*Signal the first codelet in the loop*/
        myTP->checkInCodelets543[this->getID()].decDep();
        return;
    }
    /*Signal the codelet after the loop from the end's condional node.*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets541[0].decDep();
}
void TP541::_checkInCodelets543::fire(void)
{

    /*printing node 543: ForStmt*/
    bool haveToLaunch = __sync_bool_compare_and_swap(&(myTP->controlTPParent->TP543_LoopCounter),
        myTP->controlTPParent->TP543_LoopCounterPerThread[this->getID()],
        myTP->controlTPParent->TP543_LoopCounterPerThread[this->getID()] + 1);
    unsigned int iterIdx = myTP->controlTPParent->TP543_LoopCounterPerThread[this->getID()];
    if (haveToLaunch) {
        this->resetCodelet();
        myTP->controlTPParent->TP543PtrVec.push_back(nullptr);
        myTP->controlTPParent->TP543_LoopCounterPerThread[this->getID()] += 1;
        invoke<TP543>(myTP, myTP->numThreads, this->getID(), myTP, myTP->inputsTPParent,
            &(myTP->controlTPParent->TP543PtrVec.back()));
    } else {
        if (myTP->controlTPParent->TP543PtrVec.size() == 0) {
            this->resetCodelet();
            this->decDep();
            return;
        } else if (myTP->controlTPParent->TP543PtrVec.size() < (iterIdx + 1)) {
            this->resetCodelet();
            this->decDep();
            return;
        } else if (myTP->controlTPParent->TP543PtrVec[iterIdx] == nullptr) {
            this->resetCodelet();
            this->decDep();
            return;
        } else {
            this->resetCodelet();
#if USE_SPIN_CODELETS == 0
            myTP->controlTPParent->TP543PtrVec[iterIdx]->firstCodelet[this->getID()].decDep();
#else
            myTP->controlTPParent->TP543PtrVec[iterIdx]->availableCodelets[this->getID()] = 1;
#endif
            myTP->controlTPParent->TP543_LoopCounterPerThread[this->getID()] += 1;
        }
    }
}
void TP541::_checkInCodelets546::fire(void)
{

    /*printing node 546: UnaryOperator*/
    (this->inputsTPParent->j_darts541[this->getID()])++;

    /*printing node 668: BinaryOperator*/
    /*Print the code for a condition node in a complex loop stmt */
    if ((this->inputsTPParent->j_darts541[this->getID()]) < innerreps) {
        this->resetCodelet();
        /*Signal the first codelet in the loop*/
        myTP->checkInCodelets543[this->getID()].decDep();
        return;
    }
    /*Signal the codelet after the loop from the condtional node.*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets541[0].decDep();
}
TP541::TP541(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , j_darts541(new int[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , TP543_LoopCounter(0)
    , TP543_LoopCounterPerThread(new unsigned int[this->numThreads])
    , barrierCodelets541(new _barrierCodelets541[1])
    , checkInCodelets544(new _checkInCodelets544[this->numThreads])
    , checkInCodelets543(new _checkInCodelets543[this->numThreads])
    , checkInCodelets546(new _checkInCodelets546[this->numThreads])
{
    memset((void*)TP543_LoopCounterPerThread, 0, this->numThreads * sizeof(unsigned int));
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets541[0] = _barrierCodelets541(ompNumThreads, ompNumThreads, this, 0);
    _checkInCodelets546* checkInCodelets546Ptr = (this->checkInCodelets546);
    _checkInCodelets543* checkInCodelets543Ptr = (this->checkInCodelets543);
    _checkInCodelets544* checkInCodelets544Ptr = (this->checkInCodelets544);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets546Ptr) = _checkInCodelets546(1, 1, this, codeletCounter);
        checkInCodelets546Ptr++;
        (*checkInCodelets543Ptr) = _checkInCodelets543(1, 1, this, codeletCounter);
        checkInCodelets543Ptr++;
        (*checkInCodelets544Ptr) = _checkInCodelets544(1, 1, this, codeletCounter);
        (*checkInCodelets544Ptr).decDep();
        checkInCodelets544Ptr++;
    }
}
TP541::~TP541()
{
    delete[] TP543_LoopCounterPerThread;
    delete[] barrierCodelets541;
    delete[] checkInCodelets546;
    delete[] checkInCodelets543;
    delete[] checkInCodelets544;
}
/*TP543: ForStmt*/
void TP543::_checkInCodelets548::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif

    /*printing node 548: CallExpr*/
    delay(delaylength);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 548 nextRegion: 549 */
    myTP->controlTPParent->barrierCodelets549[0].decDep();
}
void TP543::_barrierCodelets549::fire(void)
{
    TP543* myTP = static_cast<TP543*>(myTP_);
    for (size_t codeletsCounter = 0; codeletsCounter < myTP->numThreads; codeletsCounter++) {
        myTP->TPParent->checkInCodelets546[codeletsCounter].decDep();
    }
}
TP543::TP543(int in_numThreads, int in_mainCodeletID, TP541* in_TPParent, TP541* in_inputsTPParent,
    TP543** in_ptrToThisTP)
    : ompTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(in_inputsTPParent)
    , ptrToThisTP(in_ptrToThisTP)
    , checkInCodelets548(new _checkInCodelets548[this->numThreads])
    , barrierCodelets549(new _barrierCodelets549[1])
{
    /*Initialize Codelets*/
    barrierCodelets549[0] = _barrierCodelets549(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets548* checkInCodelets548Ptr = (this->checkInCodelets548);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets548);
#endif
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets548Ptr) = _checkInCodelets548(2, 1, this, codeletCounter);
#else
        (*checkInCodelets548Ptr) = _checkInCodelets548(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets548Ptr).decDep();
        checkInCodelets548Ptr++;
    }
    *(this->ptrToThisTP) = this;
#if USE_SPIN_CODELETS == 0
    this->firstCodelet[this->getID()].decDep();
#else
    this->availableCodelets[this->getID()] = 1;
#endif
}
TP543::~TP543()
{
    delete[] barrierCodelets549;
    delete[] checkInCodelets548;
}
/*TP552: OMPParallelDirective*/
void TP552::_barrierCodelets552::fire(void)
{
    TP552* myTP = static_cast<TP552*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
void TP552::_checkInCodelets555::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 555: BinaryOperator*/
    (this->inputsTPParent->j_darts552[this->getID()]) = 0;

    /*printing node 556: BinaryOperator*/
    /*Print the code for a condition node in a complex loop stmt */
    if ((this->inputsTPParent->j_darts552[this->getID()]) < innerreps) {
        /*Signal the first codelet in the loop*/
        myTP->checkInCodelets554[this->getID()].decDep();
        return;
    }
    /*Signal the codelet after the loop from the end's condional node.*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets552[0].decDep();
}
void TP552::_checkInCodelets554::fire(void)
{

    /*printing node 554: ForStmt*/
    bool haveToLaunch = __sync_bool_compare_and_swap(&(myTP->controlTPParent->TP554_LoopCounter),
        myTP->controlTPParent->TP554_LoopCounterPerThread[this->getID()],
        myTP->controlTPParent->TP554_LoopCounterPerThread[this->getID()] + 1);
    unsigned int iterIdx = myTP->controlTPParent->TP554_LoopCounterPerThread[this->getID()];
    if (haveToLaunch) {
        this->resetCodelet();
        myTP->controlTPParent->TP554PtrVec.push_back(nullptr);
        myTP->controlTPParent->TP554_LoopCounterPerThread[this->getID()] += 1;
        invoke<TP554>(myTP, myTP->numThreads, this->getID(), myTP, myTP->inputsTPParent,
            &(myTP->controlTPParent->TP554PtrVec.back()));
    } else {
        if (myTP->controlTPParent->TP554PtrVec.size() == 0) {
            this->resetCodelet();
            this->decDep();
            return;
        } else if (myTP->controlTPParent->TP554PtrVec.size() < (iterIdx + 1)) {
            this->resetCodelet();
            this->decDep();
            return;
        } else if (myTP->controlTPParent->TP554PtrVec[iterIdx] == nullptr) {
            this->resetCodelet();
            this->decDep();
            return;
        } else {
            this->resetCodelet();
#if USE_SPIN_CODELETS == 0
            myTP->controlTPParent->TP554PtrVec[iterIdx]->firstCodelet[this->getID()].decDep();
#else
            myTP->controlTPParent->TP554PtrVec[iterIdx]->availableCodelets[this->getID()] = 1;
#endif
            myTP->controlTPParent->TP554_LoopCounterPerThread[this->getID()] += 1;
        }
    }
}
void TP552::_checkInCodelets557::fire(void)
{

    /*printing node 557: UnaryOperator*/
    (this->inputsTPParent->j_darts552[this->getID()])++;

    /*printing node 669: BinaryOperator*/
    /*Print the code for a condition node in a complex loop stmt */
    if ((this->inputsTPParent->j_darts552[this->getID()]) < innerreps) {
        this->resetCodelet();
        /*Signal the first codelet in the loop*/
        myTP->checkInCodelets554[this->getID()].decDep();
        return;
    }
    /*Signal the codelet after the loop from the condtional node.*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets552[0].decDep();
}
TP552::TP552(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , j_darts552(new int[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , TP554_LoopCounter(0)
    , TP554_LoopCounterPerThread(new unsigned int[this->numThreads])
    , barrierCodelets552(new _barrierCodelets552[1])
    , checkInCodelets555(new _checkInCodelets555[this->numThreads])
    , checkInCodelets554(new _checkInCodelets554[this->numThreads])
    , checkInCodelets557(new _checkInCodelets557[this->numThreads])
{
    memset((void*)TP554_LoopCounterPerThread, 0, this->numThreads * sizeof(unsigned int));
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets552[0] = _barrierCodelets552(ompNumThreads, ompNumThreads, this, 0);
    _checkInCodelets557* checkInCodelets557Ptr = (this->checkInCodelets557);
    _checkInCodelets554* checkInCodelets554Ptr = (this->checkInCodelets554);
    _checkInCodelets555* checkInCodelets555Ptr = (this->checkInCodelets555);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets557Ptr) = _checkInCodelets557(1, 1, this, codeletCounter);
        checkInCodelets557Ptr++;
        (*checkInCodelets554Ptr) = _checkInCodelets554(1, 1, this, codeletCounter);
        checkInCodelets554Ptr++;
        (*checkInCodelets555Ptr) = _checkInCodelets555(1, 1, this, codeletCounter);
        (*checkInCodelets555Ptr).decDep();
        checkInCodelets555Ptr++;
    }
}
TP552::~TP552()
{
    delete[] TP554_LoopCounterPerThread;
    delete[] barrierCodelets552;
    delete[] checkInCodelets557;
    delete[] checkInCodelets554;
    delete[] checkInCodelets555;
}
/*TP554: ForStmt*/
void TP554::_checkInCodelets559::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    /*Select the thread executing OMPSingleDirective 559*/
    if (!__sync_val_compare_and_swap(&(myTP->TP559_alreadyLaunched), 0, 1)) {
        /*Init the vars for this region*/

        /*printing node 560: CallExpr*/
        delay(delaylength);
        /*Signaling next codelet from last stmt in the codelet*/
        /*Signaling omp region's barrier*/
        myTP->controlTPParent->barrierCodelets559[0].decDep();
    } else {
        /*Signaling omp region's barrier*/
        myTP->barrierCodelets559[0].decDep();
    }
}
void TP554::_barrierCodelets559::fire(void)
{
    TP554* myTP = static_cast<TP554*>(myTP_);
    for (size_t codeletsCounter = 0; codeletsCounter < myTP->numThreads; codeletsCounter++) {
        myTP->TPParent->checkInCodelets557[codeletsCounter].decDep();
    }
}
TP554::TP554(int in_numThreads, int in_mainCodeletID, TP552* in_TPParent, TP552* in_inputsTPParent,
    TP554** in_ptrToThisTP)
    : ompTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(in_inputsTPParent)
    , ptrToThisTP(in_ptrToThisTP)
    , TP559_alreadyLaunched(0)
    , checkInCodelets559(new _checkInCodelets559[this->numThreads])
    , barrierCodelets559(new _barrierCodelets559[1])
{
    /*Initialize Codelets*/
    barrierCodelets559[0] = _barrierCodelets559(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets559* checkInCodelets559Ptr = (this->checkInCodelets559);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets559);
#endif
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets559Ptr) = _checkInCodelets559(2, 1, this, codeletCounter);
#else
        (*checkInCodelets559Ptr) = _checkInCodelets559(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets559Ptr).decDep();
        checkInCodelets559Ptr++;
    }
    *(this->ptrToThisTP) = this;
#if USE_SPIN_CODELETS == 0
    this->firstCodelet[this->getID()].decDep();
#else
    this->availableCodelets[this->getID()] = 1;
#endif
}
TP554::~TP554()
{
    delete[] barrierCodelets559;
    delete[] checkInCodelets559;
}
/*TP563: OMPParallelDirective*/
void TP563::_barrierCodelets563::fire(void)
{
    TP563* myTP = static_cast<TP563*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
void TP563::_checkInCodelets565::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 565: ForStmt*/
    {
        int* j = &(this->inputsTPParent->j_darts563[this->getLocalID()]);
        (void)j /*OMP_PRIVATE*/;
        /*Loop's init*/
        (this->inputsTPParent->j_darts563[this->getID()]) = 0;
        int j_darts_counter_temp563 = (this->inputsTPParent->j_darts563[this->getID()]);
        for (; (j_darts_counter_temp563) < innerreps / nthreads; (j_darts_counter_temp563)++) {
            TP571mutex.lock();
            delay(delaylength);
            TP571mutex.unlock();
        }
        (this->inputsTPParent->j_darts563[this->getID()]) = j_darts_counter_temp563;
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets563[0].decDep();
}
TP563::TP563(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , j_darts563(new int[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , barrierCodelets563(new _barrierCodelets563[1])
    , checkInCodelets565(new _checkInCodelets565[this->numThreads])
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets563[0] = _barrierCodelets563(ompNumThreads, ompNumThreads, this, 0);
    _checkInCodelets565* checkInCodelets565Ptr = (this->checkInCodelets565);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets565Ptr) = _checkInCodelets565(1, 1, this, codeletCounter);
        (*checkInCodelets565Ptr).decDep();
        checkInCodelets565Ptr++;
    }
}
TP563::~TP563()
{
    delete[] barrierCodelets563;
    delete[] checkInCodelets565;
}
/*TP576: OMPParallelDirective*/
void TP576::_barrierCodelets576::fire(void)
{
    TP576* myTP = static_cast<TP576*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
void TP576::_checkInCodelets578::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 578: ForStmt*/
    {
        int* j = &(this->inputsTPParent->j_darts576[this->getLocalID()]);
        (void)j /*OMP_PRIVATE*/;
        /*Loop's init*/
        (this->inputsTPParent->j_darts576[this->getID()]) = 0;
        int j_darts_counter_temp576 = (this->inputsTPParent->j_darts576[this->getID()]);
        for (; (j_darts_counter_temp576) < innerreps / nthreads; (j_darts_counter_temp576)++) {
            omp_set_lock(&omp_lock);
            delay(delaylength);
            omp_unset_lock(&omp_lock);
        }
        (this->inputsTPParent->j_darts576[this->getID()]) = j_darts_counter_temp576;
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets576[0].decDep();
}
TP576::TP576(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , j_darts576(new int[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , barrierCodelets576(new _barrierCodelets576[1])
    , checkInCodelets578(new _checkInCodelets578[this->numThreads])
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets576[0] = _barrierCodelets576(ompNumThreads, ompNumThreads, this, 0);
    _checkInCodelets578* checkInCodelets578Ptr = (this->checkInCodelets578);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets578Ptr) = _checkInCodelets578(1, 1, this, codeletCounter);
        (*checkInCodelets578Ptr).decDep();
        checkInCodelets578Ptr++;
    }
}
TP576::~TP576()
{
    delete[] barrierCodelets576;
    delete[] checkInCodelets578;
}
/*TP634: OMPParallelDirective*/
void TP634::_barrierCodelets634::fire(void)
{
    TP634* myTP = static_cast<TP634*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
void TP634::_checkInCodelets636::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 636: ForStmt*/
    {
        double* aaaa = (this->inputsTPParent->aaaa_darts634);
        (void)aaaa /*OMP_SHARED*/;
        double* b = &(this->inputsTPParent->b_darts634[this->getLocalID()]);
        (void)b /*OMP_FIRSTPRIVATE*/;
        double* c = (this->inputsTPParent->c_darts634);
        (void)c /*OMP_SHARED*/;
        int* j = &(this->inputsTPParent->j_darts634[this->getLocalID()]);
        (void)j /*OMP_PRIVATE*/;
        /*Loop's init*/
        (this->inputsTPParent->j_darts634[this->getID()]) = 0;
        int j_darts_counter_temp634 = (this->inputsTPParent->j_darts634[this->getID()]);
        for (; (j_darts_counter_temp634) < innerreps / nthreads; (j_darts_counter_temp634)++) {
            TP642mutex.lock();
            (*(aaaa)) += (*b);
            TP642mutex.unlock();
            (*b) *= (*(c));
        }
        (this->inputsTPParent->j_darts634[this->getID()]) = j_darts_counter_temp634;
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets634[0].decDep();
}
TP634::TP634(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet, double* in_aaaa,
    double* in_b_outer634_ptr, double* in_c)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , aaaa_darts634(in_aaaa) /*OMP_SHARED - INPUT*/
    , b_darts634(new double[this->numThreads]) /*OMP_FIRSTPRIVATE - INPUT*/
    , b_outer634_ptr(in_b_outer634_ptr)
    , c_darts634(in_c) /*OMP_SHARED - INPUT*/
    , j_darts634(new int[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , barrierCodelets634(new _barrierCodelets634[1])
    , checkInCodelets636(new _checkInCodelets636[this->numThreads])
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets634[0] = _barrierCodelets634(ompNumThreads, ompNumThreads, this, 0);
    _checkInCodelets636* checkInCodelets636Ptr = (this->checkInCodelets636);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        b_darts634[codeletCounter] = *in_b_outer634_ptr; /*OMP_FIRSTPRIVATE*/
        (*checkInCodelets636Ptr) = _checkInCodelets636(1, 1, this, codeletCounter);
        (*checkInCodelets636Ptr).decDep();
        checkInCodelets636Ptr++;
    }
}
TP634::~TP634()
{
    delete[] barrierCodelets634;
    delete[] checkInCodelets636;
}
/*TP660: OMPParallelDirective*/
void TP660::_barrierCodelets660::fire(void)
{
    TP660* myTP = static_cast<TP660*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
void TP660::_checkInCodelets662::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 662: CallExpr*/
    delay(delaylength);
    int aaaa_LocalReductionVar = (int)0;
    /*printing node 663: CompoundAssignOperator*/
    (*(&(aaaa_LocalReductionVar))) += 1;
updateReduceVars:
    /*Before signaling next codelet, check if all the threads in the TP
    * performed the reduction locally and if so, update the unique
    * reduction variable.*/
    this->inputsTPParent->aaaa_darts660_mutex.lock();
    *(this->inputsTPParent->aaaa_darts660) += (aaaa_LocalReductionVar);
    this->inputsTPParent->aaaa_darts660_mutex.unlock();
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets660[0].decDep();
}
TP660::TP660(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet, int* in_aaaa)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , aaaa_darts660(in_aaaa) /*OMP_SHARED - INPUT*/
    , barrierCodelets660(new _barrierCodelets660[1])
    , checkInCodelets662(new _checkInCodelets662[this->numThreads])
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets660[0] = _barrierCodelets660(ompNumThreads, ompNumThreads, this, 0);
    _checkInCodelets662* checkInCodelets662Ptr = (this->checkInCodelets662);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets662Ptr) = _checkInCodelets662(1, 1, this, codeletCounter);
        (*checkInCodelets662Ptr).decDep();
        checkInCodelets662Ptr++;
    }
}
TP660::~TP660()
{
    delete[] barrierCodelets660;
    delete[] checkInCodelets662;
}
