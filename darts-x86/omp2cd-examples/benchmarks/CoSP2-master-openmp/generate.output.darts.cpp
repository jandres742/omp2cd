#include "generate.output.darts.h"
using namespace darts;
using namespace std;
/*Function: generateHMatrix, ID: 18*/
SparseMatrix* generateHMatrix(int N, int M, real_t a, real_t alpha)
{
    /*generateHMatrix:18*/
    /*CompoundStmt:2381*/
    SparseMatrix* spmatrix = initSparseMatrix(N, M);
    uint64_t seed = mkSeed(N, M);
    int l, mstart, mend;
    real_t hx;
    int nnz = 0;
    for (int i = 0; i < N; i++) {
        l = 0;
        mstart = ((0) > (i - M + 1) ? (0) : (i - M + 1));
        mend = ((N) < (i + M) ? (N) : (i + M));
        for (int j = mstart; j < mend; j++) {
            hx = a * lcg61(&seed) * exp(-alpha * (i - j) * (i - j));
            if (j == i || fabs(hx) > eps) {
                spmatrix->jja[i][l] = j;
                spmatrix->val[i][l] = hx;
                l++;
                nnz++;
            }
        }
        spmatrix->iia[i] = l;
    }
    printf("Generated H Matrix nnz = %d avg nnz/row = %d\n", nnz, nnz / N);
    return spmatrix;
}
