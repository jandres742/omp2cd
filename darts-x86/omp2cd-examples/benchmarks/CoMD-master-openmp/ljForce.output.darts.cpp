#include "ljForce.output.darts.h"
using namespace darts;
using namespace std;
static int ljForce(SimFlat* s);
static void ljPrint(FILE* file, BasePotential* pot);
/*Function: ljDestroy, ID: 128*/
void ljDestroy(BasePotential** inppot)
{
    /*ljDestroy:128*/
    /*CompoundStmt:3452*/
    if (!inppot)
        return;
    LjPotential* pot = (LjPotential*)(*inppot);
    if (!pot)
        return;
    comdFree(pot);
    *inppot = ((void*)0);
    return;
}
/*Function: initLjPot, ID: 129*/
struct BasePotentialSt* initLjPot()
{
    /*initLjPot:129*/
    /*CompoundStmt:3469*/
    LjPotential* pot = (LjPotential*)comdMalloc(sizeof(LjPotential));
    pot->force = ljForce;
    pot->print = ljPrint;
    pot->destroy = ljDestroy;
    pot->sigma = 2.3149999999999999;
    pot->epsilon = 0.16700000000000001;
    pot->mass = 63.549999999999997 * amuToInternalMass;
    pot->lat = 3.6150000000000002;
    strcpy(pot->latticeType, "FCC");
    pot->cutoff = 2.5 * pot->sigma;
    strcpy(pot->name, "Cu");
    pot->atomicNo = 29;
    return (BasePotential*)pot;
}
/*Function: ljPrint, ID: 127*/
void ljPrint(FILE* file, BasePotential* pot)
{
    /*ljPrint:127*/
    /*CompoundStmt:3506*/
    LjPotential* ljPot = (LjPotential*)pot;
    fprintf(file, "  Potential type   : Lennard-Jones\n");
    fprintf(file, "  Species name     : %s\n", ljPot->name);
    fprintf(file, "  Atomic number    : %d\n", ljPot->atomicNo);
    fprintf(file, "  Mass             : %lg amu\n", ljPot->mass / amuToInternalMass);
    fprintf(file, "  Lattice Type     : %s\n", ljPot->latticeType);
    fprintf(file, "  Lattice spacing  : %lg Angstroms\n", ljPot->lat);
    fprintf(file, "  Cutoff           : %lg Angstroms\n", ljPot->cutoff);
    fprintf(file, "  Epsilon          : %lg eV\n", ljPot->epsilon);
    fprintf(file, "  Sigma            : %lg Angstroms\n", ljPot->sigma);
}
/*Function: ljForce, ID: 126*/
int ljForce(SimFlat* s)
{
    /*ljForce:126*/
    /*CompoundStmt:3527*/
    LjPotential* pot = (LjPotential*)s->pot;
    real_t sigma = pot->sigma;
    real_t epsilon = pot->epsilon;
    real_t rCut = pot->cutoff;
    real_t rCut2 = rCut * rCut;
    real_t ePot = 0.;
    s->ePotential = 0.;
    int fSize = s->boxes->nTotalBoxes * 64;
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP3548>(ompNumThreads * DARTS_CODELETS_MULT, 0,
            RuntimeFinalCodelet, (int*)&((fSize)), (SimFlat**)&((s))));
    }
    real_t s6 = sigma * sigma * sigma * sigma * sigma * sigma;
    real_t rCut6 = s6 / (rCut2 * rCut2 * rCut2);
    real_t eShift = 1. * rCut6 * (rCut6 - 1.);
    int nNbrBoxes = 27;
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP3606>(ompNumThreads * DARTS_CODELETS_MULT, 0,
            RuntimeFinalCodelet, (real_t*)&((ePot)), (real_t*)&((eShift)), (real_t*)&((epsilon)),
            (real_t*)&((rCut2)), (SimFlat**)&((s)), (real_t*)&((s6))));
    }
    ePot = ePot * 4. * epsilon;
    s->ePotential = ePot;
    return 0;
}
/*TP3548: OMPParallelDirective*/
void TP3548::_barrierCodelets3548::fire(void)
{
    TP3548* myTP = static_cast<TP3548*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
void TP3548::_checkInCodelets3550::fire(void)
{
    /*printOMPForCodeletFireCode, inlined: 0*/
    /*region 3550 0*/
    /*Determine the TP to which this codelet belongs and check if this codelet spawns the TP or if
     * it signals it using dispatchCodelet()*/
    size_t idx = this->getID() / myTP->codeletsPerTP3550;
    if (idx < myTP->TPsToUse3550) {
        if (!__sync_val_compare_and_swap(&(myTP->TP3550_alreadyLaunched[idx]), 0, 1)) {
            int range = abs((*(this->inputsTPParent->fSize_darts3548)) - 0) / 1;
            int rangePerCodelet = range / myTP->TPsToUse3550;
            int minIteration = min<int>((*(this->inputsTPParent->fSize_darts3548)), 0);
            int remainderRange = range % myTP->TPsToUse3550;
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
            if (0 < (*(this->inputsTPParent->fSize_darts3548))) {
                initIteration = min(initIteration, lastIteration);
                lastIteration = max(initIteration, lastIteration);
            } else {
                initIteration = max(initIteration, lastIteration);
                lastIteration = min(initIteration, lastIteration);
            }
            if (idx == myTP->TPsToUse3550 - 1) {
                lastIteration = (*(this->inputsTPParent->fSize_darts3548));
            }
#if USEINVOKE == 1
            invoke<TP3550>(myTP, myTP->codeletsPerTP3550 * DARTS_CODELETS_MULT, this->getID(), myTP,
                initIteration, lastIteration, &(*(this->inputsTPParent->fSize_darts3548)),
                &(*(this->inputsTPParent->s_darts3548)), &(myTP->TP3550Ptr[idx]));
#else
            place<TP3550>(idx, myTP, myTP->codeletsPerTP3550 * DARTS_CODELETS_MULT, this->getID(),
                myTP, initIteration, lastIteration, &(*(this->inputsTPParent->fSize_darts3548)),
                &(*(this->inputsTPParent->s_darts3548)), &(myTP->TP3550Ptr[idx]));
#endif
        } else {
            if (myTP->TP3550Ptr[idx] != nullptr) {
                myTP->TP3550Ptr[idx]->dispatchCodelet(this->getID());
            } else {
                this->resetCodelet();
                this->decDep();
            }
        }
    }
}
void TP3548::_barrierCodelets3550::fire(void)
{
    TP3548* myTP = static_cast<TP3548*>(myTP_);
    myTP->TPParent->barrierCodelets3548[0].setDep(0);
    myTP->add(&(myTP->TPParent->barrierCodelets3548[0]));
}
TP3548::TP3548(
    int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet, int* in_fSize, SimFlat** in_s)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , fSize_darts3548(in_fSize) /*OMP_SHARED - INPUT*/
    , s_darts3548(in_s) /*OMP_SHARED - INPUT*/
    , TP3550Ptr(new TP3550*[NUMTPS3550])
    , TP3550_alreadyLaunched(new size_t[NUMTPS3550])
    , numTPsSet3550(0)
    , numTPsReady3550(0)
    , TPsToUse3550(NUMTPS3550)
    , codeletsPerTP3550(this->numThreads / NUMTPS3550)
    , totalCodelets3550(this->TPsToUse3550 * this->codeletsPerTP3550)
    , barrierCodelets3548(new _barrierCodelets3548[1])
    , checkInCodelets3550(new _checkInCodelets3550[this->numThreads])
    , barrierCodelets3550(new _barrierCodelets3550[1])
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets3548[0] = _barrierCodelets3548(ompNumThreads, ompNumThreads, this, 0);
    barrierCodelets3550[0] = _barrierCodelets3550(NUMTPS3550, NUMTPS3550, this, 0);
    _checkInCodelets3550* checkInCodelets3550Ptr = (this->checkInCodelets3550);
    for (int i = 0; i < NUMTPS3550; i++) {
        TP3550Ptr[i] = nullptr;
        TP3550_alreadyLaunched[i] = 0;
    }
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets3550Ptr) = _checkInCodelets3550(1, 1, this, codeletCounter);
        (*checkInCodelets3550Ptr).decDep();
        checkInCodelets3550Ptr++;
    }
}
TP3548::~TP3548()
{
    delete[] barrierCodelets3548;
    delete[] barrierCodelets3550;
    delete[] checkInCodelets3550;
}
/*TP3550: OMPForDirective*/
void TP3550::_barrierCodelets3550::fire(void)
{
    TP3550* myTP = static_cast<TP3550*>(myTP_);
    myTP->controlTPParent->TPParent->barrierCodelets3550[0].decDep();
}
bool TP3550::requestNewRangeIterations3550(int* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Static */
    /*Chunk = 0*/
    bool isThereNewIteration = false;
    {
        /*Static Scheduling*/
        int tempStartRange = rangePerCodelet3550 * codeletID;
        int tempEndRange = rangePerCodelet3550 * (codeletID + 1);
        if (remainderRange3550 != 0) {
            if (codeletID < (uint32_t)remainderRange3550) {
                tempStartRange += codeletID;
                tempEndRange += (codeletID + 1);
            } else {
                tempStartRange += remainderRange3550;
                tempEndRange += remainderRange3550;
            }
        }
        tempStartRange = tempStartRange * 1 + minIteration3550;
        tempEndRange = tempEndRange * 1 + minIteration3550;
        if (tempStartRange != tempEndRange) {
            isThereNewIteration = true;
        }
        if (initIteration3550 < lastIteration3550) {
            (this->inputsTPParent->ii_darts3550[codeletID]) = min(tempStartRange, tempEndRange);
            *endRange = max(tempStartRange, tempEndRange);
        } else {
            (this->inputsTPParent->ii_darts3550[codeletID]) = max(tempStartRange, tempEndRange);
            *endRange = min(tempStartRange, tempEndRange);
        }
        if (codeletID == this->numThreads - 1) {
            *endRange = lastIteration3550;
        }
    }
    return isThereNewIteration;
}
void TP3550::_checkInCodelets3551::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    /*Init the vars for this region*/

    /*printing node 3551: ForStmt*/
    /*var: fSize*/
    /*var: s*/
    SimFlat** s = (this->inputsTPParent->s_darts3550);
    (void)s /*OMP_SHARED*/;
    int* ii = &(this->inputsTPParent->ii_darts3550[this->getLocalID()]);
    (void)ii /*PRIVATE*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations3550(
        (int*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        /*Signaling omp for stmt's barrier*/
        myTP->controlTPParent->barrierCodelets3550[0].decDep();
        return;
    }
    for (int ii_darts_counter_temp3550 = (*ii); ii_darts_counter_temp3550 < endRange
         && ii_darts_counter_temp3550 < this->inputsTPParent->lastIteration3550;
         ++ii_darts_counter_temp3550) {
        {
            zeroReal3((*(s))->atoms->f[(ii_darts_counter_temp3550)]);
            (*(s))->atoms->U[(ii_darts_counter_temp3550)] = 0.;
        }
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling omp for stmt's barrier*/
    myTP->controlTPParent->barrierCodelets3550[0].decDep();
}
TP3550::TP3550(int in_numThreads, int in_mainCodeletID, TP3548* in_TPParent, int in_initIteration,
    int in_lastIteration, int* in_fSize, SimFlat** in_s, TP3550** in_ptrToThisTP)
    : ompTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , fSize_darts3550(in_fSize) /*OMP_SHARED - INPUT*/
    , s_darts3550(in_s) /*OMP_SHARED - INPUT*/
    , ii_darts3550(new int[this->numThreads]) /*VARIABLE*/
    , initIteration3550(in_initIteration)
    , lastIteration3550(in_lastIteration)
    , readyCodelets(0)
    , baseNumThreads(this->numThreads / DARTS_CODELETS_MULT)
    , barrierCodelets3550(new _barrierCodelets3550[1])
    , checkInCodelets3551(new _checkInCodelets3551[this->numThreads])
{
    /*Initialize the loop parameters*/
    range3550 = abs(lastIteration3550 - initIteration3550) / 1;
    rangePerCodelet3550 = range3550 / numThreads;
    minIteration3550 = min<int>(lastIteration3550, initIteration3550);
    remainderRange3550 = range3550 % numThreads;
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets3550[0] = _barrierCodelets3550(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets3551* checkInCodelets3551Ptr = (this->checkInCodelets3551);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets3551);
#endif
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->baseNumThreads;
         codeletCounter++) {
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets3551Ptr) = _checkInCodelets3551(2, 1, this, codeletCounter);
#else
        (*checkInCodelets3551Ptr) = _checkInCodelets3551(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets3551Ptr).decDep();
        checkInCodelets3551Ptr++;
    }
    this->dispatchCodelet(this->getID());
    *(in_ptrToThisTP) = this;
}
void TP3550::dispatchCodelet(size_t codeletID)
{
    int idx = codeletID / this->baseNumThreads;
    int localID = codeletID - this->baseNumThreads * idx;
    this->checkInCodelets3551[localID].setID(codeletID);
    this->checkInCodelets3551[localID].setLocalID(localID);
    /*Check if we want to replicate codelets*/
    for (size_t i = 1; i < (size_t)DARTS_CODELETS_MULT; i++) {
#if USE_SPIN_CODELETS == 0
        this->checkInCodelets3551[localID + this->baseNumThreads * i]
            = _checkInCodelets3551(2, 1, this, localID + this->baseNumThreads * i);
#else
        this->checkInCodelets3551[localID + this->baseNumThreads * i]
            = _checkInCodelets3551(1, 1, this, localID + this->baseNumThreads * i);
#endif
        this->checkInCodelets3551[localID + this->baseNumThreads * i].setID(codeletID);
        this->checkInCodelets3551[localID + this->baseNumThreads * i].decDep();
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
TP3550::~TP3550()
{
    delete[] ii_darts3550;
    delete[] barrierCodelets3550;
    delete[] checkInCodelets3551;
}
/*TP3606: OMPParallelDirective*/
void TP3606::_barrierCodelets3606::fire(void)
{
    TP3606* myTP = static_cast<TP3606*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
void TP3606::_checkInCodelets3608::fire(void)
{
    /*printOMPForCodeletFireCode, inlined: 0*/
    /*region 3608 0*/
    /*Determine the TP to which this codelet belongs and check if this codelet spawns the TP or if
     * it signals it using dispatchCodelet()*/
    size_t idx = this->getID() / myTP->codeletsPerTP3608;
    if (idx < myTP->TPsToUse3608) {
        if (!__sync_val_compare_and_swap(&(myTP->TP3608_alreadyLaunched[idx]), 0, 1)) {
            int range = abs((*(this->inputsTPParent->s_darts3606))->boxes->nLocalBoxes - 0) / 1;
            int rangePerCodelet = range / myTP->TPsToUse3608;
            int minIteration
                = min<int>((*(this->inputsTPParent->s_darts3606))->boxes->nLocalBoxes, 0);
            int remainderRange = range % myTP->TPsToUse3608;
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
            if (0 < (*(this->inputsTPParent->s_darts3606))->boxes->nLocalBoxes) {
                initIteration = min(initIteration, lastIteration);
                lastIteration = max(initIteration, lastIteration);
            } else {
                initIteration = max(initIteration, lastIteration);
                lastIteration = min(initIteration, lastIteration);
            }
            if (idx == myTP->TPsToUse3608 - 1) {
                lastIteration = (*(this->inputsTPParent->s_darts3606))->boxes->nLocalBoxes;
            }
#if USEINVOKE == 1
            invoke<TP3608>(myTP, myTP->codeletsPerTP3608 * DARTS_CODELETS_MULT, this->getID(), myTP,
                initIteration, lastIteration, &(*(this->inputsTPParent->ePot_darts3606)),
                &(*(this->inputsTPParent->eShift_darts3606)),
                &(*(this->inputsTPParent->epsilon_darts3606)),
                &(*(this->inputsTPParent->rCut2_darts3606)),
                &(*(this->inputsTPParent->s_darts3606)), &(*(this->inputsTPParent->s6_darts3606)),
                &(myTP->TP3608Ptr[idx]));
#else
            place<TP3608>(idx, myTP, myTP->codeletsPerTP3608 * DARTS_CODELETS_MULT, this->getID(),
                myTP, initIteration, lastIteration, &(*(this->inputsTPParent->ePot_darts3606)),
                &(*(this->inputsTPParent->eShift_darts3606)),
                &(*(this->inputsTPParent->epsilon_darts3606)),
                &(*(this->inputsTPParent->rCut2_darts3606)),
                &(*(this->inputsTPParent->s_darts3606)), &(*(this->inputsTPParent->s6_darts3606)),
                &(myTP->TP3608Ptr[idx]));
#endif
        } else {
            if (myTP->TP3608Ptr[idx] != nullptr) {
                myTP->TP3608Ptr[idx]->dispatchCodelet(this->getID());
            } else {
                this->resetCodelet();
                this->decDep();
            }
        }
    }
}
void TP3606::_barrierCodelets3608::fire(void)
{
    TP3606* myTP = static_cast<TP3606*>(myTP_);
    myTP->TPParent->barrierCodelets3606[0].setDep(0);
    myTP->add(&(myTP->TPParent->barrierCodelets3606[0]));
}
TP3606::TP3606(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet, real_t* in_ePot,
    real_t* in_eShift, real_t* in_epsilon, real_t* in_rCut2, SimFlat** in_s, real_t* in_s6)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , ePot_darts3606(in_ePot) /*OMP_SHARED - INPUT*/
    , eShift_darts3606(in_eShift) /*OMP_SHARED - INPUT*/
    , epsilon_darts3606(in_epsilon) /*OMP_SHARED - INPUT*/
    , rCut2_darts3606(in_rCut2) /*OMP_SHARED - INPUT*/
    , s_darts3606(in_s) /*OMP_SHARED - INPUT*/
    , s6_darts3606(in_s6) /*OMP_SHARED - INPUT*/
    , TP3608Ptr(new TP3608*[NUMTPS3608])
    , TP3608_alreadyLaunched(new size_t[NUMTPS3608])
    , numTPsSet3608(0)
    , numTPsReady3608(0)
    , TPsToUse3608(NUMTPS3608)
    , codeletsPerTP3608(this->numThreads / NUMTPS3608)
    , totalCodelets3608(this->TPsToUse3608 * this->codeletsPerTP3608)
    , barrierCodelets3606(new _barrierCodelets3606[1])
    , checkInCodelets3608(new _checkInCodelets3608[this->numThreads])
    , barrierCodelets3608(new _barrierCodelets3608[1])
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets3606[0] = _barrierCodelets3606(ompNumThreads, ompNumThreads, this, 0);
    barrierCodelets3608[0] = _barrierCodelets3608(NUMTPS3608, NUMTPS3608, this, 0);
    _checkInCodelets3608* checkInCodelets3608Ptr = (this->checkInCodelets3608);
    for (int i = 0; i < NUMTPS3608; i++) {
        TP3608Ptr[i] = nullptr;
        TP3608_alreadyLaunched[i] = 0;
    }
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets3608Ptr) = _checkInCodelets3608(1, 1, this, codeletCounter);
        (*checkInCodelets3608Ptr).decDep();
        checkInCodelets3608Ptr++;
    }
}
TP3606::~TP3606()
{
    delete[] barrierCodelets3606;
    delete[] barrierCodelets3608;
    delete[] checkInCodelets3608;
}
/*TP3608: OMPForDirective*/
void TP3608::_barrierCodelets3608::fire(void)
{
    TP3608* myTP = static_cast<TP3608*>(myTP_);
    myTP->controlTPParent->TPParent->barrierCodelets3608[0].decDep();
}
bool TP3608::requestNewRangeIterations3608(int* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Static */
    /*Chunk = 0*/
    bool isThereNewIteration = false;
    {
        /*Static Scheduling*/
        int tempStartRange = rangePerCodelet3608 * codeletID;
        int tempEndRange = rangePerCodelet3608 * (codeletID + 1);
        if (remainderRange3608 != 0) {
            if (codeletID < (uint32_t)remainderRange3608) {
                tempStartRange += codeletID;
                tempEndRange += (codeletID + 1);
            } else {
                tempStartRange += remainderRange3608;
                tempEndRange += remainderRange3608;
            }
        }
        tempStartRange = tempStartRange * 1 + minIteration3608;
        tempEndRange = tempEndRange * 1 + minIteration3608;
        if (tempStartRange != tempEndRange) {
            isThereNewIteration = true;
        }
        if (initIteration3608 < lastIteration3608) {
            (this->inputsTPParent->iBox_darts3608[codeletID]) = min(tempStartRange, tempEndRange);
            *endRange = max(tempStartRange, tempEndRange);
        } else {
            (this->inputsTPParent->iBox_darts3608[codeletID]) = max(tempStartRange, tempEndRange);
            *endRange = min(tempStartRange, tempEndRange);
        }
        if (codeletID == this->numThreads - 1) {
            *endRange = lastIteration3608;
        }
    }
    return isThereNewIteration;
}
void TP3608::_checkInCodelets3609::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    /*Init the vars for this region*/

    /*printing node 3609: ForStmt*/
    /*var: ePot*/
    /*var: eShift*/
    /*var: epsilon*/
    /*var: rCut2*/
    /*var: s*/
    /*var: s6*/
    /*Declare local reduction vars*/
    real_t ePot_LocalReductionVar = (real_t)0;
    real_t* eShift = (this->inputsTPParent->eShift_darts3608);
    (void)eShift /*OMP_SHARED*/;
    real_t* epsilon = (this->inputsTPParent->epsilon_darts3608);
    (void)epsilon /*OMP_SHARED*/;
    real_t* rCut2 = (this->inputsTPParent->rCut2_darts3608);
    (void)rCut2 /*OMP_SHARED*/;
    SimFlat** s = (this->inputsTPParent->s_darts3608);
    (void)s /*OMP_SHARED*/;
    real_t* s6 = (this->inputsTPParent->s6_darts3608);
    (void)s6 /*OMP_SHARED*/;
    int* iBox = &(this->inputsTPParent->iBox_darts3608[this->getLocalID()]);
    (void)iBox /*PRIVATE*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations3608(
        (int*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        goto updateReduceVars;
    }
    for (int iBox_darts_counter_temp3608 = (*iBox); iBox_darts_counter_temp3608 < endRange
         && iBox_darts_counter_temp3608 < this->inputsTPParent->lastIteration3608;
         iBox_darts_counter_temp3608++) {
        {
            int nIBox = (*(s))->boxes->nAtoms[(iBox_darts_counter_temp3608)];
            {
                /*Loop's init*/
                int jTmp = 0;
                for (; jTmp < 27; jTmp++) {
                    int jBox = (*(s))->boxes->nbrBoxes[(iBox_darts_counter_temp3608)][jTmp];
                    int nJBox = (*(s))->boxes->nAtoms[jBox];
                    {
                        /*Loop's init*/
                        int iOff = 64 * (iBox_darts_counter_temp3608);
                        for (; iOff < ((iBox_darts_counter_temp3608)*64 + nIBox); iOff++) {
                            real_t Uptr;
                            {
                                /*Loop's init*/
                                int jOff = jBox * 64;
                                for (; jOff < (jBox * 64 + nJBox); jOff++) {
                                    real3 dr;
                                    real_t r2 = 0.;
                                    {
                                        /*Loop's init*/
                                        int m = 0;
                                        for (; m < 3; m++) {
                                            dr[m] = (*(s))->atoms->r[iOff][m]
                                                - (*(s))->atoms->r[jOff][m];
                                            r2 += dr[m] * dr[m];
                                        }
                                    }
                                    if (r2 <= (*(rCut2)) && r2 > 0.) {
                                        r2 = 1. / r2;
                                        real_t r6 = (*(s6)) * (r2 * r2 * r2);
                                        real_t eLocal = r6 * (r6 - 1.) - (*(eShift));
                                        Uptr += 0.5 * eLocal;
                                        (*(&(ePot_LocalReductionVar))) += 0.5 * eLocal;
                                        real_t fr = -4. * (*(epsilon)) * r6 * r2 * (12. * r6 - 6.);
                                        {
                                            /*Loop's init*/
                                            int m = 0;
                                            for (; m < 3; m++) {
                                                (*(s))->atoms->f[iOff][m] -= dr[m] * fr;
                                            }
                                        }
                                    }
                                }
                            }
                            (*(s))->atoms->U[iOff] = Uptr;
                        }
                    }
                }
            }
        }
    }
updateReduceVars:
    /*Before signaling next codelet, check if all the threads in the TP
    * performed the reduction locally and if so, update the unique
    * reduction variable.*/
    /*Reducing for operator '+'*/
    this->inputsTPParent->ePot_darts3608_mutex->lock();
    *(this->inputsTPParent->ePot_darts3608) += (ePot_LocalReductionVar);
    this->inputsTPParent->ePot_darts3608_mutex->unlock();
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling omp for stmt's barrier*/
    myTP->controlTPParent->barrierCodelets3608[0].decDep();
}
TP3608::TP3608(int in_numThreads, int in_mainCodeletID, TP3606* in_TPParent, int in_initIteration,
    int in_lastIteration, real_t* in_ePot, real_t* in_eShift, real_t* in_epsilon, real_t* in_rCut2,
    SimFlat** in_s, real_t* in_s6, TP3608** in_ptrToThisTP)
    : ompTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , ePot_darts3608(in_ePot) /*OMP_SHARED - INPUT*/
    , eShift_darts3608(in_eShift) /*OMP_SHARED - INPUT*/
    , epsilon_darts3608(in_epsilon) /*OMP_SHARED - INPUT*/
    , rCut2_darts3608(in_rCut2) /*OMP_SHARED - INPUT*/
    , s_darts3608(in_s) /*OMP_SHARED - INPUT*/
    , s6_darts3608(in_s6) /*OMP_SHARED - INPUT*/
    , iBox_darts3608(new int[this->numThreads]) /*VARIABLE*/
    , initIteration3608(in_initIteration)
    , lastIteration3608(in_lastIteration)
    , readyCodelets(0)
    , baseNumThreads(this->numThreads / DARTS_CODELETS_MULT)
    , ePot_darts3608_mutex(&(this->TPParent->ePot_darts3608_mutex))
    , barrierCodelets3608(new _barrierCodelets3608[1])
    , checkInCodelets3609(new _checkInCodelets3609[this->numThreads])
{
    /*Initialize the loop parameters*/
    range3608 = abs(lastIteration3608 - initIteration3608) / 1;
    rangePerCodelet3608 = range3608 / numThreads;
    minIteration3608 = min<int>(lastIteration3608, initIteration3608);
    remainderRange3608 = range3608 % numThreads;
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets3608[0] = _barrierCodelets3608(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets3609* checkInCodelets3609Ptr = (this->checkInCodelets3609);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets3609);
#endif
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->baseNumThreads;
         codeletCounter++) {
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets3609Ptr) = _checkInCodelets3609(2, 1, this, codeletCounter);
#else
        (*checkInCodelets3609Ptr) = _checkInCodelets3609(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets3609Ptr).decDep();
        checkInCodelets3609Ptr++;
    }
    this->dispatchCodelet(this->getID());
    *(in_ptrToThisTP) = this;
}
void TP3608::dispatchCodelet(size_t codeletID)
{
    int idx = codeletID / this->baseNumThreads;
    int localID = codeletID - this->baseNumThreads * idx;
    this->checkInCodelets3609[localID].setID(codeletID);
    this->checkInCodelets3609[localID].setLocalID(localID);
    /*Check if we want to replicate codelets*/
    for (size_t i = 1; i < (size_t)DARTS_CODELETS_MULT; i++) {
#if USE_SPIN_CODELETS == 0
        this->checkInCodelets3609[localID + this->baseNumThreads * i]
            = _checkInCodelets3609(2, 1, this, localID + this->baseNumThreads * i);
#else
        this->checkInCodelets3609[localID + this->baseNumThreads * i]
            = _checkInCodelets3609(1, 1, this, localID + this->baseNumThreads * i);
#endif
        this->checkInCodelets3609[localID + this->baseNumThreads * i].setID(codeletID);
        this->checkInCodelets3609[localID + this->baseNumThreads * i].decDep();
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
TP3608::~TP3608()
{
    delete[] iBox_darts3608;
    delete[] barrierCodelets3608;
    delete[] checkInCodelets3609;
}
