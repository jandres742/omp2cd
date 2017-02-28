#include "eam.output.darts.h"
using namespace darts;
using namespace std;
static void fileNotFound(const char* callSite, const char* filename);
static void typeNotSupported(const char* callSite, const char* type);
static void eamReadFuncfl(EamPotential* pot, const char* dir, const char* potName);
static void eamReadSetfl(EamPotential* pot, const char* dir, const char* potName);
static void notAlloyReady(const char* callSite);
static void eamPrint(FILE* file, BasePotential* pot);
static void eamDestroy(BasePotential** pot);
static InterpolationObject* initInterpolationObject(int n, real_t x0, real_t dx, real_t* data);
static void printTableData(InterpolationObject* table, const char* fileName);
static int eamForce(SimFlat* s);
static void eamBcastPotential(EamPotential* pot);
static void destroyInterpolationObject(InterpolationObject** table);
static void interpolate(InterpolationObject* table, real_t r, real_t* f, real_t* df);
static void bcastInterpolationObject(InterpolationObject** table);
/*Function: initEamPot, ID: 54*/
struct BasePotentialSt* initEamPot(const char* dir, const char* file, const char* type)
{
    /*initEamPot:54*/
    /*CompoundStmt:2436*/
    EamPotential* pot = comdMalloc(sizeof(EamPotential));
    pot->force = eamForce;
    pot->print = eamPrint;
    pot->destroy = eamDestroy;
    pot->phi = ((void*)0);
    pot->rho = ((void*)0);
    pot->f = ((void*)0);
    pot->dfEmbed = ((void*)0);
    pot->rhobar = ((void*)0);
    pot->forceExchange = ((void*)0);
    if (getMyRank() == 0) {
        if (strcmp(type, "setfl") == 0)
            eamReadSetfl(pot, dir, file);
        else if (strcmp(type, "funcfl") == 0)
            eamReadFuncfl(pot, dir, file);
        else
            typeNotSupported("initEamPot", type);
    }
    eamBcastPotential(pot);
    return (BasePotential*)pot;
}
/*Function: eamForce, ID: 40*/
int eamForce(SimFlat* s)
{
    /*eamForce:40*/
    /*CompoundStmt:2481*/
    EamPotential* pot = (EamPotential*)s->pot;
    if (pot->forceExchange == ((void*)0)) {
        int maxTotalAtoms = 64 * s->boxes->nTotalBoxes;
        pot->dfEmbed = comdMalloc(maxTotalAtoms * sizeof(real_t));
        pot->rhobar = comdMalloc(maxTotalAtoms * sizeof(real_t));
        pot->forceExchange = initForceHaloExchange(s->domain, s->boxes);
        pot->forceExchangeData = comdMalloc(sizeof(ForceExchangeData));
        pot->forceExchangeData->dfEmbed = pot->dfEmbed;
        pot->forceExchangeData->boxes = s->boxes;
    }
    real_t rCut2 = pot->cutoff * pot->cutoff;
    real_t etot = 0.;
    int fsize = s->boxes->nTotalBoxes * 64;
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP2529>(ompNumThreads * DARTS_CODELETS_MULT, 0,
            RuntimeFinalCodelet, (int*)&((fsize)), (EamPotential**)&((pot)), (SimFlat**)&((s))));
    }
    int nNbrBoxes = 27;
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP2579>(ompNumThreads * DARTS_CODELETS_MULT, 0,
            RuntimeFinalCodelet, (real_t*)&((etot)), (int*)&((nNbrBoxes)), (EamPotential**)&((pot)),
            (real_t*)&((rCut2)), (SimFlat**)&((s))));
    }
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP2709>(ompNumThreads * DARTS_CODELETS_MULT, 0,
            RuntimeFinalCodelet, (real_t*)&((etot)), (EamPotential**)&((pot)), (SimFlat**)&((s))));
    }
    do {
        profileStart(eamHaloTimer);
    } while (0);
    haloExchange(pot->forceExchange, pot->forceExchangeData);
    do {
        profileStop(eamHaloTimer);
    } while (0);
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP2781>(ompNumThreads * DARTS_CODELETS_MULT, 0,
            RuntimeFinalCodelet, (int*)&((nNbrBoxes)), (EamPotential**)&((pot)),
            (real_t*)&((rCut2)), (SimFlat**)&((s))));
    }
    s->ePotential = (real_t)etot;
    return 0;
}
/*Function: eamPrint, ID: 41*/
void eamPrint(FILE* file, BasePotential* pot)
{
    /*eamPrint:41*/
    /*CompoundStmt:2905*/
    EamPotential* eamPot = (EamPotential*)pot;
    fprintf(file, "  Potential type  : EAM\n");
    fprintf(file, "  Species name    : %s\n", eamPot->name);
    fprintf(file, "  Atomic number   : %d\n", eamPot->atomicNo);
    fprintf(file, "  Mass            : %lg amu\n", eamPot->mass / amuToInternalMass);
    fprintf(file, "  Lattice type    : %s\n", eamPot->latticeType);
    fprintf(file, "  Lattice spacing : %lg Angstroms\n", eamPot->lat);
    fprintf(file, "  Cutoff          : %lg Angstroms\n", eamPot->cutoff);
}
/*Function: eamDestroy, ID: 42*/
void eamDestroy(BasePotential** pPot)
{
    /*eamDestroy:42*/
    /*CompoundStmt:2922*/
    if (!pPot)
        return;
    EamPotential* pot = *(EamPotential**)pPot;
    if (!pot)
        return;
    destroyInterpolationObject(&(pot->phi));
    destroyInterpolationObject(&(pot->rho));
    destroyInterpolationObject(&(pot->f));
    destroyHaloExchange(&(pot->forceExchange));
    comdFree(pot);
    *pPot = ((void*)0);
    return;
}
/*Function: eamBcastPotential, ID: 43*/
void eamBcastPotential(EamPotential* pot)
{
    /*eamBcastPotential:43*/
    /*CompoundStmt:2951*/
    struct {
        real_t cutoff;
        real_t mass;
        real_t lat;
        char latticeType[8];
        char name[3];
        int atomicNo;
    } buf;
    if (getMyRank() == 0) {
        buf.cutoff = pot->cutoff;
        buf.mass = pot->mass;
        buf.lat = pot->lat;
        buf.atomicNo = pot->atomicNo;
        strcpy(buf.latticeType, pot->latticeType);
        strcpy(buf.name, pot->name);
    }
    bcastParallel(&buf, sizeof(buf), 0);
    pot->cutoff = buf.cutoff;
    pot->mass = buf.mass;
    pot->lat = buf.lat;
    pot->atomicNo = buf.atomicNo;
    strcpy(pot->latticeType, buf.latticeType);
    strcpy(pot->name, buf.name);
    bcastInterpolationObject(&pot->phi);
    bcastInterpolationObject(&pot->rho);
    bcastInterpolationObject(&pot->f);
}
/*Function: initInterpolationObject, ID: 44*/
InterpolationObject* initInterpolationObject(int n, real_t x0, real_t dx, real_t* data)
{
    /*initInterpolationObject:44*/
    /*CompoundStmt:3004*/
    InterpolationObject* table = (InterpolationObject*)comdMalloc(sizeof(InterpolationObject));
    table->values = (real_t*)comdCalloc(1, (n + 3) * sizeof(real_t));
    table->values++;
    table->n = n;
    table->invDx = 1. / dx;
    table->x0 = x0;
    for (int ii = 0; ii < n; ++ii)
        table->values[ii] = data[ii];
    table->values[-1] = table->values[0];
    table->values[n + 1] = table->values[n] = table->values[n - 1];
    return table;
}
/*Function: destroyInterpolationObject, ID: 45*/
void destroyInterpolationObject(InterpolationObject** a)
{
    /*destroyInterpolationObject:45*/
    /*CompoundStmt:3051*/
    if (!a)
        return;
    if (!*a)
        return;
    if ((*a)->values) {
        (*a)->values--;
        comdFree((*a)->values);
    }
    comdFree(*a);
    *a = ((void*)0);
    return;
}
/*Function: interpolate, ID: 46*/
void interpolate(InterpolationObject* table, real_t r, real_t* f, real_t* df)
{
    /*interpolate:46*/
    /*CompoundStmt:3075*/
    const real_t* tt = table->values;
    if (r < table->x0)
        r = table->x0;
    r = (r - table->x0) * (table->invDx);
    int ii = (int)floor(r);
    if (ii > table->n) {
        ii = table->n;
        r = table->n / table->invDx;
    }
    r = r - floor(r);
    real_t g1 = tt[ii + 1] - tt[ii - 1];
    real_t g2 = tt[ii + 2] - tt[ii];
    *f = tt[ii] + 0.5 * r * (g1 + r * (tt[ii + 1] + tt[ii - 1] - 2. * tt[ii]));
    *df = 0.5 * (g1 + r * (g2 - g1)) * table->invDx;
}
/*Function: bcastInterpolationObject, ID: 47*/
void bcastInterpolationObject(InterpolationObject** table)
{
    /*bcastInterpolationObject:47*/
    /*CompoundStmt:3141*/
    struct {
        int n;
        real_t x0;
        real_t invDx;
    } buf;
    if (getMyRank() == 0) {
        buf.n = (*table)->n;
        buf.x0 = (*table)->x0;
        buf.invDx = (*table)->invDx;
    }
    bcastParallel(&buf, sizeof(buf), 0);
    if (getMyRank() != 0) {
        *table = comdMalloc(sizeof(InterpolationObject));
        (*table)->n = buf.n;
        (*table)->x0 = buf.x0;
        (*table)->invDx = buf.invDx;
        (*table)->values = comdMalloc(sizeof(real_t) * (buf.n + 3));
        (*table)->values++;
    }
    int valuesSize = sizeof(real_t) * ((*table)->n + 3);
    bcastParallel((*table)->values - 1, valuesSize, 0);
}
/*Function: printTableData, ID: 48*/
void printTableData(InterpolationObject* table, const char* fileName)
{
    /*printTableData:48*/
    /*CompoundStmt:3201*/
    if (!printRank())
        return;
    FILE* potData;
    potData = fopen(fileName, "w");
    real_t dR = 1. / table->invDx;
    for (int i = 0; i < table->n; i++) {
        real_t r = table->x0 + i * dR;
        fprintf(potData, "%d %e %e\n", i, r, table->values[i]);
    }
    fclose(potData);
}
/*Function: eamReadSetfl, ID: 49*/
void eamReadSetfl(EamPotential* pot, const char* dir, const char* potName)
{
    /*eamReadSetfl:49*/
    /*CompoundStmt:3227*/
    char tmp[4096];
    sprintf(tmp, "%s/%s", dir, potName);
    FILE* potFile = fopen(tmp, "r");
    if (potFile == ((void*)0))
        fileNotFound("eamReadSetfl", tmp);
    fgets(tmp, sizeof(tmp), potFile);
    fgets(tmp, sizeof(tmp), potFile);
    fgets(tmp, sizeof(tmp), potFile);
    fgets(tmp, sizeof(tmp), potFile);
    int nElems;
    sscanf(tmp, "%d", &nElems);
    if (nElems != 1)
        notAlloyReady("eamReadSetfl");
    int nRho, nR;
    double dRho, dR, cutoff;
    fgets(tmp, sizeof(tmp), potFile);
    sscanf(tmp, "%d %le %d %le %le", &nRho, &dRho, &nR, &dR, &cutoff);
    pot->cutoff = cutoff;
    fgets(tmp, sizeof(tmp), potFile);
    int nAtomic;
    double mass, lat;
    char latticeType[8];
    sscanf(tmp, "%d %le %le %s", &nAtomic, &mass, &lat, latticeType);
    pot->atomicNo = nAtomic;
    pot->lat = lat;
    pot->mass = mass * amuToInternalMass;
    strcpy(pot->latticeType, latticeType);
    int bufSize = ((nRho) > (nR) ? (nRho) : (nR));
    real_t* buf = comdMalloc(bufSize * sizeof(real_t));
    real_t x0 = 0.;
    for (int ii = 0; ii < nRho; ++ii)
        fscanf(potFile, "%lg", buf + ii);
    pot->f = initInterpolationObject(nRho, x0, dRho, buf);
    for (int ii = 0; ii < nR; ++ii)
        fscanf(potFile, "%lg", buf + ii);
    pot->rho = initInterpolationObject(nR, x0, dR, buf);
    for (int ii = 0; ii < nR; ++ii)
        fscanf(potFile, "%lg", buf + ii);
    for (int ii = 1; ii < nR; ++ii) {
        real_t r = x0 + ii * dR;
        buf[ii] /= r;
    }
    buf[0] = buf[1] + (buf[1] - buf[2]);
    pot->phi = initInterpolationObject(nR, x0, dR, buf);
    comdFree(buf);
}
/*Function: eamReadFuncfl, ID: 50*/
void eamReadFuncfl(EamPotential* pot, const char* dir, const char* potName)
{
    /*eamReadFuncfl:50*/
    /*CompoundStmt:3335*/
    char tmp[4096];
    sprintf(tmp, "%s/%s", dir, potName);
    FILE* potFile = fopen(tmp, "r");
    if (potFile == ((void*)0))
        fileNotFound("eamReadFuncfl", tmp);
    fgets(tmp, sizeof(tmp), potFile);
    char name[3];
    sscanf(tmp, "%s", name);
    strcpy(pot->name, name);
    int nAtomic;
    double mass, lat;
    char latticeType[8];
    fgets(tmp, sizeof(tmp), potFile);
    sscanf(tmp, "%d %le %le %s", &nAtomic, &mass, &lat, latticeType);
    pot->atomicNo = nAtomic;
    pot->lat = lat;
    pot->mass = mass * amuToInternalMass;
    strcpy(pot->latticeType, latticeType);
    int nRho, nR;
    double dRho, dR, cutoff;
    fgets(tmp, sizeof(tmp), potFile);
    sscanf(tmp, "%d %le %d %le %le", &nRho, &dRho, &nR, &dR, &cutoff);
    pot->cutoff = cutoff;
    real_t x0 = 0.;
    int bufSize = ((nRho) > (nR) ? (nRho) : (nR));
    real_t* buf = comdMalloc(bufSize * sizeof(real_t));
    for (int ii = 0; ii < nRho; ++ii)
        fscanf(potFile, "%lg", buf + ii);
    pot->f = initInterpolationObject(nRho, x0, dRho, buf);
    for (int ii = 0; ii < nR; ++ii)
        fscanf(potFile, "%lg", buf + ii);
    for (int ii = 1; ii < nR; ++ii) {
        real_t r = x0 + ii * dR;
        buf[ii] *= buf[ii] / r;
        buf[ii] *= hartreeToEv * bohrToAngs;
    }
    buf[0] = buf[1] + (buf[1] - buf[2]);
    pot->phi = initInterpolationObject(nR, x0, dR, buf);
    for (int ii = 0; ii < nR; ++ii)
        fscanf(potFile, "%lg", buf + ii);
    pot->rho = initInterpolationObject(nR, x0, dR, buf);
    comdFree(buf);
}
/*Function: fileNotFound, ID: 51*/
void fileNotFound(const char* callSite, const char* filename)
{
    /*fileNotFound:51*/
    /*CompoundStmt:3439*/
    fprintf(stdout, "%s: Can't open file %s.  Fatal Error.\n", callSite, filename);
    exit(-1);
}
/*Function: notAlloyReady, ID: 52*/
void notAlloyReady(const char* callSite)
{
    /*notAlloyReady:52*/
    /*CompoundStmt:3443*/
    fprintf(stdout, "%s: CoMD 1.1 does not support alloys and cannot\n   read setfl files with "
                    "multiple species.  Fatal Error.\n",
        callSite);
    exit(-1);
}
/*Function: typeNotSupported, ID: 53*/
void typeNotSupported(const char* callSite, const char* type)
{
    /*typeNotSupported:53*/
    /*CompoundStmt:3447*/
    fprintf(stdout, "%s: Potential type %s not supported. Fatal Error.\n", callSite, type);
    exit(-1);
}
/*TP2529: OMPParallelDirective*/
void TP2529::_barrierCodelets2529::fire(void)
{
    TP2529* myTP = static_cast<TP2529*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
void TP2529::_checkInCodelets2531::fire(void)
{
    /*printOMPForCodeletFireCode, inlined: 0*/
    /*region 2531 0*/
    /*Determine the TP to which this codelet belongs and check if this codelet spawns the TP or if
     * it signals it using dispatchCodelet()*/
    size_t idx = this->getID() / myTP->codeletsPerTP2531;
    if (idx < myTP->TPsToUse2531) {
        if (!__sync_val_compare_and_swap(&(myTP->TP2531_alreadyLaunched[idx]), 0, 1)) {
            int range = abs((*(this->inputsTPParent->fsize_darts2529)) - 0) / 1;
            int rangePerCodelet = range / myTP->TPsToUse2531;
            int minIteration = min<int>((*(this->inputsTPParent->fsize_darts2529)), 0);
            int remainderRange = range % myTP->TPsToUse2531;
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
            if (0 < (*(this->inputsTPParent->fsize_darts2529))) {
                initIteration = min(initIteration, lastIteration);
                lastIteration = max(initIteration, lastIteration);
            } else {
                initIteration = max(initIteration, lastIteration);
                lastIteration = min(initIteration, lastIteration);
            }
            if (idx == myTP->TPsToUse2531 - 1) {
                lastIteration = (*(this->inputsTPParent->fsize_darts2529));
            }
#if USEINVOKE == 1
            invoke<TP2531>(myTP, myTP->codeletsPerTP2531 * DARTS_CODELETS_MULT, this->getID(), myTP,
                initIteration, lastIteration, &(*(this->inputsTPParent->fsize_darts2529)),
                &(*(this->inputsTPParent->pot_darts2529)), &(*(this->inputsTPParent->s_darts2529)),
                &(myTP->TP2531Ptr[idx]));
#else
            place<TP2531>(idx, myTP, myTP->codeletsPerTP2531 * DARTS_CODELETS_MULT, this->getID(),
                myTP, initIteration, lastIteration, &(*(this->inputsTPParent->fsize_darts2529)),
                &(*(this->inputsTPParent->pot_darts2529)), &(*(this->inputsTPParent->s_darts2529)),
                &(myTP->TP2531Ptr[idx]));
#endif
        } else {
            if (myTP->TP2531Ptr[idx] != nullptr) {
                myTP->TP2531Ptr[idx]->dispatchCodelet(this->getID());
            } else {
                this->resetCodelet();
                this->decDep();
            }
        }
    }
}
void TP2529::_barrierCodelets2531::fire(void)
{
    TP2529* myTP = static_cast<TP2529*>(myTP_);
    myTP->TPParent->barrierCodelets2529[0].setDep(0);
    myTP->add(&(myTP->TPParent->barrierCodelets2529[0]));
}
TP2529::TP2529(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet, int* in_fsize,
    EamPotential** in_pot, SimFlat** in_s)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , fsize_darts2529(in_fsize) /*OMP_SHARED - INPUT*/
    , pot_darts2529(in_pot) /*OMP_SHARED - INPUT*/
    , s_darts2529(in_s) /*OMP_SHARED - INPUT*/
    , TP2531Ptr(new TP2531*[NUMTPS2531])
    , TP2531_alreadyLaunched(new size_t[NUMTPS2531])
    , numTPsSet2531(0)
    , numTPsReady2531(0)
    , TPsToUse2531(NUMTPS2531)
    , codeletsPerTP2531(this->numThreads / NUMTPS2531)
    , totalCodelets2531(this->TPsToUse2531 * this->codeletsPerTP2531)
    , barrierCodelets2529(new _barrierCodelets2529[1])
    , checkInCodelets2531(new _checkInCodelets2531[this->numThreads])
    , barrierCodelets2531(new _barrierCodelets2531[1])
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets2529[0] = _barrierCodelets2529(ompNumThreads, ompNumThreads, this, 0);
    barrierCodelets2531[0] = _barrierCodelets2531(NUMTPS2531, NUMTPS2531, this, 0);
    _checkInCodelets2531* checkInCodelets2531Ptr = (this->checkInCodelets2531);
    for (int i = 0; i < NUMTPS2531; i++) {
        TP2531Ptr[i] = nullptr;
        TP2531_alreadyLaunched[i] = 0;
    }
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets2531Ptr) = _checkInCodelets2531(1, 1, this, codeletCounter);
        (*checkInCodelets2531Ptr).decDep();
        checkInCodelets2531Ptr++;
    }
}
TP2529::~TP2529()
{
    delete[] barrierCodelets2529;
    delete[] barrierCodelets2531;
    delete[] checkInCodelets2531;
}
/*TP2531: OMPForDirective*/
void TP2531::_barrierCodelets2531::fire(void)
{
    TP2531* myTP = static_cast<TP2531*>(myTP_);
    myTP->controlTPParent->TPParent->barrierCodelets2531[0].decDep();
}
bool TP2531::requestNewRangeIterations2531(int* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Static */
    /*Chunk = 0*/
    bool isThereNewIteration = false;
    {
        /*Static Scheduling*/
        int tempStartRange = rangePerCodelet2531 * codeletID;
        int tempEndRange = rangePerCodelet2531 * (codeletID + 1);
        if (remainderRange2531 != 0) {
            if (codeletID < (uint32_t)remainderRange2531) {
                tempStartRange += codeletID;
                tempEndRange += (codeletID + 1);
            } else {
                tempStartRange += remainderRange2531;
                tempEndRange += remainderRange2531;
            }
        }
        tempStartRange = tempStartRange * 1 + minIteration2531;
        tempEndRange = tempEndRange * 1 + minIteration2531;
        if (tempStartRange != tempEndRange) {
            isThereNewIteration = true;
        }
        if (initIteration2531 < lastIteration2531) {
            (this->inputsTPParent->ii_darts2531[codeletID]) = min(tempStartRange, tempEndRange);
            *endRange = max(tempStartRange, tempEndRange);
        } else {
            (this->inputsTPParent->ii_darts2531[codeletID]) = max(tempStartRange, tempEndRange);
            *endRange = min(tempStartRange, tempEndRange);
        }
        if (codeletID == this->numThreads - 1) {
            *endRange = lastIteration2531;
        }
    }
    return isThereNewIteration;
}
void TP2531::_checkInCodelets2532::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    /*Init the vars for this region*/

    /*printing node 2532: ForStmt*/
    /*var: fsize*/
    /*var: pot*/
    /*var: s*/
    EamPotential** pot = (this->inputsTPParent->pot_darts2531);
    (void)pot /*OMP_SHARED*/;
    SimFlat** s = (this->inputsTPParent->s_darts2531);
    (void)s /*OMP_SHARED*/;
    int* ii = &(this->inputsTPParent->ii_darts2531[this->getLocalID()]);
    (void)ii /*PRIVATE*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations2531(
        (int*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        /*Signaling omp for stmt's barrier*/
        myTP->controlTPParent->barrierCodelets2531[0].decDep();
        return;
    }
    for (int ii_darts_counter_temp2531 = (*ii); ii_darts_counter_temp2531 < endRange
         && ii_darts_counter_temp2531 < this->inputsTPParent->lastIteration2531;
         ii_darts_counter_temp2531++) {
        {
            zeroReal3((*(s))->atoms->f[(ii_darts_counter_temp2531)]);
            (*(s))->atoms->U[(ii_darts_counter_temp2531)] = 0.;
            (*(pot))->dfEmbed[(ii_darts_counter_temp2531)] = 0.;
            (*(pot))->rhobar[(ii_darts_counter_temp2531)] = 0.;
        }
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling omp for stmt's barrier*/
    myTP->controlTPParent->barrierCodelets2531[0].decDep();
}
TP2531::TP2531(int in_numThreads, int in_mainCodeletID, TP2529* in_TPParent, int in_initIteration,
    int in_lastIteration, int* in_fsize, EamPotential** in_pot, SimFlat** in_s,
    TP2531** in_ptrToThisTP)
    : ompTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , fsize_darts2531(in_fsize) /*OMP_SHARED - INPUT*/
    , pot_darts2531(in_pot) /*OMP_SHARED - INPUT*/
    , s_darts2531(in_s) /*OMP_SHARED - INPUT*/
    , ii_darts2531(new int[this->numThreads]) /*VARIABLE*/
    , initIteration2531(in_initIteration)
    , lastIteration2531(in_lastIteration)
    , readyCodelets(0)
    , baseNumThreads(this->numThreads / DARTS_CODELETS_MULT)
    , barrierCodelets2531(new _barrierCodelets2531[1])
    , checkInCodelets2532(new _checkInCodelets2532[this->numThreads])
{
    /*Initialize the loop parameters*/
    range2531 = abs(lastIteration2531 - initIteration2531) / 1;
    rangePerCodelet2531 = range2531 / numThreads;
    minIteration2531 = min<int>(lastIteration2531, initIteration2531);
    remainderRange2531 = range2531 % numThreads;
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets2531[0] = _barrierCodelets2531(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets2532* checkInCodelets2532Ptr = (this->checkInCodelets2532);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets2532);
#endif
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->baseNumThreads;
         codeletCounter++) {
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets2532Ptr) = _checkInCodelets2532(2, 1, this, codeletCounter);
#else
        (*checkInCodelets2532Ptr) = _checkInCodelets2532(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets2532Ptr).decDep();
        checkInCodelets2532Ptr++;
    }
    this->dispatchCodelet(this->getID());
    *(in_ptrToThisTP) = this;
}
void TP2531::dispatchCodelet(size_t codeletID)
{
    int idx = codeletID / this->baseNumThreads;
    int localID = codeletID - this->baseNumThreads * idx;
    this->checkInCodelets2532[localID].setID(codeletID);
    this->checkInCodelets2532[localID].setLocalID(localID);
    /*Check if we want to replicate codelets*/
    for (size_t i = 1; i < (size_t)DARTS_CODELETS_MULT; i++) {
#if USE_SPIN_CODELETS == 0
        this->checkInCodelets2532[localID + this->baseNumThreads * i]
            = _checkInCodelets2532(2, 1, this, localID + this->baseNumThreads * i);
#else
        this->checkInCodelets2532[localID + this->baseNumThreads * i]
            = _checkInCodelets2532(1, 1, this, localID + this->baseNumThreads * i);
#endif
        this->checkInCodelets2532[localID + this->baseNumThreads * i].setID(codeletID);
        this->checkInCodelets2532[localID + this->baseNumThreads * i].decDep();
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
TP2531::~TP2531()
{
    delete[] ii_darts2531;
    delete[] barrierCodelets2531;
    delete[] checkInCodelets2532;
}
/*TP2579: OMPParallelDirective*/
void TP2579::_barrierCodelets2579::fire(void)
{
    TP2579* myTP = static_cast<TP2579*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
void TP2579::_checkInCodelets2581::fire(void)
{
    /*printOMPForCodeletFireCode, inlined: 0*/
    /*region 2581 0*/
    /*Determine the TP to which this codelet belongs and check if this codelet spawns the TP or if
     * it signals it using dispatchCodelet()*/
    size_t idx = this->getID() / myTP->codeletsPerTP2581;
    if (idx < myTP->TPsToUse2581) {
        if (!__sync_val_compare_and_swap(&(myTP->TP2581_alreadyLaunched[idx]), 0, 1)) {
            int range = abs((*(this->inputsTPParent->s_darts2579))->boxes->nLocalBoxes - 0) / 1;
            int rangePerCodelet = range / myTP->TPsToUse2581;
            int minIteration
                = min<int>((*(this->inputsTPParent->s_darts2579))->boxes->nLocalBoxes, 0);
            int remainderRange = range % myTP->TPsToUse2581;
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
            if (0 < (*(this->inputsTPParent->s_darts2579))->boxes->nLocalBoxes) {
                initIteration = min(initIteration, lastIteration);
                lastIteration = max(initIteration, lastIteration);
            } else {
                initIteration = max(initIteration, lastIteration);
                lastIteration = min(initIteration, lastIteration);
            }
            if (idx == myTP->TPsToUse2581 - 1) {
                lastIteration = (*(this->inputsTPParent->s_darts2579))->boxes->nLocalBoxes;
            }
#if USEINVOKE == 1
            invoke<TP2581>(myTP, myTP->codeletsPerTP2581 * DARTS_CODELETS_MULT, this->getID(), myTP,
                initIteration, lastIteration, &(*(this->inputsTPParent->etot_darts2579)),
                &(*(this->inputsTPParent->nNbrBoxes_darts2579)),
                &(*(this->inputsTPParent->pot_darts2579)),
                &(*(this->inputsTPParent->rCut2_darts2579)),
                &(*(this->inputsTPParent->s_darts2579)), &(myTP->TP2581Ptr[idx]));
#else
            place<TP2581>(idx, myTP, myTP->codeletsPerTP2581 * DARTS_CODELETS_MULT, this->getID(),
                myTP, initIteration, lastIteration, &(*(this->inputsTPParent->etot_darts2579)),
                &(*(this->inputsTPParent->nNbrBoxes_darts2579)),
                &(*(this->inputsTPParent->pot_darts2579)),
                &(*(this->inputsTPParent->rCut2_darts2579)),
                &(*(this->inputsTPParent->s_darts2579)), &(myTP->TP2581Ptr[idx]));
#endif
        } else {
            if (myTP->TP2581Ptr[idx] != nullptr) {
                myTP->TP2581Ptr[idx]->dispatchCodelet(this->getID());
            } else {
                this->resetCodelet();
                this->decDep();
            }
        }
    }
}
void TP2579::_barrierCodelets2581::fire(void)
{
    TP2579* myTP = static_cast<TP2579*>(myTP_);
    myTP->TPParent->barrierCodelets2579[0].setDep(0);
    myTP->add(&(myTP->TPParent->barrierCodelets2579[0]));
}
TP2579::TP2579(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet, real_t* in_etot,
    int* in_nNbrBoxes, EamPotential** in_pot, real_t* in_rCut2, SimFlat** in_s)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , etot_darts2579(in_etot) /*OMP_SHARED - INPUT*/
    , nNbrBoxes_darts2579(in_nNbrBoxes) /*OMP_SHARED - INPUT*/
    , pot_darts2579(in_pot) /*OMP_SHARED - INPUT*/
    , rCut2_darts2579(in_rCut2) /*OMP_SHARED - INPUT*/
    , s_darts2579(in_s) /*OMP_SHARED - INPUT*/
    , TP2581Ptr(new TP2581*[NUMTPS2581])
    , TP2581_alreadyLaunched(new size_t[NUMTPS2581])
    , numTPsSet2581(0)
    , numTPsReady2581(0)
    , TPsToUse2581(NUMTPS2581)
    , codeletsPerTP2581(this->numThreads / NUMTPS2581)
    , totalCodelets2581(this->TPsToUse2581 * this->codeletsPerTP2581)
    , barrierCodelets2579(new _barrierCodelets2579[1])
    , checkInCodelets2581(new _checkInCodelets2581[this->numThreads])
    , barrierCodelets2581(new _barrierCodelets2581[1])
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets2579[0] = _barrierCodelets2579(ompNumThreads, ompNumThreads, this, 0);
    barrierCodelets2581[0] = _barrierCodelets2581(NUMTPS2581, NUMTPS2581, this, 0);
    _checkInCodelets2581* checkInCodelets2581Ptr = (this->checkInCodelets2581);
    for (int i = 0; i < NUMTPS2581; i++) {
        TP2581Ptr[i] = nullptr;
        TP2581_alreadyLaunched[i] = 0;
    }
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets2581Ptr) = _checkInCodelets2581(1, 1, this, codeletCounter);
        (*checkInCodelets2581Ptr).decDep();
        checkInCodelets2581Ptr++;
    }
}
TP2579::~TP2579()
{
    delete[] barrierCodelets2579;
    delete[] barrierCodelets2581;
    delete[] checkInCodelets2581;
}
/*TP2581: OMPForDirective*/
void TP2581::_barrierCodelets2581::fire(void)
{
    TP2581* myTP = static_cast<TP2581*>(myTP_);
    myTP->controlTPParent->TPParent->barrierCodelets2581[0].decDep();
}
bool TP2581::requestNewRangeIterations2581(int* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Static */
    /*Chunk = 0*/
    bool isThereNewIteration = false;
    {
        /*Static Scheduling*/
        int tempStartRange = rangePerCodelet2581 * codeletID;
        int tempEndRange = rangePerCodelet2581 * (codeletID + 1);
        if (remainderRange2581 != 0) {
            if (codeletID < (uint32_t)remainderRange2581) {
                tempStartRange += codeletID;
                tempEndRange += (codeletID + 1);
            } else {
                tempStartRange += remainderRange2581;
                tempEndRange += remainderRange2581;
            }
        }
        tempStartRange = tempStartRange * 1 + minIteration2581;
        tempEndRange = tempEndRange * 1 + minIteration2581;
        if (tempStartRange != tempEndRange) {
            isThereNewIteration = true;
        }
        if (initIteration2581 < lastIteration2581) {
            (this->inputsTPParent->iBox_darts2581[codeletID]) = min(tempStartRange, tempEndRange);
            *endRange = max(tempStartRange, tempEndRange);
        } else {
            (this->inputsTPParent->iBox_darts2581[codeletID]) = max(tempStartRange, tempEndRange);
            *endRange = min(tempStartRange, tempEndRange);
        }
        if (codeletID == this->numThreads - 1) {
            *endRange = lastIteration2581;
        }
    }
    return isThereNewIteration;
}
void TP2581::_checkInCodelets2582::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    /*Init the vars for this region*/

    /*printing node 2582: ForStmt*/
    /*var: etot*/
    /*var: nNbrBoxes*/
    /*var: pot*/
    /*var: rCut2*/
    /*var: s*/
    /*Declare local reduction vars*/
    real_t etot_LocalReductionVar = (real_t)0;
    int* nNbrBoxes = (this->inputsTPParent->nNbrBoxes_darts2581);
    (void)nNbrBoxes /*OMP_SHARED*/;
    EamPotential** pot = (this->inputsTPParent->pot_darts2581);
    (void)pot /*OMP_SHARED*/;
    real_t* rCut2 = (this->inputsTPParent->rCut2_darts2581);
    (void)rCut2 /*OMP_SHARED*/;
    SimFlat** s = (this->inputsTPParent->s_darts2581);
    (void)s /*OMP_SHARED*/;
    int* iBox = &(this->inputsTPParent->iBox_darts2581[this->getLocalID()]);
    (void)iBox /*PRIVATE*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations2581(
        (int*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        goto updateReduceVars;
    }
    for (int iBox_darts_counter_temp2581 = (*iBox); iBox_darts_counter_temp2581 < endRange
         && iBox_darts_counter_temp2581 < this->inputsTPParent->lastIteration2581;
         iBox_darts_counter_temp2581++) {
        {
            int nIBox = (*(s))->boxes->nAtoms[(iBox_darts_counter_temp2581)];
            {
                /*Loop's init*/
                int jTmp = 0;
                for (; jTmp < (*(nNbrBoxes)); jTmp++) {
                    int jBox = (*(s))->boxes->nbrBoxes[(iBox_darts_counter_temp2581)][jTmp];
                    int nJBox = (*(s))->boxes->nAtoms[jBox];
                    {
                        /*Loop's init*/
                        int iOff = 64 * (iBox_darts_counter_temp2581);
                        for (; iOff < ((iBox_darts_counter_temp2581)*64 + nIBox); iOff++) {
                            {
                                /*Loop's init*/
                                int jOff = 64 * jBox;
                                for (; jOff < (jBox * 64 + nJBox); jOff++) {
                                    real3 dr;
                                    real_t r2 = 0.;
                                    {
                                        /*Loop's init*/
                                        int k = 0;
                                        for (; k < 3; k++) {
                                            dr[k] = (*(s))->atoms->r[iOff][k]
                                                - (*(s))->atoms->r[jOff][k];
                                            r2 += dr[k] * dr[k];
                                        }
                                    }
                                    if (r2 <= (*(rCut2)) && r2 > 0.) {
                                        real_t r = sqrt(r2);
                                        real_t phiTmp;
                                        real_t dPhi;
                                        real_t rhoTmp;
                                        real_t dRho;
                                        interpolate((*(pot))->phi, r, &phiTmp, &dPhi);
                                        interpolate((*(pot))->rho, r, &rhoTmp, &dRho);
                                        {
                                            /*Loop's init*/
                                            int k = 0;
                                            for (; k < 3; k++) {
                                                (*(s))->atoms->f[iOff][k] -= dPhi * dr[k] / r;
                                            }
                                        }
                                        (*(s))->atoms->U[iOff] += 0.5 * phiTmp;
                                        (*(&(etot_LocalReductionVar))) += 0.5 * phiTmp;
                                        (*(pot))->rhobar[iOff] += rhoTmp;
                                    }
                                }
                            }
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
    this->inputsTPParent->etot_darts2581_mutex->lock();
    *(this->inputsTPParent->etot_darts2581) += (etot_LocalReductionVar);
    this->inputsTPParent->etot_darts2581_mutex->unlock();
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling omp for stmt's barrier*/
    myTP->controlTPParent->barrierCodelets2581[0].decDep();
}
TP2581::TP2581(int in_numThreads, int in_mainCodeletID, TP2579* in_TPParent, int in_initIteration,
    int in_lastIteration, real_t* in_etot, int* in_nNbrBoxes, EamPotential** in_pot,
    real_t* in_rCut2, SimFlat** in_s, TP2581** in_ptrToThisTP)
    : ompTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , etot_darts2581(in_etot) /*OMP_SHARED - INPUT*/
    , nNbrBoxes_darts2581(in_nNbrBoxes) /*OMP_SHARED - INPUT*/
    , pot_darts2581(in_pot) /*OMP_SHARED - INPUT*/
    , rCut2_darts2581(in_rCut2) /*OMP_SHARED - INPUT*/
    , s_darts2581(in_s) /*OMP_SHARED - INPUT*/
    , iBox_darts2581(new int[this->numThreads]) /*VARIABLE*/
    , initIteration2581(in_initIteration)
    , lastIteration2581(in_lastIteration)
    , readyCodelets(0)
    , baseNumThreads(this->numThreads / DARTS_CODELETS_MULT)
    , etot_darts2581_mutex(&(this->TPParent->etot_darts2581_mutex))
    , barrierCodelets2581(new _barrierCodelets2581[1])
    , checkInCodelets2582(new _checkInCodelets2582[this->numThreads])
{
    /*Initialize the loop parameters*/
    range2581 = abs(lastIteration2581 - initIteration2581) / 1;
    rangePerCodelet2581 = range2581 / numThreads;
    minIteration2581 = min<int>(lastIteration2581, initIteration2581);
    remainderRange2581 = range2581 % numThreads;
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets2581[0] = _barrierCodelets2581(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets2582* checkInCodelets2582Ptr = (this->checkInCodelets2582);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets2582);
#endif
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->baseNumThreads;
         codeletCounter++) {
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets2582Ptr) = _checkInCodelets2582(2, 1, this, codeletCounter);
#else
        (*checkInCodelets2582Ptr) = _checkInCodelets2582(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets2582Ptr).decDep();
        checkInCodelets2582Ptr++;
    }
    this->dispatchCodelet(this->getID());
    *(in_ptrToThisTP) = this;
}
void TP2581::dispatchCodelet(size_t codeletID)
{
    int idx = codeletID / this->baseNumThreads;
    int localID = codeletID - this->baseNumThreads * idx;
    this->checkInCodelets2582[localID].setID(codeletID);
    this->checkInCodelets2582[localID].setLocalID(localID);
    /*Check if we want to replicate codelets*/
    for (size_t i = 1; i < (size_t)DARTS_CODELETS_MULT; i++) {
#if USE_SPIN_CODELETS == 0
        this->checkInCodelets2582[localID + this->baseNumThreads * i]
            = _checkInCodelets2582(2, 1, this, localID + this->baseNumThreads * i);
#else
        this->checkInCodelets2582[localID + this->baseNumThreads * i]
            = _checkInCodelets2582(1, 1, this, localID + this->baseNumThreads * i);
#endif
        this->checkInCodelets2582[localID + this->baseNumThreads * i].setID(codeletID);
        this->checkInCodelets2582[localID + this->baseNumThreads * i].decDep();
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
TP2581::~TP2581()
{
    delete[] iBox_darts2581;
    delete[] barrierCodelets2581;
    delete[] checkInCodelets2582;
}
/*TP2709: OMPParallelDirective*/
void TP2709::_barrierCodelets2709::fire(void)
{
    TP2709* myTP = static_cast<TP2709*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
void TP2709::_checkInCodelets2711::fire(void)
{
    /*printOMPForCodeletFireCode, inlined: 0*/
    /*region 2711 0*/
    /*Determine the TP to which this codelet belongs and check if this codelet spawns the TP or if
     * it signals it using dispatchCodelet()*/
    size_t idx = this->getID() / myTP->codeletsPerTP2711;
    if (idx < myTP->TPsToUse2711) {
        if (!__sync_val_compare_and_swap(&(myTP->TP2711_alreadyLaunched[idx]), 0, 1)) {
            int range = abs((*(this->inputsTPParent->s_darts2709))->boxes->nLocalBoxes - 0) / 1;
            int rangePerCodelet = range / myTP->TPsToUse2711;
            int minIteration
                = min<int>((*(this->inputsTPParent->s_darts2709))->boxes->nLocalBoxes, 0);
            int remainderRange = range % myTP->TPsToUse2711;
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
            if (0 < (*(this->inputsTPParent->s_darts2709))->boxes->nLocalBoxes) {
                initIteration = min(initIteration, lastIteration);
                lastIteration = max(initIteration, lastIteration);
            } else {
                initIteration = max(initIteration, lastIteration);
                lastIteration = min(initIteration, lastIteration);
            }
            if (idx == myTP->TPsToUse2711 - 1) {
                lastIteration = (*(this->inputsTPParent->s_darts2709))->boxes->nLocalBoxes;
            }
#if USEINVOKE == 1
            invoke<TP2711>(myTP, myTP->codeletsPerTP2711 * DARTS_CODELETS_MULT, this->getID(), myTP,
                initIteration, lastIteration, &(*(this->inputsTPParent->etot_darts2709)),
                &(*(this->inputsTPParent->pot_darts2709)), &(*(this->inputsTPParent->s_darts2709)),
                &(myTP->TP2711Ptr[idx]));
#else
            place<TP2711>(idx, myTP, myTP->codeletsPerTP2711 * DARTS_CODELETS_MULT, this->getID(),
                myTP, initIteration, lastIteration, &(*(this->inputsTPParent->etot_darts2709)),
                &(*(this->inputsTPParent->pot_darts2709)), &(*(this->inputsTPParent->s_darts2709)),
                &(myTP->TP2711Ptr[idx]));
#endif
        } else {
            if (myTP->TP2711Ptr[idx] != nullptr) {
                myTP->TP2711Ptr[idx]->dispatchCodelet(this->getID());
            } else {
                this->resetCodelet();
                this->decDep();
            }
        }
    }
}
void TP2709::_barrierCodelets2711::fire(void)
{
    TP2709* myTP = static_cast<TP2709*>(myTP_);
    myTP->TPParent->barrierCodelets2709[0].setDep(0);
    myTP->add(&(myTP->TPParent->barrierCodelets2709[0]));
}
TP2709::TP2709(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet, real_t* in_etot,
    EamPotential** in_pot, SimFlat** in_s)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , etot_darts2709(in_etot) /*OMP_SHARED - INPUT*/
    , pot_darts2709(in_pot) /*OMP_SHARED - INPUT*/
    , s_darts2709(in_s) /*OMP_SHARED - INPUT*/
    , TP2711Ptr(new TP2711*[NUMTPS2711])
    , TP2711_alreadyLaunched(new size_t[NUMTPS2711])
    , numTPsSet2711(0)
    , numTPsReady2711(0)
    , TPsToUse2711(NUMTPS2711)
    , codeletsPerTP2711(this->numThreads / NUMTPS2711)
    , totalCodelets2711(this->TPsToUse2711 * this->codeletsPerTP2711)
    , barrierCodelets2709(new _barrierCodelets2709[1])
    , checkInCodelets2711(new _checkInCodelets2711[this->numThreads])
    , barrierCodelets2711(new _barrierCodelets2711[1])
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets2709[0] = _barrierCodelets2709(ompNumThreads, ompNumThreads, this, 0);
    barrierCodelets2711[0] = _barrierCodelets2711(NUMTPS2711, NUMTPS2711, this, 0);
    _checkInCodelets2711* checkInCodelets2711Ptr = (this->checkInCodelets2711);
    for (int i = 0; i < NUMTPS2711; i++) {
        TP2711Ptr[i] = nullptr;
        TP2711_alreadyLaunched[i] = 0;
    }
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets2711Ptr) = _checkInCodelets2711(1, 1, this, codeletCounter);
        (*checkInCodelets2711Ptr).decDep();
        checkInCodelets2711Ptr++;
    }
}
TP2709::~TP2709()
{
    delete[] barrierCodelets2709;
    delete[] barrierCodelets2711;
    delete[] checkInCodelets2711;
}
/*TP2711: OMPForDirective*/
void TP2711::_barrierCodelets2711::fire(void)
{
    TP2711* myTP = static_cast<TP2711*>(myTP_);
    myTP->controlTPParent->TPParent->barrierCodelets2711[0].decDep();
}
bool TP2711::requestNewRangeIterations2711(int* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Static */
    /*Chunk = 0*/
    bool isThereNewIteration = false;
    {
        /*Static Scheduling*/
        int tempStartRange = rangePerCodelet2711 * codeletID;
        int tempEndRange = rangePerCodelet2711 * (codeletID + 1);
        if (remainderRange2711 != 0) {
            if (codeletID < (uint32_t)remainderRange2711) {
                tempStartRange += codeletID;
                tempEndRange += (codeletID + 1);
            } else {
                tempStartRange += remainderRange2711;
                tempEndRange += remainderRange2711;
            }
        }
        tempStartRange = tempStartRange * 1 + minIteration2711;
        tempEndRange = tempEndRange * 1 + minIteration2711;
        if (tempStartRange != tempEndRange) {
            isThereNewIteration = true;
        }
        if (initIteration2711 < lastIteration2711) {
            (this->inputsTPParent->iBox_darts2711[codeletID]) = min(tempStartRange, tempEndRange);
            *endRange = max(tempStartRange, tempEndRange);
        } else {
            (this->inputsTPParent->iBox_darts2711[codeletID]) = max(tempStartRange, tempEndRange);
            *endRange = min(tempStartRange, tempEndRange);
        }
        if (codeletID == this->numThreads - 1) {
            *endRange = lastIteration2711;
        }
    }
    return isThereNewIteration;
}
void TP2711::_checkInCodelets2712::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    /*Init the vars for this region*/

    /*printing node 2712: ForStmt*/
    /*var: etot*/
    /*var: pot*/
    /*var: s*/
    /*Declare local reduction vars*/
    real_t etot_LocalReductionVar = (real_t)0;
    EamPotential** pot = (this->inputsTPParent->pot_darts2711);
    (void)pot /*OMP_SHARED*/;
    SimFlat** s = (this->inputsTPParent->s_darts2711);
    (void)s /*OMP_SHARED*/;
    int* iBox = &(this->inputsTPParent->iBox_darts2711[this->getLocalID()]);
    (void)iBox /*PRIVATE*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations2711(
        (int*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        goto updateReduceVars;
    }
    for (int iBox_darts_counter_temp2711 = (*iBox); iBox_darts_counter_temp2711 < endRange
         && iBox_darts_counter_temp2711 < this->inputsTPParent->lastIteration2711;
         iBox_darts_counter_temp2711++) {
        {
            int nIBox = (*(s))->boxes->nAtoms[(iBox_darts_counter_temp2711)];
            {
                /*Loop's init*/
                int iOff = 64 * (iBox_darts_counter_temp2711);
                for (; iOff < (64 * (iBox_darts_counter_temp2711) + nIBox); iOff++) {
                    real_t fEmbed;
                    real_t dfEmbed;
                    interpolate((*(pot))->f, (*(pot))->rhobar[iOff], &fEmbed, &dfEmbed);
                    (*(pot))->dfEmbed[iOff] = dfEmbed;
                    (*(s))->atoms->U[iOff] += fEmbed;
                    (*(&(etot_LocalReductionVar))) += fEmbed;
                }
            }
        }
    }
updateReduceVars:
    /*Before signaling next codelet, check if all the threads in the TP
    * performed the reduction locally and if so, update the unique
    * reduction variable.*/
    /*Reducing for operator '+'*/
    this->inputsTPParent->etot_darts2711_mutex->lock();
    *(this->inputsTPParent->etot_darts2711) += (etot_LocalReductionVar);
    this->inputsTPParent->etot_darts2711_mutex->unlock();
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling omp for stmt's barrier*/
    myTP->controlTPParent->barrierCodelets2711[0].decDep();
}
TP2711::TP2711(int in_numThreads, int in_mainCodeletID, TP2709* in_TPParent, int in_initIteration,
    int in_lastIteration, real_t* in_etot, EamPotential** in_pot, SimFlat** in_s,
    TP2711** in_ptrToThisTP)
    : ompTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , etot_darts2711(in_etot) /*OMP_SHARED - INPUT*/
    , pot_darts2711(in_pot) /*OMP_SHARED - INPUT*/
    , s_darts2711(in_s) /*OMP_SHARED - INPUT*/
    , iBox_darts2711(new int[this->numThreads]) /*VARIABLE*/
    , initIteration2711(in_initIteration)
    , lastIteration2711(in_lastIteration)
    , readyCodelets(0)
    , baseNumThreads(this->numThreads / DARTS_CODELETS_MULT)
    , etot_darts2711_mutex(&(this->TPParent->etot_darts2711_mutex))
    , barrierCodelets2711(new _barrierCodelets2711[1])
    , checkInCodelets2712(new _checkInCodelets2712[this->numThreads])
{
    /*Initialize the loop parameters*/
    range2711 = abs(lastIteration2711 - initIteration2711) / 1;
    rangePerCodelet2711 = range2711 / numThreads;
    minIteration2711 = min<int>(lastIteration2711, initIteration2711);
    remainderRange2711 = range2711 % numThreads;
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets2711[0] = _barrierCodelets2711(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets2712* checkInCodelets2712Ptr = (this->checkInCodelets2712);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets2712);
#endif
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->baseNumThreads;
         codeletCounter++) {
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets2712Ptr) = _checkInCodelets2712(2, 1, this, codeletCounter);
#else
        (*checkInCodelets2712Ptr) = _checkInCodelets2712(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets2712Ptr).decDep();
        checkInCodelets2712Ptr++;
    }
    this->dispatchCodelet(this->getID());
    *(in_ptrToThisTP) = this;
}
void TP2711::dispatchCodelet(size_t codeletID)
{
    int idx = codeletID / this->baseNumThreads;
    int localID = codeletID - this->baseNumThreads * idx;
    this->checkInCodelets2712[localID].setID(codeletID);
    this->checkInCodelets2712[localID].setLocalID(localID);
    /*Check if we want to replicate codelets*/
    for (size_t i = 1; i < (size_t)DARTS_CODELETS_MULT; i++) {
#if USE_SPIN_CODELETS == 0
        this->checkInCodelets2712[localID + this->baseNumThreads * i]
            = _checkInCodelets2712(2, 1, this, localID + this->baseNumThreads * i);
#else
        this->checkInCodelets2712[localID + this->baseNumThreads * i]
            = _checkInCodelets2712(1, 1, this, localID + this->baseNumThreads * i);
#endif
        this->checkInCodelets2712[localID + this->baseNumThreads * i].setID(codeletID);
        this->checkInCodelets2712[localID + this->baseNumThreads * i].decDep();
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
TP2711::~TP2711()
{
    delete[] iBox_darts2711;
    delete[] barrierCodelets2711;
    delete[] checkInCodelets2712;
}
/*TP2781: OMPParallelDirective*/
void TP2781::_barrierCodelets2781::fire(void)
{
    TP2781* myTP = static_cast<TP2781*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
void TP2781::_checkInCodelets2783::fire(void)
{
    /*printOMPForCodeletFireCode, inlined: 0*/
    /*region 2783 0*/
    /*Determine the TP to which this codelet belongs and check if this codelet spawns the TP or if
     * it signals it using dispatchCodelet()*/
    size_t idx = this->getID() / myTP->codeletsPerTP2783;
    if (idx < myTP->TPsToUse2783) {
        if (!__sync_val_compare_and_swap(&(myTP->TP2783_alreadyLaunched[idx]), 0, 1)) {
            int range = abs((*(this->inputsTPParent->s_darts2781))->boxes->nLocalBoxes - 0) / 1;
            int rangePerCodelet = range / myTP->TPsToUse2783;
            int minIteration
                = min<int>((*(this->inputsTPParent->s_darts2781))->boxes->nLocalBoxes, 0);
            int remainderRange = range % myTP->TPsToUse2783;
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
            if (0 < (*(this->inputsTPParent->s_darts2781))->boxes->nLocalBoxes) {
                initIteration = min(initIteration, lastIteration);
                lastIteration = max(initIteration, lastIteration);
            } else {
                initIteration = max(initIteration, lastIteration);
                lastIteration = min(initIteration, lastIteration);
            }
            if (idx == myTP->TPsToUse2783 - 1) {
                lastIteration = (*(this->inputsTPParent->s_darts2781))->boxes->nLocalBoxes;
            }
#if USEINVOKE == 1
            invoke<TP2783>(myTP, myTP->codeletsPerTP2783 * DARTS_CODELETS_MULT, this->getID(), myTP,
                initIteration, lastIteration, &(*(this->inputsTPParent->nNbrBoxes_darts2781)),
                &(*(this->inputsTPParent->pot_darts2781)),
                &(*(this->inputsTPParent->rCut2_darts2781)),
                &(*(this->inputsTPParent->s_darts2781)), &(myTP->TP2783Ptr[idx]));
#else
            place<TP2783>(idx, myTP, myTP->codeletsPerTP2783 * DARTS_CODELETS_MULT, this->getID(),
                myTP, initIteration, lastIteration, &(*(this->inputsTPParent->nNbrBoxes_darts2781)),
                &(*(this->inputsTPParent->pot_darts2781)),
                &(*(this->inputsTPParent->rCut2_darts2781)),
                &(*(this->inputsTPParent->s_darts2781)), &(myTP->TP2783Ptr[idx]));
#endif
        } else {
            if (myTP->TP2783Ptr[idx] != nullptr) {
                myTP->TP2783Ptr[idx]->dispatchCodelet(this->getID());
            } else {
                this->resetCodelet();
                this->decDep();
            }
        }
    }
}
void TP2781::_barrierCodelets2783::fire(void)
{
    TP2781* myTP = static_cast<TP2781*>(myTP_);
    myTP->TPParent->barrierCodelets2781[0].setDep(0);
    myTP->add(&(myTP->TPParent->barrierCodelets2781[0]));
}
TP2781::TP2781(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet, int* in_nNbrBoxes,
    EamPotential** in_pot, real_t* in_rCut2, SimFlat** in_s)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , nNbrBoxes_darts2781(in_nNbrBoxes) /*OMP_SHARED - INPUT*/
    , pot_darts2781(in_pot) /*OMP_SHARED - INPUT*/
    , rCut2_darts2781(in_rCut2) /*OMP_SHARED - INPUT*/
    , s_darts2781(in_s) /*OMP_SHARED - INPUT*/
    , TP2783Ptr(new TP2783*[NUMTPS2783])
    , TP2783_alreadyLaunched(new size_t[NUMTPS2783])
    , numTPsSet2783(0)
    , numTPsReady2783(0)
    , TPsToUse2783(NUMTPS2783)
    , codeletsPerTP2783(this->numThreads / NUMTPS2783)
    , totalCodelets2783(this->TPsToUse2783 * this->codeletsPerTP2783)
    , barrierCodelets2781(new _barrierCodelets2781[1])
    , checkInCodelets2783(new _checkInCodelets2783[this->numThreads])
    , barrierCodelets2783(new _barrierCodelets2783[1])
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets2781[0] = _barrierCodelets2781(ompNumThreads, ompNumThreads, this, 0);
    barrierCodelets2783[0] = _barrierCodelets2783(NUMTPS2783, NUMTPS2783, this, 0);
    _checkInCodelets2783* checkInCodelets2783Ptr = (this->checkInCodelets2783);
    for (int i = 0; i < NUMTPS2783; i++) {
        TP2783Ptr[i] = nullptr;
        TP2783_alreadyLaunched[i] = 0;
    }
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets2783Ptr) = _checkInCodelets2783(1, 1, this, codeletCounter);
        (*checkInCodelets2783Ptr).decDep();
        checkInCodelets2783Ptr++;
    }
}
TP2781::~TP2781()
{
    delete[] barrierCodelets2781;
    delete[] barrierCodelets2783;
    delete[] checkInCodelets2783;
}
/*TP2783: OMPForDirective*/
void TP2783::_barrierCodelets2783::fire(void)
{
    TP2783* myTP = static_cast<TP2783*>(myTP_);
    myTP->controlTPParent->TPParent->barrierCodelets2783[0].decDep();
}
bool TP2783::requestNewRangeIterations2783(int* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Static */
    /*Chunk = 0*/
    bool isThereNewIteration = false;
    {
        /*Static Scheduling*/
        int tempStartRange = rangePerCodelet2783 * codeletID;
        int tempEndRange = rangePerCodelet2783 * (codeletID + 1);
        if (remainderRange2783 != 0) {
            if (codeletID < (uint32_t)remainderRange2783) {
                tempStartRange += codeletID;
                tempEndRange += (codeletID + 1);
            } else {
                tempStartRange += remainderRange2783;
                tempEndRange += remainderRange2783;
            }
        }
        tempStartRange = tempStartRange * 1 + minIteration2783;
        tempEndRange = tempEndRange * 1 + minIteration2783;
        if (tempStartRange != tempEndRange) {
            isThereNewIteration = true;
        }
        if (initIteration2783 < lastIteration2783) {
            (this->inputsTPParent->iBox_darts2783[codeletID]) = min(tempStartRange, tempEndRange);
            *endRange = max(tempStartRange, tempEndRange);
        } else {
            (this->inputsTPParent->iBox_darts2783[codeletID]) = max(tempStartRange, tempEndRange);
            *endRange = min(tempStartRange, tempEndRange);
        }
        if (codeletID == this->numThreads - 1) {
            *endRange = lastIteration2783;
        }
    }
    return isThereNewIteration;
}
void TP2783::_checkInCodelets2784::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    /*Init the vars for this region*/

    /*printing node 2784: ForStmt*/
    /*var: nNbrBoxes*/
    /*var: pot*/
    /*var: rCut2*/
    /*var: s*/
    int* nNbrBoxes = (this->inputsTPParent->nNbrBoxes_darts2783);
    (void)nNbrBoxes /*OMP_SHARED*/;
    EamPotential** pot = (this->inputsTPParent->pot_darts2783);
    (void)pot /*OMP_SHARED*/;
    real_t* rCut2 = (this->inputsTPParent->rCut2_darts2783);
    (void)rCut2 /*OMP_SHARED*/;
    SimFlat** s = (this->inputsTPParent->s_darts2783);
    (void)s /*OMP_SHARED*/;
    int* iBox = &(this->inputsTPParent->iBox_darts2783[this->getLocalID()]);
    (void)iBox /*PRIVATE*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations2783(
        (int*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        /*Signaling omp for stmt's barrier*/
        myTP->controlTPParent->barrierCodelets2783[0].decDep();
        return;
    }
    for (int iBox_darts_counter_temp2783 = (*iBox); iBox_darts_counter_temp2783 < endRange
         && iBox_darts_counter_temp2783 < this->inputsTPParent->lastIteration2783;
         iBox_darts_counter_temp2783++) {
        {
            int nIBox = (*(s))->boxes->nAtoms[(iBox_darts_counter_temp2783)];
            {
                /*Loop's init*/
                int jTmp = 0;
                for (; jTmp < (*(nNbrBoxes)); jTmp++) {
                    int jBox = (*(s))->boxes->nbrBoxes[(iBox_darts_counter_temp2783)][jTmp];
                    int nJBox = (*(s))->boxes->nAtoms[jBox];
                    {
                        /*Loop's init*/
                        int iOff = 64 * (iBox_darts_counter_temp2783);
                        for (; iOff < (64 * (iBox_darts_counter_temp2783) + nIBox); iOff++) {
                            {
                                /*Loop's init*/
                                int jOff = 64 * jBox;
                                for (; jOff < (64 * jBox + nJBox); jOff++) {
                                    real_t r2 = 0.;
                                    real3 dr;
                                    {
                                        /*Loop's init*/
                                        int k = 0;
                                        for (; k < 3; k++) {
                                            dr[k] = (*(s))->atoms->r[iOff][k]
                                                - (*(s))->atoms->r[jOff][k];
                                            r2 += dr[k] * dr[k];
                                        }
                                    }
                                    if (r2 <= (*(rCut2)) && r2 > 0.) {
                                        real_t r = sqrt(r2);
                                        real_t rhoTmp;
                                        real_t dRho;
                                        interpolate((*(pot))->rho, r, &rhoTmp, &dRho);
                                        {
                                            /*Loop's init*/
                                            int k = 0;
                                            for (; k < 3; k++) {
                                                (*(s))->atoms->f[iOff][k]
                                                    -= ((*(pot))->dfEmbed[iOff]
                                                           + (*(pot))->dfEmbed[jOff])
                                                    * dRho * dr[k] / r;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling omp for stmt's barrier*/
    myTP->controlTPParent->barrierCodelets2783[0].decDep();
}
TP2783::TP2783(int in_numThreads, int in_mainCodeletID, TP2781* in_TPParent, int in_initIteration,
    int in_lastIteration, int* in_nNbrBoxes, EamPotential** in_pot, real_t* in_rCut2,
    SimFlat** in_s, TP2783** in_ptrToThisTP)
    : ompTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , nNbrBoxes_darts2783(in_nNbrBoxes) /*OMP_SHARED - INPUT*/
    , pot_darts2783(in_pot) /*OMP_SHARED - INPUT*/
    , rCut2_darts2783(in_rCut2) /*OMP_SHARED - INPUT*/
    , s_darts2783(in_s) /*OMP_SHARED - INPUT*/
    , iBox_darts2783(new int[this->numThreads]) /*VARIABLE*/
    , initIteration2783(in_initIteration)
    , lastIteration2783(in_lastIteration)
    , readyCodelets(0)
    , baseNumThreads(this->numThreads / DARTS_CODELETS_MULT)
    , barrierCodelets2783(new _barrierCodelets2783[1])
    , checkInCodelets2784(new _checkInCodelets2784[this->numThreads])
{
    /*Initialize the loop parameters*/
    range2783 = abs(lastIteration2783 - initIteration2783) / 1;
    rangePerCodelet2783 = range2783 / numThreads;
    minIteration2783 = min<int>(lastIteration2783, initIteration2783);
    remainderRange2783 = range2783 % numThreads;
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets2783[0] = _barrierCodelets2783(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets2784* checkInCodelets2784Ptr = (this->checkInCodelets2784);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets2784);
#endif
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->baseNumThreads;
         codeletCounter++) {
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets2784Ptr) = _checkInCodelets2784(2, 1, this, codeletCounter);
#else
        (*checkInCodelets2784Ptr) = _checkInCodelets2784(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets2784Ptr).decDep();
        checkInCodelets2784Ptr++;
    }
    this->dispatchCodelet(this->getID());
    *(in_ptrToThisTP) = this;
}
void TP2783::dispatchCodelet(size_t codeletID)
{
    int idx = codeletID / this->baseNumThreads;
    int localID = codeletID - this->baseNumThreads * idx;
    this->checkInCodelets2784[localID].setID(codeletID);
    this->checkInCodelets2784[localID].setLocalID(localID);
    /*Check if we want to replicate codelets*/
    for (size_t i = 1; i < (size_t)DARTS_CODELETS_MULT; i++) {
#if USE_SPIN_CODELETS == 0
        this->checkInCodelets2784[localID + this->baseNumThreads * i]
            = _checkInCodelets2784(2, 1, this, localID + this->baseNumThreads * i);
#else
        this->checkInCodelets2784[localID + this->baseNumThreads * i]
            = _checkInCodelets2784(1, 1, this, localID + this->baseNumThreads * i);
#endif
        this->checkInCodelets2784[localID + this->baseNumThreads * i].setID(codeletID);
        this->checkInCodelets2784[localID + this->baseNumThreads * i].decDep();
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
TP2783::~TP2783()
{
    delete[] iBox_darts2783;
    delete[] barrierCodelets2783;
    delete[] checkInCodelets2784;
}
