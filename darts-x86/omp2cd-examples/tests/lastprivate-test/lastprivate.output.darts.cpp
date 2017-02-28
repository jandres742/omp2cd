#include "lastprivate.output.darts.h"
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
    int N = 10;
    if (argc > 1)
        N = atoi(argv[1]);
    MYTYPE* A = (double*)malloc(N * sizeof(double));
    int i;
    for (i = 0; i < N; i++) {
        *(A + i) = i + 1;
    }
    i = 0;
    int w = 20;
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP30>(ompNumThreads * DARTS_CODELETS_MULT, 0,
            RuntimeFinalCodelet, (MYTYPE**)&((A)), (int*)&((N)), (int*)&((i)), (int*)&((w))));
    }
    return 0;
}
/*TP30: OMPParallelDirective*/
void TP30::_barrierCodelets30::fire(void)
{
    TP30* myTP = static_cast<TP30*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
void TP30::_checkInCodelets32::fire(void)
{
    /*printOMPForCodeletFireCode, inlined: 0*/
    /*region 32 0*/
    /*Determine the TP to which this codelet belongs and check if this codelet spawns the TP or if
     * it signals it using dispatchCodelet()*/
    size_t idx = this->getID() / myTP->codeletsPerTP32;
    if (idx < myTP->TPsToUse32) {
        if (!__sync_val_compare_and_swap(&(myTP->TP32_alreadyLaunched[idx]), 0, 1)) {
            int range = abs((*(this->inputsTPParent->N_darts30)) - 0) / 1;
            int rangePerCodelet = range / myTP->TPsToUse32;
            int minIteration = min<int>((*(this->inputsTPParent->N_darts30)), 0);
            int remainderRange = range % myTP->TPsToUse32;
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
            if (0 < (*(this->inputsTPParent->N_darts30))) {
                initIteration = min(initIteration, lastIteration);
                lastIteration = max(initIteration, lastIteration);
            } else {
                initIteration = max(initIteration, lastIteration);
                lastIteration = min(initIteration, lastIteration);
            }
            if (idx == myTP->TPsToUse32 - 1) {
                lastIteration = (*(this->inputsTPParent->N_darts30));
            }
#if USEINVOKE == 1
            invoke<TP32>(myTP, myTP->codeletsPerTP32 * DARTS_CODELETS_MULT, this->getID(), myTP,
                initIteration, lastIteration, &(*(this->inputsTPParent->A_darts30)),
                &(*(this->inputsTPParent->N_darts30)), &(*(this->inputsTPParent->i_darts30)),
                &(*(this->inputsTPParent->w_darts30)), &(myTP->TP32Ptr[idx]));
#else
            place<TP32>(idx, myTP, myTP->codeletsPerTP32 * DARTS_CODELETS_MULT, this->getID(), myTP,
                initIteration, lastIteration, &(*(this->inputsTPParent->A_darts30)),
                &(*(this->inputsTPParent->N_darts30)), &(*(this->inputsTPParent->i_darts30)),
                &(*(this->inputsTPParent->w_darts30)), &(myTP->TP32Ptr[idx]));
#endif
        } else {
            if (myTP->TP32Ptr[idx] != nullptr) {
                myTP->TP32Ptr[idx]->dispatchCodelet(this->getID());
            } else {
                this->resetCodelet();
                this->decDep();
            }
        }
    }
}
void TP30::_barrierCodelets32::fire(void)
{
    TP30* myTP = static_cast<TP30*>(myTP_);
    {
        for (size_t codeletsCounter = 0; codeletsCounter < myTP->numThreads; codeletsCounter++) {
            myTP->checkInCodelets78[codeletsCounter].decDep();
        }
    }
}
void TP30::_checkInCodelets78::fire(void)
{

    /*printing node 78: IfStmt*/
    if ((*(this->inputsTPParent->i_darts30)) != (*(this->inputsTPParent->N_darts30))) {
        fprintf(stderr, "Error in thread = %d for lastprivate, i = %d, should be %d\n",
            omp_get_thread_num(), (*(this->inputsTPParent->i_darts30)),
            (*(this->inputsTPParent->N_darts30)));
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets30[0].decDep();
}
TP30::TP30(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet, MYTYPE** in_A,
    int* in_N, int* in_i, int* in_w)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , A_darts30(in_A) /*OMP_SHARED - INPUT*/
    , N_darts30(in_N) /*OMP_SHARED - INPUT*/
    , i_darts30(in_i) /*OMP_SHARED - INPUT*/
    , w_darts30(in_w) /*OMP_SHARED - INPUT*/
    , TP32Ptr(new TP32*[NUMTPS32])
    , TP32_alreadyLaunched(new size_t[NUMTPS32])
    , numTPsSet32(0)
    , numTPsReady32(0)
    , TPsToUse32(NUMTPS32)
    , codeletsPerTP32(this->numThreads / NUMTPS32)
    , totalCodelets32(this->TPsToUse32 * this->codeletsPerTP32)
    , barrierCodelets30(new _barrierCodelets30[1])
    , checkInCodelets32(new _checkInCodelets32[this->numThreads])
    , barrierCodelets32(new _barrierCodelets32[1])
    , checkInCodelets78(new _checkInCodelets78[this->numThreads])
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets30[0] = _barrierCodelets30(ompNumThreads, ompNumThreads, this, 0);
    barrierCodelets32[0] = _barrierCodelets32(NUMTPS32, NUMTPS32, this, 0);
    _checkInCodelets78* checkInCodelets78Ptr = (this->checkInCodelets78);
    _checkInCodelets32* checkInCodelets32Ptr = (this->checkInCodelets32);
    for (int i = 0; i < NUMTPS32; i++) {
        TP32Ptr[i] = nullptr;
        TP32_alreadyLaunched[i] = 0;
    }
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets78Ptr) = _checkInCodelets78(1, 1, this, codeletCounter);
        checkInCodelets78Ptr++;
        (*checkInCodelets32Ptr) = _checkInCodelets32(1, 1, this, codeletCounter);
        (*checkInCodelets32Ptr).decDep();
        checkInCodelets32Ptr++;
    }
}
TP30::~TP30()
{
    delete[] barrierCodelets30;
    delete[] checkInCodelets78;
    delete[] barrierCodelets32;
    delete[] checkInCodelets32;
}
/*TP32: OMPForDirective*/
void TP32::_barrierCodelets32::fire(void)
{
    TP32* myTP = static_cast<TP32*>(myTP_);
    myTP->controlTPParent->TPParent->barrierCodelets32[0].decDep();
}
bool TP32::requestNewRangeIterations32(
    int* endRange, uint32_t codeletID, bool* containsLastIteration)
{
    /*Scheduling Policy = Static */
    /*Chunk = 0*/
    bool isThereNewIteration = false;
    {
        /*Static Scheduling*/
        int tempStartRange = rangePerCodelet32 * codeletID;
        int tempEndRange = rangePerCodelet32 * (codeletID + 1);
        if (remainderRange32 != 0) {
            if (codeletID < (uint32_t)remainderRange32) {
                tempStartRange += codeletID;
                tempEndRange += (codeletID + 1);
            } else {
                tempStartRange += remainderRange32;
                tempEndRange += remainderRange32;
            }
        }
        tempStartRange = tempStartRange * 1 + minIteration32;
        tempEndRange = tempEndRange * 1 + minIteration32;
        if (tempStartRange != tempEndRange) {
            isThereNewIteration = true;
        }
        if (initIteration32 < lastIteration32) {
            (this->inputsTPParent->i_darts32[codeletID]) = min(tempStartRange, tempEndRange);
            *endRange = max(tempStartRange, tempEndRange);
        } else {
            (this->inputsTPParent->i_darts32[codeletID]) = max(tempStartRange, tempEndRange);
            *endRange = min(tempStartRange, tempEndRange);
        }
        if (codeletID == this->numThreads - 1) {
            *endRange = lastIteration32;
        }
    }
    if ((this->inputsTPParent->i_darts32[codeletID]) <= lastIteration32
        && lastIteration32 <= *endRange) {
        *containsLastIteration = true;
    }
    return isThereNewIteration;
}
void TP32::_checkInCodelets34::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    /*Init the vars for this region*/

    /*printing node 34: ForStmt*/
    /*var: A*/
    /*var: N*/
    /*var: i*/
    int* i_outer32_ptr = this->inputsTPParent->i_outer32_ptr;
    MYTYPE** A = (this->inputsTPParent->A_darts32);
    (void)A /*OMP_SHARED*/;
    int* i = &(this->inputsTPParent->i_darts32[this->getLocalID()]);
    (void)i /*OMP_LASTPRIVATE*/;
    int* w = &(this->inputsTPParent->w_darts32[this->getLocalID()]);
    (void)w /*OMP_FIRSTPRIVATE*/;
    bool containsLastIteration = false;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations32(
        (int*)&(this->endRange), this->getLocalID(), &containsLastIteration);
    if (isThereNewIteration == false) {
        /*Signaling omp for stmt's barrier*/
        myTP->controlTPParent->barrierCodelets32[0].decDep();
        return;
    }
    if (containsLastIteration) {
        for (int i_darts_counter_temp32 = (*i); i_darts_counter_temp32 < endRange
             && i_darts_counter_temp32 < this->inputsTPParent->lastIteration32;
             i_darts_counter_temp32++) {
            {
                if ((*w) != 20) {
                    fprintf(stderr, "Error in thread = %d for firstprivate, w = %d, should be %d\n",
                        omp_get_thread_num(), (*w), 20);
                }
                *((*(A)) + (i_darts_counter_temp32)) += *((*(A)) + (i_darts_counter_temp32))
                    + *((*(A)) + (i_darts_counter_temp32)) + (*w);
            }
            /*Update lastprivate variables, if any*/
            (myTP->i_darts32[this->getLocalID()])++;
            *(i_outer32_ptr) = *i;
        }
    } else {
        for (int i_darts_counter_temp32 = (*i); i_darts_counter_temp32 < endRange
             && i_darts_counter_temp32 < this->inputsTPParent->lastIteration32;
             i_darts_counter_temp32++) {
            {
                if ((*w) != 20) {
                    fprintf(stderr, "Error in thread = %d for firstprivate, w = %d, should be %d\n",
                        omp_get_thread_num(), (*w), 20);
                }
                *((*(A)) + (i_darts_counter_temp32)) += *((*(A)) + (i_darts_counter_temp32))
                    + *((*(A)) + (i_darts_counter_temp32)) + (*w);
            }
        }
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling omp for stmt's barrier*/
    myTP->controlTPParent->barrierCodelets32[0].decDep();
}
TP32::TP32(int in_numThreads, int in_mainCodeletID, TP30* in_TPParent, int in_initIteration,
    int in_lastIteration, MYTYPE** in_A, int* in_N, int* in_i_outer32_ptr, int* in_w_outer32_ptr,
    TP32** in_ptrToThisTP)
    : ompTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , A_darts32(in_A) /*OMP_SHARED - INPUT*/
    , N_darts32(in_N) /*OMP_SHARED - INPUT*/
    , i_darts32(new int[this->numThreads]) /*OMP_LASTPRIVATE - INPUT*/
    , i_outer32_ptr(in_i_outer32_ptr)
    , w_darts32(new int[this->numThreads]) /*OMP_FIRSTPRIVATE - INPUT*/
    , w_outer32_ptr(in_w_outer32_ptr)
    , initIteration32(in_initIteration)
    , lastIteration32(in_lastIteration)
    , readyCodelets(0)
    , baseNumThreads(this->numThreads / DARTS_CODELETS_MULT)
    , barrierCodelets32(new _barrierCodelets32[1])
    , checkInCodelets34(new _checkInCodelets34[this->numThreads])
{
    /*Initialize the loop parameters*/
    range32 = abs(lastIteration32 - initIteration32) / 1;
    rangePerCodelet32 = range32 / numThreads;
    minIteration32 = min<int>(lastIteration32, initIteration32);
    remainderRange32 = range32 % numThreads;
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets32[0] = _barrierCodelets32(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets34* checkInCodelets34Ptr = (this->checkInCodelets34);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets34);
#endif
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->baseNumThreads;
         codeletCounter++) {
        w_darts32[codeletCounter] = *in_w_outer32_ptr; /*OMP_FIRSTPRIVATE*/
        for (size_t i = 1; i < (size_t)DARTS_CODELETS_MULT; i++) {
            w_darts32[codeletCounter + this->baseNumThreads * i] = *in_w_outer32_ptr;
        }
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets34Ptr) = _checkInCodelets34(2, 1, this, codeletCounter);
#else
        (*checkInCodelets34Ptr) = _checkInCodelets34(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets34Ptr).decDep();
        checkInCodelets34Ptr++;
    }
    this->dispatchCodelet(this->getID());
    *(in_ptrToThisTP) = this;
}
void TP32::dispatchCodelet(size_t codeletID)
{
    int idx = codeletID / this->baseNumThreads;
    int localID = codeletID - this->baseNumThreads * idx;
    this->checkInCodelets34[localID].setID(codeletID);
    this->checkInCodelets34[localID].setLocalID(localID);
    /*Check if we want to replicate codelets*/
    for (size_t i = 1; i < (size_t)DARTS_CODELETS_MULT; i++) {
#if USE_SPIN_CODELETS == 0
        this->checkInCodelets34[localID + this->baseNumThreads * i]
            = _checkInCodelets34(2, 1, this, localID + this->baseNumThreads * i);
#else
        this->checkInCodelets34[localID + this->baseNumThreads * i]
            = _checkInCodelets34(1, 1, this, localID + this->baseNumThreads * i);
#endif
        this->checkInCodelets34[localID + this->baseNumThreads * i].setID(codeletID);
        this->checkInCodelets34[localID + this->baseNumThreads * i].decDep();
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
TP32::~TP32()
{
    delete[] barrierCodelets32;
    delete[] checkInCodelets34;
}
