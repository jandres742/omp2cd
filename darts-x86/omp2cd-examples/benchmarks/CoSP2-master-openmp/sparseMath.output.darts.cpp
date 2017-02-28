#include "sparseMath.output.darts.h"
using namespace darts;
using namespace std;
/*Function: sparseX2, ID: 92*/
void sparseX2(real_t* trX, real_t* trX2, struct SparseMatrixSt* xmatrix,
    struct SparseMatrixSt* x2matrix, struct DomainSt* domain)
{
    /*sparseX2:92*/
    /*CompoundStmt:624*/
    int hsize = xmatrix->hsize;
    int ix[hsize];
    real_t x[hsize];
    real_t traceX = ZERO;
    real_t traceX2 = ZERO;
    memset(ix, 0, hsize * sizeof(int));
    memset(x, ZERO, hsize * sizeof(real_t));
    int domain_localRowMin = domain->localRowMin;
    int domain_localRowMax = domain->localRowMax;
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP641>(ompNumThreads * DARTS_CODELETS_MULT, 0,
            RuntimeFinalCodelet, domain_localRowMin, domain_localRowMax,
            (int*)&((domain_localRowMax)), (int*)&((domain_localRowMin)), (int*)(ix), (hsize),
            (real_t*)&((traceX)), (real_t*)&((traceX2)), (real_t*)(x), (hsize),
            (struct SparseMatrixSt**)&((x2matrix)), (struct SparseMatrixSt**)&((xmatrix))));
    }
    *trX = traceX;
    *trX2 = traceX2;
}
/*Function: sparseAdd, ID: 93*/
void sparseAdd(
    struct SparseMatrixSt* xmatrix, struct SparseMatrixSt* x2matrix, struct DomainSt* domain)
{
    /*sparseAdd:93*/
    /*CompoundStmt:772*/
    int domain_localRowMax = domain->localRowMax;
    int domain_localRowMin = domain->localRowMin;
    int hsize = xmatrix->hsize;
    int ix[hsize];
    real_t x[hsize];
    memset(ix, 0, hsize * sizeof(int));
    memset(x, ZERO, hsize * sizeof(real_t));
    if (affinMaskRes) {
        myDARTSRuntime->run(
            launch<TP787>(ompNumThreads * DARTS_CODELETS_MULT, 0, RuntimeFinalCodelet,
                domain_localRowMin, domain_localRowMax, (int*)&((domain_localRowMax)),
                (int*)&((domain_localRowMin)), (int*)(ix), (hsize), (real_t*)(x), (hsize),
                (struct SparseMatrixSt**)&((x2matrix)), (struct SparseMatrixSt**)&((xmatrix))));
    }
}
/*Function: sparseSetX2, ID: 94*/
void sparseSetX2(
    struct SparseMatrixSt* xmatrix, struct SparseMatrixSt* x2matrix, struct DomainSt* domain)
{
    /*sparseSetX2:94*/
    /*CompoundStmt:923*/
    int hsize = xmatrix->hsize;
    int domain_localRowMin = domain->localRowMin;
    int domain_localRowMax = domain->localRowMax;
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP930>(ompNumThreads * DARTS_CODELETS_MULT, 0,
            RuntimeFinalCodelet, domain_localRowMin, domain_localRowMax,
            (int*)&((domain_localRowMax)), (int*)&((domain_localRowMin)),
            (struct SparseMatrixSt**)&((x2matrix)), (struct SparseMatrixSt**)&((xmatrix))));
    }
}
/*Function: sparseMultScalar, ID: 95*/
void sparseMultScalar(struct SparseMatrixSt* xmatrix, struct DomainSt* domain, real_t scalar)
{
    /*sparseMultScalar:95*/
    /*CompoundStmt:991*/
    int domain_localRowMin = domain->localRowMin;
    int domain_localRowMax = domain->localRowMax;
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP996>(ompNumThreads * DARTS_CODELETS_MULT, 0,
            RuntimeFinalCodelet, domain_localRowMin, domain_localRowMax,
            (int*)&((domain_localRowMax)), (int*)&((domain_localRowMin)), (real_t*)&((scalar)),
            (struct SparseMatrixSt**)&((xmatrix))));
    }
}
/*TP641: OMPParallelForDirective*/
void TP641::_barrierCodelets641::fire(void)
{
    TP641* myTP = static_cast<TP641*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
bool TP641::requestNewRangeIterations641(int* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Static */
    /*Chunk = 0*/
    bool isThereNewIteration = false;
    {
        /*Static Scheduling*/
        int tempStartRange = rangePerCodelet641 * codeletID;
        int tempEndRange = rangePerCodelet641 * (codeletID + 1);
        if (remainderRange641 != 0) {
            if (codeletID < (uint32_t)remainderRange641) {
                tempStartRange += codeletID;
                tempEndRange += (codeletID + 1);
            } else {
                tempStartRange += remainderRange641;
                tempEndRange += remainderRange641;
            }
        }
        tempStartRange = tempStartRange * 1 + minIteration641;
        tempEndRange = tempEndRange * 1 + minIteration641;
        if (tempStartRange != tempEndRange) {
            isThereNewIteration = true;
        }
        if (initIteration641 < lastIteration641) {
            (this->inputsTPParent->i_darts641[codeletID]) = min(tempStartRange, tempEndRange);
            *endRange = max(tempStartRange, tempEndRange);
        } else {
            (this->inputsTPParent->i_darts641[codeletID]) = max(tempStartRange, tempEndRange);
            *endRange = min(tempStartRange, tempEndRange);
        }
        if (codeletID == this->numThreads - 1) {
            *endRange = lastIteration641;
        }
    }
    return isThereNewIteration;
}
void TP641::_checkInCodelets642::fire(void)
{
    /*Init the vars for this region*/
    this->inputsTPParent->ix_darts641[this->getID()]
        = (int*)malloc(sizeof(int) * this->inputsTPParent->ix_outer641_size);
    int* temp_ix_darts_ptr = (int*)(this->inputsTPParent->ix_darts641[this->getID()]);
    int* temp_ix_outer_ptr = (int*)(this->inputsTPParent->ix_outer641_ptr);
    for (uint64_t k_outer641 = 0; k_outer641 < this->inputsTPParent->ix_outer641_size;
         k_outer641++) {
        temp_ix_darts_ptr[k_outer641] = temp_ix_outer_ptr[k_outer641];
    }
    this->inputsTPParent->x_darts641[this->getID()]
        = (real_t*)malloc(sizeof(real_t) * this->inputsTPParent->x_outer641_size);
    real_t* temp_x_darts_ptr = (real_t*)(this->inputsTPParent->x_darts641[this->getID()]);
    real_t* temp_x_outer_ptr = (real_t*)(this->inputsTPParent->x_outer641_ptr);
    for (uint64_t k_outer641 = 0; k_outer641 < this->inputsTPParent->x_outer641_size;
         k_outer641++) {
        temp_x_darts_ptr[k_outer641] = temp_x_outer_ptr[k_outer641];
    }

    /*printing node 642: ForStmt*/
    /*var: domain_localRowMax*/
    /*var: domain_localRowMin*/
    /*var: ix*/
    /*var: traceX*/
    /*var: traceX2*/
    /*var: x*/
    /*var: x2matrix*/
    /*var: xmatrix*/
    /*Declare local reduction vars*/
    real_t traceX2_LocalReductionVar = (real_t)0;
    real_t traceX_LocalReductionVar = (real_t)0;
    int** ix = &(this->inputsTPParent->ix_darts641[this->getLocalID()]);
    (void)ix /*OMP_FIRSTPRIVATE*/;
    real_t** x = &(this->inputsTPParent->x_darts641[this->getLocalID()]);
    (void)x /*OMP_FIRSTPRIVATE*/;
    struct SparseMatrixSt** x2matrix = (this->inputsTPParent->x2matrix_darts641);
    (void)x2matrix /*OMP_SHARED*/;
    struct SparseMatrixSt** xmatrix = (this->inputsTPParent->xmatrix_darts641);
    (void)xmatrix /*OMP_SHARED*/;
    int* i = &(this->inputsTPParent->i_darts641[this->getLocalID()]);
    (void)i /*PRIVATE*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations641(
        (int*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        goto updateReduceVars;
    }
    for (int i_darts_counter_temp641 = (*i); i_darts_counter_temp641 < endRange
         && i_darts_counter_temp641 < this->inputsTPParent->lastIteration641;
         i_darts_counter_temp641++) {
        {
            int l = 0;
            {
                /*Loop's init*/
                int jp = 0;
                for (; jp < (*(xmatrix))->iia[(i_darts_counter_temp641)]; jp++) {
                    real_t a = (*(xmatrix))->val[(i_darts_counter_temp641)][jp];
                    int j = (*(xmatrix))->jja[(i_darts_counter_temp641)][jp];
                    if (j == (i_darts_counter_temp641)) {
                        (*(&(traceX_LocalReductionVar))) += a;
                    }
                    {
                        /*Loop's init*/
                        int kp = 0;
                        for (; kp < (*(xmatrix))->iia[j]; kp++) {
                            int k = (*(xmatrix))->jja[j][kp];
                            if ((*ix)[k] == 0) {
                                (*x)[k] = ZERO;
                                (*(x2matrix))->jja[(i_darts_counter_temp641)][l] = k;
                                (*ix)[k] = (i_darts_counter_temp641) + 1;
                                l++;
                            }
                            (*x)[k] = (*x)[k] + a * (*(xmatrix))->val[j][kp];
                        }
                    }
                }
            }
            int ll = 0;
            {
                /*Loop's init*/
                int j = 0;
                for (; j < l; j++) {
                    int jp = (*(x2matrix))->jja[(i_darts_counter_temp641)][j];
                    real_t xtmp = (*x)[jp];
                    if (jp == (i_darts_counter_temp641)) {
                        (*(&(traceX2_LocalReductionVar))) += xtmp;
                        (*(x2matrix))->val[(i_darts_counter_temp641)][ll] = xtmp;
                        (*(x2matrix))->jja[(i_darts_counter_temp641)][ll] = jp;
                        ll++;
                    } else {
                        if (fabs(xtmp) > eps) {
                            (*(x2matrix))->val[(i_darts_counter_temp641)][ll] = xtmp;
                            (*(x2matrix))->jja[(i_darts_counter_temp641)][ll] = jp;
                            ll++;
                        }
                    }
                    (*ix)[jp] = 0;
                    (*x)[jp] = ZERO;
                }
            }
            (*(x2matrix))->iia[(i_darts_counter_temp641)] = ll;
        }
    }
updateReduceVars:
    /*Before signaling next codelet, check if all the threads in the TP
    * performed the reduction locally and if so, update the unique
    * reduction variable.*/
    /*Reducing for operator '+'*/
    this->inputsTPParent->traceX2_darts641_mutex.lock();
    *(this->inputsTPParent->traceX2_darts641) += (traceX2_LocalReductionVar);
    this->inputsTPParent->traceX2_darts641_mutex.unlock();
    /*Reducing for operator '+'*/
    this->inputsTPParent->traceX_darts641_mutex.lock();
    *(this->inputsTPParent->traceX_darts641) += (traceX_LocalReductionVar);
    this->inputsTPParent->traceX_darts641_mutex.unlock();
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets641[0].decDep();
}
TP641::TP641(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet, int in_initIteration,
    int in_lastIteration, int* in_domain_localRowMax, int* in_domain_localRowMin,
    int* in_ix_outer641_ptr, uint64_t in_ix_outer641_size, real_t* in_traceX, real_t* in_traceX2,
    real_t* in_x_outer641_ptr, uint64_t in_x_outer641_size, struct SparseMatrixSt** in_x2matrix,
    struct SparseMatrixSt** in_xmatrix)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , domain_localRowMax_darts641(in_domain_localRowMax) /*OMP_SHARED - INPUT*/
    , domain_localRowMin_darts641(in_domain_localRowMin) /*OMP_SHARED - INPUT*/
    , ix_outer641_size(in_ix_outer641_size)
    , ix_outer641_ptr(in_ix_outer641_ptr)
    , traceX_darts641(in_traceX) /*OMP_SHARED - INPUT*/
    , traceX2_darts641(in_traceX2) /*OMP_SHARED - INPUT*/
    , x_outer641_size(in_x_outer641_size)
    , x_outer641_ptr(in_x_outer641_ptr)
    , x2matrix_darts641(in_x2matrix) /*OMP_SHARED - INPUT*/
    , xmatrix_darts641(in_xmatrix) /*OMP_SHARED - INPUT*/
    , i_darts641(new int[this->numThreads]) /*VARIABLE*/
    , initIteration641(in_initIteration)
    , lastIteration641(in_lastIteration)
    , barrierCodelets641(new _barrierCodelets641[1])
    , checkInCodelets642(new _checkInCodelets642[this->numThreads])
{
    /*Initialize the loop parameters*/
    range641 = abs(lastIteration641 - initIteration641) / 1;
    rangePerCodelet641 = range641 / numThreads;
    minIteration641 = min<int>(lastIteration641, initIteration641);
    remainderRange641 = range641 % numThreads;
    /*Initialize inputs and vars.*/
    this->ix_darts641 = (int**)malloc(sizeof(int*) * this->numThreads); /*OMP_FIRSTPRIVATE*/
    this->x_darts641 = (real_t**)malloc(sizeof(real_t*) * this->numThreads); /*OMP_FIRSTPRIVATE*/
    /*Initialize Codelets*/
    barrierCodelets641[0] = _barrierCodelets641(ompNumThreads, ompNumThreads, this, 0);
    _checkInCodelets642* checkInCodelets642Ptr = (this->checkInCodelets642);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets642Ptr) = _checkInCodelets642(1, 1, this, codeletCounter);
        (*checkInCodelets642Ptr).decDep();
        checkInCodelets642Ptr++;
    }
}
TP641::~TP641()
{
    delete[] ix_darts641;
    delete[] x_darts641;
    delete[] i_darts641;
    delete[] barrierCodelets641;
    delete[] checkInCodelets642;
}
/*TP787: OMPParallelForDirective*/
void TP787::_barrierCodelets787::fire(void)
{
    TP787* myTP = static_cast<TP787*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
bool TP787::requestNewRangeIterations787(int* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Static */
    /*Chunk = 0*/
    bool isThereNewIteration = false;
    {
        /*Static Scheduling*/
        int tempStartRange = rangePerCodelet787 * codeletID;
        int tempEndRange = rangePerCodelet787 * (codeletID + 1);
        if (remainderRange787 != 0) {
            if (codeletID < (uint32_t)remainderRange787) {
                tempStartRange += codeletID;
                tempEndRange += (codeletID + 1);
            } else {
                tempStartRange += remainderRange787;
                tempEndRange += remainderRange787;
            }
        }
        tempStartRange = tempStartRange * 1 + minIteration787;
        tempEndRange = tempEndRange * 1 + minIteration787;
        if (tempStartRange != tempEndRange) {
            isThereNewIteration = true;
        }
        if (initIteration787 < lastIteration787) {
            (this->inputsTPParent->i_darts787[codeletID]) = min(tempStartRange, tempEndRange);
            *endRange = max(tempStartRange, tempEndRange);
        } else {
            (this->inputsTPParent->i_darts787[codeletID]) = max(tempStartRange, tempEndRange);
            *endRange = min(tempStartRange, tempEndRange);
        }
        if (codeletID == this->numThreads - 1) {
            *endRange = lastIteration787;
        }
    }
    return isThereNewIteration;
}
void TP787::_checkInCodelets788::fire(void)
{
    /*Init the vars for this region*/
    this->inputsTPParent->ix_darts787[this->getID()]
        = (int*)malloc(sizeof(int) * this->inputsTPParent->ix_outer787_size);
    int* temp_ix_darts_ptr = (int*)(this->inputsTPParent->ix_darts787[this->getID()]);
    int* temp_ix_outer_ptr = (int*)(this->inputsTPParent->ix_outer787_ptr);
    for (uint64_t k_outer787 = 0; k_outer787 < this->inputsTPParent->ix_outer787_size;
         k_outer787++) {
        temp_ix_darts_ptr[k_outer787] = temp_ix_outer_ptr[k_outer787];
    }
    this->inputsTPParent->x_darts787[this->getID()]
        = (real_t*)malloc(sizeof(real_t) * this->inputsTPParent->x_outer787_size);
    real_t* temp_x_darts_ptr = (real_t*)(this->inputsTPParent->x_darts787[this->getID()]);
    real_t* temp_x_outer_ptr = (real_t*)(this->inputsTPParent->x_outer787_ptr);
    for (uint64_t k_outer787 = 0; k_outer787 < this->inputsTPParent->x_outer787_size;
         k_outer787++) {
        temp_x_darts_ptr[k_outer787] = temp_x_outer_ptr[k_outer787];
    }

    /*printing node 788: ForStmt*/
    /*var: domain_localRowMax*/
    /*var: domain_localRowMin*/
    /*var: ix*/
    /*var: x*/
    /*var: x2matrix*/
    /*var: xmatrix*/
    int** ix = &(this->inputsTPParent->ix_darts787[this->getLocalID()]);
    (void)ix /*OMP_FIRSTPRIVATE*/;
    real_t** x = &(this->inputsTPParent->x_darts787[this->getLocalID()]);
    (void)x /*OMP_FIRSTPRIVATE*/;
    struct SparseMatrixSt** x2matrix = (this->inputsTPParent->x2matrix_darts787);
    (void)x2matrix /*OMP_SHARED*/;
    struct SparseMatrixSt** xmatrix = (this->inputsTPParent->xmatrix_darts787);
    (void)xmatrix /*OMP_SHARED*/;
    int* i = &(this->inputsTPParent->i_darts787[this->getLocalID()]);
    (void)i /*PRIVATE*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations787(
        (int*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        /*Find and signal the next codelet*/
        myTP->controlTPParent->TPParent->barrierCodelets787[0].decDep();
        return;
    }
    for (int i_darts_counter_temp787 = (*i); i_darts_counter_temp787 < endRange
         && i_darts_counter_temp787 < this->inputsTPParent->lastIteration787;
         i_darts_counter_temp787++) {
        {
            int l = 0;
            {
                /*Loop's init*/
                int jp = 0;
                for (; jp < (*(xmatrix))->iia[(i_darts_counter_temp787)]; jp++) {
                    int k = (*(xmatrix))->jja[(i_darts_counter_temp787)][jp];
                    if ((*ix)[k] == 0) {
                        (*x)[k] = ZERO;
                        (*ix)[k] = (i_darts_counter_temp787) + 1;
                        (*(xmatrix))->jja[(i_darts_counter_temp787)][l] = k;
                        l++;
                    }
                    (*x)[k] = (*x)[k] + TWO * (*(xmatrix))->val[(i_darts_counter_temp787)][jp];
                }
            }
            {
                /*Loop's init*/
                int jp = 0;
                for (; jp < (*(x2matrix))->iia[(i_darts_counter_temp787)]; jp++) {
                    int k = (*(x2matrix))->jja[(i_darts_counter_temp787)][jp];
                    if ((*ix)[k] == 0) {
                        (*x)[k] = ZERO;
                        (*ix)[k] = (i_darts_counter_temp787) + 1;
                        (*(xmatrix))->jja[(i_darts_counter_temp787)][l] = k;
                        l++;
                    }
                    (*x)[k] = (*x)[k] - (*(x2matrix))->val[(i_darts_counter_temp787)][jp];
                }
            }
            (*(xmatrix))->iia[(i_darts_counter_temp787)] = l;
            int ll = 0;
            {
                /*Loop's init*/
                int jp = 0;
                for (; jp < l; jp++) {
                    real_t xTmp = (*x)[(*(xmatrix))->jja[(i_darts_counter_temp787)][jp]];
                    if (fabs(xTmp) > eps) {
                        (*(xmatrix))->val[(i_darts_counter_temp787)][ll] = xTmp;
                        (*(xmatrix))->jja[(i_darts_counter_temp787)][ll]
                            = (*(xmatrix))->jja[(i_darts_counter_temp787)][jp];
                        ll++;
                    }
                    (*x)[(*(xmatrix))->jja[(i_darts_counter_temp787)][jp]] = ZERO;
                    (*ix)[(*(xmatrix))->jja[(i_darts_counter_temp787)][jp]] = 0;
                }
            }
            (*(xmatrix))->iia[(i_darts_counter_temp787)] = ll;
        }
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets787[0].decDep();
}
TP787::TP787(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet, int in_initIteration,
    int in_lastIteration, int* in_domain_localRowMax, int* in_domain_localRowMin,
    int* in_ix_outer787_ptr, uint64_t in_ix_outer787_size, real_t* in_x_outer787_ptr,
    uint64_t in_x_outer787_size, struct SparseMatrixSt** in_x2matrix,
    struct SparseMatrixSt** in_xmatrix)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , domain_localRowMax_darts787(in_domain_localRowMax) /*OMP_SHARED - INPUT*/
    , domain_localRowMin_darts787(in_domain_localRowMin) /*OMP_SHARED - INPUT*/
    , ix_outer787_size(in_ix_outer787_size)
    , ix_outer787_ptr(in_ix_outer787_ptr)
    , x_outer787_size(in_x_outer787_size)
    , x_outer787_ptr(in_x_outer787_ptr)
    , x2matrix_darts787(in_x2matrix) /*OMP_SHARED - INPUT*/
    , xmatrix_darts787(in_xmatrix) /*OMP_SHARED - INPUT*/
    , i_darts787(new int[this->numThreads]) /*VARIABLE*/
    , initIteration787(in_initIteration)
    , lastIteration787(in_lastIteration)
    , barrierCodelets787(new _barrierCodelets787[1])
    , checkInCodelets788(new _checkInCodelets788[this->numThreads])
{
    /*Initialize the loop parameters*/
    range787 = abs(lastIteration787 - initIteration787) / 1;
    rangePerCodelet787 = range787 / numThreads;
    minIteration787 = min<int>(lastIteration787, initIteration787);
    remainderRange787 = range787 % numThreads;
    /*Initialize inputs and vars.*/
    this->ix_darts787 = (int**)malloc(sizeof(int*) * this->numThreads); /*OMP_FIRSTPRIVATE*/
    this->x_darts787 = (real_t**)malloc(sizeof(real_t*) * this->numThreads); /*OMP_FIRSTPRIVATE*/
    /*Initialize Codelets*/
    barrierCodelets787[0] = _barrierCodelets787(ompNumThreads, ompNumThreads, this, 0);
    _checkInCodelets788* checkInCodelets788Ptr = (this->checkInCodelets788);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets788Ptr) = _checkInCodelets788(1, 1, this, codeletCounter);
        (*checkInCodelets788Ptr).decDep();
        checkInCodelets788Ptr++;
    }
}
TP787::~TP787()
{
    delete[] ix_darts787;
    delete[] x_darts787;
    delete[] i_darts787;
    delete[] barrierCodelets787;
    delete[] checkInCodelets788;
}
/*TP930: OMPParallelForDirective*/
void TP930::_barrierCodelets930::fire(void)
{
    TP930* myTP = static_cast<TP930*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
bool TP930::requestNewRangeIterations930(int* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Static */
    /*Chunk = 0*/
    bool isThereNewIteration = false;
    {
        /*Static Scheduling*/
        int tempStartRange = rangePerCodelet930 * codeletID;
        int tempEndRange = rangePerCodelet930 * (codeletID + 1);
        if (remainderRange930 != 0) {
            if (codeletID < (uint32_t)remainderRange930) {
                tempStartRange += codeletID;
                tempEndRange += (codeletID + 1);
            } else {
                tempStartRange += remainderRange930;
                tempEndRange += remainderRange930;
            }
        }
        tempStartRange = tempStartRange * 1 + minIteration930;
        tempEndRange = tempEndRange * 1 + minIteration930;
        if (tempStartRange != tempEndRange) {
            isThereNewIteration = true;
        }
        if (initIteration930 < lastIteration930) {
            (this->inputsTPParent->i_darts930[codeletID]) = min(tempStartRange, tempEndRange);
            *endRange = max(tempStartRange, tempEndRange);
        } else {
            (this->inputsTPParent->i_darts930[codeletID]) = max(tempStartRange, tempEndRange);
            *endRange = min(tempStartRange, tempEndRange);
        }
        if (codeletID == this->numThreads - 1) {
            *endRange = lastIteration930;
        }
    }
    return isThereNewIteration;
}
void TP930::_checkInCodelets931::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 931: ForStmt*/
    /*var: domain_localRowMax*/
    /*var: domain_localRowMin*/
    /*var: x2matrix*/
    /*var: xmatrix*/
    struct SparseMatrixSt** x2matrix = (this->inputsTPParent->x2matrix_darts930);
    (void)x2matrix /*OMP_SHARED*/;
    struct SparseMatrixSt** xmatrix = (this->inputsTPParent->xmatrix_darts930);
    (void)xmatrix /*OMP_SHARED*/;
    int* i = &(this->inputsTPParent->i_darts930[this->getLocalID()]);
    (void)i /*PRIVATE*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations930(
        (int*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        /*Find and signal the next codelet*/
        myTP->controlTPParent->TPParent->barrierCodelets930[0].decDep();
        return;
    }
    for (int i_darts_counter_temp930 = (*i); i_darts_counter_temp930 < endRange
         && i_darts_counter_temp930 < this->inputsTPParent->lastIteration930;
         i_darts_counter_temp930++) {
        {
            real_t xtmp;
            int ll = 0;
            {
                /*Loop's init*/
                int jp = 0;
                for (; jp < (*(x2matrix))->iia[(i_darts_counter_temp930)]; jp++) {
                    xtmp = (*(x2matrix))->val[(i_darts_counter_temp930)][jp];
                    if (fabs(xtmp) > eps) {
                        (*(xmatrix))->val[(i_darts_counter_temp930)][ll] = xtmp;
                        (*(xmatrix))->jja[(i_darts_counter_temp930)][ll]
                            = (*(x2matrix))->jja[(i_darts_counter_temp930)][jp];
                        ll++;
                    }
                }
            }
            (*(xmatrix))->iia[(i_darts_counter_temp930)] = ll;
        }
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets930[0].decDep();
}
TP930::TP930(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet, int in_initIteration,
    int in_lastIteration, int* in_domain_localRowMax, int* in_domain_localRowMin,
    struct SparseMatrixSt** in_x2matrix, struct SparseMatrixSt** in_xmatrix)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , domain_localRowMax_darts930(in_domain_localRowMax) /*OMP_SHARED - INPUT*/
    , domain_localRowMin_darts930(in_domain_localRowMin) /*OMP_SHARED - INPUT*/
    , x2matrix_darts930(in_x2matrix) /*OMP_SHARED - INPUT*/
    , xmatrix_darts930(in_xmatrix) /*OMP_SHARED - INPUT*/
    , i_darts930(new int[this->numThreads]) /*VARIABLE*/
    , initIteration930(in_initIteration)
    , lastIteration930(in_lastIteration)
    , barrierCodelets930(new _barrierCodelets930[1])
    , checkInCodelets931(new _checkInCodelets931[this->numThreads])
{
    /*Initialize the loop parameters*/
    range930 = abs(lastIteration930 - initIteration930) / 1;
    rangePerCodelet930 = range930 / numThreads;
    minIteration930 = min<int>(lastIteration930, initIteration930);
    remainderRange930 = range930 % numThreads;
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets930[0] = _barrierCodelets930(ompNumThreads, ompNumThreads, this, 0);
    _checkInCodelets931* checkInCodelets931Ptr = (this->checkInCodelets931);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets931Ptr) = _checkInCodelets931(1, 1, this, codeletCounter);
        (*checkInCodelets931Ptr).decDep();
        checkInCodelets931Ptr++;
    }
}
TP930::~TP930()
{
    delete[] i_darts930;
    delete[] barrierCodelets930;
    delete[] checkInCodelets931;
}
/*TP996: OMPParallelForDirective*/
void TP996::_barrierCodelets996::fire(void)
{
    TP996* myTP = static_cast<TP996*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
bool TP996::requestNewRangeIterations996(int* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Static */
    /*Chunk = 0*/
    bool isThereNewIteration = false;
    {
        /*Static Scheduling*/
        int tempStartRange = rangePerCodelet996 * codeletID;
        int tempEndRange = rangePerCodelet996 * (codeletID + 1);
        if (remainderRange996 != 0) {
            if (codeletID < (uint32_t)remainderRange996) {
                tempStartRange += codeletID;
                tempEndRange += (codeletID + 1);
            } else {
                tempStartRange += remainderRange996;
                tempEndRange += remainderRange996;
            }
        }
        tempStartRange = tempStartRange * 1 + minIteration996;
        tempEndRange = tempEndRange * 1 + minIteration996;
        if (tempStartRange != tempEndRange) {
            isThereNewIteration = true;
        }
        if (initIteration996 < lastIteration996) {
            (this->inputsTPParent->i_darts996[codeletID]) = min(tempStartRange, tempEndRange);
            *endRange = max(tempStartRange, tempEndRange);
        } else {
            (this->inputsTPParent->i_darts996[codeletID]) = max(tempStartRange, tempEndRange);
            *endRange = min(tempStartRange, tempEndRange);
        }
        if (codeletID == this->numThreads - 1) {
            *endRange = lastIteration996;
        }
    }
    return isThereNewIteration;
}
void TP996::_checkInCodelets997::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 997: ForStmt*/
    /*var: domain_localRowMax*/
    /*var: domain_localRowMin*/
    /*var: scalar*/
    /*var: xmatrix*/
    real_t* scalar = (this->inputsTPParent->scalar_darts996);
    (void)scalar /*OMP_SHARED*/;
    struct SparseMatrixSt** xmatrix = (this->inputsTPParent->xmatrix_darts996);
    (void)xmatrix /*OMP_SHARED*/;
    int* i = &(this->inputsTPParent->i_darts996[this->getLocalID()]);
    (void)i /*PRIVATE*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations996(
        (int*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        /*Find and signal the next codelet*/
        myTP->controlTPParent->TPParent->barrierCodelets996[0].decDep();
        return;
    }
    for (int i_darts_counter_temp996 = (*i); i_darts_counter_temp996 < endRange
         && i_darts_counter_temp996 < this->inputsTPParent->lastIteration996;
         i_darts_counter_temp996++) {
        {
            {
                /*Loop's init*/
                int j = 0;
                for (; j < (*(xmatrix))->iia[(i_darts_counter_temp996)]; j++) {
                    (*(xmatrix))->val[(i_darts_counter_temp996)][j] *= (*(scalar));
                }
            }
        }
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets996[0].decDep();
}
TP996::TP996(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet, int in_initIteration,
    int in_lastIteration, int* in_domain_localRowMax, int* in_domain_localRowMin, real_t* in_scalar,
    struct SparseMatrixSt** in_xmatrix)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , domain_localRowMax_darts996(in_domain_localRowMax) /*OMP_SHARED - INPUT*/
    , domain_localRowMin_darts996(in_domain_localRowMin) /*OMP_SHARED - INPUT*/
    , scalar_darts996(in_scalar) /*OMP_SHARED - INPUT*/
    , xmatrix_darts996(in_xmatrix) /*OMP_SHARED - INPUT*/
    , i_darts996(new int[this->numThreads]) /*VARIABLE*/
    , initIteration996(in_initIteration)
    , lastIteration996(in_lastIteration)
    , barrierCodelets996(new _barrierCodelets996[1])
    , checkInCodelets997(new _checkInCodelets997[this->numThreads])
{
    /*Initialize the loop parameters*/
    range996 = abs(lastIteration996 - initIteration996) / 1;
    rangePerCodelet996 = range996 / numThreads;
    minIteration996 = min<int>(lastIteration996, initIteration996);
    remainderRange996 = range996 % numThreads;
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets996[0] = _barrierCodelets996(ompNumThreads, ompNumThreads, this, 0);
    _checkInCodelets997* checkInCodelets997Ptr = (this->checkInCodelets997);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets997Ptr) = _checkInCodelets997(1, 1, this, codeletCounter);
        (*checkInCodelets997Ptr).decDep();
        checkInCodelets997Ptr++;
    }
}
TP996::~TP996()
{
    delete[] i_darts996;
    delete[] barrierCodelets996;
    delete[] checkInCodelets997;
}
