#include "parallel.output.darts.h"
using namespace darts;
using namespace std;
static int myRank = 0;
static int nRanks = 1;
static int* rUsed;
static int reqCount = 0;
/*Function: getNRanks, ID: 38*/
int getNRanks()
{
    /*getNRanks:38*/
    /*CompoundStmt:3010*/
    return nRanks;
}
/*Function: getMyRank, ID: 39*/
int getMyRank()
{
    /*getMyRank:39*/
    /*CompoundStmt:3012*/
    return myRank;
}
/*Function: printRank, ID: 40*/
int printRank()
{
    /*printRank:40*/
    /*CompoundStmt:3014*/
    if (myRank == 0)
        return 1;
    return 0;
}
/*Function: timestampBarrier, ID: 41*/
void timestampBarrier(const char* msg)
{
    /*timestampBarrier:41*/
    /*CompoundStmt:3020*/
    barrierParallel();
    if (!printRank())
        return;
    time_t t = time(((void*)0));
    char* timeString = ctime(&t);
    timeString[24] = '\x00';
    fprintf(stdout, "%s: %s\n", timeString, msg);
    fflush(stdout);
}
/*Function: initParallel, ID: 42*/
void initParallel(int* argc, char*** argv)
{
    /*initParallel:42*/
    /*CompoundStmt:3037*/
}
/*Function: destroyParallel, ID: 43*/
void destroyParallel()
{
    /*destroyParallel:43*/
    /*CompoundStmt:3038*/
}
/*Function: barrierParallel, ID: 44*/
void barrierParallel()
{
    /*barrierParallel:44*/
    /*CompoundStmt:3039*/
}
/*Function: sendReceiveParallel, ID: 45*/
int sendReceiveParallel(
    void* sendBuf, int sendLen, int dest, void* recvBuf, int recvLen, int source)
{
    /*sendReceiveParallel:45*/
    /*CompoundStmt:3040*/
    memcpy(recvBuf, sendBuf, sendLen);
    return sendLen;
}
/*Function: isendParallel, ID: 46*/
int isendParallel(void* sendBuf, int sendLen, int dest)
{
    /*isendParallel:46*/
    /*CompoundStmt:3043*/
    return 0;
}
/*Function: sendParallel, ID: 47*/
int sendParallel(void* sendBuf, int sendLen, int dest)
{
    /*sendParallel:47*/
    /*CompoundStmt:3045*/
    return sendLen;
}
/*Function: recvAnyParallel, ID: 48*/
int recvAnyParallel(void* recvBuf, int recvLen)
{
    /*recvAnyParallel:48*/
    /*CompoundStmt:3047*/
    return recvLen;
}
/*Function: irecvAnyParallel, ID: 49*/
int irecvAnyParallel(void* recvBuf, int recvLen)
{
    /*irecvAnyParallel:49*/
    /*CompoundStmt:3049*/
    return 0;
}
/*Function: waitIrecv, ID: 50*/
int waitIrecv(int rind)
{
    /*waitIrecv:50*/
    /*CompoundStmt:3051*/
    return 0;
}
/*Function: testIrecv, ID: 51*/
int testIrecv(int rind)
{
    /*testIrecv:51*/
    /*CompoundStmt:3053*/
    return -1;
}
/*Function: waitIsend, ID: 52*/
int waitIsend(int rind)
{
    /*waitIsend:52*/
    /*CompoundStmt:3056*/
    return 0;
}
/*Function: testIsend, ID: 53*/
int testIsend(int rind)
{
    /*testIsend:53*/
    /*CompoundStmt:3058*/
    return -1;
}
/*Function: recvParallel, ID: 54*/
int recvParallel(void* recvBuf, int recvLen, int source)
{
    /*recvParallel:54*/
    /*CompoundStmt:3061*/
    return recvLen;
}
/*Function: irecvParallel, ID: 55*/
int irecvParallel(void* recvBuf, int recvLen, int source)
{
    /*irecvParallel:55*/
    /*CompoundStmt:3063*/
    return 0;
}
/*Function: addIntParallel, ID: 56*/
void addIntParallel(int* sendBuf, int* recvBuf, int count)
{
    /*addIntParallel:56*/
    /*CompoundStmt:3065*/
    for (int ii = 0; ii < count; ++ii)
        recvBuf[ii] = sendBuf[ii];
}
/*Function: addRealParallel, ID: 57*/
void addRealParallel(real_t* sendBuf, real_t* recvBuf, int count)
{
    /*addRealParallel:57*/
    /*CompoundStmt:3073*/
    for (int ii = 0; ii < count; ++ii)
        recvBuf[ii] = sendBuf[ii];
}
/*Function: addDoubleParallel, ID: 58*/
void addDoubleParallel(double* sendBuf, double* recvBuf, int count)
{
    /*addDoubleParallel:58*/
    /*CompoundStmt:3081*/
    for (int ii = 0; ii < count; ++ii)
        recvBuf[ii] = sendBuf[ii];
}
/*Function: maxIntParallel, ID: 59*/
void maxIntParallel(int* sendBuf, int* recvBuf, int count)
{
    /*maxIntParallel:59*/
    /*CompoundStmt:3089*/
    for (int ii = 0; ii < count; ++ii)
        recvBuf[ii] = sendBuf[ii];
}
/*Function: maxRealParallel, ID: 60*/
void maxRealParallel(real_t* sendBuf, real_t* recvBuf, int count)
{
    /*maxRealParallel:60*/
    /*CompoundStmt:3097*/
    for (int ii = 0; ii < count; ++ii)
        recvBuf[ii] = sendBuf[ii];
}
/*Function: minRealParallel, ID: 61*/
void minRealParallel(real_t* sendBuf, real_t* recvBuf, int count)
{
    /*minRealParallel:61*/
    /*CompoundStmt:3105*/
    for (int ii = 0; ii < count; ++ii)
        recvBuf[ii] = sendBuf[ii];
}
/*Function: minRankDoubleParallel, ID: 62*/
void minRankDoubleParallel(RankReduceData* sendBuf, RankReduceData* recvBuf, int count)
{
    /*minRankDoubleParallel:62*/
    /*CompoundStmt:3113*/
    for (int ii = 0; ii < count; ++ii) {
        recvBuf[ii].val = sendBuf[ii].val;
        recvBuf[ii].rank = sendBuf[ii].rank;
    }
}
/*Function: maxRankDoubleParallel, ID: 63*/
void maxRankDoubleParallel(RankReduceData* sendBuf, RankReduceData* recvBuf, int count)
{
    /*maxRankDoubleParallel:63*/
    /*CompoundStmt:3129*/
    for (int ii = 0; ii < count; ++ii) {
        recvBuf[ii].val = sendBuf[ii].val;
        recvBuf[ii].rank = sendBuf[ii].rank;
    }
}
/*Function: minRealReduce, ID: 64*/
void minRealReduce(real_t* value)
{
    /*minRealReduce:64*/
    /*CompoundStmt:3145*/
    real_t sLocal[1], sGlobal[1];
    sLocal[0] = *value;
    minRealParallel(sLocal, sGlobal, 1);
    *value = sGlobal[0];
}
/*Function: maxRealReduce, ID: 65*/
void maxRealReduce(real_t* value)
{
    /*maxRealReduce:65*/
    /*CompoundStmt:3154*/
    real_t sLocal[1], sGlobal[1];
    sLocal[0] = *value;
    maxRealParallel(sLocal, sGlobal, 1);
    *value = sGlobal[0];
}
/*Function: maxIntReduce2, ID: 66*/
void maxIntReduce2(int* value0, int* value1)
{
    /*maxIntReduce2:66*/
    /*CompoundStmt:3163*/
    int sLocal[2], sGlobal[2];
    sLocal[0] = *value0;
    sLocal[1] = *value1;
    maxIntParallel(sLocal, sGlobal, 2);
    *value0 = sGlobal[0];
    *value1 = sGlobal[1];
}
/*Function: addIntReduce2, ID: 67*/
void addIntReduce2(int* value0, int* value1)
{
    /*addIntReduce2:67*/
    /*CompoundStmt:3178*/
    int sLocal[2], sGlobal[2];
    sLocal[0] = *value0;
    sLocal[1] = *value1;
    addIntParallel(sLocal, sGlobal, 2);
    *value0 = sGlobal[0];
    *value1 = sGlobal[1];
}
/*Function: addRealReduce2, ID: 68*/
void addRealReduce2(real_t* value0, real_t* value1)
{
    /*addRealReduce2:68*/
    /*CompoundStmt:3193*/
    real_t sLocal[2], sGlobal[2];
    sLocal[0] = *value0;
    sLocal[1] = *value1;
    addRealParallel(sLocal, sGlobal, 2);
    *value0 = sGlobal[0];
    *value1 = sGlobal[1];
}
/*Function: bcastParallel, ID: 69*/
void bcastParallel(void* buf, int count, int root)
{
    /*bcastParallel:69*/
    /*CompoundStmt:3208*/
}
/*Function: builtWithMpi, ID: 70*/
int builtWithMpi()
{
    /*builtWithMpi:70*/
    /*CompoundStmt:3209*/
    return 0;
}
