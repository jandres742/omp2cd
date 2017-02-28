#include "schedbench.output.darts.h"
using namespace darts;
using namespace std;
int cksz;
int itersperthr = 128;
char testName[32];
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
    /*CompoundStmt:399*/
    init(argc, argv);
    reference("reference time", &refer);
    benchmark("STATIC", &teststatic);
    cksz = 1;
    while (cksz <= itersperthr) {
        sprintf(testName, "STATIC %d", cksz);
        benchmark(testName, &teststaticn);
        cksz *= 2;
    }
    cksz = 1;
    while (cksz <= itersperthr) {
        sprintf(testName, "DYNAMIC %d", cksz);
        benchmark(testName, &testdynamicn);
        cksz *= 2;
    }
    cksz = 1;
    while (cksz <= itersperthr / nthreads) {
        sprintf(testName, "GUIDED %d", cksz);
        benchmark(testName, &testguidedn);
        cksz *= 2;
    }
    finalise();
    return 0;
}
/*Function: refer, ID: 2*/
void refer()
{
    /*refer:2*/
    /*CompoundStmt:432*/
    int i, j;
    for (j = 0; j < innerreps; j++) {
        for (i = 0; i < itersperthr; i++) {
            delay(delaylength);
        }
    }
}
/*Function: teststatic, ID: 3*/
void teststatic()
{
    /*teststatic:3*/
    /*CompoundStmt:445*/
    int i, j;
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP447>(
            ompNumThreads * DARTS_CODELETS_MULT, 0, RuntimeFinalCodelet, (int*)&((i))));
    }
}
/*Function: teststaticn, ID: 4*/
void teststaticn()
{
    /*teststaticn:4*/
    /*CompoundStmt:486*/
    int i, j;
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP488>(
            ompNumThreads * DARTS_CODELETS_MULT, 0, RuntimeFinalCodelet, (int*)&((i))));
    }
}
/*Function: testdynamicn, ID: 5*/
void testdynamicn()
{
    /*testdynamicn:5*/
    /*CompoundStmt:527*/
    int i, j;
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP529>(
            ompNumThreads * DARTS_CODELETS_MULT, 0, RuntimeFinalCodelet, (int*)&((i))));
    }
}
/*Function: testguidedn, ID: 6*/
void testguidedn()
{
    /*testguidedn:6*/
    /*CompoundStmt:568*/
    int i, j;
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP570>(
            ompNumThreads * DARTS_CODELETS_MULT, 0, RuntimeFinalCodelet, (int*)&((i))));
    }
}
/*TP447: OMPParallelDirective*/
void TP447::_barrierCodelets447::fire(void)
{
    TP447* myTP = static_cast<TP447*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
void TP447::_checkInCodelets450::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 450: BinaryOperator*/
    (this->inputsTPParent->j_darts447[this->getID()]) = 0;

    /*printing node 451: BinaryOperator*/
    /*Print the code for a condition node in a complex loop stmt */
    if ((this->inputsTPParent->j_darts447[this->getID()]) < innerreps) {
        /*Signal the first codelet in the loop*/
        myTP->checkInCodelets449[this->getID()].decDep();
        return;
    }
    /*Signal the codelet after the loop from the end's condional node.*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets447[0].decDep();
}
void TP447::_checkInCodelets449::fire(void)
{

    /*printing node 449: ForStmt*/
    bool haveToLaunch = __sync_bool_compare_and_swap(&(myTP->controlTPParent->TP449_LoopCounter),
        myTP->controlTPParent->TP449_LoopCounterPerThread[this->getID()],
        myTP->controlTPParent->TP449_LoopCounterPerThread[this->getID()] + 1);
    unsigned int iterIdx = myTP->controlTPParent->TP449_LoopCounterPerThread[this->getID()];
    if (haveToLaunch) {
        this->resetCodelet();
        myTP->controlTPParent->TP449PtrVec.push_back(nullptr);
        myTP->controlTPParent->TP449_LoopCounterPerThread[this->getID()] += 1;
        invoke<TP449>(myTP, myTP->numThreads, this->getID(), myTP, myTP->inputsTPParent,
            &(myTP->controlTPParent->TP449PtrVec.back()));
    } else {
        if (myTP->controlTPParent->TP449PtrVec.size() == 0) {
            this->resetCodelet();
            this->decDep();
            return;
        } else if (myTP->controlTPParent->TP449PtrVec.size() < (iterIdx + 1)) {
            this->resetCodelet();
            this->decDep();
            return;
        } else if (myTP->controlTPParent->TP449PtrVec[iterIdx] == nullptr) {
            this->resetCodelet();
            this->decDep();
            return;
        } else {
            this->resetCodelet();
#if USE_SPIN_CODELETS == 0
            myTP->controlTPParent->TP449PtrVec[iterIdx]->firstCodelet[this->getID()].decDep();
#else
            myTP->controlTPParent->TP449PtrVec[iterIdx]->availableCodelets[this->getID()] = 1;
#endif
            myTP->controlTPParent->TP449_LoopCounterPerThread[this->getID()] += 1;
        }
    }
}
void TP447::_checkInCodelets452::fire(void)
{

    /*printing node 452: UnaryOperator*/
    (this->inputsTPParent->j_darts447[this->getID()])++;

    /*printing node 611: BinaryOperator*/
    /*Print the code for a condition node in a complex loop stmt */
    if ((this->inputsTPParent->j_darts447[this->getID()]) < innerreps) {
        this->resetCodelet();
        /*Signal the first codelet in the loop*/
        myTP->checkInCodelets449[this->getID()].decDep();
        return;
    }
    /*Signal the codelet after the loop from the condtional node.*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets447[0].decDep();
}
TP447::TP447(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet, int* in_i)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , i_darts447(in_i) /*OMP_SHARED - INPUT*/
    , j_darts447(new int[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , TP449_LoopCounter(0)
    , TP449_LoopCounterPerThread(new unsigned int[this->numThreads])
    , barrierCodelets447(new _barrierCodelets447[1])
    , checkInCodelets450(new _checkInCodelets450[this->numThreads])
    , checkInCodelets449(new _checkInCodelets449[this->numThreads])
    , checkInCodelets452(new _checkInCodelets452[this->numThreads])
{
    memset((void*)TP449_LoopCounterPerThread, 0, this->numThreads * sizeof(unsigned int));
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets447[0] = _barrierCodelets447(ompNumThreads, ompNumThreads, this, 0);
    _checkInCodelets452* checkInCodelets452Ptr = (this->checkInCodelets452);
    _checkInCodelets449* checkInCodelets449Ptr = (this->checkInCodelets449);
    _checkInCodelets450* checkInCodelets450Ptr = (this->checkInCodelets450);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets452Ptr) = _checkInCodelets452(1, 1, this, codeletCounter);
        checkInCodelets452Ptr++;
        (*checkInCodelets449Ptr) = _checkInCodelets449(1, 1, this, codeletCounter);
        checkInCodelets449Ptr++;
        (*checkInCodelets450Ptr) = _checkInCodelets450(1, 1, this, codeletCounter);
        (*checkInCodelets450Ptr).decDep();
        checkInCodelets450Ptr++;
    }
}
TP447::~TP447()
{
    delete[] TP449_LoopCounterPerThread;
    delete[] barrierCodelets447;
    delete[] checkInCodelets452;
    delete[] checkInCodelets449;
    delete[] checkInCodelets450;
}
/*TP449: ForStmt*/
void TP449::_checkInCodelets454::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    /*printOMPForCodeletFireCode, inlined: 0*/
    /*region 454 0*/
    /*Determine the TP to which this codelet belongs and check if this codelet spawns the TP or if
     * it signals it using dispatchCodelet()*/
    size_t idx = this->getID() / myTP->codeletsPerTP454;
    if (idx < myTP->TPsToUse454) {
        if (!__sync_val_compare_and_swap(&(myTP->TP454_alreadyLaunched[idx]), 0, 1)) {
            int range = abs(itersperthr * nthreads - 0) / 1;
            int rangePerCodelet = range / myTP->TPsToUse454;
            int minIteration = min<int>(itersperthr * nthreads, 0);
            int remainderRange = range % myTP->TPsToUse454;
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
            if (0 < itersperthr * nthreads) {
                initIteration = min(initIteration, lastIteration);
                lastIteration = max(initIteration, lastIteration);
            } else {
                initIteration = max(initIteration, lastIteration);
                lastIteration = min(initIteration, lastIteration);
            }
            if (idx == myTP->TPsToUse454 - 1) {
                lastIteration = itersperthr * nthreads;
            }
#if USEINVOKE == 1
            invoke<TP454>(myTP, myTP->codeletsPerTP454 * DARTS_CODELETS_MULT, this->getID(), myTP,
                initIteration, lastIteration, &(myTP->TP454Ptr[idx]));
#else
            place<TP454>(idx, myTP, myTP->codeletsPerTP454 * DARTS_CODELETS_MULT, this->getID(),
                myTP, initIteration, lastIteration, &(myTP->TP454Ptr[idx]));
#endif
        } else {
            if (myTP->TP454Ptr[idx] != nullptr) {
                myTP->TP454Ptr[idx]->dispatchCodelet(this->getID());
            } else {
                this->resetCodelet();
                this->decDep();
            }
        }
    }
}
void TP449::_barrierCodelets454::fire(void)
{
    TP449* myTP = static_cast<TP449*>(myTP_);
    for (size_t codeletsCounter = 0; codeletsCounter < myTP->numThreads; codeletsCounter++) {
        myTP->TPParent->checkInCodelets452[codeletsCounter].decDep();
    }
}
TP449::TP449(int in_numThreads, int in_mainCodeletID, TP447* in_TPParent, TP447* in_inputsTPParent,
    TP449** in_ptrToThisTP)
    : ompTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(in_inputsTPParent)
    , ptrToThisTP(in_ptrToThisTP)
    , TP454Ptr(new TP454*[NUMTPS454])
    , TP454_alreadyLaunched(new size_t[NUMTPS454])
    , numTPsSet454(0)
    , numTPsReady454(0)
    , TPsToUse454(NUMTPS454)
    , codeletsPerTP454(this->numThreads / NUMTPS454)
    , totalCodelets454(this->TPsToUse454 * this->codeletsPerTP454)
    , checkInCodelets454(new _checkInCodelets454[this->numThreads])
    , barrierCodelets454(new _barrierCodelets454[1])
{
    /*Initialize Codelets*/
    barrierCodelets454[0] = _barrierCodelets454(NUMTPS454, NUMTPS454, this, 0);
    _checkInCodelets454* checkInCodelets454Ptr = (this->checkInCodelets454);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets454);
#endif
    for (int i = 0; i < NUMTPS454; i++) {
        TP454Ptr[i] = nullptr;
        TP454_alreadyLaunched[i] = 0;
    }
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets454Ptr) = _checkInCodelets454(2, 1, this, codeletCounter);
#else
        (*checkInCodelets454Ptr) = _checkInCodelets454(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets454Ptr).decDep();
        checkInCodelets454Ptr++;
    }
    *(this->ptrToThisTP) = this;
#if USE_SPIN_CODELETS == 0
    this->firstCodelet[this->getID()].decDep();
#else
    this->availableCodelets[this->getID()] = 1;
#endif
}
TP449::~TP449()
{
    delete[] barrierCodelets454;
    delete[] checkInCodelets454;
}
/*TP454: OMPForDirective*/
void TP454::_barrierCodelets454::fire(void)
{
    TP454* myTP = static_cast<TP454*>(myTP_);
    myTP->controlTPParent->TPParent->barrierCodelets454[0].decDep();
}
bool TP454::requestNewRangeIterations454(int* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Static */
    /*Chunk = 1*/
    bool isThereNewIteration = false;
    {
        /*Static Scheduling*/
        int tempStartRange = rangePerCodelet454 * codeletID;
        int tempEndRange = rangePerCodelet454 * (codeletID + 1);
        if (remainderRange454 != 0) {
            if (codeletID < (uint32_t)remainderRange454) {
                tempStartRange += codeletID;
                tempEndRange += (codeletID + 1);
            } else {
                tempStartRange += remainderRange454;
                tempEndRange += remainderRange454;
            }
        }
        tempStartRange = tempStartRange * 1 + minIteration454;
        tempEndRange = tempEndRange * 1 + minIteration454;
        if (tempStartRange != tempEndRange) {
            isThereNewIteration = true;
        }
        if (initIteration454 < lastIteration454) {
            (this->inputsTPParent->i_darts454[codeletID]) = min(tempStartRange, tempEndRange);
            *endRange = max(tempStartRange, tempEndRange);
        } else {
            (this->inputsTPParent->i_darts454[codeletID]) = max(tempStartRange, tempEndRange);
            *endRange = min(tempStartRange, tempEndRange);
        }
        if (codeletID == this->numThreads - 1) {
            *endRange = lastIteration454;
        }
    }
    return isThereNewIteration;
}
void TP454::_checkInCodelets455::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    /*Init the vars for this region*/

    /*printing node 455: ForStmt*/
    /*var: i*/
    int* i = &(this->inputsTPParent->i_darts454[this->getLocalID()]);
    (void)i /*OMP_PRIVATE*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations454(
        (int*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        /*Signaling omp for stmt's barrier*/
        myTP->controlTPParent->barrierCodelets454[0].decDep();
        return;
    }
    for (int i_darts_counter_temp454 = (*i); i_darts_counter_temp454 < endRange
         && i_darts_counter_temp454 < this->inputsTPParent->lastIteration454;
         i_darts_counter_temp454++) {
        {
            delay(delaylength);
        }
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling omp for stmt's barrier*/
    myTP->controlTPParent->barrierCodelets454[0].decDep();
}
TP454::TP454(int in_numThreads, int in_mainCodeletID, TP449* in_TPParent, int in_initIteration,
    int in_lastIteration, TP454** in_ptrToThisTP)
    : ompTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , i_darts454(new int[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , initIteration454(in_initIteration)
    , lastIteration454(in_lastIteration)
    , readyCodelets(0)
    , baseNumThreads(this->numThreads / DARTS_CODELETS_MULT)
    , barrierCodelets454(new _barrierCodelets454[1])
    , checkInCodelets455(new _checkInCodelets455[this->numThreads])
{
    /*Initialize the loop parameters*/
    range454 = abs(lastIteration454 - initIteration454) / 1;
    rangePerCodelet454 = range454 / numThreads;
    minIteration454 = min<int>(lastIteration454, initIteration454);
    remainderRange454 = range454 % numThreads;
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets454[0] = _barrierCodelets454(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets455* checkInCodelets455Ptr = (this->checkInCodelets455);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets455);
#endif
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->baseNumThreads;
         codeletCounter++) {
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets455Ptr) = _checkInCodelets455(2, 1, this, codeletCounter);
#else
        (*checkInCodelets455Ptr) = _checkInCodelets455(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets455Ptr).decDep();
        checkInCodelets455Ptr++;
    }
    this->dispatchCodelet(this->getID());
    *(in_ptrToThisTP) = this;
}
void TP454::dispatchCodelet(size_t codeletID)
{
    int idx = codeletID / this->baseNumThreads;
    int localID = codeletID - this->baseNumThreads * idx;
    this->checkInCodelets455[localID].setID(codeletID);
    this->checkInCodelets455[localID].setLocalID(localID);
    /*Check if we want to replicate codelets*/
    for (size_t i = 1; i < (size_t)DARTS_CODELETS_MULT; i++) {
#if USE_SPIN_CODELETS == 0
        this->checkInCodelets455[localID + this->baseNumThreads * i]
            = _checkInCodelets455(2, 1, this, localID + this->baseNumThreads * i);
#else
        this->checkInCodelets455[localID + this->baseNumThreads * i]
            = _checkInCodelets455(1, 1, this, localID + this->baseNumThreads * i);
#endif
        this->checkInCodelets455[localID + this->baseNumThreads * i].setID(codeletID);
        this->checkInCodelets455[localID + this->baseNumThreads * i].decDep();
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
TP454::~TP454()
{
    delete[] barrierCodelets454;
    delete[] checkInCodelets455;
}
/*TP488: OMPParallelDirective*/
void TP488::_barrierCodelets488::fire(void)
{
    TP488* myTP = static_cast<TP488*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
void TP488::_checkInCodelets491::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 491: BinaryOperator*/
    (this->inputsTPParent->j_darts488[this->getID()]) = 0;

    /*printing node 492: BinaryOperator*/
    /*Print the code for a condition node in a complex loop stmt */
    if ((this->inputsTPParent->j_darts488[this->getID()]) < innerreps) {
        /*Signal the first codelet in the loop*/
        myTP->checkInCodelets490[this->getID()].decDep();
        return;
    }
    /*Signal the codelet after the loop from the end's condional node.*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets488[0].decDep();
}
void TP488::_checkInCodelets490::fire(void)
{

    /*printing node 490: ForStmt*/
    bool haveToLaunch = __sync_bool_compare_and_swap(&(myTP->controlTPParent->TP490_LoopCounter),
        myTP->controlTPParent->TP490_LoopCounterPerThread[this->getID()],
        myTP->controlTPParent->TP490_LoopCounterPerThread[this->getID()] + 1);
    unsigned int iterIdx = myTP->controlTPParent->TP490_LoopCounterPerThread[this->getID()];
    if (haveToLaunch) {
        this->resetCodelet();
        myTP->controlTPParent->TP490PtrVec.push_back(nullptr);
        myTP->controlTPParent->TP490_LoopCounterPerThread[this->getID()] += 1;
        invoke<TP490>(myTP, myTP->numThreads, this->getID(), myTP, myTP->inputsTPParent,
            &(myTP->controlTPParent->TP490PtrVec.back()));
    } else {
        if (myTP->controlTPParent->TP490PtrVec.size() == 0) {
            this->resetCodelet();
            this->decDep();
            return;
        } else if (myTP->controlTPParent->TP490PtrVec.size() < (iterIdx + 1)) {
            this->resetCodelet();
            this->decDep();
            return;
        } else if (myTP->controlTPParent->TP490PtrVec[iterIdx] == nullptr) {
            this->resetCodelet();
            this->decDep();
            return;
        } else {
            this->resetCodelet();
#if USE_SPIN_CODELETS == 0
            myTP->controlTPParent->TP490PtrVec[iterIdx]->firstCodelet[this->getID()].decDep();
#else
            myTP->controlTPParent->TP490PtrVec[iterIdx]->availableCodelets[this->getID()] = 1;
#endif
            myTP->controlTPParent->TP490_LoopCounterPerThread[this->getID()] += 1;
        }
    }
}
void TP488::_checkInCodelets493::fire(void)
{

    /*printing node 493: UnaryOperator*/
    (this->inputsTPParent->j_darts488[this->getID()])++;

    /*printing node 610: BinaryOperator*/
    /*Print the code for a condition node in a complex loop stmt */
    if ((this->inputsTPParent->j_darts488[this->getID()]) < innerreps) {
        this->resetCodelet();
        /*Signal the first codelet in the loop*/
        myTP->checkInCodelets490[this->getID()].decDep();
        return;
    }
    /*Signal the codelet after the loop from the condtional node.*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets488[0].decDep();
}
TP488::TP488(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet, int* in_i)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , i_darts488(in_i) /*OMP_SHARED - INPUT*/
    , j_darts488(new int[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , TP490_LoopCounter(0)
    , TP490_LoopCounterPerThread(new unsigned int[this->numThreads])
    , barrierCodelets488(new _barrierCodelets488[1])
    , checkInCodelets491(new _checkInCodelets491[this->numThreads])
    , checkInCodelets490(new _checkInCodelets490[this->numThreads])
    , checkInCodelets493(new _checkInCodelets493[this->numThreads])
{
    memset((void*)TP490_LoopCounterPerThread, 0, this->numThreads * sizeof(unsigned int));
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets488[0] = _barrierCodelets488(ompNumThreads, ompNumThreads, this, 0);
    _checkInCodelets493* checkInCodelets493Ptr = (this->checkInCodelets493);
    _checkInCodelets490* checkInCodelets490Ptr = (this->checkInCodelets490);
    _checkInCodelets491* checkInCodelets491Ptr = (this->checkInCodelets491);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets493Ptr) = _checkInCodelets493(1, 1, this, codeletCounter);
        checkInCodelets493Ptr++;
        (*checkInCodelets490Ptr) = _checkInCodelets490(1, 1, this, codeletCounter);
        checkInCodelets490Ptr++;
        (*checkInCodelets491Ptr) = _checkInCodelets491(1, 1, this, codeletCounter);
        (*checkInCodelets491Ptr).decDep();
        checkInCodelets491Ptr++;
    }
}
TP488::~TP488()
{
    delete[] TP490_LoopCounterPerThread;
    delete[] barrierCodelets488;
    delete[] checkInCodelets493;
    delete[] checkInCodelets490;
    delete[] checkInCodelets491;
}
/*TP490: ForStmt*/
void TP490::_checkInCodelets495::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    /*printOMPForCodeletFireCode, inlined: 0*/
    /*region 495 0*/
    /*Determine the TP to which this codelet belongs and check if this codelet spawns the TP or if
     * it signals it using dispatchCodelet()*/
    size_t idx = this->getID() / myTP->codeletsPerTP495;
    if (idx < myTP->TPsToUse495) {
        if (!__sync_val_compare_and_swap(&(myTP->TP495_alreadyLaunched[idx]), 0, 1)) {
            int range = abs(itersperthr * nthreads - 0) / 1;
            int rangePerCodelet = range / myTP->TPsToUse495;
            int minIteration = min<int>(itersperthr * nthreads, 0);
            int remainderRange = range % myTP->TPsToUse495;
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
            if (0 < itersperthr * nthreads) {
                initIteration = min(initIteration, lastIteration);
                lastIteration = max(initIteration, lastIteration);
            } else {
                initIteration = max(initIteration, lastIteration);
                lastIteration = min(initIteration, lastIteration);
            }
            if (idx == myTP->TPsToUse495 - 1) {
                lastIteration = itersperthr * nthreads;
            }
#if USEINVOKE == 1
            invoke<TP495>(myTP, myTP->codeletsPerTP495 * DARTS_CODELETS_MULT, this->getID(), myTP,
                initIteration, lastIteration, &(myTP->TP495Ptr[idx]));
#else
            place<TP495>(idx, myTP, myTP->codeletsPerTP495 * DARTS_CODELETS_MULT, this->getID(),
                myTP, initIteration, lastIteration, &(myTP->TP495Ptr[idx]));
#endif
        } else {
            if (myTP->TP495Ptr[idx] != nullptr) {
                myTP->TP495Ptr[idx]->dispatchCodelet(this->getID());
            } else {
                this->resetCodelet();
                this->decDep();
            }
        }
    }
}
void TP490::_barrierCodelets495::fire(void)
{
    TP490* myTP = static_cast<TP490*>(myTP_);
    for (size_t codeletsCounter = 0; codeletsCounter < myTP->numThreads; codeletsCounter++) {
        myTP->TPParent->checkInCodelets493[codeletsCounter].decDep();
    }
}
TP490::TP490(int in_numThreads, int in_mainCodeletID, TP488* in_TPParent, TP488* in_inputsTPParent,
    TP490** in_ptrToThisTP)
    : ompTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(in_inputsTPParent)
    , ptrToThisTP(in_ptrToThisTP)
    , TP495Ptr(new TP495*[NUMTPS495])
    , TP495_alreadyLaunched(new size_t[NUMTPS495])
    , numTPsSet495(0)
    , numTPsReady495(0)
    , TPsToUse495(NUMTPS495)
    , codeletsPerTP495(this->numThreads / NUMTPS495)
    , totalCodelets495(this->TPsToUse495 * this->codeletsPerTP495)
    , checkInCodelets495(new _checkInCodelets495[this->numThreads])
    , barrierCodelets495(new _barrierCodelets495[1])
{
    /*Initialize Codelets*/
    barrierCodelets495[0] = _barrierCodelets495(NUMTPS495, NUMTPS495, this, 0);
    _checkInCodelets495* checkInCodelets495Ptr = (this->checkInCodelets495);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets495);
#endif
    for (int i = 0; i < NUMTPS495; i++) {
        TP495Ptr[i] = nullptr;
        TP495_alreadyLaunched[i] = 0;
    }
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets495Ptr) = _checkInCodelets495(2, 1, this, codeletCounter);
#else
        (*checkInCodelets495Ptr) = _checkInCodelets495(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets495Ptr).decDep();
        checkInCodelets495Ptr++;
    }
    *(this->ptrToThisTP) = this;
#if USE_SPIN_CODELETS == 0
    this->firstCodelet[this->getID()].decDep();
#else
    this->availableCodelets[this->getID()] = 1;
#endif
}
TP490::~TP490()
{
    delete[] barrierCodelets495;
    delete[] checkInCodelets495;
}
/*TP495: OMPForDirective*/
void TP495::_barrierCodelets495::fire(void)
{
    TP495* myTP = static_cast<TP495*>(myTP_);
    myTP->controlTPParent->TPParent->barrierCodelets495[0].decDep();
}
bool TP495::requestNewRangeIterations495(int* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Static */
    /*Chunk = cksz*/
    bool isThereNewIteration = false;
    {
        /*Static Scheduling*/
        int tempStartRange = rangePerCodelet495 * codeletID;
        int tempEndRange = rangePerCodelet495 * (codeletID + 1);
        if (remainderRange495 != 0) {
            if (codeletID < (uint32_t)remainderRange495) {
                tempStartRange += codeletID;
                tempEndRange += (codeletID + 1);
            } else {
                tempStartRange += remainderRange495;
                tempEndRange += remainderRange495;
            }
        }
        tempStartRange = tempStartRange * 1 + minIteration495;
        tempEndRange = tempEndRange * 1 + minIteration495;
        if (tempStartRange != tempEndRange) {
            isThereNewIteration = true;
        }
        if (initIteration495 < lastIteration495) {
            (this->inputsTPParent->i_darts495[codeletID]) = min(tempStartRange, tempEndRange);
            *endRange = max(tempStartRange, tempEndRange);
        } else {
            (this->inputsTPParent->i_darts495[codeletID]) = max(tempStartRange, tempEndRange);
            *endRange = min(tempStartRange, tempEndRange);
        }
        if (codeletID == this->numThreads - 1) {
            *endRange = lastIteration495;
        }
    }
    return isThereNewIteration;
}
void TP495::_checkInCodelets496::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    /*Init the vars for this region*/

    /*printing node 496: ForStmt*/
    /*var: i*/
    int* i = &(this->inputsTPParent->i_darts495[this->getLocalID()]);
    (void)i /*OMP_PRIVATE*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations495(
        (int*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        /*Signaling omp for stmt's barrier*/
        myTP->controlTPParent->barrierCodelets495[0].decDep();
        return;
    }
    for (int i_darts_counter_temp495 = (*i); i_darts_counter_temp495 < endRange
         && i_darts_counter_temp495 < this->inputsTPParent->lastIteration495;
         i_darts_counter_temp495++) {
        {
            delay(delaylength);
        }
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling omp for stmt's barrier*/
    myTP->controlTPParent->barrierCodelets495[0].decDep();
}
TP495::TP495(int in_numThreads, int in_mainCodeletID, TP490* in_TPParent, int in_initIteration,
    int in_lastIteration, TP495** in_ptrToThisTP)
    : ompTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , i_darts495(new int[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , initIteration495(in_initIteration)
    , lastIteration495(in_lastIteration)
    , readyCodelets(0)
    , baseNumThreads(this->numThreads / DARTS_CODELETS_MULT)
    , barrierCodelets495(new _barrierCodelets495[1])
    , checkInCodelets496(new _checkInCodelets496[this->numThreads])
{
    /*Initialize the loop parameters*/
    range495 = abs(lastIteration495 - initIteration495) / 1;
    rangePerCodelet495 = range495 / numThreads;
    minIteration495 = min<int>(lastIteration495, initIteration495);
    remainderRange495 = range495 % numThreads;
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets495[0] = _barrierCodelets495(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets496* checkInCodelets496Ptr = (this->checkInCodelets496);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets496);
#endif
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->baseNumThreads;
         codeletCounter++) {
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets496Ptr) = _checkInCodelets496(2, 1, this, codeletCounter);
#else
        (*checkInCodelets496Ptr) = _checkInCodelets496(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets496Ptr).decDep();
        checkInCodelets496Ptr++;
    }
    this->dispatchCodelet(this->getID());
    *(in_ptrToThisTP) = this;
}
void TP495::dispatchCodelet(size_t codeletID)
{
    int idx = codeletID / this->baseNumThreads;
    int localID = codeletID - this->baseNumThreads * idx;
    this->checkInCodelets496[localID].setID(codeletID);
    this->checkInCodelets496[localID].setLocalID(localID);
    /*Check if we want to replicate codelets*/
    for (size_t i = 1; i < (size_t)DARTS_CODELETS_MULT; i++) {
#if USE_SPIN_CODELETS == 0
        this->checkInCodelets496[localID + this->baseNumThreads * i]
            = _checkInCodelets496(2, 1, this, localID + this->baseNumThreads * i);
#else
        this->checkInCodelets496[localID + this->baseNumThreads * i]
            = _checkInCodelets496(1, 1, this, localID + this->baseNumThreads * i);
#endif
        this->checkInCodelets496[localID + this->baseNumThreads * i].setID(codeletID);
        this->checkInCodelets496[localID + this->baseNumThreads * i].decDep();
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
TP495::~TP495()
{
    delete[] barrierCodelets495;
    delete[] checkInCodelets496;
}
/*TP529: OMPParallelDirective*/
void TP529::_barrierCodelets529::fire(void)
{
    TP529* myTP = static_cast<TP529*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
void TP529::_checkInCodelets532::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 532: BinaryOperator*/
    (this->inputsTPParent->j_darts529[this->getID()]) = 0;

    /*printing node 533: BinaryOperator*/
    /*Print the code for a condition node in a complex loop stmt */
    if ((this->inputsTPParent->j_darts529[this->getID()]) < innerreps) {
        /*Signal the first codelet in the loop*/
        myTP->checkInCodelets531[this->getID()].decDep();
        return;
    }
    /*Signal the codelet after the loop from the end's condional node.*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets529[0].decDep();
}
void TP529::_checkInCodelets531::fire(void)
{

    /*printing node 531: ForStmt*/
    bool haveToLaunch = __sync_bool_compare_and_swap(&(myTP->controlTPParent->TP531_LoopCounter),
        myTP->controlTPParent->TP531_LoopCounterPerThread[this->getID()],
        myTP->controlTPParent->TP531_LoopCounterPerThread[this->getID()] + 1);
    unsigned int iterIdx = myTP->controlTPParent->TP531_LoopCounterPerThread[this->getID()];
    if (haveToLaunch) {
        this->resetCodelet();
        myTP->controlTPParent->TP531PtrVec.push_back(nullptr);
        myTP->controlTPParent->TP531_LoopCounterPerThread[this->getID()] += 1;
        invoke<TP531>(myTP, myTP->numThreads, this->getID(), myTP, myTP->inputsTPParent,
            &(myTP->controlTPParent->TP531PtrVec.back()));
    } else {
        if (myTP->controlTPParent->TP531PtrVec.size() == 0) {
            this->resetCodelet();
            this->decDep();
            return;
        } else if (myTP->controlTPParent->TP531PtrVec.size() < (iterIdx + 1)) {
            this->resetCodelet();
            this->decDep();
            return;
        } else if (myTP->controlTPParent->TP531PtrVec[iterIdx] == nullptr) {
            this->resetCodelet();
            this->decDep();
            return;
        } else {
            this->resetCodelet();
#if USE_SPIN_CODELETS == 0
            myTP->controlTPParent->TP531PtrVec[iterIdx]->firstCodelet[this->getID()].decDep();
#else
            myTP->controlTPParent->TP531PtrVec[iterIdx]->availableCodelets[this->getID()] = 1;
#endif
            myTP->controlTPParent->TP531_LoopCounterPerThread[this->getID()] += 1;
        }
    }
}
void TP529::_checkInCodelets534::fire(void)
{

    /*printing node 534: UnaryOperator*/
    (this->inputsTPParent->j_darts529[this->getID()])++;

    /*printing node 609: BinaryOperator*/
    /*Print the code for a condition node in a complex loop stmt */
    if ((this->inputsTPParent->j_darts529[this->getID()]) < innerreps) {
        this->resetCodelet();
        /*Signal the first codelet in the loop*/
        myTP->checkInCodelets531[this->getID()].decDep();
        return;
    }
    /*Signal the codelet after the loop from the condtional node.*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets529[0].decDep();
}
TP529::TP529(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet, int* in_i)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , i_darts529(in_i) /*OMP_SHARED - INPUT*/
    , j_darts529(new int[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , TP531_LoopCounter(0)
    , TP531_LoopCounterPerThread(new unsigned int[this->numThreads])
    , barrierCodelets529(new _barrierCodelets529[1])
    , checkInCodelets532(new _checkInCodelets532[this->numThreads])
    , checkInCodelets531(new _checkInCodelets531[this->numThreads])
    , checkInCodelets534(new _checkInCodelets534[this->numThreads])
{
    memset((void*)TP531_LoopCounterPerThread, 0, this->numThreads * sizeof(unsigned int));
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets529[0] = _barrierCodelets529(ompNumThreads, ompNumThreads, this, 0);
    _checkInCodelets534* checkInCodelets534Ptr = (this->checkInCodelets534);
    _checkInCodelets531* checkInCodelets531Ptr = (this->checkInCodelets531);
    _checkInCodelets532* checkInCodelets532Ptr = (this->checkInCodelets532);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets534Ptr) = _checkInCodelets534(1, 1, this, codeletCounter);
        checkInCodelets534Ptr++;
        (*checkInCodelets531Ptr) = _checkInCodelets531(1, 1, this, codeletCounter);
        checkInCodelets531Ptr++;
        (*checkInCodelets532Ptr) = _checkInCodelets532(1, 1, this, codeletCounter);
        (*checkInCodelets532Ptr).decDep();
        checkInCodelets532Ptr++;
    }
}
TP529::~TP529()
{
    delete[] TP531_LoopCounterPerThread;
    delete[] barrierCodelets529;
    delete[] checkInCodelets534;
    delete[] checkInCodelets531;
    delete[] checkInCodelets532;
}
/*TP531: ForStmt*/
void TP531::_checkInCodelets536::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    /*printOMPForCodeletFireCode, inlined: 0*/
    /*region 536 0*/
    /*Determine the TP to which this codelet belongs and check if this codelet spawns the TP or if
     * it signals it using dispatchCodelet()*/
    size_t idx = this->getID() / myTP->codeletsPerTP536;
    if (idx < myTP->TPsToUse536) {
        if (!__sync_val_compare_and_swap(&(myTP->TP536_alreadyLaunched[idx]), 0, 1)) {
            int range = abs(itersperthr * nthreads - 0) / 1;
            int rangePerCodelet = range / myTP->TPsToUse536;
            int minIteration = min<int>(itersperthr * nthreads, 0);
            int remainderRange = range % myTP->TPsToUse536;
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
            if (0 < itersperthr * nthreads) {
                initIteration = min(initIteration, lastIteration);
                lastIteration = max(initIteration, lastIteration);
            } else {
                initIteration = max(initIteration, lastIteration);
                lastIteration = min(initIteration, lastIteration);
            }
            if (idx == myTP->TPsToUse536 - 1) {
                lastIteration = itersperthr * nthreads;
            }
#if USEINVOKE == 1
            invoke<TP536>(myTP, myTP->codeletsPerTP536 * DARTS_CODELETS_MULT, this->getID(), myTP,
                initIteration, lastIteration, &(myTP->TP536Ptr[idx]));
#else
            place<TP536>(idx, myTP, myTP->codeletsPerTP536 * DARTS_CODELETS_MULT, this->getID(),
                myTP, initIteration, lastIteration, &(myTP->TP536Ptr[idx]));
#endif
        } else {
            if (myTP->TP536Ptr[idx] != nullptr) {
                myTP->TP536Ptr[idx]->dispatchCodelet(this->getID());
            } else {
                this->resetCodelet();
                this->decDep();
            }
        }
    }
}
void TP531::_barrierCodelets536::fire(void)
{
    TP531* myTP = static_cast<TP531*>(myTP_);
    for (size_t codeletsCounter = 0; codeletsCounter < myTP->numThreads; codeletsCounter++) {
        myTP->TPParent->checkInCodelets534[codeletsCounter].decDep();
    }
}
TP531::TP531(int in_numThreads, int in_mainCodeletID, TP529* in_TPParent, TP529* in_inputsTPParent,
    TP531** in_ptrToThisTP)
    : ompTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(in_inputsTPParent)
    , ptrToThisTP(in_ptrToThisTP)
    , TP536Ptr(new TP536*[NUMTPS536])
    , TP536_alreadyLaunched(new size_t[NUMTPS536])
    , numTPsSet536(0)
    , numTPsReady536(0)
    , TPsToUse536(NUMTPS536)
    , codeletsPerTP536(this->numThreads / NUMTPS536)
    , totalCodelets536(this->TPsToUse536 * this->codeletsPerTP536)
    , checkInCodelets536(new _checkInCodelets536[this->numThreads])
    , barrierCodelets536(new _barrierCodelets536[1])
{
    /*Initialize Codelets*/
    barrierCodelets536[0] = _barrierCodelets536(NUMTPS536, NUMTPS536, this, 0);
    _checkInCodelets536* checkInCodelets536Ptr = (this->checkInCodelets536);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets536);
#endif
    for (int i = 0; i < NUMTPS536; i++) {
        TP536Ptr[i] = nullptr;
        TP536_alreadyLaunched[i] = 0;
    }
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets536Ptr) = _checkInCodelets536(2, 1, this, codeletCounter);
#else
        (*checkInCodelets536Ptr) = _checkInCodelets536(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets536Ptr).decDep();
        checkInCodelets536Ptr++;
    }
    *(this->ptrToThisTP) = this;
#if USE_SPIN_CODELETS == 0
    this->firstCodelet[this->getID()].decDep();
#else
    this->availableCodelets[this->getID()] = 1;
#endif
}
TP531::~TP531()
{
    delete[] barrierCodelets536;
    delete[] checkInCodelets536;
}
/*TP536: OMPForDirective*/
void TP536::_barrierCodelets536::fire(void)
{
    TP536* myTP = static_cast<TP536*>(myTP_);
    myTP->controlTPParent->TPParent->barrierCodelets536[0].decDep();
}
bool TP536::requestNewRangeIterations536(int* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Dynamic */
    /*Chunk = cksz*/
    bool isThereNewIteration = false;
    {
        /*Dynamic Scheduling*/
        if (initIteration536 < lastIteration536) {
            (this->inputsTPParent->i_darts536[codeletID])
                = __sync_fetch_and_add(&(nextIteration536), cksz * 1);
            *endRange = (this->inputsTPParent->i_darts536[codeletID]) + cksz * 1;
        } else {
            (this->inputsTPParent->i_darts536[codeletID])
                = __sync_fetch_and_sub(&(nextIteration536), cksz * 1);
            *endRange = (this->inputsTPParent->i_darts536[codeletID]) - cksz * 1;
        }
        if ((this->inputsTPParent->i_darts536[codeletID]) < lastIteration536) {
            isThereNewIteration = true;
        }
    }
    return isThereNewIteration;
}
void TP536::_checkInCodelets537::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    /*Init the vars for this region*/

    /*printing node 537: ForStmt*/
    /*var: i*/
    int* i = &(this->inputsTPParent->i_darts536[this->getLocalID()]);
    (void)i /*OMP_PRIVATE*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations536(
        (int*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        /*Signaling omp for stmt's barrier*/
        myTP->controlTPParent->barrierCodelets536[0].decDep();
        return;
    }
    while (isThereNewIteration) {
        for (int i_darts_counter_temp536 = (*i); i_darts_counter_temp536 < endRange
             && i_darts_counter_temp536 < this->inputsTPParent->lastIteration536;
             i_darts_counter_temp536++) {
            {
                delay(delaylength);
            }
        }
        isThereNewIteration = this->inputsTPParent->requestNewRangeIterations536(
            &(this->endRange), this->getLocalID());
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling omp for stmt's barrier*/
    myTP->controlTPParent->barrierCodelets536[0].decDep();
}
TP536::TP536(int in_numThreads, int in_mainCodeletID, TP531* in_TPParent, int in_initIteration,
    int in_lastIteration, TP536** in_ptrToThisTP)
    : ompTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , i_darts536(new int[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , initIteration536(in_initIteration)
    , lastIteration536(in_lastIteration)
    , nextIteration536(INT_MAX)
    , loop536alreadySetUp(0)
    , readyCodelets(0)
    , baseNumThreads(this->numThreads / DARTS_CODELETS_MULT)
    , barrierCodelets536(new _barrierCodelets536[1])
    , checkInCodelets537(new _checkInCodelets537[this->numThreads])
{
    /*Initialize the loop parameters*/
    nextIteration536 = initIteration536;
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets536[0] = _barrierCodelets536(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets537* checkInCodelets537Ptr = (this->checkInCodelets537);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets537);
#endif
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->baseNumThreads;
         codeletCounter++) {
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets537Ptr) = _checkInCodelets537(2, 1, this, codeletCounter);
#else
        (*checkInCodelets537Ptr) = _checkInCodelets537(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets537Ptr).decDep();
        checkInCodelets537Ptr++;
    }
    this->dispatchCodelet(this->getID());
    *(in_ptrToThisTP) = this;
}
void TP536::dispatchCodelet(size_t codeletID)
{
    int currentCodelet = __sync_fetch_and_add(&(this->readyCodelets), 1);
    this->checkInCodelets537[currentCodelet].setID(codeletID);
#if USE_SPIN_CODELETS == 0
    this->firstCodelet[currentCodelet].decDep();
#else
    this->availableCodelets[currentCodelet] = 1;
#endif
}
TP536::~TP536()
{
    delete[] barrierCodelets536;
    delete[] checkInCodelets537;
}
/*TP570: OMPParallelDirective*/
void TP570::_barrierCodelets570::fire(void)
{
    TP570* myTP = static_cast<TP570*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
void TP570::_checkInCodelets573::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 573: BinaryOperator*/
    (this->inputsTPParent->j_darts570[this->getID()]) = 0;

    /*printing node 574: BinaryOperator*/
    /*Print the code for a condition node in a complex loop stmt */
    if ((this->inputsTPParent->j_darts570[this->getID()]) < innerreps) {
        /*Signal the first codelet in the loop*/
        myTP->checkInCodelets572[this->getID()].decDep();
        return;
    }
    /*Signal the codelet after the loop from the end's condional node.*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets570[0].decDep();
}
void TP570::_checkInCodelets572::fire(void)
{

    /*printing node 572: ForStmt*/
    bool haveToLaunch = __sync_bool_compare_and_swap(&(myTP->controlTPParent->TP572_LoopCounter),
        myTP->controlTPParent->TP572_LoopCounterPerThread[this->getID()],
        myTP->controlTPParent->TP572_LoopCounterPerThread[this->getID()] + 1);
    unsigned int iterIdx = myTP->controlTPParent->TP572_LoopCounterPerThread[this->getID()];
    if (haveToLaunch) {
        this->resetCodelet();
        myTP->controlTPParent->TP572PtrVec.push_back(nullptr);
        myTP->controlTPParent->TP572_LoopCounterPerThread[this->getID()] += 1;
        invoke<TP572>(myTP, myTP->numThreads, this->getID(), myTP, myTP->inputsTPParent,
            &(myTP->controlTPParent->TP572PtrVec.back()));
    } else {
        if (myTP->controlTPParent->TP572PtrVec.size() == 0) {
            this->resetCodelet();
            this->decDep();
            return;
        } else if (myTP->controlTPParent->TP572PtrVec.size() < (iterIdx + 1)) {
            this->resetCodelet();
            this->decDep();
            return;
        } else if (myTP->controlTPParent->TP572PtrVec[iterIdx] == nullptr) {
            this->resetCodelet();
            this->decDep();
            return;
        } else {
            this->resetCodelet();
#if USE_SPIN_CODELETS == 0
            myTP->controlTPParent->TP572PtrVec[iterIdx]->firstCodelet[this->getID()].decDep();
#else
            myTP->controlTPParent->TP572PtrVec[iterIdx]->availableCodelets[this->getID()] = 1;
#endif
            myTP->controlTPParent->TP572_LoopCounterPerThread[this->getID()] += 1;
        }
    }
}
void TP570::_checkInCodelets575::fire(void)
{

    /*printing node 575: UnaryOperator*/
    (this->inputsTPParent->j_darts570[this->getID()])++;

    /*printing node 612: BinaryOperator*/
    /*Print the code for a condition node in a complex loop stmt */
    if ((this->inputsTPParent->j_darts570[this->getID()]) < innerreps) {
        this->resetCodelet();
        /*Signal the first codelet in the loop*/
        myTP->checkInCodelets572[this->getID()].decDep();
        return;
    }
    /*Signal the codelet after the loop from the condtional node.*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets570[0].decDep();
}
TP570::TP570(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet, int* in_i)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , i_darts570(in_i) /*OMP_SHARED - INPUT*/
    , j_darts570(new int[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , TP572_LoopCounter(0)
    , TP572_LoopCounterPerThread(new unsigned int[this->numThreads])
    , barrierCodelets570(new _barrierCodelets570[1])
    , checkInCodelets573(new _checkInCodelets573[this->numThreads])
    , checkInCodelets572(new _checkInCodelets572[this->numThreads])
    , checkInCodelets575(new _checkInCodelets575[this->numThreads])
{
    memset((void*)TP572_LoopCounterPerThread, 0, this->numThreads * sizeof(unsigned int));
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets570[0] = _barrierCodelets570(ompNumThreads, ompNumThreads, this, 0);
    _checkInCodelets575* checkInCodelets575Ptr = (this->checkInCodelets575);
    _checkInCodelets572* checkInCodelets572Ptr = (this->checkInCodelets572);
    _checkInCodelets573* checkInCodelets573Ptr = (this->checkInCodelets573);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets575Ptr) = _checkInCodelets575(1, 1, this, codeletCounter);
        checkInCodelets575Ptr++;
        (*checkInCodelets572Ptr) = _checkInCodelets572(1, 1, this, codeletCounter);
        checkInCodelets572Ptr++;
        (*checkInCodelets573Ptr) = _checkInCodelets573(1, 1, this, codeletCounter);
        (*checkInCodelets573Ptr).decDep();
        checkInCodelets573Ptr++;
    }
}
TP570::~TP570()
{
    delete[] TP572_LoopCounterPerThread;
    delete[] barrierCodelets570;
    delete[] checkInCodelets575;
    delete[] checkInCodelets572;
    delete[] checkInCodelets573;
}
/*TP572: ForStmt*/
void TP572::_checkInCodelets577::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    /*printOMPForCodeletFireCode, inlined: 0*/
    /*region 577 0*/
    /*Determine the TP to which this codelet belongs and check if this codelet spawns the TP or if
     * it signals it using dispatchCodelet()*/
    size_t idx = this->getID() / myTP->codeletsPerTP577;
    if (idx < myTP->TPsToUse577) {
        if (!__sync_val_compare_and_swap(&(myTP->TP577_alreadyLaunched[idx]), 0, 1)) {
            int range = abs(itersperthr * nthreads - 0) / 1;
            int rangePerCodelet = range / myTP->TPsToUse577;
            int minIteration = min<int>(itersperthr * nthreads, 0);
            int remainderRange = range % myTP->TPsToUse577;
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
            if (0 < itersperthr * nthreads) {
                initIteration = min(initIteration, lastIteration);
                lastIteration = max(initIteration, lastIteration);
            } else {
                initIteration = max(initIteration, lastIteration);
                lastIteration = min(initIteration, lastIteration);
            }
            if (idx == myTP->TPsToUse577 - 1) {
                lastIteration = itersperthr * nthreads;
            }
#if USEINVOKE == 1
            invoke<TP577>(myTP, myTP->codeletsPerTP577 * DARTS_CODELETS_MULT, this->getID(), myTP,
                initIteration, lastIteration, &(myTP->TP577Ptr[idx]));
#else
            place<TP577>(idx, myTP, myTP->codeletsPerTP577 * DARTS_CODELETS_MULT, this->getID(),
                myTP, initIteration, lastIteration, &(myTP->TP577Ptr[idx]));
#endif
        } else {
            if (myTP->TP577Ptr[idx] != nullptr) {
                myTP->TP577Ptr[idx]->dispatchCodelet(this->getID());
            } else {
                this->resetCodelet();
                this->decDep();
            }
        }
    }
}
void TP572::_barrierCodelets577::fire(void)
{
    TP572* myTP = static_cast<TP572*>(myTP_);
    for (size_t codeletsCounter = 0; codeletsCounter < myTP->numThreads; codeletsCounter++) {
        myTP->TPParent->checkInCodelets575[codeletsCounter].decDep();
    }
}
TP572::TP572(int in_numThreads, int in_mainCodeletID, TP570* in_TPParent, TP570* in_inputsTPParent,
    TP572** in_ptrToThisTP)
    : ompTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(in_inputsTPParent)
    , ptrToThisTP(in_ptrToThisTP)
    , TP577Ptr(new TP577*[NUMTPS577])
    , TP577_alreadyLaunched(new size_t[NUMTPS577])
    , numTPsSet577(0)
    , numTPsReady577(0)
    , TPsToUse577(NUMTPS577)
    , codeletsPerTP577(this->numThreads / NUMTPS577)
    , totalCodelets577(this->TPsToUse577 * this->codeletsPerTP577)
    , checkInCodelets577(new _checkInCodelets577[this->numThreads])
    , barrierCodelets577(new _barrierCodelets577[1])
{
    /*Initialize Codelets*/
    barrierCodelets577[0] = _barrierCodelets577(NUMTPS577, NUMTPS577, this, 0);
    _checkInCodelets577* checkInCodelets577Ptr = (this->checkInCodelets577);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets577);
#endif
    for (int i = 0; i < NUMTPS577; i++) {
        TP577Ptr[i] = nullptr;
        TP577_alreadyLaunched[i] = 0;
    }
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets577Ptr) = _checkInCodelets577(2, 1, this, codeletCounter);
#else
        (*checkInCodelets577Ptr) = _checkInCodelets577(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets577Ptr).decDep();
        checkInCodelets577Ptr++;
    }
    *(this->ptrToThisTP) = this;
#if USE_SPIN_CODELETS == 0
    this->firstCodelet[this->getID()].decDep();
#else
    this->availableCodelets[this->getID()] = 1;
#endif
}
TP572::~TP572()
{
    delete[] barrierCodelets577;
    delete[] checkInCodelets577;
}
/*TP577: OMPForDirective*/
void TP577::_barrierCodelets577::fire(void)
{
    TP577* myTP = static_cast<TP577*>(myTP_);
    myTP->controlTPParent->TPParent->barrierCodelets577[0].decDep();
}
bool TP577::requestNewRangeIterations577(int* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Guided */
    /*Chunk = cksz*/
    bool isThereNewIteration = false;
    return isThereNewIteration;
}
void TP577::_checkInCodelets578::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    /*Init the vars for this region*/

    /*printing node 578: ForStmt*/
    /*var: i*/
    int* i = &(this->inputsTPParent->i_darts577[this->getLocalID()]);
    (void)i /*OMP_PRIVATE*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations577(
        (int*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        /*Signaling omp for stmt's barrier*/
        myTP->controlTPParent->barrierCodelets577[0].decDep();
        return;
    }
    while (isThereNewIteration) {
        for (int i_darts_counter_temp577 = (*i); i_darts_counter_temp577 < endRange
             && i_darts_counter_temp577 < this->inputsTPParent->lastIteration577;
             i_darts_counter_temp577++) {
            {
                delay(delaylength);
            }
        }
        isThereNewIteration = this->inputsTPParent->requestNewRangeIterations577(
            &(this->endRange), this->getLocalID());
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling omp for stmt's barrier*/
    myTP->controlTPParent->barrierCodelets577[0].decDep();
}
TP577::TP577(int in_numThreads, int in_mainCodeletID, TP572* in_TPParent, int in_initIteration,
    int in_lastIteration, TP577** in_ptrToThisTP)
    : ompTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , i_darts577(new int[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , initIteration577(in_initIteration)
    , lastIteration577(in_lastIteration)
    , nextIteration577(INT_MAX)
    , loop577alreadySetUp(0)
    , readyCodelets(0)
    , baseNumThreads(this->numThreads / DARTS_CODELETS_MULT)
    , barrierCodelets577(new _barrierCodelets577[1])
    , checkInCodelets578(new _checkInCodelets578[this->numThreads])
{
    /*Initialize the loop parameters*/
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets577[0] = _barrierCodelets577(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets578* checkInCodelets578Ptr = (this->checkInCodelets578);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets578);
#endif
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->baseNumThreads;
         codeletCounter++) {
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets578Ptr) = _checkInCodelets578(2, 1, this, codeletCounter);
#else
        (*checkInCodelets578Ptr) = _checkInCodelets578(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets578Ptr).decDep();
        checkInCodelets578Ptr++;
    }
    this->dispatchCodelet(this->getID());
    *(in_ptrToThisTP) = this;
}
void TP577::dispatchCodelet(size_t codeletID)
{
    int currentCodelet = __sync_fetch_and_add(&(this->readyCodelets), 1);
    this->checkInCodelets578[currentCodelet].setID(codeletID);
#if USE_SPIN_CODELETS == 0
    this->firstCodelet[currentCodelet].decDep();
#else
    this->availableCodelets[currentCodelet] = 1;
#endif
}
TP577::~TP577()
{
    delete[] barrierCodelets577;
    delete[] checkInCodelets578;
}
