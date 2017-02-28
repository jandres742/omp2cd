/// \file
/// Matrix I/O.

#ifndef __MATRIXIO_H
#define __MATRIXIO_H

#include "sparseMatrix.h"
#include "mytype.h"

/// Read/write hamiltonian and density matrices.
void writeSparsePattern(char* fname, struct SparseMatrixSt* spmatrix, real_t hthresh);
void readMTX(char* fname, struct SparseMatrixSt* spmatrix);
void writeMTX(char* fname, struct SparseMatrixSt* spmatrix);

#endif
