#include "XSutils.output.darts.h"
using namespace darts;
using namespace std;
unsigned long seed_darts24;
bool seed_darts24_initFlag;
/*Function: gpmatrix, ID: 13*/
NuclideGridPoint** gpmatrix(size_t m, size_t n)
{
    /*gpmatrix:13*/
    /*CompoundStmt:347*/
    int i, j;
    NuclideGridPoint* full = (NuclideGridPoint*)malloc(m * n * sizeof(NuclideGridPoint));
    NuclideGridPoint** M = (NuclideGridPoint**)malloc(m * sizeof(NuclideGridPoint*));
    for (i = 0, j = 0; i < m * n; i++)
        if (i % n == 0)
            M[j++] = &full[i];
    return M;
}
/*Function: gpmatrix_free, ID: 14*/
void gpmatrix_free(NuclideGridPoint** M)
{
    /*gpmatrix_free:14*/
    /*CompoundStmt:376*/
    free(*M);
    free(M);
}
/*Function: NGP_compare, ID: 15*/
int NGP_compare(const void* a, const void* b)
{
    /*NGP_compare:15*/
    /*CompoundStmt:380*/
    NuclideGridPoint *i, *j;
    i = (NuclideGridPoint*)a;
    j = (NuclideGridPoint*)b;
    if (i->energy > j->energy)
        return 1;
    else if (i->energy < j->energy)
        return -1;
    else
        return 0;
}
/*Function: logo, ID: 16*/
void logo(int version)
{
    /*logo:16*/
    /*CompoundStmt:398*/
    border_print();
    printf("                   __   __ ___________                 _                        \n     "
           "              \\ \\ / //  ___| ___ \\               | |                       \n       "
           "             \\ V / \\ `--.| |_/ / ___ _ __   ___| |__                     \n          "
           "          /   \\  `--. \\ ___ \\/ _ \\ '_ \\ / __| '_ \\                    \n         "
           "          / /^\\ \\/\\__/ / |_/ /  __/ | | | (__| | | |                   \n           "
           "        \\/   \\/\\____/\\____/ \\___|_| |_|\\___|_| |_|                   \n\n");
    border_print();
    center_print("Developed at Argonne National Laboratory", 79);
    char v[100];
    sprintf(v, "Version: %d", version);
    center_print(v, 79);
    border_print();
}
/*Function: center_print, ID: 17*/
void center_print(const char* s, int width)
{
    /*center_print:17*/
    /*CompoundStmt:407*/
    int length = strlen(s);
    int i;
    for (i = 0; i <= (width - length) / 2; i++) {
        fputs(" ", stdout);
    }
    fputs(s, stdout);
    fputs("\n", stdout);
}
/*Function: border_print, ID: 18*/
void border_print()
{
    /*border_print:18*/
    /*CompoundStmt:421*/
    printf("================================================================================\n");
}
/*Function: fancy_int, ID: 19*/
void fancy_int(int a)
{
    /*fancy_int:19*/
    /*CompoundStmt:423*/
    if (a < 1000)
        printf("%d\n", a);
    else if (a >= 1000 && a < 1000000)
        printf("%d,%03d\n", a / 1000, a % 1000);
    else if (a >= 1000000 && a < 1000000000)
        printf("%d,%03d,%03d\n", a / 1000000, (a % 1000000) / 1000, a % 1000);
    else if (a >= 1000000000)
        printf("%d,%03d,%03d,%03d\n", a / 1000000000, (a % 1000000000) / 1000000,
            (a % 1000000) / 1000, a % 1000);
    else
        printf("%d\n", a);
}
/*Function: binary_search, ID: 20*/
int binary_search(NuclideGridPoint* A, double quarry, int n)
{
    /*binary_search:20*/
    /*CompoundStmt:457*/
    int min = 0;
    int max = n - 1;
    int mid;
    if (A[0].energy > quarry)
        return 0;
    else if (A[n - 1].energy < quarry)
        return n - 2;
    while (max >= min) {
        mid = min + floor((max - min) / 2.);
        if (A[mid].energy < quarry)
            min = mid + 1;
        else if (A[mid].energy > quarry)
            max = mid - 1;
        else
            return mid;
    }
    return max;
}
/*Function: rn, ID: 21*/
double rn(unsigned long* seed)
{
    /*rn:21*/
    /*CompoundStmt:495*/
    double ret;
    unsigned long n1;
    unsigned long a = 16807;
    unsigned long m = 2147483647;
    n1 = (a * (*seed)) % m;
    *seed = n1;
    ret = (double)n1 / m;
    return ret;
}
/*Function: read_CLI, ID: 22*/
Inputs read_CLI(int argc, char* argv[])
{
    /*read_CLI:22*/
    /*CompoundStmt:510*/
    Inputs input;
    input.nthreads = omp_get_num_procs();
    input.n_isotopes = 355;
    input.n_gridpoints = 11303;
    input.lookups = 15000000;
    input.HM = (char*)malloc(6 * sizeof(char));
    input.HM[0] = 'l';
    input.HM[1] = 'a';
    input.HM[2] = 'r';
    input.HM[3] = 'g';
    input.HM[4] = 'e';
    input.HM[5] = '\x00';
    int user_g = 0;
    for (int i = 1; i < argc; i++) {
        char* arg = argv[i];
        if (strcmp(arg, "-t") == 0) {
            if (++i < argc)
                input.nthreads = atoi(argv[i]);
            else
                print_CLI_error();
        } else if (strcmp(arg, "-g") == 0) {
            if (++i < argc) {
                user_g = 1;
                input.n_gridpoints = atoi(argv[i]);
            } else
                print_CLI_error();
        } else if (strcmp(arg, "-l") == 0) {
            if (++i < argc)
                input.lookups = atoi(argv[i]);
            else
                print_CLI_error();
        } else if (strcmp(arg, "-s") == 0) {
            if (++i < argc)
                input.HM = argv[i];
            else
                print_CLI_error();
        } else
            print_CLI_error();
    }
    if (input.nthreads < 1)
        print_CLI_error();
    if (input.n_isotopes < 1)
        print_CLI_error();
    if (input.n_gridpoints < 1)
        print_CLI_error();
    if (input.lookups < 1)
        print_CLI_error();
    if (strcasecmp(input.HM, "small") != 0 && strcasecmp(input.HM, "large") != 0
        && strcasecmp(input.HM, "XL") != 0 && strcasecmp(input.HM, "XXL") != 0)
        print_CLI_error();
    if (strcasecmp(input.HM, "small") == 0)
        input.n_isotopes = 68;
    else if (strcasecmp(input.HM, "XL") == 0 && user_g == 0)
        input.n_gridpoints = 238847;
    else if (strcasecmp(input.HM, "XXL") == 0 && user_g == 0)
        input.n_gridpoints = 238847 * 2.1000000000000001;
    return input;
}
/*Function: print_CLI_error, ID: 23*/
void print_CLI_error()
{
    /*print_CLI_error:23*/
    /*CompoundStmt:652*/
    printf("Usage: ./XSBench <options>\n");
    printf("Options include:\n");
    printf("  -t <threads>     Number of OpenMP threads to run\n");
    printf("  -s <size>        Size of H-M Benchmark to run (small, large, XL, XXL)\n");
    printf("  -g <gridpoints>  Number of gridpoints per nuclide (overrides -s defaults)\n");
    printf("  -l <lookups>     Number of Cross-section (XS) lookups\n");
    printf("Default is equivalent to: -s large -l 15000000\n");
    printf("See readme for full description of default run values\n");
    exit(4);
}
/*Function: rn_v, ID: 24*/
double rn_v()
{
    /*rn_v:24*/
    /*CompoundStmt:662*/
    static unsigned long seed = 1337;
    double ret;
    unsigned long n1;
    unsigned long a = 16807;
    unsigned long m = 2147483647;
    n1 = (a * (seed)) % m;
    seed = n1;
    ret = (double)n1 / m;
    return ret;
}
/*Function: hash, ID: 25*/
unsigned int hash(unsigned char* str, int nbins)
{
    /*hash:25*/
    /*CompoundStmt:676*/
    unsigned int hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    return hash % nbins;
}
