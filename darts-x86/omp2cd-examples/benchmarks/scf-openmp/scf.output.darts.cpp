#include "scf.output.darts.h"
using namespace darts;
using namespace std;
double ax[384];
double ay[384];
double az[384];
double delo2;
double delta;
double eigv[5760];
double enrep;
double expnt[5760];
double fm[2001][5];
double* g_cache = (double*)((void*)0);
size_t g_cache_index = 0;
size_t g_cache_size = 0;
size_t g_cached_values = 0;
double* g_dens;
double* g_fock;
double* g_fock_const;
double* g_ident;
double* g_orbs;
double* g_partial_fock;
precalc* g_precalc;
double* g_schwarz;
double* g_schwarz_max_j;
double* g_tfock;
double* g_thread_fock;
double* g_work;
long long icut1;
long long icut2;
long long icut3;
long long icut4;
long long iky[5760];
int natom;
long long nbfn;
long long nnbfn;
long long nocc;
double* offsets;
double q[384];
double rdelta;
double rnorm[5760];
double tdgemm = 0.;
double x[5760];
double y[5760];
double z[5760];
/*Function: mkpre, ID: 11*/
void mkpre()
{
    /*mkpre:11*/
    /*CompoundStmt:1845*/
    int i, j, j_off = 0;
    for (j = 0; j < nbfn; j++) {
        for (i = 0; i < nbfn; i++) {
            precalc* pre = &g_precalc[j_off + i];
            double dx = x[i] - x[j];
            double dy = y[i] - y[j];
            double dz = z[i] - z[j];
            double expntIJ = expnt[i] + expnt[j];
            double r2 = dx * dx + dy * dy + dz * dz;
            double fac = expnt[i] * expnt[j] / expntIJ;
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
/*Function: main, ID: 12*/
int main(int argc, char** argv)
{
    getOMPNumThreads();
    getOMPSchedulePolicy();
    getTPLoopThresholds();
    getNumTPs();
    affin = new ThreadAffinity(
        ompNumThreads / NUMTPS - 1, NUMTPS, COMPACT, getDARTSTPPolicy(), getDARTSMCPolicy());
    affinMaskRes = affin->generateMask();
    myDARTSRuntime = new Runtime(affin);
    RuntimeFinalCodelet = &(myDARTSRuntime->finalSignal);
    /*main:12*/
    /*CompoundStmt:1932*/
    long long nints, maxint;
    double tinit = 0., tonel = 0., ttwoel = 0., tdiag = 0., tdens = 0., tprint = 0.;
    double eone = 0., etwo = 0., energy = 0., deltad = 0.;
    int iter;
    double frac, scale, totsec, tester, schwmax;
    timer();
    input();
    setarrays();
    ininrm();
    mkpre();
    denges();
    makeob();
    schwmax = makesz();
    oneel(schwmax);
    memcpy(g_fock_const, g_fock, nbfn * nbfn * sizeof(double));
    tinit = timer();
    for (iter = 0; iter < 30; iter++) {
        double i_timer = timer2();
        memcpy(g_fock, g_fock_const, nbfn * nbfn * sizeof(double));
        eone = 0.5 * contract_matrices(g_fock, g_dens);
        tonel = tonel + timer();
        etwo = twoel_fast(schwmax);
        double ttwoel_temp = timer();
        ttwoel = ttwoel + ttwoel_temp;
        tester = diagon(iter);
        (void)tester;
        double tdiag_temp = timer();
        tdiag = tdiag + tdiag_temp;
        makden();
        deltad = dendif();
        if (iter == 0)
            scale = 0.;
        else if (iter < 5)
            scale = (nbfn > 60) ? 0.5 : 0.;
        else
            scale = 0.;
        damp(scale);
        tdens = tdens + timer();
        i_timer = timer2() - i_timer;
        energy = enrep + eone + etwo;
        printf(" iter= %3d, energy=%15.8f, time=%10.4f, ttwoel=%10.4f, tdiag=%10.4f\n", iter,
            energy, i_timer, ttwoel_temp, tdiag_temp);
        tprint = tprint + timer();
        if (deltad < 0.0060000000000000001)
            break;
        if (icut3 == 0) {
            printf("no two-electron integrals computed!\n");
            printf("SCF failed to converge in %d iters\n", iter);
            break;
        }
    }
    if (iter == 30) {
        printf("SCF failed to converge in %d iters\n", iter);
        iter--;
    }
    printf("\nfinal energy = %18.11f\n", energy);
    printf("      init       onel      twoel       diag       dens       print       eigen    \n");
    printf("      ----       ----      -----       ----       ----       ----        ----    \n");
    printf("%10.2f %10.2f %10.2f %10.2f %10.2f %10.2f %10.2f", tinit, tonel, ttwoel, tdiag, tdens,
        tprint, tdgemm);
    totsec = tinit + tonel + ttwoel + tdiag + tdens + tprint;
    printf("\n twoel time in seconds %10.2f", ttwoel);
    printf("\n elapsed time in seconds %10.2f\n\n", totsec);
    nints = icut1 + icut2 + icut3 + icut4;
    frac = (double)icut3 / (double)nints;
    printf("No. of integrals screened or computed (all iters) \n\n");
    printf("-------------------------------------\n");
    printf("  failed #ij test   failed #j test  failed #kl test    #compute           #total       "
           "fraction\n");
    printf("  ---------------  ---------------  ---------------  ---------------  ---------------  "
           "--------\n");
    printf("  %15lld  %15lld  %15lld  %15lld  %15lld  %9.6f\n", icut1, icut4, icut2, icut3, nints,
        frac);
    maxint = nbfn;
    maxint = pow(maxint, 4) * (iter + 1);
    if (nints != maxint) {
        printf("Inconsistent number of integrals, should be %lld\n", maxint);
        printf("Note: largest 32-bit int is 2,147,483,647\n");
    }
    closearrays();
    return 0;
}
/*Function: makesz, ID: 13*/
double makesz()
{
    /*makesz:13*/
    /*CompoundStmt:2081*/
    int i, j;
    double smax = 0.;
    for (j = 0; j < nbfn; j++) {
        double jmax = 0.;
        for (i = 0; i < nbfn; i++) {
            double gg = sqrt(g(i, j, i, j));
            if (gg > smax)
                smax = gg;
            g_schwarz[(((i)*nbfn) + (j))] = gg;
            if (gg > jmax)
                jmax = gg;
        }
        g_schwarz_max_j[j] = jmax;
    }
    return smax;
}
/*Function: ininrm, ID: 14*/
void ininrm()
{
    /*ininrm:14*/
    /*CompoundStmt:2115*/
    int i;
    long long bf4 = pow((long long)nbfn, 4);
    printf(" Example Direct Self Consistent Field Program \n");
    printf(" -------------------------------------------- \n\n");
    printf(" no. of atoms .............. %5d\n", natom);
    printf(" no. of occupied orbitals .. %5lld\n", nocc);
    printf(" no. of basis functions .... %5lld\n", nbfn);
    printf(" basis functions^4 ......... %5lld\n", bf4);
    printf(" convergence threshold ..... %9.4f\n", 0.0060000000000000001);
    for (i = 0; i < nbfn; i++)
        iky[i] = (i + 1) * i / 2;
    for (i = 0; i < nbfn; i++)
        rnorm[i] = pow((expnt[i] * 2. / 3.1415926535897931), 0.75);
    setfm();
}
/*Function: setfm, ID: 15*/
void setfm()
{
    /*setfm:15*/
    /*CompoundStmt:2151*/
    int i, ii;
    double t[2001];
    double et[2001], rr, tt;
    delta = 0.014;
    delo2 = delta * 0.5;
    rdelta = 1. / delta;
    for (i = 0; i < 2001; i++) {
        tt = delta * (double)i;
        et[i] = exprjh(-tt);
        t[i] = tt * 2.;
        fm[i][4] = 0.;
    }
    for (i = 199; i > 3; i--) {
        rr = 1. / (double)(2 * i + 1);
        for (ii = 0; ii < 2001; ii++)
            fm[ii][4] = (et[ii] + t[ii] * fm[ii][4]) * rr;
    }
    for (i = 3; i >= 0; i--) {
        rr = 1. / (double)(2 * i + 1);
        for (ii = 0; ii < 2001; ii++)
            fm[ii][i] = (et[ii] + t[ii] * fm[ii][i + 1]) * rr;
    }
    return;
}
/*Function: h, ID: 16*/
double h(int i, int j)
{
    /*h:16*/
    /*CompoundStmt:2235*/
    double f0val = 0., sum = 0.;
    double facij, expij, repij, xp, yp, zp, rpc2, rab2;
    int iat;
    rab2 = (x[i] - x[j]) * (x[i] - x[j]) + (y[i] - y[j]) * (y[i] - y[j])
        + (z[i] - z[j]) * (z[i] - z[j]);
    facij = expnt[i] * expnt[j] / (expnt[i] + expnt[j]);
    expij = exprjh(-facij * rab2);
    repij = (2. * 3.1415926535897931 / (expnt[i] + expnt[j])) * expij;
    for (iat = 0; iat < natom; iat++) {
        xp = (x[i] * expnt[i] + x[j] * expnt[j]) / (expnt[i] + expnt[j]);
        yp = (y[i] * expnt[i] + y[j] * expnt[j]) / (expnt[i] + expnt[j]);
        zp = (z[i] * expnt[i] + z[j] * expnt[j]) / (expnt[i] + expnt[j]);
        rpc2 = (xp - ax[iat]) * (xp - ax[iat]) + (yp - ay[iat]) * (yp - ay[iat])
            + (zp - az[iat]) * (zp - az[iat]);
        f0val = f0((expnt[i] + expnt[j]) * rpc2);
        sum = sum - repij * q[iat] * f0val;
    }
    sum = sum
        + facij * (3. - 2. * facij * rab2) * pow((3.1415926535897931 / (expnt[i] + expnt[j])), 1.5)
            * expij;
    return sum * rnorm[i] * rnorm[j];
}
/*Function: s, ID: 17*/
double s(int i, int j)
{
    /*s:17*/
    /*CompoundStmt:2378*/
    double rab2, facij, temp;
    rab2 = (x[i] - x[j]) * (x[i] - x[j]) + (y[i] - y[j]) * (y[i] - y[j])
        + (z[i] - z[j]) * (z[i] - z[j]);
    facij = expnt[i] * expnt[j] / (expnt[i] + expnt[j]);
    temp = pow((3.1415926535897931 / (expnt[i] + expnt[j])), 1.5) * exprjh(-facij * rab2) * rnorm[i]
        * rnorm[j];
    return temp;
}
/*Function: makden, ID: 18*/
void makden()
{
    /*makden:18*/
    /*CompoundStmt:2429*/
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasTrans, nbfn, nbfn, nocc, 2., g_orbs, nbfn, g_orbs,
        nbfn, 0., g_work, nbfn);
}
/*Function: oneel, ID: 19*/
double oneel(double schwmax)
{
    /*oneel:19*/
    /*CompoundStmt:2433*/
    int i, j, i_off = 0;
    double tol2e_over_schwmax = 9.9999999999999995E-7 / schwmax;
    for (i = 0; i < nbfn; i++) {
        for (j = 0; j < nbfn; j++) {
            g_fock[i_off + j] = (g_schwarz[i_off + j] > tol2e_over_schwmax) ? h(i, j) : 0.;
        }
        i_off += nbfn;
    }
    return (0.5 * contract_matrices(g_fock, g_dens));
}
/*Function: damp, ID: 20*/
void damp(double fac)
{
    /*damp:20*/
    /*CompoundStmt:2462*/
    int i;
    double ofac = 1. - fac;
    double* dens = g_dens;
    double* work = g_work;
    for (i = 0; i < nbfn * nbfn; i++) {
        *dens = (fac * *dens) + (ofac * *(work++));
        dens++;
    }
}
/*Function: dendif, ID: 21*/
double dendif()
{
    /*dendif:21*/
    /*CompoundStmt:2484*/
    double denmax = 0.;
    int i;
    double* dens = g_dens;
    double* work = g_work;
    for (i = 0; i < nbfn * nbfn; i++) {
        double xdiff = fabs(*(dens++) - *(work++));
        if (xdiff > denmax)
            denmax = xdiff;
    }
    return denmax;
}
/*Function: diagon, ID: 22*/
double diagon(int iter)
{
    /*diagon:22*/
    /*CompoundStmt:2508*/
    int i, j, i_off = 0;
    double shift, tester = 0.;
    cblas_dsymm(CblasRowMajor, CblasLeft, CblasUpper, nbfn, nbfn, 1., g_fock, nbfn, g_orbs, nbfn,
        0., g_tfock, nbfn);
    cblas_dgemm(CblasRowMajor, CblasTrans, CblasNoTrans, nbfn, nbfn, nbfn, 1., g_orbs, nbfn,
        g_tfock, nbfn, 0., g_fock, nbfn);
    for (i = 0; i < nbfn; i++) {
        for (j = 0; j < nbfn; j++) {
            if (i == j)
                continue;
            double xtmp = fabs(g_fock[i_off + j]);
            if (xtmp > tester)
                tester = xtmp;
        }
        i_off += nbfn;
    }
    shift = 0.;
    if (tester > 0.29999999999999999)
        shift = 0.29999999999999999;
    else if (nbfn > 60)
        shift = 0.10000000000000001;
    else
        shift = 0.;
    if (iter >= 1 && shift != 0.)
        for (i = nocc; i < nbfn; i++)
            g_fock[(((i)*nbfn) + (i))] += shift;
    memcpy(g_tfock, g_orbs, nbfn * nbfn * sizeof(double));
    double t0 = timer2();
    Eigen_std(g_fock, g_work, eigv);
    tdgemm += (timer2() - t0);
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, nbfn, nbfn, nbfn, 1., g_tfock, nbfn,
        g_work, nbfn, 0., g_orbs, nbfn);
    if (iter >= 1 && shift != 0.)
        for (i = nocc; i < nbfn; i++)
            eigv[i] = eigv[i] - shift;
    return tester;
}
/*Function: makeob, ID: 23*/
void makeob()
{
    /*makeob:23*/
    /*CompoundStmt:2593*/
    int i, j;
    double eval[5760];
    for (i = 0; i < nbfn; i++) {
        for (j = 0; j < nbfn; j++) {
            g_ident[(((i)*nbfn) + (j))] = s(i, j);
            g_fock[(((i)*nbfn) + (j))] = 0.5;
        }
    }
    Eigen_gen(g_fock, g_ident, g_orbs, eval);
    return;
}
/*Function: denges, ID: 24*/
void denges()
{
    /*denges:24*/
    /*CompoundStmt:2619*/
    int i, j, k, l;
    double atdens[15][15] = {
        { 1.9999999999999999E-6, 2.6999999999999999E-5, 1.2899999999999999E-4, 4.28E-4,
            9.4999999999999999E-4, 0.0011800000000000001, 4.57E-4, -2.7E-4, -2.7099999999999997E-4,
            3.9999999999999998E-6, 3.9999999999999998E-6, 3.9999999999999998E-6,
            3.9999999999999998E-6, 3.9999999999999998E-6, 3.9999999999999998E-6 },
        { 2.6999999999999999E-5, 1.02E-4, 9.8700000000000003E-4, 0.0032690000000000002, 0.007254,
            0.0090069999999999993, 0.0034919999999999999, -0.0020990000000000002,
            -0.0021080000000000001, 3.4999999999999997E-5, 3.4999999999999997E-5,
            3.4999999999999997E-5, 3.4999999999999997E-5, 3.4999999999999997E-5,
            3.4999999999999997E-5 },
        { 1.2899999999999999E-4, 9.8700000000000003E-4, 0.0023809999999999999, 0.015765999999999999,
            0.034987999999999998, 0.043432999999999999, 0.016834999999999999, -0.010038,
            -0.010082000000000001, 1.66E-4, 1.66E-4, 1.66E-4, 1.66E-4, 1.66E-4, 1.66E-4 },
        { 4.28E-4, 0.0032690000000000002, 0.015765999999999999, 0.026100000000000002, 0.115858,
            0.144064, 0.055967000000000003, -0.035878, -0.035990000000000001, 5.8399999999999999E-4,
            5.8399999999999999E-4, 5.8399999999999999E-4, 5.8399999999999999E-4,
            5.8399999999999999E-4, 5.8399999999999999E-4 },
        { 9.4999999999999999E-4, 0.007254, 0.034987999999999998, 0.115858, 0.12858600000000001,
            0.32012000000000002, 0.124539, -0.083334000000000005, -0.083535999999999999, 0.001346,
            0.001346, 0.001346, 0.001346, 0.001346, 0.001346 },
        { 0.0011800000000000001, 0.0090069999999999993, 0.043432999999999999, 0.144064,
            0.32012000000000002, 0.20195199999999999, 0.15993499999999999, -0.16276199999999999,
            -0.16226699999999999, 0.0024710000000000001, 0.0024710000000000001,
            0.0024710000000000001, 0.0024710000000000001, 0.0024710000000000001,
            0.0024710000000000001 },
        { 4.57E-4, 0.0034919999999999999, 0.016834999999999999, 0.055967000000000003, 0.124539,
            0.15993499999999999, 0.032377999999999997, -0.093780000000000002, -0.093201999999999993,
            0.001372, 0.001372, 0.001372, 0.001372, 0.001372, 0.001372 },
        { -2.7E-4, -0.0020990000000000002, -0.010038, -0.035878, -0.083334000000000005,
            -0.16276199999999999, -0.093780000000000002, 0.33448800000000001, 0.66091800000000001,
            -0.0090900000000000008, -0.0090900000000000008, -0.0090900000000000008,
            -0.0090900000000000008, -0.0090900000000000008, -0.0090900000000000008 },
        { -2.7099999999999997E-4, -0.0021080000000000001, -0.010082000000000001,
            -0.035990000000000001, -0.083535999999999999, -0.16226699999999999,
            -0.093201999999999993, 0.66091800000000001, 0.32648199999999999, -0.0089820000000000004,
            -0.0089820000000000004, -0.0089809999999999994, -0.0089809999999999994,
            -0.0089809999999999994, -0.0089820000000000004 },
        { 3.9999999999999998E-6, 3.4999999999999997E-5, 1.66E-4, 5.8399999999999999E-4, 0.001346,
            0.0024710000000000001, 0.001372, -0.0090900000000000008, -0.0089820000000000004,
            6.2000000000000003E-5, 1.2400000000000001E-4, 1.2400000000000001E-4,
            1.2400000000000001E-4, 1.2400000000000001E-4, 1.2400000000000001E-4 },
        { 3.9999999999999998E-6, 3.4999999999999997E-5, 1.66E-4, 5.8399999999999999E-4, 0.001346,
            0.0024710000000000001, 0.001372, -0.0090900000000000008, -0.0089820000000000004,
            1.2400000000000001E-4, 6.2000000000000003E-5, 1.2400000000000001E-4,
            1.2400000000000001E-4, 1.2400000000000001E-4, 1.2400000000000001E-4 },
        { 3.9999999999999998E-6, 3.4999999999999997E-5, 1.66E-4, 5.8399999999999999E-4, 0.001346,
            0.0024710000000000001, 0.001372, -0.0090900000000000008, -0.0089809999999999994,
            1.2400000000000001E-4, 1.2400000000000001E-4, 6.2000000000000003E-5,
            1.2400000000000001E-4, 1.2400000000000001E-4, 1.2400000000000001E-4 },
        { 3.9999999999999998E-6, 3.4999999999999997E-5, 1.66E-4, 5.8399999999999999E-4, 0.001346,
            0.0024710000000000001, 0.001372, -0.0090900000000000008, -0.0089809999999999994,
            1.2400000000000001E-4, 1.2400000000000001E-4, 1.2400000000000001E-4,
            6.2000000000000003E-5, 1.2400000000000001E-4, 1.2400000000000001E-4 },
        { 3.9999999999999998E-6, 3.4999999999999997E-5, 1.66E-4, 5.8399999999999999E-4, 0.001346,
            0.0024710000000000001, 0.001372, -0.0090900000000000008, -0.0089809999999999994,
            1.2400000000000001E-4, 1.2400000000000001E-4, 1.2400000000000001E-4,
            1.2400000000000001E-4, 6.2000000000000003E-5, 1.2400000000000001E-4 },
        { 3.9999999999999998E-6, 3.4999999999999997E-5, 1.66E-4, 5.8399999999999999E-4, 0.001346,
            0.0024710000000000001, 0.001372, -0.0090900000000000008, -0.0089820000000000004,
            1.2400000000000001E-4, 1.2400000000000001E-4, 1.2400000000000001E-4,
            1.2400000000000001E-4, 1.2400000000000001E-4, 6.2000000000000003E-5 }
    };
    for (i = 0; i < 15; i++)
        atdens[i][i] = 2. * atdens[i][i];
    for (i = 0; i < nbfn; i++)
        for (j = 0; j < nbfn; j++)
            g_dens[(((i)*nbfn) + (j))] = 0.;
    for (i = 0; i < nbfn; i += 15) {
        for (k = 0; k < 15; k++) {
            for (l = 0; l < 15; l++) {
                g_dens[(((i + k) * nbfn) + (i + l))] = atdens[k][l];
            }
        }
    }
    return;
}
/*Function: setarrays, ID: 25*/
void setarrays()
{
    /*setarrays:25*/
    /*CompoundStmt:3809*/
    int i, j, j_off = 0;
    g_dens = (double*)malloc(nbfn * nbfn * sizeof(double));
    g_schwarz = (double*)malloc(nbfn * nbfn * sizeof(double));
    g_fock = (double*)malloc(nbfn * nbfn * sizeof(double));
    g_tfock = (double*)malloc(nbfn * nbfn * sizeof(double));
    g_work = (double*)malloc(nbfn * nbfn * sizeof(double));
    g_ident = (double*)malloc(nbfn * nbfn * sizeof(double));
    g_orbs = (double*)malloc(nbfn * nbfn * sizeof(double));
    g_schwarz_max_j = (double*)malloc(nbfn * sizeof(double));
    g_precalc = (precalc*)malloc(nbfn * nbfn * sizeof(precalc));
    g_fock_const = (double*)malloc(nbfn * nbfn * sizeof(double));
    g_partial_fock = (double*)calloc(nbfn * nbfn * sizeof(double), 1);
    g_thread_fock = (double*)calloc(omp_get_max_threads() * nbfn * nbfn * sizeof(g_thread_fock), 1);
    for (j = 0; j < nbfn; j++) {
        g_schwarz_max_j[j] = 0.;
        for (i = 0; i < nbfn; i++) {
            g_dens[j_off + i] = 0.;
            g_schwarz[j_off + i] = 0.;
            g_fock[j_off + i] = 0.;
            g_tfock[j_off + i] = 0.;
            g_work[j_off + i] = 0.;
            g_ident[j_off + i] = 0.;
            g_orbs[j_off + i] = 0.;
        }
        j_off += nbfn;
    }
    return;
}
/*Function: closearrays, ID: 26*/
void closearrays()
{
    /*closearrays:26*/
    /*CompoundStmt:3927*/
    free(g_dens);
    free(g_fock);
    free(g_schwarz);
    free(g_tfock);
    free(g_work);
    free(g_ident);
    free(g_orbs);
    free(g_schwarz_max_j);
    free(g_fock_const);
    free(g_partial_fock);
    free(g_thread_fock);
    if (g_cache)
        free(g_cache);
    return;
}
/*Function: contract_matrices, ID: 27*/
double contract_matrices(double* a, double* b)
{
    /*contract_matrices:27*/
    /*CompoundStmt:3944*/
    return cblas_ddot(nbfn * nbfn, a, 1, b, 1);
}
