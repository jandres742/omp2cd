#include "matmul.h"

int functB() { return 1; }

int main(int argc, char** argv)
{
    int M = 10;

    if (argc > 1) {
        M = atoi(argv[1]);
    }

    int N = M, P = M;

    MYTYPE* A = (double*)malloc(M * N * sizeof(double));
    double* B = (double*)malloc(N * P * sizeof(double));
    double* C = (double*)malloc(M * P * sizeof(double));
    double* Cp = (double*)malloc(M * P * sizeof(double));

    int i, j, k;
    // Init matrices
    for (i = 0; i < M; i++) {
        for (j = 0; j < N; j++) {
            *(A + i * N + j) = i + j + 1;
        }
    }

    for (i = 0; i < N; i++) {
        for (j = 0; j < P; j++) {
            *(B + i * P + j) = i - j + 3;
        }
    }

    for (i = 0; i < M; i++) {
        for (j = 0; j < P; j++) {
            *(C + i * P + j) = 0;
            *(Cp + i * P + j) = 0;
        }
    }

    // Multiply matrices
    for (i = 0; i < M; i++) {
        for (j = 0; j < P; j++) {
            for (k = 0; k < N; k++) {
                *(C + i * P + j) += *(A + i * N + k) * *(B + k * P + j);
            }
        }
    }

    int n = 0;
    int good = 1;
    while (n < 10 && functB() && good) {
        for (i = 0; i < M; i++) {
            for (j = 0; j < P; j++) {
                *(Cp + i * P + j) = 0;
            }
        }

        double zero = 0;

#pragma omp parallel
        {

            double xx;
            int i, j, k;

#pragma omp for private(j, k) firstprivate(zero)
            for (i = 0; i < M; i++) {
                for (j = 0; j < P; j++) {
                    for (k = 0; k < N; k++) {
                        double temp = zero;
                        temp += *(A + (i)*M + (k)) * *(B + (k)*P + (j));
                        *(Cp + (i)*M + (j)) += temp;
                    }
                }
            }

            if (zero > 1)
                xx = zero;

            double yy;
            yy = xx;
            double one = zero + 1 + yy;
            (void)one;

// Check matrices
#pragma omp single
            {
                for (i = 0; i < M && good; i++) {
                    for (j = 0; j < P; j++) {
                        if (*(C + i * P + j) != *(Cp + i * P + j)) {
                            printf("\nError: C[%d][%d]=%f, Cp[%d][%d]=%f\n", i, j, *(C + i * P + j),
                                i, j, *(Cp + i * P + j));
                            good = 0;
                            break;
                        }
                    }
                }
            }
        }

        n++;
    }

    return 0;
}
