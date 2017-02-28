#include "copyprivate.output.darts.h"
using namespace darts;
using namespace std;
float fGlobal = 10;
/*Function: get_float, ID: 1*/
float get_float()
{
    /*get_float:1*/
    /*CompoundStmt:9*/
    fGlobal += 1;
    return fGlobal;
}
/*Function: main, ID: 3*/
int main()
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
    /*main:3*/
    /*CompoundStmt:31*/
    float a = 9.9900000000000002, b = 123.456;
    omp_set_num_threads(16);
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP36>(ompNumThreads * DARTS_CODELETS_MULT, 0,
            RuntimeFinalCodelet, (float*)&((a)), (float*)&((b))));
    }
    return 0;
}
/*TP2: TP_CopyPrivate*/
void TP2::_checkInCodelets13::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    /*Select the thread executing OMPSingleDirective 13*/
    if (!__sync_val_compare_and_swap(&(myTP->TP13_alreadyLaunched), 0, 1)) {
        /*Init the vars for this region*/
        /*Initialize the vars of the inlined region*/
        this->inputsTPParent->a_darts13
            = &(this->inputsTPParent
                    ->a_darts2[this->getLocalID()]) /*OMP_SHARED_PRIVATE - VAR INLINED*/;
        this->inputsTPParent->b_darts13
            = &(this->inputsTPParent
                    ->b_darts2[this->getLocalID()]) /*OMP_SHARED_PRIVATE - VAR INLINED*/;

        /*printing node 15: BinaryOperator*/
        (*(this->inputsTPParent->a_darts13)) = get_float();

        /*printing node 17: BinaryOperator*/
        (*(this->inputsTPParent->b_darts13)) = get_float();
        this->inputsTPParent->codeletIDexecutingSingle13 = this->getID();
        /*Signaling next codelet from last stmt in the codelet*/
        /*Signaling omp region's barrier*/
        myTP->controlTPParent->barrierCodelets13[0].decDep();
    } else {
        /*Signaling omp region's barrier*/
        myTP->barrierCodelets13[0].decDep();
    }
}
void TP2::_barrierCodelets13::fire(void)
{
    TP2* myTP = static_cast<TP2*>(myTP_);
    /*Copy the values calculated by the single region
    to the other threads' vars specified in the
    copyprivate clause*/
    if (this->inputsTPParent->codeletIDexecutingSingle13 < ompNumThreads) {
        float temp_b = *(this->inputsTPParent->b_darts13);
        float temp_a = *(this->inputsTPParent->a_darts13);
        for (size_t codeletCounter = 0; codeletCounter < this->inputsTPParent->numThreads;
             codeletCounter++) {
            if (codeletCounter != this->inputsTPParent->codeletIDexecutingSingle13) {
                (this->inputsTPParent->b_darts2[codeletCounter]) = temp_b;
                (this->inputsTPParent->a_darts2[codeletCounter]) = temp_a;
            }
        }
    }
    {
        for (size_t codeletsCounter = 0; codeletsCounter < myTP->numThreads; codeletsCounter++) {
            myTP->checkInCodelets19[codeletsCounter].decDep();
        }
    }
}
void TP2::_checkInCodelets19::fire(void)
{

    /*printing node 19: IfStmt*/
    if ((this->inputsTPParent->a_darts2[this->getID()]) - 11 != 0) {
        fprintf(stderr, "thread = %d, a != 11, a = %f\n", omp_get_thread_num(),
            (this->inputsTPParent->a_darts2[this->getID()]));
        abort();
    }

    /*printing node 25: IfStmt*/
    if ((this->inputsTPParent->b_darts2[this->getID()]) != 12) {
        fprintf(stderr, "thread = %d, b != 12, b = %f\n", omp_get_thread_num(),
            (this->inputsTPParent->b_darts2[this->getID()]));
        abort();
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/

    myTP->controlTPParent->nextCodeletsCopyPrivate[this->getID()]->decDep();
}
TP2::TP2(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_mainNextCodelet,
    darts::Codelet* in_mainSyncCodelet, TP2** in_ptrToThisFunctionTP, float in_a, float in_b)
    : ompTP(in_numThreads, in_mainCodeletID)
    , ptrToThisFunctionTP(in_ptrToThisFunctionTP)
    , inputsTPParent(this)
    , controlTPParent(this)
    , nextCodeletsCopyPrivate(new Codelet*[in_numThreads])
    , nextSyncCodeletsCopyPrivate(new Codelet*[in_numThreads])
    , a_darts2(new float[this->numThreads])
    , b_darts2(new float[this->numThreads])
    , TP13_alreadyLaunched(0)
    , codeletIDexecutingSingle13(ompNumThreads + 1)
    , checkInCodelets13(new _checkInCodelets13[this->numThreads])
    , barrierCodelets13(new _barrierCodelets13[1])
    , checkInCodelets19(new _checkInCodelets19[this->numThreads])
{
    barrierCodelets13[0] = _barrierCodelets13(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets19* checkInCodelets19Ptr = (this->checkInCodelets19);
    _checkInCodelets13* checkInCodelets13Ptr = (this->checkInCodelets13);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets13);
#endif
    for (size_t codeletCounter = 0; codeletCounter < this->numThreads; codeletCounter++) {
        (*checkInCodelets19Ptr) = _checkInCodelets19(1, 1, this, codeletCounter);
        checkInCodelets19Ptr++;
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets13Ptr) = _checkInCodelets13(2, 1, this, codeletCounter);
#else
        (*checkInCodelets13Ptr) = _checkInCodelets13(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets13Ptr).decDep();
        checkInCodelets13Ptr++;
    }
    if (this->numThreads == 1) {
        this->nextCodeletsCopyPrivate[0] = in_mainNextCodelet;
        this->nextSyncCodeletsCopyPrivate[0] = in_mainSyncCodelet;
        this->a_darts2[0] = in_a;
        this->b_darts2[0] = in_b;
        this->availableCodelets[0] = 1;
    } else {
        this->a_darts2[this->mainCodeletID] = in_a;
        this->b_darts2[this->mainCodeletID] = in_b;
        this->nextCodeletsCopyPrivate[in_mainCodeletID] = in_mainNextCodelet;
        this->nextSyncCodeletsCopyPrivate[in_mainCodeletID] = in_mainSyncCodelet;
#if USE_SPIN_CODELETS == 0
        this->firstCodelet[this->mainCodeletID].decDep();
#else
        this->availableCodelets[this->mainCodeletID] = 1;
#endif
        *(this->ptrToThisFunctionTP) = this;
    }
}
TP2::~TP2()
{
    delete[] checkInCodelets19;
    delete[] barrierCodelets13;
    delete[] checkInCodelets13;
    delete[] nextCodeletsCopyPrivate;
    delete[] nextSyncCodeletsCopyPrivate;
    delete[] a_darts2;
    delete[] b_darts2;
}
void TP2::setNewInputs(float in_a, float in_b, size_t codeletID)
{
    this->a_darts2[codeletID] = in_a;
    this->b_darts2[codeletID] = in_b;
}
/*TP36: OMPParallelDirective*/
void TP36::_barrierCodelets36::fire(void)
{
    TP36* myTP = static_cast<TP36*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
void TP36::_checkInCodelets38::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 38: CallExpr*/
    if (!__sync_val_compare_and_swap(&(myTP->controlTPParent->TP38_alreadyLaunched), 0, 1)) {
        /*Find and signal the next codelet*/
        myTP->controlTPParent->TPParent->barrierCodelets36[0].decDep();
        /*Make the function call*/
        invoke<TP_CopyPrivate>(myTP, myTP->numThreads, this->getID(),
            &(myTP->controlTPParent->TPParent->barrierCodelets36[0]),
            &(myTP->controlTPParent->barrierCodelets36[0]), &(myTP->controlTPParent->TP38Ptr),
            (*(this->inputsTPParent->a_darts36)), (*(this->inputsTPParent->b_darts36)));
    } else {
        if (myTP->controlTPParent->TP38Ptr == nullptr) {
            myTP->add(this);
            return;
        } else {
            myTP->controlTPParent->TP38Ptr->setNewInputs((*(this->inputsTPParent->a_darts36)),
                (*(this->inputsTPParent->b_darts36)), this->getID());
            myTP->controlTPParent->TP38Ptr->nextCodeletsCopyPrivate[this->getID()]
                = &(myTP->controlTPParent->TPParent->barrierCodelets36[0]);
            myTP->controlTPParent->TP38Ptr->nextSyncCodeletsCopyPrivate[this->getID()]
                = &(myTP->controlTPParent->barrierCodelets36[0]);
#if USE_SPIN_CODELETS == 0
            myTP->controlTPParent->TP38Ptr->firstCodelet[this->getID()].decDep();
#else
            myTP->controlTPParent->TP38Ptr->availableCodelets[this->getID()] = 1;
#endif
        }
    }
}
TP36::TP36(
    int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet, float* in_a, float* in_b)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , a_darts36(in_a) /*OMP_SHARED - INPUT*/
    , b_darts36(in_b) /*OMP_SHARED - INPUT*/
    , TP38Ptr(nullptr)
    , TP38_alreadyLaunched(0)
    , barrierCodelets36(new _barrierCodelets36[1])
    , checkInCodelets38(new _checkInCodelets38[this->numThreads])
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets36[0] = _barrierCodelets36(ompNumThreads, ompNumThreads, this, 0);
    _checkInCodelets38* checkInCodelets38Ptr = (this->checkInCodelets38);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets38Ptr) = _checkInCodelets38(1, 1, this, codeletCounter);
        (*checkInCodelets38Ptr).decDep();
        checkInCodelets38Ptr++;
    }
}
TP36::~TP36()
{
    delete[] barrierCodelets36;
    delete[] checkInCodelets38;
}
