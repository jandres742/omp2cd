/* -*- mode: C; mode: folding; fill-column: 70; -*- */
/* Copyright 2010,  Georgia Institute of Technology, USA. */
/* See COPYING for license. */
#if !defined(RMAT_HEADER_)
#define RMAT_HEADER_

#include "compat.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include <assert.h>

#ifdef HAVE_ALLOCA_H
#include <alloca.h>
#endif

#include "xalloc.h"
#include "prng.h"
#include "generator/splittable_mrg.h"
#include "generator/graph_generator.h"

/** Fill IJ with a randomly permuted R-MAT generated edge list. */
void rmat_edgelist (struct packed_edge *IJ, int64_t nedge, int SCALE,
		    double A, double B, double C);
void permute_vertex_labels (struct packed_edge * restrict IJ, int64_t nedge, int64_t max_nvtx,
		       mrg_state * restrict st, int64_t * restrict newlabel);
void permute_edgelist (struct packed_edge * restrict IJ, int64_t nedge, mrg_state *st);

#endif /* RMAT_HEADER_ */
