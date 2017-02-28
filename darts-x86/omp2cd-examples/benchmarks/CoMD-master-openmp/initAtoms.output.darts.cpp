#include "initAtoms.output.darts.h"
using namespace darts;
using namespace std;
static void computeVcm(SimFlat* s, real_t vcm[3]);
/*Function: initAtoms, ID: 101*/
Atoms* initAtoms(LinkCell* boxes)
{
    /*initAtoms:101*/
    /*CompoundStmt:4836*/
    Atoms* atoms = comdMalloc(sizeof(Atoms));
    int maxTotalAtoms = 64 * boxes->nTotalBoxes;
    atoms->gid = (int*)comdMalloc(maxTotalAtoms * sizeof(int));
    atoms->iSpecies = (int*)comdMalloc(maxTotalAtoms * sizeof(int));
    atoms->r = (real3*)comdMalloc(maxTotalAtoms * sizeof(real3));
    atoms->p = (real3*)comdMalloc(maxTotalAtoms * sizeof(real3));
    atoms->f = (real3*)comdMalloc(maxTotalAtoms * sizeof(real3));
    atoms->U = (real_t*)comdMalloc(maxTotalAtoms * sizeof(real_t));
    atoms->nLocal = 0;
    atoms->nGlobal = 0;
    for (int iOff = 0; iOff < maxTotalAtoms; iOff++) {
        atoms->gid[iOff] = 0;
        atoms->iSpecies[iOff] = 0;
        zeroReal3(atoms->r[iOff]);
        zeroReal3(atoms->p[iOff]);
        zeroReal3(atoms->f[iOff]);
        atoms->U[iOff] = 0.;
    }
    return atoms;
}
/*Function: destroyAtoms, ID: 102*/
void destroyAtoms(Atoms* atoms)
{
    /*destroyAtoms:102*/
    /*CompoundStmt:4908*/
    {
        /*CompoundStmt:4909*/
        if (atoms->gid)
            comdFree(atoms->gid);
        atoms->gid = ((void*)0);
    }
    {
        /*CompoundStmt:4918*/
        if (atoms->iSpecies)
            comdFree(atoms->iSpecies);
        atoms->iSpecies = ((void*)0);
    }
    {
        /*CompoundStmt:4927*/
        if (atoms->r)
            comdFree(atoms->r);
        atoms->r = ((void*)0);
    }
    {
        /*CompoundStmt:4936*/
        if (atoms->p)
            comdFree(atoms->p);
        atoms->p = ((void*)0);
    }
    {
        /*CompoundStmt:4945*/
        if (atoms->f)
            comdFree(atoms->f);
        atoms->f = ((void*)0);
    }
    {
        /*CompoundStmt:4954*/
        if (atoms->U)
            comdFree(atoms->U);
        atoms->U = ((void*)0);
    }
    comdFree(atoms);
}
/*Function: createFccLattice, ID: 103*/
void createFccLattice(int nx, int ny, int nz, real_t lat, SimFlat* s)
{
    /*createFccLattice:103*/
    /*CompoundStmt:4964*/
    const real_t* localMin = s->domain->localMin;
    const real_t* localMax = s->domain->localMax;
    int nb = 4;
    real3 basis[4] = { { 0.25, 0.25, 0.25 }, { 0.25, 0.75, 0.75 }, { 0.75, 0.25, 0.75 },
        { 0.75, 0.75, 0.25 } };
    int begin[3];
    int end[3];
    for (int ii = 0; ii < 3; ++ii) {
        begin[ii] = floor(localMin[ii] / lat);
        end[ii] = ceil(localMax[ii] / lat);
    }
    real_t px, py, pz;
    px = py = pz = 0.;
    for (int ix = begin[0]; ix < end[0]; ++ix)
        for (int iy = begin[1]; iy < end[1]; ++iy)
            for (int iz = begin[2]; iz < end[2]; ++iz)
                for (int ib = 0; ib < nb; ++ib) {
                    real_t rx = (ix + basis[ib][0]) * lat;
                    real_t ry = (iy + basis[ib][1]) * lat;
                    real_t rz = (iz + basis[ib][2]) * lat;
                    if (rx < localMin[0] || rx >= localMax[0])
                        continue;
                    if (ry < localMin[1] || ry >= localMax[1])
                        continue;
                    if (rz < localMin[2] || rz >= localMax[2])
                        continue;
                    int id = ib + nb * (iz + nz * (iy + ny * (ix)));
                    putAtomInBox(s->boxes, s->atoms, id, 0, rx, ry, rz, px, py, pz);
                }
    do {
        profileStart(commReduceTimer);
    } while (0);
    addIntParallel(&s->atoms->nLocal, &s->atoms->nGlobal, 1);
    do {
        profileStop(commReduceTimer);
    } while (0);
}
/*Function: setVcm, ID: 104*/
void setVcm(SimFlat* s, real_t newVcm[3])
{
    /*setVcm:104*/
    /*CompoundStmt:5127*/
    real_t oldVcm[3];
    computeVcm(s, oldVcm);
    real_t* vShift = (real_t*)malloc(sizeof(real_t) * 3);
    vShift[0] = (newVcm[0] - oldVcm[0]);
    vShift[1] = (newVcm[1] - oldVcm[1]);
    vShift[2] = (newVcm[2] - oldVcm[2]);
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP5150>(ompNumThreads * DARTS_CODELETS_MULT, 0,
            RuntimeFinalCodelet, (SimFlat**)&((s)), (real_t**)&((vShift))));
    }
    free(vShift);
}
/*Function: setTemperature, ID: 105*/
void setTemperature(SimFlat* s, real_t temperature)
{
    /*setTemperature:105*/
    /*CompoundStmt:5225*/
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP5226>(ompNumThreads * DARTS_CODELETS_MULT, 0,
            RuntimeFinalCodelet, (SimFlat**)&((s)), (real_t*)&((temperature))));
    }
    if (temperature == 0.)
        return;
    real_t vZero[3] = { 0., 0., 0. };
    setVcm(s, vZero);
    kineticEnergy(s);
    real_t temp = (s->eKinetic / s->atoms->nGlobal) / kB_eV / 1.5;
    real_t scaleFactor = sqrt(temperature / temp);
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP5340>(ompNumThreads * DARTS_CODELETS_MULT, 0,
            RuntimeFinalCodelet, (SimFlat**)&((s)), (real_t*)&((scaleFactor))));
    }
    kineticEnergy(s);
    temp = s->eKinetic / s->atoms->nGlobal / kB_eV / 1.5;
}
/*Function: randomDisplacements, ID: 106*/
void randomDisplacements(SimFlat* s, real_t delta)
{
    /*randomDisplacements:106*/
    /*CompoundStmt:5409*/
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP5410>(ompNumThreads * DARTS_CODELETS_MULT, 0,
            RuntimeFinalCodelet, (real_t*)&((delta)), (SimFlat**)&((s))));
    }
}
/*Function: computeVcm, ID: 100*/
void computeVcm(SimFlat* s, real_t vcm[3])
{
    /*computeVcm:100*/
    /*CompoundStmt:5496*/
    real_t vcmLocal[4] = { 0., 0., 0., 0. };
    real_t vcmSum[4] = { 0., 0., 0., 0. };
    real_t v0 = 0.;
    real_t v1 = 0.;
    real_t v2 = 0.;
    real_t v3 = 0.;
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP5525>(ompNumThreads * DARTS_CODELETS_MULT, 0,
            RuntimeFinalCodelet, (SimFlat**)&((s)), (real_t*)&((v0)), (real_t*)&((v1)),
            (real_t*)&((v2)), (real_t*)&((v3))));
    }
    vcmLocal[0] = v0;
    vcmLocal[1] = v1;
    vcmLocal[2] = v2;
    vcmLocal[3] = v3;
    do {
        profileStart(commReduceTimer);
    } while (0);
    addRealParallel(vcmLocal, vcmSum, 4);
    do {
        profileStop(commReduceTimer);
    } while (0);
    real_t totalMass = vcmSum[3];
    vcm[0] = vcmSum[0] / totalMass;
    vcm[1] = vcmSum[1] / totalMass;
    vcm[2] = vcmSum[2] / totalMass;
}
/*TP5150: OMPParallelDirective*/
void TP5150::_barrierCodelets5150::fire(void)
{
    TP5150* myTP = static_cast<TP5150*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
void TP5150::_checkInCodelets5152::fire(void)
{
    /*printOMPForCodeletFireCode, inlined: 0*/
    /*region 5152 0*/
    /*Determine the TP to which this codelet belongs and check if this codelet spawns the TP or if
     * it signals it using dispatchCodelet()*/
    size_t idx = this->getID() / myTP->codeletsPerTP5152;
    if (idx < myTP->TPsToUse5152) {
        if (!__sync_val_compare_and_swap(&(myTP->TP5152_alreadyLaunched[idx]), 0, 1)) {
            int range = abs((*(this->inputsTPParent->s_darts5150))->boxes->nLocalBoxes - 0) / 1;
            int rangePerCodelet = range / myTP->TPsToUse5152;
            int minIteration
                = min<int>((*(this->inputsTPParent->s_darts5150))->boxes->nLocalBoxes, 0);
            int remainderRange = range % myTP->TPsToUse5152;
            int initIteration = rangePerCodelet * idx;
            int lastIteration = rangePerCodelet * (idx + 1);
            if (remainderRange != 0) {
                if (idx < (uint32_t)remainderRange) {
                    initIteration += idx;
                    lastIteration += (idx + 1);
                } else {
                    initIteration += remainderRange;
                    lastIteration += remainderRange;
                }
            }
            initIteration = initIteration * 1 + minIteration;
            lastIteration = lastIteration * 1 + minIteration;
            if (0 < (*(this->inputsTPParent->s_darts5150))->boxes->nLocalBoxes) {
                initIteration = min(initIteration, lastIteration);
                lastIteration = max(initIteration, lastIteration);
            } else {
                initIteration = max(initIteration, lastIteration);
                lastIteration = min(initIteration, lastIteration);
            }
            if (idx == myTP->TPsToUse5152 - 1) {
                lastIteration = (*(this->inputsTPParent->s_darts5150))->boxes->nLocalBoxes;
            }
#if USEINVOKE == 1
            invoke<TP5152>(myTP, myTP->codeletsPerTP5152 * DARTS_CODELETS_MULT, this->getID(), myTP,
                initIteration, lastIteration, &(*(this->inputsTPParent->s_darts5150)),
                &(*(this->inputsTPParent->vShift_darts5150)), &(myTP->TP5152Ptr[idx]));
#else
            place<TP5152>(idx, myTP, myTP->codeletsPerTP5152 * DARTS_CODELETS_MULT, this->getID(),
                myTP, initIteration, lastIteration, &(*(this->inputsTPParent->s_darts5150)),
                &(*(this->inputsTPParent->vShift_darts5150)), &(myTP->TP5152Ptr[idx]));
#endif
        } else {
            if (myTP->TP5152Ptr[idx] != nullptr) {
                myTP->TP5152Ptr[idx]->dispatchCodelet(this->getID());
            } else {
                this->resetCodelet();
                this->decDep();
            }
        }
    }
}
void TP5150::_barrierCodelets5152::fire(void)
{
    TP5150* myTP = static_cast<TP5150*>(myTP_);
    myTP->TPParent->barrierCodelets5150[0].setDep(0);
    myTP->add(&(myTP->TPParent->barrierCodelets5150[0]));
}
TP5150::TP5150(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet, SimFlat** in_s,
    real_t** in_vShift)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , s_darts5150(in_s) /*OMP_SHARED - INPUT*/
    , vShift_darts5150(in_vShift) /*OMP_SHARED - INPUT*/
    , TP5152Ptr(new TP5152*[NUMTPS5152])
    , TP5152_alreadyLaunched(new size_t[NUMTPS5152])
    , numTPsSet5152(0)
    , numTPsReady5152(0)
    , TPsToUse5152(NUMTPS5152)
    , codeletsPerTP5152(this->numThreads / NUMTPS5152)
    , totalCodelets5152(this->TPsToUse5152 * this->codeletsPerTP5152)
    , barrierCodelets5150(new _barrierCodelets5150[1])
    , checkInCodelets5152(new _checkInCodelets5152[this->numThreads])
    , barrierCodelets5152(new _barrierCodelets5152[1])
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets5150[0] = _barrierCodelets5150(ompNumThreads, ompNumThreads, this, 0);
    barrierCodelets5152[0] = _barrierCodelets5152(NUMTPS5152, NUMTPS5152, this, 0);
    _checkInCodelets5152* checkInCodelets5152Ptr = (this->checkInCodelets5152);
    for (int i = 0; i < NUMTPS5152; i++) {
        TP5152Ptr[i] = nullptr;
        TP5152_alreadyLaunched[i] = 0;
    }
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets5152Ptr) = _checkInCodelets5152(1, 1, this, codeletCounter);
        (*checkInCodelets5152Ptr).decDep();
        checkInCodelets5152Ptr++;
    }
}
TP5150::~TP5150()
{
    delete[] barrierCodelets5150;
    delete[] barrierCodelets5152;
    delete[] checkInCodelets5152;
}
/*TP5152: OMPForDirective*/
void TP5152::_barrierCodelets5152::fire(void)
{
    TP5152* myTP = static_cast<TP5152*>(myTP_);
    myTP->controlTPParent->TPParent->barrierCodelets5152[0].decDep();
}
bool TP5152::requestNewRangeIterations5152(int* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Static */
    /*Chunk = 0*/
    bool isThereNewIteration = false;
    {
        /*Static Scheduling*/
        int tempStartRange = rangePerCodelet5152 * codeletID;
        int tempEndRange = rangePerCodelet5152 * (codeletID + 1);
        if (remainderRange5152 != 0) {
            if (codeletID < (uint32_t)remainderRange5152) {
                tempStartRange += codeletID;
                tempEndRange += (codeletID + 1);
            } else {
                tempStartRange += remainderRange5152;
                tempEndRange += remainderRange5152;
            }
        }
        tempStartRange = tempStartRange * 1 + minIteration5152;
        tempEndRange = tempEndRange * 1 + minIteration5152;
        if (tempStartRange != tempEndRange) {
            isThereNewIteration = true;
        }
        if (initIteration5152 < lastIteration5152) {
            (this->inputsTPParent->iBox_darts5152[codeletID]) = min(tempStartRange, tempEndRange);
            *endRange = max(tempStartRange, tempEndRange);
        } else {
            (this->inputsTPParent->iBox_darts5152[codeletID]) = max(tempStartRange, tempEndRange);
            *endRange = min(tempStartRange, tempEndRange);
        }
        if (codeletID == this->numThreads - 1) {
            *endRange = lastIteration5152;
        }
    }
    return isThereNewIteration;
}
void TP5152::_checkInCodelets5153::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    /*Init the vars for this region*/

    /*printing node 5153: ForStmt*/
    /*var: s*/
    /*var: vShift*/
    SimFlat** s = (this->inputsTPParent->s_darts5152);
    (void)s /*OMP_SHARED*/;
    real_t** vShift = (this->inputsTPParent->vShift_darts5152);
    (void)vShift /*OMP_SHARED*/;
    int* iBox = &(this->inputsTPParent->iBox_darts5152[this->getLocalID()]);
    (void)iBox /*PRIVATE*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations5152(
        (int*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        /*Signaling omp for stmt's barrier*/
        myTP->controlTPParent->barrierCodelets5152[0].decDep();
        return;
    }
    for (int iBox_darts_counter_temp5152 = (*iBox); iBox_darts_counter_temp5152 < endRange
         && iBox_darts_counter_temp5152 < this->inputsTPParent->lastIteration5152;
         ++iBox_darts_counter_temp5152) {
        {
            {
                /*Loop's init*/
                int iOff = 64 * (iBox_darts_counter_temp5152);
                int ii = 0;
                for (; ii < (*(s))->boxes->nAtoms[(iBox_darts_counter_temp5152)]; ++ii, ++iOff) {
                    int iSpecies = (*(s))->atoms->iSpecies[iOff];
                    real_t mass = (*(s))->species[iSpecies].mass;
                    (*(s))->atoms->p[iOff][0] += mass * (*(vShift))[0];
                    (*(s))->atoms->p[iOff][1] += mass * (*(vShift))[1];
                    (*(s))->atoms->p[iOff][2] += mass * (*(vShift))[2];
                }
            }
        }
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling omp for stmt's barrier*/
    myTP->controlTPParent->barrierCodelets5152[0].decDep();
}
TP5152::TP5152(int in_numThreads, int in_mainCodeletID, TP5150* in_TPParent, int in_initIteration,
    int in_lastIteration, SimFlat** in_s, real_t** in_vShift, TP5152** in_ptrToThisTP)
    : ompTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , s_darts5152(in_s) /*OMP_SHARED - INPUT*/
    , vShift_darts5152(in_vShift) /*OMP_SHARED - INPUT*/
    , iBox_darts5152(new int[this->numThreads]) /*VARIABLE*/
    , initIteration5152(in_initIteration)
    , lastIteration5152(in_lastIteration)
    , readyCodelets(0)
    , baseNumThreads(this->numThreads / DARTS_CODELETS_MULT)
    , barrierCodelets5152(new _barrierCodelets5152[1])
    , checkInCodelets5153(new _checkInCodelets5153[this->numThreads])
{
    /*Initialize the loop parameters*/
    range5152 = abs(lastIteration5152 - initIteration5152) / 1;
    rangePerCodelet5152 = range5152 / numThreads;
    minIteration5152 = min<int>(lastIteration5152, initIteration5152);
    remainderRange5152 = range5152 % numThreads;
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets5152[0] = _barrierCodelets5152(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets5153* checkInCodelets5153Ptr = (this->checkInCodelets5153);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets5153);
#endif
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->baseNumThreads;
         codeletCounter++) {
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets5153Ptr) = _checkInCodelets5153(2, 1, this, codeletCounter);
#else
        (*checkInCodelets5153Ptr) = _checkInCodelets5153(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets5153Ptr).decDep();
        checkInCodelets5153Ptr++;
    }
    this->dispatchCodelet(this->getID());
    *(in_ptrToThisTP) = this;
}
void TP5152::dispatchCodelet(size_t codeletID)
{
    int idx = codeletID / this->baseNumThreads;
    int localID = codeletID - this->baseNumThreads * idx;
    this->checkInCodelets5153[localID].setID(codeletID);
    this->checkInCodelets5153[localID].setLocalID(localID);
    /*Check if we want to replicate codelets*/
    for (size_t i = 1; i < (size_t)DARTS_CODELETS_MULT; i++) {
#if USE_SPIN_CODELETS == 0
        this->checkInCodelets5153[localID + this->baseNumThreads * i]
            = _checkInCodelets5153(2, 1, this, localID + this->baseNumThreads * i);
#else
        this->checkInCodelets5153[localID + this->baseNumThreads * i]
            = _checkInCodelets5153(1, 1, this, localID + this->baseNumThreads * i);
#endif
        this->checkInCodelets5153[localID + this->baseNumThreads * i].setID(codeletID);
        this->checkInCodelets5153[localID + this->baseNumThreads * i].decDep();
#if USE_SPIN_CODELETS == 0
        this->firstCodelet[localID + this->baseNumThreads * i].decDep();
#else
        this->availableCodelets[localID + this->baseNumThreads * i] = 1;
#endif
    }
#if USE_SPIN_CODELETS == 0
    this->firstCodelet[localID].decDep();
#else
    this->availableCodelets[localID] = 1;
#endif
}
TP5152::~TP5152()
{
    delete[] iBox_darts5152;
    delete[] barrierCodelets5152;
    delete[] checkInCodelets5153;
}
/*TP5226: OMPParallelDirective*/
void TP5226::_barrierCodelets5226::fire(void)
{
    TP5226* myTP = static_cast<TP5226*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
void TP5226::_checkInCodelets5228::fire(void)
{
    /*printOMPForCodeletFireCode, inlined: 0*/
    /*region 5228 0*/
    /*Determine the TP to which this codelet belongs and check if this codelet spawns the TP or if
     * it signals it using dispatchCodelet()*/
    size_t idx = this->getID() / myTP->codeletsPerTP5228;
    if (idx < myTP->TPsToUse5228) {
        if (!__sync_val_compare_and_swap(&(myTP->TP5228_alreadyLaunched[idx]), 0, 1)) {
            int range = abs((*(this->inputsTPParent->s_darts5226))->boxes->nLocalBoxes - 0) / 1;
            int rangePerCodelet = range / myTP->TPsToUse5228;
            int minIteration
                = min<int>((*(this->inputsTPParent->s_darts5226))->boxes->nLocalBoxes, 0);
            int remainderRange = range % myTP->TPsToUse5228;
            int initIteration = rangePerCodelet * idx;
            int lastIteration = rangePerCodelet * (idx + 1);
            if (remainderRange != 0) {
                if (idx < (uint32_t)remainderRange) {
                    initIteration += idx;
                    lastIteration += (idx + 1);
                } else {
                    initIteration += remainderRange;
                    lastIteration += remainderRange;
                }
            }
            initIteration = initIteration * 1 + minIteration;
            lastIteration = lastIteration * 1 + minIteration;
            if (0 < (*(this->inputsTPParent->s_darts5226))->boxes->nLocalBoxes) {
                initIteration = min(initIteration, lastIteration);
                lastIteration = max(initIteration, lastIteration);
            } else {
                initIteration = max(initIteration, lastIteration);
                lastIteration = min(initIteration, lastIteration);
            }
            if (idx == myTP->TPsToUse5228 - 1) {
                lastIteration = (*(this->inputsTPParent->s_darts5226))->boxes->nLocalBoxes;
            }
#if USEINVOKE == 1
            invoke<TP5228>(myTP, myTP->codeletsPerTP5228 * DARTS_CODELETS_MULT, this->getID(), myTP,
                initIteration, lastIteration, &(*(this->inputsTPParent->s_darts5226)),
                &(*(this->inputsTPParent->temperature_darts5226)), &(myTP->TP5228Ptr[idx]));
#else
            place<TP5228>(idx, myTP, myTP->codeletsPerTP5228 * DARTS_CODELETS_MULT, this->getID(),
                myTP, initIteration, lastIteration, &(*(this->inputsTPParent->s_darts5226)),
                &(*(this->inputsTPParent->temperature_darts5226)), &(myTP->TP5228Ptr[idx]));
#endif
        } else {
            if (myTP->TP5228Ptr[idx] != nullptr) {
                myTP->TP5228Ptr[idx]->dispatchCodelet(this->getID());
            } else {
                this->resetCodelet();
                this->decDep();
            }
        }
    }
}
void TP5226::_barrierCodelets5228::fire(void)
{
    TP5226* myTP = static_cast<TP5226*>(myTP_);
    myTP->TPParent->barrierCodelets5226[0].setDep(0);
    myTP->add(&(myTP->TPParent->barrierCodelets5226[0]));
}
TP5226::TP5226(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet, SimFlat** in_s,
    real_t* in_temperature)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , s_darts5226(in_s) /*OMP_SHARED - INPUT*/
    , temperature_darts5226(in_temperature) /*OMP_SHARED - INPUT*/
    , TP5228Ptr(new TP5228*[NUMTPS5228])
    , TP5228_alreadyLaunched(new size_t[NUMTPS5228])
    , numTPsSet5228(0)
    , numTPsReady5228(0)
    , TPsToUse5228(NUMTPS5228)
    , codeletsPerTP5228(this->numThreads / NUMTPS5228)
    , totalCodelets5228(this->TPsToUse5228 * this->codeletsPerTP5228)
    , barrierCodelets5226(new _barrierCodelets5226[1])
    , checkInCodelets5228(new _checkInCodelets5228[this->numThreads])
    , barrierCodelets5228(new _barrierCodelets5228[1])
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets5226[0] = _barrierCodelets5226(ompNumThreads, ompNumThreads, this, 0);
    barrierCodelets5228[0] = _barrierCodelets5228(NUMTPS5228, NUMTPS5228, this, 0);
    _checkInCodelets5228* checkInCodelets5228Ptr = (this->checkInCodelets5228);
    for (int i = 0; i < NUMTPS5228; i++) {
        TP5228Ptr[i] = nullptr;
        TP5228_alreadyLaunched[i] = 0;
    }
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets5228Ptr) = _checkInCodelets5228(1, 1, this, codeletCounter);
        (*checkInCodelets5228Ptr).decDep();
        checkInCodelets5228Ptr++;
    }
}
TP5226::~TP5226()
{
    delete[] barrierCodelets5226;
    delete[] barrierCodelets5228;
    delete[] checkInCodelets5228;
}
/*TP5228: OMPForDirective*/
void TP5228::_barrierCodelets5228::fire(void)
{
    TP5228* myTP = static_cast<TP5228*>(myTP_);
    myTP->controlTPParent->TPParent->barrierCodelets5228[0].decDep();
}
bool TP5228::requestNewRangeIterations5228(int* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Static */
    /*Chunk = 0*/
    bool isThereNewIteration = false;
    {
        /*Static Scheduling*/
        int tempStartRange = rangePerCodelet5228 * codeletID;
        int tempEndRange = rangePerCodelet5228 * (codeletID + 1);
        if (remainderRange5228 != 0) {
            if (codeletID < (uint32_t)remainderRange5228) {
                tempStartRange += codeletID;
                tempEndRange += (codeletID + 1);
            } else {
                tempStartRange += remainderRange5228;
                tempEndRange += remainderRange5228;
            }
        }
        tempStartRange = tempStartRange * 1 + minIteration5228;
        tempEndRange = tempEndRange * 1 + minIteration5228;
        if (tempStartRange != tempEndRange) {
            isThereNewIteration = true;
        }
        if (initIteration5228 < lastIteration5228) {
            (this->inputsTPParent->iBox_darts5228[codeletID]) = min(tempStartRange, tempEndRange);
            *endRange = max(tempStartRange, tempEndRange);
        } else {
            (this->inputsTPParent->iBox_darts5228[codeletID]) = max(tempStartRange, tempEndRange);
            *endRange = min(tempStartRange, tempEndRange);
        }
        if (codeletID == this->numThreads - 1) {
            *endRange = lastIteration5228;
        }
    }
    return isThereNewIteration;
}
void TP5228::_checkInCodelets5229::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    /*Init the vars for this region*/

    /*printing node 5229: ForStmt*/
    /*var: s*/
    /*var: temperature*/
    SimFlat** s = (this->inputsTPParent->s_darts5228);
    (void)s /*OMP_SHARED*/;
    real_t* temperature = (this->inputsTPParent->temperature_darts5228);
    (void)temperature /*OMP_SHARED*/;
    int* iBox = &(this->inputsTPParent->iBox_darts5228[this->getLocalID()]);
    (void)iBox /*PRIVATE*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations5228(
        (int*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        /*Signaling omp for stmt's barrier*/
        myTP->controlTPParent->barrierCodelets5228[0].decDep();
        return;
    }
    for (int iBox_darts_counter_temp5228 = (*iBox); iBox_darts_counter_temp5228 < endRange
         && iBox_darts_counter_temp5228 < this->inputsTPParent->lastIteration5228;
         ++iBox_darts_counter_temp5228) {
        {
            {
                /*Loop's init*/
                int iOff = 64 * (iBox_darts_counter_temp5228);
                int ii = 0;
                for (; ii < (*(s))->boxes->nAtoms[(iBox_darts_counter_temp5228)]; ++ii, ++iOff) {
                    int iType = (*(s))->atoms->iSpecies[iOff];
                    real_t mass = (*(s))->species[iType].mass;
                    real_t sigma = sqrt(kB_eV * (*(temperature)) / mass);
                    uint64_t seed = mkSeed((*(s))->atoms->gid[iOff], 123);
                    (*(s))->atoms->p[iOff][0] = mass * sigma * gasdev(&seed);
                    (*(s))->atoms->p[iOff][1] = mass * sigma * gasdev(&seed);
                    (*(s))->atoms->p[iOff][2] = mass * sigma * gasdev(&seed);
                }
            }
        }
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling omp for stmt's barrier*/
    myTP->controlTPParent->barrierCodelets5228[0].decDep();
}
TP5228::TP5228(int in_numThreads, int in_mainCodeletID, TP5226* in_TPParent, int in_initIteration,
    int in_lastIteration, SimFlat** in_s, real_t* in_temperature, TP5228** in_ptrToThisTP)
    : ompTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , s_darts5228(in_s) /*OMP_SHARED - INPUT*/
    , temperature_darts5228(in_temperature) /*OMP_SHARED - INPUT*/
    , iBox_darts5228(new int[this->numThreads]) /*VARIABLE*/
    , initIteration5228(in_initIteration)
    , lastIteration5228(in_lastIteration)
    , readyCodelets(0)
    , baseNumThreads(this->numThreads / DARTS_CODELETS_MULT)
    , barrierCodelets5228(new _barrierCodelets5228[1])
    , checkInCodelets5229(new _checkInCodelets5229[this->numThreads])
{
    /*Initialize the loop parameters*/
    range5228 = abs(lastIteration5228 - initIteration5228) / 1;
    rangePerCodelet5228 = range5228 / numThreads;
    minIteration5228 = min<int>(lastIteration5228, initIteration5228);
    remainderRange5228 = range5228 % numThreads;
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets5228[0] = _barrierCodelets5228(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets5229* checkInCodelets5229Ptr = (this->checkInCodelets5229);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets5229);
#endif
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->baseNumThreads;
         codeletCounter++) {
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets5229Ptr) = _checkInCodelets5229(2, 1, this, codeletCounter);
#else
        (*checkInCodelets5229Ptr) = _checkInCodelets5229(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets5229Ptr).decDep();
        checkInCodelets5229Ptr++;
    }
    this->dispatchCodelet(this->getID());
    *(in_ptrToThisTP) = this;
}
void TP5228::dispatchCodelet(size_t codeletID)
{
    int idx = codeletID / this->baseNumThreads;
    int localID = codeletID - this->baseNumThreads * idx;
    this->checkInCodelets5229[localID].setID(codeletID);
    this->checkInCodelets5229[localID].setLocalID(localID);
    /*Check if we want to replicate codelets*/
    for (size_t i = 1; i < (size_t)DARTS_CODELETS_MULT; i++) {
#if USE_SPIN_CODELETS == 0
        this->checkInCodelets5229[localID + this->baseNumThreads * i]
            = _checkInCodelets5229(2, 1, this, localID + this->baseNumThreads * i);
#else
        this->checkInCodelets5229[localID + this->baseNumThreads * i]
            = _checkInCodelets5229(1, 1, this, localID + this->baseNumThreads * i);
#endif
        this->checkInCodelets5229[localID + this->baseNumThreads * i].setID(codeletID);
        this->checkInCodelets5229[localID + this->baseNumThreads * i].decDep();
#if USE_SPIN_CODELETS == 0
        this->firstCodelet[localID + this->baseNumThreads * i].decDep();
#else
        this->availableCodelets[localID + this->baseNumThreads * i] = 1;
#endif
    }
#if USE_SPIN_CODELETS == 0
    this->firstCodelet[localID].decDep();
#else
    this->availableCodelets[localID] = 1;
#endif
}
TP5228::~TP5228()
{
    delete[] iBox_darts5228;
    delete[] barrierCodelets5228;
    delete[] checkInCodelets5229;
}
/*TP5340: OMPParallelDirective*/
void TP5340::_barrierCodelets5340::fire(void)
{
    TP5340* myTP = static_cast<TP5340*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
void TP5340::_checkInCodelets5342::fire(void)
{
    /*printOMPForCodeletFireCode, inlined: 0*/
    /*region 5342 0*/
    /*Determine the TP to which this codelet belongs and check if this codelet spawns the TP or if
     * it signals it using dispatchCodelet()*/
    size_t idx = this->getID() / myTP->codeletsPerTP5342;
    if (idx < myTP->TPsToUse5342) {
        if (!__sync_val_compare_and_swap(&(myTP->TP5342_alreadyLaunched[idx]), 0, 1)) {
            int range = abs((*(this->inputsTPParent->s_darts5340))->boxes->nLocalBoxes - 0) / 1;
            int rangePerCodelet = range / myTP->TPsToUse5342;
            int minIteration
                = min<int>((*(this->inputsTPParent->s_darts5340))->boxes->nLocalBoxes, 0);
            int remainderRange = range % myTP->TPsToUse5342;
            int initIteration = rangePerCodelet * idx;
            int lastIteration = rangePerCodelet * (idx + 1);
            if (remainderRange != 0) {
                if (idx < (uint32_t)remainderRange) {
                    initIteration += idx;
                    lastIteration += (idx + 1);
                } else {
                    initIteration += remainderRange;
                    lastIteration += remainderRange;
                }
            }
            initIteration = initIteration * 1 + minIteration;
            lastIteration = lastIteration * 1 + minIteration;
            if (0 < (*(this->inputsTPParent->s_darts5340))->boxes->nLocalBoxes) {
                initIteration = min(initIteration, lastIteration);
                lastIteration = max(initIteration, lastIteration);
            } else {
                initIteration = max(initIteration, lastIteration);
                lastIteration = min(initIteration, lastIteration);
            }
            if (idx == myTP->TPsToUse5342 - 1) {
                lastIteration = (*(this->inputsTPParent->s_darts5340))->boxes->nLocalBoxes;
            }
#if USEINVOKE == 1
            invoke<TP5342>(myTP, myTP->codeletsPerTP5342 * DARTS_CODELETS_MULT, this->getID(), myTP,
                initIteration, lastIteration, &(*(this->inputsTPParent->s_darts5340)),
                &(*(this->inputsTPParent->scaleFactor_darts5340)), &(myTP->TP5342Ptr[idx]));
#else
            place<TP5342>(idx, myTP, myTP->codeletsPerTP5342 * DARTS_CODELETS_MULT, this->getID(),
                myTP, initIteration, lastIteration, &(*(this->inputsTPParent->s_darts5340)),
                &(*(this->inputsTPParent->scaleFactor_darts5340)), &(myTP->TP5342Ptr[idx]));
#endif
        } else {
            if (myTP->TP5342Ptr[idx] != nullptr) {
                myTP->TP5342Ptr[idx]->dispatchCodelet(this->getID());
            } else {
                this->resetCodelet();
                this->decDep();
            }
        }
    }
}
void TP5340::_barrierCodelets5342::fire(void)
{
    TP5340* myTP = static_cast<TP5340*>(myTP_);
    myTP->TPParent->barrierCodelets5340[0].setDep(0);
    myTP->add(&(myTP->TPParent->barrierCodelets5340[0]));
}
TP5340::TP5340(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet, SimFlat** in_s,
    real_t* in_scaleFactor)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , s_darts5340(in_s) /*OMP_SHARED - INPUT*/
    , scaleFactor_darts5340(in_scaleFactor) /*OMP_SHARED - INPUT*/
    , TP5342Ptr(new TP5342*[NUMTPS5342])
    , TP5342_alreadyLaunched(new size_t[NUMTPS5342])
    , numTPsSet5342(0)
    , numTPsReady5342(0)
    , TPsToUse5342(NUMTPS5342)
    , codeletsPerTP5342(this->numThreads / NUMTPS5342)
    , totalCodelets5342(this->TPsToUse5342 * this->codeletsPerTP5342)
    , barrierCodelets5340(new _barrierCodelets5340[1])
    , checkInCodelets5342(new _checkInCodelets5342[this->numThreads])
    , barrierCodelets5342(new _barrierCodelets5342[1])
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets5340[0] = _barrierCodelets5340(ompNumThreads, ompNumThreads, this, 0);
    barrierCodelets5342[0] = _barrierCodelets5342(NUMTPS5342, NUMTPS5342, this, 0);
    _checkInCodelets5342* checkInCodelets5342Ptr = (this->checkInCodelets5342);
    for (int i = 0; i < NUMTPS5342; i++) {
        TP5342Ptr[i] = nullptr;
        TP5342_alreadyLaunched[i] = 0;
    }
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets5342Ptr) = _checkInCodelets5342(1, 1, this, codeletCounter);
        (*checkInCodelets5342Ptr).decDep();
        checkInCodelets5342Ptr++;
    }
}
TP5340::~TP5340()
{
    delete[] barrierCodelets5340;
    delete[] barrierCodelets5342;
    delete[] checkInCodelets5342;
}
/*TP5342: OMPForDirective*/
void TP5342::_barrierCodelets5342::fire(void)
{
    TP5342* myTP = static_cast<TP5342*>(myTP_);
    myTP->controlTPParent->TPParent->barrierCodelets5342[0].decDep();
}
bool TP5342::requestNewRangeIterations5342(int* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Static */
    /*Chunk = 0*/
    bool isThereNewIteration = false;
    {
        /*Static Scheduling*/
        int tempStartRange = rangePerCodelet5342 * codeletID;
        int tempEndRange = rangePerCodelet5342 * (codeletID + 1);
        if (remainderRange5342 != 0) {
            if (codeletID < (uint32_t)remainderRange5342) {
                tempStartRange += codeletID;
                tempEndRange += (codeletID + 1);
            } else {
                tempStartRange += remainderRange5342;
                tempEndRange += remainderRange5342;
            }
        }
        tempStartRange = tempStartRange * 1 + minIteration5342;
        tempEndRange = tempEndRange * 1 + minIteration5342;
        if (tempStartRange != tempEndRange) {
            isThereNewIteration = true;
        }
        if (initIteration5342 < lastIteration5342) {
            (this->inputsTPParent->iBox_darts5342[codeletID]) = min(tempStartRange, tempEndRange);
            *endRange = max(tempStartRange, tempEndRange);
        } else {
            (this->inputsTPParent->iBox_darts5342[codeletID]) = max(tempStartRange, tempEndRange);
            *endRange = min(tempStartRange, tempEndRange);
        }
        if (codeletID == this->numThreads - 1) {
            *endRange = lastIteration5342;
        }
    }
    return isThereNewIteration;
}
void TP5342::_checkInCodelets5343::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    /*Init the vars for this region*/

    /*printing node 5343: ForStmt*/
    /*var: s*/
    /*var: scaleFactor*/
    SimFlat** s = (this->inputsTPParent->s_darts5342);
    (void)s /*OMP_SHARED*/;
    real_t* scaleFactor = (this->inputsTPParent->scaleFactor_darts5342);
    (void)scaleFactor /*OMP_SHARED*/;
    int* iBox = &(this->inputsTPParent->iBox_darts5342[this->getLocalID()]);
    (void)iBox /*PRIVATE*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations5342(
        (int*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        /*Signaling omp for stmt's barrier*/
        myTP->controlTPParent->barrierCodelets5342[0].decDep();
        return;
    }
    for (int iBox_darts_counter_temp5342 = (*iBox); iBox_darts_counter_temp5342 < endRange
         && iBox_darts_counter_temp5342 < this->inputsTPParent->lastIteration5342;
         ++iBox_darts_counter_temp5342) {
        {
            {
                /*Loop's init*/
                int iOff = 64 * (iBox_darts_counter_temp5342);
                int ii = 0;
                for (; ii < (*(s))->boxes->nAtoms[(iBox_darts_counter_temp5342)]; ++ii, ++iOff) {
                    (*(s))->atoms->p[iOff][0] *= (*(scaleFactor));
                    (*(s))->atoms->p[iOff][1] *= (*(scaleFactor));
                    (*(s))->atoms->p[iOff][2] *= (*(scaleFactor));
                }
            }
        }
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling omp for stmt's barrier*/
    myTP->controlTPParent->barrierCodelets5342[0].decDep();
}
TP5342::TP5342(int in_numThreads, int in_mainCodeletID, TP5340* in_TPParent, int in_initIteration,
    int in_lastIteration, SimFlat** in_s, real_t* in_scaleFactor, TP5342** in_ptrToThisTP)
    : ompTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , s_darts5342(in_s) /*OMP_SHARED - INPUT*/
    , scaleFactor_darts5342(in_scaleFactor) /*OMP_SHARED - INPUT*/
    , iBox_darts5342(new int[this->numThreads]) /*VARIABLE*/
    , initIteration5342(in_initIteration)
    , lastIteration5342(in_lastIteration)
    , readyCodelets(0)
    , baseNumThreads(this->numThreads / DARTS_CODELETS_MULT)
    , barrierCodelets5342(new _barrierCodelets5342[1])
    , checkInCodelets5343(new _checkInCodelets5343[this->numThreads])
{
    /*Initialize the loop parameters*/
    range5342 = abs(lastIteration5342 - initIteration5342) / 1;
    rangePerCodelet5342 = range5342 / numThreads;
    minIteration5342 = min<int>(lastIteration5342, initIteration5342);
    remainderRange5342 = range5342 % numThreads;
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets5342[0] = _barrierCodelets5342(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets5343* checkInCodelets5343Ptr = (this->checkInCodelets5343);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets5343);
#endif
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->baseNumThreads;
         codeletCounter++) {
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets5343Ptr) = _checkInCodelets5343(2, 1, this, codeletCounter);
#else
        (*checkInCodelets5343Ptr) = _checkInCodelets5343(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets5343Ptr).decDep();
        checkInCodelets5343Ptr++;
    }
    this->dispatchCodelet(this->getID());
    *(in_ptrToThisTP) = this;
}
void TP5342::dispatchCodelet(size_t codeletID)
{
    int idx = codeletID / this->baseNumThreads;
    int localID = codeletID - this->baseNumThreads * idx;
    this->checkInCodelets5343[localID].setID(codeletID);
    this->checkInCodelets5343[localID].setLocalID(localID);
    /*Check if we want to replicate codelets*/
    for (size_t i = 1; i < (size_t)DARTS_CODELETS_MULT; i++) {
#if USE_SPIN_CODELETS == 0
        this->checkInCodelets5343[localID + this->baseNumThreads * i]
            = _checkInCodelets5343(2, 1, this, localID + this->baseNumThreads * i);
#else
        this->checkInCodelets5343[localID + this->baseNumThreads * i]
            = _checkInCodelets5343(1, 1, this, localID + this->baseNumThreads * i);
#endif
        this->checkInCodelets5343[localID + this->baseNumThreads * i].setID(codeletID);
        this->checkInCodelets5343[localID + this->baseNumThreads * i].decDep();
#if USE_SPIN_CODELETS == 0
        this->firstCodelet[localID + this->baseNumThreads * i].decDep();
#else
        this->availableCodelets[localID + this->baseNumThreads * i] = 1;
#endif
    }
#if USE_SPIN_CODELETS == 0
    this->firstCodelet[localID].decDep();
#else
    this->availableCodelets[localID] = 1;
#endif
}
TP5342::~TP5342()
{
    delete[] iBox_darts5342;
    delete[] barrierCodelets5342;
    delete[] checkInCodelets5343;
}
/*TP5410: OMPParallelDirective*/
void TP5410::_barrierCodelets5410::fire(void)
{
    TP5410* myTP = static_cast<TP5410*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
void TP5410::_checkInCodelets5412::fire(void)
{
    /*printOMPForCodeletFireCode, inlined: 0*/
    /*region 5412 0*/
    /*Determine the TP to which this codelet belongs and check if this codelet spawns the TP or if
     * it signals it using dispatchCodelet()*/
    size_t idx = this->getID() / myTP->codeletsPerTP5412;
    if (idx < myTP->TPsToUse5412) {
        if (!__sync_val_compare_and_swap(&(myTP->TP5412_alreadyLaunched[idx]), 0, 1)) {
            int range = abs((*(this->inputsTPParent->s_darts5410))->boxes->nLocalBoxes - 0) / 1;
            int rangePerCodelet = range / myTP->TPsToUse5412;
            int minIteration
                = min<int>((*(this->inputsTPParent->s_darts5410))->boxes->nLocalBoxes, 0);
            int remainderRange = range % myTP->TPsToUse5412;
            int initIteration = rangePerCodelet * idx;
            int lastIteration = rangePerCodelet * (idx + 1);
            if (remainderRange != 0) {
                if (idx < (uint32_t)remainderRange) {
                    initIteration += idx;
                    lastIteration += (idx + 1);
                } else {
                    initIteration += remainderRange;
                    lastIteration += remainderRange;
                }
            }
            initIteration = initIteration * 1 + minIteration;
            lastIteration = lastIteration * 1 + minIteration;
            if (0 < (*(this->inputsTPParent->s_darts5410))->boxes->nLocalBoxes) {
                initIteration = min(initIteration, lastIteration);
                lastIteration = max(initIteration, lastIteration);
            } else {
                initIteration = max(initIteration, lastIteration);
                lastIteration = min(initIteration, lastIteration);
            }
            if (idx == myTP->TPsToUse5412 - 1) {
                lastIteration = (*(this->inputsTPParent->s_darts5410))->boxes->nLocalBoxes;
            }
#if USEINVOKE == 1
            invoke<TP5412>(myTP, myTP->codeletsPerTP5412 * DARTS_CODELETS_MULT, this->getID(), myTP,
                initIteration, lastIteration, &(*(this->inputsTPParent->delta_darts5410)),
                &(*(this->inputsTPParent->s_darts5410)), &(myTP->TP5412Ptr[idx]));
#else
            place<TP5412>(idx, myTP, myTP->codeletsPerTP5412 * DARTS_CODELETS_MULT, this->getID(),
                myTP, initIteration, lastIteration, &(*(this->inputsTPParent->delta_darts5410)),
                &(*(this->inputsTPParent->s_darts5410)), &(myTP->TP5412Ptr[idx]));
#endif
        } else {
            if (myTP->TP5412Ptr[idx] != nullptr) {
                myTP->TP5412Ptr[idx]->dispatchCodelet(this->getID());
            } else {
                this->resetCodelet();
                this->decDep();
            }
        }
    }
}
void TP5410::_barrierCodelets5412::fire(void)
{
    TP5410* myTP = static_cast<TP5410*>(myTP_);
    myTP->TPParent->barrierCodelets5410[0].setDep(0);
    myTP->add(&(myTP->TPParent->barrierCodelets5410[0]));
}
TP5410::TP5410(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet, real_t* in_delta,
    SimFlat** in_s)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , delta_darts5410(in_delta) /*OMP_SHARED - INPUT*/
    , s_darts5410(in_s) /*OMP_SHARED - INPUT*/
    , TP5412Ptr(new TP5412*[NUMTPS5412])
    , TP5412_alreadyLaunched(new size_t[NUMTPS5412])
    , numTPsSet5412(0)
    , numTPsReady5412(0)
    , TPsToUse5412(NUMTPS5412)
    , codeletsPerTP5412(this->numThreads / NUMTPS5412)
    , totalCodelets5412(this->TPsToUse5412 * this->codeletsPerTP5412)
    , barrierCodelets5410(new _barrierCodelets5410[1])
    , checkInCodelets5412(new _checkInCodelets5412[this->numThreads])
    , barrierCodelets5412(new _barrierCodelets5412[1])
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets5410[0] = _barrierCodelets5410(ompNumThreads, ompNumThreads, this, 0);
    barrierCodelets5412[0] = _barrierCodelets5412(NUMTPS5412, NUMTPS5412, this, 0);
    _checkInCodelets5412* checkInCodelets5412Ptr = (this->checkInCodelets5412);
    for (int i = 0; i < NUMTPS5412; i++) {
        TP5412Ptr[i] = nullptr;
        TP5412_alreadyLaunched[i] = 0;
    }
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets5412Ptr) = _checkInCodelets5412(1, 1, this, codeletCounter);
        (*checkInCodelets5412Ptr).decDep();
        checkInCodelets5412Ptr++;
    }
}
TP5410::~TP5410()
{
    delete[] barrierCodelets5410;
    delete[] barrierCodelets5412;
    delete[] checkInCodelets5412;
}
/*TP5412: OMPForDirective*/
void TP5412::_barrierCodelets5412::fire(void)
{
    TP5412* myTP = static_cast<TP5412*>(myTP_);
    myTP->controlTPParent->TPParent->barrierCodelets5412[0].decDep();
}
bool TP5412::requestNewRangeIterations5412(int* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Static */
    /*Chunk = 0*/
    bool isThereNewIteration = false;
    {
        /*Static Scheduling*/
        int tempStartRange = rangePerCodelet5412 * codeletID;
        int tempEndRange = rangePerCodelet5412 * (codeletID + 1);
        if (remainderRange5412 != 0) {
            if (codeletID < (uint32_t)remainderRange5412) {
                tempStartRange += codeletID;
                tempEndRange += (codeletID + 1);
            } else {
                tempStartRange += remainderRange5412;
                tempEndRange += remainderRange5412;
            }
        }
        tempStartRange = tempStartRange * 1 + minIteration5412;
        tempEndRange = tempEndRange * 1 + minIteration5412;
        if (tempStartRange != tempEndRange) {
            isThereNewIteration = true;
        }
        if (initIteration5412 < lastIteration5412) {
            (this->inputsTPParent->iBox_darts5412[codeletID]) = min(tempStartRange, tempEndRange);
            *endRange = max(tempStartRange, tempEndRange);
        } else {
            (this->inputsTPParent->iBox_darts5412[codeletID]) = max(tempStartRange, tempEndRange);
            *endRange = min(tempStartRange, tempEndRange);
        }
        if (codeletID == this->numThreads - 1) {
            *endRange = lastIteration5412;
        }
    }
    return isThereNewIteration;
}
void TP5412::_checkInCodelets5413::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    /*Init the vars for this region*/

    /*printing node 5413: ForStmt*/
    /*var: delta*/
    /*var: s*/
    real_t* delta = (this->inputsTPParent->delta_darts5412);
    (void)delta /*OMP_SHARED*/;
    SimFlat** s = (this->inputsTPParent->s_darts5412);
    (void)s /*OMP_SHARED*/;
    int* iBox = &(this->inputsTPParent->iBox_darts5412[this->getLocalID()]);
    (void)iBox /*PRIVATE*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations5412(
        (int*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        /*Signaling omp for stmt's barrier*/
        myTP->controlTPParent->barrierCodelets5412[0].decDep();
        return;
    }
    for (int iBox_darts_counter_temp5412 = (*iBox); iBox_darts_counter_temp5412 < endRange
         && iBox_darts_counter_temp5412 < this->inputsTPParent->lastIteration5412;
         ++iBox_darts_counter_temp5412) {
        {
            {
                /*Loop's init*/
                int iOff = 64 * (iBox_darts_counter_temp5412);
                int ii = 0;
                for (; ii < (*(s))->boxes->nAtoms[(iBox_darts_counter_temp5412)]; ++ii, ++iOff) {
                    uint64_t seed = mkSeed((*(s))->atoms->gid[iOff], 457);
                    (*(s))->atoms->r[iOff][0] += (2. * lcg61(&seed) - 1.) * (*(delta));
                    (*(s))->atoms->r[iOff][1] += (2. * lcg61(&seed) - 1.) * (*(delta));
                    (*(s))->atoms->r[iOff][2] += (2. * lcg61(&seed) - 1.) * (*(delta));
                }
            }
        }
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling omp for stmt's barrier*/
    myTP->controlTPParent->barrierCodelets5412[0].decDep();
}
TP5412::TP5412(int in_numThreads, int in_mainCodeletID, TP5410* in_TPParent, int in_initIteration,
    int in_lastIteration, real_t* in_delta, SimFlat** in_s, TP5412** in_ptrToThisTP)
    : ompTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , delta_darts5412(in_delta) /*OMP_SHARED - INPUT*/
    , s_darts5412(in_s) /*OMP_SHARED - INPUT*/
    , iBox_darts5412(new int[this->numThreads]) /*VARIABLE*/
    , initIteration5412(in_initIteration)
    , lastIteration5412(in_lastIteration)
    , readyCodelets(0)
    , baseNumThreads(this->numThreads / DARTS_CODELETS_MULT)
    , barrierCodelets5412(new _barrierCodelets5412[1])
    , checkInCodelets5413(new _checkInCodelets5413[this->numThreads])
{
    /*Initialize the loop parameters*/
    range5412 = abs(lastIteration5412 - initIteration5412) / 1;
    rangePerCodelet5412 = range5412 / numThreads;
    minIteration5412 = min<int>(lastIteration5412, initIteration5412);
    remainderRange5412 = range5412 % numThreads;
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets5412[0] = _barrierCodelets5412(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets5413* checkInCodelets5413Ptr = (this->checkInCodelets5413);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets5413);
#endif
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->baseNumThreads;
         codeletCounter++) {
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets5413Ptr) = _checkInCodelets5413(2, 1, this, codeletCounter);
#else
        (*checkInCodelets5413Ptr) = _checkInCodelets5413(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets5413Ptr).decDep();
        checkInCodelets5413Ptr++;
    }
    this->dispatchCodelet(this->getID());
    *(in_ptrToThisTP) = this;
}
void TP5412::dispatchCodelet(size_t codeletID)
{
    int idx = codeletID / this->baseNumThreads;
    int localID = codeletID - this->baseNumThreads * idx;
    this->checkInCodelets5413[localID].setID(codeletID);
    this->checkInCodelets5413[localID].setLocalID(localID);
    /*Check if we want to replicate codelets*/
    for (size_t i = 1; i < (size_t)DARTS_CODELETS_MULT; i++) {
#if USE_SPIN_CODELETS == 0
        this->checkInCodelets5413[localID + this->baseNumThreads * i]
            = _checkInCodelets5413(2, 1, this, localID + this->baseNumThreads * i);
#else
        this->checkInCodelets5413[localID + this->baseNumThreads * i]
            = _checkInCodelets5413(1, 1, this, localID + this->baseNumThreads * i);
#endif
        this->checkInCodelets5413[localID + this->baseNumThreads * i].setID(codeletID);
        this->checkInCodelets5413[localID + this->baseNumThreads * i].decDep();
#if USE_SPIN_CODELETS == 0
        this->firstCodelet[localID + this->baseNumThreads * i].decDep();
#else
        this->availableCodelets[localID + this->baseNumThreads * i] = 1;
#endif
    }
#if USE_SPIN_CODELETS == 0
    this->firstCodelet[localID].decDep();
#else
    this->availableCodelets[localID] = 1;
#endif
}
TP5412::~TP5412()
{
    delete[] iBox_darts5412;
    delete[] barrierCodelets5412;
    delete[] checkInCodelets5413;
}
/*TP5525: OMPParallelDirective*/
void TP5525::_barrierCodelets5525::fire(void)
{
    TP5525* myTP = static_cast<TP5525*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
void TP5525::_checkInCodelets5527::fire(void)
{
    /*printOMPForCodeletFireCode, inlined: 0*/
    /*region 5527 0*/
    /*Determine the TP to which this codelet belongs and check if this codelet spawns the TP or if
     * it signals it using dispatchCodelet()*/
    size_t idx = this->getID() / myTP->codeletsPerTP5527;
    if (idx < myTP->TPsToUse5527) {
        if (!__sync_val_compare_and_swap(&(myTP->TP5527_alreadyLaunched[idx]), 0, 1)) {
            int range = abs((*(this->inputsTPParent->s_darts5525))->boxes->nLocalBoxes - 0) / 1;
            int rangePerCodelet = range / myTP->TPsToUse5527;
            int minIteration
                = min<int>((*(this->inputsTPParent->s_darts5525))->boxes->nLocalBoxes, 0);
            int remainderRange = range % myTP->TPsToUse5527;
            int initIteration = rangePerCodelet * idx;
            int lastIteration = rangePerCodelet * (idx + 1);
            if (remainderRange != 0) {
                if (idx < (uint32_t)remainderRange) {
                    initIteration += idx;
                    lastIteration += (idx + 1);
                } else {
                    initIteration += remainderRange;
                    lastIteration += remainderRange;
                }
            }
            initIteration = initIteration * 1 + minIteration;
            lastIteration = lastIteration * 1 + minIteration;
            if (0 < (*(this->inputsTPParent->s_darts5525))->boxes->nLocalBoxes) {
                initIteration = min(initIteration, lastIteration);
                lastIteration = max(initIteration, lastIteration);
            } else {
                initIteration = max(initIteration, lastIteration);
                lastIteration = min(initIteration, lastIteration);
            }
            if (idx == myTP->TPsToUse5527 - 1) {
                lastIteration = (*(this->inputsTPParent->s_darts5525))->boxes->nLocalBoxes;
            }
#if USEINVOKE == 1
            invoke<TP5527>(myTP, myTP->codeletsPerTP5527 * DARTS_CODELETS_MULT, this->getID(), myTP,
                initIteration, lastIteration, &(*(this->inputsTPParent->s_darts5525)),
                &(*(this->inputsTPParent->v0_darts5525)), &(*(this->inputsTPParent->v1_darts5525)),
                &(*(this->inputsTPParent->v2_darts5525)), &(*(this->inputsTPParent->v3_darts5525)),
                &(myTP->TP5527Ptr[idx]));
#else
            place<TP5527>(idx, myTP, myTP->codeletsPerTP5527 * DARTS_CODELETS_MULT, this->getID(),
                myTP, initIteration, lastIteration, &(*(this->inputsTPParent->s_darts5525)),
                &(*(this->inputsTPParent->v0_darts5525)), &(*(this->inputsTPParent->v1_darts5525)),
                &(*(this->inputsTPParent->v2_darts5525)), &(*(this->inputsTPParent->v3_darts5525)),
                &(myTP->TP5527Ptr[idx]));
#endif
        } else {
            if (myTP->TP5527Ptr[idx] != nullptr) {
                myTP->TP5527Ptr[idx]->dispatchCodelet(this->getID());
            } else {
                this->resetCodelet();
                this->decDep();
            }
        }
    }
}
void TP5525::_barrierCodelets5527::fire(void)
{
    TP5525* myTP = static_cast<TP5525*>(myTP_);
    myTP->TPParent->barrierCodelets5525[0].setDep(0);
    myTP->add(&(myTP->TPParent->barrierCodelets5525[0]));
}
TP5525::TP5525(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet, SimFlat** in_s,
    real_t* in_v0, real_t* in_v1, real_t* in_v2, real_t* in_v3)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , s_darts5525(in_s) /*OMP_SHARED - INPUT*/
    , v0_darts5525(in_v0) /*OMP_SHARED - INPUT*/
    , v1_darts5525(in_v1) /*OMP_SHARED - INPUT*/
    , v2_darts5525(in_v2) /*OMP_SHARED - INPUT*/
    , v3_darts5525(in_v3) /*OMP_SHARED - INPUT*/
    , TP5527Ptr(new TP5527*[NUMTPS5527])
    , TP5527_alreadyLaunched(new size_t[NUMTPS5527])
    , numTPsSet5527(0)
    , numTPsReady5527(0)
    , TPsToUse5527(NUMTPS5527)
    , codeletsPerTP5527(this->numThreads / NUMTPS5527)
    , totalCodelets5527(this->TPsToUse5527 * this->codeletsPerTP5527)
    , barrierCodelets5525(new _barrierCodelets5525[1])
    , checkInCodelets5527(new _checkInCodelets5527[this->numThreads])
    , barrierCodelets5527(new _barrierCodelets5527[1])
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets5525[0] = _barrierCodelets5525(ompNumThreads, ompNumThreads, this, 0);
    barrierCodelets5527[0] = _barrierCodelets5527(NUMTPS5527, NUMTPS5527, this, 0);
    _checkInCodelets5527* checkInCodelets5527Ptr = (this->checkInCodelets5527);
    for (int i = 0; i < NUMTPS5527; i++) {
        TP5527Ptr[i] = nullptr;
        TP5527_alreadyLaunched[i] = 0;
    }
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets5527Ptr) = _checkInCodelets5527(1, 1, this, codeletCounter);
        (*checkInCodelets5527Ptr).decDep();
        checkInCodelets5527Ptr++;
    }
}
TP5525::~TP5525()
{
    delete[] barrierCodelets5525;
    delete[] barrierCodelets5527;
    delete[] checkInCodelets5527;
}
/*TP5527: OMPForDirective*/
void TP5527::_barrierCodelets5527::fire(void)
{
    TP5527* myTP = static_cast<TP5527*>(myTP_);
    myTP->controlTPParent->TPParent->barrierCodelets5527[0].decDep();
}
bool TP5527::requestNewRangeIterations5527(int* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Static */
    /*Chunk = 0*/
    bool isThereNewIteration = false;
    {
        /*Static Scheduling*/
        int tempStartRange = rangePerCodelet5527 * codeletID;
        int tempEndRange = rangePerCodelet5527 * (codeletID + 1);
        if (remainderRange5527 != 0) {
            if (codeletID < (uint32_t)remainderRange5527) {
                tempStartRange += codeletID;
                tempEndRange += (codeletID + 1);
            } else {
                tempStartRange += remainderRange5527;
                tempEndRange += remainderRange5527;
            }
        }
        tempStartRange = tempStartRange * 1 + minIteration5527;
        tempEndRange = tempEndRange * 1 + minIteration5527;
        if (tempStartRange != tempEndRange) {
            isThereNewIteration = true;
        }
        if (initIteration5527 < lastIteration5527) {
            (this->inputsTPParent->iBox_darts5527[codeletID]) = min(tempStartRange, tempEndRange);
            *endRange = max(tempStartRange, tempEndRange);
        } else {
            (this->inputsTPParent->iBox_darts5527[codeletID]) = max(tempStartRange, tempEndRange);
            *endRange = min(tempStartRange, tempEndRange);
        }
        if (codeletID == this->numThreads - 1) {
            *endRange = lastIteration5527;
        }
    }
    return isThereNewIteration;
}
void TP5527::_checkInCodelets5528::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    /*Init the vars for this region*/

    /*printing node 5528: ForStmt*/
    /*var: s*/
    /*var: v0*/
    /*var: v1*/
    /*var: v2*/
    /*var: v3*/
    /*Declare local reduction vars*/
    real_t v3_LocalReductionVar = (real_t)0;
    real_t v2_LocalReductionVar = (real_t)0;
    real_t v1_LocalReductionVar = (real_t)0;
    real_t v0_LocalReductionVar = (real_t)0;
    SimFlat** s = (this->inputsTPParent->s_darts5527);
    (void)s /*OMP_SHARED*/;
    int* iBox = &(this->inputsTPParent->iBox_darts5527[this->getLocalID()]);
    (void)iBox /*PRIVATE*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations5527(
        (int*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        goto updateReduceVars;
    }
    for (int iBox_darts_counter_temp5527 = (*iBox); iBox_darts_counter_temp5527 < endRange
         && iBox_darts_counter_temp5527 < this->inputsTPParent->lastIteration5527;
         ++iBox_darts_counter_temp5527) {
        {
            {
                /*Loop's init*/
                int iOff = 64 * (iBox_darts_counter_temp5527);
                int ii = 0;
                for (; ii < (*(s))->boxes->nAtoms[(iBox_darts_counter_temp5527)]; ++ii, ++iOff) {
                    (*(&(v0_LocalReductionVar))) += (*(s))->atoms->p[iOff][0];
                    (*(&(v1_LocalReductionVar))) += (*(s))->atoms->p[iOff][1];
                    (*(&(v2_LocalReductionVar))) += (*(s))->atoms->p[iOff][2];
                    int iSpecies = (*(s))->atoms->iSpecies[iOff];
                    (*(&(v3_LocalReductionVar))) += (*(s))->species[iSpecies].mass;
                }
            }
        }
    }
updateReduceVars:
    /*Before signaling next codelet, check if all the threads in the TP
    * performed the reduction locally and if so, update the unique
    * reduction variable.*/
    /*Reducing for operator '+'*/
    this->inputsTPParent->v3_darts5527_mutex->lock();
    *(this->inputsTPParent->v3_darts5527) += (v3_LocalReductionVar);
    this->inputsTPParent->v3_darts5527_mutex->unlock();
    /*Reducing for operator '+'*/
    this->inputsTPParent->v2_darts5527_mutex->lock();
    *(this->inputsTPParent->v2_darts5527) += (v2_LocalReductionVar);
    this->inputsTPParent->v2_darts5527_mutex->unlock();
    /*Reducing for operator '+'*/
    this->inputsTPParent->v1_darts5527_mutex->lock();
    *(this->inputsTPParent->v1_darts5527) += (v1_LocalReductionVar);
    this->inputsTPParent->v1_darts5527_mutex->unlock();
    /*Reducing for operator '+'*/
    this->inputsTPParent->v0_darts5527_mutex->lock();
    *(this->inputsTPParent->v0_darts5527) += (v0_LocalReductionVar);
    this->inputsTPParent->v0_darts5527_mutex->unlock();
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling omp for stmt's barrier*/
    myTP->controlTPParent->barrierCodelets5527[0].decDep();
}
TP5527::TP5527(int in_numThreads, int in_mainCodeletID, TP5525* in_TPParent, int in_initIteration,
    int in_lastIteration, SimFlat** in_s, real_t* in_v0, real_t* in_v1, real_t* in_v2,
    real_t* in_v3, TP5527** in_ptrToThisTP)
    : ompTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , s_darts5527(in_s) /*OMP_SHARED - INPUT*/
    , v0_darts5527(in_v0) /*OMP_SHARED - INPUT*/
    , v1_darts5527(in_v1) /*OMP_SHARED - INPUT*/
    , v2_darts5527(in_v2) /*OMP_SHARED - INPUT*/
    , v3_darts5527(in_v3) /*OMP_SHARED - INPUT*/
    , iBox_darts5527(new int[this->numThreads]) /*VARIABLE*/
    , initIteration5527(in_initIteration)
    , lastIteration5527(in_lastIteration)
    , readyCodelets(0)
    , baseNumThreads(this->numThreads / DARTS_CODELETS_MULT)
    , v3_darts5527_mutex(&(this->TPParent->v3_darts5527_mutex))
    , v2_darts5527_mutex(&(this->TPParent->v2_darts5527_mutex))
    , v1_darts5527_mutex(&(this->TPParent->v1_darts5527_mutex))
    , v0_darts5527_mutex(&(this->TPParent->v0_darts5527_mutex))
    , barrierCodelets5527(new _barrierCodelets5527[1])
    , checkInCodelets5528(new _checkInCodelets5528[this->numThreads])
{
    /*Initialize the loop parameters*/
    range5527 = abs(lastIteration5527 - initIteration5527) / 1;
    rangePerCodelet5527 = range5527 / numThreads;
    minIteration5527 = min<int>(lastIteration5527, initIteration5527);
    remainderRange5527 = range5527 % numThreads;
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets5527[0] = _barrierCodelets5527(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets5528* checkInCodelets5528Ptr = (this->checkInCodelets5528);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets5528);
#endif
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->baseNumThreads;
         codeletCounter++) {
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets5528Ptr) = _checkInCodelets5528(2, 1, this, codeletCounter);
#else
        (*checkInCodelets5528Ptr) = _checkInCodelets5528(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets5528Ptr).decDep();
        checkInCodelets5528Ptr++;
    }
    this->dispatchCodelet(this->getID());
    *(in_ptrToThisTP) = this;
}
void TP5527::dispatchCodelet(size_t codeletID)
{
    int idx = codeletID / this->baseNumThreads;
    int localID = codeletID - this->baseNumThreads * idx;
    this->checkInCodelets5528[localID].setID(codeletID);
    this->checkInCodelets5528[localID].setLocalID(localID);
    /*Check if we want to replicate codelets*/
    for (size_t i = 1; i < (size_t)DARTS_CODELETS_MULT; i++) {
#if USE_SPIN_CODELETS == 0
        this->checkInCodelets5528[localID + this->baseNumThreads * i]
            = _checkInCodelets5528(2, 1, this, localID + this->baseNumThreads * i);
#else
        this->checkInCodelets5528[localID + this->baseNumThreads * i]
            = _checkInCodelets5528(1, 1, this, localID + this->baseNumThreads * i);
#endif
        this->checkInCodelets5528[localID + this->baseNumThreads * i].setID(codeletID);
        this->checkInCodelets5528[localID + this->baseNumThreads * i].decDep();
#if USE_SPIN_CODELETS == 0
        this->firstCodelet[localID + this->baseNumThreads * i].decDep();
#else
        this->availableCodelets[localID + this->baseNumThreads * i] = 1;
#endif
    }
#if USE_SPIN_CODELETS == 0
    this->firstCodelet[localID].decDep();
#else
    this->availableCodelets[localID] = 1;
#endif
}
TP5527::~TP5527()
{
    delete[] iBox_darts5527;
    delete[] barrierCodelets5527;
    delete[] checkInCodelets5528;
}
