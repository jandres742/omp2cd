#include "parallel.output.darts.h"
using namespace darts;
using namespace std;
static int myRank = 0;
static int nRanks = 1;
/*Function: getNRanks, ID: 136*/
int getNRanks()
{
    /*getNRanks:136*/
    /*CompoundStmt:6289*/
    return nRanks;
}
/*Function: getMyRank, ID: 137*/
int getMyRank()
{
    /*getMyRank:137*/
    /*CompoundStmt:6291*/
    return myRank;
}
/*Function: printRank, ID: 138*/
int printRank()
{
    /*printRank:138*/
    /*CompoundStmt:6293*/
    if (myRank == 0)
        return 1;
    return 0;
}
/*Function: timestampBarrier, ID: 139*/
void timestampBarrier(const char* msg)
{
    /*timestampBarrier:139*/
    /*CompoundStmt:6299*/
    barrierParallel();
    if (!printRank())
        return;
    time_t t = time(((void*)0));
    char* timeString = ctime(&t);
    timeString[24] = '\x00';
    fprintf(stdout, "%s: %s\n", timeString, msg);
    fflush(stdout);
}
/*Function: initParallel, ID: 140*/
void initParallel(int* argc, char*** argv)
{
    /*initParallel:140*/
    /*CompoundStmt:6316*/
}
/*Function: destroyParallel, ID: 141*/
void destroyParallel()
{
    /*destroyParallel:141*/
    /*CompoundStmt:6317*/
}
/*Function: barrierParallel, ID: 142*/
void barrierParallel()
{
    /*barrierParallel:142*/
    /*CompoundStmt:6318*/
}
/*Function: sendReceiveParallel, ID: 143*/
int sendReceiveParallel(
    void* sendBuf, int sendLen, int dest, void* recvBuf, int recvLen, int source)
{
    /*sendReceiveParallel:143*/
    /*CompoundStmt:6319*/
    memcpy(recvBuf, sendBuf, sendLen);
    return sendLen;
}
/*Function: addIntParallel, ID: 144*/
void addIntParallel(int* sendBuf, int* recvBuf, int count)
{
    /*addIntParallel:144*/
    /*CompoundStmt:6322*/
    for (int ii = 0; ii < count; ++ii)
        recvBuf[ii] = sendBuf[ii];
}
/*Function: addRealParallel, ID: 145*/
void addRealParallel(real_t* sendBuf, real_t* recvBuf, int count)
{
    /*addRealParallel:145*/
    /*CompoundStmt:6330*/
    for (int ii = 0; ii < count; ++ii)
        recvBuf[ii] = sendBuf[ii];
}
/*Function: addDoubleParallel, ID: 146*/
void addDoubleParallel(double* sendBuf, double* recvBuf, int count)
{
    /*addDoubleParallel:146*/
    /*CompoundStmt:6338*/
    for (int ii = 0; ii < count; ++ii)
        recvBuf[ii] = sendBuf[ii];
}
/*Function: maxIntParallel, ID: 147*/
void maxIntParallel(int* sendBuf, int* recvBuf, int count)
{
    /*maxIntParallel:147*/
    /*CompoundStmt:6346*/
    for (int ii = 0; ii < count; ++ii)
        recvBuf[ii] = sendBuf[ii];
}
/*Function: minRankDoubleParallel, ID: 148*/
void minRankDoubleParallel(RankReduceData* sendBuf, RankReduceData* recvBuf, int count)
{
    /*minRankDoubleParallel:148*/
    /*CompoundStmt:6354*/
    for (int ii = 0; ii < count; ++ii) {
        recvBuf[ii].val = sendBuf[ii].val;
        recvBuf[ii].rank = sendBuf[ii].rank;
    }
}
/*Function: maxRankDoubleParallel, ID: 149*/
void maxRankDoubleParallel(RankReduceData* sendBuf, RankReduceData* recvBuf, int count)
{
    /*maxRankDoubleParallel:149*/
    /*CompoundStmt:6370*/
    for (int ii = 0; ii < count; ++ii) {
        recvBuf[ii].val = sendBuf[ii].val;
        recvBuf[ii].rank = sendBuf[ii].rank;
    }
}
/*Function: bcastParallel, ID: 150*/
void bcastParallel(void* buf, int count, int root)
{
    /*bcastParallel:150*/
    /*CompoundStmt:6386*/
}
/*Function: builtWithMpi, ID: 151*/
int builtWithMpi()
{
    /*builtWithMpi:151*/
    /*CompoundStmt:6387*/
    return 0;
}
