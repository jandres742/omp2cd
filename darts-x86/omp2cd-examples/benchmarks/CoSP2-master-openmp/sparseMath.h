/// \file
/// Sparse matrix math routines.

#ifndef __SPARSEMATH_H
#define __SPARSEMATH_H

#include "sparseMatrix.h"

#include <stdio.h>

#include "decomposition.h"
#include "mytype.h"


#include <stdio.h>
#include <string.h>
#include <math.h>
#ifndef __DARTS_
#include <omp.h>
#endif

#include "sparseMatrix.h"
#include "parallel.h"
#include "constants.h"


void sparseX2(real_t* trX, real_t* trX2, struct SparseMatrixSt* xmatrix, struct SparseMatrixSt* x2matrix, struct DomainSt* domain);
void sparseAdd(struct SparseMatrixSt* xmatrix, struct SparseMatrixSt* x2matrix, struct DomainSt* domain);
void sparseSetX2(struct SparseMatrixSt* xmatrix, struct SparseMatrixSt* x2matrix, struct DomainSt* domain);
void sparseMultScalar(struct SparseMatrixSt* xmatrix, struct DomainSt* domain, real_t scalar);

#endif
