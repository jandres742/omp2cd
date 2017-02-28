#include "tree-traversal-tasks.output.darts.h"
using namespace darts;
using namespace std;
int num_errors_darts4;
bool num_errors_darts4_initFlag;
int level_darts2;
bool level_darts2_initFlag;
/*Function: createNode, ID: 1*/
Node_t* createNode(int* countNodes)
{
    /*createNode:1*/
    /*CompoundStmt:19*/
    Node_t* node = (Node_t*)malloc(sizeof(Node_t));
    node->ID = *countNodes;
    *countNodes += 1;
    node->parent = (Node_t*)((void*)0);
    node->leftChild = (Node_t*)((void*)0);
    node->rightChild = (Node_t*)((void*)0);
    node->level = 0;
    node->numChildren = 0;
    node->distanceToRoot = 0;
    node->distanceToRootTask = 0;
    return node;
}
/*Function: constructGraph, ID: 2*/
void constructGraph(Node_t* node, int maxNodes, int* countNodes)
{
    /*constructGraph:2*/
    /*CompoundStmt:50*/
    static int level = 0;
    level++;
    if (*countNodes >= maxNodes)
        return;
    int createLeftChild = (rand() % 100 - 50);
    if (createLeftChild > 0) {
        node->leftChild = createNode(countNodes);
        node->leftChild->level = level;
        node->leftChild->parent = node;
        int createRightChild = (rand() % 100 - 50);
        if (createRightChild > 0) {
            node->rightChild = createNode(countNodes);
            node->rightChild->level = node->leftChild->level;
            node->rightChild->parent = node;
            constructGraph(node->rightChild, maxNodes, countNodes);
        }
        constructGraph(node->leftChild, maxNodes, countNodes);
    } else {
        node->rightChild = createNode(countNodes);
        node->rightChild->level = level;
        node->rightChild->parent = node;
        constructGraph(node->rightChild, maxNodes, countNodes);
    }
}
/*Function: printSpaces, ID: 3*/
void printSpaces(int numSpaces)
{
    /*printSpaces:3*/
    /*CompoundStmt:107*/
    for (int i = 0; i < numSpaces; i++)
        fprintf(stderr, " ");
}
/*Function: displayGraph, ID: 4*/
void displayGraph(Node_t* node)
{
    /*displayGraph:4*/
    /*CompoundStmt:113*/
    static int num_errors = 0;
    printSpaces(node->level);
    fprintf(stderr, "Node %d, distance = %d:%d: \n", node->ID, node->distanceToRoot,
        node->distanceToRootTask);
    if (node->distanceToRoot != node->distanceToRootTask)
        num_errors++;
    if (node->leftChild) {
        printSpaces(node->level);
        fprintf(stderr, "leftChild %d ", node->leftChild->ID);
    }
    if (node->rightChild) {
        if (node->leftChild == ((void*)0))
            printSpaces(node->level);
        fprintf(stderr, "rightChild %d", node->rightChild->ID);
    }
    fprintf(stderr, "\n");
    if (node->leftChild) {
        displayGraph(node->leftChild);
    }
    if (node->rightChild) {
        displayGraph(node->rightChild);
    }
    if (node->parent == (Node_t*)((void*)0) && num_errors)
        fprintf(stderr, "Errors = %d\n", num_errors);
}
/*Function: deleteGraph, ID: 5*/
void deleteGraph(Node_t* node)
{
    /*deleteGraph:5*/
    /*CompoundStmt:159*/
    if (node->leftChild)
        deleteGraph(node->leftChild);
    if (node->rightChild)
        deleteGraph(node->rightChild);
    free(node);
}
/*Function: getDistanceToRoot, ID: 6*/
void getDistanceToRoot(Node_t* node)
{
    /*getDistanceToRoot:6*/
    /*CompoundStmt:171*/
    if (node->parent)
        node->distanceToRoot = node->parent->distanceToRoot + 1;
    if (node->leftChild)
        getDistanceToRoot(node->leftChild);
    if (node->rightChild)
        getDistanceToRoot(node->rightChild);
}
/*Function: main, ID: 8*/
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
    num_errors_darts4 = (int)0;
    num_errors_darts4_initFlag = false;
    level_darts2 = (int)0;
    level_darts2_initFlag = false;
    /*main:8*/
    /*CompoundStmt:214*/
    srand(time((time_t*)((void*)0)));
    int maxNodes = 10;
    if (argc > 1)
        maxNodes = atoi(argv[1]);
    int countNodes = 0;
    Node_t* root = createNode(&countNodes);
    root->level = 0;
    constructGraph(root, maxNodes, &countNodes);
    getDistanceToRoot(root);
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP235>(
            ompNumThreads * DARTS_CODELETS_MULT, 0, RuntimeFinalCodelet, (Node_t**)&((root))));
    }
    displayGraph(root);
    deleteGraph(root);
    return 0;
}
/*TP7: TP_getDistanceToRootTasks*/
void TP7::_checkInCodelets191::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    /*Init the vars for this region*/

    /*printing node 191: IfStmt*/
    if ((this->inputsTPParent->node_darts7[this->getID()])->parent) {
        (this->inputsTPParent->node_darts7[this->getID()])->distanceToRootTask
            = (this->inputsTPParent->node_darts7[this->getID()])->parent->distanceToRootTask + 1;
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 191 nextRegion: 199 */
    myTP->controlTPParent->checkInCodelets199[this->getID()].decDep();
}
void TP7::_checkInCodelets199::fire(void)
{
    /*Printing conditional branch node 199: inlining: 0*/
    if ((this->inputsTPParent->node_darts7[this->getID()])->leftChild) {
        myTP->checkInCodelets202[this->getID()].decDep();
    } else {
        /*Signaling the region after the if stmt*/
        /*Signaling next codelet region: 199 nextRegion: 207 */
        myTP->controlTPParent->barrierCodelets207[0].decDep();
    }
}
void TP7::_barrierCodelets207::fire(void)
{
    TP7* myTP = static_cast<TP7*>(myTP_);
    {
        for (size_t codeletsCounter = 0; codeletsCounter < myTP->numThreads; codeletsCounter++) {
            myTP->checkInCodelets208[codeletsCounter].decDep();
        }
    }
}
void TP7::_checkInCodelets208::fire(void)
{
    /*Printing conditional branch node 208: inlining: 0*/
    if ((this->inputsTPParent->node_darts7[this->getID()])->rightChild) {
        myTP->checkInCodelets211[this->getID()].decDep();
    } else {
        /*Signaling the region after the if stmt*/
        /*Find and signal the next codelet*/

        myTP->controlTPParent->nextCodeletsgetDistanceToRootTasks[this->getID()]->decDep();
    }
}
void TP7::_checkInCodelets202::fire(void)
{

    /*printing node 202: OMPTaskDirective*/
    /*syncNode: OMPTaskwaitDirective 207*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->barrierCodelets207[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 202*/
    _task202Inputs* task202Inputs = new _task202Inputs(
        &((this->inputsTPParent->node_darts7[this->getID()])), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task202Inputs[this->getID()] = task202Inputs;
    invoke<TP202>(myTP, 1, this->getID(), myTP, task202Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 202 nextRegion: 207 */
    myTP->controlTPParent->barrierCodelets207[0].decDep();
}
void TP7::_checkInCodelets211::fire(void)
{

    /*printing node 211: OMPTaskDirective*/
    /*No syncnode, using the function's (getDistanceToRootTasks) sync codelet*/
    Codelet* nextSyncCodelet
        = (myTP->controlTPParent->nextSyncCodeletsgetDistanceToRootTasks[this->getID()]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 211*/
    _task211Inputs* task211Inputs = new _task211Inputs(
        &((this->inputsTPParent->node_darts7[this->getID()])), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task211Inputs[this->getID()] = task211Inputs;
    invoke<TP211>(myTP, 1, this->getID(), myTP, task211Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/

    myTP->controlTPParent->nextCodeletsgetDistanceToRootTasks[this->getID()]->decDep();
}
TP7::TP7(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_mainNextCodelet,
    darts::Codelet* in_mainSyncCodelet, TP7** in_ptrToThisFunctionTP, Node_t* in_node)
    : ompTP(in_numThreads, in_mainCodeletID)
    , ptrToThisFunctionTP(in_ptrToThisFunctionTP)
    , inputsTPParent(this)
    , controlTPParent(this)
    , nextCodeletsgetDistanceToRootTasks(new Codelet*[in_numThreads])
    , nextSyncCodeletsgetDistanceToRootTasks(new Codelet*[in_numThreads])
    , node_darts7(new Node_t*[this->numThreads])
    , task202Inputs(new _task202Inputs*[this->numThreads])
    , task211Inputs(new _task211Inputs*[this->numThreads])
    , checkInCodelets191(new _checkInCodelets191[this->numThreads])
    , checkInCodelets199(new _checkInCodelets199[this->numThreads])
    , barrierCodelets207(new _barrierCodelets207[1])
    , checkInCodelets208(new _checkInCodelets208[this->numThreads])
    , checkInCodelets202(new _checkInCodelets202[this->numThreads])
    , checkInCodelets211(new _checkInCodelets211[this->numThreads])
{
    barrierCodelets207[0] = _barrierCodelets207(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets211* checkInCodelets211Ptr = (this->checkInCodelets211);
    _checkInCodelets202* checkInCodelets202Ptr = (this->checkInCodelets202);
    _checkInCodelets208* checkInCodelets208Ptr = (this->checkInCodelets208);
    _checkInCodelets199* checkInCodelets199Ptr = (this->checkInCodelets199);
    _checkInCodelets191* checkInCodelets191Ptr = (this->checkInCodelets191);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets191);
#endif
    for (size_t codeletCounter = 0; codeletCounter < this->numThreads; codeletCounter++) {
        (*checkInCodelets211Ptr) = _checkInCodelets211(1, 1, this, codeletCounter);
        checkInCodelets211Ptr++;
        (*checkInCodelets202Ptr) = _checkInCodelets202(1, 1, this, codeletCounter);
        checkInCodelets202Ptr++;
        (*checkInCodelets208Ptr) = _checkInCodelets208(1, 1, this, codeletCounter);
        checkInCodelets208Ptr++;
        (*checkInCodelets199Ptr) = _checkInCodelets199(1, 1, this, codeletCounter);
        checkInCodelets199Ptr++;
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets191Ptr) = _checkInCodelets191(2, 1, this, codeletCounter);
#else
        (*checkInCodelets191Ptr) = _checkInCodelets191(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets191Ptr).decDep();
        checkInCodelets191Ptr++;
    }
    if (this->numThreads == 1) {
        this->nextCodeletsgetDistanceToRootTasks[0] = in_mainNextCodelet;
        this->nextSyncCodeletsgetDistanceToRootTasks[0] = in_mainSyncCodelet;
        this->node_darts7[0] = in_node;
        this->availableCodelets[0] = 1;
    } else {
        this->node_darts7[this->mainCodeletID] = in_node;
        this->nextCodeletsgetDistanceToRootTasks[in_mainCodeletID] = in_mainNextCodelet;
        this->nextSyncCodeletsgetDistanceToRootTasks[in_mainCodeletID] = in_mainSyncCodelet;
#if USE_SPIN_CODELETS == 0
        this->firstCodelet[this->mainCodeletID].decDep();
#else
        this->availableCodelets[this->mainCodeletID] = 1;
#endif
        *(this->ptrToThisFunctionTP) = this;
    }
}
TP7::~TP7()
{
    delete[] task202Inputs;
    delete[] task211Inputs;
    delete[] checkInCodelets211;
    delete[] checkInCodelets202;
    delete[] checkInCodelets208;
    delete[] barrierCodelets207;
    delete[] checkInCodelets199;
    delete[] checkInCodelets191;
    delete[] nextCodeletsgetDistanceToRootTasks;
    delete[] nextSyncCodeletsgetDistanceToRootTasks;
    delete[] node_darts7;
}
void TP7::setNewInputs(Node_t* in_node, size_t codeletID)
{
    this->node_darts7[codeletID] = in_node;
}
/*TP202: OMPTaskDirective*/
void TP202::_checkInCodelets204::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 204: CallExpr*/
    /*Signaling next codelet region: 204 nextRegion: 206 */
    myTP->controlTPParent->checkInCodelets206.decDep();
    /*No syncpoint found or outside task's boundaries, looking for syncCodelet*/
    /*Make the function call*/
    invoke<TP_getDistanceToRootTasks>(myTP, myTP->numThreads, this->getID(),
        &(myTP->controlTPParent->checkInCodelets206),
        (myTP->controlTPParent->task202Inputs->nextSyncCodelet), nullptr,
        (this->taskInputs->node_darts202)->leftChild);
}
void TP202::_checkInCodelets206::fire(void)
{

    /*printing node 206: CStyleCastExpr*/
    (void)(this->taskInputs->node_darts202);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task202Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task202Inputs->nextSyncCodelet->decDep();
}
TP202::TP202(
    int in_numThreads, int in_mainCodeletID, TP7* in_TPParent, _task202Inputs* in_task202Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task202Inputs(in_task202Inputs)
    , TP204Ptr(nullptr)
    , TP204_alreadyLaunched(0)
    , checkInCodelets204(1, 1, this, this->mainCodeletID)
    , checkInCodelets206(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets204.decDep();
}
TP202::~TP202() { delete (task202Inputs); }
/*TP211: OMPTaskDirective*/
void TP211::_checkInCodelets212::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 212: CallExpr*/
    /*Signaling next codelet region: 212 nextRegion: 244 */
    myTP->controlTPParent->checkInCodelets244.decDep();
    /*No syncpoint found or outside task's boundaries, looking for syncCodelet*/
    /*Make the function call*/
    invoke<TP_getDistanceToRootTasks>(myTP, myTP->numThreads, this->getID(),
        &(myTP->controlTPParent->checkInCodelets244),
        (myTP->controlTPParent->task211Inputs->nextSyncCodelet), nullptr,
        (*(this->taskInputs->node_darts211))->rightChild);
}
void TP211::_checkInCodelets244::fire(void)
{

    /*printing node 244: NullStmt*/
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task211Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task211Inputs->nextSyncCodelet->decDep();
}
TP211::TP211(
    int in_numThreads, int in_mainCodeletID, TP7* in_TPParent, _task211Inputs* in_task211Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task211Inputs(in_task211Inputs)
    , TP212Ptr(nullptr)
    , TP212_alreadyLaunched(0)
    , checkInCodelets212(1, 1, this, this->mainCodeletID)
    , checkInCodelets244(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets212.decDep();
}
TP211::~TP211() { delete (task211Inputs); }
/*TP235: OMPParallelDirective*/
void TP235::_barrierCodelets235::fire(void)
{
    TP235* myTP = static_cast<TP235*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
void TP235::_checkInCodelets237::fire(void)
{
    /*Select the thread executing OMPSingleDirective 237*/
    if (!__sync_val_compare_and_swap(&(myTP->TP237_alreadyLaunched), 0, 1)) {
        invoke<TP237>(myTP, 1, this->getID(), myTP, &(*(this->inputsTPParent->root_darts235)));
    } else {
        myTP->TPParent->barrierCodelets235[0].decDep();
    }
}
TP235::TP235(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet, Node_t** in_root)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , root_darts235(in_root) /*OMP_SHARED - INPUT*/
    , TP237Ptr(nullptr)
    , TP237_alreadyLaunched(0)
    , barrierCodelets235(new _barrierCodelets235[1])
    , checkInCodelets237(new _checkInCodelets237[this->numThreads])
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets235[0] = _barrierCodelets235(ompNumThreads, ompNumThreads, this, 0);
    _checkInCodelets237* checkInCodelets237Ptr = (this->checkInCodelets237);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets237Ptr) = _checkInCodelets237(1, 1, this, codeletCounter);
        (*checkInCodelets237Ptr).decDep();
        checkInCodelets237Ptr++;
    }
}
TP235::~TP235()
{
    delete[] barrierCodelets235;
    delete[] checkInCodelets237;
}
/*TP237: OMPSingleDirective*/
void TP237::_checkInCodelets239::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 239: CallExpr*/
    /*Signaling next codelet region: 239 nextRegion: 240 */
    myTP->controlTPParent->checkInCodelets240.decDep();
    /*Make the function call*/
    invoke<TP_getDistanceToRootTasks>(myTP, myTP->numThreads, this->getID(),
        &(myTP->controlTPParent->checkInCodelets240),
        &(myTP->controlTPParent->TPParent->barrierCodelets235[0]), nullptr,
        (*(this->inputsTPParent->root_darts237)));
}
void TP237::_checkInCodelets240::fire(void)
{

    /*printing node 240: CStyleCastExpr*/
    (void)(*(this->inputsTPParent->root_darts237));
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->TPParent->barrierCodelets235[0].decDep();
}
TP237::TP237(int in_numThreads, int in_mainCodeletID, TP235* in_TPParent, Node_t** in_root)
    : ompOMPSingleDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , root_darts237(in_root) /*OMP_SHARED - INPUT*/
    , TP239Ptr(nullptr)
    , TP239_alreadyLaunched(0)
    , checkInCodelets239(1, 1, this, this->mainCodeletID)
    , checkInCodelets240(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets239.decDep();
}
TP237::~TP237() {}
