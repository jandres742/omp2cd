#include "sort.output.darts.h"
using namespace darts;
using namespace std;
static unsigned long rand_nxt = 0;
ELM* sort_array;
ELM* tmp;
static inline ELM med3(ELM a, ELM b, ELM c);
static inline unsigned long my_rand();
static inline ELM choose_pivot(ELM* low, ELM* high);
static ELM* seqpart(ELM* low, ELM* high);
static void insertion_sort(ELM* low, ELM* high);
static inline void my_srand(unsigned long seed);
/*Function: my_rand, ID: 15*/
static inline unsigned long my_rand()
{
    /*my_rand:15*/
    /*CompoundStmt:63*/
    rand_nxt = rand_nxt * 1103515245 + 12345;
    return rand_nxt;
}
/*Function: my_srand, ID: 16*/
static inline void my_srand(unsigned long seed)
{
    /*my_srand:16*/
    /*CompoundStmt:68*/
    rand_nxt = seed;
}
/*Function: med3, ID: 17*/
static inline ELM med3(ELM a, ELM b, ELM c)
{
    /*med3:17*/
    /*CompoundStmt:70*/
    if (a < b) {
        if (b < c) {
            return b;
        } else {
            if (a < c)
                return c;
            else
                return a;
        }
    } else {
        if (b > c) {
            return b;
        } else {
            if (a > c)
                return c;
            else
                return a;
        }
    }
}
/*Function: choose_pivot, ID: 18*/
static inline ELM choose_pivot(ELM* low, ELM* high)
{
    /*choose_pivot:18*/
    /*CompoundStmt:97*/
    return med3(*low, *high, low[(high - low) / 2]);
}
/*Function: seqpart, ID: 19*/
static ELM* seqpart(ELM* low, ELM* high)
{
    /*seqpart:19*/
    /*CompoundStmt:105*/
    ELM pivot;
    ELM h, l;
    ELM* curr_low = low;
    ELM* curr_high = high;
    pivot = choose_pivot(low, high);
    while (1) {
        while ((h = *curr_high) > pivot)
            curr_high--;
        while ((l = *curr_low) < pivot)
            curr_low++;
        if (curr_low >= curr_high)
            break;
        *curr_high-- = l;
        *curr_low++ = h;
    }
    if (curr_high < high)
        return curr_high;
    else
        return curr_high - 1;
}
/*Function: insertion_sort, ID: 20*/
static void insertion_sort(ELM* low, ELM* high)
{
    /*insertion_sort:20*/
    /*CompoundStmt:142*/
    ELM *p, *q;
    ELM a, b;
    for (q = low + 1; q <= high; ++q) {
        a = q[0];
        for (p = q - 1; p >= low && (b = p[0]) > a; p--)
            p[1] = b;
        p[1] = a;
    }
}
/*Function: seqquick, ID: 21*/
void seqquick(ELM* low, ELM* high)
{
    /*seqquick:21*/
    /*CompoundStmt:166*/
    ELM* p;
    while (high - low >= bots_app_cutoff_value_2) {
        p = seqpart(low, high);
        seqquick(low, p);
        low = p + 1;
    }
    insertion_sort(low, high);
}
/*Function: seqmerge, ID: 22*/
void seqmerge(ELM* low1, ELM* high1, ELM* low2, ELM* high2, ELM* lowdest)
{
    /*seqmerge:22*/
    /*CompoundStmt:178*/
    ELM a1, a2;
    if (low1 < high1 && low2 < high2) {
        a1 = *low1;
        a2 = *low2;
        for (;;) {
            if (a1 < a2) {
                *lowdest++ = a1;
                a1 = *++low1;
                if (low1 >= high1)
                    break;
            } else {
                *lowdest++ = a2;
                a2 = *++low2;
                if (low2 >= high2)
                    break;
            }
        }
    }
    if (low1 <= high1 && low2 <= high2) {
        a1 = *low1;
        a2 = *low2;
        for (;;) {
            if (a1 < a2) {
                *lowdest++ = a1;
                ++low1;
                if (low1 > high1)
                    break;
                a1 = *low1;
            } else {
                *lowdest++ = a2;
                ++low2;
                if (low2 > high2)
                    break;
                a2 = *low2;
            }
        }
    }
    if (low1 > high1) {
        memcpy(lowdest, low2, sizeof(ELM) * (high2 - low2 + 1));
    } else {
        memcpy(lowdest, low1, sizeof(ELM) * (high1 - low1 + 1));
    }
}
/*Function: binsplit, ID: 23*/
ELM* binsplit(ELM val, ELM* low, ELM* high)
{
    /*binsplit:23*/
    /*CompoundStmt:260*/
    ELM* mid;
    while (low != high) {
        mid = low + ((high - low + 1) >> 1);
        if (val <= *mid)
            high = mid - 1;
        else
            low = mid;
    }
    if (*low > val)
        return low - 1;
    else
        return low;
}
/*Function: scramble_array, ID: 26*/
void scramble_array(ELM* in_array)
{
    /*scramble_array:26*/
    /*CompoundStmt:394*/
    unsigned long i;
    unsigned long j;
    for (i = 0; i < (unsigned long)bots_arg_size; ++i) {
        j = my_rand();
        j = j % bots_arg_size;
        {
            ELM tmp;
            tmp = in_array[i];
            in_array[i] = in_array[j];
            in_array[j] = tmp;
        };
    }
}
/*Function: fill_array, ID: 27*/
void fill_array(ELM* in_array)
{
    /*fill_array:27*/
    /*CompoundStmt:416*/
    unsigned long i;
    my_srand(1);
    for (i = 0; i < (unsigned long)bots_arg_size; ++i) {
        in_array[i] = i;
    }
}
/*Function: sort_init, ID: 28*/
void sort_init()
{
    /*sort_init:28*/
    /*CompoundStmt:427*/
    if (bots_arg_size < 4) {
        {
            if (bots_verbose_mode >= BOTS_VERBOSE_DEFAULT) {
                fprintf(
                    stdout, "%s can not be less than 4, using 4 as a parameter.\n", "Array size");
            }
        };
        bots_arg_size = 4;
    }
    if (bots_app_cutoff_value < 2) {
        {
            if (bots_verbose_mode >= BOTS_VERBOSE_DEFAULT) {
                fprintf(stdout, "%s can not be less than 2, using 2 as a parameter.\n",
                    "Sequential Merge cutoff value");
            }
        };
        bots_app_cutoff_value = 2;
    } else if (bots_app_cutoff_value > bots_arg_size) {
        {
            if (bots_verbose_mode >= BOTS_VERBOSE_DEFAULT) {
                fprintf(stdout,
                    "%s can not be greather than vector size, using %d as a parameter.\n",
                    "Sequential Merge cutoff value", bots_arg_size);
            }
        };
        bots_app_cutoff_value = bots_arg_size;
    }
    if (bots_app_cutoff_value_1 > bots_arg_size) {
        {
            if (bots_verbose_mode >= BOTS_VERBOSE_DEFAULT) {
                fprintf(stdout,
                    "%s can not be greather than vector size, using %d as a parameter.\n",
                    "Sequential Quicksort cutoff value", bots_arg_size);
            }
        };
        bots_app_cutoff_value_1 = bots_arg_size;
    }
    if (bots_app_cutoff_value_2 > bots_arg_size) {
        {
            if (bots_verbose_mode >= BOTS_VERBOSE_DEFAULT) {
                fprintf(stdout,
                    "%s can not be greather than vector size, using %d as a parameter.\n",
                    "Sequential Insertion cutoff value", bots_arg_size);
            }
        };
        bots_app_cutoff_value_2 = bots_arg_size;
    }
    if (bots_app_cutoff_value_2 > bots_app_cutoff_value_1) {
        {
            if (bots_verbose_mode >= BOTS_VERBOSE_DEFAULT) {
                fprintf(stdout, "%s can not be greather than %s, using %d as a parameter.\n",
                    "Sequential Insertion cutoff value", "Sequential Quicksort cutoff value",
                    bots_app_cutoff_value_1);
            }
        };
        bots_app_cutoff_value_2 = bots_app_cutoff_value_1;
    }
    sort_array = (ELM*)malloc(bots_arg_size * sizeof(ELM));
    tmp = (ELM*)malloc(bots_arg_size * sizeof(ELM));
    fill_array(sort_array);
    scramble_array(sort_array);
}
/*Function: sort_par, ID: 29*/
void sort_par()
{
    /*sort_par:29*/
    /*CompoundStmt:495*/
    {
        /*CompoundStmt:496*/
        if (bots_verbose_mode >= BOTS_VERBOSE_DEFAULT) {
            fprintf(stdout, "Computing multisort algorithm (n=%d) ", bots_arg_size);
        }
    }
    if (affinMaskRes) {
        myDARTSRuntime->run(
            launch<TP501>(ompNumThreads * DARTS_CODELETS_MULT, 0, RuntimeFinalCodelet));
    }
    {
        /*CompoundStmt:505*/
        if (bots_verbose_mode >= BOTS_VERBOSE_DEFAULT) {
            fprintf(stdout, " completed!\n");
        }
    }
}
/*Function: sort_verify, ID: 30*/
int sort_verify()
{
    /*sort_verify:30*/
    /*CompoundStmt:510*/
    int i, success = 1;
    for (i = 0; i < (int)bots_arg_size; ++i)
        if (sort_array[i] != i)
            success = 0;
    return success ? 1 : 2;
}
/*TP24: TP_cilkmerge_par*/
void TP24::_checkInCodelets285::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    /*Init the vars for this region*/

    /*printing node 285: DeclStmt*/

    /*printing node 286: DeclStmt*/

    /*printing node 287: IfStmt*/
    if ((this->inputsTPParent->high2_darts24[this->getID()])
            - (this->inputsTPParent->low2_darts24[this->getID()])
        > (this->inputsTPParent->high1_darts24[this->getID()])
            - (this->inputsTPParent->low1_darts24[this->getID()])) {
        {
            (this->inputsTPParent->tmp_darts24[this->getID()])
                = (this->inputsTPParent->low1_darts24[this->getID()]);
            (this->inputsTPParent->low1_darts24[this->getID()])
                = (this->inputsTPParent->low2_darts24[this->getID()]);
            (this->inputsTPParent->low2_darts24[this->getID()])
                = (this->inputsTPParent->tmp_darts24[this->getID()]);
        }
        {
            (this->inputsTPParent->tmp_darts24[this->getID()])
                = (this->inputsTPParent->high1_darts24[this->getID()]);
            (this->inputsTPParent->high1_darts24[this->getID()])
                = (this->inputsTPParent->high2_darts24[this->getID()]);
            (this->inputsTPParent->high2_darts24[this->getID()])
                = (this->inputsTPParent->tmp_darts24[this->getID()]);
        }
    }

    /*printing node 300: IfStmt*/
    if ((this->inputsTPParent->high2_darts24[this->getID()])
        < (this->inputsTPParent->low2_darts24[this->getID()])) {
        memcpy((this->inputsTPParent->lowdest_darts24[this->getID()]),
            (this->inputsTPParent->low1_darts24[this->getID()]),
            sizeof(ELM) * ((this->inputsTPParent->high1_darts24[this->getID()])
                              - (this->inputsTPParent->low1_darts24[this->getID()])));
        /*return*/
        myTP->controlTPParent->nextCodeletscilkmerge_par[this->getID()]->decDep();
        return;
    }

    /*printing node 308: IfStmt*/
    if ((this->inputsTPParent->high2_darts24[this->getID()])
            - (this->inputsTPParent->low2_darts24[this->getID()])
        < bots_app_cutoff_value) {
        seqmerge((this->inputsTPParent->low1_darts24[this->getID()]),
            (this->inputsTPParent->high1_darts24[this->getID()]),
            (this->inputsTPParent->low2_darts24[this->getID()]),
            (this->inputsTPParent->high2_darts24[this->getID()]),
            (this->inputsTPParent->lowdest_darts24[this->getID()]));
        /*return*/
        myTP->controlTPParent->nextCodeletscilkmerge_par[this->getID()]->decDep();
        return;
    }

    /*printing node 313: BinaryOperator*/
    (this->inputsTPParent->split1_darts24[this->getID()])
        = (((this->inputsTPParent->high1_darts24[this->getID()])
               - (this->inputsTPParent->low1_darts24[this->getID()]) + 1)
              / 2)
        + (this->inputsTPParent->low1_darts24[this->getID()]);

    /*printing node 318: BinaryOperator*/
    (this->inputsTPParent->split2_darts24[this->getID()])
        = binsplit(*(this->inputsTPParent->split1_darts24[this->getID()]),
            (this->inputsTPParent->low2_darts24[this->getID()]),
            (this->inputsTPParent->high2_darts24[this->getID()]));

    /*printing node 321: BinaryOperator*/
    (this->inputsTPParent->lowsize_darts24[this->getID()])
        = (this->inputsTPParent->split1_darts24[this->getID()])
        - (this->inputsTPParent->low1_darts24[this->getID()])
        + (this->inputsTPParent->split2_darts24[this->getID()])
        - (this->inputsTPParent->low2_darts24[this->getID()]);

    /*printing node 325: BinaryOperator*/
    *((this->inputsTPParent->lowdest_darts24[this->getID()])
        + (this->inputsTPParent->lowsize_darts24[this->getID()]) + 1)
        = *(this->inputsTPParent->split1_darts24[this->getID()]);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 285 nextRegion: 330 */
    myTP->controlTPParent->checkInCodelets330[this->getID()].decDep();
}
void TP24::_checkInCodelets330::fire(void)
{

    /*printing node 330: OMPTaskDirective*/
    /*syncNode: OMPTaskwaitDirective 339*/
    /*Find the TP to which the nextRegion belongs*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->barrierCodelets339[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 330*/
    _task330Inputs* task330Inputs
        = new _task330Inputs(&((this->inputsTPParent->low1_darts24[this->getID()])),
            &((this->inputsTPParent->low2_darts24[this->getID()])),
            &((this->inputsTPParent->lowdest_darts24[this->getID()])),
            &((this->inputsTPParent->split1_darts24[this->getID()])),
            &((this->inputsTPParent->split2_darts24[this->getID()])), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task330Inputs[this->getID()] = task330Inputs;
    invoke<TP330>(myTP, 1, this->getID(), myTP, task330Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 330 nextRegion: 333 */
    myTP->controlTPParent->checkInCodelets333[this->getID()].decDep();
}
void TP24::_checkInCodelets333::fire(void)
{

    /*printing node 333: OMPTaskDirective*/
    /*syncNode: OMPTaskwaitDirective 339*/
    /*Find the TP to which the nextRegion belongs*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->barrierCodelets339[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 333*/
    _task333Inputs* task333Inputs
        = new _task333Inputs(&((this->inputsTPParent->high1_darts24[this->getID()])),
            &((this->inputsTPParent->high2_darts24[this->getID()])),
            &((this->inputsTPParent->lowdest_darts24[this->getID()])),
            &((this->inputsTPParent->lowsize_darts24[this->getID()])),
            &((this->inputsTPParent->split1_darts24[this->getID()])),
            &((this->inputsTPParent->split2_darts24[this->getID()])), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task333Inputs[this->getID()] = task333Inputs;
    invoke<TP333>(myTP, 1, this->getID(), myTP, task333Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 333 nextRegion: 339 */
    myTP->controlTPParent->barrierCodelets339[0].decDep();
}
void TP24::_barrierCodelets339::fire(void)
{
    TP24* myTP = static_cast<TP24*>(myTP_);
    {
        for (size_t codeletsCounter = 0; codeletsCounter < myTP->numThreads; codeletsCounter++) {
            myTP->checkInCodelets340[codeletsCounter].decDep();
        }
    }
}
void TP24::_checkInCodelets340::fire(void)
{

    /*printing node 340: ReturnStmt*/
    /*return*/
    myTP->controlTPParent->nextCodeletscilkmerge_par[this->getID()]->decDep();
    return;
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/

    myTP->controlTPParent->nextCodeletscilkmerge_par[this->getID()]->decDep();
}
TP24::TP24(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_mainNextCodelet,
    darts::Codelet* in_mainSyncCodelet, TP24** in_ptrToThisFunctionTP, ELM* in_low1, ELM* in_high1,
    ELM* in_low2, ELM* in_high2, ELM* in_lowdest)
    : ompTP(in_numThreads, in_mainCodeletID)
    , ptrToThisFunctionTP(in_ptrToThisFunctionTP)
    , inputsTPParent(this)
    , controlTPParent(this)
    , nextCodeletscilkmerge_par(new Codelet*[in_numThreads])
    , nextSyncCodeletscilkmerge_par(new Codelet*[in_numThreads])
    , low1_darts24(new ELM*[this->numThreads])
    , high1_darts24(new ELM*[this->numThreads])
    , low2_darts24(new ELM*[this->numThreads])
    , high2_darts24(new ELM*[this->numThreads])
    , lowdest_darts24(new ELM*[this->numThreads])
    , lowsize_darts24(new long[this->numThreads])
    , split1_darts24(new ELM*[this->numThreads])
    , split2_darts24(new ELM*[this->numThreads])
    , tmp_darts24(new ELM*[this->numThreads])
    , task330Inputs(new _task330Inputs*[this->numThreads])
    , task333Inputs(new _task333Inputs*[this->numThreads])
    , checkInCodelets285(new _checkInCodelets285[this->numThreads])
    , checkInCodelets330(new _checkInCodelets330[this->numThreads])
    , checkInCodelets333(new _checkInCodelets333[this->numThreads])
    , barrierCodelets339(new _barrierCodelets339[1])
    , checkInCodelets340(new _checkInCodelets340[this->numThreads])
{
    barrierCodelets339[0] = _barrierCodelets339(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets340* checkInCodelets340Ptr = (this->checkInCodelets340);
    _checkInCodelets333* checkInCodelets333Ptr = (this->checkInCodelets333);
    _checkInCodelets330* checkInCodelets330Ptr = (this->checkInCodelets330);
    _checkInCodelets285* checkInCodelets285Ptr = (this->checkInCodelets285);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets285);
#endif
    for (size_t codeletCounter = 0; codeletCounter < this->numThreads; codeletCounter++) {
        (*checkInCodelets340Ptr) = _checkInCodelets340(1, 1, this, codeletCounter);
        checkInCodelets340Ptr++;
        (*checkInCodelets333Ptr) = _checkInCodelets333(1, 1, this, codeletCounter);
        checkInCodelets333Ptr++;
        (*checkInCodelets330Ptr) = _checkInCodelets330(1, 1, this, codeletCounter);
        checkInCodelets330Ptr++;
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets285Ptr) = _checkInCodelets285(2, 1, this, codeletCounter);
#else
        (*checkInCodelets285Ptr) = _checkInCodelets285(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets285Ptr).decDep();
        checkInCodelets285Ptr++;
    }
    if (this->numThreads == 1) {
        this->nextCodeletscilkmerge_par[0] = in_mainNextCodelet;
        this->nextSyncCodeletscilkmerge_par[0] = in_mainSyncCodelet;
        this->low1_darts24[0] = in_low1;
        this->high1_darts24[0] = in_high1;
        this->low2_darts24[0] = in_low2;
        this->high2_darts24[0] = in_high2;
        this->lowdest_darts24[0] = in_lowdest;
        this->tmp_darts24[0] = tmp;
        this->availableCodelets[0] = 1;
    } else {
        this->low1_darts24[this->mainCodeletID] = in_low1;
        this->high1_darts24[this->mainCodeletID] = in_high1;
        this->low2_darts24[this->mainCodeletID] = in_low2;
        this->high2_darts24[this->mainCodeletID] = in_high2;
        this->lowdest_darts24[this->mainCodeletID] = in_lowdest;
        this->tmp_darts24[this->mainCodeletID] = tmp;
        this->nextCodeletscilkmerge_par[in_mainCodeletID] = in_mainNextCodelet;
        this->nextSyncCodeletscilkmerge_par[in_mainCodeletID] = in_mainSyncCodelet;
#if USE_SPIN_CODELETS == 0
        this->firstCodelet[this->mainCodeletID].decDep();
#else
        this->availableCodelets[this->mainCodeletID] = 1;
#endif
        *(this->ptrToThisFunctionTP) = this;
    }
}
TP24::~TP24()
{
    delete[] task330Inputs;
    delete[] task333Inputs;
    delete[] checkInCodelets340;
    delete[] barrierCodelets339;
    delete[] checkInCodelets333;
    delete[] checkInCodelets330;
    delete[] checkInCodelets285;
    delete[] nextCodeletscilkmerge_par;
    delete[] nextSyncCodeletscilkmerge_par;
    delete[] low1_darts24;
    delete[] high1_darts24;
    delete[] low2_darts24;
    delete[] high2_darts24;
    delete[] lowdest_darts24;
    delete[] lowsize_darts24;
    delete[] split1_darts24;
    delete[] split2_darts24;
    delete[] tmp_darts24;
}
void TP24::setNewInputs(
    ELM* in_low1, ELM* in_high1, ELM* in_low2, ELM* in_high2, ELM* in_lowdest, size_t codeletID)
{
    this->low1_darts24[codeletID] = in_low1;
    this->high1_darts24[codeletID] = in_high1;
    this->low2_darts24[codeletID] = in_low2;
    this->high2_darts24[codeletID] = in_high2;
    this->lowdest_darts24[codeletID] = in_lowdest;
}
/*TP330: OMPTaskDirective*/
void TP330::_checkInCodelets331::fire(void)
{
/*Init the vars for this region*/

/*printing node 331: CallExpr*/
/*Finding the next codelet after the function call*/
#if 0
/*Signaling next codelet region: 331 nextRegion: 530 */
myTP->controlTPParent->checkInCodelets530.decDep();
#endif
/*Finding the next sync point before making the function call*/
#if 0
/*Find the sync point for a function call*/
/*syncNode: OMPTaskwaitDirective 339*/
/*No syncpoint found or outside task's boundaries, looking for syncCodelet*/
#endif
    /*Make the function call*/
    invoke<TP_cilkmerge_par>(myTP, myTP->numThreads, this->getID(),
        &(myTP->controlTPParent->checkInCodelets530),
        (myTP->controlTPParent->task330Inputs->nextSyncCodelet), nullptr,
        (this->taskInputs->low1_darts330), (this->taskInputs->split1_darts330) - 1,
        (this->taskInputs->low2_darts330), (this->taskInputs->split2_darts330),
        (this->taskInputs->lowdest_darts330));
}
void TP330::_checkInCodelets530::fire(void)
{

    /*printing node 530: NullStmt*/
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task330Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task330Inputs->nextSyncCodelet->decDep();
}
TP330::TP330(
    int in_numThreads, int in_mainCodeletID, TP24* in_TPParent, _task330Inputs* in_task330Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task330Inputs(in_task330Inputs)
    , TP331Ptr(nullptr)
    , TP331_alreadyLaunched(0)
    , checkInCodelets331(1, 1, this, this->mainCodeletID)
    , checkInCodelets530(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets331.decDep();
}
TP330::~TP330() { delete (task330Inputs); }
/*TP333: OMPTaskDirective*/
void TP333::_checkInCodelets334::fire(void)
{
/*Init the vars for this region*/

/*printing node 334: CallExpr*/
/*Finding the next codelet after the function call*/
#if 0
/*Signaling next codelet region: 334 nextRegion: 531 */
myTP->controlTPParent->checkInCodelets531.decDep();
#endif
/*Finding the next sync point before making the function call*/
#if 0
/*Find the sync point for a function call*/
/*syncNode: OMPTaskwaitDirective 339*/
/*No syncpoint found or outside task's boundaries, looking for syncCodelet*/
#endif
    /*Make the function call*/
    invoke<TP_cilkmerge_par>(myTP, myTP->numThreads, this->getID(),
        &(myTP->controlTPParent->checkInCodelets531),
        (myTP->controlTPParent->task333Inputs->nextSyncCodelet), nullptr,
        (this->taskInputs->split1_darts333) + 1, (this->taskInputs->high1_darts333),
        (this->taskInputs->split2_darts333) + 1, (this->taskInputs->high2_darts333),
        (this->taskInputs->lowdest_darts333) + (this->taskInputs->lowsize_darts333) + 2);
}
void TP333::_checkInCodelets531::fire(void)
{

    /*printing node 531: NullStmt*/
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task333Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task333Inputs->nextSyncCodelet->decDep();
}
TP333::TP333(
    int in_numThreads, int in_mainCodeletID, TP24* in_TPParent, _task333Inputs* in_task333Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task333Inputs(in_task333Inputs)
    , TP334Ptr(nullptr)
    , TP334_alreadyLaunched(0)
    , checkInCodelets334(1, 1, this, this->mainCodeletID)
    , checkInCodelets531(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets334.decDep();
}
TP333::~TP333() { delete (task333Inputs); }
/*TP25: TP_cilksort_par*/
void TP25::_checkInCodelets342::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    /*Init the vars for this region*/

    /*printing node 342: DeclStmt*/
    this->inputsTPParent->quarter_darts25[this->getID()]
        = (this->inputsTPParent->size_darts25[this->getID()]) / 4;

    /*printing node 344: DeclStmt*/

    /*printing node 345: IfStmt*/
    if ((this->inputsTPParent->size_darts25[this->getID()]) < bots_app_cutoff_value_1) {
        seqquick((this->inputsTPParent->low_darts25[this->getID()]),
            (this->inputsTPParent->low_darts25[this->getID()])
                + (this->inputsTPParent->size_darts25[this->getID()]) - 1);
        /*return*/
        myTP->controlTPParent->nextCodeletscilksort_par[this->getID()]->decDep();
        return;
    }

    /*printing node 352: BinaryOperator*/
    (this->inputsTPParent->A_darts25[this->getID()])
        = (this->inputsTPParent->low_darts25[this->getID()]);

    /*printing node 353: BinaryOperator*/
    (this->inputsTPParent->tmpA_darts25[this->getID()])
        = (this->inputsTPParent->tmp_darts25[this->getID()]);

    /*printing node 354: BinaryOperator*/
    (this->inputsTPParent->B_darts25[this->getID()])
        = (this->inputsTPParent->A_darts25[this->getID()])
        + (this->inputsTPParent->quarter_darts25[this->getID()]);

    /*printing node 356: BinaryOperator*/
    (this->inputsTPParent->tmpB_darts25[this->getID()])
        = (this->inputsTPParent->tmpA_darts25[this->getID()])
        + (this->inputsTPParent->quarter_darts25[this->getID()]);

    /*printing node 358: BinaryOperator*/
    (this->inputsTPParent->C_darts25[this->getID()])
        = (this->inputsTPParent->B_darts25[this->getID()])
        + (this->inputsTPParent->quarter_darts25[this->getID()]);

    /*printing node 360: BinaryOperator*/
    (this->inputsTPParent->tmpC_darts25[this->getID()])
        = (this->inputsTPParent->tmpB_darts25[this->getID()])
        + (this->inputsTPParent->quarter_darts25[this->getID()]);

    /*printing node 362: BinaryOperator*/
    (this->inputsTPParent->D_darts25[this->getID()])
        = (this->inputsTPParent->C_darts25[this->getID()])
        + (this->inputsTPParent->quarter_darts25[this->getID()]);

    /*printing node 364: BinaryOperator*/
    (this->inputsTPParent->tmpD_darts25[this->getID()])
        = (this->inputsTPParent->tmpC_darts25[this->getID()])
        + (this->inputsTPParent->quarter_darts25[this->getID()]);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 342 nextRegion: 366 */
    myTP->controlTPParent->checkInCodelets366[this->getID()].decDep();
}
void TP25::_checkInCodelets366::fire(void)
{

    /*printing node 366: OMPTaskDirective*/
    /*syncNode: OMPTaskwaitDirective 376*/
    /*Find the TP to which the nextRegion belongs*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->barrierCodelets376[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 366*/
    _task366Inputs* task366Inputs
        = new _task366Inputs(&((this->inputsTPParent->A_darts25[this->getID()])),
            &((this->inputsTPParent->quarter_darts25[this->getID()])),
            &((this->inputsTPParent->tmpA_darts25[this->getID()])), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task366Inputs[this->getID()] = task366Inputs;
    invoke<TP366>(myTP, 1, this->getID(), myTP, task366Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 366 nextRegion: 368 */
    myTP->controlTPParent->checkInCodelets368[this->getID()].decDep();
}
void TP25::_checkInCodelets368::fire(void)
{

    /*printing node 368: OMPTaskDirective*/
    /*syncNode: OMPTaskwaitDirective 376*/
    /*Find the TP to which the nextRegion belongs*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->barrierCodelets376[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 368*/
    _task368Inputs* task368Inputs
        = new _task368Inputs(&((this->inputsTPParent->B_darts25[this->getID()])),
            &((this->inputsTPParent->quarter_darts25[this->getID()])),
            &((this->inputsTPParent->tmpB_darts25[this->getID()])), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task368Inputs[this->getID()] = task368Inputs;
    invoke<TP368>(myTP, 1, this->getID(), myTP, task368Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 368 nextRegion: 370 */
    myTP->controlTPParent->checkInCodelets370[this->getID()].decDep();
}
void TP25::_checkInCodelets370::fire(void)
{

    /*printing node 370: OMPTaskDirective*/
    /*syncNode: OMPTaskwaitDirective 376*/
    /*Find the TP to which the nextRegion belongs*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->barrierCodelets376[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 370*/
    _task370Inputs* task370Inputs
        = new _task370Inputs(&((this->inputsTPParent->C_darts25[this->getID()])),
            &((this->inputsTPParent->quarter_darts25[this->getID()])),
            &((this->inputsTPParent->tmpC_darts25[this->getID()])), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task370Inputs[this->getID()] = task370Inputs;
    invoke<TP370>(myTP, 1, this->getID(), myTP, task370Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 370 nextRegion: 372 */
    myTP->controlTPParent->checkInCodelets372[this->getID()].decDep();
}
void TP25::_checkInCodelets372::fire(void)
{

    /*printing node 372: OMPTaskDirective*/
    /*syncNode: OMPTaskwaitDirective 376*/
    /*Find the TP to which the nextRegion belongs*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->barrierCodelets376[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 372*/
    _task372Inputs* task372Inputs
        = new _task372Inputs(&((this->inputsTPParent->D_darts25[this->getID()])),
            &((this->inputsTPParent->quarter_darts25[this->getID()])),
            &((this->inputsTPParent->size_darts25[this->getID()])),
            &((this->inputsTPParent->tmpD_darts25[this->getID()])), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task372Inputs[this->getID()] = task372Inputs;
    invoke<TP372>(myTP, 1, this->getID(), myTP, task372Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 372 nextRegion: 376 */
    myTP->controlTPParent->barrierCodelets376[0].decDep();
}
void TP25::_barrierCodelets376::fire(void)
{
    TP25* myTP = static_cast<TP25*>(myTP_);
    {
        for (size_t codeletsCounter = 0; codeletsCounter < myTP->numThreads; codeletsCounter++) {
            myTP->checkInCodelets377[codeletsCounter].decDep();
        }
    }
}
void TP25::_checkInCodelets377::fire(void)
{

    /*printing node 377: OMPTaskDirective*/
    /*syncNode: OMPTaskwaitDirective 389*/
    /*Find the TP to which the nextRegion belongs*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->barrierCodelets389[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 377*/
    _task377Inputs* task377Inputs
        = new _task377Inputs(&((this->inputsTPParent->A_darts25[this->getID()])),
            &((this->inputsTPParent->B_darts25[this->getID()])),
            &((this->inputsTPParent->quarter_darts25[this->getID()])),
            &((this->inputsTPParent->tmpA_darts25[this->getID()])), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task377Inputs[this->getID()] = task377Inputs;
    invoke<TP377>(myTP, 1, this->getID(), myTP, task377Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 377 nextRegion: 383 */
    myTP->controlTPParent->checkInCodelets383[this->getID()].decDep();
}
void TP25::_checkInCodelets383::fire(void)
{

    /*printing node 383: OMPTaskDirective*/
    /*syncNode: OMPTaskwaitDirective 389*/
    /*Find the TP to which the nextRegion belongs*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->barrierCodelets389[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 383*/
    _task383Inputs* task383Inputs
        = new _task383Inputs(&((this->inputsTPParent->C_darts25[this->getID()])),
            &((this->inputsTPParent->D_darts25[this->getID()])),
            &((this->inputsTPParent->low_darts25[this->getID()])),
            &((this->inputsTPParent->quarter_darts25[this->getID()])),
            &((this->inputsTPParent->size_darts25[this->getID()])),
            &((this->inputsTPParent->tmpC_darts25[this->getID()])), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task383Inputs[this->getID()] = task383Inputs;
    invoke<TP383>(myTP, 1, this->getID(), myTP, task383Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 383 nextRegion: 389 */
    myTP->controlTPParent->barrierCodelets389[0].decDep();
}
void TP25::_barrierCodelets389::fire(void)
{
    TP25* myTP = static_cast<TP25*>(myTP_);
    {
        for (size_t codeletsCounter = 0; codeletsCounter < myTP->numThreads; codeletsCounter++) {
            myTP->checkInCodelets390[codeletsCounter].decDep();
        }
    }
}
void TP25::_checkInCodelets390::fire(void)
{

    /*printing node 390: CallExpr*/
    if (!__sync_val_compare_and_swap(&(myTP->controlTPParent->TP390_alreadyLaunched), 0, 1)) {
/*Finding the next codelet after the function call*/
#if 0
/*Find and signal the next codelet*/

myTP->controlTPParent->nextCodeletscilksort_par[this->getID()]->decDep();
#endif
/*Finding the next sync point before making the function call*/
#if 0
/*Find the sync point for a function call*/
/*No syncpoint found or outside task's boundaries, looking for syncCodelet*/
#endif
        /*Make the function call*/
        invoke<TP_cilkmerge_par>(myTP, myTP->numThreads, this->getID(),
            (myTP->controlTPParent->nextCodeletscilksort_par[this->getID()]),
            (myTP->controlTPParent->nextSyncCodeletscilksort_par[this->getID()]),
            &(myTP->controlTPParent->TP390Ptr), (this->inputsTPParent->tmpA_darts25[this->getID()]),
            (this->inputsTPParent->tmpC_darts25[this->getID()]) - 1,
            (this->inputsTPParent->tmpC_darts25[this->getID()]),
            (this->inputsTPParent->tmpA_darts25[this->getID()])
                + (this->inputsTPParent->size_darts25[this->getID()]) - 1,
            (this->inputsTPParent->A_darts25[this->getID()]));
    } else {
        if (myTP->controlTPParent->TP390Ptr == nullptr) {
            myTP->add(this);
            return;
        } else {
            myTP->controlTPParent->TP390Ptr->setNewInputs(
                (this->inputsTPParent->tmpA_darts25[this->getID()]),
                (this->inputsTPParent->tmpC_darts25[this->getID()]) - 1,
                (this->inputsTPParent->tmpC_darts25[this->getID()]),
                (this->inputsTPParent->tmpA_darts25[this->getID()])
                    + (this->inputsTPParent->size_darts25[this->getID()]) - 1,
                (this->inputsTPParent->A_darts25[this->getID()]), this->getID());
            myTP->controlTPParent->TP390Ptr->nextCodeletscilkmerge_par[this->getID()]
                = (myTP->controlTPParent->nextCodeletscilksort_par[this->getID()]);
            myTP->controlTPParent->TP390Ptr->nextSyncCodeletscilkmerge_par[this->getID()]
                = (myTP->controlTPParent->nextSyncCodeletscilksort_par[this->getID()]);
#if USE_SPIN_CODELETS == 0
            myTP->controlTPParent->TP390Ptr->firstCodelet[this->getID()].decDep();
#else
            myTP->controlTPParent->TP390Ptr->availableCodelets[this->getID()] = 1;
#endif
        }
    }
}
TP25::TP25(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_mainNextCodelet,
    darts::Codelet* in_mainSyncCodelet, TP25** in_ptrToThisFunctionTP, ELM* in_low, ELM* in_tmp,
    long in_size)
    : ompTP(in_numThreads, in_mainCodeletID)
    , ptrToThisFunctionTP(in_ptrToThisFunctionTP)
    , inputsTPParent(this)
    , controlTPParent(this)
    , nextCodeletscilksort_par(new Codelet*[in_numThreads])
    , nextSyncCodeletscilksort_par(new Codelet*[in_numThreads])
    , low_darts25(new ELM*[this->numThreads])
    , tmp_darts25(new ELM*[this->numThreads])
    , size_darts25(new long[this->numThreads])
    , A_darts25(new ELM*[this->numThreads])
    , B_darts25(new ELM*[this->numThreads])
    , C_darts25(new ELM*[this->numThreads])
    , D_darts25(new ELM*[this->numThreads])
    , quarter_darts25(new long[this->numThreads])
    , tmpA_darts25(new ELM*[this->numThreads])
    , tmpB_darts25(new ELM*[this->numThreads])
    , tmpC_darts25(new ELM*[this->numThreads])
    , tmpD_darts25(new ELM*[this->numThreads])
    , TP390Ptr(nullptr)
    , TP390_alreadyLaunched(0)
    , task366Inputs(new _task366Inputs*[this->numThreads])
    , task368Inputs(new _task368Inputs*[this->numThreads])
    , task370Inputs(new _task370Inputs*[this->numThreads])
    , task372Inputs(new _task372Inputs*[this->numThreads])
    , task377Inputs(new _task377Inputs*[this->numThreads])
    , task383Inputs(new _task383Inputs*[this->numThreads])
    , checkInCodelets342(new _checkInCodelets342[this->numThreads])
    , checkInCodelets366(new _checkInCodelets366[this->numThreads])
    , checkInCodelets368(new _checkInCodelets368[this->numThreads])
    , checkInCodelets370(new _checkInCodelets370[this->numThreads])
    , checkInCodelets372(new _checkInCodelets372[this->numThreads])
    , barrierCodelets376(new _barrierCodelets376[1])
    , checkInCodelets377(new _checkInCodelets377[this->numThreads])
    , checkInCodelets383(new _checkInCodelets383[this->numThreads])
    , barrierCodelets389(new _barrierCodelets389[1])
    , checkInCodelets390(new _checkInCodelets390[this->numThreads])
{
    barrierCodelets389[0] = _barrierCodelets389(this->numThreads, this->numThreads, this, 0);
    barrierCodelets376[0] = _barrierCodelets376(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets390* checkInCodelets390Ptr = (this->checkInCodelets390);
    _checkInCodelets383* checkInCodelets383Ptr = (this->checkInCodelets383);
    _checkInCodelets377* checkInCodelets377Ptr = (this->checkInCodelets377);
    _checkInCodelets372* checkInCodelets372Ptr = (this->checkInCodelets372);
    _checkInCodelets370* checkInCodelets370Ptr = (this->checkInCodelets370);
    _checkInCodelets368* checkInCodelets368Ptr = (this->checkInCodelets368);
    _checkInCodelets366* checkInCodelets366Ptr = (this->checkInCodelets366);
    _checkInCodelets342* checkInCodelets342Ptr = (this->checkInCodelets342);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets342);
#endif
    for (size_t codeletCounter = 0; codeletCounter < this->numThreads; codeletCounter++) {
        (*checkInCodelets390Ptr) = _checkInCodelets390(1, 1, this, codeletCounter);
        checkInCodelets390Ptr++;
        (*checkInCodelets383Ptr) = _checkInCodelets383(1, 1, this, codeletCounter);
        checkInCodelets383Ptr++;
        (*checkInCodelets377Ptr) = _checkInCodelets377(1, 1, this, codeletCounter);
        checkInCodelets377Ptr++;
        (*checkInCodelets372Ptr) = _checkInCodelets372(1, 1, this, codeletCounter);
        checkInCodelets372Ptr++;
        (*checkInCodelets370Ptr) = _checkInCodelets370(1, 1, this, codeletCounter);
        checkInCodelets370Ptr++;
        (*checkInCodelets368Ptr) = _checkInCodelets368(1, 1, this, codeletCounter);
        checkInCodelets368Ptr++;
        (*checkInCodelets366Ptr) = _checkInCodelets366(1, 1, this, codeletCounter);
        checkInCodelets366Ptr++;
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets342Ptr) = _checkInCodelets342(2, 1, this, codeletCounter);
#else
        (*checkInCodelets342Ptr) = _checkInCodelets342(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets342Ptr).decDep();
        checkInCodelets342Ptr++;
    }
    if (this->numThreads == 1) {
        this->nextCodeletscilksort_par[0] = in_mainNextCodelet;
        this->nextSyncCodeletscilksort_par[0] = in_mainSyncCodelet;
        this->low_darts25[0] = in_low;
        this->tmp_darts25[0] = in_tmp;
        this->size_darts25[0] = in_size;
        this->availableCodelets[0] = 1;
    } else {
        this->low_darts25[this->mainCodeletID] = in_low;
        this->tmp_darts25[this->mainCodeletID] = in_tmp;
        this->size_darts25[this->mainCodeletID] = in_size;
        this->nextCodeletscilksort_par[in_mainCodeletID] = in_mainNextCodelet;
        this->nextSyncCodeletscilksort_par[in_mainCodeletID] = in_mainSyncCodelet;
#if USE_SPIN_CODELETS == 0
        this->firstCodelet[this->mainCodeletID].decDep();
#else
        this->availableCodelets[this->mainCodeletID] = 1;
#endif
        *(this->ptrToThisFunctionTP) = this;
    }
}
TP25::~TP25()
{
    delete[] task366Inputs;
    delete[] task368Inputs;
    delete[] task370Inputs;
    delete[] task372Inputs;
    delete[] task377Inputs;
    delete[] task383Inputs;
    delete[] checkInCodelets390;
    delete[] barrierCodelets389;
    delete[] checkInCodelets383;
    delete[] checkInCodelets377;
    delete[] barrierCodelets376;
    delete[] checkInCodelets372;
    delete[] checkInCodelets370;
    delete[] checkInCodelets368;
    delete[] checkInCodelets366;
    delete[] checkInCodelets342;
    delete[] nextCodeletscilksort_par;
    delete[] nextSyncCodeletscilksort_par;
    delete[] low_darts25;
    delete[] tmp_darts25;
    delete[] size_darts25;
    delete[] A_darts25;
    delete[] B_darts25;
    delete[] C_darts25;
    delete[] D_darts25;
    delete[] quarter_darts25;
    delete[] tmpA_darts25;
    delete[] tmpB_darts25;
    delete[] tmpC_darts25;
    delete[] tmpD_darts25;
}
void TP25::setNewInputs(ELM* in_low, ELM* in_tmp, long in_size, size_t codeletID)
{
    this->low_darts25[codeletID] = in_low;
    this->tmp_darts25[codeletID] = in_tmp;
    this->size_darts25[codeletID] = in_size;
}
/*TP366: OMPTaskDirective*/
void TP366::_checkInCodelets367::fire(void)
{
/*Init the vars for this region*/

/*printing node 367: CallExpr*/
/*Finding the next codelet after the function call*/
#if 0
/*Signaling next codelet region: 367 nextRegion: 524 */
myTP->controlTPParent->checkInCodelets524.decDep();
#endif
/*Finding the next sync point before making the function call*/
#if 0
/*Find the sync point for a function call*/
/*syncNode: OMPTaskwaitDirective 376*/
/*No syncpoint found or outside task's boundaries, looking for syncCodelet*/
#endif
    /*Make the function call*/
    invoke<TP_cilksort_par>(myTP, myTP->numThreads, this->getID(),
        &(myTP->controlTPParent->checkInCodelets524),
        (myTP->controlTPParent->task366Inputs->nextSyncCodelet), nullptr,
        (this->taskInputs->A_darts366), (this->taskInputs->tmpA_darts366),
        (this->taskInputs->quarter_darts366));
}
void TP366::_checkInCodelets524::fire(void)
{

    /*printing node 524: NullStmt*/
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task366Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task366Inputs->nextSyncCodelet->decDep();
}
TP366::TP366(
    int in_numThreads, int in_mainCodeletID, TP25* in_TPParent, _task366Inputs* in_task366Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task366Inputs(in_task366Inputs)
    , TP367Ptr(nullptr)
    , TP367_alreadyLaunched(0)
    , checkInCodelets367(1, 1, this, this->mainCodeletID)
    , checkInCodelets524(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets367.decDep();
}
TP366::~TP366() { delete (task366Inputs); }
/*TP368: OMPTaskDirective*/
void TP368::_checkInCodelets369::fire(void)
{
/*Init the vars for this region*/

/*printing node 369: CallExpr*/
/*Finding the next codelet after the function call*/
#if 0
/*Signaling next codelet region: 369 nextRegion: 525 */
myTP->controlTPParent->checkInCodelets525.decDep();
#endif
/*Finding the next sync point before making the function call*/
#if 0
/*Find the sync point for a function call*/
/*syncNode: OMPTaskwaitDirective 376*/
/*No syncpoint found or outside task's boundaries, looking for syncCodelet*/
#endif
    /*Make the function call*/
    invoke<TP_cilksort_par>(myTP, myTP->numThreads, this->getID(),
        &(myTP->controlTPParent->checkInCodelets525),
        (myTP->controlTPParent->task368Inputs->nextSyncCodelet), nullptr,
        (this->taskInputs->B_darts368), (this->taskInputs->tmpB_darts368),
        (this->taskInputs->quarter_darts368));
}
void TP368::_checkInCodelets525::fire(void)
{

    /*printing node 525: NullStmt*/
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task368Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task368Inputs->nextSyncCodelet->decDep();
}
TP368::TP368(
    int in_numThreads, int in_mainCodeletID, TP25* in_TPParent, _task368Inputs* in_task368Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task368Inputs(in_task368Inputs)
    , TP369Ptr(nullptr)
    , TP369_alreadyLaunched(0)
    , checkInCodelets369(1, 1, this, this->mainCodeletID)
    , checkInCodelets525(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets369.decDep();
}
TP368::~TP368() { delete (task368Inputs); }
/*TP370: OMPTaskDirective*/
void TP370::_checkInCodelets371::fire(void)
{
/*Init the vars for this region*/

/*printing node 371: CallExpr*/
/*Finding the next codelet after the function call*/
#if 0
/*Signaling next codelet region: 371 nextRegion: 526 */
myTP->controlTPParent->checkInCodelets526.decDep();
#endif
/*Finding the next sync point before making the function call*/
#if 0
/*Find the sync point for a function call*/
/*syncNode: OMPTaskwaitDirective 376*/
/*No syncpoint found or outside task's boundaries, looking for syncCodelet*/
#endif
    /*Make the function call*/
    invoke<TP_cilksort_par>(myTP, myTP->numThreads, this->getID(),
        &(myTP->controlTPParent->checkInCodelets526),
        (myTP->controlTPParent->task370Inputs->nextSyncCodelet), nullptr,
        (this->taskInputs->C_darts370), (this->taskInputs->tmpC_darts370),
        (this->taskInputs->quarter_darts370));
}
void TP370::_checkInCodelets526::fire(void)
{

    /*printing node 526: NullStmt*/
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task370Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task370Inputs->nextSyncCodelet->decDep();
}
TP370::TP370(
    int in_numThreads, int in_mainCodeletID, TP25* in_TPParent, _task370Inputs* in_task370Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task370Inputs(in_task370Inputs)
    , TP371Ptr(nullptr)
    , TP371_alreadyLaunched(0)
    , checkInCodelets371(1, 1, this, this->mainCodeletID)
    , checkInCodelets526(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets371.decDep();
}
TP370::~TP370() { delete (task370Inputs); }
/*TP372: OMPTaskDirective*/
void TP372::_checkInCodelets373::fire(void)
{
/*Init the vars for this region*/

/*printing node 373: CallExpr*/
/*Finding the next codelet after the function call*/
#if 0
/*Signaling next codelet region: 373 nextRegion: 527 */
myTP->controlTPParent->checkInCodelets527.decDep();
#endif
/*Finding the next sync point before making the function call*/
#if 0
/*Find the sync point for a function call*/
/*syncNode: OMPTaskwaitDirective 376*/
/*No syncpoint found or outside task's boundaries, looking for syncCodelet*/
#endif
    /*Make the function call*/
    invoke<TP_cilksort_par>(myTP, myTP->numThreads, this->getID(),
        &(myTP->controlTPParent->checkInCodelets527),
        (myTP->controlTPParent->task372Inputs->nextSyncCodelet), nullptr,
        (this->taskInputs->D_darts372), (this->taskInputs->tmpD_darts372),
        (this->taskInputs->size_darts372) - 3 * (this->taskInputs->quarter_darts372));
}
void TP372::_checkInCodelets527::fire(void)
{

    /*printing node 527: NullStmt*/
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task372Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task372Inputs->nextSyncCodelet->decDep();
}
TP372::TP372(
    int in_numThreads, int in_mainCodeletID, TP25* in_TPParent, _task372Inputs* in_task372Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task372Inputs(in_task372Inputs)
    , TP373Ptr(nullptr)
    , TP373_alreadyLaunched(0)
    , checkInCodelets373(1, 1, this, this->mainCodeletID)
    , checkInCodelets527(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets373.decDep();
}
TP372::~TP372() { delete (task372Inputs); }
/*TP377: OMPTaskDirective*/
void TP377::_checkInCodelets378::fire(void)
{
/*Init the vars for this region*/

/*printing node 378: CallExpr*/
/*Finding the next codelet after the function call*/
#if 0
/*Signaling next codelet region: 378 nextRegion: 528 */
myTP->controlTPParent->checkInCodelets528.decDep();
#endif
/*Finding the next sync point before making the function call*/
#if 0
/*Find the sync point for a function call*/
/*syncNode: OMPTaskwaitDirective 389*/
/*No syncpoint found or outside task's boundaries, looking for syncCodelet*/
#endif
    /*Make the function call*/
    invoke<TP_cilkmerge_par>(myTP, myTP->numThreads, this->getID(),
        &(myTP->controlTPParent->checkInCodelets528),
        (myTP->controlTPParent->task377Inputs->nextSyncCodelet), nullptr,
        (this->taskInputs->A_darts377),
        (this->taskInputs->A_darts377) + (this->taskInputs->quarter_darts377) - 1,
        (this->taskInputs->B_darts377),
        (this->taskInputs->B_darts377) + (this->taskInputs->quarter_darts377) - 1,
        (this->taskInputs->tmpA_darts377));
}
void TP377::_checkInCodelets528::fire(void)
{

    /*printing node 528: NullStmt*/
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task377Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task377Inputs->nextSyncCodelet->decDep();
}
TP377::TP377(
    int in_numThreads, int in_mainCodeletID, TP25* in_TPParent, _task377Inputs* in_task377Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task377Inputs(in_task377Inputs)
    , TP378Ptr(nullptr)
    , TP378_alreadyLaunched(0)
    , checkInCodelets378(1, 1, this, this->mainCodeletID)
    , checkInCodelets528(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets378.decDep();
}
TP377::~TP377() { delete (task377Inputs); }
/*TP383: OMPTaskDirective*/
void TP383::_checkInCodelets384::fire(void)
{
/*Init the vars for this region*/

/*printing node 384: CallExpr*/
/*Finding the next codelet after the function call*/
#if 0
/*Signaling next codelet region: 384 nextRegion: 529 */
myTP->controlTPParent->checkInCodelets529.decDep();
#endif
/*Finding the next sync point before making the function call*/
#if 0
/*Find the sync point for a function call*/
/*syncNode: OMPTaskwaitDirective 389*/
/*No syncpoint found or outside task's boundaries, looking for syncCodelet*/
#endif
    /*Make the function call*/
    invoke<TP_cilkmerge_par>(myTP, myTP->numThreads, this->getID(),
        &(myTP->controlTPParent->checkInCodelets529),
        (myTP->controlTPParent->task383Inputs->nextSyncCodelet), nullptr,
        (this->taskInputs->C_darts383),
        (this->taskInputs->C_darts383) + (this->taskInputs->quarter_darts383) - 1,
        (this->taskInputs->D_darts383),
        (this->taskInputs->low_darts383) + (this->taskInputs->size_darts383) - 1,
        (this->taskInputs->tmpC_darts383));
}
void TP383::_checkInCodelets529::fire(void)
{

    /*printing node 529: NullStmt*/
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task383Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task383Inputs->nextSyncCodelet->decDep();
}
TP383::TP383(
    int in_numThreads, int in_mainCodeletID, TP25* in_TPParent, _task383Inputs* in_task383Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task383Inputs(in_task383Inputs)
    , TP384Ptr(nullptr)
    , TP384_alreadyLaunched(0)
    , checkInCodelets384(1, 1, this, this->mainCodeletID)
    , checkInCodelets529(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets384.decDep();
}
TP383::~TP383() { delete (task383Inputs); }
/*TP501: OMPParallelDirective*/
void TP501::_barrierCodelets501::fire(void)
{
    TP501* myTP = static_cast<TP501*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
void TP501::_checkInCodelets502::fire(void)
{
    /*Select the thread executing OMPSingleDirective 502*/
    if (!__sync_val_compare_and_swap(&(myTP->TP502_alreadyLaunched), 0, 1)) {
        invoke<TP502>(myTP, 1, this->getID(), myTP);
    } else {
        myTP->TPParent->barrierCodelets501[0].decDep();
    }
}
TP501::TP501(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , TP502Ptr(nullptr)
    , TP502_alreadyLaunched(0)
    , task503Inputs(new _task503Inputs*[this->numThreads])
    , barrierCodelets501(new _barrierCodelets501[1])
    , checkInCodelets502(new _checkInCodelets502[this->numThreads])
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets501[0] = _barrierCodelets501(ompNumThreads, ompNumThreads, this, 0);
    _checkInCodelets502* checkInCodelets502Ptr = (this->checkInCodelets502);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets502Ptr) = _checkInCodelets502(1, 1, this, codeletCounter);
        (*checkInCodelets502Ptr).decDep();
        checkInCodelets502Ptr++;
    }
}
TP501::~TP501()
{
    delete[] task503Inputs;
    delete[] barrierCodelets501;
    delete[] checkInCodelets502;
}
/*TP502: OMPSingleDirective*/
void TP502::_checkInCodelets503::fire(void)
{

    /*printing node 503: OMPTaskDirective*/
    /*syncNode: OMPParallelDirective 501*/
    /*Find the TP to which the nextRegion belongs*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->TPParent->barrierCodelets501[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 503*/
    _task503Inputs* task503Inputs = new _task503Inputs(nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task503Inputs[0] = task503Inputs;
    invoke<TP503>(myTP, 1, this->getID(), myTP, task503Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->TPParent->barrierCodelets501[0].decDep();
}
TP502::TP502(int in_numThreads, int in_mainCodeletID, TP501* in_TPParent)
    : ompOMPSingleDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task503Inputs(new _task503Inputs*[this->numThreads])
    , checkInCodelets503(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets503.decDep();
}
TP502::~TP502() { delete[] task503Inputs; }
/*TP503: OMPTaskDirective*/
void TP503::_checkInCodelets504::fire(void)
{
/*Init the vars for this region*/

/*printing node 504: CallExpr*/
/*Finding the next codelet after the function call*/
#if 0
/*Signaling next codelet region: 504 nextRegion: 523 */
myTP->controlTPParent->checkInCodelets523.decDep();
#endif
/*Finding the next sync point before making the function call*/
#if 0
/*Find the sync point for a function call*/
/*syncNode: OMPParallelDirective 501*/
/*No syncpoint found or outside task's boundaries, looking for syncCodelet*/
#endif
    /*Make the function call*/
    invoke<TP_cilksort_par>(myTP, myTP->numThreads, this->getID(),
        &(myTP->controlTPParent->checkInCodelets523),
        (myTP->controlTPParent->task503Inputs->nextSyncCodelet), nullptr, sort_array, tmp,
        bots_arg_size);
}
void TP503::_checkInCodelets523::fire(void)
{

    /*printing node 523: NullStmt*/
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task503Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task503Inputs->nextSyncCodelet->decDep();
}
TP503::TP503(
    int in_numThreads, int in_mainCodeletID, TP502* in_TPParent, _task503Inputs* in_task503Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task503Inputs(in_task503Inputs)
    , TP504Ptr(nullptr)
    , TP504_alreadyLaunched(0)
    , checkInCodelets504(1, 1, this, this->mainCodeletID)
    , checkInCodelets523(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets504.decDep();
}
TP503::~TP503() { delete (task503Inputs); }
