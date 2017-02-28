/* CLOMP Benchmark to measure OpenMP overheads with typical usage situations.
 * Also measures cache effects and memory bandwidth affects on performance.
 * Provides memory layout options that may affect performance.
 * Removes earlier variations that didn't provide expected insight.
 * Version 1.2 Drastically reduces serial overhead for high part count
 *             inputs, allowing reasonable messages with large thread counts.
 *             Also added -DWITH_MPI to measure OpenMP overheads when run
 *             with multiple MPI tasks per node, defaults to no MPI.
 *             Reduced digits reported after decimal point for most measurments.
 *             Last output line prints CORAL RFP data for easy importing.
 *             Changes by John Gyllenhaal at LLNL 12/23/13
 * Version 1.1 calculates error bounds (but not yet used) and dramatically
 *             reduces chance of underflows in math.  John Gyllenhaal 6/16/10
 * Version 1 (first public release) of the CLOMP benchmark.
 * Written by John Gyllenhaal and Greg Bronevetsky at LLNL 5/25/07 
 * Based on version 0.2 (4/24/07) and version 0.1 (12/14/07) by John and Greg.
 ******************************************************************************
COPYRIGHT AND LICENSE

Copyright (c) 2007, The Regents of the University of California.
Produced at the Lawrence Livermore National Laboratory
Written by John Gyllenhaal (gyllen@llnl.gov) 
and Greg Bronevetsky (bronevetsky1@llnl.gov)
UCRL-CODE-233406.
All rights reserved.

This file is part of CLOMP. 
For details, see www.llnl.gov/asc/sequoia/benchmarks

Redistribution and use in source and binary forms, with or
without modification, are permitted provided that the following
conditions are met:

* Redistributions of source code must retain the above copyright
  notice, this list of conditions and the disclaimer below.

* Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the disclaimer (as noted below) in
  the documentation and/or other materials provided with the distribution.

* Neither the name of the UC/LLNL nor the names of its contributors may
  be used to endorse or promote products derived from this software without
  specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS OF THE UNIVERSITY
OF CALIFORNIA, THE U.S. DEPARTMENT OF ENERGY OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

ADDITIONAL BSD NOTICE

1. This notice is required to be provided under our contract with the
   U.S. Department of Energy (DOE). This work was produced at the
   University of California, Lawrence Livermore National Laboratory
   under Contract No. W-7405-ENG-48 with the DOE.

2. Neither the United States Government nor the University of California
   nor any of their employees, makes any warranty, express or implied,
   or assumes any liability or responsibility for the accuracy, completeness,
   or usefulness of any information, apparatus, product, or process disclosed,
   or represents that its use would not infringe privately-owned rights.

3. Also, reference herein to any specific commercial products, process,
   or services by trade name, trademark, manufacturer or otherwise does not
   necessarily constitute or imply its endorsement, recommendation, or
   favoring by the United States Government or the University of California.
   The views and opinions of authors expressed herein do not necessarily
   state or reflect those of the United States Government or the University
   of California, and shall not be used for advertising or product
   endorsement purposes.
******************************************************************************/
#ifndef CLOMP_H
#define CLOMP_H

#include <stdio.h>
#include <stdlib.h>
#ifndef __DARTS_
#include <omp.h>
#endif
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>

/* Simple Zone data structure */
typedef struct _Zone
{
    long zoneId;
    long partId;
    double value;
    struct _Zone *nextZone;
} Zone;


/* Part data structure */
typedef struct _Part
{
    long partId;
    long zoneCount;
    long update_count;  
    Zone *firstZone;
    Zone *lastZone;
    double deposit_ratio;
    double residue;
    double expected_first_value; /* Used to check results */
    double expected_residue;     /* Used to check results */
} Part;

void print_usage();
long convert_to_positive_long (const char *parm_name, const char *parm_val);
void update_part (Part *part, double incoming_deposit);
void reinitialize_parts();
void print_start_message (const char *desc);
void get_timestamp (struct timeval *ts);
void print_pseudocode (const char *desc, const char *pseudocode);
double print_timestats (const char *desc, struct timeval *start_ts, 
			struct timeval *end_ts, double base_seconds,
			double bestcase_seconds);
void print_data_stats (const char *desc);
double calc_deposit ();
void do_calc_deposit_only();
void do_omp_barrier_only(long num_iterations);
void serial_ref_module1();
void serial_ref_module2();
void serial_ref_module3();
void serial_ref_module4();
void serial_ref_cycle();
void do_serial_ref_version();
void static_omp_module1();
void static_omp_module2();
void static_omp_module3();
void static_omp_module4();
void static_omp_cycle();
void do_static_omp_version();
void dynamic_omp_module1();
void dynamic_omp_module2();
void dynamic_omp_module3();
void dynamic_omp_module4();
void dynamic_omp_cycle();
void do_dynamic_omp_version();
void manual_omp_module1(int startPidx, int endPidx);
void manual_omp_module2(int startPidx, int endPidx);
void manual_omp_module3(int startPidx, int endPidx);
void manual_omp_module4(int startPidx, int endPidx);
void manual_omp_cycle(int startPidx, int endPidx);
void do_manual_omp_version(long num_iterations);
void bestcase_omp_module1(int startPidx, int endPidx, double deposit);
void bestcase_omp_module2(int startPidx, int endPidx, double deposit);
void bestcase_omp_module3(int startPidx, int endPidx, double deposit);
void bestcase_omp_module4(int startPidx, int endPidx, double deposit);
void bestcase_omp_cycle(int startPidx, int endPidx, double deposit);
void do_bestcase_omp_version(long num_iterations);
void addPart (Part *part, long partId);
void addZone (Part *part, Zone *zone);
int main (int argc, char *argv[]);
#endif