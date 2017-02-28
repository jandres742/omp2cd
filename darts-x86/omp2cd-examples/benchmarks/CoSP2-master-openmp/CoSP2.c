/// \file 
/// SP2 main program.
///
/// \mainpage CoSP2: A Linear Algebra Electronic Structure Mini-app
///
/// CoSP2 is a reference implementation of typical sparse linear algebra
/// algorithms and workloads for a quantum molecular dynamics (QMD)
/// electronic structure code, such as the Los Alamos Transferable
/// Tight-binding for Energetics (LATTE) code. The algorithm is based
/// on a recursive second-order Fermi-Operator expansion method (SP2) 
/// and is tailored for density functional based tight-binding calculations
/// of non-metallic material systems. It is composed of aseries of 
/// generalized matrix-matrix multiplications. It is created and maintained by
/// The Exascale Co-Design Center for Materials in Extreme Environments
/// (ExMatEx).  http://codesign.lanl.gov/projects/exmatex.  The
/// code is intended to serve as a vehicle for co-design by allowing
/// others to extend and/or reimplement it as needed to test performance of 
/// new architectures, programming models, etc.
///
/// The current version of CoSP2 is available from:
/// http://exmatex.github.io/CoSP2
///
/// Table of Contents
/// =================
///
/// Click on the links below to browse the CoSP2 documentation.
///
/// \subpage pg_qmd_basics
///
/// \subpage pg_building_cosp2
///
/// \subpage pg_running_cosp2
///
/// \subpage pg_measuring_performance
///
/// \subpage pg_problem_selection_and_scaling
///
/// \subpage pg_cosp2_architecture
///
/// \subpage pg_optimization_targets
///

#define MAIN_FILE

#include <stdio.h>
#include <omp.h>

#include "decomposition.h"
#include "sparseMatrix.h"
#include "generate.h"
#include "matrixio.h"
#include "sp2Loop.h"
#include "parallel.h"
#include "performance.h"
#include "mycommand.h"
#include "constants.h"

int msparse;
int hDim;
int M;
int debug;

real_t eps; 
real_t hEps; 
real_t idemTol; 


SparseMatrix* initSimulation(Command cmd)
{
  SparseMatrix* spH;

  // Read in size of hamiltonian matrix and max number of non-zeroes
  if (printRank()) printf("hDim = %d M = %d\n", hDim, msparse);

  if (cmd.gen == 0)
  {
    // Calculate M - max number of non-zeroes per row
    M = nnzStart(hDim, msparse);
  
    // Allocate sparse input hamiltonian matrix
    spH = initSparseMatrix(hDim, M);
 
    // Read in hamiltonian matrix (in Matrix Market format)
    startTimer(readhTimer);
    readMTX(cmd.hmatName, spH);
    stopTimer(readhTimer);
  
  }

  else
  {
    // Calculate M - max number of non-zeroes per row
    M = nnzStart(hDim, msparse);
    
    //Banded Hamiltonian is generated
    spH = generateHMatrix(hDim, M, cmd.amp, cmd.alpha);

  }

  return spH;
}

int main(int argc, char** argv)
{
  // Start
  initParallel(&argc, &argv);
  profileStart(totalTimer);
  profileStart(loopTimer);
  if (printRank()) printf("CoSP2: SP2 Loop\n");

  // Read in command line parameters
  Command cmd = parseCommandLine(argc, argv);
  msparse = cmd.M;
  hDim = cmd.N;
  debug = cmd.debug;
  eps = cmd.eps;
  hEps = cmd.heps;
  idemTol = cmd.idemTol;
  if (printRank())
  {
    printf("\nParameters:\n");
    printf("msparse = %d  hDim = %d  debug = %d\n", msparse, hDim, debug);
    printf("hmatName = %s\n", cmd.hmatName);
    printf("eps = %lg  hEps = %lg\n", eps, hEps);
    printf("idemTol = %lg\n\n", idemTol);
  }

  // Initialize
  startTimer(preTimer);
  SparseMatrix* spH = initSimulation(cmd);
  stopTimer(preTimer);

  // Calculate domain decomposition
  Domain* domain = initDecomposition(getNRanks(), spH->hsize, spH->msize);
  
  // Determine sparsity
  sparsity(spH);
  
  // Calculate gershgorin bounds for sparse matrix
  gershgorin(spH, domain);

  // Perform SP2 loop
  barrierParallel();
  sp2Loop(spH, domain);

  // Done
  profileStop(totalTimer);
  profileStop(loopTimer);

  /// Show timing results
  printPerformanceResults(spH->hsize, 0);

  /// Write out density matrix
  if (printRank() && cmd.dout == 1)
    writeMTX("dmatrix.out.mtx", spH);

  /// Deallocate matrices, etc.
  destroySparseMatrix(spH);
  destroyDecomposition(domain);

  destroyParallel();

  return 0;
}

// --------------------------------------------------------------
//
//
/// \page pg_building_comd Building CoSP2
///
/// CoSP2 is written with portability in mind and should compile using
/// practically any compiler that implements the C99 standard.  You will
/// need to create a Makefile by copying the sample provided with the
/// distribution (Makefile.vanilla).
///
///     $ cp Makefile.vanilla Makefile
///
/// and use the make command to build the code
///
///    $ make
///
/// The sample Makefile will compile the code on many platforms.  See
/// comments in Makefile.vanilla for information about specifying the
/// name of the C compiler, and/or additional compiler switches that
/// might be necessary for your platform.
///
/// The main options available in the Makefile are toggling single/double
/// precision and enabling/disabling MPI. In the event MPI is not
/// available, setting the DO_MPI flag to OFF will create a purely
/// serial build (you will likely also need to change the setting of
/// CC).
///
/// The makefile should handle all the dependency checking needed, via
/// makedepend.
///
/// 'make clean' removes the object and dependency files.
///
/// 'make distclean' additionally removes the executable file and the
/// documentation files.

//--------------------------------------------------------------


// --------------------------------------------------------------


/// \page pg_measuring_performance Measuring Performance
///
/// CoSP2 implements a simple and extensible system of internal timers
/// and counters to measure the performance profile of the code.
/// As explained in performance.c, it is easy to create additional
/// timers and counters and associate them with code regions of specific
/// interest.  In addition, the getTime() and getTick() functions can be
/// easily reimplemented to take advantage of platform specific timing
/// resources.
///
/// A performance report is printed at the end of each simulation.
///
/// 
/// Counters for Rank 0
///        Counter          Calls    Avg/Call(MB)         Total(MB)   
/// _________________________________________________________________
/// reduce                    35           0.0000            0.0005
/// send                      75           1.0479           78.5938
/// recv                      75           1.0485           78.6367
///
/// Counter Statistics Across 16 Ranks:
///         Counter      Rank: Min(MB)        Rank: Max(MB)       Avg(MB)      Stdev(MB)
/// _______________________________________________________________________________________
/// reduce               0:      0.0005       0:      0.0005        0.0005        0.0000
/// send                 0:     78.5938      14:     78.6784       78.6164        0.0313
/// recv                 4:     78.5955      14:     78.6768       78.6164        0.0251
///
///
/// Timings for Rank 0
///         Timer        # Calls    Avg/Call (s)   Total (s)    % Loop
/// ___________________________________________________________________
/// total                      1       0.7124        0.7124      100.00
/// loop                       1       0.7124        0.7124      100.00
///   pre                      1       0.1119        0.1119       15.71
///     readh                  1       0.0945        0.0945       13.27
///   sp2Loop                  1       0.5505        0.5505       77.28
///     norm                   1       0.0028        0.0028        0.40
///     x2                    31       0.0114        0.3527       49.51
///     xadd                  14       0.0011        0.0150        2.10
///     xset                  17       0.0010        0.0162        2.27
///     exchange              62       0.0022        0.1389       19.50
///     reduceComm            35       0.0005        0.0158        2.22
///
/// Timing Statistics Across 16 Ranks:
///         Timer        Rank: Min(s)       Rank: Max(s)      Avg(s)    Stdev(s)
/// _____________________________________________________________________________
/// total                3:    0.7068       8:    0.7239      0.7157      0.0042
/// loop                 3:    0.7068       8:    0.7239      0.7157      0.0042
///   pre               12:    0.1105       2:    0.1127      0.1115      0.0006
///     readh            4:    0.0944       8:    0.0951      0.0947      0.0002
///   sp2Loop            6:    0.5501       1:    0.5508      0.5504      0.0002
///     norm             2:    0.0026      14:    0.0030      0.0028      0.0001
///     x2               7:    0.3511       2:    0.3551      0.3531      0.0010
///     xadd            10:    0.0145      14:    0.0154      0.0149      0.0002
///     xset             9:    0.0155       2:    0.0166      0.0159      0.0003
///     exchange        10:    0.1338       1:    0.1394      0.1367      0.0017
///     reduceComm      14:    0.0081      10:    0.0219      0.0173      0.0039
///
///
/// This report consists of two blocks each for counters and timers.  The
/// first block for timers lists the absolute wall clock time spent in each
/// timer on rank 0 of the job. The first block for counters lists the sizes
/// of communication messages on rank 0 of the job. The second block for
/// timers and counters reports minimum, maximum, average, and standard
/// deviation of times or sizes across all tasks. The ranks where the
/// minimum and maximum values occured are also reported to aid in identifying
/// hotspots or load imbalances.
///
/// *** Architecture/Configuration for above timing numbers:
/// A cluster with dual-socket 8-core Intel Xeon SandyBridge E5-2670 processors.
/// Each node has a total of 16 cores and shares 64 GB of RAM.
///

// --------------------------------------------------------------


/// \page pg_problem_selection_and_scaling Problem Selection and Scaling
///
/// CoSP2 is a reference linear algebra electronic structure simulation code as used in
/// materials science.
///
/// Problem Specification 
/// ======================
///
/// CoSP2 requires an input Hamiltonian matrix. A generator is included that produces
/// a synthetic banded matrix similar to polyethylene. N (number of orbitals or rows)
/// and M (number of non-zeroes per row) must be specified. The extent of the band is
/// determined by N and M. SP2 matrix multiplication adds non-zero elements per row.
/// You may have to experiment to get a reasonable M.
///
/// Given a user-specified amplitude and alpha (--amp and --alpha command line options):
///
/// value = amplitude * random[0,1] * exp(-alpha * (i-j)^2)
///
/// A more general version based on atom positions:
///
/// value = amplitude * random[0,1] * exp(-alpha * (Ri-Rj)^2)
///         where (Ri - Rj) is the distance between atoms
///
/// Two Hamiltonian matrix examples are given in data/ that represent polyethylene 
/// chains of 512 and 1024 molecules. A good M value (or number of non-zeroes per
/// row) is 256 for each.
///
/// Scaling Studies in CoSP2
/// =======================
///
/// CoSP2 implements a simple domain decomposition dividing the Hamiltonian matrix
/// of N rows into sub-matrices of chunks of rows, each owned by an MPI rank.
/// Each domain is a single-program multiple data (SPMD) partition of the larger problem.
///
/// Weak Scaling
/// -----------
///
/// A weak scaling test fixes the amount of work per processor and
/// compares the execution time over number of processors. Weak scaling
/// keeps the ratio of inter-processor communication (surface) to
/// intra-processor work (volume) fixed. The amount of inter-processor
/// work scales with the number of processors in the domain. Run increasingly
/// larger problems over more processors, ex. size N on 1 MPI rank, 2N on 
/// 2 MPI ranks, etc.
///
///
/// Strong Scaling
/// ---------------
///
/// A strong scaling test fixes the total problem size and compares the
/// execution time for different numbers of processors. Strong scaling
/// increases the ratio of inter-processor communication (surface) to
/// intra-processor work (volume). Run with increased numbers of processors,
/// ex. 1,2,4,8,16 MPI ranks.
///
///
/// \page pg_cosp2_architecture CoSP2 Architecture
///
/// Program Flow
/// ============
///
/// We have attempted to make the program flow in CoSP2 1.0 as simple and
/// transparent as possible.  The main program consists of initialization,
/// the sparse SP2 loop, and end processing.
///
/// The initialization initializes MPI (if requested), reads in command line options, 
/// reads or generates the sparse Hamiltonian matrix. The SP2 loop is as follows. 
/// 
/// SP2 Algorithm:
///
///   breakLoop = 0
///   iter = 0
///   idempErr = idempErr1 = idempErr2 = 0
///
///   do while (breakLoop == 0 && iter < 100)
///   {
///     trX = 0
///     trX2 = 0
///
///     exchange halo information dwacross (if running MPI)
///
///     X2 = X * X
///
///     reduce trX and trX2 (if running MPI)
///
///     tr2XX2 = 2.0 * trX - trX2
///     trXOLD = trX
///     limDiff = |trX2 - occ| - |tr2XX2 - occ|
///
///     if (limDiff > idemTol)
///     {
///       X = 2.0 * X - X2
///       trX = 2.0 * trX - trX2    
///     }
///     else
///     {
///       X = X2
///       trX = trX2
///     }  
///
///     idempErr2 = idempErr1
///     idempErr1 = idempErr
///     idempErr = |trX - trXOLD|
///
///     iter++
///
///     if (iter >= 25 && (idempErr >= idempErr2)) breakLoop = 1
///
///     exchange relevant sparse matrix chunks of rows (if running MPI)
///
///   }
///
/// End processing consists of printing performance results and deleting
/// structures used.
///
/// Key Data Structures
/// ==================
/// Data structures used are the following.
/// - SparseMatrixSt Sparse matrix storage structure based on ELLPACK-R.
/// - DomainSt Decomposition of chunks of rows across MPI ranks.
/// - HaloExchangeSt Structure for communication of halo data.
///

/// \page pg_optimization_targets Optimization Targets
///
/// Computation
/// ============
///
/// The computational effort of electronic structure codes is usually focused on the 
/// density matrix build. Non-metalic systems produce sparse Hamiltonian matrices.
/// The SP2 algorithm takes advantage of efficient sparse matrix-matrix multiplication
/// and addition. Using the ELLPACK-R storage structure and multi-threading using
/// OpenMP, results in sub-second performance.
///
/// Communication
/// =============
///
/// As the number of atoms per MPI rank decreases, the communication
/// routines will start to require a significant fraction of the
/// run time.  The main communication routine in CoSP2 is haloExchange().
/// The halo exchange allows communication of matrix row chunks between
/// ranks that require them for computation. All-to-all is not required
/// to do the SP2 algorithm.
///
/// Initially, all ranks have the entire Hamiltonian matrix.
/// Information on which chunks are to be exchanged is communicated at
/// the start of each SP2 iteration. Chunks are exchanged at the end of
/// each iteration. Once the SP2 algorithm is done, the remaining chunks
/// are exchanged, so each rank contains the entire new density matrix.
///

// --------------------------------------------------------------


/// \page pg_qmd_basics QMD Basics
///
/// The molecular dynamics (MD) computer simulation method is a well
/// established and important tool for the study of the dynamical
/// properties of liquids, solids, and other systems of interest in
/// Materials Science and Engineering, Chemistry and Biology. A material
/// is represented in terms of atoms and molecules. The method of MD
/// simulation involves the evaluation of the force acting on each atom
/// due to all other atoms in the system and the numerical integration
/// of the Newtonian equations of motion. Though MD was initially
/// developed to compute the equilibrium thermodynamic behavior of
/// materials (equation of state), most recent applications have used MD
/// to study non-equilibrium processes.
///
/// Wikipeda offers a basic introduction to molecular dynamics with
/// many references:
///
/// http://en.wikipedia.org/wiki/Molecular_dynamics
///
/// Quantum Molecular Dynamics (QMD) simulations capture the making and 
/// breaking of covalent bonds, charge transfer between species of 
/// differing electronegativities, and long-range electrostatic interactions.
/// Electronic structure of atoms and molecules is modeled explicitly.
/// It provides the most accurate and reliable descriptions of interatomic bonding.
/// The prohibitive computational cost has prevented widespread use. Today, 
/// better algorithms, and multi-core and GPU implementations are important 
/// paths forward.
///
/// Wikipedia background on computational chemistry:
///
/// http://en.wikipedia.org/wiki/Computational_chemistry
///
/// References:
///
/// E. H. Rubensson, A. M. N. Niklasson, 2014,
/// Interior eigenvalues from density matrix expansions in quantum mechanical 
/// molecular dynamics, SIAM J. Sci. Comput. Vol. 36, No. 2, pp. B147–B170.
///
/// P. Souvatzis, A. M. N. Niklasson, 2014,
/// First principles molecular dynamics without self-consistent field 
/// optimization, J. Chem. Physics 140, 044117.
///
/// M. J. Cawkwell, E. J. Sanville, S. M. Mniszewski, A. M. N. Niklasson, 2012,
/// Computing the density matrix in electronic structure theory on
/// graphics processing units, J. Chem. Theory Comput., 8, 4094−4101. 
///
/// M. J. Cawkwell, A. M. N. Niklasson, 2012,
/// Energy conserving, linear scaling Born-Oppenheimer molecular dynamics,
/// J. Chem. Physics 137, 134105.
///
/// A. M. N. Niklasson, P., Steneteg, N. Bock, 2011,
/// Extended Lagrangian free energy molecular dynamics,
/// J. Chem. Physics 135, 164111.
///
/// A. M. N. Niklasson, 2008,
/// Extended Born-Oppenheimer molecular dynamics,
/// PRL 100, 123004.
///
/// A. M. N. Niklasson, 2002, 
/// Expansion algorithm for the density matrix,
/// Phys. Rev. B 66, 155115.
///
