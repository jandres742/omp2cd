#include "integ.output.darts.h"
using namespace darts;
using namespace std;
/*Function: exprjh, ID: 3*/
double exprjh(double x)
{
    /*exprjh:3*/
    /*CompoundStmt:4058*/
    if (x < -37.)
        return 0.;
    else
        return exp(x);
}
/*Function: Dgemm, ID: 4*/
void Dgemm(char a, char b, int M, int N, int K, double* A, double* B, double* C)
{
    /*Dgemm:4*/
    /*CompoundStmt:4067*/
    int i, j, l;
    if ((a == 'n') && (b == 'n')) {
        for (i = 0; i < M; i++) {
            for (j = 0; j < N; j++) {
                double sum = 0.;
                for (l = 0; l < K; l++)
                    sum += A[i * K + l] * B[l * N + j];
                C[i * N + j] = sum;
            }
        }
    } else if ((a == 'n') && (b == 't')) {
        for (i = 0; i < M; i++) {
            for (j = 0; j < N; j++) {
                double sum = 0.;
                for (l = 0; l < K; l++)
                    sum += A[i * K + l] * B[j * K + l];
                C[i * N + j] = sum;
            }
        }
    } else if ((a == 't') && (b == 'n')) {
        for (i = 0; i < M; i++) {
            for (j = 0; j < N; j++) {
                double sum = 0.;
                for (l = 0; l < K; l++)
                    sum += A[l * M + i] * B[l * N + j];
                C[i * N + j] = sum;
            }
        }
    } else if ((a == 't') && (b == 't')) {
        for (i = 0; i < M; i++) {
            for (j = 0; j < N; j++) {
                double sum = 0.;
                for (l = 0; l < K; l++)
                    sum += A[l * M + i] * B[j * K + l];
                C[i * N + j] = sum;
            }
        }
    }
}
