#include "Main.output.darts.h"
using namespace darts;
using namespace std;
/*Function: main, ID: 1*/
int main(int argc, char* argv[])
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
    /*main:1*/
    /*CompoundStmt:1059*/
    int version = 11;
    int mype = 0;
    int max_procs = omp_get_num_procs();
    int n_isotopes, n_gridpoints, lookups, i, thread, nthreads, mat;
    unsigned long seed;
    double omp_start, omp_end, p_energy;
    char* HM;
    unsigned long long vhash = 0;
    srand(time(((void*)0)));
    Inputs input = read_CLI(argc, argv);
    nthreads = input.nthreads;
    n_isotopes = input.n_isotopes;
    n_gridpoints = input.n_gridpoints;
    lookups = input.lookups;
    HM = input.HM;
    omp_set_num_threads(nthreads);
    size_t single_nuclide_grid = n_gridpoints * sizeof(NuclideGridPoint);
    size_t all_nuclide_grids = n_isotopes * single_nuclide_grid;
    size_t size_GridPoint = sizeof(GridPoint) + n_isotopes * sizeof(int);
    size_t size_UEG = n_isotopes * n_gridpoints * size_GridPoint;
    size_t memtotal;
    int mem_tot;
    memtotal = all_nuclide_grids + size_UEG;
    all_nuclide_grids = all_nuclide_grids / 1048576;
    size_UEG = size_UEG / 1048576;
    memtotal = memtotal / 1048576;
    mem_tot = memtotal;
    if (mype == 0) {
        logo(version);
        center_print("INPUT SUMMARY", 79);
        border_print();
        printf("Materials:                    %d\n", 12);
        printf("H-M Benchmark Size:           %s\n", HM);
        printf("Total Nuclides:               %d\n", n_isotopes);
        printf("Gridpoints (per Nuclide):     ");
        fancy_int(n_gridpoints);
        printf("Unionized Energy Gridpoints:  ");
        fancy_int(n_isotopes * n_gridpoints);
        printf("XS Lookups:                   ");
        fancy_int(lookups);
        printf("Threads:                      %d\n", nthreads);
        printf("Est. Memory Usage (MB):       ");
        fancy_int(mem_tot);
        if (0 > 0)
            printf("Extra Flops:                  %d\n", 0);
        if (0 > 0)
            printf("Extra Loads:                  %d\n", 0);
        border_print();
        center_print("INITIALIZATION", 79);
        border_print();
    }
    if (mype == 0)
        printf("Generating Nuclide Energy Grids...\n");
    NuclideGridPoint** nuclide_grids = gpmatrix(n_isotopes, n_gridpoints);
    generate_grids(nuclide_grids, n_isotopes, n_gridpoints);
    if (mype == 0)
        printf("Sorting Nuclide Energy Grids...\n");
    sort_nuclide_grids(nuclide_grids, n_isotopes, n_gridpoints);
    GridPoint* energy_grid = generate_energy_grid(n_isotopes, n_gridpoints, nuclide_grids);
    set_grid_ptrs(energy_grid, nuclide_grids, n_isotopes, n_gridpoints);
    if (mype == 0)
        printf("Loading Mats...\n");
    int* num_nucs = load_num_nucs(n_isotopes);
    int** mats = load_mats(num_nucs, n_isotopes);
    double** concs = load_concs(num_nucs);
    if (mype == 0) {
        printf("\n");
        border_print();
        center_print("SIMULATION", 79);
        border_print();
    }
    omp_start = omp_get_wtime();
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP1173>(ompNumThreads * DARTS_CODELETS_MULT, 0,
            RuntimeFinalCodelet, (double***)&((concs)), (GridPoint**)&((energy_grid)),
            (int*)&((lookups)), (int***)&((mats)), (int*)&((mype)), (int*)&((n_gridpoints)),
            (int*)&((n_isotopes)), (int*)&((nthreads)), (NuclideGridPoint***)&((nuclide_grids)),
            (int**)&((num_nucs))));
    }
    if (mype == 0) {
        printf("\n");
        printf("Simulation complete.\n");
    }
    omp_end = omp_get_wtime();
    int lookups_per_sec = (int)((double)lookups / (omp_end - omp_start));
    if (mype == 0) {
        border_print();
        center_print("RESULTS", 79);
        border_print();
        printf("Threads:     %d\n", nthreads);
        if (0 > 0)
            printf("Extra Flops: %d\n", 0);
        if (0 > 0)
            printf("Extra Loads: %d\n", 0);
        printf("Runtime:     %.3lf seconds\n", omp_end - omp_start);
        printf("Lookups:     ");
        fancy_int(lookups);
        printf("Lookups/s:   ");
        fancy_int(lookups_per_sec);
        border_print();
        if (1) {
            FILE* out = fopen("results.txt", "a");
            fprintf(out, "%d\t%d\n", nthreads, lookups_per_sec);
            fclose(out);
        }
    }
    return 0;
}
/*TP1173: OMPParallelDirective*/
void TP1173::_barrierCodelets1173::fire(void)
{
    TP1173* myTP = static_cast<TP1173*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
void TP1173::_checkInCodelets1175::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 1175: DeclStmt*/
    this->inputsTPParent->macro_xs_vector_darts1173[this->getID()]
        = (double*)malloc(5 * sizeof(double));

    /*printing node 1180: BinaryOperator*/
    (this->inputsTPParent->thread_darts1173[this->getID()]) = omp_get_thread_num();

    /*printing node 1182: BinaryOperator*/
    (this->inputsTPParent->seed_darts1173[this->getID()])
        = ((this->inputsTPParent->thread_darts1173[this->getID()]) + 1) * 19 + 17;
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 1175 nextRegion: 1186 */
    myTP->controlTPParent->checkInCodelets1186[this->getID()].decDep();
}
void TP1173::_checkInCodelets1186::fire(void)
{
    /*printOMPForCodeletFireCode, inlined: 0*/
    /*region 1186 0*/
    /*Determine the TP to which this codelet belongs and check if this codelet spawns the TP or if
     * it signals it using dispatchCodelet()*/
    size_t idx = this->getID() / myTP->codeletsPerTP1186;
    if (idx < myTP->TPsToUse1186) {
        if (!__sync_val_compare_and_swap(&(myTP->TP1186_alreadyLaunched[idx]), 0, 1)) {
            int range = abs((*(this->inputsTPParent->lookups_darts1173)) - 0) / 1;
            int rangePerCodelet = range / myTP->TPsToUse1186;
            int minIteration = min<int>((*(this->inputsTPParent->lookups_darts1173)), 0);
            int remainderRange = range % myTP->TPsToUse1186;
            int initIteration = rangePerCodelet * idx;
            int lastIteration = rangePerCodelet * (idx + 1);
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
            if (0 < (*(this->inputsTPParent->lookups_darts1173))) {
                initIteration = min(initIteration, lastIteration);
                lastIteration = max(initIteration, lastIteration);
            } else {
                initIteration = max(initIteration, lastIteration);
                lastIteration = min(initIteration, lastIteration);
            }
            if (idx == myTP->TPsToUse1186 - 1) {
                lastIteration = (*(this->inputsTPParent->lookups_darts1173));
            }
#if USEINVOKE == 1
            invoke<TP1186>(myTP, myTP->codeletsPerTP1186 * DARTS_CODELETS_MULT, this->getID(), myTP,
                initIteration, lastIteration, &(*(this->inputsTPParent->concs_darts1173)),
                &(*(this->inputsTPParent->energy_grid_darts1173)),
                &(*(this->inputsTPParent->lookups_darts1173)),
                &(*(this->inputsTPParent->mats_darts1173)),
                &(*(this->inputsTPParent->mype_darts1173)),
                &(*(this->inputsTPParent->n_gridpoints_darts1173)),
                &(*(this->inputsTPParent->n_isotopes_darts1173)),
                &(*(this->inputsTPParent->nthreads_darts1173)),
                &(*(this->inputsTPParent->nuclide_grids_darts1173)),
                &(*(this->inputsTPParent->num_nucs_darts1173)), &(myTP->TP1186Ptr[idx]));
#else
            place<TP1186>(idx, myTP, myTP->codeletsPerTP1186 * DARTS_CODELETS_MULT, this->getID(),
                myTP, initIteration, lastIteration, &(*(this->inputsTPParent->concs_darts1173)),
                &(*(this->inputsTPParent->energy_grid_darts1173)),
                &(*(this->inputsTPParent->lookups_darts1173)),
                &(*(this->inputsTPParent->mats_darts1173)),
                &(*(this->inputsTPParent->mype_darts1173)),
                &(*(this->inputsTPParent->n_gridpoints_darts1173)),
                &(*(this->inputsTPParent->n_isotopes_darts1173)),
                &(*(this->inputsTPParent->nthreads_darts1173)),
                &(*(this->inputsTPParent->nuclide_grids_darts1173)),
                &(*(this->inputsTPParent->num_nucs_darts1173)), &(myTP->TP1186Ptr[idx]));
#endif
        } else {
            if (myTP->TP1186Ptr[idx] != nullptr) {
                myTP->TP1186Ptr[idx]->dispatchCodelet(this->getID());
            } else {
                this->resetCodelet();
                this->decDep();
            }
        }
    }
}
void TP1173::_barrierCodelets1186::fire(void)
{
    TP1173* myTP = static_cast<TP1173*>(myTP_);
    myTP->TPParent->barrierCodelets1173[0].setDep(0);
    myTP->add(&(myTP->TPParent->barrierCodelets1173[0]));
}
TP1173::TP1173(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet, double*** in_concs,
    GridPoint** in_energy_grid, int* in_lookups, int*** in_mats, int* in_mype, int* in_n_gridpoints,
    int* in_n_isotopes, int* in_nthreads, NuclideGridPoint*** in_nuclide_grids, int** in_num_nucs)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , concs_darts1173(in_concs) /*OMP_SHARED - INPUT*/
    , energy_grid_darts1173(in_energy_grid) /*OMP_SHARED - INPUT*/
    , i_darts1173(new int[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , lookups_darts1173(in_lookups) /*OMP_SHARED - INPUT*/
    , mat_darts1173(new int[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , mats_darts1173(in_mats) /*OMP_SHARED - INPUT*/
    , mype_darts1173(in_mype) /*OMP_SHARED - INPUT*/
    , n_gridpoints_darts1173(in_n_gridpoints) /*OMP_SHARED - INPUT*/
    , n_isotopes_darts1173(in_n_isotopes) /*OMP_SHARED - INPUT*/
    , nthreads_darts1173(in_nthreads) /*OMP_SHARED - INPUT*/
    , nuclide_grids_darts1173(in_nuclide_grids) /*OMP_SHARED - INPUT*/
    , num_nucs_darts1173(in_num_nucs) /*OMP_SHARED - INPUT*/
    , p_energy_darts1173(new double[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , seed_darts1173(new unsigned long[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , thread_darts1173(new int[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , macro_xs_vector_darts1173(new double*[this->numThreads]) /*VARIABLE*/
    , TP1186Ptr(new TP1186*[NUMTPS1186])
    , TP1186_alreadyLaunched(new size_t[NUMTPS1186])
    , numTPsSet1186(0)
    , numTPsReady1186(0)
    , TPsToUse1186(NUMTPS1186)
    , codeletsPerTP1186(this->numThreads / NUMTPS1186)
    , totalCodelets1186(this->TPsToUse1186 * this->codeletsPerTP1186)
    , barrierCodelets1173(new _barrierCodelets1173[1])
    , checkInCodelets1175(new _checkInCodelets1175[this->numThreads])
    , checkInCodelets1186(new _checkInCodelets1186[this->numThreads])
    , barrierCodelets1186(new _barrierCodelets1186[1])
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets1173[0] = _barrierCodelets1173(ompNumThreads, ompNumThreads, this, 0);
    barrierCodelets1186[0] = _barrierCodelets1186(NUMTPS1186, NUMTPS1186, this, 0);
    _checkInCodelets1186* checkInCodelets1186Ptr = (this->checkInCodelets1186);
    for (int i = 0; i < NUMTPS1186; i++) {
        TP1186Ptr[i] = nullptr;
        TP1186_alreadyLaunched[i] = 0;
    }
    _checkInCodelets1175* checkInCodelets1175Ptr = (this->checkInCodelets1175);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets1186Ptr) = _checkInCodelets1186(1, 1, this, codeletCounter);
        checkInCodelets1186Ptr++;
        (*checkInCodelets1175Ptr) = _checkInCodelets1175(1, 1, this, codeletCounter);
        (*checkInCodelets1175Ptr).decDep();
        checkInCodelets1175Ptr++;
    }
}
TP1173::~TP1173()
{
    delete[] macro_xs_vector_darts1173;
    delete[] barrierCodelets1173;
    delete[] barrierCodelets1186;
    delete[] checkInCodelets1186;
    delete[] checkInCodelets1175;
}
/*TP1186: OMPForDirective*/
void TP1186::_barrierCodelets1186::fire(void)
{
    TP1186* myTP = static_cast<TP1186*>(myTP_);
    myTP->controlTPParent->TPParent->barrierCodelets1186[0].decDep();
}
bool TP1186::requestNewRangeIterations1186(int* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Dynamic */
    /*Chunk = 1*/
    bool isThereNewIteration = false;
    {
        /*Dynamic Scheduling*/
        if (initIteration1186 < lastIteration1186) {
            (this->inputsTPParent->i_darts1186[codeletID])
                = __sync_fetch_and_add(&(nextIteration1186), 1 * 1);
            *endRange = (this->inputsTPParent->i_darts1186[codeletID]) + 1 * 1;
        } else {
            (this->inputsTPParent->i_darts1186[codeletID])
                = __sync_fetch_and_sub(&(nextIteration1186), 1 * 1);
            *endRange = (this->inputsTPParent->i_darts1186[codeletID]) - 1 * 1;
        }
        if ((this->inputsTPParent->i_darts1186[codeletID]) < lastIteration1186) {
            isThereNewIteration = true;
        }
    }
    return isThereNewIteration;
}
void TP1186::_checkInCodelets1187::fire(void)
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
    this->inputsTPParent->macro_xs_vector_darts1186[this->getLocalID()]
        = (double**)&(myTP->TPParent->inputsTPParent->macro_xs_vector_darts1173[this->getID()]);
    /*Get pointer from parent for variable
     with shared scope in this region but private
     in the enclosing one.*/
    this->inputsTPParent->mat_darts1186[this->getLocalID()]
        = (int*)&(myTP->TPParent->inputsTPParent->mat_darts1173[this->getID()]);
    /*Get pointer from parent for variable
     with shared scope in this region but private
     in the enclosing one.*/
    this->inputsTPParent->p_energy_darts1186[this->getLocalID()]
        = (double*)&(myTP->TPParent->inputsTPParent->p_energy_darts1173[this->getID()]);
    /*Get pointer from parent for variable
     with shared scope in this region but private
     in the enclosing one.*/
    this->inputsTPParent->seed_darts1186[this->getLocalID()]
        = (unsigned long*)&(myTP->TPParent->inputsTPParent->seed_darts1173[this->getID()]);
    /*Get pointer from parent for variable
     with shared scope in this region but private
     in the enclosing one.*/
    this->inputsTPParent->thread_darts1186[this->getLocalID()]
        = (int*)&(myTP->TPParent->inputsTPParent->thread_darts1173[this->getID()]);

    /*printing node 1187: ForStmt*/
    /*var: concs*/
    /*var: energy_grid*/
    /*var: i*/
    /*var: lookups*/
    /*var: macro_xs_vector*/
    /*var: mat*/
    /*var: mats*/
    /*var: mype*/
    /*var: n_gridpoints*/
    /*var: n_isotopes*/
    /*var: nthreads*/
    /*var: nuclide_grids*/
    /*var: num_nucs*/
    /*var: p_energy*/
    /*var: seed*/
    /*var: thread*/
    double*** concs = (this->inputsTPParent->concs_darts1186);
    (void)concs /*OMP_SHARED*/;
    GridPoint** energy_grid = (this->inputsTPParent->energy_grid_darts1186);
    (void)energy_grid /*OMP_SHARED*/;
    int* i = &(this->inputsTPParent->i_darts1186[this->getLocalID()]);
    (void)i /*OMP_PRIVATE*/;
    int* lookups = (this->inputsTPParent->lookups_darts1186);
    (void)lookups /*OMP_SHARED*/;
    double*** macro_xs_vector
        = &(this->inputsTPParent->macro_xs_vector_darts1186[this->getLocalID()]);
    (void)macro_xs_vector /*OMP_SHARED_PRIVATE*/;
    int** mat = &(this->inputsTPParent->mat_darts1186[this->getLocalID()]);
    (void)mat /*OMP_SHARED_PRIVATE*/;
    int*** mats = (this->inputsTPParent->mats_darts1186);
    (void)mats /*OMP_SHARED*/;
    int* mype = (this->inputsTPParent->mype_darts1186);
    (void)mype /*OMP_SHARED*/;
    int* n_gridpoints = (this->inputsTPParent->n_gridpoints_darts1186);
    (void)n_gridpoints /*OMP_SHARED*/;
    int* n_isotopes = (this->inputsTPParent->n_isotopes_darts1186);
    (void)n_isotopes /*OMP_SHARED*/;
    int* nthreads = (this->inputsTPParent->nthreads_darts1186);
    (void)nthreads /*OMP_SHARED*/;
    NuclideGridPoint*** nuclide_grids = (this->inputsTPParent->nuclide_grids_darts1186);
    (void)nuclide_grids /*OMP_SHARED*/;
    int** num_nucs = (this->inputsTPParent->num_nucs_darts1186);
    (void)num_nucs /*OMP_SHARED*/;
    double** p_energy = &(this->inputsTPParent->p_energy_darts1186[this->getLocalID()]);
    (void)p_energy /*OMP_SHARED_PRIVATE*/;
    unsigned long** seed = &(this->inputsTPParent->seed_darts1186[this->getLocalID()]);
    (void)seed /*OMP_SHARED_PRIVATE*/;
    int** thread = &(this->inputsTPParent->thread_darts1186[this->getLocalID()]);
    (void)thread /*OMP_SHARED_PRIVATE*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations1186(
        (int*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        /*Signaling omp for stmt's barrier*/
        myTP->controlTPParent->barrierCodelets1186[0].decDep();
        return;
    }
    while (isThereNewIteration) {
        for (int i_darts_counter_temp1186 = (*i); i_darts_counter_temp1186 < endRange
             && i_darts_counter_temp1186 < this->inputsTPParent->lastIteration1186;
             i_darts_counter_temp1186++) {
            {
                if (1 && (*(mype)) == 0 && (*(*thread)) == 0
                    && (i_darts_counter_temp1186) % 1000 == 0) {
                    printf("\015Calculating XS's... (%.0lf%% completed)",
                        ((i_darts_counter_temp1186)
                            / ((double)(*(lookups)) / (double)(*(nthreads))))
                            / (double)(*(nthreads)) * 100.);
                }
                (*(*p_energy)) = rn(&(*(*seed)));
                (*(*mat)) = pick_mat(&(*(*seed)));
                calculate_macro_xs((*(*p_energy)), (*(*mat)), (*(n_isotopes)), (*(n_gridpoints)),
                    (*(num_nucs)), (*(concs)), (*(energy_grid)), (*(nuclide_grids)), (*(mats)),
                    (*(*macro_xs_vector)));
            }
        }
        isThereNewIteration = this->inputsTPParent->requestNewRangeIterations1186(
            &(this->endRange), this->getLocalID());
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling omp for stmt's barrier*/
    myTP->controlTPParent->barrierCodelets1186[0].decDep();
}
TP1186::TP1186(int in_numThreads, int in_mainCodeletID, TP1173* in_TPParent, int in_initIteration,
    int in_lastIteration, double*** in_concs, GridPoint** in_energy_grid, int* in_lookups,
    int*** in_mats, int* in_mype, int* in_n_gridpoints, int* in_n_isotopes, int* in_nthreads,
    NuclideGridPoint*** in_nuclide_grids, int** in_num_nucs, TP1186** in_ptrToThisTP)
    : ompTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , concs_darts1186(in_concs) /*OMP_SHARED - INPUT*/
    , energy_grid_darts1186(in_energy_grid) /*OMP_SHARED - INPUT*/
    , i_darts1186(new int[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , lookups_darts1186(in_lookups) /*OMP_SHARED - INPUT*/
    , macro_xs_vector_darts1186(new double**[this->numThreads])
    , mat_darts1186(new int*[this->numThreads])
    , mats_darts1186(in_mats) /*OMP_SHARED - INPUT*/
    , mype_darts1186(in_mype) /*OMP_SHARED - INPUT*/
    , n_gridpoints_darts1186(in_n_gridpoints) /*OMP_SHARED - INPUT*/
    , n_isotopes_darts1186(in_n_isotopes) /*OMP_SHARED - INPUT*/
    , nthreads_darts1186(in_nthreads) /*OMP_SHARED - INPUT*/
    , nuclide_grids_darts1186(in_nuclide_grids) /*OMP_SHARED - INPUT*/
    , num_nucs_darts1186(in_num_nucs) /*OMP_SHARED - INPUT*/
    , p_energy_darts1186(new double*[this->numThreads])
    , seed_darts1186(new unsigned long*[this->numThreads])
    , thread_darts1186(new int*[this->numThreads])
    , initIteration1186(in_initIteration)
    , lastIteration1186(in_lastIteration)
    , nextIteration1186(INT_MAX)
    , loop1186alreadySetUp(0)
    , readyCodelets(0)
    , baseNumThreads(this->numThreads / DARTS_CODELETS_MULT)
    , barrierCodelets1186(new _barrierCodelets1186[1])
    , checkInCodelets1187(new _checkInCodelets1187[this->numThreads])
{
    /*Initialize the loop parameters*/
    nextIteration1186 = initIteration1186;
    /*Initialize inputs and vars.*/
    this->macro_xs_vector_darts1186
        = (double***)malloc(sizeof(double**) * this->numThreads) /*OMP_SHARED_PRIVATE*/;
    this->mat_darts1186 = (int**)malloc(sizeof(int*) * this->numThreads) /*OMP_SHARED_PRIVATE*/;
    this->p_energy_darts1186
        = (double**)malloc(sizeof(double*) * this->numThreads) /*OMP_SHARED_PRIVATE*/;
    this->seed_darts1186
        = (unsigned long**)malloc(sizeof(unsigned long*) * this->numThreads) /*OMP_SHARED_PRIVATE*/;
    this->thread_darts1186 = (int**)malloc(sizeof(int*) * this->numThreads) /*OMP_SHARED_PRIVATE*/;
    /*Initialize Codelets*/
    barrierCodelets1186[0] = _barrierCodelets1186(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets1187* checkInCodelets1187Ptr = (this->checkInCodelets1187);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets1187);
#endif
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->baseNumThreads;
         codeletCounter++) {
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets1187Ptr) = _checkInCodelets1187(2, 1, this, codeletCounter);
#else
        (*checkInCodelets1187Ptr) = _checkInCodelets1187(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets1187Ptr).decDep();
        checkInCodelets1187Ptr++;
    }
    this->dispatchCodelet(this->getID());
    *(in_ptrToThisTP) = this;
}
void TP1186::dispatchCodelet(size_t codeletID)
{
    int currentCodelet = __sync_fetch_and_add(&(this->readyCodelets), 1);
    this->checkInCodelets1187[currentCodelet].setID(codeletID);
#if USE_SPIN_CODELETS == 0
    this->firstCodelet[currentCodelet].decDep();
#else
    this->availableCodelets[currentCodelet] = 1;
#endif
}
TP1186::~TP1186()
{
    delete[] macro_xs_vector_darts1186;
    delete[] mat_darts1186;
    delete[] p_energy_darts1186;
    delete[] seed_darts1186;
    delete[] thread_darts1186;
    delete[] barrierCodelets1186;
    delete[] checkInCodelets1187;
}
