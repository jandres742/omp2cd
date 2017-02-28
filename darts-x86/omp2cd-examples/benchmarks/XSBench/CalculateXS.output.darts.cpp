#include "CalculateXS.output.darts.h"
using namespace darts;
using namespace std;
/*Function: calculate_micro_xs, ID: 3*/
void calculate_micro_xs(double p_energy, int nuc, int n_isotopes, int n_gridpoints,
    GridPoint* restrict energy_grid, NuclideGridPoint** restrict nuclide_grids, int idx,
    double* restrict xs_vector)
{
    /*calculate_micro_xs:3*/
    /*CompoundStmt:930*/
    double f;
    NuclideGridPoint *low, *high;
    if (energy_grid[idx].xs_ptrs[nuc] == n_gridpoints - 1)
        low = &nuclide_grids[nuc][energy_grid[idx].xs_ptrs[nuc] - 1];
    else
        low = &nuclide_grids[nuc][energy_grid[idx].xs_ptrs[nuc]];
    high = low + 1;
    f = (high->energy - p_energy) / (high->energy - low->energy);
    xs_vector[0] = high->total_xs - f * (high->total_xs - low->total_xs);
    xs_vector[1] = high->elastic_xs - f * (high->elastic_xs - low->elastic_xs);
    xs_vector[2] = high->absorbtion_xs - f * (high->absorbtion_xs - low->absorbtion_xs);
    xs_vector[3] = high->fission_xs - f * (high->fission_xs - low->fission_xs);
    xs_vector[4] = high->nu_fission_xs - f * (high->nu_fission_xs - low->nu_fission_xs);
}
/*Function: calculate_macro_xs, ID: 4*/
void calculate_macro_xs(double p_energy, int mat, int n_isotopes, int n_gridpoints,
    int* restrict num_nucs, double** restrict concs, GridPoint* restrict energy_grid,
    NuclideGridPoint** restrict nuclide_grids, int** restrict mats,
    double* restrict macro_xs_vector)
{
    /*calculate_macro_xs:4*/
    /*CompoundStmt:1001*/
    double xs_vector[5];
    int p_nuc;
    int idx = 0;
    double conc;
    for (int k = 0; k < 5; k++)
        macro_xs_vector[k] = 0;
    idx = grid_search(n_isotopes * n_gridpoints, p_energy, energy_grid);
    for (int j = 0; j < num_nucs[mat]; j++) {
        p_nuc = mats[mat][j];
        conc = concs[mat][j];
        calculate_micro_xs(
            p_energy, p_nuc, n_isotopes, n_gridpoints, energy_grid, nuclide_grids, idx, xs_vector);
        for (int k = 0; k < 5; k++)
            macro_xs_vector[k] += xs_vector[k] * conc;
    }
}
/*Function: grid_search, ID: 5*/
int grid_search(int n, double quarry, GridPoint* A)
{
    /*grid_search:5*/
    /*CompoundStmt:1036*/
    int lowerLimit = 0;
    int upperLimit = n - 1;
    int examinationPoint;
    int length = upperLimit - lowerLimit;
    while (length > 1) {
        examinationPoint = lowerLimit + (length / 2);
        if (A[examinationPoint].energy > quarry)
            upperLimit = examinationPoint;
        else
            lowerLimit = examinationPoint;
        length = upperLimit - lowerLimit;
    }
    return lowerLimit;
}
