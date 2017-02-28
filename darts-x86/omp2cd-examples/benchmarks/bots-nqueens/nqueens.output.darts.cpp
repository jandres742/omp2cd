#include "nqueens.output.darts.h"
using namespace darts;
using namespace std;
int total_count;
static int verify_solutions[14]
    = { 1, 0, 0, 2, 10, 4, 40, 92, 352, 724, 2680, 14200, 73712, 365596 };
/*Function: ok, ID: 15*/
int ok(int n, char* a)
{
    /*ok:15*/
    /*CompoundStmt:42*/
    int i, j;
    char p, q;
    for (i = 0; i < n; i++) {
        p = a[i];
        for (j = i + 1; j < n; j++) {
            q = a[j];
            if (q == p || q == p - (j - i) || q == p + (j - i))
                return 0;
        }
    }
    return 1;
}
/*Function: nqueens_ser, ID: 16*/
void nqueens_ser(int n, int j, char* a, int* solutions)
{
    /*nqueens_ser:16*/
    /*CompoundStmt:65*/
    int res;
    int i;
    if (n == j) {
        *solutions = 1;
        return;
    }
    *solutions = 0;
    for (i = 0; i < n; i++) {
        {
            a[j] = (char)i;
            if (ok(j + 1, a)) {
                nqueens_ser(n, j + 1, a, &res);
                *solutions += res;
            }
        }
    }
}
/*Function: find_queens, ID: 18*/
void find_queens(int size)
{
    /*find_queens:18*/
    /*CompoundStmt:147*/
    total_count = 0;
    {
        /*CompoundStmt:149*/
        if (bots_verbose_mode >= BOTS_VERBOSE_DEFAULT) {
            fprintf(stdout, "Computing N-Queens algorithm (n=%d) ", size);
        }
    }
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP154>(
            ompNumThreads * DARTS_CODELETS_MULT, 0, RuntimeFinalCodelet, (int*)&((size))));
    }
    {
        /*CompoundStmt:166*/
        if (bots_verbose_mode >= BOTS_VERBOSE_DEFAULT) {
            fprintf(stdout, " completed!\n");
        }
    }
}
/*Function: verify_queens, ID: 19*/
int verify_queens(int size)
{
    /*verify_queens:19*/
    /*CompoundStmt:171*/
    fprintf(stderr, "verify_queens: size = %d, verify_solutions[%d] = %d\n", size, size - 1,
        verify_solutions[size - 1]);
    if (size > (int)(sizeof(verify_solutions) / sizeof(int)))
        return 0;
    if (total_count == verify_solutions[size - 1])
        return 1;
    return 2;
}
/*TP17: TP_nqueens*/
void TP17::_checkInCodelets94::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    /*Init the vars for this region*/

    /*printing node 94: DeclStmt*/

    /*printing node 95: DeclStmt*/

    /*printing node 96: IfStmt*/
    if ((this->inputsTPParent->n_darts17[this->getID()])
        == (this->inputsTPParent->j_darts17[this->getID()])) {
        *(this->inputsTPParent->solutions_darts17[this->getID()]) = 1;
        /*return*/
        myTP->controlTPParent->nextCodeletsnqueens[this->getID()]->decDep();
        return;
    }

    /*printing node 102: BinaryOperator*/
    *(this->inputsTPParent->solutions_darts17[this->getID()]) = 0;

    /*printing node 104: BinaryOperator*/
    (this->inputsTPParent->csols_darts17[this->getID()])
        = (int*)/*Replacing alloca with malloc*/ malloc(
            (this->inputsTPParent->n_darts17[this->getID()]) * sizeof(int));

    /*printing node 107: NullStmt*/
    /*Variable csols originally allocated using alloca(). Store its pointer to delete it when
     * function nqueens is destroyed.*/
    void* topFunctionPtr = topFunctionStack(this->getLocalID());
    if (topFunctionPtr) {
        TP_nqueens* functionPtr = (TP_nqueens*)topFunctionPtr;
        functionPtr->allocaVars.push_back(
            (void*)this->inputsTPParent->csols_darts17[this->getID()]);
    }

    /*printing node 110: CallExpr*/
    memset((this->inputsTPParent->csols_darts17[this->getID()]), 0,
        (this->inputsTPParent->n_darts17[this->getID()]) * sizeof(int));

    /*printing node 114: BinaryOperator*/
    (this->inputsTPParent->i_darts17[this->getID()]) = 0;

    /*printing node 115: BinaryOperator*/
    /*Print the code for a condition node in a complex loop stmt */
    if ((this->inputsTPParent->i_darts17[this->getID()])
        < (this->inputsTPParent->n_darts17[this->getID()])) {
        /*Signal the first codelet in the loop*/
        myTP->checkInCodelets113[this->getID()].decDep();
        return;
    } else {
        /*Signal the codelet after the loop from the end condional node.*/
        /*Signaling next codelet region: 116 nextRegion: 139 */
        myTP->controlTPParent->barrierCodelets139[0].decDep();
        return;
    }
}
void TP17::_checkInCodelets113::fire(void)
{

    /*printing node 113: ForStmt*/
    bool haveToLaunch = __sync_bool_compare_and_swap(&(myTP->controlTPParent->TP113_LoopCounter),
        myTP->controlTPParent->TP113_LoopCounterPerThread[this->getID()],
        myTP->controlTPParent->TP113_LoopCounterPerThread[this->getID()] + 1);
    unsigned int iterIdx = myTP->controlTPParent->TP113_LoopCounterPerThread[this->getID()];
    if (haveToLaunch) {
        this->resetCodelet();
        myTP->controlTPParent->TP113PtrVec.push_back(nullptr);
        myTP->controlTPParent->TP113_LoopCounterPerThread[this->getID()] += 1;
        invoke<TP113>(myTP, myTP->numThreads, this->getID(), myTP, myTP->inputsTPParent,
            &(myTP->controlTPParent->TP113PtrVec.back()));
    } else {
        if (myTP->controlTPParent->TP113PtrVec.size() == 0) {
            this->resetCodelet();
            this->decDep();
            return;
        } else if (myTP->controlTPParent->TP113PtrVec.size() < (iterIdx + 1)) {
            this->resetCodelet();
            this->decDep();
            return;
        } else if (myTP->controlTPParent->TP113PtrVec[iterIdx] == nullptr) {
            this->resetCodelet();
            this->decDep();
            return;
        } else {
            this->resetCodelet();
#if USE_SPIN_CODELETS == 0
            myTP->controlTPParent->TP113PtrVec[iterIdx]->firstCodelet[this->getID()].decDep();
#else
            myTP->controlTPParent->TP113PtrVec[iterIdx]->availableCodelets[this->getID()] = 1;
#endif
            myTP->controlTPParent->TP113_LoopCounterPerThread[this->getID()] += 1;
        }
    }
}
void TP17::_checkInCodelets116::fire(void)
{

    /*printing node 116: UnaryOperator*/
    (this->inputsTPParent->i_darts17[this->getID()])++;

    /*printing node 186: BinaryOperator*/
    /*Print the code for a condition node in a complex loop stmt */
    if ((this->inputsTPParent->i_darts17[this->getID()])
        < (this->inputsTPParent->n_darts17[this->getID()])) {
        this->resetCodelet();
        /*Signal the first codelet in the loop*/
        myTP->checkInCodelets113[this->getID()].decDep();
        return;
    } else {
        /*Signal the codelet after the loop from the condtional node.*/
        /*Signaling next codelet region: 116 nextRegion: 139 */
        myTP->controlTPParent->barrierCodelets139[0].decDep();
        return;
    }
}
void TP17::_barrierCodelets139::fire(void)
{
    TP17* myTP = static_cast<TP17*>(myTP_);
    {
        for (size_t codeletsCounter = 0; codeletsCounter < myTP->numThreads; codeletsCounter++) {
            myTP->checkInCodelets140[codeletsCounter].decDep();
        }
    }
}
void TP17::_checkInCodelets140::fire(void)
{

    /*printing node 140: ForStmt*/
    {
        {
            int** csols = &(this->inputsTPParent->csols_darts17[this->getLocalID()]);
            (void)csols /*PRIVATE*/;
            int* i = &(this->inputsTPParent->i_darts17[this->getLocalID()]);
            (void)i /*PRIVATE*/;
            int* n = &(this->inputsTPParent->n_darts17[this->getLocalID()]);
            (void)n /*PRIVATE*/;
            int** solutions = &(this->inputsTPParent->solutions_darts17[this->getLocalID()]);
            (void)solutions /*PRIVATE*/;
            /*Loop's init*/
            (this->inputsTPParent->i_darts17[this->getID()]) = 0;
            int i_darts_counter_temp17 = (this->inputsTPParent->i_darts17[this->getID()]);
            for (; (i_darts_counter_temp17) < (*n); (i_darts_counter_temp17)++) {
                *(*solutions) += (*csols)[(i_darts_counter_temp17)];
            }
            (this->inputsTPParent->i_darts17[this->getID()]) = i_darts_counter_temp17;
        }
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/

    myTP->controlTPParent->nextCodeletsnqueens[this->getID()]->decDep();
    /*Remove pointer to current function from thread's stack*/
    popFunctionStack(this->getLocalID());
}
TP17::TP17(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_mainNextCodelet,
    darts::Codelet* in_mainSyncCodelet, TP17** in_ptrToThisFunctionTP, int in_n, int in_j,
    char* in_a, int* in_solutions, int in_depth)
    : ompTP(in_numThreads, in_mainCodeletID)
    , ptrToThisFunctionTP(in_ptrToThisFunctionTP)
    , inputsTPParent(this)
    , controlTPParent(this)
    , nextCodeletsnqueens(new Codelet*[in_numThreads])
    , nextSyncCodeletsnqueens(new Codelet*[in_numThreads])
    , n_darts17(new int[this->numThreads])
    , j_darts17(new int[this->numThreads])
    , a_darts17(new char*[this->numThreads])
    , solutions_darts17(new int*[this->numThreads])
    , depth_darts17(new int[this->numThreads])
    , csols_darts17(new int*[this->numThreads])
    , i_darts17(new int[this->numThreads])
    , TP113_LoopCounter(0)
    , TP113_LoopCounterPerThread(new unsigned int[this->numThreads])
    , task118Inputs(new _task118Inputs*[this->numThreads])
    , checkInCodelets94(new _checkInCodelets94[this->numThreads])
    , checkInCodelets113(new _checkInCodelets113[this->numThreads])
    , checkInCodelets116(new _checkInCodelets116[this->numThreads])
    , barrierCodelets139(new _barrierCodelets139[1])
    , checkInCodelets140(new _checkInCodelets140[this->numThreads])
{
    pushFunctionStack(this->getID(), (void*)this);
    memset((void*)TP113_LoopCounterPerThread, 0, this->numThreads * sizeof(unsigned int));
    barrierCodelets139[0] = _barrierCodelets139(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets140* checkInCodelets140Ptr = (this->checkInCodelets140);
    _checkInCodelets116* checkInCodelets116Ptr = (this->checkInCodelets116);
    _checkInCodelets113* checkInCodelets113Ptr = (this->checkInCodelets113);
    _checkInCodelets94* checkInCodelets94Ptr = (this->checkInCodelets94);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets94);
#endif
    for (size_t codeletCounter = 0; codeletCounter < this->numThreads; codeletCounter++) {
        (*checkInCodelets140Ptr) = _checkInCodelets140(1, 1, this, codeletCounter);
        checkInCodelets140Ptr++;
        (*checkInCodelets116Ptr) = _checkInCodelets116(1, 1, this, codeletCounter);
        checkInCodelets116Ptr++;
        (*checkInCodelets113Ptr) = _checkInCodelets113(1, 1, this, codeletCounter);
        checkInCodelets113Ptr++;
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets94Ptr) = _checkInCodelets94(2, 1, this, codeletCounter);
#else
        (*checkInCodelets94Ptr) = _checkInCodelets94(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets94Ptr).decDep();
        checkInCodelets94Ptr++;
    }
    if (this->numThreads == 1) {
        this->nextCodeletsnqueens[0] = in_mainNextCodelet;
        this->nextSyncCodeletsnqueens[0] = in_mainSyncCodelet;
        this->n_darts17[0] = in_n;
        this->j_darts17[0] = in_j;
        this->a_darts17[0] = in_a;
        this->solutions_darts17[0] = in_solutions;
        this->depth_darts17[0] = in_depth;
        this->availableCodelets[0] = 1;
    } else {
        this->n_darts17[this->mainCodeletID] = in_n;
        this->j_darts17[this->mainCodeletID] = in_j;
        this->a_darts17[this->mainCodeletID] = in_a;
        this->solutions_darts17[this->mainCodeletID] = in_solutions;
        this->depth_darts17[this->mainCodeletID] = in_depth;
        this->nextCodeletsnqueens[in_mainCodeletID] = in_mainNextCodelet;
        this->nextSyncCodeletsnqueens[in_mainCodeletID] = in_mainSyncCodelet;
#if USE_SPIN_CODELETS == 0
        this->firstCodelet[this->mainCodeletID].decDep();
#else
        this->availableCodelets[this->mainCodeletID] = 1;
#endif
        *(this->ptrToThisFunctionTP) = this;
    }
}
TP17::~TP17()
{
    /*Delete any variables allocated using alloca() in the original code*/
    for (void* allocaPtr : this->allocaVars)
        free(allocaPtr);
    delete[] TP113_LoopCounterPerThread;
    delete[] task118Inputs;
    delete[] checkInCodelets140;
    delete[] barrierCodelets139;
    delete[] checkInCodelets116;
    delete[] checkInCodelets113;
    delete[] checkInCodelets94;
    delete[] nextCodeletsnqueens;
    delete[] nextSyncCodeletsnqueens;
    delete[] n_darts17;
    delete[] j_darts17;
    delete[] a_darts17;
    delete[] solutions_darts17;
    delete[] depth_darts17;
    delete[] csols_darts17;
    delete[] i_darts17;
}
void TP17::setNewInputs(
    int in_n, int in_j, char* in_a, int* in_solutions, int in_depth, size_t codeletID)
{
    this->n_darts17[codeletID] = in_n;
    this->j_darts17[codeletID] = in_j;
    this->a_darts17[codeletID] = in_a;
    this->solutions_darts17[codeletID] = in_solutions;
    this->depth_darts17[codeletID] = in_depth;
}
/*TP113: ForStmt*/
void TP113::_checkInCodelets118::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif

    /*printing node 118: OMPTaskDirective*/
    /*syncNode: OMPTaskwaitDirective 139*/
    /*Find the TP to which the nextRegion belongs*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->TPParent->barrierCodelets139[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 118*/
    _task118Inputs* task118Inputs
        = new _task118Inputs(&((this->inputsTPParent->a_darts17[this->getID()])),
            &((this->inputsTPParent->csols_darts17[this->getID()])),
            &((this->inputsTPParent->depth_darts17[this->getID()])),
            &((this->inputsTPParent->i_darts17[this->getID()])),
            &((this->inputsTPParent->j_darts17[this->getID()])),
            &((this->inputsTPParent->n_darts17[this->getID()])), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task118Inputs[this->getID()] = task118Inputs;
    invoke<TP118>(myTP, 1, this->getID(), myTP, task118Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*The node is the last one in a complex loop, so signal the inc node*/
    /*Find the TP to which the nextRegion belongs*/
    myTP->controlTPParent->TPParent->checkInCodelets116[this->getID()].decDep();
}
TP113::TP113(int in_numThreads, int in_mainCodeletID, TP17* in_TPParent, TP17* in_inputsTPParent,
    TP113** in_ptrToThisTP)
    : ompTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(in_inputsTPParent)
    , ptrToThisTP(in_ptrToThisTP)
    , checkInCodelets118(new _checkInCodelets118[this->numThreads])
{
    /*Initialize Codelets*/
    _checkInCodelets118* checkInCodelets118Ptr = (this->checkInCodelets118);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets118);
#endif
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets118Ptr) = _checkInCodelets118(2, 1, this, codeletCounter);
#else
        (*checkInCodelets118Ptr) = _checkInCodelets118(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets118Ptr).decDep();
        checkInCodelets118Ptr++;
    }
    *(this->ptrToThisTP) = this;
#if USE_SPIN_CODELETS == 0
    this->firstCodelet[this->getID()].decDep();
#else
    this->availableCodelets[this->getID()] = 1;
#endif
}
TP113::~TP113() { delete[] checkInCodelets118; }
/*TP118: OMPTaskDirective*/
void TP118::_checkInCodelets120::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 120: DeclStmt*/
    this->taskInputs->b_darts118 = (char*)/*Replacing alloca with malloc*/ malloc(
        (this->taskInputs->n_darts118) * sizeof(char));

    /*printing node 123: NullStmt*/
    /*Variable b originally allocated using alloca(). Store its pointer to delete it when function
     * nqueens is destroyed.*/
    void* topFunctionPtr = topFunctionStack(this->getLocalID());
    if (topFunctionPtr) {
        TP_nqueens* functionPtr = (TP_nqueens*)topFunctionPtr;
        functionPtr->allocaVars.push_back((void*)this->taskInputs->b_darts118);
    }

    /*printing node 126: CallExpr*/
    memcpy((this->taskInputs->b_darts118), (this->taskInputs->a_darts118),
        (this->taskInputs->j_darts118) * sizeof(char));

    /*printing node 129: BinaryOperator*/
    (this->taskInputs->b_darts118)[(this->taskInputs->j_darts118)]
        = (char)(this->taskInputs->i_darts118);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 120 nextRegion: 132 */
    myTP->controlTPParent->checkInCodelets132.decDep();
}
void TP118::_checkInCodelets132::fire(void)
{
    /*Printing conditional branch node 132: inlining: 0*/
    if (ok((this->taskInputs->j_darts118) + 1, (this->taskInputs->b_darts118))) {
        myTP->checkInCodelets134.decDep();
    } else {
        /*Signaling the region after the if stmt*/
        /*Signaling next codelet region: 132 nextRegion: 185 */
        myTP->controlTPParent->checkInCodelets185.decDep();
    }
}
void TP118::_checkInCodelets185::fire(void)
{

    /*printing node 185: NullStmt*/
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task118Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task118Inputs->nextSyncCodelet->decDep();
}
void TP118::_checkInCodelets134::fire(void)
{

/*printing node 134: CallExpr*/
/*Finding the next codelet after the function call*/
#if 0
/*Signaling next codelet region: 134 nextRegion: 185 */
myTP->controlTPParent->checkInCodelets185.decDep();
#endif
/*Finding the next sync point before making the function call*/
#if 0
/*Find the sync point for a function call*/
/*syncNode: OMPTaskwaitDirective 139*/
/*No syncpoint found or outside task's boundaries, looking for syncCodelet*/
#endif
    /*Make the function call*/
    invoke<TP_nqueens>(myTP, myTP->numThreads, this->getID(),
        &(myTP->controlTPParent->checkInCodelets185),
        (myTP->controlTPParent->task118Inputs->nextSyncCodelet), nullptr,
        (this->taskInputs->n_darts118), (this->taskInputs->j_darts118) + 1,
        (this->taskInputs->b_darts118),
        &(this->taskInputs->csols_darts118)[(this->taskInputs->i_darts118)],
        (this->taskInputs->depth_darts118));
}
TP118::TP118(
    int in_numThreads, int in_mainCodeletID, TP113* in_TPParent, _task118Inputs* in_task118Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task118Inputs(in_task118Inputs)
    , TP134Ptr(nullptr)
    , TP134_alreadyLaunched(0)
    , checkInCodelets120(1, 1, this, this->mainCodeletID)
    , checkInCodelets132(1, 1, this, this->mainCodeletID)
    , checkInCodelets185(1, 1, this, this->mainCodeletID)
    , checkInCodelets134(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets120.decDep();
}
TP118::~TP118() { delete (task118Inputs); }
/*TP154: OMPParallelDirective*/
void TP154::_barrierCodelets154::fire(void)
{
    TP154* myTP = static_cast<TP154*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
void TP154::_checkInCodelets156::fire(void)
{
    /*Select the thread executing OMPSingleDirective 156*/
    if (!__sync_val_compare_and_swap(&(myTP->TP156_alreadyLaunched), 0, 1)) {
        invoke<TP156>(myTP, 1, this->getID(), myTP, &(*(this->inputsTPParent->size_darts154)));
    } else {
        myTP->barrierCodelets156[0].decDep();
    }
}
void TP154::_barrierCodelets156::fire(void)
{
    TP154* myTP = static_cast<TP154*>(myTP_);
    myTP->TPParent->barrierCodelets154[0].setDep(0);
    myTP->add(&(myTP->TPParent->barrierCodelets154[0]));
}
TP154::TP154(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet, int* in_size)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , size_darts154(in_size) /*OMP_SHARED - INPUT*/
    , TP156Ptr(nullptr)
    , TP156_alreadyLaunched(0)
    , barrierCodelets154(new _barrierCodelets154[1])
    , checkInCodelets156(new _checkInCodelets156[this->numThreads])
    , barrierCodelets156(new _barrierCodelets156[1])
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets154[0] = _barrierCodelets154(ompNumThreads, ompNumThreads, this, 0);
    barrierCodelets156[0] = _barrierCodelets156(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets156* checkInCodelets156Ptr = (this->checkInCodelets156);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets156Ptr) = _checkInCodelets156(1, 1, this, codeletCounter);
        (*checkInCodelets156Ptr).decDep();
        checkInCodelets156Ptr++;
    }
}
TP154::~TP154()
{
    delete[] barrierCodelets154;
    delete[] barrierCodelets156;
    delete[] checkInCodelets156;
}
/*TP156: OMPSingleDirective*/
void TP156::_checkInCodelets158::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 158: DeclStmt*/

    /*printing node 159: BinaryOperator*/
    (this->inputsTPParent->a_darts156) = (char*)/*Replacing alloca with malloc*/ malloc(
        (*(this->inputsTPParent->size_darts156)) * sizeof(char));
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 158 nextRegion: 164 */
    myTP->controlTPParent->checkInCodelets164.decDep();
}
void TP156::_checkInCodelets164::fire(void)
{

/*printing node 164: CallExpr*/
/*Finding the next codelet after the function call*/
#if 0
/*Find and signal the next codelet*/
myTP->controlTPParent->TPParent->barrierCodelets156[0].decDep();
#endif
/*Finding the next sync point before making the function call*/
#if 0
/*Find the sync point for a function call*/
/*syncNode: OMPSingleDirective 156*/
/*Find the TP to which the nextRegion belongs*/
#endif
    /*Make the function call*/
    invoke<TP_nqueens>(myTP, myTP->numThreads, this->getID(),
        &(myTP->controlTPParent->TPParent->barrierCodelets156[0]),
        &(myTP->controlTPParent->TPParent->barrierCodelets156[0]), nullptr,
        (*(this->inputsTPParent->size_darts156)), 0, (this->inputsTPParent->a_darts156),
        &total_count, 0);
}
TP156::TP156(int in_numThreads, int in_mainCodeletID, TP154* in_TPParent, int* in_size)
    : ompOMPSingleDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , size_darts156(in_size) /*OMP_SHARED - INPUT*/
    , TP164Ptr(nullptr)
    , TP164_alreadyLaunched(0)
    , checkInCodelets158(1, 1, this, this->mainCodeletID)
    , checkInCodelets164(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets158.decDep();
}
TP156::~TP156() {}
