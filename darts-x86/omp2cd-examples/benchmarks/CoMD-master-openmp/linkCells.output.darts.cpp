#include "linkCells.output.darts.h"
using namespace darts;
using namespace std;
static void getTuple(LinkCell* boxes, int iBox, int* ixp, int* iyp, int* izp);
static void emptyHaloCells(LinkCell* boxes);
static void copyAtom(LinkCell* boxes, Atoms* atoms, int iAtom, int iBox, int jAtom, int jBox);
static int getBoxFromCoord(LinkCell* boxes, real_t rr[3]);
/*Function: initLinkCells, ID: 113*/
LinkCell* initLinkCells(const Domain* domain, real_t cutoff)
{
    /*initLinkCells:113*/
    /*CompoundStmt:5625*/
    LinkCell* ll = comdMalloc(sizeof(LinkCell));
    for (int i = 0; i < 3; i++) {
        ll->localMin[i] = domain->localMin[i];
        ll->localMax[i] = domain->localMax[i];
        ll->gridSize[i] = domain->localExtent[i] / cutoff;
        ll->boxSize[i] = domain->localExtent[i] / ((real_t)ll->gridSize[i]);
        ll->invBoxSize[i] = 1. / ll->boxSize[i];
    }
    ll->nLocalBoxes = ll->gridSize[0] * ll->gridSize[1] * ll->gridSize[2];
    ll->nHaloBoxes = 2 * ((ll->gridSize[0] + 2) * (ll->gridSize[1] + ll->gridSize[2] + 2)
                             + (ll->gridSize[1] * ll->gridSize[2]));
    ll->nTotalBoxes = ll->nLocalBoxes + ll->nHaloBoxes;
    ll->nAtoms = comdMalloc(ll->nTotalBoxes * sizeof(int));
    for (int iBox = 0; iBox < ll->nTotalBoxes; ++iBox)
        ll->nAtoms[iBox] = 0;
    ll->nbrBoxes = comdMalloc(ll->nTotalBoxes * sizeof(int*));
    for (int iBox = 0; iBox < ll->nTotalBoxes; ++iBox) {
        ll->nbrBoxes[iBox] = comdMalloc(27 * sizeof(int));
    }
    for (int iBox = 0; iBox < ll->nLocalBoxes; ++iBox) {
        int nNbrBoxes = getNeighborBoxes(ll, iBox, ll->nbrBoxes[iBox]);
    }
    return ll;
}
/*Function: destroyLinkCells, ID: 114*/
void destroyLinkCells(LinkCell** boxes)
{
    /*destroyLinkCells:114*/
    /*CompoundStmt:5743*/
    if (!boxes)
        return;
    if (!*boxes)
        return;
    comdFree((*boxes)->nAtoms);
    comdFree(*boxes);
    *boxes = ((void*)0);
    return;
}
/*Function: getNeighborBoxes, ID: 115*/
int getNeighborBoxes(LinkCell* boxes, int iBox, int* nbrBoxes)
{
    /*getNeighborBoxes:115*/
    /*CompoundStmt:5761*/
    int ix, iy, iz;
    getTuple(boxes, iBox, &ix, &iy, &iz);
    int count = 0;
    for (int i = ix - 1; i <= ix + 1; i++)
        for (int j = iy - 1; j <= iy + 1; j++)
            for (int k = iz - 1; k <= iz + 1; k++)
                nbrBoxes[count++] = getBoxFromTuple(boxes, i, j, k);
    return count;
}
/*Function: putAtomInBox, ID: 116*/
void putAtomInBox(LinkCell* boxes, Atoms* atoms, const int gid, const int iType, const real_t x,
    const real_t y, const real_t z, const real_t px, const real_t py, const real_t pz)
{
    /*putAtomInBox:116*/
    /*CompoundStmt:5791*/
    real_t xyz[3] = { x, y, z };
    int iBox = getBoxFromCoord(boxes, xyz);
    int iOff = iBox * 64;
    iOff += boxes->nAtoms[iBox];
    if (iBox < boxes->nLocalBoxes)
        atoms->nLocal++;
    boxes->nAtoms[iBox]++;
    atoms->gid[iOff] = gid;
    atoms->iSpecies[iOff] = iType;
    atoms->r[iOff][0] = x;
    atoms->r[iOff][1] = y;
    atoms->r[iOff][2] = z;
    atoms->p[iOff][0] = px;
    atoms->p[iOff][1] = py;
    atoms->p[iOff][2] = pz;
}
/*Function: getBoxFromTuple, ID: 117*/
int getBoxFromTuple(LinkCell* boxes, int ix, int iy, int iz)
{
    /*getBoxFromTuple:117*/
    /*CompoundStmt:5839*/
    int iBox = 0;
    const int* gridSize = boxes->gridSize;
    if (iz == gridSize[2]) {
        iBox = boxes->nLocalBoxes + 2 * gridSize[2] * gridSize[1]
            + 2 * gridSize[2] * (gridSize[0] + 2) + (gridSize[0] + 2) * (gridSize[1] + 2)
            + (gridSize[0] + 2) * (iy + 1) + (ix + 1);
    } else if (iz == -1) {
        iBox = boxes->nLocalBoxes + 2 * gridSize[2] * gridSize[1]
            + 2 * gridSize[2] * (gridSize[0] + 2) + (gridSize[0] + 2) * (iy + 1) + (ix + 1);
    } else if (iy == gridSize[1]) {
        iBox = boxes->nLocalBoxes + 2 * gridSize[2] * gridSize[1] + gridSize[2] * (gridSize[0] + 2)
            + (gridSize[0] + 2) * iz + (ix + 1);
    } else if (iy == -1) {
        iBox = boxes->nLocalBoxes + 2 * gridSize[2] * gridSize[1] + iz * (gridSize[0] + 2)
            + (ix + 1);
    } else if (ix == gridSize[0]) {
        iBox = boxes->nLocalBoxes + gridSize[1] * gridSize[2] + iz * gridSize[1] + iy;
    } else if (ix == -1) {
        iBox = boxes->nLocalBoxes + iz * gridSize[1] + iy;
    } else {
        iBox = ix + gridSize[0] * iy + gridSize[0] * gridSize[1] * iz;
    }
    return iBox;
}
/*Function: moveAtom, ID: 118*/
void moveAtom(LinkCell* boxes, Atoms* atoms, int iId, int iBox, int jBox)
{
    /*moveAtom:118*/
    /*CompoundStmt:5970*/
    int nj = boxes->nAtoms[jBox];
    copyAtom(boxes, atoms, iId, iBox, nj, jBox);
    boxes->nAtoms[jBox]++;
    boxes->nAtoms[iBox]--;
    int ni = boxes->nAtoms[iBox];
    if (ni)
        copyAtom(boxes, atoms, ni, iBox, iId, iBox);
    if (jBox > boxes->nLocalBoxes)
        --atoms->nLocal;
    return;
}
/*Function: updateLinkCells, ID: 119*/
void updateLinkCells(LinkCell* boxes, Atoms* atoms)
{
    /*updateLinkCells:119*/
    /*CompoundStmt:5994*/
    emptyHaloCells(boxes);
    for (int iBox = 0; iBox < boxes->nLocalBoxes; ++iBox) {
        int iOff = iBox * 64;
        int ii = 0;
        while (ii < boxes->nAtoms[iBox]) {
            int jBox = getBoxFromCoord(boxes, atoms->r[iOff + ii]);
            if (jBox != iBox)
                moveAtom(boxes, atoms, ii, iBox, jBox);
            else
                ++ii;
        }
    }
}
/*Function: maxOccupancy, ID: 120*/
int maxOccupancy(LinkCell* boxes)
{
    /*maxOccupancy:120*/
    /*CompoundStmt:6021*/
    int localMax = 0;
    for (int ii = 0; ii < boxes->nLocalBoxes; ++ii)
        localMax = ((localMax) > (boxes->nAtoms[ii]) ? (localMax) : (boxes->nAtoms[ii]));
    int globalMax;
    do {
        profileStart(commReduceTimer);
    } while (0);
    maxIntParallel(&localMax, &globalMax, 1);
    do {
        profileStop(commReduceTimer);
    } while (0);
    return globalMax;
}
/*Function: copyAtom, ID: 109*/
void copyAtom(LinkCell* boxes, Atoms* atoms, int iAtom, int iBox, int jAtom, int jBox)
{
    /*copyAtom:109*/
    /*CompoundStmt:6048*/
    const int iOff = 64 * iBox + iAtom;
    const int jOff = 64 * jBox + jAtom;
    atoms->gid[jOff] = atoms->gid[iOff];
    atoms->iSpecies[jOff] = atoms->iSpecies[iOff];
    memcpy(atoms->r[jOff], atoms->r[iOff], sizeof(real3));
    memcpy(atoms->p[jOff], atoms->p[iOff], sizeof(real3));
    memcpy(atoms->f[jOff], atoms->f[iOff], sizeof(real3));
    memcpy(atoms->U + jOff, atoms->U + iOff, sizeof(real_t));
}
/*Function: getBoxFromCoord, ID: 110*/
int getBoxFromCoord(LinkCell* boxes, real_t rr[3])
{
    /*getBoxFromCoord:110*/
    /*CompoundStmt:6089*/
    const real_t* localMin = boxes->localMin;
    const real_t* localMax = boxes->localMax;
    const int* gridSize = boxes->gridSize;
    int ix = (int)(floor((rr[0] - localMin[0]) * boxes->invBoxSize[0]));
    int iy = (int)(floor((rr[1] - localMin[1]) * boxes->invBoxSize[1]));
    int iz = (int)(floor((rr[2] - localMin[2]) * boxes->invBoxSize[2]));
    if (rr[0] < localMax[0]) {
        if (ix == gridSize[0])
            ix = gridSize[0] - 1;
    } else
        ix = gridSize[0];
    if (rr[1] < localMax[1]) {
        if (iy == gridSize[1])
            iy = gridSize[1] - 1;
    } else
        iy = gridSize[1];
    if (rr[2] < localMax[2]) {
        if (iz == gridSize[2])
            iz = gridSize[2] - 1;
    } else
        iz = gridSize[2];
    return getBoxFromTuple(boxes, ix, iy, iz);
}
/*Function: emptyHaloCells, ID: 111*/
void emptyHaloCells(LinkCell* boxes)
{
    /*emptyHaloCells:111*/
    /*CompoundStmt:6161*/
    for (int ii = boxes->nLocalBoxes; ii < boxes->nTotalBoxes; ++ii)
        boxes->nAtoms[ii] = 0;
}
/*Function: getTuple, ID: 112*/
void getTuple(LinkCell* boxes, int iBox, int* ixp, int* iyp, int* izp)
{
    /*getTuple:112*/
    /*CompoundStmt:6171*/
    int ix, iy, iz;
    const int* gridSize = boxes->gridSize;
    if (iBox < boxes->nLocalBoxes) {
        ix = iBox % gridSize[0];
        iBox /= gridSize[0];
        iy = iBox % gridSize[1];
        iz = iBox / gridSize[1];
    } else {
        int ink;
        ink = iBox - boxes->nLocalBoxes;
        if (ink < 2 * gridSize[1] * gridSize[2]) {
            if (ink < gridSize[1] * gridSize[2]) {
                ix = 0;
            } else {
                ink -= gridSize[1] * gridSize[2];
                ix = gridSize[0] + 1;
            }
            iy = 1 + ink % gridSize[1];
            iz = 1 + ink / gridSize[1];
        } else if (ink < (2 * gridSize[2] * (gridSize[1] + gridSize[0] + 2))) {
            ink -= 2 * gridSize[2] * gridSize[1];
            if (ink < ((gridSize[0] + 2) * gridSize[2])) {
                iy = 0;
            } else {
                ink -= (gridSize[0] + 2) * gridSize[2];
                iy = gridSize[1] + 1;
            }
            ix = ink % (gridSize[0] + 2);
            iz = 1 + ink / (gridSize[0] + 2);
        } else {
            ink -= 2 * gridSize[2] * (gridSize[1] + gridSize[0] + 2);
            if (ink < ((gridSize[0] + 2) * (gridSize[1] + 2))) {
                iz = 0;
            } else {
                ink -= (gridSize[0] + 2) * (gridSize[1] + 2);
                iz = gridSize[2] + 1;
            }
            ix = ink % (gridSize[0] + 2);
            iy = ink / (gridSize[0] + 2);
        }
        ix--;
        iy--;
        iz--;
    }
    *ixp = ix;
    *iyp = iy;
    *izp = iz;
}
