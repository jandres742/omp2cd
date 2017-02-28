#include "sparseMatrix.output.darts.h"
using namespace darts;
using namespace std;
/*Function: nnzStart, ID: 97*/
int nnzStart(int hsize, int msize)
{
    /*nnzStart:97*/
    /*CompoundStmt:209*/
    int M = msize;
    if (M == 0)
        M = hsize;
    if ((M % 32) > 0)
        M += (32 - (M % 32));
    if (M > hsize)
        M = hsize;
    if (printRank())
        printf("Adjusted M = %d\n", M);
    return M;
}
/*Function: initSparseMatrix, ID: 98*/
SparseMatrix* initSparseMatrix(int hsize, int msize)
{
    /*initSparseMatrix:98*/
    /*CompoundStmt:230*/
    SparseMatrix* spmatrix = (SparseMatrix*)malloc(sizeof(SparseMatrix));
    spmatrix->hsize = hsize;
    spmatrix->msize = msize;
    spmatrix->iia = (int*)malloc(hsize * sizeof(int));
    spmatrix->jja = (int**)malloc(hsize * sizeof(int*));
    for (int i = 0; i < hsize; i++) {
        spmatrix->jja[i] = (int*)malloc(msize * sizeof(int));
    }
    spmatrix->val = (real_t**)malloc(hsize * sizeof(real_t*));
    for (int i = 0; i < hsize; i++) {
        spmatrix->val[i] = (real_t*)malloc(msize * sizeof(real_t));
    }
    memset(spmatrix->iia, 0, hsize * sizeof(int));
    spmatrix->maxEval = ZERO;
    spmatrix->minEval = ZERO;
    spmatrix->maxMinusMin = ZERO;
    spmatrix->bandwidth = 0;
    return spmatrix;
}
/*Function: destroySparseMatrix, ID: 99*/
void destroySparseMatrix(struct SparseMatrixSt* spmatrix)
{
    /*destroySparseMatrix:99*/
    /*CompoundStmt:294*/
    int hsize = spmatrix->hsize;
    free(spmatrix->iia);
    for (int i = 0; i < hsize; i++) {
    }
    free(spmatrix->jja);
    for (int i = 0; i < hsize; i++) {
        free(spmatrix->val[i]);
    }
    free(spmatrix->val);
    spmatrix->hsize = 0;
    spmatrix->msize = 0;
    spmatrix->bandwidth = 0;
    spmatrix->minEval = ZERO;
    spmatrix->maxEval = ZERO;
    spmatrix->maxMinusMin = ZERO;
}
/*Function: sparsity, ID: 100*/
void sparsity(struct SparseMatrixSt* spmatrix)
{
    /*sparsity:100*/
    /*CompoundStmt:328*/
    int hsize = spmatrix->hsize;
    int hValCount = 0;
    int hDist[hsize];
    memset(hDist, 0, hsize * sizeof(int));
    for (int i = 0; i < hsize; i++) {
        hValCount += spmatrix->iia[i];
        if (spmatrix->iia[i] > 0)
            hDist[spmatrix->iia[i]] += 1;
    }
    if (printRank()) {
        printf("\nSparsity:\nInitial sparsity = %d, fraction = %e, Avg per row = %f\n", hValCount,
            (real_t)hValCount / (real_t)(hsize * hsize), (real_t)hValCount / (real_t)hsize);
        int maxRowCount = 0;
        for (int i = 0; i < hsize; i++) {
            maxRowCount = ((maxRowCount) > (spmatrix->iia[i]) ? (maxRowCount) : (spmatrix->iia[i]));
        }
        printf("Max per row = %d\n", maxRowCount);
        for (int i = 0; i < hsize; i++) {
            if (hDist[i] > 0)
                printf("I = %d, count = %d, fraction = %f\n", i, hDist[i],
                    (real_t)hDist[i] / (real_t)hsize);
        }
    }
}
/*Function: gershgorin, ID: 101*/
void gershgorin(struct SparseMatrixSt* spmatrix, struct DomainSt* domain)
{
    /*gershgorin:101*/
    /*CompoundStmt:390*/
    int hsize = spmatrix->hsize;
    real_t eMin = 10000;
    real_t eMax = -10000;
    real_t sumP, sumM, maxMinusMin;
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP397>(ompNumThreads * DARTS_CODELETS_MULT, 0,
            RuntimeFinalCodelet, 0, hsize, (real_t*)&((eMax)), (real_t*)&((eMin)), (int*)&((hsize)),
            (struct SparseMatrixSt**)&((spmatrix))));
    }
    maxMinusMin = eMax - eMin;
    if (printRank())
        printf("\nGershgorin:\nNew  eMax, eMin = %e, %e\n", eMax, eMin);
    spmatrix->maxEval = eMax;
    spmatrix->minEval = eMin;
    spmatrix->maxMinusMin = maxMinusMin;
}
/*Function: normalize, ID: 102*/
void normalize(struct SparseMatrixSt* spmatrix)
{
    /*normalize:102*/
    /*CompoundStmt:472*/
    int hsize = spmatrix->hsize;
    int sumIia = 0;
    int maxIia = 0;
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP477>(ompNumThreads * DARTS_CODELETS_MULT, 0,
            RuntimeFinalCodelet, 0, hsize, (int*)&((hsize)), (int*)&((maxIia)),
            (struct SparseMatrixSt**)&((spmatrix)), (int*)&((sumIia))));
    }
    if (printRank() && debug == 1)
        printf("Initial sparsity normalized = %d, fraction = %e,  avg = %g, max = %d\n", sumIia,
            (real_t)sumIia / (real_t)(hsize * hsize), (real_t)sumIia / (real_t)hsize, maxIia);
}
/*Function: trace, ID: 103*/
void trace(struct SparseMatrixSt* spmatrix, struct DomainSt* domain, real_t* tr, real_t* tr2)
{
    /*trace:103*/
    /*CompoundStmt:560*/
    int hsize = spmatrix->hsize;
    real_t trace = ZERO;
    real_t trace2 = ZERO;
    if (affinMaskRes) {
        myDARTSRuntime->run(
            launch<TP565>(ompNumThreads * DARTS_CODELETS_MULT, 0, RuntimeFinalCodelet,
                domain->localRowMin, domain->localRowMax, (struct DomainSt**)&((domain)),
                (struct SparseMatrixSt**)&((spmatrix)), (real_t*)&((trace)), (real_t*)&((trace2))));
    }
    *tr = trace;
    *tr2 = trace2;
}
/*TP397: OMPParallelForDirective*/
void TP397::_barrierCodelets397::fire(void)
{
    TP397* myTP = static_cast<TP397*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
bool TP397::requestNewRangeIterations397(int* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Static */
    /*Chunk = 0*/
    bool isThereNewIteration = false;
    {
        /*Static Scheduling*/
        int tempStartRange = rangePerCodelet397 * codeletID;
        int tempEndRange = rangePerCodelet397 * (codeletID + 1);
        if (remainderRange397 != 0) {
            if (codeletID < (uint32_t)remainderRange397) {
                tempStartRange += codeletID;
                tempEndRange += (codeletID + 1);
            } else {
                tempStartRange += remainderRange397;
                tempEndRange += remainderRange397;
            }
        }
        tempStartRange = tempStartRange * 1 + minIteration397;
        tempEndRange = tempEndRange * 1 + minIteration397;
        if (tempStartRange != tempEndRange) {
            isThereNewIteration = true;
        }
        if (initIteration397 < lastIteration397) {
            (this->inputsTPParent->i_darts397[codeletID]) = min(tempStartRange, tempEndRange);
            *endRange = max(tempStartRange, tempEndRange);
        } else {
            (this->inputsTPParent->i_darts397[codeletID]) = max(tempStartRange, tempEndRange);
            *endRange = min(tempStartRange, tempEndRange);
        }
        if (codeletID == this->numThreads - 1) {
            *endRange = lastIteration397;
        }
    }
    return isThereNewIteration;
}
void TP397::_checkInCodelets398::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 398: ForStmt*/
    /*var: eMax*/
    /*var: eMin*/
    /*var: hsize*/
    /*var: spmatrix*/
    /*var: sumM*/
    /*var: sumP*/
    /*Declare local reduction vars*/
    real_t eMin_LocalReductionVar = (real_t)(std::numeric_limits<real_t>::infinity());
    real_t eMax_LocalReductionVar = (real_t)(std::numeric_limits<real_t>::infinity() * (-1));
    struct SparseMatrixSt** spmatrix = (this->inputsTPParent->spmatrix_darts397);
    (void)spmatrix /*OMP_SHARED*/;
    real_t* sumM = &(this->inputsTPParent->sumM_darts397[this->getLocalID()]);
    (void)sumM /*OMP_PRIVATE*/;
    real_t* sumP = &(this->inputsTPParent->sumP_darts397[this->getLocalID()]);
    (void)sumP /*OMP_PRIVATE*/;
    int* i = &(this->inputsTPParent->i_darts397[this->getLocalID()]);
    (void)i /*PRIVATE*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations397(
        (int*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        goto updateReduceVars;
    }
    for (int i_darts_counter_temp397 = (*i); i_darts_counter_temp397 < endRange
         && i_darts_counter_temp397 < this->inputsTPParent->lastIteration397;
         i_darts_counter_temp397++) {
        {
            (*sumM) = 0.;
            {
                /*Loop's init*/
                int j = 0;
                for (; j < (*(spmatrix))->iia[(i_darts_counter_temp397)]; j++) {
                    real_t hx = fabs((*(spmatrix))->val[(i_darts_counter_temp397)][j]);
                    (*sumM) += hx;
                    if ((*(spmatrix))->jja[(i_darts_counter_temp397)][j]
                        == (i_darts_counter_temp397)) {
                        (*sumP) = (*(spmatrix))->val[(i_darts_counter_temp397)][j];
                        (*sumM) -= hx;
                    }
                }
            }
            (*(&(eMax_LocalReductionVar))) = (((*(&(eMax_LocalReductionVar))) < ((*sumP) + (*sumM)))
                    ? (*sumP) + (*sumM)
                    : (*(&(eMax_LocalReductionVar))));
            (*(&(eMin_LocalReductionVar))) = (((*(&(eMin_LocalReductionVar))) > ((*sumP) - (*sumM)))
                    ? (*sumP) - (*sumM)
                    : (*(&(eMin_LocalReductionVar))));
        }
    }
updateReduceVars:
    /*Before signaling next codelet, check if all the threads in the TP
    * performed the reduction locally and if so, update the unique
    * reduction variable.*/
    /*Reducing for operator 'min'*/
    this->inputsTPParent->eMin_darts397_mutex.lock();
    if (*(this->inputsTPParent->eMin_darts397) > (eMin_LocalReductionVar))
        *(this->inputsTPParent->eMin_darts397) = (eMin_LocalReductionVar);
    this->inputsTPParent->eMin_darts397_mutex.unlock();
    /*Reducing for operator 'max'*/
    this->inputsTPParent->eMax_darts397_mutex.lock();
    if (*(this->inputsTPParent->eMax_darts397) < (eMax_LocalReductionVar))
        *(this->inputsTPParent->eMax_darts397) = (eMax_LocalReductionVar);
    this->inputsTPParent->eMax_darts397_mutex.unlock();
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets397[0].decDep();
}
TP397::TP397(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet, int in_initIteration,
    int in_lastIteration, real_t* in_eMax, real_t* in_eMin, int* in_hsize,
    struct SparseMatrixSt** in_spmatrix)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , eMax_darts397(in_eMax) /*OMP_SHARED - INPUT*/
    , eMin_darts397(in_eMin) /*OMP_SHARED - INPUT*/
    , hsize_darts397(in_hsize) /*OMP_SHARED - INPUT*/
    , spmatrix_darts397(in_spmatrix) /*OMP_SHARED - INPUT*/
    , sumM_darts397(new real_t[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , sumP_darts397(new real_t[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , i_darts397(new int[this->numThreads]) /*VARIABLE*/
    , initIteration397(in_initIteration)
    , lastIteration397(in_lastIteration)
    , barrierCodelets397(new _barrierCodelets397[1])
    , checkInCodelets398(new _checkInCodelets398[this->numThreads])
{
    /*Initialize the loop parameters*/
    range397 = abs(lastIteration397 - initIteration397) / 1;
    rangePerCodelet397 = range397 / numThreads;
    minIteration397 = min<int>(lastIteration397, initIteration397);
    remainderRange397 = range397 % numThreads;
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets397[0] = _barrierCodelets397(ompNumThreads, ompNumThreads, this, 0);
    _checkInCodelets398* checkInCodelets398Ptr = (this->checkInCodelets398);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets398Ptr) = _checkInCodelets398(1, 1, this, codeletCounter);
        (*checkInCodelets398Ptr).decDep();
        checkInCodelets398Ptr++;
    }
}
TP397::~TP397()
{
    delete[] i_darts397;
    delete[] barrierCodelets397;
    delete[] checkInCodelets398;
}
/*TP477: OMPParallelForDirective*/
void TP477::_barrierCodelets477::fire(void)
{
    TP477* myTP = static_cast<TP477*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
bool TP477::requestNewRangeIterations477(int* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Static */
    /*Chunk = 0*/
    bool isThereNewIteration = false;
    {
        /*Static Scheduling*/
        int tempStartRange = rangePerCodelet477 * codeletID;
        int tempEndRange = rangePerCodelet477 * (codeletID + 1);
        if (remainderRange477 != 0) {
            if (codeletID < (uint32_t)remainderRange477) {
                tempStartRange += codeletID;
                tempEndRange += (codeletID + 1);
            } else {
                tempStartRange += remainderRange477;
                tempEndRange += remainderRange477;
            }
        }
        tempStartRange = tempStartRange * 1 + minIteration477;
        tempEndRange = tempEndRange * 1 + minIteration477;
        if (tempStartRange != tempEndRange) {
            isThereNewIteration = true;
        }
        if (initIteration477 < lastIteration477) {
            (this->inputsTPParent->i_darts477[codeletID]) = min(tempStartRange, tempEndRange);
            *endRange = max(tempStartRange, tempEndRange);
        } else {
            (this->inputsTPParent->i_darts477[codeletID]) = max(tempStartRange, tempEndRange);
            *endRange = min(tempStartRange, tempEndRange);
        }
        if (codeletID == this->numThreads - 1) {
            *endRange = lastIteration477;
        }
    }
    return isThereNewIteration;
}
void TP477::_checkInCodelets478::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 478: ForStmt*/
    /*var: hsize*/
    /*var: maxIia*/
    /*var: spmatrix*/
    /*var: sumIia*/
    /*Declare local reduction vars*/
    int maxIia_LocalReductionVar = (int)(std::numeric_limits<int>::infinity() * (-1));
    int sumIia_LocalReductionVar = (int)0;
    struct SparseMatrixSt** spmatrix = (this->inputsTPParent->spmatrix_darts477);
    (void)spmatrix /*OMP_SHARED*/;
    int* i = &(this->inputsTPParent->i_darts477[this->getLocalID()]);
    (void)i /*PRIVATE*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations477(
        (int*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        goto updateReduceVars;
    }
    for (int i_darts_counter_temp477 = (*i); i_darts_counter_temp477 < endRange
         && i_darts_counter_temp477 < this->inputsTPParent->lastIteration477;
         i_darts_counter_temp477++) {
        {
            {
                /*Loop's init*/
                int j = 0;
                for (; j < (*(spmatrix))->iia[(i_darts_counter_temp477)]; j++) {
                    if ((*(spmatrix))->jja[(i_darts_counter_temp477)][j]
                        == (i_darts_counter_temp477)) {
                        (*(spmatrix))->val[(i_darts_counter_temp477)][j]
                            = ((*(spmatrix))->maxEval
                                  - (*(spmatrix))->val[(i_darts_counter_temp477)][j])
                            / (*(spmatrix))->maxMinusMin;
                    } else {
                        (*(spmatrix))->val[(i_darts_counter_temp477)][j]
                            = -(*(spmatrix))->val[(i_darts_counter_temp477)][j]
                            / (*(spmatrix))->maxMinusMin;
                    }
                }
            }
            (*(&(sumIia_LocalReductionVar))) += (*(spmatrix))->iia[(i_darts_counter_temp477)];
            (*(&(maxIia_LocalReductionVar))) = (((*(&(maxIia_LocalReductionVar))))
                        > ((*(spmatrix))->iia[(i_darts_counter_temp477)])
                    ? ((*(&(maxIia_LocalReductionVar))))
                    : ((*(spmatrix))->iia[(i_darts_counter_temp477)]));
        }
    }
updateReduceVars:
    /*Before signaling next codelet, check if all the threads in the TP
    * performed the reduction locally and if so, update the unique
    * reduction variable.*/
    /*Reducing for operator 'max'*/
    this->inputsTPParent->maxIia_darts477_mutex.lock();
    if (*(this->inputsTPParent->maxIia_darts477) < (maxIia_LocalReductionVar))
        *(this->inputsTPParent->maxIia_darts477) = (maxIia_LocalReductionVar);
    this->inputsTPParent->maxIia_darts477_mutex.unlock();
    /*Reducing for operator '+'*/
    this->inputsTPParent->sumIia_darts477_mutex.lock();
    *(this->inputsTPParent->sumIia_darts477) += (sumIia_LocalReductionVar);
    this->inputsTPParent->sumIia_darts477_mutex.unlock();
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets477[0].decDep();
}
TP477::TP477(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet, int in_initIteration,
    int in_lastIteration, int* in_hsize, int* in_maxIia, struct SparseMatrixSt** in_spmatrix,
    int* in_sumIia)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , hsize_darts477(in_hsize) /*OMP_SHARED - INPUT*/
    , maxIia_darts477(in_maxIia) /*OMP_SHARED - INPUT*/
    , spmatrix_darts477(in_spmatrix) /*OMP_SHARED - INPUT*/
    , sumIia_darts477(in_sumIia) /*OMP_SHARED - INPUT*/
    , i_darts477(new int[this->numThreads]) /*VARIABLE*/
    , initIteration477(in_initIteration)
    , lastIteration477(in_lastIteration)
    , barrierCodelets477(new _barrierCodelets477[1])
    , checkInCodelets478(new _checkInCodelets478[this->numThreads])
{
    /*Initialize the loop parameters*/
    range477 = abs(lastIteration477 - initIteration477) / 1;
    rangePerCodelet477 = range477 / numThreads;
    minIteration477 = min<int>(lastIteration477, initIteration477);
    remainderRange477 = range477 % numThreads;
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets477[0] = _barrierCodelets477(ompNumThreads, ompNumThreads, this, 0);
    _checkInCodelets478* checkInCodelets478Ptr = (this->checkInCodelets478);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets478Ptr) = _checkInCodelets478(1, 1, this, codeletCounter);
        (*checkInCodelets478Ptr).decDep();
        checkInCodelets478Ptr++;
    }
}
TP477::~TP477()
{
    delete[] i_darts477;
    delete[] barrierCodelets477;
    delete[] checkInCodelets478;
}
/*TP565: OMPParallelForDirective*/
void TP565::_barrierCodelets565::fire(void)
{
    TP565* myTP = static_cast<TP565*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
bool TP565::requestNewRangeIterations565(int* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Static */
    /*Chunk = 0*/
    bool isThereNewIteration = false;
    {
        /*Static Scheduling*/
        int tempStartRange = rangePerCodelet565 * codeletID;
        int tempEndRange = rangePerCodelet565 * (codeletID + 1);
        if (remainderRange565 != 0) {
            if (codeletID < (uint32_t)remainderRange565) {
                tempStartRange += codeletID;
                tempEndRange += (codeletID + 1);
            } else {
                tempStartRange += remainderRange565;
                tempEndRange += remainderRange565;
            }
        }
        tempStartRange = tempStartRange * 1 + minIteration565;
        tempEndRange = tempEndRange * 1 + minIteration565;
        if (tempStartRange != tempEndRange) {
            isThereNewIteration = true;
        }
        if (initIteration565 < lastIteration565) {
            (this->inputsTPParent->i_darts565[codeletID]) = min(tempStartRange, tempEndRange);
            *endRange = max(tempStartRange, tempEndRange);
        } else {
            (this->inputsTPParent->i_darts565[codeletID]) = max(tempStartRange, tempEndRange);
            *endRange = min(tempStartRange, tempEndRange);
        }
        if (codeletID == this->numThreads - 1) {
            *endRange = lastIteration565;
        }
    }
    return isThereNewIteration;
}
void TP565::_checkInCodelets566::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 566: ForStmt*/
    /*var: domain*/
    /*var: spmatrix*/
    /*var: trace*/
    /*var: trace2*/
    /*Declare local reduction vars*/
    real_t trace2_LocalReductionVar = (real_t)0;
    real_t trace_LocalReductionVar = (real_t)0;
    struct SparseMatrixSt** spmatrix = (this->inputsTPParent->spmatrix_darts565);
    (void)spmatrix /*OMP_SHARED*/;
    int* i = &(this->inputsTPParent->i_darts565[this->getLocalID()]);
    (void)i /*PRIVATE*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations565(
        (int*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        goto updateReduceVars;
    }
    for (int i_darts_counter_temp565 = (*i); i_darts_counter_temp565 < endRange
         && i_darts_counter_temp565 < this->inputsTPParent->lastIteration565;
         i_darts_counter_temp565++) {
        {
            {
                /*Loop's init*/
                int j = 0;
                for (; j < (*(spmatrix))->iia[(i_darts_counter_temp565)]; j++) {
                    if ((i_darts_counter_temp565)
                        == (*(spmatrix))->jja[(i_darts_counter_temp565)][j]) {
                        (*(&(trace_LocalReductionVar)))
                            += (*(spmatrix))->val[(i_darts_counter_temp565)][j];
                        (*(&(trace2_LocalReductionVar)))
                            += (*(spmatrix))->val[(i_darts_counter_temp565)][j]
                            * (*(spmatrix))->val[(i_darts_counter_temp565)][j];
                    }
                }
            }
        }
    }
updateReduceVars:
    /*Before signaling next codelet, check if all the threads in the TP
    * performed the reduction locally and if so, update the unique
    * reduction variable.*/
    /*Reducing for operator '+'*/
    this->inputsTPParent->trace2_darts565_mutex.lock();
    *(this->inputsTPParent->trace2_darts565) += (trace2_LocalReductionVar);
    this->inputsTPParent->trace2_darts565_mutex.unlock();
    /*Reducing for operator '+'*/
    this->inputsTPParent->trace_darts565_mutex.lock();
    *(this->inputsTPParent->trace_darts565) += (trace_LocalReductionVar);
    this->inputsTPParent->trace_darts565_mutex.unlock();
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets565[0].decDep();
}
TP565::TP565(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet, int in_initIteration,
    int in_lastIteration, struct DomainSt** in_domain, struct SparseMatrixSt** in_spmatrix,
    real_t* in_trace, real_t* in_trace2)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , domain_darts565(in_domain) /*OMP_SHARED - INPUT*/
    , spmatrix_darts565(in_spmatrix) /*OMP_SHARED - INPUT*/
    , trace_darts565(in_trace) /*OMP_SHARED - INPUT*/
    , trace2_darts565(in_trace2) /*OMP_SHARED - INPUT*/
    , i_darts565(new int[this->numThreads]) /*VARIABLE*/
    , initIteration565(in_initIteration)
    , lastIteration565(in_lastIteration)
    , barrierCodelets565(new _barrierCodelets565[1])
    , checkInCodelets566(new _checkInCodelets566[this->numThreads])
{
    /*Initialize the loop parameters*/
    range565 = abs(lastIteration565 - initIteration565) / 1;
    rangePerCodelet565 = range565 / numThreads;
    minIteration565 = min<int>(lastIteration565, initIteration565);
    remainderRange565 = range565 % numThreads;
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets565[0] = _barrierCodelets565(ompNumThreads, ompNumThreads, this, 0);
    _checkInCodelets566* checkInCodelets566Ptr = (this->checkInCodelets566);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets566Ptr) = _checkInCodelets566(1, 1, this, codeletCounter);
        (*checkInCodelets566Ptr).decDep();
        checkInCodelets566Ptr++;
    }
}
TP565::~TP565()
{
    delete[] i_darts565;
    delete[] barrierCodelets565;
    delete[] checkInCodelets566;
}
