/// \file
/// SP2 loop functions.

#ifndef __SP2LOOP_H
#define __SP2LOOP_H

#include <stdio.h>

#include "decomposition.h"
#include "sparseMatrix.h"
#include "mytype.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "sparseMatrix.h"
#include "sparseMath.h"
#include "haloExchange.h"
#include "performance.h"
#include "parallel.h"
#include "matrixio.h"
#include "constants.h"


void sp2Loop(struct SparseMatrixSt* xmatrix, struct DomainSt* domain);
void reportResults(int iter, struct SparseMatrixSt* xmatrix, struct SparseMatrixSt* x2matrix, struct DomainSt* domain);

#endif
