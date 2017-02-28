#include "yamlOutput.output.darts.h"
using namespace darts;
using namespace std;
static char* CoMDVariant = "CoMD-openmp";
static char* CoMDVersion = "1.1";
FILE* yamlFile = ((void*)0);
static void getTimeString(char* timestring);
/*Function: getTimeString, ID: 178*/
static void getTimeString(char* timestring)
{
    /*getTimeString:178*/
    /*CompoundStmt:761*/
    time_t rawtime;
    struct tm* timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    sprintf(timestring, "%4d-%02i-%02d, %02d:%02d:%02d", timeinfo->tm_year + 1900,
        timeinfo->tm_mon + 1, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min,
        timeinfo->tm_sec);
}
/*Function: yamlBegin, ID: 179*/
void yamlBegin()
{
    /*yamlBegin:179*/
    /*CompoundStmt:778*/
    if (!printRank())
        return;
    char filename[64];
    time_t rawtime;
    time(&rawtime);
    struct tm* ptm = localtime(&rawtime);
    char sdate[25];
    sprintf(sdate, "%04d:%02d:%02d-%02d:%02d:%02d", ptm->tm_year + 1900, ptm->tm_mon + 1,
        ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
    sprintf(filename, "%s.%s.yaml", CoMDVariant, sdate);
    yamlFile = fopen(filename, "w");
}
/*Function: yamlAppInfo, ID: 180*/
void yamlAppInfo(FILE* file)
{
    /*yamlAppInfo:180*/
    /*CompoundStmt:803*/
    int numThreads = omp_get_max_threads();
    if (!printRank())
        return;
    printSeparator(file);
    fprintf(file, "Mini-Application Name    : %s\n", CoMDVariant);
    fprintf(file, "Mini-Application Version : %s\n", CoMDVersion);
    fprintf(file, "Platform:\n");
    fprintf(file, "  hostname: %s\n", "g115");
    fprintf(file, "  kernel name: %s\n", "'Linux'");
    fprintf(file, "  kernel release: %s\n", "'2.6.32-573.26.1.el6.x86_64'");
    fprintf(file, "  processor: %s\n", "'x86_64'");
    fprintf(file, "Build:\n");
    fprintf(file, "  CC: %s\n", "'/opt/shared/gcc/4.9.3/bin/gcc'");
    fprintf(file, "  compiler version: %s\n", "'gcc (GCC) 4.9.3'");
    fprintf(file, "  CFLAGS: %s\n", "'-std=c99 -fopenmp -DDOUBLE -g -O5  '");
    fprintf(file, "  LDFLAGS: %s\n", "'-lm '");
    fprintf(file, "  using MPI: %s\n", builtWithMpi() ? "true" : "false");
    fprintf(file, "  Threading: OpenMP (%d threads) \n", numThreads);
    fprintf(
        file, "  Double Precision: %s\n", (sizeof(real_t) == sizeof(double) ? "true" : "false"));
    char timestring[32];
    getTimeString(timestring);
    fprintf(file, "Run Date/Time: %s\n", timestring);
    fprintf(file, "\n");
    fflush(file);
}
/*Function: yamlEnd, ID: 181*/
void yamlEnd()
{
    /*yamlEnd:181*/
    /*CompoundStmt:837*/
    if (!printRank())
        return;
    fclose(yamlFile);
}
/*Function: printSeparator, ID: 182*/
void printSeparator(FILE* file)
{
    /*printSeparator:182*/
    /*CompoundStmt:843*/
    fprintf(file, "\n");
}
