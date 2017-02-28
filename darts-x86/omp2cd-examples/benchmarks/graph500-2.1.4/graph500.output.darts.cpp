#include "graph500.output.darts.h"
using namespace darts;
using namespace std;
static packed_edge* __restrict IJ;
static int64_t bfs_nedge[8];
static int64_t bfs_root[8];
static double bfs_time[8];
static double construction_time;
static double generation_time;
static int64_t nedge;
static int64_t nvtx_scale;
static void output_results(const int64_t SCALE, int64_t nvtx_scale, int64_t edgefactor,
    const double A, const double B, const double C, const double D, const double generation_time,
    const double construction_time, const int NBFS, const double* bfs_time,
    const int64_t* bfs_nedge);
static int dcmp(const void* a, const void* b);
static void run_bfs();
/*Function: main, ID: 3*/
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
    /*main:3*/
    /*CompoundStmt:757*/
    int64_t desired_nedge;
    if (sizeof(int64_t) < 8) {
        fprintf(stderr, "No 64-bit support.\n");
        return 1;
    }
    if (argc > 1)
        get_options(argc, argv);
    nvtx_scale = ((int64_t)1) << SCALE;
    init_random();
    desired_nedge = nvtx_scale * edgefactor;
    if (!dumpname) {
        if (VERBOSE)
            fprintf(stderr, "Generating edge list...");
        if (use_RMAT) {
            nedge = desired_nedge;
            IJ = (packed_edge*)xmalloc_large_ext(nedge * sizeof(*IJ));
            do {
                tic();
                rmat_edgelist(IJ, nedge, SCALE, A, B, C);
                generation_time = toc();
            } while (0);
        } else {
            do {
                tic();
                make_graph(SCALE, desired_nedge, userseed, userseed, &nedge, (packed_edge**)(&IJ));
                generation_time = toc();
            } while (0);
        }
        if (VERBOSE)
            fprintf(stderr, " done.\n");
    } else {
        int fd;
        ssize_t sz;
        if ((fd = open(dumpname, 0)) < 0) {
            perror("Cannot open input graph file");
            return 1;
        }
        sz = nedge * sizeof(*IJ);
        if (sz != read(fd, IJ, sz)) {
            perror("Error reading input graph file");
            return 1;
        }
        close(fd);
    }
    run_bfs();
    xfree_large(IJ);
    output_results(SCALE, nvtx_scale, edgefactor, A, B, C, D, generation_time, construction_time,
        NBFS, bfs_time, bfs_nedge);
    return 0;
}
/*Function: run_bfs, ID: 1*/
void run_bfs()
{
    /*run_bfs:1*/
    /*CompoundStmt:835*/
    int* restrict has_adj;
    int m, err;
    int64_t k, t;
    if (VERBOSE)
        fprintf(stderr, "Creating graph...");
    do {
        tic();
        err = create_graph_from_edgelist(IJ, nedge);
        construction_time = toc();
    } while (0);
    if (VERBOSE)
        fprintf(stderr, "done.\n");
    if (err) {
        fprintf(stderr, "Failure creating graph.\n");
        exit(1);
    }
    if (!rootname) {
        has_adj = (int*)xmalloc_large(nvtx_scale * sizeof(*has_adj));
        //#pragma omp parallel
        {
            //#pragma omp for
            for (k = 0; k < nvtx_scale; ++k)
                has_adj[k] = 0;
            //#pragma omp for
            for (k = 0; k < nedge; ++k) {
                const int64_t i = get_v0_from_edge(&IJ[k]);
                const int64_t j = get_v1_from_edge(&IJ[k]);
                if (i != j)
                    has_adj[i] = has_adj[j] = 1;
            }
        }
        m = 0;
        t = 0;
        while (m < NBFS && t < nvtx_scale) {
            double R = mrg_get_double_orig((mrg_state*)prng_state);
            if (!has_adj[t] || (nvtx_scale - t) * R > NBFS - m)
                ++t;
            else
                bfs_root[m++] = t++;
        }
        if (t >= nvtx_scale && m < NBFS) {
            if (m > 0) {
                fprintf(stderr, "Cannot find %d sample roots of non-self degree > 0, using %d.\n",
                    NBFS, m);
                NBFS = m;
            } else {
                fprintf(stderr, "Cannot find any sample roots of non-self degree > 0.\n");
                exit(1);
            }
        }
        xfree_large(has_adj);
    } else {
        int fd;
        ssize_t sz;
        if ((fd = open(rootname, 0)) < 0) {
            perror("Cannot open input BFS root file");
            exit(1);
        }
        sz = NBFS * sizeof(*bfs_root);
        if (sz != read(fd, bfs_root, sz)) {
            perror("Error reading input BFS root file");
            exit(1);
        }
        close(fd);
    }
    for (m = 0; m < NBFS; ++m) {
        int64_t *bfs_tree, max_bfsvtx;
        bfs_tree = (int64_t*)xmalloc_large(nvtx_scale * sizeof(*bfs_tree));
        if (VERBOSE)
            fprintf(stderr, "Running bfs %d...", m);
        do {
            tic();
            err = make_bfs_tree(bfs_tree, &max_bfsvtx, bfs_root[m]);
            bfs_time[m] = toc();
        } while (0);
        if (VERBOSE)
            fprintf(stderr, "done\n");
        if (err) {
            perror("make_bfs_tree failed");
            abort();
        }
        if (VERBOSE)
            fprintf(stderr, "Verifying bfs %d...", m);
        bfs_nedge[m] = verify_bfs_tree(bfs_tree, max_bfsvtx, bfs_root[m], IJ, nedge);
        if (VERBOSE)
            fprintf(stderr, "done\n");
        if (bfs_nedge[m] < 0) {
            fprintf(stderr, "bfs %d from %ld failed verification (%ld)\n", m, bfs_root[m],
                bfs_nedge[m]);
            abort();
        }
        xfree_large(bfs_tree);
    }
    destroy_graph();
}
/*Function: dcmp, ID: 5*/
static int dcmp(const void* a, const void* b)
{
    /*dcmp:5*/
    /*CompoundStmt:1053*/
    const double da = *(const double*)a;
    const double db = *(const double*)b;
    if (da > db)
        return 1;
    if (db > da)
        return -1;
    if (da == db)
        return 0;
    fprintf(stderr, "No NaNs permitted in output.\n");
    abort();
    return 0;
}
/*Function: statistics, ID: 6*/
void statistics(double* out, double* data, int64_t n)
{
    /*statistics:6*/
    /*CompoundStmt:1076*/
    long double s, mean;
    double t;
    int k;
    qsort(data, n, sizeof(*data), dcmp);
    out[0] = data[0];
    t = (n + 1) / 4.;
    k = (int)t;
    if (t == k)
        out[1] = data[k];
    else
        out[1] = 3 * (data[k] / 4.) + data[k + 1] / 4.;
    t = (n + 1) / 2.;
    k = (int)t;
    if (t == k)
        out[2] = data[k];
    else
        out[2] = data[k] / 2. + data[k + 1] / 2.;
    t = 3 * ((n + 1) / 4.);
    k = (int)t;
    if (t == k)
        out[3] = data[k];
    else
        out[3] = data[k] / 4. + 3 * (data[k + 1] / 4.);
    out[4] = data[n - 1];
    s = data[n - 1];
    for (k = n - 1; k > 0; --k)
        s += data[k - 1];
    mean = s / n;
    out[5] = mean;
    s = data[n - 1] - mean;
    s *= s;
    for (k = n - 1; k > 0; --k) {
        long double tmp = data[k - 1] - mean;
        s += tmp * tmp;
    }
    out[6] = sqrt(s / (n - 1));
    s = (data[0] ? 1.L / data[0] : 0);
    for (k = 1; k < n; ++k)
        s += (data[k] ? 1.L / data[k] : 0);
    out[7] = n / s;
    mean = s / n;
    s = (data[0] ? 1.L / data[0] : 0) - mean;
    s *= s;
    for (k = 1; k < n; ++k) {
        long double tmp = (data[k] ? 1.L / data[k] : 0) - mean;
        s += tmp * tmp;
    }
    s = (sqrt(s) / (n - 1)) * out[7] * out[7];
    out[8] = s;
}
/*Function: output_results, ID: 2*/
void output_results(const int64_t SCALE, int64_t nvtx_scale, int64_t edgefactor, const double A,
    const double B, const double C, const double D, const double generation_time,
    const double construction_time, const int NBFS, const double* bfs_time,
    const int64_t* bfs_nedge)
{
    /*output_results:2*/
    /*CompoundStmt:1252*/
    int k;
    int64_t sz;
    double* tm;
    double* stats;
    tm = (double*)__builtin_alloca(NBFS * sizeof(*tm));
    /*NullStmt:1260*/
    stats = (double*)__builtin_alloca(9 * sizeof(*stats));
    /*NullStmt:1267*/
    if (!tm || !stats) {
        perror("Error allocating within final statistics calculation.");
        abort();
    }
    sz = (1L << SCALE) * edgefactor * 2 * sizeof(int64_t);
    printf("SCALE: %ld\nnvtx: %ld\nedgefactor: %ld\nterasize: %20.17e\n", SCALE, nvtx_scale,
        edgefactor, sz / 1.0E+12);
    printf("A: %20.17e\nB: %20.17e\nC: %20.17e\nD: %20.17e\n", A, B, C, D);
    printf("generation_time: %20.17e\n", generation_time);
    printf("construction_time: %20.17e\n", construction_time);
    printf("nbfs: %d\n", NBFS);
    memcpy(tm, bfs_time, NBFS * sizeof(tm[0]));
    statistics(stats, tm, NBFS);
    do {
        printf("min_%s: %20.17e\n", "time", stats[0]);
        printf("firstquartile_%s: %20.17e\n", "time", stats[1]);
        printf("median_%s: %20.17e\n", "time", stats[2]);
        printf("thirdquartile_%s: %20.17e\n", "time", stats[3]);
        printf("max_%s: %20.17e\n", "time", stats[4]);
        if (!0) {
            printf("mean_%s: %20.17e\n", "time", stats[5]);
            printf("stddev_%s: %20.17e\n", "time", stats[6]);
        } else {
            printf("harmonic_mean_%s: %20.17e\n", "time", stats[7]);
            printf("harmonic_stddev_%s: %20.17e\n", "time", stats[8]);
        }
    } while (0);
    for (k = 0; k < NBFS; ++k)
        tm[k] = bfs_nedge[k];
    statistics(stats, tm, NBFS);
    do {
        printf("min_%s: %20.17e\n", "nedge", stats[0]);
        printf("firstquartile_%s: %20.17e\n", "nedge", stats[1]);
        printf("median_%s: %20.17e\n", "nedge", stats[2]);
        printf("thirdquartile_%s: %20.17e\n", "nedge", stats[3]);
        printf("max_%s: %20.17e\n", "nedge", stats[4]);
        if (!0) {
            printf("mean_%s: %20.17e\n", "nedge", stats[5]);
            printf("stddev_%s: %20.17e\n", "nedge", stats[6]);
        } else {
            printf("harmonic_mean_%s: %20.17e\n", "nedge", stats[7]);
            printf("harmonic_stddev_%s: %20.17e\n", "nedge", stats[8]);
        }
    } while (0);
    for (k = 0; k < NBFS; ++k)
        tm[k] = bfs_nedge[k] / bfs_time[k];
    statistics(stats, tm, NBFS);
    do {
        printf("min_%s: %20.17e\n", "TEPS", stats[0]);
        printf("firstquartile_%s: %20.17e\n", "TEPS", stats[1]);
        printf("median_%s: %20.17e\n", "TEPS", stats[2]);
        printf("thirdquartile_%s: %20.17e\n", "TEPS", stats[3]);
        printf("max_%s: %20.17e\n", "TEPS", stats[4]);
        if (!1) {
            printf("mean_%s: %20.17e\n", "TEPS", stats[5]);
            printf("stddev_%s: %20.17e\n", "TEPS", stats[6]);
        } else {
            printf("harmonic_mean_%s: %20.17e\n", "TEPS", stats[7]);
            printf("harmonic_stddev_%s: %20.17e\n", "TEPS", stats[8]);
        }
    } while (0);
}
/*TP869: OMPParallelDirective*/
void TP869::_barrierCodelets869::fire(void)
{
    TP869* myTP = static_cast<TP869*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
void TP869::_checkInCodelets871::fire(void)
{
    /*printOMPForCodeletFireCode, inlined: 0*/
    /*region 871 0*/
    /*Determine the TP to which this codelet belongs and check if this codelet spawns the TP or if
     * it signals it using dispatchCodelet()*/
    size_t idx = this->getID() / myTP->codeletsPerTP871;
    if (idx < myTP->TPsToUse871) {
        if (!__sync_val_compare_and_swap(&(myTP->TP871_alreadyLaunched[idx]), 0, 1)) {
            int64_t range = abs(nvtx_scale - 0) / 1;
            int64_t rangePerCodelet = range / myTP->TPsToUse871;
            int64_t minIteration = min<int64_t>(nvtx_scale, 0);
            int64_t remainderRange = range % myTP->TPsToUse871;
            int64_t initIteration = rangePerCodelet * idx;
            int64_t lastIteration = rangePerCodelet * (idx + 1);
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
            if (0 < nvtx_scale) {
                initIteration = min(initIteration, lastIteration);
                lastIteration = max(initIteration, lastIteration);
            } else {
                initIteration = max(initIteration, lastIteration);
                lastIteration = min(initIteration, lastIteration);
            }
            if (idx == myTP->TPsToUse871 - 1) {
                lastIteration = nvtx_scale;
            }
#if USEINVOKE == 1
            invoke<TP871>(myTP, myTP->codeletsPerTP871 * DARTS_CODELETS_MULT, this->getID(), myTP,
                initIteration, lastIteration, &(*(this->inputsTPParent->has_adj_darts869)),
                &(myTP->TP871Ptr[idx]));
#else
            place<TP871>(idx, myTP, myTP->codeletsPerTP871 * DARTS_CODELETS_MULT, this->getID(),
                myTP, initIteration, lastIteration, &(*(this->inputsTPParent->has_adj_darts869)),
                &(myTP->TP871Ptr[idx]));
#endif
        } else {
            if (myTP->TP871Ptr[idx] != nullptr) {
                myTP->TP871Ptr[idx]->dispatchCodelet(this->getID());
            } else {
                this->resetCodelet();
                this->decDep();
            }
        }
    }
}
void TP869::_barrierCodelets871::fire(void)
{
    TP869* myTP = static_cast<TP869*>(myTP_);
    {
        for (size_t codeletsCounter = 0; codeletsCounter < myTP->numThreads; codeletsCounter++) {
            myTP->checkInCodelets902[codeletsCounter].decDep();
        }
    }
}
void TP869::_checkInCodelets902::fire(void)
{
    /*printOMPForCodeletFireCode, inlined: 0*/
    /*region 902 0*/
    /*Determine the TP to which this codelet belongs and check if this codelet spawns the TP or if
     * it signals it using dispatchCodelet()*/
    size_t idx = this->getID() / myTP->codeletsPerTP902;
    if (idx < myTP->TPsToUse902) {
        if (!__sync_val_compare_and_swap(&(myTP->TP902_alreadyLaunched[idx]), 0, 1)) {
            int64_t range = abs(nedge - 0) / 1;
            int64_t rangePerCodelet = range / myTP->TPsToUse902;
            int64_t minIteration = min<int64_t>(nedge, 0);
            int64_t remainderRange = range % myTP->TPsToUse902;
            int64_t initIteration = rangePerCodelet * idx;
            int64_t lastIteration = rangePerCodelet * (idx + 1);
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
            if (0 < nedge) {
                initIteration = min(initIteration, lastIteration);
                lastIteration = max(initIteration, lastIteration);
            } else {
                initIteration = max(initIteration, lastIteration);
                lastIteration = min(initIteration, lastIteration);
            }
            if (idx == myTP->TPsToUse902 - 1) {
                lastIteration = nedge;
            }
#if USEINVOKE == 1
            invoke<TP902>(myTP, myTP->codeletsPerTP902 * DARTS_CODELETS_MULT, this->getID(), myTP,
                initIteration, lastIteration, &(*(this->inputsTPParent->has_adj_darts869)),
                &(myTP->TP902Ptr[idx]));
#else
            place<TP902>(idx, myTP, myTP->codeletsPerTP902 * DARTS_CODELETS_MULT, this->getID(),
                myTP, initIteration, lastIteration, &(*(this->inputsTPParent->has_adj_darts869)),
                &(myTP->TP902Ptr[idx]));
#endif
        } else {
            if (myTP->TP902Ptr[idx] != nullptr) {
                myTP->TP902Ptr[idx]->dispatchCodelet(this->getID());
            } else {
                this->resetCodelet();
                this->decDep();
            }
        }
    }
}
void TP869::_barrierCodelets902::fire(void)
{
    TP869* myTP = static_cast<TP869*>(myTP_);
    myTP->TPParent->barrierCodelets869[0].setDep(0);
    myTP->add(&(myTP->TPParent->barrierCodelets869[0]));
}
TP869::TP869(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet,
    int* __restrict* in_has_adj, int64_t* in_k)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , has_adj_darts869(in_has_adj) /*OMP_SHARED - INPUT*/
    , k_darts869(in_k) /*OMP_SHARED - INPUT*/
    , TP871Ptr(new TP871*[NUMTPS871])
    , TP871_alreadyLaunched(new size_t[NUMTPS871])
    , numTPsSet871(0)
    , numTPsReady871(0)
    , TPsToUse871(NUMTPS871)
    , codeletsPerTP871(this->numThreads / NUMTPS871)
    , totalCodelets871(this->TPsToUse871 * this->codeletsPerTP871)
    , TP902Ptr(new TP902*[NUMTPS902])
    , TP902_alreadyLaunched(new size_t[NUMTPS902])
    , numTPsSet902(0)
    , numTPsReady902(0)
    , TPsToUse902(NUMTPS902)
    , codeletsPerTP902(this->numThreads / NUMTPS902)
    , totalCodelets902(this->TPsToUse902 * this->codeletsPerTP902)
    , barrierCodelets869(new _barrierCodelets869[1])
    , checkInCodelets871(new _checkInCodelets871[this->numThreads])
    , barrierCodelets871(new _barrierCodelets871[1])
    , checkInCodelets902(new _checkInCodelets902[this->numThreads])
    , barrierCodelets902(new _barrierCodelets902[1])
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets869[0] = _barrierCodelets869(ompNumThreads, ompNumThreads, this, 0);
    barrierCodelets902[0] = _barrierCodelets902(NUMTPS902, NUMTPS902, this, 0);
    barrierCodelets871[0] = _barrierCodelets871(NUMTPS871, NUMTPS871, this, 0);
    _checkInCodelets902* checkInCodelets902Ptr = (this->checkInCodelets902);
    for (int i = 0; i < NUMTPS902; i++) {
        TP902Ptr[i] = nullptr;
        TP902_alreadyLaunched[i] = 0;
    }
    _checkInCodelets871* checkInCodelets871Ptr = (this->checkInCodelets871);
    for (int i = 0; i < NUMTPS871; i++) {
        TP871Ptr[i] = nullptr;
        TP871_alreadyLaunched[i] = 0;
    }
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets902Ptr) = _checkInCodelets902(1, 1, this, codeletCounter);
        checkInCodelets902Ptr++;
        (*checkInCodelets871Ptr) = _checkInCodelets871(1, 1, this, codeletCounter);
        (*checkInCodelets871Ptr).decDep();
        checkInCodelets871Ptr++;
    }
}
TP869::~TP869()
{
    delete[] barrierCodelets869;
    delete[] barrierCodelets902;
    delete[] checkInCodelets902;
    delete[] barrierCodelets871;
    delete[] checkInCodelets871;
}
/*TP871: OMPForDirective*/
void TP871::_barrierCodelets871::fire(void)
{
    TP871* myTP = static_cast<TP871*>(myTP_);
    myTP->controlTPParent->TPParent->barrierCodelets871[0].decDep();
}
bool TP871::requestNewRangeIterations871(int64_t* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Static */
    /*Chunk = 0*/
    bool isThereNewIteration = false;
    {
        /*Static Scheduling*/
        int64_t tempStartRange = rangePerCodelet871 * codeletID;
        int64_t tempEndRange = rangePerCodelet871 * (codeletID + 1);
        if (remainderRange871 != 0) {
            if (codeletID < (uint32_t)remainderRange871) {
                tempStartRange += codeletID;
                tempEndRange += (codeletID + 1);
            } else {
                tempStartRange += remainderRange871;
                tempEndRange += remainderRange871;
            }
        }
        tempStartRange = tempStartRange * 1 + minIteration871;
        tempEndRange = tempEndRange * 1 + minIteration871;
        if (tempStartRange != tempEndRange) {
            isThereNewIteration = true;
        }
        if (initIteration871 < lastIteration871) {
            (this->inputsTPParent->k_darts871[codeletID]) = min(tempStartRange, tempEndRange);
            *endRange = max(tempStartRange, tempEndRange);
        } else {
            (this->inputsTPParent->k_darts871[codeletID]) = max(tempStartRange, tempEndRange);
            *endRange = min(tempStartRange, tempEndRange);
        }
        if (codeletID == this->numThreads - 1) {
            *endRange = lastIteration871;
        }
    }
    return isThereNewIteration;
}
void TP871::_checkInCodelets872::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    /*Init the vars for this region*/

    /*printing node 872: ForStmt*/
    /*var: has_adj*/
    /*var: k*/
    int* __restrict* has_adj = (this->inputsTPParent->has_adj_darts871);
    (void)has_adj /*OMP_SHARED*/;
    int64_t* k = &(this->inputsTPParent->k_darts871[this->getLocalID()]);
    (void)k /*OMP_PRIVATE*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations871(
        (int64_t*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        /*Signaling omp for stmt's barrier*/
        myTP->controlTPParent->barrierCodelets871[0].decDep();
        return;
    }
    for (int64_t k_darts_counter_temp871 = (*k); k_darts_counter_temp871 < endRange
         && k_darts_counter_temp871 < this->inputsTPParent->lastIteration871;
         ++k_darts_counter_temp871) {
        {
            (*(has_adj))[(k_darts_counter_temp871)] = 0;
        }
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling omp for stmt's barrier*/
    myTP->controlTPParent->barrierCodelets871[0].decDep();
}
TP871::TP871(int in_numThreads, int in_mainCodeletID, TP869* in_TPParent, int64_t in_initIteration,
    int64_t in_lastIteration, int* __restrict* in_has_adj, TP871** in_ptrToThisTP)
    : ompTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , has_adj_darts871(in_has_adj) /*OMP_SHARED - INPUT*/
    , k_darts871(new int64_t[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , initIteration871(in_initIteration)
    , lastIteration871(in_lastIteration)
    , readyCodelets(0)
    , baseNumThreads(this->numThreads / DARTS_CODELETS_MULT)
    , barrierCodelets871(new _barrierCodelets871[1])
    , checkInCodelets872(new _checkInCodelets872[this->numThreads])
{
    /*Initialize the loop parameters*/
    range871 = abs(lastIteration871 - initIteration871) / 1;
    rangePerCodelet871 = range871 / numThreads;
    minIteration871 = min<int64_t>(lastIteration871, initIteration871);
    remainderRange871 = range871 % numThreads;
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets871[0] = _barrierCodelets871(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets872* checkInCodelets872Ptr = (this->checkInCodelets872);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets872);
#endif
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->baseNumThreads;
         codeletCounter++) {
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets872Ptr) = _checkInCodelets872(2, 1, this, codeletCounter);
#else
        (*checkInCodelets872Ptr) = _checkInCodelets872(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets872Ptr).decDep();
        checkInCodelets872Ptr++;
    }
    this->dispatchCodelet(this->getID());
    *(in_ptrToThisTP) = this;
}
void TP871::dispatchCodelet(size_t codeletID)
{
    int idx = codeletID / this->baseNumThreads;
    int localID = codeletID - this->baseNumThreads * idx;
    this->checkInCodelets872[localID].setID(codeletID);
    this->checkInCodelets872[localID].setLocalID(localID);
    /*Check if we want to replicate codelets*/
    for (size_t i = 1; i < (size_t)DARTS_CODELETS_MULT; i++) {
#if USE_SPIN_CODELETS == 0
        this->checkInCodelets872[localID + this->baseNumThreads * i]
            = _checkInCodelets872(2, 1, this, localID + this->baseNumThreads * i);
#else
        this->checkInCodelets872[localID + this->baseNumThreads * i]
            = _checkInCodelets872(1, 1, this, localID + this->baseNumThreads * i);
#endif
        this->checkInCodelets872[localID + this->baseNumThreads * i].setID(codeletID);
        this->checkInCodelets872[localID + this->baseNumThreads * i].decDep();
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
TP871::~TP871()
{
    delete[] barrierCodelets871;
    delete[] checkInCodelets872;
}
/*TP902: OMPForDirective*/
void TP902::_barrierCodelets902::fire(void)
{
    TP902* myTP = static_cast<TP902*>(myTP_);
    myTP->controlTPParent->TPParent->barrierCodelets902[0].decDep();
}
bool TP902::requestNewRangeIterations902(int64_t* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Static */
    /*Chunk = 0*/
    bool isThereNewIteration = false;
    {
        /*Static Scheduling*/
        int64_t tempStartRange = rangePerCodelet902 * codeletID;
        int64_t tempEndRange = rangePerCodelet902 * (codeletID + 1);
        if (remainderRange902 != 0) {
            if (codeletID < (uint32_t)remainderRange902) {
                tempStartRange += codeletID;
                tempEndRange += (codeletID + 1);
            } else {
                tempStartRange += remainderRange902;
                tempEndRange += remainderRange902;
            }
        }
        tempStartRange = tempStartRange * 1 + minIteration902;
        tempEndRange = tempEndRange * 1 + minIteration902;
        if (tempStartRange != tempEndRange) {
            isThereNewIteration = true;
        }
        if (initIteration902 < lastIteration902) {
            (this->inputsTPParent->k_darts902[codeletID]) = min(tempStartRange, tempEndRange);
            *endRange = max(tempStartRange, tempEndRange);
        } else {
            (this->inputsTPParent->k_darts902[codeletID]) = max(tempStartRange, tempEndRange);
            *endRange = min(tempStartRange, tempEndRange);
        }
        if (codeletID == this->numThreads - 1) {
            *endRange = lastIteration902;
        }
    }
    return isThereNewIteration;
}
void TP902::_checkInCodelets903::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    /*Init the vars for this region*/

    /*printing node 903: ForStmt*/
    /*var: has_adj*/
    /*var: k*/
    int* __restrict* has_adj = (this->inputsTPParent->has_adj_darts902);
    (void)has_adj /*OMP_SHARED*/;
    int64_t* k = &(this->inputsTPParent->k_darts902[this->getLocalID()]);
    (void)k /*OMP_PRIVATE*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations902(
        (int64_t*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        /*Signaling omp for stmt's barrier*/
        myTP->controlTPParent->barrierCodelets902[0].decDep();
        return;
    }
    for (int64_t k_darts_counter_temp902 = (*k); k_darts_counter_temp902 < endRange
         && k_darts_counter_temp902 < this->inputsTPParent->lastIteration902;
         ++k_darts_counter_temp902) {
        {
            int64_t i = get_v0_from_edge(&IJ[(k_darts_counter_temp902)]);
            int64_t j = get_v1_from_edge(&IJ[(k_darts_counter_temp902)]);
            if (i != j) {
                (*(has_adj))[i] = (*(has_adj))[j] = 1;
            }
        }
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling omp for stmt's barrier*/
    myTP->controlTPParent->barrierCodelets902[0].decDep();
}
TP902::TP902(int in_numThreads, int in_mainCodeletID, TP869* in_TPParent, int64_t in_initIteration,
    int64_t in_lastIteration, int* __restrict* in_has_adj, TP902** in_ptrToThisTP)
    : ompTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , has_adj_darts902(in_has_adj) /*OMP_SHARED - INPUT*/
    , k_darts902(new int64_t[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , initIteration902(in_initIteration)
    , lastIteration902(in_lastIteration)
    , readyCodelets(0)
    , baseNumThreads(this->numThreads / DARTS_CODELETS_MULT)
    , barrierCodelets902(new _barrierCodelets902[1])
    , checkInCodelets903(new _checkInCodelets903[this->numThreads])
{
    /*Initialize the loop parameters*/
    range902 = abs(lastIteration902 - initIteration902) / 1;
    rangePerCodelet902 = range902 / numThreads;
    minIteration902 = min<int64_t>(lastIteration902, initIteration902);
    remainderRange902 = range902 % numThreads;
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets902[0] = _barrierCodelets902(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets903* checkInCodelets903Ptr = (this->checkInCodelets903);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets903);
#endif
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->baseNumThreads;
         codeletCounter++) {
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets903Ptr) = _checkInCodelets903(2, 1, this, codeletCounter);
#else
        (*checkInCodelets903Ptr) = _checkInCodelets903(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets903Ptr).decDep();
        checkInCodelets903Ptr++;
    }
    this->dispatchCodelet(this->getID());
    *(in_ptrToThisTP) = this;
}
void TP902::dispatchCodelet(size_t codeletID)
{
    int idx = codeletID / this->baseNumThreads;
    int localID = codeletID - this->baseNumThreads * idx;
    this->checkInCodelets903[localID].setID(codeletID);
    this->checkInCodelets903[localID].setLocalID(localID);
    /*Check if we want to replicate codelets*/
    for (size_t i = 1; i < (size_t)DARTS_CODELETS_MULT; i++) {
#if USE_SPIN_CODELETS == 0
        this->checkInCodelets903[localID + this->baseNumThreads * i]
            = _checkInCodelets903(2, 1, this, localID + this->baseNumThreads * i);
#else
        this->checkInCodelets903[localID + this->baseNumThreads * i]
            = _checkInCodelets903(1, 1, this, localID + this->baseNumThreads * i);
#endif
        this->checkInCodelets903[localID + this->baseNumThreads * i].setID(codeletID);
        this->checkInCodelets903[localID + this->baseNumThreads * i].decDep();
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
TP902::~TP902()
{
    delete[] barrierCodelets902;
    delete[] checkInCodelets903;
}
