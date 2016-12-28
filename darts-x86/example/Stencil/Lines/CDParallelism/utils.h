#pragma once
#ifndef H_UTILSSTCD
#define H_UTILSSTCD

#include <cstdio>
#include <stdint.h>

#define MAX_MATSIZE 5000

void feedData (double *oldA, size_t matSize);
void printResult (double *oldA, size_t matSize);

/* Execute once the different versions of the kernel */
void sequentialVersion (double *oldA, double *newA,
												size_t matSize);
void openMPVersion (double *oldA, double *newA,
										size_t matSize);
void dartsVersion (double *oldA,
									double *newA, size_t size);

/* Executes repets time the execution and store rdtsc results in fd */
void launchSequential (FILE *fd, unsigned repets,
													double *oldA, double *newA, unsigned matSize, unsigned stride);
void launchOpenMP (FILE *fd, unsigned repets,
									double *oldA, double *newA, unsigned matSize, unsigned stride);
void launchDarts (FILE *fd, unsigned repets,
												double *oldA, double *newA, unsigned matSize, unsigned stride);

#endif
