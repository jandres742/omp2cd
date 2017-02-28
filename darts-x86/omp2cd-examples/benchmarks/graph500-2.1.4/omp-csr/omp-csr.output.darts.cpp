#include "omp-csr.output.darts.h"
using namespace darts;
using namespace std;
long countFor;
long countOMPFor = 0;
long countOuterWhile = 0;
long countWhile = 0;
struct timeval endTimeFor;
struct timeval endTimeOMPFor;
struct timeval endTimeOuterWhile;
struct timeval endTimeTotal;
struct timeval endTimeWhile;
static int64_t maxvtx;
static int64_t nv;
struct timeval startTimeFor;
struct timeval startTimeOMPFor;
struct timeval startTimeOuterWhile;
struct timeval startTimeTotal;
struct timeval startTimeWhile;
static int64_t sz;
long timeFor;
long timeOMPFor = 0;
long timeOuterWhile = 0;
long timeWhile = 0;
static int64_t* __restrict xadj;
static int64_t* __restrict xadjstore;
static int64_t* __restrict xoff;
static int int64_cas(int64_t* p, int64_t oldval, int64_t newval);
static int64_t int64_casval(int64_t* p, int64_t oldval, int64_t newval);
static int alloc_graph(int64_t nedge);
static int64_t int64_fetch_add(int64_t* p, int64_t incr);
static void free_graph();
static int setup_deg_off(const struct packed_edge* restrict IJ, int64_t nedge);
static void scatter_edge(const int64_t i, const int64_t j);
static void find_nv(const struct packed_edge* restrict IJ, const int64_t nedge);
static void pack_vtx_edges(const int64_t i);
static int i64cmp(const void* a, const void* b);
static void gather_edges(const struct packed_edge* restrict IJ, int64_t nedge);
/*Function: find_nv, ID: 12*/
static void find_nv(const struct packed_edge* restrict IJ, const int64_t nedge)
{
    /*find_nv:12*/
    /*CompoundStmt:57*/
    maxvtx = -1;
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP60>(1, 0, RuntimeFinalCodelet,
            (struct packed_edge * __restrict*)&((IJ)), (int64_t*)&((nedge))));
    }
    nv = 1 + maxvtx;
}
/*Function: alloc_graph, ID: 13*/
static int alloc_graph(int64_t nedge)
{
    /*alloc_graph:13*/
    /*CompoundStmt:116*/
    sz = (2 * nv + 2) * sizeof(*xoff);
    xoff = (int64_t*)xmalloc_large_ext(sz);
    if (!xoff)
        return -1;
    return 0;
}
/*Function: free_graph, ID: 14*/
static void free_graph()
{
    /*free_graph:14*/
    /*CompoundStmt:132*/
    xfree_large(xadjstore);
    xfree_large(xoff);
}
/*Function: setup_deg_off, ID: 16*/
static int setup_deg_off(const struct packed_edge* restrict IJ, int64_t nedge)
{
    /*setup_deg_off:16*/
    /*CompoundStmt:203*/
    int err = 0;
    int64_t* buf = (int64_t*)((void*)0);
    xadj = (int64_t*)((void*)0);
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP211>(ompNumThreads * DARTS_CODELETS_MULT, 0,
            RuntimeFinalCodelet, (struct packed_edge * __restrict*)&((IJ)), (int64_t**)&((buf)),
            (int*)&((err)), (int64_t*)&((nedge))));
    }
    free(buf);
    return !xadj;
}
/*Function: scatter_edge, ID: 17*/
static void scatter_edge(const int64_t i, const int64_t j)
{
    /*scatter_edge:17*/
    /*CompoundStmt:427*/
    int64_t where;
    where = int64_fetch_add(&(xoff[1 + 2 * (i)]), 1);
    xadj[where] = j;
}
/*Function: i64cmp, ID: 18*/
static int i64cmp(const void* a, const void* b)
{
    /*i64cmp:18*/
    /*CompoundStmt:437*/
    const int64_t ia = *(const int64_t*)a;
    const int64_t ib = *(const int64_t*)b;
    if (ia < ib)
        return -1;
    if (ia > ib)
        return 1;
    return 0;
}
/*Function: pack_vtx_edges, ID: 19*/
static void pack_vtx_edges(const int64_t i)
{
    /*pack_vtx_edges:19*/
    /*CompoundStmt:454*/
    int64_t kcur, k;
    if ((xoff[2 * (i)]) + 1 >= (xoff[1 + 2 * (i)]))
        return;
    qsort(&xadj[(xoff[2 * (i)])], (xoff[1 + 2 * (i)]) - (xoff[2 * (i)]), sizeof(*xadj), i64cmp);
    kcur = (xoff[2 * (i)]);
    for (k = (xoff[2 * (i)]) + 1; k < (xoff[1 + 2 * (i)]); ++k)
        if (xadj[k] != xadj[kcur])
            xadj[++kcur] = xadj[k];
    ++kcur;
    for (k = kcur; k < (xoff[1 + 2 * (i)]); ++k)
        xadj[k] = -1;
    (xoff[1 + 2 * (i)]) = kcur;
}
/*Function: gather_edges, ID: 21*/
static void gather_edges(const struct packed_edge* restrict IJ, int64_t nedge)
{
    /*gather_edges:21*/
    /*CompoundStmt:540*/
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP541>(ompNumThreads * DARTS_CODELETS_MULT, 0,
            RuntimeFinalCodelet, (struct packed_edge * __restrict*)&((IJ)), (int64_t*)&((nedge))));
    }
}
/*Function: create_graph_from_edgelist, ID: 22*/
int create_graph_from_edgelist(struct packed_edge* IJ, int64_t nedge)
{
    /*create_graph_from_edgelist:22*/
    /*CompoundStmt:588*/
    find_nv(IJ, nedge);
    if (alloc_graph(nedge))
        return -1;
    int res;
    res = setup_deg_off(IJ, nedge);
    if (res) {
        xfree_large(xoff);
        return -1;
    }
    gather_edges(IJ, nedge);
    return 0;
}
/*Function: make_bfs_tree, ID: 23*/
int make_bfs_tree(int64_t* bfs_tree_out, int64_t* max_vtx_out, int64_t srcvtx)
{
    /*make_bfs_tree:23*/
    /*CompoundStmt:606*/
    int64_t* restrict bfs_tree = bfs_tree_out;
    int err = 0;
    int64_t* restrict vlist = (int64_t*)((void*)0);
    int64_t k1, k2;
    *max_vtx_out = maxvtx;
    vlist = (int64_t*)xmalloc_large(nv * sizeof(*vlist));
    if (!vlist)
        return -1;
    for (int64_t k = 0; k < nv; ++k)
        bfs_tree[k] = -1;
    vlist[0] = srcvtx;
    k1 = 0;
    k2 = 1;
    bfs_tree[srcvtx] = srcvtx;
    int64_t k;
    while (k1 != k2) {
        /*CompoundStmt:642*/
        const int64_t oldk2 = k2;
        if (affinMaskRes) {
            myDARTSRuntime->run(launch<TP644>(1, 0, RuntimeFinalCodelet,
                (int64_t * __restrict*)&((bfs_tree)), (int64_t*)&((k)), (int64_t*)&((k1)),
                (int64_t*)&((k2)), (int64_t*)&((oldk2)), (int64_t * __restrict*)&((vlist))));
        }
        k1 = oldk2;
    }
    xfree_large(vlist);
    return err;
}
/*Function: destroy_graph, ID: 24*/
void destroy_graph()
{
    /*destroy_graph:24*/
    /*CompoundStmt:738*/
    free_graph();
}
/*Function: int64_fetch_add, ID: 9*/
int64_t int64_fetch_add(int64_t* p, int64_t incr)
{
    /*int64_fetch_add:9*/
    /*CompoundStmt:740*/
    return __sync_fetch_and_add_8(p, incr);
}
/*Function: int64_casval, ID: 10*/
int64_t int64_casval(int64_t* p, int64_t oldval, int64_t newval)
{
    /*int64_casval:10*/
    /*CompoundStmt:745*/
    return __sync_val_compare_and_swap_8(p, oldval, newval);
}
/*Function: int64_cas, ID: 11*/
int int64_cas(int64_t* p, int64_t oldval, int64_t newval)
{
    /*int64_cas:11*/
    /*CompoundStmt:750*/
    return __sync_bool_compare_and_swap_8(p, oldval, newval);
}
/*TP60: OMPParallelDirective*/
void TP60::_barrierCodelets60::fire(void)
{
    TP60* myTP = static_cast<TP60*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
void TP60::_checkInCodelets62::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 62: DeclStmt*/
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 62 nextRegion: 64 */
    myTP->controlTPParent->checkInCodelets64[this->getID()].decDep();
}
void TP60::_checkInCodelets64_core::fire(void)
{
    TP60* myTP = static_cast<TP60*>(myTP_);
    (void)myTP;
    int64_t startRange = (this->initIteration);
    int64_t endRange = (this->lastIteration);
    /*var: IJ*/
    /*var: k*/
    /*var: nedge*/
    /*var: tmaxvtx*/
    int64_t k, gmaxvtx, tmaxvtx = -1;
    {
        for (int64_t k = startRange; k < endRange; ++k) {
            {
                if (get_v0_from_edge(&(*(IJ))[k]) > tmaxvtx)
                    tmaxvtx = get_v0_from_edge(&(*(IJ))[k]);
                if (get_v1_from_edge(&(*(IJ))[k]) > tmaxvtx)
                    tmaxvtx = get_v1_from_edge(&(*(IJ))[k]);
            }
        }
    }
    gmaxvtx = maxvtx;
    while (tmaxvtx > gmaxvtx)
        gmaxvtx = int64_casval(&maxvtx, gmaxvtx, tmaxvtx);
    this->toSignal->decDep();
}
void TP60::_checkInCodelets64::fire(void)
{
    /*printOMPForCodeletFireCode, inlined: 0*/
    /*region 64 0*/
    int64_t range = abs((*(this->inputsTPParent->nedge_darts60)) - 0);
    unsigned int workers = (ompNumThreads / NUMTPS - 1) + 1;
    if (range < dartsTPLoopTH0) {
        _checkInCodelets64_core* tempCodeletCore = new _checkInCodelets64_core(1, 1, myTP,
            &(myTP->barrierCodelets64[0]), 0, (*(this->inputsTPParent->nedge_darts60)),
            &(*(this->inputsTPParent->IJ_darts60)), &(*(this->inputsTPParent->nedge_darts60)));
        myTP->masterCodeletTP64_core = tempCodeletCore;
        myTP->masterCodeletTP64_core[0].decDep();
    } else if (range < dartsTPLoopTH1) {
        uint64_t outerTile = (range / workers) ? (range / workers) : range;
        codeletFor<TP64_loop>* tempCodeletFor
            = new codeletFor<TP64_loop>(1, 1, myTP, LONGWAIT, &(myTP->barrierCodelets64[0]), 1,
                outerTile, 0, (*(this->inputsTPParent->nedge_darts60)),
                &(*(this->inputsTPParent->IJ_darts60)), &(*(this->inputsTPParent->nedge_darts60)));
        tempCodeletFor->setIterations((range / outerTile + ((range % outerTile) ? 1 : 0)));
        myTP->masterCodeletTP64_loop = tempCodeletFor;
        myTP->masterCodeletTP64_loop[0].decDep();
    } else {
        uint64_t outerTile = (range / (NUMTPS * 2));
        if (outerTile == 0) {
            outerTile = (range / NUMTPS);
            if (outerTile == 0)
                outerTile = range;
        }
        paraFor<TP64_outer>* tempParaFor
            = new paraFor<TP64_outer>(1, 1, myTP, LONGWAIT, &(myTP->barrierCodelets64[0]), 1,
                outerTile, workers, 0, (*(this->inputsTPParent->nedge_darts60)),
                &(*(this->inputsTPParent->IJ_darts60)), &(*(this->inputsTPParent->nedge_darts60)));
        tempParaFor->setIterations((range / outerTile + ((range % outerTile) ? 1 : 0)));
        myTP->masterCodeletTP64_outer = tempParaFor;
        myTP->masterCodeletTP64_outer[0].decDep();
    }
}
void TP60::_barrierCodelets64::fire(void)
{
    TP60* myTP = static_cast<TP60*>(myTP_);
    {
        for (size_t codeletsCounter = 0; codeletsCounter < myTP->numThreads; codeletsCounter++) {
            myTP->checkInCodelets108[codeletsCounter].decDep();
        }
    }
}
void TP60::_checkInCodelets108::fire(void)
{

    /*printing node 108: BinaryOperator*/

    /*printing node 109: WhileStmt*/
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets60[0].decDep();
}
TP60::TP60(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet,
    struct packed_edge* __restrict* in_IJ, int64_t* in_nedge)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , IJ_darts60(in_IJ) /*OMP_SHARED - INPUT*/
    , nedge_darts60(in_nedge) /*OMP_SHARED - INPUT*/
    , TP64Ptr(new TP64*[NUMTPS64])
    , TP64_alreadyLaunched(new size_t[NUMTPS64])
    , numTPsSet64(0)
    , numTPsReady64(0)
    , TPsToUse64(NUMTPS64)
    , codeletsPerTP64(this->numThreads / NUMTPS64)
    , totalCodelets64(this->TPsToUse64 * this->codeletsPerTP64)
    , barrierCodelets60(new _barrierCodelets60[1])
    , checkInCodelets62(new _checkInCodelets62[this->numThreads])
    , checkInCodelets64(new _checkInCodelets64[this->numThreads])
    , masterCodeletTP64_core(new _checkInCodelets64_core[1])
    , masterCodeletTP64_loop(new codeletFor<TP64_loop>[1])
    , masterCodeletTP64_outer(new paraFor<TP64_outer>[1])
    , barrierCodelets64(new _barrierCodelets64[1])
    , checkInCodelets108(new _checkInCodelets108[this->numThreads])
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets60[0] = _barrierCodelets60(1, 1, this, 0);
    barrierCodelets64[0] = _barrierCodelets64(1, 1, this, 0);
    _checkInCodelets108* checkInCodelets108Ptr = (this->checkInCodelets108);
    _checkInCodelets64* checkInCodelets64Ptr = (this->checkInCodelets64);
    _checkInCodelets62* checkInCodelets62Ptr = (this->checkInCodelets62);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets108Ptr) = _checkInCodelets108(1, 1, this, codeletCounter);
        checkInCodelets108Ptr++;
        (*checkInCodelets64Ptr) = _checkInCodelets64(1, 1, this, codeletCounter);
        checkInCodelets64Ptr++;
        (*checkInCodelets62Ptr) = _checkInCodelets62(1, 1, this, codeletCounter);
        (*checkInCodelets62Ptr).decDep();
        checkInCodelets62Ptr++;
    }
}
TP60::~TP60()
{
    delete[] barrierCodelets60;
    delete[] checkInCodelets108;
    delete[] barrierCodelets64;
    delete[] checkInCodelets64;
    delete[] checkInCodelets62;
}
/*TP64: OMPForDirective*/
void TP64_loop::_checkInCodelets65::fire(void)
{
    TP64_loop* myTP = static_cast<TP64_loop*>(myTP_);
    (void)myTP;
    int64_t startRange = (myTP->iter) * (myTP->tileSize) + (myTP->initIteration);
    int64_t endRange
        = min((int64_t)(startRange + (myTP->tileSize)), (int64_t)(myTP->lastIteration));
    /*var: IJ*/
    /*var: k*/
    /*var: nedge*/
    /*var: tmaxvtx*/
    int64_t k, gmaxvtx, tmaxvtx = -1;
    {
        for (int64_t k = startRange; k < endRange; ++k) {
            {
                if (get_v0_from_edge(&(*(IJ))[k]) > tmaxvtx)
                    tmaxvtx = get_v0_from_edge(&(*(IJ))[k]);
                if (get_v1_from_edge(&(*(IJ))[k]) > tmaxvtx)
                    tmaxvtx = get_v1_from_edge(&(*(IJ))[k]);
            }
        }
    }
    gmaxvtx = maxvtx;
    while (tmaxvtx > gmaxvtx)
        gmaxvtx = int64_casval(&maxvtx, gmaxvtx, tmaxvtx);
    myTP->toSignal->decDep();
}
void TP64_outer::TP64_inner::_checkInCodelets65::fire(void)
{
    TP64_outer::TP64_inner* myTP = static_cast<TP64_outer::TP64_inner*>(myTP_);
    (void)myTP;
    int64_t startRange = myTP->outerIteration * myTP->outerTile
        + myTP->innerIteration * myTP->innerTile + myTP->initIteration;
    int64_t endRange = min(min(startRange + myTP->innerTile, myTP->lastIteration),
        myTP->initIteration + (myTP->outerIteration + 1) * myTP->outerTile);
    /*var: IJ*/
    /*var: k*/
    /*var: nedge*/
    /*var: tmaxvtx*/
    int64_t k, gmaxvtx, tmaxvtx = -1;
    {
        for (int64_t k = startRange; k < endRange; ++k) {
            {
                if (get_v0_from_edge(&(*(IJ))[k]) > tmaxvtx)
                    tmaxvtx = get_v0_from_edge(&(*(IJ))[k]);
                if (get_v1_from_edge(&(*(IJ))[k]) > tmaxvtx)
                    tmaxvtx = get_v1_from_edge(&(*(IJ))[k]);
            }
        }
    }
    gmaxvtx = maxvtx;
    while (tmaxvtx > gmaxvtx)
        gmaxvtx = int64_casval(&maxvtx, gmaxvtx, tmaxvtx);
    myTP->toSignal->decDep();
}
TP64_loop::TP64_loop(unsigned int it, darts::Codelet* in_nextCodelet, unsigned int in_tileSize,
    int64_t in_initIteration, int64_t in_lastIteration, struct packed_edge* __restrict* in_IJ,
    int64_t* in_nedge)
    : loop(it, in_nextCodelet)
    , tileSize(in_tileSize)
    , initIteration(in_initIteration)
    , lastIteration(in_lastIteration)
    , checkInCodelets65(new _checkInCodelets65[1])
{
    this->checkInCodelets65[0] = _checkInCodelets65(1, 1, this, in_IJ, in_nedge);
    this->checkInCodelets65[0].decDep();
}
TP64_outer::TP64_inner::TP64_inner(unsigned int it, darts::Codelet* in_nextCodelet,
    unsigned int in_outerIteration, int64_t in_outerTile, int64_t in_innerTile,
    int64_t in_initIteration, int64_t in_lastIteration, struct packed_edge* __restrict* in_IJ,
    int64_t* in_nedge)
    : loop(it, in_nextCodelet)
    , outerIteration(in_outerIteration)
    , innerIteration(it)
    , outerTile(in_outerTile)
    , innerTile(in_innerTile)
    , initIteration(in_initIteration)
    , lastIteration(in_lastIteration)
    , checkInCodelets65(new _checkInCodelets65[1])
{
    this->checkInCodelets65[0] = _checkInCodelets65(1, 1, this, in_IJ, in_nedge);
    this->checkInCodelets65[0].decDep();
}
TP64_outer::TP64_outer(unsigned int it, darts::Codelet* in_nextCodelet, int64_t in_outerTile,
    unsigned int in_workers, int64_t in_initIteration, int64_t in_lastIteration,
    struct packed_edge* __restrict* in_IJ, int64_t* in_nedge)
    : loop(it, in_nextCodelet)
    , outerTile(in_outerTile)
    , workers(in_workers)
    , initIteration(in_initIteration)
    , lastIteration(in_lastIteration)
    , masterCodeletTP64_inner(new codeletFor<TP64_inner>[1])
{
    this->width = min(((this->initIteration) + it * (this->outerTile) + (this->outerTile)),
                      this->lastIteration)
        - (it * (this->outerTile) + this->initIteration);
    if (this->width / (this->workers * 2))
        this->innerTile = (this->width / (this->workers * 2));
    else if (this->width / this->workers)
        this->innerTile = (this->width / this->workers);
    else
        this->innerTile = this->width;
    uint64_t outerIteration
        = (this->width / this->innerTile) + ((this->width % this->innerTile) ? 1 : 0);
    codeletFor<TP64_outer::TP64_inner>* tempCodeletFor = new codeletFor<TP64_outer::TP64_inner>(1,
        1, this, LONGWAIT, in_nextCodelet, outerIteration, it, this->outerTile, this->innerTile,
        this->initIteration, this->lastIteration, in_IJ, in_nedge);
    this->masterCodeletTP64_inner = tempCodeletFor;
    this->masterCodeletTP64_inner[0].decDep();
}
TP64_loop::~TP64_loop() { delete[] checkInCodelets65; }
TP64_outer::TP64_inner::~TP64_inner() { delete[] checkInCodelets65; }
TP64_outer::~TP64_outer() { delete masterCodeletTP64_inner; }
/*TP15: TP_prefix_sum*/
void TP15::_checkInCodelets136::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    /*Init the vars for this region*/

    /*printing node 136: DeclStmt*/

    /*printing node 137: DeclStmt*/

    /*printing node 138: BinaryOperator*/
    (this->inputsTPParent->nt_darts15[this->getID()]) = omp_get_num_threads();

    /*printing node 140: BinaryOperator*/
    (this->inputsTPParent->tid_darts15[this->getID()]) = omp_get_thread_num();

    /*printing node 142: BinaryOperator*/
    (this->inputsTPParent->t1_darts15[this->getID()])
        = nv / (this->inputsTPParent->nt_darts15[this->getID()]);

    /*printing node 144: BinaryOperator*/
    (this->inputsTPParent->t2_darts15[this->getID()])
        = nv % (this->inputsTPParent->nt_darts15[this->getID()]);

    /*printing node 146: BinaryOperator*/
    (this->inputsTPParent->slice_begin_darts15[this->getID()])
        = (this->inputsTPParent->t1_darts15[this->getID()])
            * (this->inputsTPParent->tid_darts15[this->getID()])
        + ((this->inputsTPParent->tid_darts15[this->getID()])
                      < (this->inputsTPParent->t2_darts15[this->getID()])
                  ? (this->inputsTPParent->tid_darts15[this->getID()])
                  : (this->inputsTPParent->t2_darts15[this->getID()]));

    /*printing node 151: BinaryOperator*/
    (this->inputsTPParent->slice_end_darts15[this->getID()])
        = (this->inputsTPParent->t1_darts15[this->getID()])
            * ((this->inputsTPParent->tid_darts15[this->getID()]) + 1)
        + (((this->inputsTPParent->tid_darts15[this->getID()]) + 1)
                      < (this->inputsTPParent->t2_darts15[this->getID()])
                  ? ((this->inputsTPParent->tid_darts15[this->getID()]) + 1)
                  : (this->inputsTPParent->t2_darts15[this->getID()]));

    /*printing node 159: BinaryOperator*/
    (this->inputsTPParent
            ->buf_darts15[this->getID()])[(this->inputsTPParent->tid_darts15[this->getID()])]
        = 0;

    /*printing node 161: ForStmt*/
    {
        {
            int64_t** buf = &(this->inputsTPParent->buf_darts15[this->getLocalID()]);
            (void)buf /*PRIVATE*/;
            int64_t* k = &(this->inputsTPParent->k_darts15[this->getLocalID()]);
            (void)k /*PRIVATE*/;
            int64_t* slice_end = &(this->inputsTPParent->slice_end_darts15[this->getLocalID()]);
            (void)slice_end /*PRIVATE*/;
            int* tid = &(this->inputsTPParent->tid_darts15[this->getLocalID()]);
            (void)tid /*PRIVATE*/;
            /*Loop's init*/
            (this->inputsTPParent->k_darts15[this->getID()])
                = (this->inputsTPParent->slice_begin_darts15[this->getID()]);
            int64_t k_darts_counter_temp15 = (this->inputsTPParent->k_darts15[this->getID()]);
            for (; (k_darts_counter_temp15) < (*slice_end); ++(k_darts_counter_temp15)) {
                (*buf)[(*tid)] += (xoff[2 * ((k_darts_counter_temp15))]);
            }
            (this->inputsTPParent->k_darts15[this->getID()]) = k_darts_counter_temp15;
        }
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 136 nextRegion: 169 */
    myTP->controlTPParent->barrierCodelets169[0].decDep();
}
void TP15::_barrierCodelets169::fire(void)
{
    TP15* myTP = static_cast<TP15*>(myTP_);
    {
        for (size_t codeletsCounter = 0; codeletsCounter < myTP->numThreads; codeletsCounter++) {
            myTP->checkInCodelets170[codeletsCounter].decDep();
        }
    }
}
void TP15::_checkInCodelets170::fire(void)
{
    /*Select the thread executing OMPSingleDirective 170*/
    if (!__sync_val_compare_and_swap(&(myTP->TP170_alreadyLaunched), 0, 1)) {
        /*Init the vars for this region*/
        /*Initialize the vars of the inlined region*/
        this->inputsTPParent->buf_darts170
            = &(this->inputsTPParent
                    ->buf_darts15[this->getLocalID()]) /*OMP_SHARED_PRIVATE - VAR INLINED*/;
        this->inputsTPParent->nt_darts170
            = &(this->inputsTPParent
                    ->nt_darts15[this->getLocalID()]) /*OMP_SHARED_PRIVATE - VAR INLINED*/;

        /*printing node 171: ForStmt*/
        {
            {
                /*Loop's init*/
                (this->inputsTPParent->k_darts170) = 1;
                int64_t k_darts_counter_temp170 = (this->inputsTPParent->k_darts170);
                for (; k_darts_counter_temp170 < (*(this->inputsTPParent->nt_darts170));
                     ++k_darts_counter_temp170) {
                    (*(this->inputsTPParent->buf_darts170))[k_darts_counter_temp170]
                        += (*(this->inputsTPParent->buf_darts170))[k_darts_counter_temp170 - 1];
                }
                (this->inputsTPParent->k_darts170) = k_darts_counter_temp170;
            }
        }
        /*Signaling next codelet from last stmt in the codelet*/
        /*Signaling omp region's barrier*/
        myTP->controlTPParent->barrierCodelets170[0].decDep();
    } else {
        /*Signaling omp region's barrier*/
        myTP->barrierCodelets170[0].decDep();
    }
}
void TP15::_barrierCodelets170::fire(void)
{
    TP15* myTP = static_cast<TP15*>(myTP_);
    {
        for (size_t codeletsCounter = 0; codeletsCounter < myTP->numThreads; codeletsCounter++) {
            myTP->checkInCodelets179[codeletsCounter].decDep();
        }
    }
}
void TP15::_checkInCodelets179::fire(void)
{

    /*printing node 179: IfStmt*/
    if ((this->inputsTPParent->tid_darts15[this->getID()])) {
        (this->inputsTPParent->t1_darts15[this->getID()])
            = (this->inputsTPParent
                    ->buf_darts15[this->getID()])[(this->inputsTPParent->tid_darts15[this->getID()])
                - 1];
    } else {
        (this->inputsTPParent->t1_darts15[this->getID()]) = 0;
    }

    /*printing node 187: ForStmt*/
    {
        int64_t* k = &(this->inputsTPParent->k_darts15[this->getLocalID()]);
        (void)k /*PRIVATE*/;
        int64_t* slice_end = &(this->inputsTPParent->slice_end_darts15[this->getLocalID()]);
        (void)slice_end /*PRIVATE*/;
        int64_t* t1 = &(this->inputsTPParent->t1_darts15[this->getLocalID()]);
        (void)t1 /*PRIVATE*/;
        int64_t* tmp = &(this->inputsTPParent->tmp_darts15[this->getLocalID()]);
        (void)tmp /*PRIVATE*/;
        /*Loop's init*/
        (this->inputsTPParent->k_darts15[this->getID()])
            = (this->inputsTPParent->slice_begin_darts15[this->getID()]);
        int64_t k_darts_counter_temp15 = (this->inputsTPParent->k_darts15[this->getID()]);
        for (; (k_darts_counter_temp15) < (*slice_end); ++(k_darts_counter_temp15)) {
            *tmp = (xoff[2 * ((k_darts_counter_temp15))]);
            (xoff[2 * ((k_darts_counter_temp15))]) = (*t1);
            (*t1) += (*tmp);
        }
        (this->inputsTPParent->k_darts15[this->getID()]) = k_darts_counter_temp15;
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 179 nextRegion: 199 */
    myTP->controlTPParent->barrierCodelets199[0].decDep();
}
void TP15::_barrierCodelets199::fire(void)
{
    TP15* myTP = static_cast<TP15*>(myTP_);
    {
        for (size_t codeletsCounter = 0; codeletsCounter < myTP->numThreads; codeletsCounter++) {
            myTP->checkInCodelets200[codeletsCounter].decDep();
        }
    }
}
void TP15::_checkInCodelets200::fire(void)
{

    /*printing node 200: ReturnStmt*/
    /*return*/
    *(this->inputsTPParent->functionResult[this->getID()])
        = (this->inputsTPParent
                ->buf_darts15[this->getID()])[(this->inputsTPParent->nt_darts15[this->getID()])
            - 1];
    myTP->controlTPParent->nextCodeletsprefix_sum[this->getID()]->decDep();
    return;
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/

    myTP->controlTPParent->nextCodeletsprefix_sum[this->getID()]->decDep();
}
TP15::TP15(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_mainNextCodelet,
    darts::Codelet* in_mainSyncCodelet, TP15** in_ptrToThisFunctionTP, int64_t* in_buf,
    int64_t* in_functionResult)
    : ompTP(in_numThreads, in_mainCodeletID)
    , ptrToThisFunctionTP(in_ptrToThisFunctionTP)
    , inputsTPParent(this)
    , controlTPParent(this)
    , nextCodeletsprefix_sum(new Codelet*[in_numThreads])
    , nextSyncCodeletsprefix_sum(new Codelet*[in_numThreads])
    , buf_darts15(new int64_t*[this->numThreads])
    , k_darts15(new int64_t[this->numThreads])
    , nt_darts15(new int[this->numThreads])
    , slice_begin_darts15(new int64_t[this->numThreads])
    , slice_end_darts15(new int64_t[this->numThreads])
    , t1_darts15(new int64_t[this->numThreads])
    , t2_darts15(new int64_t[this->numThreads])
    , tid_darts15(new int[this->numThreads])
    , tmp_darts15(new int64_t[this->numThreads])
    , functionResult(new int64_t*[this->numThreads])
    , TP170_alreadyLaunched(0)
    , checkInCodelets136(new _checkInCodelets136[this->numThreads])
    , barrierCodelets169(new _barrierCodelets169[1])
    , checkInCodelets170(new _checkInCodelets170[this->numThreads])
    , barrierCodelets170(new _barrierCodelets170[1])
    , checkInCodelets179(new _checkInCodelets179[this->numThreads])
    , barrierCodelets199(new _barrierCodelets199[1])
    , checkInCodelets200(new _checkInCodelets200[this->numThreads])
{
    barrierCodelets199[0] = _barrierCodelets199(this->numThreads, this->numThreads, this, 0);
    barrierCodelets170[0] = _barrierCodelets170(this->numThreads, this->numThreads, this, 0);
    barrierCodelets169[0] = _barrierCodelets169(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets200* checkInCodelets200Ptr = (this->checkInCodelets200);
    _checkInCodelets179* checkInCodelets179Ptr = (this->checkInCodelets179);
    _checkInCodelets170* checkInCodelets170Ptr = (this->checkInCodelets170);
    _checkInCodelets136* checkInCodelets136Ptr = (this->checkInCodelets136);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets136);
#endif
    for (size_t codeletCounter = 0; codeletCounter < this->numThreads; codeletCounter++) {
        (*checkInCodelets200Ptr) = _checkInCodelets200(1, 1, this, codeletCounter);
        checkInCodelets200Ptr++;
        (*checkInCodelets179Ptr) = _checkInCodelets179(1, 1, this, codeletCounter);
        checkInCodelets179Ptr++;
        (*checkInCodelets170Ptr) = _checkInCodelets170(1, 1, this, codeletCounter);
        checkInCodelets170Ptr++;
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets136Ptr) = _checkInCodelets136(2, 1, this, codeletCounter);
#else
        (*checkInCodelets136Ptr) = _checkInCodelets136(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets136Ptr).decDep();
        checkInCodelets136Ptr++;
    }
    if (this->numThreads == 1) {
        this->nextCodeletsprefix_sum[0] = in_mainNextCodelet;
        this->nextSyncCodeletsprefix_sum[0] = in_mainSyncCodelet;
        this->buf_darts15[0] = in_buf;
        this->functionResult[0] = in_functionResult;
        this->availableCodelets[0] = 1;
    } else {
        this->buf_darts15[this->mainCodeletID] = in_buf;
        this->functionResult[this->mainCodeletID] = in_functionResult;
        this->nextCodeletsprefix_sum[in_mainCodeletID] = in_mainNextCodelet;
        this->nextSyncCodeletsprefix_sum[in_mainCodeletID] = in_mainSyncCodelet;
#if USE_SPIN_CODELETS == 0
        this->firstCodelet[this->mainCodeletID].decDep();
#else
        this->availableCodelets[this->mainCodeletID] = 1;
#endif
        *(this->ptrToThisFunctionTP) = this;
    }
}
TP15::~TP15()
{
    delete[] checkInCodelets200;
    delete[] barrierCodelets199;
    delete[] checkInCodelets179;
    delete[] barrierCodelets170;
    delete[] checkInCodelets170;
    delete[] barrierCodelets169;
    delete[] checkInCodelets136;
    delete[] nextCodeletsprefix_sum;
    delete[] nextSyncCodeletsprefix_sum;
    delete[] buf_darts15;
    delete[] k_darts15;
    delete[] nt_darts15;
    delete[] slice_begin_darts15;
    delete[] slice_end_darts15;
    delete[] t1_darts15;
    delete[] t2_darts15;
    delete[] tid_darts15;
    delete[] tmp_darts15;
    delete[] functionResult;
}
void TP15::setNewInputs(int64_t* in_buf, int64_t* in_functionResult, size_t codeletID)
{
    this->buf_darts15[codeletID] = in_buf;
    this->functionResult[codeletID] = in_functionResult;
}
/*TP211: OMPParallelDirective*/
void TP211::_barrierCodelets211::fire(void)
{
    TP211* myTP = static_cast<TP211*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
void TP211::_checkInCodelets214::fire(void)
{
    /*printOMPForCodeletFireCode, inlined: 0*/
    /*region 214 0*/
    /*Determine the TP to which this codelet belongs and check if this codelet spawns the TP or if
     * it signals it using dispatchCodelet()*/
    size_t idx = this->getID() / myTP->codeletsPerTP214;
    if (idx < myTP->TPsToUse214) {
        if (!__sync_val_compare_and_swap(&(myTP->TP214_alreadyLaunched[idx]), 0, 1)) {
            int64_t range = abs(2 * nv + 2 - 0) / 1;
            int64_t rangePerCodelet = range / myTP->TPsToUse214;
            int64_t minIteration = min<int64_t>(2 * nv + 2, 0);
            int64_t remainderRange = range % myTP->TPsToUse214;
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
            if (0 < 2 * nv + 2) {
                initIteration = min(initIteration, lastIteration);
                lastIteration = max(initIteration, lastIteration);
            } else {
                initIteration = max(initIteration, lastIteration);
                lastIteration = min(initIteration, lastIteration);
            }
            if (idx == myTP->TPsToUse214 - 1) {
                lastIteration = 2 * nv + 2;
            }
#if USEINVOKE == 1
            invoke<TP214>(myTP, myTP->codeletsPerTP214 * DARTS_CODELETS_MULT, this->getID(), myTP,
                initIteration, lastIteration, &(myTP->TP214Ptr[idx]));
#else
            place<TP214>(idx, myTP, myTP->codeletsPerTP214 * DARTS_CODELETS_MULT, this->getID(),
                myTP, initIteration, lastIteration, &(myTP->TP214Ptr[idx]));
#endif
        } else {
            if (myTP->TP214Ptr[idx] != nullptr) {
                myTP->TP214Ptr[idx]->dispatchCodelet(this->getID());
            } else {
                this->resetCodelet();
                this->decDep();
            }
        }
    }
}
void TP211::_barrierCodelets214::fire(void)
{
    TP211* myTP = static_cast<TP211*>(myTP_);
    {
        for (size_t codeletsCounter = 0; codeletsCounter < myTP->numThreads; codeletsCounter++) {
            myTP->checkInCodelets247[codeletsCounter].decDep();
        }
    }
}
void TP211::_checkInCodelets247::fire(void)
{
    /*printOMPForCodeletFireCode, inlined: 0*/
    /*region 247 0*/
    /*Determine the TP to which this codelet belongs and check if this codelet spawns the TP or if
     * it signals it using dispatchCodelet()*/
    size_t idx = this->getID() / myTP->codeletsPerTP247;
    if (idx < myTP->TPsToUse247) {
        if (!__sync_val_compare_and_swap(&(myTP->TP247_alreadyLaunched[idx]), 0, 1)) {
            int64_t range = abs((*(this->inputsTPParent->nedge_darts211)) - 0) / 1;
            int64_t rangePerCodelet = range / myTP->TPsToUse247;
            int64_t minIteration = min<int64_t>((*(this->inputsTPParent->nedge_darts211)), 0);
            int64_t remainderRange = range % myTP->TPsToUse247;
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
            if (0 < (*(this->inputsTPParent->nedge_darts211))) {
                initIteration = min(initIteration, lastIteration);
                lastIteration = max(initIteration, lastIteration);
            } else {
                initIteration = max(initIteration, lastIteration);
                lastIteration = min(initIteration, lastIteration);
            }
            if (idx == myTP->TPsToUse247 - 1) {
                lastIteration = (*(this->inputsTPParent->nedge_darts211));
            }
#if USEINVOKE == 1
            invoke<TP247>(myTP, myTP->codeletsPerTP247 * DARTS_CODELETS_MULT, this->getID(), myTP,
                initIteration, lastIteration, &(*(this->inputsTPParent->IJ_darts211)),
                &(*(this->inputsTPParent->nedge_darts211)), &(myTP->TP247Ptr[idx]));
#else
            place<TP247>(idx, myTP, myTP->codeletsPerTP247 * DARTS_CODELETS_MULT, this->getID(),
                myTP, initIteration, lastIteration, &(*(this->inputsTPParent->IJ_darts211)),
                &(*(this->inputsTPParent->nedge_darts211)), &(myTP->TP247Ptr[idx]));
#endif
        } else {
            if (myTP->TP247Ptr[idx] != nullptr) {
                myTP->TP247Ptr[idx]->dispatchCodelet(this->getID());
            } else {
                this->resetCodelet();
                this->decDep();
            }
        }
    }
}
void TP211::_barrierCodelets247::fire(void)
{
    TP211* myTP = static_cast<TP211*>(myTP_);
    {
        for (size_t codeletsCounter = 0; codeletsCounter < myTP->numThreads; codeletsCounter++) {
            myTP->checkInCodelets312[codeletsCounter].decDep();
        }
    }
}
void TP211::_checkInCodelets312::fire(void)
{
    /*Select the thread executing OMPSingleDirective 312*/
    if (!__sync_val_compare_and_swap(&(myTP->TP312_alreadyLaunched), 0, 1)) {
        /*Init the vars for this region*/
        /*Initialize the vars of the inlined region*/
        this->inputsTPParent->buf_darts312
            = (this->inputsTPParent->buf_darts211) /*OMP_SHARED - VAR INLINED*/;

        /*printing node 314: BinaryOperator*/
        (*(this->inputsTPParent->buf_darts312)) = (int64_t*)malloc(
            omp_get_num_threads() * sizeof(*(*(this->inputsTPParent->buf_darts312))));

        /*printing node 321: IfStmt*/
        if (!(*(this->inputsTPParent->buf_darts312))) {
            perror("alloca for prefix-sum hosed");
            abort();
        }
        /*Signaling next codelet from last stmt in the codelet*/
        /*Signaling omp region's barrier*/
        myTP->controlTPParent->barrierCodelets312[0].decDep();
    } else {
        /*Signaling omp region's barrier*/
        myTP->barrierCodelets312[0].decDep();
    }
}
void TP211::_barrierCodelets312::fire(void)
{
    TP211* myTP = static_cast<TP211*>(myTP_);
    {
        for (size_t codeletsCounter = 0; codeletsCounter < myTP->numThreads; codeletsCounter++) {
            myTP->checkInCodelets326[codeletsCounter].decDep();
        }
    }
}
void TP211::_checkInCodelets326::fire(void)
{
    /*printOMPForCodeletFireCode, inlined: 0*/
    /*region 326 0*/
    /*Determine the TP to which this codelet belongs and check if this codelet spawns the TP or if
     * it signals it using dispatchCodelet()*/
    size_t idx = this->getID() / myTP->codeletsPerTP326;
    if (idx < myTP->TPsToUse326) {
        if (!__sync_val_compare_and_swap(&(myTP->TP326_alreadyLaunched[idx]), 0, 1)) {
            int64_t range = abs(nv - 0) / 1;
            int64_t rangePerCodelet = range / myTP->TPsToUse326;
            int64_t minIteration = min<int64_t>(nv, 0);
            int64_t remainderRange = range % myTP->TPsToUse326;
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
            if (0 < nv) {
                initIteration = min(initIteration, lastIteration);
                lastIteration = max(initIteration, lastIteration);
            } else {
                initIteration = max(initIteration, lastIteration);
                lastIteration = min(initIteration, lastIteration);
            }
            if (idx == myTP->TPsToUse326 - 1) {
                lastIteration = nv;
            }
#if USEINVOKE == 1
            invoke<TP326>(myTP, myTP->codeletsPerTP326 * DARTS_CODELETS_MULT, this->getID(), myTP,
                initIteration, lastIteration, &(myTP->TP326Ptr[idx]));
#else
            place<TP326>(idx, myTP, myTP->codeletsPerTP326 * DARTS_CODELETS_MULT, this->getID(),
                myTP, initIteration, lastIteration, &(myTP->TP326Ptr[idx]));
#endif
        } else {
            if (myTP->TP326Ptr[idx] != nullptr) {
                myTP->TP326Ptr[idx]->dispatchCodelet(this->getID());
            } else {
                this->resetCodelet();
                this->decDep();
            }
        }
    }
}
void TP211::_barrierCodelets326::fire(void)
{
    TP211* myTP = static_cast<TP211*>(myTP_);
    {
        for (size_t codeletsCounter = 0; codeletsCounter < myTP->numThreads; codeletsCounter++) {
            myTP->checkInCodelets362[codeletsCounter].decDep();
        }
    }
}
void TP211::_checkInCodelets362::fire(void)
{

    /*printing node 362: CallExpr*/
    if (!__sync_val_compare_and_swap(&(myTP->controlTPParent->TP362_alreadyLaunched), 0, 1)) {
/*Finding the next codelet after the function call*/
#if 0
/*Signaling next codelet region: 362 nextRegion: 363 */
myTP->controlTPParent->checkInCodelets363[this->getID()].decDep();
#endif
/*Finding the next sync point before making the function call*/
#if 0
/*Find the sync point for a function call*/
/*syncNode: OMPForDirective 363*/
/*Find the TP to which the nextRegion belongs*/
#endif
        /*Make the function call*/
        invoke<TP_prefix_sum>(myTP, myTP->numThreads, this->getID(),
            &(myTP->controlTPParent->checkInCodelets363[this->getID()]),
            &(myTP->controlTPParent->barrierCodelets363[0]), &(myTP->controlTPParent->TP362Ptr),
            (*(this->inputsTPParent->buf_darts211)),
            &((this->inputsTPParent->accum_darts211[this->getID()])));
    } else {
        if (myTP->controlTPParent->TP362Ptr == nullptr) {
            myTP->add(this);
            return;
        } else {
            myTP->controlTPParent->TP362Ptr->setNewInputs((*(this->inputsTPParent->buf_darts211)),
                &((this->inputsTPParent->accum_darts211[this->getID()])), this->getID());
            myTP->controlTPParent->TP362Ptr->nextCodeletsprefix_sum[this->getID()]
                = &(myTP->controlTPParent->checkInCodelets363[this->getID()]);
            myTP->controlTPParent->TP362Ptr->nextSyncCodeletsprefix_sum[this->getID()]
                = &(myTP->controlTPParent->barrierCodelets363[0]);
#if USE_SPIN_CODELETS == 0
            myTP->controlTPParent->TP362Ptr->firstCodelet[this->getID()].decDep();
#else
            myTP->controlTPParent->TP362Ptr->availableCodelets[this->getID()] = 1;
#endif
        }
    }
}
void TP211::_checkInCodelets363::fire(void)
{
    /*printOMPForCodeletFireCode, inlined: 0*/
    /*region 363 0*/
    /*Determine the TP to which this codelet belongs and check if this codelet spawns the TP or if
     * it signals it using dispatchCodelet()*/
    size_t idx = this->getID() / myTP->codeletsPerTP363;
    if (idx < myTP->TPsToUse363) {
        if (!__sync_val_compare_and_swap(&(myTP->TP363_alreadyLaunched[idx]), 0, 1)) {
            int64_t range = abs(nv - 0) / 1;
            int64_t rangePerCodelet = range / myTP->TPsToUse363;
            int64_t minIteration = min<int64_t>(nv, 0);
            int64_t remainderRange = range % myTP->TPsToUse363;
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
            if (0 < nv) {
                initIteration = min(initIteration, lastIteration);
                lastIteration = max(initIteration, lastIteration);
            } else {
                initIteration = max(initIteration, lastIteration);
                lastIteration = min(initIteration, lastIteration);
            }
            if (idx == myTP->TPsToUse363 - 1) {
                lastIteration = nv;
            }
#if USEINVOKE == 1
            invoke<TP363>(myTP, myTP->codeletsPerTP363 * DARTS_CODELETS_MULT, this->getID(), myTP,
                initIteration, lastIteration, &(myTP->TP363Ptr[idx]));
#else
            place<TP363>(idx, myTP, myTP->codeletsPerTP363 * DARTS_CODELETS_MULT, this->getID(),
                myTP, initIteration, lastIteration, &(myTP->TP363Ptr[idx]));
#endif
        } else {
            if (myTP->TP363Ptr[idx] != nullptr) {
                myTP->TP363Ptr[idx]->dispatchCodelet(this->getID());
            } else {
                this->resetCodelet();
                this->decDep();
            }
        }
    }
}
void TP211::_barrierCodelets363::fire(void)
{
    TP211* myTP = static_cast<TP211*>(myTP_);
    {
        for (size_t codeletsCounter = 0; codeletsCounter < myTP->numThreads; codeletsCounter++) {
            myTP->checkInCodelets398[codeletsCounter].decDep();
        }
    }
}
void TP211::_checkInCodelets398::fire(void)
{
    /*Select the thread executing OMPSingleDirective 398*/
    if (!__sync_val_compare_and_swap(&(myTP->TP398_alreadyLaunched), 0, 1)) {
        /*Init the vars for this region*/
        /*Initialize the vars of the inlined region*/
        this->inputsTPParent->accum_darts398
            = &(this->inputsTPParent
                    ->accum_darts211[this->getLocalID()]) /*OMP_SHARED_PRIVATE - VAR INLINED*/;
        this->inputsTPParent->err_darts398
            = (this->inputsTPParent->err_darts211) /*OMP_SHARED - VAR INLINED*/;

        /*printing node 400: BinaryOperator*/
        (xoff[2 * (nv)]) = (*(this->inputsTPParent->accum_darts398));

        /*printing node 403: IfStmt*/
        if (!(xadjstore
                = (int64_t*)xmalloc_large_ext(((xoff[2 * (nv)]) + 2) * sizeof(*xadjstore)))) {
            (*(this->inputsTPParent->err_darts398)) = -1;
        }

        /*printing node 408: IfStmt*/
        if (!(*(this->inputsTPParent->err_darts398))) {
            xadj = &xadjstore[2];
            {
                {
                    /*Loop's init*/
                    (this->inputsTPParent->k_darts398) = 0;
                    int64_t k_darts_counter_temp398 = (this->inputsTPParent->k_darts398);
                    for (; k_darts_counter_temp398 < (xoff[2 * (nv)]) + 2;
                         ++k_darts_counter_temp398) {
                        xadjstore[k_darts_counter_temp398] = -1;
                    }
                    (this->inputsTPParent->k_darts398) = k_darts_counter_temp398;
                }
            }
        }
        /*Signaling next codelet from last stmt in the codelet*/
        /*Signaling omp region's barrier*/
        myTP->controlTPParent->barrierCodelets398[0].decDep();
    } else {
        /*Signaling omp region's barrier*/
        myTP->barrierCodelets398[0].decDep();
    }
}
void TP211::_barrierCodelets398::fire(void)
{
    TP211* myTP = static_cast<TP211*>(myTP_);
    myTP->TPParent->barrierCodelets211[0].setDep(0);
    myTP->add(&(myTP->TPParent->barrierCodelets211[0]));
}
TP211::TP211(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet,
    struct packed_edge* __restrict* in_IJ, int64_t** in_buf, int* in_err, int64_t* in_nedge)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , IJ_darts211(in_IJ) /*OMP_SHARED - INPUT*/
    , buf_darts211(in_buf) /*OMP_SHARED - INPUT*/
    , err_darts211(in_err) /*OMP_SHARED - INPUT*/
    , nedge_darts211(in_nedge) /*OMP_SHARED - INPUT*/
    , accum_darts211(new int64_t[this->numThreads]) /*VARIABLE*/
    , k_darts211(new int64_t[this->numThreads]) /*VARIABLE*/
    , TP214Ptr(new TP214*[NUMTPS214])
    , TP214_alreadyLaunched(new size_t[NUMTPS214])
    , numTPsSet214(0)
    , numTPsReady214(0)
    , TPsToUse214(NUMTPS214)
    , codeletsPerTP214(this->numThreads / NUMTPS214)
    , totalCodelets214(this->TPsToUse214 * this->codeletsPerTP214)
    , TP247Ptr(new TP247*[NUMTPS247])
    , TP247_alreadyLaunched(new size_t[NUMTPS247])
    , numTPsSet247(0)
    , numTPsReady247(0)
    , TPsToUse247(NUMTPS247)
    , codeletsPerTP247(this->numThreads / NUMTPS247)
    , totalCodelets247(this->TPsToUse247 * this->codeletsPerTP247)
    , TP312_alreadyLaunched(0)
    , TP326Ptr(new TP326*[NUMTPS326])
    , TP326_alreadyLaunched(new size_t[NUMTPS326])
    , numTPsSet326(0)
    , numTPsReady326(0)
    , TPsToUse326(NUMTPS326)
    , codeletsPerTP326(this->numThreads / NUMTPS326)
    , totalCodelets326(this->TPsToUse326 * this->codeletsPerTP326)
    , TP362Ptr(nullptr)
    , TP362_alreadyLaunched(0)
    , TP363Ptr(new TP363*[NUMTPS363])
    , TP363_alreadyLaunched(new size_t[NUMTPS363])
    , numTPsSet363(0)
    , numTPsReady363(0)
    , TPsToUse363(NUMTPS363)
    , codeletsPerTP363(this->numThreads / NUMTPS363)
    , totalCodelets363(this->TPsToUse363 * this->codeletsPerTP363)
    , TP398_alreadyLaunched(0)
    , barrierCodelets211(new _barrierCodelets211[1])
    , checkInCodelets214(new _checkInCodelets214[this->numThreads])
    , barrierCodelets214(new _barrierCodelets214[1])
    , checkInCodelets247(new _checkInCodelets247[this->numThreads])
    , barrierCodelets247(new _barrierCodelets247[1])
    , checkInCodelets312(new _checkInCodelets312[this->numThreads])
    , barrierCodelets312(new _barrierCodelets312[1])
    , checkInCodelets326(new _checkInCodelets326[this->numThreads])
    , barrierCodelets326(new _barrierCodelets326[1])
    , checkInCodelets362(new _checkInCodelets362[this->numThreads])
    , checkInCodelets363(new _checkInCodelets363[this->numThreads])
    , barrierCodelets363(new _barrierCodelets363[1])
    , checkInCodelets398(new _checkInCodelets398[this->numThreads])
    , barrierCodelets398(new _barrierCodelets398[1])
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets211[0] = _barrierCodelets211(ompNumThreads, ompNumThreads, this, 0);
    barrierCodelets398[0] = _barrierCodelets398(this->numThreads, this->numThreads, this, 0);
    barrierCodelets363[0] = _barrierCodelets363(NUMTPS363, NUMTPS363, this, 0);
    barrierCodelets326[0] = _barrierCodelets326(NUMTPS326, NUMTPS326, this, 0);
    barrierCodelets312[0] = _barrierCodelets312(this->numThreads, this->numThreads, this, 0);
    barrierCodelets247[0] = _barrierCodelets247(NUMTPS247, NUMTPS247, this, 0);
    barrierCodelets214[0] = _barrierCodelets214(NUMTPS214, NUMTPS214, this, 0);
    _checkInCodelets398* checkInCodelets398Ptr = (this->checkInCodelets398);
    _checkInCodelets363* checkInCodelets363Ptr = (this->checkInCodelets363);
    for (int i = 0; i < NUMTPS363; i++) {
        TP363Ptr[i] = nullptr;
        TP363_alreadyLaunched[i] = 0;
    }
    _checkInCodelets362* checkInCodelets362Ptr = (this->checkInCodelets362);
    _checkInCodelets326* checkInCodelets326Ptr = (this->checkInCodelets326);
    for (int i = 0; i < NUMTPS326; i++) {
        TP326Ptr[i] = nullptr;
        TP326_alreadyLaunched[i] = 0;
    }
    _checkInCodelets312* checkInCodelets312Ptr = (this->checkInCodelets312);
    _checkInCodelets247* checkInCodelets247Ptr = (this->checkInCodelets247);
    for (int i = 0; i < NUMTPS247; i++) {
        TP247Ptr[i] = nullptr;
        TP247_alreadyLaunched[i] = 0;
    }
    _checkInCodelets214* checkInCodelets214Ptr = (this->checkInCodelets214);
    for (int i = 0; i < NUMTPS214; i++) {
        TP214Ptr[i] = nullptr;
        TP214_alreadyLaunched[i] = 0;
    }
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets398Ptr) = _checkInCodelets398(1, 1, this, codeletCounter);
        checkInCodelets398Ptr++;
        (*checkInCodelets363Ptr) = _checkInCodelets363(1, 1, this, codeletCounter);
        checkInCodelets363Ptr++;
        (*checkInCodelets362Ptr) = _checkInCodelets362(1, 1, this, codeletCounter);
        checkInCodelets362Ptr++;
        (*checkInCodelets326Ptr) = _checkInCodelets326(1, 1, this, codeletCounter);
        checkInCodelets326Ptr++;
        (*checkInCodelets312Ptr) = _checkInCodelets312(1, 1, this, codeletCounter);
        checkInCodelets312Ptr++;
        (*checkInCodelets247Ptr) = _checkInCodelets247(1, 1, this, codeletCounter);
        checkInCodelets247Ptr++;
        (*checkInCodelets214Ptr) = _checkInCodelets214(1, 1, this, codeletCounter);
        (*checkInCodelets214Ptr).decDep();
        checkInCodelets214Ptr++;
    }
}
TP211::~TP211()
{
    delete[] accum_darts211;
    delete[] k_darts211;
    delete[] barrierCodelets211;
    delete[] barrierCodelets398;
    delete[] checkInCodelets398;
    delete[] barrierCodelets363;
    delete[] checkInCodelets363;
    delete[] checkInCodelets362;
    delete[] barrierCodelets326;
    delete[] checkInCodelets326;
    delete[] barrierCodelets312;
    delete[] checkInCodelets312;
    delete[] barrierCodelets247;
    delete[] checkInCodelets247;
    delete[] barrierCodelets214;
    delete[] checkInCodelets214;
}
/*TP214: OMPForDirective*/
void TP214::_barrierCodelets214::fire(void)
{
    TP214* myTP = static_cast<TP214*>(myTP_);
    myTP->controlTPParent->TPParent->barrierCodelets214[0].decDep();
}
bool TP214::requestNewRangeIterations214(int64_t* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Static */
    /*Chunk = 0*/
    bool isThereNewIteration = false;
    {
        /*Static Scheduling*/
        int64_t tempStartRange = rangePerCodelet214 * codeletID;
        int64_t tempEndRange = rangePerCodelet214 * (codeletID + 1);
        if (remainderRange214 != 0) {
            if (codeletID < (uint32_t)remainderRange214) {
                tempStartRange += codeletID;
                tempEndRange += (codeletID + 1);
            } else {
                tempStartRange += remainderRange214;
                tempEndRange += remainderRange214;
            }
        }
        tempStartRange = tempStartRange * 1 + minIteration214;
        tempEndRange = tempEndRange * 1 + minIteration214;
        if (tempStartRange != tempEndRange) {
            isThereNewIteration = true;
        }
        if (initIteration214 < lastIteration214) {
            (this->inputsTPParent->k_darts214[codeletID]) = min(tempStartRange, tempEndRange);
            *endRange = max(tempStartRange, tempEndRange);
        } else {
            (this->inputsTPParent->k_darts214[codeletID]) = max(tempStartRange, tempEndRange);
            *endRange = min(tempStartRange, tempEndRange);
        }
        if (codeletID == this->numThreads - 1) {
            *endRange = lastIteration214;
        }
    }
    return isThereNewIteration;
}
void TP214::_checkInCodelets215::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    /*Init the vars for this region*/

    /*printing node 215: ForStmt*/
    /*var: k*/
    int64_t* k = &(this->inputsTPParent->k_darts214[this->getLocalID()]);
    (void)k /*OMP_PRIVATE*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations214(
        (int64_t*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        /*Signaling omp for stmt's barrier*/
        myTP->controlTPParent->barrierCodelets214[0].decDep();
        return;
    }
    for (int64_t k_darts_counter_temp214 = (*k); k_darts_counter_temp214 < endRange
         && k_darts_counter_temp214 < this->inputsTPParent->lastIteration214;
         ++k_darts_counter_temp214) {
        {
            xoff[(k_darts_counter_temp214)] = 0;
        }
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling omp for stmt's barrier*/
    myTP->controlTPParent->barrierCodelets214[0].decDep();
}
TP214::TP214(int in_numThreads, int in_mainCodeletID, TP211* in_TPParent, int64_t in_initIteration,
    int64_t in_lastIteration, TP214** in_ptrToThisTP)
    : ompTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , k_darts214(new int64_t[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , initIteration214(in_initIteration)
    , lastIteration214(in_lastIteration)
    , readyCodelets(0)
    , baseNumThreads(this->numThreads / DARTS_CODELETS_MULT)
    , barrierCodelets214(new _barrierCodelets214[1])
    , checkInCodelets215(new _checkInCodelets215[this->numThreads])
{
    /*Initialize the loop parameters*/
    range214 = abs(lastIteration214 - initIteration214) / 1;
    rangePerCodelet214 = range214 / numThreads;
    minIteration214 = min<int64_t>(lastIteration214, initIteration214);
    remainderRange214 = range214 % numThreads;
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets214[0] = _barrierCodelets214(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets215* checkInCodelets215Ptr = (this->checkInCodelets215);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets215);
#endif
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->baseNumThreads;
         codeletCounter++) {
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets215Ptr) = _checkInCodelets215(2, 1, this, codeletCounter);
#else
        (*checkInCodelets215Ptr) = _checkInCodelets215(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets215Ptr).decDep();
        checkInCodelets215Ptr++;
    }
    this->dispatchCodelet(this->getID());
    *(in_ptrToThisTP) = this;
}
void TP214::dispatchCodelet(size_t codeletID)
{
    int idx = codeletID / this->baseNumThreads;
    int localID = codeletID - this->baseNumThreads * idx;
    this->checkInCodelets215[localID].setID(codeletID);
    this->checkInCodelets215[localID].setLocalID(localID);
    /*Check if we want to replicate codelets*/
    for (size_t i = 1; i < (size_t)DARTS_CODELETS_MULT; i++) {
#if USE_SPIN_CODELETS == 0
        this->checkInCodelets215[localID + this->baseNumThreads * i]
            = _checkInCodelets215(2, 1, this, localID + this->baseNumThreads * i);
#else
        this->checkInCodelets215[localID + this->baseNumThreads * i]
            = _checkInCodelets215(1, 1, this, localID + this->baseNumThreads * i);
#endif
        this->checkInCodelets215[localID + this->baseNumThreads * i].setID(codeletID);
        this->checkInCodelets215[localID + this->baseNumThreads * i].decDep();
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
TP214::~TP214()
{
    delete[] barrierCodelets214;
    delete[] checkInCodelets215;
}
/*TP247: OMPForDirective*/
void TP247::_barrierCodelets247::fire(void)
{
    TP247* myTP = static_cast<TP247*>(myTP_);
    myTP->controlTPParent->TPParent->barrierCodelets247[0].decDep();
}
bool TP247::requestNewRangeIterations247(int64_t* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Static */
    /*Chunk = 0*/
    bool isThereNewIteration = false;
    {
        /*Static Scheduling*/
        int64_t tempStartRange = rangePerCodelet247 * codeletID;
        int64_t tempEndRange = rangePerCodelet247 * (codeletID + 1);
        if (remainderRange247 != 0) {
            if (codeletID < (uint32_t)remainderRange247) {
                tempStartRange += codeletID;
                tempEndRange += (codeletID + 1);
            } else {
                tempStartRange += remainderRange247;
                tempEndRange += remainderRange247;
            }
        }
        tempStartRange = tempStartRange * 1 + minIteration247;
        tempEndRange = tempEndRange * 1 + minIteration247;
        if (tempStartRange != tempEndRange) {
            isThereNewIteration = true;
        }
        if (initIteration247 < lastIteration247) {
            (this->inputsTPParent->k_darts247[codeletID]) = min(tempStartRange, tempEndRange);
            *endRange = max(tempStartRange, tempEndRange);
        } else {
            (this->inputsTPParent->k_darts247[codeletID]) = max(tempStartRange, tempEndRange);
            *endRange = min(tempStartRange, tempEndRange);
        }
        if (codeletID == this->numThreads - 1) {
            *endRange = lastIteration247;
        }
    }
    return isThereNewIteration;
}
void TP247::_checkInCodelets248::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    /*Init the vars for this region*/

    /*printing node 248: ForStmt*/
    /*var: IJ*/
    /*var: k*/
    /*var: nedge*/
    struct packed_edge* __restrict* IJ = (this->inputsTPParent->IJ_darts247);
    (void)IJ /*OMP_SHARED*/;
    int64_t* k = &(this->inputsTPParent->k_darts247[this->getLocalID()]);
    (void)k /*OMP_PRIVATE*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations247(
        (int64_t*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        /*Signaling omp for stmt's barrier*/
        myTP->controlTPParent->barrierCodelets247[0].decDep();
        return;
    }
    for (int64_t k_darts_counter_temp247 = (*k); k_darts_counter_temp247 < endRange
         && k_darts_counter_temp247 < this->inputsTPParent->lastIteration247;
         ++k_darts_counter_temp247) {
        {
            int64_t i = get_v0_from_edge(&(*(IJ))[(k_darts_counter_temp247)]);
            int64_t j = get_v1_from_edge(&(*(IJ))[(k_darts_counter_temp247)]);
            if (i != j) {
                if (i >= 0) {
                    __sync_add_and_fetch(&((xoff[2 * (i)])), 1);
                }
                if (j >= 0) {
                    __sync_add_and_fetch(&((xoff[2 * (j)])), 1);
                }
            }
        }
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling omp for stmt's barrier*/
    myTP->controlTPParent->barrierCodelets247[0].decDep();
}
TP247::TP247(int in_numThreads, int in_mainCodeletID, TP211* in_TPParent, int64_t in_initIteration,
    int64_t in_lastIteration, struct packed_edge* __restrict* in_IJ, int64_t* in_nedge,
    TP247** in_ptrToThisTP)
    : ompTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , IJ_darts247(in_IJ) /*OMP_SHARED - INPUT*/
    , k_darts247(new int64_t[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , nedge_darts247(in_nedge) /*OMP_SHARED - INPUT*/
    , initIteration247(in_initIteration)
    , lastIteration247(in_lastIteration)
    , readyCodelets(0)
    , baseNumThreads(this->numThreads / DARTS_CODELETS_MULT)
    , barrierCodelets247(new _barrierCodelets247[1])
    , checkInCodelets248(new _checkInCodelets248[this->numThreads])
{
    /*Initialize the loop parameters*/
    range247 = abs(lastIteration247 - initIteration247) / 1;
    rangePerCodelet247 = range247 / numThreads;
    minIteration247 = min<int64_t>(lastIteration247, initIteration247);
    remainderRange247 = range247 % numThreads;
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets247[0] = _barrierCodelets247(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets248* checkInCodelets248Ptr = (this->checkInCodelets248);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets248);
#endif
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->baseNumThreads;
         codeletCounter++) {
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets248Ptr) = _checkInCodelets248(2, 1, this, codeletCounter);
#else
        (*checkInCodelets248Ptr) = _checkInCodelets248(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets248Ptr).decDep();
        checkInCodelets248Ptr++;
    }
    this->dispatchCodelet(this->getID());
    *(in_ptrToThisTP) = this;
}
void TP247::dispatchCodelet(size_t codeletID)
{
    int idx = codeletID / this->baseNumThreads;
    int localID = codeletID - this->baseNumThreads * idx;
    this->checkInCodelets248[localID].setID(codeletID);
    this->checkInCodelets248[localID].setLocalID(localID);
    /*Check if we want to replicate codelets*/
    for (size_t i = 1; i < (size_t)DARTS_CODELETS_MULT; i++) {
#if USE_SPIN_CODELETS == 0
        this->checkInCodelets248[localID + this->baseNumThreads * i]
            = _checkInCodelets248(2, 1, this, localID + this->baseNumThreads * i);
#else
        this->checkInCodelets248[localID + this->baseNumThreads * i]
            = _checkInCodelets248(1, 1, this, localID + this->baseNumThreads * i);
#endif
        this->checkInCodelets248[localID + this->baseNumThreads * i].setID(codeletID);
        this->checkInCodelets248[localID + this->baseNumThreads * i].decDep();
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
TP247::~TP247()
{
    delete[] barrierCodelets247;
    delete[] checkInCodelets248;
}
/*TP326: OMPForDirective*/
void TP326::_barrierCodelets326::fire(void)
{
    TP326* myTP = static_cast<TP326*>(myTP_);
    myTP->controlTPParent->TPParent->barrierCodelets326[0].decDep();
}
bool TP326::requestNewRangeIterations326(int64_t* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Static */
    /*Chunk = 0*/
    bool isThereNewIteration = false;
    {
        /*Static Scheduling*/
        int64_t tempStartRange = rangePerCodelet326 * codeletID;
        int64_t tempEndRange = rangePerCodelet326 * (codeletID + 1);
        if (remainderRange326 != 0) {
            if (codeletID < (uint32_t)remainderRange326) {
                tempStartRange += codeletID;
                tempEndRange += (codeletID + 1);
            } else {
                tempStartRange += remainderRange326;
                tempEndRange += remainderRange326;
            }
        }
        tempStartRange = tempStartRange * 1 + minIteration326;
        tempEndRange = tempEndRange * 1 + minIteration326;
        if (tempStartRange != tempEndRange) {
            isThereNewIteration = true;
        }
        if (initIteration326 < lastIteration326) {
            (this->inputsTPParent->k_darts326[codeletID]) = min(tempStartRange, tempEndRange);
            *endRange = max(tempStartRange, tempEndRange);
        } else {
            (this->inputsTPParent->k_darts326[codeletID]) = max(tempStartRange, tempEndRange);
            *endRange = min(tempStartRange, tempEndRange);
        }
        if (codeletID == this->numThreads - 1) {
            *endRange = lastIteration326;
        }
    }
    return isThereNewIteration;
}
void TP326::_checkInCodelets327::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    /*Init the vars for this region*/

    /*printing node 327: ForStmt*/
    /*var: k*/
    int64_t* k = &(this->inputsTPParent->k_darts326[this->getLocalID()]);
    (void)k /*OMP_PRIVATE*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations326(
        (int64_t*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        /*Signaling omp for stmt's barrier*/
        myTP->controlTPParent->barrierCodelets326[0].decDep();
        return;
    }
    for (int64_t k_darts_counter_temp326 = (*k); k_darts_counter_temp326 < endRange
         && k_darts_counter_temp326 < this->inputsTPParent->lastIteration326;
         ++k_darts_counter_temp326) {
        {
            if ((xoff[2 * ((k_darts_counter_temp326))]) < 2) {
                (xoff[2 * ((k_darts_counter_temp326))]) = 2;
            }
        }
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling omp for stmt's barrier*/
    myTP->controlTPParent->barrierCodelets326[0].decDep();
}
TP326::TP326(int in_numThreads, int in_mainCodeletID, TP211* in_TPParent, int64_t in_initIteration,
    int64_t in_lastIteration, TP326** in_ptrToThisTP)
    : ompTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , k_darts326(new int64_t[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , initIteration326(in_initIteration)
    , lastIteration326(in_lastIteration)
    , readyCodelets(0)
    , baseNumThreads(this->numThreads / DARTS_CODELETS_MULT)
    , barrierCodelets326(new _barrierCodelets326[1])
    , checkInCodelets327(new _checkInCodelets327[this->numThreads])
{
    /*Initialize the loop parameters*/
    range326 = abs(lastIteration326 - initIteration326) / 1;
    rangePerCodelet326 = range326 / numThreads;
    minIteration326 = min<int64_t>(lastIteration326, initIteration326);
    remainderRange326 = range326 % numThreads;
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets326[0] = _barrierCodelets326(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets327* checkInCodelets327Ptr = (this->checkInCodelets327);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets327);
#endif
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->baseNumThreads;
         codeletCounter++) {
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets327Ptr) = _checkInCodelets327(2, 1, this, codeletCounter);
#else
        (*checkInCodelets327Ptr) = _checkInCodelets327(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets327Ptr).decDep();
        checkInCodelets327Ptr++;
    }
    this->dispatchCodelet(this->getID());
    *(in_ptrToThisTP) = this;
}
void TP326::dispatchCodelet(size_t codeletID)
{
    int idx = codeletID / this->baseNumThreads;
    int localID = codeletID - this->baseNumThreads * idx;
    this->checkInCodelets327[localID].setID(codeletID);
    this->checkInCodelets327[localID].setLocalID(localID);
    /*Check if we want to replicate codelets*/
    for (size_t i = 1; i < (size_t)DARTS_CODELETS_MULT; i++) {
#if USE_SPIN_CODELETS == 0
        this->checkInCodelets327[localID + this->baseNumThreads * i]
            = _checkInCodelets327(2, 1, this, localID + this->baseNumThreads * i);
#else
        this->checkInCodelets327[localID + this->baseNumThreads * i]
            = _checkInCodelets327(1, 1, this, localID + this->baseNumThreads * i);
#endif
        this->checkInCodelets327[localID + this->baseNumThreads * i].setID(codeletID);
        this->checkInCodelets327[localID + this->baseNumThreads * i].decDep();
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
TP326::~TP326()
{
    delete[] barrierCodelets326;
    delete[] checkInCodelets327;
}
/*TP363: OMPForDirective*/
void TP363::_barrierCodelets363::fire(void)
{
    TP363* myTP = static_cast<TP363*>(myTP_);
    myTP->controlTPParent->TPParent->barrierCodelets363[0].decDep();
}
bool TP363::requestNewRangeIterations363(int64_t* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Static */
    /*Chunk = 0*/
    bool isThereNewIteration = false;
    {
        /*Static Scheduling*/
        int64_t tempStartRange = rangePerCodelet363 * codeletID;
        int64_t tempEndRange = rangePerCodelet363 * (codeletID + 1);
        if (remainderRange363 != 0) {
            if (codeletID < (uint32_t)remainderRange363) {
                tempStartRange += codeletID;
                tempEndRange += (codeletID + 1);
            } else {
                tempStartRange += remainderRange363;
                tempEndRange += remainderRange363;
            }
        }
        tempStartRange = tempStartRange * 1 + minIteration363;
        tempEndRange = tempEndRange * 1 + minIteration363;
        if (tempStartRange != tempEndRange) {
            isThereNewIteration = true;
        }
        if (initIteration363 < lastIteration363) {
            (this->inputsTPParent->k_darts363[codeletID]) = min(tempStartRange, tempEndRange);
            *endRange = max(tempStartRange, tempEndRange);
        } else {
            (this->inputsTPParent->k_darts363[codeletID]) = max(tempStartRange, tempEndRange);
            *endRange = min(tempStartRange, tempEndRange);
        }
        if (codeletID == this->numThreads - 1) {
            *endRange = lastIteration363;
        }
    }
    return isThereNewIteration;
}
void TP363::_checkInCodelets364::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    /*Init the vars for this region*/

    /*printing node 364: ForStmt*/
    /*var: k*/
    int64_t* k = &(this->inputsTPParent->k_darts363[this->getLocalID()]);
    (void)k /*OMP_PRIVATE*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations363(
        (int64_t*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        /*Signaling omp for stmt's barrier*/
        myTP->controlTPParent->barrierCodelets363[0].decDep();
        return;
    }
    for (int64_t k_darts_counter_temp363 = (*k); k_darts_counter_temp363 < endRange
         && k_darts_counter_temp363 < this->inputsTPParent->lastIteration363;
         ++k_darts_counter_temp363) {
        {
            (xoff[1 + 2 * ((k_darts_counter_temp363))]) = (xoff[2 * ((k_darts_counter_temp363))]);
        }
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling omp for stmt's barrier*/
    myTP->controlTPParent->barrierCodelets363[0].decDep();
}
TP363::TP363(int in_numThreads, int in_mainCodeletID, TP211* in_TPParent, int64_t in_initIteration,
    int64_t in_lastIteration, TP363** in_ptrToThisTP)
    : ompTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , k_darts363(new int64_t[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , initIteration363(in_initIteration)
    , lastIteration363(in_lastIteration)
    , readyCodelets(0)
    , baseNumThreads(this->numThreads / DARTS_CODELETS_MULT)
    , barrierCodelets363(new _barrierCodelets363[1])
    , checkInCodelets364(new _checkInCodelets364[this->numThreads])
{
    /*Initialize the loop parameters*/
    range363 = abs(lastIteration363 - initIteration363) / 1;
    rangePerCodelet363 = range363 / numThreads;
    minIteration363 = min<int64_t>(lastIteration363, initIteration363);
    remainderRange363 = range363 % numThreads;
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets363[0] = _barrierCodelets363(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets364* checkInCodelets364Ptr = (this->checkInCodelets364);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets364);
#endif
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->baseNumThreads;
         codeletCounter++) {
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets364Ptr) = _checkInCodelets364(2, 1, this, codeletCounter);
#else
        (*checkInCodelets364Ptr) = _checkInCodelets364(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets364Ptr).decDep();
        checkInCodelets364Ptr++;
    }
    this->dispatchCodelet(this->getID());
    *(in_ptrToThisTP) = this;
}
void TP363::dispatchCodelet(size_t codeletID)
{
    int idx = codeletID / this->baseNumThreads;
    int localID = codeletID - this->baseNumThreads * idx;
    this->checkInCodelets364[localID].setID(codeletID);
    this->checkInCodelets364[localID].setLocalID(localID);
    /*Check if we want to replicate codelets*/
    for (size_t i = 1; i < (size_t)DARTS_CODELETS_MULT; i++) {
#if USE_SPIN_CODELETS == 0
        this->checkInCodelets364[localID + this->baseNumThreads * i]
            = _checkInCodelets364(2, 1, this, localID + this->baseNumThreads * i);
#else
        this->checkInCodelets364[localID + this->baseNumThreads * i]
            = _checkInCodelets364(1, 1, this, localID + this->baseNumThreads * i);
#endif
        this->checkInCodelets364[localID + this->baseNumThreads * i].setID(codeletID);
        this->checkInCodelets364[localID + this->baseNumThreads * i].decDep();
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
TP363::~TP363()
{
    delete[] barrierCodelets363;
    delete[] checkInCodelets364;
}
/*TP20: TP_pack_edges*/
void TP20::_checkInCodelets510::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    /*printOMPForCodeletFireCode, inlined: 0*/
    /*region 510 0*/
    /*Determine the TP to which this codelet belongs and check if this codelet spawns the TP or if
     * it signals it using dispatchCodelet()*/
    size_t idx = this->getID() / myTP->codeletsPerTP510;
    if (idx < myTP->TPsToUse510) {
        if (!__sync_val_compare_and_swap(&(myTP->TP510_alreadyLaunched[idx]), 0, 1)) {
            int64_t range = abs(nv - 0) / 1;
            int64_t rangePerCodelet = range / myTP->TPsToUse510;
            int64_t minIteration = min<int64_t>(nv, 0);
            int64_t remainderRange = range % myTP->TPsToUse510;
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
            if (0 < nv) {
                initIteration = min(initIteration, lastIteration);
                lastIteration = max(initIteration, lastIteration);
            } else {
                initIteration = max(initIteration, lastIteration);
                lastIteration = min(initIteration, lastIteration);
            }
            if (idx == myTP->TPsToUse510 - 1) {
                lastIteration = nv;
            }
#if USEINVOKE == 1
            invoke<TP510>(myTP, myTP->codeletsPerTP510 * DARTS_CODELETS_MULT, this->getID(), myTP,
                initIteration, lastIteration, &(myTP->TP510Ptr[idx]));
#else
            place<TP510>(idx, myTP, myTP->codeletsPerTP510 * DARTS_CODELETS_MULT, this->getID(),
                myTP, initIteration, lastIteration, &(myTP->TP510Ptr[idx]));
#endif
        } else {
            if (myTP->TP510Ptr[idx] != nullptr) {
                myTP->TP510Ptr[idx]->dispatchCodelet(this->getID());
            } else {
                this->resetCodelet();
                this->decDep();
            }
        }
    }
}
void TP20::_barrierCodelets510::fire(void)
{
    TP20* myTP = static_cast<TP20*>(myTP_);

    for (size_t codeletsCounter = 0; codeletsCounter < (size_t)myTP->numThreads;
         codeletsCounter++) {
        myTP->nextCodeletspack_edges[codeletsCounter]->decDep();
    }
}
TP20::TP20(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_mainNextCodelet,
    darts::Codelet* in_mainSyncCodelet, TP20** in_ptrToThisFunctionTP)
    : ompTP(in_numThreads, in_mainCodeletID)
    , ptrToThisFunctionTP(in_ptrToThisFunctionTP)
    , inputsTPParent(this)
    , controlTPParent(this)
    , nextCodeletspack_edges(new Codelet*[in_numThreads])
    , nextSyncCodeletspack_edges(new Codelet*[in_numThreads])
    , v_darts20(new int64_t[this->numThreads])
    , TP510Ptr(new TP510*[NUMTPS510])
    , TP510_alreadyLaunched(new size_t[NUMTPS510])
    , numTPsSet510(0)
    , numTPsReady510(0)
    , TPsToUse510(NUMTPS510)
    , codeletsPerTP510(this->numThreads / NUMTPS510)
    , totalCodelets510(this->TPsToUse510 * this->codeletsPerTP510)
    , checkInCodelets510(new _checkInCodelets510[this->numThreads])
    , barrierCodelets510(new _barrierCodelets510[1])
{
    barrierCodelets510[0] = _barrierCodelets510(NUMTPS510, NUMTPS510, this, 0);
    _checkInCodelets510* checkInCodelets510Ptr = (this->checkInCodelets510);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets510);
#endif
    for (int i = 0; i < NUMTPS510; i++) {
        TP510Ptr[i] = nullptr;
        TP510_alreadyLaunched[i] = 0;
    }
    for (size_t codeletCounter = 0; codeletCounter < this->numThreads; codeletCounter++) {
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets510Ptr) = _checkInCodelets510(2, 1, this, codeletCounter);
#else
        (*checkInCodelets510Ptr) = _checkInCodelets510(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets510Ptr).decDep();
        checkInCodelets510Ptr++;
    }
    if (this->numThreads == 1) {
        this->nextCodeletspack_edges[0] = in_mainNextCodelet;
        this->nextSyncCodeletspack_edges[0] = in_mainSyncCodelet;
        this->availableCodelets[0] = 1;
    } else {
        this->nextCodeletspack_edges[in_mainCodeletID] = in_mainNextCodelet;
        this->nextSyncCodeletspack_edges[in_mainCodeletID] = in_mainSyncCodelet;
#if USE_SPIN_CODELETS == 0
        this->firstCodelet[this->mainCodeletID].decDep();
#else
        this->availableCodelets[this->mainCodeletID] = 1;
#endif
        *(this->ptrToThisFunctionTP) = this;
    }
}
TP20::~TP20()
{
    delete[] barrierCodelets510;
    delete[] checkInCodelets510;
    delete[] nextCodeletspack_edges;
    delete[] nextSyncCodeletspack_edges;
    delete[] v_darts20;
}
/*TP510: OMPForDirective*/
void TP510::_barrierCodelets510::fire(void)
{
    TP510* myTP = static_cast<TP510*>(myTP_);
    myTP->controlTPParent->TPParent->barrierCodelets510[0].decDep();
}
bool TP510::requestNewRangeIterations510(int64_t* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Static */
    /*Chunk = 0*/
    bool isThereNewIteration = false;
    {
        /*Static Scheduling*/
        int64_t tempStartRange = rangePerCodelet510 * codeletID;
        int64_t tempEndRange = rangePerCodelet510 * (codeletID + 1);
        if (remainderRange510 != 0) {
            if (codeletID < (uint32_t)remainderRange510) {
                tempStartRange += codeletID;
                tempEndRange += (codeletID + 1);
            } else {
                tempStartRange += remainderRange510;
                tempEndRange += remainderRange510;
            }
        }
        tempStartRange = tempStartRange * 1 + minIteration510;
        tempEndRange = tempEndRange * 1 + minIteration510;
        if (tempStartRange != tempEndRange) {
            isThereNewIteration = true;
        }
        if (initIteration510 < lastIteration510) {
            (this->inputsTPParent->v_darts510[codeletID]) = min(tempStartRange, tempEndRange);
            *endRange = max(tempStartRange, tempEndRange);
        } else {
            (this->inputsTPParent->v_darts510[codeletID]) = max(tempStartRange, tempEndRange);
            *endRange = min(tempStartRange, tempEndRange);
        }
        if (codeletID == this->numThreads - 1) {
            *endRange = lastIteration510;
        }
    }
    return isThereNewIteration;
}
void TP510::_checkInCodelets511::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    /*Init the vars for this region*/

    /*printing node 511: ForStmt*/
    /*var: v*/
    int64_t* v = &(this->inputsTPParent->v_darts510[this->getLocalID()]);
    (void)v /*OMP_PRIVATE*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations510(
        (int64_t*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        /*Signaling omp for stmt's barrier*/
        myTP->controlTPParent->barrierCodelets510[0].decDep();
        return;
    }
    for (int64_t v_darts_counter_temp510 = (*v); v_darts_counter_temp510 < endRange
         && v_darts_counter_temp510 < this->inputsTPParent->lastIteration510;
         ++v_darts_counter_temp510) {
        {
            pack_vtx_edges((v_darts_counter_temp510));
        }
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling omp for stmt's barrier*/
    myTP->controlTPParent->barrierCodelets510[0].decDep();
}
TP510::TP510(int in_numThreads, int in_mainCodeletID, TP20* in_TPParent, int64_t in_initIteration,
    int64_t in_lastIteration, TP510** in_ptrToThisTP)
    : ompTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , v_darts510(new int64_t[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , initIteration510(in_initIteration)
    , lastIteration510(in_lastIteration)
    , readyCodelets(0)
    , baseNumThreads(this->numThreads / DARTS_CODELETS_MULT)
    , barrierCodelets510(new _barrierCodelets510[1])
    , checkInCodelets511(new _checkInCodelets511[this->numThreads])
{
    /*Initialize the loop parameters*/
    range510 = abs(lastIteration510 - initIteration510) / 1;
    rangePerCodelet510 = range510 / numThreads;
    minIteration510 = min<int64_t>(lastIteration510, initIteration510);
    remainderRange510 = range510 % numThreads;
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets510[0] = _barrierCodelets510(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets511* checkInCodelets511Ptr = (this->checkInCodelets511);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets511);
#endif
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->baseNumThreads;
         codeletCounter++) {
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets511Ptr) = _checkInCodelets511(2, 1, this, codeletCounter);
#else
        (*checkInCodelets511Ptr) = _checkInCodelets511(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets511Ptr).decDep();
        checkInCodelets511Ptr++;
    }
    this->dispatchCodelet(this->getID());
    *(in_ptrToThisTP) = this;
}
void TP510::dispatchCodelet(size_t codeletID)
{
    int idx = codeletID / this->baseNumThreads;
    int localID = codeletID - this->baseNumThreads * idx;
    this->checkInCodelets511[localID].setID(codeletID);
    this->checkInCodelets511[localID].setLocalID(localID);
    /*Check if we want to replicate codelets*/
    for (size_t i = 1; i < (size_t)DARTS_CODELETS_MULT; i++) {
#if USE_SPIN_CODELETS == 0
        this->checkInCodelets511[localID + this->baseNumThreads * i]
            = _checkInCodelets511(2, 1, this, localID + this->baseNumThreads * i);
#else
        this->checkInCodelets511[localID + this->baseNumThreads * i]
            = _checkInCodelets511(1, 1, this, localID + this->baseNumThreads * i);
#endif
        this->checkInCodelets511[localID + this->baseNumThreads * i].setID(codeletID);
        this->checkInCodelets511[localID + this->baseNumThreads * i].decDep();
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
TP510::~TP510()
{
    delete[] barrierCodelets510;
    delete[] checkInCodelets511;
}
/*TP541: OMPParallelDirective*/
void TP541::_barrierCodelets541::fire(void)
{
    TP541* myTP = static_cast<TP541*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
void TP541::_checkInCodelets544::fire(void)
{
    /*printOMPForCodeletFireCode, inlined: 0*/
    /*region 544 0*/
    /*Determine the TP to which this codelet belongs and check if this codelet spawns the TP or if
     * it signals it using dispatchCodelet()*/
    size_t idx = this->getID() / myTP->codeletsPerTP544;
    if (idx < myTP->TPsToUse544) {
        if (!__sync_val_compare_and_swap(&(myTP->TP544_alreadyLaunched[idx]), 0, 1)) {
            int64_t range = abs((*(this->inputsTPParent->nedge_darts541)) - 0) / 1;
            int64_t rangePerCodelet = range / myTP->TPsToUse544;
            int64_t minIteration = min<int64_t>((*(this->inputsTPParent->nedge_darts541)), 0);
            int64_t remainderRange = range % myTP->TPsToUse544;
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
            if (0 < (*(this->inputsTPParent->nedge_darts541))) {
                initIteration = min(initIteration, lastIteration);
                lastIteration = max(initIteration, lastIteration);
            } else {
                initIteration = max(initIteration, lastIteration);
                lastIteration = min(initIteration, lastIteration);
            }
            if (idx == myTP->TPsToUse544 - 1) {
                lastIteration = (*(this->inputsTPParent->nedge_darts541));
            }
#if USEINVOKE == 1
            invoke<TP544>(myTP, myTP->codeletsPerTP544 * DARTS_CODELETS_MULT, this->getID(), myTP,
                initIteration, lastIteration, &(*(this->inputsTPParent->IJ_darts541)),
                &(*(this->inputsTPParent->nedge_darts541)), &(myTP->TP544Ptr[idx]));
#else
            place<TP544>(idx, myTP, myTP->codeletsPerTP544 * DARTS_CODELETS_MULT, this->getID(),
                myTP, initIteration, lastIteration, &(*(this->inputsTPParent->IJ_darts541)),
                &(*(this->inputsTPParent->nedge_darts541)), &(myTP->TP544Ptr[idx]));
#endif
        } else {
            if (myTP->TP544Ptr[idx] != nullptr) {
                myTP->TP544Ptr[idx]->dispatchCodelet(this->getID());
            } else {
                this->resetCodelet();
                this->decDep();
            }
        }
    }
}
void TP541::_barrierCodelets544::fire(void)
{
    TP541* myTP = static_cast<TP541*>(myTP_);
    {
        for (size_t codeletsCounter = 0; codeletsCounter < myTP->numThreads; codeletsCounter++) {
            myTP->checkInCodelets587[codeletsCounter].decDep();
        }
    }
}
void TP541::_checkInCodelets587::fire(void)
{

    /*printing node 587: CallExpr*/
    if (!__sync_val_compare_and_swap(&(myTP->controlTPParent->TP587_alreadyLaunched), 0, 1)) {
/*Finding the next codelet after the function call*/
#if 0
/*Find and signal the next codelet*/
myTP->controlTPParent->TPParent->barrierCodelets541[0].decDep();
#endif
/*Finding the next sync point before making the function call*/
#if 0
/*Find the sync point for a function call*/
/*syncNode: OMPParallelDirective 541*/
/*Find the TP to which the nextRegion belongs*/
#endif
        /*Make the function call*/
        invoke<TP_pack_edges>(myTP, myTP->numThreads, this->getID(),
            &(myTP->controlTPParent->TPParent->barrierCodelets541[0]),
            &(myTP->controlTPParent->barrierCodelets541[0]), &(myTP->controlTPParent->TP587Ptr));
    } else {
        if (myTP->controlTPParent->TP587Ptr == nullptr) {
            myTP->add(this);
            return;
        } else {
            myTP->controlTPParent->TP587Ptr->nextCodeletspack_edges[this->getID()]
                = &(myTP->controlTPParent->TPParent->barrierCodelets541[0]);
            myTP->controlTPParent->TP587Ptr->nextSyncCodeletspack_edges[this->getID()]
                = &(myTP->controlTPParent->barrierCodelets541[0]);
#if USE_SPIN_CODELETS == 0
            myTP->controlTPParent->TP587Ptr->firstCodelet[this->getID()].decDep();
#else
            myTP->controlTPParent->TP587Ptr->availableCodelets[this->getID()] = 1;
#endif
        }
    }
}
TP541::TP541(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet,
    struct packed_edge* __restrict* in_IJ, int64_t* in_nedge)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , IJ_darts541(in_IJ) /*OMP_SHARED - INPUT*/
    , nedge_darts541(in_nedge) /*OMP_SHARED - INPUT*/
    , k_darts541(new int64_t[this->numThreads]) /*VARIABLE*/
    , TP544Ptr(new TP544*[NUMTPS544])
    , TP544_alreadyLaunched(new size_t[NUMTPS544])
    , numTPsSet544(0)
    , numTPsReady544(0)
    , TPsToUse544(NUMTPS544)
    , codeletsPerTP544(this->numThreads / NUMTPS544)
    , totalCodelets544(this->TPsToUse544 * this->codeletsPerTP544)
    , TP587Ptr(nullptr)
    , TP587_alreadyLaunched(0)
    , barrierCodelets541(new _barrierCodelets541[1])
    , checkInCodelets544(new _checkInCodelets544[this->numThreads])
    , barrierCodelets544(new _barrierCodelets544[1])
    , checkInCodelets587(new _checkInCodelets587[this->numThreads])
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets541[0] = _barrierCodelets541(ompNumThreads, ompNumThreads, this, 0);
    barrierCodelets544[0] = _barrierCodelets544(NUMTPS544, NUMTPS544, this, 0);
    _checkInCodelets587* checkInCodelets587Ptr = (this->checkInCodelets587);
    _checkInCodelets544* checkInCodelets544Ptr = (this->checkInCodelets544);
    for (int i = 0; i < NUMTPS544; i++) {
        TP544Ptr[i] = nullptr;
        TP544_alreadyLaunched[i] = 0;
    }
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets587Ptr) = _checkInCodelets587(1, 1, this, codeletCounter);
        checkInCodelets587Ptr++;
        (*checkInCodelets544Ptr) = _checkInCodelets544(1, 1, this, codeletCounter);
        (*checkInCodelets544Ptr).decDep();
        checkInCodelets544Ptr++;
    }
}
TP541::~TP541()
{
    delete[] k_darts541;
    delete[] barrierCodelets541;
    delete[] checkInCodelets587;
    delete[] barrierCodelets544;
    delete[] checkInCodelets544;
}
/*TP544: OMPForDirective*/
void TP544::_barrierCodelets544::fire(void)
{
    TP544* myTP = static_cast<TP544*>(myTP_);
    myTP->controlTPParent->TPParent->barrierCodelets544[0].decDep();
}
bool TP544::requestNewRangeIterations544(int64_t* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Static */
    /*Chunk = 0*/
    bool isThereNewIteration = false;
    {
        /*Static Scheduling*/
        int64_t tempStartRange = rangePerCodelet544 * codeletID;
        int64_t tempEndRange = rangePerCodelet544 * (codeletID + 1);
        if (remainderRange544 != 0) {
            if (codeletID < (uint32_t)remainderRange544) {
                tempStartRange += codeletID;
                tempEndRange += (codeletID + 1);
            } else {
                tempStartRange += remainderRange544;
                tempEndRange += remainderRange544;
            }
        }
        tempStartRange = tempStartRange * 1 + minIteration544;
        tempEndRange = tempEndRange * 1 + minIteration544;
        if (tempStartRange != tempEndRange) {
            isThereNewIteration = true;
        }
        if (initIteration544 < lastIteration544) {
            (this->inputsTPParent->k_darts544[codeletID]) = min(tempStartRange, tempEndRange);
            *endRange = max(tempStartRange, tempEndRange);
        } else {
            (this->inputsTPParent->k_darts544[codeletID]) = max(tempStartRange, tempEndRange);
            *endRange = min(tempStartRange, tempEndRange);
        }
        if (codeletID == this->numThreads - 1) {
            *endRange = lastIteration544;
        }
    }
    return isThereNewIteration;
}
void TP544::_checkInCodelets545::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    /*Init the vars for this region*/

    /*printing node 545: ForStmt*/
    /*var: IJ*/
    /*var: k*/
    /*var: nedge*/
    struct packed_edge* __restrict* IJ = (this->inputsTPParent->IJ_darts544);
    (void)IJ /*OMP_SHARED*/;
    int64_t* k = &(this->inputsTPParent->k_darts544[this->getLocalID()]);
    (void)k /*OMP_PRIVATE*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations544(
        (int64_t*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        /*Signaling omp for stmt's barrier*/
        myTP->controlTPParent->barrierCodelets544[0].decDep();
        return;
    }
    for (int64_t k_darts_counter_temp544 = (*k); k_darts_counter_temp544 < endRange
         && k_darts_counter_temp544 < this->inputsTPParent->lastIteration544;
         ++k_darts_counter_temp544) {
        {
            int64_t i = get_v0_from_edge(&(*(IJ))[(k_darts_counter_temp544)]);
            int64_t j = get_v1_from_edge(&(*(IJ))[(k_darts_counter_temp544)]);
            if (i >= 0 && j >= 0 && i != j) {
                scatter_edge(i, j);
                scatter_edge(j, i);
            }
        }
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling omp for stmt's barrier*/
    myTP->controlTPParent->barrierCodelets544[0].decDep();
}
TP544::TP544(int in_numThreads, int in_mainCodeletID, TP541* in_TPParent, int64_t in_initIteration,
    int64_t in_lastIteration, struct packed_edge* __restrict* in_IJ, int64_t* in_nedge,
    TP544** in_ptrToThisTP)
    : ompTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , IJ_darts544(in_IJ) /*OMP_SHARED - INPUT*/
    , k_darts544(new int64_t[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , nedge_darts544(in_nedge) /*OMP_SHARED - INPUT*/
    , initIteration544(in_initIteration)
    , lastIteration544(in_lastIteration)
    , readyCodelets(0)
    , baseNumThreads(this->numThreads / DARTS_CODELETS_MULT)
    , barrierCodelets544(new _barrierCodelets544[1])
    , checkInCodelets545(new _checkInCodelets545[this->numThreads])
{
    /*Initialize the loop parameters*/
    range544 = abs(lastIteration544 - initIteration544) / 1;
    rangePerCodelet544 = range544 / numThreads;
    minIteration544 = min<int64_t>(lastIteration544, initIteration544);
    remainderRange544 = range544 % numThreads;
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets544[0] = _barrierCodelets544(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets545* checkInCodelets545Ptr = (this->checkInCodelets545);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets545);
#endif
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->baseNumThreads;
         codeletCounter++) {
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets545Ptr) = _checkInCodelets545(2, 1, this, codeletCounter);
#else
        (*checkInCodelets545Ptr) = _checkInCodelets545(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets545Ptr).decDep();
        checkInCodelets545Ptr++;
    }
    this->dispatchCodelet(this->getID());
    *(in_ptrToThisTP) = this;
}
void TP544::dispatchCodelet(size_t codeletID)
{
    int idx = codeletID / this->baseNumThreads;
    int localID = codeletID - this->baseNumThreads * idx;
    this->checkInCodelets545[localID].setID(codeletID);
    this->checkInCodelets545[localID].setLocalID(localID);
    /*Check if we want to replicate codelets*/
    for (size_t i = 1; i < (size_t)DARTS_CODELETS_MULT; i++) {
#if USE_SPIN_CODELETS == 0
        this->checkInCodelets545[localID + this->baseNumThreads * i]
            = _checkInCodelets545(2, 1, this, localID + this->baseNumThreads * i);
#else
        this->checkInCodelets545[localID + this->baseNumThreads * i]
            = _checkInCodelets545(1, 1, this, localID + this->baseNumThreads * i);
#endif
        this->checkInCodelets545[localID + this->baseNumThreads * i].setID(codeletID);
        this->checkInCodelets545[localID + this->baseNumThreads * i].decDep();
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
TP544::~TP544()
{
    delete[] barrierCodelets544;
    delete[] checkInCodelets545;
}
/*TP644: OMPParallelDirective*/
void TP644::_barrierCodelets644::fire(void)
{
    TP644* myTP = static_cast<TP644*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
void TP644::_checkInCodelets646::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 646: DeclStmt*/

    /*printing node 647: DeclStmt*/
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 646 nextRegion: 648 */
    myTP->controlTPParent->checkInCodelets648[this->getID()].decDep();
}
void TP644::_checkInCodelets648_core::fire(void)
{
    TP644* myTP = static_cast<TP644*>(myTP_);
    (void)myTP;
    int64_t startRange = (this->initIteration);
    int64_t endRange = (this->lastIteration);
    /*var: bfs_tree*/
    /*var: k*/
    /*var: k1*/
    /*var: k2*/
    /*var: kbuf*/
    /*var: nbuf*/
    /*var: oldk2*/
    /*var: vlist*/
    int64_t nbuf[16384];
    int64_t kbuf = 0;
    {
        for (int64_t k = startRange; k < endRange; ++k) {
            {
                const int64_t v = (*(vlist))[k];
                const int64_t veo = (xoff[1 + 2 * (v)]);
                int64_t vo;
                for (vo = (xoff[2 * (v)]); vo < veo; ++vo) {
                    const int64_t j = xadj[vo];
                    if ((*(bfs_tree))[j] == -1) {
                        if (int64_cas(&(*(bfs_tree))[j], -1, v)) {
                            if (kbuf < 16384) {
                                nbuf[kbuf++] = j;
                            } else {
                                int64_t voff = int64_fetch_add(&(*(k2)), 16384), vk;
                                for (vk = 0; vk < 16384; ++vk)
                                    (*(vlist))[voff + vk] = nbuf[vk];
                                nbuf[0] = j;
                                kbuf = 1;
                            }
                        }
                    }
                }
            }
        }
    }
    if (kbuf) {
        int64_t voff = int64_fetch_add(&(*(k2)), kbuf), vk;
        for (vk = 0; vk < kbuf; ++vk)
            (*(vlist))[voff + vk] = nbuf[vk];
    }
    this->toSignal->decDep();
}
void TP644::_checkInCodelets648::fire(void)
{
    /*printOMPForCodeletFireCode, inlined: 0*/
    /*region 648 0*/
    int64_t range
        = abs((*(this->inputsTPParent->oldk2_darts644)) - (*(this->inputsTPParent->k1_darts644)));
    unsigned int workers = (ompNumThreads / NUMTPS - 1) + 1;
    if (range < dartsTPLoopTH0) {
        _checkInCodelets648_core* tempCodeletCore = new _checkInCodelets648_core(1, 1, myTP,
            &(myTP->barrierCodelets648[0]), (*(this->inputsTPParent->k1_darts644)),
            (*(this->inputsTPParent->oldk2_darts644)),
            &(*(this->inputsTPParent->bfs_tree_darts644)), &(*(this->inputsTPParent->k1_darts644)),
            &(*(this->inputsTPParent->k2_darts644)), &(*(this->inputsTPParent->oldk2_darts644)),
            &(*(this->inputsTPParent->vlist_darts644)));
        myTP->masterCodeletTP648_core = tempCodeletCore;
        myTP->masterCodeletTP648_core[0].decDep();
    } else if (range < dartsTPLoopTH1) {
        uint64_t outerTile = (range / workers) ? (range / workers) : range;
        codeletFor<TP648_loop>* tempCodeletFor = new codeletFor<TP648_loop>(1, 1, myTP, LONGWAIT,
            &(myTP->barrierCodelets648[0]), 1, outerTile, (*(this->inputsTPParent->k1_darts644)),
            (*(this->inputsTPParent->oldk2_darts644)),
            &(*(this->inputsTPParent->bfs_tree_darts644)), &(*(this->inputsTPParent->k1_darts644)),
            &(*(this->inputsTPParent->k2_darts644)), &(*(this->inputsTPParent->oldk2_darts644)),
            &(*(this->inputsTPParent->vlist_darts644)));
        tempCodeletFor->setIterations((range / outerTile + ((range % outerTile) ? 1 : 0)));
        myTP->masterCodeletTP648_loop = tempCodeletFor;
        myTP->masterCodeletTP648_loop[0].decDep();
    } else {
        uint64_t outerTile = (range / (NUMTPS * 2));
        if (outerTile == 0) {
            outerTile = (range / NUMTPS);
            if (outerTile == 0)
                outerTile = range;
        }
        paraFor<TP648_outer>* tempParaFor = new paraFor<TP648_outer>(1, 1, myTP, LONGWAIT,
            &(myTP->barrierCodelets648[0]), 1, outerTile, workers,
            (*(this->inputsTPParent->k1_darts644)), (*(this->inputsTPParent->oldk2_darts644)),
            &(*(this->inputsTPParent->bfs_tree_darts644)), &(*(this->inputsTPParent->k1_darts644)),
            &(*(this->inputsTPParent->k2_darts644)), &(*(this->inputsTPParent->oldk2_darts644)),
            &(*(this->inputsTPParent->vlist_darts644)));
        tempParaFor->setIterations((range / outerTile + ((range % outerTile) ? 1 : 0)));
        myTP->masterCodeletTP648_outer = tempParaFor;
        myTP->masterCodeletTP648_outer[0].decDep();
    }
}
void TP644::_barrierCodelets648::fire(void)
{
    TP644* myTP = static_cast<TP644*>(myTP_);
    {
        for (size_t codeletsCounter = 0; codeletsCounter < myTP->numThreads; codeletsCounter++) {
            myTP->checkInCodelets721[codeletsCounter].decDep();
        }
    }
}
void TP644::_checkInCodelets721::fire(void)
{

    /*printing node 721: IfStmt*/
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets644[0].decDep();
}
TP644::TP644(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet,
    int64_t* __restrict* in_bfs_tree, int64_t* in_k, int64_t* in_k1, int64_t* in_k2,
    int64_t* in_oldk2, int64_t* __restrict* in_vlist)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , bfs_tree_darts644(in_bfs_tree) /*OMP_SHARED - INPUT*/
    , k_darts644(in_k) /*OMP_SHARED - INPUT*/
    , k1_darts644(in_k1) /*OMP_SHARED - INPUT*/
    , k2_darts644(in_k2) /*OMP_SHARED - INPUT*/
    , oldk2_darts644(in_oldk2) /*OMP_SHARED - INPUT*/
    , vlist_darts644(in_vlist) /*OMP_SHARED - INPUT*/
    , TP648Ptr(new TP648*[NUMTPS648])
    , TP648_alreadyLaunched(new size_t[NUMTPS648])
    , numTPsSet648(0)
    , numTPsReady648(0)
    , TPsToUse648(NUMTPS648)
    , codeletsPerTP648(this->numThreads / NUMTPS648)
    , totalCodelets648(this->TPsToUse648 * this->codeletsPerTP648)
    , barrierCodelets644(new _barrierCodelets644[1])
    , checkInCodelets646(new _checkInCodelets646[this->numThreads])
    , checkInCodelets648(new _checkInCodelets648[this->numThreads])
    , masterCodeletTP648_core(new _checkInCodelets648_core[1])
    , masterCodeletTP648_loop(new codeletFor<TP648_loop>[1])
    , masterCodeletTP648_outer(new paraFor<TP648_outer>[1])
    , barrierCodelets648(new _barrierCodelets648[1])
    , checkInCodelets721(new _checkInCodelets721[this->numThreads])
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets644[0] = _barrierCodelets644(1, 1, this, 0);
    barrierCodelets648[0] = _barrierCodelets648(1, 1, this, 0);
    _checkInCodelets721* checkInCodelets721Ptr = (this->checkInCodelets721);
    _checkInCodelets648* checkInCodelets648Ptr = (this->checkInCodelets648);
    _checkInCodelets646* checkInCodelets646Ptr = (this->checkInCodelets646);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets721Ptr) = _checkInCodelets721(1, 1, this, codeletCounter);
        checkInCodelets721Ptr++;
        (*checkInCodelets648Ptr) = _checkInCodelets648(1, 1, this, codeletCounter);
        checkInCodelets648Ptr++;
        (*checkInCodelets646Ptr) = _checkInCodelets646(1, 1, this, codeletCounter);
        (*checkInCodelets646Ptr).decDep();
        checkInCodelets646Ptr++;
    }
}
TP644::~TP644()
{
    delete[] barrierCodelets644;
    delete[] checkInCodelets721;
    delete[] barrierCodelets648;
    delete[] checkInCodelets648;
    delete[] checkInCodelets646;
}
/*TP648: OMPForDirective*/
void TP648_loop::_checkInCodelets649::fire(void)
{
    TP648_loop* myTP = static_cast<TP648_loop*>(myTP_);
    (void)myTP;
    int64_t startRange = (myTP->iter) * (myTP->tileSize) + (myTP->initIteration);
    int64_t endRange
        = min((int64_t)(startRange + (myTP->tileSize)), (int64_t)(myTP->lastIteration));
    /*var: bfs_tree*/
    /*var: k*/
    /*var: k1*/
    /*var: k2*/
    /*var: kbuf*/
    /*var: nbuf*/
    /*var: oldk2*/
    /*var: vlist*/
    int64_t nbuf[16384];
    int64_t kbuf = 0;
    {
        for (int64_t k = startRange; k < endRange; ++k) {
            {
                const int64_t v = (*(vlist))[k];
                const int64_t veo = (xoff[1 + 2 * (v)]);
                int64_t vo;
                for (vo = (xoff[2 * (v)]); vo < veo; ++vo) {
                    const int64_t j = xadj[vo];
                    if ((*(bfs_tree))[j] == -1) {
                        if (int64_cas(&(*(bfs_tree))[j], -1, v)) {
                            if (kbuf < 16384) {
                                nbuf[kbuf++] = j;
                            } else {
                                int64_t voff = int64_fetch_add(&(*(k2)), 16384), vk;
                                for (vk = 0; vk < 16384; ++vk)
                                    (*(vlist))[voff + vk] = nbuf[vk];
                                nbuf[0] = j;
                                kbuf = 1;
                            }
                        }
                    }
                }
            }
        }
    }
    if (kbuf) {
        int64_t voff = int64_fetch_add(&(*(k2)), kbuf), vk;
        for (vk = 0; vk < kbuf; ++vk)
            (*(vlist))[voff + vk] = nbuf[vk];
    }
    myTP->toSignal->decDep();
}
void TP648_outer::TP648_inner::_checkInCodelets649::fire(void)
{
    TP648_outer::TP648_inner* myTP = static_cast<TP648_outer::TP648_inner*>(myTP_);
    (void)myTP;
    int64_t startRange = myTP->outerIteration * myTP->outerTile
        + myTP->innerIteration * myTP->innerTile + myTP->initIteration;
    int64_t endRange = min(min(startRange + myTP->innerTile, myTP->lastIteration),
        myTP->initIteration + (myTP->outerIteration + 1) * myTP->outerTile);
    /*var: bfs_tree*/
    /*var: k*/
    /*var: k1*/
    /*var: k2*/
    /*var: kbuf*/
    /*var: nbuf*/
    /*var: oldk2*/
    /*var: vlist*/
    int64_t nbuf[16384];
    int64_t kbuf = 0;
    {
        for (int64_t k = startRange; k < endRange; ++k) {
            {
                const int64_t v = (*(vlist))[k];
                const int64_t veo = (xoff[1 + 2 * (v)]);
                int64_t vo;
                for (vo = (xoff[2 * (v)]); vo < veo; ++vo) {
                    const int64_t j = xadj[vo];
                    if ((*(bfs_tree))[j] == -1) {
                        if (int64_cas(&(*(bfs_tree))[j], -1, v)) {
                            if (kbuf < 16384) {
                                nbuf[kbuf++] = j;
                            } else {
                                int64_t voff = int64_fetch_add(&(*(k2)), 16384), vk;
                                for (vk = 0; vk < 16384; ++vk)
                                    (*(vlist))[voff + vk] = nbuf[vk];
                                nbuf[0] = j;
                                kbuf = 1;
                            }
                        }
                    }
                }
            }
        }
    }
    if (kbuf) {
        int64_t voff = int64_fetch_add(&(*(k2)), kbuf), vk;
        for (vk = 0; vk < kbuf; ++vk)
            (*(vlist))[voff + vk] = nbuf[vk];
    }
    myTP->toSignal->decDep();
}
TP648_loop::TP648_loop(unsigned int it, darts::Codelet* in_nextCodelet, unsigned int in_tileSize,
    int64_t in_initIteration, int64_t in_lastIteration, int64_t* __restrict* in_bfs_tree,
    int64_t* in_k1, int64_t* in_k2, int64_t* in_oldk2, int64_t* __restrict* in_vlist)
    : loop(it, in_nextCodelet)
    , tileSize(in_tileSize)
    , initIteration(in_initIteration)
    , lastIteration(in_lastIteration)
    , checkInCodelets649(new _checkInCodelets649[1])
{
    this->checkInCodelets649[0]
        = _checkInCodelets649(1, 1, this, in_bfs_tree, in_k1, in_k2, in_oldk2, in_vlist);
    this->checkInCodelets649[0].decDep();
}
TP648_outer::TP648_inner::TP648_inner(unsigned int it, darts::Codelet* in_nextCodelet,
    unsigned int in_outerIteration, int64_t in_outerTile, int64_t in_innerTile,
    int64_t in_initIteration, int64_t in_lastIteration, int64_t* __restrict* in_bfs_tree,
    int64_t* in_k1, int64_t* in_k2, int64_t* in_oldk2, int64_t* __restrict* in_vlist)
    : loop(it, in_nextCodelet)
    , outerIteration(in_outerIteration)
    , innerIteration(it)
    , outerTile(in_outerTile)
    , innerTile(in_innerTile)
    , initIteration(in_initIteration)
    , lastIteration(in_lastIteration)
    , checkInCodelets649(new _checkInCodelets649[1])
{
    this->checkInCodelets649[0]
        = _checkInCodelets649(1, 1, this, in_bfs_tree, in_k1, in_k2, in_oldk2, in_vlist);
    this->checkInCodelets649[0].decDep();
}
TP648_outer::TP648_outer(unsigned int it, darts::Codelet* in_nextCodelet, int64_t in_outerTile,
    unsigned int in_workers, int64_t in_initIteration, int64_t in_lastIteration,
    int64_t* __restrict* in_bfs_tree, int64_t* in_k1, int64_t* in_k2, int64_t* in_oldk2,
    int64_t* __restrict* in_vlist)
    : loop(it, in_nextCodelet)
    , outerTile(in_outerTile)
    , workers(in_workers)
    , initIteration(in_initIteration)
    , lastIteration(in_lastIteration)
    , masterCodeletTP648_inner(new codeletFor<TP648_inner>[1])
{
    this->width = min(((this->initIteration) + it * (this->outerTile) + (this->outerTile)),
                      this->lastIteration)
        - (it * (this->outerTile) + this->initIteration);
    if (this->width / (this->workers * 2))
        this->innerTile = (this->width / (this->workers * 2));
    else if (this->width / this->workers)
        this->innerTile = (this->width / this->workers);
    else
        this->innerTile = this->width;
    uint64_t outerIteration
        = (this->width / this->innerTile) + ((this->width % this->innerTile) ? 1 : 0);
    codeletFor<TP648_outer::TP648_inner>* tempCodeletFor = new codeletFor<TP648_outer::TP648_inner>(
        1, 1, this, LONGWAIT, in_nextCodelet, outerIteration, it, this->outerTile, this->innerTile,
        this->initIteration, this->lastIteration, in_bfs_tree, in_k1, in_k2, in_oldk2, in_vlist);
    this->masterCodeletTP648_inner = tempCodeletFor;
    this->masterCodeletTP648_inner[0].decDep();
}
TP648_loop::~TP648_loop() { delete[] checkInCodelets649; }
TP648_outer::TP648_inner::~TP648_inner() { delete[] checkInCodelets649; }
TP648_outer::~TP648_outer() { delete masterCodeletTP648_inner; }
