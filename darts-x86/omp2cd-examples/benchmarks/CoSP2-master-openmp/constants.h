/// \File
/// Contains frequently used constants

#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

#include "mytype.h"

#define ABS fabs

#define MAX(A,B) ((A) > (B) ? (A) : (B))
#define MIN(A,B) ((A) < (B) ? (A) : (B))

/// Numerical values used
static const real_t ZERO = 0.0;
static const real_t TWO = 2.0;
static const real_t THREE = 3.0;
static const real_t HALF = 0.5;

static const int HUNDRED = 100;

#ifdef MAIN_FILE
// int msparse;
// int hDim;
// int M;
// int debug;

// real_t eps; 
// real_t hEps; 
// real_t idemTol; 
#else
extern int msparse;
extern int hDim;
extern int M;
extern int debug;

extern real_t eps;          
extern real_t hEps;           
extern real_t idemTol;
#endif

#endif
