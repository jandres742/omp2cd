#ifndef _SCF_H
#define _SCF_H

/* C version of the SCF code */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// #ifdef USE_MKL
// #include <mkl_cblas.h>
// #elif defined BLAS
#ifdef __cplusplus
extern "C" {
#endif
#include <cblas.h>
#ifdef __cplusplus
}
#endif
// #endif

#include "cscc.h"
#include "integ.h"
#include "input.h"
#include "output.h"
#include "timer.h"
#include "g.h"
#include "twoel.h"

//function declaration
void mkpre(void);
double makesz(void);
void ininrm(void);
void makden(void);
double h(int, int);
double s(int, int);
double oneel(double);
double twoel(double);
void Dgemm(char, char, int, int, int, double *, double *, double *);
int Eigen_gen(double *, double *, double *, double *);
int Eigen_std(double *, double *, double *);
void damp(double fac);
double dendif(void);
double diagon(int);
void makeob(void);
void denges(void);
void setarrays(void);
void closearrays(void);
double contract_matrices(double *, double *);
void setfm(void);
double twoel_fast(double);
int main(int argc, char **argv);
#endif