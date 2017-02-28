#include "sp2Loop.output.darts.h"
using namespace darts;
using namespace std;
/*Function: sp2Loop, ID: 89*/
void sp2Loop(struct SparseMatrixSt* xmatrix, struct DomainSt* domain)
{
    /*sp2Loop:89*/
    /*CompoundStmt:2768*/
    HaloExchange* haloExchange;
    do {
        profileStart(sp2LoopTimer);
    } while (0);
    int hsize = xmatrix->hsize;
    SparseMatrix* x2matrix = initSparseMatrix(xmatrix->hsize, xmatrix->msize);
    do {
        profileStart(normTimer);
    } while (0);
    normalize(xmatrix);
    do {
        profileStop(normTimer);
    } while (0);
    real_t idempErr = ZERO;
    real_t idempErr1 = ZERO;
    real_t idempErr2 = ZERO;
    real_t occ = hsize * HALF;
    real_t trX = ZERO;
    real_t trX2 = ZERO;
    real_t tr2XX2, trXOLD, limDiff;
    int iter = 0;
    int breakLoop = 0;
    if (printRank() && debug == 1)
        printf("\nSP2Loop:\n");
    while (breakLoop == 0 && iter < 100) {
        trX = ZERO;
        trX2 = ZERO;
        do {
            profileStart(x2Timer);
        } while (0);
        sparseX2(&trX, &trX2, xmatrix, x2matrix, domain);
        do {
            profileStop(x2Timer);
        } while (0);
        if (printRank() && debug == 1)
            printf("iter = %d  trX = %e  trX2 = %e\n", iter, trX, trX2);
        tr2XX2 = TWO * trX - trX2;
        trXOLD = trX;
        limDiff = fabs(trX2 - occ) - fabs(tr2XX2 - occ);
        if (limDiff > idemTol) {
            trX = TWO * trX - trX2;
            do {
                profileStart(xaddTimer);
            } while (0);
            sparseAdd(xmatrix, x2matrix, domain);
            do {
                profileStop(xaddTimer);
            } while (0);
        } else if (limDiff < -idemTol) {
            trX = trX2;
            do {
                profileStart(xsetTimer);
            } while (0);
            sparseSetX2(xmatrix, x2matrix, domain);
            do {
                profileStop(xsetTimer);
            } while (0);
        } else {
            trX = trXOLD;
            breakLoop = 1;
        }
        idempErr2 = idempErr1;
        idempErr1 = idempErr;
        idempErr = fabs(trX - trXOLD);
        iter++;
        if (iter >= 25 && (idempErr >= idempErr2))
            breakLoop = 1;
    }
    do {
        profileStop(sp2LoopTimer);
    } while (0);
    sparseMultScalar(xmatrix, domain, TWO);
    reportResults(iter, xmatrix, x2matrix, domain);
    destroySparseMatrix(x2matrix);
}
/*Function: reportResults, ID: 90*/
void reportResults(int iter, struct SparseMatrixSt* xmatrix, struct SparseMatrixSt* x2matrix,
    struct DomainSt* domain)
{
    /*reportResults:90*/
    /*CompoundStmt:2884*/
    int hsize = xmatrix->hsize;
    int sumIIA = 0;
    int sumIIC = 0;
    int maxIIA = 0;
    int maxIIC = 0;
    for (int i = domain->localRowMin; i < domain->localRowMax; i++) {
        sumIIA += xmatrix->iia[i];
        sumIIC += x2matrix->iia[i];
        maxIIA = ((maxIIA) > (xmatrix->iia[i]) ? (maxIIA) : (xmatrix->iia[i]));
        maxIIC = ((maxIIC) > (x2matrix->iia[i]) ? (maxIIC) : (x2matrix->iia[i]));
    }
    if (printRank()) {
        printf("\nResults:\n");
        printf("X2 Sparsity CCN = %d, fraction = %e avg = %g, max = %d\n", sumIIC,
            (real_t)sumIIC / (real_t)(hsize * hsize), (real_t)sumIIC / (real_t)hsize, maxIIC);
        printf("D Sparsity AAN = %d, fraction = %e avg = %g, max = %d\n", sumIIA,
            (real_t)sumIIA / (real_t)(hsize * hsize), (real_t)sumIIA / (real_t)hsize, maxIIA);
        printf("Number of iterations = %d\n", iter);
    }
}
