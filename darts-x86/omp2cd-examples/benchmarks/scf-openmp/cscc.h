#define maxatom 384
#define maxnbfn 15 * maxatom
#define mxiter 30
#define maxnnbfn maxnbfn * (maxnbfn + 1) / 2
#define pi 3.141592653589793
#define two_pi_pow_2_5 34.98683665524972496996269910596311092376708984375
#define tol 0.006
#define tol2e 0.000001

#define MAX(a,b) (((a) >= (b)) ? (a) : (b))
#define MIN(a,b) (((a) <= (b)) ? (a) : (b))

extern double * g_dens, * g_fock, * g_tfock, * g_schwarz, * g_work, * g_ident, * g_orbs, *g_schwarz_max_j;
extern double * g_partial_fock;

// used to parallelize per openmp thread.. initialized/malloced in scf.c because it is reused until convergence... can represent
extern double * g_thread_fock;

extern double enrep, q[maxatom], ax[maxatom], ay[maxatom], az[maxatom], eigv[maxnbfn];
extern double x[maxnbfn], y[maxnbfn], z[maxnbfn], expnt[maxnbfn], rnorm[maxnbfn];
extern long long int iky[maxnbfn], nocc, nbfn, nnbfn;
extern long long int icut1,icut2,icut3,icut4;
extern int natom;



#define OFFSET(a,b) (((a) * nbfn) + (b))
