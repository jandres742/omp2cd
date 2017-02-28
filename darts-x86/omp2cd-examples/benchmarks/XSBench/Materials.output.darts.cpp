#include "Materials.output.darts.h"
using namespace darts;
using namespace std;
/*Function: load_num_nucs, ID: 27*/
int* load_num_nucs(int n_isotopes)
{
    /*load_num_nucs:27*/
    /*CompoundStmt:65*/
    int* num_nucs = (int*)malloc(12 * sizeof(int));
    if (n_isotopes == 68)
        num_nucs[0] = 34;
    else
        num_nucs[0] = 321;
    num_nucs[1] = 5;
    num_nucs[2] = 4;
    num_nucs[3] = 4;
    num_nucs[4] = 27;
    num_nucs[5] = 21;
    num_nucs[6] = 21;
    num_nucs[7] = 21;
    num_nucs[8] = 21;
    num_nucs[9] = 21;
    num_nucs[10] = 9;
    num_nucs[11] = 9;
    return num_nucs;
}
/*Function: load_mats, ID: 28*/
int** load_mats(int* num_nucs, int n_isotopes)
{
    /*load_mats:28*/
    /*CompoundStmt:102*/
    int** mats = (int**)malloc(12 * sizeof(int*));
    for (int i = 0; i < 12; i++)
        mats[i] = (int*)malloc(num_nucs[i] * sizeof(int));
    int mats0_Sml[] = { 58, 59, 60, 61, 40, 42, 43, 44, 45, 46, 1, 2, 3, 7, 8, 9, 10, 29, 57, 47,
        48, 0, 62, 15, 33, 34, 52, 53, 54, 55, 56, 18, 23, 41 };
    int mats0_Lrg[321] = { 58, 59, 60, 61, 40, 42, 43, 44, 45, 46, 1, 2, 3, 7, 8, 9, 10, 29, 57, 47,
        48, 0, 62, 15, 33, 34, 52, 53, 54, 55, 56, 18, 23, 41 };
    for (int i = 0; i < 321 - 34; i++)
        mats0_Lrg[34 + i] = 68 + i;
    int mats1[] = { 63, 64, 65, 66, 67 };
    int mats2[] = { 24, 41, 4, 5 };
    int mats3[] = { 24, 41, 4, 5 };
    int mats4[] = { 19, 20, 21, 22, 35, 36, 37, 38, 39, 25, 27, 28, 29, 30, 31, 32, 26, 49, 50, 51,
        11, 12, 13, 14, 6, 16, 17 };
    int mats5[]
        = { 24, 41, 4, 5, 19, 20, 21, 22, 35, 36, 37, 38, 39, 25, 49, 50, 51, 11, 12, 13, 14 };
    int mats6[]
        = { 24, 41, 4, 5, 19, 20, 21, 22, 35, 36, 37, 38, 39, 25, 49, 50, 51, 11, 12, 13, 14 };
    int mats7[]
        = { 24, 41, 4, 5, 19, 20, 21, 22, 35, 36, 37, 38, 39, 25, 49, 50, 51, 11, 12, 13, 14 };
    int mats8[]
        = { 24, 41, 4, 5, 19, 20, 21, 22, 35, 36, 37, 38, 39, 25, 49, 50, 51, 11, 12, 13, 14 };
    int mats9[]
        = { 24, 41, 4, 5, 19, 20, 21, 22, 35, 36, 37, 38, 39, 25, 49, 50, 51, 11, 12, 13, 14 };
    int mats10[] = { 24, 41, 4, 5, 63, 64, 65, 66, 67 };
    int mats11[] = { 24, 41, 4, 5, 63, 64, 65, 66, 67 };
    if (n_isotopes == 68)
        memcpy(mats[0], mats0_Sml, num_nucs[0] * sizeof(int));
    else
        memcpy(mats[0], mats0_Lrg, num_nucs[0] * sizeof(int));
    memcpy(mats[1], mats1, num_nucs[1] * sizeof(int));
    memcpy(mats[2], mats2, num_nucs[2] * sizeof(int));
    memcpy(mats[3], mats3, num_nucs[3] * sizeof(int));
    memcpy(mats[4], mats4, num_nucs[4] * sizeof(int));
    memcpy(mats[5], mats5, num_nucs[5] * sizeof(int));
    memcpy(mats[6], mats6, num_nucs[6] * sizeof(int));
    memcpy(mats[7], mats7, num_nucs[7] * sizeof(int));
    memcpy(mats[8], mats8, num_nucs[8] * sizeof(int));
    memcpy(mats[9], mats9, num_nucs[9] * sizeof(int));
    memcpy(mats[10], mats10, num_nucs[10] * sizeof(int));
    memcpy(mats[11], mats11, num_nucs[11] * sizeof(int));
    return mats;
}
/*Function: load_concs, ID: 29*/
double** load_concs(int* num_nucs)
{
    /*load_concs:29*/
    /*CompoundStmt:224*/
    double** concs = (double**)malloc(12 * sizeof(double*));
    for (int i = 0; i < 12; i++)
        concs[i] = (double*)malloc(num_nucs[i] * sizeof(double));
    for (int i = 0; i < 12; i++)
        for (int j = 0; j < num_nucs[i]; j++)
            concs[i][j] = (double)rand() / (double)2147483647;
    return concs;
}
/*Function: load_concs_v, ID: 30*/
double** load_concs_v(int* num_nucs)
{
    /*load_concs_v:30*/
    /*CompoundStmt:258*/
    double** concs = (double**)malloc(12 * sizeof(double*));
    for (int i = 0; i < 12; i++)
        concs[i] = (double*)malloc(num_nucs[i] * sizeof(double));
    for (int i = 0; i < 12; i++)
        for (int j = 0; j < num_nucs[i]; j++)
            concs[i][j] = rn_v();
    return concs;
}
/*Function: pick_mat, ID: 31*/
int pick_mat(unsigned long* seed)
{
    /*pick_mat:31*/
    /*CompoundStmt:289*/
    double dist[12];
    dist[0] = 0.14000000000000001;
    dist[1] = 0.051999999999999998;
    dist[2] = 0.27500000000000002;
    dist[3] = 0.13400000000000001;
    dist[4] = 0.154;
    dist[5] = 0.064000000000000001;
    dist[6] = 0.066000000000000003;
    dist[7] = 0.055;
    dist[8] = 0.0080000000000000002;
    dist[9] = 0.014999999999999999;
    dist[10] = 0.025000000000000001;
    dist[11] = 0.012999999999999999;
    double roll = rn(seed);
    for (int i = 0; i < 12; i++) {
        double running = 0;
        for (int j = i; j > 0; j--)
            running += dist[j];
        if (roll < running)
            return i;
    }
    return 0;
}
