/// \file
/// Generate Hamiltonian matrix.

#ifndef __GENERATE_H
#define __GENERATE_H

#include "sparseMatrix.h"
#include "mytype.h"

/// Generate Hamiltonian given N and M
SparseMatrix* generateHMatrix(int N, int M, real_t a, real_t alpha);

#endif
