#include "mycommand.output.darts.h"
using namespace darts;
using namespace std;
/*Function: parseCommandLine, ID: 133*/
Command parseCommandLine(int argc, char** argv)
{
    /*parseCommandLine:133*/
    /*CompoundStmt:1203*/
    Command cmd;
    memset(cmd.potDir, 0, 1024);
    memset(cmd.potName, 0, 1024);
    memset(cmd.potType, 0, 1024);
    strcpy(cmd.potDir, "pots");
    strcpy(cmd.potName, "\000");
    strcpy(cmd.potType, "funcfl");
    cmd.doeam = 0;
    cmd.nx = 20;
    cmd.ny = 20;
    cmd.nz = 20;
    cmd.xproc = 1;
    cmd.yproc = 1;
    cmd.zproc = 1;
    cmd.nSteps = 100;
    cmd.printRate = 10;
    cmd.dt = 1.;
    cmd.lat = -1.;
    cmd.temperature = 600.;
    cmd.initialDelta = 0.;
    int help = 0;
    addArg("help", 'h', 0, 'i', &(help), 0, "print this message");
    addArg("potDir", 'd', 1, 's', cmd.potDir, sizeof(cmd.potDir), "potential directory");
    addArg("potName", 'p', 1, 's', cmd.potName, sizeof(cmd.potName), "potential name");
    addArg("potType", 't', 1, 's', cmd.potType, sizeof(cmd.potType),
        "potential type (funcfl or setfl)");
    addArg("doeam", 'e', 0, 'i', &(cmd.doeam), 0, "compute eam potentials");
    addArg("nx", 'x', 1, 'i', &(cmd.nx), 0, "number of unit cells in x");
    addArg("ny", 'y', 1, 'i', &(cmd.ny), 0, "number of unit cells in y");
    addArg("nz", 'z', 1, 'i', &(cmd.nz), 0, "number of unit cells in z");
    addArg("xproc", 'i', 1, 'i', &(cmd.xproc), 0, "processors in x direction");
    addArg("yproc", 'j', 1, 'i', &(cmd.yproc), 0, "processors in y direction");
    addArg("zproc", 'k', 1, 'i', &(cmd.zproc), 0, "processors in z direction");
    addArg("nSteps", 'N', 1, 'i', &(cmd.nSteps), 0, "number of time steps");
    addArg("printRate", 'n', 1, 'i', &(cmd.printRate), 0, "number of steps between output");
    addArg("dt", 'D', 1, 'd', &(cmd.dt), 0, "time step (in fs)");
    addArg("lat", 'l', 1, 'd', &(cmd.lat), 0, "lattice parameter (Angstroms)");
    addArg("temp", 'T', 1, 'd', &(cmd.temperature), 0, "initial temperature (K)");
    addArg("delta", 'r', 1, 'd', &(cmd.initialDelta), 0, "initial delta (Angstroms)");
    processArgs(argc, argv);
    if (strlen(cmd.potName) == 0) {
        if (strcmp(cmd.potType, "setfl") == 0)
            strcpy(cmd.potName, "Cu01.eam.alloy");
        if (strcmp(cmd.potType, "funcfl") == 0)
            strcpy(cmd.potName, "Cu_u6.eam");
    }
    if (help) {
        printArgs();
        freeArgs();
        exit(2);
    }
    freeArgs();
    return cmd;
}
/*Function: printCmdYaml, ID: 134*/
void printCmdYaml(FILE* file, Command* cmd)
{
    /*printCmdYaml:134*/
    /*CompoundStmt:1358*/
    if (!printRank())
        return;
    fprintf(file, "Command Line Parameters:\n  doeam: %d\n  potDir: %s\n  potName: %s\n  potType: "
                  "%s\n  nx: %d\n  ny: %d\n  nz: %d\n  xproc: %d\n  yproc: %d\n  zproc: %d\n  "
                  "Lattice constant: %g Angstroms\n  nSteps: %d\n  printRate: %d\n  Time step: %g "
                  "fs\n  Initial Temperature: %g K\n  Initial Delta: %g Angstroms\n\n",
        cmd->doeam, cmd->potDir, cmd->potName, cmd->potType, cmd->nx, cmd->ny, cmd->nz, cmd->xproc,
        cmd->yproc, cmd->zproc, cmd->lat, cmd->nSteps, cmd->printRate, cmd->dt, cmd->temperature,
        cmd->initialDelta);
    fflush(file);
}
