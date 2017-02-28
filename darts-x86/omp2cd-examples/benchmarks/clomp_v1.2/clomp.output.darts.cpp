#include "clomp.output.darts.h"
using namespace darts;
using namespace std;
long CLOMP_allocThreads = -2;
double CLOMP_error_bound = 0.;
char* CLOMP_exe_name = (char*)((void*)0);
long CLOMP_flopScale = -1;
long CLOMP_inputAllocThreads = -2;
double CLOMP_max_residue = 0.;
long CLOMP_numParts = -1;
long CLOMP_numThreads = -2;
long CLOMP_num_iterations = 0.;
double CLOMP_partRatio = 0.;
double CLOMP_residue_ratio_part0 = 0.;
double CLOMP_tightest_error_bound = 0.;
long CLOMP_timeScale = -1;
long CLOMP_zoneSize = -1;
long CLOMP_zonesPerPart = -1;
Part** partArray = (Part**)((void*)0);
double deposit_darts31;
double deposit_darts32;
double deposit_darts33;
double deposit_darts34;
/*Function: print_usage, ID: 1*/
void print_usage()
{
    /*print_usage:1*/
    /*CompoundStmt:109*/
    fprintf(stderr, "Usage: clomp numThreads allocThreads numParts \\\n           zonesPerPart "
                    "zoneSize flopScale timeScale\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "New in Version 1.2: Compile with -DWITH_MPI to generate clomp_mpi\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "  numThreads: Number of OpenMP threads to use (-1 for system default)\n");
    fprintf(stderr, "  allocThreads: #threads when allocating data (-1 for numThreads)\n");
    fprintf(stderr, "  numParts: Number of independent pieces of work (loop iterations)\n");
    fprintf(stderr, "  zonesPerPart: Number of zones in the first part (3 flops/zone/part)\n");
    fprintf(stderr, "  zoneSize: Bytes in zone, only first ~32 used (512 nominal, >= 32 valid)\n");
    fprintf(
        stderr, "  flopScale: Scales flops/zone to increase memory reuse (1 nominal, >=1 Valid)\n");
    fprintf(stderr, "  timeScale: Scales target time per test (10-100 nominal, 1-10000 Valid)\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "Some interesting testcases (last number controls run time):\n");
    fprintf(stderr, "           Target input:    clomp 16 1 16 400 32 1 100\n");
    fprintf(stderr, "   Target/NUMA friendly:    clomp 16 -1 16 400 32 1 100\n");
    fprintf(stderr, "    Weak Scaling Target:    clomp N -1 N 400 32 1 100\n");
    fprintf(stderr, "      Weak Scaling Huge:    clomp N -1 N 6400 32 1 100\n");
    fprintf(stderr, "  Strong Scaling Target:    clomp -1 -1 1024 10 32 1 100\n");
    fprintf(stderr, "        Mem-bound input:    clomp N 1 N 640000 32 1 100\n");
    fprintf(stderr, "Mem-bound/NUMA friendly:    clomp N -1 N 640000 32 1 100\n");
    fprintf(stderr, "  MPI/OMP Hybrid Target:    (mpirun -np M) clomp_mpi 16 1 16 400 32 1 100\n");
}
/*Function: convert_to_positive_long, ID: 2*/
long convert_to_positive_long(const char* parm_name, const char* parm_val)
{
    /*convert_to_positive_long:2*/
    /*CompoundStmt:131*/
    long val;
    char* endPtr;
    if ((parm_name == (char*)((void*)0)) || (parm_val == (char*)((void*)0))) {
        fprintf(stderr, "Error in convert_to_positive_long: Passed NULL pointers!\n");
        exit(1);
    }
    val = strtol(parm_val, &endPtr, 0);
    if (endPtr[0] != 0) {
        fprintf(stderr, "Error converting '%s' parameter value '%s' to long at '%s'!\n", parm_name,
            parm_val, endPtr);
        exit(1);
    }
    if ((strcmp(parm_name, "numThreads") == 0) || (strcmp(parm_name, "allocThreads") == 0)) {
        if ((val < 1) && (val != -1)) {
            fprintf(
                stderr, "Invalid value %ld for parameter %s, must be > 0 or -1!\n", val, parm_name);
            print_usage();
            exit(1);
        }
    } else if (val < 1) {
        fprintf(stderr, "Invalid value %ld for parameter %s, must be > 0\n", val, parm_name);
        print_usage();
        exit(1);
    }
    return (val);
}
/*Function: update_part, ID: 3*/
void update_part(Part* part, double incoming_deposit)
{
    /*update_part:3*/
    /*CompoundStmt:164*/
    Zone* zone;
    double deposit_ratio, remaining_deposit, deposit;
    long scale_count;
    part->update_count++;
    deposit_ratio = part->deposit_ratio;
    remaining_deposit = incoming_deposit;
    if (CLOMP_flopScale == 1) {
        for (zone = part->firstZone; zone != (Zone*)((void*)0); zone = zone->nextZone) {
            deposit = remaining_deposit * deposit_ratio;
            zone->value += deposit;
            remaining_deposit -= deposit;
        }
    } else {
        for (zone = part->firstZone; zone != (Zone*)((void*)0); zone = zone->nextZone) {
            for (scale_count = 0; scale_count < CLOMP_flopScale; scale_count++) {
                deposit = remaining_deposit * deposit_ratio;
                zone->value += deposit;
                remaining_deposit -= deposit;
            }
        }
    }
    part->residue = remaining_deposit;
}
/*Function: reinitialize_parts, ID: 4*/
void reinitialize_parts()
{
    /*reinitialize_parts:4*/
    /*CompoundStmt:212*/
    long pidx;
    Zone* zone;
    for (pidx = 0; pidx < CLOMP_numParts; pidx++) {
        for (zone = partArray[pidx]->firstZone; zone != (Zone*)((void*)0); zone = zone->nextZone) {
            zone->value = 0.;
        }
        partArray[pidx]->residue = 0.;
        partArray[pidx]->update_count = 0;
    }
    for (pidx = 0; pidx < CLOMP_numParts; pidx++) {
        update_part(partArray[pidx], 0.);
    }
}
/*Function: print_start_message, ID: 5*/
void print_start_message(const char* desc)
{
    /*print_start_message:5*/
    /*CompoundStmt:248*/
    time_t starttime;
    char startdate[50];
    time(&starttime);
    ctime_r(&starttime, startdate);
    printf("%13s  Started: %s", desc, startdate);
    if (strcmp(desc, "calc_deposit") != 0) {
        if (strcmp(desc, "Serial Ref") != 0) {
            printf("%13s #Threads: %d\n", desc, omp_get_max_threads());
        } else {
            printf("%13s #Threads: N/A\n", desc);
        }
    }
}
/*Function: get_timestamp, ID: 6*/
void get_timestamp(struct timeval* ts)
{
    /*get_timestamp:6*/
    /*CompoundStmt:266*/
    if (gettimeofday(ts, (struct timezone*)((void*)0)) != 0) {
        fprintf(stderr, "Unable to get time of day, exiting\n");
        exit(1);
    }
}
/*Function: print_pseudocode, ID: 7*/
void print_pseudocode(const char* desc, const char* pseudocode)
{
    /*print_pseudocode:7*/
    /*CompoundStmt:272*/
    printf("%13s:| %s\n", desc, pseudocode);
}
/*Function: print_timestats, ID: 8*/
double print_timestats(const char* desc, struct timeval* start_ts, struct timeval* end_ts,
    double base_seconds, double bestcase_seconds)
{
    /*print_timestats:8*/
    /*CompoundStmt:274*/
    double seconds;
    char used_desc[100] = "";
    seconds = ((double)end_ts->tv_sec + ((double)end_ts->tv_usec * 9.9999999999999995E-7))
        - ((double)start_ts->tv_sec + ((double)start_ts->tv_usec * 9.9999999999999995E-7));
    printf("%13s  Runtime: %.3f (wallclock, in seconds%s)\n", desc, seconds, used_desc);
    printf("%13s  us/Loop: %.2f (wallclock, in microseconds)\n", desc,
        (seconds * 1.0E+6) / ((double)CLOMP_num_iterations * 10.));
    if ((base_seconds > 0.) && (seconds > 0.)) {
        if (base_seconds > seconds) {
            printf("%13s  Speedup: %.1f\n", desc, base_seconds / seconds);
        } else {
            printf("%13s  Speedup: %.1f (%.1fX slowdown)\n", desc, base_seconds / seconds,
                seconds / base_seconds);
        }
    }
    if ((bestcase_seconds > 0.) && (seconds > 0.)) {
        printf("%13s Efficacy: %.1f%% (of bestcase %.2f us/Loop)\n", desc,
            (bestcase_seconds / seconds) * 100.,
            (bestcase_seconds * 1.0E+6) / ((double)CLOMP_num_iterations * 10.));
        printf("%13s Overhead: %.2f (versus bestcase, in us/Loop)\n", desc,
            ((seconds - bestcase_seconds) * 1.0E+6) / ((double)CLOMP_num_iterations * 10.));
    }
    printf("---------------------\n");
    return (seconds);
}
/*Function: print_data_stats, ID: 9*/
void print_data_stats(const char* desc)
{
    /*print_data_stats:9*/
    /*CompoundStmt:336*/
    double value_sum, residue_sum, last_value, dtotal;
    long pidx;
    Zone* zone;
    int is_reference, error_count;
    value_sum = 0.;
    residue_sum = 0.;
    if (strcmp(desc, "Serial Ref") == 0)
        is_reference = 1;
    else
        is_reference = 0;
    error_count = 0;
    for (pidx = 0; pidx < CLOMP_numParts; pidx++) {
        if (is_reference) {
            partArray[pidx]->expected_first_value = partArray[pidx]->firstZone->value;
            partArray[pidx]->expected_residue = partArray[pidx]->residue;
        } else {
            if (partArray[pidx]->expected_first_value != partArray[pidx]->firstZone->value) {
                error_count++;
                fprintf(stderr,
                    "%s check failure: part %i first zone value (%g) != reference value (%g)!\n",
                    desc, (int)pidx, partArray[pidx]->firstZone->value,
                    partArray[pidx]->expected_first_value);
            }
            if (partArray[pidx]->expected_residue != partArray[pidx]->residue) {
                error_count++;
                fprintf(stderr,
                    "%s check failure: part %i residue (%g) != reference residue (%g)!\n", desc,
                    (int)pidx, partArray[pidx]->residue, partArray[pidx]->expected_residue);
            }
        }
        last_value = partArray[pidx]->firstZone->value;
        for (zone = partArray[pidx]->firstZone; zone != (Zone*)((void*)0); zone = zone->nextZone) {
            if (zone->value > last_value) {
                fprintf(stderr,
                    "*** %s check failure (part %i zone %i): previous (%g) < current (%g)!\n", desc,
                    (int)zone->partId, (int)zone->zoneId, last_value, zone->value);
                error_count++;
            }
            value_sum += zone->value;
            last_value = zone->value;
        }
        residue_sum += partArray[pidx]->residue;
    }
    dtotal = value_sum + residue_sum;
    if (((dtotal + 1.0000000000000001E-5) < ((double)CLOMP_num_iterations * 10.))
        || ((dtotal - 1.0000000000000001E-5) > ((double)CLOMP_num_iterations * 10.))) {
        fprintf(stderr, "*** %s check failure:  Total (%-.15g) != Expected (%.15g)\n", desc, dtotal,
            ((double)CLOMP_num_iterations * 10.));
        error_count++;
    }
    if ((residue_sum < 0.) || (residue_sum > (CLOMP_max_residue + 9.9999999999999995E-7))) {
        fprintf(stderr, "*** %s check failure: Residue (%-.15g) outside bounds 0 - %.15g\n", desc,
            residue_sum, CLOMP_max_residue);
        error_count++;
    }
    if (partArray[0]->update_count != 1) {
        fprintf(stderr, "Error in calc_deposit: Part updated %i times since last calc_deposit!\n",
            (int)partArray[0]->update_count);
        fprintf(
            stderr, "Benchmark designed to have calc_deposit called exactly once per update!\n");
        fprintf(stderr, "Critical error: Exiting...\n");
        exit(1);
    }
    if (error_count > 0) {
        fprintf(stderr, "ERROR: %i check failures detected in '%s' data. Exiting...\n", error_count,
            desc);
        exit(1);
    }
    printf("%13s Checksum: Sum=%-8.8g Residue=%-8.8g Total=%-.9g\n", desc, value_sum, residue_sum,
        dtotal);
}
/*Function: calc_deposit, ID: 10*/
double calc_deposit()
{
    /*calc_deposit:10*/
    /*CompoundStmt:455*/
    double residue, deposit;
    if (partArray[0]->update_count != 1) {
        fprintf(stderr, "Error in calc_deposit: Part updated %i times since last call!\n",
            (int)partArray[0]->update_count);
        fprintf(
            stderr, "Benchmark designed to have calc_deposit called exactly once per update!\n");
        fprintf(stderr, "Critical error: Exiting...\n");
        exit(1);
    }
    partArray[0]->update_count = 0;
    residue = partArray[0]->residue * CLOMP_residue_ratio_part0;
    deposit = (1. + residue) * CLOMP_partRatio;
    return (deposit);
}
/*Function: do_calc_deposit_only, ID: 11*/
void do_calc_deposit_only()
{
    /*do_calc_deposit_only:11*/
    /*CompoundStmt:479*/
    long iteration, subcycle;
    for (iteration = 0; iteration < CLOMP_num_iterations; iteration++) {
        for (subcycle = 0; subcycle < 10; subcycle++) {
            partArray[0]->update_count = 1;
            partArray[0]->firstZone->value = calc_deposit();
        }
    }
}
/*Function: do_omp_barrier_only, ID: 12*/
void do_omp_barrier_only(long num_iterations)
{
    /*do_omp_barrier_only:12*/
    /*CompoundStmt:499*/
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP500>(ompNumThreads * DARTS_CODELETS_MULT, 0,
            RuntimeFinalCodelet, (long*)&((num_iterations))));
    }
}
/*Function: serial_ref_module1, ID: 13*/
void serial_ref_module1()
{
    /*serial_ref_module1:13*/
    /*CompoundStmt:514*/
    double deposit;
    long pidx;
    deposit = calc_deposit();
    for (pidx = 0; pidx < CLOMP_numParts; pidx++)
        update_part(partArray[pidx], deposit);
}
/*Function: serial_ref_module2, ID: 14*/
void serial_ref_module2()
{
    /*serial_ref_module2:14*/
    /*CompoundStmt:525*/
    double deposit;
    long pidx;
    deposit = calc_deposit();
    for (pidx = 0; pidx < CLOMP_numParts; pidx++)
        update_part(partArray[pidx], deposit);
    deposit = calc_deposit();
    for (pidx = 0; pidx < CLOMP_numParts; pidx++)
        update_part(partArray[pidx], deposit);
}
/*Function: serial_ref_module3, ID: 15*/
void serial_ref_module3()
{
    /*serial_ref_module3:15*/
    /*CompoundStmt:544*/
    double deposit;
    long pidx;
    deposit = calc_deposit();
    for (pidx = 0; pidx < CLOMP_numParts; pidx++)
        update_part(partArray[pidx], deposit);
    deposit = calc_deposit();
    for (pidx = 0; pidx < CLOMP_numParts; pidx++)
        update_part(partArray[pidx], deposit);
    deposit = calc_deposit();
    for (pidx = 0; pidx < CLOMP_numParts; pidx++)
        update_part(partArray[pidx], deposit);
}
/*Function: serial_ref_module4, ID: 16*/
void serial_ref_module4()
{
    /*serial_ref_module4:16*/
    /*CompoundStmt:571*/
    double deposit;
    long pidx;
    deposit = calc_deposit();
    for (pidx = 0; pidx < CLOMP_numParts; pidx++)
        update_part(partArray[pidx], deposit);
    deposit = calc_deposit();
    for (pidx = 0; pidx < CLOMP_numParts; pidx++)
        update_part(partArray[pidx], deposit);
    deposit = calc_deposit();
    for (pidx = 0; pidx < CLOMP_numParts; pidx++)
        update_part(partArray[pidx], deposit);
    deposit = calc_deposit();
    for (pidx = 0; pidx < CLOMP_numParts; pidx++)
        update_part(partArray[pidx], deposit);
}
/*Function: serial_ref_cycle, ID: 17*/
void serial_ref_cycle()
{
    /*serial_ref_cycle:17*/
    /*CompoundStmt:606*/
    serial_ref_module1();
    serial_ref_module2();
    serial_ref_module3();
    serial_ref_module4();
}
/*Function: do_serial_ref_version, ID: 18*/
void do_serial_ref_version()
{
    /*do_serial_ref_version:18*/
    /*CompoundStmt:611*/
    long iteration;
    for (iteration = 0; iteration < CLOMP_num_iterations; iteration++)
        serial_ref_cycle();
}
/*Function: static_omp_module1, ID: 19*/
void static_omp_module1()
{
    /*static_omp_module1:19*/
    /*CompoundStmt:618*/
    double deposit;
    long pidx;
    deposit = calc_deposit();
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP623>(ompNumThreads * DARTS_CODELETS_MULT, 0,
            RuntimeFinalCodelet, 0, CLOMP_numParts, (double*)&((deposit))));
    }
}
/*Function: static_omp_module2, ID: 20*/
void static_omp_module2()
{
    /*static_omp_module2:20*/
    /*CompoundStmt:654*/
    double deposit;
    long pidx;
    deposit = calc_deposit();
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP659>(ompNumThreads * DARTS_CODELETS_MULT, 0,
            RuntimeFinalCodelet, 0, CLOMP_numParts, (double*)&((deposit))));
    }
    deposit = calc_deposit();
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP692>(ompNumThreads * DARTS_CODELETS_MULT, 0,
            RuntimeFinalCodelet, 0, CLOMP_numParts, (double*)&((deposit))));
    }
}
/*Function: static_omp_module3, ID: 21*/
void static_omp_module3()
{
    /*static_omp_module3:21*/
    /*CompoundStmt:723*/
    double deposit;
    long pidx;
    deposit = calc_deposit();
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP728>(ompNumThreads * DARTS_CODELETS_MULT, 0,
            RuntimeFinalCodelet, 0, CLOMP_numParts, (double*)&((deposit))));
    }
    deposit = calc_deposit();
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP761>(ompNumThreads * DARTS_CODELETS_MULT, 0,
            RuntimeFinalCodelet, 0, CLOMP_numParts, (double*)&((deposit))));
    }
    deposit = calc_deposit();
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP794>(ompNumThreads * DARTS_CODELETS_MULT, 0,
            RuntimeFinalCodelet, 0, CLOMP_numParts, (double*)&((deposit))));
    }
}
/*Function: static_omp_module4, ID: 22*/
void static_omp_module4()
{
    /*static_omp_module4:22*/
    /*CompoundStmt:825*/
    double deposit;
    long pidx;
    deposit = calc_deposit();
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP830>(ompNumThreads * DARTS_CODELETS_MULT, 0,
            RuntimeFinalCodelet, 0, CLOMP_numParts, (double*)&((deposit))));
    }
    deposit = calc_deposit();
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP863>(ompNumThreads * DARTS_CODELETS_MULT, 0,
            RuntimeFinalCodelet, 0, CLOMP_numParts, (double*)&((deposit))));
    }
    deposit = calc_deposit();
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP896>(ompNumThreads * DARTS_CODELETS_MULT, 0,
            RuntimeFinalCodelet, 0, CLOMP_numParts, (double*)&((deposit))));
    }
    deposit = calc_deposit();
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP929>(ompNumThreads * DARTS_CODELETS_MULT, 0,
            RuntimeFinalCodelet, 0, CLOMP_numParts, (double*)&((deposit))));
    }
}
/*Function: static_omp_cycle, ID: 23*/
void static_omp_cycle()
{
    /*static_omp_cycle:23*/
    /*CompoundStmt:960*/
    static_omp_module1();
    static_omp_module2();
    static_omp_module3();
    static_omp_module4();
}
/*Function: do_static_omp_version, ID: 24*/
void do_static_omp_version()
{
    /*do_static_omp_version:24*/
    /*CompoundStmt:965*/
    long iteration;
    for (iteration = 0; iteration < CLOMP_num_iterations; iteration++) {
        static_omp_cycle();
    }
}
/*Function: dynamic_omp_module1, ID: 25*/
void dynamic_omp_module1()
{
    /*dynamic_omp_module1:25*/
    /*CompoundStmt:973*/
    double deposit;
    long pidx;
    deposit = calc_deposit();
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP978>(ompNumThreads * DARTS_CODELETS_MULT, 0,
            RuntimeFinalCodelet, 0, CLOMP_numParts, (double*)&((deposit))));
    }
}
/*Function: dynamic_omp_module2, ID: 26*/
void dynamic_omp_module2()
{
    /*dynamic_omp_module2:26*/
    /*CompoundStmt:1009*/
    double deposit;
    long pidx;
    deposit = calc_deposit();
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP1014>(ompNumThreads * DARTS_CODELETS_MULT, 0,
            RuntimeFinalCodelet, 0, CLOMP_numParts, (double*)&((deposit))));
    }
    deposit = calc_deposit();
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP1047>(ompNumThreads * DARTS_CODELETS_MULT, 0,
            RuntimeFinalCodelet, 0, CLOMP_numParts, (double*)&((deposit))));
    }
}
/*Function: dynamic_omp_module3, ID: 27*/
void dynamic_omp_module3()
{
    /*dynamic_omp_module3:27*/
    /*CompoundStmt:1078*/
    double deposit;
    long pidx;
    deposit = calc_deposit();
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP1083>(ompNumThreads * DARTS_CODELETS_MULT, 0,
            RuntimeFinalCodelet, 0, CLOMP_numParts, (double*)&((deposit))));
    }
    deposit = calc_deposit();
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP1116>(ompNumThreads * DARTS_CODELETS_MULT, 0,
            RuntimeFinalCodelet, 0, CLOMP_numParts, (double*)&((deposit))));
    }
    deposit = calc_deposit();
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP1149>(ompNumThreads * DARTS_CODELETS_MULT, 0,
            RuntimeFinalCodelet, 0, CLOMP_numParts, (double*)&((deposit))));
    }
}
/*Function: dynamic_omp_module4, ID: 28*/
void dynamic_omp_module4()
{
    /*dynamic_omp_module4:28*/
    /*CompoundStmt:1180*/
    double deposit;
    long pidx;
    deposit = calc_deposit();
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP1185>(ompNumThreads * DARTS_CODELETS_MULT, 0,
            RuntimeFinalCodelet, 0, CLOMP_numParts, (double*)&((deposit))));
    }
    deposit = calc_deposit();
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP1218>(ompNumThreads * DARTS_CODELETS_MULT, 0,
            RuntimeFinalCodelet, 0, CLOMP_numParts, (double*)&((deposit))));
    }
    deposit = calc_deposit();
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP1251>(ompNumThreads * DARTS_CODELETS_MULT, 0,
            RuntimeFinalCodelet, 0, CLOMP_numParts, (double*)&((deposit))));
    }
    deposit = calc_deposit();
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP1284>(ompNumThreads * DARTS_CODELETS_MULT, 0,
            RuntimeFinalCodelet, 0, CLOMP_numParts, (double*)&((deposit))));
    }
}
/*Function: dynamic_omp_cycle, ID: 29*/
void dynamic_omp_cycle()
{
    /*dynamic_omp_cycle:29*/
    /*CompoundStmt:1315*/
    dynamic_omp_module1();
    dynamic_omp_module2();
    dynamic_omp_module3();
    dynamic_omp_module4();
}
/*Function: do_dynamic_omp_version, ID: 30*/
void do_dynamic_omp_version()
{
    /*do_dynamic_omp_version:30*/
    /*CompoundStmt:1320*/
    long iteration;
    for (iteration = 0; iteration < CLOMP_num_iterations; iteration++) {
        dynamic_omp_cycle();
    }
}
/*Function: do_manual_omp_version, ID: 35*/
void do_manual_omp_version(long num_iterations)
{
    /*do_manual_omp_version:35*/
    /*CompoundStmt:1450*/
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP1451>(ompNumThreads * DARTS_CODELETS_MULT, 0,
            RuntimeFinalCodelet, (long*)&((num_iterations))));
    }
}
/*Function: bestcase_omp_module1, ID: 36*/
void bestcase_omp_module1(int startPidx, int endPidx, double deposit)
{
    /*bestcase_omp_module1:36*/
    /*CompoundStmt:1499*/
    long pidx;
    for (pidx = startPidx; pidx <= endPidx; pidx++)
        update_part(partArray[pidx], deposit);
}
/*Function: bestcase_omp_module2, ID: 37*/
void bestcase_omp_module2(int startPidx, int endPidx, double deposit)
{
    /*bestcase_omp_module2:37*/
    /*CompoundStmt:1507*/
    long pidx;
    for (pidx = startPidx; pidx <= endPidx; pidx++)
        update_part(partArray[pidx], deposit);
    for (pidx = startPidx; pidx <= endPidx; pidx++)
        update_part(partArray[pidx], deposit);
}
/*Function: bestcase_omp_module3, ID: 38*/
void bestcase_omp_module3(int startPidx, int endPidx, double deposit)
{
    /*bestcase_omp_module3:38*/
    /*CompoundStmt:1521*/
    long pidx;
    for (pidx = startPidx; pidx <= endPidx; pidx++)
        update_part(partArray[pidx], deposit);
    for (pidx = startPidx; pidx <= endPidx; pidx++)
        update_part(partArray[pidx], deposit);
    for (pidx = startPidx; pidx <= endPidx; pidx++)
        update_part(partArray[pidx], deposit);
}
/*Function: bestcase_omp_module4, ID: 39*/
void bestcase_omp_module4(int startPidx, int endPidx, double deposit)
{
    /*bestcase_omp_module4:39*/
    /*CompoundStmt:1541*/
    long pidx;
    for (pidx = startPidx; pidx <= endPidx; pidx++)
        update_part(partArray[pidx], deposit);
    for (pidx = startPidx; pidx <= endPidx; pidx++)
        update_part(partArray[pidx], deposit);
    for (pidx = startPidx; pidx <= endPidx; pidx++)
        update_part(partArray[pidx], deposit);
    for (pidx = startPidx; pidx <= endPidx; pidx++)
        update_part(partArray[pidx], deposit);
}
/*Function: bestcase_omp_cycle, ID: 40*/
void bestcase_omp_cycle(int startPidx, int endPidx, double deposit)
{
    /*bestcase_omp_cycle:40*/
    /*CompoundStmt:1567*/
    bestcase_omp_module1(startPidx, endPidx, deposit);
    bestcase_omp_module2(startPidx, endPidx, deposit);
    bestcase_omp_module3(startPidx, endPidx, deposit);
    bestcase_omp_module4(startPidx, endPidx, deposit);
}
/*Function: do_bestcase_omp_version, ID: 41*/
void do_bestcase_omp_version(long num_iterations)
{
    /*do_bestcase_omp_version:41*/
    /*CompoundStmt:1572*/
    long iter, subcycle;
    double deposit;
    deposit = (1. + CLOMP_max_residue) / CLOMP_numParts;
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP1579>(ompNumThreads * DARTS_CODELETS_MULT, 0,
            RuntimeFinalCodelet, (double*)&((deposit)), (long*)&((num_iterations))));
    }
    for (iter = 0; iter < num_iterations; iter++) {
        for (subcycle = 0; subcycle < 10; subcycle++) {
            partArray[0]->update_count = 1;
            partArray[0]->firstZone->value = calc_deposit();
        }
    }
}
/*Function: addPart, ID: 42*/
void addPart(Part* part, long partId)
{
    /*addPart:42*/
    /*CompoundStmt:1642*/
    if ((partId < 0) || (partId >= CLOMP_numParts)) {
        fprintf(stderr, "addPart error: partId (%i) out of bounds!\n", (int)partId);
        exit(1);
    }
    if (partArray[partId] != (Part*)((void*)0)) {
        fprintf(stderr, "addPart error: partId (%i) already initialized!\n", (int)partId);
        exit(1);
    }
    partArray[partId] = part;
    part->partId = partId;
    part->zoneCount = CLOMP_zonesPerPart;
    part->deposit_ratio = ((double)((1.5 * (double)CLOMP_numParts) + partId))
        / ((double)(CLOMP_zonesPerPart * CLOMP_numParts));
    part->residue = 0.;
    part->firstZone = (Zone*)((void*)0);
    part->lastZone = (Zone*)((void*)0);
    part->expected_first_value = -1.;
    part->expected_residue = -1.;
}
/*Function: addZone, ID: 43*/
void addZone(Part* part, Zone* zone)
{
    /*addZone:43*/
    /*CompoundStmt:1690*/
    if (part == (Part*)((void*)0)) {
        fprintf(stderr, "addZone error: part NULL!\n");
        exit(1);
    }
    if (zone == (Zone*)((void*)0)) {
        fprintf(stderr, "addZone error: zone NULL!\n");
        exit(1);
    }
    memset(zone, 255, CLOMP_zoneSize);
    if (part->lastZone == (Zone*)((void*)0)) {
        zone->zoneId = 1;
        part->firstZone = zone;
        part->lastZone = zone;
    } else {
        zone->zoneId = part->lastZone->zoneId + 1;
        part->lastZone->nextZone = zone;
        part->lastZone = zone;
    }
    zone->nextZone = (Zone*)((void*)0);
    zone->partId = part->partId;
    zone->value = 0.;
}
/*Function: main, ID: 44*/
int main(int argc, char* argv[])
{
    getOMPNumThreads();
    getOMPSchedulePolicy();
    getTPLoopThresholds();
    getNumTPs();
    affin = new ThreadAffinity(
        ompNumThreads / NUMTPS - 1, NUMTPS, COMPACT, getDARTSTPPolicy(), getDARTSMCPolicy());
    affinMaskRes = affin->generateMask();
    myDARTSRuntime = new Runtime(affin);
    RuntimeFinalCodelet = &(myDARTSRuntime->finalSignal);
    deposit_darts31 = (double)0;
    deposit_darts32 = (double)0;
    deposit_darts33 = (double)0;
    deposit_darts34 = (double)0;
    /*main:44*/
    /*CompoundStmt:1732*/
    char hostname[200];
    time_t starttime;
    char startdate[50];
    long partId;
    double totalZoneCount;
    double deposit, percent_residue, part_deposit_bound;
    double deposit_diff_bound;
    double diterations;
    struct timeval calc_deposit_start_ts, calc_deposit_end_ts;
    double calc_deposit_seconds;
    struct timeval omp_barrier_start_ts, omp_barrier_end_ts;
    double omp_barrier_seconds;
    struct timeval serial_ref_start_ts, serial_ref_end_ts;
    double serial_ref_seconds;
    struct timeval bestcase_omp_start_ts, bestcase_omp_end_ts;
    double bestcase_omp_seconds;
    struct timeval static_omp_start_ts, static_omp_end_ts;
    double static_omp_seconds;
    struct timeval manual_omp_start_ts, manual_omp_end_ts;
    double manual_omp_seconds;
    struct timeval dynamic_omp_start_ts, dynamic_omp_end_ts;
    double dynamic_omp_seconds;
    int aidx;
    CLOMP_exe_name = argv[0];
    printf("CORAL Benchmark Version 1.2\n");
    if (argc != 8) {
        print_usage();
        exit(1);
    }
    if (gethostname(hostname, sizeof(hostname)) != 0)
        strcpy(hostname, "(Unknown host)");
    time(&starttime);
    ctime_r(&starttime, startdate);
    CLOMP_numThreads = convert_to_positive_long("numThreads", argv[1]);
    CLOMP_allocThreads = convert_to_positive_long("numThreads", argv[2]);
    CLOMP_numParts = convert_to_positive_long("numParts", argv[3]);
    CLOMP_zonesPerPart = convert_to_positive_long("zonesPerPart", argv[4]);
    CLOMP_zoneSize = convert_to_positive_long("zoneSize", argv[5]);
    CLOMP_flopScale = convert_to_positive_long("flopScale", argv[6]);
    CLOMP_timeScale = convert_to_positive_long("timeScale", argv[7]);
    if (CLOMP_zoneSize < (long)sizeof(Zone)) {
        printf("***Forcing zoneSize (%ld specified) to minimum zone size %ld\n\n", CLOMP_zoneSize,
            (long)sizeof(Zone));
        CLOMP_zoneSize = sizeof(Zone);
    }
    printf("       Invocation:");
    for (aidx = 0; aidx < argc; aidx++) {
        printf(" %s", argv[aidx]);
    }
    printf("\n");
    printf("         Hostname: %s\n", hostname);
    printf("       Start time: %s", startdate);
    printf("       Executable: %s\n", CLOMP_exe_name);
    if (CLOMP_numThreads == -1) {
        CLOMP_numThreads = omp_get_max_threads();
        printf("      numThreads: %d (using system default)\n", (int)CLOMP_numThreads);
    } else {
        printf("      numThreads: %ld\n", CLOMP_numThreads);
    }
    CLOMP_inputAllocThreads = CLOMP_allocThreads;
    if (CLOMP_allocThreads == -1) {
        CLOMP_allocThreads = CLOMP_numThreads;
        printf("    allocThreads: %ld (using numThreads)\n", CLOMP_allocThreads);
    } else {
        printf("    allocThreads: %ld\n", CLOMP_allocThreads);
    }
    printf("        numParts: %ld\n", CLOMP_numParts);
    printf("    zonesPerPart: %ld\n", CLOMP_zonesPerPart);
    printf("       flopScale: %ld\n", CLOMP_flopScale);
    printf("       timeScale: %ld\n", CLOMP_timeScale);
    printf("        zoneSize: %ld\n", CLOMP_zoneSize);
    omp_set_num_threads((int)CLOMP_allocThreads);
    partArray = (Part**)malloc(CLOMP_numParts * sizeof(Part*));
    if (partArray == (Part**)((void*)0)) {
        fprintf(stderr, "Out of memory allocating part array\n");
        exit(1);
    }
    for (partId = 0; partId < CLOMP_numParts; partId++) {
        partArray[partId] = (Part*)((void*)0);
    }
    CLOMP_partRatio = 1. / ((double)CLOMP_numParts);
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP1860>(
            ompNumThreads * DARTS_CODELETS_MULT, 0, RuntimeFinalCodelet, 0, CLOMP_numParts));
    }
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP1897>(
            ompNumThreads * DARTS_CODELETS_MULT, 0, RuntimeFinalCodelet, 0, CLOMP_numParts));
    }
    totalZoneCount = (double)CLOMP_numParts * (double)CLOMP_zonesPerPart;
    printf("   Zones per Part: %.0f\n", (double)CLOMP_zonesPerPart);
    printf("      Total Zones: %.0f\n", (double)totalZoneCount);
    printf("Memory (in bytes): %.0f\n",
        (double)(totalZoneCount * CLOMP_zoneSize) + (double)(sizeof(Part) * CLOMP_numParts));
    diterations = ceil((((double)1000000) * ((double)CLOMP_timeScale))
        / ((double)totalZoneCount * (double)CLOMP_flopScale));
    if (diterations > 2.0E+9) {
        printf("*** Forcing iterations from (%g) to 2 billion\n", diterations);
        diterations = 2.0E+9;
    }
    CLOMP_num_iterations = (long)diterations;
    printf("Scaled Iterations: %i\n", (int)CLOMP_num_iterations);
    printf("  Total Subcycles: %.0f\n", (double)CLOMP_num_iterations * (double)10.);
    percent_residue = 0.;
    deposit = CLOMP_partRatio;
    CLOMP_error_bound = deposit;
    CLOMP_tightest_error_bound = deposit;
    for (partId = 0; partId < CLOMP_numParts; partId++) {
        update_part(partArray[partId], deposit);
        percent_residue += partArray[partId]->residue;
        part_deposit_bound = partArray[partId]->residue * partArray[partId]->deposit_ratio;
        if (CLOMP_error_bound > part_deposit_bound) {
            CLOMP_error_bound = part_deposit_bound;
        }
        deposit_diff_bound = part_deposit_bound * partArray[partId]->deposit_ratio;
        if (CLOMP_tightest_error_bound > deposit_diff_bound) {
            CLOMP_tightest_error_bound = deposit_diff_bound;
        }
    }
    printf("Iteration Residue: %.6f%%\n", percent_residue * 100.);
    printf("  Max Error bound: %-8.8g\n", CLOMP_error_bound);
    printf("Tight Error bound: %-8.8g\n", CLOMP_tightest_error_bound);
    CLOMP_max_residue = (1. * percent_residue) / (1 - percent_residue);
    printf("      Max Residue: %-8.8g\n", CLOMP_max_residue);
    CLOMP_residue_ratio_part0 = percent_residue / partArray[0]->residue;
    omp_set_num_threads((int)CLOMP_numThreads);
    printf("---------------------\n");
    print_pseudocode("calc_deposit", "------ Start calc_deposit Pseudocode ------");
    print_pseudocode("calc_deposit", "/* Measure *only* non-threadable calc_deposit() overhead.*/");
    print_pseudocode("calc_deposit", "/* Expect this overhead to be negligible.*/");
    print_pseudocode("calc_deposit", "deposit = calc_deposit ();");
    print_pseudocode("calc_deposit", "------- End calc_deposit Pseudocode -------");
    print_start_message("calc_deposit");
    get_timestamp(&calc_deposit_start_ts);
    do_calc_deposit_only();
    get_timestamp(&calc_deposit_end_ts);
    calc_deposit_seconds
        = print_timestats("calc_deposit", &calc_deposit_start_ts, &calc_deposit_end_ts, -1., -1.);
    do_omp_barrier_only(1);
    print_pseudocode("OMP Barrier", "------ Start OMP Barrier Pseudocode ------");
    print_pseudocode("OMP Barrier", "/* Measure *only* OMP barrier overhead.*/");
    print_pseudocode("OMP Barrier", "//#pragma omp barrier");
    print_pseudocode("OMP Barrier", "------- End OMP Barrier Pseudocode -------");
    print_start_message("OMP Barrier");
    get_timestamp(&omp_barrier_start_ts);
    do_omp_barrier_only(CLOMP_num_iterations);
    get_timestamp(&omp_barrier_end_ts);
    omp_barrier_seconds
        = print_timestats("OMP Barrier", &omp_barrier_start_ts, &omp_barrier_end_ts, -1., -1.);
    reinitialize_parts();
    serial_ref_cycle();
    reinitialize_parts();
    print_pseudocode("Serial Ref", "------ Start Serial Ref Pseudocode ------");
    print_pseudocode("Serial Ref", "/* Measure serial reference performance */");
    print_pseudocode("Serial Ref", "deposit = calc_deposit ();");
    print_pseudocode("Serial Ref", "for (pidx = 0; pidx < numParts; pidx++)");
    print_pseudocode("Serial Ref", "  update_part (partArray[pidx], deposit);");
    print_pseudocode("Serial Ref", "------- End Serial Ref Pseudocode -------");
    print_start_message("Serial Ref");
    get_timestamp(&serial_ref_start_ts);
    do_serial_ref_version();
    get_timestamp(&serial_ref_end_ts);
    print_data_stats("Serial Ref");
    serial_ref_seconds
        = print_timestats("Serial Ref", &serial_ref_start_ts, &serial_ref_end_ts, -1., -1.);
    reinitialize_parts();
    do_bestcase_omp_version(1);
    reinitialize_parts();
    print_pseudocode("Bestcase OMP", "------ Start Bestcase OMP Pseudocode ------");
    print_pseudocode("Bestcase OMP", "/* Measure the bestcase ref loop runtime for */");
    print_pseudocode("Bestcase OMP", "/* \"free\" threading the following code: */");
    print_pseudocode("Bestcase OMP", "deposit = calc_deposit ();");
    print_pseudocode("Bestcase OMP", "for (pidx = 0; pidx < numParts; pidx++)");
    print_pseudocode("Bestcase OMP", "  update_part (partArray[pidx], deposit);");
    print_pseudocode("Bestcase OMP", "------- End Bestcase OMP Pseudocode -------");
    print_start_message("Bestcase OMP");
    get_timestamp(&bestcase_omp_start_ts);
    do_bestcase_omp_version(CLOMP_num_iterations);
    get_timestamp(&bestcase_omp_end_ts);
    bestcase_omp_seconds = print_timestats(
        "Bestcase OMP", &bestcase_omp_start_ts, &bestcase_omp_end_ts, serial_ref_seconds, -1.);
    reinitialize_parts();
    static_omp_cycle();
    reinitialize_parts();
    print_pseudocode("Static OMP", "------ Start Static OMP Pseudocode ------");
    print_pseudocode(
        "Static OMP", "/* Use OpenMP parallel for schedule(static) on original loop. */");
    print_pseudocode("Static OMP", "deposit = calc_deposit ();");
    print_pseudocode("Static OMP", "//#pragma omp parallel for private (pidx) schedule(static)");
    print_pseudocode("Static OMP", "for (pidx = 0; pidx < numParts; pidx++)");
    print_pseudocode("Static OMP", "  update_part (partArray[pidx], deposit);");
    print_pseudocode("Static OMP", "------- End Static OMP Pseudocode -------");
    print_start_message("Static OMP");
    get_timestamp(&static_omp_start_ts);
    do_static_omp_version();
    get_timestamp(&static_omp_end_ts);
    print_data_stats("Static OMP");
    static_omp_seconds = print_timestats("Static OMP", &static_omp_start_ts, &static_omp_end_ts,
        serial_ref_seconds, bestcase_omp_seconds);
    reinitialize_parts();
    dynamic_omp_cycle();
    reinitialize_parts();
    print_pseudocode("Dynamic OMP", "------ Start Dynamic OMP Pseudocode ------");
    print_pseudocode(
        "Dynamic OMP", "/* Use OpenMP parallel for schedule(dynamic) on orig loop. */");
    print_pseudocode("Dynamic OMP", "deposit = calc_deposit ();");
    print_pseudocode("Dynamic OMP", "//#pragma omp parallel for private (pidx) schedule(dynamic)");
    print_pseudocode("Dynamic OMP", "for (pidx = 0; pidx < numParts; pidx++)");
    print_pseudocode("Dynamic OMP", "  update_part (partArray[pidx], deposit);");
    print_pseudocode("Dynamic OMP", "------- End Dynamic OMP Pseudocode -------");
    print_start_message("Dynamic OMP");
    get_timestamp(&dynamic_omp_start_ts);
    do_dynamic_omp_version();
    get_timestamp(&dynamic_omp_end_ts);
    print_data_stats("Dynamic OMP");
    dynamic_omp_seconds = print_timestats("Dynamic OMP", &dynamic_omp_start_ts, &dynamic_omp_end_ts,
        serial_ref_seconds, bestcase_omp_seconds);
    reinitialize_parts();
    do_manual_omp_version(1);
    reinitialize_parts();
    print_pseudocode("Manual OMP", "------ Start Manual OMP Pseudocode ------");
    print_pseudocode("Manual OMP", "/* At top level, spawn threads and manually partition parts*/");
    print_pseudocode("Manual OMP", "//#pragma omp parallel");
    print_pseudocode("Manual OMP", "{");
    print_pseudocode("Manual OMP", "   int startPidx = ... /* slice based on thread_id*/");
    print_pseudocode("Manual OMP", "   for (iter = 0; iter < num_iterations; iter++) ");
    print_pseudocode("Manual OMP", "      do_iter(startPidx, endPidx);");
    print_pseudocode("Manual OMP", "}");
    print_pseudocode("Manual OMP", "...");
    print_pseudocode("Manual OMP", "do_modN(int startPidx, int endPidx) /*do_iter() calls*/");
    print_pseudocode("Manual OMP", "{");
    print_pseudocode("Manual OMP", "  //#pragma omp barrier /* All threads must finish first!*/");
    print_pseudocode("Manual OMP", "  //#pragma omp single  /* Only one thread calcs deposit!*/");
    print_pseudocode("Manual OMP", "  {");
    print_pseudocode(
        "Manual OMP", "    deposit = calc_deposit (); /* Deposit shared by threads */");
    print_pseudocode("Manual OMP", "  }  /* Implicit omp barrier at end of omp single */");
    print_pseudocode("Manual OMP", "  /* All threads execute loop working just on their parts*/");
    print_pseudocode("Manual OMP", "  for (pidx = startPidx; pidx <= endPidx; pidx++)");
    print_pseudocode("Manual OMP", "    update_part (partArray[pidx], deposit);");
    print_pseudocode("Manual OMP", "}");
    print_pseudocode("Manual OMP", "------- End Manual OMP Pseudocode -------");
    print_start_message("Manual OMP");
    get_timestamp(&manual_omp_start_ts);
    do_manual_omp_version(CLOMP_num_iterations);
    get_timestamp(&manual_omp_end_ts);
    print_data_stats("Manual OMP");
    manual_omp_seconds = print_timestats("Manual OMP", &manual_omp_start_ts, &manual_omp_end_ts,
        serial_ref_seconds, bestcase_omp_seconds);
    printf("----------- Comma-delimited summary ----------\n");
    printf("%s %ld %ld %ld %ld %ld %ld %ld, calc_deposit, OMP Barrier, Serial Ref, Bestcase OMP, "
           "Static OMP, Dynamic OMP, Manual OMP\n",
        CLOMP_exe_name, CLOMP_numThreads, CLOMP_inputAllocThreads, CLOMP_numParts,
        CLOMP_zonesPerPart, CLOMP_zoneSize, CLOMP_flopScale, CLOMP_timeScale);
    printf("Runtime, %7.3f, %7.3f, %7.3f, %7.3f, %7.3f, %7.3f, %7.3f\n", calc_deposit_seconds,
        omp_barrier_seconds, serial_ref_seconds, bestcase_omp_seconds, static_omp_seconds,
        dynamic_omp_seconds, manual_omp_seconds);
    printf("us/Loop, %7.2f, %7.2f, %7.2f, %7.2f, %7.2f, %7.2f, %7.2f\n",
        (((calc_deposit_seconds * 1.0E+6) / ((double)CLOMP_num_iterations * 10.))),
        (((omp_barrier_seconds * 1.0E+6) / ((double)CLOMP_num_iterations * 10.))),
        (((serial_ref_seconds * 1.0E+6) / ((double)CLOMP_num_iterations * 10.))),
        (((bestcase_omp_seconds * 1.0E+6) / ((double)CLOMP_num_iterations * 10.))),
        (((static_omp_seconds * 1.0E+6) / ((double)CLOMP_num_iterations * 10.))),
        (((dynamic_omp_seconds * 1.0E+6) / ((double)CLOMP_num_iterations * 10.))),
        (((manual_omp_seconds * 1.0E+6) / ((double)CLOMP_num_iterations * 10.))));
    printf("Speedup,     N/A,     N/A, %7.1f, %7.1f, %7.1f, %7.1f, %7.1f\n",
        ((serial_ref_seconds / serial_ref_seconds)), ((serial_ref_seconds / bestcase_omp_seconds)),
        ((serial_ref_seconds / static_omp_seconds)), ((serial_ref_seconds / dynamic_omp_seconds)),
        ((serial_ref_seconds / manual_omp_seconds)));
    printf("Efficacy,    N/A,     N/A,     N/A,  %6.1f%%, %6.1f%%, %6.1f%%, %6.1f%%\n",
        (((bestcase_omp_seconds / bestcase_omp_seconds) * 100.)),
        (((bestcase_omp_seconds / static_omp_seconds) * 100.)),
        (((bestcase_omp_seconds / dynamic_omp_seconds) * 100.)),
        (((bestcase_omp_seconds / manual_omp_seconds) * 100.)));
    printf("Overhead,    N/A,     N/A,     N/A, %7.2f, %7.2f, %7.2f, %7.2f\n",
        (((bestcase_omp_seconds - bestcase_omp_seconds) * 1.0E+6)
            / ((double)CLOMP_num_iterations * 10.)),
        (((static_omp_seconds - bestcase_omp_seconds) * 1.0E+6)
            / ((double)CLOMP_num_iterations * 10.)),
        (((dynamic_omp_seconds - bestcase_omp_seconds) * 1.0E+6)
            / ((double)CLOMP_num_iterations * 10.)),
        (((manual_omp_seconds - bestcase_omp_seconds) * 1.0E+6)
            / ((double)CLOMP_num_iterations * 10.)));
    {
        /*CompoundStmt:2293*/
        char mpi_marker[100] = "";
        printf("CORAL RFP, %s%ld %ld %ld %ld %ld %ld %ld, %.2f, %.2f, %.1f, %.2f, %.1f, %.2f, "
               "%.1f, %.2f, %.1f\n",
            mpi_marker, CLOMP_numThreads, CLOMP_inputAllocThreads, CLOMP_numParts,
            CLOMP_zonesPerPart, CLOMP_zoneSize, CLOMP_flopScale, CLOMP_timeScale,
            (((serial_ref_seconds * 1.0E+6) / ((double)CLOMP_num_iterations * 10.))),
            (((omp_barrier_seconds * 1.0E+6) / ((double)CLOMP_num_iterations * 10.))),
            ((serial_ref_seconds / bestcase_omp_seconds)),
            (((static_omp_seconds - bestcase_omp_seconds) * 1.0E+6)
                / ((double)CLOMP_num_iterations * 10.)),
            ((serial_ref_seconds / static_omp_seconds)),
            (((dynamic_omp_seconds - bestcase_omp_seconds) * 1.0E+6)
                / ((double)CLOMP_num_iterations * 10.)),
            ((serial_ref_seconds / dynamic_omp_seconds)),
            (((manual_omp_seconds - bestcase_omp_seconds) * 1.0E+6)
                / ((double)CLOMP_num_iterations * 10.)),
            ((serial_ref_seconds / manual_omp_seconds)));
    }
    return (0);
}
/*TP500: OMPParallelDirective*/
void TP500::_barrierCodelets500::fire(void)
{
    TP500* myTP = static_cast<TP500*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
void TP500::_checkInCodelets502::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 502: DeclStmt*/

    /*printing node 504: BinaryOperator*/
    (this->inputsTPParent->iteration_darts500[this->getID()]) = 0;

    /*printing node 505: BinaryOperator*/
    /*Print the code for a condition node in a complex loop stmt */
    if ((this->inputsTPParent->iteration_darts500[this->getID()])
        < (*(this->inputsTPParent->num_iterations_darts500))) {
        /*Signal the first codelet in the loop*/
        myTP->checkInCodelets503[this->getID()].decDep();
        return;
    }
    /*Signal the codelet after the loop from the end's condional node.*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets500[0].decDep();
}
void TP500::_checkInCodelets503::fire(void)
{

    /*printing node 503: ForStmt*/
    bool haveToLaunch = __sync_bool_compare_and_swap(&(myTP->controlTPParent->TP503_LoopCounter),
        myTP->controlTPParent->TP503_LoopCounterPerThread[this->getID()],
        myTP->controlTPParent->TP503_LoopCounterPerThread[this->getID()] + 1);
    unsigned int iterIdx = myTP->controlTPParent->TP503_LoopCounterPerThread[this->getID()];
    if (haveToLaunch) {
        this->resetCodelet();
        myTP->controlTPParent->TP503PtrVec.push_back(nullptr);
        myTP->controlTPParent->TP503_LoopCounterPerThread[this->getID()] += 1;
        invoke<TP503>(myTP, myTP->numThreads, this->getID(), myTP, myTP->inputsTPParent,
            &(myTP->controlTPParent->TP503PtrVec.back()));
    } else {
        if (myTP->controlTPParent->TP503PtrVec.size() == 0) {
            this->resetCodelet();
            this->decDep();
            return;
        } else if (myTP->controlTPParent->TP503PtrVec.size() < (iterIdx + 1)) {
            this->resetCodelet();
            this->decDep();
            return;
        } else if (myTP->controlTPParent->TP503PtrVec[iterIdx] == nullptr) {
            this->resetCodelet();
            this->decDep();
            return;
        } else {
            this->resetCodelet();
#if USE_SPIN_CODELETS == 0
            myTP->controlTPParent->TP503PtrVec[iterIdx]->firstCodelet[this->getID()].decDep();
#else
            myTP->controlTPParent->TP503PtrVec[iterIdx]->availableCodelets[this->getID()] = 1;
#endif
            myTP->controlTPParent->TP503_LoopCounterPerThread[this->getID()] += 1;
        }
    }
}
void TP500::_checkInCodelets506::fire(void)
{

    /*printing node 506: UnaryOperator*/
    (this->inputsTPParent->iteration_darts500[this->getID()])++;

    /*printing node 2335: BinaryOperator*/
    /*Print the code for a condition node in a complex loop stmt */
    if ((this->inputsTPParent->iteration_darts500[this->getID()])
        < (*(this->inputsTPParent->num_iterations_darts500))) {
        this->resetCodelet();
        /*Signal the first codelet in the loop*/
        myTP->checkInCodelets503[this->getID()].decDep();
        return;
    }
    /*Signal the codelet after the loop from the condtional node.*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets500[0].decDep();
}
TP500::TP500(
    int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet, long* in_num_iterations)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , num_iterations_darts500(in_num_iterations) /*OMP_SHARED - INPUT*/
    , iteration_darts500(new long[this->numThreads]) /*VARIABLE*/
    , subcycle_darts500(new long[this->numThreads]) /*VARIABLE*/
    , TP503_LoopCounter(0)
    , TP503_LoopCounterPerThread(new unsigned int[this->numThreads])
    , barrierCodelets500(new _barrierCodelets500[1])
    , checkInCodelets502(new _checkInCodelets502[this->numThreads])
    , checkInCodelets503(new _checkInCodelets503[this->numThreads])
    , checkInCodelets506(new _checkInCodelets506[this->numThreads])
{
    memset((void*)TP503_LoopCounterPerThread, 0, this->numThreads * sizeof(unsigned int));
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets500[0] = _barrierCodelets500(ompNumThreads, ompNumThreads, this, 0);
    _checkInCodelets506* checkInCodelets506Ptr = (this->checkInCodelets506);
    _checkInCodelets503* checkInCodelets503Ptr = (this->checkInCodelets503);
    _checkInCodelets502* checkInCodelets502Ptr = (this->checkInCodelets502);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets506Ptr) = _checkInCodelets506(1, 1, this, codeletCounter);
        checkInCodelets506Ptr++;
        (*checkInCodelets503Ptr) = _checkInCodelets503(1, 1, this, codeletCounter);
        checkInCodelets503Ptr++;
        (*checkInCodelets502Ptr) = _checkInCodelets502(1, 1, this, codeletCounter);
        (*checkInCodelets502Ptr).decDep();
        checkInCodelets502Ptr++;
    }
}
TP500::~TP500()
{
    delete[] TP503_LoopCounterPerThread;
    delete[] iteration_darts500;
    delete[] subcycle_darts500;
    delete[] barrierCodelets500;
    delete[] checkInCodelets506;
    delete[] checkInCodelets503;
    delete[] checkInCodelets502;
}
/*TP503: ForStmt*/
void TP503::_checkInCodelets509::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif

    /*printing node 509: BinaryOperator*/
    (this->inputsTPParent->subcycle_darts500[this->getID()]) = 0;

    /*printing node 510: BinaryOperator*/
    /*Print the code for a condition node in a complex loop stmt */
    if ((this->inputsTPParent->subcycle_darts500[this->getID()]) < 10) {
        /*Signal the first codelet in the loop*/
        myTP->checkInCodelets508[this->getID()].decDep();
        return;
    }
    /*Signal the codelet after the loop from the end's condional node.*/
    /*The node is the last one in a complex loop, so signal the inc node*/
    /*Find the TP to which the nextRegion belongs*/
    myTP->controlTPParent->TPParent->checkInCodelets506[this->getID()].decDep();
}
void TP503::_checkInCodelets508::fire(void)
{

    /*printing node 508: ForStmt*/
    bool haveToLaunch = __sync_bool_compare_and_swap(&(myTP->controlTPParent->TP508_LoopCounter),
        myTP->controlTPParent->TP508_LoopCounterPerThread[this->getID()],
        myTP->controlTPParent->TP508_LoopCounterPerThread[this->getID()] + 1);
    unsigned int iterIdx = myTP->controlTPParent->TP508_LoopCounterPerThread[this->getID()];
    if (haveToLaunch) {
        this->resetCodelet();
        myTP->controlTPParent->TP508PtrVec.push_back(nullptr);
        myTP->controlTPParent->TP508_LoopCounterPerThread[this->getID()] += 1;
        invoke<TP508>(myTP, myTP->numThreads, this->getID(), myTP, myTP->inputsTPParent,
            &(myTP->controlTPParent->TP508PtrVec.back()));
    } else {
        if (myTP->controlTPParent->TP508PtrVec.size() == 0) {
            this->resetCodelet();
            this->decDep();
            return;
        } else if (myTP->controlTPParent->TP508PtrVec.size() < (iterIdx + 1)) {
            this->resetCodelet();
            this->decDep();
            return;
        } else if (myTP->controlTPParent->TP508PtrVec[iterIdx] == nullptr) {
            this->resetCodelet();
            this->decDep();
            return;
        } else {
            this->resetCodelet();
#if USE_SPIN_CODELETS == 0
            myTP->controlTPParent->TP508PtrVec[iterIdx]->firstCodelet[this->getID()].decDep();
#else
            myTP->controlTPParent->TP508PtrVec[iterIdx]->availableCodelets[this->getID()] = 1;
#endif
            myTP->controlTPParent->TP508_LoopCounterPerThread[this->getID()] += 1;
        }
    }
}
void TP503::_checkInCodelets511::fire(void)
{

    /*printing node 511: UnaryOperator*/
    (this->inputsTPParent->subcycle_darts500[this->getID()])++;

    /*printing node 2334: BinaryOperator*/
    /*Print the code for a condition node in a complex loop stmt */
    if ((this->inputsTPParent->subcycle_darts500[this->getID()]) < 10) {
        this->resetCodelet();
        /*Signal the first codelet in the loop*/
        myTP->checkInCodelets508[this->getID()].decDep();
        return;
    }
    /*Signal the codelet after the loop from the condtional node.*/
    /*The node is the last one in a complex loop, so signal the inc node*/
    /*Find the TP to which the nextRegion belongs*/
    myTP->controlTPParent->TPParent->checkInCodelets506[this->getID()].decDep();
}
TP503::TP503(int in_numThreads, int in_mainCodeletID, TP500* in_TPParent, TP500* in_inputsTPParent,
    TP503** in_ptrToThisTP)
    : ompTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(in_inputsTPParent)
    , ptrToThisTP(in_ptrToThisTP)
    , TP508_LoopCounter(0)
    , TP508_LoopCounterPerThread(new unsigned int[this->numThreads])
    , checkInCodelets509(new _checkInCodelets509[this->numThreads])
    , checkInCodelets508(new _checkInCodelets508[this->numThreads])
    , checkInCodelets511(new _checkInCodelets511[this->numThreads])
{
    memset((void*)TP508_LoopCounterPerThread, 0, this->numThreads * sizeof(unsigned int));
    /*Initialize Codelets*/
    _checkInCodelets511* checkInCodelets511Ptr = (this->checkInCodelets511);
    _checkInCodelets508* checkInCodelets508Ptr = (this->checkInCodelets508);
    _checkInCodelets509* checkInCodelets509Ptr = (this->checkInCodelets509);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets509);
#endif
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets511Ptr) = _checkInCodelets511(1, 1, this, codeletCounter);
        checkInCodelets511Ptr++;
        (*checkInCodelets508Ptr) = _checkInCodelets508(1, 1, this, codeletCounter);
        checkInCodelets508Ptr++;
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets509Ptr) = _checkInCodelets509(2, 1, this, codeletCounter);
#else
        (*checkInCodelets509Ptr) = _checkInCodelets509(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets509Ptr).decDep();
        checkInCodelets509Ptr++;
    }
    *(this->ptrToThisTP) = this;
#if USE_SPIN_CODELETS == 0
    this->firstCodelet[this->getID()].decDep();
#else
    this->availableCodelets[this->getID()] = 1;
#endif
}
TP503::~TP503()
{
    delete[] TP508_LoopCounterPerThread;
    delete[] checkInCodelets511;
    delete[] checkInCodelets508;
    delete[] checkInCodelets509;
}
/*TP508: ForStmt*/
void TP508::_checkInCodelets513::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    myTP->barrierCodelets513[0].decDep();
}
void TP508::_barrierCodelets513::fire(void)
{
    TP508* myTP = static_cast<TP508*>(myTP_);
    for (size_t codeletsCounter = 0; codeletsCounter < myTP->numThreads; codeletsCounter++) {
        myTP->TPParent->checkInCodelets511[codeletsCounter].decDep();
    }
}
TP508::TP508(int in_numThreads, int in_mainCodeletID, TP503* in_TPParent, TP500* in_inputsTPParent,
    TP508** in_ptrToThisTP)
    : ompTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(in_inputsTPParent)
    , ptrToThisTP(in_ptrToThisTP)
    , checkInCodelets513(new _checkInCodelets513[this->numThreads])
    , barrierCodelets513(new _barrierCodelets513[1])
{
    /*Initialize Codelets*/
    barrierCodelets513[0] = _barrierCodelets513(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets513* checkInCodelets513Ptr = (this->checkInCodelets513);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets513);
#endif
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets513Ptr) = _checkInCodelets513(2, 1, this, codeletCounter);
#else
        (*checkInCodelets513Ptr) = _checkInCodelets513(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets513Ptr).decDep();
        checkInCodelets513Ptr++;
    }
    *(this->ptrToThisTP) = this;
#if USE_SPIN_CODELETS == 0
    this->firstCodelet[this->getID()].decDep();
#else
    this->availableCodelets[this->getID()] = 1;
#endif
}
TP508::~TP508()
{
    delete[] barrierCodelets513;
    delete[] checkInCodelets513;
}
/*TP623: OMPParallelForDirective*/
void TP623::_barrierCodelets623::fire(void)
{
    TP623* myTP = static_cast<TP623*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
bool TP623::requestNewRangeIterations623(long* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Static */
    /*Chunk = 1*/
    bool isThereNewIteration = false;
    {
        /*Static Scheduling*/
        long tempStartRange = rangePerCodelet623 * codeletID;
        long tempEndRange = rangePerCodelet623 * (codeletID + 1);
        if (remainderRange623 != 0) {
            if (codeletID < (uint32_t)remainderRange623) {
                tempStartRange += codeletID;
                tempEndRange += (codeletID + 1);
            } else {
                tempStartRange += remainderRange623;
                tempEndRange += remainderRange623;
            }
        }
        tempStartRange = tempStartRange * 1 + minIteration623;
        tempEndRange = tempEndRange * 1 + minIteration623;
        if (tempStartRange != tempEndRange) {
            isThereNewIteration = true;
        }
        if (initIteration623 < lastIteration623) {
            (this->inputsTPParent->pidx_darts623[codeletID]) = min(tempStartRange, tempEndRange);
            *endRange = max(tempStartRange, tempEndRange);
        } else {
            (this->inputsTPParent->pidx_darts623[codeletID]) = max(tempStartRange, tempEndRange);
            *endRange = min(tempStartRange, tempEndRange);
        }
        if (codeletID == this->numThreads - 1) {
            *endRange = lastIteration623;
        }
    }
    return isThereNewIteration;
}
void TP623::_checkInCodelets624::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 624: ForStmt*/
    /*var: deposit*/
    /*var: pidx*/
    double* deposit = (this->inputsTPParent->deposit_darts623);
    (void)deposit /*OMP_SHARED*/;
    long* pidx = &(this->inputsTPParent->pidx_darts623[this->getLocalID()]);
    (void)pidx /*OMP_PRIVATE*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations623(
        (long*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        /*Find and signal the next codelet*/
        myTP->controlTPParent->TPParent->barrierCodelets623[0].decDep();
        return;
    }
    for (long pidx_darts_counter_temp623 = (*pidx); pidx_darts_counter_temp623 < endRange
         && pidx_darts_counter_temp623 < this->inputsTPParent->lastIteration623;
         pidx_darts_counter_temp623++) {
        {
            update_part(partArray[(pidx_darts_counter_temp623)], (*(deposit)));
        }
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets623[0].decDep();
}
TP623::TP623(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet,
    long in_initIteration, long in_lastIteration, double* in_deposit)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , deposit_darts623(in_deposit) /*OMP_SHARED - INPUT*/
    , pidx_darts623(new long[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , initIteration623(in_initIteration)
    , lastIteration623(in_lastIteration)
    , barrierCodelets623(new _barrierCodelets623[1])
    , checkInCodelets624(new _checkInCodelets624[this->numThreads])
{
    /*Initialize the loop parameters*/
    range623 = abs(lastIteration623 - initIteration623) / 1;
    rangePerCodelet623 = range623 / numThreads;
    minIteration623 = min<long>(lastIteration623, initIteration623);
    remainderRange623 = range623 % numThreads;
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets623[0] = _barrierCodelets623(ompNumThreads, ompNumThreads, this, 0);
    _checkInCodelets624* checkInCodelets624Ptr = (this->checkInCodelets624);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets624Ptr) = _checkInCodelets624(1, 1, this, codeletCounter);
        (*checkInCodelets624Ptr).decDep();
        checkInCodelets624Ptr++;
    }
}
TP623::~TP623()
{
    delete[] barrierCodelets623;
    delete[] checkInCodelets624;
}
/*TP659: OMPParallelForDirective*/
void TP659::_barrierCodelets659::fire(void)
{
    TP659* myTP = static_cast<TP659*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
bool TP659::requestNewRangeIterations659(long* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Static */
    /*Chunk = 1*/
    bool isThereNewIteration = false;
    {
        /*Static Scheduling*/
        long tempStartRange = rangePerCodelet659 * codeletID;
        long tempEndRange = rangePerCodelet659 * (codeletID + 1);
        if (remainderRange659 != 0) {
            if (codeletID < (uint32_t)remainderRange659) {
                tempStartRange += codeletID;
                tempEndRange += (codeletID + 1);
            } else {
                tempStartRange += remainderRange659;
                tempEndRange += remainderRange659;
            }
        }
        tempStartRange = tempStartRange * 1 + minIteration659;
        tempEndRange = tempEndRange * 1 + minIteration659;
        if (tempStartRange != tempEndRange) {
            isThereNewIteration = true;
        }
        if (initIteration659 < lastIteration659) {
            (this->inputsTPParent->pidx_darts659[codeletID]) = min(tempStartRange, tempEndRange);
            *endRange = max(tempStartRange, tempEndRange);
        } else {
            (this->inputsTPParent->pidx_darts659[codeletID]) = max(tempStartRange, tempEndRange);
            *endRange = min(tempStartRange, tempEndRange);
        }
        if (codeletID == this->numThreads - 1) {
            *endRange = lastIteration659;
        }
    }
    return isThereNewIteration;
}
void TP659::_checkInCodelets660::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 660: ForStmt*/
    /*var: deposit*/
    /*var: pidx*/
    double* deposit = (this->inputsTPParent->deposit_darts659);
    (void)deposit /*OMP_SHARED*/;
    long* pidx = &(this->inputsTPParent->pidx_darts659[this->getLocalID()]);
    (void)pidx /*OMP_PRIVATE*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations659(
        (long*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        /*Find and signal the next codelet*/
        myTP->controlTPParent->TPParent->barrierCodelets659[0].decDep();
        return;
    }
    for (long pidx_darts_counter_temp659 = (*pidx); pidx_darts_counter_temp659 < endRange
         && pidx_darts_counter_temp659 < this->inputsTPParent->lastIteration659;
         pidx_darts_counter_temp659++) {
        {
            update_part(partArray[(pidx_darts_counter_temp659)], (*(deposit)));
        }
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets659[0].decDep();
}
TP659::TP659(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet,
    long in_initIteration, long in_lastIteration, double* in_deposit)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , deposit_darts659(in_deposit) /*OMP_SHARED - INPUT*/
    , pidx_darts659(new long[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , initIteration659(in_initIteration)
    , lastIteration659(in_lastIteration)
    , barrierCodelets659(new _barrierCodelets659[1])
    , checkInCodelets660(new _checkInCodelets660[this->numThreads])
{
    /*Initialize the loop parameters*/
    range659 = abs(lastIteration659 - initIteration659) / 1;
    rangePerCodelet659 = range659 / numThreads;
    minIteration659 = min<long>(lastIteration659, initIteration659);
    remainderRange659 = range659 % numThreads;
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets659[0] = _barrierCodelets659(ompNumThreads, ompNumThreads, this, 0);
    _checkInCodelets660* checkInCodelets660Ptr = (this->checkInCodelets660);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets660Ptr) = _checkInCodelets660(1, 1, this, codeletCounter);
        (*checkInCodelets660Ptr).decDep();
        checkInCodelets660Ptr++;
    }
}
TP659::~TP659()
{
    delete[] barrierCodelets659;
    delete[] checkInCodelets660;
}
/*TP692: OMPParallelForDirective*/
void TP692::_barrierCodelets692::fire(void)
{
    TP692* myTP = static_cast<TP692*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
bool TP692::requestNewRangeIterations692(long* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Static */
    /*Chunk = 1*/
    bool isThereNewIteration = false;
    {
        /*Static Scheduling*/
        long tempStartRange = rangePerCodelet692 * codeletID;
        long tempEndRange = rangePerCodelet692 * (codeletID + 1);
        if (remainderRange692 != 0) {
            if (codeletID < (uint32_t)remainderRange692) {
                tempStartRange += codeletID;
                tempEndRange += (codeletID + 1);
            } else {
                tempStartRange += remainderRange692;
                tempEndRange += remainderRange692;
            }
        }
        tempStartRange = tempStartRange * 1 + minIteration692;
        tempEndRange = tempEndRange * 1 + minIteration692;
        if (tempStartRange != tempEndRange) {
            isThereNewIteration = true;
        }
        if (initIteration692 < lastIteration692) {
            (this->inputsTPParent->pidx_darts692[codeletID]) = min(tempStartRange, tempEndRange);
            *endRange = max(tempStartRange, tempEndRange);
        } else {
            (this->inputsTPParent->pidx_darts692[codeletID]) = max(tempStartRange, tempEndRange);
            *endRange = min(tempStartRange, tempEndRange);
        }
        if (codeletID == this->numThreads - 1) {
            *endRange = lastIteration692;
        }
    }
    return isThereNewIteration;
}
void TP692::_checkInCodelets693::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 693: ForStmt*/
    /*var: deposit*/
    /*var: pidx*/
    double* deposit = (this->inputsTPParent->deposit_darts692);
    (void)deposit /*OMP_SHARED*/;
    long* pidx = &(this->inputsTPParent->pidx_darts692[this->getLocalID()]);
    (void)pidx /*OMP_PRIVATE*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations692(
        (long*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        /*Find and signal the next codelet*/
        myTP->controlTPParent->TPParent->barrierCodelets692[0].decDep();
        return;
    }
    for (long pidx_darts_counter_temp692 = (*pidx); pidx_darts_counter_temp692 < endRange
         && pidx_darts_counter_temp692 < this->inputsTPParent->lastIteration692;
         pidx_darts_counter_temp692++) {
        {
            update_part(partArray[(pidx_darts_counter_temp692)], (*(deposit)));
        }
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets692[0].decDep();
}
TP692::TP692(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet,
    long in_initIteration, long in_lastIteration, double* in_deposit)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , deposit_darts692(in_deposit) /*OMP_SHARED - INPUT*/
    , pidx_darts692(new long[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , initIteration692(in_initIteration)
    , lastIteration692(in_lastIteration)
    , barrierCodelets692(new _barrierCodelets692[1])
    , checkInCodelets693(new _checkInCodelets693[this->numThreads])
{
    /*Initialize the loop parameters*/
    range692 = abs(lastIteration692 - initIteration692) / 1;
    rangePerCodelet692 = range692 / numThreads;
    minIteration692 = min<long>(lastIteration692, initIteration692);
    remainderRange692 = range692 % numThreads;
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets692[0] = _barrierCodelets692(ompNumThreads, ompNumThreads, this, 0);
    _checkInCodelets693* checkInCodelets693Ptr = (this->checkInCodelets693);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets693Ptr) = _checkInCodelets693(1, 1, this, codeletCounter);
        (*checkInCodelets693Ptr).decDep();
        checkInCodelets693Ptr++;
    }
}
TP692::~TP692()
{
    delete[] barrierCodelets692;
    delete[] checkInCodelets693;
}
/*TP728: OMPParallelForDirective*/
void TP728::_barrierCodelets728::fire(void)
{
    TP728* myTP = static_cast<TP728*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
bool TP728::requestNewRangeIterations728(long* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Static */
    /*Chunk = 1*/
    bool isThereNewIteration = false;
    {
        /*Static Scheduling*/
        long tempStartRange = rangePerCodelet728 * codeletID;
        long tempEndRange = rangePerCodelet728 * (codeletID + 1);
        if (remainderRange728 != 0) {
            if (codeletID < (uint32_t)remainderRange728) {
                tempStartRange += codeletID;
                tempEndRange += (codeletID + 1);
            } else {
                tempStartRange += remainderRange728;
                tempEndRange += remainderRange728;
            }
        }
        tempStartRange = tempStartRange * 1 + minIteration728;
        tempEndRange = tempEndRange * 1 + minIteration728;
        if (tempStartRange != tempEndRange) {
            isThereNewIteration = true;
        }
        if (initIteration728 < lastIteration728) {
            (this->inputsTPParent->pidx_darts728[codeletID]) = min(tempStartRange, tempEndRange);
            *endRange = max(tempStartRange, tempEndRange);
        } else {
            (this->inputsTPParent->pidx_darts728[codeletID]) = max(tempStartRange, tempEndRange);
            *endRange = min(tempStartRange, tempEndRange);
        }
        if (codeletID == this->numThreads - 1) {
            *endRange = lastIteration728;
        }
    }
    return isThereNewIteration;
}
void TP728::_checkInCodelets729::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 729: ForStmt*/
    /*var: deposit*/
    /*var: pidx*/
    double* deposit = (this->inputsTPParent->deposit_darts728);
    (void)deposit /*OMP_SHARED*/;
    long* pidx = &(this->inputsTPParent->pidx_darts728[this->getLocalID()]);
    (void)pidx /*OMP_PRIVATE*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations728(
        (long*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        /*Find and signal the next codelet*/
        myTP->controlTPParent->TPParent->barrierCodelets728[0].decDep();
        return;
    }
    for (long pidx_darts_counter_temp728 = (*pidx); pidx_darts_counter_temp728 < endRange
         && pidx_darts_counter_temp728 < this->inputsTPParent->lastIteration728;
         pidx_darts_counter_temp728++) {
        {
            update_part(partArray[(pidx_darts_counter_temp728)], (*(deposit)));
        }
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets728[0].decDep();
}
TP728::TP728(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet,
    long in_initIteration, long in_lastIteration, double* in_deposit)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , deposit_darts728(in_deposit) /*OMP_SHARED - INPUT*/
    , pidx_darts728(new long[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , initIteration728(in_initIteration)
    , lastIteration728(in_lastIteration)
    , barrierCodelets728(new _barrierCodelets728[1])
    , checkInCodelets729(new _checkInCodelets729[this->numThreads])
{
    /*Initialize the loop parameters*/
    range728 = abs(lastIteration728 - initIteration728) / 1;
    rangePerCodelet728 = range728 / numThreads;
    minIteration728 = min<long>(lastIteration728, initIteration728);
    remainderRange728 = range728 % numThreads;
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets728[0] = _barrierCodelets728(ompNumThreads, ompNumThreads, this, 0);
    _checkInCodelets729* checkInCodelets729Ptr = (this->checkInCodelets729);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets729Ptr) = _checkInCodelets729(1, 1, this, codeletCounter);
        (*checkInCodelets729Ptr).decDep();
        checkInCodelets729Ptr++;
    }
}
TP728::~TP728()
{
    delete[] barrierCodelets728;
    delete[] checkInCodelets729;
}
/*TP761: OMPParallelForDirective*/
void TP761::_barrierCodelets761::fire(void)
{
    TP761* myTP = static_cast<TP761*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
bool TP761::requestNewRangeIterations761(long* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Static */
    /*Chunk = 1*/
    bool isThereNewIteration = false;
    {
        /*Static Scheduling*/
        long tempStartRange = rangePerCodelet761 * codeletID;
        long tempEndRange = rangePerCodelet761 * (codeletID + 1);
        if (remainderRange761 != 0) {
            if (codeletID < (uint32_t)remainderRange761) {
                tempStartRange += codeletID;
                tempEndRange += (codeletID + 1);
            } else {
                tempStartRange += remainderRange761;
                tempEndRange += remainderRange761;
            }
        }
        tempStartRange = tempStartRange * 1 + minIteration761;
        tempEndRange = tempEndRange * 1 + minIteration761;
        if (tempStartRange != tempEndRange) {
            isThereNewIteration = true;
        }
        if (initIteration761 < lastIteration761) {
            (this->inputsTPParent->pidx_darts761[codeletID]) = min(tempStartRange, tempEndRange);
            *endRange = max(tempStartRange, tempEndRange);
        } else {
            (this->inputsTPParent->pidx_darts761[codeletID]) = max(tempStartRange, tempEndRange);
            *endRange = min(tempStartRange, tempEndRange);
        }
        if (codeletID == this->numThreads - 1) {
            *endRange = lastIteration761;
        }
    }
    return isThereNewIteration;
}
void TP761::_checkInCodelets762::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 762: ForStmt*/
    /*var: deposit*/
    /*var: pidx*/
    double* deposit = (this->inputsTPParent->deposit_darts761);
    (void)deposit /*OMP_SHARED*/;
    long* pidx = &(this->inputsTPParent->pidx_darts761[this->getLocalID()]);
    (void)pidx /*OMP_PRIVATE*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations761(
        (long*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        /*Find and signal the next codelet*/
        myTP->controlTPParent->TPParent->barrierCodelets761[0].decDep();
        return;
    }
    for (long pidx_darts_counter_temp761 = (*pidx); pidx_darts_counter_temp761 < endRange
         && pidx_darts_counter_temp761 < this->inputsTPParent->lastIteration761;
         pidx_darts_counter_temp761++) {
        {
            update_part(partArray[(pidx_darts_counter_temp761)], (*(deposit)));
        }
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets761[0].decDep();
}
TP761::TP761(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet,
    long in_initIteration, long in_lastIteration, double* in_deposit)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , deposit_darts761(in_deposit) /*OMP_SHARED - INPUT*/
    , pidx_darts761(new long[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , initIteration761(in_initIteration)
    , lastIteration761(in_lastIteration)
    , barrierCodelets761(new _barrierCodelets761[1])
    , checkInCodelets762(new _checkInCodelets762[this->numThreads])
{
    /*Initialize the loop parameters*/
    range761 = abs(lastIteration761 - initIteration761) / 1;
    rangePerCodelet761 = range761 / numThreads;
    minIteration761 = min<long>(lastIteration761, initIteration761);
    remainderRange761 = range761 % numThreads;
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets761[0] = _barrierCodelets761(ompNumThreads, ompNumThreads, this, 0);
    _checkInCodelets762* checkInCodelets762Ptr = (this->checkInCodelets762);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets762Ptr) = _checkInCodelets762(1, 1, this, codeletCounter);
        (*checkInCodelets762Ptr).decDep();
        checkInCodelets762Ptr++;
    }
}
TP761::~TP761()
{
    delete[] barrierCodelets761;
    delete[] checkInCodelets762;
}
/*TP794: OMPParallelForDirective*/
void TP794::_barrierCodelets794::fire(void)
{
    TP794* myTP = static_cast<TP794*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
bool TP794::requestNewRangeIterations794(long* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Static */
    /*Chunk = 1*/
    bool isThereNewIteration = false;
    {
        /*Static Scheduling*/
        long tempStartRange = rangePerCodelet794 * codeletID;
        long tempEndRange = rangePerCodelet794 * (codeletID + 1);
        if (remainderRange794 != 0) {
            if (codeletID < (uint32_t)remainderRange794) {
                tempStartRange += codeletID;
                tempEndRange += (codeletID + 1);
            } else {
                tempStartRange += remainderRange794;
                tempEndRange += remainderRange794;
            }
        }
        tempStartRange = tempStartRange * 1 + minIteration794;
        tempEndRange = tempEndRange * 1 + minIteration794;
        if (tempStartRange != tempEndRange) {
            isThereNewIteration = true;
        }
        if (initIteration794 < lastIteration794) {
            (this->inputsTPParent->pidx_darts794[codeletID]) = min(tempStartRange, tempEndRange);
            *endRange = max(tempStartRange, tempEndRange);
        } else {
            (this->inputsTPParent->pidx_darts794[codeletID]) = max(tempStartRange, tempEndRange);
            *endRange = min(tempStartRange, tempEndRange);
        }
        if (codeletID == this->numThreads - 1) {
            *endRange = lastIteration794;
        }
    }
    return isThereNewIteration;
}
void TP794::_checkInCodelets795::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 795: ForStmt*/
    /*var: deposit*/
    /*var: pidx*/
    double* deposit = (this->inputsTPParent->deposit_darts794);
    (void)deposit /*OMP_SHARED*/;
    long* pidx = &(this->inputsTPParent->pidx_darts794[this->getLocalID()]);
    (void)pidx /*OMP_PRIVATE*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations794(
        (long*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        /*Find and signal the next codelet*/
        myTP->controlTPParent->TPParent->barrierCodelets794[0].decDep();
        return;
    }
    for (long pidx_darts_counter_temp794 = (*pidx); pidx_darts_counter_temp794 < endRange
         && pidx_darts_counter_temp794 < this->inputsTPParent->lastIteration794;
         pidx_darts_counter_temp794++) {
        {
            update_part(partArray[(pidx_darts_counter_temp794)], (*(deposit)));
        }
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets794[0].decDep();
}
TP794::TP794(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet,
    long in_initIteration, long in_lastIteration, double* in_deposit)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , deposit_darts794(in_deposit) /*OMP_SHARED - INPUT*/
    , pidx_darts794(new long[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , initIteration794(in_initIteration)
    , lastIteration794(in_lastIteration)
    , barrierCodelets794(new _barrierCodelets794[1])
    , checkInCodelets795(new _checkInCodelets795[this->numThreads])
{
    /*Initialize the loop parameters*/
    range794 = abs(lastIteration794 - initIteration794) / 1;
    rangePerCodelet794 = range794 / numThreads;
    minIteration794 = min<long>(lastIteration794, initIteration794);
    remainderRange794 = range794 % numThreads;
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets794[0] = _barrierCodelets794(ompNumThreads, ompNumThreads, this, 0);
    _checkInCodelets795* checkInCodelets795Ptr = (this->checkInCodelets795);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets795Ptr) = _checkInCodelets795(1, 1, this, codeletCounter);
        (*checkInCodelets795Ptr).decDep();
        checkInCodelets795Ptr++;
    }
}
TP794::~TP794()
{
    delete[] barrierCodelets794;
    delete[] checkInCodelets795;
}
/*TP830: OMPParallelForDirective*/
void TP830::_barrierCodelets830::fire(void)
{
    TP830* myTP = static_cast<TP830*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
bool TP830::requestNewRangeIterations830(long* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Static */
    /*Chunk = 1*/
    bool isThereNewIteration = false;
    {
        /*Static Scheduling*/
        long tempStartRange = rangePerCodelet830 * codeletID;
        long tempEndRange = rangePerCodelet830 * (codeletID + 1);
        if (remainderRange830 != 0) {
            if (codeletID < (uint32_t)remainderRange830) {
                tempStartRange += codeletID;
                tempEndRange += (codeletID + 1);
            } else {
                tempStartRange += remainderRange830;
                tempEndRange += remainderRange830;
            }
        }
        tempStartRange = tempStartRange * 1 + minIteration830;
        tempEndRange = tempEndRange * 1 + minIteration830;
        if (tempStartRange != tempEndRange) {
            isThereNewIteration = true;
        }
        if (initIteration830 < lastIteration830) {
            (this->inputsTPParent->pidx_darts830[codeletID]) = min(tempStartRange, tempEndRange);
            *endRange = max(tempStartRange, tempEndRange);
        } else {
            (this->inputsTPParent->pidx_darts830[codeletID]) = max(tempStartRange, tempEndRange);
            *endRange = min(tempStartRange, tempEndRange);
        }
        if (codeletID == this->numThreads - 1) {
            *endRange = lastIteration830;
        }
    }
    return isThereNewIteration;
}
void TP830::_checkInCodelets831::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 831: ForStmt*/
    /*var: deposit*/
    /*var: pidx*/
    double* deposit = (this->inputsTPParent->deposit_darts830);
    (void)deposit /*OMP_SHARED*/;
    long* pidx = &(this->inputsTPParent->pidx_darts830[this->getLocalID()]);
    (void)pidx /*OMP_PRIVATE*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations830(
        (long*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        /*Find and signal the next codelet*/
        myTP->controlTPParent->TPParent->barrierCodelets830[0].decDep();
        return;
    }
    for (long pidx_darts_counter_temp830 = (*pidx); pidx_darts_counter_temp830 < endRange
         && pidx_darts_counter_temp830 < this->inputsTPParent->lastIteration830;
         pidx_darts_counter_temp830++) {
        {
            update_part(partArray[(pidx_darts_counter_temp830)], (*(deposit)));
        }
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets830[0].decDep();
}
TP830::TP830(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet,
    long in_initIteration, long in_lastIteration, double* in_deposit)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , deposit_darts830(in_deposit) /*OMP_SHARED - INPUT*/
    , pidx_darts830(new long[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , initIteration830(in_initIteration)
    , lastIteration830(in_lastIteration)
    , barrierCodelets830(new _barrierCodelets830[1])
    , checkInCodelets831(new _checkInCodelets831[this->numThreads])
{
    /*Initialize the loop parameters*/
    range830 = abs(lastIteration830 - initIteration830) / 1;
    rangePerCodelet830 = range830 / numThreads;
    minIteration830 = min<long>(lastIteration830, initIteration830);
    remainderRange830 = range830 % numThreads;
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets830[0] = _barrierCodelets830(ompNumThreads, ompNumThreads, this, 0);
    _checkInCodelets831* checkInCodelets831Ptr = (this->checkInCodelets831);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets831Ptr) = _checkInCodelets831(1, 1, this, codeletCounter);
        (*checkInCodelets831Ptr).decDep();
        checkInCodelets831Ptr++;
    }
}
TP830::~TP830()
{
    delete[] barrierCodelets830;
    delete[] checkInCodelets831;
}
/*TP863: OMPParallelForDirective*/
void TP863::_barrierCodelets863::fire(void)
{
    TP863* myTP = static_cast<TP863*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
bool TP863::requestNewRangeIterations863(long* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Static */
    /*Chunk = 1*/
    bool isThereNewIteration = false;
    {
        /*Static Scheduling*/
        long tempStartRange = rangePerCodelet863 * codeletID;
        long tempEndRange = rangePerCodelet863 * (codeletID + 1);
        if (remainderRange863 != 0) {
            if (codeletID < (uint32_t)remainderRange863) {
                tempStartRange += codeletID;
                tempEndRange += (codeletID + 1);
            } else {
                tempStartRange += remainderRange863;
                tempEndRange += remainderRange863;
            }
        }
        tempStartRange = tempStartRange * 1 + minIteration863;
        tempEndRange = tempEndRange * 1 + minIteration863;
        if (tempStartRange != tempEndRange) {
            isThereNewIteration = true;
        }
        if (initIteration863 < lastIteration863) {
            (this->inputsTPParent->pidx_darts863[codeletID]) = min(tempStartRange, tempEndRange);
            *endRange = max(tempStartRange, tempEndRange);
        } else {
            (this->inputsTPParent->pidx_darts863[codeletID]) = max(tempStartRange, tempEndRange);
            *endRange = min(tempStartRange, tempEndRange);
        }
        if (codeletID == this->numThreads - 1) {
            *endRange = lastIteration863;
        }
    }
    return isThereNewIteration;
}
void TP863::_checkInCodelets864::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 864: ForStmt*/
    /*var: deposit*/
    /*var: pidx*/
    double* deposit = (this->inputsTPParent->deposit_darts863);
    (void)deposit /*OMP_SHARED*/;
    long* pidx = &(this->inputsTPParent->pidx_darts863[this->getLocalID()]);
    (void)pidx /*OMP_PRIVATE*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations863(
        (long*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        /*Find and signal the next codelet*/
        myTP->controlTPParent->TPParent->barrierCodelets863[0].decDep();
        return;
    }
    for (long pidx_darts_counter_temp863 = (*pidx); pidx_darts_counter_temp863 < endRange
         && pidx_darts_counter_temp863 < this->inputsTPParent->lastIteration863;
         pidx_darts_counter_temp863++) {
        {
            update_part(partArray[(pidx_darts_counter_temp863)], (*(deposit)));
        }
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets863[0].decDep();
}
TP863::TP863(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet,
    long in_initIteration, long in_lastIteration, double* in_deposit)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , deposit_darts863(in_deposit) /*OMP_SHARED - INPUT*/
    , pidx_darts863(new long[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , initIteration863(in_initIteration)
    , lastIteration863(in_lastIteration)
    , barrierCodelets863(new _barrierCodelets863[1])
    , checkInCodelets864(new _checkInCodelets864[this->numThreads])
{
    /*Initialize the loop parameters*/
    range863 = abs(lastIteration863 - initIteration863) / 1;
    rangePerCodelet863 = range863 / numThreads;
    minIteration863 = min<long>(lastIteration863, initIteration863);
    remainderRange863 = range863 % numThreads;
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets863[0] = _barrierCodelets863(ompNumThreads, ompNumThreads, this, 0);
    _checkInCodelets864* checkInCodelets864Ptr = (this->checkInCodelets864);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets864Ptr) = _checkInCodelets864(1, 1, this, codeletCounter);
        (*checkInCodelets864Ptr).decDep();
        checkInCodelets864Ptr++;
    }
}
TP863::~TP863()
{
    delete[] barrierCodelets863;
    delete[] checkInCodelets864;
}
/*TP896: OMPParallelForDirective*/
void TP896::_barrierCodelets896::fire(void)
{
    TP896* myTP = static_cast<TP896*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
bool TP896::requestNewRangeIterations896(long* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Static */
    /*Chunk = 1*/
    bool isThereNewIteration = false;
    {
        /*Static Scheduling*/
        long tempStartRange = rangePerCodelet896 * codeletID;
        long tempEndRange = rangePerCodelet896 * (codeletID + 1);
        if (remainderRange896 != 0) {
            if (codeletID < (uint32_t)remainderRange896) {
                tempStartRange += codeletID;
                tempEndRange += (codeletID + 1);
            } else {
                tempStartRange += remainderRange896;
                tempEndRange += remainderRange896;
            }
        }
        tempStartRange = tempStartRange * 1 + minIteration896;
        tempEndRange = tempEndRange * 1 + minIteration896;
        if (tempStartRange != tempEndRange) {
            isThereNewIteration = true;
        }
        if (initIteration896 < lastIteration896) {
            (this->inputsTPParent->pidx_darts896[codeletID]) = min(tempStartRange, tempEndRange);
            *endRange = max(tempStartRange, tempEndRange);
        } else {
            (this->inputsTPParent->pidx_darts896[codeletID]) = max(tempStartRange, tempEndRange);
            *endRange = min(tempStartRange, tempEndRange);
        }
        if (codeletID == this->numThreads - 1) {
            *endRange = lastIteration896;
        }
    }
    return isThereNewIteration;
}
void TP896::_checkInCodelets897::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 897: ForStmt*/
    /*var: deposit*/
    /*var: pidx*/
    double* deposit = (this->inputsTPParent->deposit_darts896);
    (void)deposit /*OMP_SHARED*/;
    long* pidx = &(this->inputsTPParent->pidx_darts896[this->getLocalID()]);
    (void)pidx /*OMP_PRIVATE*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations896(
        (long*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        /*Find and signal the next codelet*/
        myTP->controlTPParent->TPParent->barrierCodelets896[0].decDep();
        return;
    }
    for (long pidx_darts_counter_temp896 = (*pidx); pidx_darts_counter_temp896 < endRange
         && pidx_darts_counter_temp896 < this->inputsTPParent->lastIteration896;
         pidx_darts_counter_temp896++) {
        {
            update_part(partArray[(pidx_darts_counter_temp896)], (*(deposit)));
        }
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets896[0].decDep();
}
TP896::TP896(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet,
    long in_initIteration, long in_lastIteration, double* in_deposit)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , deposit_darts896(in_deposit) /*OMP_SHARED - INPUT*/
    , pidx_darts896(new long[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , initIteration896(in_initIteration)
    , lastIteration896(in_lastIteration)
    , barrierCodelets896(new _barrierCodelets896[1])
    , checkInCodelets897(new _checkInCodelets897[this->numThreads])
{
    /*Initialize the loop parameters*/
    range896 = abs(lastIteration896 - initIteration896) / 1;
    rangePerCodelet896 = range896 / numThreads;
    minIteration896 = min<long>(lastIteration896, initIteration896);
    remainderRange896 = range896 % numThreads;
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets896[0] = _barrierCodelets896(ompNumThreads, ompNumThreads, this, 0);
    _checkInCodelets897* checkInCodelets897Ptr = (this->checkInCodelets897);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets897Ptr) = _checkInCodelets897(1, 1, this, codeletCounter);
        (*checkInCodelets897Ptr).decDep();
        checkInCodelets897Ptr++;
    }
}
TP896::~TP896()
{
    delete[] barrierCodelets896;
    delete[] checkInCodelets897;
}
/*TP929: OMPParallelForDirective*/
void TP929::_barrierCodelets929::fire(void)
{
    TP929* myTP = static_cast<TP929*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
bool TP929::requestNewRangeIterations929(long* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Static */
    /*Chunk = 1*/
    bool isThereNewIteration = false;
    {
        /*Static Scheduling*/
        long tempStartRange = rangePerCodelet929 * codeletID;
        long tempEndRange = rangePerCodelet929 * (codeletID + 1);
        if (remainderRange929 != 0) {
            if (codeletID < (uint32_t)remainderRange929) {
                tempStartRange += codeletID;
                tempEndRange += (codeletID + 1);
            } else {
                tempStartRange += remainderRange929;
                tempEndRange += remainderRange929;
            }
        }
        tempStartRange = tempStartRange * 1 + minIteration929;
        tempEndRange = tempEndRange * 1 + minIteration929;
        if (tempStartRange != tempEndRange) {
            isThereNewIteration = true;
        }
        if (initIteration929 < lastIteration929) {
            (this->inputsTPParent->pidx_darts929[codeletID]) = min(tempStartRange, tempEndRange);
            *endRange = max(tempStartRange, tempEndRange);
        } else {
            (this->inputsTPParent->pidx_darts929[codeletID]) = max(tempStartRange, tempEndRange);
            *endRange = min(tempStartRange, tempEndRange);
        }
        if (codeletID == this->numThreads - 1) {
            *endRange = lastIteration929;
        }
    }
    return isThereNewIteration;
}
void TP929::_checkInCodelets930::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 930: ForStmt*/
    /*var: deposit*/
    /*var: pidx*/
    double* deposit = (this->inputsTPParent->deposit_darts929);
    (void)deposit /*OMP_SHARED*/;
    long* pidx = &(this->inputsTPParent->pidx_darts929[this->getLocalID()]);
    (void)pidx /*OMP_PRIVATE*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations929(
        (long*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        /*Find and signal the next codelet*/
        myTP->controlTPParent->TPParent->barrierCodelets929[0].decDep();
        return;
    }
    for (long pidx_darts_counter_temp929 = (*pidx); pidx_darts_counter_temp929 < endRange
         && pidx_darts_counter_temp929 < this->inputsTPParent->lastIteration929;
         pidx_darts_counter_temp929++) {
        {
            update_part(partArray[(pidx_darts_counter_temp929)], (*(deposit)));
        }
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets929[0].decDep();
}
TP929::TP929(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet,
    long in_initIteration, long in_lastIteration, double* in_deposit)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , deposit_darts929(in_deposit) /*OMP_SHARED - INPUT*/
    , pidx_darts929(new long[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , initIteration929(in_initIteration)
    , lastIteration929(in_lastIteration)
    , barrierCodelets929(new _barrierCodelets929[1])
    , checkInCodelets930(new _checkInCodelets930[this->numThreads])
{
    /*Initialize the loop parameters*/
    range929 = abs(lastIteration929 - initIteration929) / 1;
    rangePerCodelet929 = range929 / numThreads;
    minIteration929 = min<long>(lastIteration929, initIteration929);
    remainderRange929 = range929 % numThreads;
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets929[0] = _barrierCodelets929(ompNumThreads, ompNumThreads, this, 0);
    _checkInCodelets930* checkInCodelets930Ptr = (this->checkInCodelets930);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets930Ptr) = _checkInCodelets930(1, 1, this, codeletCounter);
        (*checkInCodelets930Ptr).decDep();
        checkInCodelets930Ptr++;
    }
}
TP929::~TP929()
{
    delete[] barrierCodelets929;
    delete[] checkInCodelets930;
}
/*TP978: OMPParallelForDirective*/
void TP978::_barrierCodelets978::fire(void)
{
    TP978* myTP = static_cast<TP978*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
bool TP978::requestNewRangeIterations978(long* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Dynamic */
    /*Chunk = 1*/
    bool isThereNewIteration = false;
    {
        /*Dynamic Scheduling*/
        if (initIteration978 < lastIteration978) {
            (this->inputsTPParent->pidx_darts978[codeletID])
                = __sync_fetch_and_add(&(nextIteration978), 1 * 1);
            *endRange = (this->inputsTPParent->pidx_darts978[codeletID]) + 1 * 1;
        } else {
            (this->inputsTPParent->pidx_darts978[codeletID])
                = __sync_fetch_and_sub(&(nextIteration978), 1 * 1);
            *endRange = (this->inputsTPParent->pidx_darts978[codeletID]) - 1 * 1;
        }
        if ((this->inputsTPParent->pidx_darts978[codeletID]) < lastIteration978) {
            isThereNewIteration = true;
        }
    }
    return isThereNewIteration;
}
void TP978::_checkInCodelets979::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 979: ForStmt*/
    /*var: deposit*/
    /*var: pidx*/
    double* deposit = (this->inputsTPParent->deposit_darts978);
    (void)deposit /*OMP_SHARED*/;
    long* pidx = &(this->inputsTPParent->pidx_darts978[this->getLocalID()]);
    (void)pidx /*OMP_PRIVATE*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations978(
        (long*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        /*Find and signal the next codelet*/
        myTP->controlTPParent->TPParent->barrierCodelets978[0].decDep();
        return;
    }
    while (isThereNewIteration) {
        for (long pidx_darts_counter_temp978 = (*pidx); pidx_darts_counter_temp978 < endRange
             && pidx_darts_counter_temp978 < this->inputsTPParent->lastIteration978;
             pidx_darts_counter_temp978++) {
            {
                update_part(partArray[(pidx_darts_counter_temp978)], (*(deposit)));
            }
        }
        isThereNewIteration = this->inputsTPParent->requestNewRangeIterations978(
            &(this->endRange), this->getLocalID());
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets978[0].decDep();
}
TP978::TP978(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet,
    long in_initIteration, long in_lastIteration, double* in_deposit)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , deposit_darts978(in_deposit) /*OMP_SHARED - INPUT*/
    , pidx_darts978(new long[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , initIteration978(in_initIteration)
    , lastIteration978(in_lastIteration)
    , nextIteration978(INT_MAX)
    , loop978alreadySetUp(0)
    , barrierCodelets978(new _barrierCodelets978[1])
    , checkInCodelets979(new _checkInCodelets979[this->numThreads])
{
    /*Initialize the loop parameters*/
    nextIteration978 = initIteration978;
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets978[0] = _barrierCodelets978(ompNumThreads, ompNumThreads, this, 0);
    _checkInCodelets979* checkInCodelets979Ptr = (this->checkInCodelets979);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets979Ptr) = _checkInCodelets979(1, 1, this, codeletCounter);
        (*checkInCodelets979Ptr).decDep();
        checkInCodelets979Ptr++;
    }
}
TP978::~TP978()
{
    delete[] barrierCodelets978;
    delete[] checkInCodelets979;
}
/*TP1014: OMPParallelForDirective*/
void TP1014::_barrierCodelets1014::fire(void)
{
    TP1014* myTP = static_cast<TP1014*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
bool TP1014::requestNewRangeIterations1014(long* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Dynamic */
    /*Chunk = 1*/
    bool isThereNewIteration = false;
    {
        /*Dynamic Scheduling*/
        if (initIteration1014 < lastIteration1014) {
            (this->inputsTPParent->pidx_darts1014[codeletID])
                = __sync_fetch_and_add(&(nextIteration1014), 1 * 1);
            *endRange = (this->inputsTPParent->pidx_darts1014[codeletID]) + 1 * 1;
        } else {
            (this->inputsTPParent->pidx_darts1014[codeletID])
                = __sync_fetch_and_sub(&(nextIteration1014), 1 * 1);
            *endRange = (this->inputsTPParent->pidx_darts1014[codeletID]) - 1 * 1;
        }
        if ((this->inputsTPParent->pidx_darts1014[codeletID]) < lastIteration1014) {
            isThereNewIteration = true;
        }
    }
    return isThereNewIteration;
}
void TP1014::_checkInCodelets1015::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 1015: ForStmt*/
    /*var: deposit*/
    /*var: pidx*/
    double* deposit = (this->inputsTPParent->deposit_darts1014);
    (void)deposit /*OMP_SHARED*/;
    long* pidx = &(this->inputsTPParent->pidx_darts1014[this->getLocalID()]);
    (void)pidx /*OMP_PRIVATE*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations1014(
        (long*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        /*Find and signal the next codelet*/
        myTP->controlTPParent->TPParent->barrierCodelets1014[0].decDep();
        return;
    }
    while (isThereNewIteration) {
        for (long pidx_darts_counter_temp1014 = (*pidx); pidx_darts_counter_temp1014 < endRange
             && pidx_darts_counter_temp1014 < this->inputsTPParent->lastIteration1014;
             pidx_darts_counter_temp1014++) {
            {
                update_part(partArray[(pidx_darts_counter_temp1014)], (*(deposit)));
            }
        }
        isThereNewIteration = this->inputsTPParent->requestNewRangeIterations1014(
            &(this->endRange), this->getLocalID());
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets1014[0].decDep();
}
TP1014::TP1014(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet,
    long in_initIteration, long in_lastIteration, double* in_deposit)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , deposit_darts1014(in_deposit) /*OMP_SHARED - INPUT*/
    , pidx_darts1014(new long[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , initIteration1014(in_initIteration)
    , lastIteration1014(in_lastIteration)
    , nextIteration1014(INT_MAX)
    , loop1014alreadySetUp(0)
    , barrierCodelets1014(new _barrierCodelets1014[1])
    , checkInCodelets1015(new _checkInCodelets1015[this->numThreads])
{
    /*Initialize the loop parameters*/
    nextIteration1014 = initIteration1014;
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets1014[0] = _barrierCodelets1014(ompNumThreads, ompNumThreads, this, 0);
    _checkInCodelets1015* checkInCodelets1015Ptr = (this->checkInCodelets1015);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets1015Ptr) = _checkInCodelets1015(1, 1, this, codeletCounter);
        (*checkInCodelets1015Ptr).decDep();
        checkInCodelets1015Ptr++;
    }
}
TP1014::~TP1014()
{
    delete[] barrierCodelets1014;
    delete[] checkInCodelets1015;
}
/*TP1047: OMPParallelForDirective*/
void TP1047::_barrierCodelets1047::fire(void)
{
    TP1047* myTP = static_cast<TP1047*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
bool TP1047::requestNewRangeIterations1047(long* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Dynamic */
    /*Chunk = 1*/
    bool isThereNewIteration = false;
    {
        /*Dynamic Scheduling*/
        if (initIteration1047 < lastIteration1047) {
            (this->inputsTPParent->pidx_darts1047[codeletID])
                = __sync_fetch_and_add(&(nextIteration1047), 1 * 1);
            *endRange = (this->inputsTPParent->pidx_darts1047[codeletID]) + 1 * 1;
        } else {
            (this->inputsTPParent->pidx_darts1047[codeletID])
                = __sync_fetch_and_sub(&(nextIteration1047), 1 * 1);
            *endRange = (this->inputsTPParent->pidx_darts1047[codeletID]) - 1 * 1;
        }
        if ((this->inputsTPParent->pidx_darts1047[codeletID]) < lastIteration1047) {
            isThereNewIteration = true;
        }
    }
    return isThereNewIteration;
}
void TP1047::_checkInCodelets1048::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 1048: ForStmt*/
    /*var: deposit*/
    /*var: pidx*/
    double* deposit = (this->inputsTPParent->deposit_darts1047);
    (void)deposit /*OMP_SHARED*/;
    long* pidx = &(this->inputsTPParent->pidx_darts1047[this->getLocalID()]);
    (void)pidx /*OMP_PRIVATE*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations1047(
        (long*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        /*Find and signal the next codelet*/
        myTP->controlTPParent->TPParent->barrierCodelets1047[0].decDep();
        return;
    }
    while (isThereNewIteration) {
        for (long pidx_darts_counter_temp1047 = (*pidx); pidx_darts_counter_temp1047 < endRange
             && pidx_darts_counter_temp1047 < this->inputsTPParent->lastIteration1047;
             pidx_darts_counter_temp1047++) {
            {
                update_part(partArray[(pidx_darts_counter_temp1047)], (*(deposit)));
            }
        }
        isThereNewIteration = this->inputsTPParent->requestNewRangeIterations1047(
            &(this->endRange), this->getLocalID());
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets1047[0].decDep();
}
TP1047::TP1047(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet,
    long in_initIteration, long in_lastIteration, double* in_deposit)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , deposit_darts1047(in_deposit) /*OMP_SHARED - INPUT*/
    , pidx_darts1047(new long[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , initIteration1047(in_initIteration)
    , lastIteration1047(in_lastIteration)
    , nextIteration1047(INT_MAX)
    , loop1047alreadySetUp(0)
    , barrierCodelets1047(new _barrierCodelets1047[1])
    , checkInCodelets1048(new _checkInCodelets1048[this->numThreads])
{
    /*Initialize the loop parameters*/
    nextIteration1047 = initIteration1047;
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets1047[0] = _barrierCodelets1047(ompNumThreads, ompNumThreads, this, 0);
    _checkInCodelets1048* checkInCodelets1048Ptr = (this->checkInCodelets1048);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets1048Ptr) = _checkInCodelets1048(1, 1, this, codeletCounter);
        (*checkInCodelets1048Ptr).decDep();
        checkInCodelets1048Ptr++;
    }
}
TP1047::~TP1047()
{
    delete[] barrierCodelets1047;
    delete[] checkInCodelets1048;
}
/*TP1083: OMPParallelForDirective*/
void TP1083::_barrierCodelets1083::fire(void)
{
    TP1083* myTP = static_cast<TP1083*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
bool TP1083::requestNewRangeIterations1083(long* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Dynamic */
    /*Chunk = 1*/
    bool isThereNewIteration = false;
    {
        /*Dynamic Scheduling*/
        if (initIteration1083 < lastIteration1083) {
            (this->inputsTPParent->pidx_darts1083[codeletID])
                = __sync_fetch_and_add(&(nextIteration1083), 1 * 1);
            *endRange = (this->inputsTPParent->pidx_darts1083[codeletID]) + 1 * 1;
        } else {
            (this->inputsTPParent->pidx_darts1083[codeletID])
                = __sync_fetch_and_sub(&(nextIteration1083), 1 * 1);
            *endRange = (this->inputsTPParent->pidx_darts1083[codeletID]) - 1 * 1;
        }
        if ((this->inputsTPParent->pidx_darts1083[codeletID]) < lastIteration1083) {
            isThereNewIteration = true;
        }
    }
    return isThereNewIteration;
}
void TP1083::_checkInCodelets1084::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 1084: ForStmt*/
    /*var: deposit*/
    /*var: pidx*/
    double* deposit = (this->inputsTPParent->deposit_darts1083);
    (void)deposit /*OMP_SHARED*/;
    long* pidx = &(this->inputsTPParent->pidx_darts1083[this->getLocalID()]);
    (void)pidx /*OMP_PRIVATE*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations1083(
        (long*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        /*Find and signal the next codelet*/
        myTP->controlTPParent->TPParent->barrierCodelets1083[0].decDep();
        return;
    }
    while (isThereNewIteration) {
        for (long pidx_darts_counter_temp1083 = (*pidx); pidx_darts_counter_temp1083 < endRange
             && pidx_darts_counter_temp1083 < this->inputsTPParent->lastIteration1083;
             pidx_darts_counter_temp1083++) {
            {
                update_part(partArray[(pidx_darts_counter_temp1083)], (*(deposit)));
            }
        }
        isThereNewIteration = this->inputsTPParent->requestNewRangeIterations1083(
            &(this->endRange), this->getLocalID());
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets1083[0].decDep();
}
TP1083::TP1083(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet,
    long in_initIteration, long in_lastIteration, double* in_deposit)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , deposit_darts1083(in_deposit) /*OMP_SHARED - INPUT*/
    , pidx_darts1083(new long[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , initIteration1083(in_initIteration)
    , lastIteration1083(in_lastIteration)
    , nextIteration1083(INT_MAX)
    , loop1083alreadySetUp(0)
    , barrierCodelets1083(new _barrierCodelets1083[1])
    , checkInCodelets1084(new _checkInCodelets1084[this->numThreads])
{
    /*Initialize the loop parameters*/
    nextIteration1083 = initIteration1083;
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets1083[0] = _barrierCodelets1083(ompNumThreads, ompNumThreads, this, 0);
    _checkInCodelets1084* checkInCodelets1084Ptr = (this->checkInCodelets1084);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets1084Ptr) = _checkInCodelets1084(1, 1, this, codeletCounter);
        (*checkInCodelets1084Ptr).decDep();
        checkInCodelets1084Ptr++;
    }
}
TP1083::~TP1083()
{
    delete[] barrierCodelets1083;
    delete[] checkInCodelets1084;
}
/*TP1116: OMPParallelForDirective*/
void TP1116::_barrierCodelets1116::fire(void)
{
    TP1116* myTP = static_cast<TP1116*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
bool TP1116::requestNewRangeIterations1116(long* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Dynamic */
    /*Chunk = 1*/
    bool isThereNewIteration = false;
    {
        /*Dynamic Scheduling*/
        if (initIteration1116 < lastIteration1116) {
            (this->inputsTPParent->pidx_darts1116[codeletID])
                = __sync_fetch_and_add(&(nextIteration1116), 1 * 1);
            *endRange = (this->inputsTPParent->pidx_darts1116[codeletID]) + 1 * 1;
        } else {
            (this->inputsTPParent->pidx_darts1116[codeletID])
                = __sync_fetch_and_sub(&(nextIteration1116), 1 * 1);
            *endRange = (this->inputsTPParent->pidx_darts1116[codeletID]) - 1 * 1;
        }
        if ((this->inputsTPParent->pidx_darts1116[codeletID]) < lastIteration1116) {
            isThereNewIteration = true;
        }
    }
    return isThereNewIteration;
}
void TP1116::_checkInCodelets1117::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 1117: ForStmt*/
    /*var: deposit*/
    /*var: pidx*/
    double* deposit = (this->inputsTPParent->deposit_darts1116);
    (void)deposit /*OMP_SHARED*/;
    long* pidx = &(this->inputsTPParent->pidx_darts1116[this->getLocalID()]);
    (void)pidx /*OMP_PRIVATE*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations1116(
        (long*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        /*Find and signal the next codelet*/
        myTP->controlTPParent->TPParent->barrierCodelets1116[0].decDep();
        return;
    }
    while (isThereNewIteration) {
        for (long pidx_darts_counter_temp1116 = (*pidx); pidx_darts_counter_temp1116 < endRange
             && pidx_darts_counter_temp1116 < this->inputsTPParent->lastIteration1116;
             pidx_darts_counter_temp1116++) {
            {
                update_part(partArray[(pidx_darts_counter_temp1116)], (*(deposit)));
            }
        }
        isThereNewIteration = this->inputsTPParent->requestNewRangeIterations1116(
            &(this->endRange), this->getLocalID());
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets1116[0].decDep();
}
TP1116::TP1116(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet,
    long in_initIteration, long in_lastIteration, double* in_deposit)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , deposit_darts1116(in_deposit) /*OMP_SHARED - INPUT*/
    , pidx_darts1116(new long[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , initIteration1116(in_initIteration)
    , lastIteration1116(in_lastIteration)
    , nextIteration1116(INT_MAX)
    , loop1116alreadySetUp(0)
    , barrierCodelets1116(new _barrierCodelets1116[1])
    , checkInCodelets1117(new _checkInCodelets1117[this->numThreads])
{
    /*Initialize the loop parameters*/
    nextIteration1116 = initIteration1116;
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets1116[0] = _barrierCodelets1116(ompNumThreads, ompNumThreads, this, 0);
    _checkInCodelets1117* checkInCodelets1117Ptr = (this->checkInCodelets1117);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets1117Ptr) = _checkInCodelets1117(1, 1, this, codeletCounter);
        (*checkInCodelets1117Ptr).decDep();
        checkInCodelets1117Ptr++;
    }
}
TP1116::~TP1116()
{
    delete[] barrierCodelets1116;
    delete[] checkInCodelets1117;
}
/*TP1149: OMPParallelForDirective*/
void TP1149::_barrierCodelets1149::fire(void)
{
    TP1149* myTP = static_cast<TP1149*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
bool TP1149::requestNewRangeIterations1149(long* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Dynamic */
    /*Chunk = 1*/
    bool isThereNewIteration = false;
    {
        /*Dynamic Scheduling*/
        if (initIteration1149 < lastIteration1149) {
            (this->inputsTPParent->pidx_darts1149[codeletID])
                = __sync_fetch_and_add(&(nextIteration1149), 1 * 1);
            *endRange = (this->inputsTPParent->pidx_darts1149[codeletID]) + 1 * 1;
        } else {
            (this->inputsTPParent->pidx_darts1149[codeletID])
                = __sync_fetch_and_sub(&(nextIteration1149), 1 * 1);
            *endRange = (this->inputsTPParent->pidx_darts1149[codeletID]) - 1 * 1;
        }
        if ((this->inputsTPParent->pidx_darts1149[codeletID]) < lastIteration1149) {
            isThereNewIteration = true;
        }
    }
    return isThereNewIteration;
}
void TP1149::_checkInCodelets1150::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 1150: ForStmt*/
    /*var: deposit*/
    /*var: pidx*/
    double* deposit = (this->inputsTPParent->deposit_darts1149);
    (void)deposit /*OMP_SHARED*/;
    long* pidx = &(this->inputsTPParent->pidx_darts1149[this->getLocalID()]);
    (void)pidx /*OMP_PRIVATE*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations1149(
        (long*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        /*Find and signal the next codelet*/
        myTP->controlTPParent->TPParent->barrierCodelets1149[0].decDep();
        return;
    }
    while (isThereNewIteration) {
        for (long pidx_darts_counter_temp1149 = (*pidx); pidx_darts_counter_temp1149 < endRange
             && pidx_darts_counter_temp1149 < this->inputsTPParent->lastIteration1149;
             pidx_darts_counter_temp1149++) {
            {
                update_part(partArray[(pidx_darts_counter_temp1149)], (*(deposit)));
            }
        }
        isThereNewIteration = this->inputsTPParent->requestNewRangeIterations1149(
            &(this->endRange), this->getLocalID());
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets1149[0].decDep();
}
TP1149::TP1149(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet,
    long in_initIteration, long in_lastIteration, double* in_deposit)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , deposit_darts1149(in_deposit) /*OMP_SHARED - INPUT*/
    , pidx_darts1149(new long[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , initIteration1149(in_initIteration)
    , lastIteration1149(in_lastIteration)
    , nextIteration1149(INT_MAX)
    , loop1149alreadySetUp(0)
    , barrierCodelets1149(new _barrierCodelets1149[1])
    , checkInCodelets1150(new _checkInCodelets1150[this->numThreads])
{
    /*Initialize the loop parameters*/
    nextIteration1149 = initIteration1149;
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets1149[0] = _barrierCodelets1149(ompNumThreads, ompNumThreads, this, 0);
    _checkInCodelets1150* checkInCodelets1150Ptr = (this->checkInCodelets1150);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets1150Ptr) = _checkInCodelets1150(1, 1, this, codeletCounter);
        (*checkInCodelets1150Ptr).decDep();
        checkInCodelets1150Ptr++;
    }
}
TP1149::~TP1149()
{
    delete[] barrierCodelets1149;
    delete[] checkInCodelets1150;
}
/*TP1185: OMPParallelForDirective*/
void TP1185::_barrierCodelets1185::fire(void)
{
    TP1185* myTP = static_cast<TP1185*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
bool TP1185::requestNewRangeIterations1185(long* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Dynamic */
    /*Chunk = 1*/
    bool isThereNewIteration = false;
    {
        /*Dynamic Scheduling*/
        if (initIteration1185 < lastIteration1185) {
            (this->inputsTPParent->pidx_darts1185[codeletID])
                = __sync_fetch_and_add(&(nextIteration1185), 1 * 1);
            *endRange = (this->inputsTPParent->pidx_darts1185[codeletID]) + 1 * 1;
        } else {
            (this->inputsTPParent->pidx_darts1185[codeletID])
                = __sync_fetch_and_sub(&(nextIteration1185), 1 * 1);
            *endRange = (this->inputsTPParent->pidx_darts1185[codeletID]) - 1 * 1;
        }
        if ((this->inputsTPParent->pidx_darts1185[codeletID]) < lastIteration1185) {
            isThereNewIteration = true;
        }
    }
    return isThereNewIteration;
}
void TP1185::_checkInCodelets1186::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 1186: ForStmt*/
    /*var: deposit*/
    /*var: pidx*/
    double* deposit = (this->inputsTPParent->deposit_darts1185);
    (void)deposit /*OMP_SHARED*/;
    long* pidx = &(this->inputsTPParent->pidx_darts1185[this->getLocalID()]);
    (void)pidx /*OMP_PRIVATE*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations1185(
        (long*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        /*Find and signal the next codelet*/
        myTP->controlTPParent->TPParent->barrierCodelets1185[0].decDep();
        return;
    }
    while (isThereNewIteration) {
        for (long pidx_darts_counter_temp1185 = (*pidx); pidx_darts_counter_temp1185 < endRange
             && pidx_darts_counter_temp1185 < this->inputsTPParent->lastIteration1185;
             pidx_darts_counter_temp1185++) {
            {
                update_part(partArray[(pidx_darts_counter_temp1185)], (*(deposit)));
            }
        }
        isThereNewIteration = this->inputsTPParent->requestNewRangeIterations1185(
            &(this->endRange), this->getLocalID());
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets1185[0].decDep();
}
TP1185::TP1185(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet,
    long in_initIteration, long in_lastIteration, double* in_deposit)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , deposit_darts1185(in_deposit) /*OMP_SHARED - INPUT*/
    , pidx_darts1185(new long[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , initIteration1185(in_initIteration)
    , lastIteration1185(in_lastIteration)
    , nextIteration1185(INT_MAX)
    , loop1185alreadySetUp(0)
    , barrierCodelets1185(new _barrierCodelets1185[1])
    , checkInCodelets1186(new _checkInCodelets1186[this->numThreads])
{
    /*Initialize the loop parameters*/
    nextIteration1185 = initIteration1185;
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets1185[0] = _barrierCodelets1185(ompNumThreads, ompNumThreads, this, 0);
    _checkInCodelets1186* checkInCodelets1186Ptr = (this->checkInCodelets1186);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets1186Ptr) = _checkInCodelets1186(1, 1, this, codeletCounter);
        (*checkInCodelets1186Ptr).decDep();
        checkInCodelets1186Ptr++;
    }
}
TP1185::~TP1185()
{
    delete[] barrierCodelets1185;
    delete[] checkInCodelets1186;
}
/*TP1218: OMPParallelForDirective*/
void TP1218::_barrierCodelets1218::fire(void)
{
    TP1218* myTP = static_cast<TP1218*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
bool TP1218::requestNewRangeIterations1218(long* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Dynamic */
    /*Chunk = 1*/
    bool isThereNewIteration = false;
    {
        /*Dynamic Scheduling*/
        if (initIteration1218 < lastIteration1218) {
            (this->inputsTPParent->pidx_darts1218[codeletID])
                = __sync_fetch_and_add(&(nextIteration1218), 1 * 1);
            *endRange = (this->inputsTPParent->pidx_darts1218[codeletID]) + 1 * 1;
        } else {
            (this->inputsTPParent->pidx_darts1218[codeletID])
                = __sync_fetch_and_sub(&(nextIteration1218), 1 * 1);
            *endRange = (this->inputsTPParent->pidx_darts1218[codeletID]) - 1 * 1;
        }
        if ((this->inputsTPParent->pidx_darts1218[codeletID]) < lastIteration1218) {
            isThereNewIteration = true;
        }
    }
    return isThereNewIteration;
}
void TP1218::_checkInCodelets1219::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 1219: ForStmt*/
    /*var: deposit*/
    /*var: pidx*/
    double* deposit = (this->inputsTPParent->deposit_darts1218);
    (void)deposit /*OMP_SHARED*/;
    long* pidx = &(this->inputsTPParent->pidx_darts1218[this->getLocalID()]);
    (void)pidx /*OMP_PRIVATE*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations1218(
        (long*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        /*Find and signal the next codelet*/
        myTP->controlTPParent->TPParent->barrierCodelets1218[0].decDep();
        return;
    }
    while (isThereNewIteration) {
        for (long pidx_darts_counter_temp1218 = (*pidx); pidx_darts_counter_temp1218 < endRange
             && pidx_darts_counter_temp1218 < this->inputsTPParent->lastIteration1218;
             pidx_darts_counter_temp1218++) {
            {
                update_part(partArray[(pidx_darts_counter_temp1218)], (*(deposit)));
            }
        }
        isThereNewIteration = this->inputsTPParent->requestNewRangeIterations1218(
            &(this->endRange), this->getLocalID());
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets1218[0].decDep();
}
TP1218::TP1218(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet,
    long in_initIteration, long in_lastIteration, double* in_deposit)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , deposit_darts1218(in_deposit) /*OMP_SHARED - INPUT*/
    , pidx_darts1218(new long[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , initIteration1218(in_initIteration)
    , lastIteration1218(in_lastIteration)
    , nextIteration1218(INT_MAX)
    , loop1218alreadySetUp(0)
    , barrierCodelets1218(new _barrierCodelets1218[1])
    , checkInCodelets1219(new _checkInCodelets1219[this->numThreads])
{
    /*Initialize the loop parameters*/
    nextIteration1218 = initIteration1218;
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets1218[0] = _barrierCodelets1218(ompNumThreads, ompNumThreads, this, 0);
    _checkInCodelets1219* checkInCodelets1219Ptr = (this->checkInCodelets1219);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets1219Ptr) = _checkInCodelets1219(1, 1, this, codeletCounter);
        (*checkInCodelets1219Ptr).decDep();
        checkInCodelets1219Ptr++;
    }
}
TP1218::~TP1218()
{
    delete[] barrierCodelets1218;
    delete[] checkInCodelets1219;
}
/*TP1251: OMPParallelForDirective*/
void TP1251::_barrierCodelets1251::fire(void)
{
    TP1251* myTP = static_cast<TP1251*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
bool TP1251::requestNewRangeIterations1251(long* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Dynamic */
    /*Chunk = 1*/
    bool isThereNewIteration = false;
    {
        /*Dynamic Scheduling*/
        if (initIteration1251 < lastIteration1251) {
            (this->inputsTPParent->pidx_darts1251[codeletID])
                = __sync_fetch_and_add(&(nextIteration1251), 1 * 1);
            *endRange = (this->inputsTPParent->pidx_darts1251[codeletID]) + 1 * 1;
        } else {
            (this->inputsTPParent->pidx_darts1251[codeletID])
                = __sync_fetch_and_sub(&(nextIteration1251), 1 * 1);
            *endRange = (this->inputsTPParent->pidx_darts1251[codeletID]) - 1 * 1;
        }
        if ((this->inputsTPParent->pidx_darts1251[codeletID]) < lastIteration1251) {
            isThereNewIteration = true;
        }
    }
    return isThereNewIteration;
}
void TP1251::_checkInCodelets1252::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 1252: ForStmt*/
    /*var: deposit*/
    /*var: pidx*/
    double* deposit = (this->inputsTPParent->deposit_darts1251);
    (void)deposit /*OMP_SHARED*/;
    long* pidx = &(this->inputsTPParent->pidx_darts1251[this->getLocalID()]);
    (void)pidx /*OMP_PRIVATE*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations1251(
        (long*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        /*Find and signal the next codelet*/
        myTP->controlTPParent->TPParent->barrierCodelets1251[0].decDep();
        return;
    }
    while (isThereNewIteration) {
        for (long pidx_darts_counter_temp1251 = (*pidx); pidx_darts_counter_temp1251 < endRange
             && pidx_darts_counter_temp1251 < this->inputsTPParent->lastIteration1251;
             pidx_darts_counter_temp1251++) {
            {
                update_part(partArray[(pidx_darts_counter_temp1251)], (*(deposit)));
            }
        }
        isThereNewIteration = this->inputsTPParent->requestNewRangeIterations1251(
            &(this->endRange), this->getLocalID());
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets1251[0].decDep();
}
TP1251::TP1251(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet,
    long in_initIteration, long in_lastIteration, double* in_deposit)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , deposit_darts1251(in_deposit) /*OMP_SHARED - INPUT*/
    , pidx_darts1251(new long[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , initIteration1251(in_initIteration)
    , lastIteration1251(in_lastIteration)
    , nextIteration1251(INT_MAX)
    , loop1251alreadySetUp(0)
    , barrierCodelets1251(new _barrierCodelets1251[1])
    , checkInCodelets1252(new _checkInCodelets1252[this->numThreads])
{
    /*Initialize the loop parameters*/
    nextIteration1251 = initIteration1251;
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets1251[0] = _barrierCodelets1251(ompNumThreads, ompNumThreads, this, 0);
    _checkInCodelets1252* checkInCodelets1252Ptr = (this->checkInCodelets1252);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets1252Ptr) = _checkInCodelets1252(1, 1, this, codeletCounter);
        (*checkInCodelets1252Ptr).decDep();
        checkInCodelets1252Ptr++;
    }
}
TP1251::~TP1251()
{
    delete[] barrierCodelets1251;
    delete[] checkInCodelets1252;
}
/*TP1284: OMPParallelForDirective*/
void TP1284::_barrierCodelets1284::fire(void)
{
    TP1284* myTP = static_cast<TP1284*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
bool TP1284::requestNewRangeIterations1284(long* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Dynamic */
    /*Chunk = 1*/
    bool isThereNewIteration = false;
    {
        /*Dynamic Scheduling*/
        if (initIteration1284 < lastIteration1284) {
            (this->inputsTPParent->pidx_darts1284[codeletID])
                = __sync_fetch_and_add(&(nextIteration1284), 1 * 1);
            *endRange = (this->inputsTPParent->pidx_darts1284[codeletID]) + 1 * 1;
        } else {
            (this->inputsTPParent->pidx_darts1284[codeletID])
                = __sync_fetch_and_sub(&(nextIteration1284), 1 * 1);
            *endRange = (this->inputsTPParent->pidx_darts1284[codeletID]) - 1 * 1;
        }
        if ((this->inputsTPParent->pidx_darts1284[codeletID]) < lastIteration1284) {
            isThereNewIteration = true;
        }
    }
    return isThereNewIteration;
}
void TP1284::_checkInCodelets1285::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 1285: ForStmt*/
    /*var: deposit*/
    /*var: pidx*/
    double* deposit = (this->inputsTPParent->deposit_darts1284);
    (void)deposit /*OMP_SHARED*/;
    long* pidx = &(this->inputsTPParent->pidx_darts1284[this->getLocalID()]);
    (void)pidx /*OMP_PRIVATE*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations1284(
        (long*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        /*Find and signal the next codelet*/
        myTP->controlTPParent->TPParent->barrierCodelets1284[0].decDep();
        return;
    }
    while (isThereNewIteration) {
        for (long pidx_darts_counter_temp1284 = (*pidx); pidx_darts_counter_temp1284 < endRange
             && pidx_darts_counter_temp1284 < this->inputsTPParent->lastIteration1284;
             pidx_darts_counter_temp1284++) {
            {
                update_part(partArray[(pidx_darts_counter_temp1284)], (*(deposit)));
            }
        }
        isThereNewIteration = this->inputsTPParent->requestNewRangeIterations1284(
            &(this->endRange), this->getLocalID());
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets1284[0].decDep();
}
TP1284::TP1284(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet,
    long in_initIteration, long in_lastIteration, double* in_deposit)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , deposit_darts1284(in_deposit) /*OMP_SHARED - INPUT*/
    , pidx_darts1284(new long[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , initIteration1284(in_initIteration)
    , lastIteration1284(in_lastIteration)
    , nextIteration1284(INT_MAX)
    , loop1284alreadySetUp(0)
    , barrierCodelets1284(new _barrierCodelets1284[1])
    , checkInCodelets1285(new _checkInCodelets1285[this->numThreads])
{
    /*Initialize the loop parameters*/
    nextIteration1284 = initIteration1284;
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets1284[0] = _barrierCodelets1284(ompNumThreads, ompNumThreads, this, 0);
    _checkInCodelets1285* checkInCodelets1285Ptr = (this->checkInCodelets1285);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets1285Ptr) = _checkInCodelets1285(1, 1, this, codeletCounter);
        (*checkInCodelets1285Ptr).decDep();
        checkInCodelets1285Ptr++;
    }
}
TP1284::~TP1284()
{
    delete[] barrierCodelets1284;
    delete[] checkInCodelets1285;
}
/*TP31: TP_manual_omp_module1*/
void TP31::_checkInCodelets1331::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    myTP->barrierCodelets1331[0].decDep();
}
void TP31::_barrierCodelets1331::fire(void)
{
    TP31* myTP = static_cast<TP31*>(myTP_);
    {
        for (size_t codeletsCounter = 0; codeletsCounter < myTP->numThreads; codeletsCounter++) {
            myTP->checkInCodelets1332[codeletsCounter].decDep();
        }
    }
}
void TP31::_checkInCodelets1332::fire(void)
{
    /*Select the thread executing OMPSingleDirective 1332*/
    if (!__sync_val_compare_and_swap(&(myTP->TP1332_alreadyLaunched), 0, 1)) {
        /*Init the vars for this region*/

        /*printing node 1334: BinaryOperator*/
        deposit_darts31 = calc_deposit();
        /*Signaling next codelet from last stmt in the codelet*/
        /*Signaling omp region's barrier*/
        myTP->controlTPParent->barrierCodelets1332[0].decDep();
    } else {
        /*Signaling omp region's barrier*/
        myTP->barrierCodelets1332[0].decDep();
    }
}
void TP31::_barrierCodelets1332::fire(void)
{
    TP31* myTP = static_cast<TP31*>(myTP_);
    {
        for (size_t codeletsCounter = 0; codeletsCounter < myTP->numThreads; codeletsCounter++) {
            myTP->checkInCodelets1336[codeletsCounter].decDep();
        }
    }
}
void TP31::_checkInCodelets1336::fire(void)
{

    /*printing node 1336: ForStmt*/
    {
        {
            double* deposit = &(deposit_darts31);
            (void)deposit /*PRIVATE*/;
            int* endPidx = &(this->inputsTPParent->endPidx_darts31[this->getLocalID()]);
            (void)endPidx /*PRIVATE*/;
            long* pidx = &(this->inputsTPParent->pidx_darts31[this->getLocalID()]);
            (void)pidx /*PRIVATE*/;
            /*Loop's init*/
            (this->inputsTPParent->pidx_darts31[this->getID()])
                = (this->inputsTPParent->startPidx_darts31[this->getID()]);
            long pidx_darts_counter_temp31 = (this->inputsTPParent->pidx_darts31[this->getID()]);
            for (; (pidx_darts_counter_temp31) <= (*endPidx); (pidx_darts_counter_temp31)++) {
                update_part(partArray[(pidx_darts_counter_temp31)], *deposit);
            }
            (this->inputsTPParent->pidx_darts31[this->getID()]) = pidx_darts_counter_temp31;
        }
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/

    myTP->controlTPParent->nextCodeletsmanual_omp_module1[this->getID()]->decDep();
}
TP31::TP31(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_mainNextCodelet,
    darts::Codelet* in_mainSyncCodelet, TP31** in_ptrToThisFunctionTP, int in_startPidx,
    int in_endPidx)
    : ompTP(in_numThreads, in_mainCodeletID)
    , ptrToThisFunctionTP(in_ptrToThisFunctionTP)
    , inputsTPParent(this)
    , controlTPParent(this)
    , nextCodeletsmanual_omp_module1(new Codelet*[in_numThreads])
    , nextSyncCodeletsmanual_omp_module1(new Codelet*[in_numThreads])
    , startPidx_darts31(new int[this->numThreads])
    , endPidx_darts31(new int[this->numThreads])
    , pidx_darts31(new long[this->numThreads])
    , TP1332_alreadyLaunched(0)
    , checkInCodelets1331(new _checkInCodelets1331[this->numThreads])
    , barrierCodelets1331(new _barrierCodelets1331[1])
    , checkInCodelets1332(new _checkInCodelets1332[this->numThreads])
    , barrierCodelets1332(new _barrierCodelets1332[1])
    , checkInCodelets1336(new _checkInCodelets1336[this->numThreads])
{
    barrierCodelets1332[0] = _barrierCodelets1332(this->numThreads, this->numThreads, this, 0);
    barrierCodelets1331[0] = _barrierCodelets1331(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets1336* checkInCodelets1336Ptr = (this->checkInCodelets1336);
    _checkInCodelets1332* checkInCodelets1332Ptr = (this->checkInCodelets1332);
    _checkInCodelets1331* checkInCodelets1331Ptr = (this->checkInCodelets1331);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets1331);
#endif
    for (size_t codeletCounter = 0; codeletCounter < this->numThreads; codeletCounter++) {
        (*checkInCodelets1336Ptr) = _checkInCodelets1336(1, 1, this, codeletCounter);
        checkInCodelets1336Ptr++;
        (*checkInCodelets1332Ptr) = _checkInCodelets1332(1, 1, this, codeletCounter);
        checkInCodelets1332Ptr++;
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets1331Ptr) = _checkInCodelets1331(2, 1, this, codeletCounter);
#else
        (*checkInCodelets1331Ptr) = _checkInCodelets1331(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets1331Ptr).decDep();
        checkInCodelets1331Ptr++;
    }
    if (this->numThreads == 1) {
        this->nextCodeletsmanual_omp_module1[0] = in_mainNextCodelet;
        this->nextSyncCodeletsmanual_omp_module1[0] = in_mainSyncCodelet;
        this->startPidx_darts31[0] = in_startPidx;
        this->endPidx_darts31[0] = in_endPidx;
        this->availableCodelets[0] = 1;
    } else {
        this->startPidx_darts31[this->mainCodeletID] = in_startPidx;
        this->endPidx_darts31[this->mainCodeletID] = in_endPidx;
        this->nextCodeletsmanual_omp_module1[in_mainCodeletID] = in_mainNextCodelet;
        this->nextSyncCodeletsmanual_omp_module1[in_mainCodeletID] = in_mainSyncCodelet;
#if USE_SPIN_CODELETS == 0
        this->firstCodelet[this->mainCodeletID].decDep();
#else
        this->availableCodelets[this->mainCodeletID] = 1;
#endif
        *(this->ptrToThisFunctionTP) = this;
    }
}
TP31::~TP31()
{
    delete[] checkInCodelets1336;
    delete[] barrierCodelets1332;
    delete[] checkInCodelets1332;
    delete[] barrierCodelets1331;
    delete[] checkInCodelets1331;
    delete[] nextCodeletsmanual_omp_module1;
    delete[] nextSyncCodeletsmanual_omp_module1;
    delete[] startPidx_darts31;
    delete[] endPidx_darts31;
    delete[] pidx_darts31;
}
void TP31::setNewInputs(int in_startPidx, int in_endPidx, size_t codeletID)
{
    this->startPidx_darts31[codeletID] = in_startPidx;
    this->endPidx_darts31[codeletID] = in_endPidx;
}
/*TP32: TP_manual_omp_module2*/
void TP32::_checkInCodelets1345::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    myTP->barrierCodelets1345[0].decDep();
}
void TP32::_barrierCodelets1345::fire(void)
{
    TP32* myTP = static_cast<TP32*>(myTP_);
    {
        for (size_t codeletsCounter = 0; codeletsCounter < myTP->numThreads; codeletsCounter++) {
            myTP->checkInCodelets1346[codeletsCounter].decDep();
        }
    }
}
void TP32::_checkInCodelets1346::fire(void)
{
    /*Select the thread executing OMPSingleDirective 1346*/
    if (!__sync_val_compare_and_swap(&(myTP->TP1346_alreadyLaunched), 0, 1)) {
        /*Init the vars for this region*/

        /*printing node 1348: BinaryOperator*/
        deposit_darts32 = calc_deposit();
        /*Signaling next codelet from last stmt in the codelet*/
        /*Signaling omp region's barrier*/
        myTP->controlTPParent->barrierCodelets1346[0].decDep();
    } else {
        /*Signaling omp region's barrier*/
        myTP->barrierCodelets1346[0].decDep();
    }
}
void TP32::_barrierCodelets1346::fire(void)
{
    TP32* myTP = static_cast<TP32*>(myTP_);
    {
        for (size_t codeletsCounter = 0; codeletsCounter < myTP->numThreads; codeletsCounter++) {
            myTP->checkInCodelets1350[codeletsCounter].decDep();
        }
    }
}
void TP32::_checkInCodelets1350::fire(void)
{

    /*printing node 1350: ForStmt*/
    {
        {
            double* deposit = &(deposit_darts32);
            (void)deposit /*PRIVATE*/;
            int* endPidx = &(this->inputsTPParent->endPidx_darts32[this->getLocalID()]);
            (void)endPidx /*PRIVATE*/;
            long* pidx = &(this->inputsTPParent->pidx_darts32[this->getLocalID()]);
            (void)pidx /*PRIVATE*/;
            /*Loop's init*/
            (this->inputsTPParent->pidx_darts32[this->getID()])
                = (this->inputsTPParent->startPidx_darts32[this->getID()]);
            long pidx_darts_counter_temp32 = (this->inputsTPParent->pidx_darts32[this->getID()]);
            for (; (pidx_darts_counter_temp32) <= (*endPidx); (pidx_darts_counter_temp32)++) {
                update_part(partArray[(pidx_darts_counter_temp32)], *deposit);
            }
            (this->inputsTPParent->pidx_darts32[this->getID()]) = pidx_darts_counter_temp32;
        }
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 1350 nextRegion: 1356 */
    myTP->controlTPParent->barrierCodelets1356[0].decDep();
}
void TP32::_barrierCodelets1356::fire(void)
{
    TP32* myTP = static_cast<TP32*>(myTP_);
    {
        for (size_t codeletsCounter = 0; codeletsCounter < myTP->numThreads; codeletsCounter++) {
            myTP->checkInCodelets1357[codeletsCounter].decDep();
        }
    }
}
void TP32::_checkInCodelets1357::fire(void)
{
    /*Select the thread executing OMPSingleDirective 1357*/
    if (!__sync_val_compare_and_swap(&(myTP->TP1357_alreadyLaunched), 0, 1)) {
        /*Init the vars for this region*/

        /*printing node 1359: BinaryOperator*/
        deposit_darts32 = calc_deposit();
        /*Signaling next codelet from last stmt in the codelet*/
        /*Signaling omp region's barrier*/
        myTP->controlTPParent->barrierCodelets1357[0].decDep();
    } else {
        /*Signaling omp region's barrier*/
        myTP->barrierCodelets1357[0].decDep();
    }
}
void TP32::_barrierCodelets1357::fire(void)
{
    TP32* myTP = static_cast<TP32*>(myTP_);
    {
        for (size_t codeletsCounter = 0; codeletsCounter < myTP->numThreads; codeletsCounter++) {
            myTP->checkInCodelets1361[codeletsCounter].decDep();
        }
    }
}
void TP32::_checkInCodelets1361::fire(void)
{

    /*printing node 1361: ForStmt*/
    {
        {
            double* deposit = &(deposit_darts32);
            (void)deposit /*PRIVATE*/;
            int* endPidx = &(this->inputsTPParent->endPidx_darts32[this->getLocalID()]);
            (void)endPidx /*PRIVATE*/;
            long* pidx = &(this->inputsTPParent->pidx_darts32[this->getLocalID()]);
            (void)pidx /*PRIVATE*/;
            /*Loop's init*/
            (this->inputsTPParent->pidx_darts32[this->getID()])
                = (this->inputsTPParent->startPidx_darts32[this->getID()]);
            long pidx_darts_counter_temp32 = (this->inputsTPParent->pidx_darts32[this->getID()]);
            for (; (pidx_darts_counter_temp32) <= (*endPidx); (pidx_darts_counter_temp32)++) {
                update_part(partArray[(pidx_darts_counter_temp32)], *deposit);
            }
            (this->inputsTPParent->pidx_darts32[this->getID()]) = pidx_darts_counter_temp32;
        }
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/

    myTP->controlTPParent->nextCodeletsmanual_omp_module2[this->getID()]->decDep();
}
TP32::TP32(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_mainNextCodelet,
    darts::Codelet* in_mainSyncCodelet, TP32** in_ptrToThisFunctionTP, int in_startPidx,
    int in_endPidx)
    : ompTP(in_numThreads, in_mainCodeletID)
    , ptrToThisFunctionTP(in_ptrToThisFunctionTP)
    , inputsTPParent(this)
    , controlTPParent(this)
    , nextCodeletsmanual_omp_module2(new Codelet*[in_numThreads])
    , nextSyncCodeletsmanual_omp_module2(new Codelet*[in_numThreads])
    , startPidx_darts32(new int[this->numThreads])
    , endPidx_darts32(new int[this->numThreads])
    , pidx_darts32(new long[this->numThreads])
    , TP1346_alreadyLaunched(0)
    , TP1357_alreadyLaunched(0)
    , checkInCodelets1345(new _checkInCodelets1345[this->numThreads])
    , barrierCodelets1345(new _barrierCodelets1345[1])
    , checkInCodelets1346(new _checkInCodelets1346[this->numThreads])
    , barrierCodelets1346(new _barrierCodelets1346[1])
    , checkInCodelets1350(new _checkInCodelets1350[this->numThreads])
    , barrierCodelets1356(new _barrierCodelets1356[1])
    , checkInCodelets1357(new _checkInCodelets1357[this->numThreads])
    , barrierCodelets1357(new _barrierCodelets1357[1])
    , checkInCodelets1361(new _checkInCodelets1361[this->numThreads])
{
    barrierCodelets1357[0] = _barrierCodelets1357(this->numThreads, this->numThreads, this, 0);
    barrierCodelets1356[0] = _barrierCodelets1356(this->numThreads, this->numThreads, this, 0);
    barrierCodelets1346[0] = _barrierCodelets1346(this->numThreads, this->numThreads, this, 0);
    barrierCodelets1345[0] = _barrierCodelets1345(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets1361* checkInCodelets1361Ptr = (this->checkInCodelets1361);
    _checkInCodelets1357* checkInCodelets1357Ptr = (this->checkInCodelets1357);
    _checkInCodelets1350* checkInCodelets1350Ptr = (this->checkInCodelets1350);
    _checkInCodelets1346* checkInCodelets1346Ptr = (this->checkInCodelets1346);
    _checkInCodelets1345* checkInCodelets1345Ptr = (this->checkInCodelets1345);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets1345);
#endif
    for (size_t codeletCounter = 0; codeletCounter < this->numThreads; codeletCounter++) {
        (*checkInCodelets1361Ptr) = _checkInCodelets1361(1, 1, this, codeletCounter);
        checkInCodelets1361Ptr++;
        (*checkInCodelets1357Ptr) = _checkInCodelets1357(1, 1, this, codeletCounter);
        checkInCodelets1357Ptr++;
        (*checkInCodelets1350Ptr) = _checkInCodelets1350(1, 1, this, codeletCounter);
        checkInCodelets1350Ptr++;
        (*checkInCodelets1346Ptr) = _checkInCodelets1346(1, 1, this, codeletCounter);
        checkInCodelets1346Ptr++;
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets1345Ptr) = _checkInCodelets1345(2, 1, this, codeletCounter);
#else
        (*checkInCodelets1345Ptr) = _checkInCodelets1345(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets1345Ptr).decDep();
        checkInCodelets1345Ptr++;
    }
    if (this->numThreads == 1) {
        this->nextCodeletsmanual_omp_module2[0] = in_mainNextCodelet;
        this->nextSyncCodeletsmanual_omp_module2[0] = in_mainSyncCodelet;
        this->startPidx_darts32[0] = in_startPidx;
        this->endPidx_darts32[0] = in_endPidx;
        this->availableCodelets[0] = 1;
    } else {
        this->startPidx_darts32[this->mainCodeletID] = in_startPidx;
        this->endPidx_darts32[this->mainCodeletID] = in_endPidx;
        this->nextCodeletsmanual_omp_module2[in_mainCodeletID] = in_mainNextCodelet;
        this->nextSyncCodeletsmanual_omp_module2[in_mainCodeletID] = in_mainSyncCodelet;
#if USE_SPIN_CODELETS == 0
        this->firstCodelet[this->mainCodeletID].decDep();
#else
        this->availableCodelets[this->mainCodeletID] = 1;
#endif
        *(this->ptrToThisFunctionTP) = this;
    }
}
TP32::~TP32()
{
    delete[] checkInCodelets1361;
    delete[] barrierCodelets1357;
    delete[] checkInCodelets1357;
    delete[] barrierCodelets1356;
    delete[] checkInCodelets1350;
    delete[] barrierCodelets1346;
    delete[] checkInCodelets1346;
    delete[] barrierCodelets1345;
    delete[] checkInCodelets1345;
    delete[] nextCodeletsmanual_omp_module2;
    delete[] nextSyncCodeletsmanual_omp_module2;
    delete[] startPidx_darts32;
    delete[] endPidx_darts32;
    delete[] pidx_darts32;
}
void TP32::setNewInputs(int in_startPidx, int in_endPidx, size_t codeletID)
{
    this->startPidx_darts32[codeletID] = in_startPidx;
    this->endPidx_darts32[codeletID] = in_endPidx;
}
/*TP33: TP_manual_omp_module3*/
void TP33::_checkInCodelets1370::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    myTP->barrierCodelets1370[0].decDep();
}
void TP33::_barrierCodelets1370::fire(void)
{
    TP33* myTP = static_cast<TP33*>(myTP_);
    {
        for (size_t codeletsCounter = 0; codeletsCounter < myTP->numThreads; codeletsCounter++) {
            myTP->checkInCodelets1371[codeletsCounter].decDep();
        }
    }
}
void TP33::_checkInCodelets1371::fire(void)
{
    /*Select the thread executing OMPSingleDirective 1371*/
    if (!__sync_val_compare_and_swap(&(myTP->TP1371_alreadyLaunched), 0, 1)) {
        /*Init the vars for this region*/

        /*printing node 1373: BinaryOperator*/
        deposit_darts33 = calc_deposit();
        /*Signaling next codelet from last stmt in the codelet*/
        /*Signaling omp region's barrier*/
        myTP->controlTPParent->barrierCodelets1371[0].decDep();
    } else {
        /*Signaling omp region's barrier*/
        myTP->barrierCodelets1371[0].decDep();
    }
}
void TP33::_barrierCodelets1371::fire(void)
{
    TP33* myTP = static_cast<TP33*>(myTP_);
    {
        for (size_t codeletsCounter = 0; codeletsCounter < myTP->numThreads; codeletsCounter++) {
            myTP->checkInCodelets1375[codeletsCounter].decDep();
        }
    }
}
void TP33::_checkInCodelets1375::fire(void)
{

    /*printing node 1375: ForStmt*/
    {
        {
            double* deposit = &(deposit_darts33);
            (void)deposit /*PRIVATE*/;
            int* endPidx = &(this->inputsTPParent->endPidx_darts33[this->getLocalID()]);
            (void)endPidx /*PRIVATE*/;
            long* pidx = &(this->inputsTPParent->pidx_darts33[this->getLocalID()]);
            (void)pidx /*PRIVATE*/;
            /*Loop's init*/
            (this->inputsTPParent->pidx_darts33[this->getID()])
                = (this->inputsTPParent->startPidx_darts33[this->getID()]);
            long pidx_darts_counter_temp33 = (this->inputsTPParent->pidx_darts33[this->getID()]);
            for (; (pidx_darts_counter_temp33) <= (*endPidx); (pidx_darts_counter_temp33)++) {
                update_part(partArray[(pidx_darts_counter_temp33)], *deposit);
            }
            (this->inputsTPParent->pidx_darts33[this->getID()]) = pidx_darts_counter_temp33;
        }
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 1375 nextRegion: 1381 */
    myTP->controlTPParent->barrierCodelets1381[0].decDep();
}
void TP33::_barrierCodelets1381::fire(void)
{
    TP33* myTP = static_cast<TP33*>(myTP_);
    {
        for (size_t codeletsCounter = 0; codeletsCounter < myTP->numThreads; codeletsCounter++) {
            myTP->checkInCodelets1382[codeletsCounter].decDep();
        }
    }
}
void TP33::_checkInCodelets1382::fire(void)
{
    /*Select the thread executing OMPSingleDirective 1382*/
    if (!__sync_val_compare_and_swap(&(myTP->TP1382_alreadyLaunched), 0, 1)) {
        /*Init the vars for this region*/

        /*printing node 1384: BinaryOperator*/
        deposit_darts33 = calc_deposit();
        /*Signaling next codelet from last stmt in the codelet*/
        /*Signaling omp region's barrier*/
        myTP->controlTPParent->barrierCodelets1382[0].decDep();
    } else {
        /*Signaling omp region's barrier*/
        myTP->barrierCodelets1382[0].decDep();
    }
}
void TP33::_barrierCodelets1382::fire(void)
{
    TP33* myTP = static_cast<TP33*>(myTP_);
    {
        for (size_t codeletsCounter = 0; codeletsCounter < myTP->numThreads; codeletsCounter++) {
            myTP->checkInCodelets1386[codeletsCounter].decDep();
        }
    }
}
void TP33::_checkInCodelets1386::fire(void)
{

    /*printing node 1386: ForStmt*/
    {
        {
            double* deposit = &(deposit_darts33);
            (void)deposit /*PRIVATE*/;
            int* endPidx = &(this->inputsTPParent->endPidx_darts33[this->getLocalID()]);
            (void)endPidx /*PRIVATE*/;
            long* pidx = &(this->inputsTPParent->pidx_darts33[this->getLocalID()]);
            (void)pidx /*PRIVATE*/;
            /*Loop's init*/
            (this->inputsTPParent->pidx_darts33[this->getID()])
                = (this->inputsTPParent->startPidx_darts33[this->getID()]);
            long pidx_darts_counter_temp33 = (this->inputsTPParent->pidx_darts33[this->getID()]);
            for (; (pidx_darts_counter_temp33) <= (*endPidx); (pidx_darts_counter_temp33)++) {
                update_part(partArray[(pidx_darts_counter_temp33)], *deposit);
            }
            (this->inputsTPParent->pidx_darts33[this->getID()]) = pidx_darts_counter_temp33;
        }
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 1386 nextRegion: 1392 */
    myTP->controlTPParent->barrierCodelets1392[0].decDep();
}
void TP33::_barrierCodelets1392::fire(void)
{
    TP33* myTP = static_cast<TP33*>(myTP_);
    {
        for (size_t codeletsCounter = 0; codeletsCounter < myTP->numThreads; codeletsCounter++) {
            myTP->checkInCodelets1393[codeletsCounter].decDep();
        }
    }
}
void TP33::_checkInCodelets1393::fire(void)
{
    /*Select the thread executing OMPSingleDirective 1393*/
    if (!__sync_val_compare_and_swap(&(myTP->TP1393_alreadyLaunched), 0, 1)) {
        /*Init the vars for this region*/

        /*printing node 1395: BinaryOperator*/
        deposit_darts33 = calc_deposit();
        /*Signaling next codelet from last stmt in the codelet*/
        /*Signaling omp region's barrier*/
        myTP->controlTPParent->barrierCodelets1393[0].decDep();
    } else {
        /*Signaling omp region's barrier*/
        myTP->barrierCodelets1393[0].decDep();
    }
}
void TP33::_barrierCodelets1393::fire(void)
{
    TP33* myTP = static_cast<TP33*>(myTP_);
    {
        for (size_t codeletsCounter = 0; codeletsCounter < myTP->numThreads; codeletsCounter++) {
            myTP->checkInCodelets1397[codeletsCounter].decDep();
        }
    }
}
void TP33::_checkInCodelets1397::fire(void)
{

    /*printing node 1397: ForStmt*/
    {
        {
            double* deposit = &(deposit_darts33);
            (void)deposit /*PRIVATE*/;
            int* endPidx = &(this->inputsTPParent->endPidx_darts33[this->getLocalID()]);
            (void)endPidx /*PRIVATE*/;
            long* pidx = &(this->inputsTPParent->pidx_darts33[this->getLocalID()]);
            (void)pidx /*PRIVATE*/;
            /*Loop's init*/
            (this->inputsTPParent->pidx_darts33[this->getID()])
                = (this->inputsTPParent->startPidx_darts33[this->getID()]);
            long pidx_darts_counter_temp33 = (this->inputsTPParent->pidx_darts33[this->getID()]);
            for (; (pidx_darts_counter_temp33) <= (*endPidx); (pidx_darts_counter_temp33)++) {
                update_part(partArray[(pidx_darts_counter_temp33)], *deposit);
            }
            (this->inputsTPParent->pidx_darts33[this->getID()]) = pidx_darts_counter_temp33;
        }
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/

    myTP->controlTPParent->nextCodeletsmanual_omp_module3[this->getID()]->decDep();
}
TP33::TP33(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_mainNextCodelet,
    darts::Codelet* in_mainSyncCodelet, TP33** in_ptrToThisFunctionTP, int in_startPidx,
    int in_endPidx)
    : ompTP(in_numThreads, in_mainCodeletID)
    , ptrToThisFunctionTP(in_ptrToThisFunctionTP)
    , inputsTPParent(this)
    , controlTPParent(this)
    , nextCodeletsmanual_omp_module3(new Codelet*[in_numThreads])
    , nextSyncCodeletsmanual_omp_module3(new Codelet*[in_numThreads])
    , startPidx_darts33(new int[this->numThreads])
    , endPidx_darts33(new int[this->numThreads])
    , pidx_darts33(new long[this->numThreads])
    , TP1371_alreadyLaunched(0)
    , TP1382_alreadyLaunched(0)
    , TP1393_alreadyLaunched(0)
    , checkInCodelets1370(new _checkInCodelets1370[this->numThreads])
    , barrierCodelets1370(new _barrierCodelets1370[1])
    , checkInCodelets1371(new _checkInCodelets1371[this->numThreads])
    , barrierCodelets1371(new _barrierCodelets1371[1])
    , checkInCodelets1375(new _checkInCodelets1375[this->numThreads])
    , barrierCodelets1381(new _barrierCodelets1381[1])
    , checkInCodelets1382(new _checkInCodelets1382[this->numThreads])
    , barrierCodelets1382(new _barrierCodelets1382[1])
    , checkInCodelets1386(new _checkInCodelets1386[this->numThreads])
    , barrierCodelets1392(new _barrierCodelets1392[1])
    , checkInCodelets1393(new _checkInCodelets1393[this->numThreads])
    , barrierCodelets1393(new _barrierCodelets1393[1])
    , checkInCodelets1397(new _checkInCodelets1397[this->numThreads])
{
    barrierCodelets1393[0] = _barrierCodelets1393(this->numThreads, this->numThreads, this, 0);
    barrierCodelets1392[0] = _barrierCodelets1392(this->numThreads, this->numThreads, this, 0);
    barrierCodelets1382[0] = _barrierCodelets1382(this->numThreads, this->numThreads, this, 0);
    barrierCodelets1381[0] = _barrierCodelets1381(this->numThreads, this->numThreads, this, 0);
    barrierCodelets1371[0] = _barrierCodelets1371(this->numThreads, this->numThreads, this, 0);
    barrierCodelets1370[0] = _barrierCodelets1370(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets1397* checkInCodelets1397Ptr = (this->checkInCodelets1397);
    _checkInCodelets1393* checkInCodelets1393Ptr = (this->checkInCodelets1393);
    _checkInCodelets1386* checkInCodelets1386Ptr = (this->checkInCodelets1386);
    _checkInCodelets1382* checkInCodelets1382Ptr = (this->checkInCodelets1382);
    _checkInCodelets1375* checkInCodelets1375Ptr = (this->checkInCodelets1375);
    _checkInCodelets1371* checkInCodelets1371Ptr = (this->checkInCodelets1371);
    _checkInCodelets1370* checkInCodelets1370Ptr = (this->checkInCodelets1370);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets1370);
#endif
    for (size_t codeletCounter = 0; codeletCounter < this->numThreads; codeletCounter++) {
        (*checkInCodelets1397Ptr) = _checkInCodelets1397(1, 1, this, codeletCounter);
        checkInCodelets1397Ptr++;
        (*checkInCodelets1393Ptr) = _checkInCodelets1393(1, 1, this, codeletCounter);
        checkInCodelets1393Ptr++;
        (*checkInCodelets1386Ptr) = _checkInCodelets1386(1, 1, this, codeletCounter);
        checkInCodelets1386Ptr++;
        (*checkInCodelets1382Ptr) = _checkInCodelets1382(1, 1, this, codeletCounter);
        checkInCodelets1382Ptr++;
        (*checkInCodelets1375Ptr) = _checkInCodelets1375(1, 1, this, codeletCounter);
        checkInCodelets1375Ptr++;
        (*checkInCodelets1371Ptr) = _checkInCodelets1371(1, 1, this, codeletCounter);
        checkInCodelets1371Ptr++;
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets1370Ptr) = _checkInCodelets1370(2, 1, this, codeletCounter);
#else
        (*checkInCodelets1370Ptr) = _checkInCodelets1370(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets1370Ptr).decDep();
        checkInCodelets1370Ptr++;
    }
    if (this->numThreads == 1) {
        this->nextCodeletsmanual_omp_module3[0] = in_mainNextCodelet;
        this->nextSyncCodeletsmanual_omp_module3[0] = in_mainSyncCodelet;
        this->startPidx_darts33[0] = in_startPidx;
        this->endPidx_darts33[0] = in_endPidx;
        this->availableCodelets[0] = 1;
    } else {
        this->startPidx_darts33[this->mainCodeletID] = in_startPidx;
        this->endPidx_darts33[this->mainCodeletID] = in_endPidx;
        this->nextCodeletsmanual_omp_module3[in_mainCodeletID] = in_mainNextCodelet;
        this->nextSyncCodeletsmanual_omp_module3[in_mainCodeletID] = in_mainSyncCodelet;
#if USE_SPIN_CODELETS == 0
        this->firstCodelet[this->mainCodeletID].decDep();
#else
        this->availableCodelets[this->mainCodeletID] = 1;
#endif
        *(this->ptrToThisFunctionTP) = this;
    }
}
TP33::~TP33()
{
    delete[] checkInCodelets1397;
    delete[] barrierCodelets1393;
    delete[] checkInCodelets1393;
    delete[] barrierCodelets1392;
    delete[] checkInCodelets1386;
    delete[] barrierCodelets1382;
    delete[] checkInCodelets1382;
    delete[] barrierCodelets1381;
    delete[] checkInCodelets1375;
    delete[] barrierCodelets1371;
    delete[] checkInCodelets1371;
    delete[] barrierCodelets1370;
    delete[] checkInCodelets1370;
    delete[] nextCodeletsmanual_omp_module3;
    delete[] nextSyncCodeletsmanual_omp_module3;
    delete[] startPidx_darts33;
    delete[] endPidx_darts33;
    delete[] pidx_darts33;
}
void TP33::setNewInputs(int in_startPidx, int in_endPidx, size_t codeletID)
{
    this->startPidx_darts33[codeletID] = in_startPidx;
    this->endPidx_darts33[codeletID] = in_endPidx;
}
/*TP34: TP_manual_omp_module4*/
void TP34::_checkInCodelets1406::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    myTP->barrierCodelets1406[0].decDep();
}
void TP34::_barrierCodelets1406::fire(void)
{
    TP34* myTP = static_cast<TP34*>(myTP_);
    {
        for (size_t codeletsCounter = 0; codeletsCounter < myTP->numThreads; codeletsCounter++) {
            myTP->checkInCodelets1407[codeletsCounter].decDep();
        }
    }
}
void TP34::_checkInCodelets1407::fire(void)
{
    /*Select the thread executing OMPSingleDirective 1407*/
    if (!__sync_val_compare_and_swap(&(myTP->TP1407_alreadyLaunched), 0, 1)) {
        /*Init the vars for this region*/

        /*printing node 1409: BinaryOperator*/
        deposit_darts34 = calc_deposit();
        /*Signaling next codelet from last stmt in the codelet*/
        /*Signaling omp region's barrier*/
        myTP->controlTPParent->barrierCodelets1407[0].decDep();
    } else {
        /*Signaling omp region's barrier*/
        myTP->barrierCodelets1407[0].decDep();
    }
}
void TP34::_barrierCodelets1407::fire(void)
{
    TP34* myTP = static_cast<TP34*>(myTP_);
    {
        for (size_t codeletsCounter = 0; codeletsCounter < myTP->numThreads; codeletsCounter++) {
            myTP->checkInCodelets1411[codeletsCounter].decDep();
        }
    }
}
void TP34::_checkInCodelets1411::fire(void)
{

    /*printing node 1411: ForStmt*/
    {
        {
            double* deposit = &(deposit_darts34);
            (void)deposit /*PRIVATE*/;
            int* endPidx = &(this->inputsTPParent->endPidx_darts34[this->getLocalID()]);
            (void)endPidx /*PRIVATE*/;
            long* pidx = &(this->inputsTPParent->pidx_darts34[this->getLocalID()]);
            (void)pidx /*PRIVATE*/;
            /*Loop's init*/
            (this->inputsTPParent->pidx_darts34[this->getID()])
                = (this->inputsTPParent->startPidx_darts34[this->getID()]);
            long pidx_darts_counter_temp34 = (this->inputsTPParent->pidx_darts34[this->getID()]);
            for (; (pidx_darts_counter_temp34) <= (*endPidx); (pidx_darts_counter_temp34)++) {
                update_part(partArray[(pidx_darts_counter_temp34)], *deposit);
            }
            (this->inputsTPParent->pidx_darts34[this->getID()]) = pidx_darts_counter_temp34;
        }
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 1411 nextRegion: 1417 */
    myTP->controlTPParent->barrierCodelets1417[0].decDep();
}
void TP34::_barrierCodelets1417::fire(void)
{
    TP34* myTP = static_cast<TP34*>(myTP_);
    {
        for (size_t codeletsCounter = 0; codeletsCounter < myTP->numThreads; codeletsCounter++) {
            myTP->checkInCodelets1418[codeletsCounter].decDep();
        }
    }
}
void TP34::_checkInCodelets1418::fire(void)
{
    /*Select the thread executing OMPSingleDirective 1418*/
    if (!__sync_val_compare_and_swap(&(myTP->TP1418_alreadyLaunched), 0, 1)) {
        /*Init the vars for this region*/

        /*printing node 1420: BinaryOperator*/
        deposit_darts34 = calc_deposit();
        /*Signaling next codelet from last stmt in the codelet*/
        /*Signaling omp region's barrier*/
        myTP->controlTPParent->barrierCodelets1418[0].decDep();
    } else {
        /*Signaling omp region's barrier*/
        myTP->barrierCodelets1418[0].decDep();
    }
}
void TP34::_barrierCodelets1418::fire(void)
{
    TP34* myTP = static_cast<TP34*>(myTP_);
    {
        for (size_t codeletsCounter = 0; codeletsCounter < myTP->numThreads; codeletsCounter++) {
            myTP->checkInCodelets1422[codeletsCounter].decDep();
        }
    }
}
void TP34::_checkInCodelets1422::fire(void)
{

    /*printing node 1422: ForStmt*/
    {
        {
            double* deposit = &(deposit_darts34);
            (void)deposit /*PRIVATE*/;
            int* endPidx = &(this->inputsTPParent->endPidx_darts34[this->getLocalID()]);
            (void)endPidx /*PRIVATE*/;
            long* pidx = &(this->inputsTPParent->pidx_darts34[this->getLocalID()]);
            (void)pidx /*PRIVATE*/;
            /*Loop's init*/
            (this->inputsTPParent->pidx_darts34[this->getID()])
                = (this->inputsTPParent->startPidx_darts34[this->getID()]);
            long pidx_darts_counter_temp34 = (this->inputsTPParent->pidx_darts34[this->getID()]);
            for (; (pidx_darts_counter_temp34) <= (*endPidx); (pidx_darts_counter_temp34)++) {
                update_part(partArray[(pidx_darts_counter_temp34)], *deposit);
            }
            (this->inputsTPParent->pidx_darts34[this->getID()]) = pidx_darts_counter_temp34;
        }
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 1422 nextRegion: 1428 */
    myTP->controlTPParent->barrierCodelets1428[0].decDep();
}
void TP34::_barrierCodelets1428::fire(void)
{
    TP34* myTP = static_cast<TP34*>(myTP_);
    {
        for (size_t codeletsCounter = 0; codeletsCounter < myTP->numThreads; codeletsCounter++) {
            myTP->checkInCodelets1429[codeletsCounter].decDep();
        }
    }
}
void TP34::_checkInCodelets1429::fire(void)
{
    /*Select the thread executing OMPSingleDirective 1429*/
    if (!__sync_val_compare_and_swap(&(myTP->TP1429_alreadyLaunched), 0, 1)) {
        /*Init the vars for this region*/

        /*printing node 1431: BinaryOperator*/
        deposit_darts34 = calc_deposit();
        /*Signaling next codelet from last stmt in the codelet*/
        /*Signaling omp region's barrier*/
        myTP->controlTPParent->barrierCodelets1429[0].decDep();
    } else {
        /*Signaling omp region's barrier*/
        myTP->barrierCodelets1429[0].decDep();
    }
}
void TP34::_barrierCodelets1429::fire(void)
{
    TP34* myTP = static_cast<TP34*>(myTP_);
    {
        for (size_t codeletsCounter = 0; codeletsCounter < myTP->numThreads; codeletsCounter++) {
            myTP->checkInCodelets1433[codeletsCounter].decDep();
        }
    }
}
void TP34::_checkInCodelets1433::fire(void)
{

    /*printing node 1433: ForStmt*/
    {
        {
            double* deposit = &(deposit_darts34);
            (void)deposit /*PRIVATE*/;
            int* endPidx = &(this->inputsTPParent->endPidx_darts34[this->getLocalID()]);
            (void)endPidx /*PRIVATE*/;
            long* pidx = &(this->inputsTPParent->pidx_darts34[this->getLocalID()]);
            (void)pidx /*PRIVATE*/;
            /*Loop's init*/
            (this->inputsTPParent->pidx_darts34[this->getID()])
                = (this->inputsTPParent->startPidx_darts34[this->getID()]);
            long pidx_darts_counter_temp34 = (this->inputsTPParent->pidx_darts34[this->getID()]);
            for (; (pidx_darts_counter_temp34) <= (*endPidx); (pidx_darts_counter_temp34)++) {
                update_part(partArray[(pidx_darts_counter_temp34)], *deposit);
            }
            (this->inputsTPParent->pidx_darts34[this->getID()]) = pidx_darts_counter_temp34;
        }
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 1433 nextRegion: 1439 */
    myTP->controlTPParent->barrierCodelets1439[0].decDep();
}
void TP34::_barrierCodelets1439::fire(void)
{
    TP34* myTP = static_cast<TP34*>(myTP_);
    {
        for (size_t codeletsCounter = 0; codeletsCounter < myTP->numThreads; codeletsCounter++) {
            myTP->checkInCodelets1440[codeletsCounter].decDep();
        }
    }
}
void TP34::_checkInCodelets1440::fire(void)
{
    /*Select the thread executing OMPSingleDirective 1440*/
    if (!__sync_val_compare_and_swap(&(myTP->TP1440_alreadyLaunched), 0, 1)) {
        /*Init the vars for this region*/

        /*printing node 1442: BinaryOperator*/
        deposit_darts34 = calc_deposit();
        /*Signaling next codelet from last stmt in the codelet*/
        /*Signaling omp region's barrier*/
        myTP->controlTPParent->barrierCodelets1440[0].decDep();
    } else {
        /*Signaling omp region's barrier*/
        myTP->barrierCodelets1440[0].decDep();
    }
}
void TP34::_barrierCodelets1440::fire(void)
{
    TP34* myTP = static_cast<TP34*>(myTP_);
    {
        for (size_t codeletsCounter = 0; codeletsCounter < myTP->numThreads; codeletsCounter++) {
            myTP->checkInCodelets1444[codeletsCounter].decDep();
        }
    }
}
void TP34::_checkInCodelets1444::fire(void)
{

    /*printing node 1444: ForStmt*/
    {
        {
            double* deposit = &(deposit_darts34);
            (void)deposit /*PRIVATE*/;
            int* endPidx = &(this->inputsTPParent->endPidx_darts34[this->getLocalID()]);
            (void)endPidx /*PRIVATE*/;
            long* pidx = &(this->inputsTPParent->pidx_darts34[this->getLocalID()]);
            (void)pidx /*PRIVATE*/;
            /*Loop's init*/
            (this->inputsTPParent->pidx_darts34[this->getID()])
                = (this->inputsTPParent->startPidx_darts34[this->getID()]);
            long pidx_darts_counter_temp34 = (this->inputsTPParent->pidx_darts34[this->getID()]);
            for (; (pidx_darts_counter_temp34) <= (*endPidx); (pidx_darts_counter_temp34)++) {
                update_part(partArray[(pidx_darts_counter_temp34)], *deposit);
            }
            (this->inputsTPParent->pidx_darts34[this->getID()]) = pidx_darts_counter_temp34;
        }
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/

    myTP->controlTPParent->nextCodeletsmanual_omp_module4[this->getID()]->decDep();
}
TP34::TP34(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_mainNextCodelet,
    darts::Codelet* in_mainSyncCodelet, TP34** in_ptrToThisFunctionTP, int in_startPidx,
    int in_endPidx)
    : ompTP(in_numThreads, in_mainCodeletID)
    , ptrToThisFunctionTP(in_ptrToThisFunctionTP)
    , inputsTPParent(this)
    , controlTPParent(this)
    , nextCodeletsmanual_omp_module4(new Codelet*[in_numThreads])
    , nextSyncCodeletsmanual_omp_module4(new Codelet*[in_numThreads])
    , startPidx_darts34(new int[this->numThreads])
    , endPidx_darts34(new int[this->numThreads])
    , pidx_darts34(new long[this->numThreads])
    , TP1407_alreadyLaunched(0)
    , TP1418_alreadyLaunched(0)
    , TP1429_alreadyLaunched(0)
    , TP1440_alreadyLaunched(0)
    , checkInCodelets1406(new _checkInCodelets1406[this->numThreads])
    , barrierCodelets1406(new _barrierCodelets1406[1])
    , checkInCodelets1407(new _checkInCodelets1407[this->numThreads])
    , barrierCodelets1407(new _barrierCodelets1407[1])
    , checkInCodelets1411(new _checkInCodelets1411[this->numThreads])
    , barrierCodelets1417(new _barrierCodelets1417[1])
    , checkInCodelets1418(new _checkInCodelets1418[this->numThreads])
    , barrierCodelets1418(new _barrierCodelets1418[1])
    , checkInCodelets1422(new _checkInCodelets1422[this->numThreads])
    , barrierCodelets1428(new _barrierCodelets1428[1])
    , checkInCodelets1429(new _checkInCodelets1429[this->numThreads])
    , barrierCodelets1429(new _barrierCodelets1429[1])
    , checkInCodelets1433(new _checkInCodelets1433[this->numThreads])
    , barrierCodelets1439(new _barrierCodelets1439[1])
    , checkInCodelets1440(new _checkInCodelets1440[this->numThreads])
    , barrierCodelets1440(new _barrierCodelets1440[1])
    , checkInCodelets1444(new _checkInCodelets1444[this->numThreads])
{
    barrierCodelets1440[0] = _barrierCodelets1440(this->numThreads, this->numThreads, this, 0);
    barrierCodelets1439[0] = _barrierCodelets1439(this->numThreads, this->numThreads, this, 0);
    barrierCodelets1429[0] = _barrierCodelets1429(this->numThreads, this->numThreads, this, 0);
    barrierCodelets1428[0] = _barrierCodelets1428(this->numThreads, this->numThreads, this, 0);
    barrierCodelets1418[0] = _barrierCodelets1418(this->numThreads, this->numThreads, this, 0);
    barrierCodelets1417[0] = _barrierCodelets1417(this->numThreads, this->numThreads, this, 0);
    barrierCodelets1407[0] = _barrierCodelets1407(this->numThreads, this->numThreads, this, 0);
    barrierCodelets1406[0] = _barrierCodelets1406(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets1444* checkInCodelets1444Ptr = (this->checkInCodelets1444);
    _checkInCodelets1440* checkInCodelets1440Ptr = (this->checkInCodelets1440);
    _checkInCodelets1433* checkInCodelets1433Ptr = (this->checkInCodelets1433);
    _checkInCodelets1429* checkInCodelets1429Ptr = (this->checkInCodelets1429);
    _checkInCodelets1422* checkInCodelets1422Ptr = (this->checkInCodelets1422);
    _checkInCodelets1418* checkInCodelets1418Ptr = (this->checkInCodelets1418);
    _checkInCodelets1411* checkInCodelets1411Ptr = (this->checkInCodelets1411);
    _checkInCodelets1407* checkInCodelets1407Ptr = (this->checkInCodelets1407);
    _checkInCodelets1406* checkInCodelets1406Ptr = (this->checkInCodelets1406);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets1406);
#endif
    for (size_t codeletCounter = 0; codeletCounter < this->numThreads; codeletCounter++) {
        (*checkInCodelets1444Ptr) = _checkInCodelets1444(1, 1, this, codeletCounter);
        checkInCodelets1444Ptr++;
        (*checkInCodelets1440Ptr) = _checkInCodelets1440(1, 1, this, codeletCounter);
        checkInCodelets1440Ptr++;
        (*checkInCodelets1433Ptr) = _checkInCodelets1433(1, 1, this, codeletCounter);
        checkInCodelets1433Ptr++;
        (*checkInCodelets1429Ptr) = _checkInCodelets1429(1, 1, this, codeletCounter);
        checkInCodelets1429Ptr++;
        (*checkInCodelets1422Ptr) = _checkInCodelets1422(1, 1, this, codeletCounter);
        checkInCodelets1422Ptr++;
        (*checkInCodelets1418Ptr) = _checkInCodelets1418(1, 1, this, codeletCounter);
        checkInCodelets1418Ptr++;
        (*checkInCodelets1411Ptr) = _checkInCodelets1411(1, 1, this, codeletCounter);
        checkInCodelets1411Ptr++;
        (*checkInCodelets1407Ptr) = _checkInCodelets1407(1, 1, this, codeletCounter);
        checkInCodelets1407Ptr++;
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets1406Ptr) = _checkInCodelets1406(2, 1, this, codeletCounter);
#else
        (*checkInCodelets1406Ptr) = _checkInCodelets1406(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets1406Ptr).decDep();
        checkInCodelets1406Ptr++;
    }
    if (this->numThreads == 1) {
        this->nextCodeletsmanual_omp_module4[0] = in_mainNextCodelet;
        this->nextSyncCodeletsmanual_omp_module4[0] = in_mainSyncCodelet;
        this->startPidx_darts34[0] = in_startPidx;
        this->endPidx_darts34[0] = in_endPidx;
        this->availableCodelets[0] = 1;
    } else {
        this->startPidx_darts34[this->mainCodeletID] = in_startPidx;
        this->endPidx_darts34[this->mainCodeletID] = in_endPidx;
        this->nextCodeletsmanual_omp_module4[in_mainCodeletID] = in_mainNextCodelet;
        this->nextSyncCodeletsmanual_omp_module4[in_mainCodeletID] = in_mainSyncCodelet;
#if USE_SPIN_CODELETS == 0
        this->firstCodelet[this->mainCodeletID].decDep();
#else
        this->availableCodelets[this->mainCodeletID] = 1;
#endif
        *(this->ptrToThisFunctionTP) = this;
    }
}
TP34::~TP34()
{
    delete[] checkInCodelets1444;
    delete[] barrierCodelets1440;
    delete[] checkInCodelets1440;
    delete[] barrierCodelets1439;
    delete[] checkInCodelets1433;
    delete[] barrierCodelets1429;
    delete[] checkInCodelets1429;
    delete[] barrierCodelets1428;
    delete[] checkInCodelets1422;
    delete[] barrierCodelets1418;
    delete[] checkInCodelets1418;
    delete[] barrierCodelets1417;
    delete[] checkInCodelets1411;
    delete[] barrierCodelets1407;
    delete[] checkInCodelets1407;
    delete[] barrierCodelets1406;
    delete[] checkInCodelets1406;
    delete[] nextCodeletsmanual_omp_module4;
    delete[] nextSyncCodeletsmanual_omp_module4;
    delete[] startPidx_darts34;
    delete[] endPidx_darts34;
    delete[] pidx_darts34;
}
void TP34::setNewInputs(int in_startPidx, int in_endPidx, size_t codeletID)
{
    this->startPidx_darts34[codeletID] = in_startPidx;
    this->endPidx_darts34[codeletID] = in_endPidx;
}
/*TP1451: OMPParallelDirective*/
void TP1451::_barrierCodelets1451::fire(void)
{
    TP1451* myTP = static_cast<TP1451*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
void TP1451::_checkInCodelets1453::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 1453: DeclStmt*/

    /*printing node 1454: DeclStmt*/

    /*printing node 1455: DeclStmt*/

    /*printing node 1456: DeclStmt*/
    this->inputsTPParent->thread_id_darts1451[this->getID()] = omp_get_thread_num();

    /*printing node 1458: DeclStmt*/
    this->inputsTPParent->numThreads_darts1451[this->getID()] = omp_get_num_threads();

    /*printing node 1460: BinaryOperator*/
    (this->inputsTPParent->dparts_per_thread_darts1451[this->getID()]) = ((double)(CLOMP_numParts))
        / ((double)((this->inputsTPParent->numThreads_darts1451[this->getID()])));

    /*printing node 1464: IfStmt*/
    if ((this->inputsTPParent->dparts_per_thread_darts1451[this->getID()]) < 1.) {
        (this->inputsTPParent->dparts_per_thread_darts1451[this->getID()]) = 1.;
    }

    /*printing node 1469: BinaryOperator*/
    (this->inputsTPParent->startPidx_darts1451[this->getID()])
        = (int)nearbyint(((double)(this->inputsTPParent->thread_id_darts1451[this->getID()]))
            * (this->inputsTPParent->dparts_per_thread_darts1451[this->getID()]));

    /*printing node 1474: BinaryOperator*/
    (this->inputsTPParent->endPidx_darts1451[this->getID()])
        = (int)nearbyint(((double)(this->inputsTPParent->thread_id_darts1451[this->getID()]) + 1)
              * (this->inputsTPParent->dparts_per_thread_darts1451[this->getID()]))
        - 1;

    /*printing node 1481: IfStmt*/
    if ((this->inputsTPParent->endPidx_darts1451[this->getID()]) >= CLOMP_numParts) {
        (this->inputsTPParent->endPidx_darts1451[this->getID()]) = CLOMP_numParts - 1;
    }

    /*printing node 1486: IfStmt*/
    if ((this->inputsTPParent->startPidx_darts1451[this->getID()]) >= CLOMP_numParts) {
        printf("*** No parts available for thread %i\n",
            (this->inputsTPParent->thread_id_darts1451[this->getID()]));
    }

    /*printing node 1491: BinaryOperator*/
    (this->inputsTPParent->iteration_darts1451[this->getID()]) = 0;

    /*printing node 1492: BinaryOperator*/
    /*Print the code for a condition node in a complex loop stmt */
    if ((this->inputsTPParent->iteration_darts1451[this->getID()])
        < (*(this->inputsTPParent->num_iterations_darts1451))) {
        /*Signal the first codelet in the loop*/
        myTP->checkInCodelets1490[this->getID()].decDep();
        return;
    }
    /*Signal the codelet after the loop from the end's condional node.*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets1451[0].decDep();
}
void TP1451::_checkInCodelets1490::fire(void)
{

    /*printing node 1490: ForStmt*/
    bool haveToLaunch = __sync_bool_compare_and_swap(&(myTP->controlTPParent->TP1490_LoopCounter),
        myTP->controlTPParent->TP1490_LoopCounterPerThread[this->getID()],
        myTP->controlTPParent->TP1490_LoopCounterPerThread[this->getID()] + 1);
    unsigned int iterIdx = myTP->controlTPParent->TP1490_LoopCounterPerThread[this->getID()];
    if (haveToLaunch) {
        this->resetCodelet();
        myTP->controlTPParent->TP1490PtrVec.push_back(nullptr);
        myTP->controlTPParent->TP1490_LoopCounterPerThread[this->getID()] += 1;
        invoke<TP1490>(myTP, myTP->numThreads, this->getID(), myTP, myTP->inputsTPParent,
            &(myTP->controlTPParent->TP1490PtrVec.back()));
    } else {
        if (myTP->controlTPParent->TP1490PtrVec.size() == 0) {
            this->resetCodelet();
            this->decDep();
            return;
        } else if (myTP->controlTPParent->TP1490PtrVec.size() < (iterIdx + 1)) {
            this->resetCodelet();
            this->decDep();
            return;
        } else if (myTP->controlTPParent->TP1490PtrVec[iterIdx] == nullptr) {
            this->resetCodelet();
            this->decDep();
            return;
        } else {
            this->resetCodelet();
#if USE_SPIN_CODELETS == 0
            myTP->controlTPParent->TP1490PtrVec[iterIdx]->firstCodelet[this->getID()].decDep();
#else
            myTP->controlTPParent->TP1490PtrVec[iterIdx]->availableCodelets[this->getID()] = 1;
#endif
            myTP->controlTPParent->TP1490_LoopCounterPerThread[this->getID()] += 1;
        }
    }
}
void TP1451::_checkInCodelets1493::fire(void)
{

    /*printing node 1493: UnaryOperator*/
    (this->inputsTPParent->iteration_darts1451[this->getID()])++;

    /*printing node 2336: BinaryOperator*/
    /*Print the code for a condition node in a complex loop stmt */
    if ((this->inputsTPParent->iteration_darts1451[this->getID()])
        < (*(this->inputsTPParent->num_iterations_darts1451))) {
        this->resetCodelet();
        /*Signal the first codelet in the loop*/
        myTP->checkInCodelets1490[this->getID()].decDep();
        return;
    }
    /*Signal the codelet after the loop from the condtional node.*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets1451[0].decDep();
}
TP1451::TP1451(
    int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet, long* in_num_iterations)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , num_iterations_darts1451(in_num_iterations) /*OMP_SHARED - INPUT*/
    , dparts_per_thread_darts1451(new double[this->numThreads]) /*VARIABLE*/
    , endPidx_darts1451(new int[this->numThreads]) /*VARIABLE*/
    , iteration_darts1451(new long[this->numThreads]) /*VARIABLE*/
    , numThreads_darts1451(new int[this->numThreads]) /*VARIABLE*/
    , startPidx_darts1451(new int[this->numThreads]) /*VARIABLE*/
    , thread_id_darts1451(new int[this->numThreads]) /*VARIABLE*/
    , TP1490_LoopCounter(0)
    , TP1490_LoopCounterPerThread(new unsigned int[this->numThreads])
    , barrierCodelets1451(new _barrierCodelets1451[1])
    , checkInCodelets1453(new _checkInCodelets1453[this->numThreads])
    , checkInCodelets1490(new _checkInCodelets1490[this->numThreads])
    , checkInCodelets1493(new _checkInCodelets1493[this->numThreads])
{
    memset((void*)TP1490_LoopCounterPerThread, 0, this->numThreads * sizeof(unsigned int));
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets1451[0] = _barrierCodelets1451(ompNumThreads, ompNumThreads, this, 0);
    _checkInCodelets1493* checkInCodelets1493Ptr = (this->checkInCodelets1493);
    _checkInCodelets1490* checkInCodelets1490Ptr = (this->checkInCodelets1490);
    _checkInCodelets1453* checkInCodelets1453Ptr = (this->checkInCodelets1453);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets1493Ptr) = _checkInCodelets1493(1, 1, this, codeletCounter);
        checkInCodelets1493Ptr++;
        (*checkInCodelets1490Ptr) = _checkInCodelets1490(1, 1, this, codeletCounter);
        checkInCodelets1490Ptr++;
        (*checkInCodelets1453Ptr) = _checkInCodelets1453(1, 1, this, codeletCounter);
        (*checkInCodelets1453Ptr).decDep();
        checkInCodelets1453Ptr++;
    }
}
TP1451::~TP1451()
{
    delete[] TP1490_LoopCounterPerThread;
    delete[] dparts_per_thread_darts1451;
    delete[] endPidx_darts1451;
    delete[] iteration_darts1451;
    delete[] numThreads_darts1451;
    delete[] startPidx_darts1451;
    delete[] thread_id_darts1451;
    delete[] barrierCodelets1451;
    delete[] checkInCodelets1493;
    delete[] checkInCodelets1490;
    delete[] checkInCodelets1453;
}
/*TP1490: ForStmt*/
void TP1490::_checkInCodelets1495::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif

    /*printing node 1495: CallExpr*/
    if (!__sync_val_compare_and_swap(&(myTP->controlTPParent->TP1495_alreadyLaunched), 0, 1)) {
/*Finding the next codelet after the function call*/
#if 0
/*Signaling next codelet region: 1495 nextRegion: 1496 */
myTP->controlTPParent->checkInCodelets1496[this->getID()].decDep();
#endif
/*Finding the next sync point before making the function call*/
#if 0
/*Find the sync point for a function call*/
/*syncNode: OMPParallelDirective 1451*/
/*Find the TP to which the nextRegion belongs*/
#endif
        /*Make the function call*/
        invoke<TP_manual_omp_module1>(myTP, myTP->numThreads, this->getID(),
            &(myTP->controlTPParent->checkInCodelets1496[this->getID()]),
            &(myTP->controlTPParent->TPParent->barrierCodelets1451[0]),
            &(myTP->controlTPParent->TP1495Ptr),
            (this->inputsTPParent->startPidx_darts1451[this->getID()]),
            (this->inputsTPParent->endPidx_darts1451[this->getID()]));
    } else {
        if (myTP->controlTPParent->TP1495Ptr == nullptr) {
            myTP->add(this);
            return;
        } else {
            myTP->controlTPParent->TP1495Ptr->setNewInputs(
                (this->inputsTPParent->startPidx_darts1451[this->getID()]),
                (this->inputsTPParent->endPidx_darts1451[this->getID()]), this->getID());
            myTP->controlTPParent->TP1495Ptr->nextCodeletsmanual_omp_module1[this->getID()]
                = &(myTP->controlTPParent->checkInCodelets1496[this->getID()]);
            myTP->controlTPParent->TP1495Ptr->nextSyncCodeletsmanual_omp_module1[this->getID()]
                = &(myTP->controlTPParent->TPParent->barrierCodelets1451[0]);
#if USE_SPIN_CODELETS == 0
            myTP->controlTPParent->TP1495Ptr->firstCodelet[this->getID()].decDep();
#else
            myTP->controlTPParent->TP1495Ptr->availableCodelets[this->getID()] = 1;
#endif
        }
    }
}
void TP1490::_checkInCodelets1496::fire(void)
{

    /*printing node 1496: CallExpr*/
    if (!__sync_val_compare_and_swap(&(myTP->controlTPParent->TP1496_alreadyLaunched), 0, 1)) {
/*Finding the next codelet after the function call*/
#if 0
/*Signaling next codelet region: 1496 nextRegion: 1497 */
myTP->controlTPParent->checkInCodelets1497[this->getID()].decDep();
#endif
/*Finding the next sync point before making the function call*/
#if 0
/*Find the sync point for a function call*/
/*syncNode: OMPParallelDirective 1451*/
/*Find the TP to which the nextRegion belongs*/
#endif
        /*Make the function call*/
        invoke<TP_manual_omp_module2>(myTP, myTP->numThreads, this->getID(),
            &(myTP->controlTPParent->checkInCodelets1497[this->getID()]),
            &(myTP->controlTPParent->TPParent->barrierCodelets1451[0]),
            &(myTP->controlTPParent->TP1496Ptr),
            (this->inputsTPParent->startPidx_darts1451[this->getID()]),
            (this->inputsTPParent->endPidx_darts1451[this->getID()]));
    } else {
        if (myTP->controlTPParent->TP1496Ptr == nullptr) {
            myTP->add(this);
            return;
        } else {
            myTP->controlTPParent->TP1496Ptr->setNewInputs(
                (this->inputsTPParent->startPidx_darts1451[this->getID()]),
                (this->inputsTPParent->endPidx_darts1451[this->getID()]), this->getID());
            myTP->controlTPParent->TP1496Ptr->nextCodeletsmanual_omp_module2[this->getID()]
                = &(myTP->controlTPParent->checkInCodelets1497[this->getID()]);
            myTP->controlTPParent->TP1496Ptr->nextSyncCodeletsmanual_omp_module2[this->getID()]
                = &(myTP->controlTPParent->TPParent->barrierCodelets1451[0]);
#if USE_SPIN_CODELETS == 0
            myTP->controlTPParent->TP1496Ptr->firstCodelet[this->getID()].decDep();
#else
            myTP->controlTPParent->TP1496Ptr->availableCodelets[this->getID()] = 1;
#endif
        }
    }
}
void TP1490::_checkInCodelets1497::fire(void)
{

    /*printing node 1497: CallExpr*/
    if (!__sync_val_compare_and_swap(&(myTP->controlTPParent->TP1497_alreadyLaunched), 0, 1)) {
/*Finding the next codelet after the function call*/
#if 0
/*Signaling next codelet region: 1497 nextRegion: 1498 */
myTP->controlTPParent->checkInCodelets1498[this->getID()].decDep();
#endif
/*Finding the next sync point before making the function call*/
#if 0
/*Find the sync point for a function call*/
/*syncNode: OMPParallelDirective 1451*/
/*Find the TP to which the nextRegion belongs*/
#endif
        /*Make the function call*/
        invoke<TP_manual_omp_module3>(myTP, myTP->numThreads, this->getID(),
            &(myTP->controlTPParent->checkInCodelets1498[this->getID()]),
            &(myTP->controlTPParent->TPParent->barrierCodelets1451[0]),
            &(myTP->controlTPParent->TP1497Ptr),
            (this->inputsTPParent->startPidx_darts1451[this->getID()]),
            (this->inputsTPParent->endPidx_darts1451[this->getID()]));
    } else {
        if (myTP->controlTPParent->TP1497Ptr == nullptr) {
            myTP->add(this);
            return;
        } else {
            myTP->controlTPParent->TP1497Ptr->setNewInputs(
                (this->inputsTPParent->startPidx_darts1451[this->getID()]),
                (this->inputsTPParent->endPidx_darts1451[this->getID()]), this->getID());
            myTP->controlTPParent->TP1497Ptr->nextCodeletsmanual_omp_module3[this->getID()]
                = &(myTP->controlTPParent->checkInCodelets1498[this->getID()]);
            myTP->controlTPParent->TP1497Ptr->nextSyncCodeletsmanual_omp_module3[this->getID()]
                = &(myTP->controlTPParent->TPParent->barrierCodelets1451[0]);
#if USE_SPIN_CODELETS == 0
            myTP->controlTPParent->TP1497Ptr->firstCodelet[this->getID()].decDep();
#else
            myTP->controlTPParent->TP1497Ptr->availableCodelets[this->getID()] = 1;
#endif
        }
    }
}
void TP1490::_checkInCodelets1498::fire(void)
{

    /*printing node 1498: CallExpr*/
    if (!__sync_val_compare_and_swap(&(myTP->controlTPParent->TP1498_alreadyLaunched), 0, 1)) {
/*Finding the next codelet after the function call*/
#if 0
/*The node is the last one in a complex loop, so signal the inc node*/
/*Find the TP to which the nextRegion belongs*/
myTP->controlTPParent->TPParent->checkInCodelets1493[this->getID()].decDep();
#endif
/*Finding the next sync point before making the function call*/
#if 0
/*Find the sync point for a function call*/
/*syncNode: OMPParallelDirective 1451*/
/*Find the TP to which the nextRegion belongs*/
#endif
        /*Make the function call*/
        invoke<TP_manual_omp_module4>(myTP, myTP->numThreads, this->getID(),
            &(myTP->controlTPParent->TPParent->checkInCodelets1493[this->getID()]),
            &(myTP->controlTPParent->TPParent->barrierCodelets1451[0]),
            &(myTP->controlTPParent->TP1498Ptr),
            (this->inputsTPParent->startPidx_darts1451[this->getID()]),
            (this->inputsTPParent->endPidx_darts1451[this->getID()]));
    } else {
        if (myTP->controlTPParent->TP1498Ptr == nullptr) {
            myTP->add(this);
            return;
        } else {
            myTP->controlTPParent->TP1498Ptr->setNewInputs(
                (this->inputsTPParent->startPidx_darts1451[this->getID()]),
                (this->inputsTPParent->endPidx_darts1451[this->getID()]), this->getID());
            myTP->controlTPParent->TP1498Ptr->nextCodeletsmanual_omp_module4[this->getID()]
                = &(myTP->controlTPParent->TPParent->checkInCodelets1493[this->getID()]);
            myTP->controlTPParent->TP1498Ptr->nextSyncCodeletsmanual_omp_module4[this->getID()]
                = &(myTP->controlTPParent->TPParent->barrierCodelets1451[0]);
#if USE_SPIN_CODELETS == 0
            myTP->controlTPParent->TP1498Ptr->firstCodelet[this->getID()].decDep();
#else
            myTP->controlTPParent->TP1498Ptr->availableCodelets[this->getID()] = 1;
#endif
        }
    }
}
TP1490::TP1490(int in_numThreads, int in_mainCodeletID, TP1451* in_TPParent,
    TP1451* in_inputsTPParent, TP1490** in_ptrToThisTP)
    : ompTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(in_inputsTPParent)
    , ptrToThisTP(in_ptrToThisTP)
    , TP1495Ptr(nullptr)
    , TP1495_alreadyLaunched(0)
    , TP1496Ptr(nullptr)
    , TP1496_alreadyLaunched(0)
    , TP1497Ptr(nullptr)
    , TP1497_alreadyLaunched(0)
    , TP1498Ptr(nullptr)
    , TP1498_alreadyLaunched(0)
    , checkInCodelets1495(new _checkInCodelets1495[this->numThreads])
    , checkInCodelets1496(new _checkInCodelets1496[this->numThreads])
    , checkInCodelets1497(new _checkInCodelets1497[this->numThreads])
    , checkInCodelets1498(new _checkInCodelets1498[this->numThreads])
{
    /*Initialize Codelets*/
    _checkInCodelets1498* checkInCodelets1498Ptr = (this->checkInCodelets1498);
    _checkInCodelets1497* checkInCodelets1497Ptr = (this->checkInCodelets1497);
    _checkInCodelets1496* checkInCodelets1496Ptr = (this->checkInCodelets1496);
    _checkInCodelets1495* checkInCodelets1495Ptr = (this->checkInCodelets1495);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets1495);
#endif
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets1498Ptr) = _checkInCodelets1498(1, 1, this, codeletCounter);
        checkInCodelets1498Ptr++;
        (*checkInCodelets1497Ptr) = _checkInCodelets1497(1, 1, this, codeletCounter);
        checkInCodelets1497Ptr++;
        (*checkInCodelets1496Ptr) = _checkInCodelets1496(1, 1, this, codeletCounter);
        checkInCodelets1496Ptr++;
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets1495Ptr) = _checkInCodelets1495(2, 1, this, codeletCounter);
#else
        (*checkInCodelets1495Ptr) = _checkInCodelets1495(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets1495Ptr).decDep();
        checkInCodelets1495Ptr++;
    }
    *(this->ptrToThisTP) = this;
#if USE_SPIN_CODELETS == 0
    this->firstCodelet[this->getID()].decDep();
#else
    this->availableCodelets[this->getID()] = 1;
#endif
}
TP1490::~TP1490()
{
    delete[] checkInCodelets1498;
    delete[] checkInCodelets1497;
    delete[] checkInCodelets1496;
    delete[] checkInCodelets1495;
}
/*TP1579: OMPParallelDirective*/
void TP1579::_barrierCodelets1579::fire(void)
{
    TP1579* myTP = static_cast<TP1579*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
void TP1579::_checkInCodelets1581::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 1581: DeclStmt*/

    /*printing node 1582: DeclStmt*/

    /*printing node 1583: DeclStmt*/

    /*printing node 1584: DeclStmt*/
    this->inputsTPParent->thread_id_darts1579[this->getID()] = omp_get_thread_num();

    /*printing node 1586: DeclStmt*/
    this->inputsTPParent->numThreads_darts1579[this->getID()] = omp_get_num_threads();

    /*printing node 1588: BinaryOperator*/
    (this->inputsTPParent->dparts_per_thread_darts1579[this->getID()]) = ((double)(CLOMP_numParts))
        / ((double)((this->inputsTPParent->numThreads_darts1579[this->getID()])));

    /*printing node 1592: IfStmt*/
    if ((this->inputsTPParent->dparts_per_thread_darts1579[this->getID()]) < 1.) {
        (this->inputsTPParent->dparts_per_thread_darts1579[this->getID()]) = 1.;
    }

    /*printing node 1597: BinaryOperator*/
    (this->inputsTPParent->startPidx_darts1579[this->getID()])
        = (int)nearbyint(((double)(this->inputsTPParent->thread_id_darts1579[this->getID()]))
            * (this->inputsTPParent->dparts_per_thread_darts1579[this->getID()]));

    /*printing node 1602: BinaryOperator*/
    (this->inputsTPParent->endPidx_darts1579[this->getID()])
        = (int)nearbyint(((double)(this->inputsTPParent->thread_id_darts1579[this->getID()]) + 1)
              * (this->inputsTPParent->dparts_per_thread_darts1579[this->getID()]))
        - 1;

    /*printing node 1609: IfStmt*/
    if ((this->inputsTPParent->endPidx_darts1579[this->getID()]) >= CLOMP_numParts) {
        (this->inputsTPParent->endPidx_darts1579[this->getID()]) = CLOMP_numParts - 1;
    }

    /*printing node 1614: IfStmt*/
    if ((this->inputsTPParent->startPidx_darts1579[this->getID()]) >= CLOMP_numParts) {
        printf("*** No parts available for thread %i\n",
            (this->inputsTPParent->thread_id_darts1579[this->getID()]));
    }

    /*printing node 1618: ForStmt*/
    {
        double* deposit = (this->inputsTPParent->deposit_darts1579);
        (void)deposit /*OMP_SHARED*/;
        long* num_iterations = (this->inputsTPParent->num_iterations_darts1579);
        (void)num_iterations /*OMP_SHARED*/;
        int* endPidx = &(this->inputsTPParent->endPidx_darts1579[this->getLocalID()]);
        (void)endPidx /*PRIVATE*/;
        long* iteration = &(this->inputsTPParent->iteration_darts1579[this->getLocalID()]);
        (void)iteration /*PRIVATE*/;
        int* startPidx = &(this->inputsTPParent->startPidx_darts1579[this->getLocalID()]);
        (void)startPidx /*PRIVATE*/;
        /*Loop's init*/
        (this->inputsTPParent->iteration_darts1579[this->getID()]) = 0;
        long iteration_darts_counter_temp1579
            = (this->inputsTPParent->iteration_darts1579[this->getID()]);
        for (; (iteration_darts_counter_temp1579) < (*(num_iterations));
             (iteration_darts_counter_temp1579)++) {
            bestcase_omp_cycle((*startPidx), (*endPidx), (*(deposit)));
        }
        (this->inputsTPParent->iteration_darts1579[this->getID()])
            = iteration_darts_counter_temp1579;
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets1579[0].decDep();
}
TP1579::TP1579(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet, double* in_deposit,
    long* in_num_iterations)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , deposit_darts1579(in_deposit) /*OMP_SHARED - INPUT*/
    , num_iterations_darts1579(in_num_iterations) /*OMP_SHARED - INPUT*/
    , dparts_per_thread_darts1579(new double[this->numThreads]) /*VARIABLE*/
    , endPidx_darts1579(new int[this->numThreads]) /*VARIABLE*/
    , iteration_darts1579(new long[this->numThreads]) /*VARIABLE*/
    , numThreads_darts1579(new int[this->numThreads]) /*VARIABLE*/
    , startPidx_darts1579(new int[this->numThreads]) /*VARIABLE*/
    , thread_id_darts1579(new int[this->numThreads]) /*VARIABLE*/
    , barrierCodelets1579(new _barrierCodelets1579[1])
    , checkInCodelets1581(new _checkInCodelets1581[this->numThreads])
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets1579[0] = _barrierCodelets1579(ompNumThreads, ompNumThreads, this, 0);
    _checkInCodelets1581* checkInCodelets1581Ptr = (this->checkInCodelets1581);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets1581Ptr) = _checkInCodelets1581(1, 1, this, codeletCounter);
        (*checkInCodelets1581Ptr).decDep();
        checkInCodelets1581Ptr++;
    }
}
TP1579::~TP1579()
{
    delete[] dparts_per_thread_darts1579;
    delete[] endPidx_darts1579;
    delete[] iteration_darts1579;
    delete[] numThreads_darts1579;
    delete[] startPidx_darts1579;
    delete[] thread_id_darts1579;
    delete[] barrierCodelets1579;
    delete[] checkInCodelets1581;
}
/*TP1860: OMPParallelForDirective*/
void TP1860::_barrierCodelets1860::fire(void)
{
    TP1860* myTP = static_cast<TP1860*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
bool TP1860::requestNewRangeIterations1860(long* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Static */
    /*Chunk = 1*/
    bool isThereNewIteration = false;
    {
        /*Static Scheduling*/
        long tempStartRange = rangePerCodelet1860 * codeletID;
        long tempEndRange = rangePerCodelet1860 * (codeletID + 1);
        if (remainderRange1860 != 0) {
            if (codeletID < (uint32_t)remainderRange1860) {
                tempStartRange += codeletID;
                tempEndRange += (codeletID + 1);
            } else {
                tempStartRange += remainderRange1860;
                tempEndRange += remainderRange1860;
            }
        }
        tempStartRange = tempStartRange * 1 + minIteration1860;
        tempEndRange = tempEndRange * 1 + minIteration1860;
        if (tempStartRange != tempEndRange) {
            isThereNewIteration = true;
        }
        if (initIteration1860 < lastIteration1860) {
            (this->inputsTPParent->partId_darts1860[codeletID]) = min(tempStartRange, tempEndRange);
            *endRange = max(tempStartRange, tempEndRange);
        } else {
            (this->inputsTPParent->partId_darts1860[codeletID]) = max(tempStartRange, tempEndRange);
            *endRange = min(tempStartRange, tempEndRange);
        }
        if (codeletID == this->numThreads - 1) {
            *endRange = lastIteration1860;
        }
    }
    return isThereNewIteration;
}
void TP1860::_checkInCodelets1861::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 1861: ForStmt*/
    /*var: partId*/
    long* partId = &(this->inputsTPParent->partId_darts1860[this->getLocalID()]);
    (void)partId /*OMP_PRIVATE*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations1860(
        (long*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        /*Find and signal the next codelet*/
        myTP->controlTPParent->TPParent->barrierCodelets1860[0].decDep();
        return;
    }
    for (long partId_darts_counter_temp1860 = (*partId); partId_darts_counter_temp1860 < endRange
         && partId_darts_counter_temp1860 < this->inputsTPParent->lastIteration1860;
         partId_darts_counter_temp1860++) {
        {
            Part* part;
            if ((part = (Part*)malloc(sizeof(Part))) == (Part*)((void*)0)) {
                fprintf(stderr, "Out of memory allocating part\n");
                exit(1);
            }
            addPart(part, (partId_darts_counter_temp1860));
        }
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets1860[0].decDep();
}
TP1860::TP1860(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet,
    long in_initIteration, long in_lastIteration)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , partId_darts1860(new long[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , initIteration1860(in_initIteration)
    , lastIteration1860(in_lastIteration)
    , barrierCodelets1860(new _barrierCodelets1860[1])
    , checkInCodelets1861(new _checkInCodelets1861[this->numThreads])
{
    /*Initialize the loop parameters*/
    range1860 = abs(lastIteration1860 - initIteration1860) / 1;
    rangePerCodelet1860 = range1860 / numThreads;
    minIteration1860 = min<long>(lastIteration1860, initIteration1860);
    remainderRange1860 = range1860 % numThreads;
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets1860[0] = _barrierCodelets1860(ompNumThreads, ompNumThreads, this, 0);
    _checkInCodelets1861* checkInCodelets1861Ptr = (this->checkInCodelets1861);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets1861Ptr) = _checkInCodelets1861(1, 1, this, codeletCounter);
        (*checkInCodelets1861Ptr).decDep();
        checkInCodelets1861Ptr++;
    }
}
TP1860::~TP1860()
{
    delete[] barrierCodelets1860;
    delete[] checkInCodelets1861;
}
/*TP1897: OMPParallelForDirective*/
void TP1897::_barrierCodelets1897::fire(void)
{
    TP1897* myTP = static_cast<TP1897*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
bool TP1897::requestNewRangeIterations1897(long* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Static */
    /*Chunk = 1*/
    bool isThereNewIteration = false;
    {
        /*Static Scheduling*/
        long tempStartRange = rangePerCodelet1897 * codeletID;
        long tempEndRange = rangePerCodelet1897 * (codeletID + 1);
        if (remainderRange1897 != 0) {
            if (codeletID < (uint32_t)remainderRange1897) {
                tempStartRange += codeletID;
                tempEndRange += (codeletID + 1);
            } else {
                tempStartRange += remainderRange1897;
                tempEndRange += remainderRange1897;
            }
        }
        tempStartRange = tempStartRange * 1 + minIteration1897;
        tempEndRange = tempEndRange * 1 + minIteration1897;
        if (tempStartRange != tempEndRange) {
            isThereNewIteration = true;
        }
        if (initIteration1897 < lastIteration1897) {
            (this->inputsTPParent->partId_darts1897[codeletID]) = min(tempStartRange, tempEndRange);
            *endRange = max(tempStartRange, tempEndRange);
        } else {
            (this->inputsTPParent->partId_darts1897[codeletID]) = max(tempStartRange, tempEndRange);
            *endRange = min(tempStartRange, tempEndRange);
        }
        if (codeletID == this->numThreads - 1) {
            *endRange = lastIteration1897;
        }
    }
    return isThereNewIteration;
}
void TP1897::_checkInCodelets1898::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 1898: ForStmt*/
    /*var: partId*/
    long* partId = &(this->inputsTPParent->partId_darts1897[this->getLocalID()]);
    (void)partId /*OMP_PRIVATE*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations1897(
        (long*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        /*Find and signal the next codelet*/
        myTP->controlTPParent->TPParent->barrierCodelets1897[0].decDep();
        return;
    }
    for (long partId_darts_counter_temp1897 = (*partId); partId_darts_counter_temp1897 < endRange
         && partId_darts_counter_temp1897 < this->inputsTPParent->lastIteration1897;
         partId_darts_counter_temp1897++) {
        {
            Zone* zoneArray;
            Zone* zone;
            int zoneId;
            zoneArray = (Zone*)malloc(CLOMP_zoneSize * CLOMP_zonesPerPart);
            if (zoneArray == (Zone*)((void*)0)) {
                fprintf(stderr, "Out of memory allocate zone array\n");
                exit(1);
            }
            {
                /*Loop's init*/
                zoneId = 0;
                for (; zoneId < CLOMP_zonesPerPart; zoneId++) {
                    zone = &zoneArray[zoneId];
                    addZone(partArray[(partId_darts_counter_temp1897)], zone);
                }
            }
        }
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets1897[0].decDep();
}
TP1897::TP1897(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet,
    long in_initIteration, long in_lastIteration)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , partId_darts1897(new long[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , initIteration1897(in_initIteration)
    , lastIteration1897(in_lastIteration)
    , barrierCodelets1897(new _barrierCodelets1897[1])
    , checkInCodelets1898(new _checkInCodelets1898[this->numThreads])
{
    /*Initialize the loop parameters*/
    range1897 = abs(lastIteration1897 - initIteration1897) / 1;
    rangePerCodelet1897 = range1897 / numThreads;
    minIteration1897 = min<long>(lastIteration1897, initIteration1897);
    remainderRange1897 = range1897 % numThreads;
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets1897[0] = _barrierCodelets1897(ompNumThreads, ompNumThreads, this, 0);
    _checkInCodelets1898* checkInCodelets1898Ptr = (this->checkInCodelets1898);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets1898Ptr) = _checkInCodelets1898(1, 1, this, codeletCounter);
        (*checkInCodelets1898Ptr).decDep();
        checkInCodelets1898Ptr++;
    }
}
TP1897::~TP1897()
{
    delete[] barrierCodelets1897;
    delete[] checkInCodelets1898;
}
