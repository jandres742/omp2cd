/// \file
/// Handle command line arguments.

#ifndef MYCOMMAND_H
#define MYCOMMAND_H

#include <stdio.h>

#include "mytype.h"

/// A structure to hold the value of every run-time parameter that can
/// be read from the command line.
typedef struct CommandSt
{
   char hmatName[1024]; //!< name of the dense H matrix file
   int N;               //!< number of rows in H matrix (N x N)
   int M;               //!< max number of non-zeroes in H matrix row
   int dout;            //!< if == 1, write out density matrix
   int gen;             //!< if == 1, generate sparse hamiltonian
   int debug;           //!< if == 1, write out debug messages

   real_t amp;          //!< amplitude
   real_t alpha;        //!< alpha

   real_t eps;          //!< threshold for sparse math 
   real_t heps;         //!< threshold for dense->sparse conversion
   real_t idemTol;      //!< threshold for SP2 loop
} Command;

/// Process command line arguments into an easy to handle structure.
Command parseCommandLine(int argc, char** argv);

#endif
