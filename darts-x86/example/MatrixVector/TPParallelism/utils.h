#pragma once
#ifndef H_UTILSMVTP
#define H_UTILSMVTP

#include <cstdio>
#include <stdint.h>

#define MAX_MATSIZE 5000

void feedData (double *vec, double *mat, size_t matSize);
void printResult (double *res, size_t matSize);

/* Execute once the different versions of the kernel */
void sequentialVersion (double *vec, double *mat, double *res, size_t matSize);
void openMPVersion (double *vec, double *mat, double *res, size_t matSize);
void dartsVersion (double *vec, double *mat, double *res, size_t size);

/* Executes repets time the execution and store rdtsc results in fd */
void launchSequential (FILE *fd, unsigned repets, double *vec, double *mat, double *res, unsigned matSize, unsigned stride);
void launchOpenMP (FILE *fd, unsigned repets, double *vec, double *mat, double *res, unsigned matSize, unsigned stride);
void launchDarts (FILE *fd, unsigned repets, double *vec, double *mat, double *res, unsigned matSize, unsigned stride);

#endif
