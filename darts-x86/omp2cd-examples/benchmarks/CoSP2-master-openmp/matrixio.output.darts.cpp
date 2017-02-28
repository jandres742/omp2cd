#include "matrixio.output.darts.h"
using namespace darts;
using namespace std;
/*Function: writeSparsePattern, ID: 32*/
void writeSparsePattern(char* fname, struct SparseMatrixSt* spmatrix, real_t hthresh)
{
    /*writeSparsePattern:32*/
    /*CompoundStmt:2504*/
    int spmatrix_hsize = spmatrix->hsize;
    char* hrow = (char*)malloc(spmatrix->hsize * sizeof(char));
    FILE* sFile;
    sFile = fopen(fname, "w");
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP2516>(ompNumThreads * DARTS_CODELETS_MULT, 0,
            RuntimeFinalCodelet, 0, spmatrix_hsize, (char**)&((hrow)), (real_t*)&((hthresh)),
            (FILE**)&((sFile)), (struct SparseMatrixSt**)&((spmatrix)), (int*)&((spmatrix_hsize))));
    }
    free(hrow);
    fclose(sFile);
}
/*Function: readMTX, ID: 33*/
void readMTX(char* fname, struct SparseMatrixSt* hmatrix)
{
    /*readMTX:33*/
    /*CompoundStmt:2582*/
    int hvalue, msum, irow, icol, ind;
    char line[100], header1[20], header2[20], header3[20], header4[20], header5[20];
    double value;
    FILE* hFile;
    hFile = fopen(fname, "r");
    fscanf(hFile, "%s %s %s %s %s", header1, header2, header3, header4, header5);
    fscanf(hFile, "%d %d %d", &hvalue, &hvalue, &msum);
    for (int i = 0; i < msum; i++) {
        fscanf(hFile, "%d %d %lg", &irow, &icol, &value);
        irow--;
        icol--;
        ind = hmatrix->iia[irow];
        hmatrix->jja[irow][ind] = icol;
        hmatrix->val[irow][ind] = value;
        hmatrix->iia[irow]++;
    }
    fclose(hFile);
}
/*Function: writeMTX, ID: 34*/
void writeMTX(char* fname, struct SparseMatrixSt* spmatrix)
{
    /*writeMTX:34*/
    /*CompoundStmt:2620*/
    FILE* mFile;
    int msum;
    mFile = fopen(fname, "w");
    fprintf(mFile, "=====MatrixMarket matrix coordinate real general\n");
    msum = 0;
    for (int i = 0; i < spmatrix->hsize; i++) {
        msum += spmatrix->iia[i];
    }
    fprintf(mFile, "%d %d %d\n", spmatrix->hsize, spmatrix->hsize, msum);
    for (int i = 0; i < spmatrix->hsize; i++) {
        for (int j = 0; j < spmatrix->iia[i]; j++) {
            fprintf(mFile, "%d %d %lg\n", i + 1, spmatrix->jja[i][j] + 1, spmatrix->val[i][j]);
        }
    }
    fclose(mFile);
}
/*TP2516: OMPParallelForDirective*/
void TP2516::_barrierCodelets2516::fire(void)
{
    TP2516* myTP = static_cast<TP2516*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
bool TP2516::requestNewRangeIterations2516(int* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Static */
    /*Chunk = 0*/
    bool isThereNewIteration = false;
    {
        /*Static Scheduling*/
        int tempStartRange = rangePerCodelet2516 * codeletID;
        int tempEndRange = rangePerCodelet2516 * (codeletID + 1);
        if (remainderRange2516 != 0) {
            if (codeletID < (uint32_t)remainderRange2516) {
                tempStartRange += codeletID;
                tempEndRange += (codeletID + 1);
            } else {
                tempStartRange += remainderRange2516;
                tempEndRange += remainderRange2516;
            }
        }
        tempStartRange = tempStartRange * 1 + minIteration2516;
        tempEndRange = tempEndRange * 1 + minIteration2516;
        if (tempStartRange != tempEndRange) {
            isThereNewIteration = true;
        }
        if (initIteration2516 < lastIteration2516) {
            (this->inputsTPParent->i_darts2516[codeletID]) = min(tempStartRange, tempEndRange);
            *endRange = max(tempStartRange, tempEndRange);
        } else {
            (this->inputsTPParent->i_darts2516[codeletID]) = max(tempStartRange, tempEndRange);
            *endRange = min(tempStartRange, tempEndRange);
        }
        if (codeletID == this->numThreads - 1) {
            *endRange = lastIteration2516;
        }
    }
    return isThereNewIteration;
}
void TP2516::_checkInCodelets2517::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 2517: ForStmt*/
    /*var: hrow*/
    /*var: hthresh*/
    /*var: sFile*/
    /*var: spmatrix*/
    /*var: spmatrix_hsize*/
    char** hrow = (this->inputsTPParent->hrow_darts2516);
    (void)hrow /*OMP_SHARED*/;
    real_t* hthresh = (this->inputsTPParent->hthresh_darts2516);
    (void)hthresh /*OMP_SHARED*/;
    FILE** sFile = (this->inputsTPParent->sFile_darts2516);
    (void)sFile /*OMP_SHARED*/;
    struct SparseMatrixSt** spmatrix = (this->inputsTPParent->spmatrix_darts2516);
    (void)spmatrix /*OMP_SHARED*/;
    int* i = &(this->inputsTPParent->i_darts2516[this->getLocalID()]);
    (void)i /*PRIVATE*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations2516(
        (int*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        /*Find and signal the next codelet*/
        myTP->controlTPParent->TPParent->barrierCodelets2516[0].decDep();
        return;
    }
    for (int i_darts_counter_temp2516 = (*i); i_darts_counter_temp2516 < endRange
         && i_darts_counter_temp2516 < this->inputsTPParent->lastIteration2516;
         i_darts_counter_temp2516++) {
        {
            {
                /*Loop's init*/
                int j = 0;
                for (; j < (*(spmatrix))->hsize; j++) {
                    (*(hrow))[j] = '.';
                }
            }
            {
                /*Loop's init*/
                int j = 0;
                for (; j < (*(spmatrix))->iia[(i_darts_counter_temp2516)]; j++) {
                    if (fabs((*(spmatrix))->val[(i_darts_counter_temp2516)][j]) > (*(hthresh))) {
                        (*(hrow))[(*(spmatrix))->jja[(i_darts_counter_temp2516)][j]] = '*';
                    }
                }
            }
            {
                /*Loop's init*/
                int j = 0;
                for (; j < (*(spmatrix))->hsize; j++) {
                    fprintf((*(sFile)), "%c", (*(hrow))[j]);
                }
            }
            fprintf((*(sFile)), "\n");
        }
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets2516[0].decDep();
}
TP2516::TP2516(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet,
    int in_initIteration, int in_lastIteration, char** in_hrow, real_t* in_hthresh, FILE** in_sFile,
    struct SparseMatrixSt** in_spmatrix, int* in_spmatrix_hsize)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , hrow_darts2516(in_hrow) /*OMP_SHARED - INPUT*/
    , hthresh_darts2516(in_hthresh) /*OMP_SHARED - INPUT*/
    , sFile_darts2516(in_sFile) /*OMP_SHARED - INPUT*/
    , spmatrix_darts2516(in_spmatrix) /*OMP_SHARED - INPUT*/
    , spmatrix_hsize_darts2516(in_spmatrix_hsize) /*OMP_SHARED - INPUT*/
    , i_darts2516(new int[this->numThreads]) /*VARIABLE*/
    , initIteration2516(in_initIteration)
    , lastIteration2516(in_lastIteration)
    , barrierCodelets2516(new _barrierCodelets2516[1])
    , checkInCodelets2517(new _checkInCodelets2517[this->numThreads])
{
    /*Initialize the loop parameters*/
    range2516 = abs(lastIteration2516 - initIteration2516) / 1;
    rangePerCodelet2516 = range2516 / numThreads;
    minIteration2516 = min<int>(lastIteration2516, initIteration2516);
    remainderRange2516 = range2516 % numThreads;
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets2516[0] = _barrierCodelets2516(ompNumThreads, ompNumThreads, this, 0);
    _checkInCodelets2517* checkInCodelets2517Ptr = (this->checkInCodelets2517);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets2517Ptr) = _checkInCodelets2517(1, 1, this, codeletCounter);
        (*checkInCodelets2517Ptr).decDep();
        checkInCodelets2517Ptr++;
    }
}
TP2516::~TP2516()
{
    delete[] i_darts2516;
    delete[] barrierCodelets2516;
    delete[] checkInCodelets2517;
}
