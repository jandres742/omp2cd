#ifndef _MATMUL_H
#define _MATMUL_H

#if !defined(__DARTS_)
#include <omp.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

typedef double MYTYPE;
int functB();
void functionA(unsigned long long int timeTotal);
int main(int argc, char** argv);
#endif