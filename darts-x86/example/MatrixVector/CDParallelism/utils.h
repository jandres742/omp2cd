/* 
 * File:   utils.h
 * Author: jphalimi
 *
 * Created on May 16, 2012, 2:14 PM
 */

#pragma once
#ifndef UTILSMVCD_H
#define	UTILSMVCD_H

#include <cstdio>

void feedData (double *mat, double *vec, size_t matSize);
void printResult (double *res, size_t matSize);

void sequentialVersion (double *mat, double *vec, double *res, size_t size);
void openMPVersion (double *mat, double *vec, double *res, size_t size);
void dartsVersion (double *mat, double *vec, double *res, size_t size);

void launchSequential (FILE *fd, unsigned repets,
													double *mat, double *vec, double *res, unsigned matSize, unsigned stride);
void launchOpenMP (FILE *fd, unsigned repets,
									double *mat, double *vec, double *res, unsigned matSize, unsigned stride);
void launchDarts (FILE *fd, unsigned repets,
												double *mat, double *vec, double *res, unsigned matSize, unsigned stride);

#endif	/* UTILS_H */
