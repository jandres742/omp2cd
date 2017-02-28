#include "bots_common.output.darts.h"
using namespace darts;
using namespace std;
/*Function: bots_error, ID: 7*/
void bots_error(int error, char* message)
{
    /*bots_error:7*/
    /*CompoundStmt:1274*/
    if (message == ((void*)0)) {
        switch (error) {
        case 0:
            fprintf(stderr, "Error (%d): %s\n", error, "Unspecified error.");
            break;
        case 1:
            fprintf(stderr, "Error (%d): %s\n", error, "Not enough memory.");
            break;
        case 2:
            fprintf(stderr, "Error (%d): %s\n", error, "Unrecognized parameter.");
            bots_print_usage();
            break;
        default:
            fprintf(stderr, "Error (%d): %s\n", error, "Invalid error code.");
            break;
        }
    } else
        fprintf(stderr, "Error (%d): %s\n", error, message);
    exit(100 + error);
}
/*Function: bots_warning, ID: 8*/
void bots_warning(int warning, char* message)
{
    /*bots_warning:8*/
    /*CompoundStmt:1297*/
    if (message == ((void*)0)) {
        switch (warning) {
        case 0:
            fprintf(stderr, "Warning (%d): %s\n", warning, "Unspecified warning.");
            break;
        default:
            fprintf(stderr, "Warning (%d): %s\n", warning, "Invalid warning code.");
            break;
        }
    } else
        fprintf(stderr, "Warning (%d): %s\n", warning, message);
}
/*Function: bots_usecs, ID: 9*/
long bots_usecs()
{
    /*bots_usecs:9*/
    /*CompoundStmt:1311*/
    struct timeval t;
    gettimeofday((struct timeval*)&t, (struct timezone*)((void*)0));
    return t.tv_sec * 1000000 + t.tv_usec;
}
/*Function: bots_get_date, ID: 10*/
void bots_get_date(char* str)
{
    /*bots_get_date:10*/
    /*CompoundStmt:1323*/
    time_t now;
    time(&now);
    strftime(str, 32, "%Y/%m/%d;%H:%M", gmtime(&now));
}
/*Function: bots_get_architecture, ID: 11*/
void bots_get_architecture(char* str)
{
    /*bots_get_architecture:11*/
    /*CompoundStmt:1330*/
    int ncpus = sysconf(_SC_NPROCESSORS_CONF);
    struct utsname architecture;
    uname(&architecture);
    snprintf(str, 256, "%s-%s;%d", architecture.sysname, architecture.machine, ncpus);
}
/*Function: bots_get_load_average, ID: 12*/
void bots_get_load_average(char* str)
{
    /*bots_get_load_average:12*/
    /*CompoundStmt:1339*/
    double loadavg[3];
    getloadavg(loadavg, 3);
    snprintf(str, 256, "%.2f;%.2f;%.2f", loadavg[0], loadavg[1], loadavg[2]);
}
/*Function: bots_print_results, ID: 13*/
void bots_print_results()
{
    /*bots_print_results:13*/
    /*CompoundStmt:1346*/
    char str_name[256];
    char str_parameters[256];
    char str_model[256];
    char str_resources[256];
    char str_result[15];
    char str_time_program[15];
    char str_time_sequential[15];
    char str_speed_up[15];
    char str_number_of_tasks[15];
    char str_number_of_tasks_per_second[15];
    char str_exec_date[256];
    char str_exec_message[256];
    char str_architecture[256];
    char str_load_avg[256];
    char str_comp_date[256];
    char str_comp_message[256];
    char str_cc[256];
    char str_cflags[256];
    char str_ld[256];
    char str_ldflags[256];
    char str_cutoff[256];
    sprintf(str_name, "%s", bots_name);
    sprintf(str_parameters, "%s", bots_parameters);
    sprintf(str_model, "%s", bots_model);
    sprintf(str_cutoff, "%s", bots_cutoff);
    sprintf(str_resources, "%s", bots_resources);
    switch (bots_result) {
    case 0:
        sprintf(str_result, "n/a");
        break;
    case 1:
        sprintf(str_result, "successful");
        break;
    case 2:
        sprintf(str_result, "UNSUCCESSFUL");
        break;
    case 3:
        sprintf(str_result, "Not requested");
        break;
    default:
        sprintf(str_result, "error");
        break;
    }
    sprintf(str_time_program, "%f", bots_time_program);
    if (bots_sequential_flag)
        sprintf(str_time_sequential, "%f", bots_time_sequential);
    else
        sprintf(str_time_sequential, "n/a");
    if (bots_sequential_flag)
        sprintf(str_speed_up, "%3.2f", bots_time_sequential / bots_time_program);
    else
        sprintf(str_speed_up, "n/a");
    sprintf(str_number_of_tasks, "%3.2f", (float)bots_number_of_tasks);
    sprintf(
        str_number_of_tasks_per_second, "%3.2f", (float)bots_number_of_tasks / bots_time_program);
    sprintf(str_exec_date, "%s", bots_exec_date);
    sprintf(str_exec_message, "%s", bots_exec_message);
    bots_get_architecture(str_architecture);
    bots_get_load_average(str_load_avg);
    sprintf(str_comp_date, "%s", bots_comp_date);
    sprintf(str_comp_message, "%s", bots_comp_message);
    sprintf(str_cc, "%s", bots_cc);
    sprintf(str_cflags, "%s", bots_cflags);
    sprintf(str_ld, "%s", bots_ld);
    sprintf(str_ldflags, "%s", bots_ldflags);
    if (bots_print_header) {
        switch (bots_output_format) {
        case 0:
            break;
        case 1:
            break;
        case 2:
            fprintf(stdout, "Benchmark;Parameters;Model;Cutoff;Resources;Result;Time;Sequential;"
                            "Speed-up;Nodes;Nodes/Sec;Exec Date;Exec Time;Exec "
                            "Message;Architecture;Processors;Load Avg-1;Load Avg-5;Load "
                            "Avg-15;Comp Date;Comp Time;Comp Message;CC;CFLAGS;LD;LDFLAGS\n");
            break;
        case 3:
            break;
        case 4:
            fprintf(stdout, "Benchmark;Parameters;Model;Cutoff;Resources;Result;Time;Sequential;"
                            "Speed-up;Nodes;Nodes/Sec;\n");
            break;
        default:
            break;
        }
    }
    switch (bots_output_format) {
    case 0:
        break;
    case 1:
        fprintf(stdout, "\n");
        fprintf(stdout, "Program             = %s\n", str_name);
        fprintf(stdout, "Parameters          = %s\n", str_parameters);
        fprintf(stdout, "Model               = %s\n", str_model);
        fprintf(stdout, "Embedded cut-off    = %s\n", str_cutoff);
        fprintf(stdout, "# of Threads        = %s\n", str_resources);
        fprintf(stdout, "Verification        = %s\n", str_result);
        fprintf(stdout, "Time Program        = %s seconds\n", str_time_program);
        if (bots_sequential_flag) {
            fprintf(stdout, "Time Sequential     = %s seconds\n", str_time_sequential);
            fprintf(stdout, "Speed-up            = %s\n", str_speed_up);
        }
        if (bots_number_of_tasks > 0) {
            fprintf(stdout, "Nodes               = %s\n", str_number_of_tasks);
            fprintf(stdout, "Nodes/Sec           = %s\n", str_number_of_tasks_per_second);
        }
        fprintf(stdout, "Execution Message   = %s\n", str_exec_message);
        fprintf(stdout, "Load Avg [1:5:15]   = %s\n", str_load_avg);
        fflush(stdout);
        break;
    case 2:
        fprintf(stdout, "%s;%s;%s;%s;%s;%s;", str_name, str_parameters, str_model, str_cutoff,
            str_resources, str_result);
        fprintf(stdout, "%s;%s;%s;", str_time_program, str_time_sequential, str_speed_up);
        fprintf(stdout, "%s;%s;", str_number_of_tasks, str_number_of_tasks_per_second);
        fprintf(stdout, "%s;%s;", str_exec_date, str_exec_message);
        fprintf(stdout, "%s;%s;", str_architecture, str_load_avg);
        fprintf(stdout, "%s;%s;", str_comp_date, str_comp_message);
        fprintf(stdout, "%s;%s;%s;%s;", str_cc, str_cflags, str_ld, str_ldflags);
        fprintf(stdout, "\n");
        break;
    case 3:
        fprintf(stdout, "\n");
        fprintf(stdout, "Program             = %s\n", str_name);
        fprintf(stdout, "Parameters          = %s\n", str_parameters);
        fprintf(stdout, "Model               = %s\n", str_model);
        fprintf(stdout, "Embedded cut-off    = %s\n", str_cutoff);
        fprintf(stdout, "# of Threads        = %s\n", str_resources);
        fprintf(stdout, "Verification        = %s\n", str_result);
        fprintf(stdout, "Time Program        = %s seconds\n", str_time_program);
        if (bots_sequential_flag) {
            fprintf(stdout, "Time Sequential     = %s seconds\n", str_time_sequential);
            fprintf(stdout, "Speed-up            = %s\n", str_speed_up);
        }
        if (bots_number_of_tasks > 0) {
            fprintf(stdout, "Nodes               = %s\n", str_number_of_tasks);
            fprintf(stdout, "Nodes/Sec           = %s\n", str_number_of_tasks_per_second);
        }
        break;
    case 4:
        fprintf(stdout, "%s;%s;%s;%s;%s;%s;", str_name, str_parameters, str_model, str_cutoff,
            str_resources, str_result);
        fprintf(stdout, "%s;%s;%s;", str_time_program, str_time_sequential, str_speed_up);
        fprintf(stdout, "%s;%s;", str_number_of_tasks, str_number_of_tasks_per_second);
        fprintf(stdout, "\n");
        break;
    default:
        bots_error(0, (char*)"No valid output format\n");
        break;
    }
}
