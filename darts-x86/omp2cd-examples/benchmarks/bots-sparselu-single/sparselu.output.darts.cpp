#include "sparselu.output.darts.h"
using namespace darts;
using namespace std;
/*Function: checkmat, ID: 15*/
int checkmat(float* M, float* N)
{
    /*checkmat:15*/
    /*CompoundStmt:57*/
    int i, j;
    float r_err;
    for (i = 0; i < bots_arg_size_1; i++) {
        for (j = 0; j < bots_arg_size_1; j++) {
            r_err = M[i * bots_arg_size_1 + j] - N[i * bots_arg_size_1 + j];
            if (r_err == 0.)
                continue;
            if (r_err < 0.)
                r_err = -r_err;
            if (M[i * bots_arg_size_1 + j] == 0) {
                {
                    if (bots_verbose_mode >= BOTS_VERBOSE_DEFAULT) {
                        fprintf(stdout, "Checking failure: A[%d][%d]=%f  B[%d][%d]=%f; \n", i, j,
                            M[i * bots_arg_size_1 + j], i, j, N[i * bots_arg_size_1 + j]);
                    }
                };
                return 0;
            }
            r_err = r_err / M[i * bots_arg_size_1 + j];
            if (r_err > 9.9999999999999995E-7) {
                {
                    if (bots_verbose_mode >= BOTS_VERBOSE_DEFAULT) {
                        fprintf(stdout,
                            "Checking failure: A[%d][%d]=%f  B[%d][%d]=%f; Relative Error=%f\n", i,
                            j, M[i * bots_arg_size_1 + j], i, j, N[i * bots_arg_size_1 + j], r_err);
                    }
                };
                return 0;
            }
        }
    }
    return 1;
}
/*Function: genmat, ID: 16*/
void genmat(float* M[])
{
    /*genmat:16*/
    /*CompoundStmt:123*/
    int null_entry, init_val, i, j, ii, jj;
    float* p;
    init_val = 1325;
    for (ii = 0; ii < bots_arg_size; ii++) {
        for (jj = 0; jj < bots_arg_size; jj++) {
            null_entry = 0;
            if ((ii < jj) && (ii % 3 != 0))
                null_entry = 1;
            if ((ii > jj) && (jj % 3 != 0))
                null_entry = 1;
            if (ii % 2 == 1)
                null_entry = 1;
            if (jj % 2 == 1)
                null_entry = 1;
            if (ii == jj)
                null_entry = 0;
            if (ii == jj - 1)
                null_entry = 0;
            if (ii - 1 == jj)
                null_entry = 0;
            if (null_entry == 0) {
                M[ii * bots_arg_size + jj]
                    = (float*)malloc(bots_arg_size_1 * bots_arg_size_1 * sizeof(float));
                if (M[ii * bots_arg_size + jj] == (float*)((void*)0)) {
                    {
                        if (bots_verbose_mode >= BOTS_VERBOSE_DEFAULT) {
                            fprintf(stdout, "Error: Out of memory\n");
                        }
                    };
                    exit(101);
                }
                p = M[ii * bots_arg_size + jj];
                for (i = 0; i < bots_arg_size_1; i++) {
                    for (j = 0; j < bots_arg_size_1; j++) {
                        init_val = (3125 * init_val) % 65536;
                        (*p) = (float)((init_val - 32768.) / 16384.);
                        p++;
                    }
                }
            } else {
                M[ii * bots_arg_size + jj] = (float*)((void*)0);
            }
        }
    }
}
/*Function: print_structure, ID: 17*/
void print_structure(char* name, float* M[])
{
    /*print_structure:17*/
    /*CompoundStmt:219*/
    int ii, jj;
    {
        /*CompoundStmt:221*/
        if (bots_verbose_mode >= BOTS_VERBOSE_DEFAULT) {
            fprintf(stdout, "Structure for matrix %s @ 0x%p\n", name, M);
        }
    }
    for (ii = 0; ii < bots_arg_size; ii++) {
        for (jj = 0; jj < bots_arg_size; jj++) {
            if (M[ii * bots_arg_size + jj] != ((void*)0)) {
                {
                    if (bots_verbose_mode >= BOTS_VERBOSE_DEFAULT) {
                        fprintf(stdout, "x");
                    }
                };
            } else {
                if (bots_verbose_mode >= BOTS_VERBOSE_DEFAULT) {
                    fprintf(stdout, " ");
                }
            };
        }
        {
            if (bots_verbose_mode >= BOTS_VERBOSE_DEFAULT) {
                fprintf(stdout, "\n");
            }
        };
    }
    {
        /*CompoundStmt:254*/
        if (bots_verbose_mode >= BOTS_VERBOSE_DEFAULT) {
            fprintf(stdout, "\n");
        }
    }
}
/*Function: allocate_clean_block, ID: 18*/
float* allocate_clean_block()
{
    /*allocate_clean_block:18*/
    /*CompoundStmt:259*/
    int i, j;
    float *p, *q;
    p = (float*)malloc(bots_arg_size_1 * bots_arg_size_1 * sizeof(float));
    q = p;
    if (p != ((void*)0)) {
        for (i = 0; i < bots_arg_size_1; i++)
            for (j = 0; j < bots_arg_size_1; j++) {
                (*p) = 0.;
                p++;
            }
    } else {
        {
            if (bots_verbose_mode >= BOTS_VERBOSE_DEFAULT) {
                fprintf(stdout, "Error: Out of memory\n");
            }
        };
        exit(101);
    }
    return (q);
}
/*Function: lu0, ID: 19*/
void lu0(float* diag)
{
    /*lu0:19*/
    /*CompoundStmt:293*/
    int i, j, k;
    for (k = 0; k < bots_arg_size_1; k++)
        for (i = k + 1; i < bots_arg_size_1; i++) {
            diag[i * bots_arg_size_1 + k]
                = diag[i * bots_arg_size_1 + k] / diag[k * bots_arg_size_1 + k];
            for (j = k + 1; j < bots_arg_size_1; j++)
                diag[i * bots_arg_size_1 + j] = diag[i * bots_arg_size_1 + j]
                    - diag[i * bots_arg_size_1 + k] * diag[k * bots_arg_size_1 + j];
        }
}
/*Function: bdiv, ID: 20*/
void bdiv(float* diag, float* row)
{
    /*bdiv:20*/
    /*CompoundStmt:336*/
    int i, j, k;
    for (i = 0; i < bots_arg_size_1; i++)
        for (k = 0; k < bots_arg_size_1; k++) {
            row[i * bots_arg_size_1 + k]
                = row[i * bots_arg_size_1 + k] / diag[k * bots_arg_size_1 + k];
            for (j = k + 1; j < bots_arg_size_1; j++)
                row[i * bots_arg_size_1 + j] = row[i * bots_arg_size_1 + j]
                    - row[i * bots_arg_size_1 + k] * diag[k * bots_arg_size_1 + j];
        }
}
/*Function: bmod, ID: 21*/
void bmod(float* row, float* col, float* inner)
{
    /*bmod:21*/
    /*CompoundStmt:378*/
    int i, j, k;
    for (i = 0; i < bots_arg_size_1; i++)
        for (j = 0; j < bots_arg_size_1; j++)
            for (k = 0; k < bots_arg_size_1; k++)
                inner[i * bots_arg_size_1 + j] = inner[i * bots_arg_size_1 + j]
                    - row[i * bots_arg_size_1 + k] * col[k * bots_arg_size_1 + j];
}
/*Function: fwd, ID: 22*/
void fwd(float* diag, float* col)
{
    /*fwd:22*/
    /*CompoundStmt:407*/
    int i, j, k;
    for (j = 0; j < bots_arg_size_1; j++)
        for (k = 0; k < bots_arg_size_1; k++)
            for (i = k + 1; i < bots_arg_size_1; i++)
                col[i * bots_arg_size_1 + j] = col[i * bots_arg_size_1 + j]
                    - diag[i * bots_arg_size_1 + k] * col[k * bots_arg_size_1 + j];
}
/*Function: sparselu_init, ID: 23*/
void sparselu_init(float*** pBENCH, char* pass)
{
    /*sparselu_init:23*/
    /*CompoundStmt:437*/
    *pBENCH = (float**)malloc(bots_arg_size * bots_arg_size * sizeof(float*));
    genmat(*pBENCH);
}
/*Function: sparselu_par_call, ID: 24*/
void sparselu_par_call(float** BENCH)
{
    /*sparselu_par_call:24*/
    /*CompoundStmt:447*/
    int ii, jj, kk;
    {
        /*CompoundStmt:449*/
        if (bots_verbose_mode >= BOTS_VERBOSE_DEFAULT) {
            fprintf(stdout, "Computing SparseLU Factorization (%dx%d matrix with %dx%d blocks) ",
                bots_arg_size, bots_arg_size, bots_arg_size_1, bots_arg_size_1);
        }
    }
    if (affinMaskRes) {
        myDARTSRuntime->run(
            launch<TP454>(ompNumThreads * DARTS_CODELETS_MULT, 0, RuntimeFinalCodelet,
                (float***)&((BENCH)), (int*)&((ii)), (int*)&((jj)), (int*)&((kk))));
    }
    {
        /*CompoundStmt:538*/
        if (bots_verbose_mode >= BOTS_VERBOSE_DEFAULT) {
            fprintf(stdout, " completed!\n");
        }
    }
}
/*Function: sparselu_seq_call, ID: 25*/
void sparselu_seq_call(float** BENCH)
{
    /*sparselu_seq_call:25*/
    /*CompoundStmt:543*/
    int ii, jj, kk;
    for (kk = 0; kk < bots_arg_size; kk++) {
        lu0(BENCH[kk * bots_arg_size + kk]);
        for (jj = kk + 1; jj < bots_arg_size; jj++)
            if (BENCH[kk * bots_arg_size + jj] != ((void*)0)) {
                fwd(BENCH[kk * bots_arg_size + kk], BENCH[kk * bots_arg_size + jj]);
            }
        for (ii = kk + 1; ii < bots_arg_size; ii++)
            if (BENCH[ii * bots_arg_size + kk] != ((void*)0)) {
                bdiv(BENCH[kk * bots_arg_size + kk], BENCH[ii * bots_arg_size + kk]);
            }
        for (ii = kk + 1; ii < bots_arg_size; ii++)
            if (BENCH[ii * bots_arg_size + kk] != ((void*)0))
                for (jj = kk + 1; jj < bots_arg_size; jj++)
                    if (BENCH[kk * bots_arg_size + jj] != ((void*)0)) {
                        if (BENCH[ii * bots_arg_size + jj] == ((void*)0))
                            BENCH[ii * bots_arg_size + jj] = allocate_clean_block();
                        bmod(BENCH[ii * bots_arg_size + kk], BENCH[kk * bots_arg_size + jj],
                            BENCH[ii * bots_arg_size + jj]);
                    }
    }
}
/*Function: sparselu_fini, ID: 26*/
void sparselu_fini(float** BENCH, char* pass)
{
    /*sparselu_fini:26*/
    /*CompoundStmt:618*/
    return;
}
/*Function: sparselu_check, ID: 27*/
int sparselu_check(float** SEQ, float** BENCH)
{
    /*sparselu_check:27*/
    /*CompoundStmt:620*/
    int ii, jj, ok = 1;
    for (ii = 0; ((ii < bots_arg_size) && ok); ii++) {
        for (jj = 0; ((jj < bots_arg_size) && ok); jj++) {
            if ((SEQ[ii * bots_arg_size + jj] == ((void*)0))
                && (BENCH[ii * bots_arg_size + jj] != ((void*)0)))
                ok = 0;
            if ((SEQ[ii * bots_arg_size + jj] != ((void*)0))
                && (BENCH[ii * bots_arg_size + jj] == ((void*)0)))
                ok = 0;
            if ((SEQ[ii * bots_arg_size + jj] != ((void*)0))
                && (BENCH[ii * bots_arg_size + jj] != ((void*)0)))
                ok = checkmat(SEQ[ii * bots_arg_size + jj], BENCH[ii * bots_arg_size + jj]);
        }
    }
    if (ok)
        return 1;
    else
        return 2;
}
/*TP454: OMPParallelDirective*/
void TP454::_barrierCodelets454::fire(void)
{
    TP454* myTP = static_cast<TP454*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
void TP454::_checkInCodelets455::fire(void)
{
    /*Select the thread executing OMPSingleDirective 455*/
    if (!__sync_val_compare_and_swap(&(myTP->TP455_alreadyLaunched), 0, 1)) {
        invoke<TP455>(myTP, 1, this->getID(), myTP, &(*(this->inputsTPParent->BENCH_darts454)),
            &(*(this->inputsTPParent->ii_darts454)), &(*(this->inputsTPParent->jj_darts454)),
            &(*(this->inputsTPParent->kk_darts454)));
    } else {
        myTP->TPParent->barrierCodelets454[0].decDep();
    }
}
TP454::TP454(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet, float*** in_BENCH,
    int* in_ii, int* in_jj, int* in_kk)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , BENCH_darts454(in_BENCH) /*OMP_SHARED - INPUT*/
    , ii_darts454(in_ii) /*OMP_SHARED - INPUT*/
    , jj_darts454(in_jj) /*OMP_SHARED - INPUT*/
    , kk_darts454(in_kk) /*OMP_SHARED - INPUT*/
    , TP455Ptr(nullptr)
    , TP455_alreadyLaunched(0)
    , task456Inputs(new _task456Inputs*[this->numThreads])
    , task473Inputs(new _task473Inputs*[this->numThreads])
    , task490Inputs(new _task490Inputs*[this->numThreads])
    , task515Inputs(new _task515Inputs*[this->numThreads])
    , barrierCodelets454(new _barrierCodelets454[1])
    , checkInCodelets455(new _checkInCodelets455[this->numThreads])
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets454[0] = _barrierCodelets454(ompNumThreads, ompNumThreads, this, 0);
    _checkInCodelets455* checkInCodelets455Ptr = (this->checkInCodelets455);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets455Ptr) = _checkInCodelets455(1, 1, this, codeletCounter);
        (*checkInCodelets455Ptr).decDep();
        checkInCodelets455Ptr++;
    }
}
TP454::~TP454()
{
    delete[] task456Inputs;
    delete[] task473Inputs;
    delete[] task490Inputs;
    delete[] task515Inputs;
    delete[] barrierCodelets454;
    delete[] checkInCodelets455;
}
/*TP455: OMPSingleDirective*/
void TP455::_checkInCodelets456::fire(void)
{

    /*printing node 456: OMPTaskDirective*/
    /*syncNode: OMPParallelDirective 454*/
    /*Find the TP to which the nextRegion belongs*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->TPParent->barrierCodelets454[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 456*/
    _task456Inputs* task456Inputs = new _task456Inputs(&(*(this->inputsTPParent->BENCH_darts455)),
        &(*(this->inputsTPParent->ii_darts455)), &(*(this->inputsTPParent->jj_darts455)),
        &(*(this->inputsTPParent->kk_darts455)), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task456Inputs[0] = task456Inputs;
    invoke<TP456>(myTP, 1, this->getID(), myTP, task456Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->TPParent->barrierCodelets454[0].decDep();
}
TP455::TP455(int in_numThreads, int in_mainCodeletID, TP454* in_TPParent, float*** in_BENCH,
    int* in_ii, int* in_jj, int* in_kk)
    : ompOMPSingleDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , BENCH_darts455(in_BENCH) /*OMP_SHARED - INPUT*/
    , ii_darts455(in_ii) /*OMP_SHARED - INPUT*/
    , jj_darts455(in_jj) /*OMP_SHARED - INPUT*/
    , kk_darts455(in_kk) /*OMP_SHARED - INPUT*/
    , task456Inputs(new _task456Inputs*[this->numThreads])
    , task473Inputs(new _task473Inputs*[this->numThreads])
    , task490Inputs(new _task490Inputs*[this->numThreads])
    , task515Inputs(new _task515Inputs*[this->numThreads])
    , checkInCodelets456(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets456.decDep();
}
TP455::~TP455()
{
    delete[] task456Inputs;
    delete[] task473Inputs;
    delete[] task490Inputs;
    delete[] task515Inputs;
}
/*TP456: OMPTaskDirective*/
void TP456::_checkInCodelets458::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 458: BinaryOperator*/
    (this->taskInputs->kk_darts456) = 0;

    /*printing node 459: BinaryOperator*/
    /*Print the code for a condition node in a complex loop stmt */
    if ((this->taskInputs->kk_darts456) < bots_arg_size) {
        /*Signal the first codelet in the loop*/
        myTP->checkInCodelets457.decDep();
        return;
    }
    /*Signal the codelet after the loop from the end's condional node.*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task456Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task456Inputs->nextSyncCodelet->decDep();
}
void TP456::_checkInCodelets457::fire(void)
{

    /*printing node 457: ForStmt*/
    bool haveToLaunch = __sync_bool_compare_and_swap(&(myTP->controlTPParent->TP457_LoopCounter),
        myTP->controlTPParent->TP457_LoopCounterPerThread[this->getID()],
        myTP->controlTPParent->TP457_LoopCounterPerThread[this->getID()] + 1);
    unsigned int iterIdx = myTP->controlTPParent->TP457_LoopCounterPerThread[this->getID()];
    if (haveToLaunch) {
        this->resetCodelet();
        myTP->controlTPParent->TP457PtrVec.push_back(nullptr);
        myTP->controlTPParent->TP457_LoopCounterPerThread[this->getID()] += 1;
        invoke<TP457>(myTP, myTP->numThreads, this->getID(), myTP, myTP->inputsTPParent,
            &(myTP->controlTPParent->TP457PtrVec.back()));
    } else {
        if (myTP->controlTPParent->TP457PtrVec.size() == 0) {
            this->resetCodelet();
            this->decDep();
            return;
        } else if (myTP->controlTPParent->TP457PtrVec.size() < (iterIdx + 1)) {
            this->resetCodelet();
            this->decDep();
            return;
        } else if (myTP->controlTPParent->TP457PtrVec[iterIdx] == nullptr) {
            this->resetCodelet();
            this->decDep();
            return;
        } else {
            this->resetCodelet();
#if USE_SPIN_CODELETS == 0
            myTP->controlTPParent->TP457PtrVec[iterIdx]->firstCodelet[this->getID()].decDep();
#else
            myTP->controlTPParent->TP457PtrVec[iterIdx]->availableCodelets[this->getID()] = 1;
#endif
            myTP->controlTPParent->TP457_LoopCounterPerThread[this->getID()] += 1;
        }
    }
}
void TP456::_checkInCodelets460::fire(void)
{

    /*printing node 460: UnaryOperator*/
    (this->taskInputs->kk_darts456)++;

    /*printing node 665: BinaryOperator*/
    /*Print the code for a condition node in a complex loop stmt */
    if ((this->taskInputs->kk_darts456) < bots_arg_size) {
        this->resetCodelet();
        /*Signal the first codelet in the loop*/
        myTP->checkInCodelets457.decDep();
        return;
    }
    /*Signal the codelet after the loop from the condtional node.*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task456Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task456Inputs->nextSyncCodelet->decDep();
}
TP456::TP456(
    int in_numThreads, int in_mainCodeletID, TP455* in_TPParent, _task456Inputs* in_task456Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task456Inputs(in_task456Inputs)
    , TP457_LoopCounter(0)
    , TP457_LoopCounterPerThread(new unsigned int[this->numThreads])
    , task473Inputs(new _task473Inputs*[this->numThreads])
    , task490Inputs(new _task490Inputs*[this->numThreads])
    , task515Inputs(new _task515Inputs*[this->numThreads])
    , checkInCodelets458(1, 1, this, this->mainCodeletID)
    , checkInCodelets457(1, 1, this, this->mainCodeletID)
    , checkInCodelets460(1, 1, this, this->mainCodeletID)
{
    memset((void*)TP457_LoopCounterPerThread, 0, this->numThreads * sizeof(unsigned int));
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets458.decDep();
}
TP456::~TP456()
{
    delete[] TP457_LoopCounterPerThread;
    delete[] task473Inputs;
    delete[] task490Inputs;
    delete[] task515Inputs;
    delete (task456Inputs);
}
/*TP457: ForStmt*/
void TP457::_checkInCodelets462::fire(void)
{

    /*printing node 462: CallExpr*/
    lu0((*(this->taskInputs->BENCH_darts456))[(this->taskInputs->kk_darts456) * bots_arg_size
        + (this->taskInputs->kk_darts456)]);

    /*printing node 467: BinaryOperator*/
    (this->taskInputs->jj_darts456) = (this->taskInputs->kk_darts456) + 1;

    /*printing node 469: BinaryOperator*/
    /*Print the code for a condition node in a complex loop stmt */
    if ((this->taskInputs->jj_darts456) < bots_arg_size) {
        /*Signal the first codelet in the loop*/
        myTP->checkInCodelets466.decDep();
        return;
    } else {
        /*Signal the codelet after the loop from the end condional node.*/
        /*Signaling next codelet region: 470 nextRegion: 484 */
        myTP->controlTPParent->checkInCodelets484.decDep();
        return;
    }
}
void TP457::_checkInCodelets466::fire(void)
{

    /*printing node 466: ForStmt*/
    bool haveToLaunch = __sync_bool_compare_and_swap(&(myTP->controlTPParent->TP466_LoopCounter),
        myTP->controlTPParent->TP466_LoopCounterPerThread[this->getID()],
        myTP->controlTPParent->TP466_LoopCounterPerThread[this->getID()] + 1);
    unsigned int iterIdx = myTP->controlTPParent->TP466_LoopCounterPerThread[this->getID()];
    if (haveToLaunch) {
        this->resetCodelet();
        myTP->controlTPParent->TP466PtrVec.push_back(nullptr);
        myTP->controlTPParent->TP466_LoopCounterPerThread[this->getID()] += 1;
        invoke<TP466>(myTP, myTP->numThreads, this->getID(), myTP, myTP->inputsTPParent,
            &(myTP->controlTPParent->TP466PtrVec.back()));
    } else {
        if (myTP->controlTPParent->TP466PtrVec.size() == 0) {
            this->resetCodelet();
            this->decDep();
            return;
        } else if (myTP->controlTPParent->TP466PtrVec.size() < (iterIdx + 1)) {
            this->resetCodelet();
            this->decDep();
            return;
        } else if (myTP->controlTPParent->TP466PtrVec[iterIdx] == nullptr) {
            this->resetCodelet();
            this->decDep();
            return;
        } else {
            this->resetCodelet();
#if USE_SPIN_CODELETS == 0
            myTP->controlTPParent->TP466PtrVec[iterIdx]->firstCodelet[this->getID()].decDep();
#else
            myTP->controlTPParent->TP466PtrVec[iterIdx]->availableCodelets[this->getID()] = 1;
#endif
            myTP->controlTPParent->TP466_LoopCounterPerThread[this->getID()] += 1;
        }
    }
}
void TP457::_checkInCodelets470::fire(void)
{

    /*printing node 470: UnaryOperator*/
    (this->taskInputs->jj_darts456)++;

    /*printing node 661: BinaryOperator*/
    /*Print the code for a condition node in a complex loop stmt */
    if ((this->taskInputs->jj_darts456) < bots_arg_size) {
        this->resetCodelet();
        /*Signal the first codelet in the loop*/
        myTP->checkInCodelets466.decDep();
        return;
    } else {
        /*Signal the codelet after the loop from the condtional node.*/
        /*Signaling next codelet region: 470 nextRegion: 484 */
        myTP->controlTPParent->checkInCodelets484.decDep();
        return;
    }
}
void TP457::_checkInCodelets484::fire(void)
{

    /*printing node 484: BinaryOperator*/
    (this->taskInputs->ii_darts456) = (this->taskInputs->kk_darts456) + 1;

    /*printing node 486: BinaryOperator*/
    /*Print the code for a condition node in a complex loop stmt */
    if ((this->taskInputs->ii_darts456) < bots_arg_size) {
        /*Signal the first codelet in the loop*/
        myTP->checkInCodelets483.decDep();
        return;
    } else {
        /*Signal the codelet after the loop from the end condional node.*/
        /*Signaling next codelet region: 487 nextRegion: 500 */
        myTP->controlTPParent->barrierCodelets500[0].decDep();
        return;
    }
}
void TP457::_checkInCodelets483::fire(void)
{

    /*printing node 483: ForStmt*/
    bool haveToLaunch = __sync_bool_compare_and_swap(&(myTP->controlTPParent->TP483_LoopCounter),
        myTP->controlTPParent->TP483_LoopCounterPerThread[this->getID()],
        myTP->controlTPParent->TP483_LoopCounterPerThread[this->getID()] + 1);
    unsigned int iterIdx = myTP->controlTPParent->TP483_LoopCounterPerThread[this->getID()];
    if (haveToLaunch) {
        this->resetCodelet();
        myTP->controlTPParent->TP483PtrVec.push_back(nullptr);
        myTP->controlTPParent->TP483_LoopCounterPerThread[this->getID()] += 1;
        invoke<TP483>(myTP, myTP->numThreads, this->getID(), myTP, myTP->inputsTPParent,
            &(myTP->controlTPParent->TP483PtrVec.back()));
    } else {
        if (myTP->controlTPParent->TP483PtrVec.size() == 0) {
            this->resetCodelet();
            this->decDep();
            return;
        } else if (myTP->controlTPParent->TP483PtrVec.size() < (iterIdx + 1)) {
            this->resetCodelet();
            this->decDep();
            return;
        } else if (myTP->controlTPParent->TP483PtrVec[iterIdx] == nullptr) {
            this->resetCodelet();
            this->decDep();
            return;
        } else {
            this->resetCodelet();
#if USE_SPIN_CODELETS == 0
            myTP->controlTPParent->TP483PtrVec[iterIdx]->firstCodelet[this->getID()].decDep();
#else
            myTP->controlTPParent->TP483PtrVec[iterIdx]->availableCodelets[this->getID()] = 1;
#endif
            myTP->controlTPParent->TP483_LoopCounterPerThread[this->getID()] += 1;
        }
    }
}
void TP457::_checkInCodelets487::fire(void)
{

    /*printing node 487: UnaryOperator*/
    (this->taskInputs->ii_darts456)++;

    /*printing node 662: BinaryOperator*/
    /*Print the code for a condition node in a complex loop stmt */
    if ((this->taskInputs->ii_darts456) < bots_arg_size) {
        this->resetCodelet();
        /*Signal the first codelet in the loop*/
        myTP->checkInCodelets483.decDep();
        return;
    } else {
        /*Signal the codelet after the loop from the condtional node.*/
        /*Signaling next codelet region: 487 nextRegion: 500 */
        myTP->controlTPParent->barrierCodelets500[0].decDep();
        return;
    }
}
void TP457::_barrierCodelets500::fire(void)
{
    TP457* myTP = static_cast<TP457*>(myTP_);
    {
        myTP->checkInCodelets502.decDep();
    }
}
void TP457::_checkInCodelets502::fire(void)
{

    /*printing node 502: BinaryOperator*/
    (this->taskInputs->ii_darts456) = (this->taskInputs->kk_darts456) + 1;

    /*printing node 504: BinaryOperator*/
    /*Print the code for a condition node in a complex loop stmt */
    if ((this->taskInputs->ii_darts456) < bots_arg_size) {
        /*Signal the first codelet in the loop*/
        myTP->checkInCodelets501.decDep();
        return;
    } else {
        /*Signal the codelet after the loop from the end condional node.*/
        /*Signaling next codelet region: 505 nextRegion: 537 */
        myTP->controlTPParent->barrierCodelets537[0].decDep();
        return;
    }
}
void TP457::_checkInCodelets501::fire(void)
{

    /*printing node 501: ForStmt*/
    bool haveToLaunch = __sync_bool_compare_and_swap(&(myTP->controlTPParent->TP501_LoopCounter),
        myTP->controlTPParent->TP501_LoopCounterPerThread[this->getID()],
        myTP->controlTPParent->TP501_LoopCounterPerThread[this->getID()] + 1);
    unsigned int iterIdx = myTP->controlTPParent->TP501_LoopCounterPerThread[this->getID()];
    if (haveToLaunch) {
        this->resetCodelet();
        myTP->controlTPParent->TP501PtrVec.push_back(nullptr);
        myTP->controlTPParent->TP501_LoopCounterPerThread[this->getID()] += 1;
        invoke<TP501>(myTP, myTP->numThreads, this->getID(), myTP, myTP->inputsTPParent,
            &(myTP->controlTPParent->TP501PtrVec.back()));
    } else {
        if (myTP->controlTPParent->TP501PtrVec.size() == 0) {
            this->resetCodelet();
            this->decDep();
            return;
        } else if (myTP->controlTPParent->TP501PtrVec.size() < (iterIdx + 1)) {
            this->resetCodelet();
            this->decDep();
            return;
        } else if (myTP->controlTPParent->TP501PtrVec[iterIdx] == nullptr) {
            this->resetCodelet();
            this->decDep();
            return;
        } else {
            this->resetCodelet();
#if USE_SPIN_CODELETS == 0
            myTP->controlTPParent->TP501PtrVec[iterIdx]->firstCodelet[this->getID()].decDep();
#else
            myTP->controlTPParent->TP501PtrVec[iterIdx]->availableCodelets[this->getID()] = 1;
#endif
            myTP->controlTPParent->TP501_LoopCounterPerThread[this->getID()] += 1;
        }
    }
}
void TP457::_checkInCodelets505::fire(void)
{

    /*printing node 505: UnaryOperator*/
    (this->taskInputs->ii_darts456)++;

    /*printing node 664: BinaryOperator*/
    /*Print the code for a condition node in a complex loop stmt */
    if ((this->taskInputs->ii_darts456) < bots_arg_size) {
        this->resetCodelet();
        /*Signal the first codelet in the loop*/
        myTP->checkInCodelets501.decDep();
        return;
    } else {
        /*Signal the codelet after the loop from the condtional node.*/
        /*Signaling next codelet region: 505 nextRegion: 537 */
        myTP->controlTPParent->barrierCodelets537[0].decDep();
        return;
    }
}
void TP457::_barrierCodelets537::fire(void)
{
    TP457* myTP = static_cast<TP457*>(myTP_);
    for (size_t codeletsCounter = 0; codeletsCounter < myTP->numThreads; codeletsCounter++) {
        myTP->TPParent->checkInCodelets460.decDep();
    }
}
TP457::TP457(int in_numThreads, int in_mainCodeletID, TP456* in_TPParent, TP456* in_inputsTPParent,
    TP457** in_ptrToThisTP)
    : ompTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(in_inputsTPParent)
    , ptrToThisTP(in_ptrToThisTP)
    , TP466_LoopCounter(0)
    , TP466_LoopCounterPerThread(new unsigned int[this->numThreads])
    , TP483_LoopCounter(0)
    , TP483_LoopCounterPerThread(new unsigned int[this->numThreads])
    , TP501_LoopCounter(0)
    , TP501_LoopCounterPerThread(new unsigned int[this->numThreads])
    , checkInCodelets462(1, 1, this, this->mainCodeletID)
    , checkInCodelets466(1, 1, this, this->mainCodeletID)
    , checkInCodelets470(1, 1, this, this->mainCodeletID)
    , checkInCodelets484(1, 1, this, this->mainCodeletID)
    , checkInCodelets483(1, 1, this, this->mainCodeletID)
    , checkInCodelets487(1, 1, this, this->mainCodeletID)
    , barrierCodelets500(new _barrierCodelets500[1])
    , checkInCodelets502(1, 1, this, this->mainCodeletID)
    , checkInCodelets501(1, 1, this, this->mainCodeletID)
    , checkInCodelets505(1, 1, this, this->mainCodeletID)
    , barrierCodelets537(new _barrierCodelets537[1])
{
    memset((void*)TP466_LoopCounterPerThread, 0, this->numThreads * sizeof(unsigned int));
    memset((void*)TP483_LoopCounterPerThread, 0, this->numThreads * sizeof(unsigned int));
    memset((void*)TP501_LoopCounterPerThread, 0, this->numThreads * sizeof(unsigned int));
    /*Initialize Codelets*/
    barrierCodelets537[0] = _barrierCodelets537(this->numThreads, this->numThreads, this, 0);
    barrierCodelets500[0] = _barrierCodelets500(this->numThreads, this->numThreads, this, 0);
    checkInCodelets462.decDep();
    *(this->ptrToThisTP) = this;
#if USE_SPIN_CODELETS == 0
    this->firstCodelet[this->getID()].decDep();
#else
    this->availableCodelets[this->getID()] = 1;
#endif
}
TP457::~TP457()
{
    delete[] TP466_LoopCounterPerThread;
    delete[] TP483_LoopCounterPerThread;
    delete[] TP501_LoopCounterPerThread;
    delete[] barrierCodelets537;
    delete[] barrierCodelets500;
}
/*TP466: ForStmt*/
void TP466::_checkInCodelets471::fire(void)
{
    /*Printing conditional branch node 471: inlining: 0*/
    if ((*(this->taskInputs->BENCH_darts456))[(this->taskInputs->kk_darts456) * bots_arg_size
            + (this->taskInputs->jj_darts456)]
        != (float*)((void*)0)) {
        myTP->checkInCodelets473.decDep();
    } else {
        /*Signaling the region after the if stmt*/
        /*The node is the last one in a complex loop, so signal the inc node*/
        /*Find the TP to which the nextRegion belongs*/
        myTP->controlTPParent->TPParent->checkInCodelets470.decDep();
    }
}
void TP466::_checkInCodelets473::fire(void)
{

    /*printing node 473: OMPTaskDirective*/
    /*syncNode: OMPTaskwaitDirective 500*/
    /*Find the TP to which the nextRegion belongs*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->TPParent->barrierCodelets500[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 473*/
    _task473Inputs* task473Inputs
        = new _task473Inputs(&(*(this->inputsTPParent->task456Inputs[0].BENCH_darts456)),
            &((this->inputsTPParent->task456Inputs[0].jj_darts456)),
            &((this->inputsTPParent->task456Inputs[0].kk_darts456)), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task473Inputs[0] = task473Inputs;
    invoke<TP473>(myTP, 1, this->getID(), myTP, task473Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*The node is the last one in a complex loop, so signal the inc node*/
    /*Find the TP to which the nextRegion belongs*/
    myTP->controlTPParent->TPParent->checkInCodelets470.decDep();
}
TP466::TP466(int in_numThreads, int in_mainCodeletID, TP457* in_TPParent, TP456* in_inputsTPParent,
    TP466** in_ptrToThisTP)
    : ompTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(in_inputsTPParent)
    , ptrToThisTP(in_ptrToThisTP)
    , checkInCodelets471(1, 1, this, this->mainCodeletID)
    , checkInCodelets473(1, 1, this, this->mainCodeletID)
{
    /*Initialize Codelets*/
    checkInCodelets471.decDep();
    *(this->ptrToThisTP) = this;
#if USE_SPIN_CODELETS == 0
    this->firstCodelet[this->getID()].decDep();
#else
    this->availableCodelets[this->getID()] = 1;
#endif
}
TP466::~TP466() {}
/*TP473: OMPTaskDirective*/
void TP473::_checkInCodelets475::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 475: CallExpr*/
    fwd((*(this->taskInputs->BENCH_darts473))[(this->taskInputs->kk_darts473) * bots_arg_size
            + (this->taskInputs->kk_darts473)],
        (*(this->taskInputs->BENCH_darts473))[(this->taskInputs->kk_darts473) * bots_arg_size
            + (this->taskInputs->jj_darts473)]);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task473Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task473Inputs->nextSyncCodelet->decDep();
}
TP473::TP473(
    int in_numThreads, int in_mainCodeletID, TP466* in_TPParent, _task473Inputs* in_task473Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task473Inputs(in_task473Inputs)
    , checkInCodelets475(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets475.decDep();
}
TP473::~TP473() { delete (task473Inputs); }
/*TP483: ForStmt*/
void TP483::_checkInCodelets488::fire(void)
{
    /*Printing conditional branch node 488: inlining: 0*/
    if ((*(this->taskInputs->BENCH_darts456))[(this->taskInputs->ii_darts456) * bots_arg_size
            + (this->taskInputs->kk_darts456)]
        != (float*)((void*)0)) {
        myTP->checkInCodelets490.decDep();
    } else {
        /*Signaling the region after the if stmt*/
        /*The node is the last one in a complex loop, so signal the inc node*/
        /*Find the TP to which the nextRegion belongs*/
        myTP->controlTPParent->TPParent->checkInCodelets487.decDep();
    }
}
void TP483::_checkInCodelets490::fire(void)
{

    /*printing node 490: OMPTaskDirective*/
    /*syncNode: OMPTaskwaitDirective 500*/
    /*Find the TP to which the nextRegion belongs*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->TPParent->barrierCodelets500[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 490*/
    _task490Inputs* task490Inputs
        = new _task490Inputs(&(*(this->inputsTPParent->task456Inputs[0].BENCH_darts456)),
            &((this->inputsTPParent->task456Inputs[0].ii_darts456)),
            &((this->inputsTPParent->task456Inputs[0].kk_darts456)), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task490Inputs[0] = task490Inputs;
    invoke<TP490>(myTP, 1, this->getID(), myTP, task490Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*The node is the last one in a complex loop, so signal the inc node*/
    /*Find the TP to which the nextRegion belongs*/
    myTP->controlTPParent->TPParent->checkInCodelets487.decDep();
}
TP483::TP483(int in_numThreads, int in_mainCodeletID, TP457* in_TPParent, TP456* in_inputsTPParent,
    TP483** in_ptrToThisTP)
    : ompTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(in_inputsTPParent)
    , ptrToThisTP(in_ptrToThisTP)
    , checkInCodelets488(1, 1, this, this->mainCodeletID)
    , checkInCodelets490(1, 1, this, this->mainCodeletID)
{
    /*Initialize Codelets*/
    checkInCodelets488.decDep();
    *(this->ptrToThisTP) = this;
#if USE_SPIN_CODELETS == 0
    this->firstCodelet[this->getID()].decDep();
#else
    this->availableCodelets[this->getID()] = 1;
#endif
}
TP483::~TP483() {}
/*TP490: OMPTaskDirective*/
void TP490::_checkInCodelets492::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 492: CallExpr*/
    bdiv((*(this->taskInputs->BENCH_darts490))[(this->taskInputs->kk_darts490) * bots_arg_size
             + (this->taskInputs->kk_darts490)],
        (*(this->taskInputs->BENCH_darts490))[(this->taskInputs->ii_darts490) * bots_arg_size
            + (this->taskInputs->kk_darts490)]);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task490Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task490Inputs->nextSyncCodelet->decDep();
}
TP490::TP490(
    int in_numThreads, int in_mainCodeletID, TP483* in_TPParent, _task490Inputs* in_task490Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task490Inputs(in_task490Inputs)
    , checkInCodelets492(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets492.decDep();
}
TP490::~TP490() { delete (task490Inputs); }
/*TP501: ForStmt*/
void TP501::_checkInCodelets506::fire(void)
{
    /*Printing conditional branch node 506: inlining: 0*/
    if ((*(this->taskInputs->BENCH_darts456))[(this->taskInputs->ii_darts456) * bots_arg_size
            + (this->taskInputs->kk_darts456)]
        != (float*)((void*)0)) {
        myTP->checkInCodelets509.decDep();
    } else {
        /*Signaling the region after the if stmt*/
        /*The node is the last one in a complex loop, so signal the inc node*/
        /*Find the TP to which the nextRegion belongs*/
        myTP->controlTPParent->TPParent->checkInCodelets505.decDep();
    }
}
void TP501::_checkInCodelets509::fire(void)
{

    /*printing node 509: BinaryOperator*/
    (this->taskInputs->jj_darts456) = (this->taskInputs->kk_darts456) + 1;

    /*printing node 511: BinaryOperator*/
    /*Print the code for a condition node in a complex loop stmt */
    if ((this->taskInputs->jj_darts456) < bots_arg_size) {
        /*Signal the first codelet in the loop*/
        myTP->checkInCodelets508.decDep();
        return;
    }
    /*Signal the codelet after the loop from the end's condional node.*/
    /*The node is the last one in a complex loop, so signal the inc node*/
    /*Find the TP to which the nextRegion belongs*/
    myTP->controlTPParent->TPParent->checkInCodelets505.decDep();
}
void TP501::_checkInCodelets508::fire(void)
{

    /*printing node 508: ForStmt*/
    bool haveToLaunch = __sync_bool_compare_and_swap(&(myTP->controlTPParent->TP508_LoopCounter),
        myTP->controlTPParent->TP508_LoopCounterPerThread[this->getID()],
        myTP->controlTPParent->TP508_LoopCounterPerThread[this->getID()] + 1);
    unsigned int iterIdx = myTP->controlTPParent->TP508_LoopCounterPerThread[this->getID()];
    if (haveToLaunch) {
        this->resetCodelet();
        myTP->controlTPParent->TP508PtrVec.push_back(nullptr);
        myTP->controlTPParent->TP508_LoopCounterPerThread[this->getID()] += 1;
        invoke<TP508>(myTP, myTP->numThreads, this->getID(), myTP, myTP->inputsTPParent,
            &(myTP->controlTPParent->TP508PtrVec.back()));
    } else {
        if (myTP->controlTPParent->TP508PtrVec.size() == 0) {
            this->resetCodelet();
            this->decDep();
            return;
        } else if (myTP->controlTPParent->TP508PtrVec.size() < (iterIdx + 1)) {
            this->resetCodelet();
            this->decDep();
            return;
        } else if (myTP->controlTPParent->TP508PtrVec[iterIdx] == nullptr) {
            this->resetCodelet();
            this->decDep();
            return;
        } else {
            this->resetCodelet();
#if USE_SPIN_CODELETS == 0
            myTP->controlTPParent->TP508PtrVec[iterIdx]->firstCodelet[this->getID()].decDep();
#else
            myTP->controlTPParent->TP508PtrVec[iterIdx]->availableCodelets[this->getID()] = 1;
#endif
            myTP->controlTPParent->TP508_LoopCounterPerThread[this->getID()] += 1;
        }
    }
}
void TP501::_checkInCodelets512::fire(void)
{

    /*printing node 512: UnaryOperator*/
    (this->taskInputs->jj_darts456)++;

    /*printing node 663: BinaryOperator*/
    /*Print the code for a condition node in a complex loop stmt */
    if ((this->taskInputs->jj_darts456) < bots_arg_size) {
        this->resetCodelet();
        /*Signal the first codelet in the loop*/
        myTP->checkInCodelets508.decDep();
        return;
    }
    /*Signal the codelet after the loop from the condtional node.*/
    /*The node is the last one in a complex loop, so signal the inc node*/
    /*Find the TP to which the nextRegion belongs*/
    myTP->controlTPParent->TPParent->checkInCodelets505.decDep();
}
TP501::TP501(int in_numThreads, int in_mainCodeletID, TP457* in_TPParent, TP456* in_inputsTPParent,
    TP501** in_ptrToThisTP)
    : ompTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(in_inputsTPParent)
    , ptrToThisTP(in_ptrToThisTP)
    , TP508_LoopCounter(0)
    , TP508_LoopCounterPerThread(new unsigned int[this->numThreads])
    , checkInCodelets506(1, 1, this, this->mainCodeletID)
    , checkInCodelets509(1, 1, this, this->mainCodeletID)
    , checkInCodelets508(1, 1, this, this->mainCodeletID)
    , checkInCodelets512(1, 1, this, this->mainCodeletID)
{
    memset((void*)TP508_LoopCounterPerThread, 0, this->numThreads * sizeof(unsigned int));
    /*Initialize Codelets*/
    checkInCodelets506.decDep();
    *(this->ptrToThisTP) = this;
#if USE_SPIN_CODELETS == 0
    this->firstCodelet[this->getID()].decDep();
#else
    this->availableCodelets[this->getID()] = 1;
#endif
}
TP501::~TP501() { delete[] TP508_LoopCounterPerThread; }
/*TP508: ForStmt*/
void TP508::_checkInCodelets513::fire(void)
{
    /*Printing conditional branch node 513: inlining: 0*/
    if ((*(this->taskInputs->BENCH_darts456))[(this->taskInputs->kk_darts456) * bots_arg_size
            + (this->taskInputs->jj_darts456)]
        != (float*)((void*)0)) {
        myTP->checkInCodelets515.decDep();
    } else {
        /*Signaling the region after the if stmt*/
        /*The node is the last one in a complex loop, so signal the inc node*/
        /*Find the TP to which the nextRegion belongs*/
        myTP->controlTPParent->TPParent->checkInCodelets512.decDep();
    }
}
void TP508::_checkInCodelets515::fire(void)
{

    /*printing node 515: OMPTaskDirective*/
    /*syncNode: OMPTaskwaitDirective 537*/
    /*Find the TP to which the nextRegion belongs*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->TPParent->TPParent->barrierCodelets537[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 515*/
    _task515Inputs* task515Inputs
        = new _task515Inputs(&(*(this->inputsTPParent->task456Inputs[0].BENCH_darts456)),
            &((this->inputsTPParent->task456Inputs[0].ii_darts456)),
            &((this->inputsTPParent->task456Inputs[0].jj_darts456)),
            &((this->inputsTPParent->task456Inputs[0].kk_darts456)), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task515Inputs[0] = task515Inputs;
    invoke<TP515>(myTP, 1, this->getID(), myTP, task515Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*The node is the last one in a complex loop, so signal the inc node*/
    /*Find the TP to which the nextRegion belongs*/
    myTP->controlTPParent->TPParent->checkInCodelets512.decDep();
}
TP508::TP508(int in_numThreads, int in_mainCodeletID, TP501* in_TPParent, TP456* in_inputsTPParent,
    TP508** in_ptrToThisTP)
    : ompTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(in_inputsTPParent)
    , ptrToThisTP(in_ptrToThisTP)
    , checkInCodelets513(1, 1, this, this->mainCodeletID)
    , checkInCodelets515(1, 1, this, this->mainCodeletID)
{
    /*Initialize Codelets*/
    checkInCodelets513.decDep();
    *(this->ptrToThisTP) = this;
#if USE_SPIN_CODELETS == 0
    this->firstCodelet[this->getID()].decDep();
#else
    this->availableCodelets[this->getID()] = 1;
#endif
}
TP508::~TP508() {}
/*TP515: OMPTaskDirective*/
void TP515::_checkInCodelets517::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 517: IfStmt*/
    if ((*(this->taskInputs->BENCH_darts515))[(this->taskInputs->ii_darts515) * bots_arg_size
            + (this->taskInputs->jj_darts515)]
        == (float*)((void*)0)) {
        (*(this->taskInputs->BENCH_darts515))[(this->taskInputs->ii_darts515) * bots_arg_size
            + (this->taskInputs->jj_darts515)]
            = allocate_clean_block();
    }

    /*printing node 525: CallExpr*/
    bmod((*(this->taskInputs->BENCH_darts515))[(this->taskInputs->ii_darts515) * bots_arg_size
             + (this->taskInputs->kk_darts515)],
        (*(this->taskInputs->BENCH_darts515))[(this->taskInputs->kk_darts515) * bots_arg_size
            + (this->taskInputs->jj_darts515)],
        (*(this->taskInputs->BENCH_darts515))[(this->taskInputs->ii_darts515) * bots_arg_size
            + (this->taskInputs->jj_darts515)]);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task515Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task515Inputs->nextSyncCodelet->decDep();
}
TP515::TP515(
    int in_numThreads, int in_mainCodeletID, TP508* in_TPParent, _task515Inputs* in_task515Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task515Inputs(in_task515Inputs)
    , checkInCodelets517(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets517.decDep();
}
TP515::~TP515() { delete (task515Inputs); }
