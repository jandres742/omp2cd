/*****************************************************************/
/******     C  _  P  R  I  N  T  _  R  E  S  U  L  T  S     ******/
/*****************************************************************/

#ifndef C_PRINTRESULTS_H
#define C_PRINTRESULTS_H

#include <stdio.h>
#include <stdlib.h>
#ifdef _OPENMP
#include <omp.h>
#endif

void c_print_results(char* name, char class_is, int n1, int n2, int n3, int niter, double t, double mops, char* optype, int passed_verification, char* npbversion, char* compiletime, char* cc, char* clink, char* c_lib, char* c_inc,
    char* cflags, char* clinkflags);

#endif