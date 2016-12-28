#include "matmul.output.darts.h"
using namespace darts;
using namespace std;
/*Function: functB, ID: 1*/
int functB()
{
    /*functB:1*/
    /*CompoundStmt:7*/
    return 1;
}
/*Function: main, ID: 2*/
int main(int argc, char** argv)
{
    getOMPNumThreads();
    getOMPSchedulePolicy();
    getNumTPs();
    affin = new ThreadAffinity(ompNumThreads / NUMTPS - 1, NUMTPS, COMPACT, getDARTSTPPolicy(), getDARTSMCPolicy());
    affinMaskRes = affin->generateMask();
    myDARTSRuntime = new Runtime(affin);
    RuntimeFinalCodelet = &(myDARTSRuntime->finalSignal); /*main:2*/
    /*CompoundStmt:9*/
    int M = 10;
    if (argc > 1) {
        M = atoi(argv[1]);
    }
    int N = M, P = M;
    MYTYPE* A = (double*)malloc(M * N * sizeof(double));
    double* B = (double*)malloc(N * P * sizeof(double));
    double* C = (double*)malloc(M * P * sizeof(double));
    double* Cp = (double*)malloc(M * P * sizeof(double));
    int i, j, k;
    for (i = 0; i < M; i++) {
        for (j = 0; j < N; j++) {
            *(A + i * N + j) = i + j + 1;
        }
    }
    for (i = 0; i < N; i++) {
        for (j = 0; j < P; j++) {
            *(B + i * P + j) = i - j + 3;
        }
    }
    for (i = 0; i < M; i++) {
        for (j = 0; j < P; j++) {
            *(C + i * P + j) = 0;
            *(Cp + i * P + j) = 0;
        }
    }
    for (i = 0; i < M; i++) {
        for (j = 0; j < P; j++) {
            for (k = 0; k < N; k++) {
                *(C + i * P + j) += *(A + i * N + k) * *(B + k * P + j);
            }
        }
    }
    int n = 0;
    int good = 1;
    while (n < 10 && functB() && good) {
        /*CompoundStmt:133*/
        for (i = 0; i < M; i++) {
            for (j = 0; j < P; j++) {
                *(Cp + i * P + j) = 0;
            }
        }
        double zero = 0;
        if (affinMaskRes) {
            myDARTSRuntime->run(launch<TP150>(ompNumThreads * DARTS_CODELETS_MULT, 0, RuntimeFinalCodelet, (MYTYPE**)&((A)), (double**)&((B)), (double**)&((Cp)), (int*)&((M)), (int*)&((N)), (int*)&((P)), (double*)&((zero))));
        }
        /*OMPSingleDirective:219*/
        {
            /*CompoundStmt:220*/
            for (i = 0; i < M && good; i++) {
                for (j = 0; j < P; j++) {
                    if (*(C + i * P + j) != *(Cp + i * P + j)) {
                        printf("\nError: C[%d][%d]=%f, Cp[%d][%d]=%f\n", i, j, *(C + i * P + j), i, j, *(Cp + i * P + j));
                        good = 0;
                        break;
                    }
                }
            }
        }
        n++;
    }
    return 0;
}
/*TP150: OMPParallelDirective*/
void TP150::_barrierCodelets150::fire(void)
{
    TP150* myTP = static_cast<TP150*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
void TP150::_checkInCodelets154::fire(void)
{
    TP150* myTP = static_cast<TP150*>(myTP_);
    (void)myTP;
    /*Check for the loop's range and modify the number and size of TPs to launch*/
    if (!__sync_val_compare_and_swap(&(myTP->numTPsSet154), 0, 1)) {
        int range = (*(myTP->inputsTPParent->M_darts150)) - 0;
        if (range < RANGELIMIT154_0) {
            myTP->codeletsPerTP154 = 1;
            myTP->TPsToUse154 = 1;
        } else if (range < RANGELIMIT154_1) {
            myTP->codeletsPerTP154 /= 2;
            myTP->TPsToUse154 /= 2;
            if (myTP->codeletsPerTP154 <= 0)
                myTP->codeletsPerTP154 = 1;
            if (myTP->TPsToUse154 <= 0)
                myTP->TPsToUse154 = 1;
        }
        myTP->barrierCodelets154[0].setDep(myTP->TPsToUse154);
        myTP->totalCodelets154 = myTP->TPsToUse154 * myTP->codeletsPerTP154;
        myTP->numTPsReady154 = 1;
    }
    /*Reset if the number and size of TPs are not ready yet*/
    if (myTP->numTPsReady154 == 0) {
        this->resetCodelet();
        this->decDep();
        return;
    }
    /*Determine the TP to which this codelet belongs and check if this codelet spawns the TP or if it signals it using dispatchCodelet()*/
    size_t idx = this->getID() / myTP->codeletsPerTP154;
    if (idx < myTP->TPsToUse154) {
        if (!__sync_val_compare_and_swap(&(myTP->TP154_alreadyLaunched[idx]), 0, 1)) {
            int range = abs((*(myTP->inputsTPParent->M_darts150)) - 0) / 1;
            int rangePerCodelet = range / myTP->TPsToUse154;
            int minIteration = min<int>((*(myTP->inputsTPParent->M_darts150)), 0);
            int remainderRange = range % myTP->TPsToUse154;
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
            if (0 < (*(myTP->inputsTPParent->M_darts150))) {
                initIteration = min(initIteration, lastIteration);
                lastIteration = max(initIteration, lastIteration);
            } else {
                initIteration = max(initIteration, lastIteration);
                lastIteration = min(initIteration, lastIteration);
            }
            if (idx == myTP->TPsToUse154 - 1) {
                lastIteration = (*(myTP->inputsTPParent->M_darts150));
            }
#if USEINVOKE == 1
            invoke<TP154>(myTP, myTP->codeletsPerTP154 * DARTS_CODELETS_MULT, this->getID(), myTP, initIteration, lastIteration, &(*(myTP->inputsTPParent->A_darts150)), &(*(myTP->inputsTPParent->B_darts150)), &(*(myTP->inputsTPParent->Cp_darts150)), &(*(myTP->inputsTPParent->M_darts150)), &(*(myTP->inputsTPParent->N_darts150)), &(*(myTP->inputsTPParent->P_darts150)), &(*(myTP->inputsTPParent->zero_darts150)), &(myTP->TP154Ptr[idx]));
#else
            place<TP154>(idx, myTP, myTP->codeletsPerTP154 * DARTS_CODELETS_MULT, this->getID(), myTP, initIteration, lastIteration, &(*(myTP->inputsTPParent->A_darts150)), &(*(myTP->inputsTPParent->B_darts150)), &(*(myTP->inputsTPParent->Cp_darts150)), &(*(myTP->inputsTPParent->M_darts150)), &(*(myTP->inputsTPParent->N_darts150)), &(*(myTP->inputsTPParent->P_darts150)), &(*(myTP->inputsTPParent->zero_darts150)), &(myTP->TP154Ptr[idx]));
#endif
        } else {
            if (myTP->TP154Ptr[idx] != nullptr) {
                myTP->TP154Ptr[idx]->dispatchCodelet(this->getID());
            } else {
                this->resetCodelet();
                this->decDep();
            }
        }
    }
}
void TP150::_barrierCodelets154::fire(void)
{
    TP150* myTP = static_cast<TP150*>(myTP_);
    {
        for (size_t codeletsCounter = 0; codeletsCounter < myTP->numThreads; codeletsCounter++) {
            myTP->checkInCodelets210[codeletsCounter].decDep();
        }
    }
}
void TP150::_checkInCodelets210::fire(void)
{
    TP150* myTP = static_cast<TP150*>(myTP_);
    (void)myTP;

    /*printing node 210: IfStmt*/
    if ((*(myTP->inputsTPParent->zero_darts150)) > 1) {
        (myTP->inputsTPParent->xx_darts150[this->getID()]) = (*(myTP->inputsTPParent->zero_darts150));
    }

    /*printing node 213: DeclStmt*/

    /*printing node 214: BinaryOperator*/
    (myTP->inputsTPParent->yy_darts150[this->getID()]) = (myTP->inputsTPParent->xx_darts150[this->getID()]);

    /*printing node 215: DeclStmt*/
    myTP->inputsTPParent->one_darts150[this->getID()] = (*(myTP->inputsTPParent->zero_darts150)) + 1 + (myTP->inputsTPParent->yy_darts150[this->getID()]);

    /*printing node 218: CStyleCastExpr*/
    (void)(myTP->inputsTPParent->one_darts150[this->getID()]);
    /*Signaling next codelet from last stmt in the codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets150[0].decDep();
}
TP150::TP150(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet, MYTYPE** in_A, double** in_B, double** in_Cp, int* in_M, int* in_N, int* in_P, double* in_zero)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , A_darts150(in_A)
    , B_darts150(in_B)
    , Cp_darts150(in_Cp)
    , M_darts150(in_M)
    , N_darts150(in_N)
    , P_darts150(in_P)
    , zero_darts150(in_zero)
    , i_darts150(new int[this->numThreads])
    , j_darts150(new int[this->numThreads])
    , k_darts150(new int[this->numThreads])
    , one_darts150(new double[this->numThreads])
    , xx_darts150(new double[this->numThreads])
    , yy_darts150(new double[this->numThreads])
    , TP154Ptr(new TP154*[NUMTPS154])
    , TP154_alreadyLaunched(new size_t[NUMTPS154])
    , numTPsSet154(0)
    , numTPsReady154(0)
    , TPsToUse154(NUMTPS154)
    , codeletsPerTP154(this->numThreads / NUMTPS154)
    , totalCodelets154(this->TPsToUse154 * this->codeletsPerTP154)
    , barrierCodelets150(new _barrierCodelets150[1])
    , checkInCodelets154(new _checkInCodelets154[this->numThreads])
    , barrierCodelets154(new _barrierCodelets154[1])
    , checkInCodelets210(new _checkInCodelets210[this->numThreads])
{
    barrierCodelets150[0] = _barrierCodelets150(ompNumThreads, ompNumThreads, this, 0);
    barrierCodelets154[0] = _barrierCodelets154(NUMTPS154, NUMTPS154, this, 0);
    _checkInCodelets210* checkInCodelets210Ptr = (this->checkInCodelets210);
    _checkInCodelets154* checkInCodelets154Ptr = (this->checkInCodelets154);
    for (int i = 0; i < NUMTPS154; i++) {
        TP154Ptr[i] = nullptr;
        TP154_alreadyLaunched[i] = 0;
    }
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets210Ptr) = _checkInCodelets210(1, 1, this, codeletCounter);
        checkInCodelets210Ptr++;
        (*checkInCodelets154Ptr) = _checkInCodelets154(1, 1, this, codeletCounter);
        (*checkInCodelets154Ptr).decDep();
        checkInCodelets154Ptr++;
    }
}
/*childRegions 2
OMPForDirective:154 , IfStmt:210 , */
TP150::~TP150()
{
    delete[] i_darts150;
    delete[] j_darts150;
    delete[] k_darts150;
    delete[] one_darts150;
    delete[] xx_darts150;
    delete[] yy_darts150;
    delete[] barrierCodelets150;
    delete[] checkInCodelets210;
    delete[] barrierCodelets154;
    delete[] checkInCodelets154;
}
/*TP154: OMPForDirective*/
void TP154::_barrierCodelets154::fire(void)
{
    TP154* myTP = static_cast<TP154*>(myTP_);
    myTP->controlTPParent->TPParent->barrierCodelets154[0].decDep();
}
bool TP154::requestNewRangeIterations154(int* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Static */
    /*Chunk = 0*/
    bool isThereNewIteration = false;
    {
        /*Static Scheduling*/
        int tempStartRange = rangePerCodelet154 * codeletID;
        int tempEndRange = rangePerCodelet154 * (codeletID + 1);
        if (remainderRange154 != 0) {
            if (codeletID < (uint32_t)remainderRange154) {
                tempStartRange += codeletID;
                tempEndRange += (codeletID + 1);
            } else {
                tempStartRange += remainderRange154;
                tempEndRange += remainderRange154;
            }
        }
        tempStartRange = tempStartRange * 1 + minIteration154;
        tempEndRange = tempEndRange * 1 + minIteration154;
        if (tempStartRange != tempEndRange) {
            isThereNewIteration = true;
        }
        if (initIteration154 < lastIteration154) {
            (this->inputsTPParent->i_darts154[codeletID]) = min(tempStartRange, tempEndRange);
            *endRange = max(tempStartRange, tempEndRange);
        } else {
            (this->inputsTPParent->i_darts154[codeletID]) = max(tempStartRange, tempEndRange);
            *endRange = min(tempStartRange, tempEndRange);
        }
        if (codeletID == this->numThreads - 1) {
            *endRange = lastIteration154;
        }
    }
    return isThereNewIteration;
}
void TP154::_checkInCodelets155::fire(void)
{
    TP154* myTP = static_cast<TP154*>(myTP_);
    (void)myTP;
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif

    /*printing node 155: ForStmt*/
    MYTYPE** A = (myTP->inputsTPParent->A_darts154);
    (void)A /*OMP_SHARED*/;
    double** B = (myTP->inputsTPParent->B_darts154);
    (void)B /*OMP_SHARED*/;
    double** Cp = (myTP->inputsTPParent->Cp_darts154);
    (void)Cp /*OMP_SHARED*/;
    int* M = (myTP->inputsTPParent->M_darts154);
    (void)M /*OMP_SHARED*/;
    int* N = (myTP->inputsTPParent->N_darts154);
    (void)N /*OMP_SHARED*/;
    int* P = (myTP->inputsTPParent->P_darts154);
    (void)P /*OMP_SHARED*/;
    int* i = &(myTP->inputsTPParent->i_darts154[this->getLocalID()]);
    (void)i /*OMP_PRIVATE*/;
    int* j = &(myTP->inputsTPParent->j_darts154[this->getLocalID()]);
    (void)j /*OMP_PRIVATE*/;
    int* k = &(myTP->inputsTPParent->k_darts154[this->getLocalID()]);
    (void)k /*OMP_PRIVATE*/;
    double* zero = &(myTP->inputsTPParent->zero_darts154[this->getLocalID()]);
    (void)zero /*OMP_FIRSTPRIVATE*/;
    bool isThereNewIteration = myTP->inputsTPParent->requestNewRangeIterations154((int*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        myTP->controlTPParent->barrierCodelets154[0].decDep();
        return;
    }
    for (int i_darts_counter_temp154 = (*i); i_darts_counter_temp154 < endRange && i_darts_counter_temp154 < myTP->inputsTPParent->lastIteration154; i_darts_counter_temp154++) {
        {
            {
                (*j) = 0;
                int j_darts_counter_temp154 = (*j);
                for (; j_darts_counter_temp154 < (*(P)); j_darts_counter_temp154++) /*node->getLoopInfo()->localLoopVarStr: j_darts_counter_temp154*/
                {
                    {
                        (*k) = 0;
                        int k_darts_counter_temp154 = (*k);
                        for (; k_darts_counter_temp154 < (*(N)); k_darts_counter_temp154++) /*node->getLoopInfo()->localLoopVarStr: k_darts_counter_temp154*/
                        {
                            double temp = (*zero);
                            temp += *((*(A)) + ((i_darts_counter_temp154)) * (*(M)) + (k_darts_counter_temp154)) * *((*(B)) + (k_darts_counter_temp154) * (*(P)) + (j_darts_counter_temp154));
                            *((*(Cp)) + ((i_darts_counter_temp154)) * (*(M)) + (j_darts_counter_temp154)) += temp;
                        }
                        (*k) = k_darts_counter_temp154;
                    }
                }
                (*j) = j_darts_counter_temp154;
            }
        }
    }
    /*Signaling next codelet from last stmt in the codelet*/
    myTP->controlTPParent->barrierCodelets154[0].decDep();
}
TP154::TP154(int in_numThreads, int in_mainCodeletID, TP150* in_TPParent, int in_initIteration, int in_lastIteration, MYTYPE** in_A, double** in_B, double** in_Cp, int* in_M, int* in_N, int* in_P, double* in_zero_outer154_ptr, TP154** in_ptrToThisTP)
    : ompTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , A_darts154(in_A)
    , B_darts154(in_B)
    , Cp_darts154(in_Cp)
    , M_darts154(in_M)
    , N_darts154(in_N)
    , P_darts154(in_P)
    , i_darts154(new int[this->numThreads])
    , j_darts154(new int[this->numThreads])
    , k_darts154(new int[this->numThreads])
    , zero_darts154(new double[this->numThreads])
    , zero_outer154_ptr(in_zero_outer154_ptr)
    , initIteration154(in_initIteration)
    , lastIteration154(in_lastIteration)
    , readyCodelets(0)
    , baseNumThreads(this->numThreads / DARTS_CODELETS_MULT)
    , barrierCodelets154(new _barrierCodelets154[1])
    , checkInCodelets155(new _checkInCodelets155[this->numThreads])
{
    /*Initialize the loop parameters*/
    range154 = abs(lastIteration154 - initIteration154) / 1;
    rangePerCodelet154 = range154 / numThreads;
    minIteration154 = min<int>(lastIteration154, initIteration154);
    remainderRange154 = range154 % numThreads;
    barrierCodelets154[0] = _barrierCodelets154(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets155* checkInCodelets155Ptr = (this->checkInCodelets155);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets155);
#endif
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->baseNumThreads; codeletCounter++) {
        zero_darts154[codeletCounter] = *in_zero_outer154_ptr;
        for (size_t i = 1; i < (size_t)DARTS_CODELETS_MULT; i++) {
            zero_darts154[codeletCounter + this->baseNumThreads * i] = *in_zero_outer154_ptr;
        }
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets155Ptr) = _checkInCodelets155(2, 1, this, codeletCounter);
#else
        (*checkInCodelets155Ptr) = _checkInCodelets155(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets155Ptr).decDep();
        checkInCodelets155Ptr++;
    }
    this->dispatchCodelet(this->getID());
    *(in_ptrToThisTP) = this;
}
void TP154::dispatchCodelet(size_t codeletID)
{
    int idx = codeletID / this->baseNumThreads;
    int localID = codeletID - this->baseNumThreads * idx;
    this->checkInCodelets155[localID].setID(codeletID);
    this->checkInCodelets155[localID].setLocalID(localID);
    /*Check if we want to replicate codelets*/
    for (size_t i = 1; i < (size_t)DARTS_CODELETS_MULT; i++) {
#if USE_SPIN_CODELETS == 0
        this->checkInCodelets155[localID + this->baseNumThreads * i] = _checkInCodelets155(2, 1, this, localID + this->baseNumThreads * i);
#else
        this->checkInCodelets155[localID + this->baseNumThreads * i] = _checkInCodelets155(1, 1, this, localID + this->baseNumThreads * i);
#endif
        this->checkInCodelets155[localID + this->baseNumThreads * i].setID(codeletID);
        this->checkInCodelets155[localID + this->baseNumThreads * i].decDep();
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
/*childRegions 1
ForStmt:155 , */
TP154::~TP154()
{
    delete[] i_darts154;
    delete[] j_darts154;
    delete[] k_darts154;
    delete[] zero_darts154;
    delete[] barrierCodelets154;
    delete[] checkInCodelets155;
}
