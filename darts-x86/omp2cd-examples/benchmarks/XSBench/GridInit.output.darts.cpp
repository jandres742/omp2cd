#include "GridInit.output.darts.h"
using namespace darts;
using namespace std;
/*Function: generate_grids, ID: 7*/
void generate_grids(NuclideGridPoint** nuclide_grids, int n_isotopes, int n_gridpoints)
{
    /*generate_grids:7*/
    /*CompoundStmt:691*/
    for (int i = 0; i < n_isotopes; i++)
        for (int j = 0; j < n_gridpoints; j++) {
            nuclide_grids[i][j].energy = ((double)rand() / (double)2147483647);
            nuclide_grids[i][j].total_xs = ((double)rand() / (double)2147483647);
            nuclide_grids[i][j].elastic_xs = ((double)rand() / (double)2147483647);
            nuclide_grids[i][j].absorbtion_xs = ((double)rand() / (double)2147483647);
            nuclide_grids[i][j].fission_xs = ((double)rand() / (double)2147483647);
            nuclide_grids[i][j].nu_fission_xs = ((double)rand() / (double)2147483647);
        }
}
/*Function: generate_grids_v, ID: 8*/
void generate_grids_v(NuclideGridPoint** nuclide_grids, int n_isotopes, int n_gridpoints)
{
    /*generate_grids_v:8*/
    /*CompoundStmt:749*/
    for (int i = 0; i < n_isotopes; i++)
        for (int j = 0; j < n_gridpoints; j++) {
            nuclide_grids[i][j].energy = rn_v();
            nuclide_grids[i][j].total_xs = rn_v();
            nuclide_grids[i][j].elastic_xs = rn_v();
            nuclide_grids[i][j].absorbtion_xs = rn_v();
            nuclide_grids[i][j].fission_xs = rn_v();
            nuclide_grids[i][j].nu_fission_xs = rn_v();
        }
}
/*Function: sort_nuclide_grids, ID: 9*/
void sort_nuclide_grids(NuclideGridPoint** nuclide_grids, int n_isotopes, int n_gridpoints)
{
    /*sort_nuclide_grids:9*/
    /*CompoundStmt:789*/
    int (*cmp)(const void*, const void*);
    cmp = NGP_compare;
    for (int i = 0; i < n_isotopes; i++)
        qsort(nuclide_grids[i], n_gridpoints, sizeof(NuclideGridPoint), cmp);
}
/*Function: generate_energy_grid, ID: 10*/
GridPoint* generate_energy_grid(int n_isotopes, int n_gridpoints, NuclideGridPoint** nuclide_grids)
{
    /*generate_energy_grid:10*/
    /*CompoundStmt:799*/
    int mype = 0;
    if (mype == 0)
        printf("Generating Unionized Energy Grid...\n");
    int n_unionized_grid_points = n_isotopes * n_gridpoints;
    int (*cmp)(const void*, const void*);
    cmp = NGP_compare;
    GridPoint* energy_grid = (GridPoint*)malloc(n_unionized_grid_points * sizeof(GridPoint));
    if (mype == 0)
        printf("Copying and Sorting all nuclide grids...\n");
    NuclideGridPoint** n_grid_sorted = gpmatrix(n_isotopes, n_gridpoints);
    memcpy(
        n_grid_sorted[0], nuclide_grids[0], n_isotopes * n_gridpoints * sizeof(NuclideGridPoint));
    qsort(&n_grid_sorted[0][0], n_unionized_grid_points, sizeof(NuclideGridPoint), cmp);
    if (mype == 0)
        printf("Assigning energies to unionized grid...\n");
    for (int i = 0; i < n_unionized_grid_points; i++)
        energy_grid[i].energy = n_grid_sorted[0][i].energy;
    gpmatrix_free(n_grid_sorted);
    int* full = (int*)malloc(n_isotopes * n_unionized_grid_points * sizeof(int));
    for (int i = 0; i < n_unionized_grid_points; i++)
        energy_grid[i].xs_ptrs = &full[n_isotopes * i];
    return energy_grid;
}
/*Function: set_grid_ptrs, ID: 11*/
void set_grid_ptrs(
    GridPoint* energy_grid, NuclideGridPoint** nuclide_grids, int n_isotopes, int n_gridpoints)
{
    /*set_grid_ptrs:11*/
    /*CompoundStmt:863*/
    int mype = 0;
    if (mype == 0)
        printf("Assigning pointers to Unionized Energy Grid...\n");
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP869>(ompNumThreads * DARTS_CODELETS_MULT, 0,
            RuntimeFinalCodelet, 0, n_isotopes * n_gridpoints, (GridPoint**)&((energy_grid)),
            (int*)&((mype)), (int*)&((n_gridpoints)), (int*)&((n_isotopes)),
            (NuclideGridPoint***)&((nuclide_grids))));
    }
    if (mype == 0)
        printf("\n");
}
/*TP869: OMPParallelForDirective*/
void TP869::_barrierCodelets869::fire(void)
{
    TP869* myTP = static_cast<TP869*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
bool TP869::requestNewRangeIterations869(int* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Static */
    /*Chunk = 0*/
    bool isThereNewIteration = false;
    {
        /*Static Scheduling*/
        int tempStartRange = rangePerCodelet869 * codeletID;
        int tempEndRange = rangePerCodelet869 * (codeletID + 1);
        if (remainderRange869 != 0) {
            if (codeletID < (uint32_t)remainderRange869) {
                tempStartRange += codeletID;
                tempEndRange += (codeletID + 1);
            } else {
                tempStartRange += remainderRange869;
                tempEndRange += remainderRange869;
            }
        }
        tempStartRange = tempStartRange * 1 + minIteration869;
        tempEndRange = tempEndRange * 1 + minIteration869;
        if (tempStartRange != tempEndRange) {
            isThereNewIteration = true;
        }
        if (initIteration869 < lastIteration869) {
            (this->inputsTPParent->i_darts869[codeletID]) = min(tempStartRange, tempEndRange);
            *endRange = max(tempStartRange, tempEndRange);
        } else {
            (this->inputsTPParent->i_darts869[codeletID]) = max(tempStartRange, tempEndRange);
            *endRange = min(tempStartRange, tempEndRange);
        }
        if (codeletID == this->numThreads - 1) {
            *endRange = lastIteration869;
        }
    }
    return isThereNewIteration;
}
void TP869::_checkInCodelets870::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 870: ForStmt*/
    /*var: energy_grid*/
    /*var: mype*/
    /*var: n_gridpoints*/
    /*var: n_isotopes*/
    /*var: nuclide_grids*/
    GridPoint** energy_grid = (this->inputsTPParent->energy_grid_darts869);
    (void)energy_grid /*OMP_SHARED*/;
    int* mype = (this->inputsTPParent->mype_darts869);
    (void)mype /*OMP_SHARED*/;
    int* n_gridpoints = (this->inputsTPParent->n_gridpoints_darts869);
    (void)n_gridpoints /*OMP_SHARED*/;
    int* n_isotopes = (this->inputsTPParent->n_isotopes_darts869);
    (void)n_isotopes /*OMP_SHARED*/;
    NuclideGridPoint*** nuclide_grids = (this->inputsTPParent->nuclide_grids_darts869);
    (void)nuclide_grids /*OMP_SHARED*/;
    int* i = &(this->inputsTPParent->i_darts869[this->getLocalID()]);
    (void)i /*PRIVATE*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations869(
        (int*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        /*Find and signal the next codelet*/
        myTP->controlTPParent->TPParent->barrierCodelets869[0].decDep();
        return;
    }
    for (int i_darts_counter_temp869 = (*i); i_darts_counter_temp869 < endRange
         && i_darts_counter_temp869 < this->inputsTPParent->lastIteration869;
         i_darts_counter_temp869++) {
        {
            double quarry = (*(energy_grid))[(i_darts_counter_temp869)].energy;
            if (1 && (*(mype)) == 0 && omp_get_thread_num() == 0
                && (i_darts_counter_temp869) % 200 == 0) {
                printf("\015Aligning Unionized Grid...(%.0lf%% complete)", 100.
                        * (double)(i_darts_counter_temp869)
                        / ((*(n_isotopes)) * (*(n_gridpoints)) / omp_get_num_threads()));
            }
            {
                /*Loop's init*/
                int j = 0;
                for (; j < (*(n_isotopes)); j++) {
                    (*(energy_grid))[(i_darts_counter_temp869)].xs_ptrs[j]
                        = binary_search((*(nuclide_grids))[j], quarry, (*(n_gridpoints)));
                }
            }
        }
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets869[0].decDep();
}
TP869::TP869(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet, int in_initIteration,
    int in_lastIteration, GridPoint** in_energy_grid, int* in_mype, int* in_n_gridpoints,
    int* in_n_isotopes, NuclideGridPoint*** in_nuclide_grids)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , energy_grid_darts869(in_energy_grid) /*OMP_SHARED - INPUT*/
    , mype_darts869(in_mype) /*OMP_SHARED - INPUT*/
    , n_gridpoints_darts869(in_n_gridpoints) /*OMP_SHARED - INPUT*/
    , n_isotopes_darts869(in_n_isotopes) /*OMP_SHARED - INPUT*/
    , nuclide_grids_darts869(in_nuclide_grids) /*OMP_SHARED - INPUT*/
    , i_darts869(new int[this->numThreads]) /*VARIABLE*/
    , initIteration869(in_initIteration)
    , lastIteration869(in_lastIteration)
    , barrierCodelets869(new _barrierCodelets869[1])
    , checkInCodelets870(new _checkInCodelets870[this->numThreads])
{
    /*Initialize the loop parameters*/
    range869 = abs(lastIteration869 - initIteration869) / 1;
    rangePerCodelet869 = range869 / numThreads;
    minIteration869 = min<int>(lastIteration869, initIteration869);
    remainderRange869 = range869 % numThreads;
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets869[0] = _barrierCodelets869(ompNumThreads, ompNumThreads, this, 0);
    _checkInCodelets870* checkInCodelets870Ptr = (this->checkInCodelets870);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets870Ptr) = _checkInCodelets870(1, 1, this, codeletCounter);
        (*checkInCodelets870Ptr).decDep();
        checkInCodelets870Ptr++;
    }
}
TP869::~TP869()
{
    delete[] i_darts869;
    delete[] barrierCodelets869;
    delete[] checkInCodelets870;
}
