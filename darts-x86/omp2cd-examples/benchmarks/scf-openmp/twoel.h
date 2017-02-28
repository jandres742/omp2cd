#ifndef _TWOEL_H
#define _TWOEL_H

#include "cscc.h"
#include "g.h"
#if !defined(__DARTS_) && !defined(__OMP2CD_SWARM_)
#include <omp.h>
#endif

#include <string.h>

double twoel_reducer(void);
double contract_matrices(double *, double *);

#define OFF(a, b)             ((a ##_off) + (b))
#define DENS(a, b)            (g_dens_ ## a ## _ ## b)
#define DENS_DECL(a, b)       double g_dens_ ## a ## _ ## b = g_dens[OFF(a,b)]

#ifdef BAD_CACHE_UTILIZATION
#define DENS_DECL_SYMM(a, b)  double g_dens_ ## a ## _ ## b = g_dens[OFF(a,b)]; \
                              double g_dens_ ## b ## _ ## a = g_dens[OFF(b,a)]
#else
#define DENS_DECL_SYMM(a, b)  double g_dens_ ## a ## _ ## b = g_dens[OFF(a,b)]; \
                              double g_dens_ ## b ## _ ## a = g_dens_ ## a ## _ ## b
#endif

#define TOL_DECL(a, b)        double tol2e_over_g_schwarz_ ## a ## _ ## b = tol2e / g_schwarz[OFF(a,b)]



#define UPDATE(a, b, c, d)    fock[OFF(a,b)] += (gg * DENS(c,d)); \
				   fock[OFF(a,c)] -= (0.50 * gg * DENS(b,d));


#define UPDATE1(a, b, c, d)    fock[OFF(a,b)] += (gg * DENS(c,d)); 
#define UPDATE2(a, b, c, d)    fock[OFF(a,c)] -= (0.50 * gg * DENS(b,d));

#define UPDATE_COULOMB1(a, b, c, d)  fock[OFF(a,b)] += (gg * DENS(c,d))
#define UPDATE_COULOMB2(a, b, c, d)  fock[OFF(a,b)] += (gg * DENS(c,d)) + (gg * DENS(c,d))
#define UPDATE_EXCHANGE(a, b, c, d)  fock[OFF(a,c)] -= (0.50 * gg * DENS(b,d));

#ifdef BOOKKEEPING
#define CUT_DECLS       long long int temp_icut1 = 0; \
                        long long int temp_icut2 = 0; \
                        long long int temp_icut3 = 0; \
                        long long int temp_icut4 = 0
#define CUT_WRITEBACK   icut1 += temp_icut1; \
                        icut2 += temp_icut2; \
                        icut3 += temp_icut3; \
                        icut4 += temp_icut4

#define CUT1(N)   temp_icut1 += N
#define CUT2(N)   temp_icut2 += N
#define CUT3(N)   temp_icut3 += N
#define CUT4(N)   temp_icut4 += N

#define ICUTVARS temp_icut1,temp_icut2,temp_icut3,temp_icut4

#define REDUCE_CLAUSE reduction(+:ICUTVARS) 
#else
#define CUT1(N)
#define CUT2(N)
#define CUT3(N)
#define CUT4(N)
#define CUT_DECLS
#define CUT_WRITEBACK
#define ICUTVARS
#define REDUCE_CLAUSE 
#endif

#ifndef NO_PRECALC
#define COMPUTE_G(a, b, c, d) g_fast(OFF(a, b), OFF(c, d))
#define CACHED_G(a, b, c, d) g_cached_fast(OFF(a, b), OFF(c, d))
#else
#define COMPUTE_G(a, b, c, d) g(a, b, c, d)
#define CACHED_G(a, b, c, d) g_cached(a, b, c, d)
#endif

#ifdef USE_CACHE
#define G CACHED_G
#else
#define G COMPUTE_G
#endif

#ifndef OMP_SCHEDULE
#define OMP_SCHEDULE schedule(runtime)
#endif


#define TTILE 5

void twoel_i_j_k_l_all_different(double tol2e_over_schwmax);
void twoel_i_eq_j(double tol2e_over_schwmax);
void twoel_k_eq_l(double tol2e_over_schwmax);
void twoel_ij_eq_kl(double tol2e_over_schwmax);
void twoel_i_eq_j_and_k_eq_l(double tol2e_over_schwmax);
void twoel_i_eq_j_eq_k_eq_l(double tol2e_over_schwmax);
double twoel_fast(double schwmax);
#endif