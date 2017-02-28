#include "CoMD.output.darts.h"
using namespace darts;
using namespace std;
int firstCall_darts20;
bool firstCall_darts20_initFlag;
int iStepPrev_darts20;
bool iStepPrev_darts20_initFlag;
static void sanityChecks(Command cmd, double cutoff, double latticeConst, char latticeType[8]);
static void printSimulationDataYaml(FILE* file, SimFlat* s);
static void printThings(SimFlat* s, int iStep, double elapsedTime);
static SimFlat* initSimulation(Command cmd);
static BasePotential* initPotential(
    int doeam, const char* potDir, const char* potName, const char* potType);
static SpeciesData* initSpecies(BasePotential* pot);
static void initSubsystems();
static void finalizeSubsystems();
static void destroySimulation(SimFlat** ps);
static void validateResult(const Validate* val, SimFlat* sim);
static void sumAtoms(SimFlat* s);
static Validate* initValidate(SimFlat* s);
/*Function: main, ID: 23*/
int main(int argc, char** argv)
{
    getOMPNumThreads();
    getOMPSchedulePolicy();
    getTPLoopThresholds();
    getNumTPs();
    affin = new ThreadAffinity(
        ompNumThreads / NUMTPS - 1, NUMTPS, COMPACT, getDARTSTPPolicy(), getDARTSMCPolicy());
    affinMaskRes = affin->generateMask();
    myDARTSRuntime = new Runtime(affin);
    RuntimeFinalCodelet = &(myDARTSRuntime->finalSignal);
    firstCall_darts20 = (int)0;
    firstCall_darts20_initFlag = false;
    iStepPrev_darts20 = (int)0;
    iStepPrev_darts20_initFlag = false;
    /*main:23*/
    /*CompoundStmt:1382*/
    initParallel(&argc, &argv);
    profileStart(totalTimer);
    initSubsystems();
    timestampBarrier("Starting Initialization\n");
    yamlAppInfo(yamlFile);
    yamlAppInfo(stdout);
    Command cmd = parseCommandLine(argc, argv);
    printCmdYaml(yamlFile, &cmd);
    printCmdYaml(stdout, &cmd);
    SimFlat* sim = initSimulation(cmd);
    printSimulationDataYaml(yamlFile, sim);
    printSimulationDataYaml(stdout, sim);
    Validate* validate = initValidate(sim);
    timestampBarrier("Initialization Finished\n");
    timestampBarrier("Starting simulation\n");
    const int nSteps = sim->nSteps;
    const int printRate = sim->printRate;
    int iStep = 0;
    profileStart(loopTimer);
    for (; iStep < nSteps;) {
        do {
            profileStart(commReduceTimer);
        } while (0);
        sumAtoms(sim);
        do {
            profileStop(commReduceTimer);
        } while (0);
        printThings(sim, iStep, getElapsedTime(timestepTimer));
        do {
            profileStart(timestepTimer);
        } while (0);
        timestep(sim, printRate, sim->dt);
        do {
            profileStop(timestepTimer);
        } while (0);
        iStep += printRate;
    }
    profileStop(loopTimer);
    sumAtoms(sim);
    printThings(sim, iStep, getElapsedTime(timestepTimer));
    timestampBarrier("Ending simulation\n");
    validateResult(validate, sim);
    profileStop(totalTimer);
    printPerformanceResults(sim->atoms->nGlobal, sim->printRate);
    printPerformanceResultsYaml(yamlFile);
    destroySimulation(&sim);
    comdFree(validate);
    finalizeSubsystems();
    timestampBarrier("CoMD Ending\n");
    destroyParallel();
    return 0;
}
/*Function: initSimulation, ID: 11*/
SimFlat* initSimulation(Command cmd)
{
    /*initSimulation:11*/
    /*CompoundStmt:1455*/
    SimFlat* sim = comdMalloc(sizeof(SimFlat));
    sim->nSteps = cmd.nSteps;
    sim->printRate = cmd.printRate;
    sim->dt = cmd.dt;
    sim->domain = ((void*)0);
    sim->boxes = ((void*)0);
    sim->atoms = ((void*)0);
    sim->ePotential = 0.;
    sim->eKinetic = 0.;
    sim->atomExchange = ((void*)0);
    sim->pot = initPotential(cmd.doeam, cmd.potDir, cmd.potName, cmd.potType);
    real_t latticeConstant = cmd.lat;
    if (cmd.lat < 0.)
        latticeConstant = sim->pot->lat;
    sanityChecks(cmd, sim->pot->cutoff, latticeConstant, sim->pot->latticeType);
    sim->species = initSpecies(sim->pot);
    real3 globalExtent;
    globalExtent[0] = cmd.nx * latticeConstant;
    globalExtent[1] = cmd.ny * latticeConstant;
    globalExtent[2] = cmd.nz * latticeConstant;
    sim->domain = initDecomposition(cmd.xproc, cmd.yproc, cmd.zproc, globalExtent);
    sim->boxes = initLinkCells(sim->domain, sim->pot->cutoff);
    sim->atoms = initAtoms(sim->boxes);
    createFccLattice(cmd.nx, cmd.ny, cmd.nz, latticeConstant, sim);
    setTemperature(sim, cmd.temperature);
    randomDisplacements(sim, cmd.initialDelta);
    sim->atomExchange = initAtomHaloExchange(sim->domain, sim->boxes);
    do {
        profileStart(redistributeTimer);
    } while (0);
    redistributeAtoms(sim);
    do {
        profileStop(redistributeTimer);
    } while (0);
    do {
        profileStart(computeForceTimer);
    } while (0);
    computeForce(sim);
    do {
        profileStop(computeForceTimer);
    } while (0);
    kineticEnergy(sim);
    return sim;
}
/*Function: destroySimulation, ID: 12*/
void destroySimulation(SimFlat** ps)
{
    /*destroySimulation:12*/
    /*CompoundStmt:1572*/
    if (!ps)
        return;
    SimFlat* s = *ps;
    if (!s)
        return;
    BasePotential* pot = s->pot;
    if (pot)
        pot->destroy(&pot);
    destroyLinkCells(&(s->boxes));
    destroyAtoms(s->atoms);
    destroyHaloExchange(&(s->atomExchange));
    comdFree(s->species);
    comdFree(s->domain);
    comdFree(s);
    *ps = ((void*)0);
    return;
}
/*Function: initSubsystems, ID: 13*/
void initSubsystems()
{
    /*initSubsystems:13*/
    /*CompoundStmt:1608*/
    yamlBegin();
}
/*Function: finalizeSubsystems, ID: 14*/
void finalizeSubsystems()
{
    /*finalizeSubsystems:14*/
    /*CompoundStmt:1610*/
    yamlEnd();
}
/*Function: initPotential, ID: 15*/
BasePotential* initPotential(
    int doeam, const char* potDir, const char* potName, const char* potType)
{
    /*initPotential:15*/
    /*CompoundStmt:1612*/
    BasePotential* pot = ((void*)0);
    if (doeam)
        pot = initEamPot(potDir, potName, potType);
    else
        pot = initLjPot();
    return pot;
}
/*Function: initSpecies, ID: 16*/
SpeciesData* initSpecies(BasePotential* pot)
{
    /*initSpecies:16*/
    /*CompoundStmt:1624*/
    SpeciesData* species = comdMalloc(sizeof(SpeciesData));
    strcpy(species->name, pot->name);
    species->atomicNo = pot->atomicNo;
    species->mass = pot->mass;
    return species;
}
/*Function: initValidate, ID: 17*/
Validate* initValidate(SimFlat* sim)
{
    /*initValidate:17*/
    /*CompoundStmt:1638*/
    sumAtoms(sim);
    Validate* val = comdMalloc(sizeof(Validate));
    val->eTot0 = (sim->ePotential + sim->eKinetic) / sim->atoms->nGlobal;
    val->nAtoms0 = sim->atoms->nGlobal;
    if (printRank()) {
        fprintf(stdout, "\n");
        printSeparator(stdout);
        fprintf(stdout, "Initial energy : %14.12f, atom count : %d \n", val->eTot0, val->nAtoms0);
        fprintf(stdout, "\n");
    }
    return val;
}
/*Function: validateResult, ID: 18*/
void validateResult(const Validate* val, SimFlat* sim)
{
    /*validateResult:18*/
    /*CompoundStmt:1665*/
    if (printRank()) {
        real_t eFinal = (sim->ePotential + sim->eKinetic) / sim->atoms->nGlobal;
        int nAtomsDelta = (sim->atoms->nGlobal - val->nAtoms0);
        fprintf(stdout, "\n");
        fprintf(stdout, "\n");
        fprintf(stdout, "Simulation Validation:\n");
        fprintf(stdout, "  Initial energy  : %14.12f\n", val->eTot0);
        fprintf(stdout, "  Final energy    : %14.12f\n", eFinal);
        fprintf(stdout, "  eFinal/eInitial : %f\n", eFinal / val->eTot0);
        if (nAtomsDelta == 0) {
            fprintf(stdout, "  Final atom count : %d, no atoms lost\n", sim->atoms->nGlobal);
        } else {
            fprintf(stdout, "#############################\n");
            fprintf(stdout, "# WARNING: %6d atoms lost #\n", nAtomsDelta);
            fprintf(stdout, "#############################\n");
        }
    }
}
/*Function: sumAtoms, ID: 19*/
void sumAtoms(SimFlat* s)
{
    /*sumAtoms:19*/
    /*CompoundStmt:1700*/
    s->atoms->nLocal = 0;
    for (int i = 0; i < s->boxes->nLocalBoxes; i++) {
        s->atoms->nLocal += s->boxes->nAtoms[i];
    }
    do {
        profileStart(commReduceTimer);
    } while (0);
    addIntParallel(&s->atoms->nLocal, &s->atoms->nGlobal, 1);
    do {
        profileStop(commReduceTimer);
    } while (0);
}
/*Function: printThings, ID: 20*/
void printThings(SimFlat* s, int iStep, double elapsedTime)
{
    /*printThings:20*/
    /*CompoundStmt:1732*/
    static int iStepPrev = -1;
    static int firstCall = 1;
    int nEval = iStep - iStepPrev;
    iStepPrev = iStep;
    if (!printRank())
        return;
    if (firstCall) {
        firstCall = 0;
        fprintf(stdout, "#                                                                         "
                        "                Performance\n#  Loop   Time(fs)       Total Energy   "
                        "Potential Energy     Kinetic Energy  Temperature   (us/atom)     # "
                        "Atoms\n");
        fflush(stdout);
    }
    real_t time = iStep * s->dt;
    real_t eTotal = (s->ePotential + s->eKinetic) / s->atoms->nGlobal;
    real_t eK = s->eKinetic / s->atoms->nGlobal;
    real_t eU = s->ePotential / s->atoms->nGlobal;
    real_t Temp = (s->eKinetic / s->atoms->nGlobal) / (kB_eV * 1.5);
    double timePerAtom = 1.0E+6 * elapsedTime / (double)(nEval * s->atoms->nLocal);
    fprintf(stdout, " %6d %10.2f %18.12f %18.12f %18.12f %12.4f %10.4f %12d\n", iStep, time, eTotal,
        eU, eK, Temp, timePerAtom, s->atoms->nGlobal);
}
/*Function: printSimulationDataYaml, ID: 21*/
void printSimulationDataYaml(FILE* file, SimFlat* s)
{
    /*printSimulationDataYaml:21*/
    /*CompoundStmt:1788*/
    int maxOcc = maxOccupancy(s->boxes);
    if (!printRank())
        return;
    fprintf(file, "Simulation data: \n");
    fprintf(file, "  Total atoms        : %d\n", s->atoms->nGlobal);
    fprintf(file, "  Min global bounds  : [ %14.10f, %14.10f, %14.10f ]\n", s->domain->globalMin[0],
        s->domain->globalMin[1], s->domain->globalMin[2]);
    fprintf(file, "  Max global bounds  : [ %14.10f, %14.10f, %14.10f ]\n", s->domain->globalMax[0],
        s->domain->globalMax[1], s->domain->globalMax[2]);
    printSeparator(file);
    fprintf(file, "Decomposition data: \n");
    fprintf(file, "  Processors         : %6d,%6d,%6d\n", s->domain->procGrid[0],
        s->domain->procGrid[1], s->domain->procGrid[2]);
    fprintf(file, "  Local boxes        : %6d,%6d,%6d = %8d\n", s->boxes->gridSize[0],
        s->boxes->gridSize[1], s->boxes->gridSize[2],
        s->boxes->gridSize[0] * s->boxes->gridSize[1] * s->boxes->gridSize[2]);
    fprintf(file, "  Box size           : [ %14.10f, %14.10f, %14.10f ]\n", s->boxes->boxSize[0],
        s->boxes->boxSize[1], s->boxes->boxSize[2]);
    fprintf(file, "  Box factor         : [ %14.10f, %14.10f, %14.10f ] \n",
        s->boxes->boxSize[0] / s->pot->cutoff, s->boxes->boxSize[1] / s->pot->cutoff,
        s->boxes->boxSize[2] / s->pot->cutoff);
    fprintf(file, "  Max Link Cell Occupancy: %d of %d\n", maxOcc, 64);
    printSeparator(file);
    fprintf(file, "Potential data: \n");
    s->pot->print(file, s->pot);
    fflush(file);
}
/*Function: sanityChecks, ID: 22*/
void sanityChecks(Command cmd, double cutoff, double latticeConst, char latticeType[8])
{
    /*sanityChecks:22*/
    /*CompoundStmt:1890*/
    int failCode = 0;
    int nProcs = cmd.xproc * cmd.yproc * cmd.zproc;
    if (nProcs != getNRanks()) {
        failCode |= 1;
        if (printRank())
            fprintf(stdout, "\nNumber of MPI ranks must match xproc * yproc * zproc\n");
    }
    double minx = 2 * cutoff * cmd.xproc;
    double miny = 2 * cutoff * cmd.yproc;
    double minz = 2 * cutoff * cmd.zproc;
    double sizex = cmd.nx * latticeConst;
    double sizey = cmd.ny * latticeConst;
    double sizez = cmd.nz * latticeConst;
    if (sizex < minx || sizey < miny || sizez < minz) {
        failCode |= 2;
        if (printRank())
            fprintf(stdout, "\nSimulation too small.\n  Increase the number of unit cells to make "
                            "the simulation\n  at least (%3.2f, %3.2f. %3.2f) Ansgstroms in size\n",
                minx, miny, minz);
    }
    if (strcasecmp(latticeType, "FCC") != 0) {
        failCode |= 4;
        if (printRank())
            fprintf(
                stdout, "\nOnly FCC Lattice type supported, not %s. Fatal Error.\n", latticeType);
    }
    int checkCode = failCode;
    bcastParallel(&checkCode, sizeof(int), 0);
    if (failCode != 0)
        exit(failCode);
}
