#include <iostream>
#include <cstdlib>
#include <cassert>
#include <cstring>
#include <cstdio>
#include <stdint.h>
#include <sched.h>
#include "utils.h"

typedef void (*Launcher) (FILE *fd, unsigned nbRepets, double *vec, double *mat, double *res, unsigned matSize, unsigned stride);
typedef void (*Function) (double *vec, double *mat, double *res, size_t matSize);

int main (int argc, char *argv[]) {
	double *vec, *mat, *res;
	Launcher launcher[3] = {launchSequential, launchOpenMP, launchDarts};
  //Function function[3] = {sequentialVersion, openMPVersion, dartsVersion};

	if (argc != 5) {
		std::cerr << "Use: " << argv[0] << " {Sequential (0), OpenMP (1), Darts (2)}, {number_of_repetitions}, {matrix_size}, {stride}" << std::endl;
		exit (EXIT_FAILURE);
	}
	
	unsigned launchType = strtol (argv[1], 0, 10);
	unsigned nbRepets = strtol (argv[2], 0, 10);
	unsigned matSize = strtol (argv[3], 0, 10);
	assert (matSize != 0);
	unsigned stride = strtol (argv[4], 0, 10);
	unsigned allocSize = matSize*2 + matSize*matSize; // 2 vectors + 1 matrix

	/* ALLOCATION */
	vec = new double [allocSize];
	assert (vec != 0);
	res = vec + matSize;
	mat = res + matSize;
	
	/* INITIALIZATION */
	srand48 (4884);
	memset (vec, 0, sizeof (*vec)*allocSize);
	feedData (vec, mat, matSize);
	
	/* If we launch the program in sequential, we want to pin the process to one
	 * of the machine's core to avoid possible context switching
	 */
	if (launchType == 0) {	
		cpu_set_t set;
		CPU_ZERO (&set);
		CPU_SET (0, &set);
		if (sched_setaffinity (0, sizeof (cpu_set_t), &set) == -1) {
			perror ("Sched_setAffinity error");
		}
	}

	launcher[launchType] (stdout, nbRepets, vec, mat, res, matSize, stride);
  //printResult (vec, matSize);
  //function[launchType] (vec, mat, res, matSize);
  //printResult (res, matSize);

	delete [] vec;
	
	(void) argc;
	(void) argv;
	
	return EXIT_SUCCESS;
}
