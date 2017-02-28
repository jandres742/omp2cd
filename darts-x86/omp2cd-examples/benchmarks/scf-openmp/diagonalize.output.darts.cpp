#include "diagonalize.output.darts.h"
using namespace darts;
using namespace std;
/*Function: my_pythag, ID: 29*/
double my_pythag(double a, double b)
{
    /*my_pythag:29*/
    /*CompoundStmt:895*/
    double p, r, t, s, u;
    p = (((fabs(a)) >= (fabs(b))) ? (fabs(a)) : (fabs(b)));
    if (p == 0.)
        return p;
    r = (((fabs(a)) <= (fabs(b))) ? (fabs(a)) : (fabs(b))) / p;
    r = r * r;
    while (r > 0.) {
        s = r / (4. + r);
        u = 1. + 2. * s;
        p = u * p;
        t = s / u;
        r = t * t * r;
    }
    return p;
}
/*Function: my_ga_rrreduc, ID: 30*/
int my_ga_rrreduc(int nm, int n, double* a, double* b, double* fv2)
{
    /*my_ga_rrreduc:30*/
    /*CompoundStmt:939*/
    int i, j, k;
    double x, y = 0;
    for (i = 0; i < n; i++) {
        for (j = i; j < n; j++) {
            x = b[(((i)*nbfn) + (j))];
            for (k = 0; k < i; k++)
                x = x - b[(((i)*nbfn) + (k))] * b[(((j)*nbfn) + (k))];
            if (j == i) {
                if (x <= 0.)
                    return 7 * n + 1;
                else {
                    y = sqrt(x);
                    fv2[i] = y;
                }
            } else
                b[(((j)*nbfn) + (i))] = x / y;
        }
    }
    for (i = 0; i < n; i++) {
        for (j = i; j < n; j++) {
            double x = a[(((i)*nbfn) + (j))];
            for (k = 0; k < i; k++)
                x = x - b[(((i)*nbfn) + (k))] * a[(((j)*nbfn) + (k))];
            a[(((j)*nbfn) + (i))] = x / fv2[i];
        }
    }
    for (j = 0; j < n; j++) {
        for (i = j; i < n; i++) {
            double x = a[(((i)*nbfn) + (j))];
            if (i != j)
                for (k = j; k < i; k++)
                    x = x - a[(((k)*nbfn) + (j))] * b[(((i)*nbfn) + (k))];
            if (j > 0)
                for (k = 0; k < j; k++)
                    x = x - a[(((j)*nbfn) + (k))] * b[(((i)*nbfn) + (k))];
            a[(((i)*nbfn) + (j))] = x / fv2[i];
        }
    }
    return 0;
}
/*Function: my_ga_tred2, ID: 31*/
void my_ga_tred2(int nm, int n, double* a, double* d, double* e, double* z)
{
    /*my_ga_tred2:31*/
    /*CompoundStmt:1075*/
    int i, j, k;
    double f, g, h, hh, scale;
    for (i = 0; i < n; i++) {
        for (j = i; j < n; j++)
            z[(((j)*nbfn) + (i))] = a[(((j)*nbfn) + (i))];
        for (j = 0; j < i; j++)
            z[(((j)*nbfn) + (i))] = 0.;
        d[i] = a[(((n - 1) * nbfn) + (i))];
    }
    for (i = n - 1; i >= 1; i--) {
        h = 0.;
        scale = 0.;
        if (i != 1)
            for (k = 0; k < i; k++)
                scale = scale + fabs(d[k]);
        if (scale == 0.) {
            e[i] = d[i - 1];
            for (j = 0; j < i; j++) {
                d[j] = z[(((i - 1) * nbfn) + (j))];
                z[(((i)*nbfn) + (j))] = 0.;
                z[(((j)*nbfn) + (i))] = 0.;
            }
        } else {
            for (k = 0; k < i; k++) {
                d[k] = d[k] / scale;
                h = h + d[k] * d[k];
            }
            f = d[i - 1];
            g = -copysign(sqrt(h), f);
            e[i] = scale * g;
            h = h - f * g;
            d[i - 1] = f - g;
            for (j = 0; j < i; j++)
                e[j] = 0.;
            for (j = 0; j < i; j++) {
                f = d[j];
                z[(((j)*nbfn) + (i))] = f;
                g = e[j] + z[(((j)*nbfn) + (j))] * f;
                for (k = j + 1; k < i; k++) {
                    g = g + z[(((k)*nbfn) + (j))] * d[k];
                    e[k] = e[k] + z[(((k)*nbfn) + (j))] * f;
                }
                e[j] = g;
            }
            f = 0.;
            for (j = 0; j < i; j++) {
                e[j] = e[j] / h;
                f = f + e[j] * d[j];
            }
            hh = f / (h + h);
            for (j = 0; j < i; j++)
                e[j] = e[j] - hh * d[j];
            for (j = 0; j < i; j++) {
                f = d[j];
                g = e[j];
                for (k = j; k < i; k++)
                    z[(((k)*nbfn) + (j))] = z[(((k)*nbfn) + (j))] - f * e[k] - g * d[k];
                d[j] = z[(((i - 1) * nbfn) + (j))];
                z[(((i)*nbfn) + (j))] = 0.;
            }
        }
        d[i] = h;
    }
    for (i = 1; i < n; i++) {
        z[(((n - 1) * nbfn) + (i - 1))] = z[(((i - 1) * nbfn) + (i - 1))];
        z[(((i - 1) * nbfn) + (i - 1))] = 1.;
        h = d[i];
        if (h != 0.) {
            for (k = 0; k < i; k++)
                d[k] = z[(((k)*nbfn) + (i))] / h;
            for (j = 0; j < i; j++) {
                g = 0.;
                for (k = 0; k < i; k++)
                    g = g + z[(((k)*nbfn) + (i))] * z[(((k)*nbfn) + (j))];
                for (k = 0; k < i; k++)
                    z[(((k)*nbfn) + (j))] = z[(((k)*nbfn) + (j))] - g * d[k];
            }
        }
        for (k = 0; k < i; k++)
            z[(((k)*nbfn) + (i))] = 0.;
    }
    for (i = 0; i < n; i++) {
        d[i] = z[(((n - 1) * nbfn) + (i))];
        z[(((n - 1) * nbfn) + (i))] = 0.;
    }
    z[(((n - 1) * nbfn) + (n - 1))] = 1.;
    e[0] = 0.;
}
/*Function: my_ga_tql2, ID: 32*/
int my_ga_tql2(int nm, int n, double* d, double* e, double* z)
{
    /*my_ga_tql2:32*/
    /*CompoundStmt:1414*/
    int i, ii, j, k, l, l1, l2, m;
    double c, c2, c3 = 0, dl1, el1, f, g, h, p, r, s, s2 = 0, tst1, tst2;
    if (n == 1)
        return 0;
    for (i = 1; i < n; i++)
        e[i - 1] = e[i];
    f = 0.;
    tst1 = 0.;
    e[n - 1] = 0.;
    for (l = 0; l < n; l++) {
        h = fabs(d[l]) + fabs(e[l]);
        if (tst1 < h)
            tst1 = h;
        for (m = l; m < n; m++) {
            tst2 = fabs(e[m]) + tst1;
            if (tst2 == tst1)
                break;
        }
        if (m == n)
            m--;
        if (m == l) {
            d[l] = d[l] + f;
            continue;
        }
        for (j = 0; j < 30; j++) {
            l1 = l + 1;
            l2 = l1 + 1;
            g = d[l];
            p = (d[l1] - g) / (2. * e[l]);
            r = my_pythag(p, 1.);
            d[l] = e[l] / (p + copysign(r, p));
            d[l1] = e[l] * (p + copysign(r, p));
            dl1 = d[l1];
            h = g - d[l];
            for (i = l2; i < n; i++)
                d[i] = d[i] - h;
            f = f + h;
            p = d[m];
            c = 1.;
            c2 = c;
            el1 = e[l1];
            s = 0.;
            for (i = m - 1; i >= l; i--) {
                c3 = c2;
                c2 = c;
                s2 = s;
                g = c * e[i];
                h = c * p;
                r = my_pythag(p, e[i]);
                e[i + 1] = s * r;
                s = e[i] / r;
                c = p / r;
                p = c * d[i] - s * g;
                d[i + 1] = h + s * (c * g + s * d[i]);
                for (k = 0; k < n; k++) {
                    h = z[(((k)*nbfn) + (i + 1))];
                    z[(((k)*nbfn) + (i + 1))] = s * z[(((k)*nbfn) + (i))] + c * h;
                    z[(((k)*nbfn) + (i))] = c * z[(((k)*nbfn) + (i))] - s * h;
                }
            }
            p = -s * s2 * c3 * el1 * e[l] / dl1;
            e[l] = s * p;
            d[l] = c * p;
            tst2 = tst1 + fabs(e[l]);
            if (tst2 <= tst1)
                break;
        }
        if (j == 30)
            return l;
        d[l] = d[l] + f;
    }
    for (ii = 1; ii < n; ii++) {
        i = ii - 1;
        k = ii - 1;
        p = d[i];
        for (j = ii; j < n; j++) {
            if (d[j] >= p)
                continue;
            k = j;
            p = d[j];
        }
        if (k == i)
            continue;
        d[k] = d[i];
        d[i] = p;
        for (j = 0; j < n; j++) {
            p = z[(((j)*nbfn) + (i))];
            z[(((j)*nbfn) + (i))] = z[(((j)*nbfn) + (k))];
            z[(((j)*nbfn) + (k))] = p;
        }
    }
    return 0;
}
/*Function: my_ga_rrrebak, ID: 33*/
void my_ga_rrrebak(int nm, int n, double* b, double* dl, int m, double* z)
{
    /*my_ga_rrrebak:33*/
    /*CompoundStmt:1688*/
    int i, j, k;
    double x;
    if (m == 0)
        return;
    for (j = 0; j < m; j++) {
        for (i = n - 1; i >= 0; i--) {
            x = z[(((i)*nbfn) + (j))];
            for (k = i + 1; k < n; k++)
                x = x - b[(((k)*nbfn) + (i))] * z[(((k)*nbfn) + (j))];
            z[(((i)*nbfn) + (j))] = x / dl[i];
        }
    }
}
/*Function: my_rsg, ID: 34*/
int my_rsg(int nm, int n, double* a, double* b, double* w, double* z, double* fv1, double* fv2)
{
    /*my_rsg:34*/
    /*CompoundStmt:1730*/
    if (n > nm)
        return 10 * n;
    int ierr = my_ga_rrreduc(nm, n, a, b, fv2);
    if (ierr != 0)
        return ierr;
    my_ga_tred2(nm, n, a, w, fv1, z);
    ierr = my_ga_tql2(nm, n, w, fv1, z);
    if (ierr != 0)
        return ierr;
    my_ga_rrrebak(nm, n, b, fv2, n, z);
    return ierr;
}
/*Function: my_rs, ID: 35*/
int my_rs(int nm, int n, double* a, double* w, double* z, double* fv1, double* fv2)
{
    /*my_rs:35*/
    /*CompoundStmt:1751*/
    int ierr;
    if (n > nm)
        return 10 * n;
    my_ga_tred2(nm, n, a, w, fv1, z);
    ierr = my_ga_tql2(nm, n, w, fv1, z);
    return ierr;
}
/*Function: Eigen_gen, ID: 36*/
void Eigen_gen(double* g_a, double* g_s, double* g_v, double* evals)
{
    /*Eigen_gen:36*/
    /*CompoundStmt:1762*/
    double* a = (double*)malloc(nbfn * nbfn * sizeof(double));
    double* s = (double*)malloc(nbfn * nbfn * sizeof(double));
    double* fv1 = (double*)malloc(nbfn * sizeof(double));
    double* fv2 = (double*)malloc(nbfn * sizeof(double));
    memcpy(a, g_a, nbfn * nbfn * sizeof(double));
    memcpy(s, g_s, nbfn * nbfn * sizeof(double));
    int ierr = my_rsg(nbfn, nbfn, a, s, evals, g_v, fv1, fv2);
    if (ierr != 0) {
        printf("Error code %d\n", ierr);
        exit(1);
    }
    free(a);
    free(s);
    free(fv1);
    free(fv2);
}
/*Function: Eigen_std, ID: 37*/
void Eigen_std(double* g_a, double* g_v, double* evals)
{
    /*Eigen_std:37*/
    /*CompoundStmt:1804*/
    double* a = (double*)malloc(nbfn * nbfn * sizeof(double));
    double* fv1 = (double*)malloc(nbfn * sizeof(double));
    double* fv2 = (double*)malloc(nbfn * sizeof(double));
    memcpy(a, g_a, nbfn * nbfn * sizeof(double));
    int ierr = my_rs(nbfn, nbfn, a, evals, g_v, fv1, fv2);
    if (ierr != 0) {
        printf("Error code %d\n", ierr);
        exit(1);
    }
    free(a);
    free(fv1);
    free(fv2);
}
