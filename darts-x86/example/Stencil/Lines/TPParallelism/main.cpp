#include <iostream>
#include <cstdlib>
#include <cassert>
#include <cstring>
#include <cstdio>
#include <stdint.h>
#include <sched.h>
#include "utils.h"

typedef void (*Launcher) (FILE *fd, unsigned nbRepets, double *oldA, double *newA, unsigned matSize, unsigned stride);
typedef void (*Function) (double *oldA, double *newA, size_t matSize);

int main (int argc, char *argv[]) {
	double *oldA, *newA;
	Launcher launcher[3] = {launchSequential, launchOpenMP, launchDarts};
  //Function function[3] = {sequentialVersion, openMPVersion, dartsVersion};

	
	if (argc != 5) {
		std::cerr << "Use: " << argv[0] << " {Sequential (0), OpenMP (1), Darts (2)}, {number_of_repetitions}, {matrix_size}, {stride}" << std::endl;
		exit (EXIT_FAILURE);
	}
	
	unsigned launchType = strtol (argv[1], 0, 10);
	unsigned nbRepets = strtol (argv[2], 0, 10);
	assert (nbRepets > 0);
	unsigned matSize = strtol (argv[3], 0, 10);
	assert (matSize != 0);
	unsigned stride = strtol (argv[4], 0, 10);
	unsigned allocSize = matSize*matSize*2;

	/* ALLOCATION */
	oldA = new double [allocSize];
	assert (oldA != 0);
	newA = oldA + matSize * matSize;
	
	/* INITIALIZATION */
	srand48 (4884);
	memset (oldA, 0, sizeof (*oldA)*allocSize);
	feedData (oldA, matSize);
	
	/* If we launch the program in sequential, we want to pin the process to one
	 * of the machine's core to avoid possible context switching
	 */
	if (launchType == 0) {
		cpu_set_t set;
		CPU_ZERO (&set);
		CPU_SET (0, &set);
		if (sched_setaffinity (0, sizeof (cpu_set_t), &set) == -1) {
			perror ("Sched_setAffinity error");
			exit (EXIT_FAILURE);
		}
	}

	launcher[launchType] (stdout, nbRepets, oldA, newA, matSize, stride);
  //function[launchType] (oldA, newA, matSize);
  //printResult (oldA, matSize);

	delete [] oldA, oldA = 0;
	
	return EXIT_SUCCESS;
}
