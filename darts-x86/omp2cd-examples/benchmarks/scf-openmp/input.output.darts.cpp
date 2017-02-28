#include "input.output.darts.h"
using namespace darts;
using namespace std;
/*Function: input, ID: 1*/
void input()
{
    /*input:1*/
    /*CompoundStmt:4197*/
    int i, j;
    FILE* fp = fopen("be.inpt", "r");
    if (fscanf(fp, "%d", &natom) != 1) {
        printf("Invalid input file\n");
        exit(1);
    }
    for (i = 0; i < 384; i++) {
        ax[i] = 0.;
        ay[i] = 0.;
        az[i] = 0.;
    }
    for (i = 0; i < natom; i++) {
        if (fscanf(fp, "%d", &j) != 1) {
            printf("Invalid input file\n");
            exit(1);
        }
        if (fscanf(fp, "%lf", &ax[i]) != 1) {
            printf("Invalid input file\n");
            exit(1);
        }
        if (fscanf(fp, "%lf", &ay[i]) != 1) {
            printf("Invalid input file\n");
            exit(1);
        }
        if (fscanf(fp, "%lf", &az[i]) != 1) {
            printf("Invalid input file\n");
            exit(1);
        }
    }
    fclose(fp);
    for (i = 0; i < natom; i++) {
        q[i] = 4.;
        expnt[15 * i + 0] = 1741.;
        expnt[15 * i + 1] = 262.10000000000002;
        expnt[15 * i + 2] = 60.329999999999998;
        expnt[15 * i + 3] = 17.620000000000001;
        expnt[15 * i + 4] = 5.9329999999999998;
        expnt[15 * i + 5] = 2.1850000000000001;
        expnt[15 * i + 6] = 0.85899999999999999;
        expnt[15 * i + 7] = 0.18060000000000001;
        expnt[15 * i + 8] = 0.058349999999999999;
        expnt[15 * i + 9] = 0.29999999999999999;
        expnt[15 * i + 10] = 0.29999999999999999;
        expnt[15 * i + 11] = 0.29999999999999999;
        expnt[15 * i + 12] = 0.29999999999999999;
        expnt[15 * i + 13] = 0.29999999999999999;
        expnt[15 * i + 14] = 0.29999999999999999;
        for (j = 0; j < 15; j++) {
            x[15 * i + j] = ax[i];
            y[15 * i + j] = ay[i];
            z[15 * i + j] = az[i];
            if (j == 9)
                x[15 * i + j] += 1.6000000000000001;
            if (j == 10)
                x[15 * i + j] -= 1.6000000000000001;
            if (j == 11)
                y[15 * i + j] += 1.6000000000000001;
            if (j == 12)
                y[15 * i + j] -= 1.6000000000000001;
            if (j == 13)
                z[15 * i + j] += 1.6000000000000001;
            if (j == 14)
                z[15 * i + j] -= 1.6000000000000001;
        }
    }
    enrep = 0.;
    for (i = 0; i < natom; i++) {
        for (j = i + 1; j < natom; j++) {
            double ax2 = (ax[i] - ax[j]) * (ax[i] - ax[j]);
            double ay2 = (ay[i] - ay[j]) * (ay[i] - ay[j]);
            double az2 = (az[i] - az[j]) * (az[i] - az[j]);
            enrep += q[i] * q[j] / sqrt(ax2 + ay2 + az2);
        }
    }
    nocc = natom << 1;
    nbfn = natom * 15;
    nnbfn = nbfn * (nbfn + 1) / 2;
    return;
}
