/* C version of the SCF code */
#include "scf.h"

double * g_dens, * g_fock, * g_tfock, * g_schwarz, * g_work, * g_ident, * g_orbs, * offsets;
double * g_schwarz_max_j, * g_fock_const;
double * g_partial_fock;
double *  g_thread_fock;//for openmp

precalc * g_precalc;

double* g_cache = (double*)NULL;
size_t g_cache_index = 0;
size_t g_cache_size = 0;
size_t g_cached_values = 0;

double enrep, q[maxatom], ax[maxatom], ay[maxatom], az[maxatom], eigv[maxnbfn];
double x[maxnbfn], y[maxnbfn], z[maxnbfn], expnt[maxnbfn], rnorm[maxnbfn];
long long int iky[maxnbfn], nocc, nbfn, nnbfn;
long long int icut1,icut2,icut3,icut4;
int natom;

double fm[2001][5];
double rdelta, delta, delo2;

void mkpre(void) {
  int i, j, j_off=0;

  for (j = 0; j < nbfn; j++) {
    for (i = 0; i < nbfn; i++) {
      precalc* pre = &g_precalc[j_off + i];

      double dx = x[i] - x[j];
      double dy = y[i] - y[j];
      double dz = z[i] - z[j];

      double expntIJ = expnt[i] + expnt[j];

      double r2 = dx * dx + dy * dy + dz * dz;
      double fac  = expnt[i] * expnt[j] / expntIJ;

      pre->facr2 = exp(-fac * r2);
      pre->expnt = expntIJ;

      pre->x = (x[i] * expnt[i] + x[j] * expnt[j]) / expntIJ;
      pre->y = (y[i] * expnt[i] + y[j] * expnt[j]) / expntIJ;
      pre->z = (z[i] * expnt[i] + z[j] * expnt[j]) / expntIJ;

      pre->rnorm = rnorm[i] * rnorm[j];
    }
    j_off += nbfn;
  }
}

double tdgemm = 0.0;

int main(int argc, char **argv) {
  long long int nints, maxint; 

//     CAUTION: int precision requirements;
//     nints, maxint, etc. are proportional to the number of basis functions to the
//     fourth power! 216^4 is greater than the largest number that can be represented
//     as a 32-bit signed interger, so 64-bit arithmetic is needed to count integrals
//     when calculating more than 14 Be atoms with 15 basis functions each. Since
//     integrals are counted over all iterations, 18 iterations with 7 atoms can result
//     in precision problems. Note that the wave function could be calculated correctly;
//     for much larger basis sets without 64-bit ints because the required indexing is
//     usually proportional to nbfn^2, which is good to 46,340 basis functions.

  double tinit = 0.0, tonel = 0.0, ttwoel = 0.0, tdiag  = 0.0, tdens = 0.0, tprint = 0.0;
  double eone  = 0.0, etwo  = 0.0, energy = 0.0, deltad = 0.0;
      
  int iter;
  double frac, scale, totsec, tester, schwmax;
//ENTER_LOG(timer);
  // init the timer
  timer();

// get input from file be.inpt;
  input();

// create and allocate global arrays;
  setarrays();
  ininrm();

  mkpre();

// create initial guess for density matrix by using single atom densities;
  denges();


// make initial orthogonal orbital set for solution method using similarity transform;
  makeob();

// make info for sparsity test;
  schwmax = makesz();

  oneel(schwmax);
  memcpy(g_fock_const, g_fock, nbfn * nbfn * sizeof(double));

  // get the actual time spent initializing
  tinit = timer();

 //EXIT_LOG(timer);

// *** ITERATE ****
  for (iter = 0; iter <  mxiter; iter++) {
  // for (iter = 0; iter <  1; iter++) {
    
    double i_timer = timer2();
   
   //ENTER_LOG(oneel);
// make the one particle contribution to the fock matrix and get the partial contribution to the energy;
    memcpy(g_fock, g_fock_const, nbfn * nbfn * sizeof(double));
    eone = 0.50 * contract_matrices(g_fock, g_dens);
    tonel = tonel + timer();
   //EXIT_LOG(oneel);
// compute the two particle contributions to the fock matrix and get the total energy;

    etwo   = twoel_fast(schwmax);

    double ttwoel_temp = timer();
    ttwoel = ttwoel + ttwoel_temp;

// diagonalize the fock matrix.

    //ENTER_LOG(diagon);
    tester = diagon(iter);
	(void) tester;
    
    double tdiag_temp = timer();
    tdiag  = tdiag + tdiag_temp;
    
    //EXIT_LOG(diagon);

// make the new density matrix in g_work from orbitals in g_orbs, compute the norm of the change in
// the density matrix and then update the density matrix in g_dens with damping.;

    //ENTER_LOG( makden);
    makden();
    //EXIT_LOG( makden);
    //ENTER_LOG(dendif);
    deltad = dendif();
    //EXIT_LOG(dendif);

    if (iter == 0)
       scale = 0.00;
    else if (iter < 5)
       scale = (nbfn > 60) ? 0.50 : 0.00;
    else
       scale = 0.00;

    damp(scale);
    tdens = tdens + timer();
      
    i_timer = timer2() - i_timer;

// add up energy and print out convergence information;
    energy = enrep + eone + etwo;
    printf(" iter= %3d, energy=%15.8f, time=%10.4f, ttwoel=%10.4f, tdiag=%10.4f\n", iter, energy, i_timer, ttwoel_temp, tdiag_temp);
    tprint = tprint + timer();
    
// if converged exit iteration loop;
    if (deltad < tol) break;

#ifdef BOOKKEEPING
// something has gone wrong--print what you know and quit;
    if (icut3 == 0) {
       printf("no two-electron integrals computed!\n");
       printf("SCF failed to converge in %d iters\n", iter);
       break;
  }
#endif
  }

  if (iter == mxiter) {
      printf("SCF failed to converge in %d iters\n", iter);
      iter--;
  }

//...v....1....v....2....v....3....v....4....v....5....v....6....v....7..;
//
//     finished ... print out eigenvalues and occupied orbitals;
//

// print out timing information;

  printf("\nfinal energy = %18.11f\n",energy);

  printf("      init       onel      twoel       diag       dens       print       eigen    \n");
  printf("      ----       ----      -----       ----       ----       ----        ----    \n");

  printf("%10.2f %10.2f %10.2f %10.2f %10.2f %10.2f %10.2f", tinit, tonel, ttwoel, tdiag, tdens, tprint, tdgemm);

  totsec = tinit + tonel + ttwoel + tdiag + tdens + tprint;
  
  //====================================================================//
  printf("\n twoel time in seconds %10.2f", ttwoel);
  //====================================================================//
  
  printf("\n elapsed time in seconds %10.2f\n\n", totsec);

#ifdef USE_CACHE
  printf("Total number of cache values:%lu\n", cache_size());
  printf("Total cache capacity:%lu\n\n", cache_capacity());
#endif

  // print out information on # integrals evaluated each iteration;
  nints = icut1 + icut2 + icut3 + icut4;
  frac = (double)icut3 / (double)nints;

  printf("No. of integrals screened or computed (all iters) \n\n");
  printf("-------------------------------------\n");
  printf("  failed #ij test   failed #j test  failed #kl test    #compute           #total       fraction\n");
  printf("  ---------------  ---------------  ---------------  ---------------  ---------------  --------\n");
  printf("  %15lld  %15lld  %15lld  %15lld  %15lld  %9.6f\n", icut1, icut4, icut2, icut3, nints, frac);
         
  maxint = nbfn;
  maxint = pow(maxint, 4) * (iter + 1);

  if (nints != maxint) {
    printf("Inconsistent number of integrals, should be %lld\n", maxint);
    printf("Note: largest 32-bit int is 2,147,483,647\n");
  }
 
  closearrays();

  return 0;
}

double makesz() {
  int i, j;
  double smax = 0.0;

    for (j = 0; j < nbfn; j++) {
      double jmax = 0.0;
    for (i = 0; i < nbfn; i++) {
    // g_fast can be called instead here, but NEVER use a caching variant
    // of g unless it actually looks up the result based on the arguments
    double gg = sqrt( g(i, j, i, j) );
    if (gg > smax) smax = gg;
    g_schwarz[OFFSET(i,j)] = gg;
    if (gg > jmax) jmax = gg;
  }
  g_schwarz_max_j[j] = jmax;
  }

  return smax;
}

void ininrm(void)  {
  int i;
  long long int bf4 = pow((long long int) nbfn, 4);
      
// write a little welcome message;
  printf(" Example Direct Self Consistent Field Program \n");
  printf(" -------------------------------------------- \n\n");
  printf(" no. of atoms .............. %5d\n", natom);
  printf(" no. of occupied orbitals .. %5lld\n", nocc);
  printf(" no. of basis functions .... %5lld\n", nbfn);
  printf(" basis functions^4 ......... %5lld\n", bf4);
  printf(" convergence threshold ..... %9.4f\n", tol);

// generate normalisation coefficients for the basis functions and the index array iky;
  for (i = 0; i < nbfn; i++) iky[i] = (i + 1) * i / 2;
  for (i = 0;i < nbfn;i++) rnorm[i] = pow((expnt[i] * 2.00 / pi), 0.750);

// initialize common for computing f0;
  setfm();
}

void setfm(void)
{
  int i, ii;
  double t[2001];
  double et[2001], rr, tt;

  delta  = 0.014;
  delo2  = delta * 0.5;
  rdelta = 1.0 / delta;

  for (i = 0; i < 2001; i++) {
    tt       = delta * (double)i;
    et[i]    = exprjh(-tt);
    t[i]     = tt * 2.0;
    fm[i][4] = 0.0;
  }

  for (i = 199; i > 3; i--) {
    rr = 1.0 / (double)(2 * i + 1);
    for (ii = 0; ii < 2001; ii++) fm[ii][4] = (et[ii] + t[ii] * fm[ii][4]) * rr;
  }

  for (i = 3; i >= 0; i--) {
    rr = 1.0 / (double) (2 * i + 1);
    for (ii = 0; ii < 2001; ii++) fm[ii][i] = (et[ii] + t[ii] * fm[ii][i+1]) * rr;
  }

  return;
}

// generate the one particle hamiltonian matrix element over
// the normalized primitive 1s functions i and j;
double h(int i,int j) {
  double f0val = 0.00, sum = 0.00;
  double facij,expij,repij, xp,yp,zp,rpc2, rab2;
  int iat;
     
  rab2  = (x[i]-x[j])*(x[i]-x[j]) + (y[i]-y[j])*(y[i]-y[j]) + (z[i]-z[j])*(z[i]-z[j]);
  facij = expnt[i]*expnt[j]/(expnt[i]+expnt[j]);
  expij = exprjh(-facij * rab2);
  repij = (2.00 * pi / (expnt[i] + expnt[j])) * expij;

// first do the nuclear attraction integrals;
  for (iat = 0;iat < natom;iat++) {
      xp = (x[i]*expnt[i] + x[j]*expnt[j])/(expnt[i]+expnt[j]);
      yp = (y[i]*expnt[i] + y[j]*expnt[j])/(expnt[i]+expnt[j]);
      zp = (z[i]*expnt[i] + z[j]*expnt[j])/(expnt[i]+expnt[j]);
      rpc2 = (xp-ax[iat])*(xp-ax[iat]) + (yp-ay[iat])*(yp-ay[iat]) + (zp-az[iat])*(zp-az[iat]);
      f0val = f0((expnt[i] + expnt[j]) * rpc2);
      sum = sum - repij * q[iat] * f0val;
   }

// add on the kinetic energy term;
   sum = sum + facij*(3.00-2.00*facij*rab2) * pow((pi/(expnt[i]+expnt[j])),1.50) * expij;

// finally multiply by the normalization constants;
   return sum * rnorm[i] * rnorm[j];
}


// generate the overlap matrix element between the normalized primitve gaussian 1s functions i and j;
double s(int i, int j) {
  double rab2, facij, temp;

  rab2  = (x[i] - x[j]) * (x[i] - x[j]) + (y[i] - y[j]) * (y[i] - y[j]) + (z[i] - z[j]) * (z[i] - z[j]);
  facij = expnt[i] * expnt[j] / (expnt[i] + expnt[j]);
  temp  = pow((pi / (expnt[i] + expnt[j])), 1.50) * exprjh(-facij * rab2) * rnorm[i] * rnorm[j];

  return temp;
}


// generate density matrix from orbitals in g_orbs. the first nocc orbitals are doubly occupied.
void makden(void) {
#ifdef BLAS
  cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasTrans, \
              nbfn, nbfn, nocc, 2.0, g_orbs, nbfn, g_orbs, nbfn, 0.0, g_work, nbfn);
#else
  int i, j, k;

  for (i = 0; i < nbfn; i++) {
  for (j = 0; j < nbfn; j++) {
    double p = 0.0;
    for (k = 0; k < nocc; k++) p = p + g_orbs[OFFSET(i,k)] * g_orbs[OFFSET(j,k)];
    g_work[OFFSET(i,j)] = 2.0 * p;
  } }
#endif
}


// fill in the one-electron part of the fock matrix and compute the one-electron energy contribution;
double oneel(double schwmax) {
  int i, j, i_off=0;
  double tol2e_over_schwmax = tol2e / schwmax;
  for (i = 0; i < nbfn; i++) {
  for (j = 0; j < nbfn; j++) {
    g_fock[i_off + j] = (g_schwarz[i_off + j]  > tol2e_over_schwmax) ? h(i, j) : 0.0;
  }
  i_off += nbfn;
  }

  return (0.50 * contract_matrices(g_fock, g_dens));
}


// create damped density matrix as a linear combination of old density matrix
// and density matrix formed from new orbitals;
void damp(double fac) {
#ifdef BLAS_L1
  cblas_dscal(nbfn*nbfn, fac, g_dens, 1);
  cblas_daxpy(nbfn*nbfn, 1.0 - fac, g_work, 1, g_dens, 1);
#else
  int i;
  double ofac = 1.00 - fac;
  double* dens = g_dens;
  double* work = g_work;

  for (i = 0; i < nbfn * nbfn; i++) {
    *dens = (fac * *dens) + (ofac * *(work++));
    dens++;
  }
#endif
}


// compute largest change in {density - work} matrix elements;
double dendif(void) {
  double denmax = 0.00;
#ifdef BLAS_L1
  cblas_dcopy(nbfn*nbfn, g_dens, 1, g_tfock, 1);
  cblas_daxpy(nbfn*nbfn, -1.0, g_work, 1, g_tfock, 1);
  denmax = fabs(g_tfock[cblas_idamax(nbfn*nbfn, g_tfock, 1)]);
#else
  int i;
  double* dens = g_dens;
  double* work = g_work;

  for (i = 0; i < nbfn * nbfn; i++) {
    double xdiff = fabs(*(dens++) - *(work++));
    if (xdiff > denmax) denmax = xdiff;
  }
#endif

  return denmax;
}


double diagon(int iter) {
  int i, j, i_off=0;
  double shift, tester = 0.00;

// use similarity transform to solve standard eigenvalue problem
// (overlap matrix has been transformed out of the problem);

#ifdef BLAS
  cblas_dsymm(CblasRowMajor, CblasLeft, CblasUpper, \
      nbfn, nbfn, 1.0, g_fock, nbfn, g_orbs, nbfn, 0.0, g_tfock, nbfn);
  cblas_dgemm(CblasRowMajor, CblasTrans, CblasNoTrans, \
      nbfn, nbfn, nbfn, 1.0, g_orbs, nbfn, g_tfock, nbfn, 0.0, g_fock, nbfn);
#else
  Dgemm('n', 'n', nbfn, nbfn, nbfn, g_fock, g_orbs,  g_tfock);
  Dgemm('t', 'n', nbfn, nbfn, nbfn, g_orbs, g_tfock, g_fock );
#endif

// compute largest change in off-diagonal fock matrix elements;
  for (i = 0; i < nbfn; i++) {
  for (j = 0; j < nbfn; j++) {
      if (i == j) continue;
      double xtmp = fabs(g_fock[i_off + j]);
      if (xtmp > tester) tester = xtmp;
  }
  i_off += nbfn;
  }

  shift = 0.00;
  if      (tester > 0.30) shift = 0.30;
  else if (nbfn   > 60  ) shift = 0.10;
  else                    shift = 0.00;
      
  if (iter >= 1 && shift != 0.00) 
     for (i = nocc; i < nbfn; i++) g_fock[OFFSET(i,i)] += shift;


  memcpy(g_tfock, g_orbs, nbfn*nbfn * sizeof(double));

  double t0 = timer2();
  Eigen_std(g_fock, g_work, eigv);
  tdgemm += (timer2() - t0);

// back transform eigenvectors;
#ifdef BLAS
  cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, \
      nbfn, nbfn, nbfn, 1.0, g_tfock, nbfn, g_work, nbfn, 0.0, g_orbs, nbfn);
#else
  Dgemm('n', 'n', nbfn, nbfn, nbfn, g_tfock, g_work, g_orbs);
#endif

  if (iter >= 1 && shift != 0.00)
    for (i = nocc; i < nbfn; i++) eigv[i] = eigv[i] - shift;

  return tester;
}


// generate set of orthonormal vectors by creating a random symmetric matrix
// and solving associated generalized eigenvalue problem using the correct overlap matrix.
void makeob(void) {
  int i, j;
  double eval[maxnbfn];

  for (i = 0; i < nbfn; i ++) {
  for (j = 0; j < nbfn; j ++) {
      g_ident[OFFSET(i,j)] = s(i, j);
      g_fock [OFFSET(i,j)] = 0.5;
  } }
  
  Eigen_gen(g_fock, g_ident, g_orbs, eval);
  return;
}


// form guess density from superposition of atomic densities in the AO basis set ...
// instead of doing the atomic SCF hardwire for this small basis set for the Be atom;
void denges(void) {
  int i, j, k, l;

  double atdens[15][15] = {
    {0.000002,0.000027,0.000129,0.000428,0.000950,0.001180,
     0.000457,-0.000270,-0.000271,0.000004,0.000004,0.000004,
     0.000004,0.000004,0.000004},
    {0.000027,0.000102,0.000987,
     0.003269,0.007254,0.009007,0.003492,-0.002099,-0.002108,
     0.000035,0.000035,0.000035,0.000035,0.000035,0.000035},
    {0.000129,0.000987,0.002381,0.015766,0.034988,0.043433,
     0.016835,-0.010038,-0.010082,0.000166,0.000166,0.000166,
     0.000166,0.000166,0.000166},
    {0.000428,0.003269,0.015766,
     0.026100,0.115858,0.144064,0.055967,-0.035878,-0.035990,
     0.000584,0.000584,0.000584,0.000584,0.000584,0.000584},
    {0.000950,0.007254,0.034988,0.115858,0.128586,0.320120,
     0.124539,-0.083334,-0.083536,0.001346,0.001346,0.001346,
     0.001346,0.001346,0.001346},
    {0.001180,0.009007,0.043433,
     0.144064,0.320120,0.201952,0.159935,-0.162762,-0.162267,
     0.002471,0.002471,0.002471,0.002471,0.002471,0.002471},
    {0.000457,0.003492,0.016835,0.055967,0.124539,0.159935,
     0.032378,-0.093780,-0.093202,0.001372,0.001372,0.001372,
     0.001372,0.001372,0.001372},
    {-0.000270,-0.002099,-0.010038,
     -0.035878,-0.083334,-0.162762,-0.093780,0.334488,0.660918,
     -0.009090,-0.009090,-0.009090,-0.009090,-0.009090,-0.009090},
    {-0.000271,-0.002108,-0.010082,-0.035990,-0.083536,-0.162267,
     -0.093202,0.660918,0.326482,-0.008982,-0.008982,-0.008981,
     -0.008981,-0.008981,-0.008982},
    {0.000004,0.000035,0.000166,
     0.000584,0.001346,0.002471,0.001372,-0.009090,-0.008982,
     0.000062,0.000124,0.000124,0.000124,0.000124,0.000124},
    {0.000004,0.000035,0.000166,0.000584,0.001346,0.002471,
     0.001372,-0.009090,-0.008982,0.000124,0.000062,0.000124,
     0.000124,0.000124,0.000124},
    {0.000004,0.000035,0.000166,
     0.000584,0.001346,0.002471,0.001372,-0.009090,-0.008981,
     0.000124,0.000124,0.000062,0.000124,0.000124,0.000124},
    {0.000004,0.000035,0.000166,0.000584,0.001346,0.002471,
     0.001372,-0.009090,-0.008981,0.000124,0.000124,0.000124,
     0.000062,0.000124,0.000124},
    {0.000004,0.000035,0.000166,
     0.000584,0.001346,0.002471,0.001372,-0.009090,-0.008981,
     0.000124,0.000124,0.000124,0.000124,0.000062,0.000124},
    {0.000004,0.000035,0.000166,0.000584,0.001346,0.002471,
     0.001372,-0.009090,-0.008982,0.000124,0.000124,0.000124,
     0.000124,0.000124,0.000062}};
      
// correct for a factor of two along the diagonal;
  for (i = 0; i < 15; i++) atdens[i][i] = 2.00 * atdens[i][i];

// fill in each block of 15 rows and 15 columns
  for (i = 0; i < nbfn; i++)
  for (j = 0; j < nbfn; j++) g_dens[OFFSET(i,j)] = 0.0;

  for (i = 0; i < nbfn; i += 15) {
      for (k = 0; k < 15; k++) {
      for (l = 0; l < 15; l++) {
          g_dens[OFFSET(i+k,i+l)] = atdens[k][l];
  } } }

  return;
}


void setarrays(void) {
  int i, j, j_off=0;

  g_dens    = (double *) malloc(nbfn * nbfn * sizeof(double));
  g_schwarz = (double *) malloc(nbfn * nbfn * sizeof(double));
  g_fock    = (double *) malloc(nbfn * nbfn * sizeof(double));
  g_tfock   = (double *) malloc(nbfn * nbfn * sizeof(double));
  g_work    = (double *) malloc(nbfn * nbfn * sizeof(double));
  g_ident   = (double *) malloc(nbfn * nbfn * sizeof(double));
  g_orbs    = (double *) malloc(nbfn * nbfn * sizeof(double));
  g_schwarz_max_j = (double*) malloc(nbfn * sizeof(double));
  g_precalc = (precalc*) malloc(nbfn * nbfn * sizeof(precalc));
  g_fock_const = (double *) malloc(nbfn * nbfn * sizeof(double));
  g_partial_fock = (double *) calloc(nbfn * nbfn * sizeof(double), 1);

  #if OMP_VERSION > 2

  //separate malloc for each gfock temp buffer in hopes it'll help with caching
  g_thread_fock = (double * ) calloc( omp_get_max_threads() * nbfn * nbfn * sizeof(g_thread_fock ),1);
  
#endif
  
  for (j = 0; j < nbfn; j ++) {
      g_schwarz_max_j[j] = 0.0;

  for (i = 0; i < nbfn; i ++) {
      g_dens[j_off + i]    = 0.0;
      g_schwarz[j_off + i] = 0.0;
      g_fock[j_off + i]    = 0.0;
      g_tfock[j_off + i]   = 0.0;
      g_work[j_off + i]    = 0.0;
      g_ident[j_off + i]   = 0.0;
      g_orbs [j_off + i]   = 0.0;
  }
  j_off += nbfn;
  }

  return;
}

void closearrays(void) {
  free(g_dens);  free(g_fock); free(g_schwarz);
  free(g_tfock); free(g_work); free(g_ident); free(g_orbs);
  free(g_schwarz_max_j); free(g_fock_const); free(g_partial_fock);

   #if OMP_VERSION > 2


       free(g_thread_fock);

#endif

  if (g_cache) free(g_cache);

  return;
}


// evalute sum_ij a_ij * b_ij;
double contract_matrices(double * a, double * b) {
#ifdef BLAS
  return cblas_ddot(nbfn*nbfn, a, 1, b, 1);
#else
  int i;
  double value = 0.0;

  for (i = 0; i < nbfn * nbfn; i++) {
    value += *(a++) * *(b++);
  }

  return value;

#endif
}
