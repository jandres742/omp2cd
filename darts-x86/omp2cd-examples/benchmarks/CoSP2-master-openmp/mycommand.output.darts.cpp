#include "mycommand.output.darts.h"
using namespace darts;
using namespace std;
/*Function: parseCommandLine, ID: 36*/
Command parseCommandLine(int argc, char** argv)
{
    /*parseCommandLine:36*/
    /*CompoundStmt:2663*/
    Command cmd;
    memset(cmd.hmatName, 0, 1024);
    cmd.N = 1600;
    cmd.M = 80;
    cmd.dout = 0;
    cmd.gen = 0;
    cmd.debug = 0;
    cmd.amp = 1.;
    cmd.alpha = 1.;
    cmd.eps = 1.0000000000000001E-5;
    cmd.heps = 9.9999999999999997E-17;
    cmd.idemTol = 9.9999999999999999E-15;
    int help = 0;
    addArg("help", 'h', 0, 'i', &(help), 0, "print this message");
    addArg("hmatName", 'f', 1, 's', cmd.hmatName, sizeof(cmd.hmatName), "H matrix file name");
    addArg("N", 'n', 1, 'i', &(cmd.N), 0, "rows in matrix");
    addArg("M", 'm', 1, 'i', &(cmd.M), 0, "non-zeroes per row");
    addArg("gen", 'g', 1, 'i', &(cmd.gen), 0, "generate H matrix");
    addArg("dout", 'd', 1, 'i', &(cmd.dout), 0, "write out density matrix");
    addArg("debug", 'b', 1, 'i', &(cmd.debug), 0, "write out debug messages");
    addArg("amp", 'a', 1, 'd', &(cmd.amp), 0, "amplitude");
    addArg("alpha", 'l', 1, 'd', &(cmd.alpha), 0, "alpha");
    addArg("eps", 'e', 1, 'd', &(cmd.eps), 0, "threshold for sparse math");
    addArg("heps", 'p', 1, 'd', &(cmd.heps), 0, "threshold for dense2sparse");
    addArg("idemtol", 'i', 1, 'd', &(cmd.idemTol), 0, "threshold for SP2 loop");
    processArgs(argc, argv);
    if (strlen(cmd.hmatName) == 0) {
        cmd.gen = 1;
    }
    if (help) {
        printArgs();
        freeArgs();
        exit(0);
    }
    freeArgs();
    return cmd;
}
