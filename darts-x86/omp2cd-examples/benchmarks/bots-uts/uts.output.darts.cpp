#include "uts.output.darts.h"
using namespace darts;
using namespace std;
double b_0 = 4.;
int computeGranularity = 1;
unsigned long long exp_num_leaves = 0;
int exp_tree_depth = 0;
unsigned long long exp_tree_size = 0;
int maxTreeDepth = 0;
unsigned long long nLeaves = 0;
int nonLeafBF = 4;
double nonLeafProb = 15. / 64.;
int rootId = 0;
/*Function: rng_toProb, ID: 27*/
double rng_toProb(int n)
{
    /*rng_toProb:27*/
    /*CompoundStmt:77*/
    if (n < 0) {
        printf("*** toProb: rand n = %d out of range\n", n);
    }
    return ((n < 0) ? 0. : ((double)n) / 2147483648.);
}
/*Function: uts_initRoot, ID: 28*/
void uts_initRoot(Node* root)
{
    /*uts_initRoot:28*/
    /*CompoundStmt:89*/
    root->height = 0;
    root->numChildren = -1;
    rng_init(root->state.state, rootId);
    {
        /*CompoundStmt:98*/
        if (bots_verbose_mode >= BOTS_VERBOSE_DEFAULT) {
            fprintf(stdout, "Root node at %p\n", root);
        }
    }
}
/*Function: uts_numChildren_bin, ID: 29*/
int uts_numChildren_bin(Node* parent)
{
    /*uts_numChildren_bin:29*/
    /*CompoundStmt:103*/
    int v = rng_rand(parent->state.state);
    double d = rng_toProb(v);
    return (d < nonLeafProb) ? nonLeafBF : 0;
}
/*Function: uts_numChildren, ID: 30*/
int uts_numChildren(Node* parent)
{
    /*uts_numChildren:30*/
    /*CompoundStmt:113*/
    int numChildren = 0;
    if (parent->height == 0)
        numChildren = (int)floor(b_0);
    else
        numChildren = uts_numChildren_bin(parent);
    if (parent->height == 0) {
        int rootBF = (int)ceil(b_0);
        if (numChildren > rootBF) {
            ;
            numChildren = rootBF;
        }
    } else {
        if (numChildren > 100) {
            ;
            numChildren = 100;
        }
    }
    return numChildren;
}
/*Function: parallel_uts, ID: 31*/
unsigned long long parallel_uts(Node* root)
{
    /*parallel_uts:31*/
    /*CompoundStmt:140*/
    fprintf(stderr, "parallel_uts\n");
    unsigned long long num_nodes = 0;
    root->numChildren = uts_numChildren(root);
    {
        /*CompoundStmt:146*/
        if (bots_verbose_mode >= BOTS_VERBOSE_DEFAULT) {
            fprintf(stdout, "Computing Unbalance Tree Search algorithm ");
        }
    }
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP151>(ompNumThreads * DARTS_CODELETS_MULT, 0,
            RuntimeFinalCodelet, (unsigned long long*)&((num_nodes)), (Node**)&((root))));
    }
    {
        /*CompoundStmt:157*/
        if (bots_verbose_mode >= BOTS_VERBOSE_DEFAULT) {
            fprintf(stdout, " completed!");
        }
    }
    return num_nodes;
}
/*Function: uts_read_file, ID: 33*/
void uts_read_file(char* filename)
{
    /*uts_read_file:33*/
    /*CompoundStmt:207*/
    FILE* fin;
    if ((fin = fopen(filename, "r")) == ((void*)0)) {
        {
            if (bots_verbose_mode >= BOTS_VERBOSE_DEFAULT) {
                fprintf(stdout, "Could not open input file (%s)\n", filename);
            }
        };
        exit(-1);
    }
    fscanf(fin, "%lf %lf %d %d %d %llu %d %llu", &b_0, &nonLeafProb, &nonLeafBF, &rootId,
        &computeGranularity, &exp_tree_size, &exp_tree_depth, &exp_num_leaves);
    fclose(fin);
    computeGranularity = (((1) > (computeGranularity)) ? (1) : (computeGranularity));
    {
        /*CompoundStmt:232*/
        if (bots_verbose_mode >= BOTS_VERBOSE_DEFAULT) {
            fprintf(stdout, "\n");
        }
    }
    {
        /*CompoundStmt:237*/
        if (bots_verbose_mode >= BOTS_VERBOSE_DEFAULT) {
            fprintf(stdout, "Root branching factor                = %f\n", b_0);
        }
    }
    {
        /*CompoundStmt:242*/
        if (bots_verbose_mode >= BOTS_VERBOSE_DEFAULT) {
            fprintf(stdout, "Root seed (0 <= 2^31)                = %d\n", rootId);
        }
    }
    {
        /*CompoundStmt:247*/
        if (bots_verbose_mode >= BOTS_VERBOSE_DEFAULT) {
            fprintf(stdout, "Probability of non-leaf node         = %f\n", nonLeafProb);
        }
    }
    {
        /*CompoundStmt:252*/
        if (bots_verbose_mode >= BOTS_VERBOSE_DEFAULT) {
            fprintf(stdout, "Number of children for non-leaf node = %d\n", nonLeafBF);
        }
    }
    {
        /*CompoundStmt:257*/
        if (bots_verbose_mode >= BOTS_VERBOSE_DEFAULT) {
            fprintf(stdout, "E(n)                                 = %f\n",
                (double)(nonLeafProb * nonLeafBF));
        }
    }
    {
        /*CompoundStmt:264*/
        if (bots_verbose_mode >= BOTS_VERBOSE_DEFAULT) {
            fprintf(stdout, "E(s)                                 = %f\n",
                (double)(1. / (1. - nonLeafProb * nonLeafBF)));
        }
    }
    {
        /*CompoundStmt:275*/
        if (bots_verbose_mode >= BOTS_VERBOSE_DEFAULT) {
            fprintf(stdout, "Compute granularity                  = %d\n", computeGranularity);
        }
    }
    {
        /*CompoundStmt:280*/
        if (bots_verbose_mode >= BOTS_VERBOSE_DEFAULT) {
            fprintf(stdout, "Random number generator              = ");
        }
    }
    rng_showtype();
}
/*Function: uts_show_stats, ID: 34*/
void uts_show_stats()
{
    /*uts_show_stats:34*/
    /*CompoundStmt:286*/
    int nPes = atoi(bots_resources);
    int chunkSize = 0;
    {
        /*CompoundStmt:290*/
        if (bots_verbose_mode >= BOTS_VERBOSE_DEFAULT) {
            fprintf(stdout, "\n");
        }
    }
    {
        /*CompoundStmt:295*/
        if (bots_verbose_mode >= BOTS_VERBOSE_DEFAULT) {
            fprintf(stdout, "Tree size                            = %llu\n",
                (unsigned long long)bots_number_of_tasks);
        }
    }
    {
        /*CompoundStmt:301*/
        if (bots_verbose_mode >= BOTS_VERBOSE_DEFAULT) {
            fprintf(stdout, "Maximum tree depth                   = %d\n", maxTreeDepth);
        }
    }
    {
        /*CompoundStmt:306*/
        if (bots_verbose_mode >= BOTS_VERBOSE_DEFAULT) {
            fprintf(stdout, "Chunk size                           = %d\n", chunkSize);
        }
    }
    {
        /*CompoundStmt:311*/
        if (bots_verbose_mode >= BOTS_VERBOSE_DEFAULT) {
            fprintf(stdout, "Number of leaves                     = %llu (%.2f%%)\n", nLeaves,
                nLeaves / (float)bots_number_of_tasks * 100.);
        }
    }
    {
        /*CompoundStmt:320*/
        if (bots_verbose_mode >= BOTS_VERBOSE_DEFAULT) {
            fprintf(stdout, "Number of PE's                       = %.4d threads\n", nPes);
        }
    }
    {
        /*CompoundStmt:325*/
        if (bots_verbose_mode >= BOTS_VERBOSE_DEFAULT) {
            fprintf(stdout, "Wallclock time                       = %.3f sec\n", bots_time_program);
        }
    }
    {
        /*CompoundStmt:330*/
        if (bots_verbose_mode >= BOTS_VERBOSE_DEFAULT) {
            fprintf(stdout, "Overall performance                  = %.0f nodes/sec\n",
                (bots_number_of_tasks / bots_time_program));
        }
    }
    {
        /*CompoundStmt:336*/
        if (bots_verbose_mode >= BOTS_VERBOSE_DEFAULT) {
            fprintf(stdout, "Performance per PE                   = %.0f nodes/sec\n",
                (bots_number_of_tasks / bots_time_program / nPes));
        }
    }
}
/*Function: uts_check_result, ID: 35*/
int uts_check_result()
{
    /*uts_check_result:35*/
    /*CompoundStmt:343*/
    int answer = 1;
    if (bots_number_of_tasks != exp_tree_size) {
        answer = 2;
        {
            if (bots_verbose_mode >= BOTS_VERBOSE_DEFAULT) {
                fprintf(stdout, "Incorrect tree size result (%llu instead of %llu).\n",
                    bots_number_of_tasks, exp_tree_size);
            }
        };
    }
    return answer;
}
/*TP151: OMPParallelDirective*/
void TP151::_barrierCodelets151::fire(void)
{
    TP151* myTP = static_cast<TP151*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
void TP151::_checkInCodelets152::fire(void)
{
    /*Select the thread executing OMPSingleDirective 152*/
    if (!__sync_val_compare_and_swap(&(myTP->TP152_alreadyLaunched), 0, 1)) {
        invoke<TP152>(myTP, 1, this->getID(), myTP, &(*(this->inputsTPParent->num_nodes_darts151)),
            &(*(this->inputsTPParent->root_darts151)));
    } else {
        myTP->TPParent->barrierCodelets151[0].decDep();
    }
}
TP151::TP151(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet,
    unsigned long long* in_num_nodes, Node** in_root)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , num_nodes_darts151(in_num_nodes) /*OMP_SHARED - INPUT*/
    , root_darts151(in_root) /*OMP_SHARED - INPUT*/
    , TP152Ptr(nullptr)
    , TP152_alreadyLaunched(0)
    , task153Inputs(new _task153Inputs*[this->numThreads])
    , barrierCodelets151(new _barrierCodelets151[1])
    , checkInCodelets152(new _checkInCodelets152[this->numThreads])
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets151[0] = _barrierCodelets151(ompNumThreads, ompNumThreads, this, 0);
    _checkInCodelets152* checkInCodelets152Ptr = (this->checkInCodelets152);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets152Ptr) = _checkInCodelets152(1, 1, this, codeletCounter);
        (*checkInCodelets152Ptr).decDep();
        checkInCodelets152Ptr++;
    }
}
TP151::~TP151()
{
    delete[] task153Inputs;
    delete[] barrierCodelets151;
    delete[] checkInCodelets152;
}
/*TP152: OMPSingleDirective*/
void TP152::_checkInCodelets153::fire(void)
{

    /*printing node 153: OMPTaskDirective*/
    /*syncNode: OMPParallelDirective 151*/
    /*Find the TP to which the nextRegion belongs*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->TPParent->barrierCodelets151[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 153*/
    _task153Inputs* task153Inputs
        = new _task153Inputs(&(*(this->inputsTPParent->num_nodes_darts152)),
            &(*(this->inputsTPParent->root_darts152)), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task153Inputs[0] = task153Inputs;
    invoke<TP153>(myTP, 1, this->getID(), myTP, task153Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->TPParent->barrierCodelets151[0].decDep();
}
TP152::TP152(int in_numThreads, int in_mainCodeletID, TP151* in_TPParent,
    unsigned long long* in_num_nodes, Node** in_root)
    : ompOMPSingleDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , num_nodes_darts152(in_num_nodes) /*OMP_SHARED - INPUT*/
    , root_darts152(in_root) /*OMP_SHARED - INPUT*/
    , task153Inputs(new _task153Inputs*[this->numThreads])
    , checkInCodelets153(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets153.decDep();
}
TP152::~TP152() { delete[] task153Inputs; }
/*TP153: OMPTaskDirective*/
void TP153::_checkInCodelets155::fire(void)
{
/*Init the vars for this region*/

/*printing node 155: CallExpr*/
/*Finding the next codelet after the function call*/
#if 0
/*Signaling next codelet region: 155 nextRegion: 357 */
myTP->controlTPParent->checkInCodelets357.decDep();
#endif
/*Finding the next sync point before making the function call*/
#if 0
/*Find the sync point for a function call*/
/*syncNode: OMPParallelDirective 151*/
/*No syncpoint found or outside task's boundaries, looking for syncCodelet*/
#endif
    /*Make the function call*/
    invoke<TP_parTreeSearch>(myTP, myTP->numThreads, this->getID(),
        &(myTP->controlTPParent->checkInCodelets357),
        (myTP->controlTPParent->task153Inputs->nextSyncCodelet), nullptr, 0,
        (*(this->taskInputs->root_darts153)), (*(this->taskInputs->root_darts153))->numChildren,
        &(*(this->taskInputs->num_nodes_darts153)));
}
void TP153::_checkInCodelets357::fire(void)
{

    /*printing node 357: NullStmt*/
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task153Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task153Inputs->nextSyncCodelet->decDep();
}
TP153::TP153(
    int in_numThreads, int in_mainCodeletID, TP152* in_TPParent, _task153Inputs* in_task153Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task153Inputs(in_task153Inputs)
    , TP155Ptr(nullptr)
    , TP155_alreadyLaunched(0)
    , checkInCodelets155(1, 1, this, this->mainCodeletID)
    , checkInCodelets357(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets155.decDep();
}
TP153::~TP153() { delete (task153Inputs); }
/*TP32: TP_parTreeSearch*/
void TP32::_checkInCodelets164::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    /*Init the vars for this region*/
    /*Allocate each array var in the codelet */
    this->inputsTPParent->n_outer32_size
        = (this->inputsTPParent->numChildren_darts32[this->getLocalID()]);
    this->inputsTPParent->n_darts32[this->getID()]
        = (Node*)malloc(sizeof(Node) * this->inputsTPParent->n_outer32_size);
    /*Allocate each array var in the codelet */
    this->inputsTPParent->partialCount_outer32_size
        = (this->inputsTPParent->numChildren_darts32[this->getLocalID()]);
    this->inputsTPParent->partialCount_darts32[this->getID()] = (unsigned long long*)malloc(
        sizeof(unsigned long long) * this->inputsTPParent->partialCount_outer32_size);

    /*printing node 164: DeclStmt*/

    /*printing node 165: DeclStmt*/

    /*printing node 166: DeclStmt*/
    this->inputsTPParent->subtreesize_darts32[this->getID()] = 1;

    /*printing node 168: BinaryOperator*/
    (this->inputsTPParent->i_darts32[this->getID()]) = 0;

    /*printing node 169: BinaryOperator*/
    /*Print the code for a condition node in a complex loop stmt */
    if ((this->inputsTPParent->i_darts32[this->getID()])
        < (this->inputsTPParent->numChildren_darts32[this->getID()])) {
        /*Signal the first codelet in the loop*/
        myTP->checkInCodelets167[this->getID()].decDep();
        return;
    } else {
        /*Signal the codelet after the loop from the end condional node.*/
        /*Signaling next codelet region: 170 nextRegion: 198 */
        myTP->controlTPParent->barrierCodelets198[0].decDep();
        return;
    }
}
void TP32::_checkInCodelets167::fire(void)
{

    /*printing node 167: ForStmt*/
    bool haveToLaunch = __sync_bool_compare_and_swap(&(myTP->controlTPParent->TP167_LoopCounter),
        myTP->controlTPParent->TP167_LoopCounterPerThread[this->getID()],
        myTP->controlTPParent->TP167_LoopCounterPerThread[this->getID()] + 1);
    unsigned int iterIdx = myTP->controlTPParent->TP167_LoopCounterPerThread[this->getID()];
    if (haveToLaunch) {
        this->resetCodelet();
        myTP->controlTPParent->TP167PtrVec.push_back(nullptr);
        myTP->controlTPParent->TP167_LoopCounterPerThread[this->getID()] += 1;
        invoke<TP167>(myTP, myTP->numThreads, this->getID(), myTP, myTP->inputsTPParent,
            &(myTP->controlTPParent->TP167PtrVec.back()));
    } else {
        if (myTP->controlTPParent->TP167PtrVec.size() == 0) {
            this->resetCodelet();
            this->decDep();
            return;
        } else if (myTP->controlTPParent->TP167PtrVec.size() < (iterIdx + 1)) {
            this->resetCodelet();
            this->decDep();
            return;
        } else if (myTP->controlTPParent->TP167PtrVec[iterIdx] == nullptr) {
            this->resetCodelet();
            this->decDep();
            return;
        } else {
            this->resetCodelet();
#if USE_SPIN_CODELETS == 0
            myTP->controlTPParent->TP167PtrVec[iterIdx]->firstCodelet[this->getID()].decDep();
#else
            myTP->controlTPParent->TP167PtrVec[iterIdx]->availableCodelets[this->getID()] = 1;
#endif
            myTP->controlTPParent->TP167_LoopCounterPerThread[this->getID()] += 1;
        }
    }
}
void TP32::_checkInCodelets170::fire(void)
{

    /*printing node 170: UnaryOperator*/
    (this->inputsTPParent->i_darts32[this->getID()])++;

    /*printing node 356: BinaryOperator*/
    /*Print the code for a condition node in a complex loop stmt */
    if ((this->inputsTPParent->i_darts32[this->getID()])
        < (this->inputsTPParent->numChildren_darts32[this->getID()])) {
        this->resetCodelet();
        /*Signal the first codelet in the loop*/
        myTP->checkInCodelets167[this->getID()].decDep();
        return;
    } else {
        /*Signal the codelet after the loop from the condtional node.*/
        /*Signaling next codelet region: 170 nextRegion: 198 */
        myTP->controlTPParent->barrierCodelets198[0].decDep();
        return;
    }
}
void TP32::_barrierCodelets198::fire(void)
{
    TP32* myTP = static_cast<TP32*>(myTP_);
    {
        for (size_t codeletsCounter = 0; codeletsCounter < myTP->numThreads; codeletsCounter++) {
            myTP->checkInCodelets199[codeletsCounter].decDep();
        }
    }
}
void TP32::_checkInCodelets199::fire(void)
{

    /*printing node 199: ForStmt*/
    {
        int* i = &(this->inputsTPParent->i_darts32[this->getLocalID()]);
        (void)i /*PRIVATE*/;
        int* numChildren = &(this->inputsTPParent->numChildren_darts32[this->getLocalID()]);
        (void)numChildren /*PRIVATE*/;
        unsigned long long* subtreesize
            = &(this->inputsTPParent->subtreesize_darts32[this->getLocalID()]);
        (void)subtreesize /*PRIVATE*/;
        /*Loop's init*/
        (this->inputsTPParent->i_darts32[this->getID()]) = 0;
        int i_darts_counter_temp32 = (this->inputsTPParent->i_darts32[this->getID()]);
        for (; (i_darts_counter_temp32) < (*numChildren); (i_darts_counter_temp32)++) {
            (*subtreesize) += (this->inputsTPParent
                                   ->partialCount_darts32[this->getID()])[(i_darts_counter_temp32)];
        }
        (this->inputsTPParent->i_darts32[this->getID()]) = i_darts_counter_temp32;
    }

    /*printing node 206: ReturnStmt*/
    /*return*/
    *(this->inputsTPParent->functionResult[this->getID()])
        = (this->inputsTPParent->subtreesize_darts32[this->getID()]);
    myTP->controlTPParent->nextCodeletsparTreeSearch[this->getID()]->decDep();
    return;
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/

    myTP->controlTPParent->nextCodeletsparTreeSearch[this->getID()]->decDep();
}
TP32::TP32(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_mainNextCodelet,
    darts::Codelet* in_mainSyncCodelet, TP32** in_ptrToThisFunctionTP, int in_depth,
    Node* in_parent, int in_numChildren, unsigned long long* in_functionResult)
    : ompTP(in_numThreads, in_mainCodeletID)
    , ptrToThisFunctionTP(in_ptrToThisFunctionTP)
    , inputsTPParent(this)
    , controlTPParent(this)
    , nextCodeletsparTreeSearch(new Codelet*[in_numThreads])
    , nextSyncCodeletsparTreeSearch(new Codelet*[in_numThreads])
    , depth_darts32(new int[this->numThreads])
    , parent_darts32(new Node*[this->numThreads])
    , numChildren_darts32(new int[this->numThreads])
    , i_darts32(new int[this->numThreads])
    , j_darts32(new int[this->numThreads])
    , n_darts32(new Node*[this->numThreads])
    , nodePtr_darts32(new Node*[this->numThreads])
    , partialCount_darts32(new unsigned long long*[this->numThreads])
    , subtreesize_darts32(new unsigned long long[this->numThreads])
    , functionResult(new unsigned long long*[this->numThreads])
    , TP167_LoopCounter(0)
    , TP167_LoopCounterPerThread(new unsigned int[this->numThreads])
    , task192Inputs(new _task192Inputs*[this->numThreads])
    , checkInCodelets164(new _checkInCodelets164[this->numThreads])
    , checkInCodelets167(new _checkInCodelets167[this->numThreads])
    , checkInCodelets170(new _checkInCodelets170[this->numThreads])
    , barrierCodelets198(new _barrierCodelets198[1])
    , checkInCodelets199(new _checkInCodelets199[this->numThreads])
{
    memset((void*)TP167_LoopCounterPerThread, 0, this->numThreads * sizeof(unsigned int));
    barrierCodelets198[0] = _barrierCodelets198(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets199* checkInCodelets199Ptr = (this->checkInCodelets199);
    _checkInCodelets170* checkInCodelets170Ptr = (this->checkInCodelets170);
    _checkInCodelets167* checkInCodelets167Ptr = (this->checkInCodelets167);
    _checkInCodelets164* checkInCodelets164Ptr = (this->checkInCodelets164);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets164);
#endif
    for (size_t codeletCounter = 0; codeletCounter < this->numThreads; codeletCounter++) {
        (*checkInCodelets199Ptr) = _checkInCodelets199(1, 1, this, codeletCounter);
        checkInCodelets199Ptr++;
        (*checkInCodelets170Ptr) = _checkInCodelets170(1, 1, this, codeletCounter);
        checkInCodelets170Ptr++;
        (*checkInCodelets167Ptr) = _checkInCodelets167(1, 1, this, codeletCounter);
        checkInCodelets167Ptr++;
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets164Ptr) = _checkInCodelets164(2, 1, this, codeletCounter);
#else
        (*checkInCodelets164Ptr) = _checkInCodelets164(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets164Ptr).decDep();
        checkInCodelets164Ptr++;
    }
    if (this->numThreads == 1) {
        this->nextCodeletsparTreeSearch[0] = in_mainNextCodelet;
        this->nextSyncCodeletsparTreeSearch[0] = in_mainSyncCodelet;
        this->depth_darts32[0] = in_depth;
        this->parent_darts32[0] = in_parent;
        this->numChildren_darts32[0] = in_numChildren;
        this->functionResult[0] = in_functionResult;
        this->availableCodelets[0] = 1;
    } else {
        this->depth_darts32[this->mainCodeletID] = in_depth;
        this->parent_darts32[this->mainCodeletID] = in_parent;
        this->numChildren_darts32[this->mainCodeletID] = in_numChildren;
        this->functionResult[this->mainCodeletID] = in_functionResult;
        this->nextCodeletsparTreeSearch[in_mainCodeletID] = in_mainNextCodelet;
        this->nextSyncCodeletsparTreeSearch[in_mainCodeletID] = in_mainSyncCodelet;
#if USE_SPIN_CODELETS == 0
        this->firstCodelet[this->mainCodeletID].decDep();
#else
        this->availableCodelets[this->mainCodeletID] = 1;
#endif
        *(this->ptrToThisFunctionTP) = this;
    }
}
TP32::~TP32()
{
    delete[] TP167_LoopCounterPerThread;
    delete[] task192Inputs;
    delete[] checkInCodelets199;
    delete[] barrierCodelets198;
    delete[] checkInCodelets170;
    delete[] checkInCodelets167;
    delete[] checkInCodelets164;
    delete[] nextCodeletsparTreeSearch;
    delete[] nextSyncCodeletsparTreeSearch;
    delete[] depth_darts32;
    delete[] parent_darts32;
    delete[] numChildren_darts32;
    delete[] i_darts32;
    delete[] j_darts32;
    delete[] n_darts32;
    delete[] nodePtr_darts32;
    delete[] partialCount_darts32;
    delete[] subtreesize_darts32;
    delete[] functionResult;
}
void TP32::setNewInputs(int in_depth, Node* in_parent, int in_numChildren,
    unsigned long long* in_functionResult, size_t codeletID)
{
    this->depth_darts32[codeletID] = in_depth;
    this->parent_darts32[codeletID] = in_parent;
    this->numChildren_darts32[codeletID] = in_numChildren;
    this->functionResult[codeletID] = in_functionResult;
}
/*TP167: ForStmt*/
void TP167::_checkInCodelets172::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif

    /*printing node 172: BinaryOperator*/
    (this->inputsTPParent->nodePtr_darts32[this->getID()])
        = &(this->inputsTPParent
                ->n_darts32[this->getID()])[(this->inputsTPParent->i_darts32[this->getID()])];

    /*printing node 175: BinaryOperator*/
    (this->inputsTPParent->nodePtr_darts32[this->getID()])->height
        = (this->inputsTPParent->parent_darts32[this->getID()])->height + 1;

    /*printing node 179: ForStmt*/
    {
        int* i = &(this->inputsTPParent->i_darts32[this->getLocalID()]);
        (void)i /*PRIVATE*/;
        int* j = &(this->inputsTPParent->j_darts32[this->getLocalID()]);
        (void)j /*PRIVATE*/;
        Node** nodePtr = &(this->inputsTPParent->nodePtr_darts32[this->getLocalID()]);
        (void)nodePtr /*PRIVATE*/;
        Node** parent = &(this->inputsTPParent->parent_darts32[this->getLocalID()]);
        (void)parent /*PRIVATE*/;
        /*Loop's init*/
        (this->inputsTPParent->j_darts32[this->getID()]) = 0;
        int j_darts_counter_temp32 = (this->inputsTPParent->j_darts32[this->getID()]);
        for (; (j_darts_counter_temp32) < computeGranularity; (j_darts_counter_temp32)++) {
            rng_spawn((*parent)->state.state, (*nodePtr)->state.state, (*i));
        }
        (this->inputsTPParent->j_darts32[this->getID()]) = j_darts_counter_temp32;
    }

    /*printing node 189: BinaryOperator*/
    (this->inputsTPParent->nodePtr_darts32[this->getID()])->numChildren
        = uts_numChildren((this->inputsTPParent->nodePtr_darts32[this->getID()]));
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 172 nextRegion: 192 */
    myTP->controlTPParent->checkInCodelets192[this->getID()].decDep();
}
void TP167::_checkInCodelets192::fire(void)
{

    /*printing node 192: OMPTaskDirective*/
    /*syncNode: OMPTaskwaitDirective 198*/
    /*Find the TP to which the nextRegion belongs*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->TPParent->barrierCodelets198[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 192*/
    _task192Inputs* task192Inputs
        = new _task192Inputs(&((this->inputsTPParent->depth_darts32[this->getID()])),
            &((this->inputsTPParent->i_darts32[this->getID()])),
            &((this->inputsTPParent->nodePtr_darts32[this->getID()])),
            ((this->inputsTPParent->partialCount_darts32[this->getID()])),
            (this->inputsTPParent->partialCount_outer32_size), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task192Inputs[this->getID()] = task192Inputs;
    invoke<TP192>(myTP, 1, this->getID(), myTP, task192Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*The node is the last one in a complex loop, so signal the inc node*/
    /*Find the TP to which the nextRegion belongs*/
    myTP->controlTPParent->TPParent->checkInCodelets170[this->getID()].decDep();
}
TP167::TP167(int in_numThreads, int in_mainCodeletID, TP32* in_TPParent, TP32* in_inputsTPParent,
    TP167** in_ptrToThisTP)
    : ompTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(in_inputsTPParent)
    , ptrToThisTP(in_ptrToThisTP)
    , checkInCodelets172(new _checkInCodelets172[this->numThreads])
    , checkInCodelets192(new _checkInCodelets192[this->numThreads])
{
    /*Initialize Codelets*/
    _checkInCodelets192* checkInCodelets192Ptr = (this->checkInCodelets192);
    _checkInCodelets172* checkInCodelets172Ptr = (this->checkInCodelets172);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets172);
#endif
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets192Ptr) = _checkInCodelets192(1, 1, this, codeletCounter);
        checkInCodelets192Ptr++;
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets172Ptr) = _checkInCodelets172(2, 1, this, codeletCounter);
#else
        (*checkInCodelets172Ptr) = _checkInCodelets172(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets172Ptr).decDep();
        checkInCodelets172Ptr++;
    }
    *(this->ptrToThisTP) = this;
#if USE_SPIN_CODELETS == 0
    this->firstCodelet[this->getID()].decDep();
#else
    this->availableCodelets[this->getID()] = 1;
#endif
}
TP167::~TP167()
{
    delete[] checkInCodelets192;
    delete[] checkInCodelets172;
}
/*TP192: OMPTaskDirective*/
void TP192::_checkInCodelets195::fire(void)
{
/*Init the vars for this region*/

/*printing node 195: CallExpr*/
/*Finding the next codelet after the function call*/
#if 0
/*Signaling next codelet region: 195 nextRegion: 355 */
myTP->controlTPParent->checkInCodelets355.decDep();
#endif
/*Finding the next sync point before making the function call*/
#if 0
/*Find the sync point for a function call*/
/*syncNode: OMPTaskwaitDirective 198*/
/*No syncpoint found or outside task's boundaries, looking for syncCodelet*/
#endif
    /*Make the function call*/
    invoke<TP_parTreeSearch>(myTP, myTP->numThreads, this->getID(),
        &(myTP->controlTPParent->checkInCodelets355),
        (myTP->controlTPParent->task192Inputs->nextSyncCodelet), nullptr,
        (this->taskInputs->depth_darts192) + 1, (this->taskInputs->nodePtr_darts192),
        (this->taskInputs->nodePtr_darts192)->numChildren,
        &((this->taskInputs->partialCount_darts192[this->taskInputs->i_darts192])));
}
void TP192::_checkInCodelets355::fire(void)
{

    /*printing node 355: NullStmt*/
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task192Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task192Inputs->nextSyncCodelet->decDep();
}
TP192::TP192(
    int in_numThreads, int in_mainCodeletID, TP167* in_TPParent, _task192Inputs* in_task192Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task192Inputs(in_task192Inputs)
    , TP195Ptr(nullptr)
    , TP195_alreadyLaunched(0)
    , checkInCodelets195(1, 1, this, this->mainCodeletID)
    , checkInCodelets355(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets195.decDep();
}
TP192::~TP192() { delete (task192Inputs); }
