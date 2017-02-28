#include "timestep.output.darts.h"
using namespace darts;
using namespace std;
static void advancePosition(SimFlat* s, int nBoxes, real_t dt);
static void advanceVelocity(SimFlat* s, int nBoxes, real_t dt);
/*Function: timestep, ID: 171*/
double timestep(SimFlat* s, int nSteps, real_t dt)
{
    /*timestep:171*/
    /*CompoundStmt:367*/
    for (int ii = 0; ii < nSteps; ++ii) {
        do {
            profileStart(velocityTimer);
        } while (0);
        advanceVelocity(s, s->boxes->nLocalBoxes, 0.5 * dt);
        do {
            profileStop(velocityTimer);
        } while (0);
        do {
            profileStart(positionTimer);
        } while (0);
        advancePosition(s, s->boxes->nLocalBoxes, dt);
        do {
            profileStop(positionTimer);
        } while (0);
        do {
            profileStart(redistributeTimer);
        } while (0);
        redistributeAtoms(s);
        do {
            profileStop(redistributeTimer);
        } while (0);
        do {
            profileStart(computeForceTimer);
        } while (0);
        computeForce(s);
        do {
            profileStop(computeForceTimer);
        } while (0);
        do {
            profileStart(velocityTimer);
        } while (0);
        advanceVelocity(s, s->boxes->nLocalBoxes, 0.5 * dt);
        do {
            profileStop(velocityTimer);
        } while (0);
    }
    kineticEnergy(s);
    return s->ePotential;
}
/*Function: computeForce, ID: 172*/
void computeForce(SimFlat* s)
{
    /*computeForce:172*/
    /*CompoundStmt:431*/
    s->pot->force(s);
}
/*Function: advanceVelocity, ID: 169*/
void advanceVelocity(SimFlat* s, int nBoxes, real_t dt)
{
    /*advanceVelocity:169*/
    /*CompoundStmt:435*/
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP436>(ompNumThreads * DARTS_CODELETS_MULT, 0,
            RuntimeFinalCodelet, (real_t*)&((dt)), (int*)&((nBoxes)), (SimFlat**)&((s))));
    }
}
/*Function: advancePosition, ID: 170*/
void advancePosition(SimFlat* s, int nBoxes, real_t dt)
{
    /*advancePosition:170*/
    /*CompoundStmt:509*/
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP510>(ompNumThreads * DARTS_CODELETS_MULT, 0,
            RuntimeFinalCodelet, (real_t*)&((dt)), (int*)&((nBoxes)), (SimFlat**)&((s))));
    }
}
/*Function: kineticEnergy, ID: 175*/
void kineticEnergy(SimFlat* s)
{
    /*kineticEnergy:175*/
    /*CompoundStmt:596*/
    real_t eLocal[2];
    real_t kenergy = 0.;
    eLocal[0] = s->ePotential;
    eLocal[1] = 0;
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP605>(ompNumThreads * DARTS_CODELETS_MULT, 0,
            RuntimeFinalCodelet, (real_t*)&((kenergy)), (SimFlat**)&((s))));
    }
    eLocal[1] = kenergy;
    real_t eSum[2];
    do {
        profileStart(commReduceTimer);
    } while (0);
    addRealParallel(eLocal, eSum, 2);
    do {
        profileStop(commReduceTimer);
    } while (0);
    s->ePotential = eSum[0];
    s->eKinetic = eSum[1];
}
/*Function: redistributeAtoms, ID: 176*/
void redistributeAtoms(SimFlat* sim)
{
    /*redistributeAtoms:176*/
    /*CompoundStmt:709*/
    updateLinkCells(sim->boxes, sim->atoms);
    do {
        profileStart(atomHaloTimer);
    } while (0);
    haloExchange(sim->atomExchange, sim);
    do {
        profileStop(atomHaloTimer);
    } while (0);
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP723>(
            ompNumThreads * DARTS_CODELETS_MULT, 0, RuntimeFinalCodelet, (SimFlat**)&((sim))));
    }
}
/*TP436: OMPParallelDirective*/
void TP436::_barrierCodelets436::fire(void)
{
    TP436* myTP = static_cast<TP436*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
void TP436::_checkInCodelets438::fire(void)
{
    /*printOMPForCodeletFireCode, inlined: 0*/
    /*region 438 0*/
    /*Determine the TP to which this codelet belongs and check if this codelet spawns the TP or if
     * it signals it using dispatchCodelet()*/
    size_t idx = this->getID() / myTP->codeletsPerTP438;
    if (idx < myTP->TPsToUse438) {
        if (!__sync_val_compare_and_swap(&(myTP->TP438_alreadyLaunched[idx]), 0, 1)) {
            int range = abs((*(this->inputsTPParent->nBoxes_darts436)) - 0) / 1;
            int rangePerCodelet = range / myTP->TPsToUse438;
            int minIteration = min<int>((*(this->inputsTPParent->nBoxes_darts436)), 0);
            int remainderRange = range % myTP->TPsToUse438;
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
            if (0 < (*(this->inputsTPParent->nBoxes_darts436))) {
                initIteration = min(initIteration, lastIteration);
                lastIteration = max(initIteration, lastIteration);
            } else {
                initIteration = max(initIteration, lastIteration);
                lastIteration = min(initIteration, lastIteration);
            }
            if (idx == myTP->TPsToUse438 - 1) {
                lastIteration = (*(this->inputsTPParent->nBoxes_darts436));
            }
#if USEINVOKE == 1
            invoke<TP438>(myTP, myTP->codeletsPerTP438 * DARTS_CODELETS_MULT, this->getID(), myTP,
                initIteration, lastIteration, &(*(this->inputsTPParent->dt_darts436)),
                &(*(this->inputsTPParent->nBoxes_darts436)), &(*(this->inputsTPParent->s_darts436)),
                &(myTP->TP438Ptr[idx]));
#else
            place<TP438>(idx, myTP, myTP->codeletsPerTP438 * DARTS_CODELETS_MULT, this->getID(),
                myTP, initIteration, lastIteration, &(*(this->inputsTPParent->dt_darts436)),
                &(*(this->inputsTPParent->nBoxes_darts436)), &(*(this->inputsTPParent->s_darts436)),
                &(myTP->TP438Ptr[idx]));
#endif
        } else {
            if (myTP->TP438Ptr[idx] != nullptr) {
                myTP->TP438Ptr[idx]->dispatchCodelet(this->getID());
            } else {
                this->resetCodelet();
                this->decDep();
            }
        }
    }
}
void TP436::_barrierCodelets438::fire(void)
{
    TP436* myTP = static_cast<TP436*>(myTP_);
    myTP->TPParent->barrierCodelets436[0].setDep(0);
    myTP->add(&(myTP->TPParent->barrierCodelets436[0]));
}
TP436::TP436(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet, real_t* in_dt,
    int* in_nBoxes, SimFlat** in_s)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , dt_darts436(in_dt) /*OMP_SHARED - INPUT*/
    , nBoxes_darts436(in_nBoxes) /*OMP_SHARED - INPUT*/
    , s_darts436(in_s) /*OMP_SHARED - INPUT*/
    , TP438Ptr(new TP438*[NUMTPS438])
    , TP438_alreadyLaunched(new size_t[NUMTPS438])
    , numTPsSet438(0)
    , numTPsReady438(0)
    , TPsToUse438(NUMTPS438)
    , codeletsPerTP438(this->numThreads / NUMTPS438)
    , totalCodelets438(this->TPsToUse438 * this->codeletsPerTP438)
    , barrierCodelets436(new _barrierCodelets436[1])
    , checkInCodelets438(new _checkInCodelets438[this->numThreads])
    , barrierCodelets438(new _barrierCodelets438[1])
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets436[0] = _barrierCodelets436(ompNumThreads, ompNumThreads, this, 0);
    barrierCodelets438[0] = _barrierCodelets438(NUMTPS438, NUMTPS438, this, 0);
    _checkInCodelets438* checkInCodelets438Ptr = (this->checkInCodelets438);
    for (int i = 0; i < NUMTPS438; i++) {
        TP438Ptr[i] = nullptr;
        TP438_alreadyLaunched[i] = 0;
    }
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets438Ptr) = _checkInCodelets438(1, 1, this, codeletCounter);
        (*checkInCodelets438Ptr).decDep();
        checkInCodelets438Ptr++;
    }
}
TP436::~TP436()
{
    delete[] barrierCodelets436;
    delete[] barrierCodelets438;
    delete[] checkInCodelets438;
}
/*TP438: OMPForDirective*/
void TP438::_barrierCodelets438::fire(void)
{
    TP438* myTP = static_cast<TP438*>(myTP_);
    myTP->controlTPParent->TPParent->barrierCodelets438[0].decDep();
}
bool TP438::requestNewRangeIterations438(int* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Static */
    /*Chunk = 0*/
    bool isThereNewIteration = false;
    {
        /*Static Scheduling*/
        int tempStartRange = rangePerCodelet438 * codeletID;
        int tempEndRange = rangePerCodelet438 * (codeletID + 1);
        if (remainderRange438 != 0) {
            if (codeletID < (uint32_t)remainderRange438) {
                tempStartRange += codeletID;
                tempEndRange += (codeletID + 1);
            } else {
                tempStartRange += remainderRange438;
                tempEndRange += remainderRange438;
            }
        }
        tempStartRange = tempStartRange * 1 + minIteration438;
        tempEndRange = tempEndRange * 1 + minIteration438;
        if (tempStartRange != tempEndRange) {
            isThereNewIteration = true;
        }
        if (initIteration438 < lastIteration438) {
            (this->inputsTPParent->iBox_darts438[codeletID]) = min(tempStartRange, tempEndRange);
            *endRange = max(tempStartRange, tempEndRange);
        } else {
            (this->inputsTPParent->iBox_darts438[codeletID]) = max(tempStartRange, tempEndRange);
            *endRange = min(tempStartRange, tempEndRange);
        }
        if (codeletID == this->numThreads - 1) {
            *endRange = lastIteration438;
        }
    }
    return isThereNewIteration;
}
void TP438::_checkInCodelets439::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    /*Init the vars for this region*/

    /*printing node 439: ForStmt*/
    /*var: dt*/
    /*var: nBoxes*/
    /*var: s*/
    real_t* dt = (this->inputsTPParent->dt_darts438);
    (void)dt /*OMP_SHARED*/;
    SimFlat** s = (this->inputsTPParent->s_darts438);
    (void)s /*OMP_SHARED*/;
    int* iBox = &(this->inputsTPParent->iBox_darts438[this->getLocalID()]);
    (void)iBox /*PRIVATE*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations438(
        (int*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        /*Signaling omp for stmt's barrier*/
        myTP->controlTPParent->barrierCodelets438[0].decDep();
        return;
    }
    for (int iBox_darts_counter_temp438 = (*iBox); iBox_darts_counter_temp438 < endRange
         && iBox_darts_counter_temp438 < this->inputsTPParent->lastIteration438;
         iBox_darts_counter_temp438++) {
        {
            {
                /*Loop's init*/
                int iOff = 64 * (iBox_darts_counter_temp438);
                int ii = 0;
                for (; ii < (*(s))->boxes->nAtoms[(iBox_darts_counter_temp438)]; ii++, iOff++) {
                    (*(s))->atoms->p[iOff][0] += (*(dt)) * (*(s))->atoms->f[iOff][0];
                    (*(s))->atoms->p[iOff][1] += (*(dt)) * (*(s))->atoms->f[iOff][1];
                    (*(s))->atoms->p[iOff][2] += (*(dt)) * (*(s))->atoms->f[iOff][2];
                }
            }
        }
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling omp for stmt's barrier*/
    myTP->controlTPParent->barrierCodelets438[0].decDep();
}
TP438::TP438(int in_numThreads, int in_mainCodeletID, TP436* in_TPParent, int in_initIteration,
    int in_lastIteration, real_t* in_dt, int* in_nBoxes, SimFlat** in_s, TP438** in_ptrToThisTP)
    : ompTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , dt_darts438(in_dt) /*OMP_SHARED - INPUT*/
    , nBoxes_darts438(in_nBoxes) /*OMP_SHARED - INPUT*/
    , s_darts438(in_s) /*OMP_SHARED - INPUT*/
    , iBox_darts438(new int[this->numThreads]) /*VARIABLE*/
    , initIteration438(in_initIteration)
    , lastIteration438(in_lastIteration)
    , readyCodelets(0)
    , baseNumThreads(this->numThreads / DARTS_CODELETS_MULT)
    , barrierCodelets438(new _barrierCodelets438[1])
    , checkInCodelets439(new _checkInCodelets439[this->numThreads])
{
    /*Initialize the loop parameters*/
    range438 = abs(lastIteration438 - initIteration438) / 1;
    rangePerCodelet438 = range438 / numThreads;
    minIteration438 = min<int>(lastIteration438, initIteration438);
    remainderRange438 = range438 % numThreads;
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets438[0] = _barrierCodelets438(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets439* checkInCodelets439Ptr = (this->checkInCodelets439);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets439);
#endif
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->baseNumThreads;
         codeletCounter++) {
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets439Ptr) = _checkInCodelets439(2, 1, this, codeletCounter);
#else
        (*checkInCodelets439Ptr) = _checkInCodelets439(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets439Ptr).decDep();
        checkInCodelets439Ptr++;
    }
    this->dispatchCodelet(this->getID());
    *(in_ptrToThisTP) = this;
}
void TP438::dispatchCodelet(size_t codeletID)
{
    int idx = codeletID / this->baseNumThreads;
    int localID = codeletID - this->baseNumThreads * idx;
    this->checkInCodelets439[localID].setID(codeletID);
    this->checkInCodelets439[localID].setLocalID(localID);
    /*Check if we want to replicate codelets*/
    for (size_t i = 1; i < (size_t)DARTS_CODELETS_MULT; i++) {
#if USE_SPIN_CODELETS == 0
        this->checkInCodelets439[localID + this->baseNumThreads * i]
            = _checkInCodelets439(2, 1, this, localID + this->baseNumThreads * i);
#else
        this->checkInCodelets439[localID + this->baseNumThreads * i]
            = _checkInCodelets439(1, 1, this, localID + this->baseNumThreads * i);
#endif
        this->checkInCodelets439[localID + this->baseNumThreads * i].setID(codeletID);
        this->checkInCodelets439[localID + this->baseNumThreads * i].decDep();
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
TP438::~TP438()
{
    delete[] iBox_darts438;
    delete[] barrierCodelets438;
    delete[] checkInCodelets439;
}
/*TP510: OMPParallelDirective*/
void TP510::_barrierCodelets510::fire(void)
{
    TP510* myTP = static_cast<TP510*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
void TP510::_checkInCodelets512::fire(void)
{
    /*printOMPForCodeletFireCode, inlined: 0*/
    /*region 512 0*/
    /*Determine the TP to which this codelet belongs and check if this codelet spawns the TP or if
     * it signals it using dispatchCodelet()*/
    size_t idx = this->getID() / myTP->codeletsPerTP512;
    if (idx < myTP->TPsToUse512) {
        if (!__sync_val_compare_and_swap(&(myTP->TP512_alreadyLaunched[idx]), 0, 1)) {
            int range = abs((*(this->inputsTPParent->nBoxes_darts510)) - 0) / 1;
            int rangePerCodelet = range / myTP->TPsToUse512;
            int minIteration = min<int>((*(this->inputsTPParent->nBoxes_darts510)), 0);
            int remainderRange = range % myTP->TPsToUse512;
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
            if (0 < (*(this->inputsTPParent->nBoxes_darts510))) {
                initIteration = min(initIteration, lastIteration);
                lastIteration = max(initIteration, lastIteration);
            } else {
                initIteration = max(initIteration, lastIteration);
                lastIteration = min(initIteration, lastIteration);
            }
            if (idx == myTP->TPsToUse512 - 1) {
                lastIteration = (*(this->inputsTPParent->nBoxes_darts510));
            }
#if USEINVOKE == 1
            invoke<TP512>(myTP, myTP->codeletsPerTP512 * DARTS_CODELETS_MULT, this->getID(), myTP,
                initIteration, lastIteration, &(*(this->inputsTPParent->dt_darts510)),
                &(*(this->inputsTPParent->nBoxes_darts510)), &(*(this->inputsTPParent->s_darts510)),
                &(myTP->TP512Ptr[idx]));
#else
            place<TP512>(idx, myTP, myTP->codeletsPerTP512 * DARTS_CODELETS_MULT, this->getID(),
                myTP, initIteration, lastIteration, &(*(this->inputsTPParent->dt_darts510)),
                &(*(this->inputsTPParent->nBoxes_darts510)), &(*(this->inputsTPParent->s_darts510)),
                &(myTP->TP512Ptr[idx]));
#endif
        } else {
            if (myTP->TP512Ptr[idx] != nullptr) {
                myTP->TP512Ptr[idx]->dispatchCodelet(this->getID());
            } else {
                this->resetCodelet();
                this->decDep();
            }
        }
    }
}
void TP510::_barrierCodelets512::fire(void)
{
    TP510* myTP = static_cast<TP510*>(myTP_);
    myTP->TPParent->barrierCodelets510[0].setDep(0);
    myTP->add(&(myTP->TPParent->barrierCodelets510[0]));
}
TP510::TP510(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet, real_t* in_dt,
    int* in_nBoxes, SimFlat** in_s)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , dt_darts510(in_dt) /*OMP_SHARED - INPUT*/
    , nBoxes_darts510(in_nBoxes) /*OMP_SHARED - INPUT*/
    , s_darts510(in_s) /*OMP_SHARED - INPUT*/
    , TP512Ptr(new TP512*[NUMTPS512])
    , TP512_alreadyLaunched(new size_t[NUMTPS512])
    , numTPsSet512(0)
    , numTPsReady512(0)
    , TPsToUse512(NUMTPS512)
    , codeletsPerTP512(this->numThreads / NUMTPS512)
    , totalCodelets512(this->TPsToUse512 * this->codeletsPerTP512)
    , barrierCodelets510(new _barrierCodelets510[1])
    , checkInCodelets512(new _checkInCodelets512[this->numThreads])
    , barrierCodelets512(new _barrierCodelets512[1])
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets510[0] = _barrierCodelets510(ompNumThreads, ompNumThreads, this, 0);
    barrierCodelets512[0] = _barrierCodelets512(NUMTPS512, NUMTPS512, this, 0);
    _checkInCodelets512* checkInCodelets512Ptr = (this->checkInCodelets512);
    for (int i = 0; i < NUMTPS512; i++) {
        TP512Ptr[i] = nullptr;
        TP512_alreadyLaunched[i] = 0;
    }
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets512Ptr) = _checkInCodelets512(1, 1, this, codeletCounter);
        (*checkInCodelets512Ptr).decDep();
        checkInCodelets512Ptr++;
    }
}
TP510::~TP510()
{
    delete[] barrierCodelets510;
    delete[] barrierCodelets512;
    delete[] checkInCodelets512;
}
/*TP512: OMPForDirective*/
void TP512::_barrierCodelets512::fire(void)
{
    TP512* myTP = static_cast<TP512*>(myTP_);
    myTP->controlTPParent->TPParent->barrierCodelets512[0].decDep();
}
bool TP512::requestNewRangeIterations512(int* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Static */
    /*Chunk = 0*/
    bool isThereNewIteration = false;
    {
        /*Static Scheduling*/
        int tempStartRange = rangePerCodelet512 * codeletID;
        int tempEndRange = rangePerCodelet512 * (codeletID + 1);
        if (remainderRange512 != 0) {
            if (codeletID < (uint32_t)remainderRange512) {
                tempStartRange += codeletID;
                tempEndRange += (codeletID + 1);
            } else {
                tempStartRange += remainderRange512;
                tempEndRange += remainderRange512;
            }
        }
        tempStartRange = tempStartRange * 1 + minIteration512;
        tempEndRange = tempEndRange * 1 + minIteration512;
        if (tempStartRange != tempEndRange) {
            isThereNewIteration = true;
        }
        if (initIteration512 < lastIteration512) {
            (this->inputsTPParent->iBox_darts512[codeletID]) = min(tempStartRange, tempEndRange);
            *endRange = max(tempStartRange, tempEndRange);
        } else {
            (this->inputsTPParent->iBox_darts512[codeletID]) = max(tempStartRange, tempEndRange);
            *endRange = min(tempStartRange, tempEndRange);
        }
        if (codeletID == this->numThreads - 1) {
            *endRange = lastIteration512;
        }
    }
    return isThereNewIteration;
}
void TP512::_checkInCodelets513::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    /*Init the vars for this region*/

    /*printing node 513: ForStmt*/
    /*var: dt*/
    /*var: nBoxes*/
    /*var: s*/
    real_t* dt = (this->inputsTPParent->dt_darts512);
    (void)dt /*OMP_SHARED*/;
    SimFlat** s = (this->inputsTPParent->s_darts512);
    (void)s /*OMP_SHARED*/;
    int* iBox = &(this->inputsTPParent->iBox_darts512[this->getLocalID()]);
    (void)iBox /*PRIVATE*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations512(
        (int*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        /*Signaling omp for stmt's barrier*/
        myTP->controlTPParent->barrierCodelets512[0].decDep();
        return;
    }
    for (int iBox_darts_counter_temp512 = (*iBox); iBox_darts_counter_temp512 < endRange
         && iBox_darts_counter_temp512 < this->inputsTPParent->lastIteration512;
         iBox_darts_counter_temp512++) {
        {
            {
                /*Loop's init*/
                int iOff = 64 * (iBox_darts_counter_temp512);
                int ii = 0;
                for (; ii < (*(s))->boxes->nAtoms[(iBox_darts_counter_temp512)]; ii++, iOff++) {
                    int iSpecies = (*(s))->atoms->iSpecies[iOff];
                    real_t invMass = 1. / (*(s))->species[iSpecies].mass;
                    (*(s))->atoms->r[iOff][0] += (*(dt)) * (*(s))->atoms->p[iOff][0] * invMass;
                    (*(s))->atoms->r[iOff][1] += (*(dt)) * (*(s))->atoms->p[iOff][1] * invMass;
                    (*(s))->atoms->r[iOff][2] += (*(dt)) * (*(s))->atoms->p[iOff][2] * invMass;
                }
            }
        }
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling omp for stmt's barrier*/
    myTP->controlTPParent->barrierCodelets512[0].decDep();
}
TP512::TP512(int in_numThreads, int in_mainCodeletID, TP510* in_TPParent, int in_initIteration,
    int in_lastIteration, real_t* in_dt, int* in_nBoxes, SimFlat** in_s, TP512** in_ptrToThisTP)
    : ompTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , dt_darts512(in_dt) /*OMP_SHARED - INPUT*/
    , nBoxes_darts512(in_nBoxes) /*OMP_SHARED - INPUT*/
    , s_darts512(in_s) /*OMP_SHARED - INPUT*/
    , iBox_darts512(new int[this->numThreads]) /*VARIABLE*/
    , initIteration512(in_initIteration)
    , lastIteration512(in_lastIteration)
    , readyCodelets(0)
    , baseNumThreads(this->numThreads / DARTS_CODELETS_MULT)
    , barrierCodelets512(new _barrierCodelets512[1])
    , checkInCodelets513(new _checkInCodelets513[this->numThreads])
{
    /*Initialize the loop parameters*/
    range512 = abs(lastIteration512 - initIteration512) / 1;
    rangePerCodelet512 = range512 / numThreads;
    minIteration512 = min<int>(lastIteration512, initIteration512);
    remainderRange512 = range512 % numThreads;
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets512[0] = _barrierCodelets512(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets513* checkInCodelets513Ptr = (this->checkInCodelets513);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets513);
#endif
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->baseNumThreads;
         codeletCounter++) {
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets513Ptr) = _checkInCodelets513(2, 1, this, codeletCounter);
#else
        (*checkInCodelets513Ptr) = _checkInCodelets513(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets513Ptr).decDep();
        checkInCodelets513Ptr++;
    }
    this->dispatchCodelet(this->getID());
    *(in_ptrToThisTP) = this;
}
void TP512::dispatchCodelet(size_t codeletID)
{
    int idx = codeletID / this->baseNumThreads;
    int localID = codeletID - this->baseNumThreads * idx;
    this->checkInCodelets513[localID].setID(codeletID);
    this->checkInCodelets513[localID].setLocalID(localID);
    /*Check if we want to replicate codelets*/
    for (size_t i = 1; i < (size_t)DARTS_CODELETS_MULT; i++) {
#if USE_SPIN_CODELETS == 0
        this->checkInCodelets513[localID + this->baseNumThreads * i]
            = _checkInCodelets513(2, 1, this, localID + this->baseNumThreads * i);
#else
        this->checkInCodelets513[localID + this->baseNumThreads * i]
            = _checkInCodelets513(1, 1, this, localID + this->baseNumThreads * i);
#endif
        this->checkInCodelets513[localID + this->baseNumThreads * i].setID(codeletID);
        this->checkInCodelets513[localID + this->baseNumThreads * i].decDep();
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
TP512::~TP512()
{
    delete[] iBox_darts512;
    delete[] barrierCodelets512;
    delete[] checkInCodelets513;
}
/*TP605: OMPParallelDirective*/
void TP605::_barrierCodelets605::fire(void)
{
    TP605* myTP = static_cast<TP605*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
void TP605::_checkInCodelets607::fire(void)
{
    /*printOMPForCodeletFireCode, inlined: 0*/
    /*region 607 0*/
    /*Determine the TP to which this codelet belongs and check if this codelet spawns the TP or if
     * it signals it using dispatchCodelet()*/
    size_t idx = this->getID() / myTP->codeletsPerTP607;
    if (idx < myTP->TPsToUse607) {
        if (!__sync_val_compare_and_swap(&(myTP->TP607_alreadyLaunched[idx]), 0, 1)) {
            int range = abs((*(this->inputsTPParent->s_darts605))->boxes->nLocalBoxes - 0) / 1;
            int rangePerCodelet = range / myTP->TPsToUse607;
            int minIteration
                = min<int>((*(this->inputsTPParent->s_darts605))->boxes->nLocalBoxes, 0);
            int remainderRange = range % myTP->TPsToUse607;
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
            if (0 < (*(this->inputsTPParent->s_darts605))->boxes->nLocalBoxes) {
                initIteration = min(initIteration, lastIteration);
                lastIteration = max(initIteration, lastIteration);
            } else {
                initIteration = max(initIteration, lastIteration);
                lastIteration = min(initIteration, lastIteration);
            }
            if (idx == myTP->TPsToUse607 - 1) {
                lastIteration = (*(this->inputsTPParent->s_darts605))->boxes->nLocalBoxes;
            }
#if USEINVOKE == 1
            invoke<TP607>(myTP, myTP->codeletsPerTP607 * DARTS_CODELETS_MULT, this->getID(), myTP,
                initIteration, lastIteration, &(*(this->inputsTPParent->kenergy_darts605)),
                &(*(this->inputsTPParent->s_darts605)), &(myTP->TP607Ptr[idx]));
#else
            place<TP607>(idx, myTP, myTP->codeletsPerTP607 * DARTS_CODELETS_MULT, this->getID(),
                myTP, initIteration, lastIteration, &(*(this->inputsTPParent->kenergy_darts605)),
                &(*(this->inputsTPParent->s_darts605)), &(myTP->TP607Ptr[idx]));
#endif
        } else {
            if (myTP->TP607Ptr[idx] != nullptr) {
                myTP->TP607Ptr[idx]->dispatchCodelet(this->getID());
            } else {
                this->resetCodelet();
                this->decDep();
            }
        }
    }
}
void TP605::_barrierCodelets607::fire(void)
{
    TP605* myTP = static_cast<TP605*>(myTP_);
    myTP->TPParent->barrierCodelets605[0].setDep(0);
    myTP->add(&(myTP->TPParent->barrierCodelets605[0]));
}
TP605::TP605(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet, real_t* in_kenergy,
    SimFlat** in_s)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , kenergy_darts605(in_kenergy) /*OMP_SHARED - INPUT*/
    , s_darts605(in_s) /*OMP_SHARED - INPUT*/
    , TP607Ptr(new TP607*[NUMTPS607])
    , TP607_alreadyLaunched(new size_t[NUMTPS607])
    , numTPsSet607(0)
    , numTPsReady607(0)
    , TPsToUse607(NUMTPS607)
    , codeletsPerTP607(this->numThreads / NUMTPS607)
    , totalCodelets607(this->TPsToUse607 * this->codeletsPerTP607)
    , barrierCodelets605(new _barrierCodelets605[1])
    , checkInCodelets607(new _checkInCodelets607[this->numThreads])
    , barrierCodelets607(new _barrierCodelets607[1])
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets605[0] = _barrierCodelets605(ompNumThreads, ompNumThreads, this, 0);
    barrierCodelets607[0] = _barrierCodelets607(NUMTPS607, NUMTPS607, this, 0);
    _checkInCodelets607* checkInCodelets607Ptr = (this->checkInCodelets607);
    for (int i = 0; i < NUMTPS607; i++) {
        TP607Ptr[i] = nullptr;
        TP607_alreadyLaunched[i] = 0;
    }
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets607Ptr) = _checkInCodelets607(1, 1, this, codeletCounter);
        (*checkInCodelets607Ptr).decDep();
        checkInCodelets607Ptr++;
    }
}
TP605::~TP605()
{
    delete[] barrierCodelets605;
    delete[] barrierCodelets607;
    delete[] checkInCodelets607;
}
/*TP607: OMPForDirective*/
void TP607::_barrierCodelets607::fire(void)
{
    TP607* myTP = static_cast<TP607*>(myTP_);
    myTP->controlTPParent->TPParent->barrierCodelets607[0].decDep();
}
bool TP607::requestNewRangeIterations607(int* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Static */
    /*Chunk = 0*/
    bool isThereNewIteration = false;
    {
        /*Static Scheduling*/
        int tempStartRange = rangePerCodelet607 * codeletID;
        int tempEndRange = rangePerCodelet607 * (codeletID + 1);
        if (remainderRange607 != 0) {
            if (codeletID < (uint32_t)remainderRange607) {
                tempStartRange += codeletID;
                tempEndRange += (codeletID + 1);
            } else {
                tempStartRange += remainderRange607;
                tempEndRange += remainderRange607;
            }
        }
        tempStartRange = tempStartRange * 1 + minIteration607;
        tempEndRange = tempEndRange * 1 + minIteration607;
        if (tempStartRange != tempEndRange) {
            isThereNewIteration = true;
        }
        if (initIteration607 < lastIteration607) {
            (this->inputsTPParent->iBox_darts607[codeletID]) = min(tempStartRange, tempEndRange);
            *endRange = max(tempStartRange, tempEndRange);
        } else {
            (this->inputsTPParent->iBox_darts607[codeletID]) = max(tempStartRange, tempEndRange);
            *endRange = min(tempStartRange, tempEndRange);
        }
        if (codeletID == this->numThreads - 1) {
            *endRange = lastIteration607;
        }
    }
    return isThereNewIteration;
}
void TP607::_checkInCodelets608::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    /*Init the vars for this region*/

    /*printing node 608: ForStmt*/
    /*var: kenergy*/
    /*var: s*/
    /*Declare local reduction vars*/
    real_t kenergy_LocalReductionVar = (real_t)0;
    SimFlat** s = (this->inputsTPParent->s_darts607);
    (void)s /*OMP_SHARED*/;
    int* iBox = &(this->inputsTPParent->iBox_darts607[this->getLocalID()]);
    (void)iBox /*PRIVATE*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations607(
        (int*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        goto updateReduceVars;
    }
    for (int iBox_darts_counter_temp607 = (*iBox); iBox_darts_counter_temp607 < endRange
         && iBox_darts_counter_temp607 < this->inputsTPParent->lastIteration607;
         iBox_darts_counter_temp607++) {
        {
            {
                /*Loop's init*/
                int iOff = 64 * (iBox_darts_counter_temp607);
                int ii = 0;
                for (; ii < (*(s))->boxes->nAtoms[(iBox_darts_counter_temp607)]; ii++, iOff++) {
                    int iSpecies = (*(s))->atoms->iSpecies[iOff];
                    real_t invMass = 0.5 / (*(s))->species[iSpecies].mass;
                    (*(&(kenergy_LocalReductionVar)))
                        += ((*(s))->atoms->p[iOff][0] * (*(s))->atoms->p[iOff][0]
                               + (*(s))->atoms->p[iOff][1] * (*(s))->atoms->p[iOff][1]
                               + (*(s))->atoms->p[iOff][2] * (*(s))->atoms->p[iOff][2])
                        * invMass;
                }
            }
        }
    }
updateReduceVars:
    /*Before signaling next codelet, check if all the threads in the TP
    * performed the reduction locally and if so, update the unique
    * reduction variable.*/
    /*Reducing for operator '+'*/
    this->inputsTPParent->kenergy_darts607_mutex->lock();
    *(this->inputsTPParent->kenergy_darts607) += (kenergy_LocalReductionVar);
    this->inputsTPParent->kenergy_darts607_mutex->unlock();
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling omp for stmt's barrier*/
    myTP->controlTPParent->barrierCodelets607[0].decDep();
}
TP607::TP607(int in_numThreads, int in_mainCodeletID, TP605* in_TPParent, int in_initIteration,
    int in_lastIteration, real_t* in_kenergy, SimFlat** in_s, TP607** in_ptrToThisTP)
    : ompTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , kenergy_darts607(in_kenergy) /*OMP_SHARED - INPUT*/
    , s_darts607(in_s) /*OMP_SHARED - INPUT*/
    , iBox_darts607(new int[this->numThreads]) /*VARIABLE*/
    , initIteration607(in_initIteration)
    , lastIteration607(in_lastIteration)
    , readyCodelets(0)
    , baseNumThreads(this->numThreads / DARTS_CODELETS_MULT)
    , kenergy_darts607_mutex(&(this->TPParent->kenergy_darts607_mutex))
    , barrierCodelets607(new _barrierCodelets607[1])
    , checkInCodelets608(new _checkInCodelets608[this->numThreads])
{
    /*Initialize the loop parameters*/
    range607 = abs(lastIteration607 - initIteration607) / 1;
    rangePerCodelet607 = range607 / numThreads;
    minIteration607 = min<int>(lastIteration607, initIteration607);
    remainderRange607 = range607 % numThreads;
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets607[0] = _barrierCodelets607(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets608* checkInCodelets608Ptr = (this->checkInCodelets608);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets608);
#endif
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->baseNumThreads;
         codeletCounter++) {
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets608Ptr) = _checkInCodelets608(2, 1, this, codeletCounter);
#else
        (*checkInCodelets608Ptr) = _checkInCodelets608(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets608Ptr).decDep();
        checkInCodelets608Ptr++;
    }
    this->dispatchCodelet(this->getID());
    *(in_ptrToThisTP) = this;
}
void TP607::dispatchCodelet(size_t codeletID)
{
    int idx = codeletID / this->baseNumThreads;
    int localID = codeletID - this->baseNumThreads * idx;
    this->checkInCodelets608[localID].setID(codeletID);
    this->checkInCodelets608[localID].setLocalID(localID);
    /*Check if we want to replicate codelets*/
    for (size_t i = 1; i < (size_t)DARTS_CODELETS_MULT; i++) {
#if USE_SPIN_CODELETS == 0
        this->checkInCodelets608[localID + this->baseNumThreads * i]
            = _checkInCodelets608(2, 1, this, localID + this->baseNumThreads * i);
#else
        this->checkInCodelets608[localID + this->baseNumThreads * i]
            = _checkInCodelets608(1, 1, this, localID + this->baseNumThreads * i);
#endif
        this->checkInCodelets608[localID + this->baseNumThreads * i].setID(codeletID);
        this->checkInCodelets608[localID + this->baseNumThreads * i].decDep();
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
TP607::~TP607()
{
    delete[] iBox_darts607;
    delete[] barrierCodelets607;
    delete[] checkInCodelets608;
}
/*TP723: OMPParallelDirective*/
void TP723::_barrierCodelets723::fire(void)
{
    TP723* myTP = static_cast<TP723*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
void TP723::_checkInCodelets725::fire(void)
{
    /*printOMPForCodeletFireCode, inlined: 0*/
    /*region 725 0*/
    /*Determine the TP to which this codelet belongs and check if this codelet spawns the TP or if
     * it signals it using dispatchCodelet()*/
    size_t idx = this->getID() / myTP->codeletsPerTP725;
    if (idx < myTP->TPsToUse725) {
        if (!__sync_val_compare_and_swap(&(myTP->TP725_alreadyLaunched[idx]), 0, 1)) {
            int range = abs((*(this->inputsTPParent->sim_darts723))->boxes->nTotalBoxes - 0) / 1;
            int rangePerCodelet = range / myTP->TPsToUse725;
            int minIteration
                = min<int>((*(this->inputsTPParent->sim_darts723))->boxes->nTotalBoxes, 0);
            int remainderRange = range % myTP->TPsToUse725;
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
            if (0 < (*(this->inputsTPParent->sim_darts723))->boxes->nTotalBoxes) {
                initIteration = min(initIteration, lastIteration);
                lastIteration = max(initIteration, lastIteration);
            } else {
                initIteration = max(initIteration, lastIteration);
                lastIteration = min(initIteration, lastIteration);
            }
            if (idx == myTP->TPsToUse725 - 1) {
                lastIteration = (*(this->inputsTPParent->sim_darts723))->boxes->nTotalBoxes;
            }
#if USEINVOKE == 1
            invoke<TP725>(myTP, myTP->codeletsPerTP725 * DARTS_CODELETS_MULT, this->getID(), myTP,
                initIteration, lastIteration, &(*(this->inputsTPParent->sim_darts723)),
                &(myTP->TP725Ptr[idx]));
#else
            place<TP725>(idx, myTP, myTP->codeletsPerTP725 * DARTS_CODELETS_MULT, this->getID(),
                myTP, initIteration, lastIteration, &(*(this->inputsTPParent->sim_darts723)),
                &(myTP->TP725Ptr[idx]));
#endif
        } else {
            if (myTP->TP725Ptr[idx] != nullptr) {
                myTP->TP725Ptr[idx]->dispatchCodelet(this->getID());
            } else {
                this->resetCodelet();
                this->decDep();
            }
        }
    }
}
void TP723::_barrierCodelets725::fire(void)
{
    TP723* myTP = static_cast<TP723*>(myTP_);
    myTP->TPParent->barrierCodelets723[0].setDep(0);
    myTP->add(&(myTP->TPParent->barrierCodelets723[0]));
}
TP723::TP723(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet, SimFlat** in_sim)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , sim_darts723(in_sim) /*OMP_SHARED - INPUT*/
    , TP725Ptr(new TP725*[NUMTPS725])
    , TP725_alreadyLaunched(new size_t[NUMTPS725])
    , numTPsSet725(0)
    , numTPsReady725(0)
    , TPsToUse725(NUMTPS725)
    , codeletsPerTP725(this->numThreads / NUMTPS725)
    , totalCodelets725(this->TPsToUse725 * this->codeletsPerTP725)
    , barrierCodelets723(new _barrierCodelets723[1])
    , checkInCodelets725(new _checkInCodelets725[this->numThreads])
    , barrierCodelets725(new _barrierCodelets725[1])
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets723[0] = _barrierCodelets723(ompNumThreads, ompNumThreads, this, 0);
    barrierCodelets725[0] = _barrierCodelets725(NUMTPS725, NUMTPS725, this, 0);
    _checkInCodelets725* checkInCodelets725Ptr = (this->checkInCodelets725);
    for (int i = 0; i < NUMTPS725; i++) {
        TP725Ptr[i] = nullptr;
        TP725_alreadyLaunched[i] = 0;
    }
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets725Ptr) = _checkInCodelets725(1, 1, this, codeletCounter);
        (*checkInCodelets725Ptr).decDep();
        checkInCodelets725Ptr++;
    }
}
TP723::~TP723()
{
    delete[] barrierCodelets723;
    delete[] barrierCodelets725;
    delete[] checkInCodelets725;
}
/*TP725: OMPForDirective*/
void TP725::_barrierCodelets725::fire(void)
{
    TP725* myTP = static_cast<TP725*>(myTP_);
    myTP->controlTPParent->TPParent->barrierCodelets725[0].decDep();
}
bool TP725::requestNewRangeIterations725(int* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Static */
    /*Chunk = 0*/
    bool isThereNewIteration = false;
    {
        /*Static Scheduling*/
        int tempStartRange = rangePerCodelet725 * codeletID;
        int tempEndRange = rangePerCodelet725 * (codeletID + 1);
        if (remainderRange725 != 0) {
            if (codeletID < (uint32_t)remainderRange725) {
                tempStartRange += codeletID;
                tempEndRange += (codeletID + 1);
            } else {
                tempStartRange += remainderRange725;
                tempEndRange += remainderRange725;
            }
        }
        tempStartRange = tempStartRange * 1 + minIteration725;
        tempEndRange = tempEndRange * 1 + minIteration725;
        if (tempStartRange != tempEndRange) {
            isThereNewIteration = true;
        }
        if (initIteration725 < lastIteration725) {
            (this->inputsTPParent->ii_darts725[codeletID]) = min(tempStartRange, tempEndRange);
            *endRange = max(tempStartRange, tempEndRange);
        } else {
            (this->inputsTPParent->ii_darts725[codeletID]) = max(tempStartRange, tempEndRange);
            *endRange = min(tempStartRange, tempEndRange);
        }
        if (codeletID == this->numThreads - 1) {
            *endRange = lastIteration725;
        }
    }
    return isThereNewIteration;
}
void TP725::_checkInCodelets726::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    /*Init the vars for this region*/

    /*printing node 726: ForStmt*/
    /*var: sim*/
    SimFlat** sim = (this->inputsTPParent->sim_darts725);
    (void)sim /*OMP_SHARED*/;
    int* ii = &(this->inputsTPParent->ii_darts725[this->getLocalID()]);
    (void)ii /*PRIVATE*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations725(
        (int*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        /*Signaling omp for stmt's barrier*/
        myTP->controlTPParent->barrierCodelets725[0].decDep();
        return;
    }
    for (int ii_darts_counter_temp725 = (*ii); ii_darts_counter_temp725 < endRange
         && ii_darts_counter_temp725 < this->inputsTPParent->lastIteration725;
         ++ii_darts_counter_temp725) {
        {
            sortAtomsInCell((*(sim))->atoms, (*(sim))->boxes, (ii_darts_counter_temp725));
        }
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling omp for stmt's barrier*/
    myTP->controlTPParent->barrierCodelets725[0].decDep();
}
TP725::TP725(int in_numThreads, int in_mainCodeletID, TP723* in_TPParent, int in_initIteration,
    int in_lastIteration, SimFlat** in_sim, TP725** in_ptrToThisTP)
    : ompTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , sim_darts725(in_sim) /*OMP_SHARED - INPUT*/
    , ii_darts725(new int[this->numThreads]) /*VARIABLE*/
    , initIteration725(in_initIteration)
    , lastIteration725(in_lastIteration)
    , readyCodelets(0)
    , baseNumThreads(this->numThreads / DARTS_CODELETS_MULT)
    , barrierCodelets725(new _barrierCodelets725[1])
    , checkInCodelets726(new _checkInCodelets726[this->numThreads])
{
    /*Initialize the loop parameters*/
    range725 = abs(lastIteration725 - initIteration725) / 1;
    rangePerCodelet725 = range725 / numThreads;
    minIteration725 = min<int>(lastIteration725, initIteration725);
    remainderRange725 = range725 % numThreads;
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets725[0] = _barrierCodelets725(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets726* checkInCodelets726Ptr = (this->checkInCodelets726);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets726);
#endif
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->baseNumThreads;
         codeletCounter++) {
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets726Ptr) = _checkInCodelets726(2, 1, this, codeletCounter);
#else
        (*checkInCodelets726Ptr) = _checkInCodelets726(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets726Ptr).decDep();
        checkInCodelets726Ptr++;
    }
    this->dispatchCodelet(this->getID());
    *(in_ptrToThisTP) = this;
}
void TP725::dispatchCodelet(size_t codeletID)
{
    int idx = codeletID / this->baseNumThreads;
    int localID = codeletID - this->baseNumThreads * idx;
    this->checkInCodelets726[localID].setID(codeletID);
    this->checkInCodelets726[localID].setLocalID(localID);
    /*Check if we want to replicate codelets*/
    for (size_t i = 1; i < (size_t)DARTS_CODELETS_MULT; i++) {
#if USE_SPIN_CODELETS == 0
        this->checkInCodelets726[localID + this->baseNumThreads * i]
            = _checkInCodelets726(2, 1, this, localID + this->baseNumThreads * i);
#else
        this->checkInCodelets726[localID + this->baseNumThreads * i]
            = _checkInCodelets726(1, 1, this, localID + this->baseNumThreads * i);
#endif
        this->checkInCodelets726[localID + this->baseNumThreads * i].setID(codeletID);
        this->checkInCodelets726[localID + this->baseNumThreads * i].decDep();
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
TP725::~TP725()
{
    delete[] ii_darts725;
    delete[] barrierCodelets725;
    delete[] checkInCodelets726;
}
