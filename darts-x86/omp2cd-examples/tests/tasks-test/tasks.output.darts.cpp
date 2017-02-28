#include "tasks.output.darts.h"
using namespace darts;
using namespace std;
/*Function: myFunction, ID: 1*/
void myFunction(double* inVec, double* outVec, int idx)
{
    /*myFunction:1*/
    /*CompoundStmt:7*/
    *(outVec + idx) = (*(inVec + idx)) * (*(inVec + idx)) + 1;
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
    /*CompoundStmt:17*/
    srand(time((time_t*)((void*)0)));
    int totalThreads = atoi(getenv("OMP_NUM_THREADS"));
    double* inVector = (double*)malloc(totalThreads * sizeof(double));
    double* outVectorSeq = (double*)malloc(totalThreads * sizeof(double));
    double* outVectorPar = (double*)malloc(totalThreads * sizeof(double));
    int i;
    for (i = 0; i < totalThreads; i++) {
        inVector[i] = (rand() % 100 - 50) / 0.5;
    }
    for (i = 0; i < totalThreads; i++) {
        outVectorSeq[i] = inVector[i] * inVector[i] + 1;
    }
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP64>(ompNumThreads * DARTS_CODELETS_MULT, 0,
            RuntimeFinalCodelet, (double**)&((inVector)), (double**)&((outVectorPar))));
    }
    int good = 1;
    for (i = 0; i < totalThreads && good == 1; i++) {
        if (*(outVectorSeq + i) != *(outVectorPar + i)) {
            printf("Error in tasks test\n");
            printf("*(outVectorSeq + %d) = %f\n", i, *(outVectorSeq + i));
            printf("*(outVectorPar + %d) = %f\n", i, *(outVectorPar + i));
            good = 0;
        }
    }
    free(inVector);
    free(outVectorPar);
    free(outVectorSeq);
    return 0;
}
/*TP64: OMPParallelDirective*/
void TP64::_barrierCodelets64::fire(void)
{
    TP64* myTP = static_cast<TP64*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
void TP64::_checkInCodelets66::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 66: DeclStmt*/
    this->inputsTPParent->id_darts64[this->getID()] = omp_get_thread_num();
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 66 nextRegion: 68 */
    myTP->controlTPParent->checkInCodelets68[this->getID()].decDep();
}
void TP64::_checkInCodelets68::fire(void)
{

    /*printing node 68: OMPTaskDirective*/
    /*syncNode: OMPTaskwaitDirective 72*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->barrierCodelets72[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 68*/
    _task68Inputs* task68Inputs
        = new _task68Inputs(&((this->inputsTPParent->id_darts64[this->getID()])),
            &(*(this->inputsTPParent->inVector_darts64)),
            &(*(this->inputsTPParent->outVectorPar_darts64)), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task68Inputs[this->getID()] = task68Inputs;
    invoke<TP68>(myTP, 1, this->getID(), myTP, task68Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 68 nextRegion: 71 */
    myTP->controlTPParent->checkInCodelets71[this->getID()].decDep();
}
void TP64::_checkInCodelets71::fire(void)
{

    /*printing node 71: CStyleCastExpr*/
    (void)(this->inputsTPParent->id_darts64[this->getID()]);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 71 nextRegion: 72 */
    myTP->controlTPParent->barrierCodelets72[0].decDep();
}
void TP64::_barrierCodelets72::fire(void)
{
    TP64* myTP = static_cast<TP64*>(myTP_);
    {
        for (size_t codeletsCounter = 0; codeletsCounter < myTP->numThreads; codeletsCounter++) {
            myTP->checkInCodelets73[codeletsCounter].decDep();
        }
    }
}
void TP64::_checkInCodelets73::fire(void)
{

    /*printing node 73: CStyleCastExpr*/
    (void)(this->inputsTPParent->id_darts64[this->getID()]);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 73 nextRegion: 74 */
    myTP->controlTPParent->barrierCodelets74[0].decDep();
}
void TP64::_barrierCodelets74::fire(void)
{
    TP64* myTP = static_cast<TP64*>(myTP_);
    myTP->TPParent->barrierCodelets64[0].setDep(0);
    myTP->add(&(myTP->TPParent->barrierCodelets64[0]));
}
TP64::TP64(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet, double** in_inVector,
    double** in_outVectorPar)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , inVector_darts64(in_inVector) /*OMP_SHARED - INPUT*/
    , outVectorPar_darts64(in_outVectorPar) /*OMP_SHARED - INPUT*/
    , id_darts64(new int[this->numThreads]) /*VARIABLE*/
    , task68Inputs(new _task68Inputs*[this->numThreads])
    , barrierCodelets64(new _barrierCodelets64[1])
    , checkInCodelets66(new _checkInCodelets66[this->numThreads])
    , checkInCodelets68(new _checkInCodelets68[this->numThreads])
    , checkInCodelets71(new _checkInCodelets71[this->numThreads])
    , barrierCodelets72(new _barrierCodelets72[1])
    , checkInCodelets73(new _checkInCodelets73[this->numThreads])
    , barrierCodelets74(new _barrierCodelets74[1])
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets64[0] = _barrierCodelets64(ompNumThreads, ompNumThreads, this, 0);
    barrierCodelets74[0] = _barrierCodelets74(this->numThreads, this->numThreads, this, 0);
    barrierCodelets72[0] = _barrierCodelets72(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets73* checkInCodelets73Ptr = (this->checkInCodelets73);
    _checkInCodelets71* checkInCodelets71Ptr = (this->checkInCodelets71);
    _checkInCodelets68* checkInCodelets68Ptr = (this->checkInCodelets68);
    _checkInCodelets66* checkInCodelets66Ptr = (this->checkInCodelets66);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets73Ptr) = _checkInCodelets73(1, 1, this, codeletCounter);
        checkInCodelets73Ptr++;
        (*checkInCodelets71Ptr) = _checkInCodelets71(1, 1, this, codeletCounter);
        checkInCodelets71Ptr++;
        (*checkInCodelets68Ptr) = _checkInCodelets68(1, 1, this, codeletCounter);
        checkInCodelets68Ptr++;
        (*checkInCodelets66Ptr) = _checkInCodelets66(1, 1, this, codeletCounter);
        (*checkInCodelets66Ptr).decDep();
        checkInCodelets66Ptr++;
    }
}
TP64::~TP64()
{
    delete[] task68Inputs;
    delete[] id_darts64;
    delete[] barrierCodelets64;
    delete[] barrierCodelets74;
    delete[] checkInCodelets73;
    delete[] barrierCodelets72;
    delete[] checkInCodelets71;
    delete[] checkInCodelets68;
    delete[] checkInCodelets66;
}
/*TP68: OMPTaskDirective*/
void TP68::_checkInCodelets70::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 70: CallExpr*/
    myFunction((*(this->taskInputs->inVector_darts68)), (*(this->taskInputs->outVectorPar_darts68)),
        (this->taskInputs->id_darts68));
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task68Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task68Inputs->nextSyncCodelet->decDep();
}
TP68::TP68(
    int in_numThreads, int in_mainCodeletID, TP64* in_TPParent, _task68Inputs* in_task68Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task68Inputs(in_task68Inputs)
    , checkInCodelets70(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets70.decDep();
}
TP68::~TP68() { delete (task68Inputs); }
