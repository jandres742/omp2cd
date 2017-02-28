#include "is.output.darts.h"
using namespace darts;
using namespace std;
INT_TYPE A_test_index_array[5] = { 2112377, 662041, 5336171, 3642833, 4250760 };
INT_TYPE A_test_rank_array[5] = { 104, 17523, 123928, 8288932, 8388264 };
INT_TYPE B_test_index_array[5] = { 41869, 812306, 5102857, 18232239, 26860214 };
INT_TYPE B_test_rank_array[5] = { 33422937, 10244, 59149, 33135281, 99 };
char CLASS;
INT_TYPE C_test_index_array[5] = { 44172927, 72999161, 74326391, 129606274, 21736814 };
INT_TYPE C_test_rank_array[5] = { 61147, 882988, 266290, 133997595, 133525895 };
INT_TYPE D_test_index_array[5] = { 1317351170, 995930646, 1157283250, 1503301535, 1453734525 };
INT_TYPE D_test_rank_array[5] = { 1, 36538729, 1978098519, 2145192618, 2147425337 };
unsigned int MAX_KEY;
unsigned int MAX_KEY_LOG_2;
unsigned int NUM_BUCKETS;
unsigned int NUM_BUCKETS_LOG_2;
unsigned int NUM_KEYS;
unsigned int SIZE_OF_BUFFERS;
INT_TYPE S_test_index_array[5] = { 48427, 17148, 23627, 62548, 4431 };
INT_TYPE S_test_rank_array[5] = { 0, 18, 346, 64917, 65463 };
unsigned int TOTAL_KEYS;
unsigned int TOTAL_KEYS_LOG_2;
INT_TYPE W_test_index_array[5] = { 357773, 934767, 875723, 898999, 404505 };
INT_TYPE W_test_rank_array[5] = { 1249, 11698, 1039987, 1043896, 1048018 };
INT_TYPE** bucket_size;
INT_TYPE* key_array;
INT_TYPE* key_buff1;
INT_TYPE** key_buff1_aptr = ((void*)0);
INT_TYPE* key_buff2;
INT_TYPE* key_buff_ptr_global;
INT_TYPE partial_verify_vals[5];
int passed_verification;
INT_TYPE test_index_array[5];
INT_TYPE test_rank_array[5];
INT_TYPE bucket_ptrs_dartsThreadPriv0[MAXNUMTHREADS][1024];
double R46_dartsThreadPriv0[MAXNUMTHREADS];
int KS_dartsThreadPriv0[MAXNUMTHREADS] = { 0 };
double R23_dartsThreadPriv0[MAXNUMTHREADS];
double T23_dartsThreadPriv0[MAXNUMTHREADS];
double T46_dartsThreadPriv0[MAXNUMTHREADS];
/*Function: randlc, ID: 11*/
double randlc(double* X, double* A)
{
    /*randlc:11*/
    /*CompoundStmt:53*/
    double T1, T2, T3, T4;
    double A1;
    double A2;
    double X1;
    double X2;
    double Z;
    int i, j;
    if (((KS_dartsThreadPriv0[0])) == 0) {
        ((R23_dartsThreadPriv0[0])) = 1.;
        ((R46_dartsThreadPriv0[0])) = 1.;
        ((T23_dartsThreadPriv0[0])) = 1.;
        ((T46_dartsThreadPriv0[0])) = 1.;
        for (i = 1; i <= 23; i++) {
            ((R23_dartsThreadPriv0[0])) = 0.5 * ((R23_dartsThreadPriv0[0]));
            ((T23_dartsThreadPriv0[0])) = 2. * ((T23_dartsThreadPriv0[0]));
        }
        for (i = 1; i <= 46; i++) {
            ((R46_dartsThreadPriv0[0])) = 0.5 * ((R46_dartsThreadPriv0[0]));
            ((T46_dartsThreadPriv0[0])) = 2. * ((T46_dartsThreadPriv0[0]));
        }
        ((KS_dartsThreadPriv0[0])) = 1;
    }
    T1 = ((R23_dartsThreadPriv0[0])) * *A;
    j = T1;
    A1 = j;
    A2 = *A - ((T23_dartsThreadPriv0[0])) * A1;
    T1 = ((R23_dartsThreadPriv0[0])) * *X;
    j = T1;
    X1 = j;
    X2 = *X - ((T23_dartsThreadPriv0[0])) * X1;
    T1 = A1 * X2 + A2 * X1;
    j = ((R23_dartsThreadPriv0[0])) * T1;
    T2 = j;
    Z = T1 - ((T23_dartsThreadPriv0[0])) * T2;
    T3 = ((T23_dartsThreadPriv0[0])) * Z + A2 * X2;
    j = ((R46_dartsThreadPriv0[0])) * T3;
    T4 = j;
    *X = T3 - ((T46_dartsThreadPriv0[0])) * T4;
    return (((R46_dartsThreadPriv0[0])) * *X);
}
/*Function: find_my_seed, ID: 12*/
double find_my_seed(int kn, int np, long nn, double s, double a)
{
    /*find_my_seed:12*/
    /*CompoundStmt:137*/
    double t1, t2;
    long mq, nq, kk, ik;
    if (kn == 0)
        return s;
    mq = (nn / 4 + np - 1) / np;
    nq = mq * 4 * kn;
    t1 = s;
    t2 = a;
    kk = nq;
    while (kk > 1) {
        ik = kk / 2;
        if (2 * ik == kk) {
            (void)randlc(&t2, &t2);
            kk = ik;
        } else {
            (void)randlc(&t1, &t2);
            kk = kk - 1;
        }
    }
    (void)randlc(&t1, &t2);
    return (t1);
}
/*Function: create_seq, ID: 13*/
void create_seq(double seed, double a)
{
    /*create_seq:13*/
    /*CompoundStmt:180*/
    double x, s;
    INT_TYPE i, k;
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP183>(ompNumThreads * DARTS_CODELETS_MULT, 0,
            RuntimeFinalCodelet, (double*)&((a)), (double*)&((seed))));
    }
}
/*Function: alloc_mem, ID: 14*/
void* alloc_mem(size_t size)
{
    /*alloc_mem:14*/
    /*CompoundStmt:236*/
    void* p;
    p = (void*)malloc(size);
    if (!p) {
        perror("Memory allocation error");
        exit(1);
    }
    return p;
}
/*Function: alloc_key_buff, ID: 15*/
void alloc_key_buff()
{
    /*alloc_key_buff:15*/
    /*CompoundStmt:247*/
    INT_TYPE i;
    int num_procs;
    num_procs = omp_get_max_threads();
    bucket_size = (INT_TYPE**)alloc_mem(sizeof(INT_TYPE*) * num_procs);
    for (i = 0; i < num_procs; i++) {
        bucket_size[i] = (INT_TYPE*)alloc_mem(sizeof(INT_TYPE) * NUM_BUCKETS);
    }
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP268>(
            ompNumThreads * DARTS_CODELETS_MULT, 0, RuntimeFinalCodelet, 0, NUM_KEYS));
    }
}
/*Function: full_verify, ID: 16*/
void full_verify()
{
    /*full_verify:16*/
    /*CompoundStmt:300*/
    INT_TYPE i, j;
    INT_TYPE k, k1, k2;
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP303>(
            ompNumThreads * DARTS_CODELETS_MULT, 0, RuntimeFinalCodelet, 0, NUM_BUCKETS));
    }
    j = 0;
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP352>(ompNumThreads * DARTS_CODELETS_MULT, 0,
            RuntimeFinalCodelet, 1, NUM_KEYS, (INT_TYPE*)&((j))));
    }
    if (j != 0)
        printf("Full_verify: number of keys out of sort: %ld\n", (long)j);
    else
        passed_verification++;
}
/*Function: rank_is, ID: 17*/
void rank_is(int iteration)
{
    /*rank_is:17*/
    /*CompoundStmt:392*/
    INT_TYPE i, k;
    INT_TYPE *key_buff_ptr, *key_buff_ptr2;
    int shift = MAX_KEY_LOG_2 - NUM_BUCKETS_LOG_2;
    INT_TYPE num_bucket_keys = (1L << shift);
    key_array[iteration] = iteration;
    key_array[iteration + 10] = MAX_KEY - iteration;
    for (i = 0; i < 5; i++) {
        partial_verify_vals[i] = key_array[test_index_array[i]];
    }
    key_buff_ptr2 = key_buff2;
    key_buff_ptr = key_buff1;
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP416>(ompNumThreads * DARTS_CODELETS_MULT, 0,
            RuntimeFinalCodelet, (INT_TYPE**)&((key_buff_ptr)), (INT_TYPE**)&((key_buff_ptr2)),
            (INT_TYPE*)&((num_bucket_keys)), (int*)&((shift))));
    }
    for (i = 0; i < 5; i++) {
        k = partial_verify_vals[i];
        if (0 < k && k <= NUM_KEYS - 1) {
            INT_TYPE key_rank = key_buff_ptr[k - 1];
            int failed = 0;
            switch (CLASS) {
            case 'S':
                if (i <= 2) {
                    if (key_rank != test_rank_array[i] + iteration)
                        failed = 1;
                    else
                        passed_verification++;
                } else {
                    if (key_rank != test_rank_array[i] - iteration)
                        failed = 1;
                    else
                        passed_verification++;
                }
                break;
            case 'W':
                if (i < 2) {
                    if (key_rank != test_rank_array[i] + (iteration - 2))
                        failed = 1;
                    else
                        passed_verification++;
                } else {
                    if (key_rank != test_rank_array[i] - iteration)
                        failed = 1;
                    else
                        passed_verification++;
                }
                break;
            case 'A':
                if (i <= 2) {
                    if (key_rank != test_rank_array[i] + (iteration - 1))
                        failed = 1;
                    else
                        passed_verification++;
                } else {
                    if (key_rank != test_rank_array[i] - (iteration - 1))
                        failed = 1;
                    else
                        passed_verification++;
                }
                break;
            case 'B':
                if (i == 1 || i == 2 || i == 4) {
                    if (key_rank != test_rank_array[i] + iteration)
                        failed = 1;
                    else
                        passed_verification++;
                } else {
                    if (key_rank != test_rank_array[i] - iteration)
                        failed = 1;
                    else
                        passed_verification++;
                }
                break;
            case 'C':
                if (i <= 2) {
                    if (key_rank != test_rank_array[i] + iteration)
                        failed = 1;
                    else
                        passed_verification++;
                } else {
                    if (key_rank != test_rank_array[i] - iteration)
                        failed = 1;
                    else
                        passed_verification++;
                }
                break;
            case 'D':
                if (i < 2) {
                    if (key_rank != test_rank_array[i] + iteration)
                        failed = 1;
                    else
                        passed_verification++;
                } else {
                    if (key_rank != test_rank_array[i] - iteration)
                        failed = 1;
                    else
                        passed_verification++;
                }
                break;
            }
            if (failed == 1)
                printf(
                    "Failed partial verification: iteration %d, test key %d\n", iteration, (int)i);
        }
    }
    if (iteration == 10)
        key_buff_ptr_global = key_buff_ptr;
}
/*Function: main, ID: 18*/
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
    /*main:18*/
    /*CompoundStmt:758*/
    if (argc == 2) {
        CLASS = argv[1][0];
    } else {
        fprintf(stderr, "Usage: <program name> <scale>\n");
        fprintf(stderr, "Example: bin/is.exe S\n");
        exit(1);
    }
    if (CLASS == 'S') {
        TOTAL_KEYS_LOG_2 = 16;
        MAX_KEY_LOG_2 = 11;
        NUM_BUCKETS_LOG_2 = 9;
        TOTAL_KEYS = (1 << TOTAL_KEYS_LOG_2);
    } else if (CLASS == 'W') {
        TOTAL_KEYS_LOG_2 = 20;
        MAX_KEY_LOG_2 = 16;
        NUM_BUCKETS_LOG_2 = 10;
        TOTAL_KEYS = (1 << TOTAL_KEYS_LOG_2);
    } else if (CLASS == 'A') {
        TOTAL_KEYS_LOG_2 = 23;
        MAX_KEY_LOG_2 = 19;
        NUM_BUCKETS_LOG_2 = 10;
        TOTAL_KEYS = (1 << TOTAL_KEYS_LOG_2);
    } else if (CLASS == 'B') {
        TOTAL_KEYS_LOG_2 = 25;
        MAX_KEY_LOG_2 = 21;
        NUM_BUCKETS_LOG_2 = 10;
        TOTAL_KEYS = (1 << TOTAL_KEYS_LOG_2);
    } else if (CLASS == 'C') {
        TOTAL_KEYS_LOG_2 = 27;
        MAX_KEY_LOG_2 = 23;
        NUM_BUCKETS_LOG_2 = 10;
        TOTAL_KEYS = (1 << TOTAL_KEYS_LOG_2);
    } else if (CLASS == 'D') {
        TOTAL_KEYS_LOG_2 = 31;
        MAX_KEY_LOG_2 = 27;
        NUM_BUCKETS_LOG_2 = 10;
        TOTAL_KEYS = (1L << TOTAL_KEYS_LOG_2);
    }
    MAX_KEY = (1 << MAX_KEY_LOG_2);
    NUM_BUCKETS = (1 << NUM_BUCKETS_LOG_2);
    NUM_KEYS = TOTAL_KEYS;
    SIZE_OF_BUFFERS = NUM_KEYS;
    key_array = (INT_TYPE*)malloc(sizeof(INT_TYPE) * SIZE_OF_BUFFERS);
    key_buff1 = (INT_TYPE*)malloc(sizeof(INT_TYPE) * MAX_KEY);
    key_buff2 = (INT_TYPE*)malloc(sizeof(INT_TYPE) * SIZE_OF_BUFFERS);
    int i, iteration, timer_on;
    double timecounter;
    FILE* fp;
    timer_on = 0;
    if ((fp = fopen("timer.flag", "r")) != ((void*)0)) {
        fclose(fp);
        timer_on = 1;
    }
    timer_clear(0);
    if (timer_on) {
        timer_clear(1);
        timer_clear(2);
        timer_clear(3);
    }
    if (timer_on)
        timer_start(3);
    for (i = 0; i < 5; i++)
        switch (CLASS) {
        case 'S':
            test_index_array[i] = S_test_index_array[i];
            test_rank_array[i] = S_test_rank_array[i];
            break;
        case 'A':
            test_index_array[i] = A_test_index_array[i];
            test_rank_array[i] = A_test_rank_array[i];
            break;
        case 'W':
            test_index_array[i] = W_test_index_array[i];
            test_rank_array[i] = W_test_rank_array[i];
            break;
        case 'B':
            test_index_array[i] = B_test_index_array[i];
            test_rank_array[i] = B_test_rank_array[i];
            break;
        case 'C':
            test_index_array[i] = C_test_index_array[i];
            test_rank_array[i] = C_test_rank_array[i];
            break;
        case 'D':
            test_index_array[i] = D_test_index_array[i];
            test_rank_array[i] = D_test_rank_array[i];
            break;
        }
    printf("\n\n NAS Parallel Benchmarks (NPB3.3-OMP) - IS Benchmark\n\n");
    printf(" Size:  %ld  (class %c)\n", (long)TOTAL_KEYS, CLASS);
    printf(" Iterations:  %d\n", 10);
    printf(" Number of available threads:  %d\n", omp_get_max_threads());
    printf("\n");
    if (timer_on)
        timer_start(1);
    create_seq(314159265., 1220703125.);
    alloc_key_buff();
    if (timer_on)
        timer_stop(1);
    rank_is(1);
    passed_verification = 0;
    if (CLASS != 'S')
        printf("\n   iteration\n");
    timer_start(0);
    for (iteration = 1; iteration <= 10; iteration++) {
        if (CLASS != 'S')
            printf("        %d\n", iteration);
        rank_is(iteration);
    }
    timer_stop(0);
    timecounter = timer_read(0);
    if (timer_on)
        timer_start(2);
    full_verify();
    if (timer_on)
        timer_stop(2);
    if (timer_on)
        timer_stop(3);
    if (passed_verification != 5 * 10 + 1)
        passed_verification = 0;
    c_print_results("IS", CLASS, (int)(TOTAL_KEYS / 64), 64, 0, 10, timecounter,
        ((double)(10 * TOTAL_KEYS)) / timecounter / 1.0E+6, "keys ranked", passed_verification,
        "3.3.1", "24 Jul 2016", "gcc", "$(CC)", "-lm -L/home/jaime/phd/clang-llvm/openmp/run...",
        "-I/home/jaime/phd/clang-llvm/openmp/runtime...", "-O3 -fopenmp", "-O3 -fopenmp");
    if (timer_on) {
        double t_total, t_percent;
        t_total = timer_read(3);
        printf("\nAdditional timers -\n");
        printf(" Total execution: %8.3f\n", t_total);
        if (t_total == 0.)
            t_total = 1.;
        timecounter = timer_read(1);
        t_percent = timecounter / t_total * 100.;
        printf(" Initialization : %8.3f (%5.2f%%)\n", timecounter, t_percent);
        timecounter = timer_read(0);
        t_percent = timecounter / t_total * 100.;
        printf(" Benchmarking   : %8.3f (%5.2f%%)\n", timecounter, t_percent);
        timecounter = timer_read(2);
        t_percent = timecounter / t_total * 100.;
        printf(" Sorting        : %8.3f (%5.2f%%)\n", timecounter, t_percent);
    }
    free(key_array);
    free(key_buff1);
    free(key_buff2);
    return 0;
}
/*TP183: OMPParallelDirective*/
void TP183::_barrierCodelets183::fire(void)
{
    TP183* myTP = static_cast<TP183*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
void TP183::_checkInCodelets185::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 185: DeclStmt*/

    /*printing node 186: DeclStmt*/
    this->inputsTPParent->an_darts183[this->getID()] = (*(this->inputsTPParent->a_darts183));

    /*printing node 187: DeclStmt*/

    /*printing node 188: DeclStmt*/

    /*printing node 189: BinaryOperator*/
    (this->inputsTPParent->myid_darts183[this->getID()]) = omp_get_thread_num();

    /*printing node 191: BinaryOperator*/
    (this->inputsTPParent->num_procs_darts183[this->getID()]) = omp_get_num_threads();

    /*printing node 193: BinaryOperator*/
    (this->inputsTPParent->mq_darts183[this->getID()])
        = (NUM_KEYS + (this->inputsTPParent->num_procs_darts183[this->getID()]) - 1)
        / (this->inputsTPParent->num_procs_darts183[this->getID()]);

    /*printing node 197: BinaryOperator*/
    (this->inputsTPParent->k1_darts183[this->getID()])
        = (this->inputsTPParent->mq_darts183[this->getID()])
        * (this->inputsTPParent->myid_darts183[this->getID()]);

    /*printing node 199: BinaryOperator*/
    (this->inputsTPParent->k2_darts183[this->getID()])
        = (this->inputsTPParent->k1_darts183[this->getID()])
        + (this->inputsTPParent->mq_darts183[this->getID()]);

    /*printing node 201: IfStmt*/
    if ((this->inputsTPParent->k2_darts183[this->getID()]) > NUM_KEYS) {
        (this->inputsTPParent->k2_darts183[this->getID()]) = NUM_KEYS;
    }

    /*printing node 205: BinaryOperator*/
    ((KS_dartsThreadPriv0[this->getID()])) = 0;

    /*printing node 206: BinaryOperator*/
    (this->inputsTPParent->s_darts183[this->getID()])
        = find_my_seed((this->inputsTPParent->myid_darts183[this->getID()]),
            (this->inputsTPParent->num_procs_darts183[this->getID()]), (long)4 * NUM_KEYS,
            (*(this->inputsTPParent->seed_darts183)),
            (this->inputsTPParent->an_darts183[this->getID()]));

    /*printing node 210: BinaryOperator*/
    (this->inputsTPParent->k_darts183[this->getID()]) = MAX_KEY / 4;

    /*printing node 212: ForStmt*/
    {
        INT_TYPE* i = &(this->inputsTPParent->i_darts183[this->getLocalID()]);
        (void)i /*OMP_PRIVATE*/;
        INT_TYPE* k = &(this->inputsTPParent->k_darts183[this->getLocalID()]);
        (void)k /*OMP_PRIVATE*/;
        double* s = &(this->inputsTPParent->s_darts183[this->getLocalID()]);
        (void)s /*OMP_PRIVATE*/;
        double* x = &(this->inputsTPParent->x_darts183[this->getLocalID()]);
        (void)x /*OMP_PRIVATE*/;
        double* an = &(this->inputsTPParent->an_darts183[this->getLocalID()]);
        (void)an /*PRIVATE*/;
        INT_TYPE* k2 = &(this->inputsTPParent->k2_darts183[this->getLocalID()]);
        (void)k2 /*PRIVATE*/;
        /*Loop's init*/
        (this->inputsTPParent->i_darts183[this->getID()])
            = (this->inputsTPParent->k1_darts183[this->getID()]);
        INT_TYPE i_darts_counter_temp183 = (this->inputsTPParent->i_darts183[this->getID()]);
        for (; (i_darts_counter_temp183) < (*k2); (i_darts_counter_temp183)++) {
            (*x) = randlc(&(*s), &(*an));
            (*x) += randlc(&(*s), &(*an));
            (*x) += randlc(&(*s), &(*an));
            (*x) += randlc(&(*s), &(*an));
            key_array[(i_darts_counter_temp183)] = (*k) * (*x);
        }
        (this->inputsTPParent->i_darts183[this->getID()]) = i_darts_counter_temp183;
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets183[0].decDep();
}
TP183::TP183(
    int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet, double* in_a, double* in_seed)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , a_darts183(in_a) /*OMP_SHARED - INPUT*/
    , i_darts183(new INT_TYPE[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , k_darts183(new INT_TYPE[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , s_darts183(new double[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , seed_darts183(in_seed) /*OMP_SHARED - INPUT*/
    , x_darts183(new double[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , an_darts183(new double[this->numThreads]) /*VARIABLE*/
    , k1_darts183(new INT_TYPE[this->numThreads]) /*VARIABLE*/
    , k2_darts183(new INT_TYPE[this->numThreads]) /*VARIABLE*/
    , mq_darts183(new INT_TYPE[this->numThreads]) /*VARIABLE*/
    , myid_darts183(new int[this->numThreads]) /*VARIABLE*/
    , num_procs_darts183(new int[this->numThreads]) /*VARIABLE*/
    , barrierCodelets183(new _barrierCodelets183[1])
    , checkInCodelets185(new _checkInCodelets185[this->numThreads])
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets183[0] = _barrierCodelets183(ompNumThreads, ompNumThreads, this, 0);
    _checkInCodelets185* checkInCodelets185Ptr = (this->checkInCodelets185);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets185Ptr) = _checkInCodelets185(1, 1, this, codeletCounter);
        (*checkInCodelets185Ptr).decDep();
        checkInCodelets185Ptr++;
    }
}
TP183::~TP183()
{
    delete[] an_darts183;
    delete[] k1_darts183;
    delete[] k2_darts183;
    delete[] mq_darts183;
    delete[] myid_darts183;
    delete[] num_procs_darts183;
    delete[] barrierCodelets183;
    delete[] checkInCodelets185;
}
/*TP268: OMPParallelForDirective*/
void TP268::_barrierCodelets268::fire(void)
{
    TP268* myTP = static_cast<TP268*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
bool TP268::requestNewRangeIterations268(INT_TYPE* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Static */
    /*Chunk = 0*/
    bool isThereNewIteration = false;
    {
        /*Static Scheduling*/
        INT_TYPE tempStartRange = rangePerCodelet268 * codeletID;
        INT_TYPE tempEndRange = rangePerCodelet268 * (codeletID + 1);
        if (remainderRange268 != 0) {
            if (codeletID < (uint32_t)remainderRange268) {
                tempStartRange += codeletID;
                tempEndRange += (codeletID + 1);
            } else {
                tempStartRange += remainderRange268;
                tempEndRange += remainderRange268;
            }
        }
        tempStartRange = tempStartRange * 1 + minIteration268;
        tempEndRange = tempEndRange * 1 + minIteration268;
        if (tempStartRange != tempEndRange) {
            isThereNewIteration = true;
        }
        if (initIteration268 < lastIteration268) {
            (this->inputsTPParent->i_darts268[codeletID]) = min(tempStartRange, tempEndRange);
            *endRange = max(tempStartRange, tempEndRange);
        } else {
            (this->inputsTPParent->i_darts268[codeletID]) = max(tempStartRange, tempEndRange);
            *endRange = min(tempStartRange, tempEndRange);
        }
        if (codeletID == this->numThreads - 1) {
            *endRange = lastIteration268;
        }
    }
    return isThereNewIteration;
}
void TP268::_checkInCodelets269::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 269: ForStmt*/
    /*var: i*/
    INT_TYPE* i = &(this->inputsTPParent->i_darts268[this->getLocalID()]);
    (void)i /*OMP_PRIVATE*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations268(
        (INT_TYPE*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        /*Find and signal the next codelet*/
        myTP->controlTPParent->TPParent->barrierCodelets268[0].decDep();
        return;
    }
    for (INT_TYPE i_darts_counter_temp268 = (*i); i_darts_counter_temp268 < endRange
         && i_darts_counter_temp268 < this->inputsTPParent->lastIteration268;
         i_darts_counter_temp268++) {
        {
            key_buff2[(i_darts_counter_temp268)] = 0;
        }
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets268[0].decDep();
}
TP268::TP268(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet,
    INT_TYPE in_initIteration, INT_TYPE in_lastIteration)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , i_darts268(new INT_TYPE[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , initIteration268(in_initIteration)
    , lastIteration268(in_lastIteration)
    , barrierCodelets268(new _barrierCodelets268[1])
    , checkInCodelets269(new _checkInCodelets269[this->numThreads])
{
    /*Initialize the loop parameters*/
    range268 = abs(lastIteration268 - initIteration268) / 1;
    rangePerCodelet268 = range268 / numThreads;
    minIteration268 = min<INT_TYPE>(lastIteration268, initIteration268);
    remainderRange268 = range268 % numThreads;
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets268[0] = _barrierCodelets268(ompNumThreads, ompNumThreads, this, 0);
    _checkInCodelets269* checkInCodelets269Ptr = (this->checkInCodelets269);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets269Ptr) = _checkInCodelets269(1, 1, this, codeletCounter);
        (*checkInCodelets269Ptr).decDep();
        checkInCodelets269Ptr++;
    }
}
TP268::~TP268()
{
    delete[] barrierCodelets268;
    delete[] checkInCodelets269;
}
/*TP303: OMPParallelForDirective*/
void TP303::_barrierCodelets303::fire(void)
{
    TP303* myTP = static_cast<TP303*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
bool TP303::requestNewRangeIterations303(INT_TYPE* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Dynamic */
    /*Chunk = 1*/
    bool isThereNewIteration = false;
    {
        /*Dynamic Scheduling*/
        if (initIteration303 < lastIteration303) {
            (this->inputsTPParent->j_darts303[codeletID])
                = __sync_fetch_and_add(&(nextIteration303), 1 * 1);
            *endRange = (this->inputsTPParent->j_darts303[codeletID]) + 1 * 1;
        } else {
            (this->inputsTPParent->j_darts303[codeletID])
                = __sync_fetch_and_sub(&(nextIteration303), 1 * 1);
            *endRange = (this->inputsTPParent->j_darts303[codeletID]) - 1 * 1;
        }
        if ((this->inputsTPParent->j_darts303[codeletID]) < lastIteration303) {
            isThereNewIteration = true;
        }
    }
    return isThereNewIteration;
}
void TP303::_checkInCodelets304::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 304: ForStmt*/
    /*var: i*/
    /*var: j*/
    /*var: k*/
    /*var: k1*/
    INT_TYPE* i = &(this->inputsTPParent->i_darts303[this->getLocalID()]);
    (void)i /*OMP_PRIVATE*/;
    INT_TYPE* j = &(this->inputsTPParent->j_darts303[this->getLocalID()]);
    (void)j /*OMP_PRIVATE*/;
    INT_TYPE* k = &(this->inputsTPParent->k_darts303[this->getLocalID()]);
    (void)k /*OMP_PRIVATE*/;
    INT_TYPE* k1 = &(this->inputsTPParent->k1_darts303[this->getLocalID()]);
    (void)k1 /*OMP_PRIVATE*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations303(
        (INT_TYPE*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        /*Find and signal the next codelet*/
        myTP->controlTPParent->TPParent->barrierCodelets303[0].decDep();
        return;
    }
    while (isThereNewIteration) {
        for (INT_TYPE j_darts_counter_temp303 = (*j); j_darts_counter_temp303 < endRange
             && j_darts_counter_temp303 < this->inputsTPParent->lastIteration303;
             j_darts_counter_temp303++) {
            {
                (*k1) = ((j_darts_counter_temp303) > 0)
                    ? ((bucket_ptrs_dartsThreadPriv0[this->getID()]))[(j_darts_counter_temp303)-1]
                    : 0;
                {
                    /*Loop's init*/
                    (*i) = (*k1);
                    INT_TYPE i_darts_counter_temp303 = (*i);
                    for (;
                         i_darts_counter_temp303 < ((bucket_ptrs_dartsThreadPriv0[this->getID()]))[(
                                                       j_darts_counter_temp303)];
                         i_darts_counter_temp303++) {
                        (*k) = --key_buff_ptr_global[key_buff2[i_darts_counter_temp303]];
                        key_array[(*k)] = key_buff2[i_darts_counter_temp303];
                    }
                    (*i) = i_darts_counter_temp303;
                }
            }
        }
        isThereNewIteration = this->inputsTPParent->requestNewRangeIterations303(
            &(this->endRange), this->getLocalID());
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets303[0].decDep();
}
TP303::TP303(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet,
    INT_TYPE in_initIteration, INT_TYPE in_lastIteration)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , i_darts303(new INT_TYPE[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , j_darts303(new INT_TYPE[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , k_darts303(new INT_TYPE[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , k1_darts303(new INT_TYPE[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , initIteration303(in_initIteration)
    , lastIteration303(in_lastIteration)
    , nextIteration303(INT_MAX)
    , loop303alreadySetUp(0)
    , barrierCodelets303(new _barrierCodelets303[1])
    , checkInCodelets304(new _checkInCodelets304[this->numThreads])
{
    /*Initialize the loop parameters*/
    nextIteration303 = initIteration303;
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets303[0] = _barrierCodelets303(ompNumThreads, ompNumThreads, this, 0);
    _checkInCodelets304* checkInCodelets304Ptr = (this->checkInCodelets304);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets304Ptr) = _checkInCodelets304(1, 1, this, codeletCounter);
        (*checkInCodelets304Ptr).decDep();
        checkInCodelets304Ptr++;
    }
}
TP303::~TP303()
{
    delete[] barrierCodelets303;
    delete[] checkInCodelets304;
}
/*TP352: OMPParallelForDirective*/
void TP352::_barrierCodelets352::fire(void)
{
    TP352* myTP = static_cast<TP352*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
bool TP352::requestNewRangeIterations352(INT_TYPE* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Static */
    /*Chunk = 0*/
    bool isThereNewIteration = false;
    {
        /*Static Scheduling*/
        INT_TYPE tempStartRange = rangePerCodelet352 * codeletID;
        INT_TYPE tempEndRange = rangePerCodelet352 * (codeletID + 1);
        if (remainderRange352 != 0) {
            if (codeletID < (uint32_t)remainderRange352) {
                tempStartRange += codeletID;
                tempEndRange += (codeletID + 1);
            } else {
                tempStartRange += remainderRange352;
                tempEndRange += remainderRange352;
            }
        }
        tempStartRange = tempStartRange * 1 + minIteration352;
        tempEndRange = tempEndRange * 1 + minIteration352;
        if (tempStartRange != tempEndRange) {
            isThereNewIteration = true;
        }
        if (initIteration352 < lastIteration352) {
            (this->inputsTPParent->i_darts352[codeletID]) = min(tempStartRange, tempEndRange);
            *endRange = max(tempStartRange, tempEndRange);
        } else {
            (this->inputsTPParent->i_darts352[codeletID]) = max(tempStartRange, tempEndRange);
            *endRange = min(tempStartRange, tempEndRange);
        }
        if (codeletID == this->numThreads - 1) {
            *endRange = lastIteration352;
        }
    }
    return isThereNewIteration;
}
void TP352::_checkInCodelets353::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 353: ForStmt*/
    /*var: i*/
    /*var: j*/
    /*Declare local reduction vars*/
    INT_TYPE j_LocalReductionVar = (INT_TYPE)0;
    INT_TYPE* i = &(this->inputsTPParent->i_darts352[this->getLocalID()]);
    (void)i /*OMP_PRIVATE*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations352(
        (INT_TYPE*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        goto updateReduceVars;
    }
    for (INT_TYPE i_darts_counter_temp352 = (*i); i_darts_counter_temp352 < endRange
         && i_darts_counter_temp352 < this->inputsTPParent->lastIteration352;
         i_darts_counter_temp352++) {
        {
            if (key_array[(i_darts_counter_temp352)-1] > key_array[(i_darts_counter_temp352)]) {
                (*(&(j_LocalReductionVar)))++;
            }
        }
    }
updateReduceVars:
    /*Before signaling next codelet, check if all the threads in the TP
    * performed the reduction locally and if so, update the unique
    * reduction variable.*/
    /*Reducing for operator '+'*/
    this->inputsTPParent->j_darts352_mutex.lock();
    *(this->inputsTPParent->j_darts352) += (j_LocalReductionVar);
    this->inputsTPParent->j_darts352_mutex.unlock();
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets352[0].decDep();
}
TP352::TP352(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet,
    INT_TYPE in_initIteration, INT_TYPE in_lastIteration, INT_TYPE* in_j)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , i_darts352(new INT_TYPE[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , j_darts352(in_j) /*OMP_SHARED - INPUT*/
    , initIteration352(in_initIteration)
    , lastIteration352(in_lastIteration)
    , barrierCodelets352(new _barrierCodelets352[1])
    , checkInCodelets353(new _checkInCodelets353[this->numThreads])
{
    /*Initialize the loop parameters*/
    range352 = abs(lastIteration352 - initIteration352) / 1;
    rangePerCodelet352 = range352 / numThreads;
    minIteration352 = min<INT_TYPE>(lastIteration352, initIteration352);
    remainderRange352 = range352 % numThreads;
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets352[0] = _barrierCodelets352(ompNumThreads, ompNumThreads, this, 0);
    _checkInCodelets353* checkInCodelets353Ptr = (this->checkInCodelets353);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets353Ptr) = _checkInCodelets353(1, 1, this, codeletCounter);
        (*checkInCodelets353Ptr).decDep();
        checkInCodelets353Ptr++;
    }
}
TP352::~TP352()
{
    delete[] barrierCodelets352;
    delete[] checkInCodelets353;
}
/*TP416: OMPParallelDirective*/
void TP416::_barrierCodelets416::fire(void)
{
    TP416* myTP = static_cast<TP416*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
void TP416::_checkInCodelets418::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 418: DeclStmt*/

    /*printing node 419: DeclStmt*/
    this->inputsTPParent->myid_darts416[this->getID()] = 0;
    this->inputsTPParent->num_procs_darts416[this->getID()] = 1;

    /*printing node 420: BinaryOperator*/
    (this->inputsTPParent->myid_darts416[this->getID()]) = omp_get_thread_num();

    /*printing node 422: BinaryOperator*/
    (this->inputsTPParent->num_procs_darts416[this->getID()]) = omp_get_num_threads();

    /*printing node 424: BinaryOperator*/
    (this->inputsTPParent->work_buff_darts416[this->getID()])
        = bucket_size[(this->inputsTPParent->myid_darts416[this->getID()])];

    /*printing node 426: ForStmt*/
    {
        {
            INT_TYPE* i = &(this->inputsTPParent->i_darts416[this->getLocalID()]);
            (void)i /*OMP_PRIVATE*/;
            INT_TYPE** work_buff = &(this->inputsTPParent->work_buff_darts416[this->getLocalID()]);
            (void)work_buff /*PRIVATE*/;
            /*Loop's init*/
            (this->inputsTPParent->i_darts416[this->getID()]) = 0;
            INT_TYPE i_darts_counter_temp416 = (this->inputsTPParent->i_darts416[this->getID()]);
            for (; (i_darts_counter_temp416) < NUM_BUCKETS; (i_darts_counter_temp416)++) {
                (*work_buff)[(i_darts_counter_temp416)] = 0;
            }
            (this->inputsTPParent->i_darts416[this->getID()]) = i_darts_counter_temp416;
        }
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 418 nextRegion: 432 */
    myTP->controlTPParent->checkInCodelets432[this->getID()].decDep();
}
void TP416::_checkInCodelets432::fire(void)
{
    /*printOMPForCodeletFireCode, inlined: 0*/
    /*region 432 0*/
    /*Determine the TP to which this codelet belongs and check if this codelet spawns the TP or if
     * it signals it using dispatchCodelet()*/
    size_t idx = this->getID() / myTP->codeletsPerTP432;
    if (idx < myTP->TPsToUse432) {
        if (!__sync_val_compare_and_swap(&(myTP->TP432_alreadyLaunched[idx]), 0, 1)) {
            INT_TYPE range = abs(NUM_KEYS - 0) / 1;
            INT_TYPE rangePerCodelet = range / myTP->TPsToUse432;
            INT_TYPE minIteration = min<INT_TYPE>(NUM_KEYS, 0);
            INT_TYPE remainderRange = range % myTP->TPsToUse432;
            INT_TYPE initIteration = rangePerCodelet * idx;
            INT_TYPE lastIteration = rangePerCodelet * (idx + 1);
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
            if (0 < NUM_KEYS) {
                initIteration = min(initIteration, lastIteration);
                lastIteration = max(initIteration, lastIteration);
            } else {
                initIteration = max(initIteration, lastIteration);
                lastIteration = min(initIteration, lastIteration);
            }
            if (idx == myTP->TPsToUse432 - 1) {
                lastIteration = NUM_KEYS;
            }
#if USEINVOKE == 1
            invoke<TP432>(myTP, myTP->codeletsPerTP432 * DARTS_CODELETS_MULT, this->getID(), myTP,
                initIteration, lastIteration, &(*(this->inputsTPParent->shift_darts416)),
                &(myTP->TP432Ptr[idx]));
#else
            place<TP432>(idx, myTP, myTP->codeletsPerTP432 * DARTS_CODELETS_MULT, this->getID(),
                myTP, initIteration, lastIteration, &(*(this->inputsTPParent->shift_darts416)),
                &(myTP->TP432Ptr[idx]));
#endif
        } else {
            if (myTP->TP432Ptr[idx] != nullptr) {
                myTP->TP432Ptr[idx]->dispatchCodelet(this->getID());
            } else {
                this->resetCodelet();
                this->decDep();
            }
        }
    }
}
void TP416::_barrierCodelets432::fire(void)
{
    TP416* myTP = static_cast<TP416*>(myTP_);
    {
        for (size_t codeletsCounter = 0; codeletsCounter < myTP->numThreads; codeletsCounter++) {
            myTP->checkInCodelets466[codeletsCounter].decDep();
        }
    }
}
void TP416::_checkInCodelets466::fire(void)
{

    /*printing node 466: BinaryOperator*/
    ((bucket_ptrs_dartsThreadPriv0[this->getID()]))[0] = 0;

    /*printing node 468: ForStmt*/
    { { INT_TYPE* k = &(this->inputsTPParent->k_darts416[this->getLocalID()]);
    (void)k /*OMP_PRIVATE*/;
    int* myid = &(this->inputsTPParent->myid_darts416[this->getLocalID()]);
    (void)myid /*PRIVATE*/;
    /*Loop's init*/
    (this->inputsTPParent->k_darts416[this->getID()]) = 0;
    INT_TYPE k_darts_counter_temp416 = (this->inputsTPParent->k_darts416[this->getID()]);
    for (; (k_darts_counter_temp416) < (*myid); (k_darts_counter_temp416)++) {
        ((bucket_ptrs_dartsThreadPriv0[this->getID()]))[0]
            += bucket_size[(k_darts_counter_temp416)][0];
    }
    (this->inputsTPParent->k_darts416[this->getID()]) = k_darts_counter_temp416;
}
}

/*printing node 476: ForStmt*/
{
    INT_TYPE* i = &(this->inputsTPParent->i_darts416[this->getLocalID()]);
    (void)i /*OMP_PRIVATE*/;
    INT_TYPE* k = &(this->inputsTPParent->k_darts416[this->getLocalID()]);
    (void)k /*OMP_PRIVATE*/;
    int* myid = &(this->inputsTPParent->myid_darts416[this->getLocalID()]);
    (void)myid /*PRIVATE*/;
    int* num_procs = &(this->inputsTPParent->num_procs_darts416[this->getLocalID()]);
    (void)num_procs /*PRIVATE*/;
    /*Loop's init*/
    (this->inputsTPParent->i_darts416[this->getID()]) = 1;
    INT_TYPE i_darts_counter_temp416 = (this->inputsTPParent->i_darts416[this->getID()]);
    for (; (i_darts_counter_temp416) < NUM_BUCKETS; (i_darts_counter_temp416)++) {
        ((bucket_ptrs_dartsThreadPriv0[this->getID()]))[(i_darts_counter_temp416)]
            = ((bucket_ptrs_dartsThreadPriv0[this->getID()]))[(i_darts_counter_temp416)-1];
        { { /*Loop's init*/
            (*k) = 0;
        INT_TYPE k_darts_counter_temp416 = (*k);
        for (; k_darts_counter_temp416 < (*myid); k_darts_counter_temp416++) {
            ((bucket_ptrs_dartsThreadPriv0[this->getID()]))[(i_darts_counter_temp416)]
                += bucket_size[k_darts_counter_temp416][(i_darts_counter_temp416)];
        }
        (*k) = k_darts_counter_temp416;
    }
}
{
    {
        /*Loop's init*/
        (*k) = (*myid);
        INT_TYPE k_darts_counter_temp416 = (*k);
        for (; k_darts_counter_temp416 < (*num_procs); k_darts_counter_temp416++) {
            ((bucket_ptrs_dartsThreadPriv0[this->getID()]))[(i_darts_counter_temp416)]
                += bucket_size[k_darts_counter_temp416][(i_darts_counter_temp416)-1];
        }
        (*k) = k_darts_counter_temp416;
    }
}
}
(this->inputsTPParent->i_darts416[this->getID()]) = i_darts_counter_temp416;
}
/*Signaling next codelet from last stmt in the codelet*/
/*Signaling next codelet region: 466 nextRegion: 502 */
myTP->controlTPParent->checkInCodelets502[this->getID()].decDep();
}
void TP416::_checkInCodelets502::fire(void)
{
    /*printOMPForCodeletFireCode, inlined: 0*/
    /*region 502 0*/
    /*Determine the TP to which this codelet belongs and check if this codelet spawns the TP or if
     * it signals it using dispatchCodelet()*/
    size_t idx = this->getID() / myTP->codeletsPerTP502;
    if (idx < myTP->TPsToUse502) {
        if (!__sync_val_compare_and_swap(&(myTP->TP502_alreadyLaunched[idx]), 0, 1)) {
            INT_TYPE range = abs(NUM_KEYS - 0) / 1;
            INT_TYPE rangePerCodelet = range / myTP->TPsToUse502;
            INT_TYPE minIteration = min<INT_TYPE>(NUM_KEYS, 0);
            INT_TYPE remainderRange = range % myTP->TPsToUse502;
            INT_TYPE initIteration = rangePerCodelet * idx;
            INT_TYPE lastIteration = rangePerCodelet * (idx + 1);
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
            if (0 < NUM_KEYS) {
                initIteration = min(initIteration, lastIteration);
                lastIteration = max(initIteration, lastIteration);
            } else {
                initIteration = max(initIteration, lastIteration);
                lastIteration = min(initIteration, lastIteration);
            }
            if (idx == myTP->TPsToUse502 - 1) {
                lastIteration = NUM_KEYS;
            }
#if USEINVOKE == 1
            invoke<TP502>(myTP, myTP->codeletsPerTP502 * DARTS_CODELETS_MULT, this->getID(), myTP,
                initIteration, lastIteration, &(*(this->inputsTPParent->shift_darts416)),
                &(myTP->TP502Ptr[idx]));
#else
            place<TP502>(idx, myTP, myTP->codeletsPerTP502 * DARTS_CODELETS_MULT, this->getID(),
                myTP, initIteration, lastIteration, &(*(this->inputsTPParent->shift_darts416)),
                &(myTP->TP502Ptr[idx]));
#endif
        } else {
            if (myTP->TP502Ptr[idx] != nullptr) {
                myTP->TP502Ptr[idx]->dispatchCodelet(this->getID());
            } else {
                this->resetCodelet();
                this->decDep();
            }
        }
    }
}
void TP416::_barrierCodelets502::fire(void)
{
    TP416* myTP = static_cast<TP416*>(myTP_);
    {
        for (size_t codeletsCounter = 0; codeletsCounter < myTP->numThreads; codeletsCounter++) {
            myTP->checkInCodelets539[codeletsCounter].decDep();
        }
    }
}
void TP416::_checkInCodelets539::fire(void)
{

    /*printing node 539: IfStmt*/
    if ((this->inputsTPParent->myid_darts416[this->getID()])
        < (this->inputsTPParent->num_procs_darts416[this->getID()]) - 1) {
        {
            {
                /*Loop's init*/
                (this->inputsTPParent->i_darts416[this->getID()]) = 0;
                INT_TYPE i_darts_counter_temp416
                    = (this->inputsTPParent->i_darts416[this->getID()]);
                for (; i_darts_counter_temp416 < NUM_BUCKETS; i_darts_counter_temp416++) {
                    {
                        {
                            /*Loop's init*/
                            (this->inputsTPParent->k_darts416[this->getID()])
                                = (this->inputsTPParent->myid_darts416[this->getID()]) + 1;
                            INT_TYPE k_darts_counter_temp416
                                = (this->inputsTPParent->k_darts416[this->getID()]);
                            for (; k_darts_counter_temp416
                                 < (this->inputsTPParent->num_procs_darts416[this->getID()]);
                                 k_darts_counter_temp416++) {
                                ((bucket_ptrs_dartsThreadPriv0[this->getID()]))
                                    [i_darts_counter_temp416]
                                    += bucket_size[k_darts_counter_temp416]
                                                  [i_darts_counter_temp416];
                            }
                            (this->inputsTPParent->k_darts416[this->getID()])
                                = k_darts_counter_temp416;
                        }
                    }
                }
                (this->inputsTPParent->i_darts416[this->getID()]) = i_darts_counter_temp416;
            }
        }
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 539 nextRegion: 555 */
    myTP->controlTPParent->checkInCodelets555[this->getID()].decDep();
}
void TP416::_checkInCodelets555::fire(void)
{
    /*printOMPForCodeletFireCode, inlined: 0*/
    /*region 555 0*/
    /*Determine the TP to which this codelet belongs and check if this codelet spawns the TP or if
     * it signals it using dispatchCodelet()*/
    size_t idx = this->getID() / myTP->codeletsPerTP555;
    if (idx < myTP->TPsToUse555) {
        if (!__sync_val_compare_and_swap(&(myTP->TP555_alreadyLaunched[idx]), 0, 1)) {
            INT_TYPE range = abs(NUM_BUCKETS - 0) / 1;
            INT_TYPE rangePerCodelet = range / myTP->TPsToUse555;
            INT_TYPE minIteration = min<INT_TYPE>(NUM_BUCKETS, 0);
            INT_TYPE remainderRange = range % myTP->TPsToUse555;
            INT_TYPE initIteration = rangePerCodelet * idx;
            INT_TYPE lastIteration = rangePerCodelet * (idx + 1);
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
            if (0 < NUM_BUCKETS) {
                initIteration = min(initIteration, lastIteration);
                lastIteration = max(initIteration, lastIteration);
            } else {
                initIteration = max(initIteration, lastIteration);
                lastIteration = min(initIteration, lastIteration);
            }
            if (idx == myTP->TPsToUse555 - 1) {
                lastIteration = NUM_BUCKETS;
            }
#if USEINVOKE == 1
            invoke<TP555>(myTP, myTP->codeletsPerTP555 * DARTS_CODELETS_MULT, this->getID(), myTP,
                initIteration, lastIteration, &(*(this->inputsTPParent->key_buff_ptr_darts416)),
                &(*(this->inputsTPParent->key_buff_ptr2_darts416)),
                &(*(this->inputsTPParent->num_bucket_keys_darts416)), &(myTP->TP555Ptr[idx]));
#else
            place<TP555>(idx, myTP, myTP->codeletsPerTP555 * DARTS_CODELETS_MULT, this->getID(),
                myTP, initIteration, lastIteration,
                &(*(this->inputsTPParent->key_buff_ptr_darts416)),
                &(*(this->inputsTPParent->key_buff_ptr2_darts416)),
                &(*(this->inputsTPParent->num_bucket_keys_darts416)), &(myTP->TP555Ptr[idx]));
#endif
        } else {
            if (myTP->TP555Ptr[idx] != nullptr) {
                myTP->TP555Ptr[idx]->dispatchCodelet(this->getID());
            } else {
                this->resetCodelet();
                this->decDep();
            }
        }
    }
}
void TP416::_barrierCodelets555::fire(void)
{
    TP416* myTP = static_cast<TP416*>(myTP_);
    myTP->TPParent->barrierCodelets416[0].setDep(0);
    myTP->add(&(myTP->TPParent->barrierCodelets416[0]));
}
TP416::TP416(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet,
    INT_TYPE** in_key_buff_ptr, INT_TYPE** in_key_buff_ptr2, INT_TYPE* in_num_bucket_keys,
    int* in_shift)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , i_darts416(new INT_TYPE[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , k_darts416(new INT_TYPE[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , key_buff_ptr_darts416(in_key_buff_ptr) /*OMP_SHARED - INPUT*/
    , key_buff_ptr2_darts416(in_key_buff_ptr2) /*OMP_SHARED - INPUT*/
    , num_bucket_keys_darts416(in_num_bucket_keys) /*OMP_SHARED - INPUT*/
    , shift_darts416(in_shift) /*OMP_SHARED - INPUT*/
    , k1_darts416(new INT_TYPE[this->numThreads]) /*VARIABLE*/
    , k2_darts416(new INT_TYPE[this->numThreads]) /*VARIABLE*/
    , m_darts416(new INT_TYPE[this->numThreads]) /*VARIABLE*/
    , myid_darts416(new int[this->numThreads]) /*VARIABLE*/
    , num_procs_darts416(new int[this->numThreads]) /*VARIABLE*/
    , work_buff_darts416(new INT_TYPE*[this->numThreads]) /*VARIABLE*/
    , TP432Ptr(new TP432*[NUMTPS432])
    , TP432_alreadyLaunched(new size_t[NUMTPS432])
    , numTPsSet432(0)
    , numTPsReady432(0)
    , TPsToUse432(NUMTPS432)
    , codeletsPerTP432(this->numThreads / NUMTPS432)
    , totalCodelets432(this->TPsToUse432 * this->codeletsPerTP432)
    , TP502Ptr(new TP502*[NUMTPS502])
    , TP502_alreadyLaunched(new size_t[NUMTPS502])
    , numTPsSet502(0)
    , numTPsReady502(0)
    , TPsToUse502(NUMTPS502)
    , codeletsPerTP502(this->numThreads / NUMTPS502)
    , totalCodelets502(this->TPsToUse502 * this->codeletsPerTP502)
    , TP555Ptr(new TP555*[NUMTPS555])
    , TP555_alreadyLaunched(new size_t[NUMTPS555])
    , numTPsSet555(0)
    , numTPsReady555(0)
    , TPsToUse555(NUMTPS555)
    , codeletsPerTP555(this->numThreads / NUMTPS555)
    , totalCodelets555(this->TPsToUse555 * this->codeletsPerTP555)
    , barrierCodelets416(new _barrierCodelets416[1])
    , checkInCodelets418(new _checkInCodelets418[this->numThreads])
    , checkInCodelets432(new _checkInCodelets432[this->numThreads])
    , barrierCodelets432(new _barrierCodelets432[1])
    , checkInCodelets466(new _checkInCodelets466[this->numThreads])
    , checkInCodelets502(new _checkInCodelets502[this->numThreads])
    , barrierCodelets502(new _barrierCodelets502[1])
    , checkInCodelets539(new _checkInCodelets539[this->numThreads])
    , checkInCodelets555(new _checkInCodelets555[this->numThreads])
    , barrierCodelets555(new _barrierCodelets555[1])
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets416[0] = _barrierCodelets416(ompNumThreads, ompNumThreads, this, 0);
    barrierCodelets555[0] = _barrierCodelets555(NUMTPS555, NUMTPS555, this, 0);
    barrierCodelets502[0] = _barrierCodelets502(NUMTPS502, NUMTPS502, this, 0);
    barrierCodelets432[0] = _barrierCodelets432(NUMTPS432, NUMTPS432, this, 0);
    _checkInCodelets555* checkInCodelets555Ptr = (this->checkInCodelets555);
    for (int i = 0; i < NUMTPS555; i++) {
        TP555Ptr[i] = nullptr;
        TP555_alreadyLaunched[i] = 0;
    }
    _checkInCodelets539* checkInCodelets539Ptr = (this->checkInCodelets539);
    _checkInCodelets502* checkInCodelets502Ptr = (this->checkInCodelets502);
    for (int i = 0; i < NUMTPS502; i++) {
        TP502Ptr[i] = nullptr;
        TP502_alreadyLaunched[i] = 0;
    }
    _checkInCodelets466* checkInCodelets466Ptr = (this->checkInCodelets466);
    _checkInCodelets432* checkInCodelets432Ptr = (this->checkInCodelets432);
    for (int i = 0; i < NUMTPS432; i++) {
        TP432Ptr[i] = nullptr;
        TP432_alreadyLaunched[i] = 0;
    }
    _checkInCodelets418* checkInCodelets418Ptr = (this->checkInCodelets418);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets555Ptr) = _checkInCodelets555(1, 1, this, codeletCounter);
        checkInCodelets555Ptr++;
        (*checkInCodelets539Ptr) = _checkInCodelets539(1, 1, this, codeletCounter);
        checkInCodelets539Ptr++;
        (*checkInCodelets502Ptr) = _checkInCodelets502(1, 1, this, codeletCounter);
        checkInCodelets502Ptr++;
        (*checkInCodelets466Ptr) = _checkInCodelets466(1, 1, this, codeletCounter);
        checkInCodelets466Ptr++;
        (*checkInCodelets432Ptr) = _checkInCodelets432(1, 1, this, codeletCounter);
        checkInCodelets432Ptr++;
        (*checkInCodelets418Ptr) = _checkInCodelets418(1, 1, this, codeletCounter);
        (*checkInCodelets418Ptr).decDep();
        checkInCodelets418Ptr++;
    }
}
TP416::~TP416()
{
    delete[] k1_darts416;
    delete[] k2_darts416;
    delete[] m_darts416;
    delete[] myid_darts416;
    delete[] num_procs_darts416;
    delete[] work_buff_darts416;
    delete[] barrierCodelets416;
    delete[] barrierCodelets555;
    delete[] checkInCodelets555;
    delete[] checkInCodelets539;
    delete[] barrierCodelets502;
    delete[] checkInCodelets502;
    delete[] checkInCodelets466;
    delete[] barrierCodelets432;
    delete[] checkInCodelets432;
    delete[] checkInCodelets418;
}
/*TP432: OMPForDirective*/
void TP432::_barrierCodelets432::fire(void)
{
    TP432* myTP = static_cast<TP432*>(myTP_);
    myTP->controlTPParent->TPParent->barrierCodelets432[0].decDep();
}
bool TP432::requestNewRangeIterations432(INT_TYPE* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Static */
    /*Chunk = 1*/
    bool isThereNewIteration = false;
    {
        /*Static Scheduling*/
        INT_TYPE tempStartRange = rangePerCodelet432 * codeletID;
        INT_TYPE tempEndRange = rangePerCodelet432 * (codeletID + 1);
        if (remainderRange432 != 0) {
            if (codeletID < (uint32_t)remainderRange432) {
                tempStartRange += codeletID;
                tempEndRange += (codeletID + 1);
            } else {
                tempStartRange += remainderRange432;
                tempEndRange += remainderRange432;
            }
        }
        tempStartRange = tempStartRange * 1 + minIteration432;
        tempEndRange = tempEndRange * 1 + minIteration432;
        if (tempStartRange != tempEndRange) {
            isThereNewIteration = true;
        }
        if (initIteration432 < lastIteration432) {
            (this->inputsTPParent->i_darts432[codeletID]) = min(tempStartRange, tempEndRange);
            *endRange = max(tempStartRange, tempEndRange);
        } else {
            (this->inputsTPParent->i_darts432[codeletID]) = max(tempStartRange, tempEndRange);
            *endRange = min(tempStartRange, tempEndRange);
        }
        if (codeletID == this->numThreads - 1) {
            *endRange = lastIteration432;
        }
    }
    return isThereNewIteration;
}
void TP432::_checkInCodelets433::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    /*Init the vars for this region*/
    /*Get pointer from parent for variable
     with shared scope in this region but private
     in the enclosing one.*/
    this->inputsTPParent->work_buff_darts432[this->getLocalID()]
        = (INT_TYPE**)&(myTP->TPParent->inputsTPParent->work_buff_darts416[this->getID()]);

    /*printing node 433: ForStmt*/
    /*var: i*/
    /*var: shift*/
    /*var: work_buff*/
    INT_TYPE* i = &(this->inputsTPParent->i_darts432[this->getLocalID()]);
    (void)i /*OMP_PRIVATE*/;
    int* shift = (this->inputsTPParent->shift_darts432);
    (void)shift /*OMP_SHARED*/;
    INT_TYPE*** work_buff = &(this->inputsTPParent->work_buff_darts432[this->getLocalID()]);
    (void)work_buff /*OMP_SHARED_PRIVATE*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations432(
        (INT_TYPE*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        /*Signaling omp for stmt's barrier*/
        myTP->controlTPParent->barrierCodelets432[0].decDep();
        return;
    }
    for (INT_TYPE i_darts_counter_temp432 = (*i); i_darts_counter_temp432 < endRange
         && i_darts_counter_temp432 < this->inputsTPParent->lastIteration432;
         i_darts_counter_temp432++) {
        {
            (*(*work_buff))[key_array[(i_darts_counter_temp432)] >> (*(shift))]++;
        }
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling omp for stmt's barrier*/
    myTP->controlTPParent->barrierCodelets432[0].decDep();
}
TP432::TP432(int in_numThreads, int in_mainCodeletID, TP416* in_TPParent, INT_TYPE in_initIteration,
    INT_TYPE in_lastIteration, int* in_shift, TP432** in_ptrToThisTP)
    : ompTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , i_darts432(new INT_TYPE[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , shift_darts432(in_shift) /*OMP_SHARED - INPUT*/
    , work_buff_darts432(new INT_TYPE**[this->numThreads])
    , initIteration432(in_initIteration)
    , lastIteration432(in_lastIteration)
    , readyCodelets(0)
    , baseNumThreads(this->numThreads / DARTS_CODELETS_MULT)
    , barrierCodelets432(new _barrierCodelets432[1])
    , checkInCodelets433(new _checkInCodelets433[this->numThreads])
{
    /*Initialize the loop parameters*/
    range432 = abs(lastIteration432 - initIteration432) / 1;
    rangePerCodelet432 = range432 / numThreads;
    minIteration432 = min<INT_TYPE>(lastIteration432, initIteration432);
    remainderRange432 = range432 % numThreads;
    /*Initialize inputs and vars.*/
    this->work_buff_darts432
        = (INT_TYPE***)malloc(sizeof(INT_TYPE**) * this->numThreads) /*OMP_SHARED_PRIVATE*/;
    /*Initialize Codelets*/
    barrierCodelets432[0] = _barrierCodelets432(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets433* checkInCodelets433Ptr = (this->checkInCodelets433);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets433);
#endif
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->baseNumThreads;
         codeletCounter++) {
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets433Ptr) = _checkInCodelets433(2, 1, this, codeletCounter);
#else
        (*checkInCodelets433Ptr) = _checkInCodelets433(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets433Ptr).decDep();
        checkInCodelets433Ptr++;
    }
    this->dispatchCodelet(this->getID());
    *(in_ptrToThisTP) = this;
}
void TP432::dispatchCodelet(size_t codeletID)
{
    int idx = codeletID / this->baseNumThreads;
    int localID = codeletID - this->baseNumThreads * idx;
    this->checkInCodelets433[localID].setID(codeletID);
    this->checkInCodelets433[localID].setLocalID(localID);
    /*Check if we want to replicate codelets*/
    for (size_t i = 1; i < (size_t)DARTS_CODELETS_MULT; i++) {
#if USE_SPIN_CODELETS == 0
        this->checkInCodelets433[localID + this->baseNumThreads * i]
            = _checkInCodelets433(2, 1, this, localID + this->baseNumThreads * i);
#else
        this->checkInCodelets433[localID + this->baseNumThreads * i]
            = _checkInCodelets433(1, 1, this, localID + this->baseNumThreads * i);
#endif
        this->checkInCodelets433[localID + this->baseNumThreads * i].setID(codeletID);
        this->checkInCodelets433[localID + this->baseNumThreads * i].decDep();
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
TP432::~TP432()
{
    delete[] work_buff_darts432;
    delete[] barrierCodelets432;
    delete[] checkInCodelets433;
}
/*TP502: OMPForDirective*/
void TP502::_barrierCodelets502::fire(void)
{
    TP502* myTP = static_cast<TP502*>(myTP_);
    myTP->controlTPParent->TPParent->barrierCodelets502[0].decDep();
}
bool TP502::requestNewRangeIterations502(INT_TYPE* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Static */
    /*Chunk = 1*/
    bool isThereNewIteration = false;
    {
        /*Static Scheduling*/
        INT_TYPE tempStartRange = rangePerCodelet502 * codeletID;
        INT_TYPE tempEndRange = rangePerCodelet502 * (codeletID + 1);
        if (remainderRange502 != 0) {
            if (codeletID < (uint32_t)remainderRange502) {
                tempStartRange += codeletID;
                tempEndRange += (codeletID + 1);
            } else {
                tempStartRange += remainderRange502;
                tempEndRange += remainderRange502;
            }
        }
        tempStartRange = tempStartRange * 1 + minIteration502;
        tempEndRange = tempEndRange * 1 + minIteration502;
        if (tempStartRange != tempEndRange) {
            isThereNewIteration = true;
        }
        if (initIteration502 < lastIteration502) {
            (this->inputsTPParent->i_darts502[codeletID]) = min(tempStartRange, tempEndRange);
            *endRange = max(tempStartRange, tempEndRange);
        } else {
            (this->inputsTPParent->i_darts502[codeletID]) = max(tempStartRange, tempEndRange);
            *endRange = min(tempStartRange, tempEndRange);
        }
        if (codeletID == this->numThreads - 1) {
            *endRange = lastIteration502;
        }
    }
    return isThereNewIteration;
}
void TP502::_checkInCodelets503::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    /*Init the vars for this region*/
    /*Get pointer from parent for variable
     with shared scope in this region but private
     in the enclosing one.*/
    this->inputsTPParent->k_darts502[this->getLocalID()]
        = (INT_TYPE*)&(myTP->TPParent->inputsTPParent->k_darts416[this->getID()]);

    /*printing node 503: ForStmt*/
    /*var: i*/
    /*var: k*/
    /*var: shift*/
    INT_TYPE* i = &(this->inputsTPParent->i_darts502[this->getLocalID()]);
    (void)i /*OMP_PRIVATE*/;
    INT_TYPE** k = &(this->inputsTPParent->k_darts502[this->getLocalID()]);
    (void)k /*OMP_SHARED_PRIVATE*/;
    int* shift = (this->inputsTPParent->shift_darts502);
    (void)shift /*OMP_SHARED*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations502(
        (INT_TYPE*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        /*Signaling omp for stmt's barrier*/
        myTP->controlTPParent->barrierCodelets502[0].decDep();
        return;
    }
    for (INT_TYPE i_darts_counter_temp502 = (*i); i_darts_counter_temp502 < endRange
         && i_darts_counter_temp502 < this->inputsTPParent->lastIteration502;
         i_darts_counter_temp502++) {
        {
            (*(*k)) = key_array[(i_darts_counter_temp502)];
            key_buff2[((bucket_ptrs_dartsThreadPriv0[this->getID()]))[(*(*k)) >> (*(shift))]++]
                = (*(*k));
        }
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling omp for stmt's barrier*/
    myTP->controlTPParent->barrierCodelets502[0].decDep();
}
TP502::TP502(int in_numThreads, int in_mainCodeletID, TP416* in_TPParent, INT_TYPE in_initIteration,
    INT_TYPE in_lastIteration, int* in_shift, TP502** in_ptrToThisTP)
    : ompTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , i_darts502(new INT_TYPE[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , k_darts502(new INT_TYPE*[this->numThreads])
    , shift_darts502(in_shift) /*OMP_SHARED - INPUT*/
    , initIteration502(in_initIteration)
    , lastIteration502(in_lastIteration)
    , readyCodelets(0)
    , baseNumThreads(this->numThreads / DARTS_CODELETS_MULT)
    , barrierCodelets502(new _barrierCodelets502[1])
    , checkInCodelets503(new _checkInCodelets503[this->numThreads])
{
    /*Initialize the loop parameters*/
    range502 = abs(lastIteration502 - initIteration502) / 1;
    rangePerCodelet502 = range502 / numThreads;
    minIteration502 = min<INT_TYPE>(lastIteration502, initIteration502);
    remainderRange502 = range502 % numThreads;
    /*Initialize inputs and vars.*/
    this->k_darts502
        = (INT_TYPE**)malloc(sizeof(INT_TYPE*) * this->numThreads) /*OMP_SHARED_PRIVATE*/;
    /*Initialize Codelets*/
    barrierCodelets502[0] = _barrierCodelets502(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets503* checkInCodelets503Ptr = (this->checkInCodelets503);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets503);
#endif
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->baseNumThreads;
         codeletCounter++) {
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets503Ptr) = _checkInCodelets503(2, 1, this, codeletCounter);
#else
        (*checkInCodelets503Ptr) = _checkInCodelets503(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets503Ptr).decDep();
        checkInCodelets503Ptr++;
    }
    this->dispatchCodelet(this->getID());
    *(in_ptrToThisTP) = this;
}
void TP502::dispatchCodelet(size_t codeletID)
{
    int idx = codeletID / this->baseNumThreads;
    int localID = codeletID - this->baseNumThreads * idx;
    this->checkInCodelets503[localID].setID(codeletID);
    this->checkInCodelets503[localID].setLocalID(localID);
    /*Check if we want to replicate codelets*/
    for (size_t i = 1; i < (size_t)DARTS_CODELETS_MULT; i++) {
#if USE_SPIN_CODELETS == 0
        this->checkInCodelets503[localID + this->baseNumThreads * i]
            = _checkInCodelets503(2, 1, this, localID + this->baseNumThreads * i);
#else
        this->checkInCodelets503[localID + this->baseNumThreads * i]
            = _checkInCodelets503(1, 1, this, localID + this->baseNumThreads * i);
#endif
        this->checkInCodelets503[localID + this->baseNumThreads * i].setID(codeletID);
        this->checkInCodelets503[localID + this->baseNumThreads * i].decDep();
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
TP502::~TP502()
{
    delete[] k_darts502;
    delete[] barrierCodelets502;
    delete[] checkInCodelets503;
}
/*TP555: OMPForDirective*/
void TP555::_barrierCodelets555::fire(void)
{
    TP555* myTP = static_cast<TP555*>(myTP_);
    myTP->controlTPParent->TPParent->barrierCodelets555[0].decDep();
}
bool TP555::requestNewRangeIterations555(INT_TYPE* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Dynamic */
    /*Chunk = 1*/
    bool isThereNewIteration = false;
    {
        /*Dynamic Scheduling*/
        if (initIteration555 < lastIteration555) {
            (this->inputsTPParent->i_darts555[codeletID])
                = __sync_fetch_and_add(&(nextIteration555), 1 * 1);
            *endRange = (this->inputsTPParent->i_darts555[codeletID]) + 1 * 1;
        } else {
            (this->inputsTPParent->i_darts555[codeletID])
                = __sync_fetch_and_sub(&(nextIteration555), 1 * 1);
            *endRange = (this->inputsTPParent->i_darts555[codeletID]) - 1 * 1;
        }
        if ((this->inputsTPParent->i_darts555[codeletID]) < lastIteration555) {
            isThereNewIteration = true;
        }
    }
    return isThereNewIteration;
}
void TP555::_checkInCodelets556::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    /*Init the vars for this region*/
    /*Get pointer from parent for variable
     with shared scope in this region but private
     in the enclosing one.*/
    this->inputsTPParent->k1_darts555[this->getLocalID()]
        = (INT_TYPE*)&(myTP->TPParent->inputsTPParent->k1_darts416[this->getID()]);
    /*Get pointer from parent for variable
     with shared scope in this region but private
     in the enclosing one.*/
    this->inputsTPParent->k2_darts555[this->getLocalID()]
        = (INT_TYPE*)&(myTP->TPParent->inputsTPParent->k2_darts416[this->getID()]);
    /*Get pointer from parent for variable
     with shared scope in this region but private
     in the enclosing one.*/
    this->inputsTPParent->m_darts555[this->getLocalID()]
        = (INT_TYPE*)&(myTP->TPParent->inputsTPParent->m_darts416[this->getID()]);

    /*printing node 556: ForStmt*/
    /*var: i*/
    /*var: k*/
    /*var: k1*/
    /*var: k2*/
    /*var: key_buff_ptr*/
    /*var: key_buff_ptr2*/
    /*var: m*/
    /*var: num_bucket_keys*/
    INT_TYPE* i = &(this->inputsTPParent->i_darts555[this->getLocalID()]);
    (void)i /*OMP_PRIVATE*/;
    INT_TYPE* k = &(this->inputsTPParent->k_darts555[this->getLocalID()]);
    (void)k /*OMP_PRIVATE*/;
    INT_TYPE** k1 = &(this->inputsTPParent->k1_darts555[this->getLocalID()]);
    (void)k1 /*OMP_SHARED_PRIVATE*/;
    INT_TYPE** k2 = &(this->inputsTPParent->k2_darts555[this->getLocalID()]);
    (void)k2 /*OMP_SHARED_PRIVATE*/;
    INT_TYPE** key_buff_ptr = (this->inputsTPParent->key_buff_ptr_darts555);
    (void)key_buff_ptr /*OMP_SHARED*/;
    INT_TYPE** key_buff_ptr2 = (this->inputsTPParent->key_buff_ptr2_darts555);
    (void)key_buff_ptr2 /*OMP_SHARED*/;
    INT_TYPE** m = &(this->inputsTPParent->m_darts555[this->getLocalID()]);
    (void)m /*OMP_SHARED_PRIVATE*/;
    INT_TYPE* num_bucket_keys = (this->inputsTPParent->num_bucket_keys_darts555);
    (void)num_bucket_keys /*OMP_SHARED*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations555(
        (INT_TYPE*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        /*Signaling omp for stmt's barrier*/
        myTP->controlTPParent->barrierCodelets555[0].decDep();
        return;
    }
    while (isThereNewIteration) {
        for (INT_TYPE i_darts_counter_temp555 = (*i); i_darts_counter_temp555 < endRange
             && i_darts_counter_temp555 < this->inputsTPParent->lastIteration555;
             i_darts_counter_temp555++) {
            {
                (*(*k1)) = (i_darts_counter_temp555) * (*(num_bucket_keys));
                (*(*k2)) = (*(*k1)) + (*(num_bucket_keys));
                { { /*Loop's init*/
                    (*k) = (*(*k1));
                INT_TYPE k_darts_counter_temp555 = (*k);
                for (; k_darts_counter_temp555 < (*(*k2)); k_darts_counter_temp555++) {
                    (*(key_buff_ptr))[k_darts_counter_temp555] = 0;
                }
                (*k) = k_darts_counter_temp555;
            }
        }
        (*(*m)) = ((i_darts_counter_temp555) > 0)
            ? ((bucket_ptrs_dartsThreadPriv0[this->getID()]))[(i_darts_counter_temp555)-1]
            : 0;
        { { /*Loop's init*/
            (*k) = (*(*m));
        INT_TYPE k_darts_counter_temp555 = (*k);
        for (; k_darts_counter_temp555
             < ((bucket_ptrs_dartsThreadPriv0[this->getID()]))[(i_darts_counter_temp555)];
             k_darts_counter_temp555++) {
            (*(key_buff_ptr))[(*(key_buff_ptr2))[k_darts_counter_temp555]]++;
        }
        (*k) = k_darts_counter_temp555;
    }
}
(*(key_buff_ptr))[(*(*k1))] += (*(*m));
{
    {
        /*Loop's init*/
        (*k) = (*(*k1)) + 1;
        INT_TYPE k_darts_counter_temp555 = (*k);
        for (; k_darts_counter_temp555 < (*(*k2)); k_darts_counter_temp555++) {
            (*(key_buff_ptr))[k_darts_counter_temp555]
                += (*(key_buff_ptr))[k_darts_counter_temp555 - 1];
        }
        (*k) = k_darts_counter_temp555;
    }
}
}
}
isThereNewIteration
    = this->inputsTPParent->requestNewRangeIterations555(&(this->endRange), this->getLocalID());
}
/*Signaling next codelet from last stmt in the codelet*/
/*Signaling omp for stmt's barrier*/
myTP->controlTPParent->barrierCodelets555[0].decDep();
}
TP555::TP555(int in_numThreads, int in_mainCodeletID, TP416* in_TPParent, INT_TYPE in_initIteration,
    INT_TYPE in_lastIteration, INT_TYPE** in_key_buff_ptr, INT_TYPE** in_key_buff_ptr2,
    INT_TYPE* in_num_bucket_keys, TP555** in_ptrToThisTP)
    : ompTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , i_darts555(new INT_TYPE[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , k_darts555(new INT_TYPE[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , k1_darts555(new INT_TYPE*[this->numThreads])
    , k2_darts555(new INT_TYPE*[this->numThreads])
    , key_buff_ptr_darts555(in_key_buff_ptr) /*OMP_SHARED - INPUT*/
    , key_buff_ptr2_darts555(in_key_buff_ptr2) /*OMP_SHARED - INPUT*/
    , m_darts555(new INT_TYPE*[this->numThreads])
    , num_bucket_keys_darts555(in_num_bucket_keys) /*OMP_SHARED - INPUT*/
    , initIteration555(in_initIteration)
    , lastIteration555(in_lastIteration)
    , nextIteration555(INT_MAX)
    , loop555alreadySetUp(0)
    , readyCodelets(0)
    , baseNumThreads(this->numThreads / DARTS_CODELETS_MULT)
    , barrierCodelets555(new _barrierCodelets555[1])
    , checkInCodelets556(new _checkInCodelets556[this->numThreads])
{
    /*Initialize the loop parameters*/
    nextIteration555 = initIteration555;
    /*Initialize inputs and vars.*/
    this->k1_darts555
        = (INT_TYPE**)malloc(sizeof(INT_TYPE*) * this->numThreads) /*OMP_SHARED_PRIVATE*/;
    this->k2_darts555
        = (INT_TYPE**)malloc(sizeof(INT_TYPE*) * this->numThreads) /*OMP_SHARED_PRIVATE*/;
    this->m_darts555
        = (INT_TYPE**)malloc(sizeof(INT_TYPE*) * this->numThreads) /*OMP_SHARED_PRIVATE*/;
    /*Initialize Codelets*/
    barrierCodelets555[0] = _barrierCodelets555(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets556* checkInCodelets556Ptr = (this->checkInCodelets556);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets556);
#endif
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->baseNumThreads;
         codeletCounter++) {
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets556Ptr) = _checkInCodelets556(2, 1, this, codeletCounter);
#else
        (*checkInCodelets556Ptr) = _checkInCodelets556(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets556Ptr).decDep();
        checkInCodelets556Ptr++;
    }
    this->dispatchCodelet(this->getID());
    *(in_ptrToThisTP) = this;
}
void TP555::dispatchCodelet(size_t codeletID)
{
    int currentCodelet = __sync_fetch_and_add(&(this->readyCodelets), 1);
    this->checkInCodelets556[currentCodelet].setID(codeletID);
#if USE_SPIN_CODELETS == 0
    this->firstCodelet[currentCodelet].decDep();
#else
    this->availableCodelets[currentCodelet] = 1;
#endif
}
TP555::~TP555()
{
    delete[] k1_darts555;
    delete[] k2_darts555;
    delete[] m_darts555;
    delete[] barrierCodelets555;
    delete[] checkInCodelets556;
}
