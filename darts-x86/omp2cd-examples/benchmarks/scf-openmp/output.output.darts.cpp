#include "output.output.darts.h"
using namespace darts;
using namespace std;
double zero_darts6;
bool zero_darts6_initFlag;
int kcol_darts6;
bool kcol_darts6_initFlag;
/*Function: output, ID: 6*/
void output(
    double* z, int rowlow, int rowhi, int collow, int colhi, int rowdim, int coldim, int nctl)
{
    /*output:6*/
    /*CompoundStmt:3949*/
    static int kcol = 8;
    static double zero = 0.;
    int i, k;
    int last, begin;
    for (i = rowlow; i < rowhi; i++)
        if (z[i] != zero)
            goto L15;
    printf(" zero matrix \n");
    goto L3;
L15:
    if ((rowhi < rowlow) || (colhi < collow))
        goto L3;
    last = (((colhi) <= (collow + kcol - 1)) ? (colhi) : (collow + kcol - 1));
    for (begin = collow; begin < colhi; begin += kcol) {
        for (i = begin; i < last; i++)
            printf("            col %3d ", i);
        printf("\n");
        for (k = rowlow; k < rowhi; k++) {
            if (z[k] == zero)
                continue;
            printf("row %4d %9.4f", k, z[k]);
            printf("\n");
        }
        last = ((((last + kcol)) <= (colhi)) ? ((last + kcol)) : (colhi));
    }
L3:
    printf("\n");
    return;
}
