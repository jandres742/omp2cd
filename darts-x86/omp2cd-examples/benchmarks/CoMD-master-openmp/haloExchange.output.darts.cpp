#include "haloExchange.output.darts.h"
using namespace darts;
using namespace std;
static void destroyForceExchange(void* vparms);
static void unloadForceBuffer(void* vparms, void* data, int face, int bufSize, char* charBuf);
static int* mkForceRecvCellList(LinkCell* boxes, int face, int nCells);
static int sortAtomsById(const void* a, const void* b);
static int* mkForceSendCellList(LinkCell* boxes, int face, int nCells);
static void exchangeData(HaloExchange* haloExchange, void* data, int iAxis);
static HaloExchange* initHaloExchange(Domain* domain);
static int* mkAtomCellList(LinkCell* boxes, enum HaloFaceOrder iFace, const int nCells);
static void unloadAtomsBuffer(void* vparms, void* data, int face, int bufSize, char* charBuf);
static int loadAtomsBuffer(void* vparms, void* data, int face, char* charBuf);
static int loadForceBuffer(void* vparms, void* data, int face, char* charBuf);
static void destroyAtomsExchange(void* vparms);
/*Function: initAtomHaloExchange, ID: 82*/
HaloExchange* initAtomHaloExchange(Domain* domain, LinkCell* boxes)
{
    /*initAtomHaloExchange:82*/
    /*CompoundStmt:3754*/
    HaloExchange* hh = initHaloExchange(domain);
    int size0 = (boxes->gridSize[1] + 2) * (boxes->gridSize[2] + 2);
    int size1 = (boxes->gridSize[0] + 2) * (boxes->gridSize[2] + 2);
    int size2 = (boxes->gridSize[0] + 2) * (boxes->gridSize[1] + 2);
    int maxSize = ((size0) > (size1) ? (size0) : (size1));
    maxSize = ((size1) > (size2) ? (size1) : (size2));
    hh->bufCapacity = maxSize * 2 * 64 * sizeof(AtomMsg);
    hh->loadBuffer = loadAtomsBuffer;
    hh->unloadBuffer = unloadAtomsBuffer;
    hh->destroy = destroyAtomsExchange;
    AtomExchangeParms* parms = comdMalloc(sizeof(AtomExchangeParms));
    parms->nCells[HALO_X_MINUS] = 2 * (boxes->gridSize[1] + 2) * (boxes->gridSize[2] + 2);
    parms->nCells[HALO_Y_MINUS] = 2 * (boxes->gridSize[0] + 2) * (boxes->gridSize[2] + 2);
    parms->nCells[HALO_Z_MINUS] = 2 * (boxes->gridSize[0] + 2) * (boxes->gridSize[1] + 2);
    parms->nCells[HALO_X_PLUS] = parms->nCells[HALO_X_MINUS];
    parms->nCells[HALO_Y_PLUS] = parms->nCells[HALO_Y_MINUS];
    parms->nCells[HALO_Z_PLUS] = parms->nCells[HALO_Z_MINUS];
    for (int ii = 0; ii < 6; ++ii)
        parms->cellList[ii] = mkAtomCellList(boxes, ii, parms->nCells[ii]);
    for (int ii = 0; ii < 6; ++ii) {
        parms->pbcFactor[ii] = comdMalloc(3 * sizeof(real_t));
        for (int jj = 0; jj < 3; ++jj)
            parms->pbcFactor[ii][jj] = 0.;
    }
    int* procCoord = domain->procCoord;
    int* procGrid = domain->procGrid;
    if (procCoord[HALO_X_AXIS] == 0)
        parms->pbcFactor[HALO_X_MINUS][HALO_X_AXIS] = +1.;
    if (procCoord[HALO_X_AXIS] == procGrid[HALO_X_AXIS] - 1)
        parms->pbcFactor[HALO_X_PLUS][HALO_X_AXIS] = -1.;
    if (procCoord[HALO_Y_AXIS] == 0)
        parms->pbcFactor[HALO_Y_MINUS][HALO_Y_AXIS] = +1.;
    if (procCoord[HALO_Y_AXIS] == procGrid[HALO_Y_AXIS] - 1)
        parms->pbcFactor[HALO_Y_PLUS][HALO_Y_AXIS] = -1.;
    if (procCoord[HALO_Z_AXIS] == 0)
        parms->pbcFactor[HALO_Z_MINUS][HALO_Z_AXIS] = +1.;
    if (procCoord[HALO_Z_AXIS] == procGrid[HALO_Z_AXIS] - 1)
        parms->pbcFactor[HALO_Z_PLUS][HALO_Z_AXIS] = -1.;
    hh->parms = parms;
    return hh;
}
/*Function: initForceHaloExchange, ID: 83*/
HaloExchange* initForceHaloExchange(Domain* domain, LinkCell* boxes)
{
    /*initForceHaloExchange:83*/
    /*CompoundStmt:3941*/
    HaloExchange* hh = initHaloExchange(domain);
    hh->loadBuffer = loadForceBuffer;
    hh->unloadBuffer = unloadForceBuffer;
    hh->destroy = destroyForceExchange;
    int size0 = (boxes->gridSize[1]) * (boxes->gridSize[2]);
    int size1 = (boxes->gridSize[0] + 2) * (boxes->gridSize[2]);
    int size2 = (boxes->gridSize[0] + 2) * (boxes->gridSize[1] + 2);
    int maxSize = ((size0) > (size1) ? (size0) : (size1));
    maxSize = ((size1) > (size2) ? (size1) : (size2));
    hh->bufCapacity = (maxSize)*64 * sizeof(ForceMsg);
    ForceExchangeParms* parms = comdMalloc(sizeof(ForceExchangeParms));
    parms->nCells[HALO_X_MINUS] = (boxes->gridSize[1]) * (boxes->gridSize[2]);
    parms->nCells[HALO_Y_MINUS] = (boxes->gridSize[0] + 2) * (boxes->gridSize[2]);
    parms->nCells[HALO_Z_MINUS] = (boxes->gridSize[0] + 2) * (boxes->gridSize[1] + 2);
    parms->nCells[HALO_X_PLUS] = parms->nCells[HALO_X_MINUS];
    parms->nCells[HALO_Y_PLUS] = parms->nCells[HALO_Y_MINUS];
    parms->nCells[HALO_Z_PLUS] = parms->nCells[HALO_Z_MINUS];
    for (int ii = 0; ii < 6; ++ii) {
        parms->sendCells[ii] = mkForceSendCellList(boxes, ii, parms->nCells[ii]);
        parms->recvCells[ii] = mkForceRecvCellList(boxes, ii, parms->nCells[ii]);
    }
    hh->parms = parms;
    return hh;
}
/*Function: destroyHaloExchange, ID: 84*/
void destroyHaloExchange(HaloExchange** haloExchange)
{
    /*destroyHaloExchange:84*/
    /*CompoundStmt:4047*/
    (*haloExchange)->destroy((*haloExchange)->parms);
    comdFree((*haloExchange)->parms);
    comdFree(*haloExchange);
    *haloExchange = ((void*)0);
}
/*Function: haloExchange, ID: 85*/
void haloExchange(HaloExchange* haloExchange, void* data)
{
    /*haloExchange:85*/
    /*CompoundStmt:4061*/
    for (int iAxis = 0; iAxis < 3; ++iAxis)
        exchangeData(haloExchange, data, iAxis);
}
/*Function: initHaloExchange, ID: 70*/
HaloExchange* initHaloExchange(Domain* domain)
{
    /*initHaloExchange:70*/
    /*CompoundStmt:4067*/
    HaloExchange* hh = comdMalloc(sizeof(HaloExchange));
    hh->nbrRank[HALO_X_MINUS] = processorNum(domain, -1, 0, 0);
    hh->nbrRank[HALO_X_PLUS] = processorNum(domain, +1, 0, 0);
    hh->nbrRank[HALO_Y_MINUS] = processorNum(domain, 0, -1, 0);
    hh->nbrRank[HALO_Y_PLUS] = processorNum(domain, 0, +1, 0);
    hh->nbrRank[HALO_Z_MINUS] = processorNum(domain, 0, 0, -1);
    hh->nbrRank[HALO_Z_PLUS] = processorNum(domain, 0, 0, +1);
    hh->bufCapacity = 0;
    return hh;
}
/*Function: exchangeData, ID: 71*/
void exchangeData(HaloExchange* haloExchange, void* data, int iAxis)
{
    /*exchangeData:71*/
    /*CompoundStmt:4104*/
    enum HaloFaceOrder faceM = 2 * iAxis;
    enum HaloFaceOrder faceP = faceM + 1;
    char* sendBufM = comdMalloc(haloExchange->bufCapacity);
    char* sendBufP = comdMalloc(haloExchange->bufCapacity);
    char* recvBufM = comdMalloc(haloExchange->bufCapacity);
    char* recvBufP = comdMalloc(haloExchange->bufCapacity);
    int nSendM = haloExchange->loadBuffer(haloExchange->parms, data, faceM, sendBufM);
    int nSendP = haloExchange->loadBuffer(haloExchange->parms, data, faceP, sendBufP);
    int nbrRankM = haloExchange->nbrRank[faceM];
    int nbrRankP = haloExchange->nbrRank[faceP];
    int nRecvM, nRecvP;
    do {
        profileStart(commHaloTimer);
    } while (0);
    nRecvP = sendReceiveParallel(
        sendBufM, nSendM, nbrRankM, recvBufP, haloExchange->bufCapacity, nbrRankP);
    nRecvM = sendReceiveParallel(
        sendBufP, nSendP, nbrRankP, recvBufM, haloExchange->bufCapacity, nbrRankM);
    do {
        profileStop(commHaloTimer);
    } while (0);
    haloExchange->unloadBuffer(haloExchange->parms, data, faceM, nRecvM, recvBufM);
    haloExchange->unloadBuffer(haloExchange->parms, data, faceP, nRecvP, recvBufP);
    comdFree(recvBufP);
    comdFree(recvBufM);
    comdFree(sendBufP);
    comdFree(sendBufM);
}
/*Function: mkAtomCellList, ID: 72*/
int* mkAtomCellList(LinkCell* boxes, enum HaloFaceOrder iFace, const int nCells)
{
    /*mkAtomCellList:72*/
    /*CompoundStmt:4160*/
    int* list = comdMalloc(nCells * sizeof(int));
    int xBegin = -1;
    int xEnd = boxes->gridSize[0] + 1;
    int yBegin = -1;
    int yEnd = boxes->gridSize[1] + 1;
    int zBegin = -1;
    int zEnd = boxes->gridSize[2] + 1;
    if (iFace == HALO_X_MINUS)
        xEnd = xBegin + 2;
    if (iFace == HALO_X_PLUS)
        xBegin = xEnd - 2;
    if (iFace == HALO_Y_MINUS)
        yEnd = yBegin + 2;
    if (iFace == HALO_Y_PLUS)
        yBegin = yEnd - 2;
    if (iFace == HALO_Z_MINUS)
        zEnd = zBegin + 2;
    if (iFace == HALO_Z_PLUS)
        zBegin = zEnd - 2;
    int count = 0;
    for (int ix = xBegin; ix < xEnd; ++ix)
        for (int iy = yBegin; iy < yEnd; ++iy)
            for (int iz = zBegin; iz < zEnd; ++iz)
                list[count++] = getBoxFromTuple(boxes, ix, iy, iz);
    return list;
}
/*Function: loadAtomsBuffer, ID: 73*/
int loadAtomsBuffer(void* vparms, void* data, int face, char* charBuf)
{
    /*loadAtomsBuffer:73*/
    /*CompoundStmt:4231*/
    AtomExchangeParms* parms = (AtomExchangeParms*)vparms;
    SimFlat* s = (SimFlat*)data;
    AtomMsg* buf = (AtomMsg*)charBuf;
    real_t* pbcFactor = parms->pbcFactor[face];
    real3 shift;
    shift[0] = pbcFactor[0] * s->domain->globalExtent[0];
    shift[1] = pbcFactor[1] * s->domain->globalExtent[1];
    shift[2] = pbcFactor[2] * s->domain->globalExtent[2];
    int nCells = parms->nCells[face];
    int* cellList = parms->cellList[face];
    int nBuf = 0;
    for (int iCell = 0; iCell < nCells; ++iCell) {
        int iBox = cellList[iCell];
        int iOff = iBox * 64;
        for (int ii = iOff; ii < iOff + s->boxes->nAtoms[iBox]; ++ii) {
            buf[nBuf].gid = s->atoms->gid[ii];
            buf[nBuf].type = s->atoms->iSpecies[ii];
            buf[nBuf].rx = s->atoms->r[ii][0] + shift[0];
            buf[nBuf].ry = s->atoms->r[ii][1] + shift[1];
            buf[nBuf].rz = s->atoms->r[ii][2] + shift[2];
            buf[nBuf].px = s->atoms->p[ii][0];
            buf[nBuf].py = s->atoms->p[ii][1];
            buf[nBuf].pz = s->atoms->p[ii][2];
            ++nBuf;
        }
    }
    return nBuf * sizeof(AtomMsg);
}
/*Function: unloadAtomsBuffer, ID: 74*/
void unloadAtomsBuffer(void* vparms, void* data, int face, int bufSize, char* charBuf)
{
    /*unloadAtomsBuffer:74*/
    /*CompoundStmt:4352*/
    AtomExchangeParms* parms = (AtomExchangeParms*)vparms;
    SimFlat* s = (SimFlat*)data;
    AtomMsg* buf = (AtomMsg*)charBuf;
    int nBuf = bufSize / sizeof(AtomMsg);
    for (int ii = 0; ii < nBuf; ++ii) {
        int gid = buf[ii].gid;
        int type = buf[ii].type;
        real_t rx = buf[ii].rx;
        real_t ry = buf[ii].ry;
        real_t rz = buf[ii].rz;
        real_t px = buf[ii].px;
        real_t py = buf[ii].py;
        real_t pz = buf[ii].pz;
        putAtomInBox(s->boxes, s->atoms, gid, type, rx, ry, rz, px, py, pz);
    }
}
/*Function: destroyAtomsExchange, ID: 75*/
void destroyAtomsExchange(void* vparms)
{
    /*destroyAtomsExchange:75*/
    /*CompoundStmt:4394*/
    AtomExchangeParms* parms = (AtomExchangeParms*)vparms;
    for (int ii = 0; ii < 6; ++ii) {
        comdFree(parms->pbcFactor[ii]);
        comdFree(parms->cellList[ii]);
    }
}
/*Function: mkForceSendCellList, ID: 76*/
int* mkForceSendCellList(LinkCell* boxes, int face, int nCells)
{
    /*mkForceSendCellList:76*/
    /*CompoundStmt:4408*/
    int* list = comdMalloc(nCells * sizeof(int));
    int xBegin, xEnd, yBegin, yEnd, zBegin, zEnd;
    int nx = boxes->gridSize[0];
    int ny = boxes->gridSize[1];
    int nz = boxes->gridSize[2];
    switch (face) {
    case HALO_X_MINUS:
        xBegin = 0;
        xEnd = 1;
        yBegin = 0;
        yEnd = ny;
        zBegin = 0;
        zEnd = nz;
        break;
    case HALO_X_PLUS:
        xBegin = nx - 1;
        xEnd = nx;
        yBegin = 0;
        yEnd = ny;
        zBegin = 0;
        zEnd = nz;
        break;
    case HALO_Y_MINUS:
        xBegin = -1;
        xEnd = nx + 1;
        yBegin = 0;
        yEnd = 1;
        zBegin = 0;
        zEnd = nz;
        break;
    case HALO_Y_PLUS:
        xBegin = -1;
        xEnd = nx + 1;
        yBegin = ny - 1;
        yEnd = ny;
        zBegin = 0;
        zEnd = nz;
        break;
    case HALO_Z_MINUS:
        xBegin = -1;
        xEnd = nx + 1;
        yBegin = -1;
        yEnd = ny + 1;
        zBegin = 0;
        zEnd = 1;
        break;
    case HALO_Z_PLUS:
        xBegin = -1;
        xEnd = nx + 1;
        yBegin = -1;
        yEnd = ny + 1;
        zBegin = nz - 1;
        zEnd = nz;
        break;
    default:
        asm("nop");
    }
    int count = 0;
    for (int ix = xBegin; ix < xEnd; ++ix)
        for (int iy = yBegin; iy < yEnd; ++iy)
            for (int iz = zBegin; iz < zEnd; ++iz)
                list[count++] = getBoxFromTuple(boxes, ix, iy, iz);
    return list;
}
/*Function: mkForceRecvCellList, ID: 77*/
int* mkForceRecvCellList(LinkCell* boxes, int face, int nCells)
{
    /*mkForceRecvCellList:77*/
    /*CompoundStmt:4508*/
    int* list = comdMalloc(nCells * sizeof(int));
    int xBegin, xEnd, yBegin, yEnd, zBegin, zEnd;
    int nx = boxes->gridSize[0];
    int ny = boxes->gridSize[1];
    int nz = boxes->gridSize[2];
    switch (face) {
    case HALO_X_MINUS:
        xBegin = -1;
        xEnd = 0;
        yBegin = 0;
        yEnd = ny;
        zBegin = 0;
        zEnd = nz;
        break;
    case HALO_X_PLUS:
        xBegin = nx;
        xEnd = nx + 1;
        yBegin = 0;
        yEnd = ny;
        zBegin = 0;
        zEnd = nz;
        break;
    case HALO_Y_MINUS:
        xBegin = -1;
        xEnd = nx + 1;
        yBegin = -1;
        yEnd = 0;
        zBegin = 0;
        zEnd = nz;
        break;
    case HALO_Y_PLUS:
        xBegin = -1;
        xEnd = nx + 1;
        yBegin = ny;
        yEnd = ny + 1;
        zBegin = 0;
        zEnd = nz;
        break;
    case HALO_Z_MINUS:
        xBegin = -1;
        xEnd = nx + 1;
        yBegin = -1;
        yEnd = ny + 1;
        zBegin = -1;
        zEnd = 0;
        break;
    case HALO_Z_PLUS:
        xBegin = -1;
        xEnd = nx + 1;
        yBegin = -1;
        yEnd = ny + 1;
        zBegin = nz;
        zEnd = nz + 1;
        break;
    default:
        asm("nop");
    }
    int count = 0;
    for (int ix = xBegin; ix < xEnd; ++ix)
        for (int iy = yBegin; iy < yEnd; ++iy)
            for (int iz = zBegin; iz < zEnd; ++iz)
                list[count++] = getBoxFromTuple(boxes, ix, iy, iz);
    return list;
}
/*Function: loadForceBuffer, ID: 78*/
int loadForceBuffer(void* vparms, void* vdata, int face, char* charBuf)
{
    /*loadForceBuffer:78*/
    /*CompoundStmt:4611*/
    ForceExchangeParms* parms = (ForceExchangeParms*)vparms;
    ForceExchangeData* data = (ForceExchangeData*)vdata;
    ForceMsg* buf = (ForceMsg*)charBuf;
    int nCells = parms->nCells[face];
    int* cellList = parms->sendCells[face];
    int nBuf = 0;
    for (int iCell = 0; iCell < nCells; ++iCell) {
        int iBox = cellList[iCell];
        int iOff = iBox * 64;
        for (int ii = iOff; ii < iOff + data->boxes->nAtoms[iBox]; ++ii) {
            buf[nBuf].dfEmbed = data->dfEmbed[ii];
            ++nBuf;
        }
    }
    return nBuf * sizeof(ForceMsg);
}
/*Function: unloadForceBuffer, ID: 79*/
void unloadForceBuffer(void* vparms, void* vdata, int face, int bufSize, char* charBuf)
{
    /*unloadForceBuffer:79*/
    /*CompoundStmt:4652*/
    ForceExchangeParms* parms = (ForceExchangeParms*)vparms;
    ForceExchangeData* data = (ForceExchangeData*)vdata;
    ForceMsg* buf = (ForceMsg*)charBuf;
    int nCells = parms->nCells[face];
    int* cellList = parms->recvCells[face];
    int iBuf = 0;
    for (int iCell = 0; iCell < nCells; ++iCell) {
        int iBox = cellList[iCell];
        int iOff = iBox * 64;
        for (int ii = iOff; ii < iOff + data->boxes->nAtoms[iBox]; ++ii) {
            data->dfEmbed[ii] = buf[iBuf].dfEmbed;
            ++iBuf;
        }
    }
}
/*Function: destroyForceExchange, ID: 80*/
void destroyForceExchange(void* vparms)
{
    /*destroyForceExchange:80*/
    /*CompoundStmt:4690*/
    ForceExchangeParms* parms = (ForceExchangeParms*)vparms;
    for (int ii = 0; ii < 6; ++ii) {
        comdFree(parms->sendCells[ii]);
        comdFree(parms->recvCells[ii]);
    }
}
/*Function: sortAtomsInCell, ID: 97*/
void sortAtomsInCell(Atoms* atoms, LinkCell* boxes, int iBox)
{
    /*sortAtomsInCell:97*/
    /*CompoundStmt:4704*/
    int nAtoms = boxes->nAtoms[iBox];
    AtomMsg tmp[nAtoms];
    int begin = iBox * 64;
    int end = begin + nAtoms;
    for (int ii = begin, iTmp = 0; ii < end; ++ii, ++iTmp) {
        tmp[iTmp].gid = atoms->gid[ii];
        tmp[iTmp].type = atoms->iSpecies[ii];
        tmp[iTmp].rx = atoms->r[ii][0];
        tmp[iTmp].ry = atoms->r[ii][1];
        tmp[iTmp].rz = atoms->r[ii][2];
        tmp[iTmp].px = atoms->p[ii][0];
        tmp[iTmp].py = atoms->p[ii][1];
        tmp[iTmp].pz = atoms->p[ii][2];
    }
    qsort(&tmp, nAtoms, sizeof(AtomMsg), sortAtomsById);
    for (int ii = begin, iTmp = 0; ii < end; ++ii, ++iTmp) {
        atoms->gid[ii] = tmp[iTmp].gid;
        atoms->iSpecies[ii] = tmp[iTmp].type;
        atoms->r[ii][0] = tmp[iTmp].rx;
        atoms->r[ii][1] = tmp[iTmp].ry;
        atoms->r[ii][2] = tmp[iTmp].rz;
        atoms->p[ii][0] = tmp[iTmp].px;
        atoms->p[ii][1] = tmp[iTmp].py;
        atoms->p[ii][2] = tmp[iTmp].pz;
    }
}
/*Function: sortAtomsById, ID: 81*/
int sortAtomsById(const void* a, const void* b)
{
    /*sortAtomsById:81*/
    /*CompoundStmt:4822*/
    int aId = ((AtomMsg*)a)->gid;
    int bId = ((AtomMsg*)b)->gid;
    if (aId < bId)
        return -1;
    return 1;
}
