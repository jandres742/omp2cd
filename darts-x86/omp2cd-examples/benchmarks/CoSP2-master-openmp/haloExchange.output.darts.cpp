#include "haloExchange.output.darts.h"
using namespace darts;
using namespace std;
/*Function: initHaloExchange, ID: 20*/
HaloExchange* initHaloExchange(struct DomainSt* domain)
{
    /*initHaloExchange:20*/
    /*CompoundStmt:1334*/
    HaloExchange* hh = (HaloExchange*)malloc(sizeof(HaloExchange));
    hh->maxHalo = domain->totalProcs;
    hh->haloCount = 0;
    hh->haloProc = (int*)malloc(hh->maxHalo * sizeof(int));
    hh->bufferSize
        = domain->localRowExtent * domain->totalCols * (2 * sizeof(int) + sizeof(real_t));
    if (printRank() && debug == 1)
        printf("bufferSize = %d\n", hh->bufferSize);
    hh->sendBuf = (char*)malloc(hh->bufferSize * sizeof(char));
    hh->recvBuf = (char**)malloc(getNRanks() * sizeof(char*));
    for (int i = 0; i < getNRanks(); i++) {
        hh->recvBuf[i] = (char*)malloc(hh->bufferSize * sizeof(char));
    }
    return hh;
}
/*Function: destroyHaloExchange, ID: 21*/
void destroyHaloExchange(struct HaloExchangeSt* haloExchange)
{
    /*destroyHaloExchange:21*/
    /*CompoundStmt:1395*/
    free(haloExchange->haloProc);
    free(haloExchange->sendBuf);
    for (int i = 0; i < getNRanks(); i++) {
        free(haloExchange->recvBuf[i]);
    }
    free(haloExchange->recvBuf);
    free(haloExchange);
}
/*Function: exchangeSetup, ID: 22*/
void exchangeSetup(
    struct HaloExchangeSt* haloExchange, struct SparseMatrixSt* spmatrix, struct DomainSt* domain)
{
    /*exchangeSetup:22*/
    /*CompoundStmt:1412*/
    updateData(haloExchange, spmatrix, domain);
    if (haloExchange->haloCount > 0) {
        haloExchange->rlist = (int*)malloc(haloExchange->haloCount * sizeof(int));
        for (int i = 0; i < haloExchange->haloCount; i++) {
            haloExchange->rlist[i]
                = irecvAnyParallel(haloExchange->recvBuf[i], haloExchange->bufferSize);
        }
    }
}
/*Function: exchangeData, ID: 23*/
void exchangeData(
    struct HaloExchangeSt* haloExchange, struct SparseMatrixSt* spmatrix, struct DomainSt* domain)
{
    /*exchangeData:23*/
    /*CompoundStmt:1437*/
    if (haloExchange->haloCount > 0) {
        int nSendLen = loadBuffer(haloExchange->sendBuf, spmatrix, domain);
        for (int i = 0; i < haloExchange->haloCount; i++) {
            int nSend = sendParallel(haloExchange->sendBuf, nSendLen, haloExchange->haloProc[i]);
            do {
                addToCounter(sendCounter, nSendLen);
            } while (0);
        }
        for (int i = 0; i < haloExchange->haloCount; i++) {
            int nRecv = waitIrecv(haloExchange->rlist[i]);
            unloadBuffer(haloExchange->recvBuf[i], nRecv, spmatrix, domain);
            do {
                addToCounter(recvCounter, nRecv);
            } while (0);
        }
    }
}
/*Function: updateData, ID: 24*/
void updateData(
    struct HaloExchangeSt* haloExchange, struct SparseMatrixSt* spmatrix, struct DomainSt* domain)
{
    /*updateData:24*/
    /*CompoundStmt:1476*/
    haloExchange->haloCount = 0;
    for (int i = domain->localRowMin; i < domain->localRowMax; i++) {
        for (int j = 0; j < spmatrix->iia[i]; j++) {
            int rnum = spmatrix->jja[i][j];
            if (rnum < domain->localRowMin || rnum >= domain->localRowMax) {
                int rowProc = processorNum(domain, rnum);
                addHaloProc(haloExchange, rowProc);
            }
        }
    }
}
/*Function: gatherData, ID: 25*/
void gatherData(
    struct HaloExchangeSt* haloExchange, struct SparseMatrixSt* spmatrix, struct DomainSt* domain)
{
    /*gatherData:25*/
    /*CompoundStmt:1503*/
    int myRank = getMyRank();
    if (myRank == 0) {
        free(haloExchange->rlist);
        haloExchange->rlist = (int*)malloc((getNRanks() - haloExchange->haloCount) * sizeof(int));
        int ir = 0;
        for (int i = 1; i < getNRanks(); i++) {
            if (!isHaloProc(haloExchange, i)) {
                haloExchange->rlist[ir]
                    = irecvAnyParallel(haloExchange->recvBuf[ir], haloExchange->bufferSize);
                ir++;
            }
        }
        for (int i = 0; i < ir; i++) {
            int nRecv = waitIrecv(haloExchange->rlist[i]);
            unloadBuffer(haloExchange->recvBuf[i], nRecv, spmatrix, domain);
            do {
                addToCounter(recvCounter, nRecv);
            } while (0);
        }
    } else {
        if (!isHaloProc(haloExchange, 0)) {
            int nSendLen = loadBuffer(haloExchange->sendBuf, spmatrix, domain);
            int nSend = sendParallel(haloExchange->sendBuf, nSendLen, 0);
            do {
                addToCounter(sendCounter, nSendLen);
            } while (0);
        }
    }
}
/*Function: allGatherData, ID: 26*/
void allGatherData(
    struct HaloExchangeSt* haloExchange, struct SparseMatrixSt* spmatrix, struct DomainSt* domain)
{
    /*allGatherData:26*/
    /*CompoundStmt:1568*/
    int myRank = getMyRank();
    free(haloExchange->rlist);
    haloExchange->rlist = (int*)malloc((getNRanks() - haloExchange->haloCount) * sizeof(int));
    int ir = 0;
    for (int i = 0; i < getNRanks(); i++) {
        if (i != myRank && !isHaloProc(haloExchange, i)) {
            haloExchange->rlist[ir]
                = irecvAnyParallel(haloExchange->recvBuf[ir], haloExchange->bufferSize);
            ir++;
        }
    }
    int nSendLen = loadBuffer(haloExchange->sendBuf, spmatrix, domain);
    for (int i = 0; i < getNRanks(); i++) {
        if (i != myRank && !isHaloProc(haloExchange, i)) {
            int nSend = sendParallel(haloExchange->sendBuf, nSendLen, i);
            do {
                addToCounter(sendCounter, nSendLen);
            } while (0);
        }
    }
    for (int i = 0; i < ir; i++) {
        int nRecv = waitIrecv(haloExchange->rlist[i]);
        unloadBuffer(haloExchange->recvBuf[i], nRecv, spmatrix, domain);
        do {
            addToCounter(recvCounter, nRecv);
        } while (0);
    }
}
/*Function: isHaloProc, ID: 27*/
int isHaloProc(struct HaloExchangeSt* haloExchange, int rproc)
{
    /*isHaloProc:27*/
    /*CompoundStmt:1635*/
    for (int i = 0; i < haloExchange->haloCount; i++) {
        if (haloExchange->haloProc[i] == rproc)
            return 1;
    }
    return 0;
}
/*Function: addHaloProc, ID: 28*/
void addHaloProc(struct HaloExchangeSt* haloExchange, int rproc)
{
    /*addHaloProc:28*/
    /*CompoundStmt:1647*/
    if (haloExchange->haloCount == 0) {
        haloExchange->haloProc[0] = rproc;
        haloExchange->haloCount = 1;
    } else {
        if (isHaloProc(haloExchange, rproc) == 1)
            return;
        haloExchange->haloProc[haloExchange->haloCount] = rproc;
        haloExchange->haloCount++;
    }
}
/*Function: loadBuffer, ID: 29*/
int loadBuffer(char* buf, struct SparseMatrixSt* xmatrix, struct DomainSt* domain)
{
    /*loadBuffer:29*/
    /*CompoundStmt:1667*/
    NonZeroMsg* rbuf = (NonZeroMsg*)buf;
    int nBuf = 0;
    for (int i = domain->localRowMin; i < domain->localRowMax; i++) {
        for (int j = 0; j < xmatrix->iia[i]; j++) {
            rbuf[nBuf].irow = i;
            rbuf[nBuf].icol = xmatrix->jja[i][j];
            rbuf[nBuf].val = xmatrix->val[i][j];
            nBuf++;
        }
    }
    return nBuf * sizeof(NonZeroMsg);
}
/*Function: unloadBuffer, ID: 30*/
void unloadBuffer(char* buf, int bufSize, struct SparseMatrixSt* xmatrix, struct DomainSt* domain)
{
    /*unloadBuffer:30*/
    /*CompoundStmt:1704*/
    NonZeroMsg* rbuf = (NonZeroMsg*)buf;
    int nBuf = bufSize / sizeof(NonZeroMsg);
    int rcurrent = -1;
    int j = 0;
    for (int i = 0; i < nBuf; i++) {
        int irow = rbuf[i].irow;
        if (irow != rcurrent) {
            xmatrix->iia[irow] = 0;
            rcurrent = irow;
            j = 0;
        }
        xmatrix->iia[irow]++;
        xmatrix->jja[irow][j] = rbuf[i].icol;
        xmatrix->val[irow][j] = rbuf[i].val;
        j++;
    }
}
