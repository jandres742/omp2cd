#include "parallelfor.output.darts.h"
using namespace darts;
using namespace std;
/*Function: applyFunction, ID: 1*/
int applyFunction(int x)
{
    /*applyFunction:1*/
    /*CompoundStmt:7*/
    int y = x * x - 2 * x + 1;
    return y;
}
/*Function: main, ID: 2*/
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
    /*main:2*/
    /*CompoundStmt:14*/
    srand(time((time_t*)((void*)0)));
    int size = 200, i;
    if (argc >= 2) {
        size = atoi(argv[1]);
    }
    if (size < 0) {
        printf("Size should be greater than 0\n");
        return 0;
    }
    int* inVector = (int*)malloc(size * sizeof(int));
    int* outVectorSeq = (int*)malloc(size * sizeof(int));
    int* outVectorPar = (int*)malloc(4 * size * sizeof(int));
    for (i = 0; i < size; i++) {
        inVector[i] = rand() % 100 - 50;
    }
    for (i = 0; i < size; i++) {
        if (i % 2 == 0) {
            outVectorSeq[i] = applyFunction(inVector[i]);
        } else {
            outVectorSeq[i] = inVector[i];
        }
    }
    int xx, yy = 9;
    (void)xx;
    if (affinMaskRes) {
        myDARTSRuntime->run(
            launch<TP75>(ompNumThreads * DARTS_CODELETS_MULT, 0, RuntimeFinalCodelet, (int*)&((i)),
                (int**)&((inVector)), (int**)&((outVectorPar)), (int*)&((size)), (int*)&((yy))));
    }
    if (affinMaskRes) {
        myDARTSRuntime->run(
            launch<TP215>(ompNumThreads * DARTS_CODELETS_MULT, 0, RuntimeFinalCodelet, 0, size,
                (int**)&((inVector)), (int**)&((outVectorPar)), (int*)&((size))));
    }
    int good = 1;
    for (int n = 0; n < 4 && good == 1; n++) {
        for (i = 0; i < size && good == 1; i++) {
            if (*(outVectorSeq + i) != *(outVectorPar + i + size * n)) {
                printf("Error in parallel test=%d pos=%d\n", n, i);
                good = 0;
            }
        }
    }
    free(inVector);
    free(outVectorPar);
    free(outVectorSeq);
    return 0;
}
/*TP75: OMPParallelDirective*/
void TP75::_barrierCodelets75::fire(void)
{
    TP75* myTP = static_cast<TP75*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
void TP75::_checkInCodelets77::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 77: BinaryOperator*/
    (this->inputsTPParent->xx_darts75[this->getID()]) = 0 + (*(this->inputsTPParent->yy_darts75));

    /*printing node 79: CStyleCastExpr*/
    (void)(this->inputsTPParent->xx_darts75[this->getID()]);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 77 nextRegion: 80 */
    myTP->controlTPParent->checkInCodelets80[this->getID()].decDep();
}
void TP75::_checkInCodelets80::fire(void)
{
    /*printOMPForCodeletFireCode, inlined: 0*/
    /*region 80 0*/
    /*Determine the TP to which this codelet belongs and check if this codelet spawns the TP or if
     * it signals it using dispatchCodelet()*/
    size_t idx = this->getID() / myTP->codeletsPerTP80;
    if (idx < myTP->TPsToUse80) {
        if (!__sync_val_compare_and_swap(&(myTP->TP80_alreadyLaunched[idx]), 0, 1)) {
            int range = abs((*(this->inputsTPParent->size_darts75)) - 0) / 1;
            int rangePerCodelet = range / myTP->TPsToUse80;
            int minIteration = min<int>((*(this->inputsTPParent->size_darts75)), 0);
            int remainderRange = range % myTP->TPsToUse80;
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
            if (0 < (*(this->inputsTPParent->size_darts75))) {
                initIteration = min(initIteration, lastIteration);
                lastIteration = max(initIteration, lastIteration);
            } else {
                initIteration = max(initIteration, lastIteration);
                lastIteration = min(initIteration, lastIteration);
            }
            if (idx == myTP->TPsToUse80 - 1) {
                lastIteration = (*(this->inputsTPParent->size_darts75));
            }
#if USEINVOKE == 1
            invoke<TP80>(myTP, myTP->codeletsPerTP80 * DARTS_CODELETS_MULT, this->getID(), myTP,
                initIteration, lastIteration, &(*(this->inputsTPParent->inVector_darts75)),
                &(*(this->inputsTPParent->outVectorPar_darts75)),
                &(*(this->inputsTPParent->size_darts75)), &(myTP->TP80Ptr[idx]));
#else
            place<TP80>(idx, myTP, myTP->codeletsPerTP80 * DARTS_CODELETS_MULT, this->getID(), myTP,
                initIteration, lastIteration, &(*(this->inputsTPParent->inVector_darts75)),
                &(*(this->inputsTPParent->outVectorPar_darts75)),
                &(*(this->inputsTPParent->size_darts75)), &(myTP->TP80Ptr[idx]));
#endif
        } else {
            if (myTP->TP80Ptr[idx] != nullptr) {
                myTP->TP80Ptr[idx]->dispatchCodelet(this->getID());
            } else {
                this->resetCodelet();
                this->decDep();
            }
        }
    }
}
void TP75::_barrierCodelets80::fire(void)
{
    TP75* myTP = static_cast<TP75*>(myTP_);
    {
        for (size_t codeletsCounter = 0; codeletsCounter < myTP->numThreads; codeletsCounter++) {
            myTP->checkInCodelets123[codeletsCounter].decDep();
        }
    }
}
void TP75::_checkInCodelets123::fire(void)
{
    /*printOMPForCodeletFireCode, inlined: 0*/
    /*region 123 0*/
    /*Determine the TP to which this codelet belongs and check if this codelet spawns the TP or if
     * it signals it using dispatchCodelet()*/
    size_t idx = this->getID() / myTP->codeletsPerTP123;
    if (idx < myTP->TPsToUse123) {
        if (!__sync_val_compare_and_swap(&(myTP->TP123_alreadyLaunched[idx]), 0, 1)) {
            int range = abs((*(this->inputsTPParent->size_darts75)) - 0) / 1;
            int rangePerCodelet = range / myTP->TPsToUse123;
            int minIteration = min<int>((*(this->inputsTPParent->size_darts75)), 0);
            int remainderRange = range % myTP->TPsToUse123;
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
            if (0 < (*(this->inputsTPParent->size_darts75))) {
                initIteration = min(initIteration, lastIteration);
                lastIteration = max(initIteration, lastIteration);
            } else {
                initIteration = max(initIteration, lastIteration);
                lastIteration = min(initIteration, lastIteration);
            }
            if (idx == myTP->TPsToUse123 - 1) {
                lastIteration = (*(this->inputsTPParent->size_darts75));
            }
#if USEINVOKE == 1
            invoke<TP123>(myTP, myTP->codeletsPerTP123 * DARTS_CODELETS_MULT, this->getID(), myTP,
                initIteration, lastIteration, &(*(this->inputsTPParent->inVector_darts75)),
                &(*(this->inputsTPParent->outVectorPar_darts75)),
                &(*(this->inputsTPParent->size_darts75)), &(myTP->TP123Ptr[idx]));
#else
            place<TP123>(idx, myTP, myTP->codeletsPerTP123 * DARTS_CODELETS_MULT, this->getID(),
                myTP, initIteration, lastIteration, &(*(this->inputsTPParent->inVector_darts75)),
                &(*(this->inputsTPParent->outVectorPar_darts75)),
                &(*(this->inputsTPParent->size_darts75)), &(myTP->TP123Ptr[idx]));
#endif
        } else {
            if (myTP->TP123Ptr[idx] != nullptr) {
                myTP->TP123Ptr[idx]->dispatchCodelet(this->getID());
            } else {
                this->resetCodelet();
                this->decDep();
            }
        }
    }
}
void TP75::_barrierCodelets123::fire(void)
{
    TP75* myTP = static_cast<TP75*>(myTP_);
    {
        for (size_t codeletsCounter = 0; codeletsCounter < myTP->numThreads; codeletsCounter++) {
            myTP->checkInCodelets168[codeletsCounter].decDep();
        }
    }
}
void TP75::_checkInCodelets168::fire(void)
{
    /*printOMPForCodeletFireCode, inlined: 0*/
    /*region 168 0*/
    /*Determine the TP to which this codelet belongs and check if this codelet spawns the TP or if
     * it signals it using dispatchCodelet()*/
    size_t idx = this->getID() / myTP->codeletsPerTP168;
    if (idx < myTP->TPsToUse168) {
        if (!__sync_val_compare_and_swap(&(myTP->TP168_alreadyLaunched[idx]), 0, 1)) {
            int range = abs((*(this->inputsTPParent->size_darts75)) - 0) / 1;
            int rangePerCodelet = range / myTP->TPsToUse168;
            int minIteration = min<int>((*(this->inputsTPParent->size_darts75)), 0);
            int remainderRange = range % myTP->TPsToUse168;
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
            if (0 < (*(this->inputsTPParent->size_darts75))) {
                initIteration = min(initIteration, lastIteration);
                lastIteration = max(initIteration, lastIteration);
            } else {
                initIteration = max(initIteration, lastIteration);
                lastIteration = min(initIteration, lastIteration);
            }
            if (idx == myTP->TPsToUse168 - 1) {
                lastIteration = (*(this->inputsTPParent->size_darts75));
            }
#if USEINVOKE == 1
            invoke<TP168>(myTP, myTP->codeletsPerTP168 * DARTS_CODELETS_MULT, this->getID(), myTP,
                initIteration, lastIteration, &(*(this->inputsTPParent->inVector_darts75)),
                &(*(this->inputsTPParent->outVectorPar_darts75)),
                &(*(this->inputsTPParent->size_darts75)), &(myTP->TP168Ptr[idx]));
#else
            place<TP168>(idx, myTP, myTP->codeletsPerTP168 * DARTS_CODELETS_MULT, this->getID(),
                myTP, initIteration, lastIteration, &(*(this->inputsTPParent->inVector_darts75)),
                &(*(this->inputsTPParent->outVectorPar_darts75)),
                &(*(this->inputsTPParent->size_darts75)), &(myTP->TP168Ptr[idx]));
#endif
        } else {
            if (myTP->TP168Ptr[idx] != nullptr) {
                myTP->TP168Ptr[idx]->dispatchCodelet(this->getID());
            } else {
                this->resetCodelet();
                this->decDep();
            }
        }
    }
}
void TP75::_barrierCodelets168::fire(void)
{
    TP75* myTP = static_cast<TP75*>(myTP_);
    myTP->TPParent->barrierCodelets75[0].setDep(0);
    myTP->add(&(myTP->TPParent->barrierCodelets75[0]));
}
TP75::TP75(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet, int* in_i,
    int** in_inVector, int** in_outVectorPar, int* in_size, int* in_yy)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , i_darts75(in_i) /*OMP_SHARED - INPUT*/
    , inVector_darts75(in_inVector) /*OMP_SHARED - INPUT*/
    , outVectorPar_darts75(in_outVectorPar) /*OMP_SHARED - INPUT*/
    , size_darts75(in_size) /*OMP_SHARED - INPUT*/
    , xx_darts75(new int[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , yy_darts75(in_yy) /*OMP_SHARED - INPUT*/
    , TP80Ptr(new TP80*[NUMTPS80])
    , TP80_alreadyLaunched(new size_t[NUMTPS80])
    , numTPsSet80(0)
    , numTPsReady80(0)
    , TPsToUse80(NUMTPS80)
    , codeletsPerTP80(this->numThreads / NUMTPS80)
    , totalCodelets80(this->TPsToUse80 * this->codeletsPerTP80)
    , TP123Ptr(new TP123*[NUMTPS123])
    , TP123_alreadyLaunched(new size_t[NUMTPS123])
    , numTPsSet123(0)
    , numTPsReady123(0)
    , TPsToUse123(NUMTPS123)
    , codeletsPerTP123(this->numThreads / NUMTPS123)
    , totalCodelets123(this->TPsToUse123 * this->codeletsPerTP123)
    , TP168Ptr(new TP168*[NUMTPS168])
    , TP168_alreadyLaunched(new size_t[NUMTPS168])
    , numTPsSet168(0)
    , numTPsReady168(0)
    , TPsToUse168(NUMTPS168)
    , codeletsPerTP168(this->numThreads / NUMTPS168)
    , totalCodelets168(this->TPsToUse168 * this->codeletsPerTP168)
    , barrierCodelets75(new _barrierCodelets75[1])
    , checkInCodelets77(new _checkInCodelets77[this->numThreads])
    , checkInCodelets80(new _checkInCodelets80[this->numThreads])
    , barrierCodelets80(new _barrierCodelets80[1])
    , checkInCodelets123(new _checkInCodelets123[this->numThreads])
    , barrierCodelets123(new _barrierCodelets123[1])
    , checkInCodelets168(new _checkInCodelets168[this->numThreads])
    , barrierCodelets168(new _barrierCodelets168[1])
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets75[0] = _barrierCodelets75(ompNumThreads, ompNumThreads, this, 0);
    barrierCodelets168[0] = _barrierCodelets168(NUMTPS168, NUMTPS168, this, 0);
    barrierCodelets123[0] = _barrierCodelets123(NUMTPS123, NUMTPS123, this, 0);
    barrierCodelets80[0] = _barrierCodelets80(NUMTPS80, NUMTPS80, this, 0);
    _checkInCodelets168* checkInCodelets168Ptr = (this->checkInCodelets168);
    for (int i = 0; i < NUMTPS168; i++) {
        TP168Ptr[i] = nullptr;
        TP168_alreadyLaunched[i] = 0;
    }
    _checkInCodelets123* checkInCodelets123Ptr = (this->checkInCodelets123);
    for (int i = 0; i < NUMTPS123; i++) {
        TP123Ptr[i] = nullptr;
        TP123_alreadyLaunched[i] = 0;
    }
    _checkInCodelets80* checkInCodelets80Ptr = (this->checkInCodelets80);
    for (int i = 0; i < NUMTPS80; i++) {
        TP80Ptr[i] = nullptr;
        TP80_alreadyLaunched[i] = 0;
    }
    _checkInCodelets77* checkInCodelets77Ptr = (this->checkInCodelets77);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets168Ptr) = _checkInCodelets168(1, 1, this, codeletCounter);
        checkInCodelets168Ptr++;
        (*checkInCodelets123Ptr) = _checkInCodelets123(1, 1, this, codeletCounter);
        checkInCodelets123Ptr++;
        (*checkInCodelets80Ptr) = _checkInCodelets80(1, 1, this, codeletCounter);
        checkInCodelets80Ptr++;
        (*checkInCodelets77Ptr) = _checkInCodelets77(1, 1, this, codeletCounter);
        (*checkInCodelets77Ptr).decDep();
        checkInCodelets77Ptr++;
    }
}
TP75::~TP75()
{
    delete[] barrierCodelets75;
    delete[] barrierCodelets168;
    delete[] checkInCodelets168;
    delete[] barrierCodelets123;
    delete[] checkInCodelets123;
    delete[] barrierCodelets80;
    delete[] checkInCodelets80;
    delete[] checkInCodelets77;
}
/*TP80: OMPForDirective*/
void TP80::_barrierCodelets80::fire(void)
{
    TP80* myTP = static_cast<TP80*>(myTP_);
    myTP->controlTPParent->TPParent->barrierCodelets80[0].decDep();
}
bool TP80::requestNewRangeIterations80(int* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Runtime */
    /*Chunk = 1*/
    bool isThereNewIteration = false;
    if (ompLoopSched80 == 1) {
        /*Static Scheduling*/
        int tempStartRange = rangePerCodelet80 * codeletID;
        int tempEndRange = rangePerCodelet80 * (codeletID + 1);
        if (remainderRange80 != 0) {
            if (codeletID < (uint32_t)remainderRange80) {
                tempStartRange += codeletID;
                tempEndRange += (codeletID + 1);
            } else {
                tempStartRange += remainderRange80;
                tempEndRange += remainderRange80;
            }
        }
        tempStartRange = tempStartRange * 1 + minIteration80;
        tempEndRange = tempEndRange * 1 + minIteration80;
        if (tempStartRange != tempEndRange) {
            isThereNewIteration = true;
        }
        if (initIteration80 < lastIteration80) {
            (this->inputsTPParent->i_darts80[codeletID]) = min(tempStartRange, tempEndRange);
            *endRange = max(tempStartRange, tempEndRange);
        } else {
            (this->inputsTPParent->i_darts80[codeletID]) = max(tempStartRange, tempEndRange);
            *endRange = min(tempStartRange, tempEndRange);
        }
        if (codeletID == this->numThreads - 1) {
            *endRange = lastIteration80;
        }
    } else if (ompLoopSched80 == 2) {
        /*Dynamic Scheduling*/
        if (initIteration80 < lastIteration80) {
            (this->inputsTPParent->i_darts80[codeletID])
                = __sync_fetch_and_add(&(nextIteration80), ompLoopChunk80 * 1);
            *endRange = (this->inputsTPParent->i_darts80[codeletID]) + ompLoopChunk80 * 1;
        } else {
            (this->inputsTPParent->i_darts80[codeletID])
                = __sync_fetch_and_sub(&(nextIteration80), ompLoopChunk80 * 1);
            *endRange = (this->inputsTPParent->i_darts80[codeletID]) - ompLoopChunk80 * 1;
        }
        if ((this->inputsTPParent->i_darts80[codeletID]) < lastIteration80) {
            isThereNewIteration = true;
        }
    }
    return isThereNewIteration;
}
void TP80::_checkInCodelets81::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    /*Init the vars for this region*/

    /*printing node 81: ForStmt*/
    /*var: i*/
    /*var: inVector*/
    /*var: outVectorPar*/
    /*var: size*/
    int* i = &(this->inputsTPParent->i_darts80[this->getLocalID()]);
    (void)i /*OMP_PRIVATE*/;
    int** inVector = (this->inputsTPParent->inVector_darts80);
    (void)inVector /*OMP_SHARED*/;
    int** outVectorPar = (this->inputsTPParent->outVectorPar_darts80);
    (void)outVectorPar /*OMP_SHARED*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations80(
        (int*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        /*Signaling omp for stmt's barrier*/
        myTP->controlTPParent->barrierCodelets80[0].decDep();
        return;
    }
    while (isThereNewIteration) {
        for (int i_darts_counter_temp80 = (*i); i_darts_counter_temp80 < endRange
             && i_darts_counter_temp80 < this->inputsTPParent->lastIteration80;
             i_darts_counter_temp80++) {
            {
                if ((i_darts_counter_temp80) % 2 == 0) {
                    *((*(outVectorPar)) + (i_darts_counter_temp80))
                        = applyFunction((*(inVector))[(i_darts_counter_temp80)]);
                } else {
                    *((*(outVectorPar)) + (i_darts_counter_temp80))
                        = (*(inVector))[(i_darts_counter_temp80)];
                }
            }
        }
        /*dont ask for more iterations if the scheduling selected at runtime is static*/
        if (this->inputsTPParent->ompLoopSched80 == 1)
            break;
        isThereNewIteration = this->inputsTPParent->requestNewRangeIterations80(
            &(this->endRange), this->getLocalID());
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling omp for stmt's barrier*/
    myTP->controlTPParent->barrierCodelets80[0].decDep();
}
TP80::TP80(int in_numThreads, int in_mainCodeletID, TP75* in_TPParent, int in_initIteration,
    int in_lastIteration, int** in_inVector, int** in_outVectorPar, int* in_size,
    TP80** in_ptrToThisTP)
    : ompTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , i_darts80(new int[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , inVector_darts80(in_inVector) /*OMP_SHARED - INPUT*/
    , outVectorPar_darts80(in_outVectorPar) /*OMP_SHARED - INPUT*/
    , size_darts80(in_size) /*OMP_SHARED - INPUT*/
    , initIteration80(in_initIteration)
    , lastIteration80(in_lastIteration)
    , ompLoopSched80(1) /*Static by default with runtime scheduling*/
    , ompLoopChunk80(1) /*Chunk =1 by default with runtime scheduling*/
    , nextIteration80(INT_MAX)
    , loop80alreadySetUp(0)
    , readyCodelets(0)
    , baseNumThreads(this->numThreads / DARTS_CODELETS_MULT)
    , barrierCodelets80(new _barrierCodelets80[1])
    , checkInCodelets81(new _checkInCodelets81[this->numThreads])
{
    /*Initialize the loop parameters*/
    ompLoopSched80 = ompSchedulePolicy;
    ompLoopChunk80 = ompScheduleChunk;
    range80 = abs(lastIteration80 - initIteration80) / 1;
    rangePerCodelet80 = range80 / numThreads;
    minIteration80 = min<int>(lastIteration80, initIteration80);
    remainderRange80 = range80 % numThreads;
    nextIteration80 = initIteration80;
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets80[0] = _barrierCodelets80(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets81* checkInCodelets81Ptr = (this->checkInCodelets81);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets81);
#endif
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->baseNumThreads;
         codeletCounter++) {
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets81Ptr) = _checkInCodelets81(2, 1, this, codeletCounter);
#else
        (*checkInCodelets81Ptr) = _checkInCodelets81(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets81Ptr).decDep();
        checkInCodelets81Ptr++;
    }
    this->dispatchCodelet(this->getID());
    *(in_ptrToThisTP) = this;
}
void TP80::dispatchCodelet(size_t codeletID)
{
    int idx = codeletID / this->baseNumThreads;
    int localID = codeletID - this->baseNumThreads * idx;
    this->checkInCodelets81[localID].setID(codeletID);
    this->checkInCodelets81[localID].setLocalID(localID);
    /*Check if we want to replicate codelets*/
    for (size_t i = 1; i < (size_t)DARTS_CODELETS_MULT; i++) {
#if USE_SPIN_CODELETS == 0
        this->checkInCodelets81[localID + this->baseNumThreads * i]
            = _checkInCodelets81(2, 1, this, localID + this->baseNumThreads * i);
#else
        this->checkInCodelets81[localID + this->baseNumThreads * i]
            = _checkInCodelets81(1, 1, this, localID + this->baseNumThreads * i);
#endif
        this->checkInCodelets81[localID + this->baseNumThreads * i].setID(codeletID);
        this->checkInCodelets81[localID + this->baseNumThreads * i].decDep();
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
TP80::~TP80()
{
    delete[] barrierCodelets80;
    delete[] checkInCodelets81;
}
/*TP123: OMPForDirective*/
void TP123::_barrierCodelets123::fire(void)
{
    TP123* myTP = static_cast<TP123*>(myTP_);
    myTP->controlTPParent->TPParent->barrierCodelets123[0].decDep();
}
bool TP123::requestNewRangeIterations123(int* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Static */
    /*Chunk = 2*/
    bool isThereNewIteration = false;
    {
        /*Static Scheduling*/
        int tempStartRange = rangePerCodelet123 * codeletID;
        int tempEndRange = rangePerCodelet123 * (codeletID + 1);
        if (remainderRange123 != 0) {
            if (codeletID < (uint32_t)remainderRange123) {
                tempStartRange += codeletID;
                tempEndRange += (codeletID + 1);
            } else {
                tempStartRange += remainderRange123;
                tempEndRange += remainderRange123;
            }
        }
        tempStartRange = tempStartRange * 1 + minIteration123;
        tempEndRange = tempEndRange * 1 + minIteration123;
        if (tempStartRange != tempEndRange) {
            isThereNewIteration = true;
        }
        if (initIteration123 < lastIteration123) {
            (this->inputsTPParent->i_darts123[codeletID]) = min(tempStartRange, tempEndRange);
            *endRange = max(tempStartRange, tempEndRange);
        } else {
            (this->inputsTPParent->i_darts123[codeletID]) = max(tempStartRange, tempEndRange);
            *endRange = min(tempStartRange, tempEndRange);
        }
        if (codeletID == this->numThreads - 1) {
            *endRange = lastIteration123;
        }
    }
    return isThereNewIteration;
}
void TP123::_checkInCodelets124::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    /*Init the vars for this region*/

    /*printing node 124: ForStmt*/
    /*var: i*/
    /*var: inVector*/
    /*var: outVectorPar*/
    /*var: size*/
    int* i = &(this->inputsTPParent->i_darts123[this->getLocalID()]);
    (void)i /*OMP_PRIVATE*/;
    int** inVector = (this->inputsTPParent->inVector_darts123);
    (void)inVector /*OMP_SHARED*/;
    int** outVectorPar = (this->inputsTPParent->outVectorPar_darts123);
    (void)outVectorPar /*OMP_SHARED*/;
    int* size = (this->inputsTPParent->size_darts123);
    (void)size /*OMP_SHARED*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations123(
        (int*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        /*Signaling omp for stmt's barrier*/
        myTP->controlTPParent->barrierCodelets123[0].decDep();
        return;
    }
    for (int i_darts_counter_temp123 = (*i); i_darts_counter_temp123 < endRange
         && i_darts_counter_temp123 < this->inputsTPParent->lastIteration123;
         i_darts_counter_temp123++) {
        {
            if ((i_darts_counter_temp123) % 2 == 0) {
                *((*(outVectorPar)) + (i_darts_counter_temp123) + (*(size)))
                    = applyFunction((*(inVector))[(i_darts_counter_temp123)]);
            } else {
                *((*(outVectorPar)) + (i_darts_counter_temp123) + (*(size)))
                    = (*(inVector))[(i_darts_counter_temp123)];
            }
        }
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling omp for stmt's barrier*/
    myTP->controlTPParent->barrierCodelets123[0].decDep();
}
TP123::TP123(int in_numThreads, int in_mainCodeletID, TP75* in_TPParent, int in_initIteration,
    int in_lastIteration, int** in_inVector, int** in_outVectorPar, int* in_size,
    TP123** in_ptrToThisTP)
    : ompTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , i_darts123(new int[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , inVector_darts123(in_inVector) /*OMP_SHARED - INPUT*/
    , outVectorPar_darts123(in_outVectorPar) /*OMP_SHARED - INPUT*/
    , size_darts123(in_size) /*OMP_SHARED - INPUT*/
    , initIteration123(in_initIteration)
    , lastIteration123(in_lastIteration)
    , readyCodelets(0)
    , baseNumThreads(this->numThreads / DARTS_CODELETS_MULT)
    , barrierCodelets123(new _barrierCodelets123[1])
    , checkInCodelets124(new _checkInCodelets124[this->numThreads])
{
    /*Initialize the loop parameters*/
    range123 = abs(lastIteration123 - initIteration123) / 1;
    rangePerCodelet123 = range123 / numThreads;
    minIteration123 = min<int>(lastIteration123, initIteration123);
    remainderRange123 = range123 % numThreads;
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets123[0] = _barrierCodelets123(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets124* checkInCodelets124Ptr = (this->checkInCodelets124);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets124);
#endif
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->baseNumThreads;
         codeletCounter++) {
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets124Ptr) = _checkInCodelets124(2, 1, this, codeletCounter);
#else
        (*checkInCodelets124Ptr) = _checkInCodelets124(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets124Ptr).decDep();
        checkInCodelets124Ptr++;
    }
    this->dispatchCodelet(this->getID());
    *(in_ptrToThisTP) = this;
}
void TP123::dispatchCodelet(size_t codeletID)
{
    int idx = codeletID / this->baseNumThreads;
    int localID = codeletID - this->baseNumThreads * idx;
    this->checkInCodelets124[localID].setID(codeletID);
    this->checkInCodelets124[localID].setLocalID(localID);
    /*Check if we want to replicate codelets*/
    for (size_t i = 1; i < (size_t)DARTS_CODELETS_MULT; i++) {
#if USE_SPIN_CODELETS == 0
        this->checkInCodelets124[localID + this->baseNumThreads * i]
            = _checkInCodelets124(2, 1, this, localID + this->baseNumThreads * i);
#else
        this->checkInCodelets124[localID + this->baseNumThreads * i]
            = _checkInCodelets124(1, 1, this, localID + this->baseNumThreads * i);
#endif
        this->checkInCodelets124[localID + this->baseNumThreads * i].setID(codeletID);
        this->checkInCodelets124[localID + this->baseNumThreads * i].decDep();
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
TP123::~TP123()
{
    delete[] barrierCodelets123;
    delete[] checkInCodelets124;
}
/*TP168: OMPForDirective*/
void TP168::_barrierCodelets168::fire(void)
{
    TP168* myTP = static_cast<TP168*>(myTP_);
    myTP->controlTPParent->TPParent->barrierCodelets168[0].decDep();
}
bool TP168::requestNewRangeIterations168(int* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Dynamic */
    /*Chunk = 3*/
    bool isThereNewIteration = false;
    {
        /*Dynamic Scheduling*/
        if (initIteration168 < lastIteration168) {
            (this->inputsTPParent->i_darts168[codeletID])
                = __sync_fetch_and_add(&(nextIteration168), 3 * 1);
            *endRange = (this->inputsTPParent->i_darts168[codeletID]) + 3 * 1;
        } else {
            (this->inputsTPParent->i_darts168[codeletID])
                = __sync_fetch_and_sub(&(nextIteration168), 3 * 1);
            *endRange = (this->inputsTPParent->i_darts168[codeletID]) - 3 * 1;
        }
        if ((this->inputsTPParent->i_darts168[codeletID]) < lastIteration168) {
            isThereNewIteration = true;
        }
    }
    return isThereNewIteration;
}
void TP168::_checkInCodelets169::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    /*Init the vars for this region*/

    /*printing node 169: ForStmt*/
    /*var: i*/
    /*var: inVector*/
    /*var: outVectorPar*/
    /*var: size*/
    int* i = &(this->inputsTPParent->i_darts168[this->getLocalID()]);
    (void)i /*OMP_PRIVATE*/;
    int** inVector = (this->inputsTPParent->inVector_darts168);
    (void)inVector /*OMP_SHARED*/;
    int** outVectorPar = (this->inputsTPParent->outVectorPar_darts168);
    (void)outVectorPar /*OMP_SHARED*/;
    int* size = (this->inputsTPParent->size_darts168);
    (void)size /*OMP_SHARED*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations168(
        (int*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        /*Signaling omp for stmt's barrier*/
        myTP->controlTPParent->barrierCodelets168[0].decDep();
        return;
    }
    while (isThereNewIteration) {
        for (int i_darts_counter_temp168 = (*i); i_darts_counter_temp168 < endRange
             && i_darts_counter_temp168 < this->inputsTPParent->lastIteration168;
             i_darts_counter_temp168++) {
            {
                if ((i_darts_counter_temp168) % 2 == 0) {
                    *((*(outVectorPar)) + (i_darts_counter_temp168) + (*(size)) * 2)
                        = applyFunction((*(inVector))[(i_darts_counter_temp168)]);
                } else {
                    *((*(outVectorPar)) + (i_darts_counter_temp168) + (*(size)) * 2)
                        = (*(inVector))[(i_darts_counter_temp168)];
                }
            }
        }
        isThereNewIteration = this->inputsTPParent->requestNewRangeIterations168(
            &(this->endRange), this->getLocalID());
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling omp for stmt's barrier*/
    myTP->controlTPParent->barrierCodelets168[0].decDep();
}
TP168::TP168(int in_numThreads, int in_mainCodeletID, TP75* in_TPParent, int in_initIteration,
    int in_lastIteration, int** in_inVector, int** in_outVectorPar, int* in_size,
    TP168** in_ptrToThisTP)
    : ompTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , i_darts168(new int[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , inVector_darts168(in_inVector) /*OMP_SHARED - INPUT*/
    , outVectorPar_darts168(in_outVectorPar) /*OMP_SHARED - INPUT*/
    , size_darts168(in_size) /*OMP_SHARED - INPUT*/
    , initIteration168(in_initIteration)
    , lastIteration168(in_lastIteration)
    , nextIteration168(INT_MAX)
    , loop168alreadySetUp(0)
    , readyCodelets(0)
    , baseNumThreads(this->numThreads / DARTS_CODELETS_MULT)
    , barrierCodelets168(new _barrierCodelets168[1])
    , checkInCodelets169(new _checkInCodelets169[this->numThreads])
{
    /*Initialize the loop parameters*/
    nextIteration168 = initIteration168;
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets168[0] = _barrierCodelets168(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets169* checkInCodelets169Ptr = (this->checkInCodelets169);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets169);
#endif
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->baseNumThreads;
         codeletCounter++) {
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets169Ptr) = _checkInCodelets169(2, 1, this, codeletCounter);
#else
        (*checkInCodelets169Ptr) = _checkInCodelets169(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets169Ptr).decDep();
        checkInCodelets169Ptr++;
    }
    this->dispatchCodelet(this->getID());
    *(in_ptrToThisTP) = this;
}
void TP168::dispatchCodelet(size_t codeletID)
{
    int currentCodelet = __sync_fetch_and_add(&(this->readyCodelets), 1);
    this->checkInCodelets169[currentCodelet].setID(codeletID);
#if USE_SPIN_CODELETS == 0
    this->firstCodelet[currentCodelet].decDep();
#else
    this->availableCodelets[currentCodelet] = 1;
#endif
}
TP168::~TP168()
{
    delete[] barrierCodelets168;
    delete[] checkInCodelets169;
}
/*TP215: OMPParallelForDirective*/
void TP215::_barrierCodelets215::fire(void)
{
    TP215* myTP = static_cast<TP215*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
bool TP215::requestNewRangeIterations215(int* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Dynamic */
    /*Chunk = 1*/
    bool isThereNewIteration = false;
    {
        /*Dynamic Scheduling*/
        if (initIteration215 < lastIteration215) {
            (this->inputsTPParent->i_darts215[codeletID])
                = __sync_fetch_and_add(&(nextIteration215), 1 * 1);
            *endRange = (this->inputsTPParent->i_darts215[codeletID]) + 1 * 1;
        } else {
            (this->inputsTPParent->i_darts215[codeletID])
                = __sync_fetch_and_sub(&(nextIteration215), 1 * 1);
            *endRange = (this->inputsTPParent->i_darts215[codeletID]) - 1 * 1;
        }
        if ((this->inputsTPParent->i_darts215[codeletID]) < lastIteration215) {
            isThereNewIteration = true;
        }
    }
    return isThereNewIteration;
}
void TP215::_checkInCodelets216::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 216: ForStmt*/
    /*var: i*/
    /*var: inVector*/
    /*var: outVectorPar*/
    /*var: size*/
    int* i = &(this->inputsTPParent->i_darts215[this->getLocalID()]);
    (void)i /*OMP_PRIVATE*/;
    int** inVector = (this->inputsTPParent->inVector_darts215);
    (void)inVector /*OMP_SHARED*/;
    int** outVectorPar = (this->inputsTPParent->outVectorPar_darts215);
    (void)outVectorPar /*OMP_SHARED*/;
    int* size = (this->inputsTPParent->size_darts215);
    (void)size /*OMP_SHARED*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations215(
        (int*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        /*Find and signal the next codelet*/
        myTP->controlTPParent->TPParent->barrierCodelets215[0].decDep();
        return;
    }
    while (isThereNewIteration) {
        for (int i_darts_counter_temp215 = (*i); i_darts_counter_temp215 < endRange
             && i_darts_counter_temp215 < this->inputsTPParent->lastIteration215;
             i_darts_counter_temp215++) {
            {
                if ((i_darts_counter_temp215) % 2 == 0) {
                    *((*(outVectorPar)) + (i_darts_counter_temp215) + (*(size)) * 3)
                        = applyFunction((*(inVector))[(i_darts_counter_temp215)]);
                } else {
                    *((*(outVectorPar)) + (i_darts_counter_temp215) + (*(size)) * 3)
                        = (*(inVector))[(i_darts_counter_temp215)];
                }
            }
        }
        isThereNewIteration = this->inputsTPParent->requestNewRangeIterations215(
            &(this->endRange), this->getLocalID());
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets215[0].decDep();
}
TP215::TP215(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet, int in_initIteration,
    int in_lastIteration, int** in_inVector, int** in_outVectorPar, int* in_size)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , i_darts215(new int[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , inVector_darts215(in_inVector) /*OMP_SHARED - INPUT*/
    , outVectorPar_darts215(in_outVectorPar) /*OMP_SHARED - INPUT*/
    , size_darts215(in_size) /*OMP_SHARED - INPUT*/
    , initIteration215(in_initIteration)
    , lastIteration215(in_lastIteration)
    , nextIteration215(INT_MAX)
    , loop215alreadySetUp(0)
    , barrierCodelets215(new _barrierCodelets215[1])
    , checkInCodelets216(new _checkInCodelets216[this->numThreads])
{
    /*Initialize the loop parameters*/
    nextIteration215 = initIteration215;
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets215[0] = _barrierCodelets215(ompNumThreads, ompNumThreads, this, 0);
    _checkInCodelets216* checkInCodelets216Ptr = (this->checkInCodelets216);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets216Ptr) = _checkInCodelets216(1, 1, this, codeletCounter);
        (*checkInCodelets216Ptr).decDep();
        checkInCodelets216Ptr++;
    }
}
TP215::~TP215()
{
    delete[] barrierCodelets215;
    delete[] checkInCodelets216;
}
