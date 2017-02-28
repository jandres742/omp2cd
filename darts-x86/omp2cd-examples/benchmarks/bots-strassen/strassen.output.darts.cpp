#include "strassen.output.darts.h"
using namespace darts;
using namespace std;
/*Function: matrixmul, ID: 15*/
void matrixmul(int n, REAL* A, int an, REAL* B, int bn, REAL* C, int cn)
{
    /*matrixmul:15*/
    /*CompoundStmt:53*/
    int i, j, k;
    REAL s;
    for (i = 0; i < n; ++i) {
        for (j = 0; j < n; ++j) {
            s = 0.;
            for (k = 0; k < n; ++k)
                s += (A[(i) * (an) + (k)]) * (B[(k) * (bn) + (j)]);
            (C[(i) * (cn) + (j)]) = s;
        }
    }
}
/*Function: FastNaiveMatrixMultiply, ID: 16*/
void FastNaiveMatrixMultiply(REAL* C, REAL* A, REAL* B, unsigned int MatrixSize,
    unsigned int RowWidthC, unsigned int RowWidthA, unsigned int RowWidthB)
{
    /*FastNaiveMatrixMultiply:16*/
    /*CompoundStmt:84*/
    PTR RowWidthBInBytes = RowWidthB << 3;
    PTR RowWidthAInBytes = RowWidthA << 3;
    PTR MatrixWidthInBytes = MatrixSize << 3;
    PTR RowIncrementC = (RowWidthC - MatrixSize) << 3;
    unsigned int Horizontal, Vertical;
    REAL* ARowStart = A;
    for (Vertical = 0; Vertical < MatrixSize; Vertical++) {
        for (Horizontal = 0; Horizontal < MatrixSize; Horizontal += 8) {
            REAL* BColumnStart = B + Horizontal;
            REAL FirstARowValue = *ARowStart++;
            REAL Sum0 = FirstARowValue * (*BColumnStart);
            REAL Sum1 = FirstARowValue * (*(BColumnStart + 1));
            REAL Sum2 = FirstARowValue * (*(BColumnStart + 2));
            REAL Sum3 = FirstARowValue * (*(BColumnStart + 3));
            REAL Sum4 = FirstARowValue * (*(BColumnStart + 4));
            REAL Sum5 = FirstARowValue * (*(BColumnStart + 5));
            REAL Sum6 = FirstARowValue * (*(BColumnStart + 6));
            REAL Sum7 = FirstARowValue * (*(BColumnStart + 7));
            unsigned int Products;
            for (Products = 1; Products < MatrixSize; Products++) {
                REAL ARowValue = *ARowStart++;
                BColumnStart = (REAL*)(((PTR)BColumnStart) + RowWidthBInBytes);
                Sum0 += ARowValue * (*BColumnStart);
                Sum1 += ARowValue * (*(BColumnStart + 1));
                Sum2 += ARowValue * (*(BColumnStart + 2));
                Sum3 += ARowValue * (*(BColumnStart + 3));
                Sum4 += ARowValue * (*(BColumnStart + 4));
                Sum5 += ARowValue * (*(BColumnStart + 5));
                Sum6 += ARowValue * (*(BColumnStart + 6));
                Sum7 += ARowValue * (*(BColumnStart + 7));
            }
            ARowStart = (REAL*)(((PTR)ARowStart) - MatrixWidthInBytes);
            *(C) = Sum0;
            *(C + 1) = Sum1;
            *(C + 2) = Sum2;
            *(C + 3) = Sum3;
            *(C + 4) = Sum4;
            *(C + 5) = Sum5;
            *(C + 6) = Sum6;
            *(C + 7) = Sum7;
            C += 8;
        }
        ARowStart = (REAL*)(((PTR)ARowStart) + RowWidthAInBytes);
        C = (REAL*)(((PTR)C) + RowIncrementC);
    }
}
/*Function: FastAdditiveNaiveMatrixMultiply, ID: 17*/
void FastAdditiveNaiveMatrixMultiply(REAL* C, REAL* A, REAL* B, unsigned int MatrixSize,
    unsigned int RowWidthC, unsigned int RowWidthA, unsigned int RowWidthB)
{
    /*FastAdditiveNaiveMatrixMultiply:17*/
    /*CompoundStmt:222*/
    PTR RowWidthBInBytes = RowWidthB << 3;
    PTR RowWidthAInBytes = RowWidthA << 3;
    PTR MatrixWidthInBytes = MatrixSize << 3;
    PTR RowIncrementC = (RowWidthC - MatrixSize) << 3;
    unsigned int Horizontal, Vertical;
    REAL* ARowStart = A;
    for (Vertical = 0; Vertical < MatrixSize; Vertical++) {
        for (Horizontal = 0; Horizontal < MatrixSize; Horizontal += 8) {
            REAL* BColumnStart = B + Horizontal;
            REAL Sum0 = *C;
            REAL Sum1 = *(C + 1);
            REAL Sum2 = *(C + 2);
            REAL Sum3 = *(C + 3);
            REAL Sum4 = *(C + 4);
            REAL Sum5 = *(C + 5);
            REAL Sum6 = *(C + 6);
            REAL Sum7 = *(C + 7);
            unsigned int Products;
            for (Products = 0; Products < MatrixSize; Products++) {
                REAL ARowValue = *ARowStart++;
                Sum0 += ARowValue * (*BColumnStart);
                Sum1 += ARowValue * (*(BColumnStart + 1));
                Sum2 += ARowValue * (*(BColumnStart + 2));
                Sum3 += ARowValue * (*(BColumnStart + 3));
                Sum4 += ARowValue * (*(BColumnStart + 4));
                Sum5 += ARowValue * (*(BColumnStart + 5));
                Sum6 += ARowValue * (*(BColumnStart + 6));
                Sum7 += ARowValue * (*(BColumnStart + 7));
                BColumnStart = (REAL*)(((PTR)BColumnStart) + RowWidthBInBytes);
            }
            ARowStart = (REAL*)(((PTR)ARowStart) - MatrixWidthInBytes);
            *(C) = Sum0;
            *(C + 1) = Sum1;
            *(C + 2) = Sum2;
            *(C + 3) = Sum3;
            *(C + 4) = Sum4;
            *(C + 5) = Sum5;
            *(C + 6) = Sum6;
            *(C + 7) = Sum7;
            C += 8;
        }
        ARowStart = (REAL*)(((PTR)ARowStart) + RowWidthAInBytes);
        C = (REAL*)(((PTR)C) + RowIncrementC);
    }
}
/*Function: MultiplyByDivideAndConquer, ID: 18*/
void MultiplyByDivideAndConquer(REAL* C, REAL* A, REAL* B, unsigned int MatrixSize,
    unsigned int RowWidthC, unsigned int RowWidthA, unsigned int RowWidthB, int AdditiveMode)
{
    /*MultiplyByDivideAndConquer:18*/
    /*CompoundStmt:349*/
    REAL *A01, *A10, *A11, *B01, *B10, *B11, *C01, *C10, *C11;
    unsigned int QuadrantSize = MatrixSize >> 1;
    A01 = A + QuadrantSize;
    A10 = A + RowWidthA * QuadrantSize;
    A11 = A10 + QuadrantSize;
    B01 = B + QuadrantSize;
    B10 = B + RowWidthB * QuadrantSize;
    B11 = B10 + QuadrantSize;
    C01 = C + QuadrantSize;
    C10 = C + RowWidthC * QuadrantSize;
    C11 = C10 + QuadrantSize;
    if (QuadrantSize > 16) {
        MultiplyByDivideAndConquer(
            C, A, B, QuadrantSize, RowWidthC, RowWidthA, RowWidthB, AdditiveMode);
        MultiplyByDivideAndConquer(
            C01, A, B01, QuadrantSize, RowWidthC, RowWidthA, RowWidthB, AdditiveMode);
        MultiplyByDivideAndConquer(
            C11, A10, B01, QuadrantSize, RowWidthC, RowWidthA, RowWidthB, AdditiveMode);
        MultiplyByDivideAndConquer(
            C10, A10, B, QuadrantSize, RowWidthC, RowWidthA, RowWidthB, AdditiveMode);
        MultiplyByDivideAndConquer(C, A01, B10, QuadrantSize, RowWidthC, RowWidthA, RowWidthB, 1);
        MultiplyByDivideAndConquer(C01, A01, B11, QuadrantSize, RowWidthC, RowWidthA, RowWidthB, 1);
        MultiplyByDivideAndConquer(C11, A11, B11, QuadrantSize, RowWidthC, RowWidthA, RowWidthB, 1);
        MultiplyByDivideAndConquer(C10, A11, B10, QuadrantSize, RowWidthC, RowWidthA, RowWidthB, 1);
    } else {
        if (AdditiveMode) {
            FastAdditiveNaiveMatrixMultiply(C, A, B, QuadrantSize, RowWidthC, RowWidthA, RowWidthB);
            FastAdditiveNaiveMatrixMultiply(
                C01, A, B01, QuadrantSize, RowWidthC, RowWidthA, RowWidthB);
            FastAdditiveNaiveMatrixMultiply(
                C11, A10, B01, QuadrantSize, RowWidthC, RowWidthA, RowWidthB);
            FastAdditiveNaiveMatrixMultiply(
                C10, A10, B, QuadrantSize, RowWidthC, RowWidthA, RowWidthB);
        } else {
            FastNaiveMatrixMultiply(C, A, B, QuadrantSize, RowWidthC, RowWidthA, RowWidthB);
            FastNaiveMatrixMultiply(C01, A, B01, QuadrantSize, RowWidthC, RowWidthA, RowWidthB);
            FastNaiveMatrixMultiply(C11, A10, B01, QuadrantSize, RowWidthC, RowWidthA, RowWidthB);
            FastNaiveMatrixMultiply(C10, A10, B, QuadrantSize, RowWidthC, RowWidthA, RowWidthB);
        }
        FastAdditiveNaiveMatrixMultiply(C, A01, B10, QuadrantSize, RowWidthC, RowWidthA, RowWidthB);
        FastAdditiveNaiveMatrixMultiply(
            C01, A01, B11, QuadrantSize, RowWidthC, RowWidthA, RowWidthB);
        FastAdditiveNaiveMatrixMultiply(
            C11, A11, B11, QuadrantSize, RowWidthC, RowWidthA, RowWidthB);
        FastAdditiveNaiveMatrixMultiply(
            C10, A11, B10, QuadrantSize, RowWidthC, RowWidthA, RowWidthB);
    }
    return;
}
/*Function: OptimizedStrassenMultiply_seq, ID: 19*/
void OptimizedStrassenMultiply_seq(REAL* C, REAL* A, REAL* B, unsigned int MatrixSize,
    unsigned int RowWidthC, unsigned int RowWidthA, unsigned int RowWidthB, int Depth)
{
    /*OptimizedStrassenMultiply_seq:19*/
    /*CompoundStmt:403*/
    unsigned int QuadrantSize = MatrixSize >> 1;
    unsigned int QuadrantSizeInBytes = sizeof(REAL) * QuadrantSize * QuadrantSize + 32;
    unsigned int Column, Row;
    REAL *A12, *B12, *C12, *A21, *B21, *C21, *A22, *B22, *C22;
    REAL *S1, *S2, *S3, *S4, *S5, *S6, *S7, *S8, *M2, *M5, *T1sMULT;
    PTR TempMatrixOffset = 0;
    PTR MatrixOffsetA = 0;
    PTR MatrixOffsetB = 0;
    char* Heap;
    void* StartHeap;
    PTR RowIncrementA = (RowWidthA - QuadrantSize) << 3;
    PTR RowIncrementB = (RowWidthB - QuadrantSize) << 3;
    PTR RowIncrementC = (RowWidthC - QuadrantSize) << 3;
    if (MatrixSize <= (unsigned int)bots_app_cutoff_value) {
        MultiplyByDivideAndConquer(C, A, B, MatrixSize, RowWidthC, RowWidthA, RowWidthB, 0);
        return;
    }
    A12 = A + QuadrantSize;
    B12 = B + QuadrantSize;
    C12 = C + QuadrantSize;
    A21 = A + (RowWidthA * QuadrantSize);
    B21 = B + (RowWidthB * QuadrantSize);
    C21 = C + (RowWidthC * QuadrantSize);
    A22 = A21 + QuadrantSize;
    B22 = B21 + QuadrantSize;
    C22 = C21 + QuadrantSize;
    Heap = (char*)malloc(QuadrantSizeInBytes * 11);
    StartHeap = (void*)Heap;
    if (((PTR)Heap) & 31)
        Heap = (char*)(((PTR)Heap) + 32 - (((PTR)Heap) & 31));
    S1 = (REAL*)Heap;
    Heap += QuadrantSizeInBytes;
    S2 = (REAL*)Heap;
    Heap += QuadrantSizeInBytes;
    S3 = (REAL*)Heap;
    Heap += QuadrantSizeInBytes;
    S4 = (REAL*)Heap;
    Heap += QuadrantSizeInBytes;
    S5 = (REAL*)Heap;
    Heap += QuadrantSizeInBytes;
    S6 = (REAL*)Heap;
    Heap += QuadrantSizeInBytes;
    S7 = (REAL*)Heap;
    Heap += QuadrantSizeInBytes;
    S8 = (REAL*)Heap;
    Heap += QuadrantSizeInBytes;
    M2 = (REAL*)Heap;
    Heap += QuadrantSizeInBytes;
    M5 = (REAL*)Heap;
    Heap += QuadrantSizeInBytes;
    T1sMULT = (REAL*)Heap;
    Heap += QuadrantSizeInBytes;
    for (Row = 0; Row < QuadrantSize; Row++) {
        for (Column = 0; Column < QuadrantSize; Column++) {
            (*(REAL*)(((PTR)S4) + TempMatrixOffset)) = (*(REAL*)(((PTR)A12) + MatrixOffsetA))
                - ((*(REAL*)(((PTR)S2) + TempMatrixOffset))
                      = ((*(REAL*)(((PTR)S1) + TempMatrixOffset))
                            = (*(REAL*)(((PTR)A21) + MatrixOffsetA))
                                + (*(REAL*)(((PTR)A22) + MatrixOffsetA)))
                          - (*(REAL*)(((PTR)A) + MatrixOffsetA)));
            (*(REAL*)(((PTR)S8) + TempMatrixOffset))
                = ((*(REAL*)(((PTR)S6) + TempMatrixOffset)) = (*(REAL*)(((PTR)B22) + MatrixOffsetB))
                          - ((*(REAL*)(((PTR)S5) + TempMatrixOffset))
                                = (*(REAL*)(((PTR)B12) + MatrixOffsetB))
                                    - (*(REAL*)(((PTR)B) + MatrixOffsetB))))
                - (*(REAL*)(((PTR)B21) + MatrixOffsetB));
            (*(REAL*)(((PTR)S3) + TempMatrixOffset))
                = (*(REAL*)(((PTR)A) + MatrixOffsetA)) - (*(REAL*)(((PTR)A21) + MatrixOffsetA));
            (*(REAL*)(((PTR)S7) + TempMatrixOffset))
                = (*(REAL*)(((PTR)B22) + MatrixOffsetB)) - (*(REAL*)(((PTR)B12) + MatrixOffsetB));
            TempMatrixOffset += sizeof(REAL);
            MatrixOffsetA += sizeof(REAL);
            MatrixOffsetB += sizeof(REAL);
        }
        MatrixOffsetA += RowIncrementA;
        MatrixOffsetB += RowIncrementB;
    }
    OptimizedStrassenMultiply_seq(
        M2, A, B, QuadrantSize, QuadrantSize, RowWidthA, RowWidthB, Depth + 1);
    OptimizedStrassenMultiply_seq(
        M5, S1, S5, QuadrantSize, QuadrantSize, QuadrantSize, QuadrantSize, Depth + 1);
    OptimizedStrassenMultiply_seq(
        T1sMULT, S2, S6, QuadrantSize, QuadrantSize, QuadrantSize, QuadrantSize, Depth + 1);
    OptimizedStrassenMultiply_seq(
        C22, S3, S7, QuadrantSize, RowWidthC, QuadrantSize, QuadrantSize, Depth + 1);
    OptimizedStrassenMultiply_seq(
        C, A12, B21, QuadrantSize, RowWidthC, RowWidthA, RowWidthB, Depth + 1);
    OptimizedStrassenMultiply_seq(
        C12, S4, B22, QuadrantSize, RowWidthC, QuadrantSize, RowWidthB, Depth + 1);
    OptimizedStrassenMultiply_seq(
        C21, A22, S8, QuadrantSize, RowWidthC, RowWidthA, QuadrantSize, Depth + 1);
    for (Row = 0; Row < QuadrantSize; Row++) {
        for (Column = 0; Column < QuadrantSize; Column += 4) {
            REAL LocalM5_0 = *(M5);
            REAL LocalM5_1 = *(M5 + 1);
            REAL LocalM5_2 = *(M5 + 2);
            REAL LocalM5_3 = *(M5 + 3);
            REAL LocalM2_0 = *(M2);
            REAL LocalM2_1 = *(M2 + 1);
            REAL LocalM2_2 = *(M2 + 2);
            REAL LocalM2_3 = *(M2 + 3);
            REAL T1_0 = *(T1sMULT) + LocalM2_0;
            REAL T1_1 = *(T1sMULT + 1) + LocalM2_1;
            REAL T1_2 = *(T1sMULT + 2) + LocalM2_2;
            REAL T1_3 = *(T1sMULT + 3) + LocalM2_3;
            REAL T2_0 = *(C22) + T1_0;
            REAL T2_1 = *(C22 + 1) + T1_1;
            REAL T2_2 = *(C22 + 2) + T1_2;
            REAL T2_3 = *(C22 + 3) + T1_3;
            (*(C)) += LocalM2_0;
            (*(C + 1)) += LocalM2_1;
            (*(C + 2)) += LocalM2_2;
            (*(C + 3)) += LocalM2_3;
            (*(C12)) += LocalM5_0 + T1_0;
            (*(C12 + 1)) += LocalM5_1 + T1_1;
            (*(C12 + 2)) += LocalM5_2 + T1_2;
            (*(C12 + 3)) += LocalM5_3 + T1_3;
            (*(C22)) = LocalM5_0 + T2_0;
            (*(C22 + 1)) = LocalM5_1 + T2_1;
            (*(C22 + 2)) = LocalM5_2 + T2_2;
            (*(C22 + 3)) = LocalM5_3 + T2_3;
            (*(C21)) = (-*(C21)) + T2_0;
            (*(C21 + 1)) = (-*(C21 + 1)) + T2_1;
            (*(C21 + 2)) = (-*(C21 + 2)) + T2_2;
            (*(C21 + 3)) = (-*(C21 + 3)) + T2_3;
            M5 += 4;
            M2 += 4;
            T1sMULT += 4;
            C += 4;
            C12 += 4;
            C21 += 4;
            C22 += 4;
        }
        C = (REAL*)(((PTR)C) + RowIncrementC);
        C12 = (REAL*)(((PTR)C12) + RowIncrementC);
        C21 = (REAL*)(((PTR)C21) + RowIncrementC);
        C22 = (REAL*)(((PTR)C22) + RowIncrementC);
    }
    free(StartHeap);
}
/*Function: init_matrix, ID: 21*/
void init_matrix(int n, REAL* A, int an)
{
    /*init_matrix:21*/
    /*CompoundStmt:1173*/
    int i, j;
    for (i = 0; i < n; ++i)
        for (j = 0; j < n; ++j)
            (A[(i) * (an) + (j)]) = ((double)rand()) / (double)2147483647;
}
/*Function: compare_matrix, ID: 22*/
int compare_matrix(int n, REAL* A, int an, REAL* B, int bn)
{
    /*compare_matrix:22*/
    /*CompoundStmt:1191*/
    int i, j;
    REAL c;
    for (i = 0; i < n; ++i)
        for (j = 0; j < n; ++j) {
            c = (A[(i) * (an) + (j)]) - (B[(i) * (bn) + (j)]);
            if (c < 0.)
                c = -c;
            c = c / (A[(i) * (an) + (j)]);
            if (c > (9.9999999999999995E-7)) {
                {
                    if (bots_verbose_mode >= BOTS_VERBOSE_DEFAULT) {
                        fprintf(stdout, "Strassen: Wrong answer!\n");
                    }
                };
                return 2;
            }
        }
    return 1;
}
/*Function: alloc_matrix, ID: 23*/
REAL* alloc_matrix(int n)
{
    /*alloc_matrix:23*/
    /*CompoundStmt:1231*/
    return (REAL*)malloc(n * n * sizeof(REAL));
}
/*Function: strassen_main_par, ID: 24*/
void strassen_main_par(REAL* A, REAL* B, REAL* C, int n)
{
    /*strassen_main_par:24*/
    /*CompoundStmt:1238*/
    {
        /*CompoundStmt:1239*/
        if (bots_verbose_mode >= BOTS_VERBOSE_DEFAULT) {
            fprintf(stdout, "Computing parallel Strassen algorithm (n=%d) ", n);
        }
    }
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP1244>(ompNumThreads * DARTS_CODELETS_MULT, 0,
            RuntimeFinalCodelet, (REAL**)&((A)), (REAL**)&((B)), (REAL**)&((C)), (int*)&((n))));
    }
    {
        /*CompoundStmt:1248*/
        if (bots_verbose_mode >= BOTS_VERBOSE_DEFAULT) {
            fprintf(stdout, " completed!\n");
        }
    }
}
/*Function: strassen_main_seq, ID: 25*/
void strassen_main_seq(REAL* A, REAL* B, REAL* C, int n)
{
    /*strassen_main_seq:25*/
    /*CompoundStmt:1253*/
    {
        /*CompoundStmt:1254*/
        if (bots_verbose_mode >= BOTS_VERBOSE_DEFAULT) {
            fprintf(stdout, "Computing sequential Strassen algorithm (n=%d) ", n);
        }
    }
    OptimizedStrassenMultiply_seq(C, A, B, n, n, n, n, 1);
    {
        /*CompoundStmt:1260*/
        if (bots_verbose_mode >= BOTS_VERBOSE_DEFAULT) {
            fprintf(stdout, " completed!\n");
        }
    }
}
/*TP20: TP_OptimizedStrassenMultiply_par*/
void TP20::_checkInCodelets785::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    /*Init the vars for this region*/

    /*printing node 785: DeclStmt*/
    this->inputsTPParent->QuadrantSize_darts20[this->getID()]
        = (this->inputsTPParent->MatrixSize_darts20[this->getID()]) >> 1;

    /*printing node 787: DeclStmt*/
    this->inputsTPParent->QuadrantSizeInBytes_darts20[this->getID()] = sizeof(REAL)
            * (this->inputsTPParent->QuadrantSize_darts20[this->getID()])
            * (this->inputsTPParent->QuadrantSize_darts20[this->getID()])
        + 32;

    /*printing node 792: DeclStmt*/

    /*printing node 793: DeclStmt*/

    /*printing node 794: DeclStmt*/

    /*printing node 795: DeclStmt*/
    this->inputsTPParent->TempMatrixOffset_darts20[this->getID()] = 0;

    /*printing node 796: DeclStmt*/
    this->inputsTPParent->MatrixOffsetA_darts20[this->getID()] = 0;

    /*printing node 797: DeclStmt*/
    this->inputsTPParent->MatrixOffsetB_darts20[this->getID()] = 0;

    /*printing node 798: DeclStmt*/

    /*printing node 799: DeclStmt*/

    /*printing node 800: DeclStmt*/
    this->inputsTPParent->RowIncrementA_darts20[this->getID()]
        = ((this->inputsTPParent->RowWidthA_darts20[this->getID()])
              - (this->inputsTPParent->QuadrantSize_darts20[this->getID()]))
        << 3;

    /*printing node 803: DeclStmt*/
    this->inputsTPParent->RowIncrementB_darts20[this->getID()]
        = ((this->inputsTPParent->RowWidthB_darts20[this->getID()])
              - (this->inputsTPParent->QuadrantSize_darts20[this->getID()]))
        << 3;

    /*printing node 806: DeclStmt*/
    this->inputsTPParent->RowIncrementC_darts20[this->getID()]
        = ((this->inputsTPParent->RowWidthC_darts20[this->getID()])
              - (this->inputsTPParent->QuadrantSize_darts20[this->getID()]))
        << 3;

    /*printing node 809: IfStmt*/
    if ((this->inputsTPParent->MatrixSize_darts20[this->getID()])
        <= (unsigned int)bots_app_cutoff_value) {
        MultiplyByDivideAndConquer((this->inputsTPParent->C_darts20[this->getID()]),
            (this->inputsTPParent->A_darts20[this->getID()]),
            (this->inputsTPParent->B_darts20[this->getID()]),
            (this->inputsTPParent->MatrixSize_darts20[this->getID()]),
            (this->inputsTPParent->RowWidthC_darts20[this->getID()]),
            (this->inputsTPParent->RowWidthA_darts20[this->getID()]),
            (this->inputsTPParent->RowWidthB_darts20[this->getID()]), 0);
        /*return*/
        myTP->controlTPParent->nextCodeletsOptimizedStrassenMultiply_par[this->getID()]->decDep();
        return;
    }

    /*printing node 814: BinaryOperator*/
    (this->inputsTPParent->A12_darts20[this->getID()])
        = (this->inputsTPParent->A_darts20[this->getID()])
        + (this->inputsTPParent->QuadrantSize_darts20[this->getID()]);

    /*printing node 816: BinaryOperator*/
    (this->inputsTPParent->B12_darts20[this->getID()])
        = (this->inputsTPParent->B_darts20[this->getID()])
        + (this->inputsTPParent->QuadrantSize_darts20[this->getID()]);

    /*printing node 818: BinaryOperator*/
    (this->inputsTPParent->C12_darts20[this->getID()])
        = (this->inputsTPParent->C_darts20[this->getID()])
        + (this->inputsTPParent->QuadrantSize_darts20[this->getID()]);

    /*printing node 820: BinaryOperator*/
    (this->inputsTPParent->A21_darts20[this->getID()])
        = (this->inputsTPParent->A_darts20[this->getID()])
        + ((this->inputsTPParent->RowWidthA_darts20[this->getID()])
              * (this->inputsTPParent->QuadrantSize_darts20[this->getID()]));

    /*printing node 823: BinaryOperator*/
    (this->inputsTPParent->B21_darts20[this->getID()])
        = (this->inputsTPParent->B_darts20[this->getID()])
        + ((this->inputsTPParent->RowWidthB_darts20[this->getID()])
              * (this->inputsTPParent->QuadrantSize_darts20[this->getID()]));

    /*printing node 826: BinaryOperator*/
    (this->inputsTPParent->C21_darts20[this->getID()])
        = (this->inputsTPParent->C_darts20[this->getID()])
        + ((this->inputsTPParent->RowWidthC_darts20[this->getID()])
              * (this->inputsTPParent->QuadrantSize_darts20[this->getID()]));

    /*printing node 829: BinaryOperator*/
    (this->inputsTPParent->A22_darts20[this->getID()])
        = (this->inputsTPParent->A21_darts20[this->getID()])
        + (this->inputsTPParent->QuadrantSize_darts20[this->getID()]);

    /*printing node 831: BinaryOperator*/
    (this->inputsTPParent->B22_darts20[this->getID()])
        = (this->inputsTPParent->B21_darts20[this->getID()])
        + (this->inputsTPParent->QuadrantSize_darts20[this->getID()]);

    /*printing node 833: BinaryOperator*/
    (this->inputsTPParent->C22_darts20[this->getID()])
        = (this->inputsTPParent->C21_darts20[this->getID()])
        + (this->inputsTPParent->QuadrantSize_darts20[this->getID()]);

    /*printing node 835: BinaryOperator*/
    (this->inputsTPParent->Heap_darts20[this->getID()])
        = (char*)malloc((this->inputsTPParent->QuadrantSizeInBytes_darts20[this->getID()]) * 11);

    /*printing node 839: BinaryOperator*/
    (this->inputsTPParent->StartHeap_darts20[this->getID()])
        = (void*)(this->inputsTPParent->Heap_darts20[this->getID()]);

    /*printing node 841: IfStmt*/
    if (((PTR)(this->inputsTPParent->Heap_darts20[this->getID()])) & 31) {
        (this->inputsTPParent->Heap_darts20[this->getID()])
            = (char*)(((PTR)(this->inputsTPParent->Heap_darts20[this->getID()])) + 32
                - (((PTR)(this->inputsTPParent->Heap_darts20[this->getID()])) & 31));
    }

    /*printing node 851: BinaryOperator*/
    (this->inputsTPParent->S1_darts20[this->getID()])
        = (REAL*)(this->inputsTPParent->Heap_darts20[this->getID()]);

    /*printing node 853: CompoundAssignOperator*/
    (this->inputsTPParent->Heap_darts20[this->getID()])
        += (this->inputsTPParent->QuadrantSizeInBytes_darts20[this->getID()]);

    /*printing node 854: BinaryOperator*/
    (this->inputsTPParent->S2_darts20[this->getID()])
        = (REAL*)(this->inputsTPParent->Heap_darts20[this->getID()]);

    /*printing node 856: CompoundAssignOperator*/
    (this->inputsTPParent->Heap_darts20[this->getID()])
        += (this->inputsTPParent->QuadrantSizeInBytes_darts20[this->getID()]);

    /*printing node 857: BinaryOperator*/
    (this->inputsTPParent->S3_darts20[this->getID()])
        = (REAL*)(this->inputsTPParent->Heap_darts20[this->getID()]);

    /*printing node 859: CompoundAssignOperator*/
    (this->inputsTPParent->Heap_darts20[this->getID()])
        += (this->inputsTPParent->QuadrantSizeInBytes_darts20[this->getID()]);

    /*printing node 860: BinaryOperator*/
    (this->inputsTPParent->S4_darts20[this->getID()])
        = (REAL*)(this->inputsTPParent->Heap_darts20[this->getID()]);

    /*printing node 862: CompoundAssignOperator*/
    (this->inputsTPParent->Heap_darts20[this->getID()])
        += (this->inputsTPParent->QuadrantSizeInBytes_darts20[this->getID()]);

    /*printing node 863: BinaryOperator*/
    (this->inputsTPParent->S5_darts20[this->getID()])
        = (REAL*)(this->inputsTPParent->Heap_darts20[this->getID()]);

    /*printing node 865: CompoundAssignOperator*/
    (this->inputsTPParent->Heap_darts20[this->getID()])
        += (this->inputsTPParent->QuadrantSizeInBytes_darts20[this->getID()]);

    /*printing node 866: BinaryOperator*/
    (this->inputsTPParent->S6_darts20[this->getID()])
        = (REAL*)(this->inputsTPParent->Heap_darts20[this->getID()]);

    /*printing node 868: CompoundAssignOperator*/
    (this->inputsTPParent->Heap_darts20[this->getID()])
        += (this->inputsTPParent->QuadrantSizeInBytes_darts20[this->getID()]);

    /*printing node 869: BinaryOperator*/
    (this->inputsTPParent->S7_darts20[this->getID()])
        = (REAL*)(this->inputsTPParent->Heap_darts20[this->getID()]);

    /*printing node 871: CompoundAssignOperator*/
    (this->inputsTPParent->Heap_darts20[this->getID()])
        += (this->inputsTPParent->QuadrantSizeInBytes_darts20[this->getID()]);

    /*printing node 872: BinaryOperator*/
    (this->inputsTPParent->S8_darts20[this->getID()])
        = (REAL*)(this->inputsTPParent->Heap_darts20[this->getID()]);

    /*printing node 874: CompoundAssignOperator*/
    (this->inputsTPParent->Heap_darts20[this->getID()])
        += (this->inputsTPParent->QuadrantSizeInBytes_darts20[this->getID()]);

    /*printing node 875: BinaryOperator*/
    (this->inputsTPParent->M2_darts20[this->getID()])
        = (REAL*)(this->inputsTPParent->Heap_darts20[this->getID()]);

    /*printing node 877: CompoundAssignOperator*/
    (this->inputsTPParent->Heap_darts20[this->getID()])
        += (this->inputsTPParent->QuadrantSizeInBytes_darts20[this->getID()]);

    /*printing node 878: BinaryOperator*/
    (this->inputsTPParent->M5_darts20[this->getID()])
        = (REAL*)(this->inputsTPParent->Heap_darts20[this->getID()]);

    /*printing node 880: CompoundAssignOperator*/
    (this->inputsTPParent->Heap_darts20[this->getID()])
        += (this->inputsTPParent->QuadrantSizeInBytes_darts20[this->getID()]);

    /*printing node 881: BinaryOperator*/
    (this->inputsTPParent->T1sMULT_darts20[this->getID()])
        = (REAL*)(this->inputsTPParent->Heap_darts20[this->getID()]);

    /*printing node 883: CompoundAssignOperator*/
    (this->inputsTPParent->Heap_darts20[this->getID()])
        += (this->inputsTPParent->QuadrantSizeInBytes_darts20[this->getID()]);

    /*printing node 884: ForStmt*/
    {
        REAL** A = &(this->inputsTPParent->A_darts20[this->getLocalID()]);
        (void)A /*PRIVATE*/;
        REAL** A12 = &(this->inputsTPParent->A12_darts20[this->getLocalID()]);
        (void)A12 /*PRIVATE*/;
        REAL** A21 = &(this->inputsTPParent->A21_darts20[this->getLocalID()]);
        (void)A21 /*PRIVATE*/;
        REAL** A22 = &(this->inputsTPParent->A22_darts20[this->getLocalID()]);
        (void)A22 /*PRIVATE*/;
        REAL** B = &(this->inputsTPParent->B_darts20[this->getLocalID()]);
        (void)B /*PRIVATE*/;
        REAL** B12 = &(this->inputsTPParent->B12_darts20[this->getLocalID()]);
        (void)B12 /*PRIVATE*/;
        REAL** B21 = &(this->inputsTPParent->B21_darts20[this->getLocalID()]);
        (void)B21 /*PRIVATE*/;
        REAL** B22 = &(this->inputsTPParent->B22_darts20[this->getLocalID()]);
        (void)B22 /*PRIVATE*/;
        unsigned int* Column = &(this->inputsTPParent->Column_darts20[this->getLocalID()]);
        (void)Column /*PRIVATE*/;
        PTR* MatrixOffsetA = &(this->inputsTPParent->MatrixOffsetA_darts20[this->getLocalID()]);
        (void)MatrixOffsetA /*PRIVATE*/;
        PTR* MatrixOffsetB = &(this->inputsTPParent->MatrixOffsetB_darts20[this->getLocalID()]);
        (void)MatrixOffsetB /*PRIVATE*/;
        unsigned int* QuadrantSize
            = &(this->inputsTPParent->QuadrantSize_darts20[this->getLocalID()]);
        (void)QuadrantSize /*PRIVATE*/;
        unsigned int* Row = &(this->inputsTPParent->Row_darts20[this->getLocalID()]);
        (void)Row /*PRIVATE*/;
        PTR* RowIncrementA = &(this->inputsTPParent->RowIncrementA_darts20[this->getLocalID()]);
        (void)RowIncrementA /*PRIVATE*/;
        PTR* RowIncrementB = &(this->inputsTPParent->RowIncrementB_darts20[this->getLocalID()]);
        (void)RowIncrementB /*PRIVATE*/;
        REAL** S1 = &(this->inputsTPParent->S1_darts20[this->getLocalID()]);
        (void)S1 /*PRIVATE*/;
        REAL** S2 = &(this->inputsTPParent->S2_darts20[this->getLocalID()]);
        (void)S2 /*PRIVATE*/;
        REAL** S3 = &(this->inputsTPParent->S3_darts20[this->getLocalID()]);
        (void)S3 /*PRIVATE*/;
        REAL** S4 = &(this->inputsTPParent->S4_darts20[this->getLocalID()]);
        (void)S4 /*PRIVATE*/;
        REAL** S5 = &(this->inputsTPParent->S5_darts20[this->getLocalID()]);
        (void)S5 /*PRIVATE*/;
        REAL** S6 = &(this->inputsTPParent->S6_darts20[this->getLocalID()]);
        (void)S6 /*PRIVATE*/;
        REAL** S7 = &(this->inputsTPParent->S7_darts20[this->getLocalID()]);
        (void)S7 /*PRIVATE*/;
        REAL** S8 = &(this->inputsTPParent->S8_darts20[this->getLocalID()]);
        (void)S8 /*PRIVATE*/;
        PTR* TempMatrixOffset
            = &(this->inputsTPParent->TempMatrixOffset_darts20[this->getLocalID()]);
        (void)TempMatrixOffset /*PRIVATE*/;
        /*Loop's init*/
        (this->inputsTPParent->Row_darts20[this->getID()]) = 0;
        unsigned int Row_darts_counter_temp20 = (this->inputsTPParent->Row_darts20[this->getID()]);
        for (; (Row_darts_counter_temp20) < (*QuadrantSize); (Row_darts_counter_temp20)++) {
            {
                /*Loop's init*/
                (*Column) = 0;
                unsigned int Column_darts_counter_temp20 = (*Column);
                for (; Column_darts_counter_temp20 < (*QuadrantSize);
                     Column_darts_counter_temp20++) {
                    (*(REAL*)(((PTR)(*S4)) + (*TempMatrixOffset)))
                        = (*(REAL*)(((PTR)(*A12)) + (*MatrixOffsetA)))
                        - ((*(REAL*)(((PTR)(*S2)) + (*TempMatrixOffset)))
                              = ((*(REAL*)(((PTR)(*S1)) + (*TempMatrixOffset)))
                                    = (*(REAL*)(((PTR)(*A21)) + (*MatrixOffsetA)))
                                        + (*(REAL*)(((PTR)(*A22)) + (*MatrixOffsetA))))
                                  - (*(REAL*)(((PTR)(*A)) + (*MatrixOffsetA))));
                    (*(REAL*)(((PTR)(*S8)) + (*TempMatrixOffset)))
                        = ((*(REAL*)(((PTR)(*S6)) + (*TempMatrixOffset)))
                              = (*(REAL*)(((PTR)(*B22)) + (*MatrixOffsetB)))
                                  - ((*(REAL*)(((PTR)(*S5)) + (*TempMatrixOffset)))
                                        = (*(REAL*)(((PTR)(*B12)) + (*MatrixOffsetB)))
                                            - (*(REAL*)(((PTR)(*B)) + (*MatrixOffsetB)))))
                        - (*(REAL*)(((PTR)(*B21)) + (*MatrixOffsetB)));
                    (*(REAL*)(((PTR)(*S3)) + (*TempMatrixOffset)))
                        = (*(REAL*)(((PTR)(*A)) + (*MatrixOffsetA)))
                        - (*(REAL*)(((PTR)(*A21)) + (*MatrixOffsetA)));
                    (*(REAL*)(((PTR)(*S7)) + (*TempMatrixOffset)))
                        = (*(REAL*)(((PTR)(*B22)) + (*MatrixOffsetB)))
                        - (*(REAL*)(((PTR)(*B12)) + (*MatrixOffsetB)));
                    (*TempMatrixOffset) += sizeof(REAL);
                    (*MatrixOffsetA) += sizeof(REAL);
                    (*MatrixOffsetB) += sizeof(REAL);
                }
                (*Column) = Column_darts_counter_temp20;
            }
            (*MatrixOffsetA) += (*RowIncrementA);
            (*MatrixOffsetB) += (*RowIncrementB);
        }
        (this->inputsTPParent->Row_darts20[this->getID()]) = Row_darts_counter_temp20;
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 785 nextRegion: 998 */
    myTP->controlTPParent->checkInCodelets998[this->getID()].decDep();
}
void TP20::_checkInCodelets998::fire(void)
{

    /*printing node 998: OMPTaskDirective*/
    /*syncNode: OMPTaskwaitDirective 1019*/
    /*Find the TP to which the nextRegion belongs*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->barrierCodelets1019[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 998*/
    _task998Inputs* task998Inputs
        = new _task998Inputs(&((this->inputsTPParent->A_darts20[this->getID()])),
            &((this->inputsTPParent->B_darts20[this->getID()])),
            &((this->inputsTPParent->Depth_darts20[this->getID()])),
            &((this->inputsTPParent->M2_darts20[this->getID()])),
            &((this->inputsTPParent->QuadrantSize_darts20[this->getID()])),
            &((this->inputsTPParent->RowWidthA_darts20[this->getID()])),
            &((this->inputsTPParent->RowWidthB_darts20[this->getID()])), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task998Inputs[this->getID()] = task998Inputs;
    invoke<TP998>(myTP, 1, this->getID(), myTP, task998Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 998 nextRegion: 1001 */
    myTP->controlTPParent->checkInCodelets1001[this->getID()].decDep();
}
void TP20::_checkInCodelets1001::fire(void)
{

    /*printing node 1001: OMPTaskDirective*/
    /*syncNode: OMPTaskwaitDirective 1019*/
    /*Find the TP to which the nextRegion belongs*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->barrierCodelets1019[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 1001*/
    _task1001Inputs* task1001Inputs
        = new _task1001Inputs(&((this->inputsTPParent->Depth_darts20[this->getID()])),
            &((this->inputsTPParent->M5_darts20[this->getID()])),
            &((this->inputsTPParent->QuadrantSize_darts20[this->getID()])),
            &((this->inputsTPParent->S1_darts20[this->getID()])),
            &((this->inputsTPParent->S5_darts20[this->getID()])), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task1001Inputs[this->getID()] = task1001Inputs;
    invoke<TP1001>(myTP, 1, this->getID(), myTP, task1001Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 1001 nextRegion: 1004 */
    myTP->controlTPParent->checkInCodelets1004[this->getID()].decDep();
}
void TP20::_checkInCodelets1004::fire(void)
{

    /*printing node 1004: OMPTaskDirective*/
    /*syncNode: OMPTaskwaitDirective 1019*/
    /*Find the TP to which the nextRegion belongs*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->barrierCodelets1019[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 1004*/
    _task1004Inputs* task1004Inputs
        = new _task1004Inputs(&((this->inputsTPParent->Depth_darts20[this->getID()])),
            &((this->inputsTPParent->QuadrantSize_darts20[this->getID()])),
            &((this->inputsTPParent->S2_darts20[this->getID()])),
            &((this->inputsTPParent->S6_darts20[this->getID()])),
            &((this->inputsTPParent->T1sMULT_darts20[this->getID()])), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task1004Inputs[this->getID()] = task1004Inputs;
    invoke<TP1004>(myTP, 1, this->getID(), myTP, task1004Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 1004 nextRegion: 1007 */
    myTP->controlTPParent->checkInCodelets1007[this->getID()].decDep();
}
void TP20::_checkInCodelets1007::fire(void)
{

    /*printing node 1007: OMPTaskDirective*/
    /*syncNode: OMPTaskwaitDirective 1019*/
    /*Find the TP to which the nextRegion belongs*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->barrierCodelets1019[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 1007*/
    _task1007Inputs* task1007Inputs
        = new _task1007Inputs(&((this->inputsTPParent->C22_darts20[this->getID()])),
            &((this->inputsTPParent->Depth_darts20[this->getID()])),
            &((this->inputsTPParent->QuadrantSize_darts20[this->getID()])),
            &((this->inputsTPParent->RowWidthC_darts20[this->getID()])),
            &((this->inputsTPParent->S3_darts20[this->getID()])),
            &((this->inputsTPParent->S7_darts20[this->getID()])), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task1007Inputs[this->getID()] = task1007Inputs;
    invoke<TP1007>(myTP, 1, this->getID(), myTP, task1007Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 1007 nextRegion: 1010 */
    myTP->controlTPParent->checkInCodelets1010[this->getID()].decDep();
}
void TP20::_checkInCodelets1010::fire(void)
{

    /*printing node 1010: OMPTaskDirective*/
    /*syncNode: OMPTaskwaitDirective 1019*/
    /*Find the TP to which the nextRegion belongs*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->barrierCodelets1019[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 1010*/
    _task1010Inputs* task1010Inputs
        = new _task1010Inputs(&((this->inputsTPParent->A12_darts20[this->getID()])),
            &((this->inputsTPParent->B21_darts20[this->getID()])),
            &((this->inputsTPParent->C_darts20[this->getID()])),
            &((this->inputsTPParent->Depth_darts20[this->getID()])),
            &((this->inputsTPParent->QuadrantSize_darts20[this->getID()])),
            &((this->inputsTPParent->RowWidthA_darts20[this->getID()])),
            &((this->inputsTPParent->RowWidthB_darts20[this->getID()])),
            &((this->inputsTPParent->RowWidthC_darts20[this->getID()])), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task1010Inputs[this->getID()] = task1010Inputs;
    invoke<TP1010>(myTP, 1, this->getID(), myTP, task1010Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 1010 nextRegion: 1013 */
    myTP->controlTPParent->checkInCodelets1013[this->getID()].decDep();
}
void TP20::_checkInCodelets1013::fire(void)
{

    /*printing node 1013: OMPTaskDirective*/
    /*syncNode: OMPTaskwaitDirective 1019*/
    /*Find the TP to which the nextRegion belongs*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->barrierCodelets1019[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 1013*/
    _task1013Inputs* task1013Inputs
        = new _task1013Inputs(&((this->inputsTPParent->B22_darts20[this->getID()])),
            &((this->inputsTPParent->C12_darts20[this->getID()])),
            &((this->inputsTPParent->Depth_darts20[this->getID()])),
            &((this->inputsTPParent->QuadrantSize_darts20[this->getID()])),
            &((this->inputsTPParent->RowWidthB_darts20[this->getID()])),
            &((this->inputsTPParent->RowWidthC_darts20[this->getID()])),
            &((this->inputsTPParent->S4_darts20[this->getID()])), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task1013Inputs[this->getID()] = task1013Inputs;
    invoke<TP1013>(myTP, 1, this->getID(), myTP, task1013Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 1013 nextRegion: 1016 */
    myTP->controlTPParent->checkInCodelets1016[this->getID()].decDep();
}
void TP20::_checkInCodelets1016::fire(void)
{

    /*printing node 1016: OMPTaskDirective*/
    /*syncNode: OMPTaskwaitDirective 1019*/
    /*Find the TP to which the nextRegion belongs*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->barrierCodelets1019[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 1016*/
    _task1016Inputs* task1016Inputs
        = new _task1016Inputs(&((this->inputsTPParent->A22_darts20[this->getID()])),
            &((this->inputsTPParent->C21_darts20[this->getID()])),
            &((this->inputsTPParent->Depth_darts20[this->getID()])),
            &((this->inputsTPParent->QuadrantSize_darts20[this->getID()])),
            &((this->inputsTPParent->RowWidthA_darts20[this->getID()])),
            &((this->inputsTPParent->RowWidthC_darts20[this->getID()])),
            &((this->inputsTPParent->S8_darts20[this->getID()])), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task1016Inputs[this->getID()] = task1016Inputs;
    invoke<TP1016>(myTP, 1, this->getID(), myTP, task1016Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 1016 nextRegion: 1019 */
    myTP->controlTPParent->barrierCodelets1019[0].decDep();
}
void TP20::_barrierCodelets1019::fire(void)
{
    TP20* myTP = static_cast<TP20*>(myTP_);
    {
        for (size_t codeletsCounter = 0; codeletsCounter < myTP->numThreads; codeletsCounter++) {
            myTP->checkInCodelets1020[codeletsCounter].decDep();
        }
    }
}
void TP20::_checkInCodelets1020::fire(void)
{

    /*printing node 1020: ForStmt*/
    {
        REAL** C = &(this->inputsTPParent->C_darts20[this->getLocalID()]);
        (void)C /*PRIVATE*/;
        REAL** C12 = &(this->inputsTPParent->C12_darts20[this->getLocalID()]);
        (void)C12 /*PRIVATE*/;
        REAL** C21 = &(this->inputsTPParent->C21_darts20[this->getLocalID()]);
        (void)C21 /*PRIVATE*/;
        REAL** C22 = &(this->inputsTPParent->C22_darts20[this->getLocalID()]);
        (void)C22 /*PRIVATE*/;
        unsigned int* Column = &(this->inputsTPParent->Column_darts20[this->getLocalID()]);
        (void)Column /*PRIVATE*/;
        REAL* LocalM2_0 = &(this->inputsTPParent->LocalM2_0_darts20[this->getLocalID()]);
        (void)LocalM2_0 /*PRIVATE*/;
        REAL* LocalM2_1 = &(this->inputsTPParent->LocalM2_1_darts20[this->getLocalID()]);
        (void)LocalM2_1 /*PRIVATE*/;
        REAL* LocalM2_2 = &(this->inputsTPParent->LocalM2_2_darts20[this->getLocalID()]);
        (void)LocalM2_2 /*PRIVATE*/;
        REAL* LocalM2_3 = &(this->inputsTPParent->LocalM2_3_darts20[this->getLocalID()]);
        (void)LocalM2_3 /*PRIVATE*/;
        REAL* LocalM5_0 = &(this->inputsTPParent->LocalM5_0_darts20[this->getLocalID()]);
        (void)LocalM5_0 /*PRIVATE*/;
        REAL* LocalM5_1 = &(this->inputsTPParent->LocalM5_1_darts20[this->getLocalID()]);
        (void)LocalM5_1 /*PRIVATE*/;
        REAL* LocalM5_2 = &(this->inputsTPParent->LocalM5_2_darts20[this->getLocalID()]);
        (void)LocalM5_2 /*PRIVATE*/;
        REAL* LocalM5_3 = &(this->inputsTPParent->LocalM5_3_darts20[this->getLocalID()]);
        (void)LocalM5_3 /*PRIVATE*/;
        REAL** M2 = &(this->inputsTPParent->M2_darts20[this->getLocalID()]);
        (void)M2 /*PRIVATE*/;
        REAL** M5 = &(this->inputsTPParent->M5_darts20[this->getLocalID()]);
        (void)M5 /*PRIVATE*/;
        unsigned int* QuadrantSize
            = &(this->inputsTPParent->QuadrantSize_darts20[this->getLocalID()]);
        (void)QuadrantSize /*PRIVATE*/;
        unsigned int* Row = &(this->inputsTPParent->Row_darts20[this->getLocalID()]);
        (void)Row /*PRIVATE*/;
        PTR* RowIncrementC = &(this->inputsTPParent->RowIncrementC_darts20[this->getLocalID()]);
        (void)RowIncrementC /*PRIVATE*/;
        REAL* T1_0 = &(this->inputsTPParent->T1_0_darts20[this->getLocalID()]);
        (void)T1_0 /*PRIVATE*/;
        REAL* T1_1 = &(this->inputsTPParent->T1_1_darts20[this->getLocalID()]);
        (void)T1_1 /*PRIVATE*/;
        REAL* T1_2 = &(this->inputsTPParent->T1_2_darts20[this->getLocalID()]);
        (void)T1_2 /*PRIVATE*/;
        REAL* T1_3 = &(this->inputsTPParent->T1_3_darts20[this->getLocalID()]);
        (void)T1_3 /*PRIVATE*/;
        REAL** T1sMULT = &(this->inputsTPParent->T1sMULT_darts20[this->getLocalID()]);
        (void)T1sMULT /*PRIVATE*/;
        REAL* T2_0 = &(this->inputsTPParent->T2_0_darts20[this->getLocalID()]);
        (void)T2_0 /*PRIVATE*/;
        REAL* T2_1 = &(this->inputsTPParent->T2_1_darts20[this->getLocalID()]);
        (void)T2_1 /*PRIVATE*/;
        REAL* T2_2 = &(this->inputsTPParent->T2_2_darts20[this->getLocalID()]);
        (void)T2_2 /*PRIVATE*/;
        REAL* T2_3 = &(this->inputsTPParent->T2_3_darts20[this->getLocalID()]);
        (void)T2_3 /*PRIVATE*/;
        /*Loop's init*/
        (this->inputsTPParent->Row_darts20[this->getID()]) = 0;
        unsigned int Row_darts_counter_temp20 = (this->inputsTPParent->Row_darts20[this->getID()]);
        for (; (Row_darts_counter_temp20) < (*QuadrantSize); (Row_darts_counter_temp20)++) {
            {
                /*Loop's init*/
                (*Column) = 0;
                unsigned int Column_darts_counter_temp20 = (*Column);
                for (; Column_darts_counter_temp20 < (*QuadrantSize);
                     Column_darts_counter_temp20 += 4) {
                    *LocalM5_0 = *((*M5));
                    *LocalM5_1 = *((*M5) + 1);
                    *LocalM5_2 = *((*M5) + 2);
                    *LocalM5_3 = *((*M5) + 3);
                    *LocalM2_0 = *((*M2));
                    *LocalM2_1 = *((*M2) + 1);
                    *LocalM2_2 = *((*M2) + 2);
                    *LocalM2_3 = *((*M2) + 3);
                    *T1_0 = *((*T1sMULT)) + (*LocalM2_0);
                    *T1_1 = *((*T1sMULT) + 1) + (*LocalM2_1);
                    *T1_2 = *((*T1sMULT) + 2) + (*LocalM2_2);
                    *T1_3 = *((*T1sMULT) + 3) + (*LocalM2_3);
                    *T2_0 = *((*C22)) + (*T1_0);
                    *T2_1 = *((*C22) + 1) + (*T1_1);
                    *T2_2 = *((*C22) + 2) + (*T1_2);
                    *T2_3 = *((*C22) + 3) + (*T1_3);
                    (*((*C))) += (*LocalM2_0);
                    (*((*C) + 1)) += (*LocalM2_1);
                    (*((*C) + 2)) += (*LocalM2_2);
                    (*((*C) + 3)) += (*LocalM2_3);
                    (*((*C12))) += (*LocalM5_0) + (*T1_0);
                    (*((*C12) + 1)) += (*LocalM5_1) + (*T1_1);
                    (*((*C12) + 2)) += (*LocalM5_2) + (*T1_2);
                    (*((*C12) + 3)) += (*LocalM5_3) + (*T1_3);
                    (*((*C22))) = (*LocalM5_0) + (*T2_0);
                    (*((*C22) + 1)) = (*LocalM5_1) + (*T2_1);
                    (*((*C22) + 2)) = (*LocalM5_2) + (*T2_2);
                    (*((*C22) + 3)) = (*LocalM5_3) + (*T2_3);
                    (*((*C21))) = (-*((*C21))) + (*T2_0);
                    (*((*C21) + 1)) = (-*((*C21) + 1)) + (*T2_1);
                    (*((*C21) + 2)) = (-*((*C21) + 2)) + (*T2_2);
                    (*((*C21) + 3)) = (-*((*C21) + 3)) + (*T2_3);
                    (*M5) += 4;
                    (*M2) += 4;
                    (*T1sMULT) += 4;
                    (*C) += 4;
                    (*C12) += 4;
                    (*C21) += 4;
                    (*C22) += 4;
                }
                (*Column) = Column_darts_counter_temp20;
            }
            (*C) = (REAL*)(((PTR)(*C)) + (*RowIncrementC));
            (*C12) = (REAL*)(((PTR)(*C12)) + (*RowIncrementC));
            (*C21) = (REAL*)(((PTR)(*C21)) + (*RowIncrementC));
            (*C22) = (REAL*)(((PTR)(*C22)) + (*RowIncrementC));
        }
        (this->inputsTPParent->Row_darts20[this->getID()]) = Row_darts_counter_temp20;
    }

    /*printing node 1172: CallExpr*/
    free((this->inputsTPParent->StartHeap_darts20[this->getID()]));
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/

    myTP->controlTPParent->nextCodeletsOptimizedStrassenMultiply_par[this->getID()]->decDep();
}
TP20::TP20(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_mainNextCodelet,
    darts::Codelet* in_mainSyncCodelet, TP20** in_ptrToThisFunctionTP, REAL* in_C, REAL* in_A,
    REAL* in_B, unsigned int in_MatrixSize, unsigned int in_RowWidthC, unsigned int in_RowWidthA,
    unsigned int in_RowWidthB, int in_Depth)
    : ompTP(in_numThreads, in_mainCodeletID)
    , ptrToThisFunctionTP(in_ptrToThisFunctionTP)
    , inputsTPParent(this)
    , controlTPParent(this)
    , nextCodeletsOptimizedStrassenMultiply_par(new Codelet*[in_numThreads])
    , nextSyncCodeletsOptimizedStrassenMultiply_par(new Codelet*[in_numThreads])
    , C_darts20(new REAL*[this->numThreads])
    , A_darts20(new REAL*[this->numThreads])
    , B_darts20(new REAL*[this->numThreads])
    , MatrixSize_darts20(new unsigned int[this->numThreads])
    , RowWidthC_darts20(new unsigned int[this->numThreads])
    , RowWidthA_darts20(new unsigned int[this->numThreads])
    , RowWidthB_darts20(new unsigned int[this->numThreads])
    , Depth_darts20(new int[this->numThreads])
    , A12_darts20(new REAL*[this->numThreads])
    , A21_darts20(new REAL*[this->numThreads])
    , A22_darts20(new REAL*[this->numThreads])
    , B12_darts20(new REAL*[this->numThreads])
    , B21_darts20(new REAL*[this->numThreads])
    , B22_darts20(new REAL*[this->numThreads])
    , C12_darts20(new REAL*[this->numThreads])
    , C21_darts20(new REAL*[this->numThreads])
    , C22_darts20(new REAL*[this->numThreads])
    , Column_darts20(new unsigned int[this->numThreads])
    , Heap_darts20(new char*[this->numThreads])
    , LocalM2_0_darts20(new REAL[this->numThreads])
    , LocalM2_1_darts20(new REAL[this->numThreads])
    , LocalM2_2_darts20(new REAL[this->numThreads])
    , LocalM2_3_darts20(new REAL[this->numThreads])
    , LocalM5_0_darts20(new REAL[this->numThreads])
    , LocalM5_1_darts20(new REAL[this->numThreads])
    , LocalM5_2_darts20(new REAL[this->numThreads])
    , LocalM5_3_darts20(new REAL[this->numThreads])
    , M2_darts20(new REAL*[this->numThreads])
    , M5_darts20(new REAL*[this->numThreads])
    , MatrixOffsetA_darts20(new PTR[this->numThreads])
    , MatrixOffsetB_darts20(new PTR[this->numThreads])
    , QuadrantSize_darts20(new unsigned int[this->numThreads])
    , QuadrantSizeInBytes_darts20(new unsigned int[this->numThreads])
    , Row_darts20(new unsigned int[this->numThreads])
    , RowIncrementA_darts20(new PTR[this->numThreads])
    , RowIncrementB_darts20(new PTR[this->numThreads])
    , RowIncrementC_darts20(new PTR[this->numThreads])
    , S1_darts20(new REAL*[this->numThreads])
    , S2_darts20(new REAL*[this->numThreads])
    , S3_darts20(new REAL*[this->numThreads])
    , S4_darts20(new REAL*[this->numThreads])
    , S5_darts20(new REAL*[this->numThreads])
    , S6_darts20(new REAL*[this->numThreads])
    , S7_darts20(new REAL*[this->numThreads])
    , S8_darts20(new REAL*[this->numThreads])
    , StartHeap_darts20(new void*[this->numThreads])
    , T1_0_darts20(new REAL[this->numThreads])
    , T1_1_darts20(new REAL[this->numThreads])
    , T1_2_darts20(new REAL[this->numThreads])
    , T1_3_darts20(new REAL[this->numThreads])
    , T1sMULT_darts20(new REAL*[this->numThreads])
    , T2_0_darts20(new REAL[this->numThreads])
    , T2_1_darts20(new REAL[this->numThreads])
    , T2_2_darts20(new REAL[this->numThreads])
    , T2_3_darts20(new REAL[this->numThreads])
    , TempMatrixOffset_darts20(new PTR[this->numThreads])
    , task998Inputs(new _task998Inputs*[this->numThreads])
    , task1001Inputs(new _task1001Inputs*[this->numThreads])
    , task1004Inputs(new _task1004Inputs*[this->numThreads])
    , task1007Inputs(new _task1007Inputs*[this->numThreads])
    , task1010Inputs(new _task1010Inputs*[this->numThreads])
    , task1013Inputs(new _task1013Inputs*[this->numThreads])
    , task1016Inputs(new _task1016Inputs*[this->numThreads])
    , checkInCodelets785(new _checkInCodelets785[this->numThreads])
    , checkInCodelets998(new _checkInCodelets998[this->numThreads])
    , checkInCodelets1001(new _checkInCodelets1001[this->numThreads])
    , checkInCodelets1004(new _checkInCodelets1004[this->numThreads])
    , checkInCodelets1007(new _checkInCodelets1007[this->numThreads])
    , checkInCodelets1010(new _checkInCodelets1010[this->numThreads])
    , checkInCodelets1013(new _checkInCodelets1013[this->numThreads])
    , checkInCodelets1016(new _checkInCodelets1016[this->numThreads])
    , barrierCodelets1019(new _barrierCodelets1019[1])
    , checkInCodelets1020(new _checkInCodelets1020[this->numThreads])
{
    barrierCodelets1019[0] = _barrierCodelets1019(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets1020* checkInCodelets1020Ptr = (this->checkInCodelets1020);
    _checkInCodelets1016* checkInCodelets1016Ptr = (this->checkInCodelets1016);
    _checkInCodelets1013* checkInCodelets1013Ptr = (this->checkInCodelets1013);
    _checkInCodelets1010* checkInCodelets1010Ptr = (this->checkInCodelets1010);
    _checkInCodelets1007* checkInCodelets1007Ptr = (this->checkInCodelets1007);
    _checkInCodelets1004* checkInCodelets1004Ptr = (this->checkInCodelets1004);
    _checkInCodelets1001* checkInCodelets1001Ptr = (this->checkInCodelets1001);
    _checkInCodelets998* checkInCodelets998Ptr = (this->checkInCodelets998);
    _checkInCodelets785* checkInCodelets785Ptr = (this->checkInCodelets785);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets785);
#endif
    for (size_t codeletCounter = 0; codeletCounter < this->numThreads; codeletCounter++) {
        (*checkInCodelets1020Ptr) = _checkInCodelets1020(1, 1, this, codeletCounter);
        checkInCodelets1020Ptr++;
        (*checkInCodelets1016Ptr) = _checkInCodelets1016(1, 1, this, codeletCounter);
        checkInCodelets1016Ptr++;
        (*checkInCodelets1013Ptr) = _checkInCodelets1013(1, 1, this, codeletCounter);
        checkInCodelets1013Ptr++;
        (*checkInCodelets1010Ptr) = _checkInCodelets1010(1, 1, this, codeletCounter);
        checkInCodelets1010Ptr++;
        (*checkInCodelets1007Ptr) = _checkInCodelets1007(1, 1, this, codeletCounter);
        checkInCodelets1007Ptr++;
        (*checkInCodelets1004Ptr) = _checkInCodelets1004(1, 1, this, codeletCounter);
        checkInCodelets1004Ptr++;
        (*checkInCodelets1001Ptr) = _checkInCodelets1001(1, 1, this, codeletCounter);
        checkInCodelets1001Ptr++;
        (*checkInCodelets998Ptr) = _checkInCodelets998(1, 1, this, codeletCounter);
        checkInCodelets998Ptr++;
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets785Ptr) = _checkInCodelets785(2, 1, this, codeletCounter);
#else
        (*checkInCodelets785Ptr) = _checkInCodelets785(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets785Ptr).decDep();
        checkInCodelets785Ptr++;
    }
    if (this->numThreads == 1) {
        this->nextCodeletsOptimizedStrassenMultiply_par[0] = in_mainNextCodelet;
        this->nextSyncCodeletsOptimizedStrassenMultiply_par[0] = in_mainSyncCodelet;
        this->C_darts20[0] = in_C;
        this->A_darts20[0] = in_A;
        this->B_darts20[0] = in_B;
        this->MatrixSize_darts20[0] = in_MatrixSize;
        this->RowWidthC_darts20[0] = in_RowWidthC;
        this->RowWidthA_darts20[0] = in_RowWidthA;
        this->RowWidthB_darts20[0] = in_RowWidthB;
        this->Depth_darts20[0] = in_Depth;
        this->availableCodelets[0] = 1;
    } else {
        this->C_darts20[this->mainCodeletID] = in_C;
        this->A_darts20[this->mainCodeletID] = in_A;
        this->B_darts20[this->mainCodeletID] = in_B;
        this->MatrixSize_darts20[this->mainCodeletID] = in_MatrixSize;
        this->RowWidthC_darts20[this->mainCodeletID] = in_RowWidthC;
        this->RowWidthA_darts20[this->mainCodeletID] = in_RowWidthA;
        this->RowWidthB_darts20[this->mainCodeletID] = in_RowWidthB;
        this->Depth_darts20[this->mainCodeletID] = in_Depth;
        this->nextCodeletsOptimizedStrassenMultiply_par[in_mainCodeletID] = in_mainNextCodelet;
        this->nextSyncCodeletsOptimizedStrassenMultiply_par[in_mainCodeletID] = in_mainSyncCodelet;
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
    delete[] task998Inputs;
    delete[] task1001Inputs;
    delete[] task1004Inputs;
    delete[] task1007Inputs;
    delete[] task1010Inputs;
    delete[] task1013Inputs;
    delete[] task1016Inputs;
    delete[] checkInCodelets1020;
    delete[] barrierCodelets1019;
    delete[] checkInCodelets1016;
    delete[] checkInCodelets1013;
    delete[] checkInCodelets1010;
    delete[] checkInCodelets1007;
    delete[] checkInCodelets1004;
    delete[] checkInCodelets1001;
    delete[] checkInCodelets998;
    delete[] checkInCodelets785;
    delete[] nextCodeletsOptimizedStrassenMultiply_par;
    delete[] nextSyncCodeletsOptimizedStrassenMultiply_par;
    delete[] C_darts20;
    delete[] A_darts20;
    delete[] B_darts20;
    delete[] MatrixSize_darts20;
    delete[] RowWidthC_darts20;
    delete[] RowWidthA_darts20;
    delete[] RowWidthB_darts20;
    delete[] Depth_darts20;
    delete[] A12_darts20;
    delete[] A21_darts20;
    delete[] A22_darts20;
    delete[] B12_darts20;
    delete[] B21_darts20;
    delete[] B22_darts20;
    delete[] C12_darts20;
    delete[] C21_darts20;
    delete[] C22_darts20;
    delete[] Column_darts20;
    delete[] Heap_darts20;
    delete[] LocalM2_0_darts20;
    delete[] LocalM2_1_darts20;
    delete[] LocalM2_2_darts20;
    delete[] LocalM2_3_darts20;
    delete[] LocalM5_0_darts20;
    delete[] LocalM5_1_darts20;
    delete[] LocalM5_2_darts20;
    delete[] LocalM5_3_darts20;
    delete[] M2_darts20;
    delete[] M5_darts20;
    delete[] MatrixOffsetA_darts20;
    delete[] MatrixOffsetB_darts20;
    delete[] QuadrantSize_darts20;
    delete[] QuadrantSizeInBytes_darts20;
    delete[] Row_darts20;
    delete[] RowIncrementA_darts20;
    delete[] RowIncrementB_darts20;
    delete[] RowIncrementC_darts20;
    delete[] S1_darts20;
    delete[] S2_darts20;
    delete[] S3_darts20;
    delete[] S4_darts20;
    delete[] S5_darts20;
    delete[] S6_darts20;
    delete[] S7_darts20;
    delete[] S8_darts20;
    delete[] StartHeap_darts20;
    delete[] T1_0_darts20;
    delete[] T1_1_darts20;
    delete[] T1_2_darts20;
    delete[] T1_3_darts20;
    delete[] T1sMULT_darts20;
    delete[] T2_0_darts20;
    delete[] T2_1_darts20;
    delete[] T2_2_darts20;
    delete[] T2_3_darts20;
    delete[] TempMatrixOffset_darts20;
}
void TP20::setNewInputs(REAL* in_C, REAL* in_A, REAL* in_B, unsigned int in_MatrixSize,
    unsigned int in_RowWidthC, unsigned int in_RowWidthA, unsigned int in_RowWidthB, int in_Depth,
    size_t codeletID)
{
    this->C_darts20[codeletID] = in_C;
    this->A_darts20[codeletID] = in_A;
    this->B_darts20[codeletID] = in_B;
    this->MatrixSize_darts20[codeletID] = in_MatrixSize;
    this->RowWidthC_darts20[codeletID] = in_RowWidthC;
    this->RowWidthA_darts20[codeletID] = in_RowWidthA;
    this->RowWidthB_darts20[codeletID] = in_RowWidthB;
    this->Depth_darts20[codeletID] = in_Depth;
}
/*TP998: OMPTaskDirective*/
void TP998::_checkInCodelets999::fire(void)
{
/*Init the vars for this region*/

/*printing node 999: CallExpr*/
/*Finding the next codelet after the function call*/
#if 0
/*Signaling next codelet region: 999 nextRegion: 1265 */
myTP->controlTPParent->checkInCodelets1265.decDep();
#endif
/*Finding the next sync point before making the function call*/
#if 0
/*Find the sync point for a function call*/
/*syncNode: OMPTaskwaitDirective 1019*/
/*No syncpoint found or outside task's boundaries, looking for syncCodelet*/
#endif
    /*Make the function call*/
    invoke<TP_OptimizedStrassenMultiply_par>(myTP, myTP->numThreads, this->getID(),
        &(myTP->controlTPParent->checkInCodelets1265),
        (myTP->controlTPParent->task998Inputs->nextSyncCodelet), nullptr,
        (this->taskInputs->M2_darts998), (this->taskInputs->A_darts998),
        (this->taskInputs->B_darts998), (this->taskInputs->QuadrantSize_darts998),
        (this->taskInputs->QuadrantSize_darts998), (this->taskInputs->RowWidthA_darts998),
        (this->taskInputs->RowWidthB_darts998), (this->taskInputs->Depth_darts998) + 1);
}
void TP998::_checkInCodelets1265::fire(void)
{

    /*printing node 1265: NullStmt*/
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task998Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task998Inputs->nextSyncCodelet->decDep();
}
TP998::TP998(
    int in_numThreads, int in_mainCodeletID, TP20* in_TPParent, _task998Inputs* in_task998Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task998Inputs(in_task998Inputs)
    , TP999Ptr(nullptr)
    , TP999_alreadyLaunched(0)
    , checkInCodelets999(1, 1, this, this->mainCodeletID)
    , checkInCodelets1265(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets999.decDep();
}
TP998::~TP998() { delete (task998Inputs); }
/*TP1001: OMPTaskDirective*/
void TP1001::_checkInCodelets1002::fire(void)
{
/*Init the vars for this region*/

/*printing node 1002: CallExpr*/
/*Finding the next codelet after the function call*/
#if 0
/*Signaling next codelet region: 1002 nextRegion: 1266 */
myTP->controlTPParent->checkInCodelets1266.decDep();
#endif
/*Finding the next sync point before making the function call*/
#if 0
/*Find the sync point for a function call*/
/*syncNode: OMPTaskwaitDirective 1019*/
/*No syncpoint found or outside task's boundaries, looking for syncCodelet*/
#endif
    /*Make the function call*/
    invoke<TP_OptimizedStrassenMultiply_par>(myTP, myTP->numThreads, this->getID(),
        &(myTP->controlTPParent->checkInCodelets1266),
        (myTP->controlTPParent->task1001Inputs->nextSyncCodelet), nullptr,
        (this->taskInputs->M5_darts1001), (this->taskInputs->S1_darts1001),
        (this->taskInputs->S5_darts1001), (this->taskInputs->QuadrantSize_darts1001),
        (this->taskInputs->QuadrantSize_darts1001), (this->taskInputs->QuadrantSize_darts1001),
        (this->taskInputs->QuadrantSize_darts1001), (this->taskInputs->Depth_darts1001) + 1);
}
void TP1001::_checkInCodelets1266::fire(void)
{

    /*printing node 1266: NullStmt*/
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task1001Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task1001Inputs->nextSyncCodelet->decDep();
}
TP1001::TP1001(
    int in_numThreads, int in_mainCodeletID, TP20* in_TPParent, _task1001Inputs* in_task1001Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task1001Inputs(in_task1001Inputs)
    , TP1002Ptr(nullptr)
    , TP1002_alreadyLaunched(0)
    , checkInCodelets1002(1, 1, this, this->mainCodeletID)
    , checkInCodelets1266(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets1002.decDep();
}
TP1001::~TP1001() { delete (task1001Inputs); }
/*TP1004: OMPTaskDirective*/
void TP1004::_checkInCodelets1005::fire(void)
{
/*Init the vars for this region*/

/*printing node 1005: CallExpr*/
/*Finding the next codelet after the function call*/
#if 0
/*Signaling next codelet region: 1005 nextRegion: 1267 */
myTP->controlTPParent->checkInCodelets1267.decDep();
#endif
/*Finding the next sync point before making the function call*/
#if 0
/*Find the sync point for a function call*/
/*syncNode: OMPTaskwaitDirective 1019*/
/*No syncpoint found or outside task's boundaries, looking for syncCodelet*/
#endif
    /*Make the function call*/
    invoke<TP_OptimizedStrassenMultiply_par>(myTP, myTP->numThreads, this->getID(),
        &(myTP->controlTPParent->checkInCodelets1267),
        (myTP->controlTPParent->task1004Inputs->nextSyncCodelet), nullptr,
        (this->taskInputs->T1sMULT_darts1004), (this->taskInputs->S2_darts1004),
        (this->taskInputs->S6_darts1004), (this->taskInputs->QuadrantSize_darts1004),
        (this->taskInputs->QuadrantSize_darts1004), (this->taskInputs->QuadrantSize_darts1004),
        (this->taskInputs->QuadrantSize_darts1004), (this->taskInputs->Depth_darts1004) + 1);
}
void TP1004::_checkInCodelets1267::fire(void)
{

    /*printing node 1267: NullStmt*/
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task1004Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task1004Inputs->nextSyncCodelet->decDep();
}
TP1004::TP1004(
    int in_numThreads, int in_mainCodeletID, TP20* in_TPParent, _task1004Inputs* in_task1004Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task1004Inputs(in_task1004Inputs)
    , TP1005Ptr(nullptr)
    , TP1005_alreadyLaunched(0)
    , checkInCodelets1005(1, 1, this, this->mainCodeletID)
    , checkInCodelets1267(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets1005.decDep();
}
TP1004::~TP1004() { delete (task1004Inputs); }
/*TP1007: OMPTaskDirective*/
void TP1007::_checkInCodelets1008::fire(void)
{
/*Init the vars for this region*/

/*printing node 1008: CallExpr*/
/*Finding the next codelet after the function call*/
#if 0
/*Signaling next codelet region: 1008 nextRegion: 1268 */
myTP->controlTPParent->checkInCodelets1268.decDep();
#endif
/*Finding the next sync point before making the function call*/
#if 0
/*Find the sync point for a function call*/
/*syncNode: OMPTaskwaitDirective 1019*/
/*No syncpoint found or outside task's boundaries, looking for syncCodelet*/
#endif
    /*Make the function call*/
    invoke<TP_OptimizedStrassenMultiply_par>(myTP, myTP->numThreads, this->getID(),
        &(myTP->controlTPParent->checkInCodelets1268),
        (myTP->controlTPParent->task1007Inputs->nextSyncCodelet), nullptr,
        (this->taskInputs->C22_darts1007), (this->taskInputs->S3_darts1007),
        (this->taskInputs->S7_darts1007), (this->taskInputs->QuadrantSize_darts1007),
        (this->taskInputs->RowWidthC_darts1007), (this->taskInputs->QuadrantSize_darts1007),
        (this->taskInputs->QuadrantSize_darts1007), (this->taskInputs->Depth_darts1007) + 1);
}
void TP1007::_checkInCodelets1268::fire(void)
{

    /*printing node 1268: NullStmt*/
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task1007Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task1007Inputs->nextSyncCodelet->decDep();
}
TP1007::TP1007(
    int in_numThreads, int in_mainCodeletID, TP20* in_TPParent, _task1007Inputs* in_task1007Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task1007Inputs(in_task1007Inputs)
    , TP1008Ptr(nullptr)
    , TP1008_alreadyLaunched(0)
    , checkInCodelets1008(1, 1, this, this->mainCodeletID)
    , checkInCodelets1268(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets1008.decDep();
}
TP1007::~TP1007() { delete (task1007Inputs); }
/*TP1010: OMPTaskDirective*/
void TP1010::_checkInCodelets1011::fire(void)
{
/*Init the vars for this region*/

/*printing node 1011: CallExpr*/
/*Finding the next codelet after the function call*/
#if 0
/*Signaling next codelet region: 1011 nextRegion: 1269 */
myTP->controlTPParent->checkInCodelets1269.decDep();
#endif
/*Finding the next sync point before making the function call*/
#if 0
/*Find the sync point for a function call*/
/*syncNode: OMPTaskwaitDirective 1019*/
/*No syncpoint found or outside task's boundaries, looking for syncCodelet*/
#endif
    /*Make the function call*/
    invoke<TP_OptimizedStrassenMultiply_par>(myTP, myTP->numThreads, this->getID(),
        &(myTP->controlTPParent->checkInCodelets1269),
        (myTP->controlTPParent->task1010Inputs->nextSyncCodelet), nullptr,
        (this->taskInputs->C_darts1010), (this->taskInputs->A12_darts1010),
        (this->taskInputs->B21_darts1010), (this->taskInputs->QuadrantSize_darts1010),
        (this->taskInputs->RowWidthC_darts1010), (this->taskInputs->RowWidthA_darts1010),
        (this->taskInputs->RowWidthB_darts1010), (this->taskInputs->Depth_darts1010) + 1);
}
void TP1010::_checkInCodelets1269::fire(void)
{

    /*printing node 1269: NullStmt*/
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task1010Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task1010Inputs->nextSyncCodelet->decDep();
}
TP1010::TP1010(
    int in_numThreads, int in_mainCodeletID, TP20* in_TPParent, _task1010Inputs* in_task1010Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task1010Inputs(in_task1010Inputs)
    , TP1011Ptr(nullptr)
    , TP1011_alreadyLaunched(0)
    , checkInCodelets1011(1, 1, this, this->mainCodeletID)
    , checkInCodelets1269(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets1011.decDep();
}
TP1010::~TP1010() { delete (task1010Inputs); }
/*TP1013: OMPTaskDirective*/
void TP1013::_checkInCodelets1014::fire(void)
{
/*Init the vars for this region*/

/*printing node 1014: CallExpr*/
/*Finding the next codelet after the function call*/
#if 0
/*Signaling next codelet region: 1014 nextRegion: 1270 */
myTP->controlTPParent->checkInCodelets1270.decDep();
#endif
/*Finding the next sync point before making the function call*/
#if 0
/*Find the sync point for a function call*/
/*syncNode: OMPTaskwaitDirective 1019*/
/*No syncpoint found or outside task's boundaries, looking for syncCodelet*/
#endif
    /*Make the function call*/
    invoke<TP_OptimizedStrassenMultiply_par>(myTP, myTP->numThreads, this->getID(),
        &(myTP->controlTPParent->checkInCodelets1270),
        (myTP->controlTPParent->task1013Inputs->nextSyncCodelet), nullptr,
        (this->taskInputs->C12_darts1013), (this->taskInputs->S4_darts1013),
        (this->taskInputs->B22_darts1013), (this->taskInputs->QuadrantSize_darts1013),
        (this->taskInputs->RowWidthC_darts1013), (this->taskInputs->QuadrantSize_darts1013),
        (this->taskInputs->RowWidthB_darts1013), (this->taskInputs->Depth_darts1013) + 1);
}
void TP1013::_checkInCodelets1270::fire(void)
{

    /*printing node 1270: NullStmt*/
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task1013Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task1013Inputs->nextSyncCodelet->decDep();
}
TP1013::TP1013(
    int in_numThreads, int in_mainCodeletID, TP20* in_TPParent, _task1013Inputs* in_task1013Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task1013Inputs(in_task1013Inputs)
    , TP1014Ptr(nullptr)
    , TP1014_alreadyLaunched(0)
    , checkInCodelets1014(1, 1, this, this->mainCodeletID)
    , checkInCodelets1270(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets1014.decDep();
}
TP1013::~TP1013() { delete (task1013Inputs); }
/*TP1016: OMPTaskDirective*/
void TP1016::_checkInCodelets1017::fire(void)
{
/*Init the vars for this region*/

/*printing node 1017: CallExpr*/
/*Finding the next codelet after the function call*/
#if 0
/*Signaling next codelet region: 1017 nextRegion: 1271 */
myTP->controlTPParent->checkInCodelets1271.decDep();
#endif
/*Finding the next sync point before making the function call*/
#if 0
/*Find the sync point for a function call*/
/*syncNode: OMPTaskwaitDirective 1019*/
/*No syncpoint found or outside task's boundaries, looking for syncCodelet*/
#endif
    /*Make the function call*/
    invoke<TP_OptimizedStrassenMultiply_par>(myTP, myTP->numThreads, this->getID(),
        &(myTP->controlTPParent->checkInCodelets1271),
        (myTP->controlTPParent->task1016Inputs->nextSyncCodelet), nullptr,
        (this->taskInputs->C21_darts1016), (this->taskInputs->A22_darts1016),
        (this->taskInputs->S8_darts1016), (this->taskInputs->QuadrantSize_darts1016),
        (this->taskInputs->RowWidthC_darts1016), (this->taskInputs->RowWidthA_darts1016),
        (this->taskInputs->QuadrantSize_darts1016), (this->taskInputs->Depth_darts1016) + 1);
}
void TP1016::_checkInCodelets1271::fire(void)
{

    /*printing node 1271: NullStmt*/
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task1016Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task1016Inputs->nextSyncCodelet->decDep();
}
TP1016::TP1016(
    int in_numThreads, int in_mainCodeletID, TP20* in_TPParent, _task1016Inputs* in_task1016Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task1016Inputs(in_task1016Inputs)
    , TP1017Ptr(nullptr)
    , TP1017_alreadyLaunched(0)
    , checkInCodelets1017(1, 1, this, this->mainCodeletID)
    , checkInCodelets1271(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets1017.decDep();
}
TP1016::~TP1016() { delete (task1016Inputs); }
/*TP1244: OMPParallelDirective*/
void TP1244::_barrierCodelets1244::fire(void)
{
    TP1244* myTP = static_cast<TP1244*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
void TP1244::_checkInCodelets1245::fire(void)
{
    /*Select the thread executing OMPSingleDirective 1245*/
    if (!__sync_val_compare_and_swap(&(myTP->TP1245_alreadyLaunched), 0, 1)) {
        invoke<TP1245>(myTP, 1, this->getID(), myTP, &(*(this->inputsTPParent->A_darts1244)),
            &(*(this->inputsTPParent->B_darts1244)), &(*(this->inputsTPParent->C_darts1244)),
            &(*(this->inputsTPParent->n_darts1244)));
    } else {
        myTP->barrierCodelets1245[0].decDep();
    }
}
void TP1244::_barrierCodelets1245::fire(void)
{
    TP1244* myTP = static_cast<TP1244*>(myTP_);
    myTP->TPParent->barrierCodelets1244[0].setDep(0);
    myTP->add(&(myTP->TPParent->barrierCodelets1244[0]));
}
TP1244::TP1244(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet, REAL** in_A,
    REAL** in_B, REAL** in_C, int* in_n)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , A_darts1244(in_A) /*OMP_SHARED - INPUT*/
    , B_darts1244(in_B) /*OMP_SHARED - INPUT*/
    , C_darts1244(in_C) /*OMP_SHARED - INPUT*/
    , n_darts1244(in_n) /*OMP_SHARED - INPUT*/
    , TP1245Ptr(nullptr)
    , TP1245_alreadyLaunched(0)
    , task1246Inputs(new _task1246Inputs*[this->numThreads])
    , barrierCodelets1244(new _barrierCodelets1244[1])
    , checkInCodelets1245(new _checkInCodelets1245[this->numThreads])
    , barrierCodelets1245(new _barrierCodelets1245[1])
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets1244[0] = _barrierCodelets1244(ompNumThreads, ompNumThreads, this, 0);
    barrierCodelets1245[0] = _barrierCodelets1245(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets1245* checkInCodelets1245Ptr = (this->checkInCodelets1245);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets1245Ptr) = _checkInCodelets1245(1, 1, this, codeletCounter);
        (*checkInCodelets1245Ptr).decDep();
        checkInCodelets1245Ptr++;
    }
}
TP1244::~TP1244()
{
    delete[] task1246Inputs;
    delete[] barrierCodelets1244;
    delete[] barrierCodelets1245;
    delete[] checkInCodelets1245;
}
/*TP1245: OMPSingleDirective*/
void TP1245::_checkInCodelets1246::fire(void)
{

    /*printing node 1246: OMPTaskDirective*/
    /*syncNode: OMPSingleDirective 1245*/
    /*Find the TP to which the nextRegion belongs*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->TPParent->barrierCodelets1245[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 1246*/
    _task1246Inputs* task1246Inputs = new _task1246Inputs(&(*(this->inputsTPParent->A_darts1245)),
        &(*(this->inputsTPParent->B_darts1245)), &(*(this->inputsTPParent->C_darts1245)),
        &(*(this->inputsTPParent->n_darts1245)), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task1246Inputs[0] = task1246Inputs;
    invoke<TP1246>(myTP, 1, this->getID(), myTP, task1246Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets1245[0].decDep();
}
TP1245::TP1245(int in_numThreads, int in_mainCodeletID, TP1244* in_TPParent, REAL** in_A,
    REAL** in_B, REAL** in_C, int* in_n)
    : ompOMPSingleDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , A_darts1245(in_A) /*OMP_SHARED - INPUT*/
    , B_darts1245(in_B) /*OMP_SHARED - INPUT*/
    , C_darts1245(in_C) /*OMP_SHARED - INPUT*/
    , n_darts1245(in_n) /*OMP_SHARED - INPUT*/
    , task1246Inputs(new _task1246Inputs*[this->numThreads])
    , checkInCodelets1246(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets1246.decDep();
}
TP1245::~TP1245() { delete[] task1246Inputs; }
/*TP1246: OMPTaskDirective*/
void TP1246::_checkInCodelets1247::fire(void)
{
/*Init the vars for this region*/

/*printing node 1247: CallExpr*/
/*Finding the next codelet after the function call*/
#if 0
/*Signaling next codelet region: 1247 nextRegion: 1272 */
myTP->controlTPParent->checkInCodelets1272.decDep();
#endif
/*Finding the next sync point before making the function call*/
#if 0
/*Find the sync point for a function call*/
/*syncNode: OMPSingleDirective 1245*/
/*No syncpoint found or outside task's boundaries, looking for syncCodelet*/
#endif
    /*Make the function call*/
    invoke<TP_OptimizedStrassenMultiply_par>(myTP, myTP->numThreads, this->getID(),
        &(myTP->controlTPParent->checkInCodelets1272),
        (myTP->controlTPParent->task1246Inputs->nextSyncCodelet), nullptr,
        (*(this->taskInputs->C_darts1246)), (*(this->taskInputs->A_darts1246)),
        (*(this->taskInputs->B_darts1246)), (*(this->taskInputs->n_darts1246)),
        (*(this->taskInputs->n_darts1246)), (*(this->taskInputs->n_darts1246)),
        (*(this->taskInputs->n_darts1246)), 1);
}
void TP1246::_checkInCodelets1272::fire(void)
{

    /*printing node 1272: NullStmt*/
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task1246Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task1246Inputs->nextSyncCodelet->decDep();
}
TP1246::TP1246(int in_numThreads, int in_mainCodeletID, TP1245* in_TPParent,
    _task1246Inputs* in_task1246Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task1246Inputs(in_task1246Inputs)
    , TP1247Ptr(nullptr)
    , TP1247_alreadyLaunched(0)
    , checkInCodelets1247(1, 1, this, this->mainCodeletID)
    , checkInCodelets1272(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets1247.decDep();
}
TP1246::~TP1246() { delete (task1246Inputs); }
