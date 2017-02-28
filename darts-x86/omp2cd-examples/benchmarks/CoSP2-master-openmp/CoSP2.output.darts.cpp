#include "CoSP2.output.darts.h"
using namespace darts;
using namespace std;
int M;
int debug;
real_t eps;
int hDim;
real_t hEps;
real_t idemTol;
int msparse;
/*Function: initSimulation, ID: 11*/
SparseMatrix* initSimulation(Command cmd)
{
    /*initSimulation:11*/
    /*CompoundStmt:2285*/
    SparseMatrix* spH;
    if (printRank())
        printf("hDim = %d M = %d\n", hDim, msparse);
    if (cmd.gen == 0) {
        M = nnzStart(hDim, msparse);
        spH = initSparseMatrix(hDim, M);
        do {
            profileStart(readhTimer);
        } while (0);
        readMTX(cmd.hmatName, spH);
        do {
            profileStop(readhTimer);
        } while (0);
    } else {
        M = nnzStart(hDim, msparse);
        spH = generateHMatrix(hDim, M, cmd.amp, cmd.alpha);
    }
    return spH;
}
/*Function: main, ID: 12*/
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
    /*main:12*/
    /*CompoundStmt:2316*/
    initParallel(&argc, &argv);
    profileStart(totalTimer);
    profileStart(loopTimer);
    if (printRank())
        printf("CoSP2: SP2 Loop\n");
    Command cmd = parseCommandLine(argc, argv);
    msparse = cmd.M;
    hDim = cmd.N;
    debug = cmd.debug;
    eps = cmd.eps;
    hEps = cmd.heps;
    idemTol = cmd.idemTol;
    if (printRank()) {
        printf("\nParameters:\n");
        printf("msparse = %d  hDim = %d  debug = %d\n", msparse, hDim, debug);
        printf("hmatName = %s\n", cmd.hmatName);
        printf("eps = %lg  hEps = %lg\n", eps, hEps);
        printf("idemTol = %lg\n\n", idemTol);
    }
    do {
        profileStart(preTimer);
    } while (0);
    SparseMatrix* spH = initSimulation(cmd);
    do {
        profileStop(preTimer);
    } while (0);
    Domain* domain = initDecomposition(getNRanks(), spH->hsize, spH->msize);
    sparsity(spH);
    gershgorin(spH, domain);
    barrierParallel();
    sp2Loop(spH, domain);
    profileStop(totalTimer);
    profileStop(loopTimer);
    printPerformanceResults(spH->hsize, 0);
    if (printRank() && cmd.dout == 1)
        writeMTX("dmatrix.out.mtx", spH);
    destroySparseMatrix(spH);
    destroyDecomposition(domain);
    destroyParallel();
    return 0;
}
