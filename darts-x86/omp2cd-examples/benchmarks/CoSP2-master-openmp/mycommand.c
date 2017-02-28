/// \file
/// Handle command line arguments.

#include "mycommand.h"

#include <string.h>
#include <stdlib.h>

#include "cmdLineParser.h"
#include "parallel.h"
#include "mytype.h"

/// \page pg_running_cosp2 Running CoSP2
///
/// \section sec_command_line_options Command Line Options
///
/// CoSP2 accepts a number of command line options to set the parameters
/// of the simulation. Every option has both a long form and a short
/// form. The long and short form of the arguments are entirely
/// interchangeable and may be mixed. 
///
/// Supported options are:
///
/// | Long  Form    | Short Form  | Default Value | Description
/// | :------------ | :---------: | :-----------: | :----------
/// | \--help       | -h          | N/A           | print this message
/// | \--hmatName   | -f          |               | H matrix file name in Matrix Market format
/// | \--N          | -n          | 1600          | number of rows
/// | \--M          | -m          | 80            | max non-zeroes per row
/// | \--amp        | -a          | 1.0           | amplitude
/// | \--alpha      | -l          | 1.0           | alpha
/// | \--eps        | -e          | 1.0E-05       | threshold for sparse math
/// | \--heps       | -p          | 1.0E-16       | numeric threshold
/// | \--idemtol    | -i          | 1.0E-14       | threshold for SP2 loop
/// | \--gen        | -g          | 0             | generate H matrix if 1
/// | \--dout       | -d          | 0             | write out density matrix if 1
/// | \--dbg        | -b          | 0             | write debug messages if 1
///
/// Notes: 
/// 
/// 
/// \subsection ssec_example_command_lines Examples
///
/// All of the examples below assume:
/// - The CoSP2 bin directory is located in ../bin
///
/// Running in the examples directory will satisfy these requirements.
///
/// ------------------------------
///
/// The canonical base simulation, is 
///
///     $ mpirun -np 1 ../bin/CoSP2Parallel 
/// 
/// Or, if the code was built without MPI:
///
///     $ ../bin/CoSP2
///
/// ------------------------------
///
/// \subsubsection cmd_examples_hmatrix Specify H matrix
///
/// To run with a given H matrix(in Matrix Market format), specify -f:
///
///     $ ../bin/CoSP2-mpi -f my_hmatrix.mtx 
///
/// ------------------------------
///
/// To run with a chosen size a matrix generated:
///
///     $ ../bin/CoSP2Parallel -N 10000 -M 124
///
/// ------------------------------
///
/// To run with a chosen SP2 loop tolerance:
///
///     $ ../bin/CoSP2Parallel -N 20000 -M 40 -i 1.0E-08 
/// 
///
/// ------------------------------
///

/// \details Initialize a Command structure with default values, then
/// parse any command line arguments that were supplied to overwrite
/// defaults.
///
/// \param [in] argc the number of command line arguments
/// \param [in] argv the command line arguments array
Command parseCommandLine(int argc, char** argv)
{
   Command cmd;

   memset(cmd.hmatName, 0, 1024);
   cmd.N = 1600;
   cmd.M = 80;
   cmd.dout = 0;
   cmd.gen = 0;
   cmd.debug = 0;
   cmd.amp = 1.0;
   cmd.alpha = 1.0;
   cmd.eps = 1.0E-05;
   cmd.heps= 1.0E-16;
   cmd.idemTol = 1.0E-14;

   int help=0;
   // add arguments for processing.  Please update the html documentation too!
   addArg("help",       'h', 0, 'i',  &(help),             0,             "print this message");
   addArg("hmatName",   'f', 1, 's',  cmd.hmatName,   sizeof(cmd.hmatName), "H matrix file name");
   addArg("N",          'n', 1, 'i',  &(cmd.N),            0,             "rows in matrix");
   addArg("M",          'm', 1, 'i',  &(cmd.M),            0,             "non-zeroes per row");
   addArg("gen",        'g', 1, 'i',  &(cmd.gen),          0,             "generate H matrix");
   addArg("dout",       'd', 1, 'i',  &(cmd.dout),         0,             "write out density matrix");
   addArg("debug",      'b', 1, 'i',  &(cmd.debug),        0,             "write out debug messages");
   addArg("amp",        'a', 1, 'd',  &(cmd.amp),          0,             "amplitude");
   addArg("alpha",      'l', 1, 'd',  &(cmd.alpha),        0,             "alpha");
   addArg("eps",        'e', 1, 'd',  &(cmd.eps),          0,             "threshold for sparse math");
   addArg("heps",       'p', 1, 'd',  &(cmd.heps),         0,             "threshold for dense2sparse");
   addArg("idemtol",    'i', 1, 'd',  &(cmd.idemTol),      0,             "threshold for SP2 loop");

   processArgs(argc,argv);

   // If user didn't set hmatName, set to generate H matrix.
   if (strlen(cmd.hmatName) == 0) 
   {
     cmd.gen = 1;
   }
      
   if (help)
   {
      printArgs();
      freeArgs();
#ifdef DO_MPI
      destroyParallel();
#endif
      exit(0);
   }
   freeArgs();

   return cmd;
}
