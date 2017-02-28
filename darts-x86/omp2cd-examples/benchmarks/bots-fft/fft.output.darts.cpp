#include "fft.output.darts.h"
using namespace darts;
using namespace std;
/*Function: compute_w_coefficients_seq, ID: 16*/
void compute_w_coefficients_seq(int n, int a, int b, COMPLEX* W)
{
    /*compute_w_coefficients_seq:16*/
    /*CompoundStmt:155*/
    register double twoPiOverN;
    register int k;
    register REAL s, c;
    if (b - a < 128) {
        twoPiOverN = 2. * 3.1415926535897931 / n;
        for (k = a; k <= b; ++k) {
            c = cos(twoPiOverN * k);
            ((W[k]).re) = ((W[n - k]).re) = c;
            s = sin(twoPiOverN * k);
            ((W[k]).im) = -s;
            ((W[n - k]).im) = s;
        }
    } else {
        int ab = (a + b) / 2;
        compute_w_coefficients_seq(n, a, ab, W);
        compute_w_coefficients_seq(n, ab + 1, b, W);
    }
}
/*Function: factor, ID: 17*/
int factor(int n)
{
    /*factor:17*/
    /*CompoundStmt:200*/
    int r;
    if (n < 2)
        return 1;
    if (n == 64 || n == 128 || n == 256 || n == 1024 || n == 2048 || n == 4096)
        return 8;
    if ((n & 15) == 0)
        return 16;
    if ((n & 7) == 0)
        return 8;
    if ((n & 3) == 0)
        return 4;
    if ((n & 1) == 0)
        return 2;
    for (r = 3; r < n; r += 2)
        if (n % r == 0)
            return r;
    return n;
}
/*Function: unshuffle_seq, ID: 19*/
void unshuffle_seq(int a, int b, COMPLEX* in, COMPLEX* out, int r, int m)
{
    /*unshuffle_seq:19*/
    /*CompoundStmt:295*/
    int i, j;
    int r4 = r & (~3);
    const COMPLEX* ip;
    COMPLEX* jp;
    if (b - a < 16) {
        ip = in + a * r;
        for (i = a; i < b; ++i) {
            jp = out + i;
            for (j = 0; j < r4; j += 4) {
                jp[0] = ip[0];
                jp[m] = ip[1];
                jp[2 * m] = ip[2];
                jp[3 * m] = ip[3];
                jp += 4 * m;
                ip += 4;
            }
            for (; j < r; ++j) {
                *jp = *ip;
                ip++;
                jp += m;
            }
        }
    } else {
        int ab = (a + b) / 2;
        unshuffle_seq(a, ab, in, out, r, m);
        unshuffle_seq(ab, b, in, out, r, m);
    }
}
/*Function: fft_twiddle_gen1, ID: 20*/
void fft_twiddle_gen1(
    COMPLEX* in, COMPLEX* out, COMPLEX* W, int r, int m, int nW, int nWdnti, int nWdntm)
{
    /*fft_twiddle_gen1:20*/
    /*CompoundStmt:352*/
    int j, k;
    COMPLEX *jp, *kp;
    for (k = 0, kp = out; k < r; ++k, kp += m) {
        REAL r0, i0, rt, it, rw, iw;
        int l1 = nWdnti + nWdntm * k;
        int l0;
        r0 = i0 = 0.;
        for (j = 0, jp = in, l0 = 0; j < r; ++j, jp += m) {
            rw = ((W[l0]).re);
            iw = ((W[l0]).im);
            rt = ((*jp).re);
            it = ((*jp).im);
            r0 += rt * rw - it * iw;
            i0 += rt * iw + it * rw;
            l0 += l1;
            if (l0 > nW)
                l0 -= nW;
        }
        ((*kp).re) = r0;
        ((*kp).im) = i0;
    }
}
/*Function: fft_twiddle_gen_seq, ID: 22*/
void fft_twiddle_gen_seq(
    int i, int i1, COMPLEX* in, COMPLEX* out, COMPLEX* W, int nW, int nWdn, int r, int m)
{
    /*fft_twiddle_gen_seq:22*/
    /*CompoundStmt:433*/
    if (i == i1 - 1) {
        fft_twiddle_gen1(in + i, out + i, W, r, m, nW, nWdn * i, nWdn * m);
    } else {
        int i2 = (i + i1) / 2;
        fft_twiddle_gen_seq(i, i2, in, out, W, nW, nWdn, r, m);
        fft_twiddle_gen_seq(i2, i1, in, out, W, nW, nWdn, r, m);
    }
}
/*Function: fft_base_2, ID: 23*/
void fft_base_2(COMPLEX* in, COMPLEX* out)
{
    /*fft_base_2:23*/
    /*CompoundStmt:448*/
    REAL r1_0, i1_0;
    REAL r1_1, i1_1;
    r1_0 = ((in[0]).re);
    i1_0 = ((in[0]).im);
    r1_1 = ((in[1]).re);
    i1_1 = ((in[1]).im);
    ((out[0]).re) = (r1_0 + r1_1);
    ((out[0]).im) = (i1_0 + i1_1);
    ((out[1]).re) = (r1_0 - r1_1);
    ((out[1]).im) = (i1_0 - i1_1);
}
/*Function: fft_twiddle_2_seq, ID: 25*/
void fft_twiddle_2_seq(int a, int b, COMPLEX* in, COMPLEX* out, COMPLEX* W, int nW, int nWdn, int m)
{
    /*fft_twiddle_2_seq:25*/
    /*CompoundStmt:563*/
    int l1, i;
    COMPLEX *jp, *kp;
    REAL tmpr, tmpi, wr, wi;
    if ((b - a) < 128) {
        for (i = a, l1 = nWdn * i, kp = out + i; i < b; i++, l1 += nWdn, kp++) {
            jp = in + i;
            {
                REAL r1_0, i1_0;
                REAL r1_1, i1_1;
                r1_0 = ((jp[0 * m]).re);
                i1_0 = ((jp[0 * m]).im);
                wr = ((W[1 * l1]).re);
                wi = ((W[1 * l1]).im);
                tmpr = ((jp[1 * m]).re);
                tmpi = ((jp[1 * m]).im);
                r1_1 = ((wr * tmpr) - (wi * tmpi));
                i1_1 = ((wi * tmpr) + (wr * tmpi));
                ((kp[0 * m]).re) = (r1_0 + r1_1);
                ((kp[0 * m]).im) = (i1_0 + i1_1);
                ((kp[1 * m]).re) = (r1_0 - r1_1);
                ((kp[1 * m]).im) = (i1_0 - i1_1);
            }
        }
    } else {
        int ab = (a + b) / 2;
        fft_twiddle_2_seq(a, ab, in, out, W, nW, nWdn, m);
        fft_twiddle_2_seq(ab, b, in, out, W, nW, nWdn, m);
    }
}
/*Function: fft_unshuffle_2_seq, ID: 27*/
void fft_unshuffle_2_seq(int a, int b, COMPLEX* in, COMPLEX* out, int m)
{
    /*fft_unshuffle_2_seq:27*/
    /*CompoundStmt:681*/
    int i;
    const COMPLEX* ip;
    COMPLEX* jp;
    if ((b - a) < 128) {
        ip = in + a * 2;
        for (i = a; i < b; ++i) {
            jp = out + i;
            jp[0] = ip[0];
            jp[m] = ip[1];
            ip += 2;
        }
    } else {
        int ab = (a + b) / 2;
        fft_unshuffle_2_seq(a, ab, in, out, m);
        fft_unshuffle_2_seq(ab, b, in, out, m);
    }
}
/*Function: fft_base_4, ID: 28*/
void fft_base_4(COMPLEX* in, COMPLEX* out)
{
    /*fft_base_4:28*/
    /*CompoundStmt:711*/
    REAL r1_0, i1_0;
    REAL r1_1, i1_1;
    REAL r1_2, i1_2;
    REAL r1_3, i1_3;
    {
        /*CompoundStmt:716*/
        REAL r2_0, i2_0;
        REAL r2_2, i2_2;
        r2_0 = ((in[0]).re);
        i2_0 = ((in[0]).im);
        r2_2 = ((in[2]).re);
        i2_2 = ((in[2]).im);
        r1_0 = (r2_0 + r2_2);
        i1_0 = (i2_0 + i2_2);
        r1_2 = (r2_0 - r2_2);
        i1_2 = (i2_0 - i2_2);
    }
    {
        /*CompoundStmt:739*/
        REAL r2_1, i2_1;
        REAL r2_3, i2_3;
        r2_1 = ((in[1]).re);
        i2_1 = ((in[1]).im);
        r2_3 = ((in[3]).re);
        i2_3 = ((in[3]).im);
        r1_1 = (r2_1 + r2_3);
        i1_1 = (i2_1 + i2_3);
        r1_3 = (r2_1 - r2_3);
        i1_3 = (i2_1 - i2_3);
    }
    ((out[0]).re) = (r1_0 + r1_1);
    ((out[0]).im) = (i1_0 + i1_1);
    ((out[2]).re) = (r1_0 - r1_1);
    ((out[2]).im) = (i1_0 - i1_1);
    ((out[1]).re) = (r1_2 + i1_3);
    ((out[1]).im) = (i1_2 - r1_3);
    ((out[3]).re) = (r1_2 - i1_3);
    ((out[3]).im) = (i1_2 + r1_3);
}
/*Function: fft_twiddle_4_seq, ID: 30*/
void fft_twiddle_4_seq(int a, int b, COMPLEX* in, COMPLEX* out, COMPLEX* W, int nW, int nWdn, int m)
{
    /*fft_twiddle_4_seq:30*/
    /*CompoundStmt:970*/
    int l1, i;
    COMPLEX *jp, *kp;
    REAL tmpr, tmpi, wr, wi;
    if ((b - a) < 128) {
        for (i = a, l1 = nWdn * i, kp = out + i; i < b; i++, l1 += nWdn, kp++) {
            jp = in + i;
            {
                REAL r1_0, i1_0;
                REAL r1_1, i1_1;
                REAL r1_2, i1_2;
                REAL r1_3, i1_3;
                {
                    REAL r2_0, i2_0;
                    REAL r2_2, i2_2;
                    r2_0 = ((jp[0 * m]).re);
                    i2_0 = ((jp[0 * m]).im);
                    wr = ((W[2 * l1]).re);
                    wi = ((W[2 * l1]).im);
                    tmpr = ((jp[2 * m]).re);
                    tmpi = ((jp[2 * m]).im);
                    r2_2 = ((wr * tmpr) - (wi * tmpi));
                    i2_2 = ((wi * tmpr) + (wr * tmpi));
                    r1_0 = (r2_0 + r2_2);
                    i1_0 = (i2_0 + i2_2);
                    r1_2 = (r2_0 - r2_2);
                    i1_2 = (i2_0 - i2_2);
                }
                {
                    REAL r2_1, i2_1;
                    REAL r2_3, i2_3;
                    wr = ((W[1 * l1]).re);
                    wi = ((W[1 * l1]).im);
                    tmpr = ((jp[1 * m]).re);
                    tmpi = ((jp[1 * m]).im);
                    r2_1 = ((wr * tmpr) - (wi * tmpi));
                    i2_1 = ((wi * tmpr) + (wr * tmpi));
                    wr = ((W[3 * l1]).re);
                    wi = ((W[3 * l1]).im);
                    tmpr = ((jp[3 * m]).re);
                    tmpi = ((jp[3 * m]).im);
                    r2_3 = ((wr * tmpr) - (wi * tmpi));
                    i2_3 = ((wi * tmpr) + (wr * tmpi));
                    r1_1 = (r2_1 + r2_3);
                    i1_1 = (i2_1 + i2_3);
                    r1_3 = (r2_1 - r2_3);
                    i1_3 = (i2_1 - i2_3);
                }
                ((kp[0 * m]).re) = (r1_0 + r1_1);
                ((kp[0 * m]).im) = (i1_0 + i1_1);
                ((kp[2 * m]).re) = (r1_0 - r1_1);
                ((kp[2 * m]).im) = (i1_0 - i1_1);
                ((kp[1 * m]).re) = (r1_2 + i1_3);
                ((kp[1 * m]).im) = (i1_2 - r1_3);
                ((kp[3 * m]).re) = (r1_2 - i1_3);
                ((kp[3 * m]).im) = (i1_2 + r1_3);
            }
        }
    } else {
        int ab = (a + b) / 2;
        fft_twiddle_4_seq(a, ab, in, out, W, nW, nWdn, m);
        fft_twiddle_4_seq(ab, b, in, out, W, nW, nWdn, m);
    }
}
/*Function: fft_unshuffle_4_seq, ID: 32*/
void fft_unshuffle_4_seq(int a, int b, COMPLEX* in, COMPLEX* out, int m)
{
    /*fft_unshuffle_4_seq:32*/
    /*CompoundStmt:1189*/
    int i;
    const COMPLEX* ip;
    COMPLEX* jp;
    if ((b - a) < 128) {
        ip = in + a * 4;
        for (i = a; i < b; ++i) {
            jp = out + i;
            jp[0] = ip[0];
            jp[m] = ip[1];
            ip += 2;
            jp += 2 * m;
            jp[0] = ip[0];
            jp[m] = ip[1];
            ip += 2;
        }
    } else {
        int ab = (a + b) / 2;
        fft_unshuffle_4_seq(a, ab, in, out, m);
        fft_unshuffle_4_seq(ab, b, in, out, m);
    }
}
/*Function: fft_base_8, ID: 33*/
void fft_base_8(COMPLEX* in, COMPLEX* out)
{
    /*fft_base_8:33*/
    /*CompoundStmt:1228*/
    REAL tmpr, tmpi;
    {
        /*CompoundStmt:1230*/
        REAL r1_0, i1_0;
        REAL r1_1, i1_1;
        REAL r1_2, i1_2;
        REAL r1_3, i1_3;
        REAL r1_4, i1_4;
        REAL r1_5, i1_5;
        REAL r1_6, i1_6;
        REAL r1_7, i1_7;
        {
            /*CompoundStmt:1239*/
            REAL r2_0, i2_0;
            REAL r2_2, i2_2;
            REAL r2_4, i2_4;
            REAL r2_6, i2_6;
            {
                /*CompoundStmt:1244*/
                REAL r3_0, i3_0;
                REAL r3_4, i3_4;
                r3_0 = ((in[0]).re);
                i3_0 = ((in[0]).im);
                r3_4 = ((in[4]).re);
                i3_4 = ((in[4]).im);
                r2_0 = (r3_0 + r3_4);
                i2_0 = (i3_0 + i3_4);
                r2_4 = (r3_0 - r3_4);
                i2_4 = (i3_0 - i3_4);
            }
            {
                /*CompoundStmt:1267*/
                REAL r3_2, i3_2;
                REAL r3_6, i3_6;
                r3_2 = ((in[2]).re);
                i3_2 = ((in[2]).im);
                r3_6 = ((in[6]).re);
                i3_6 = ((in[6]).im);
                r2_2 = (r3_2 + r3_6);
                i2_2 = (i3_2 + i3_6);
                r2_6 = (r3_2 - r3_6);
                i2_6 = (i3_2 - i3_6);
            }
            r1_0 = (r2_0 + r2_2);
            i1_0 = (i2_0 + i2_2);
            r1_4 = (r2_0 - r2_2);
            i1_4 = (i2_0 - i2_2);
            r1_2 = (r2_4 + i2_6);
            i1_2 = (i2_4 - r2_6);
            r1_6 = (r2_4 - i2_6);
            i1_6 = (i2_4 + r2_6);
        }
        {
            /*CompoundStmt:1306*/
            REAL r2_1, i2_1;
            REAL r2_3, i2_3;
            REAL r2_5, i2_5;
            REAL r2_7, i2_7;
            {
                /*CompoundStmt:1311*/
                REAL r3_1, i3_1;
                REAL r3_5, i3_5;
                r3_1 = ((in[1]).re);
                i3_1 = ((in[1]).im);
                r3_5 = ((in[5]).re);
                i3_5 = ((in[5]).im);
                r2_1 = (r3_1 + r3_5);
                i2_1 = (i3_1 + i3_5);
                r2_5 = (r3_1 - r3_5);
                i2_5 = (i3_1 - i3_5);
            }
            {
                /*CompoundStmt:1334*/
                REAL r3_3, i3_3;
                REAL r3_7, i3_7;
                r3_3 = ((in[3]).re);
                i3_3 = ((in[3]).im);
                r3_7 = ((in[7]).re);
                i3_7 = ((in[7]).im);
                r2_3 = (r3_3 + r3_7);
                i2_3 = (i3_3 + i3_7);
                r2_7 = (r3_3 - r3_7);
                i2_7 = (i3_3 - i3_7);
            }
            r1_1 = (r2_1 + r2_3);
            i1_1 = (i2_1 + i2_3);
            r1_5 = (r2_1 - r2_3);
            i1_5 = (i2_1 - i2_3);
            r1_3 = (r2_5 + i2_7);
            i1_3 = (i2_5 - r2_7);
            r1_7 = (r2_5 - i2_7);
            i1_7 = (i2_5 + r2_7);
        }
        ((out[0]).re) = (r1_0 + r1_1);
        ((out[0]).im) = (i1_0 + i1_1);
        ((out[4]).re) = (r1_0 - r1_1);
        ((out[4]).im) = (i1_0 - i1_1);
        tmpr = (0.70710678118699999 * (r1_3 + i1_3));
        tmpi = (0.70710678118699999 * (i1_3 - r1_3));
        ((out[1]).re) = (r1_2 + tmpr);
        ((out[1]).im) = (i1_2 + tmpi);
        ((out[5]).re) = (r1_2 - tmpr);
        ((out[5]).im) = (i1_2 - tmpi);
        ((out[2]).re) = (r1_4 + i1_5);
        ((out[2]).im) = (i1_4 - r1_5);
        ((out[6]).re) = (r1_4 - i1_5);
        ((out[6]).im) = (i1_4 + r1_5);
        tmpr = (0.70710678118699999 * (i1_7 - r1_7));
        tmpi = (0.70710678118699999 * (r1_7 + i1_7));
        ((out[3]).re) = (r1_6 + tmpr);
        ((out[3]).im) = (i1_6 - tmpi);
        ((out[7]).re) = (r1_6 - tmpr);
        ((out[7]).im) = (i1_6 + tmpi);
    }
}
/*Function: fft_twiddle_8_seq, ID: 35*/
void fft_twiddle_8_seq(int a, int b, COMPLEX* in, COMPLEX* out, COMPLEX* W, int nW, int nWdn, int m)
{
    /*fft_twiddle_8_seq:35*/
    /*CompoundStmt:1849*/
    int l1, i;
    COMPLEX *jp, *kp;
    REAL tmpr, tmpi, wr, wi;
    if ((b - a) < 128) {
        for (i = a, l1 = nWdn * i, kp = out + i; i < b; i++, l1 += nWdn, kp++) {
            jp = in + i;
            {
                REAL r1_0, i1_0;
                REAL r1_1, i1_1;
                REAL r1_2, i1_2;
                REAL r1_3, i1_3;
                REAL r1_4, i1_4;
                REAL r1_5, i1_5;
                REAL r1_6, i1_6;
                REAL r1_7, i1_7;
                {
                    REAL r2_0, i2_0;
                    REAL r2_2, i2_2;
                    REAL r2_4, i2_4;
                    REAL r2_6, i2_6;
                    {
                        REAL r3_0, i3_0;
                        REAL r3_4, i3_4;
                        r3_0 = ((jp[0 * m]).re);
                        i3_0 = ((jp[0 * m]).im);
                        wr = ((W[4 * l1]).re);
                        wi = ((W[4 * l1]).im);
                        tmpr = ((jp[4 * m]).re);
                        tmpi = ((jp[4 * m]).im);
                        r3_4 = ((wr * tmpr) - (wi * tmpi));
                        i3_4 = ((wi * tmpr) + (wr * tmpi));
                        r2_0 = (r3_0 + r3_4);
                        i2_0 = (i3_0 + i3_4);
                        r2_4 = (r3_0 - r3_4);
                        i2_4 = (i3_0 - i3_4);
                    }
                    {
                        REAL r3_2, i3_2;
                        REAL r3_6, i3_6;
                        wr = ((W[2 * l1]).re);
                        wi = ((W[2 * l1]).im);
                        tmpr = ((jp[2 * m]).re);
                        tmpi = ((jp[2 * m]).im);
                        r3_2 = ((wr * tmpr) - (wi * tmpi));
                        i3_2 = ((wi * tmpr) + (wr * tmpi));
                        wr = ((W[6 * l1]).re);
                        wi = ((W[6 * l1]).im);
                        tmpr = ((jp[6 * m]).re);
                        tmpi = ((jp[6 * m]).im);
                        r3_6 = ((wr * tmpr) - (wi * tmpi));
                        i3_6 = ((wi * tmpr) + (wr * tmpi));
                        r2_2 = (r3_2 + r3_6);
                        i2_2 = (i3_2 + i3_6);
                        r2_6 = (r3_2 - r3_6);
                        i2_6 = (i3_2 - i3_6);
                    }
                    r1_0 = (r2_0 + r2_2);
                    i1_0 = (i2_0 + i2_2);
                    r1_4 = (r2_0 - r2_2);
                    i1_4 = (i2_0 - i2_2);
                    r1_2 = (r2_4 + i2_6);
                    i1_2 = (i2_4 - r2_6);
                    r1_6 = (r2_4 - i2_6);
                    i1_6 = (i2_4 + r2_6);
                }
                {
                    REAL r2_1, i2_1;
                    REAL r2_3, i2_3;
                    REAL r2_5, i2_5;
                    REAL r2_7, i2_7;
                    {
                        REAL r3_1, i3_1;
                        REAL r3_5, i3_5;
                        wr = ((W[1 * l1]).re);
                        wi = ((W[1 * l1]).im);
                        tmpr = ((jp[1 * m]).re);
                        tmpi = ((jp[1 * m]).im);
                        r3_1 = ((wr * tmpr) - (wi * tmpi));
                        i3_1 = ((wi * tmpr) + (wr * tmpi));
                        wr = ((W[5 * l1]).re);
                        wi = ((W[5 * l1]).im);
                        tmpr = ((jp[5 * m]).re);
                        tmpi = ((jp[5 * m]).im);
                        r3_5 = ((wr * tmpr) - (wi * tmpi));
                        i3_5 = ((wi * tmpr) + (wr * tmpi));
                        r2_1 = (r3_1 + r3_5);
                        i2_1 = (i3_1 + i3_5);
                        r2_5 = (r3_1 - r3_5);
                        i2_5 = (i3_1 - i3_5);
                    }
                    {
                        REAL r3_3, i3_3;
                        REAL r3_7, i3_7;
                        wr = ((W[3 * l1]).re);
                        wi = ((W[3 * l1]).im);
                        tmpr = ((jp[3 * m]).re);
                        tmpi = ((jp[3 * m]).im);
                        r3_3 = ((wr * tmpr) - (wi * tmpi));
                        i3_3 = ((wi * tmpr) + (wr * tmpi));
                        wr = ((W[7 * l1]).re);
                        wi = ((W[7 * l1]).im);
                        tmpr = ((jp[7 * m]).re);
                        tmpi = ((jp[7 * m]).im);
                        r3_7 = ((wr * tmpr) - (wi * tmpi));
                        i3_7 = ((wi * tmpr) + (wr * tmpi));
                        r2_3 = (r3_3 + r3_7);
                        i2_3 = (i3_3 + i3_7);
                        r2_7 = (r3_3 - r3_7);
                        i2_7 = (i3_3 - i3_7);
                    }
                    r1_1 = (r2_1 + r2_3);
                    i1_1 = (i2_1 + i2_3);
                    r1_5 = (r2_1 - r2_3);
                    i1_5 = (i2_1 - i2_3);
                    r1_3 = (r2_5 + i2_7);
                    i1_3 = (i2_5 - r2_7);
                    r1_7 = (r2_5 - i2_7);
                    i1_7 = (i2_5 + r2_7);
                }
                ((kp[0 * m]).re) = (r1_0 + r1_1);
                ((kp[0 * m]).im) = (i1_0 + i1_1);
                ((kp[4 * m]).re) = (r1_0 - r1_1);
                ((kp[4 * m]).im) = (i1_0 - i1_1);
                tmpr = (0.70710678118699999 * (r1_3 + i1_3));
                tmpi = (0.70710678118699999 * (i1_3 - r1_3));
                ((kp[1 * m]).re) = (r1_2 + tmpr);
                ((kp[1 * m]).im) = (i1_2 + tmpi);
                ((kp[5 * m]).re) = (r1_2 - tmpr);
                ((kp[5 * m]).im) = (i1_2 - tmpi);
                ((kp[2 * m]).re) = (r1_4 + i1_5);
                ((kp[2 * m]).im) = (i1_4 - r1_5);
                ((kp[6 * m]).re) = (r1_4 - i1_5);
                ((kp[6 * m]).im) = (i1_4 + r1_5);
                tmpr = (0.70710678118699999 * (i1_7 - r1_7));
                tmpi = (0.70710678118699999 * (r1_7 + i1_7));
                ((kp[3 * m]).re) = (r1_6 + tmpr);
                ((kp[3 * m]).im) = (i1_6 - tmpi);
                ((kp[7 * m]).re) = (r1_6 - tmpr);
                ((kp[7 * m]).im) = (i1_6 + tmpi);
            }
        }
    } else {
        int ab = (a + b) / 2;
        fft_twiddle_8_seq(a, ab, in, out, W, nW, nWdn, m);
        fft_twiddle_8_seq(ab, b, in, out, W, nW, nWdn, m);
    }
}
/*Function: fft_unshuffle_8_seq, ID: 37*/
void fft_unshuffle_8_seq(int a, int b, COMPLEX* in, COMPLEX* out, int m)
{
    /*fft_unshuffle_8_seq:37*/
    /*CompoundStmt:2306*/
    int i;
    const COMPLEX* ip;
    COMPLEX* jp;
    if ((b - a) < 128) {
        ip = in + a * 8;
        for (i = a; i < b; ++i) {
            jp = out + i;
            jp[0] = ip[0];
            jp[m] = ip[1];
            ip += 2;
            jp += 2 * m;
            jp[0] = ip[0];
            jp[m] = ip[1];
            ip += 2;
            jp += 2 * m;
            jp[0] = ip[0];
            jp[m] = ip[1];
            ip += 2;
            jp += 2 * m;
            jp[0] = ip[0];
            jp[m] = ip[1];
            ip += 2;
        }
    } else {
        int ab = (a + b) / 2;
        fft_unshuffle_8_seq(a, ab, in, out, m);
        fft_unshuffle_8_seq(ab, b, in, out, m);
    }
}
/*Function: fft_base_16, ID: 38*/
void fft_base_16(COMPLEX* in, COMPLEX* out)
{
    /*fft_base_16:38*/
    /*CompoundStmt:2363*/
    REAL tmpr, tmpi;
    {
        /*CompoundStmt:2365*/
        REAL r1_0, i1_0;
        REAL r1_1, i1_1;
        REAL r1_2, i1_2;
        REAL r1_3, i1_3;
        REAL r1_4, i1_4;
        REAL r1_5, i1_5;
        REAL r1_6, i1_6;
        REAL r1_7, i1_7;
        REAL r1_8, i1_8;
        REAL r1_9, i1_9;
        REAL r1_10, i1_10;
        REAL r1_11, i1_11;
        REAL r1_12, i1_12;
        REAL r1_13, i1_13;
        REAL r1_14, i1_14;
        REAL r1_15, i1_15;
        {
            /*CompoundStmt:2382*/
            REAL r2_0, i2_0;
            REAL r2_2, i2_2;
            REAL r2_4, i2_4;
            REAL r2_6, i2_6;
            REAL r2_8, i2_8;
            REAL r2_10, i2_10;
            REAL r2_12, i2_12;
            REAL r2_14, i2_14;
            {
                /*CompoundStmt:2391*/
                REAL r3_0, i3_0;
                REAL r3_4, i3_4;
                REAL r3_8, i3_8;
                REAL r3_12, i3_12;
                {
                    /*CompoundStmt:2396*/
                    REAL r4_0, i4_0;
                    REAL r4_8, i4_8;
                    r4_0 = ((in[0]).re);
                    i4_0 = ((in[0]).im);
                    r4_8 = ((in[8]).re);
                    i4_8 = ((in[8]).im);
                    r3_0 = (r4_0 + r4_8);
                    i3_0 = (i4_0 + i4_8);
                    r3_8 = (r4_0 - r4_8);
                    i3_8 = (i4_0 - i4_8);
                }
                {
                    /*CompoundStmt:2419*/
                    REAL r4_4, i4_4;
                    REAL r4_12, i4_12;
                    r4_4 = ((in[4]).re);
                    i4_4 = ((in[4]).im);
                    r4_12 = ((in[12]).re);
                    i4_12 = ((in[12]).im);
                    r3_4 = (r4_4 + r4_12);
                    i3_4 = (i4_4 + i4_12);
                    r3_12 = (r4_4 - r4_12);
                    i3_12 = (i4_4 - i4_12);
                }
                r2_0 = (r3_0 + r3_4);
                i2_0 = (i3_0 + i3_4);
                r2_8 = (r3_0 - r3_4);
                i2_8 = (i3_0 - i3_4);
                r2_4 = (r3_8 + i3_12);
                i2_4 = (i3_8 - r3_12);
                r2_12 = (r3_8 - i3_12);
                i2_12 = (i3_8 + r3_12);
            }
            {
                /*CompoundStmt:2458*/
                REAL r3_2, i3_2;
                REAL r3_6, i3_6;
                REAL r3_10, i3_10;
                REAL r3_14, i3_14;
                {
                    /*CompoundStmt:2463*/
                    REAL r4_2, i4_2;
                    REAL r4_10, i4_10;
                    r4_2 = ((in[2]).re);
                    i4_2 = ((in[2]).im);
                    r4_10 = ((in[10]).re);
                    i4_10 = ((in[10]).im);
                    r3_2 = (r4_2 + r4_10);
                    i3_2 = (i4_2 + i4_10);
                    r3_10 = (r4_2 - r4_10);
                    i3_10 = (i4_2 - i4_10);
                }
                {
                    /*CompoundStmt:2486*/
                    REAL r4_6, i4_6;
                    REAL r4_14, i4_14;
                    r4_6 = ((in[6]).re);
                    i4_6 = ((in[6]).im);
                    r4_14 = ((in[14]).re);
                    i4_14 = ((in[14]).im);
                    r3_6 = (r4_6 + r4_14);
                    i3_6 = (i4_6 + i4_14);
                    r3_14 = (r4_6 - r4_14);
                    i3_14 = (i4_6 - i4_14);
                }
                r2_2 = (r3_2 + r3_6);
                i2_2 = (i3_2 + i3_6);
                r2_10 = (r3_2 - r3_6);
                i2_10 = (i3_2 - i3_6);
                r2_6 = (r3_10 + i3_14);
                i2_6 = (i3_10 - r3_14);
                r2_14 = (r3_10 - i3_14);
                i2_14 = (i3_10 + r3_14);
            }
            r1_0 = (r2_0 + r2_2);
            i1_0 = (i2_0 + i2_2);
            r1_8 = (r2_0 - r2_2);
            i1_8 = (i2_0 - i2_2);
            tmpr = (0.70710678118699999 * (r2_6 + i2_6));
            tmpi = (0.70710678118699999 * (i2_6 - r2_6));
            r1_2 = (r2_4 + tmpr);
            i1_2 = (i2_4 + tmpi);
            r1_10 = (r2_4 - tmpr);
            i1_10 = (i2_4 - tmpi);
            r1_4 = (r2_8 + i2_10);
            i1_4 = (i2_8 - r2_10);
            r1_12 = (r2_8 - i2_10);
            i1_12 = (i2_8 + r2_10);
            tmpr = (0.70710678118699999 * (i2_14 - r2_14));
            tmpi = (0.70710678118699999 * (r2_14 + i2_14));
            r1_6 = (r2_12 + tmpr);
            i1_6 = (i2_12 - tmpi);
            r1_14 = (r2_12 - tmpr);
            i1_14 = (i2_12 + tmpi);
        }
        {
            /*CompoundStmt:2573*/
            REAL r2_1, i2_1;
            REAL r2_3, i2_3;
            REAL r2_5, i2_5;
            REAL r2_7, i2_7;
            REAL r2_9, i2_9;
            REAL r2_11, i2_11;
            REAL r2_13, i2_13;
            REAL r2_15, i2_15;
            {
                /*CompoundStmt:2582*/
                REAL r3_1, i3_1;
                REAL r3_5, i3_5;
                REAL r3_9, i3_9;
                REAL r3_13, i3_13;
                {
                    /*CompoundStmt:2587*/
                    REAL r4_1, i4_1;
                    REAL r4_9, i4_9;
                    r4_1 = ((in[1]).re);
                    i4_1 = ((in[1]).im);
                    r4_9 = ((in[9]).re);
                    i4_9 = ((in[9]).im);
                    r3_1 = (r4_1 + r4_9);
                    i3_1 = (i4_1 + i4_9);
                    r3_9 = (r4_1 - r4_9);
                    i3_9 = (i4_1 - i4_9);
                }
                {
                    /*CompoundStmt:2610*/
                    REAL r4_5, i4_5;
                    REAL r4_13, i4_13;
                    r4_5 = ((in[5]).re);
                    i4_5 = ((in[5]).im);
                    r4_13 = ((in[13]).re);
                    i4_13 = ((in[13]).im);
                    r3_5 = (r4_5 + r4_13);
                    i3_5 = (i4_5 + i4_13);
                    r3_13 = (r4_5 - r4_13);
                    i3_13 = (i4_5 - i4_13);
                }
                r2_1 = (r3_1 + r3_5);
                i2_1 = (i3_1 + i3_5);
                r2_9 = (r3_1 - r3_5);
                i2_9 = (i3_1 - i3_5);
                r2_5 = (r3_9 + i3_13);
                i2_5 = (i3_9 - r3_13);
                r2_13 = (r3_9 - i3_13);
                i2_13 = (i3_9 + r3_13);
            }
            {
                /*CompoundStmt:2649*/
                REAL r3_3, i3_3;
                REAL r3_7, i3_7;
                REAL r3_11, i3_11;
                REAL r3_15, i3_15;
                {
                    /*CompoundStmt:2654*/
                    REAL r4_3, i4_3;
                    REAL r4_11, i4_11;
                    r4_3 = ((in[3]).re);
                    i4_3 = ((in[3]).im);
                    r4_11 = ((in[11]).re);
                    i4_11 = ((in[11]).im);
                    r3_3 = (r4_3 + r4_11);
                    i3_3 = (i4_3 + i4_11);
                    r3_11 = (r4_3 - r4_11);
                    i3_11 = (i4_3 - i4_11);
                }
                {
                    /*CompoundStmt:2677*/
                    REAL r4_7, i4_7;
                    REAL r4_15, i4_15;
                    r4_7 = ((in[7]).re);
                    i4_7 = ((in[7]).im);
                    r4_15 = ((in[15]).re);
                    i4_15 = ((in[15]).im);
                    r3_7 = (r4_7 + r4_15);
                    i3_7 = (i4_7 + i4_15);
                    r3_15 = (r4_7 - r4_15);
                    i3_15 = (i4_7 - i4_15);
                }
                r2_3 = (r3_3 + r3_7);
                i2_3 = (i3_3 + i3_7);
                r2_11 = (r3_3 - r3_7);
                i2_11 = (i3_3 - i3_7);
                r2_7 = (r3_11 + i3_15);
                i2_7 = (i3_11 - r3_15);
                r2_15 = (r3_11 - i3_15);
                i2_15 = (i3_11 + r3_15);
            }
            r1_1 = (r2_1 + r2_3);
            i1_1 = (i2_1 + i2_3);
            r1_9 = (r2_1 - r2_3);
            i1_9 = (i2_1 - i2_3);
            tmpr = (0.70710678118699999 * (r2_7 + i2_7));
            tmpi = (0.70710678118699999 * (i2_7 - r2_7));
            r1_3 = (r2_5 + tmpr);
            i1_3 = (i2_5 + tmpi);
            r1_11 = (r2_5 - tmpr);
            i1_11 = (i2_5 - tmpi);
            r1_5 = (r2_9 + i2_11);
            i1_5 = (i2_9 - r2_11);
            r1_13 = (r2_9 - i2_11);
            i1_13 = (i2_9 + r2_11);
            tmpr = (0.70710678118699999 * (i2_15 - r2_15));
            tmpi = (0.70710678118699999 * (r2_15 + i2_15));
            r1_7 = (r2_13 + tmpr);
            i1_7 = (i2_13 - tmpi);
            r1_15 = (r2_13 - tmpr);
            i1_15 = (i2_13 + tmpi);
        }
        ((out[0]).re) = (r1_0 + r1_1);
        ((out[0]).im) = (i1_0 + i1_1);
        ((out[8]).re) = (r1_0 - r1_1);
        ((out[8]).im) = (i1_0 - i1_1);
        tmpr = ((0.92387953251099997 * r1_3) + (0.38268343236500002 * i1_3));
        tmpi = ((0.92387953251099997 * i1_3) - (0.38268343236500002 * r1_3));
        ((out[1]).re) = (r1_2 + tmpr);
        ((out[1]).im) = (i1_2 + tmpi);
        ((out[9]).re) = (r1_2 - tmpr);
        ((out[9]).im) = (i1_2 - tmpi);
        tmpr = (0.70710678118699999 * (r1_5 + i1_5));
        tmpi = (0.70710678118699999 * (i1_5 - r1_5));
        ((out[2]).re) = (r1_4 + tmpr);
        ((out[2]).im) = (i1_4 + tmpi);
        ((out[10]).re) = (r1_4 - tmpr);
        ((out[10]).im) = (i1_4 - tmpi);
        tmpr = ((0.38268343236500002 * r1_7) + (0.92387953251099997 * i1_7));
        tmpi = ((0.38268343236500002 * i1_7) - (0.92387953251099997 * r1_7));
        ((out[3]).re) = (r1_6 + tmpr);
        ((out[3]).im) = (i1_6 + tmpi);
        ((out[11]).re) = (r1_6 - tmpr);
        ((out[11]).im) = (i1_6 - tmpi);
        ((out[4]).re) = (r1_8 + i1_9);
        ((out[4]).im) = (i1_8 - r1_9);
        ((out[12]).re) = (r1_8 - i1_9);
        ((out[12]).im) = (i1_8 + r1_9);
        tmpr = ((0.92387953251099997 * i1_11) - (0.38268343236500002 * r1_11));
        tmpi = ((0.92387953251099997 * r1_11) + (0.38268343236500002 * i1_11));
        ((out[5]).re) = (r1_10 + tmpr);
        ((out[5]).im) = (i1_10 - tmpi);
        ((out[13]).re) = (r1_10 - tmpr);
        ((out[13]).im) = (i1_10 + tmpi);
        tmpr = (0.70710678118699999 * (i1_13 - r1_13));
        tmpi = (0.70710678118699999 * (r1_13 + i1_13));
        ((out[6]).re) = (r1_12 + tmpr);
        ((out[6]).im) = (i1_12 - tmpi);
        ((out[14]).re) = (r1_12 - tmpr);
        ((out[14]).im) = (i1_12 + tmpi);
        tmpr = ((0.38268343236500002 * i1_15) - (0.92387953251099997 * r1_15));
        tmpi = ((0.38268343236500002 * r1_15) + (0.92387953251099997 * i1_15));
        ((out[7]).re) = (r1_14 + tmpr);
        ((out[7]).im) = (i1_14 - tmpi);
        ((out[15]).re) = (r1_14 - tmpr);
        ((out[15]).im) = (i1_14 + tmpi);
    }
}
/*Function: fft_twiddle_16_seq, ID: 40*/
void fft_twiddle_16_seq(
    int a, int b, COMPLEX* in, COMPLEX* out, COMPLEX* W, int nW, int nWdn, int m)
{
    /*fft_twiddle_16_seq:40*/
    /*CompoundStmt:3880*/
    int l1, i;
    COMPLEX *jp, *kp;
    REAL tmpr, tmpi, wr, wi;
    if ((b - a) < 128) {
        for (i = a, l1 = nWdn * i, kp = out + i; i < b; i++, l1 += nWdn, kp++) {
            jp = in + i;
            {
                REAL r1_0, i1_0;
                REAL r1_1, i1_1;
                REAL r1_2, i1_2;
                REAL r1_3, i1_3;
                REAL r1_4, i1_4;
                REAL r1_5, i1_5;
                REAL r1_6, i1_6;
                REAL r1_7, i1_7;
                REAL r1_8, i1_8;
                REAL r1_9, i1_9;
                REAL r1_10, i1_10;
                REAL r1_11, i1_11;
                REAL r1_12, i1_12;
                REAL r1_13, i1_13;
                REAL r1_14, i1_14;
                REAL r1_15, i1_15;
                {
                    REAL r2_0, i2_0;
                    REAL r2_2, i2_2;
                    REAL r2_4, i2_4;
                    REAL r2_6, i2_6;
                    REAL r2_8, i2_8;
                    REAL r2_10, i2_10;
                    REAL r2_12, i2_12;
                    REAL r2_14, i2_14;
                    {
                        REAL r3_0, i3_0;
                        REAL r3_4, i3_4;
                        REAL r3_8, i3_8;
                        REAL r3_12, i3_12;
                        {
                            REAL r4_0, i4_0;
                            REAL r4_8, i4_8;
                            r4_0 = ((jp[0 * m]).re);
                            i4_0 = ((jp[0 * m]).im);
                            wr = ((W[8 * l1]).re);
                            wi = ((W[8 * l1]).im);
                            tmpr = ((jp[8 * m]).re);
                            tmpi = ((jp[8 * m]).im);
                            r4_8 = ((wr * tmpr) - (wi * tmpi));
                            i4_8 = ((wi * tmpr) + (wr * tmpi));
                            r3_0 = (r4_0 + r4_8);
                            i3_0 = (i4_0 + i4_8);
                            r3_8 = (r4_0 - r4_8);
                            i3_8 = (i4_0 - i4_8);
                        }
                        {
                            REAL r4_4, i4_4;
                            REAL r4_12, i4_12;
                            wr = ((W[4 * l1]).re);
                            wi = ((W[4 * l1]).im);
                            tmpr = ((jp[4 * m]).re);
                            tmpi = ((jp[4 * m]).im);
                            r4_4 = ((wr * tmpr) - (wi * tmpi));
                            i4_4 = ((wi * tmpr) + (wr * tmpi));
                            wr = ((W[12 * l1]).re);
                            wi = ((W[12 * l1]).im);
                            tmpr = ((jp[12 * m]).re);
                            tmpi = ((jp[12 * m]).im);
                            r4_12 = ((wr * tmpr) - (wi * tmpi));
                            i4_12 = ((wi * tmpr) + (wr * tmpi));
                            r3_4 = (r4_4 + r4_12);
                            i3_4 = (i4_4 + i4_12);
                            r3_12 = (r4_4 - r4_12);
                            i3_12 = (i4_4 - i4_12);
                        }
                        r2_0 = (r3_0 + r3_4);
                        i2_0 = (i3_0 + i3_4);
                        r2_8 = (r3_0 - r3_4);
                        i2_8 = (i3_0 - i3_4);
                        r2_4 = (r3_8 + i3_12);
                        i2_4 = (i3_8 - r3_12);
                        r2_12 = (r3_8 - i3_12);
                        i2_12 = (i3_8 + r3_12);
                    }
                    {
                        REAL r3_2, i3_2;
                        REAL r3_6, i3_6;
                        REAL r3_10, i3_10;
                        REAL r3_14, i3_14;
                        {
                            REAL r4_2, i4_2;
                            REAL r4_10, i4_10;
                            wr = ((W[2 * l1]).re);
                            wi = ((W[2 * l1]).im);
                            tmpr = ((jp[2 * m]).re);
                            tmpi = ((jp[2 * m]).im);
                            r4_2 = ((wr * tmpr) - (wi * tmpi));
                            i4_2 = ((wi * tmpr) + (wr * tmpi));
                            wr = ((W[10 * l1]).re);
                            wi = ((W[10 * l1]).im);
                            tmpr = ((jp[10 * m]).re);
                            tmpi = ((jp[10 * m]).im);
                            r4_10 = ((wr * tmpr) - (wi * tmpi));
                            i4_10 = ((wi * tmpr) + (wr * tmpi));
                            r3_2 = (r4_2 + r4_10);
                            i3_2 = (i4_2 + i4_10);
                            r3_10 = (r4_2 - r4_10);
                            i3_10 = (i4_2 - i4_10);
                        }
                        {
                            REAL r4_6, i4_6;
                            REAL r4_14, i4_14;
                            wr = ((W[6 * l1]).re);
                            wi = ((W[6 * l1]).im);
                            tmpr = ((jp[6 * m]).re);
                            tmpi = ((jp[6 * m]).im);
                            r4_6 = ((wr * tmpr) - (wi * tmpi));
                            i4_6 = ((wi * tmpr) + (wr * tmpi));
                            wr = ((W[14 * l1]).re);
                            wi = ((W[14 * l1]).im);
                            tmpr = ((jp[14 * m]).re);
                            tmpi = ((jp[14 * m]).im);
                            r4_14 = ((wr * tmpr) - (wi * tmpi));
                            i4_14 = ((wi * tmpr) + (wr * tmpi));
                            r3_6 = (r4_6 + r4_14);
                            i3_6 = (i4_6 + i4_14);
                            r3_14 = (r4_6 - r4_14);
                            i3_14 = (i4_6 - i4_14);
                        }
                        r2_2 = (r3_2 + r3_6);
                        i2_2 = (i3_2 + i3_6);
                        r2_10 = (r3_2 - r3_6);
                        i2_10 = (i3_2 - i3_6);
                        r2_6 = (r3_10 + i3_14);
                        i2_6 = (i3_10 - r3_14);
                        r2_14 = (r3_10 - i3_14);
                        i2_14 = (i3_10 + r3_14);
                    }
                    r1_0 = (r2_0 + r2_2);
                    i1_0 = (i2_0 + i2_2);
                    r1_8 = (r2_0 - r2_2);
                    i1_8 = (i2_0 - i2_2);
                    tmpr = (0.70710678118699999 * (r2_6 + i2_6));
                    tmpi = (0.70710678118699999 * (i2_6 - r2_6));
                    r1_2 = (r2_4 + tmpr);
                    i1_2 = (i2_4 + tmpi);
                    r1_10 = (r2_4 - tmpr);
                    i1_10 = (i2_4 - tmpi);
                    r1_4 = (r2_8 + i2_10);
                    i1_4 = (i2_8 - r2_10);
                    r1_12 = (r2_8 - i2_10);
                    i1_12 = (i2_8 + r2_10);
                    tmpr = (0.70710678118699999 * (i2_14 - r2_14));
                    tmpi = (0.70710678118699999 * (r2_14 + i2_14));
                    r1_6 = (r2_12 + tmpr);
                    i1_6 = (i2_12 - tmpi);
                    r1_14 = (r2_12 - tmpr);
                    i1_14 = (i2_12 + tmpi);
                }
                {
                    REAL r2_1, i2_1;
                    REAL r2_3, i2_3;
                    REAL r2_5, i2_5;
                    REAL r2_7, i2_7;
                    REAL r2_9, i2_9;
                    REAL r2_11, i2_11;
                    REAL r2_13, i2_13;
                    REAL r2_15, i2_15;
                    {
                        REAL r3_1, i3_1;
                        REAL r3_5, i3_5;
                        REAL r3_9, i3_9;
                        REAL r3_13, i3_13;
                        {
                            REAL r4_1, i4_1;
                            REAL r4_9, i4_9;
                            wr = ((W[1 * l1]).re);
                            wi = ((W[1 * l1]).im);
                            tmpr = ((jp[1 * m]).re);
                            tmpi = ((jp[1 * m]).im);
                            r4_1 = ((wr * tmpr) - (wi * tmpi));
                            i4_1 = ((wi * tmpr) + (wr * tmpi));
                            wr = ((W[9 * l1]).re);
                            wi = ((W[9 * l1]).im);
                            tmpr = ((jp[9 * m]).re);
                            tmpi = ((jp[9 * m]).im);
                            r4_9 = ((wr * tmpr) - (wi * tmpi));
                            i4_9 = ((wi * tmpr) + (wr * tmpi));
                            r3_1 = (r4_1 + r4_9);
                            i3_1 = (i4_1 + i4_9);
                            r3_9 = (r4_1 - r4_9);
                            i3_9 = (i4_1 - i4_9);
                        }
                        {
                            REAL r4_5, i4_5;
                            REAL r4_13, i4_13;
                            wr = ((W[5 * l1]).re);
                            wi = ((W[5 * l1]).im);
                            tmpr = ((jp[5 * m]).re);
                            tmpi = ((jp[5 * m]).im);
                            r4_5 = ((wr * tmpr) - (wi * tmpi));
                            i4_5 = ((wi * tmpr) + (wr * tmpi));
                            wr = ((W[13 * l1]).re);
                            wi = ((W[13 * l1]).im);
                            tmpr = ((jp[13 * m]).re);
                            tmpi = ((jp[13 * m]).im);
                            r4_13 = ((wr * tmpr) - (wi * tmpi));
                            i4_13 = ((wi * tmpr) + (wr * tmpi));
                            r3_5 = (r4_5 + r4_13);
                            i3_5 = (i4_5 + i4_13);
                            r3_13 = (r4_5 - r4_13);
                            i3_13 = (i4_5 - i4_13);
                        }
                        r2_1 = (r3_1 + r3_5);
                        i2_1 = (i3_1 + i3_5);
                        r2_9 = (r3_1 - r3_5);
                        i2_9 = (i3_1 - i3_5);
                        r2_5 = (r3_9 + i3_13);
                        i2_5 = (i3_9 - r3_13);
                        r2_13 = (r3_9 - i3_13);
                        i2_13 = (i3_9 + r3_13);
                    }
                    {
                        REAL r3_3, i3_3;
                        REAL r3_7, i3_7;
                        REAL r3_11, i3_11;
                        REAL r3_15, i3_15;
                        {
                            REAL r4_3, i4_3;
                            REAL r4_11, i4_11;
                            wr = ((W[3 * l1]).re);
                            wi = ((W[3 * l1]).im);
                            tmpr = ((jp[3 * m]).re);
                            tmpi = ((jp[3 * m]).im);
                            r4_3 = ((wr * tmpr) - (wi * tmpi));
                            i4_3 = ((wi * tmpr) + (wr * tmpi));
                            wr = ((W[11 * l1]).re);
                            wi = ((W[11 * l1]).im);
                            tmpr = ((jp[11 * m]).re);
                            tmpi = ((jp[11 * m]).im);
                            r4_11 = ((wr * tmpr) - (wi * tmpi));
                            i4_11 = ((wi * tmpr) + (wr * tmpi));
                            r3_3 = (r4_3 + r4_11);
                            i3_3 = (i4_3 + i4_11);
                            r3_11 = (r4_3 - r4_11);
                            i3_11 = (i4_3 - i4_11);
                        }
                        {
                            REAL r4_7, i4_7;
                            REAL r4_15, i4_15;
                            wr = ((W[7 * l1]).re);
                            wi = ((W[7 * l1]).im);
                            tmpr = ((jp[7 * m]).re);
                            tmpi = ((jp[7 * m]).im);
                            r4_7 = ((wr * tmpr) - (wi * tmpi));
                            i4_7 = ((wi * tmpr) + (wr * tmpi));
                            wr = ((W[15 * l1]).re);
                            wi = ((W[15 * l1]).im);
                            tmpr = ((jp[15 * m]).re);
                            tmpi = ((jp[15 * m]).im);
                            r4_15 = ((wr * tmpr) - (wi * tmpi));
                            i4_15 = ((wi * tmpr) + (wr * tmpi));
                            r3_7 = (r4_7 + r4_15);
                            i3_7 = (i4_7 + i4_15);
                            r3_15 = (r4_7 - r4_15);
                            i3_15 = (i4_7 - i4_15);
                        }
                        r2_3 = (r3_3 + r3_7);
                        i2_3 = (i3_3 + i3_7);
                        r2_11 = (r3_3 - r3_7);
                        i2_11 = (i3_3 - i3_7);
                        r2_7 = (r3_11 + i3_15);
                        i2_7 = (i3_11 - r3_15);
                        r2_15 = (r3_11 - i3_15);
                        i2_15 = (i3_11 + r3_15);
                    }
                    r1_1 = (r2_1 + r2_3);
                    i1_1 = (i2_1 + i2_3);
                    r1_9 = (r2_1 - r2_3);
                    i1_9 = (i2_1 - i2_3);
                    tmpr = (0.70710678118699999 * (r2_7 + i2_7));
                    tmpi = (0.70710678118699999 * (i2_7 - r2_7));
                    r1_3 = (r2_5 + tmpr);
                    i1_3 = (i2_5 + tmpi);
                    r1_11 = (r2_5 - tmpr);
                    i1_11 = (i2_5 - tmpi);
                    r1_5 = (r2_9 + i2_11);
                    i1_5 = (i2_9 - r2_11);
                    r1_13 = (r2_9 - i2_11);
                    i1_13 = (i2_9 + r2_11);
                    tmpr = (0.70710678118699999 * (i2_15 - r2_15));
                    tmpi = (0.70710678118699999 * (r2_15 + i2_15));
                    r1_7 = (r2_13 + tmpr);
                    i1_7 = (i2_13 - tmpi);
                    r1_15 = (r2_13 - tmpr);
                    i1_15 = (i2_13 + tmpi);
                }
                ((kp[0 * m]).re) = (r1_0 + r1_1);
                ((kp[0 * m]).im) = (i1_0 + i1_1);
                ((kp[8 * m]).re) = (r1_0 - r1_1);
                ((kp[8 * m]).im) = (i1_0 - i1_1);
                tmpr = ((0.92387953251099997 * r1_3) + (0.38268343236500002 * i1_3));
                tmpi = ((0.92387953251099997 * i1_3) - (0.38268343236500002 * r1_3));
                ((kp[1 * m]).re) = (r1_2 + tmpr);
                ((kp[1 * m]).im) = (i1_2 + tmpi);
                ((kp[9 * m]).re) = (r1_2 - tmpr);
                ((kp[9 * m]).im) = (i1_2 - tmpi);
                tmpr = (0.70710678118699999 * (r1_5 + i1_5));
                tmpi = (0.70710678118699999 * (i1_5 - r1_5));
                ((kp[2 * m]).re) = (r1_4 + tmpr);
                ((kp[2 * m]).im) = (i1_4 + tmpi);
                ((kp[10 * m]).re) = (r1_4 - tmpr);
                ((kp[10 * m]).im) = (i1_4 - tmpi);
                tmpr = ((0.38268343236500002 * r1_7) + (0.92387953251099997 * i1_7));
                tmpi = ((0.38268343236500002 * i1_7) - (0.92387953251099997 * r1_7));
                ((kp[3 * m]).re) = (r1_6 + tmpr);
                ((kp[3 * m]).im) = (i1_6 + tmpi);
                ((kp[11 * m]).re) = (r1_6 - tmpr);
                ((kp[11 * m]).im) = (i1_6 - tmpi);
                ((kp[4 * m]).re) = (r1_8 + i1_9);
                ((kp[4 * m]).im) = (i1_8 - r1_9);
                ((kp[12 * m]).re) = (r1_8 - i1_9);
                ((kp[12 * m]).im) = (i1_8 + r1_9);
                tmpr = ((0.92387953251099997 * i1_11) - (0.38268343236500002 * r1_11));
                tmpi = ((0.92387953251099997 * r1_11) + (0.38268343236500002 * i1_11));
                ((kp[5 * m]).re) = (r1_10 + tmpr);
                ((kp[5 * m]).im) = (i1_10 - tmpi);
                ((kp[13 * m]).re) = (r1_10 - tmpr);
                ((kp[13 * m]).im) = (i1_10 + tmpi);
                tmpr = (0.70710678118699999 * (i1_13 - r1_13));
                tmpi = (0.70710678118699999 * (r1_13 + i1_13));
                ((kp[6 * m]).re) = (r1_12 + tmpr);
                ((kp[6 * m]).im) = (i1_12 - tmpi);
                ((kp[14 * m]).re) = (r1_12 - tmpr);
                ((kp[14 * m]).im) = (i1_12 + tmpi);
                tmpr = ((0.38268343236500002 * i1_15) - (0.92387953251099997 * r1_15));
                tmpi = ((0.38268343236500002 * r1_15) + (0.92387953251099997 * i1_15));
                ((kp[7 * m]).re) = (r1_14 + tmpr);
                ((kp[7 * m]).im) = (i1_14 - tmpi);
                ((kp[15 * m]).re) = (r1_14 - tmpr);
                ((kp[15 * m]).im) = (i1_14 + tmpi);
            }
        }
    } else {
        int ab = (a + b) / 2;
        fft_twiddle_16_seq(a, ab, in, out, W, nW, nWdn, m);
        fft_twiddle_16_seq(ab, b, in, out, W, nW, nWdn, m);
    }
}
/*Function: fft_unshuffle_16_seq, ID: 42*/
void fft_unshuffle_16_seq(int a, int b, COMPLEX* in, COMPLEX* out, int m)
{
    /*fft_unshuffle_16_seq:42*/
    /*CompoundStmt:4901*/
    int i;
    const COMPLEX* ip;
    COMPLEX* jp;
    if ((b - a) < 128) {
        ip = in + a * 16;
        for (i = a; i < b; ++i) {
            jp = out + i;
            jp[0] = ip[0];
            jp[m] = ip[1];
            ip += 2;
            jp += 2 * m;
            jp[0] = ip[0];
            jp[m] = ip[1];
            ip += 2;
            jp += 2 * m;
            jp[0] = ip[0];
            jp[m] = ip[1];
            ip += 2;
            jp += 2 * m;
            jp[0] = ip[0];
            jp[m] = ip[1];
            ip += 2;
            jp += 2 * m;
            jp[0] = ip[0];
            jp[m] = ip[1];
            ip += 2;
            jp += 2 * m;
            jp[0] = ip[0];
            jp[m] = ip[1];
            ip += 2;
            jp += 2 * m;
            jp[0] = ip[0];
            jp[m] = ip[1];
            ip += 2;
            jp += 2 * m;
            jp[0] = ip[0];
            jp[m] = ip[1];
            ip += 2;
        }
    } else {
        int ab = (a + b) / 2;
        fft_unshuffle_16_seq(a, ab, in, out, m);
        fft_unshuffle_16_seq(ab, b, in, out, m);
    }
}
/*Function: fft_base_32, ID: 43*/
void fft_base_32(COMPLEX* in, COMPLEX* out)
{
    /*fft_base_32:43*/
    /*CompoundStmt:4994*/
    REAL tmpr, tmpi;
    {
        /*CompoundStmt:4996*/
        REAL r1_0, i1_0;
        REAL r1_1, i1_1;
        REAL r1_2, i1_2;
        REAL r1_3, i1_3;
        REAL r1_4, i1_4;
        REAL r1_5, i1_5;
        REAL r1_6, i1_6;
        REAL r1_7, i1_7;
        REAL r1_8, i1_8;
        REAL r1_9, i1_9;
        REAL r1_10, i1_10;
        REAL r1_11, i1_11;
        REAL r1_12, i1_12;
        REAL r1_13, i1_13;
        REAL r1_14, i1_14;
        REAL r1_15, i1_15;
        REAL r1_16, i1_16;
        REAL r1_17, i1_17;
        REAL r1_18, i1_18;
        REAL r1_19, i1_19;
        REAL r1_20, i1_20;
        REAL r1_21, i1_21;
        REAL r1_22, i1_22;
        REAL r1_23, i1_23;
        REAL r1_24, i1_24;
        REAL r1_25, i1_25;
        REAL r1_26, i1_26;
        REAL r1_27, i1_27;
        REAL r1_28, i1_28;
        REAL r1_29, i1_29;
        REAL r1_30, i1_30;
        REAL r1_31, i1_31;
        {
            /*CompoundStmt:5029*/
            REAL r2_0, i2_0;
            REAL r2_2, i2_2;
            REAL r2_4, i2_4;
            REAL r2_6, i2_6;
            REAL r2_8, i2_8;
            REAL r2_10, i2_10;
            REAL r2_12, i2_12;
            REAL r2_14, i2_14;
            REAL r2_16, i2_16;
            REAL r2_18, i2_18;
            REAL r2_20, i2_20;
            REAL r2_22, i2_22;
            REAL r2_24, i2_24;
            REAL r2_26, i2_26;
            REAL r2_28, i2_28;
            REAL r2_30, i2_30;
            {
                /*CompoundStmt:5046*/
                REAL r3_0, i3_0;
                REAL r3_4, i3_4;
                REAL r3_8, i3_8;
                REAL r3_12, i3_12;
                REAL r3_16, i3_16;
                REAL r3_20, i3_20;
                REAL r3_24, i3_24;
                REAL r3_28, i3_28;
                {
                    /*CompoundStmt:5055*/
                    REAL r4_0, i4_0;
                    REAL r4_8, i4_8;
                    REAL r4_16, i4_16;
                    REAL r4_24, i4_24;
                    {
                        /*CompoundStmt:5060*/
                        REAL r5_0, i5_0;
                        REAL r5_16, i5_16;
                        r5_0 = ((in[0]).re);
                        i5_0 = ((in[0]).im);
                        r5_16 = ((in[16]).re);
                        i5_16 = ((in[16]).im);
                        r4_0 = (r5_0 + r5_16);
                        i4_0 = (i5_0 + i5_16);
                        r4_16 = (r5_0 - r5_16);
                        i4_16 = (i5_0 - i5_16);
                    }
                    {
                        /*CompoundStmt:5083*/
                        REAL r5_8, i5_8;
                        REAL r5_24, i5_24;
                        r5_8 = ((in[8]).re);
                        i5_8 = ((in[8]).im);
                        r5_24 = ((in[24]).re);
                        i5_24 = ((in[24]).im);
                        r4_8 = (r5_8 + r5_24);
                        i4_8 = (i5_8 + i5_24);
                        r4_24 = (r5_8 - r5_24);
                        i4_24 = (i5_8 - i5_24);
                    }
                    r3_0 = (r4_0 + r4_8);
                    i3_0 = (i4_0 + i4_8);
                    r3_16 = (r4_0 - r4_8);
                    i3_16 = (i4_0 - i4_8);
                    r3_8 = (r4_16 + i4_24);
                    i3_8 = (i4_16 - r4_24);
                    r3_24 = (r4_16 - i4_24);
                    i3_24 = (i4_16 + r4_24);
                }
                {
                    /*CompoundStmt:5122*/
                    REAL r4_4, i4_4;
                    REAL r4_12, i4_12;
                    REAL r4_20, i4_20;
                    REAL r4_28, i4_28;
                    {
                        /*CompoundStmt:5127*/
                        REAL r5_4, i5_4;
                        REAL r5_20, i5_20;
                        r5_4 = ((in[4]).re);
                        i5_4 = ((in[4]).im);
                        r5_20 = ((in[20]).re);
                        i5_20 = ((in[20]).im);
                        r4_4 = (r5_4 + r5_20);
                        i4_4 = (i5_4 + i5_20);
                        r4_20 = (r5_4 - r5_20);
                        i4_20 = (i5_4 - i5_20);
                    }
                    {
                        /*CompoundStmt:5150*/
                        REAL r5_12, i5_12;
                        REAL r5_28, i5_28;
                        r5_12 = ((in[12]).re);
                        i5_12 = ((in[12]).im);
                        r5_28 = ((in[28]).re);
                        i5_28 = ((in[28]).im);
                        r4_12 = (r5_12 + r5_28);
                        i4_12 = (i5_12 + i5_28);
                        r4_28 = (r5_12 - r5_28);
                        i4_28 = (i5_12 - i5_28);
                    }
                    r3_4 = (r4_4 + r4_12);
                    i3_4 = (i4_4 + i4_12);
                    r3_20 = (r4_4 - r4_12);
                    i3_20 = (i4_4 - i4_12);
                    r3_12 = (r4_20 + i4_28);
                    i3_12 = (i4_20 - r4_28);
                    r3_28 = (r4_20 - i4_28);
                    i3_28 = (i4_20 + r4_28);
                }
                r2_0 = (r3_0 + r3_4);
                i2_0 = (i3_0 + i3_4);
                r2_16 = (r3_0 - r3_4);
                i2_16 = (i3_0 - i3_4);
                tmpr = (0.70710678118699999 * (r3_12 + i3_12));
                tmpi = (0.70710678118699999 * (i3_12 - r3_12));
                r2_4 = (r3_8 + tmpr);
                i2_4 = (i3_8 + tmpi);
                r2_20 = (r3_8 - tmpr);
                i2_20 = (i3_8 - tmpi);
                r2_8 = (r3_16 + i3_20);
                i2_8 = (i3_16 - r3_20);
                r2_24 = (r3_16 - i3_20);
                i2_24 = (i3_16 + r3_20);
                tmpr = (0.70710678118699999 * (i3_28 - r3_28));
                tmpi = (0.70710678118699999 * (r3_28 + i3_28));
                r2_12 = (r3_24 + tmpr);
                i2_12 = (i3_24 - tmpi);
                r2_28 = (r3_24 - tmpr);
                i2_28 = (i3_24 + tmpi);
            }
            {
                /*CompoundStmt:5237*/
                REAL r3_2, i3_2;
                REAL r3_6, i3_6;
                REAL r3_10, i3_10;
                REAL r3_14, i3_14;
                REAL r3_18, i3_18;
                REAL r3_22, i3_22;
                REAL r3_26, i3_26;
                REAL r3_30, i3_30;
                {
                    /*CompoundStmt:5246*/
                    REAL r4_2, i4_2;
                    REAL r4_10, i4_10;
                    REAL r4_18, i4_18;
                    REAL r4_26, i4_26;
                    {
                        /*CompoundStmt:5251*/
                        REAL r5_2, i5_2;
                        REAL r5_18, i5_18;
                        r5_2 = ((in[2]).re);
                        i5_2 = ((in[2]).im);
                        r5_18 = ((in[18]).re);
                        i5_18 = ((in[18]).im);
                        r4_2 = (r5_2 + r5_18);
                        i4_2 = (i5_2 + i5_18);
                        r4_18 = (r5_2 - r5_18);
                        i4_18 = (i5_2 - i5_18);
                    }
                    {
                        /*CompoundStmt:5274*/
                        REAL r5_10, i5_10;
                        REAL r5_26, i5_26;
                        r5_10 = ((in[10]).re);
                        i5_10 = ((in[10]).im);
                        r5_26 = ((in[26]).re);
                        i5_26 = ((in[26]).im);
                        r4_10 = (r5_10 + r5_26);
                        i4_10 = (i5_10 + i5_26);
                        r4_26 = (r5_10 - r5_26);
                        i4_26 = (i5_10 - i5_26);
                    }
                    r3_2 = (r4_2 + r4_10);
                    i3_2 = (i4_2 + i4_10);
                    r3_18 = (r4_2 - r4_10);
                    i3_18 = (i4_2 - i4_10);
                    r3_10 = (r4_18 + i4_26);
                    i3_10 = (i4_18 - r4_26);
                    r3_26 = (r4_18 - i4_26);
                    i3_26 = (i4_18 + r4_26);
                }
                {
                    /*CompoundStmt:5313*/
                    REAL r4_6, i4_6;
                    REAL r4_14, i4_14;
                    REAL r4_22, i4_22;
                    REAL r4_30, i4_30;
                    {
                        /*CompoundStmt:5318*/
                        REAL r5_6, i5_6;
                        REAL r5_22, i5_22;
                        r5_6 = ((in[6]).re);
                        i5_6 = ((in[6]).im);
                        r5_22 = ((in[22]).re);
                        i5_22 = ((in[22]).im);
                        r4_6 = (r5_6 + r5_22);
                        i4_6 = (i5_6 + i5_22);
                        r4_22 = (r5_6 - r5_22);
                        i4_22 = (i5_6 - i5_22);
                    }
                    {
                        /*CompoundStmt:5341*/
                        REAL r5_14, i5_14;
                        REAL r5_30, i5_30;
                        r5_14 = ((in[14]).re);
                        i5_14 = ((in[14]).im);
                        r5_30 = ((in[30]).re);
                        i5_30 = ((in[30]).im);
                        r4_14 = (r5_14 + r5_30);
                        i4_14 = (i5_14 + i5_30);
                        r4_30 = (r5_14 - r5_30);
                        i4_30 = (i5_14 - i5_30);
                    }
                    r3_6 = (r4_6 + r4_14);
                    i3_6 = (i4_6 + i4_14);
                    r3_22 = (r4_6 - r4_14);
                    i3_22 = (i4_6 - i4_14);
                    r3_14 = (r4_22 + i4_30);
                    i3_14 = (i4_22 - r4_30);
                    r3_30 = (r4_22 - i4_30);
                    i3_30 = (i4_22 + r4_30);
                }
                r2_2 = (r3_2 + r3_6);
                i2_2 = (i3_2 + i3_6);
                r2_18 = (r3_2 - r3_6);
                i2_18 = (i3_2 - i3_6);
                tmpr = (0.70710678118699999 * (r3_14 + i3_14));
                tmpi = (0.70710678118699999 * (i3_14 - r3_14));
                r2_6 = (r3_10 + tmpr);
                i2_6 = (i3_10 + tmpi);
                r2_22 = (r3_10 - tmpr);
                i2_22 = (i3_10 - tmpi);
                r2_10 = (r3_18 + i3_22);
                i2_10 = (i3_18 - r3_22);
                r2_26 = (r3_18 - i3_22);
                i2_26 = (i3_18 + r3_22);
                tmpr = (0.70710678118699999 * (i3_30 - r3_30));
                tmpi = (0.70710678118699999 * (r3_30 + i3_30));
                r2_14 = (r3_26 + tmpr);
                i2_14 = (i3_26 - tmpi);
                r2_30 = (r3_26 - tmpr);
                i2_30 = (i3_26 + tmpi);
            }
            r1_0 = (r2_0 + r2_2);
            i1_0 = (i2_0 + i2_2);
            r1_16 = (r2_0 - r2_2);
            i1_16 = (i2_0 - i2_2);
            tmpr = ((0.92387953251099997 * r2_6) + (0.38268343236500002 * i2_6));
            tmpi = ((0.92387953251099997 * i2_6) - (0.38268343236500002 * r2_6));
            r1_2 = (r2_4 + tmpr);
            i1_2 = (i2_4 + tmpi);
            r1_18 = (r2_4 - tmpr);
            i1_18 = (i2_4 - tmpi);
            tmpr = (0.70710678118699999 * (r2_10 + i2_10));
            tmpi = (0.70710678118699999 * (i2_10 - r2_10));
            r1_4 = (r2_8 + tmpr);
            i1_4 = (i2_8 + tmpi);
            r1_20 = (r2_8 - tmpr);
            i1_20 = (i2_8 - tmpi);
            tmpr = ((0.38268343236500002 * r2_14) + (0.92387953251099997 * i2_14));
            tmpi = ((0.38268343236500002 * i2_14) - (0.92387953251099997 * r2_14));
            r1_6 = (r2_12 + tmpr);
            i1_6 = (i2_12 + tmpi);
            r1_22 = (r2_12 - tmpr);
            i1_22 = (i2_12 - tmpi);
            r1_8 = (r2_16 + i2_18);
            i1_8 = (i2_16 - r2_18);
            r1_24 = (r2_16 - i2_18);
            i1_24 = (i2_16 + r2_18);
            tmpr = ((0.92387953251099997 * i2_22) - (0.38268343236500002 * r2_22));
            tmpi = ((0.92387953251099997 * r2_22) + (0.38268343236500002 * i2_22));
            r1_10 = (r2_20 + tmpr);
            i1_10 = (i2_20 - tmpi);
            r1_26 = (r2_20 - tmpr);
            i1_26 = (i2_20 + tmpi);
            tmpr = (0.70710678118699999 * (i2_26 - r2_26));
            tmpi = (0.70710678118699999 * (r2_26 + i2_26));
            r1_12 = (r2_24 + tmpr);
            i1_12 = (i2_24 - tmpi);
            r1_28 = (r2_24 - tmpr);
            i1_28 = (i2_24 + tmpi);
            tmpr = ((0.38268343236500002 * i2_30) - (0.92387953251099997 * r2_30));
            tmpi = ((0.38268343236500002 * r2_30) + (0.92387953251099997 * i2_30));
            r1_14 = (r2_28 + tmpr);
            i1_14 = (i2_28 - tmpi);
            r1_30 = (r2_28 - tmpr);
            i1_30 = (i2_28 + tmpi);
        }
        {
            /*CompoundStmt:5556*/
            REAL r2_1, i2_1;
            REAL r2_3, i2_3;
            REAL r2_5, i2_5;
            REAL r2_7, i2_7;
            REAL r2_9, i2_9;
            REAL r2_11, i2_11;
            REAL r2_13, i2_13;
            REAL r2_15, i2_15;
            REAL r2_17, i2_17;
            REAL r2_19, i2_19;
            REAL r2_21, i2_21;
            REAL r2_23, i2_23;
            REAL r2_25, i2_25;
            REAL r2_27, i2_27;
            REAL r2_29, i2_29;
            REAL r2_31, i2_31;
            {
                /*CompoundStmt:5573*/
                REAL r3_1, i3_1;
                REAL r3_5, i3_5;
                REAL r3_9, i3_9;
                REAL r3_13, i3_13;
                REAL r3_17, i3_17;
                REAL r3_21, i3_21;
                REAL r3_25, i3_25;
                REAL r3_29, i3_29;
                {
                    /*CompoundStmt:5582*/
                    REAL r4_1, i4_1;
                    REAL r4_9, i4_9;
                    REAL r4_17, i4_17;
                    REAL r4_25, i4_25;
                    {
                        /*CompoundStmt:5587*/
                        REAL r5_1, i5_1;
                        REAL r5_17, i5_17;
                        r5_1 = ((in[1]).re);
                        i5_1 = ((in[1]).im);
                        r5_17 = ((in[17]).re);
                        i5_17 = ((in[17]).im);
                        r4_1 = (r5_1 + r5_17);
                        i4_1 = (i5_1 + i5_17);
                        r4_17 = (r5_1 - r5_17);
                        i4_17 = (i5_1 - i5_17);
                    }
                    {
                        /*CompoundStmt:5610*/
                        REAL r5_9, i5_9;
                        REAL r5_25, i5_25;
                        r5_9 = ((in[9]).re);
                        i5_9 = ((in[9]).im);
                        r5_25 = ((in[25]).re);
                        i5_25 = ((in[25]).im);
                        r4_9 = (r5_9 + r5_25);
                        i4_9 = (i5_9 + i5_25);
                        r4_25 = (r5_9 - r5_25);
                        i4_25 = (i5_9 - i5_25);
                    }
                    r3_1 = (r4_1 + r4_9);
                    i3_1 = (i4_1 + i4_9);
                    r3_17 = (r4_1 - r4_9);
                    i3_17 = (i4_1 - i4_9);
                    r3_9 = (r4_17 + i4_25);
                    i3_9 = (i4_17 - r4_25);
                    r3_25 = (r4_17 - i4_25);
                    i3_25 = (i4_17 + r4_25);
                }
                {
                    /*CompoundStmt:5649*/
                    REAL r4_5, i4_5;
                    REAL r4_13, i4_13;
                    REAL r4_21, i4_21;
                    REAL r4_29, i4_29;
                    {
                        /*CompoundStmt:5654*/
                        REAL r5_5, i5_5;
                        REAL r5_21, i5_21;
                        r5_5 = ((in[5]).re);
                        i5_5 = ((in[5]).im);
                        r5_21 = ((in[21]).re);
                        i5_21 = ((in[21]).im);
                        r4_5 = (r5_5 + r5_21);
                        i4_5 = (i5_5 + i5_21);
                        r4_21 = (r5_5 - r5_21);
                        i4_21 = (i5_5 - i5_21);
                    }
                    {
                        /*CompoundStmt:5677*/
                        REAL r5_13, i5_13;
                        REAL r5_29, i5_29;
                        r5_13 = ((in[13]).re);
                        i5_13 = ((in[13]).im);
                        r5_29 = ((in[29]).re);
                        i5_29 = ((in[29]).im);
                        r4_13 = (r5_13 + r5_29);
                        i4_13 = (i5_13 + i5_29);
                        r4_29 = (r5_13 - r5_29);
                        i4_29 = (i5_13 - i5_29);
                    }
                    r3_5 = (r4_5 + r4_13);
                    i3_5 = (i4_5 + i4_13);
                    r3_21 = (r4_5 - r4_13);
                    i3_21 = (i4_5 - i4_13);
                    r3_13 = (r4_21 + i4_29);
                    i3_13 = (i4_21 - r4_29);
                    r3_29 = (r4_21 - i4_29);
                    i3_29 = (i4_21 + r4_29);
                }
                r2_1 = (r3_1 + r3_5);
                i2_1 = (i3_1 + i3_5);
                r2_17 = (r3_1 - r3_5);
                i2_17 = (i3_1 - i3_5);
                tmpr = (0.70710678118699999 * (r3_13 + i3_13));
                tmpi = (0.70710678118699999 * (i3_13 - r3_13));
                r2_5 = (r3_9 + tmpr);
                i2_5 = (i3_9 + tmpi);
                r2_21 = (r3_9 - tmpr);
                i2_21 = (i3_9 - tmpi);
                r2_9 = (r3_17 + i3_21);
                i2_9 = (i3_17 - r3_21);
                r2_25 = (r3_17 - i3_21);
                i2_25 = (i3_17 + r3_21);
                tmpr = (0.70710678118699999 * (i3_29 - r3_29));
                tmpi = (0.70710678118699999 * (r3_29 + i3_29));
                r2_13 = (r3_25 + tmpr);
                i2_13 = (i3_25 - tmpi);
                r2_29 = (r3_25 - tmpr);
                i2_29 = (i3_25 + tmpi);
            }
            {
                /*CompoundStmt:5764*/
                REAL r3_3, i3_3;
                REAL r3_7, i3_7;
                REAL r3_11, i3_11;
                REAL r3_15, i3_15;
                REAL r3_19, i3_19;
                REAL r3_23, i3_23;
                REAL r3_27, i3_27;
                REAL r3_31, i3_31;
                {
                    /*CompoundStmt:5773*/
                    REAL r4_3, i4_3;
                    REAL r4_11, i4_11;
                    REAL r4_19, i4_19;
                    REAL r4_27, i4_27;
                    {
                        /*CompoundStmt:5778*/
                        REAL r5_3, i5_3;
                        REAL r5_19, i5_19;
                        r5_3 = ((in[3]).re);
                        i5_3 = ((in[3]).im);
                        r5_19 = ((in[19]).re);
                        i5_19 = ((in[19]).im);
                        r4_3 = (r5_3 + r5_19);
                        i4_3 = (i5_3 + i5_19);
                        r4_19 = (r5_3 - r5_19);
                        i4_19 = (i5_3 - i5_19);
                    }
                    {
                        /*CompoundStmt:5801*/
                        REAL r5_11, i5_11;
                        REAL r5_27, i5_27;
                        r5_11 = ((in[11]).re);
                        i5_11 = ((in[11]).im);
                        r5_27 = ((in[27]).re);
                        i5_27 = ((in[27]).im);
                        r4_11 = (r5_11 + r5_27);
                        i4_11 = (i5_11 + i5_27);
                        r4_27 = (r5_11 - r5_27);
                        i4_27 = (i5_11 - i5_27);
                    }
                    r3_3 = (r4_3 + r4_11);
                    i3_3 = (i4_3 + i4_11);
                    r3_19 = (r4_3 - r4_11);
                    i3_19 = (i4_3 - i4_11);
                    r3_11 = (r4_19 + i4_27);
                    i3_11 = (i4_19 - r4_27);
                    r3_27 = (r4_19 - i4_27);
                    i3_27 = (i4_19 + r4_27);
                }
                {
                    /*CompoundStmt:5840*/
                    REAL r4_7, i4_7;
                    REAL r4_15, i4_15;
                    REAL r4_23, i4_23;
                    REAL r4_31, i4_31;
                    {
                        /*CompoundStmt:5845*/
                        REAL r5_7, i5_7;
                        REAL r5_23, i5_23;
                        r5_7 = ((in[7]).re);
                        i5_7 = ((in[7]).im);
                        r5_23 = ((in[23]).re);
                        i5_23 = ((in[23]).im);
                        r4_7 = (r5_7 + r5_23);
                        i4_7 = (i5_7 + i5_23);
                        r4_23 = (r5_7 - r5_23);
                        i4_23 = (i5_7 - i5_23);
                    }
                    {
                        /*CompoundStmt:5868*/
                        REAL r5_15, i5_15;
                        REAL r5_31, i5_31;
                        r5_15 = ((in[15]).re);
                        i5_15 = ((in[15]).im);
                        r5_31 = ((in[31]).re);
                        i5_31 = ((in[31]).im);
                        r4_15 = (r5_15 + r5_31);
                        i4_15 = (i5_15 + i5_31);
                        r4_31 = (r5_15 - r5_31);
                        i4_31 = (i5_15 - i5_31);
                    }
                    r3_7 = (r4_7 + r4_15);
                    i3_7 = (i4_7 + i4_15);
                    r3_23 = (r4_7 - r4_15);
                    i3_23 = (i4_7 - i4_15);
                    r3_15 = (r4_23 + i4_31);
                    i3_15 = (i4_23 - r4_31);
                    r3_31 = (r4_23 - i4_31);
                    i3_31 = (i4_23 + r4_31);
                }
                r2_3 = (r3_3 + r3_7);
                i2_3 = (i3_3 + i3_7);
                r2_19 = (r3_3 - r3_7);
                i2_19 = (i3_3 - i3_7);
                tmpr = (0.70710678118699999 * (r3_15 + i3_15));
                tmpi = (0.70710678118699999 * (i3_15 - r3_15));
                r2_7 = (r3_11 + tmpr);
                i2_7 = (i3_11 + tmpi);
                r2_23 = (r3_11 - tmpr);
                i2_23 = (i3_11 - tmpi);
                r2_11 = (r3_19 + i3_23);
                i2_11 = (i3_19 - r3_23);
                r2_27 = (r3_19 - i3_23);
                i2_27 = (i3_19 + r3_23);
                tmpr = (0.70710678118699999 * (i3_31 - r3_31));
                tmpi = (0.70710678118699999 * (r3_31 + i3_31));
                r2_15 = (r3_27 + tmpr);
                i2_15 = (i3_27 - tmpi);
                r2_31 = (r3_27 - tmpr);
                i2_31 = (i3_27 + tmpi);
            }
            r1_1 = (r2_1 + r2_3);
            i1_1 = (i2_1 + i2_3);
            r1_17 = (r2_1 - r2_3);
            i1_17 = (i2_1 - i2_3);
            tmpr = ((0.92387953251099997 * r2_7) + (0.38268343236500002 * i2_7));
            tmpi = ((0.92387953251099997 * i2_7) - (0.38268343236500002 * r2_7));
            r1_3 = (r2_5 + tmpr);
            i1_3 = (i2_5 + tmpi);
            r1_19 = (r2_5 - tmpr);
            i1_19 = (i2_5 - tmpi);
            tmpr = (0.70710678118699999 * (r2_11 + i2_11));
            tmpi = (0.70710678118699999 * (i2_11 - r2_11));
            r1_5 = (r2_9 + tmpr);
            i1_5 = (i2_9 + tmpi);
            r1_21 = (r2_9 - tmpr);
            i1_21 = (i2_9 - tmpi);
            tmpr = ((0.38268343236500002 * r2_15) + (0.92387953251099997 * i2_15));
            tmpi = ((0.38268343236500002 * i2_15) - (0.92387953251099997 * r2_15));
            r1_7 = (r2_13 + tmpr);
            i1_7 = (i2_13 + tmpi);
            r1_23 = (r2_13 - tmpr);
            i1_23 = (i2_13 - tmpi);
            r1_9 = (r2_17 + i2_19);
            i1_9 = (i2_17 - r2_19);
            r1_25 = (r2_17 - i2_19);
            i1_25 = (i2_17 + r2_19);
            tmpr = ((0.92387953251099997 * i2_23) - (0.38268343236500002 * r2_23));
            tmpi = ((0.92387953251099997 * r2_23) + (0.38268343236500002 * i2_23));
            r1_11 = (r2_21 + tmpr);
            i1_11 = (i2_21 - tmpi);
            r1_27 = (r2_21 - tmpr);
            i1_27 = (i2_21 + tmpi);
            tmpr = (0.70710678118699999 * (i2_27 - r2_27));
            tmpi = (0.70710678118699999 * (r2_27 + i2_27));
            r1_13 = (r2_25 + tmpr);
            i1_13 = (i2_25 - tmpi);
            r1_29 = (r2_25 - tmpr);
            i1_29 = (i2_25 + tmpi);
            tmpr = ((0.38268343236500002 * i2_31) - (0.92387953251099997 * r2_31));
            tmpi = ((0.38268343236500002 * r2_31) + (0.92387953251099997 * i2_31));
            r1_15 = (r2_29 + tmpr);
            i1_15 = (i2_29 - tmpi);
            r1_31 = (r2_29 - tmpr);
            i1_31 = (i2_29 + tmpi);
        }
        ((out[0]).re) = (r1_0 + r1_1);
        ((out[0]).im) = (i1_0 + i1_1);
        ((out[16]).re) = (r1_0 - r1_1);
        ((out[16]).im) = (i1_0 - i1_1);
        tmpr = ((0.98078528040299994 * r1_3) + (0.19509032201599999 * i1_3));
        tmpi = ((0.98078528040299994 * i1_3) - (0.19509032201599999 * r1_3));
        ((out[1]).re) = (r1_2 + tmpr);
        ((out[1]).im) = (i1_2 + tmpi);
        ((out[17]).re) = (r1_2 - tmpr);
        ((out[17]).im) = (i1_2 - tmpi);
        tmpr = ((0.92387953251099997 * r1_5) + (0.38268343236500002 * i1_5));
        tmpi = ((0.92387953251099997 * i1_5) - (0.38268343236500002 * r1_5));
        ((out[2]).re) = (r1_4 + tmpr);
        ((out[2]).im) = (i1_4 + tmpi);
        ((out[18]).re) = (r1_4 - tmpr);
        ((out[18]).im) = (i1_4 - tmpi);
        tmpr = ((0.83146961230299998 * r1_7) + (0.55557023301999997 * i1_7));
        tmpi = ((0.83146961230299998 * i1_7) - (0.55557023301999997 * r1_7));
        ((out[3]).re) = (r1_6 + tmpr);
        ((out[3]).im) = (i1_6 + tmpi);
        ((out[19]).re) = (r1_6 - tmpr);
        ((out[19]).im) = (i1_6 - tmpi);
        tmpr = (0.70710678118699999 * (r1_9 + i1_9));
        tmpi = (0.70710678118699999 * (i1_9 - r1_9));
        ((out[4]).re) = (r1_8 + tmpr);
        ((out[4]).im) = (i1_8 + tmpi);
        ((out[20]).re) = (r1_8 - tmpr);
        ((out[20]).im) = (i1_8 - tmpi);
        tmpr = ((0.55557023301999997 * r1_11) + (0.83146961230299998 * i1_11));
        tmpi = ((0.55557023301999997 * i1_11) - (0.83146961230299998 * r1_11));
        ((out[5]).re) = (r1_10 + tmpr);
        ((out[5]).im) = (i1_10 + tmpi);
        ((out[21]).re) = (r1_10 - tmpr);
        ((out[21]).im) = (i1_10 - tmpi);
        tmpr = ((0.38268343236500002 * r1_13) + (0.92387953251099997 * i1_13));
        tmpi = ((0.38268343236500002 * i1_13) - (0.92387953251099997 * r1_13));
        ((out[6]).re) = (r1_12 + tmpr);
        ((out[6]).im) = (i1_12 + tmpi);
        ((out[22]).re) = (r1_12 - tmpr);
        ((out[22]).im) = (i1_12 - tmpi);
        tmpr = ((0.19509032201599999 * r1_15) + (0.98078528040299994 * i1_15));
        tmpi = ((0.19509032201599999 * i1_15) - (0.98078528040299994 * r1_15));
        ((out[7]).re) = (r1_14 + tmpr);
        ((out[7]).im) = (i1_14 + tmpi);
        ((out[23]).re) = (r1_14 - tmpr);
        ((out[23]).im) = (i1_14 - tmpi);
        ((out[8]).re) = (r1_16 + i1_17);
        ((out[8]).im) = (i1_16 - r1_17);
        ((out[24]).re) = (r1_16 - i1_17);
        ((out[24]).im) = (i1_16 + r1_17);
        tmpr = ((0.98078528040299994 * i1_19) - (0.19509032201599999 * r1_19));
        tmpi = ((0.98078528040299994 * r1_19) + (0.19509032201599999 * i1_19));
        ((out[9]).re) = (r1_18 + tmpr);
        ((out[9]).im) = (i1_18 - tmpi);
        ((out[25]).re) = (r1_18 - tmpr);
        ((out[25]).im) = (i1_18 + tmpi);
        tmpr = ((0.92387953251099997 * i1_21) - (0.38268343236500002 * r1_21));
        tmpi = ((0.92387953251099997 * r1_21) + (0.38268343236500002 * i1_21));
        ((out[10]).re) = (r1_20 + tmpr);
        ((out[10]).im) = (i1_20 - tmpi);
        ((out[26]).re) = (r1_20 - tmpr);
        ((out[26]).im) = (i1_20 + tmpi);
        tmpr = ((0.83146961230299998 * i1_23) - (0.55557023301999997 * r1_23));
        tmpi = ((0.83146961230299998 * r1_23) + (0.55557023301999997 * i1_23));
        ((out[11]).re) = (r1_22 + tmpr);
        ((out[11]).im) = (i1_22 - tmpi);
        ((out[27]).re) = (r1_22 - tmpr);
        ((out[27]).im) = (i1_22 + tmpi);
        tmpr = (0.70710678118699999 * (i1_25 - r1_25));
        tmpi = (0.70710678118699999 * (r1_25 + i1_25));
        ((out[12]).re) = (r1_24 + tmpr);
        ((out[12]).im) = (i1_24 - tmpi);
        ((out[28]).re) = (r1_24 - tmpr);
        ((out[28]).im) = (i1_24 + tmpi);
        tmpr = ((0.55557023301999997 * i1_27) - (0.83146961230299998 * r1_27));
        tmpi = ((0.55557023301999997 * r1_27) + (0.83146961230299998 * i1_27));
        ((out[13]).re) = (r1_26 + tmpr);
        ((out[13]).im) = (i1_26 - tmpi);
        ((out[29]).re) = (r1_26 - tmpr);
        ((out[29]).im) = (i1_26 + tmpi);
        tmpr = ((0.38268343236500002 * i1_29) - (0.92387953251099997 * r1_29));
        tmpi = ((0.38268343236500002 * r1_29) + (0.92387953251099997 * i1_29));
        ((out[14]).re) = (r1_28 + tmpr);
        ((out[14]).im) = (i1_28 - tmpi);
        ((out[30]).re) = (r1_28 - tmpr);
        ((out[30]).im) = (i1_28 + tmpi);
        tmpr = ((0.19509032201599999 * i1_31) - (0.98078528040299994 * r1_31));
        tmpi = ((0.19509032201599999 * r1_31) + (0.98078528040299994 * i1_31));
        ((out[15]).re) = (r1_30 + tmpr);
        ((out[15]).im) = (i1_30 - tmpi);
        ((out[31]).re) = (r1_30 - tmpr);
        ((out[31]).im) = (i1_30 + tmpi);
    }
}
/*Function: fft_twiddle_32_seq, ID: 45*/
void fft_twiddle_32_seq(
    int a, int b, COMPLEX* in, COMPLEX* out, COMPLEX* W, int nW, int nWdn, int m)
{
    /*fft_twiddle_32_seq:45*/
    /*CompoundStmt:8655*/
    int l1, i;
    COMPLEX *jp, *kp;
    REAL tmpr, tmpi, wr, wi;
    if ((b - a) < 128) {
        for (i = a, l1 = nWdn * i, kp = out + i; i < b; i++, l1 += nWdn, kp++) {
            jp = in + i;
            {
                REAL r1_0, i1_0;
                REAL r1_1, i1_1;
                REAL r1_2, i1_2;
                REAL r1_3, i1_3;
                REAL r1_4, i1_4;
                REAL r1_5, i1_5;
                REAL r1_6, i1_6;
                REAL r1_7, i1_7;
                REAL r1_8, i1_8;
                REAL r1_9, i1_9;
                REAL r1_10, i1_10;
                REAL r1_11, i1_11;
                REAL r1_12, i1_12;
                REAL r1_13, i1_13;
                REAL r1_14, i1_14;
                REAL r1_15, i1_15;
                REAL r1_16, i1_16;
                REAL r1_17, i1_17;
                REAL r1_18, i1_18;
                REAL r1_19, i1_19;
                REAL r1_20, i1_20;
                REAL r1_21, i1_21;
                REAL r1_22, i1_22;
                REAL r1_23, i1_23;
                REAL r1_24, i1_24;
                REAL r1_25, i1_25;
                REAL r1_26, i1_26;
                REAL r1_27, i1_27;
                REAL r1_28, i1_28;
                REAL r1_29, i1_29;
                REAL r1_30, i1_30;
                REAL r1_31, i1_31;
                {
                    REAL r2_0, i2_0;
                    REAL r2_2, i2_2;
                    REAL r2_4, i2_4;
                    REAL r2_6, i2_6;
                    REAL r2_8, i2_8;
                    REAL r2_10, i2_10;
                    REAL r2_12, i2_12;
                    REAL r2_14, i2_14;
                    REAL r2_16, i2_16;
                    REAL r2_18, i2_18;
                    REAL r2_20, i2_20;
                    REAL r2_22, i2_22;
                    REAL r2_24, i2_24;
                    REAL r2_26, i2_26;
                    REAL r2_28, i2_28;
                    REAL r2_30, i2_30;
                    {
                        REAL r3_0, i3_0;
                        REAL r3_4, i3_4;
                        REAL r3_8, i3_8;
                        REAL r3_12, i3_12;
                        REAL r3_16, i3_16;
                        REAL r3_20, i3_20;
                        REAL r3_24, i3_24;
                        REAL r3_28, i3_28;
                        {
                            REAL r4_0, i4_0;
                            REAL r4_8, i4_8;
                            REAL r4_16, i4_16;
                            REAL r4_24, i4_24;
                            {
                                REAL r5_0, i5_0;
                                REAL r5_16, i5_16;
                                r5_0 = ((jp[0 * m]).re);
                                i5_0 = ((jp[0 * m]).im);
                                wr = ((W[16 * l1]).re);
                                wi = ((W[16 * l1]).im);
                                tmpr = ((jp[16 * m]).re);
                                tmpi = ((jp[16 * m]).im);
                                r5_16 = ((wr * tmpr) - (wi * tmpi));
                                i5_16 = ((wi * tmpr) + (wr * tmpi));
                                r4_0 = (r5_0 + r5_16);
                                i4_0 = (i5_0 + i5_16);
                                r4_16 = (r5_0 - r5_16);
                                i4_16 = (i5_0 - i5_16);
                            }
                            {
                                REAL r5_8, i5_8;
                                REAL r5_24, i5_24;
                                wr = ((W[8 * l1]).re);
                                wi = ((W[8 * l1]).im);
                                tmpr = ((jp[8 * m]).re);
                                tmpi = ((jp[8 * m]).im);
                                r5_8 = ((wr * tmpr) - (wi * tmpi));
                                i5_8 = ((wi * tmpr) + (wr * tmpi));
                                wr = ((W[24 * l1]).re);
                                wi = ((W[24 * l1]).im);
                                tmpr = ((jp[24 * m]).re);
                                tmpi = ((jp[24 * m]).im);
                                r5_24 = ((wr * tmpr) - (wi * tmpi));
                                i5_24 = ((wi * tmpr) + (wr * tmpi));
                                r4_8 = (r5_8 + r5_24);
                                i4_8 = (i5_8 + i5_24);
                                r4_24 = (r5_8 - r5_24);
                                i4_24 = (i5_8 - i5_24);
                            }
                            r3_0 = (r4_0 + r4_8);
                            i3_0 = (i4_0 + i4_8);
                            r3_16 = (r4_0 - r4_8);
                            i3_16 = (i4_0 - i4_8);
                            r3_8 = (r4_16 + i4_24);
                            i3_8 = (i4_16 - r4_24);
                            r3_24 = (r4_16 - i4_24);
                            i3_24 = (i4_16 + r4_24);
                        }
                        {
                            REAL r4_4, i4_4;
                            REAL r4_12, i4_12;
                            REAL r4_20, i4_20;
                            REAL r4_28, i4_28;
                            {
                                REAL r5_4, i5_4;
                                REAL r5_20, i5_20;
                                wr = ((W[4 * l1]).re);
                                wi = ((W[4 * l1]).im);
                                tmpr = ((jp[4 * m]).re);
                                tmpi = ((jp[4 * m]).im);
                                r5_4 = ((wr * tmpr) - (wi * tmpi));
                                i5_4 = ((wi * tmpr) + (wr * tmpi));
                                wr = ((W[20 * l1]).re);
                                wi = ((W[20 * l1]).im);
                                tmpr = ((jp[20 * m]).re);
                                tmpi = ((jp[20 * m]).im);
                                r5_20 = ((wr * tmpr) - (wi * tmpi));
                                i5_20 = ((wi * tmpr) + (wr * tmpi));
                                r4_4 = (r5_4 + r5_20);
                                i4_4 = (i5_4 + i5_20);
                                r4_20 = (r5_4 - r5_20);
                                i4_20 = (i5_4 - i5_20);
                            }
                            {
                                REAL r5_12, i5_12;
                                REAL r5_28, i5_28;
                                wr = ((W[12 * l1]).re);
                                wi = ((W[12 * l1]).im);
                                tmpr = ((jp[12 * m]).re);
                                tmpi = ((jp[12 * m]).im);
                                r5_12 = ((wr * tmpr) - (wi * tmpi));
                                i5_12 = ((wi * tmpr) + (wr * tmpi));
                                wr = ((W[28 * l1]).re);
                                wi = ((W[28 * l1]).im);
                                tmpr = ((jp[28 * m]).re);
                                tmpi = ((jp[28 * m]).im);
                                r5_28 = ((wr * tmpr) - (wi * tmpi));
                                i5_28 = ((wi * tmpr) + (wr * tmpi));
                                r4_12 = (r5_12 + r5_28);
                                i4_12 = (i5_12 + i5_28);
                                r4_28 = (r5_12 - r5_28);
                                i4_28 = (i5_12 - i5_28);
                            }
                            r3_4 = (r4_4 + r4_12);
                            i3_4 = (i4_4 + i4_12);
                            r3_20 = (r4_4 - r4_12);
                            i3_20 = (i4_4 - i4_12);
                            r3_12 = (r4_20 + i4_28);
                            i3_12 = (i4_20 - r4_28);
                            r3_28 = (r4_20 - i4_28);
                            i3_28 = (i4_20 + r4_28);
                        }
                        r2_0 = (r3_0 + r3_4);
                        i2_0 = (i3_0 + i3_4);
                        r2_16 = (r3_0 - r3_4);
                        i2_16 = (i3_0 - i3_4);
                        tmpr = (0.70710678118699999 * (r3_12 + i3_12));
                        tmpi = (0.70710678118699999 * (i3_12 - r3_12));
                        r2_4 = (r3_8 + tmpr);
                        i2_4 = (i3_8 + tmpi);
                        r2_20 = (r3_8 - tmpr);
                        i2_20 = (i3_8 - tmpi);
                        r2_8 = (r3_16 + i3_20);
                        i2_8 = (i3_16 - r3_20);
                        r2_24 = (r3_16 - i3_20);
                        i2_24 = (i3_16 + r3_20);
                        tmpr = (0.70710678118699999 * (i3_28 - r3_28));
                        tmpi = (0.70710678118699999 * (r3_28 + i3_28));
                        r2_12 = (r3_24 + tmpr);
                        i2_12 = (i3_24 - tmpi);
                        r2_28 = (r3_24 - tmpr);
                        i2_28 = (i3_24 + tmpi);
                    }
                    {
                        REAL r3_2, i3_2;
                        REAL r3_6, i3_6;
                        REAL r3_10, i3_10;
                        REAL r3_14, i3_14;
                        REAL r3_18, i3_18;
                        REAL r3_22, i3_22;
                        REAL r3_26, i3_26;
                        REAL r3_30, i3_30;
                        {
                            REAL r4_2, i4_2;
                            REAL r4_10, i4_10;
                            REAL r4_18, i4_18;
                            REAL r4_26, i4_26;
                            {
                                REAL r5_2, i5_2;
                                REAL r5_18, i5_18;
                                wr = ((W[2 * l1]).re);
                                wi = ((W[2 * l1]).im);
                                tmpr = ((jp[2 * m]).re);
                                tmpi = ((jp[2 * m]).im);
                                r5_2 = ((wr * tmpr) - (wi * tmpi));
                                i5_2 = ((wi * tmpr) + (wr * tmpi));
                                wr = ((W[18 * l1]).re);
                                wi = ((W[18 * l1]).im);
                                tmpr = ((jp[18 * m]).re);
                                tmpi = ((jp[18 * m]).im);
                                r5_18 = ((wr * tmpr) - (wi * tmpi));
                                i5_18 = ((wi * tmpr) + (wr * tmpi));
                                r4_2 = (r5_2 + r5_18);
                                i4_2 = (i5_2 + i5_18);
                                r4_18 = (r5_2 - r5_18);
                                i4_18 = (i5_2 - i5_18);
                            }
                            {
                                REAL r5_10, i5_10;
                                REAL r5_26, i5_26;
                                wr = ((W[10 * l1]).re);
                                wi = ((W[10 * l1]).im);
                                tmpr = ((jp[10 * m]).re);
                                tmpi = ((jp[10 * m]).im);
                                r5_10 = ((wr * tmpr) - (wi * tmpi));
                                i5_10 = ((wi * tmpr) + (wr * tmpi));
                                wr = ((W[26 * l1]).re);
                                wi = ((W[26 * l1]).im);
                                tmpr = ((jp[26 * m]).re);
                                tmpi = ((jp[26 * m]).im);
                                r5_26 = ((wr * tmpr) - (wi * tmpi));
                                i5_26 = ((wi * tmpr) + (wr * tmpi));
                                r4_10 = (r5_10 + r5_26);
                                i4_10 = (i5_10 + i5_26);
                                r4_26 = (r5_10 - r5_26);
                                i4_26 = (i5_10 - i5_26);
                            }
                            r3_2 = (r4_2 + r4_10);
                            i3_2 = (i4_2 + i4_10);
                            r3_18 = (r4_2 - r4_10);
                            i3_18 = (i4_2 - i4_10);
                            r3_10 = (r4_18 + i4_26);
                            i3_10 = (i4_18 - r4_26);
                            r3_26 = (r4_18 - i4_26);
                            i3_26 = (i4_18 + r4_26);
                        }
                        {
                            REAL r4_6, i4_6;
                            REAL r4_14, i4_14;
                            REAL r4_22, i4_22;
                            REAL r4_30, i4_30;
                            {
                                REAL r5_6, i5_6;
                                REAL r5_22, i5_22;
                                wr = ((W[6 * l1]).re);
                                wi = ((W[6 * l1]).im);
                                tmpr = ((jp[6 * m]).re);
                                tmpi = ((jp[6 * m]).im);
                                r5_6 = ((wr * tmpr) - (wi * tmpi));
                                i5_6 = ((wi * tmpr) + (wr * tmpi));
                                wr = ((W[22 * l1]).re);
                                wi = ((W[22 * l1]).im);
                                tmpr = ((jp[22 * m]).re);
                                tmpi = ((jp[22 * m]).im);
                                r5_22 = ((wr * tmpr) - (wi * tmpi));
                                i5_22 = ((wi * tmpr) + (wr * tmpi));
                                r4_6 = (r5_6 + r5_22);
                                i4_6 = (i5_6 + i5_22);
                                r4_22 = (r5_6 - r5_22);
                                i4_22 = (i5_6 - i5_22);
                            }
                            {
                                REAL r5_14, i5_14;
                                REAL r5_30, i5_30;
                                wr = ((W[14 * l1]).re);
                                wi = ((W[14 * l1]).im);
                                tmpr = ((jp[14 * m]).re);
                                tmpi = ((jp[14 * m]).im);
                                r5_14 = ((wr * tmpr) - (wi * tmpi));
                                i5_14 = ((wi * tmpr) + (wr * tmpi));
                                wr = ((W[30 * l1]).re);
                                wi = ((W[30 * l1]).im);
                                tmpr = ((jp[30 * m]).re);
                                tmpi = ((jp[30 * m]).im);
                                r5_30 = ((wr * tmpr) - (wi * tmpi));
                                i5_30 = ((wi * tmpr) + (wr * tmpi));
                                r4_14 = (r5_14 + r5_30);
                                i4_14 = (i5_14 + i5_30);
                                r4_30 = (r5_14 - r5_30);
                                i4_30 = (i5_14 - i5_30);
                            }
                            r3_6 = (r4_6 + r4_14);
                            i3_6 = (i4_6 + i4_14);
                            r3_22 = (r4_6 - r4_14);
                            i3_22 = (i4_6 - i4_14);
                            r3_14 = (r4_22 + i4_30);
                            i3_14 = (i4_22 - r4_30);
                            r3_30 = (r4_22 - i4_30);
                            i3_30 = (i4_22 + r4_30);
                        }
                        r2_2 = (r3_2 + r3_6);
                        i2_2 = (i3_2 + i3_6);
                        r2_18 = (r3_2 - r3_6);
                        i2_18 = (i3_2 - i3_6);
                        tmpr = (0.70710678118699999 * (r3_14 + i3_14));
                        tmpi = (0.70710678118699999 * (i3_14 - r3_14));
                        r2_6 = (r3_10 + tmpr);
                        i2_6 = (i3_10 + tmpi);
                        r2_22 = (r3_10 - tmpr);
                        i2_22 = (i3_10 - tmpi);
                        r2_10 = (r3_18 + i3_22);
                        i2_10 = (i3_18 - r3_22);
                        r2_26 = (r3_18 - i3_22);
                        i2_26 = (i3_18 + r3_22);
                        tmpr = (0.70710678118699999 * (i3_30 - r3_30));
                        tmpi = (0.70710678118699999 * (r3_30 + i3_30));
                        r2_14 = (r3_26 + tmpr);
                        i2_14 = (i3_26 - tmpi);
                        r2_30 = (r3_26 - tmpr);
                        i2_30 = (i3_26 + tmpi);
                    }
                    r1_0 = (r2_0 + r2_2);
                    i1_0 = (i2_0 + i2_2);
                    r1_16 = (r2_0 - r2_2);
                    i1_16 = (i2_0 - i2_2);
                    tmpr = ((0.92387953251099997 * r2_6) + (0.38268343236500002 * i2_6));
                    tmpi = ((0.92387953251099997 * i2_6) - (0.38268343236500002 * r2_6));
                    r1_2 = (r2_4 + tmpr);
                    i1_2 = (i2_4 + tmpi);
                    r1_18 = (r2_4 - tmpr);
                    i1_18 = (i2_4 - tmpi);
                    tmpr = (0.70710678118699999 * (r2_10 + i2_10));
                    tmpi = (0.70710678118699999 * (i2_10 - r2_10));
                    r1_4 = (r2_8 + tmpr);
                    i1_4 = (i2_8 + tmpi);
                    r1_20 = (r2_8 - tmpr);
                    i1_20 = (i2_8 - tmpi);
                    tmpr = ((0.38268343236500002 * r2_14) + (0.92387953251099997 * i2_14));
                    tmpi = ((0.38268343236500002 * i2_14) - (0.92387953251099997 * r2_14));
                    r1_6 = (r2_12 + tmpr);
                    i1_6 = (i2_12 + tmpi);
                    r1_22 = (r2_12 - tmpr);
                    i1_22 = (i2_12 - tmpi);
                    r1_8 = (r2_16 + i2_18);
                    i1_8 = (i2_16 - r2_18);
                    r1_24 = (r2_16 - i2_18);
                    i1_24 = (i2_16 + r2_18);
                    tmpr = ((0.92387953251099997 * i2_22) - (0.38268343236500002 * r2_22));
                    tmpi = ((0.92387953251099997 * r2_22) + (0.38268343236500002 * i2_22));
                    r1_10 = (r2_20 + tmpr);
                    i1_10 = (i2_20 - tmpi);
                    r1_26 = (r2_20 - tmpr);
                    i1_26 = (i2_20 + tmpi);
                    tmpr = (0.70710678118699999 * (i2_26 - r2_26));
                    tmpi = (0.70710678118699999 * (r2_26 + i2_26));
                    r1_12 = (r2_24 + tmpr);
                    i1_12 = (i2_24 - tmpi);
                    r1_28 = (r2_24 - tmpr);
                    i1_28 = (i2_24 + tmpi);
                    tmpr = ((0.38268343236500002 * i2_30) - (0.92387953251099997 * r2_30));
                    tmpi = ((0.38268343236500002 * r2_30) + (0.92387953251099997 * i2_30));
                    r1_14 = (r2_28 + tmpr);
                    i1_14 = (i2_28 - tmpi);
                    r1_30 = (r2_28 - tmpr);
                    i1_30 = (i2_28 + tmpi);
                }
                {
                    REAL r2_1, i2_1;
                    REAL r2_3, i2_3;
                    REAL r2_5, i2_5;
                    REAL r2_7, i2_7;
                    REAL r2_9, i2_9;
                    REAL r2_11, i2_11;
                    REAL r2_13, i2_13;
                    REAL r2_15, i2_15;
                    REAL r2_17, i2_17;
                    REAL r2_19, i2_19;
                    REAL r2_21, i2_21;
                    REAL r2_23, i2_23;
                    REAL r2_25, i2_25;
                    REAL r2_27, i2_27;
                    REAL r2_29, i2_29;
                    REAL r2_31, i2_31;
                    {
                        REAL r3_1, i3_1;
                        REAL r3_5, i3_5;
                        REAL r3_9, i3_9;
                        REAL r3_13, i3_13;
                        REAL r3_17, i3_17;
                        REAL r3_21, i3_21;
                        REAL r3_25, i3_25;
                        REAL r3_29, i3_29;
                        {
                            REAL r4_1, i4_1;
                            REAL r4_9, i4_9;
                            REAL r4_17, i4_17;
                            REAL r4_25, i4_25;
                            {
                                REAL r5_1, i5_1;
                                REAL r5_17, i5_17;
                                wr = ((W[1 * l1]).re);
                                wi = ((W[1 * l1]).im);
                                tmpr = ((jp[1 * m]).re);
                                tmpi = ((jp[1 * m]).im);
                                r5_1 = ((wr * tmpr) - (wi * tmpi));
                                i5_1 = ((wi * tmpr) + (wr * tmpi));
                                wr = ((W[17 * l1]).re);
                                wi = ((W[17 * l1]).im);
                                tmpr = ((jp[17 * m]).re);
                                tmpi = ((jp[17 * m]).im);
                                r5_17 = ((wr * tmpr) - (wi * tmpi));
                                i5_17 = ((wi * tmpr) + (wr * tmpi));
                                r4_1 = (r5_1 + r5_17);
                                i4_1 = (i5_1 + i5_17);
                                r4_17 = (r5_1 - r5_17);
                                i4_17 = (i5_1 - i5_17);
                            }
                            {
                                REAL r5_9, i5_9;
                                REAL r5_25, i5_25;
                                wr = ((W[9 * l1]).re);
                                wi = ((W[9 * l1]).im);
                                tmpr = ((jp[9 * m]).re);
                                tmpi = ((jp[9 * m]).im);
                                r5_9 = ((wr * tmpr) - (wi * tmpi));
                                i5_9 = ((wi * tmpr) + (wr * tmpi));
                                wr = ((W[25 * l1]).re);
                                wi = ((W[25 * l1]).im);
                                tmpr = ((jp[25 * m]).re);
                                tmpi = ((jp[25 * m]).im);
                                r5_25 = ((wr * tmpr) - (wi * tmpi));
                                i5_25 = ((wi * tmpr) + (wr * tmpi));
                                r4_9 = (r5_9 + r5_25);
                                i4_9 = (i5_9 + i5_25);
                                r4_25 = (r5_9 - r5_25);
                                i4_25 = (i5_9 - i5_25);
                            }
                            r3_1 = (r4_1 + r4_9);
                            i3_1 = (i4_1 + i4_9);
                            r3_17 = (r4_1 - r4_9);
                            i3_17 = (i4_1 - i4_9);
                            r3_9 = (r4_17 + i4_25);
                            i3_9 = (i4_17 - r4_25);
                            r3_25 = (r4_17 - i4_25);
                            i3_25 = (i4_17 + r4_25);
                        }
                        {
                            REAL r4_5, i4_5;
                            REAL r4_13, i4_13;
                            REAL r4_21, i4_21;
                            REAL r4_29, i4_29;
                            {
                                REAL r5_5, i5_5;
                                REAL r5_21, i5_21;
                                wr = ((W[5 * l1]).re);
                                wi = ((W[5 * l1]).im);
                                tmpr = ((jp[5 * m]).re);
                                tmpi = ((jp[5 * m]).im);
                                r5_5 = ((wr * tmpr) - (wi * tmpi));
                                i5_5 = ((wi * tmpr) + (wr * tmpi));
                                wr = ((W[21 * l1]).re);
                                wi = ((W[21 * l1]).im);
                                tmpr = ((jp[21 * m]).re);
                                tmpi = ((jp[21 * m]).im);
                                r5_21 = ((wr * tmpr) - (wi * tmpi));
                                i5_21 = ((wi * tmpr) + (wr * tmpi));
                                r4_5 = (r5_5 + r5_21);
                                i4_5 = (i5_5 + i5_21);
                                r4_21 = (r5_5 - r5_21);
                                i4_21 = (i5_5 - i5_21);
                            }
                            {
                                REAL r5_13, i5_13;
                                REAL r5_29, i5_29;
                                wr = ((W[13 * l1]).re);
                                wi = ((W[13 * l1]).im);
                                tmpr = ((jp[13 * m]).re);
                                tmpi = ((jp[13 * m]).im);
                                r5_13 = ((wr * tmpr) - (wi * tmpi));
                                i5_13 = ((wi * tmpr) + (wr * tmpi));
                                wr = ((W[29 * l1]).re);
                                wi = ((W[29 * l1]).im);
                                tmpr = ((jp[29 * m]).re);
                                tmpi = ((jp[29 * m]).im);
                                r5_29 = ((wr * tmpr) - (wi * tmpi));
                                i5_29 = ((wi * tmpr) + (wr * tmpi));
                                r4_13 = (r5_13 + r5_29);
                                i4_13 = (i5_13 + i5_29);
                                r4_29 = (r5_13 - r5_29);
                                i4_29 = (i5_13 - i5_29);
                            }
                            r3_5 = (r4_5 + r4_13);
                            i3_5 = (i4_5 + i4_13);
                            r3_21 = (r4_5 - r4_13);
                            i3_21 = (i4_5 - i4_13);
                            r3_13 = (r4_21 + i4_29);
                            i3_13 = (i4_21 - r4_29);
                            r3_29 = (r4_21 - i4_29);
                            i3_29 = (i4_21 + r4_29);
                        }
                        r2_1 = (r3_1 + r3_5);
                        i2_1 = (i3_1 + i3_5);
                        r2_17 = (r3_1 - r3_5);
                        i2_17 = (i3_1 - i3_5);
                        tmpr = (0.70710678118699999 * (r3_13 + i3_13));
                        tmpi = (0.70710678118699999 * (i3_13 - r3_13));
                        r2_5 = (r3_9 + tmpr);
                        i2_5 = (i3_9 + tmpi);
                        r2_21 = (r3_9 - tmpr);
                        i2_21 = (i3_9 - tmpi);
                        r2_9 = (r3_17 + i3_21);
                        i2_9 = (i3_17 - r3_21);
                        r2_25 = (r3_17 - i3_21);
                        i2_25 = (i3_17 + r3_21);
                        tmpr = (0.70710678118699999 * (i3_29 - r3_29));
                        tmpi = (0.70710678118699999 * (r3_29 + i3_29));
                        r2_13 = (r3_25 + tmpr);
                        i2_13 = (i3_25 - tmpi);
                        r2_29 = (r3_25 - tmpr);
                        i2_29 = (i3_25 + tmpi);
                    }
                    {
                        REAL r3_3, i3_3;
                        REAL r3_7, i3_7;
                        REAL r3_11, i3_11;
                        REAL r3_15, i3_15;
                        REAL r3_19, i3_19;
                        REAL r3_23, i3_23;
                        REAL r3_27, i3_27;
                        REAL r3_31, i3_31;
                        {
                            REAL r4_3, i4_3;
                            REAL r4_11, i4_11;
                            REAL r4_19, i4_19;
                            REAL r4_27, i4_27;
                            {
                                REAL r5_3, i5_3;
                                REAL r5_19, i5_19;
                                wr = ((W[3 * l1]).re);
                                wi = ((W[3 * l1]).im);
                                tmpr = ((jp[3 * m]).re);
                                tmpi = ((jp[3 * m]).im);
                                r5_3 = ((wr * tmpr) - (wi * tmpi));
                                i5_3 = ((wi * tmpr) + (wr * tmpi));
                                wr = ((W[19 * l1]).re);
                                wi = ((W[19 * l1]).im);
                                tmpr = ((jp[19 * m]).re);
                                tmpi = ((jp[19 * m]).im);
                                r5_19 = ((wr * tmpr) - (wi * tmpi));
                                i5_19 = ((wi * tmpr) + (wr * tmpi));
                                r4_3 = (r5_3 + r5_19);
                                i4_3 = (i5_3 + i5_19);
                                r4_19 = (r5_3 - r5_19);
                                i4_19 = (i5_3 - i5_19);
                            }
                            {
                                REAL r5_11, i5_11;
                                REAL r5_27, i5_27;
                                wr = ((W[11 * l1]).re);
                                wi = ((W[11 * l1]).im);
                                tmpr = ((jp[11 * m]).re);
                                tmpi = ((jp[11 * m]).im);
                                r5_11 = ((wr * tmpr) - (wi * tmpi));
                                i5_11 = ((wi * tmpr) + (wr * tmpi));
                                wr = ((W[27 * l1]).re);
                                wi = ((W[27 * l1]).im);
                                tmpr = ((jp[27 * m]).re);
                                tmpi = ((jp[27 * m]).im);
                                r5_27 = ((wr * tmpr) - (wi * tmpi));
                                i5_27 = ((wi * tmpr) + (wr * tmpi));
                                r4_11 = (r5_11 + r5_27);
                                i4_11 = (i5_11 + i5_27);
                                r4_27 = (r5_11 - r5_27);
                                i4_27 = (i5_11 - i5_27);
                            }
                            r3_3 = (r4_3 + r4_11);
                            i3_3 = (i4_3 + i4_11);
                            r3_19 = (r4_3 - r4_11);
                            i3_19 = (i4_3 - i4_11);
                            r3_11 = (r4_19 + i4_27);
                            i3_11 = (i4_19 - r4_27);
                            r3_27 = (r4_19 - i4_27);
                            i3_27 = (i4_19 + r4_27);
                        }
                        {
                            REAL r4_7, i4_7;
                            REAL r4_15, i4_15;
                            REAL r4_23, i4_23;
                            REAL r4_31, i4_31;
                            {
                                REAL r5_7, i5_7;
                                REAL r5_23, i5_23;
                                wr = ((W[7 * l1]).re);
                                wi = ((W[7 * l1]).im);
                                tmpr = ((jp[7 * m]).re);
                                tmpi = ((jp[7 * m]).im);
                                r5_7 = ((wr * tmpr) - (wi * tmpi));
                                i5_7 = ((wi * tmpr) + (wr * tmpi));
                                wr = ((W[23 * l1]).re);
                                wi = ((W[23 * l1]).im);
                                tmpr = ((jp[23 * m]).re);
                                tmpi = ((jp[23 * m]).im);
                                r5_23 = ((wr * tmpr) - (wi * tmpi));
                                i5_23 = ((wi * tmpr) + (wr * tmpi));
                                r4_7 = (r5_7 + r5_23);
                                i4_7 = (i5_7 + i5_23);
                                r4_23 = (r5_7 - r5_23);
                                i4_23 = (i5_7 - i5_23);
                            }
                            {
                                REAL r5_15, i5_15;
                                REAL r5_31, i5_31;
                                wr = ((W[15 * l1]).re);
                                wi = ((W[15 * l1]).im);
                                tmpr = ((jp[15 * m]).re);
                                tmpi = ((jp[15 * m]).im);
                                r5_15 = ((wr * tmpr) - (wi * tmpi));
                                i5_15 = ((wi * tmpr) + (wr * tmpi));
                                wr = ((W[31 * l1]).re);
                                wi = ((W[31 * l1]).im);
                                tmpr = ((jp[31 * m]).re);
                                tmpi = ((jp[31 * m]).im);
                                r5_31 = ((wr * tmpr) - (wi * tmpi));
                                i5_31 = ((wi * tmpr) + (wr * tmpi));
                                r4_15 = (r5_15 + r5_31);
                                i4_15 = (i5_15 + i5_31);
                                r4_31 = (r5_15 - r5_31);
                                i4_31 = (i5_15 - i5_31);
                            }
                            r3_7 = (r4_7 + r4_15);
                            i3_7 = (i4_7 + i4_15);
                            r3_23 = (r4_7 - r4_15);
                            i3_23 = (i4_7 - i4_15);
                            r3_15 = (r4_23 + i4_31);
                            i3_15 = (i4_23 - r4_31);
                            r3_31 = (r4_23 - i4_31);
                            i3_31 = (i4_23 + r4_31);
                        }
                        r2_3 = (r3_3 + r3_7);
                        i2_3 = (i3_3 + i3_7);
                        r2_19 = (r3_3 - r3_7);
                        i2_19 = (i3_3 - i3_7);
                        tmpr = (0.70710678118699999 * (r3_15 + i3_15));
                        tmpi = (0.70710678118699999 * (i3_15 - r3_15));
                        r2_7 = (r3_11 + tmpr);
                        i2_7 = (i3_11 + tmpi);
                        r2_23 = (r3_11 - tmpr);
                        i2_23 = (i3_11 - tmpi);
                        r2_11 = (r3_19 + i3_23);
                        i2_11 = (i3_19 - r3_23);
                        r2_27 = (r3_19 - i3_23);
                        i2_27 = (i3_19 + r3_23);
                        tmpr = (0.70710678118699999 * (i3_31 - r3_31));
                        tmpi = (0.70710678118699999 * (r3_31 + i3_31));
                        r2_15 = (r3_27 + tmpr);
                        i2_15 = (i3_27 - tmpi);
                        r2_31 = (r3_27 - tmpr);
                        i2_31 = (i3_27 + tmpi);
                    }
                    r1_1 = (r2_1 + r2_3);
                    i1_1 = (i2_1 + i2_3);
                    r1_17 = (r2_1 - r2_3);
                    i1_17 = (i2_1 - i2_3);
                    tmpr = ((0.92387953251099997 * r2_7) + (0.38268343236500002 * i2_7));
                    tmpi = ((0.92387953251099997 * i2_7) - (0.38268343236500002 * r2_7));
                    r1_3 = (r2_5 + tmpr);
                    i1_3 = (i2_5 + tmpi);
                    r1_19 = (r2_5 - tmpr);
                    i1_19 = (i2_5 - tmpi);
                    tmpr = (0.70710678118699999 * (r2_11 + i2_11));
                    tmpi = (0.70710678118699999 * (i2_11 - r2_11));
                    r1_5 = (r2_9 + tmpr);
                    i1_5 = (i2_9 + tmpi);
                    r1_21 = (r2_9 - tmpr);
                    i1_21 = (i2_9 - tmpi);
                    tmpr = ((0.38268343236500002 * r2_15) + (0.92387953251099997 * i2_15));
                    tmpi = ((0.38268343236500002 * i2_15) - (0.92387953251099997 * r2_15));
                    r1_7 = (r2_13 + tmpr);
                    i1_7 = (i2_13 + tmpi);
                    r1_23 = (r2_13 - tmpr);
                    i1_23 = (i2_13 - tmpi);
                    r1_9 = (r2_17 + i2_19);
                    i1_9 = (i2_17 - r2_19);
                    r1_25 = (r2_17 - i2_19);
                    i1_25 = (i2_17 + r2_19);
                    tmpr = ((0.92387953251099997 * i2_23) - (0.38268343236500002 * r2_23));
                    tmpi = ((0.92387953251099997 * r2_23) + (0.38268343236500002 * i2_23));
                    r1_11 = (r2_21 + tmpr);
                    i1_11 = (i2_21 - tmpi);
                    r1_27 = (r2_21 - tmpr);
                    i1_27 = (i2_21 + tmpi);
                    tmpr = (0.70710678118699999 * (i2_27 - r2_27));
                    tmpi = (0.70710678118699999 * (r2_27 + i2_27));
                    r1_13 = (r2_25 + tmpr);
                    i1_13 = (i2_25 - tmpi);
                    r1_29 = (r2_25 - tmpr);
                    i1_29 = (i2_25 + tmpi);
                    tmpr = ((0.38268343236500002 * i2_31) - (0.92387953251099997 * r2_31));
                    tmpi = ((0.38268343236500002 * r2_31) + (0.92387953251099997 * i2_31));
                    r1_15 = (r2_29 + tmpr);
                    i1_15 = (i2_29 - tmpi);
                    r1_31 = (r2_29 - tmpr);
                    i1_31 = (i2_29 + tmpi);
                }
                ((kp[0 * m]).re) = (r1_0 + r1_1);
                ((kp[0 * m]).im) = (i1_0 + i1_1);
                ((kp[16 * m]).re) = (r1_0 - r1_1);
                ((kp[16 * m]).im) = (i1_0 - i1_1);
                tmpr = ((0.98078528040299994 * r1_3) + (0.19509032201599999 * i1_3));
                tmpi = ((0.98078528040299994 * i1_3) - (0.19509032201599999 * r1_3));
                ((kp[1 * m]).re) = (r1_2 + tmpr);
                ((kp[1 * m]).im) = (i1_2 + tmpi);
                ((kp[17 * m]).re) = (r1_2 - tmpr);
                ((kp[17 * m]).im) = (i1_2 - tmpi);
                tmpr = ((0.92387953251099997 * r1_5) + (0.38268343236500002 * i1_5));
                tmpi = ((0.92387953251099997 * i1_5) - (0.38268343236500002 * r1_5));
                ((kp[2 * m]).re) = (r1_4 + tmpr);
                ((kp[2 * m]).im) = (i1_4 + tmpi);
                ((kp[18 * m]).re) = (r1_4 - tmpr);
                ((kp[18 * m]).im) = (i1_4 - tmpi);
                tmpr = ((0.83146961230299998 * r1_7) + (0.55557023301999997 * i1_7));
                tmpi = ((0.83146961230299998 * i1_7) - (0.55557023301999997 * r1_7));
                ((kp[3 * m]).re) = (r1_6 + tmpr);
                ((kp[3 * m]).im) = (i1_6 + tmpi);
                ((kp[19 * m]).re) = (r1_6 - tmpr);
                ((kp[19 * m]).im) = (i1_6 - tmpi);
                tmpr = (0.70710678118699999 * (r1_9 + i1_9));
                tmpi = (0.70710678118699999 * (i1_9 - r1_9));
                ((kp[4 * m]).re) = (r1_8 + tmpr);
                ((kp[4 * m]).im) = (i1_8 + tmpi);
                ((kp[20 * m]).re) = (r1_8 - tmpr);
                ((kp[20 * m]).im) = (i1_8 - tmpi);
                tmpr = ((0.55557023301999997 * r1_11) + (0.83146961230299998 * i1_11));
                tmpi = ((0.55557023301999997 * i1_11) - (0.83146961230299998 * r1_11));
                ((kp[5 * m]).re) = (r1_10 + tmpr);
                ((kp[5 * m]).im) = (i1_10 + tmpi);
                ((kp[21 * m]).re) = (r1_10 - tmpr);
                ((kp[21 * m]).im) = (i1_10 - tmpi);
                tmpr = ((0.38268343236500002 * r1_13) + (0.92387953251099997 * i1_13));
                tmpi = ((0.38268343236500002 * i1_13) - (0.92387953251099997 * r1_13));
                ((kp[6 * m]).re) = (r1_12 + tmpr);
                ((kp[6 * m]).im) = (i1_12 + tmpi);
                ((kp[22 * m]).re) = (r1_12 - tmpr);
                ((kp[22 * m]).im) = (i1_12 - tmpi);
                tmpr = ((0.19509032201599999 * r1_15) + (0.98078528040299994 * i1_15));
                tmpi = ((0.19509032201599999 * i1_15) - (0.98078528040299994 * r1_15));
                ((kp[7 * m]).re) = (r1_14 + tmpr);
                ((kp[7 * m]).im) = (i1_14 + tmpi);
                ((kp[23 * m]).re) = (r1_14 - tmpr);
                ((kp[23 * m]).im) = (i1_14 - tmpi);
                ((kp[8 * m]).re) = (r1_16 + i1_17);
                ((kp[8 * m]).im) = (i1_16 - r1_17);
                ((kp[24 * m]).re) = (r1_16 - i1_17);
                ((kp[24 * m]).im) = (i1_16 + r1_17);
                tmpr = ((0.98078528040299994 * i1_19) - (0.19509032201599999 * r1_19));
                tmpi = ((0.98078528040299994 * r1_19) + (0.19509032201599999 * i1_19));
                ((kp[9 * m]).re) = (r1_18 + tmpr);
                ((kp[9 * m]).im) = (i1_18 - tmpi);
                ((kp[25 * m]).re) = (r1_18 - tmpr);
                ((kp[25 * m]).im) = (i1_18 + tmpi);
                tmpr = ((0.92387953251099997 * i1_21) - (0.38268343236500002 * r1_21));
                tmpi = ((0.92387953251099997 * r1_21) + (0.38268343236500002 * i1_21));
                ((kp[10 * m]).re) = (r1_20 + tmpr);
                ((kp[10 * m]).im) = (i1_20 - tmpi);
                ((kp[26 * m]).re) = (r1_20 - tmpr);
                ((kp[26 * m]).im) = (i1_20 + tmpi);
                tmpr = ((0.83146961230299998 * i1_23) - (0.55557023301999997 * r1_23));
                tmpi = ((0.83146961230299998 * r1_23) + (0.55557023301999997 * i1_23));
                ((kp[11 * m]).re) = (r1_22 + tmpr);
                ((kp[11 * m]).im) = (i1_22 - tmpi);
                ((kp[27 * m]).re) = (r1_22 - tmpr);
                ((kp[27 * m]).im) = (i1_22 + tmpi);
                tmpr = (0.70710678118699999 * (i1_25 - r1_25));
                tmpi = (0.70710678118699999 * (r1_25 + i1_25));
                ((kp[12 * m]).re) = (r1_24 + tmpr);
                ((kp[12 * m]).im) = (i1_24 - tmpi);
                ((kp[28 * m]).re) = (r1_24 - tmpr);
                ((kp[28 * m]).im) = (i1_24 + tmpi);
                tmpr = ((0.55557023301999997 * i1_27) - (0.83146961230299998 * r1_27));
                tmpi = ((0.55557023301999997 * r1_27) + (0.83146961230299998 * i1_27));
                ((kp[13 * m]).re) = (r1_26 + tmpr);
                ((kp[13 * m]).im) = (i1_26 - tmpi);
                ((kp[29 * m]).re) = (r1_26 - tmpr);
                ((kp[29 * m]).im) = (i1_26 + tmpi);
                tmpr = ((0.38268343236500002 * i1_29) - (0.92387953251099997 * r1_29));
                tmpi = ((0.38268343236500002 * r1_29) + (0.92387953251099997 * i1_29));
                ((kp[14 * m]).re) = (r1_28 + tmpr);
                ((kp[14 * m]).im) = (i1_28 - tmpi);
                ((kp[30 * m]).re) = (r1_28 - tmpr);
                ((kp[30 * m]).im) = (i1_28 + tmpi);
                tmpr = ((0.19509032201599999 * i1_31) - (0.98078528040299994 * r1_31));
                tmpi = ((0.19509032201599999 * r1_31) + (0.98078528040299994 * i1_31));
                ((kp[15 * m]).re) = (r1_30 + tmpr);
                ((kp[15 * m]).im) = (i1_30 - tmpi);
                ((kp[31 * m]).re) = (r1_30 - tmpr);
                ((kp[31 * m]).im) = (i1_30 + tmpi);
            }
        }
    } else {
        int ab = (a + b) / 2;
        fft_twiddle_32_seq(a, ab, in, out, W, nW, nWdn, m);
        fft_twiddle_32_seq(ab, b, in, out, W, nW, nWdn, m);
    }
}
/*Function: fft_unshuffle_32_seq, ID: 47*/
void fft_unshuffle_32_seq(int a, int b, COMPLEX* in, COMPLEX* out, int m)
{
    /*fft_unshuffle_32_seq:47*/
    /*CompoundStmt:10980*/
    int i;
    const COMPLEX* ip;
    COMPLEX* jp;
    if ((b - a) < 128) {
        ip = in + a * 32;
        for (i = a; i < b; ++i) {
            jp = out + i;
            jp[0] = ip[0];
            jp[m] = ip[1];
            ip += 2;
            jp += 2 * m;
            jp[0] = ip[0];
            jp[m] = ip[1];
            ip += 2;
            jp += 2 * m;
            jp[0] = ip[0];
            jp[m] = ip[1];
            ip += 2;
            jp += 2 * m;
            jp[0] = ip[0];
            jp[m] = ip[1];
            ip += 2;
            jp += 2 * m;
            jp[0] = ip[0];
            jp[m] = ip[1];
            ip += 2;
            jp += 2 * m;
            jp[0] = ip[0];
            jp[m] = ip[1];
            ip += 2;
            jp += 2 * m;
            jp[0] = ip[0];
            jp[m] = ip[1];
            ip += 2;
            jp += 2 * m;
            jp[0] = ip[0];
            jp[m] = ip[1];
            ip += 2;
            jp += 2 * m;
            jp[0] = ip[0];
            jp[m] = ip[1];
            ip += 2;
            jp += 2 * m;
            jp[0] = ip[0];
            jp[m] = ip[1];
            ip += 2;
            jp += 2 * m;
            jp[0] = ip[0];
            jp[m] = ip[1];
            ip += 2;
            jp += 2 * m;
            jp[0] = ip[0];
            jp[m] = ip[1];
            ip += 2;
            jp += 2 * m;
            jp[0] = ip[0];
            jp[m] = ip[1];
            ip += 2;
            jp += 2 * m;
            jp[0] = ip[0];
            jp[m] = ip[1];
            ip += 2;
            jp += 2 * m;
            jp[0] = ip[0];
            jp[m] = ip[1];
            ip += 2;
            jp += 2 * m;
            jp[0] = ip[0];
            jp[m] = ip[1];
            ip += 2;
        }
    } else {
        int ab = (a + b) / 2;
        fft_unshuffle_32_seq(a, ab, in, out, m);
        fft_unshuffle_32_seq(ab, b, in, out, m);
    }
}
/*Function: fft_aux_seq, ID: 49*/
void fft_aux_seq(int n, COMPLEX* in, COMPLEX* out, int* factors, COMPLEX* W, int nW)
{
    /*fft_aux_seq:49*/
    /*CompoundStmt:11263*/
    int r, m;
    int k;
    if (n == 32) {
        fft_base_32(in, out);
        return;
    }
    if (n == 16) {
        fft_base_16(in, out);
        return;
    }
    if (n == 8) {
        fft_base_8(in, out);
        return;
    }
    if (n == 4) {
        fft_base_4(in, out);
        return;
    }
    if (n == 2) {
        fft_base_2(in, out);
        return;
    }
    r = *factors;
    m = n / r;
    if (r < n) {
        if (r == 32)
            fft_unshuffle_32_seq(0, m, in, out, m);
        else if (r == 16)
            fft_unshuffle_16_seq(0, m, in, out, m);
        else if (r == 8)
            fft_unshuffle_8_seq(0, m, in, out, m);
        else if (r == 4)
            fft_unshuffle_4_seq(0, m, in, out, m);
        else if (r == 2)
            fft_unshuffle_2_seq(0, m, in, out, m);
        else
            unshuffle_seq(0, m, in, out, r, m);
        for (k = 0; k < n; k += m) {
            fft_aux_seq(m, out + k, in + k, factors + 1, W, nW);
        }
    }
    if (r == 2)
        fft_twiddle_2_seq(0, m, in, out, W, nW, nW / n, m);
    else if (r == 4)
        fft_twiddle_4_seq(0, m, in, out, W, nW, nW / n, m);
    else if (r == 8)
        fft_twiddle_8_seq(0, m, in, out, W, nW, nW / n, m);
    else if (r == 16)
        fft_twiddle_16_seq(0, m, in, out, W, nW, nW / n, m);
    else if (r == 32)
        fft_twiddle_32_seq(0, m, in, out, W, nW, nW / n, m);
    else
        fft_twiddle_gen_seq(0, m, in, out, W, nW, nW / n, r, m);
    return;
}
/*Function: fft, ID: 50*/
void fft(int n, COMPLEX* in, COMPLEX* out)
{
    /*fft:50*/
    /*CompoundStmt:11366*/
    int factors[40];
    int* p = factors;
    int l = n;
    int r;
    COMPLEX* W;
    {
        /*CompoundStmt:11372*/
        if (bots_verbose_mode >= BOTS_VERBOSE_DEFAULT) {
            fprintf(stdout, "Computing coefficients ");
        }
    }
    W = (COMPLEX*)malloc((n + 1) * sizeof(COMPLEX));
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP11383>(ompNumThreads * DARTS_CODELETS_MULT, 0,
            RuntimeFinalCodelet, (COMPLEX**)&((W)), (int*)&((n))));
    }
    {
        /*CompoundStmt:11388*/
        if (bots_verbose_mode >= BOTS_VERBOSE_DEFAULT) {
            fprintf(stdout, " completed!\n");
        }
    }
    do {
        r = factor(l);
        *p++ = r;
        l /= r;
    } while (l > 1);
    {
        /*CompoundStmt:11402*/
        if (bots_verbose_mode >= BOTS_VERBOSE_DEFAULT) {
            fprintf(stdout, "Computing FFT ");
        }
    }
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP11407>(ompNumThreads * DARTS_CODELETS_MULT, 0,
            RuntimeFinalCodelet, (COMPLEX**)&((W)), (int*)((factors)), (40), (COMPLEX**)&((in)),
            (int*)&((n)), (COMPLEX**)&((out))));
    }
    {
        /*CompoundStmt:11411*/
        if (bots_verbose_mode >= BOTS_VERBOSE_DEFAULT) {
            fprintf(stdout, " completed!\n");
        }
    }
    free(W);
    return;
}
/*Function: fft_seq, ID: 51*/
void fft_seq(int n, COMPLEX* in, COMPLEX* out)
{
    /*fft_seq:51*/
    /*CompoundStmt:11418*/
    int factors[40];
    int* p = factors;
    int l = n;
    int r;
    COMPLEX* W;
    W = (COMPLEX*)malloc((n + 1) * sizeof(COMPLEX));
    compute_w_coefficients_seq(n, 0, n / 2, W);
    do {
        r = factor(l);
        *p++ = r;
        l /= r;
    } while (l > 1);
    fft_aux_seq(n, in, out, factors, W, n);
    free(W);
    return;
}
/*Function: test_correctness, ID: 52*/
int test_correctness(int n, COMPLEX* out1, COMPLEX* out2)
{
    /*test_correctness:52*/
    /*CompoundStmt:11444*/
    int i;
    double a, d, error = 0.;
    for (i = 0; i < n; ++i) {
        a = sqrt((((out1[i]).re) - ((out2[i]).re)) * (((out1[i]).re) - ((out2[i]).re))
            + (((out1[i]).im) - ((out2[i]).im)) * (((out1[i]).im) - ((out2[i]).im)));
        d = sqrt(((out2[i]).re) * ((out2[i]).re) + ((out2[i]).im) * ((out2[i]).im));
        if (d < -1.0E-10 || d > 1.0E-10)
            a /= d;
        if (a > error)
            error = a;
    }
    {
        /*CompoundStmt:11499*/
        if (bots_verbose_mode >= BOTS_VERBOSE_DEFAULT) {
            fprintf(stdout, "relative error=%e\n", error);
        }
    }
    if (error > 0.001)
        return 2;
    else
        return 1;
}
/*TP15: TP_compute_w_coefficients*/
void TP15::_checkInCodelets111::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    /*Init the vars for this region*/
    /*Printing conditional branch node 111: inlining: 0*/
    if ((this->inputsTPParent->b_darts15[this->getID()])
            - (this->inputsTPParent->a_darts15[this->getID()])
        < 128) {
        myTP->checkInCodelets114[this->getID()].decDep();
    } else {
        /*elseRegion: DeclStmt 146*/
        myTP->checkInCodelets146[this->getID()].decDep();
    }
}
void TP15::_checkInCodelets114::fire(void)
{

    /*printing node 114: BinaryOperator*/
    (this->inputsTPParent->twoPiOverN_darts15[this->getID()])
        = 2. * 3.1415926535897931 / (this->inputsTPParent->n_darts15[this->getID()]);

    /*printing node 119: ForStmt*/
    {
        COMPLEX** W = &(this->inputsTPParent->W_darts15[this->getLocalID()]);
        (void)W /*PRIVATE*/;
        int* b = &(this->inputsTPParent->b_darts15[this->getLocalID()]);
        (void)b /*PRIVATE*/;
        REAL* c = &(this->inputsTPParent->c_darts15[this->getLocalID()]);
        (void)c /*PRIVATE*/;
        int* k = &(this->inputsTPParent->k_darts15[this->getLocalID()]);
        (void)k /*PRIVATE*/;
        int* n = &(this->inputsTPParent->n_darts15[this->getLocalID()]);
        (void)n /*PRIVATE*/;
        REAL* s = &(this->inputsTPParent->s_darts15[this->getLocalID()]);
        (void)s /*PRIVATE*/;
        double* twoPiOverN = &(this->inputsTPParent->twoPiOverN_darts15[this->getLocalID()]);
        (void)twoPiOverN /*PRIVATE*/;
        /*Loop's init*/
        (this->inputsTPParent->k_darts15[this->getID()])
            = (this->inputsTPParent->a_darts15[this->getID()]);
        int k_darts_counter_temp15 = (this->inputsTPParent->k_darts15[this->getID()]);
        for (; (k_darts_counter_temp15) <= (*b); ++(k_darts_counter_temp15)) {
            (*c) = cos((*twoPiOverN) * (k_darts_counter_temp15));
            (((*W)[(k_darts_counter_temp15)]).re) = (((*W)[(*n) - (k_darts_counter_temp15)]).re)
                = (*c);
            (*s) = sin((*twoPiOverN) * (k_darts_counter_temp15));
            (((*W)[(k_darts_counter_temp15)]).im) = -(*s);
            (((*W)[(*n) - (k_darts_counter_temp15)]).im) = (*s);
        }
        (this->inputsTPParent->k_darts15[this->getID()]) = k_darts_counter_temp15;
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/

    myTP->controlTPParent->nextCodeletscompute_w_coefficients[this->getID()]->decDep();
}
void TP15::_checkInCodelets146::fire(void)
{

    /*printing node 146: DeclStmt*/
    this->inputsTPParent->ab_darts15[this->getID()]
        = ((this->inputsTPParent->a_darts15[this->getID()])
              + (this->inputsTPParent->b_darts15[this->getID()]))
        / 2;
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 146 nextRegion: 149 */
    myTP->controlTPParent->checkInCodelets149[this->getID()].decDep();
}
void TP15::_checkInCodelets149::fire(void)
{

    /*printing node 149: OMPTaskDirective*/
    /*syncNode: OMPTaskwaitDirective 154*/
    /*Find the TP to which the nextRegion belongs*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->barrierCodelets154[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 149*/
    _task149Inputs* task149Inputs
        = new _task149Inputs(&((this->inputsTPParent->W_darts15[this->getID()])),
            &((this->inputsTPParent->a_darts15[this->getID()])),
            &((this->inputsTPParent->ab_darts15[this->getID()])),
            &((this->inputsTPParent->n_darts15[this->getID()])), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task149Inputs[this->getID()] = task149Inputs;
    invoke<TP149>(myTP, 1, this->getID(), myTP, task149Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 149 nextRegion: 151 */
    myTP->controlTPParent->checkInCodelets151[this->getID()].decDep();
}
void TP15::_checkInCodelets151::fire(void)
{

    /*printing node 151: OMPTaskDirective*/
    /*syncNode: OMPTaskwaitDirective 154*/
    /*Find the TP to which the nextRegion belongs*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->barrierCodelets154[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 151*/
    _task151Inputs* task151Inputs
        = new _task151Inputs(&((this->inputsTPParent->W_darts15[this->getID()])),
            &((this->inputsTPParent->ab_darts15[this->getID()])),
            &((this->inputsTPParent->b_darts15[this->getID()])),
            &((this->inputsTPParent->n_darts15[this->getID()])), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task151Inputs[this->getID()] = task151Inputs;
    invoke<TP151>(myTP, 1, this->getID(), myTP, task151Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 151 nextRegion: 154 */
    myTP->controlTPParent->barrierCodelets154[0].decDep();
}
void TP15::_barrierCodelets154::fire(void)
{
    TP15* myTP = static_cast<TP15*>(myTP_);

    for (size_t codeletsCounter = 0; codeletsCounter < (size_t)myTP->numThreads;
         codeletsCounter++) {
        myTP->nextCodeletscompute_w_coefficients[codeletsCounter]->decDep();
    }
}
TP15::TP15(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_mainNextCodelet,
    darts::Codelet* in_mainSyncCodelet, TP15** in_ptrToThisFunctionTP, int in_n, int in_a, int in_b,
    COMPLEX* in_W)
    : ompTP(in_numThreads, in_mainCodeletID)
    , ptrToThisFunctionTP(in_ptrToThisFunctionTP)
    , inputsTPParent(this)
    , controlTPParent(this)
    , nextCodeletscompute_w_coefficients(new Codelet*[in_numThreads])
    , nextSyncCodeletscompute_w_coefficients(new Codelet*[in_numThreads])
    , n_darts15(new int[this->numThreads])
    , a_darts15(new int[this->numThreads])
    , b_darts15(new int[this->numThreads])
    , W_darts15(new COMPLEX*[this->numThreads])
    , ab_darts15(new int[this->numThreads])
    , c_darts15(new REAL[this->numThreads])
    , k_darts15(new int[this->numThreads])
    , s_darts15(new REAL[this->numThreads])
    , twoPiOverN_darts15(new double[this->numThreads])
    , task149Inputs(new _task149Inputs*[this->numThreads])
    , task151Inputs(new _task151Inputs*[this->numThreads])
    , checkInCodelets111(new _checkInCodelets111[this->numThreads])
    , checkInCodelets114(new _checkInCodelets114[this->numThreads])
    , checkInCodelets146(new _checkInCodelets146[this->numThreads])
    , checkInCodelets149(new _checkInCodelets149[this->numThreads])
    , checkInCodelets151(new _checkInCodelets151[this->numThreads])
    , barrierCodelets154(new _barrierCodelets154[1])
{
    barrierCodelets154[0] = _barrierCodelets154(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets151* checkInCodelets151Ptr = (this->checkInCodelets151);
    _checkInCodelets149* checkInCodelets149Ptr = (this->checkInCodelets149);
    _checkInCodelets146* checkInCodelets146Ptr = (this->checkInCodelets146);
    _checkInCodelets114* checkInCodelets114Ptr = (this->checkInCodelets114);
    _checkInCodelets111* checkInCodelets111Ptr = (this->checkInCodelets111);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets111);
#endif
    for (size_t codeletCounter = 0; codeletCounter < this->numThreads; codeletCounter++) {
        (*checkInCodelets151Ptr) = _checkInCodelets151(1, 1, this, codeletCounter);
        checkInCodelets151Ptr++;
        (*checkInCodelets149Ptr) = _checkInCodelets149(1, 1, this, codeletCounter);
        checkInCodelets149Ptr++;
        (*checkInCodelets146Ptr) = _checkInCodelets146(1, 1, this, codeletCounter);
        checkInCodelets146Ptr++;
        (*checkInCodelets114Ptr) = _checkInCodelets114(1, 1, this, codeletCounter);
        checkInCodelets114Ptr++;
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets111Ptr) = _checkInCodelets111(2, 1, this, codeletCounter);
#else
        (*checkInCodelets111Ptr) = _checkInCodelets111(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets111Ptr).decDep();
        checkInCodelets111Ptr++;
    }
    if (this->numThreads == 1) {
        this->nextCodeletscompute_w_coefficients[0] = in_mainNextCodelet;
        this->nextSyncCodeletscompute_w_coefficients[0] = in_mainSyncCodelet;
        this->n_darts15[0] = in_n;
        this->a_darts15[0] = in_a;
        this->b_darts15[0] = in_b;
        this->W_darts15[0] = in_W;
        this->availableCodelets[0] = 1;
    } else {
        this->n_darts15[this->mainCodeletID] = in_n;
        this->a_darts15[this->mainCodeletID] = in_a;
        this->b_darts15[this->mainCodeletID] = in_b;
        this->W_darts15[this->mainCodeletID] = in_W;
        this->nextCodeletscompute_w_coefficients[in_mainCodeletID] = in_mainNextCodelet;
        this->nextSyncCodeletscompute_w_coefficients[in_mainCodeletID] = in_mainSyncCodelet;
#if USE_SPIN_CODELETS == 0
        this->firstCodelet[this->mainCodeletID].decDep();
#else
        this->availableCodelets[this->mainCodeletID] = 1;
#endif
        *(this->ptrToThisFunctionTP) = this;
    }
}
TP15::~TP15()
{
    delete[] task149Inputs;
    delete[] task151Inputs;
    delete[] barrierCodelets154;
    delete[] checkInCodelets151;
    delete[] checkInCodelets149;
    delete[] checkInCodelets146;
    delete[] checkInCodelets114;
    delete[] checkInCodelets111;
    delete[] nextCodeletscompute_w_coefficients;
    delete[] nextSyncCodeletscompute_w_coefficients;
    delete[] n_darts15;
    delete[] a_darts15;
    delete[] b_darts15;
    delete[] W_darts15;
    delete[] ab_darts15;
    delete[] c_darts15;
    delete[] k_darts15;
    delete[] s_darts15;
    delete[] twoPiOverN_darts15;
}
void TP15::setNewInputs(int in_n, int in_a, int in_b, COMPLEX* in_W, size_t codeletID)
{
    this->n_darts15[codeletID] = in_n;
    this->a_darts15[codeletID] = in_a;
    this->b_darts15[codeletID] = in_b;
    this->W_darts15[codeletID] = in_W;
}
/*TP149: OMPTaskDirective*/
void TP149::_checkInCodelets150::fire(void)
{
/*Init the vars for this region*/

/*printing node 150: CallExpr*/
/*Finding the next codelet after the function call*/
#if 0
/*Signaling next codelet region: 150 nextRegion: 11539 */
myTP->controlTPParent->checkInCodelets11539.decDep();
#endif
/*Finding the next sync point before making the function call*/
#if 0
/*Find the sync point for a function call*/
/*syncNode: OMPTaskwaitDirective 154*/
/*No syncpoint found or outside task's boundaries, looking for syncCodelet*/
#endif
    /*Make the function call*/
    invoke<TP_compute_w_coefficients>(myTP, myTP->numThreads, this->getID(),
        &(myTP->controlTPParent->checkInCodelets11539),
        (myTP->controlTPParent->task149Inputs->nextSyncCodelet), nullptr,
        (this->taskInputs->n_darts149), (this->taskInputs->a_darts149),
        (this->taskInputs->ab_darts149), (this->taskInputs->W_darts149));
}
void TP149::_checkInCodelets11539::fire(void)
{

    /*printing node 11539: NullStmt*/
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task149Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task149Inputs->nextSyncCodelet->decDep();
}
TP149::TP149(
    int in_numThreads, int in_mainCodeletID, TP15* in_TPParent, _task149Inputs* in_task149Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task149Inputs(in_task149Inputs)
    , TP150Ptr(nullptr)
    , TP150_alreadyLaunched(0)
    , checkInCodelets150(1, 1, this, this->mainCodeletID)
    , checkInCodelets11539(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets150.decDep();
}
TP149::~TP149() { delete (task149Inputs); }
/*TP151: OMPTaskDirective*/
void TP151::_checkInCodelets152::fire(void)
{
/*Init the vars for this region*/

/*printing node 152: CallExpr*/
/*Finding the next codelet after the function call*/
#if 0
/*Signaling next codelet region: 152 nextRegion: 11540 */
myTP->controlTPParent->checkInCodelets11540.decDep();
#endif
/*Finding the next sync point before making the function call*/
#if 0
/*Find the sync point for a function call*/
/*syncNode: OMPTaskwaitDirective 154*/
/*No syncpoint found or outside task's boundaries, looking for syncCodelet*/
#endif
    /*Make the function call*/
    invoke<TP_compute_w_coefficients>(myTP, myTP->numThreads, this->getID(),
        &(myTP->controlTPParent->checkInCodelets11540),
        (myTP->controlTPParent->task151Inputs->nextSyncCodelet), nullptr,
        (this->taskInputs->n_darts151), (this->taskInputs->ab_darts151) + 1,
        (this->taskInputs->b_darts151), (this->taskInputs->W_darts151));
}
void TP151::_checkInCodelets11540::fire(void)
{

    /*printing node 11540: NullStmt*/
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task151Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task151Inputs->nextSyncCodelet->decDep();
}
TP151::TP151(
    int in_numThreads, int in_mainCodeletID, TP15* in_TPParent, _task151Inputs* in_task151Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task151Inputs(in_task151Inputs)
    , TP152Ptr(nullptr)
    , TP152_alreadyLaunched(0)
    , checkInCodelets152(1, 1, this, this->mainCodeletID)
    , checkInCodelets11540(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets152.decDep();
}
TP151::~TP151() { delete (task151Inputs); }
/*TP18: TP_unshuffle*/
void TP18::_checkInCodelets236::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    /*Init the vars for this region*/

    /*printing node 236: DeclStmt*/

    /*printing node 237: DeclStmt*/
    this->inputsTPParent->r4_darts18[this->getID()]
        = (this->inputsTPParent->r_darts18[this->getID()]) & (~3);

    /*printing node 240: DeclStmt*/

    /*printing node 241: DeclStmt*/
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 236 nextRegion: 242 */
    myTP->controlTPParent->checkInCodelets242[this->getID()].decDep();
}
void TP18::_checkInCodelets242::fire(void)
{
    /*Printing conditional branch node 242: inlining: 0*/
    if ((this->inputsTPParent->b_darts18[this->getID()])
            - (this->inputsTPParent->a_darts18[this->getID()])
        < 16) {
        myTP->checkInCodelets245[this->getID()].decDep();
    } else {
        /*elseRegion: DeclStmt 287*/
        myTP->checkInCodelets287[this->getID()].decDep();
    }
}
void TP18::_checkInCodelets245::fire(void)
{

    /*printing node 245: BinaryOperator*/
    (this->inputsTPParent->ip_darts18[this->getID()])
        = (this->inputsTPParent->in_darts18[this->getID()])
        + (this->inputsTPParent->a_darts18[this->getID()])
            * (this->inputsTPParent->r_darts18[this->getID()]);

    /*printing node 248: ForStmt*/
    {
        int* b = &(this->inputsTPParent->b_darts18[this->getLocalID()]);
        (void)b /*PRIVATE*/;
        int* i = &(this->inputsTPParent->i_darts18[this->getLocalID()]);
        (void)i /*PRIVATE*/;
        COMPLEX** ip = &(this->inputsTPParent->ip_darts18[this->getLocalID()]);
        (void)ip /*PRIVATE*/;
        int* j = &(this->inputsTPParent->j_darts18[this->getLocalID()]);
        (void)j /*PRIVATE*/;
        COMPLEX** jp = &(this->inputsTPParent->jp_darts18[this->getLocalID()]);
        (void)jp /*PRIVATE*/;
        int* m = &(this->inputsTPParent->m_darts18[this->getLocalID()]);
        (void)m /*PRIVATE*/;
        COMPLEX** out = &(this->inputsTPParent->out_darts18[this->getLocalID()]);
        (void)out /*PRIVATE*/;
        int* r = &(this->inputsTPParent->r_darts18[this->getLocalID()]);
        (void)r /*PRIVATE*/;
        int* r4 = &(this->inputsTPParent->r4_darts18[this->getLocalID()]);
        (void)r4 /*PRIVATE*/;
        /*Loop's init*/
        (this->inputsTPParent->i_darts18[this->getID()])
            = (this->inputsTPParent->a_darts18[this->getID()]);
        int i_darts_counter_temp18 = (this->inputsTPParent->i_darts18[this->getID()]);
        for (; (i_darts_counter_temp18) < (*b); ++(i_darts_counter_temp18)) {
            (*jp) = (*out) + (i_darts_counter_temp18);
            {
                /*Loop's init*/
                (*j) = 0;
                int j_darts_counter_temp18 = (*j);
                for (; j_darts_counter_temp18 < (*r4); j_darts_counter_temp18 += 4) {
                    (*jp)[0] = (*ip)[0];
                    (*jp)[(*m)] = (*ip)[1];
                    (*jp)[2 * (*m)] = (*ip)[2];
                    (*jp)[3 * (*m)] = (*ip)[3];
                    (*jp) += 4 * (*m);
                    (*ip) += 4;
                }
                (*j) = j_darts_counter_temp18;
            }
            {
                for (; (*j) < (*r); ++(*j)) {
                    *(*jp) = *(*ip);
                    (*ip)++;
                    (*jp) += (*m);
                }
            }
        }
        (this->inputsTPParent->i_darts18[this->getID()]) = i_darts_counter_temp18;
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/

    myTP->controlTPParent->nextCodeletsunshuffle[this->getID()]->decDep();
}
void TP18::_checkInCodelets287::fire(void)
{

    /*printing node 287: DeclStmt*/
    this->inputsTPParent->ab_darts18[this->getID()]
        = ((this->inputsTPParent->a_darts18[this->getID()])
              + (this->inputsTPParent->b_darts18[this->getID()]))
        / 2;
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 287 nextRegion: 290 */
    myTP->controlTPParent->checkInCodelets290[this->getID()].decDep();
}
void TP18::_checkInCodelets290::fire(void)
{

    /*printing node 290: OMPTaskDirective*/
    /*syncNode: OMPTaskwaitDirective 294*/
    /*Find the TP to which the nextRegion belongs*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->barrierCodelets294[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 290*/
    _task290Inputs* task290Inputs
        = new _task290Inputs(&((this->inputsTPParent->a_darts18[this->getID()])),
            &((this->inputsTPParent->ab_darts18[this->getID()])),
            &((this->inputsTPParent->in_darts18[this->getID()])),
            &((this->inputsTPParent->m_darts18[this->getID()])),
            &((this->inputsTPParent->out_darts18[this->getID()])),
            &((this->inputsTPParent->r_darts18[this->getID()])), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task290Inputs[this->getID()] = task290Inputs;
    invoke<TP290>(myTP, 1, this->getID(), myTP, task290Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 290 nextRegion: 292 */
    myTP->controlTPParent->checkInCodelets292[this->getID()].decDep();
}
void TP18::_checkInCodelets292::fire(void)
{

    /*printing node 292: OMPTaskDirective*/
    /*syncNode: OMPTaskwaitDirective 294*/
    /*Find the TP to which the nextRegion belongs*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->barrierCodelets294[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 292*/
    _task292Inputs* task292Inputs
        = new _task292Inputs(&((this->inputsTPParent->ab_darts18[this->getID()])),
            &((this->inputsTPParent->b_darts18[this->getID()])),
            &((this->inputsTPParent->in_darts18[this->getID()])),
            &((this->inputsTPParent->m_darts18[this->getID()])),
            &((this->inputsTPParent->out_darts18[this->getID()])),
            &((this->inputsTPParent->r_darts18[this->getID()])), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task292Inputs[this->getID()] = task292Inputs;
    invoke<TP292>(myTP, 1, this->getID(), myTP, task292Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 292 nextRegion: 294 */
    myTP->controlTPParent->barrierCodelets294[0].decDep();
}
void TP18::_barrierCodelets294::fire(void)
{
    TP18* myTP = static_cast<TP18*>(myTP_);

    for (size_t codeletsCounter = 0; codeletsCounter < (size_t)myTP->numThreads;
         codeletsCounter++) {
        myTP->nextCodeletsunshuffle[codeletsCounter]->decDep();
    }
}
TP18::TP18(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_mainNextCodelet,
    darts::Codelet* in_mainSyncCodelet, TP18** in_ptrToThisFunctionTP, int in_a, int in_b,
    COMPLEX* in_in, COMPLEX* in_out, int in_r, int in_m)
    : ompTP(in_numThreads, in_mainCodeletID)
    , ptrToThisFunctionTP(in_ptrToThisFunctionTP)
    , inputsTPParent(this)
    , controlTPParent(this)
    , nextCodeletsunshuffle(new Codelet*[in_numThreads])
    , nextSyncCodeletsunshuffle(new Codelet*[in_numThreads])
    , a_darts18(new int[this->numThreads])
    , b_darts18(new int[this->numThreads])
    , in_darts18(new COMPLEX*[this->numThreads])
    , out_darts18(new COMPLEX*[this->numThreads])
    , r_darts18(new int[this->numThreads])
    , m_darts18(new int[this->numThreads])
    , ab_darts18(new int[this->numThreads])
    , i_darts18(new int[this->numThreads])
    , ip_darts18(new COMPLEX*[this->numThreads])
    , j_darts18(new int[this->numThreads])
    , jp_darts18(new COMPLEX*[this->numThreads])
    , r4_darts18(new int[this->numThreads])
    , task290Inputs(new _task290Inputs*[this->numThreads])
    , task292Inputs(new _task292Inputs*[this->numThreads])
    , checkInCodelets236(new _checkInCodelets236[this->numThreads])
    , checkInCodelets242(new _checkInCodelets242[this->numThreads])
    , checkInCodelets245(new _checkInCodelets245[this->numThreads])
    , checkInCodelets287(new _checkInCodelets287[this->numThreads])
    , checkInCodelets290(new _checkInCodelets290[this->numThreads])
    , checkInCodelets292(new _checkInCodelets292[this->numThreads])
    , barrierCodelets294(new _barrierCodelets294[1])
{
    barrierCodelets294[0] = _barrierCodelets294(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets292* checkInCodelets292Ptr = (this->checkInCodelets292);
    _checkInCodelets290* checkInCodelets290Ptr = (this->checkInCodelets290);
    _checkInCodelets287* checkInCodelets287Ptr = (this->checkInCodelets287);
    _checkInCodelets245* checkInCodelets245Ptr = (this->checkInCodelets245);
    _checkInCodelets242* checkInCodelets242Ptr = (this->checkInCodelets242);
    _checkInCodelets236* checkInCodelets236Ptr = (this->checkInCodelets236);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets236);
#endif
    for (size_t codeletCounter = 0; codeletCounter < this->numThreads; codeletCounter++) {
        (*checkInCodelets292Ptr) = _checkInCodelets292(1, 1, this, codeletCounter);
        checkInCodelets292Ptr++;
        (*checkInCodelets290Ptr) = _checkInCodelets290(1, 1, this, codeletCounter);
        checkInCodelets290Ptr++;
        (*checkInCodelets287Ptr) = _checkInCodelets287(1, 1, this, codeletCounter);
        checkInCodelets287Ptr++;
        (*checkInCodelets245Ptr) = _checkInCodelets245(1, 1, this, codeletCounter);
        checkInCodelets245Ptr++;
        (*checkInCodelets242Ptr) = _checkInCodelets242(1, 1, this, codeletCounter);
        checkInCodelets242Ptr++;
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets236Ptr) = _checkInCodelets236(2, 1, this, codeletCounter);
#else
        (*checkInCodelets236Ptr) = _checkInCodelets236(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets236Ptr).decDep();
        checkInCodelets236Ptr++;
    }
    if (this->numThreads == 1) {
        this->nextCodeletsunshuffle[0] = in_mainNextCodelet;
        this->nextSyncCodeletsunshuffle[0] = in_mainSyncCodelet;
        this->a_darts18[0] = in_a;
        this->b_darts18[0] = in_b;
        this->in_darts18[0] = in_in;
        this->out_darts18[0] = in_out;
        this->r_darts18[0] = in_r;
        this->m_darts18[0] = in_m;
        this->availableCodelets[0] = 1;
    } else {
        this->a_darts18[this->mainCodeletID] = in_a;
        this->b_darts18[this->mainCodeletID] = in_b;
        this->in_darts18[this->mainCodeletID] = in_in;
        this->out_darts18[this->mainCodeletID] = in_out;
        this->r_darts18[this->mainCodeletID] = in_r;
        this->m_darts18[this->mainCodeletID] = in_m;
        this->nextCodeletsunshuffle[in_mainCodeletID] = in_mainNextCodelet;
        this->nextSyncCodeletsunshuffle[in_mainCodeletID] = in_mainSyncCodelet;
#if USE_SPIN_CODELETS == 0
        this->firstCodelet[this->mainCodeletID].decDep();
#else
        this->availableCodelets[this->mainCodeletID] = 1;
#endif
        *(this->ptrToThisFunctionTP) = this;
    }
}
TP18::~TP18()
{
    delete[] task290Inputs;
    delete[] task292Inputs;
    delete[] barrierCodelets294;
    delete[] checkInCodelets292;
    delete[] checkInCodelets290;
    delete[] checkInCodelets287;
    delete[] checkInCodelets245;
    delete[] checkInCodelets242;
    delete[] checkInCodelets236;
    delete[] nextCodeletsunshuffle;
    delete[] nextSyncCodeletsunshuffle;
    delete[] a_darts18;
    delete[] b_darts18;
    delete[] in_darts18;
    delete[] out_darts18;
    delete[] r_darts18;
    delete[] m_darts18;
    delete[] ab_darts18;
    delete[] i_darts18;
    delete[] ip_darts18;
    delete[] j_darts18;
    delete[] jp_darts18;
    delete[] r4_darts18;
}
void TP18::setNewInputs(
    int in_a, int in_b, COMPLEX* in_in, COMPLEX* in_out, int in_r, int in_m, size_t codeletID)
{
    this->a_darts18[codeletID] = in_a;
    this->b_darts18[codeletID] = in_b;
    this->in_darts18[codeletID] = in_in;
    this->out_darts18[codeletID] = in_out;
    this->r_darts18[codeletID] = in_r;
    this->m_darts18[codeletID] = in_m;
}
/*TP290: OMPTaskDirective*/
void TP290::_checkInCodelets291::fire(void)
{
/*Init the vars for this region*/

/*printing node 291: CallExpr*/
/*Finding the next codelet after the function call*/
#if 0
/*Signaling next codelet region: 291 nextRegion: 11520 */
myTP->controlTPParent->checkInCodelets11520.decDep();
#endif
/*Finding the next sync point before making the function call*/
#if 0
/*Find the sync point for a function call*/
/*syncNode: OMPTaskwaitDirective 294*/
/*No syncpoint found or outside task's boundaries, looking for syncCodelet*/
#endif
    /*Make the function call*/
    invoke<TP_unshuffle>(myTP, myTP->numThreads, this->getID(),
        &(myTP->controlTPParent->checkInCodelets11520),
        (myTP->controlTPParent->task290Inputs->nextSyncCodelet), nullptr,
        (this->taskInputs->a_darts290), (this->taskInputs->ab_darts290),
        (this->taskInputs->in_darts290), (this->taskInputs->out_darts290),
        (this->taskInputs->r_darts290), (this->taskInputs->m_darts290));
}
void TP290::_checkInCodelets11520::fire(void)
{

    /*printing node 11520: NullStmt*/
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task290Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task290Inputs->nextSyncCodelet->decDep();
}
TP290::TP290(
    int in_numThreads, int in_mainCodeletID, TP18* in_TPParent, _task290Inputs* in_task290Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task290Inputs(in_task290Inputs)
    , TP291Ptr(nullptr)
    , TP291_alreadyLaunched(0)
    , checkInCodelets291(1, 1, this, this->mainCodeletID)
    , checkInCodelets11520(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets291.decDep();
}
TP290::~TP290() { delete (task290Inputs); }
/*TP292: OMPTaskDirective*/
void TP292::_checkInCodelets293::fire(void)
{
/*Init the vars for this region*/

/*printing node 293: CallExpr*/
/*Finding the next codelet after the function call*/
#if 0
/*Signaling next codelet region: 293 nextRegion: 11521 */
myTP->controlTPParent->checkInCodelets11521.decDep();
#endif
/*Finding the next sync point before making the function call*/
#if 0
/*Find the sync point for a function call*/
/*syncNode: OMPTaskwaitDirective 294*/
/*No syncpoint found or outside task's boundaries, looking for syncCodelet*/
#endif
    /*Make the function call*/
    invoke<TP_unshuffle>(myTP, myTP->numThreads, this->getID(),
        &(myTP->controlTPParent->checkInCodelets11521),
        (myTP->controlTPParent->task292Inputs->nextSyncCodelet), nullptr,
        (this->taskInputs->ab_darts292), (this->taskInputs->b_darts292),
        (this->taskInputs->in_darts292), (this->taskInputs->out_darts292),
        (this->taskInputs->r_darts292), (this->taskInputs->m_darts292));
}
void TP292::_checkInCodelets11521::fire(void)
{

    /*printing node 11521: NullStmt*/
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task292Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task292Inputs->nextSyncCodelet->decDep();
}
TP292::TP292(
    int in_numThreads, int in_mainCodeletID, TP18* in_TPParent, _task292Inputs* in_task292Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task292Inputs(in_task292Inputs)
    , TP293Ptr(nullptr)
    , TP293_alreadyLaunched(0)
    , checkInCodelets293(1, 1, this, this->mainCodeletID)
    , checkInCodelets11521(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets293.decDep();
}
TP292::~TP292() { delete (task292Inputs); }
/*TP21: TP_fft_twiddle_gen*/
void TP21::_checkInCodelets415::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    /*Init the vars for this region*/
    /*Printing conditional branch node 415: inlining: 0*/
    if ((this->inputsTPParent->i_darts21[this->getID()])
        == (this->inputsTPParent->i1_darts21[this->getID()]) - 1) {
        myTP->checkInCodelets418[this->getID()].decDep();
    } else {
        /*elseRegion: DeclStmt 425*/
        myTP->checkInCodelets425[this->getID()].decDep();
    }
}
void TP21::_barrierCodelets432::fire(void)
{
    TP21* myTP = static_cast<TP21*>(myTP_);

    for (size_t codeletsCounter = 0; codeletsCounter < (size_t)myTP->numThreads;
         codeletsCounter++) {
        myTP->nextCodeletsfft_twiddle_gen[codeletsCounter]->decDep();
    }
}
void TP21::_checkInCodelets418::fire(void)
{

    /*printing node 418: OMPTaskDirective*/
    /*syncNode: OMPTaskwaitDirective 432*/
    /*Find the TP to which the nextRegion belongs*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->barrierCodelets432[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 418*/
    _task418Inputs* task418Inputs
        = new _task418Inputs(&((this->inputsTPParent->W_darts21[this->getID()])),
            &((this->inputsTPParent->i_darts21[this->getID()])),
            &((this->inputsTPParent->in_darts21[this->getID()])),
            &((this->inputsTPParent->m_darts21[this->getID()])),
            &((this->inputsTPParent->nW_darts21[this->getID()])),
            &((this->inputsTPParent->nWdn_darts21[this->getID()])),
            &((this->inputsTPParent->out_darts21[this->getID()])),
            &((this->inputsTPParent->r_darts21[this->getID()])), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task418Inputs[this->getID()] = task418Inputs;
    invoke<TP418>(myTP, 1, this->getID(), myTP, task418Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 418 nextRegion: 432 */
    myTP->controlTPParent->barrierCodelets432[0].decDep();
}
void TP21::_checkInCodelets425::fire(void)
{

    /*printing node 425: DeclStmt*/
    this->inputsTPParent->i2_darts21[this->getID()]
        = ((this->inputsTPParent->i_darts21[this->getID()])
              + (this->inputsTPParent->i1_darts21[this->getID()]))
        / 2;
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 425 nextRegion: 428 */
    myTP->controlTPParent->checkInCodelets428[this->getID()].decDep();
}
void TP21::_checkInCodelets428::fire(void)
{

    /*printing node 428: OMPTaskDirective*/
    /*syncNode: OMPTaskwaitDirective 432*/
    /*Find the TP to which the nextRegion belongs*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->barrierCodelets432[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 428*/
    _task428Inputs* task428Inputs
        = new _task428Inputs(&((this->inputsTPParent->W_darts21[this->getID()])),
            &((this->inputsTPParent->i_darts21[this->getID()])),
            &((this->inputsTPParent->i2_darts21[this->getID()])),
            &((this->inputsTPParent->in_darts21[this->getID()])),
            &((this->inputsTPParent->m_darts21[this->getID()])),
            &((this->inputsTPParent->nW_darts21[this->getID()])),
            &((this->inputsTPParent->nWdn_darts21[this->getID()])),
            &((this->inputsTPParent->out_darts21[this->getID()])),
            &((this->inputsTPParent->r_darts21[this->getID()])), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task428Inputs[this->getID()] = task428Inputs;
    invoke<TP428>(myTP, 1, this->getID(), myTP, task428Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 428 nextRegion: 430 */
    myTP->controlTPParent->checkInCodelets430[this->getID()].decDep();
}
void TP21::_checkInCodelets430::fire(void)
{

    /*printing node 430: OMPTaskDirective*/
    /*syncNode: OMPTaskwaitDirective 432*/
    /*Find the TP to which the nextRegion belongs*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->barrierCodelets432[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 430*/
    _task430Inputs* task430Inputs
        = new _task430Inputs(&((this->inputsTPParent->W_darts21[this->getID()])),
            &((this->inputsTPParent->i1_darts21[this->getID()])),
            &((this->inputsTPParent->i2_darts21[this->getID()])),
            &((this->inputsTPParent->in_darts21[this->getID()])),
            &((this->inputsTPParent->m_darts21[this->getID()])),
            &((this->inputsTPParent->nW_darts21[this->getID()])),
            &((this->inputsTPParent->nWdn_darts21[this->getID()])),
            &((this->inputsTPParent->out_darts21[this->getID()])),
            &((this->inputsTPParent->r_darts21[this->getID()])), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task430Inputs[this->getID()] = task430Inputs;
    invoke<TP430>(myTP, 1, this->getID(), myTP, task430Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 430 nextRegion: 432 */
    myTP->controlTPParent->barrierCodelets432[0].decDep();
}
TP21::TP21(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_mainNextCodelet,
    darts::Codelet* in_mainSyncCodelet, TP21** in_ptrToThisFunctionTP, int in_i, int in_i1,
    COMPLEX* in_in, COMPLEX* in_out, COMPLEX* in_W, int in_nW, int in_nWdn, int in_r, int in_m)
    : ompTP(in_numThreads, in_mainCodeletID)
    , ptrToThisFunctionTP(in_ptrToThisFunctionTP)
    , inputsTPParent(this)
    , controlTPParent(this)
    , nextCodeletsfft_twiddle_gen(new Codelet*[in_numThreads])
    , nextSyncCodeletsfft_twiddle_gen(new Codelet*[in_numThreads])
    , i_darts21(new int[this->numThreads])
    , i1_darts21(new int[this->numThreads])
    , in_darts21(new COMPLEX*[this->numThreads])
    , out_darts21(new COMPLEX*[this->numThreads])
    , W_darts21(new COMPLEX*[this->numThreads])
    , nW_darts21(new int[this->numThreads])
    , nWdn_darts21(new int[this->numThreads])
    , r_darts21(new int[this->numThreads])
    , m_darts21(new int[this->numThreads])
    , i2_darts21(new int[this->numThreads])
    , task418Inputs(new _task418Inputs*[this->numThreads])
    , task428Inputs(new _task428Inputs*[this->numThreads])
    , task430Inputs(new _task430Inputs*[this->numThreads])
    , checkInCodelets415(new _checkInCodelets415[this->numThreads])
    , barrierCodelets432(new _barrierCodelets432[1])
    , checkInCodelets418(new _checkInCodelets418[this->numThreads])
    , checkInCodelets425(new _checkInCodelets425[this->numThreads])
    , checkInCodelets428(new _checkInCodelets428[this->numThreads])
    , checkInCodelets430(new _checkInCodelets430[this->numThreads])
{
    barrierCodelets432[0] = _barrierCodelets432(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets430* checkInCodelets430Ptr = (this->checkInCodelets430);
    _checkInCodelets428* checkInCodelets428Ptr = (this->checkInCodelets428);
    _checkInCodelets425* checkInCodelets425Ptr = (this->checkInCodelets425);
    _checkInCodelets418* checkInCodelets418Ptr = (this->checkInCodelets418);
    _checkInCodelets415* checkInCodelets415Ptr = (this->checkInCodelets415);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets415);
#endif
    for (size_t codeletCounter = 0; codeletCounter < this->numThreads; codeletCounter++) {
        (*checkInCodelets430Ptr) = _checkInCodelets430(1, 1, this, codeletCounter);
        checkInCodelets430Ptr++;
        (*checkInCodelets428Ptr) = _checkInCodelets428(1, 1, this, codeletCounter);
        checkInCodelets428Ptr++;
        (*checkInCodelets425Ptr) = _checkInCodelets425(1, 1, this, codeletCounter);
        checkInCodelets425Ptr++;
        (*checkInCodelets418Ptr) = _checkInCodelets418(1, 1, this, codeletCounter);
        checkInCodelets418Ptr++;
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets415Ptr) = _checkInCodelets415(2, 1, this, codeletCounter);
#else
        (*checkInCodelets415Ptr) = _checkInCodelets415(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets415Ptr).decDep();
        checkInCodelets415Ptr++;
    }
    if (this->numThreads == 1) {
        this->nextCodeletsfft_twiddle_gen[0] = in_mainNextCodelet;
        this->nextSyncCodeletsfft_twiddle_gen[0] = in_mainSyncCodelet;
        this->i_darts21[0] = in_i;
        this->i1_darts21[0] = in_i1;
        this->in_darts21[0] = in_in;
        this->out_darts21[0] = in_out;
        this->W_darts21[0] = in_W;
        this->nW_darts21[0] = in_nW;
        this->nWdn_darts21[0] = in_nWdn;
        this->r_darts21[0] = in_r;
        this->m_darts21[0] = in_m;
        this->availableCodelets[0] = 1;
    } else {
        this->i_darts21[this->mainCodeletID] = in_i;
        this->i1_darts21[this->mainCodeletID] = in_i1;
        this->in_darts21[this->mainCodeletID] = in_in;
        this->out_darts21[this->mainCodeletID] = in_out;
        this->W_darts21[this->mainCodeletID] = in_W;
        this->nW_darts21[this->mainCodeletID] = in_nW;
        this->nWdn_darts21[this->mainCodeletID] = in_nWdn;
        this->r_darts21[this->mainCodeletID] = in_r;
        this->m_darts21[this->mainCodeletID] = in_m;
        this->nextCodeletsfft_twiddle_gen[in_mainCodeletID] = in_mainNextCodelet;
        this->nextSyncCodeletsfft_twiddle_gen[in_mainCodeletID] = in_mainSyncCodelet;
#if USE_SPIN_CODELETS == 0
        this->firstCodelet[this->mainCodeletID].decDep();
#else
        this->availableCodelets[this->mainCodeletID] = 1;
#endif
        *(this->ptrToThisFunctionTP) = this;
    }
}
TP21::~TP21()
{
    delete[] task418Inputs;
    delete[] task428Inputs;
    delete[] task430Inputs;
    delete[] checkInCodelets430;
    delete[] checkInCodelets428;
    delete[] checkInCodelets425;
    delete[] checkInCodelets418;
    delete[] barrierCodelets432;
    delete[] checkInCodelets415;
    delete[] nextCodeletsfft_twiddle_gen;
    delete[] nextSyncCodeletsfft_twiddle_gen;
    delete[] i_darts21;
    delete[] i1_darts21;
    delete[] in_darts21;
    delete[] out_darts21;
    delete[] W_darts21;
    delete[] nW_darts21;
    delete[] nWdn_darts21;
    delete[] r_darts21;
    delete[] m_darts21;
    delete[] i2_darts21;
}
void TP21::setNewInputs(int in_i, int in_i1, COMPLEX* in_in, COMPLEX* in_out, COMPLEX* in_W,
    int in_nW, int in_nWdn, int in_r, int in_m, size_t codeletID)
{
    this->i_darts21[codeletID] = in_i;
    this->i1_darts21[codeletID] = in_i1;
    this->in_darts21[codeletID] = in_in;
    this->out_darts21[codeletID] = in_out;
    this->W_darts21[codeletID] = in_W;
    this->nW_darts21[codeletID] = in_nW;
    this->nWdn_darts21[codeletID] = in_nWdn;
    this->r_darts21[codeletID] = in_r;
    this->m_darts21[codeletID] = in_m;
}
/*TP418: OMPTaskDirective*/
void TP418::_checkInCodelets419::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 419: CallExpr*/
    fft_twiddle_gen1((this->taskInputs->in_darts418) + (this->taskInputs->i_darts418),
        (this->taskInputs->out_darts418) + (this->taskInputs->i_darts418),
        (this->taskInputs->W_darts418), (this->taskInputs->r_darts418),
        (this->taskInputs->m_darts418), (this->taskInputs->nW_darts418),
        (this->taskInputs->nWdn_darts418) * (this->taskInputs->i_darts418),
        (this->taskInputs->nWdn_darts418) * (this->taskInputs->m_darts418));
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task418Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task418Inputs->nextSyncCodelet->decDep();
}
TP418::TP418(
    int in_numThreads, int in_mainCodeletID, TP21* in_TPParent, _task418Inputs* in_task418Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task418Inputs(in_task418Inputs)
    , checkInCodelets419(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets419.decDep();
}
TP418::~TP418() { delete (task418Inputs); }
/*TP428: OMPTaskDirective*/
void TP428::_checkInCodelets429::fire(void)
{
/*Init the vars for this region*/

/*printing node 429: CallExpr*/
/*Finding the next codelet after the function call*/
#if 0
/*Signaling next codelet region: 429 nextRegion: 11516 */
myTP->controlTPParent->checkInCodelets11516.decDep();
#endif
/*Finding the next sync point before making the function call*/
#if 0
/*Find the sync point for a function call*/
/*syncNode: OMPTaskwaitDirective 432*/
/*No syncpoint found or outside task's boundaries, looking for syncCodelet*/
#endif
    /*Make the function call*/
    invoke<TP_fft_twiddle_gen>(myTP, myTP->numThreads, this->getID(),
        &(myTP->controlTPParent->checkInCodelets11516),
        (myTP->controlTPParent->task428Inputs->nextSyncCodelet), nullptr,
        (this->taskInputs->i_darts428), (this->taskInputs->i2_darts428),
        (this->taskInputs->in_darts428), (this->taskInputs->out_darts428),
        (this->taskInputs->W_darts428), (this->taskInputs->nW_darts428),
        (this->taskInputs->nWdn_darts428), (this->taskInputs->r_darts428),
        (this->taskInputs->m_darts428));
}
void TP428::_checkInCodelets11516::fire(void)
{

    /*printing node 11516: NullStmt*/
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task428Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task428Inputs->nextSyncCodelet->decDep();
}
TP428::TP428(
    int in_numThreads, int in_mainCodeletID, TP21* in_TPParent, _task428Inputs* in_task428Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task428Inputs(in_task428Inputs)
    , TP429Ptr(nullptr)
    , TP429_alreadyLaunched(0)
    , checkInCodelets429(1, 1, this, this->mainCodeletID)
    , checkInCodelets11516(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets429.decDep();
}
TP428::~TP428() { delete (task428Inputs); }
/*TP430: OMPTaskDirective*/
void TP430::_checkInCodelets431::fire(void)
{
/*Init the vars for this region*/

/*printing node 431: CallExpr*/
/*Finding the next codelet after the function call*/
#if 0
/*Signaling next codelet region: 431 nextRegion: 11517 */
myTP->controlTPParent->checkInCodelets11517.decDep();
#endif
/*Finding the next sync point before making the function call*/
#if 0
/*Find the sync point for a function call*/
/*syncNode: OMPTaskwaitDirective 432*/
/*No syncpoint found or outside task's boundaries, looking for syncCodelet*/
#endif
    /*Make the function call*/
    invoke<TP_fft_twiddle_gen>(myTP, myTP->numThreads, this->getID(),
        &(myTP->controlTPParent->checkInCodelets11517),
        (myTP->controlTPParent->task430Inputs->nextSyncCodelet), nullptr,
        (this->taskInputs->i2_darts430), (this->taskInputs->i1_darts430),
        (this->taskInputs->in_darts430), (this->taskInputs->out_darts430),
        (this->taskInputs->W_darts430), (this->taskInputs->nW_darts430),
        (this->taskInputs->nWdn_darts430), (this->taskInputs->r_darts430),
        (this->taskInputs->m_darts430));
}
void TP430::_checkInCodelets11517::fire(void)
{

    /*printing node 11517: NullStmt*/
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task430Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task430Inputs->nextSyncCodelet->decDep();
}
TP430::TP430(
    int in_numThreads, int in_mainCodeletID, TP21* in_TPParent, _task430Inputs* in_task430Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task430Inputs(in_task430Inputs)
    , TP431Ptr(nullptr)
    , TP431_alreadyLaunched(0)
    , checkInCodelets431(1, 1, this, this->mainCodeletID)
    , checkInCodelets11517(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets431.decDep();
}
TP430::~TP430() { delete (task430Inputs); }
/*TP24: TP_fft_twiddle_2*/
void TP24::_checkInCodelets483::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    /*Init the vars for this region*/
    /*Printing conditional branch node 483: inlining: 0*/
    if (((this->inputsTPParent->b_darts24[this->getID()])
            - (this->inputsTPParent->a_darts24[this->getID()]))
        < 128) {
        myTP->checkInCodelets486[this->getID()].decDep();
    } else {
        /*elseRegion: DeclStmt 555*/
        myTP->checkInCodelets555[this->getID()].decDep();
    }
}
void TP24::_checkInCodelets486::fire(void)
{

    /*printing node 486: ForStmt*/
    {
        COMPLEX** W = &(this->inputsTPParent->W_darts24[this->getLocalID()]);
        (void)W /*PRIVATE*/;
        int* b = &(this->inputsTPParent->b_darts24[this->getLocalID()]);
        (void)b /*PRIVATE*/;
        int* i = &(this->inputsTPParent->i_darts24[this->getLocalID()]);
        (void)i /*PRIVATE*/;
        REAL* i1_0 = &(this->inputsTPParent->i1_0_darts24[this->getLocalID()]);
        (void)i1_0 /*PRIVATE*/;
        REAL* i1_1 = &(this->inputsTPParent->i1_1_darts24[this->getLocalID()]);
        (void)i1_1 /*PRIVATE*/;
        COMPLEX** in = &(this->inputsTPParent->in_darts24[this->getLocalID()]);
        (void)in /*PRIVATE*/;
        COMPLEX** jp = &(this->inputsTPParent->jp_darts24[this->getLocalID()]);
        (void)jp /*PRIVATE*/;
        COMPLEX** kp = &(this->inputsTPParent->kp_darts24[this->getLocalID()]);
        (void)kp /*PRIVATE*/;
        int* l1 = &(this->inputsTPParent->l1_darts24[this->getLocalID()]);
        (void)l1 /*PRIVATE*/;
        int* m = &(this->inputsTPParent->m_darts24[this->getLocalID()]);
        (void)m /*PRIVATE*/;
        int* nWdn = &(this->inputsTPParent->nWdn_darts24[this->getLocalID()]);
        (void)nWdn /*PRIVATE*/;
        COMPLEX** out = &(this->inputsTPParent->out_darts24[this->getLocalID()]);
        (void)out /*PRIVATE*/;
        REAL* r1_0 = &(this->inputsTPParent->r1_0_darts24[this->getLocalID()]);
        (void)r1_0 /*PRIVATE*/;
        REAL* r1_1 = &(this->inputsTPParent->r1_1_darts24[this->getLocalID()]);
        (void)r1_1 /*PRIVATE*/;
        REAL* tmpi = &(this->inputsTPParent->tmpi_darts24[this->getLocalID()]);
        (void)tmpi /*PRIVATE*/;
        REAL* tmpr = &(this->inputsTPParent->tmpr_darts24[this->getLocalID()]);
        (void)tmpr /*PRIVATE*/;
        REAL* wi = &(this->inputsTPParent->wi_darts24[this->getLocalID()]);
        (void)wi /*PRIVATE*/;
        REAL* wr = &(this->inputsTPParent->wr_darts24[this->getLocalID()]);
        (void)wr /*PRIVATE*/;
        /*Loop's init*/
        (this->inputsTPParent->i_darts24[this->getID()])
            = (this->inputsTPParent->a_darts24[this->getID()]);
        int i_darts_counter_temp24 = (this->inputsTPParent->i_darts24[this->getID()]);
        for (; (i_darts_counter_temp24) < (*b); (i_darts_counter_temp24)++) {
            (*l1) = (*nWdn) * (i_darts_counter_temp24);
            (*kp) = (*out) + (i_darts_counter_temp24);
            (*jp) = (*in) + (i_darts_counter_temp24);
            {
                (*r1_0) = (((*jp)[0 * (*m)]).re);
                (*i1_0) = (((*jp)[0 * (*m)]).im);
                (*wr) = (((*W)[1 * (*l1)]).re);
                (*wi) = (((*W)[1 * (*l1)]).im);
                (*tmpr) = (((*jp)[1 * (*m)]).re);
                (*tmpi) = (((*jp)[1 * (*m)]).im);
                (*r1_1) = (((*wr) * (*tmpr)) - ((*wi) * (*tmpi)));
                (*i1_1) = (((*wi) * (*tmpr)) + ((*wr) * (*tmpi)));
                (((*kp)[0 * (*m)]).re) = ((*r1_0) + (*r1_1));
                (((*kp)[0 * (*m)]).im) = ((*i1_0) + (*i1_1));
                (((*kp)[1 * (*m)]).re) = ((*r1_0) - (*r1_1));
                (((*kp)[1 * (*m)]).im) = ((*i1_0) - (*i1_1));
            }
            (*l1) += (*nWdn);
            (*kp)++;
        }
        (this->inputsTPParent->i_darts24[this->getID()]) = i_darts_counter_temp24;
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/

    myTP->controlTPParent->nextCodeletsfft_twiddle_2[this->getID()]->decDep();
}
void TP24::_checkInCodelets555::fire(void)
{

    /*printing node 555: DeclStmt*/
    this->inputsTPParent->ab_darts24[this->getID()]
        = ((this->inputsTPParent->a_darts24[this->getID()])
              + (this->inputsTPParent->b_darts24[this->getID()]))
        / 2;
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 555 nextRegion: 558 */
    myTP->controlTPParent->checkInCodelets558[this->getID()].decDep();
}
void TP24::_checkInCodelets558::fire(void)
{

    /*printing node 558: OMPTaskDirective*/
    /*syncNode: OMPTaskwaitDirective 562*/
    /*Find the TP to which the nextRegion belongs*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->barrierCodelets562[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 558*/
    _task558Inputs* task558Inputs
        = new _task558Inputs(&((this->inputsTPParent->W_darts24[this->getID()])),
            &((this->inputsTPParent->a_darts24[this->getID()])),
            &((this->inputsTPParent->ab_darts24[this->getID()])),
            &((this->inputsTPParent->in_darts24[this->getID()])),
            &((this->inputsTPParent->m_darts24[this->getID()])),
            &((this->inputsTPParent->nW_darts24[this->getID()])),
            &((this->inputsTPParent->nWdn_darts24[this->getID()])),
            &((this->inputsTPParent->out_darts24[this->getID()])), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task558Inputs[this->getID()] = task558Inputs;
    invoke<TP558>(myTP, 1, this->getID(), myTP, task558Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 558 nextRegion: 560 */
    myTP->controlTPParent->checkInCodelets560[this->getID()].decDep();
}
void TP24::_checkInCodelets560::fire(void)
{

    /*printing node 560: OMPTaskDirective*/
    /*syncNode: OMPTaskwaitDirective 562*/
    /*Find the TP to which the nextRegion belongs*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->barrierCodelets562[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 560*/
    _task560Inputs* task560Inputs
        = new _task560Inputs(&((this->inputsTPParent->W_darts24[this->getID()])),
            &((this->inputsTPParent->ab_darts24[this->getID()])),
            &((this->inputsTPParent->b_darts24[this->getID()])),
            &((this->inputsTPParent->in_darts24[this->getID()])),
            &((this->inputsTPParent->m_darts24[this->getID()])),
            &((this->inputsTPParent->nW_darts24[this->getID()])),
            &((this->inputsTPParent->nWdn_darts24[this->getID()])),
            &((this->inputsTPParent->out_darts24[this->getID()])), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task560Inputs[this->getID()] = task560Inputs;
    invoke<TP560>(myTP, 1, this->getID(), myTP, task560Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 560 nextRegion: 562 */
    myTP->controlTPParent->barrierCodelets562[0].decDep();
}
void TP24::_barrierCodelets562::fire(void)
{
    TP24* myTP = static_cast<TP24*>(myTP_);

    for (size_t codeletsCounter = 0; codeletsCounter < (size_t)myTP->numThreads;
         codeletsCounter++) {
        myTP->nextCodeletsfft_twiddle_2[codeletsCounter]->decDep();
    }
}
TP24::TP24(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_mainNextCodelet,
    darts::Codelet* in_mainSyncCodelet, TP24** in_ptrToThisFunctionTP, int in_a, int in_b,
    COMPLEX* in_in, COMPLEX* in_out, COMPLEX* in_W, int in_nW, int in_nWdn, int in_m)
    : ompTP(in_numThreads, in_mainCodeletID)
    , ptrToThisFunctionTP(in_ptrToThisFunctionTP)
    , inputsTPParent(this)
    , controlTPParent(this)
    , nextCodeletsfft_twiddle_2(new Codelet*[in_numThreads])
    , nextSyncCodeletsfft_twiddle_2(new Codelet*[in_numThreads])
    , a_darts24(new int[this->numThreads])
    , b_darts24(new int[this->numThreads])
    , in_darts24(new COMPLEX*[this->numThreads])
    , out_darts24(new COMPLEX*[this->numThreads])
    , W_darts24(new COMPLEX*[this->numThreads])
    , nW_darts24(new int[this->numThreads])
    , nWdn_darts24(new int[this->numThreads])
    , m_darts24(new int[this->numThreads])
    , ab_darts24(new int[this->numThreads])
    , i_darts24(new int[this->numThreads])
    , i1_0_darts24(new REAL[this->numThreads])
    , i1_1_darts24(new REAL[this->numThreads])
    , jp_darts24(new COMPLEX*[this->numThreads])
    , kp_darts24(new COMPLEX*[this->numThreads])
    , l1_darts24(new int[this->numThreads])
    , r1_0_darts24(new REAL[this->numThreads])
    , r1_1_darts24(new REAL[this->numThreads])
    , tmpi_darts24(new REAL[this->numThreads])
    , tmpr_darts24(new REAL[this->numThreads])
    , wi_darts24(new REAL[this->numThreads])
    , wr_darts24(new REAL[this->numThreads])
    , task558Inputs(new _task558Inputs*[this->numThreads])
    , task560Inputs(new _task560Inputs*[this->numThreads])
    , checkInCodelets483(new _checkInCodelets483[this->numThreads])
    , checkInCodelets486(new _checkInCodelets486[this->numThreads])
    , checkInCodelets555(new _checkInCodelets555[this->numThreads])
    , checkInCodelets558(new _checkInCodelets558[this->numThreads])
    , checkInCodelets560(new _checkInCodelets560[this->numThreads])
    , barrierCodelets562(new _barrierCodelets562[1])
{
    barrierCodelets562[0] = _barrierCodelets562(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets560* checkInCodelets560Ptr = (this->checkInCodelets560);
    _checkInCodelets558* checkInCodelets558Ptr = (this->checkInCodelets558);
    _checkInCodelets555* checkInCodelets555Ptr = (this->checkInCodelets555);
    _checkInCodelets486* checkInCodelets486Ptr = (this->checkInCodelets486);
    _checkInCodelets483* checkInCodelets483Ptr = (this->checkInCodelets483);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets483);
#endif
    for (size_t codeletCounter = 0; codeletCounter < this->numThreads; codeletCounter++) {
        (*checkInCodelets560Ptr) = _checkInCodelets560(1, 1, this, codeletCounter);
        checkInCodelets560Ptr++;
        (*checkInCodelets558Ptr) = _checkInCodelets558(1, 1, this, codeletCounter);
        checkInCodelets558Ptr++;
        (*checkInCodelets555Ptr) = _checkInCodelets555(1, 1, this, codeletCounter);
        checkInCodelets555Ptr++;
        (*checkInCodelets486Ptr) = _checkInCodelets486(1, 1, this, codeletCounter);
        checkInCodelets486Ptr++;
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets483Ptr) = _checkInCodelets483(2, 1, this, codeletCounter);
#else
        (*checkInCodelets483Ptr) = _checkInCodelets483(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets483Ptr).decDep();
        checkInCodelets483Ptr++;
    }
    if (this->numThreads == 1) {
        this->nextCodeletsfft_twiddle_2[0] = in_mainNextCodelet;
        this->nextSyncCodeletsfft_twiddle_2[0] = in_mainSyncCodelet;
        this->a_darts24[0] = in_a;
        this->b_darts24[0] = in_b;
        this->in_darts24[0] = in_in;
        this->out_darts24[0] = in_out;
        this->W_darts24[0] = in_W;
        this->nW_darts24[0] = in_nW;
        this->nWdn_darts24[0] = in_nWdn;
        this->m_darts24[0] = in_m;
        this->availableCodelets[0] = 1;
    } else {
        this->a_darts24[this->mainCodeletID] = in_a;
        this->b_darts24[this->mainCodeletID] = in_b;
        this->in_darts24[this->mainCodeletID] = in_in;
        this->out_darts24[this->mainCodeletID] = in_out;
        this->W_darts24[this->mainCodeletID] = in_W;
        this->nW_darts24[this->mainCodeletID] = in_nW;
        this->nWdn_darts24[this->mainCodeletID] = in_nWdn;
        this->m_darts24[this->mainCodeletID] = in_m;
        this->nextCodeletsfft_twiddle_2[in_mainCodeletID] = in_mainNextCodelet;
        this->nextSyncCodeletsfft_twiddle_2[in_mainCodeletID] = in_mainSyncCodelet;
#if USE_SPIN_CODELETS == 0
        this->firstCodelet[this->mainCodeletID].decDep();
#else
        this->availableCodelets[this->mainCodeletID] = 1;
#endif
        *(this->ptrToThisFunctionTP) = this;
    }
}
TP24::~TP24()
{
    delete[] task558Inputs;
    delete[] task560Inputs;
    delete[] barrierCodelets562;
    delete[] checkInCodelets560;
    delete[] checkInCodelets558;
    delete[] checkInCodelets555;
    delete[] checkInCodelets486;
    delete[] checkInCodelets483;
    delete[] nextCodeletsfft_twiddle_2;
    delete[] nextSyncCodeletsfft_twiddle_2;
    delete[] a_darts24;
    delete[] b_darts24;
    delete[] in_darts24;
    delete[] out_darts24;
    delete[] W_darts24;
    delete[] nW_darts24;
    delete[] nWdn_darts24;
    delete[] m_darts24;
    delete[] ab_darts24;
    delete[] i_darts24;
    delete[] i1_0_darts24;
    delete[] i1_1_darts24;
    delete[] jp_darts24;
    delete[] kp_darts24;
    delete[] l1_darts24;
    delete[] r1_0_darts24;
    delete[] r1_1_darts24;
    delete[] tmpi_darts24;
    delete[] tmpr_darts24;
    delete[] wi_darts24;
    delete[] wr_darts24;
}
void TP24::setNewInputs(int in_a, int in_b, COMPLEX* in_in, COMPLEX* in_out, COMPLEX* in_W,
    int in_nW, int in_nWdn, int in_m, size_t codeletID)
{
    this->a_darts24[codeletID] = in_a;
    this->b_darts24[codeletID] = in_b;
    this->in_darts24[codeletID] = in_in;
    this->out_darts24[codeletID] = in_out;
    this->W_darts24[codeletID] = in_W;
    this->nW_darts24[codeletID] = in_nW;
    this->nWdn_darts24[codeletID] = in_nWdn;
    this->m_darts24[codeletID] = in_m;
}
/*TP558: OMPTaskDirective*/
void TP558::_checkInCodelets559::fire(void)
{
/*Init the vars for this region*/

/*printing node 559: CallExpr*/
/*Finding the next codelet after the function call*/
#if 0
/*Signaling next codelet region: 559 nextRegion: 11541 */
myTP->controlTPParent->checkInCodelets11541.decDep();
#endif
/*Finding the next sync point before making the function call*/
#if 0
/*Find the sync point for a function call*/
/*syncNode: OMPTaskwaitDirective 562*/
/*No syncpoint found or outside task's boundaries, looking for syncCodelet*/
#endif
    /*Make the function call*/
    invoke<TP_fft_twiddle_2>(myTP, myTP->numThreads, this->getID(),
        &(myTP->controlTPParent->checkInCodelets11541),
        (myTP->controlTPParent->task558Inputs->nextSyncCodelet), nullptr,
        (this->taskInputs->a_darts558), (this->taskInputs->ab_darts558),
        (this->taskInputs->in_darts558), (this->taskInputs->out_darts558),
        (this->taskInputs->W_darts558), (this->taskInputs->nW_darts558),
        (this->taskInputs->nWdn_darts558), (this->taskInputs->m_darts558));
}
void TP558::_checkInCodelets11541::fire(void)
{

    /*printing node 11541: NullStmt*/
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task558Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task558Inputs->nextSyncCodelet->decDep();
}
TP558::TP558(
    int in_numThreads, int in_mainCodeletID, TP24* in_TPParent, _task558Inputs* in_task558Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task558Inputs(in_task558Inputs)
    , TP559Ptr(nullptr)
    , TP559_alreadyLaunched(0)
    , checkInCodelets559(1, 1, this, this->mainCodeletID)
    , checkInCodelets11541(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets559.decDep();
}
TP558::~TP558() { delete (task558Inputs); }
/*TP560: OMPTaskDirective*/
void TP560::_checkInCodelets561::fire(void)
{
/*Init the vars for this region*/

/*printing node 561: CallExpr*/
/*Finding the next codelet after the function call*/
#if 0
/*Signaling next codelet region: 561 nextRegion: 11542 */
myTP->controlTPParent->checkInCodelets11542.decDep();
#endif
/*Finding the next sync point before making the function call*/
#if 0
/*Find the sync point for a function call*/
/*syncNode: OMPTaskwaitDirective 562*/
/*No syncpoint found or outside task's boundaries, looking for syncCodelet*/
#endif
    /*Make the function call*/
    invoke<TP_fft_twiddle_2>(myTP, myTP->numThreads, this->getID(),
        &(myTP->controlTPParent->checkInCodelets11542),
        (myTP->controlTPParent->task560Inputs->nextSyncCodelet), nullptr,
        (this->taskInputs->ab_darts560), (this->taskInputs->b_darts560),
        (this->taskInputs->in_darts560), (this->taskInputs->out_darts560),
        (this->taskInputs->W_darts560), (this->taskInputs->nW_darts560),
        (this->taskInputs->nWdn_darts560), (this->taskInputs->m_darts560));
}
void TP560::_checkInCodelets11542::fire(void)
{

    /*printing node 11542: NullStmt*/
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task560Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task560Inputs->nextSyncCodelet->decDep();
}
TP560::TP560(
    int in_numThreads, int in_mainCodeletID, TP24* in_TPParent, _task560Inputs* in_task560Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task560Inputs(in_task560Inputs)
    , TP561Ptr(nullptr)
    , TP561_alreadyLaunched(0)
    , checkInCodelets561(1, 1, this, this->mainCodeletID)
    , checkInCodelets11542(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets561.decDep();
}
TP560::~TP560() { delete (task560Inputs); }
/*TP26: TP_fft_unshuffle_2*/
void TP26::_checkInCodelets652::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    /*Init the vars for this region*/
    /*Printing conditional branch node 652: inlining: 0*/
    if (((this->inputsTPParent->b_darts26[this->getID()])
            - (this->inputsTPParent->a_darts26[this->getID()]))
        < 128) {
        myTP->checkInCodelets655[this->getID()].decDep();
    } else {
        /*elseRegion: DeclStmt 673*/
        myTP->checkInCodelets673[this->getID()].decDep();
    }
}
void TP26::_checkInCodelets655::fire(void)
{

    /*printing node 655: BinaryOperator*/
    (this->inputsTPParent->ip_darts26[this->getID()])
        = (this->inputsTPParent->in_darts26[this->getID()])
        + (this->inputsTPParent->a_darts26[this->getID()]) * 2;

    /*printing node 658: ForStmt*/
    {
        int* b = &(this->inputsTPParent->b_darts26[this->getLocalID()]);
        (void)b /*PRIVATE*/;
        int* i = &(this->inputsTPParent->i_darts26[this->getLocalID()]);
        (void)i /*PRIVATE*/;
        COMPLEX** ip = &(this->inputsTPParent->ip_darts26[this->getLocalID()]);
        (void)ip /*PRIVATE*/;
        COMPLEX** jp = &(this->inputsTPParent->jp_darts26[this->getLocalID()]);
        (void)jp /*PRIVATE*/;
        int* m = &(this->inputsTPParent->m_darts26[this->getLocalID()]);
        (void)m /*PRIVATE*/;
        COMPLEX** out = &(this->inputsTPParent->out_darts26[this->getLocalID()]);
        (void)out /*PRIVATE*/;
        /*Loop's init*/
        (this->inputsTPParent->i_darts26[this->getID()])
            = (this->inputsTPParent->a_darts26[this->getID()]);
        int i_darts_counter_temp26 = (this->inputsTPParent->i_darts26[this->getID()]);
        for (; (i_darts_counter_temp26) < (*b); ++(i_darts_counter_temp26)) {
            (*jp) = (*out) + (i_darts_counter_temp26);
            (*jp)[0] = (*ip)[0];
            (*jp)[(*m)] = (*ip)[1];
            (*ip) += 2;
        }
        (this->inputsTPParent->i_darts26[this->getID()]) = i_darts_counter_temp26;
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/

    myTP->controlTPParent->nextCodeletsfft_unshuffle_2[this->getID()]->decDep();
}
void TP26::_checkInCodelets673::fire(void)
{

    /*printing node 673: DeclStmt*/
    this->inputsTPParent->ab_darts26[this->getID()]
        = ((this->inputsTPParent->a_darts26[this->getID()])
              + (this->inputsTPParent->b_darts26[this->getID()]))
        / 2;
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 673 nextRegion: 676 */
    myTP->controlTPParent->checkInCodelets676[this->getID()].decDep();
}
void TP26::_checkInCodelets676::fire(void)
{

    /*printing node 676: OMPTaskDirective*/
    /*syncNode: OMPTaskwaitDirective 680*/
    /*Find the TP to which the nextRegion belongs*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->barrierCodelets680[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 676*/
    _task676Inputs* task676Inputs
        = new _task676Inputs(&((this->inputsTPParent->a_darts26[this->getID()])),
            &((this->inputsTPParent->ab_darts26[this->getID()])),
            &((this->inputsTPParent->in_darts26[this->getID()])),
            &((this->inputsTPParent->m_darts26[this->getID()])),
            &((this->inputsTPParent->out_darts26[this->getID()])), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task676Inputs[this->getID()] = task676Inputs;
    invoke<TP676>(myTP, 1, this->getID(), myTP, task676Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 676 nextRegion: 678 */
    myTP->controlTPParent->checkInCodelets678[this->getID()].decDep();
}
void TP26::_checkInCodelets678::fire(void)
{

    /*printing node 678: OMPTaskDirective*/
    /*syncNode: OMPTaskwaitDirective 680*/
    /*Find the TP to which the nextRegion belongs*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->barrierCodelets680[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 678*/
    _task678Inputs* task678Inputs
        = new _task678Inputs(&((this->inputsTPParent->ab_darts26[this->getID()])),
            &((this->inputsTPParent->b_darts26[this->getID()])),
            &((this->inputsTPParent->in_darts26[this->getID()])),
            &((this->inputsTPParent->m_darts26[this->getID()])),
            &((this->inputsTPParent->out_darts26[this->getID()])), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task678Inputs[this->getID()] = task678Inputs;
    invoke<TP678>(myTP, 1, this->getID(), myTP, task678Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 678 nextRegion: 680 */
    myTP->controlTPParent->barrierCodelets680[0].decDep();
}
void TP26::_barrierCodelets680::fire(void)
{
    TP26* myTP = static_cast<TP26*>(myTP_);

    for (size_t codeletsCounter = 0; codeletsCounter < (size_t)myTP->numThreads;
         codeletsCounter++) {
        myTP->nextCodeletsfft_unshuffle_2[codeletsCounter]->decDep();
    }
}
TP26::TP26(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_mainNextCodelet,
    darts::Codelet* in_mainSyncCodelet, TP26** in_ptrToThisFunctionTP, int in_a, int in_b,
    COMPLEX* in_in, COMPLEX* in_out, int in_m)
    : ompTP(in_numThreads, in_mainCodeletID)
    , ptrToThisFunctionTP(in_ptrToThisFunctionTP)
    , inputsTPParent(this)
    , controlTPParent(this)
    , nextCodeletsfft_unshuffle_2(new Codelet*[in_numThreads])
    , nextSyncCodeletsfft_unshuffle_2(new Codelet*[in_numThreads])
    , a_darts26(new int[this->numThreads])
    , b_darts26(new int[this->numThreads])
    , in_darts26(new COMPLEX*[this->numThreads])
    , out_darts26(new COMPLEX*[this->numThreads])
    , m_darts26(new int[this->numThreads])
    , ab_darts26(new int[this->numThreads])
    , i_darts26(new int[this->numThreads])
    , ip_darts26(new COMPLEX*[this->numThreads])
    , jp_darts26(new COMPLEX*[this->numThreads])
    , task676Inputs(new _task676Inputs*[this->numThreads])
    , task678Inputs(new _task678Inputs*[this->numThreads])
    , checkInCodelets652(new _checkInCodelets652[this->numThreads])
    , checkInCodelets655(new _checkInCodelets655[this->numThreads])
    , checkInCodelets673(new _checkInCodelets673[this->numThreads])
    , checkInCodelets676(new _checkInCodelets676[this->numThreads])
    , checkInCodelets678(new _checkInCodelets678[this->numThreads])
    , barrierCodelets680(new _barrierCodelets680[1])
{
    barrierCodelets680[0] = _barrierCodelets680(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets678* checkInCodelets678Ptr = (this->checkInCodelets678);
    _checkInCodelets676* checkInCodelets676Ptr = (this->checkInCodelets676);
    _checkInCodelets673* checkInCodelets673Ptr = (this->checkInCodelets673);
    _checkInCodelets655* checkInCodelets655Ptr = (this->checkInCodelets655);
    _checkInCodelets652* checkInCodelets652Ptr = (this->checkInCodelets652);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets652);
#endif
    for (size_t codeletCounter = 0; codeletCounter < this->numThreads; codeletCounter++) {
        (*checkInCodelets678Ptr) = _checkInCodelets678(1, 1, this, codeletCounter);
        checkInCodelets678Ptr++;
        (*checkInCodelets676Ptr) = _checkInCodelets676(1, 1, this, codeletCounter);
        checkInCodelets676Ptr++;
        (*checkInCodelets673Ptr) = _checkInCodelets673(1, 1, this, codeletCounter);
        checkInCodelets673Ptr++;
        (*checkInCodelets655Ptr) = _checkInCodelets655(1, 1, this, codeletCounter);
        checkInCodelets655Ptr++;
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets652Ptr) = _checkInCodelets652(2, 1, this, codeletCounter);
#else
        (*checkInCodelets652Ptr) = _checkInCodelets652(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets652Ptr).decDep();
        checkInCodelets652Ptr++;
    }
    if (this->numThreads == 1) {
        this->nextCodeletsfft_unshuffle_2[0] = in_mainNextCodelet;
        this->nextSyncCodeletsfft_unshuffle_2[0] = in_mainSyncCodelet;
        this->a_darts26[0] = in_a;
        this->b_darts26[0] = in_b;
        this->in_darts26[0] = in_in;
        this->out_darts26[0] = in_out;
        this->m_darts26[0] = in_m;
        this->availableCodelets[0] = 1;
    } else {
        this->a_darts26[this->mainCodeletID] = in_a;
        this->b_darts26[this->mainCodeletID] = in_b;
        this->in_darts26[this->mainCodeletID] = in_in;
        this->out_darts26[this->mainCodeletID] = in_out;
        this->m_darts26[this->mainCodeletID] = in_m;
        this->nextCodeletsfft_unshuffle_2[in_mainCodeletID] = in_mainNextCodelet;
        this->nextSyncCodeletsfft_unshuffle_2[in_mainCodeletID] = in_mainSyncCodelet;
#if USE_SPIN_CODELETS == 0
        this->firstCodelet[this->mainCodeletID].decDep();
#else
        this->availableCodelets[this->mainCodeletID] = 1;
#endif
        *(this->ptrToThisFunctionTP) = this;
    }
}
TP26::~TP26()
{
    delete[] task676Inputs;
    delete[] task678Inputs;
    delete[] barrierCodelets680;
    delete[] checkInCodelets678;
    delete[] checkInCodelets676;
    delete[] checkInCodelets673;
    delete[] checkInCodelets655;
    delete[] checkInCodelets652;
    delete[] nextCodeletsfft_unshuffle_2;
    delete[] nextSyncCodeletsfft_unshuffle_2;
    delete[] a_darts26;
    delete[] b_darts26;
    delete[] in_darts26;
    delete[] out_darts26;
    delete[] m_darts26;
    delete[] ab_darts26;
    delete[] i_darts26;
    delete[] ip_darts26;
    delete[] jp_darts26;
}
void TP26::setNewInputs(
    int in_a, int in_b, COMPLEX* in_in, COMPLEX* in_out, int in_m, size_t codeletID)
{
    this->a_darts26[codeletID] = in_a;
    this->b_darts26[codeletID] = in_b;
    this->in_darts26[codeletID] = in_in;
    this->out_darts26[codeletID] = in_out;
    this->m_darts26[codeletID] = in_m;
}
/*TP676: OMPTaskDirective*/
void TP676::_checkInCodelets677::fire(void)
{
/*Init the vars for this region*/

/*printing node 677: CallExpr*/
/*Finding the next codelet after the function call*/
#if 0
/*Signaling next codelet region: 677 nextRegion: 11545 */
myTP->controlTPParent->checkInCodelets11545.decDep();
#endif
/*Finding the next sync point before making the function call*/
#if 0
/*Find the sync point for a function call*/
/*syncNode: OMPTaskwaitDirective 680*/
/*No syncpoint found or outside task's boundaries, looking for syncCodelet*/
#endif
    /*Make the function call*/
    invoke<TP_fft_unshuffle_2>(myTP, myTP->numThreads, this->getID(),
        &(myTP->controlTPParent->checkInCodelets11545),
        (myTP->controlTPParent->task676Inputs->nextSyncCodelet), nullptr,
        (this->taskInputs->a_darts676), (this->taskInputs->ab_darts676),
        (this->taskInputs->in_darts676), (this->taskInputs->out_darts676),
        (this->taskInputs->m_darts676));
}
void TP676::_checkInCodelets11545::fire(void)
{

    /*printing node 11545: NullStmt*/
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task676Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task676Inputs->nextSyncCodelet->decDep();
}
TP676::TP676(
    int in_numThreads, int in_mainCodeletID, TP26* in_TPParent, _task676Inputs* in_task676Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task676Inputs(in_task676Inputs)
    , TP677Ptr(nullptr)
    , TP677_alreadyLaunched(0)
    , checkInCodelets677(1, 1, this, this->mainCodeletID)
    , checkInCodelets11545(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets677.decDep();
}
TP676::~TP676() { delete (task676Inputs); }
/*TP678: OMPTaskDirective*/
void TP678::_checkInCodelets679::fire(void)
{
/*Init the vars for this region*/

/*printing node 679: CallExpr*/
/*Finding the next codelet after the function call*/
#if 0
/*Signaling next codelet region: 679 nextRegion: 11546 */
myTP->controlTPParent->checkInCodelets11546.decDep();
#endif
/*Finding the next sync point before making the function call*/
#if 0
/*Find the sync point for a function call*/
/*syncNode: OMPTaskwaitDirective 680*/
/*No syncpoint found or outside task's boundaries, looking for syncCodelet*/
#endif
    /*Make the function call*/
    invoke<TP_fft_unshuffle_2>(myTP, myTP->numThreads, this->getID(),
        &(myTP->controlTPParent->checkInCodelets11546),
        (myTP->controlTPParent->task678Inputs->nextSyncCodelet), nullptr,
        (this->taskInputs->ab_darts678), (this->taskInputs->b_darts678),
        (this->taskInputs->in_darts678), (this->taskInputs->out_darts678),
        (this->taskInputs->m_darts678));
}
void TP678::_checkInCodelets11546::fire(void)
{

    /*printing node 11546: NullStmt*/
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task678Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task678Inputs->nextSyncCodelet->decDep();
}
TP678::TP678(
    int in_numThreads, int in_mainCodeletID, TP26* in_TPParent, _task678Inputs* in_task678Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task678Inputs(in_task678Inputs)
    , TP679Ptr(nullptr)
    , TP679_alreadyLaunched(0)
    , checkInCodelets679(1, 1, this, this->mainCodeletID)
    , checkInCodelets11546(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets679.decDep();
}
TP678::~TP678() { delete (task678Inputs); }
/*TP29: TP_fft_twiddle_4*/
void TP29::_checkInCodelets798::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    /*Init the vars for this region*/
    /*Printing conditional branch node 798: inlining: 0*/
    if (((this->inputsTPParent->b_darts29[this->getID()])
            - (this->inputsTPParent->a_darts29[this->getID()]))
        < 128) {
        myTP->checkInCodelets801[this->getID()].decDep();
    } else {
        /*elseRegion: DeclStmt 962*/
        myTP->checkInCodelets962[this->getID()].decDep();
    }
}
void TP29::_checkInCodelets801::fire(void)
{

    /*printing node 801: ForStmt*/
    {
        COMPLEX** W = &(this->inputsTPParent->W_darts29[this->getLocalID()]);
        (void)W /*PRIVATE*/;
        int* b = &(this->inputsTPParent->b_darts29[this->getLocalID()]);
        (void)b /*PRIVATE*/;
        int* i = &(this->inputsTPParent->i_darts29[this->getLocalID()]);
        (void)i /*PRIVATE*/;
        REAL* i1_0 = &(this->inputsTPParent->i1_0_darts29[this->getLocalID()]);
        (void)i1_0 /*PRIVATE*/;
        REAL* i1_1 = &(this->inputsTPParent->i1_1_darts29[this->getLocalID()]);
        (void)i1_1 /*PRIVATE*/;
        REAL* i1_2 = &(this->inputsTPParent->i1_2_darts29[this->getLocalID()]);
        (void)i1_2 /*PRIVATE*/;
        REAL* i1_3 = &(this->inputsTPParent->i1_3_darts29[this->getLocalID()]);
        (void)i1_3 /*PRIVATE*/;
        REAL* i2_0 = &(this->inputsTPParent->i2_0_darts29[this->getLocalID()]);
        (void)i2_0 /*PRIVATE*/;
        REAL* i2_1 = &(this->inputsTPParent->i2_1_darts29[this->getLocalID()]);
        (void)i2_1 /*PRIVATE*/;
        REAL* i2_2 = &(this->inputsTPParent->i2_2_darts29[this->getLocalID()]);
        (void)i2_2 /*PRIVATE*/;
        REAL* i2_3 = &(this->inputsTPParent->i2_3_darts29[this->getLocalID()]);
        (void)i2_3 /*PRIVATE*/;
        COMPLEX** in = &(this->inputsTPParent->in_darts29[this->getLocalID()]);
        (void)in /*PRIVATE*/;
        COMPLEX** jp = &(this->inputsTPParent->jp_darts29[this->getLocalID()]);
        (void)jp /*PRIVATE*/;
        COMPLEX** kp = &(this->inputsTPParent->kp_darts29[this->getLocalID()]);
        (void)kp /*PRIVATE*/;
        int* l1 = &(this->inputsTPParent->l1_darts29[this->getLocalID()]);
        (void)l1 /*PRIVATE*/;
        int* m = &(this->inputsTPParent->m_darts29[this->getLocalID()]);
        (void)m /*PRIVATE*/;
        int* nWdn = &(this->inputsTPParent->nWdn_darts29[this->getLocalID()]);
        (void)nWdn /*PRIVATE*/;
        COMPLEX** out = &(this->inputsTPParent->out_darts29[this->getLocalID()]);
        (void)out /*PRIVATE*/;
        REAL* r1_0 = &(this->inputsTPParent->r1_0_darts29[this->getLocalID()]);
        (void)r1_0 /*PRIVATE*/;
        REAL* r1_1 = &(this->inputsTPParent->r1_1_darts29[this->getLocalID()]);
        (void)r1_1 /*PRIVATE*/;
        REAL* r1_2 = &(this->inputsTPParent->r1_2_darts29[this->getLocalID()]);
        (void)r1_2 /*PRIVATE*/;
        REAL* r1_3 = &(this->inputsTPParent->r1_3_darts29[this->getLocalID()]);
        (void)r1_3 /*PRIVATE*/;
        REAL* r2_0 = &(this->inputsTPParent->r2_0_darts29[this->getLocalID()]);
        (void)r2_0 /*PRIVATE*/;
        REAL* r2_1 = &(this->inputsTPParent->r2_1_darts29[this->getLocalID()]);
        (void)r2_1 /*PRIVATE*/;
        REAL* r2_2 = &(this->inputsTPParent->r2_2_darts29[this->getLocalID()]);
        (void)r2_2 /*PRIVATE*/;
        REAL* r2_3 = &(this->inputsTPParent->r2_3_darts29[this->getLocalID()]);
        (void)r2_3 /*PRIVATE*/;
        REAL* tmpi = &(this->inputsTPParent->tmpi_darts29[this->getLocalID()]);
        (void)tmpi /*PRIVATE*/;
        REAL* tmpr = &(this->inputsTPParent->tmpr_darts29[this->getLocalID()]);
        (void)tmpr /*PRIVATE*/;
        REAL* wi = &(this->inputsTPParent->wi_darts29[this->getLocalID()]);
        (void)wi /*PRIVATE*/;
        REAL* wr = &(this->inputsTPParent->wr_darts29[this->getLocalID()]);
        (void)wr /*PRIVATE*/;
        /*Loop's init*/
        (this->inputsTPParent->i_darts29[this->getID()])
            = (this->inputsTPParent->a_darts29[this->getID()]);
        int i_darts_counter_temp29 = (this->inputsTPParent->i_darts29[this->getID()]);
        for (; (i_darts_counter_temp29) < (*b); (i_darts_counter_temp29)++) {
            (*l1) = (*nWdn) * (i_darts_counter_temp29);
            (*kp) = (*out) + (i_darts_counter_temp29);
            (*jp) = (*in) + (i_darts_counter_temp29);
            {
                {
                    (*r2_0) = (((*jp)[0 * (*m)]).re);
                    (*i2_0) = (((*jp)[0 * (*m)]).im);
                    (*wr) = (((*W)[2 * (*l1)]).re);
                    (*wi) = (((*W)[2 * (*l1)]).im);
                    (*tmpr) = (((*jp)[2 * (*m)]).re);
                    (*tmpi) = (((*jp)[2 * (*m)]).im);
                    (*r2_2) = (((*wr) * (*tmpr)) - ((*wi) * (*tmpi)));
                    (*i2_2) = (((*wi) * (*tmpr)) + ((*wr) * (*tmpi)));
                    (*r1_0) = ((*r2_0) + (*r2_2));
                    (*i1_0) = ((*i2_0) + (*i2_2));
                    (*r1_2) = ((*r2_0) - (*r2_2));
                    (*i1_2) = ((*i2_0) - (*i2_2));
                }
                {
                    (*wr) = (((*W)[1 * (*l1)]).re);
                    (*wi) = (((*W)[1 * (*l1)]).im);
                    (*tmpr) = (((*jp)[1 * (*m)]).re);
                    (*tmpi) = (((*jp)[1 * (*m)]).im);
                    (*r2_1) = (((*wr) * (*tmpr)) - ((*wi) * (*tmpi)));
                    (*i2_1) = (((*wi) * (*tmpr)) + ((*wr) * (*tmpi)));
                    (*wr) = (((*W)[3 * (*l1)]).re);
                    (*wi) = (((*W)[3 * (*l1)]).im);
                    (*tmpr) = (((*jp)[3 * (*m)]).re);
                    (*tmpi) = (((*jp)[3 * (*m)]).im);
                    (*r2_3) = (((*wr) * (*tmpr)) - ((*wi) * (*tmpi)));
                    (*i2_3) = (((*wi) * (*tmpr)) + ((*wr) * (*tmpi)));
                    (*r1_1) = ((*r2_1) + (*r2_3));
                    (*i1_1) = ((*i2_1) + (*i2_3));
                    (*r1_3) = ((*r2_1) - (*r2_3));
                    (*i1_3) = ((*i2_1) - (*i2_3));
                }
                (((*kp)[0 * (*m)]).re) = ((*r1_0) + (*r1_1));
                (((*kp)[0 * (*m)]).im) = ((*i1_0) + (*i1_1));
                (((*kp)[2 * (*m)]).re) = ((*r1_0) - (*r1_1));
                (((*kp)[2 * (*m)]).im) = ((*i1_0) - (*i1_1));
                (((*kp)[1 * (*m)]).re) = ((*r1_2) + (*i1_3));
                (((*kp)[1 * (*m)]).im) = ((*i1_2) - (*r1_3));
                (((*kp)[3 * (*m)]).re) = ((*r1_2) - (*i1_3));
                (((*kp)[3 * (*m)]).im) = ((*i1_2) + (*r1_3));
            }
            (*l1) += (*nWdn);
            (*kp)++;
        }
        (this->inputsTPParent->i_darts29[this->getID()]) = i_darts_counter_temp29;
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/

    myTP->controlTPParent->nextCodeletsfft_twiddle_4[this->getID()]->decDep();
}
void TP29::_checkInCodelets962::fire(void)
{

    /*printing node 962: DeclStmt*/
    this->inputsTPParent->ab_darts29[this->getID()]
        = ((this->inputsTPParent->a_darts29[this->getID()])
              + (this->inputsTPParent->b_darts29[this->getID()]))
        / 2;
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 962 nextRegion: 965 */
    myTP->controlTPParent->checkInCodelets965[this->getID()].decDep();
}
void TP29::_checkInCodelets965::fire(void)
{

    /*printing node 965: OMPTaskDirective*/
    /*syncNode: OMPTaskwaitDirective 969*/
    /*Find the TP to which the nextRegion belongs*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->barrierCodelets969[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 965*/
    _task965Inputs* task965Inputs
        = new _task965Inputs(&((this->inputsTPParent->W_darts29[this->getID()])),
            &((this->inputsTPParent->a_darts29[this->getID()])),
            &((this->inputsTPParent->ab_darts29[this->getID()])),
            &((this->inputsTPParent->in_darts29[this->getID()])),
            &((this->inputsTPParent->m_darts29[this->getID()])),
            &((this->inputsTPParent->nW_darts29[this->getID()])),
            &((this->inputsTPParent->nWdn_darts29[this->getID()])),
            &((this->inputsTPParent->out_darts29[this->getID()])), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task965Inputs[this->getID()] = task965Inputs;
    invoke<TP965>(myTP, 1, this->getID(), myTP, task965Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 965 nextRegion: 967 */
    myTP->controlTPParent->checkInCodelets967[this->getID()].decDep();
}
void TP29::_checkInCodelets967::fire(void)
{

    /*printing node 967: OMPTaskDirective*/
    /*syncNode: OMPTaskwaitDirective 969*/
    /*Find the TP to which the nextRegion belongs*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->barrierCodelets969[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 967*/
    _task967Inputs* task967Inputs
        = new _task967Inputs(&((this->inputsTPParent->W_darts29[this->getID()])),
            &((this->inputsTPParent->ab_darts29[this->getID()])),
            &((this->inputsTPParent->b_darts29[this->getID()])),
            &((this->inputsTPParent->in_darts29[this->getID()])),
            &((this->inputsTPParent->m_darts29[this->getID()])),
            &((this->inputsTPParent->nW_darts29[this->getID()])),
            &((this->inputsTPParent->nWdn_darts29[this->getID()])),
            &((this->inputsTPParent->out_darts29[this->getID()])), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task967Inputs[this->getID()] = task967Inputs;
    invoke<TP967>(myTP, 1, this->getID(), myTP, task967Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 967 nextRegion: 969 */
    myTP->controlTPParent->barrierCodelets969[0].decDep();
}
void TP29::_barrierCodelets969::fire(void)
{
    TP29* myTP = static_cast<TP29*>(myTP_);

    for (size_t codeletsCounter = 0; codeletsCounter < (size_t)myTP->numThreads;
         codeletsCounter++) {
        myTP->nextCodeletsfft_twiddle_4[codeletsCounter]->decDep();
    }
}
TP29::TP29(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_mainNextCodelet,
    darts::Codelet* in_mainSyncCodelet, TP29** in_ptrToThisFunctionTP, int in_a, int in_b,
    COMPLEX* in_in, COMPLEX* in_out, COMPLEX* in_W, int in_nW, int in_nWdn, int in_m)
    : ompTP(in_numThreads, in_mainCodeletID)
    , ptrToThisFunctionTP(in_ptrToThisFunctionTP)
    , inputsTPParent(this)
    , controlTPParent(this)
    , nextCodeletsfft_twiddle_4(new Codelet*[in_numThreads])
    , nextSyncCodeletsfft_twiddle_4(new Codelet*[in_numThreads])
    , a_darts29(new int[this->numThreads])
    , b_darts29(new int[this->numThreads])
    , in_darts29(new COMPLEX*[this->numThreads])
    , out_darts29(new COMPLEX*[this->numThreads])
    , W_darts29(new COMPLEX*[this->numThreads])
    , nW_darts29(new int[this->numThreads])
    , nWdn_darts29(new int[this->numThreads])
    , m_darts29(new int[this->numThreads])
    , ab_darts29(new int[this->numThreads])
    , i_darts29(new int[this->numThreads])
    , i1_0_darts29(new REAL[this->numThreads])
    , i1_1_darts29(new REAL[this->numThreads])
    , i1_2_darts29(new REAL[this->numThreads])
    , i1_3_darts29(new REAL[this->numThreads])
    , i2_0_darts29(new REAL[this->numThreads])
    , i2_1_darts29(new REAL[this->numThreads])
    , i2_2_darts29(new REAL[this->numThreads])
    , i2_3_darts29(new REAL[this->numThreads])
    , jp_darts29(new COMPLEX*[this->numThreads])
    , kp_darts29(new COMPLEX*[this->numThreads])
    , l1_darts29(new int[this->numThreads])
    , r1_0_darts29(new REAL[this->numThreads])
    , r1_1_darts29(new REAL[this->numThreads])
    , r1_2_darts29(new REAL[this->numThreads])
    , r1_3_darts29(new REAL[this->numThreads])
    , r2_0_darts29(new REAL[this->numThreads])
    , r2_1_darts29(new REAL[this->numThreads])
    , r2_2_darts29(new REAL[this->numThreads])
    , r2_3_darts29(new REAL[this->numThreads])
    , tmpi_darts29(new REAL[this->numThreads])
    , tmpr_darts29(new REAL[this->numThreads])
    , wi_darts29(new REAL[this->numThreads])
    , wr_darts29(new REAL[this->numThreads])
    , task965Inputs(new _task965Inputs*[this->numThreads])
    , task967Inputs(new _task967Inputs*[this->numThreads])
    , checkInCodelets798(new _checkInCodelets798[this->numThreads])
    , checkInCodelets801(new _checkInCodelets801[this->numThreads])
    , checkInCodelets962(new _checkInCodelets962[this->numThreads])
    , checkInCodelets965(new _checkInCodelets965[this->numThreads])
    , checkInCodelets967(new _checkInCodelets967[this->numThreads])
    , barrierCodelets969(new _barrierCodelets969[1])
{
    barrierCodelets969[0] = _barrierCodelets969(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets967* checkInCodelets967Ptr = (this->checkInCodelets967);
    _checkInCodelets965* checkInCodelets965Ptr = (this->checkInCodelets965);
    _checkInCodelets962* checkInCodelets962Ptr = (this->checkInCodelets962);
    _checkInCodelets801* checkInCodelets801Ptr = (this->checkInCodelets801);
    _checkInCodelets798* checkInCodelets798Ptr = (this->checkInCodelets798);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets798);
#endif
    for (size_t codeletCounter = 0; codeletCounter < this->numThreads; codeletCounter++) {
        (*checkInCodelets967Ptr) = _checkInCodelets967(1, 1, this, codeletCounter);
        checkInCodelets967Ptr++;
        (*checkInCodelets965Ptr) = _checkInCodelets965(1, 1, this, codeletCounter);
        checkInCodelets965Ptr++;
        (*checkInCodelets962Ptr) = _checkInCodelets962(1, 1, this, codeletCounter);
        checkInCodelets962Ptr++;
        (*checkInCodelets801Ptr) = _checkInCodelets801(1, 1, this, codeletCounter);
        checkInCodelets801Ptr++;
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets798Ptr) = _checkInCodelets798(2, 1, this, codeletCounter);
#else
        (*checkInCodelets798Ptr) = _checkInCodelets798(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets798Ptr).decDep();
        checkInCodelets798Ptr++;
    }
    if (this->numThreads == 1) {
        this->nextCodeletsfft_twiddle_4[0] = in_mainNextCodelet;
        this->nextSyncCodeletsfft_twiddle_4[0] = in_mainSyncCodelet;
        this->a_darts29[0] = in_a;
        this->b_darts29[0] = in_b;
        this->in_darts29[0] = in_in;
        this->out_darts29[0] = in_out;
        this->W_darts29[0] = in_W;
        this->nW_darts29[0] = in_nW;
        this->nWdn_darts29[0] = in_nWdn;
        this->m_darts29[0] = in_m;
        this->availableCodelets[0] = 1;
    } else {
        this->a_darts29[this->mainCodeletID] = in_a;
        this->b_darts29[this->mainCodeletID] = in_b;
        this->in_darts29[this->mainCodeletID] = in_in;
        this->out_darts29[this->mainCodeletID] = in_out;
        this->W_darts29[this->mainCodeletID] = in_W;
        this->nW_darts29[this->mainCodeletID] = in_nW;
        this->nWdn_darts29[this->mainCodeletID] = in_nWdn;
        this->m_darts29[this->mainCodeletID] = in_m;
        this->nextCodeletsfft_twiddle_4[in_mainCodeletID] = in_mainNextCodelet;
        this->nextSyncCodeletsfft_twiddle_4[in_mainCodeletID] = in_mainSyncCodelet;
#if USE_SPIN_CODELETS == 0
        this->firstCodelet[this->mainCodeletID].decDep();
#else
        this->availableCodelets[this->mainCodeletID] = 1;
#endif
        *(this->ptrToThisFunctionTP) = this;
    }
}
TP29::~TP29()
{
    delete[] task965Inputs;
    delete[] task967Inputs;
    delete[] barrierCodelets969;
    delete[] checkInCodelets967;
    delete[] checkInCodelets965;
    delete[] checkInCodelets962;
    delete[] checkInCodelets801;
    delete[] checkInCodelets798;
    delete[] nextCodeletsfft_twiddle_4;
    delete[] nextSyncCodeletsfft_twiddle_4;
    delete[] a_darts29;
    delete[] b_darts29;
    delete[] in_darts29;
    delete[] out_darts29;
    delete[] W_darts29;
    delete[] nW_darts29;
    delete[] nWdn_darts29;
    delete[] m_darts29;
    delete[] ab_darts29;
    delete[] i_darts29;
    delete[] i1_0_darts29;
    delete[] i1_1_darts29;
    delete[] i1_2_darts29;
    delete[] i1_3_darts29;
    delete[] i2_0_darts29;
    delete[] i2_1_darts29;
    delete[] i2_2_darts29;
    delete[] i2_3_darts29;
    delete[] jp_darts29;
    delete[] kp_darts29;
    delete[] l1_darts29;
    delete[] r1_0_darts29;
    delete[] r1_1_darts29;
    delete[] r1_2_darts29;
    delete[] r1_3_darts29;
    delete[] r2_0_darts29;
    delete[] r2_1_darts29;
    delete[] r2_2_darts29;
    delete[] r2_3_darts29;
    delete[] tmpi_darts29;
    delete[] tmpr_darts29;
    delete[] wi_darts29;
    delete[] wr_darts29;
}
void TP29::setNewInputs(int in_a, int in_b, COMPLEX* in_in, COMPLEX* in_out, COMPLEX* in_W,
    int in_nW, int in_nWdn, int in_m, size_t codeletID)
{
    this->a_darts29[codeletID] = in_a;
    this->b_darts29[codeletID] = in_b;
    this->in_darts29[codeletID] = in_in;
    this->out_darts29[codeletID] = in_out;
    this->W_darts29[codeletID] = in_W;
    this->nW_darts29[codeletID] = in_nW;
    this->nWdn_darts29[codeletID] = in_nWdn;
    this->m_darts29[codeletID] = in_m;
}
/*TP965: OMPTaskDirective*/
void TP965::_checkInCodelets966::fire(void)
{
/*Init the vars for this region*/

/*printing node 966: CallExpr*/
/*Finding the next codelet after the function call*/
#if 0
/*Signaling next codelet region: 966 nextRegion: 11543 */
myTP->controlTPParent->checkInCodelets11543.decDep();
#endif
/*Finding the next sync point before making the function call*/
#if 0
/*Find the sync point for a function call*/
/*syncNode: OMPTaskwaitDirective 969*/
/*No syncpoint found or outside task's boundaries, looking for syncCodelet*/
#endif
    /*Make the function call*/
    invoke<TP_fft_twiddle_4>(myTP, myTP->numThreads, this->getID(),
        &(myTP->controlTPParent->checkInCodelets11543),
        (myTP->controlTPParent->task965Inputs->nextSyncCodelet), nullptr,
        (this->taskInputs->a_darts965), (this->taskInputs->ab_darts965),
        (this->taskInputs->in_darts965), (this->taskInputs->out_darts965),
        (this->taskInputs->W_darts965), (this->taskInputs->nW_darts965),
        (this->taskInputs->nWdn_darts965), (this->taskInputs->m_darts965));
}
void TP965::_checkInCodelets11543::fire(void)
{

    /*printing node 11543: NullStmt*/
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task965Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task965Inputs->nextSyncCodelet->decDep();
}
TP965::TP965(
    int in_numThreads, int in_mainCodeletID, TP29* in_TPParent, _task965Inputs* in_task965Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task965Inputs(in_task965Inputs)
    , TP966Ptr(nullptr)
    , TP966_alreadyLaunched(0)
    , checkInCodelets966(1, 1, this, this->mainCodeletID)
    , checkInCodelets11543(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets966.decDep();
}
TP965::~TP965() { delete (task965Inputs); }
/*TP967: OMPTaskDirective*/
void TP967::_checkInCodelets968::fire(void)
{
/*Init the vars for this region*/

/*printing node 968: CallExpr*/
/*Finding the next codelet after the function call*/
#if 0
/*Signaling next codelet region: 968 nextRegion: 11544 */
myTP->controlTPParent->checkInCodelets11544.decDep();
#endif
/*Finding the next sync point before making the function call*/
#if 0
/*Find the sync point for a function call*/
/*syncNode: OMPTaskwaitDirective 969*/
/*No syncpoint found or outside task's boundaries, looking for syncCodelet*/
#endif
    /*Make the function call*/
    invoke<TP_fft_twiddle_4>(myTP, myTP->numThreads, this->getID(),
        &(myTP->controlTPParent->checkInCodelets11544),
        (myTP->controlTPParent->task967Inputs->nextSyncCodelet), nullptr,
        (this->taskInputs->ab_darts967), (this->taskInputs->b_darts967),
        (this->taskInputs->in_darts967), (this->taskInputs->out_darts967),
        (this->taskInputs->W_darts967), (this->taskInputs->nW_darts967),
        (this->taskInputs->nWdn_darts967), (this->taskInputs->m_darts967));
}
void TP967::_checkInCodelets11544::fire(void)
{

    /*printing node 11544: NullStmt*/
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task967Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task967Inputs->nextSyncCodelet->decDep();
}
TP967::TP967(
    int in_numThreads, int in_mainCodeletID, TP29* in_TPParent, _task967Inputs* in_task967Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task967Inputs(in_task967Inputs)
    , TP968Ptr(nullptr)
    , TP968_alreadyLaunched(0)
    , checkInCodelets968(1, 1, this, this->mainCodeletID)
    , checkInCodelets11544(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets968.decDep();
}
TP967::~TP967() { delete (task967Inputs); }
/*TP31: TP_fft_unshuffle_4*/
void TP31::_checkInCodelets1151::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    /*Init the vars for this region*/
    /*Printing conditional branch node 1151: inlining: 0*/
    if (((this->inputsTPParent->b_darts31[this->getID()])
            - (this->inputsTPParent->a_darts31[this->getID()]))
        < 128) {
        myTP->checkInCodelets1154[this->getID()].decDep();
    } else {
        /*elseRegion: DeclStmt 1181*/
        myTP->checkInCodelets1181[this->getID()].decDep();
    }
}
void TP31::_checkInCodelets1154::fire(void)
{

    /*printing node 1154: BinaryOperator*/
    (this->inputsTPParent->ip_darts31[this->getID()])
        = (this->inputsTPParent->in_darts31[this->getID()])
        + (this->inputsTPParent->a_darts31[this->getID()]) * 4;

    /*printing node 1157: ForStmt*/
    {
        int* b = &(this->inputsTPParent->b_darts31[this->getLocalID()]);
        (void)b /*PRIVATE*/;
        int* i = &(this->inputsTPParent->i_darts31[this->getLocalID()]);
        (void)i /*PRIVATE*/;
        COMPLEX** ip = &(this->inputsTPParent->ip_darts31[this->getLocalID()]);
        (void)ip /*PRIVATE*/;
        COMPLEX** jp = &(this->inputsTPParent->jp_darts31[this->getLocalID()]);
        (void)jp /*PRIVATE*/;
        int* m = &(this->inputsTPParent->m_darts31[this->getLocalID()]);
        (void)m /*PRIVATE*/;
        COMPLEX** out = &(this->inputsTPParent->out_darts31[this->getLocalID()]);
        (void)out /*PRIVATE*/;
        /*Loop's init*/
        (this->inputsTPParent->i_darts31[this->getID()])
            = (this->inputsTPParent->a_darts31[this->getID()]);
        int i_darts_counter_temp31 = (this->inputsTPParent->i_darts31[this->getID()]);
        for (; (i_darts_counter_temp31) < (*b); ++(i_darts_counter_temp31)) {
            (*jp) = (*out) + (i_darts_counter_temp31);
            (*jp)[0] = (*ip)[0];
            (*jp)[(*m)] = (*ip)[1];
            (*ip) += 2;
            (*jp) += 2 * (*m);
            (*jp)[0] = (*ip)[0];
            (*jp)[(*m)] = (*ip)[1];
            (*ip) += 2;
        }
        (this->inputsTPParent->i_darts31[this->getID()]) = i_darts_counter_temp31;
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/

    myTP->controlTPParent->nextCodeletsfft_unshuffle_4[this->getID()]->decDep();
}
void TP31::_checkInCodelets1181::fire(void)
{

    /*printing node 1181: DeclStmt*/
    this->inputsTPParent->ab_darts31[this->getID()]
        = ((this->inputsTPParent->a_darts31[this->getID()])
              + (this->inputsTPParent->b_darts31[this->getID()]))
        / 2;
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 1181 nextRegion: 1184 */
    myTP->controlTPParent->checkInCodelets1184[this->getID()].decDep();
}
void TP31::_checkInCodelets1184::fire(void)
{

    /*printing node 1184: OMPTaskDirective*/
    /*syncNode: OMPTaskwaitDirective 1188*/
    /*Find the TP to which the nextRegion belongs*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->barrierCodelets1188[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 1184*/
    _task1184Inputs* task1184Inputs
        = new _task1184Inputs(&((this->inputsTPParent->a_darts31[this->getID()])),
            &((this->inputsTPParent->ab_darts31[this->getID()])),
            &((this->inputsTPParent->in_darts31[this->getID()])),
            &((this->inputsTPParent->m_darts31[this->getID()])),
            &((this->inputsTPParent->out_darts31[this->getID()])), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task1184Inputs[this->getID()] = task1184Inputs;
    invoke<TP1184>(myTP, 1, this->getID(), myTP, task1184Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 1184 nextRegion: 1186 */
    myTP->controlTPParent->checkInCodelets1186[this->getID()].decDep();
}
void TP31::_checkInCodelets1186::fire(void)
{

    /*printing node 1186: OMPTaskDirective*/
    /*syncNode: OMPTaskwaitDirective 1188*/
    /*Find the TP to which the nextRegion belongs*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->barrierCodelets1188[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 1186*/
    _task1186Inputs* task1186Inputs
        = new _task1186Inputs(&((this->inputsTPParent->ab_darts31[this->getID()])),
            &((this->inputsTPParent->b_darts31[this->getID()])),
            &((this->inputsTPParent->in_darts31[this->getID()])),
            &((this->inputsTPParent->m_darts31[this->getID()])),
            &((this->inputsTPParent->out_darts31[this->getID()])), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task1186Inputs[this->getID()] = task1186Inputs;
    invoke<TP1186>(myTP, 1, this->getID(), myTP, task1186Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 1186 nextRegion: 1188 */
    myTP->controlTPParent->barrierCodelets1188[0].decDep();
}
void TP31::_barrierCodelets1188::fire(void)
{
    TP31* myTP = static_cast<TP31*>(myTP_);

    for (size_t codeletsCounter = 0; codeletsCounter < (size_t)myTP->numThreads;
         codeletsCounter++) {
        myTP->nextCodeletsfft_unshuffle_4[codeletsCounter]->decDep();
    }
}
TP31::TP31(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_mainNextCodelet,
    darts::Codelet* in_mainSyncCodelet, TP31** in_ptrToThisFunctionTP, int in_a, int in_b,
    COMPLEX* in_in, COMPLEX* in_out, int in_m)
    : ompTP(in_numThreads, in_mainCodeletID)
    , ptrToThisFunctionTP(in_ptrToThisFunctionTP)
    , inputsTPParent(this)
    , controlTPParent(this)
    , nextCodeletsfft_unshuffle_4(new Codelet*[in_numThreads])
    , nextSyncCodeletsfft_unshuffle_4(new Codelet*[in_numThreads])
    , a_darts31(new int[this->numThreads])
    , b_darts31(new int[this->numThreads])
    , in_darts31(new COMPLEX*[this->numThreads])
    , out_darts31(new COMPLEX*[this->numThreads])
    , m_darts31(new int[this->numThreads])
    , ab_darts31(new int[this->numThreads])
    , i_darts31(new int[this->numThreads])
    , ip_darts31(new COMPLEX*[this->numThreads])
    , jp_darts31(new COMPLEX*[this->numThreads])
    , task1184Inputs(new _task1184Inputs*[this->numThreads])
    , task1186Inputs(new _task1186Inputs*[this->numThreads])
    , checkInCodelets1151(new _checkInCodelets1151[this->numThreads])
    , checkInCodelets1154(new _checkInCodelets1154[this->numThreads])
    , checkInCodelets1181(new _checkInCodelets1181[this->numThreads])
    , checkInCodelets1184(new _checkInCodelets1184[this->numThreads])
    , checkInCodelets1186(new _checkInCodelets1186[this->numThreads])
    , barrierCodelets1188(new _barrierCodelets1188[1])
{
    barrierCodelets1188[0] = _barrierCodelets1188(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets1186* checkInCodelets1186Ptr = (this->checkInCodelets1186);
    _checkInCodelets1184* checkInCodelets1184Ptr = (this->checkInCodelets1184);
    _checkInCodelets1181* checkInCodelets1181Ptr = (this->checkInCodelets1181);
    _checkInCodelets1154* checkInCodelets1154Ptr = (this->checkInCodelets1154);
    _checkInCodelets1151* checkInCodelets1151Ptr = (this->checkInCodelets1151);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets1151);
#endif
    for (size_t codeletCounter = 0; codeletCounter < this->numThreads; codeletCounter++) {
        (*checkInCodelets1186Ptr) = _checkInCodelets1186(1, 1, this, codeletCounter);
        checkInCodelets1186Ptr++;
        (*checkInCodelets1184Ptr) = _checkInCodelets1184(1, 1, this, codeletCounter);
        checkInCodelets1184Ptr++;
        (*checkInCodelets1181Ptr) = _checkInCodelets1181(1, 1, this, codeletCounter);
        checkInCodelets1181Ptr++;
        (*checkInCodelets1154Ptr) = _checkInCodelets1154(1, 1, this, codeletCounter);
        checkInCodelets1154Ptr++;
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets1151Ptr) = _checkInCodelets1151(2, 1, this, codeletCounter);
#else
        (*checkInCodelets1151Ptr) = _checkInCodelets1151(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets1151Ptr).decDep();
        checkInCodelets1151Ptr++;
    }
    if (this->numThreads == 1) {
        this->nextCodeletsfft_unshuffle_4[0] = in_mainNextCodelet;
        this->nextSyncCodeletsfft_unshuffle_4[0] = in_mainSyncCodelet;
        this->a_darts31[0] = in_a;
        this->b_darts31[0] = in_b;
        this->in_darts31[0] = in_in;
        this->out_darts31[0] = in_out;
        this->m_darts31[0] = in_m;
        this->availableCodelets[0] = 1;
    } else {
        this->a_darts31[this->mainCodeletID] = in_a;
        this->b_darts31[this->mainCodeletID] = in_b;
        this->in_darts31[this->mainCodeletID] = in_in;
        this->out_darts31[this->mainCodeletID] = in_out;
        this->m_darts31[this->mainCodeletID] = in_m;
        this->nextCodeletsfft_unshuffle_4[in_mainCodeletID] = in_mainNextCodelet;
        this->nextSyncCodeletsfft_unshuffle_4[in_mainCodeletID] = in_mainSyncCodelet;
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
    delete[] task1184Inputs;
    delete[] task1186Inputs;
    delete[] barrierCodelets1188;
    delete[] checkInCodelets1186;
    delete[] checkInCodelets1184;
    delete[] checkInCodelets1181;
    delete[] checkInCodelets1154;
    delete[] checkInCodelets1151;
    delete[] nextCodeletsfft_unshuffle_4;
    delete[] nextSyncCodeletsfft_unshuffle_4;
    delete[] a_darts31;
    delete[] b_darts31;
    delete[] in_darts31;
    delete[] out_darts31;
    delete[] m_darts31;
    delete[] ab_darts31;
    delete[] i_darts31;
    delete[] ip_darts31;
    delete[] jp_darts31;
}
void TP31::setNewInputs(
    int in_a, int in_b, COMPLEX* in_in, COMPLEX* in_out, int in_m, size_t codeletID)
{
    this->a_darts31[codeletID] = in_a;
    this->b_darts31[codeletID] = in_b;
    this->in_darts31[codeletID] = in_in;
    this->out_darts31[codeletID] = in_out;
    this->m_darts31[codeletID] = in_m;
}
/*TP1184: OMPTaskDirective*/
void TP1184::_checkInCodelets1185::fire(void)
{
/*Init the vars for this region*/

/*printing node 1185: CallExpr*/
/*Finding the next codelet after the function call*/
#if 0
/*Signaling next codelet region: 1185 nextRegion: 11547 */
myTP->controlTPParent->checkInCodelets11547.decDep();
#endif
/*Finding the next sync point before making the function call*/
#if 0
/*Find the sync point for a function call*/
/*syncNode: OMPTaskwaitDirective 1188*/
/*No syncpoint found or outside task's boundaries, looking for syncCodelet*/
#endif
    /*Make the function call*/
    invoke<TP_fft_unshuffle_4>(myTP, myTP->numThreads, this->getID(),
        &(myTP->controlTPParent->checkInCodelets11547),
        (myTP->controlTPParent->task1184Inputs->nextSyncCodelet), nullptr,
        (this->taskInputs->a_darts1184), (this->taskInputs->ab_darts1184),
        (this->taskInputs->in_darts1184), (this->taskInputs->out_darts1184),
        (this->taskInputs->m_darts1184));
}
void TP1184::_checkInCodelets11547::fire(void)
{

    /*printing node 11547: NullStmt*/
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task1184Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task1184Inputs->nextSyncCodelet->decDep();
}
TP1184::TP1184(
    int in_numThreads, int in_mainCodeletID, TP31* in_TPParent, _task1184Inputs* in_task1184Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task1184Inputs(in_task1184Inputs)
    , TP1185Ptr(nullptr)
    , TP1185_alreadyLaunched(0)
    , checkInCodelets1185(1, 1, this, this->mainCodeletID)
    , checkInCodelets11547(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets1185.decDep();
}
TP1184::~TP1184() { delete (task1184Inputs); }
/*TP1186: OMPTaskDirective*/
void TP1186::_checkInCodelets1187::fire(void)
{
/*Init the vars for this region*/

/*printing node 1187: CallExpr*/
/*Finding the next codelet after the function call*/
#if 0
/*Signaling next codelet region: 1187 nextRegion: 11548 */
myTP->controlTPParent->checkInCodelets11548.decDep();
#endif
/*Finding the next sync point before making the function call*/
#if 0
/*Find the sync point for a function call*/
/*syncNode: OMPTaskwaitDirective 1188*/
/*No syncpoint found or outside task's boundaries, looking for syncCodelet*/
#endif
    /*Make the function call*/
    invoke<TP_fft_unshuffle_4>(myTP, myTP->numThreads, this->getID(),
        &(myTP->controlTPParent->checkInCodelets11548),
        (myTP->controlTPParent->task1186Inputs->nextSyncCodelet), nullptr,
        (this->taskInputs->ab_darts1186), (this->taskInputs->b_darts1186),
        (this->taskInputs->in_darts1186), (this->taskInputs->out_darts1186),
        (this->taskInputs->m_darts1186));
}
void TP1186::_checkInCodelets11548::fire(void)
{

    /*printing node 11548: NullStmt*/
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task1186Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task1186Inputs->nextSyncCodelet->decDep();
}
TP1186::TP1186(
    int in_numThreads, int in_mainCodeletID, TP31* in_TPParent, _task1186Inputs* in_task1186Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task1186Inputs(in_task1186Inputs)
    , TP1187Ptr(nullptr)
    , TP1187_alreadyLaunched(0)
    , checkInCodelets1187(1, 1, this, this->mainCodeletID)
    , checkInCodelets11548(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets1187.decDep();
}
TP1186::~TP1186() { delete (task1186Inputs); }
/*TP34: TP_fft_twiddle_8*/
void TP34::_checkInCodelets1457::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    /*Init the vars for this region*/
    /*Printing conditional branch node 1457: inlining: 0*/
    if (((this->inputsTPParent->b_darts34[this->getID()])
            - (this->inputsTPParent->a_darts34[this->getID()]))
        < 128) {
        myTP->checkInCodelets1460[this->getID()].decDep();
    } else {
        /*elseRegion: DeclStmt 1841*/
        myTP->checkInCodelets1841[this->getID()].decDep();
    }
}
void TP34::_checkInCodelets1460::fire(void)
{

    /*printing node 1460: ForStmt*/
    {
        COMPLEX** W = &(this->inputsTPParent->W_darts34[this->getLocalID()]);
        (void)W /*PRIVATE*/;
        int* b = &(this->inputsTPParent->b_darts34[this->getLocalID()]);
        (void)b /*PRIVATE*/;
        int* i = &(this->inputsTPParent->i_darts34[this->getLocalID()]);
        (void)i /*PRIVATE*/;
        REAL* i1_0 = &(this->inputsTPParent->i1_0_darts34[this->getLocalID()]);
        (void)i1_0 /*PRIVATE*/;
        REAL* i1_1 = &(this->inputsTPParent->i1_1_darts34[this->getLocalID()]);
        (void)i1_1 /*PRIVATE*/;
        REAL* i1_2 = &(this->inputsTPParent->i1_2_darts34[this->getLocalID()]);
        (void)i1_2 /*PRIVATE*/;
        REAL* i1_3 = &(this->inputsTPParent->i1_3_darts34[this->getLocalID()]);
        (void)i1_3 /*PRIVATE*/;
        REAL* i1_4 = &(this->inputsTPParent->i1_4_darts34[this->getLocalID()]);
        (void)i1_4 /*PRIVATE*/;
        REAL* i1_5 = &(this->inputsTPParent->i1_5_darts34[this->getLocalID()]);
        (void)i1_5 /*PRIVATE*/;
        REAL* i1_6 = &(this->inputsTPParent->i1_6_darts34[this->getLocalID()]);
        (void)i1_6 /*PRIVATE*/;
        REAL* i1_7 = &(this->inputsTPParent->i1_7_darts34[this->getLocalID()]);
        (void)i1_7 /*PRIVATE*/;
        REAL* i2_0 = &(this->inputsTPParent->i2_0_darts34[this->getLocalID()]);
        (void)i2_0 /*PRIVATE*/;
        REAL* i2_1 = &(this->inputsTPParent->i2_1_darts34[this->getLocalID()]);
        (void)i2_1 /*PRIVATE*/;
        REAL* i2_2 = &(this->inputsTPParent->i2_2_darts34[this->getLocalID()]);
        (void)i2_2 /*PRIVATE*/;
        REAL* i2_3 = &(this->inputsTPParent->i2_3_darts34[this->getLocalID()]);
        (void)i2_3 /*PRIVATE*/;
        REAL* i2_4 = &(this->inputsTPParent->i2_4_darts34[this->getLocalID()]);
        (void)i2_4 /*PRIVATE*/;
        REAL* i2_5 = &(this->inputsTPParent->i2_5_darts34[this->getLocalID()]);
        (void)i2_5 /*PRIVATE*/;
        REAL* i2_6 = &(this->inputsTPParent->i2_6_darts34[this->getLocalID()]);
        (void)i2_6 /*PRIVATE*/;
        REAL* i2_7 = &(this->inputsTPParent->i2_7_darts34[this->getLocalID()]);
        (void)i2_7 /*PRIVATE*/;
        REAL* i3_0 = &(this->inputsTPParent->i3_0_darts34[this->getLocalID()]);
        (void)i3_0 /*PRIVATE*/;
        REAL* i3_1 = &(this->inputsTPParent->i3_1_darts34[this->getLocalID()]);
        (void)i3_1 /*PRIVATE*/;
        REAL* i3_2 = &(this->inputsTPParent->i3_2_darts34[this->getLocalID()]);
        (void)i3_2 /*PRIVATE*/;
        REAL* i3_3 = &(this->inputsTPParent->i3_3_darts34[this->getLocalID()]);
        (void)i3_3 /*PRIVATE*/;
        REAL* i3_4 = &(this->inputsTPParent->i3_4_darts34[this->getLocalID()]);
        (void)i3_4 /*PRIVATE*/;
        REAL* i3_5 = &(this->inputsTPParent->i3_5_darts34[this->getLocalID()]);
        (void)i3_5 /*PRIVATE*/;
        REAL* i3_6 = &(this->inputsTPParent->i3_6_darts34[this->getLocalID()]);
        (void)i3_6 /*PRIVATE*/;
        REAL* i3_7 = &(this->inputsTPParent->i3_7_darts34[this->getLocalID()]);
        (void)i3_7 /*PRIVATE*/;
        COMPLEX** in = &(this->inputsTPParent->in_darts34[this->getLocalID()]);
        (void)in /*PRIVATE*/;
        COMPLEX** jp = &(this->inputsTPParent->jp_darts34[this->getLocalID()]);
        (void)jp /*PRIVATE*/;
        COMPLEX** kp = &(this->inputsTPParent->kp_darts34[this->getLocalID()]);
        (void)kp /*PRIVATE*/;
        int* l1 = &(this->inputsTPParent->l1_darts34[this->getLocalID()]);
        (void)l1 /*PRIVATE*/;
        int* m = &(this->inputsTPParent->m_darts34[this->getLocalID()]);
        (void)m /*PRIVATE*/;
        int* nWdn = &(this->inputsTPParent->nWdn_darts34[this->getLocalID()]);
        (void)nWdn /*PRIVATE*/;
        COMPLEX** out = &(this->inputsTPParent->out_darts34[this->getLocalID()]);
        (void)out /*PRIVATE*/;
        REAL* r1_0 = &(this->inputsTPParent->r1_0_darts34[this->getLocalID()]);
        (void)r1_0 /*PRIVATE*/;
        REAL* r1_1 = &(this->inputsTPParent->r1_1_darts34[this->getLocalID()]);
        (void)r1_1 /*PRIVATE*/;
        REAL* r1_2 = &(this->inputsTPParent->r1_2_darts34[this->getLocalID()]);
        (void)r1_2 /*PRIVATE*/;
        REAL* r1_3 = &(this->inputsTPParent->r1_3_darts34[this->getLocalID()]);
        (void)r1_3 /*PRIVATE*/;
        REAL* r1_4 = &(this->inputsTPParent->r1_4_darts34[this->getLocalID()]);
        (void)r1_4 /*PRIVATE*/;
        REAL* r1_5 = &(this->inputsTPParent->r1_5_darts34[this->getLocalID()]);
        (void)r1_5 /*PRIVATE*/;
        REAL* r1_6 = &(this->inputsTPParent->r1_6_darts34[this->getLocalID()]);
        (void)r1_6 /*PRIVATE*/;
        REAL* r1_7 = &(this->inputsTPParent->r1_7_darts34[this->getLocalID()]);
        (void)r1_7 /*PRIVATE*/;
        REAL* r2_0 = &(this->inputsTPParent->r2_0_darts34[this->getLocalID()]);
        (void)r2_0 /*PRIVATE*/;
        REAL* r2_1 = &(this->inputsTPParent->r2_1_darts34[this->getLocalID()]);
        (void)r2_1 /*PRIVATE*/;
        REAL* r2_2 = &(this->inputsTPParent->r2_2_darts34[this->getLocalID()]);
        (void)r2_2 /*PRIVATE*/;
        REAL* r2_3 = &(this->inputsTPParent->r2_3_darts34[this->getLocalID()]);
        (void)r2_3 /*PRIVATE*/;
        REAL* r2_4 = &(this->inputsTPParent->r2_4_darts34[this->getLocalID()]);
        (void)r2_4 /*PRIVATE*/;
        REAL* r2_5 = &(this->inputsTPParent->r2_5_darts34[this->getLocalID()]);
        (void)r2_5 /*PRIVATE*/;
        REAL* r2_6 = &(this->inputsTPParent->r2_6_darts34[this->getLocalID()]);
        (void)r2_6 /*PRIVATE*/;
        REAL* r2_7 = &(this->inputsTPParent->r2_7_darts34[this->getLocalID()]);
        (void)r2_7 /*PRIVATE*/;
        REAL* r3_0 = &(this->inputsTPParent->r3_0_darts34[this->getLocalID()]);
        (void)r3_0 /*PRIVATE*/;
        REAL* r3_1 = &(this->inputsTPParent->r3_1_darts34[this->getLocalID()]);
        (void)r3_1 /*PRIVATE*/;
        REAL* r3_2 = &(this->inputsTPParent->r3_2_darts34[this->getLocalID()]);
        (void)r3_2 /*PRIVATE*/;
        REAL* r3_3 = &(this->inputsTPParent->r3_3_darts34[this->getLocalID()]);
        (void)r3_3 /*PRIVATE*/;
        REAL* r3_4 = &(this->inputsTPParent->r3_4_darts34[this->getLocalID()]);
        (void)r3_4 /*PRIVATE*/;
        REAL* r3_5 = &(this->inputsTPParent->r3_5_darts34[this->getLocalID()]);
        (void)r3_5 /*PRIVATE*/;
        REAL* r3_6 = &(this->inputsTPParent->r3_6_darts34[this->getLocalID()]);
        (void)r3_6 /*PRIVATE*/;
        REAL* r3_7 = &(this->inputsTPParent->r3_7_darts34[this->getLocalID()]);
        (void)r3_7 /*PRIVATE*/;
        REAL* tmpi = &(this->inputsTPParent->tmpi_darts34[this->getLocalID()]);
        (void)tmpi /*PRIVATE*/;
        REAL* tmpr = &(this->inputsTPParent->tmpr_darts34[this->getLocalID()]);
        (void)tmpr /*PRIVATE*/;
        REAL* wi = &(this->inputsTPParent->wi_darts34[this->getLocalID()]);
        (void)wi /*PRIVATE*/;
        REAL* wr = &(this->inputsTPParent->wr_darts34[this->getLocalID()]);
        (void)wr /*PRIVATE*/;
        /*Loop's init*/
        (this->inputsTPParent->i_darts34[this->getID()])
            = (this->inputsTPParent->a_darts34[this->getID()]);
        int i_darts_counter_temp34 = (this->inputsTPParent->i_darts34[this->getID()]);
        for (; (i_darts_counter_temp34) < (*b); (i_darts_counter_temp34)++) {
            (*l1) = (*nWdn) * (i_darts_counter_temp34);
            (*kp) = (*out) + (i_darts_counter_temp34);
            (*jp) = (*in) + (i_darts_counter_temp34);
            {
                {
                    {
                        (*r3_0) = (((*jp)[0 * (*m)]).re);
                        (*i3_0) = (((*jp)[0 * (*m)]).im);
                        (*wr) = (((*W)[4 * (*l1)]).re);
                        (*wi) = (((*W)[4 * (*l1)]).im);
                        (*tmpr) = (((*jp)[4 * (*m)]).re);
                        (*tmpi) = (((*jp)[4 * (*m)]).im);
                        (*r3_4) = (((*wr) * (*tmpr)) - ((*wi) * (*tmpi)));
                        (*i3_4) = (((*wi) * (*tmpr)) + ((*wr) * (*tmpi)));
                        (*r2_0) = ((*r3_0) + (*r3_4));
                        (*i2_0) = ((*i3_0) + (*i3_4));
                        (*r2_4) = ((*r3_0) - (*r3_4));
                        (*i2_4) = ((*i3_0) - (*i3_4));
                    }
                    {
                        (*wr) = (((*W)[2 * (*l1)]).re);
                        (*wi) = (((*W)[2 * (*l1)]).im);
                        (*tmpr) = (((*jp)[2 * (*m)]).re);
                        (*tmpi) = (((*jp)[2 * (*m)]).im);
                        (*r3_2) = (((*wr) * (*tmpr)) - ((*wi) * (*tmpi)));
                        (*i3_2) = (((*wi) * (*tmpr)) + ((*wr) * (*tmpi)));
                        (*wr) = (((*W)[6 * (*l1)]).re);
                        (*wi) = (((*W)[6 * (*l1)]).im);
                        (*tmpr) = (((*jp)[6 * (*m)]).re);
                        (*tmpi) = (((*jp)[6 * (*m)]).im);
                        (*r3_6) = (((*wr) * (*tmpr)) - ((*wi) * (*tmpi)));
                        (*i3_6) = (((*wi) * (*tmpr)) + ((*wr) * (*tmpi)));
                        (*r2_2) = ((*r3_2) + (*r3_6));
                        (*i2_2) = ((*i3_2) + (*i3_6));
                        (*r2_6) = ((*r3_2) - (*r3_6));
                        (*i2_6) = ((*i3_2) - (*i3_6));
                    }
                    (*r1_0) = ((*r2_0) + (*r2_2));
                    (*i1_0) = ((*i2_0) + (*i2_2));
                    (*r1_4) = ((*r2_0) - (*r2_2));
                    (*i1_4) = ((*i2_0) - (*i2_2));
                    (*r1_2) = ((*r2_4) + (*i2_6));
                    (*i1_2) = ((*i2_4) - (*r2_6));
                    (*r1_6) = ((*r2_4) - (*i2_6));
                    (*i1_6) = ((*i2_4) + (*r2_6));
                }
                {
                    {
                        (*wr) = (((*W)[1 * (*l1)]).re);
                        (*wi) = (((*W)[1 * (*l1)]).im);
                        (*tmpr) = (((*jp)[1 * (*m)]).re);
                        (*tmpi) = (((*jp)[1 * (*m)]).im);
                        (*r3_1) = (((*wr) * (*tmpr)) - ((*wi) * (*tmpi)));
                        (*i3_1) = (((*wi) * (*tmpr)) + ((*wr) * (*tmpi)));
                        (*wr) = (((*W)[5 * (*l1)]).re);
                        (*wi) = (((*W)[5 * (*l1)]).im);
                        (*tmpr) = (((*jp)[5 * (*m)]).re);
                        (*tmpi) = (((*jp)[5 * (*m)]).im);
                        (*r3_5) = (((*wr) * (*tmpr)) - ((*wi) * (*tmpi)));
                        (*i3_5) = (((*wi) * (*tmpr)) + ((*wr) * (*tmpi)));
                        (*r2_1) = ((*r3_1) + (*r3_5));
                        (*i2_1) = ((*i3_1) + (*i3_5));
                        (*r2_5) = ((*r3_1) - (*r3_5));
                        (*i2_5) = ((*i3_1) - (*i3_5));
                    }
                    {
                        (*wr) = (((*W)[3 * (*l1)]).re);
                        (*wi) = (((*W)[3 * (*l1)]).im);
                        (*tmpr) = (((*jp)[3 * (*m)]).re);
                        (*tmpi) = (((*jp)[3 * (*m)]).im);
                        (*r3_3) = (((*wr) * (*tmpr)) - ((*wi) * (*tmpi)));
                        (*i3_3) = (((*wi) * (*tmpr)) + ((*wr) * (*tmpi)));
                        (*wr) = (((*W)[7 * (*l1)]).re);
                        (*wi) = (((*W)[7 * (*l1)]).im);
                        (*tmpr) = (((*jp)[7 * (*m)]).re);
                        (*tmpi) = (((*jp)[7 * (*m)]).im);
                        (*r3_7) = (((*wr) * (*tmpr)) - ((*wi) * (*tmpi)));
                        (*i3_7) = (((*wi) * (*tmpr)) + ((*wr) * (*tmpi)));
                        (*r2_3) = ((*r3_3) + (*r3_7));
                        (*i2_3) = ((*i3_3) + (*i3_7));
                        (*r2_7) = ((*r3_3) - (*r3_7));
                        (*i2_7) = ((*i3_3) - (*i3_7));
                    }
                    (*r1_1) = ((*r2_1) + (*r2_3));
                    (*i1_1) = ((*i2_1) + (*i2_3));
                    (*r1_5) = ((*r2_1) - (*r2_3));
                    (*i1_5) = ((*i2_1) - (*i2_3));
                    (*r1_3) = ((*r2_5) + (*i2_7));
                    (*i1_3) = ((*i2_5) - (*r2_7));
                    (*r1_7) = ((*r2_5) - (*i2_7));
                    (*i1_7) = ((*i2_5) + (*r2_7));
                }
                (((*kp)[0 * (*m)]).re) = ((*r1_0) + (*r1_1));
                (((*kp)[0 * (*m)]).im) = ((*i1_0) + (*i1_1));
                (((*kp)[4 * (*m)]).re) = ((*r1_0) - (*r1_1));
                (((*kp)[4 * (*m)]).im) = ((*i1_0) - (*i1_1));
                (*tmpr) = (0.70710678118699999 * ((*r1_3) + (*i1_3)));
                (*tmpi) = (0.70710678118699999 * ((*i1_3) - (*r1_3)));
                (((*kp)[1 * (*m)]).re) = ((*r1_2) + (*tmpr));
                (((*kp)[1 * (*m)]).im) = ((*i1_2) + (*tmpi));
                (((*kp)[5 * (*m)]).re) = ((*r1_2) - (*tmpr));
                (((*kp)[5 * (*m)]).im) = ((*i1_2) - (*tmpi));
                (((*kp)[2 * (*m)]).re) = ((*r1_4) + (*i1_5));
                (((*kp)[2 * (*m)]).im) = ((*i1_4) - (*r1_5));
                (((*kp)[6 * (*m)]).re) = ((*r1_4) - (*i1_5));
                (((*kp)[6 * (*m)]).im) = ((*i1_4) + (*r1_5));
                (*tmpr) = (0.70710678118699999 * ((*i1_7) - (*r1_7)));
                (*tmpi) = (0.70710678118699999 * ((*r1_7) + (*i1_7)));
                (((*kp)[3 * (*m)]).re) = ((*r1_6) + (*tmpr));
                (((*kp)[3 * (*m)]).im) = ((*i1_6) - (*tmpi));
                (((*kp)[7 * (*m)]).re) = ((*r1_6) - (*tmpr));
                (((*kp)[7 * (*m)]).im) = ((*i1_6) + (*tmpi));
            }
            (*l1) += (*nWdn);
            (*kp)++;
        }
        (this->inputsTPParent->i_darts34[this->getID()]) = i_darts_counter_temp34;
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/

    myTP->controlTPParent->nextCodeletsfft_twiddle_8[this->getID()]->decDep();
}
void TP34::_checkInCodelets1841::fire(void)
{

    /*printing node 1841: DeclStmt*/
    this->inputsTPParent->ab_darts34[this->getID()]
        = ((this->inputsTPParent->a_darts34[this->getID()])
              + (this->inputsTPParent->b_darts34[this->getID()]))
        / 2;
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 1841 nextRegion: 1844 */
    myTP->controlTPParent->checkInCodelets1844[this->getID()].decDep();
}
void TP34::_checkInCodelets1844::fire(void)
{

    /*printing node 1844: OMPTaskDirective*/
    /*syncNode: OMPTaskwaitDirective 1848*/
    /*Find the TP to which the nextRegion belongs*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->barrierCodelets1848[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 1844*/
    _task1844Inputs* task1844Inputs
        = new _task1844Inputs(&((this->inputsTPParent->W_darts34[this->getID()])),
            &((this->inputsTPParent->a_darts34[this->getID()])),
            &((this->inputsTPParent->ab_darts34[this->getID()])),
            &((this->inputsTPParent->in_darts34[this->getID()])),
            &((this->inputsTPParent->m_darts34[this->getID()])),
            &((this->inputsTPParent->nW_darts34[this->getID()])),
            &((this->inputsTPParent->nWdn_darts34[this->getID()])),
            &((this->inputsTPParent->out_darts34[this->getID()])), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task1844Inputs[this->getID()] = task1844Inputs;
    invoke<TP1844>(myTP, 1, this->getID(), myTP, task1844Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 1844 nextRegion: 1846 */
    myTP->controlTPParent->checkInCodelets1846[this->getID()].decDep();
}
void TP34::_checkInCodelets1846::fire(void)
{

    /*printing node 1846: OMPTaskDirective*/
    /*syncNode: OMPTaskwaitDirective 1848*/
    /*Find the TP to which the nextRegion belongs*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->barrierCodelets1848[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 1846*/
    _task1846Inputs* task1846Inputs
        = new _task1846Inputs(&((this->inputsTPParent->W_darts34[this->getID()])),
            &((this->inputsTPParent->ab_darts34[this->getID()])),
            &((this->inputsTPParent->b_darts34[this->getID()])),
            &((this->inputsTPParent->in_darts34[this->getID()])),
            &((this->inputsTPParent->m_darts34[this->getID()])),
            &((this->inputsTPParent->nW_darts34[this->getID()])),
            &((this->inputsTPParent->nWdn_darts34[this->getID()])),
            &((this->inputsTPParent->out_darts34[this->getID()])), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task1846Inputs[this->getID()] = task1846Inputs;
    invoke<TP1846>(myTP, 1, this->getID(), myTP, task1846Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 1846 nextRegion: 1848 */
    myTP->controlTPParent->barrierCodelets1848[0].decDep();
}
void TP34::_barrierCodelets1848::fire(void)
{
    TP34* myTP = static_cast<TP34*>(myTP_);

    for (size_t codeletsCounter = 0; codeletsCounter < (size_t)myTP->numThreads;
         codeletsCounter++) {
        myTP->nextCodeletsfft_twiddle_8[codeletsCounter]->decDep();
    }
}
TP34::TP34(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_mainNextCodelet,
    darts::Codelet* in_mainSyncCodelet, TP34** in_ptrToThisFunctionTP, int in_a, int in_b,
    COMPLEX* in_in, COMPLEX* in_out, COMPLEX* in_W, int in_nW, int in_nWdn, int in_m)
    : ompTP(in_numThreads, in_mainCodeletID)
    , ptrToThisFunctionTP(in_ptrToThisFunctionTP)
    , inputsTPParent(this)
    , controlTPParent(this)
    , nextCodeletsfft_twiddle_8(new Codelet*[in_numThreads])
    , nextSyncCodeletsfft_twiddle_8(new Codelet*[in_numThreads])
    , a_darts34(new int[this->numThreads])
    , b_darts34(new int[this->numThreads])
    , in_darts34(new COMPLEX*[this->numThreads])
    , out_darts34(new COMPLEX*[this->numThreads])
    , W_darts34(new COMPLEX*[this->numThreads])
    , nW_darts34(new int[this->numThreads])
    , nWdn_darts34(new int[this->numThreads])
    , m_darts34(new int[this->numThreads])
    , ab_darts34(new int[this->numThreads])
    , i_darts34(new int[this->numThreads])
    , i1_0_darts34(new REAL[this->numThreads])
    , i1_1_darts34(new REAL[this->numThreads])
    , i1_2_darts34(new REAL[this->numThreads])
    , i1_3_darts34(new REAL[this->numThreads])
    , i1_4_darts34(new REAL[this->numThreads])
    , i1_5_darts34(new REAL[this->numThreads])
    , i1_6_darts34(new REAL[this->numThreads])
    , i1_7_darts34(new REAL[this->numThreads])
    , i2_0_darts34(new REAL[this->numThreads])
    , i2_1_darts34(new REAL[this->numThreads])
    , i2_2_darts34(new REAL[this->numThreads])
    , i2_3_darts34(new REAL[this->numThreads])
    , i2_4_darts34(new REAL[this->numThreads])
    , i2_5_darts34(new REAL[this->numThreads])
    , i2_6_darts34(new REAL[this->numThreads])
    , i2_7_darts34(new REAL[this->numThreads])
    , i3_0_darts34(new REAL[this->numThreads])
    , i3_1_darts34(new REAL[this->numThreads])
    , i3_2_darts34(new REAL[this->numThreads])
    , i3_3_darts34(new REAL[this->numThreads])
    , i3_4_darts34(new REAL[this->numThreads])
    , i3_5_darts34(new REAL[this->numThreads])
    , i3_6_darts34(new REAL[this->numThreads])
    , i3_7_darts34(new REAL[this->numThreads])
    , jp_darts34(new COMPLEX*[this->numThreads])
    , kp_darts34(new COMPLEX*[this->numThreads])
    , l1_darts34(new int[this->numThreads])
    , r1_0_darts34(new REAL[this->numThreads])
    , r1_1_darts34(new REAL[this->numThreads])
    , r1_2_darts34(new REAL[this->numThreads])
    , r1_3_darts34(new REAL[this->numThreads])
    , r1_4_darts34(new REAL[this->numThreads])
    , r1_5_darts34(new REAL[this->numThreads])
    , r1_6_darts34(new REAL[this->numThreads])
    , r1_7_darts34(new REAL[this->numThreads])
    , r2_0_darts34(new REAL[this->numThreads])
    , r2_1_darts34(new REAL[this->numThreads])
    , r2_2_darts34(new REAL[this->numThreads])
    , r2_3_darts34(new REAL[this->numThreads])
    , r2_4_darts34(new REAL[this->numThreads])
    , r2_5_darts34(new REAL[this->numThreads])
    , r2_6_darts34(new REAL[this->numThreads])
    , r2_7_darts34(new REAL[this->numThreads])
    , r3_0_darts34(new REAL[this->numThreads])
    , r3_1_darts34(new REAL[this->numThreads])
    , r3_2_darts34(new REAL[this->numThreads])
    , r3_3_darts34(new REAL[this->numThreads])
    , r3_4_darts34(new REAL[this->numThreads])
    , r3_5_darts34(new REAL[this->numThreads])
    , r3_6_darts34(new REAL[this->numThreads])
    , r3_7_darts34(new REAL[this->numThreads])
    , tmpi_darts34(new REAL[this->numThreads])
    , tmpr_darts34(new REAL[this->numThreads])
    , wi_darts34(new REAL[this->numThreads])
    , wr_darts34(new REAL[this->numThreads])
    , task1844Inputs(new _task1844Inputs*[this->numThreads])
    , task1846Inputs(new _task1846Inputs*[this->numThreads])
    , checkInCodelets1457(new _checkInCodelets1457[this->numThreads])
    , checkInCodelets1460(new _checkInCodelets1460[this->numThreads])
    , checkInCodelets1841(new _checkInCodelets1841[this->numThreads])
    , checkInCodelets1844(new _checkInCodelets1844[this->numThreads])
    , checkInCodelets1846(new _checkInCodelets1846[this->numThreads])
    , barrierCodelets1848(new _barrierCodelets1848[1])
{
    barrierCodelets1848[0] = _barrierCodelets1848(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets1846* checkInCodelets1846Ptr = (this->checkInCodelets1846);
    _checkInCodelets1844* checkInCodelets1844Ptr = (this->checkInCodelets1844);
    _checkInCodelets1841* checkInCodelets1841Ptr = (this->checkInCodelets1841);
    _checkInCodelets1460* checkInCodelets1460Ptr = (this->checkInCodelets1460);
    _checkInCodelets1457* checkInCodelets1457Ptr = (this->checkInCodelets1457);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets1457);
#endif
    for (size_t codeletCounter = 0; codeletCounter < this->numThreads; codeletCounter++) {
        (*checkInCodelets1846Ptr) = _checkInCodelets1846(1, 1, this, codeletCounter);
        checkInCodelets1846Ptr++;
        (*checkInCodelets1844Ptr) = _checkInCodelets1844(1, 1, this, codeletCounter);
        checkInCodelets1844Ptr++;
        (*checkInCodelets1841Ptr) = _checkInCodelets1841(1, 1, this, codeletCounter);
        checkInCodelets1841Ptr++;
        (*checkInCodelets1460Ptr) = _checkInCodelets1460(1, 1, this, codeletCounter);
        checkInCodelets1460Ptr++;
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets1457Ptr) = _checkInCodelets1457(2, 1, this, codeletCounter);
#else
        (*checkInCodelets1457Ptr) = _checkInCodelets1457(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets1457Ptr).decDep();
        checkInCodelets1457Ptr++;
    }
    if (this->numThreads == 1) {
        this->nextCodeletsfft_twiddle_8[0] = in_mainNextCodelet;
        this->nextSyncCodeletsfft_twiddle_8[0] = in_mainSyncCodelet;
        this->a_darts34[0] = in_a;
        this->b_darts34[0] = in_b;
        this->in_darts34[0] = in_in;
        this->out_darts34[0] = in_out;
        this->W_darts34[0] = in_W;
        this->nW_darts34[0] = in_nW;
        this->nWdn_darts34[0] = in_nWdn;
        this->m_darts34[0] = in_m;
        this->availableCodelets[0] = 1;
    } else {
        this->a_darts34[this->mainCodeletID] = in_a;
        this->b_darts34[this->mainCodeletID] = in_b;
        this->in_darts34[this->mainCodeletID] = in_in;
        this->out_darts34[this->mainCodeletID] = in_out;
        this->W_darts34[this->mainCodeletID] = in_W;
        this->nW_darts34[this->mainCodeletID] = in_nW;
        this->nWdn_darts34[this->mainCodeletID] = in_nWdn;
        this->m_darts34[this->mainCodeletID] = in_m;
        this->nextCodeletsfft_twiddle_8[in_mainCodeletID] = in_mainNextCodelet;
        this->nextSyncCodeletsfft_twiddle_8[in_mainCodeletID] = in_mainSyncCodelet;
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
    delete[] task1844Inputs;
    delete[] task1846Inputs;
    delete[] barrierCodelets1848;
    delete[] checkInCodelets1846;
    delete[] checkInCodelets1844;
    delete[] checkInCodelets1841;
    delete[] checkInCodelets1460;
    delete[] checkInCodelets1457;
    delete[] nextCodeletsfft_twiddle_8;
    delete[] nextSyncCodeletsfft_twiddle_8;
    delete[] a_darts34;
    delete[] b_darts34;
    delete[] in_darts34;
    delete[] out_darts34;
    delete[] W_darts34;
    delete[] nW_darts34;
    delete[] nWdn_darts34;
    delete[] m_darts34;
    delete[] ab_darts34;
    delete[] i_darts34;
    delete[] i1_0_darts34;
    delete[] i1_1_darts34;
    delete[] i1_2_darts34;
    delete[] i1_3_darts34;
    delete[] i1_4_darts34;
    delete[] i1_5_darts34;
    delete[] i1_6_darts34;
    delete[] i1_7_darts34;
    delete[] i2_0_darts34;
    delete[] i2_1_darts34;
    delete[] i2_2_darts34;
    delete[] i2_3_darts34;
    delete[] i2_4_darts34;
    delete[] i2_5_darts34;
    delete[] i2_6_darts34;
    delete[] i2_7_darts34;
    delete[] i3_0_darts34;
    delete[] i3_1_darts34;
    delete[] i3_2_darts34;
    delete[] i3_3_darts34;
    delete[] i3_4_darts34;
    delete[] i3_5_darts34;
    delete[] i3_6_darts34;
    delete[] i3_7_darts34;
    delete[] jp_darts34;
    delete[] kp_darts34;
    delete[] l1_darts34;
    delete[] r1_0_darts34;
    delete[] r1_1_darts34;
    delete[] r1_2_darts34;
    delete[] r1_3_darts34;
    delete[] r1_4_darts34;
    delete[] r1_5_darts34;
    delete[] r1_6_darts34;
    delete[] r1_7_darts34;
    delete[] r2_0_darts34;
    delete[] r2_1_darts34;
    delete[] r2_2_darts34;
    delete[] r2_3_darts34;
    delete[] r2_4_darts34;
    delete[] r2_5_darts34;
    delete[] r2_6_darts34;
    delete[] r2_7_darts34;
    delete[] r3_0_darts34;
    delete[] r3_1_darts34;
    delete[] r3_2_darts34;
    delete[] r3_3_darts34;
    delete[] r3_4_darts34;
    delete[] r3_5_darts34;
    delete[] r3_6_darts34;
    delete[] r3_7_darts34;
    delete[] tmpi_darts34;
    delete[] tmpr_darts34;
    delete[] wi_darts34;
    delete[] wr_darts34;
}
void TP34::setNewInputs(int in_a, int in_b, COMPLEX* in_in, COMPLEX* in_out, COMPLEX* in_W,
    int in_nW, int in_nWdn, int in_m, size_t codeletID)
{
    this->a_darts34[codeletID] = in_a;
    this->b_darts34[codeletID] = in_b;
    this->in_darts34[codeletID] = in_in;
    this->out_darts34[codeletID] = in_out;
    this->W_darts34[codeletID] = in_W;
    this->nW_darts34[codeletID] = in_nW;
    this->nWdn_darts34[codeletID] = in_nWdn;
    this->m_darts34[codeletID] = in_m;
}
/*TP1844: OMPTaskDirective*/
void TP1844::_checkInCodelets1845::fire(void)
{
/*Init the vars for this region*/

/*printing node 1845: CallExpr*/
/*Finding the next codelet after the function call*/
#if 0
/*Signaling next codelet region: 1845 nextRegion: 11549 */
myTP->controlTPParent->checkInCodelets11549.decDep();
#endif
/*Finding the next sync point before making the function call*/
#if 0
/*Find the sync point for a function call*/
/*syncNode: OMPTaskwaitDirective 1848*/
/*No syncpoint found or outside task's boundaries, looking for syncCodelet*/
#endif
    /*Make the function call*/
    invoke<TP_fft_twiddle_8>(myTP, myTP->numThreads, this->getID(),
        &(myTP->controlTPParent->checkInCodelets11549),
        (myTP->controlTPParent->task1844Inputs->nextSyncCodelet), nullptr,
        (this->taskInputs->a_darts1844), (this->taskInputs->ab_darts1844),
        (this->taskInputs->in_darts1844), (this->taskInputs->out_darts1844),
        (this->taskInputs->W_darts1844), (this->taskInputs->nW_darts1844),
        (this->taskInputs->nWdn_darts1844), (this->taskInputs->m_darts1844));
}
void TP1844::_checkInCodelets11549::fire(void)
{

    /*printing node 11549: NullStmt*/
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task1844Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task1844Inputs->nextSyncCodelet->decDep();
}
TP1844::TP1844(
    int in_numThreads, int in_mainCodeletID, TP34* in_TPParent, _task1844Inputs* in_task1844Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task1844Inputs(in_task1844Inputs)
    , TP1845Ptr(nullptr)
    , TP1845_alreadyLaunched(0)
    , checkInCodelets1845(1, 1, this, this->mainCodeletID)
    , checkInCodelets11549(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets1845.decDep();
}
TP1844::~TP1844() { delete (task1844Inputs); }
/*TP1846: OMPTaskDirective*/
void TP1846::_checkInCodelets1847::fire(void)
{
/*Init the vars for this region*/

/*printing node 1847: CallExpr*/
/*Finding the next codelet after the function call*/
#if 0
/*Signaling next codelet region: 1847 nextRegion: 11550 */
myTP->controlTPParent->checkInCodelets11550.decDep();
#endif
/*Finding the next sync point before making the function call*/
#if 0
/*Find the sync point for a function call*/
/*syncNode: OMPTaskwaitDirective 1848*/
/*No syncpoint found or outside task's boundaries, looking for syncCodelet*/
#endif
    /*Make the function call*/
    invoke<TP_fft_twiddle_8>(myTP, myTP->numThreads, this->getID(),
        &(myTP->controlTPParent->checkInCodelets11550),
        (myTP->controlTPParent->task1846Inputs->nextSyncCodelet), nullptr,
        (this->taskInputs->ab_darts1846), (this->taskInputs->b_darts1846),
        (this->taskInputs->in_darts1846), (this->taskInputs->out_darts1846),
        (this->taskInputs->W_darts1846), (this->taskInputs->nW_darts1846),
        (this->taskInputs->nWdn_darts1846), (this->taskInputs->m_darts1846));
}
void TP1846::_checkInCodelets11550::fire(void)
{

    /*printing node 11550: NullStmt*/
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task1846Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task1846Inputs->nextSyncCodelet->decDep();
}
TP1846::TP1846(
    int in_numThreads, int in_mainCodeletID, TP34* in_TPParent, _task1846Inputs* in_task1846Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task1846Inputs(in_task1846Inputs)
    , TP1847Ptr(nullptr)
    , TP1847_alreadyLaunched(0)
    , checkInCodelets1847(1, 1, this, this->mainCodeletID)
    , checkInCodelets11550(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets1847.decDep();
}
TP1846::~TP1846() { delete (task1846Inputs); }
/*TP36: TP_fft_unshuffle_8*/
void TP36::_checkInCodelets2250::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    /*Init the vars for this region*/
    /*Printing conditional branch node 2250: inlining: 0*/
    if (((this->inputsTPParent->b_darts36[this->getID()])
            - (this->inputsTPParent->a_darts36[this->getID()]))
        < 128) {
        myTP->checkInCodelets2253[this->getID()].decDep();
    } else {
        /*elseRegion: DeclStmt 2298*/
        myTP->checkInCodelets2298[this->getID()].decDep();
    }
}
void TP36::_checkInCodelets2253::fire(void)
{

    /*printing node 2253: BinaryOperator*/
    (this->inputsTPParent->ip_darts36[this->getID()])
        = (this->inputsTPParent->in_darts36[this->getID()])
        + (this->inputsTPParent->a_darts36[this->getID()]) * 8;

    /*printing node 2256: ForStmt*/
    {
        int* b = &(this->inputsTPParent->b_darts36[this->getLocalID()]);
        (void)b /*PRIVATE*/;
        int* i = &(this->inputsTPParent->i_darts36[this->getLocalID()]);
        (void)i /*PRIVATE*/;
        COMPLEX** ip = &(this->inputsTPParent->ip_darts36[this->getLocalID()]);
        (void)ip /*PRIVATE*/;
        COMPLEX** jp = &(this->inputsTPParent->jp_darts36[this->getLocalID()]);
        (void)jp /*PRIVATE*/;
        int* m = &(this->inputsTPParent->m_darts36[this->getLocalID()]);
        (void)m /*PRIVATE*/;
        COMPLEX** out = &(this->inputsTPParent->out_darts36[this->getLocalID()]);
        (void)out /*PRIVATE*/;
        /*Loop's init*/
        (this->inputsTPParent->i_darts36[this->getID()])
            = (this->inputsTPParent->a_darts36[this->getID()]);
        int i_darts_counter_temp36 = (this->inputsTPParent->i_darts36[this->getID()]);
        for (; (i_darts_counter_temp36) < (*b); ++(i_darts_counter_temp36)) {
            (*jp) = (*out) + (i_darts_counter_temp36);
            (*jp)[0] = (*ip)[0];
            (*jp)[(*m)] = (*ip)[1];
            (*ip) += 2;
            (*jp) += 2 * (*m);
            (*jp)[0] = (*ip)[0];
            (*jp)[(*m)] = (*ip)[1];
            (*ip) += 2;
            (*jp) += 2 * (*m);
            (*jp)[0] = (*ip)[0];
            (*jp)[(*m)] = (*ip)[1];
            (*ip) += 2;
            (*jp) += 2 * (*m);
            (*jp)[0] = (*ip)[0];
            (*jp)[(*m)] = (*ip)[1];
            (*ip) += 2;
        }
        (this->inputsTPParent->i_darts36[this->getID()]) = i_darts_counter_temp36;
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/

    myTP->controlTPParent->nextCodeletsfft_unshuffle_8[this->getID()]->decDep();
}
void TP36::_checkInCodelets2298::fire(void)
{

    /*printing node 2298: DeclStmt*/
    this->inputsTPParent->ab_darts36[this->getID()]
        = ((this->inputsTPParent->a_darts36[this->getID()])
              + (this->inputsTPParent->b_darts36[this->getID()]))
        / 2;
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 2298 nextRegion: 2301 */
    myTP->controlTPParent->checkInCodelets2301[this->getID()].decDep();
}
void TP36::_checkInCodelets2301::fire(void)
{

    /*printing node 2301: OMPTaskDirective*/
    /*syncNode: OMPTaskwaitDirective 2305*/
    /*Find the TP to which the nextRegion belongs*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->barrierCodelets2305[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 2301*/
    _task2301Inputs* task2301Inputs
        = new _task2301Inputs(&((this->inputsTPParent->a_darts36[this->getID()])),
            &((this->inputsTPParent->ab_darts36[this->getID()])),
            &((this->inputsTPParent->in_darts36[this->getID()])),
            &((this->inputsTPParent->m_darts36[this->getID()])),
            &((this->inputsTPParent->out_darts36[this->getID()])), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task2301Inputs[this->getID()] = task2301Inputs;
    invoke<TP2301>(myTP, 1, this->getID(), myTP, task2301Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 2301 nextRegion: 2303 */
    myTP->controlTPParent->checkInCodelets2303[this->getID()].decDep();
}
void TP36::_checkInCodelets2303::fire(void)
{

    /*printing node 2303: OMPTaskDirective*/
    /*syncNode: OMPTaskwaitDirective 2305*/
    /*Find the TP to which the nextRegion belongs*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->barrierCodelets2305[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 2303*/
    _task2303Inputs* task2303Inputs
        = new _task2303Inputs(&((this->inputsTPParent->ab_darts36[this->getID()])),
            &((this->inputsTPParent->b_darts36[this->getID()])),
            &((this->inputsTPParent->in_darts36[this->getID()])),
            &((this->inputsTPParent->m_darts36[this->getID()])),
            &((this->inputsTPParent->out_darts36[this->getID()])), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task2303Inputs[this->getID()] = task2303Inputs;
    invoke<TP2303>(myTP, 1, this->getID(), myTP, task2303Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 2303 nextRegion: 2305 */
    myTP->controlTPParent->barrierCodelets2305[0].decDep();
}
void TP36::_barrierCodelets2305::fire(void)
{
    TP36* myTP = static_cast<TP36*>(myTP_);

    for (size_t codeletsCounter = 0; codeletsCounter < (size_t)myTP->numThreads;
         codeletsCounter++) {
        myTP->nextCodeletsfft_unshuffle_8[codeletsCounter]->decDep();
    }
}
TP36::TP36(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_mainNextCodelet,
    darts::Codelet* in_mainSyncCodelet, TP36** in_ptrToThisFunctionTP, int in_a, int in_b,
    COMPLEX* in_in, COMPLEX* in_out, int in_m)
    : ompTP(in_numThreads, in_mainCodeletID)
    , ptrToThisFunctionTP(in_ptrToThisFunctionTP)
    , inputsTPParent(this)
    , controlTPParent(this)
    , nextCodeletsfft_unshuffle_8(new Codelet*[in_numThreads])
    , nextSyncCodeletsfft_unshuffle_8(new Codelet*[in_numThreads])
    , a_darts36(new int[this->numThreads])
    , b_darts36(new int[this->numThreads])
    , in_darts36(new COMPLEX*[this->numThreads])
    , out_darts36(new COMPLEX*[this->numThreads])
    , m_darts36(new int[this->numThreads])
    , ab_darts36(new int[this->numThreads])
    , i_darts36(new int[this->numThreads])
    , ip_darts36(new COMPLEX*[this->numThreads])
    , jp_darts36(new COMPLEX*[this->numThreads])
    , task2301Inputs(new _task2301Inputs*[this->numThreads])
    , task2303Inputs(new _task2303Inputs*[this->numThreads])
    , checkInCodelets2250(new _checkInCodelets2250[this->numThreads])
    , checkInCodelets2253(new _checkInCodelets2253[this->numThreads])
    , checkInCodelets2298(new _checkInCodelets2298[this->numThreads])
    , checkInCodelets2301(new _checkInCodelets2301[this->numThreads])
    , checkInCodelets2303(new _checkInCodelets2303[this->numThreads])
    , barrierCodelets2305(new _barrierCodelets2305[1])
{
    barrierCodelets2305[0] = _barrierCodelets2305(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets2303* checkInCodelets2303Ptr = (this->checkInCodelets2303);
    _checkInCodelets2301* checkInCodelets2301Ptr = (this->checkInCodelets2301);
    _checkInCodelets2298* checkInCodelets2298Ptr = (this->checkInCodelets2298);
    _checkInCodelets2253* checkInCodelets2253Ptr = (this->checkInCodelets2253);
    _checkInCodelets2250* checkInCodelets2250Ptr = (this->checkInCodelets2250);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets2250);
#endif
    for (size_t codeletCounter = 0; codeletCounter < this->numThreads; codeletCounter++) {
        (*checkInCodelets2303Ptr) = _checkInCodelets2303(1, 1, this, codeletCounter);
        checkInCodelets2303Ptr++;
        (*checkInCodelets2301Ptr) = _checkInCodelets2301(1, 1, this, codeletCounter);
        checkInCodelets2301Ptr++;
        (*checkInCodelets2298Ptr) = _checkInCodelets2298(1, 1, this, codeletCounter);
        checkInCodelets2298Ptr++;
        (*checkInCodelets2253Ptr) = _checkInCodelets2253(1, 1, this, codeletCounter);
        checkInCodelets2253Ptr++;
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets2250Ptr) = _checkInCodelets2250(2, 1, this, codeletCounter);
#else
        (*checkInCodelets2250Ptr) = _checkInCodelets2250(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets2250Ptr).decDep();
        checkInCodelets2250Ptr++;
    }
    if (this->numThreads == 1) {
        this->nextCodeletsfft_unshuffle_8[0] = in_mainNextCodelet;
        this->nextSyncCodeletsfft_unshuffle_8[0] = in_mainSyncCodelet;
        this->a_darts36[0] = in_a;
        this->b_darts36[0] = in_b;
        this->in_darts36[0] = in_in;
        this->out_darts36[0] = in_out;
        this->m_darts36[0] = in_m;
        this->availableCodelets[0] = 1;
    } else {
        this->a_darts36[this->mainCodeletID] = in_a;
        this->b_darts36[this->mainCodeletID] = in_b;
        this->in_darts36[this->mainCodeletID] = in_in;
        this->out_darts36[this->mainCodeletID] = in_out;
        this->m_darts36[this->mainCodeletID] = in_m;
        this->nextCodeletsfft_unshuffle_8[in_mainCodeletID] = in_mainNextCodelet;
        this->nextSyncCodeletsfft_unshuffle_8[in_mainCodeletID] = in_mainSyncCodelet;
#if USE_SPIN_CODELETS == 0
        this->firstCodelet[this->mainCodeletID].decDep();
#else
        this->availableCodelets[this->mainCodeletID] = 1;
#endif
        *(this->ptrToThisFunctionTP) = this;
    }
}
TP36::~TP36()
{
    delete[] task2301Inputs;
    delete[] task2303Inputs;
    delete[] barrierCodelets2305;
    delete[] checkInCodelets2303;
    delete[] checkInCodelets2301;
    delete[] checkInCodelets2298;
    delete[] checkInCodelets2253;
    delete[] checkInCodelets2250;
    delete[] nextCodeletsfft_unshuffle_8;
    delete[] nextSyncCodeletsfft_unshuffle_8;
    delete[] a_darts36;
    delete[] b_darts36;
    delete[] in_darts36;
    delete[] out_darts36;
    delete[] m_darts36;
    delete[] ab_darts36;
    delete[] i_darts36;
    delete[] ip_darts36;
    delete[] jp_darts36;
}
void TP36::setNewInputs(
    int in_a, int in_b, COMPLEX* in_in, COMPLEX* in_out, int in_m, size_t codeletID)
{
    this->a_darts36[codeletID] = in_a;
    this->b_darts36[codeletID] = in_b;
    this->in_darts36[codeletID] = in_in;
    this->out_darts36[codeletID] = in_out;
    this->m_darts36[codeletID] = in_m;
}
/*TP2301: OMPTaskDirective*/
void TP2301::_checkInCodelets2302::fire(void)
{
/*Init the vars for this region*/

/*printing node 2302: CallExpr*/
/*Finding the next codelet after the function call*/
#if 0
/*Signaling next codelet region: 2302 nextRegion: 11537 */
myTP->controlTPParent->checkInCodelets11537.decDep();
#endif
/*Finding the next sync point before making the function call*/
#if 0
/*Find the sync point for a function call*/
/*syncNode: OMPTaskwaitDirective 2305*/
/*No syncpoint found or outside task's boundaries, looking for syncCodelet*/
#endif
    /*Make the function call*/
    invoke<TP_fft_unshuffle_8>(myTP, myTP->numThreads, this->getID(),
        &(myTP->controlTPParent->checkInCodelets11537),
        (myTP->controlTPParent->task2301Inputs->nextSyncCodelet), nullptr,
        (this->taskInputs->a_darts2301), (this->taskInputs->ab_darts2301),
        (this->taskInputs->in_darts2301), (this->taskInputs->out_darts2301),
        (this->taskInputs->m_darts2301));
}
void TP2301::_checkInCodelets11537::fire(void)
{

    /*printing node 11537: NullStmt*/
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task2301Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task2301Inputs->nextSyncCodelet->decDep();
}
TP2301::TP2301(
    int in_numThreads, int in_mainCodeletID, TP36* in_TPParent, _task2301Inputs* in_task2301Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task2301Inputs(in_task2301Inputs)
    , TP2302Ptr(nullptr)
    , TP2302_alreadyLaunched(0)
    , checkInCodelets2302(1, 1, this, this->mainCodeletID)
    , checkInCodelets11537(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets2302.decDep();
}
TP2301::~TP2301() { delete (task2301Inputs); }
/*TP2303: OMPTaskDirective*/
void TP2303::_checkInCodelets2304::fire(void)
{
/*Init the vars for this region*/

/*printing node 2304: CallExpr*/
/*Finding the next codelet after the function call*/
#if 0
/*Signaling next codelet region: 2304 nextRegion: 11538 */
myTP->controlTPParent->checkInCodelets11538.decDep();
#endif
/*Finding the next sync point before making the function call*/
#if 0
/*Find the sync point for a function call*/
/*syncNode: OMPTaskwaitDirective 2305*/
/*No syncpoint found or outside task's boundaries, looking for syncCodelet*/
#endif
    /*Make the function call*/
    invoke<TP_fft_unshuffle_8>(myTP, myTP->numThreads, this->getID(),
        &(myTP->controlTPParent->checkInCodelets11538),
        (myTP->controlTPParent->task2303Inputs->nextSyncCodelet), nullptr,
        (this->taskInputs->ab_darts2303), (this->taskInputs->b_darts2303),
        (this->taskInputs->in_darts2303), (this->taskInputs->out_darts2303),
        (this->taskInputs->m_darts2303));
}
void TP2303::_checkInCodelets11538::fire(void)
{

    /*printing node 11538: NullStmt*/
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task2303Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task2303Inputs->nextSyncCodelet->decDep();
}
TP2303::TP2303(
    int in_numThreads, int in_mainCodeletID, TP36* in_TPParent, _task2303Inputs* in_task2303Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task2303Inputs(in_task2303Inputs)
    , TP2304Ptr(nullptr)
    , TP2304_alreadyLaunched(0)
    , checkInCodelets2304(1, 1, this, this->mainCodeletID)
    , checkInCodelets11538(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets2304.decDep();
}
TP2303::~TP2303() { delete (task2303Inputs); }
/*TP39: TP_fft_twiddle_16*/
void TP39::_checkInCodelets2960::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    /*Init the vars for this region*/
    /*Printing conditional branch node 2960: inlining: 0*/
    if (((this->inputsTPParent->b_darts39[this->getID()])
            - (this->inputsTPParent->a_darts39[this->getID()]))
        < 128) {
        myTP->checkInCodelets2963[this->getID()].decDep();
    } else {
        /*elseRegion: DeclStmt 3872*/
        myTP->checkInCodelets3872[this->getID()].decDep();
    }
}
void TP39::_checkInCodelets2963::fire(void)
{

    /*printing node 2963: ForStmt*/
    {
        COMPLEX** W = &(this->inputsTPParent->W_darts39[this->getLocalID()]);
        (void)W /*PRIVATE*/;
        int* b = &(this->inputsTPParent->b_darts39[this->getLocalID()]);
        (void)b /*PRIVATE*/;
        int* i = &(this->inputsTPParent->i_darts39[this->getLocalID()]);
        (void)i /*PRIVATE*/;
        REAL* i1_0 = &(this->inputsTPParent->i1_0_darts39[this->getLocalID()]);
        (void)i1_0 /*PRIVATE*/;
        REAL* i1_1 = &(this->inputsTPParent->i1_1_darts39[this->getLocalID()]);
        (void)i1_1 /*PRIVATE*/;
        REAL* i1_10 = &(this->inputsTPParent->i1_10_darts39[this->getLocalID()]);
        (void)i1_10 /*PRIVATE*/;
        REAL* i1_11 = &(this->inputsTPParent->i1_11_darts39[this->getLocalID()]);
        (void)i1_11 /*PRIVATE*/;
        REAL* i1_12 = &(this->inputsTPParent->i1_12_darts39[this->getLocalID()]);
        (void)i1_12 /*PRIVATE*/;
        REAL* i1_13 = &(this->inputsTPParent->i1_13_darts39[this->getLocalID()]);
        (void)i1_13 /*PRIVATE*/;
        REAL* i1_14 = &(this->inputsTPParent->i1_14_darts39[this->getLocalID()]);
        (void)i1_14 /*PRIVATE*/;
        REAL* i1_15 = &(this->inputsTPParent->i1_15_darts39[this->getLocalID()]);
        (void)i1_15 /*PRIVATE*/;
        REAL* i1_2 = &(this->inputsTPParent->i1_2_darts39[this->getLocalID()]);
        (void)i1_2 /*PRIVATE*/;
        REAL* i1_3 = &(this->inputsTPParent->i1_3_darts39[this->getLocalID()]);
        (void)i1_3 /*PRIVATE*/;
        REAL* i1_4 = &(this->inputsTPParent->i1_4_darts39[this->getLocalID()]);
        (void)i1_4 /*PRIVATE*/;
        REAL* i1_5 = &(this->inputsTPParent->i1_5_darts39[this->getLocalID()]);
        (void)i1_5 /*PRIVATE*/;
        REAL* i1_6 = &(this->inputsTPParent->i1_6_darts39[this->getLocalID()]);
        (void)i1_6 /*PRIVATE*/;
        REAL* i1_7 = &(this->inputsTPParent->i1_7_darts39[this->getLocalID()]);
        (void)i1_7 /*PRIVATE*/;
        REAL* i1_8 = &(this->inputsTPParent->i1_8_darts39[this->getLocalID()]);
        (void)i1_8 /*PRIVATE*/;
        REAL* i1_9 = &(this->inputsTPParent->i1_9_darts39[this->getLocalID()]);
        (void)i1_9 /*PRIVATE*/;
        REAL* i2_0 = &(this->inputsTPParent->i2_0_darts39[this->getLocalID()]);
        (void)i2_0 /*PRIVATE*/;
        REAL* i2_1 = &(this->inputsTPParent->i2_1_darts39[this->getLocalID()]);
        (void)i2_1 /*PRIVATE*/;
        REAL* i2_10 = &(this->inputsTPParent->i2_10_darts39[this->getLocalID()]);
        (void)i2_10 /*PRIVATE*/;
        REAL* i2_11 = &(this->inputsTPParent->i2_11_darts39[this->getLocalID()]);
        (void)i2_11 /*PRIVATE*/;
        REAL* i2_12 = &(this->inputsTPParent->i2_12_darts39[this->getLocalID()]);
        (void)i2_12 /*PRIVATE*/;
        REAL* i2_13 = &(this->inputsTPParent->i2_13_darts39[this->getLocalID()]);
        (void)i2_13 /*PRIVATE*/;
        REAL* i2_14 = &(this->inputsTPParent->i2_14_darts39[this->getLocalID()]);
        (void)i2_14 /*PRIVATE*/;
        REAL* i2_15 = &(this->inputsTPParent->i2_15_darts39[this->getLocalID()]);
        (void)i2_15 /*PRIVATE*/;
        REAL* i2_2 = &(this->inputsTPParent->i2_2_darts39[this->getLocalID()]);
        (void)i2_2 /*PRIVATE*/;
        REAL* i2_3 = &(this->inputsTPParent->i2_3_darts39[this->getLocalID()]);
        (void)i2_3 /*PRIVATE*/;
        REAL* i2_4 = &(this->inputsTPParent->i2_4_darts39[this->getLocalID()]);
        (void)i2_4 /*PRIVATE*/;
        REAL* i2_5 = &(this->inputsTPParent->i2_5_darts39[this->getLocalID()]);
        (void)i2_5 /*PRIVATE*/;
        REAL* i2_6 = &(this->inputsTPParent->i2_6_darts39[this->getLocalID()]);
        (void)i2_6 /*PRIVATE*/;
        REAL* i2_7 = &(this->inputsTPParent->i2_7_darts39[this->getLocalID()]);
        (void)i2_7 /*PRIVATE*/;
        REAL* i2_8 = &(this->inputsTPParent->i2_8_darts39[this->getLocalID()]);
        (void)i2_8 /*PRIVATE*/;
        REAL* i2_9 = &(this->inputsTPParent->i2_9_darts39[this->getLocalID()]);
        (void)i2_9 /*PRIVATE*/;
        REAL* i3_0 = &(this->inputsTPParent->i3_0_darts39[this->getLocalID()]);
        (void)i3_0 /*PRIVATE*/;
        REAL* i3_1 = &(this->inputsTPParent->i3_1_darts39[this->getLocalID()]);
        (void)i3_1 /*PRIVATE*/;
        REAL* i3_10 = &(this->inputsTPParent->i3_10_darts39[this->getLocalID()]);
        (void)i3_10 /*PRIVATE*/;
        REAL* i3_11 = &(this->inputsTPParent->i3_11_darts39[this->getLocalID()]);
        (void)i3_11 /*PRIVATE*/;
        REAL* i3_12 = &(this->inputsTPParent->i3_12_darts39[this->getLocalID()]);
        (void)i3_12 /*PRIVATE*/;
        REAL* i3_13 = &(this->inputsTPParent->i3_13_darts39[this->getLocalID()]);
        (void)i3_13 /*PRIVATE*/;
        REAL* i3_14 = &(this->inputsTPParent->i3_14_darts39[this->getLocalID()]);
        (void)i3_14 /*PRIVATE*/;
        REAL* i3_15 = &(this->inputsTPParent->i3_15_darts39[this->getLocalID()]);
        (void)i3_15 /*PRIVATE*/;
        REAL* i3_2 = &(this->inputsTPParent->i3_2_darts39[this->getLocalID()]);
        (void)i3_2 /*PRIVATE*/;
        REAL* i3_3 = &(this->inputsTPParent->i3_3_darts39[this->getLocalID()]);
        (void)i3_3 /*PRIVATE*/;
        REAL* i3_4 = &(this->inputsTPParent->i3_4_darts39[this->getLocalID()]);
        (void)i3_4 /*PRIVATE*/;
        REAL* i3_5 = &(this->inputsTPParent->i3_5_darts39[this->getLocalID()]);
        (void)i3_5 /*PRIVATE*/;
        REAL* i3_6 = &(this->inputsTPParent->i3_6_darts39[this->getLocalID()]);
        (void)i3_6 /*PRIVATE*/;
        REAL* i3_7 = &(this->inputsTPParent->i3_7_darts39[this->getLocalID()]);
        (void)i3_7 /*PRIVATE*/;
        REAL* i3_8 = &(this->inputsTPParent->i3_8_darts39[this->getLocalID()]);
        (void)i3_8 /*PRIVATE*/;
        REAL* i3_9 = &(this->inputsTPParent->i3_9_darts39[this->getLocalID()]);
        (void)i3_9 /*PRIVATE*/;
        REAL* i4_0 = &(this->inputsTPParent->i4_0_darts39[this->getLocalID()]);
        (void)i4_0 /*PRIVATE*/;
        REAL* i4_1 = &(this->inputsTPParent->i4_1_darts39[this->getLocalID()]);
        (void)i4_1 /*PRIVATE*/;
        REAL* i4_10 = &(this->inputsTPParent->i4_10_darts39[this->getLocalID()]);
        (void)i4_10 /*PRIVATE*/;
        REAL* i4_11 = &(this->inputsTPParent->i4_11_darts39[this->getLocalID()]);
        (void)i4_11 /*PRIVATE*/;
        REAL* i4_12 = &(this->inputsTPParent->i4_12_darts39[this->getLocalID()]);
        (void)i4_12 /*PRIVATE*/;
        REAL* i4_13 = &(this->inputsTPParent->i4_13_darts39[this->getLocalID()]);
        (void)i4_13 /*PRIVATE*/;
        REAL* i4_14 = &(this->inputsTPParent->i4_14_darts39[this->getLocalID()]);
        (void)i4_14 /*PRIVATE*/;
        REAL* i4_15 = &(this->inputsTPParent->i4_15_darts39[this->getLocalID()]);
        (void)i4_15 /*PRIVATE*/;
        REAL* i4_2 = &(this->inputsTPParent->i4_2_darts39[this->getLocalID()]);
        (void)i4_2 /*PRIVATE*/;
        REAL* i4_3 = &(this->inputsTPParent->i4_3_darts39[this->getLocalID()]);
        (void)i4_3 /*PRIVATE*/;
        REAL* i4_4 = &(this->inputsTPParent->i4_4_darts39[this->getLocalID()]);
        (void)i4_4 /*PRIVATE*/;
        REAL* i4_5 = &(this->inputsTPParent->i4_5_darts39[this->getLocalID()]);
        (void)i4_5 /*PRIVATE*/;
        REAL* i4_6 = &(this->inputsTPParent->i4_6_darts39[this->getLocalID()]);
        (void)i4_6 /*PRIVATE*/;
        REAL* i4_7 = &(this->inputsTPParent->i4_7_darts39[this->getLocalID()]);
        (void)i4_7 /*PRIVATE*/;
        REAL* i4_8 = &(this->inputsTPParent->i4_8_darts39[this->getLocalID()]);
        (void)i4_8 /*PRIVATE*/;
        REAL* i4_9 = &(this->inputsTPParent->i4_9_darts39[this->getLocalID()]);
        (void)i4_9 /*PRIVATE*/;
        COMPLEX** in = &(this->inputsTPParent->in_darts39[this->getLocalID()]);
        (void)in /*PRIVATE*/;
        COMPLEX** jp = &(this->inputsTPParent->jp_darts39[this->getLocalID()]);
        (void)jp /*PRIVATE*/;
        COMPLEX** kp = &(this->inputsTPParent->kp_darts39[this->getLocalID()]);
        (void)kp /*PRIVATE*/;
        int* l1 = &(this->inputsTPParent->l1_darts39[this->getLocalID()]);
        (void)l1 /*PRIVATE*/;
        int* m = &(this->inputsTPParent->m_darts39[this->getLocalID()]);
        (void)m /*PRIVATE*/;
        int* nWdn = &(this->inputsTPParent->nWdn_darts39[this->getLocalID()]);
        (void)nWdn /*PRIVATE*/;
        COMPLEX** out = &(this->inputsTPParent->out_darts39[this->getLocalID()]);
        (void)out /*PRIVATE*/;
        REAL* r1_0 = &(this->inputsTPParent->r1_0_darts39[this->getLocalID()]);
        (void)r1_0 /*PRIVATE*/;
        REAL* r1_1 = &(this->inputsTPParent->r1_1_darts39[this->getLocalID()]);
        (void)r1_1 /*PRIVATE*/;
        REAL* r1_10 = &(this->inputsTPParent->r1_10_darts39[this->getLocalID()]);
        (void)r1_10 /*PRIVATE*/;
        REAL* r1_11 = &(this->inputsTPParent->r1_11_darts39[this->getLocalID()]);
        (void)r1_11 /*PRIVATE*/;
        REAL* r1_12 = &(this->inputsTPParent->r1_12_darts39[this->getLocalID()]);
        (void)r1_12 /*PRIVATE*/;
        REAL* r1_13 = &(this->inputsTPParent->r1_13_darts39[this->getLocalID()]);
        (void)r1_13 /*PRIVATE*/;
        REAL* r1_14 = &(this->inputsTPParent->r1_14_darts39[this->getLocalID()]);
        (void)r1_14 /*PRIVATE*/;
        REAL* r1_15 = &(this->inputsTPParent->r1_15_darts39[this->getLocalID()]);
        (void)r1_15 /*PRIVATE*/;
        REAL* r1_2 = &(this->inputsTPParent->r1_2_darts39[this->getLocalID()]);
        (void)r1_2 /*PRIVATE*/;
        REAL* r1_3 = &(this->inputsTPParent->r1_3_darts39[this->getLocalID()]);
        (void)r1_3 /*PRIVATE*/;
        REAL* r1_4 = &(this->inputsTPParent->r1_4_darts39[this->getLocalID()]);
        (void)r1_4 /*PRIVATE*/;
        REAL* r1_5 = &(this->inputsTPParent->r1_5_darts39[this->getLocalID()]);
        (void)r1_5 /*PRIVATE*/;
        REAL* r1_6 = &(this->inputsTPParent->r1_6_darts39[this->getLocalID()]);
        (void)r1_6 /*PRIVATE*/;
        REAL* r1_7 = &(this->inputsTPParent->r1_7_darts39[this->getLocalID()]);
        (void)r1_7 /*PRIVATE*/;
        REAL* r1_8 = &(this->inputsTPParent->r1_8_darts39[this->getLocalID()]);
        (void)r1_8 /*PRIVATE*/;
        REAL* r1_9 = &(this->inputsTPParent->r1_9_darts39[this->getLocalID()]);
        (void)r1_9 /*PRIVATE*/;
        REAL* r2_0 = &(this->inputsTPParent->r2_0_darts39[this->getLocalID()]);
        (void)r2_0 /*PRIVATE*/;
        REAL* r2_1 = &(this->inputsTPParent->r2_1_darts39[this->getLocalID()]);
        (void)r2_1 /*PRIVATE*/;
        REAL* r2_10 = &(this->inputsTPParent->r2_10_darts39[this->getLocalID()]);
        (void)r2_10 /*PRIVATE*/;
        REAL* r2_11 = &(this->inputsTPParent->r2_11_darts39[this->getLocalID()]);
        (void)r2_11 /*PRIVATE*/;
        REAL* r2_12 = &(this->inputsTPParent->r2_12_darts39[this->getLocalID()]);
        (void)r2_12 /*PRIVATE*/;
        REAL* r2_13 = &(this->inputsTPParent->r2_13_darts39[this->getLocalID()]);
        (void)r2_13 /*PRIVATE*/;
        REAL* r2_14 = &(this->inputsTPParent->r2_14_darts39[this->getLocalID()]);
        (void)r2_14 /*PRIVATE*/;
        REAL* r2_15 = &(this->inputsTPParent->r2_15_darts39[this->getLocalID()]);
        (void)r2_15 /*PRIVATE*/;
        REAL* r2_2 = &(this->inputsTPParent->r2_2_darts39[this->getLocalID()]);
        (void)r2_2 /*PRIVATE*/;
        REAL* r2_3 = &(this->inputsTPParent->r2_3_darts39[this->getLocalID()]);
        (void)r2_3 /*PRIVATE*/;
        REAL* r2_4 = &(this->inputsTPParent->r2_4_darts39[this->getLocalID()]);
        (void)r2_4 /*PRIVATE*/;
        REAL* r2_5 = &(this->inputsTPParent->r2_5_darts39[this->getLocalID()]);
        (void)r2_5 /*PRIVATE*/;
        REAL* r2_6 = &(this->inputsTPParent->r2_6_darts39[this->getLocalID()]);
        (void)r2_6 /*PRIVATE*/;
        REAL* r2_7 = &(this->inputsTPParent->r2_7_darts39[this->getLocalID()]);
        (void)r2_7 /*PRIVATE*/;
        REAL* r2_8 = &(this->inputsTPParent->r2_8_darts39[this->getLocalID()]);
        (void)r2_8 /*PRIVATE*/;
        REAL* r2_9 = &(this->inputsTPParent->r2_9_darts39[this->getLocalID()]);
        (void)r2_9 /*PRIVATE*/;
        REAL* r3_0 = &(this->inputsTPParent->r3_0_darts39[this->getLocalID()]);
        (void)r3_0 /*PRIVATE*/;
        REAL* r3_1 = &(this->inputsTPParent->r3_1_darts39[this->getLocalID()]);
        (void)r3_1 /*PRIVATE*/;
        REAL* r3_10 = &(this->inputsTPParent->r3_10_darts39[this->getLocalID()]);
        (void)r3_10 /*PRIVATE*/;
        REAL* r3_11 = &(this->inputsTPParent->r3_11_darts39[this->getLocalID()]);
        (void)r3_11 /*PRIVATE*/;
        REAL* r3_12 = &(this->inputsTPParent->r3_12_darts39[this->getLocalID()]);
        (void)r3_12 /*PRIVATE*/;
        REAL* r3_13 = &(this->inputsTPParent->r3_13_darts39[this->getLocalID()]);
        (void)r3_13 /*PRIVATE*/;
        REAL* r3_14 = &(this->inputsTPParent->r3_14_darts39[this->getLocalID()]);
        (void)r3_14 /*PRIVATE*/;
        REAL* r3_15 = &(this->inputsTPParent->r3_15_darts39[this->getLocalID()]);
        (void)r3_15 /*PRIVATE*/;
        REAL* r3_2 = &(this->inputsTPParent->r3_2_darts39[this->getLocalID()]);
        (void)r3_2 /*PRIVATE*/;
        REAL* r3_3 = &(this->inputsTPParent->r3_3_darts39[this->getLocalID()]);
        (void)r3_3 /*PRIVATE*/;
        REAL* r3_4 = &(this->inputsTPParent->r3_4_darts39[this->getLocalID()]);
        (void)r3_4 /*PRIVATE*/;
        REAL* r3_5 = &(this->inputsTPParent->r3_5_darts39[this->getLocalID()]);
        (void)r3_5 /*PRIVATE*/;
        REAL* r3_6 = &(this->inputsTPParent->r3_6_darts39[this->getLocalID()]);
        (void)r3_6 /*PRIVATE*/;
        REAL* r3_7 = &(this->inputsTPParent->r3_7_darts39[this->getLocalID()]);
        (void)r3_7 /*PRIVATE*/;
        REAL* r3_8 = &(this->inputsTPParent->r3_8_darts39[this->getLocalID()]);
        (void)r3_8 /*PRIVATE*/;
        REAL* r3_9 = &(this->inputsTPParent->r3_9_darts39[this->getLocalID()]);
        (void)r3_9 /*PRIVATE*/;
        REAL* r4_0 = &(this->inputsTPParent->r4_0_darts39[this->getLocalID()]);
        (void)r4_0 /*PRIVATE*/;
        REAL* r4_1 = &(this->inputsTPParent->r4_1_darts39[this->getLocalID()]);
        (void)r4_1 /*PRIVATE*/;
        REAL* r4_10 = &(this->inputsTPParent->r4_10_darts39[this->getLocalID()]);
        (void)r4_10 /*PRIVATE*/;
        REAL* r4_11 = &(this->inputsTPParent->r4_11_darts39[this->getLocalID()]);
        (void)r4_11 /*PRIVATE*/;
        REAL* r4_12 = &(this->inputsTPParent->r4_12_darts39[this->getLocalID()]);
        (void)r4_12 /*PRIVATE*/;
        REAL* r4_13 = &(this->inputsTPParent->r4_13_darts39[this->getLocalID()]);
        (void)r4_13 /*PRIVATE*/;
        REAL* r4_14 = &(this->inputsTPParent->r4_14_darts39[this->getLocalID()]);
        (void)r4_14 /*PRIVATE*/;
        REAL* r4_15 = &(this->inputsTPParent->r4_15_darts39[this->getLocalID()]);
        (void)r4_15 /*PRIVATE*/;
        REAL* r4_2 = &(this->inputsTPParent->r4_2_darts39[this->getLocalID()]);
        (void)r4_2 /*PRIVATE*/;
        REAL* r4_3 = &(this->inputsTPParent->r4_3_darts39[this->getLocalID()]);
        (void)r4_3 /*PRIVATE*/;
        REAL* r4_4 = &(this->inputsTPParent->r4_4_darts39[this->getLocalID()]);
        (void)r4_4 /*PRIVATE*/;
        REAL* r4_5 = &(this->inputsTPParent->r4_5_darts39[this->getLocalID()]);
        (void)r4_5 /*PRIVATE*/;
        REAL* r4_6 = &(this->inputsTPParent->r4_6_darts39[this->getLocalID()]);
        (void)r4_6 /*PRIVATE*/;
        REAL* r4_7 = &(this->inputsTPParent->r4_7_darts39[this->getLocalID()]);
        (void)r4_7 /*PRIVATE*/;
        REAL* r4_8 = &(this->inputsTPParent->r4_8_darts39[this->getLocalID()]);
        (void)r4_8 /*PRIVATE*/;
        REAL* r4_9 = &(this->inputsTPParent->r4_9_darts39[this->getLocalID()]);
        (void)r4_9 /*PRIVATE*/;
        REAL* tmpi = &(this->inputsTPParent->tmpi_darts39[this->getLocalID()]);
        (void)tmpi /*PRIVATE*/;
        REAL* tmpr = &(this->inputsTPParent->tmpr_darts39[this->getLocalID()]);
        (void)tmpr /*PRIVATE*/;
        REAL* wi = &(this->inputsTPParent->wi_darts39[this->getLocalID()]);
        (void)wi /*PRIVATE*/;
        REAL* wr = &(this->inputsTPParent->wr_darts39[this->getLocalID()]);
        (void)wr /*PRIVATE*/;
        /*Loop's init*/
        (this->inputsTPParent->i_darts39[this->getID()])
            = (this->inputsTPParent->a_darts39[this->getID()]);
        int i_darts_counter_temp39 = (this->inputsTPParent->i_darts39[this->getID()]);
        for (; (i_darts_counter_temp39) < (*b); (i_darts_counter_temp39)++) {
            (*l1) = (*nWdn) * (i_darts_counter_temp39);
            (*kp) = (*out) + (i_darts_counter_temp39);
            (*jp) = (*in) + (i_darts_counter_temp39);
            {
                {
                    {
                        {
                            (*r4_0) = (((*jp)[0 * (*m)]).re);
                            (*i4_0) = (((*jp)[0 * (*m)]).im);
                            (*wr) = (((*W)[8 * (*l1)]).re);
                            (*wi) = (((*W)[8 * (*l1)]).im);
                            (*tmpr) = (((*jp)[8 * (*m)]).re);
                            (*tmpi) = (((*jp)[8 * (*m)]).im);
                            (*r4_8) = (((*wr) * (*tmpr)) - ((*wi) * (*tmpi)));
                            (*i4_8) = (((*wi) * (*tmpr)) + ((*wr) * (*tmpi)));
                            (*r3_0) = ((*r4_0) + (*r4_8));
                            (*i3_0) = ((*i4_0) + (*i4_8));
                            (*r3_8) = ((*r4_0) - (*r4_8));
                            (*i3_8) = ((*i4_0) - (*i4_8));
                        }
                        {
                            (*wr) = (((*W)[4 * (*l1)]).re);
                            (*wi) = (((*W)[4 * (*l1)]).im);
                            (*tmpr) = (((*jp)[4 * (*m)]).re);
                            (*tmpi) = (((*jp)[4 * (*m)]).im);
                            (*r4_4) = (((*wr) * (*tmpr)) - ((*wi) * (*tmpi)));
                            (*i4_4) = (((*wi) * (*tmpr)) + ((*wr) * (*tmpi)));
                            (*wr) = (((*W)[12 * (*l1)]).re);
                            (*wi) = (((*W)[12 * (*l1)]).im);
                            (*tmpr) = (((*jp)[12 * (*m)]).re);
                            (*tmpi) = (((*jp)[12 * (*m)]).im);
                            (*r4_12) = (((*wr) * (*tmpr)) - ((*wi) * (*tmpi)));
                            (*i4_12) = (((*wi) * (*tmpr)) + ((*wr) * (*tmpi)));
                            (*r3_4) = ((*r4_4) + (*r4_12));
                            (*i3_4) = ((*i4_4) + (*i4_12));
                            (*r3_12) = ((*r4_4) - (*r4_12));
                            (*i3_12) = ((*i4_4) - (*i4_12));
                        }
                        (*r2_0) = ((*r3_0) + (*r3_4));
                        (*i2_0) = ((*i3_0) + (*i3_4));
                        (*r2_8) = ((*r3_0) - (*r3_4));
                        (*i2_8) = ((*i3_0) - (*i3_4));
                        (*r2_4) = ((*r3_8) + (*i3_12));
                        (*i2_4) = ((*i3_8) - (*r3_12));
                        (*r2_12) = ((*r3_8) - (*i3_12));
                        (*i2_12) = ((*i3_8) + (*r3_12));
                    }
                    {
                        {
                            (*wr) = (((*W)[2 * (*l1)]).re);
                            (*wi) = (((*W)[2 * (*l1)]).im);
                            (*tmpr) = (((*jp)[2 * (*m)]).re);
                            (*tmpi) = (((*jp)[2 * (*m)]).im);
                            (*r4_2) = (((*wr) * (*tmpr)) - ((*wi) * (*tmpi)));
                            (*i4_2) = (((*wi) * (*tmpr)) + ((*wr) * (*tmpi)));
                            (*wr) = (((*W)[10 * (*l1)]).re);
                            (*wi) = (((*W)[10 * (*l1)]).im);
                            (*tmpr) = (((*jp)[10 * (*m)]).re);
                            (*tmpi) = (((*jp)[10 * (*m)]).im);
                            (*r4_10) = (((*wr) * (*tmpr)) - ((*wi) * (*tmpi)));
                            (*i4_10) = (((*wi) * (*tmpr)) + ((*wr) * (*tmpi)));
                            (*r3_2) = ((*r4_2) + (*r4_10));
                            (*i3_2) = ((*i4_2) + (*i4_10));
                            (*r3_10) = ((*r4_2) - (*r4_10));
                            (*i3_10) = ((*i4_2) - (*i4_10));
                        }
                        {
                            (*wr) = (((*W)[6 * (*l1)]).re);
                            (*wi) = (((*W)[6 * (*l1)]).im);
                            (*tmpr) = (((*jp)[6 * (*m)]).re);
                            (*tmpi) = (((*jp)[6 * (*m)]).im);
                            (*r4_6) = (((*wr) * (*tmpr)) - ((*wi) * (*tmpi)));
                            (*i4_6) = (((*wi) * (*tmpr)) + ((*wr) * (*tmpi)));
                            (*wr) = (((*W)[14 * (*l1)]).re);
                            (*wi) = (((*W)[14 * (*l1)]).im);
                            (*tmpr) = (((*jp)[14 * (*m)]).re);
                            (*tmpi) = (((*jp)[14 * (*m)]).im);
                            (*r4_14) = (((*wr) * (*tmpr)) - ((*wi) * (*tmpi)));
                            (*i4_14) = (((*wi) * (*tmpr)) + ((*wr) * (*tmpi)));
                            (*r3_6) = ((*r4_6) + (*r4_14));
                            (*i3_6) = ((*i4_6) + (*i4_14));
                            (*r3_14) = ((*r4_6) - (*r4_14));
                            (*i3_14) = ((*i4_6) - (*i4_14));
                        }
                        (*r2_2) = ((*r3_2) + (*r3_6));
                        (*i2_2) = ((*i3_2) + (*i3_6));
                        (*r2_10) = ((*r3_2) - (*r3_6));
                        (*i2_10) = ((*i3_2) - (*i3_6));
                        (*r2_6) = ((*r3_10) + (*i3_14));
                        (*i2_6) = ((*i3_10) - (*r3_14));
                        (*r2_14) = ((*r3_10) - (*i3_14));
                        (*i2_14) = ((*i3_10) + (*r3_14));
                    }
                    (*r1_0) = ((*r2_0) + (*r2_2));
                    (*i1_0) = ((*i2_0) + (*i2_2));
                    (*r1_8) = ((*r2_0) - (*r2_2));
                    (*i1_8) = ((*i2_0) - (*i2_2));
                    (*tmpr) = (0.70710678118699999 * ((*r2_6) + (*i2_6)));
                    (*tmpi) = (0.70710678118699999 * ((*i2_6) - (*r2_6)));
                    (*r1_2) = ((*r2_4) + (*tmpr));
                    (*i1_2) = ((*i2_4) + (*tmpi));
                    (*r1_10) = ((*r2_4) - (*tmpr));
                    (*i1_10) = ((*i2_4) - (*tmpi));
                    (*r1_4) = ((*r2_8) + (*i2_10));
                    (*i1_4) = ((*i2_8) - (*r2_10));
                    (*r1_12) = ((*r2_8) - (*i2_10));
                    (*i1_12) = ((*i2_8) + (*r2_10));
                    (*tmpr) = (0.70710678118699999 * ((*i2_14) - (*r2_14)));
                    (*tmpi) = (0.70710678118699999 * ((*r2_14) + (*i2_14)));
                    (*r1_6) = ((*r2_12) + (*tmpr));
                    (*i1_6) = ((*i2_12) - (*tmpi));
                    (*r1_14) = ((*r2_12) - (*tmpr));
                    (*i1_14) = ((*i2_12) + (*tmpi));
                }
                {
                    {
                        {
                            (*wr) = (((*W)[1 * (*l1)]).re);
                            (*wi) = (((*W)[1 * (*l1)]).im);
                            (*tmpr) = (((*jp)[1 * (*m)]).re);
                            (*tmpi) = (((*jp)[1 * (*m)]).im);
                            (*r4_1) = (((*wr) * (*tmpr)) - ((*wi) * (*tmpi)));
                            (*i4_1) = (((*wi) * (*tmpr)) + ((*wr) * (*tmpi)));
                            (*wr) = (((*W)[9 * (*l1)]).re);
                            (*wi) = (((*W)[9 * (*l1)]).im);
                            (*tmpr) = (((*jp)[9 * (*m)]).re);
                            (*tmpi) = (((*jp)[9 * (*m)]).im);
                            (*r4_9) = (((*wr) * (*tmpr)) - ((*wi) * (*tmpi)));
                            (*i4_9) = (((*wi) * (*tmpr)) + ((*wr) * (*tmpi)));
                            (*r3_1) = ((*r4_1) + (*r4_9));
                            (*i3_1) = ((*i4_1) + (*i4_9));
                            (*r3_9) = ((*r4_1) - (*r4_9));
                            (*i3_9) = ((*i4_1) - (*i4_9));
                        }
                        {
                            (*wr) = (((*W)[5 * (*l1)]).re);
                            (*wi) = (((*W)[5 * (*l1)]).im);
                            (*tmpr) = (((*jp)[5 * (*m)]).re);
                            (*tmpi) = (((*jp)[5 * (*m)]).im);
                            (*r4_5) = (((*wr) * (*tmpr)) - ((*wi) * (*tmpi)));
                            (*i4_5) = (((*wi) * (*tmpr)) + ((*wr) * (*tmpi)));
                            (*wr) = (((*W)[13 * (*l1)]).re);
                            (*wi) = (((*W)[13 * (*l1)]).im);
                            (*tmpr) = (((*jp)[13 * (*m)]).re);
                            (*tmpi) = (((*jp)[13 * (*m)]).im);
                            (*r4_13) = (((*wr) * (*tmpr)) - ((*wi) * (*tmpi)));
                            (*i4_13) = (((*wi) * (*tmpr)) + ((*wr) * (*tmpi)));
                            (*r3_5) = ((*r4_5) + (*r4_13));
                            (*i3_5) = ((*i4_5) + (*i4_13));
                            (*r3_13) = ((*r4_5) - (*r4_13));
                            (*i3_13) = ((*i4_5) - (*i4_13));
                        }
                        (*r2_1) = ((*r3_1) + (*r3_5));
                        (*i2_1) = ((*i3_1) + (*i3_5));
                        (*r2_9) = ((*r3_1) - (*r3_5));
                        (*i2_9) = ((*i3_1) - (*i3_5));
                        (*r2_5) = ((*r3_9) + (*i3_13));
                        (*i2_5) = ((*i3_9) - (*r3_13));
                        (*r2_13) = ((*r3_9) - (*i3_13));
                        (*i2_13) = ((*i3_9) + (*r3_13));
                    }
                    {
                        {
                            (*wr) = (((*W)[3 * (*l1)]).re);
                            (*wi) = (((*W)[3 * (*l1)]).im);
                            (*tmpr) = (((*jp)[3 * (*m)]).re);
                            (*tmpi) = (((*jp)[3 * (*m)]).im);
                            (*r4_3) = (((*wr) * (*tmpr)) - ((*wi) * (*tmpi)));
                            (*i4_3) = (((*wi) * (*tmpr)) + ((*wr) * (*tmpi)));
                            (*wr) = (((*W)[11 * (*l1)]).re);
                            (*wi) = (((*W)[11 * (*l1)]).im);
                            (*tmpr) = (((*jp)[11 * (*m)]).re);
                            (*tmpi) = (((*jp)[11 * (*m)]).im);
                            (*r4_11) = (((*wr) * (*tmpr)) - ((*wi) * (*tmpi)));
                            (*i4_11) = (((*wi) * (*tmpr)) + ((*wr) * (*tmpi)));
                            (*r3_3) = ((*r4_3) + (*r4_11));
                            (*i3_3) = ((*i4_3) + (*i4_11));
                            (*r3_11) = ((*r4_3) - (*r4_11));
                            (*i3_11) = ((*i4_3) - (*i4_11));
                        }
                        {
                            (*wr) = (((*W)[7 * (*l1)]).re);
                            (*wi) = (((*W)[7 * (*l1)]).im);
                            (*tmpr) = (((*jp)[7 * (*m)]).re);
                            (*tmpi) = (((*jp)[7 * (*m)]).im);
                            (*r4_7) = (((*wr) * (*tmpr)) - ((*wi) * (*tmpi)));
                            (*i4_7) = (((*wi) * (*tmpr)) + ((*wr) * (*tmpi)));
                            (*wr) = (((*W)[15 * (*l1)]).re);
                            (*wi) = (((*W)[15 * (*l1)]).im);
                            (*tmpr) = (((*jp)[15 * (*m)]).re);
                            (*tmpi) = (((*jp)[15 * (*m)]).im);
                            (*r4_15) = (((*wr) * (*tmpr)) - ((*wi) * (*tmpi)));
                            (*i4_15) = (((*wi) * (*tmpr)) + ((*wr) * (*tmpi)));
                            (*r3_7) = ((*r4_7) + (*r4_15));
                            (*i3_7) = ((*i4_7) + (*i4_15));
                            (*r3_15) = ((*r4_7) - (*r4_15));
                            (*i3_15) = ((*i4_7) - (*i4_15));
                        }
                        (*r2_3) = ((*r3_3) + (*r3_7));
                        (*i2_3) = ((*i3_3) + (*i3_7));
                        (*r2_11) = ((*r3_3) - (*r3_7));
                        (*i2_11) = ((*i3_3) - (*i3_7));
                        (*r2_7) = ((*r3_11) + (*i3_15));
                        (*i2_7) = ((*i3_11) - (*r3_15));
                        (*r2_15) = ((*r3_11) - (*i3_15));
                        (*i2_15) = ((*i3_11) + (*r3_15));
                    }
                    (*r1_1) = ((*r2_1) + (*r2_3));
                    (*i1_1) = ((*i2_1) + (*i2_3));
                    (*r1_9) = ((*r2_1) - (*r2_3));
                    (*i1_9) = ((*i2_1) - (*i2_3));
                    (*tmpr) = (0.70710678118699999 * ((*r2_7) + (*i2_7)));
                    (*tmpi) = (0.70710678118699999 * ((*i2_7) - (*r2_7)));
                    (*r1_3) = ((*r2_5) + (*tmpr));
                    (*i1_3) = ((*i2_5) + (*tmpi));
                    (*r1_11) = ((*r2_5) - (*tmpr));
                    (*i1_11) = ((*i2_5) - (*tmpi));
                    (*r1_5) = ((*r2_9) + (*i2_11));
                    (*i1_5) = ((*i2_9) - (*r2_11));
                    (*r1_13) = ((*r2_9) - (*i2_11));
                    (*i1_13) = ((*i2_9) + (*r2_11));
                    (*tmpr) = (0.70710678118699999 * ((*i2_15) - (*r2_15)));
                    (*tmpi) = (0.70710678118699999 * ((*r2_15) + (*i2_15)));
                    (*r1_7) = ((*r2_13) + (*tmpr));
                    (*i1_7) = ((*i2_13) - (*tmpi));
                    (*r1_15) = ((*r2_13) - (*tmpr));
                    (*i1_15) = ((*i2_13) + (*tmpi));
                }
                (((*kp)[0 * (*m)]).re) = ((*r1_0) + (*r1_1));
                (((*kp)[0 * (*m)]).im) = ((*i1_0) + (*i1_1));
                (((*kp)[8 * (*m)]).re) = ((*r1_0) - (*r1_1));
                (((*kp)[8 * (*m)]).im) = ((*i1_0) - (*i1_1));
                (*tmpr) = ((0.92387953251099997 * (*r1_3)) + (0.38268343236500002 * (*i1_3)));
                (*tmpi) = ((0.92387953251099997 * (*i1_3)) - (0.38268343236500002 * (*r1_3)));
                (((*kp)[1 * (*m)]).re) = ((*r1_2) + (*tmpr));
                (((*kp)[1 * (*m)]).im) = ((*i1_2) + (*tmpi));
                (((*kp)[9 * (*m)]).re) = ((*r1_2) - (*tmpr));
                (((*kp)[9 * (*m)]).im) = ((*i1_2) - (*tmpi));
                (*tmpr) = (0.70710678118699999 * ((*r1_5) + (*i1_5)));
                (*tmpi) = (0.70710678118699999 * ((*i1_5) - (*r1_5)));
                (((*kp)[2 * (*m)]).re) = ((*r1_4) + (*tmpr));
                (((*kp)[2 * (*m)]).im) = ((*i1_4) + (*tmpi));
                (((*kp)[10 * (*m)]).re) = ((*r1_4) - (*tmpr));
                (((*kp)[10 * (*m)]).im) = ((*i1_4) - (*tmpi));
                (*tmpr) = ((0.38268343236500002 * (*r1_7)) + (0.92387953251099997 * (*i1_7)));
                (*tmpi) = ((0.38268343236500002 * (*i1_7)) - (0.92387953251099997 * (*r1_7)));
                (((*kp)[3 * (*m)]).re) = ((*r1_6) + (*tmpr));
                (((*kp)[3 * (*m)]).im) = ((*i1_6) + (*tmpi));
                (((*kp)[11 * (*m)]).re) = ((*r1_6) - (*tmpr));
                (((*kp)[11 * (*m)]).im) = ((*i1_6) - (*tmpi));
                (((*kp)[4 * (*m)]).re) = ((*r1_8) + (*i1_9));
                (((*kp)[4 * (*m)]).im) = ((*i1_8) - (*r1_9));
                (((*kp)[12 * (*m)]).re) = ((*r1_8) - (*i1_9));
                (((*kp)[12 * (*m)]).im) = ((*i1_8) + (*r1_9));
                (*tmpr) = ((0.92387953251099997 * (*i1_11)) - (0.38268343236500002 * (*r1_11)));
                (*tmpi) = ((0.92387953251099997 * (*r1_11)) + (0.38268343236500002 * (*i1_11)));
                (((*kp)[5 * (*m)]).re) = ((*r1_10) + (*tmpr));
                (((*kp)[5 * (*m)]).im) = ((*i1_10) - (*tmpi));
                (((*kp)[13 * (*m)]).re) = ((*r1_10) - (*tmpr));
                (((*kp)[13 * (*m)]).im) = ((*i1_10) + (*tmpi));
                (*tmpr) = (0.70710678118699999 * ((*i1_13) - (*r1_13)));
                (*tmpi) = (0.70710678118699999 * ((*r1_13) + (*i1_13)));
                (((*kp)[6 * (*m)]).re) = ((*r1_12) + (*tmpr));
                (((*kp)[6 * (*m)]).im) = ((*i1_12) - (*tmpi));
                (((*kp)[14 * (*m)]).re) = ((*r1_12) - (*tmpr));
                (((*kp)[14 * (*m)]).im) = ((*i1_12) + (*tmpi));
                (*tmpr) = ((0.38268343236500002 * (*i1_15)) - (0.92387953251099997 * (*r1_15)));
                (*tmpi) = ((0.38268343236500002 * (*r1_15)) + (0.92387953251099997 * (*i1_15)));
                (((*kp)[7 * (*m)]).re) = ((*r1_14) + (*tmpr));
                (((*kp)[7 * (*m)]).im) = ((*i1_14) - (*tmpi));
                (((*kp)[15 * (*m)]).re) = ((*r1_14) - (*tmpr));
                (((*kp)[15 * (*m)]).im) = ((*i1_14) + (*tmpi));
            }
            (*l1) += (*nWdn);
            (*kp)++;
        }
        (this->inputsTPParent->i_darts39[this->getID()]) = i_darts_counter_temp39;
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/

    myTP->controlTPParent->nextCodeletsfft_twiddle_16[this->getID()]->decDep();
}
void TP39::_checkInCodelets3872::fire(void)
{

    /*printing node 3872: DeclStmt*/
    this->inputsTPParent->ab_darts39[this->getID()]
        = ((this->inputsTPParent->a_darts39[this->getID()])
              + (this->inputsTPParent->b_darts39[this->getID()]))
        / 2;
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 3872 nextRegion: 3875 */
    myTP->controlTPParent->checkInCodelets3875[this->getID()].decDep();
}
void TP39::_checkInCodelets3875::fire(void)
{

    /*printing node 3875: OMPTaskDirective*/
    /*syncNode: OMPTaskwaitDirective 3879*/
    /*Find the TP to which the nextRegion belongs*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->barrierCodelets3879[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 3875*/
    _task3875Inputs* task3875Inputs
        = new _task3875Inputs(&((this->inputsTPParent->W_darts39[this->getID()])),
            &((this->inputsTPParent->a_darts39[this->getID()])),
            &((this->inputsTPParent->ab_darts39[this->getID()])),
            &((this->inputsTPParent->in_darts39[this->getID()])),
            &((this->inputsTPParent->m_darts39[this->getID()])),
            &((this->inputsTPParent->nW_darts39[this->getID()])),
            &((this->inputsTPParent->nWdn_darts39[this->getID()])),
            &((this->inputsTPParent->out_darts39[this->getID()])), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task3875Inputs[this->getID()] = task3875Inputs;
    invoke<TP3875>(myTP, 1, this->getID(), myTP, task3875Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 3875 nextRegion: 3877 */
    myTP->controlTPParent->checkInCodelets3877[this->getID()].decDep();
}
void TP39::_checkInCodelets3877::fire(void)
{

    /*printing node 3877: OMPTaskDirective*/
    /*syncNode: OMPTaskwaitDirective 3879*/
    /*Find the TP to which the nextRegion belongs*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->barrierCodelets3879[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 3877*/
    _task3877Inputs* task3877Inputs
        = new _task3877Inputs(&((this->inputsTPParent->W_darts39[this->getID()])),
            &((this->inputsTPParent->ab_darts39[this->getID()])),
            &((this->inputsTPParent->b_darts39[this->getID()])),
            &((this->inputsTPParent->in_darts39[this->getID()])),
            &((this->inputsTPParent->m_darts39[this->getID()])),
            &((this->inputsTPParent->nW_darts39[this->getID()])),
            &((this->inputsTPParent->nWdn_darts39[this->getID()])),
            &((this->inputsTPParent->out_darts39[this->getID()])), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task3877Inputs[this->getID()] = task3877Inputs;
    invoke<TP3877>(myTP, 1, this->getID(), myTP, task3877Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 3877 nextRegion: 3879 */
    myTP->controlTPParent->barrierCodelets3879[0].decDep();
}
void TP39::_barrierCodelets3879::fire(void)
{
    TP39* myTP = static_cast<TP39*>(myTP_);

    for (size_t codeletsCounter = 0; codeletsCounter < (size_t)myTP->numThreads;
         codeletsCounter++) {
        myTP->nextCodeletsfft_twiddle_16[codeletsCounter]->decDep();
    }
}
TP39::TP39(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_mainNextCodelet,
    darts::Codelet* in_mainSyncCodelet, TP39** in_ptrToThisFunctionTP, int in_a, int in_b,
    COMPLEX* in_in, COMPLEX* in_out, COMPLEX* in_W, int in_nW, int in_nWdn, int in_m)
    : ompTP(in_numThreads, in_mainCodeletID)
    , ptrToThisFunctionTP(in_ptrToThisFunctionTP)
    , inputsTPParent(this)
    , controlTPParent(this)
    , nextCodeletsfft_twiddle_16(new Codelet*[in_numThreads])
    , nextSyncCodeletsfft_twiddle_16(new Codelet*[in_numThreads])
    , a_darts39(new int[this->numThreads])
    , b_darts39(new int[this->numThreads])
    , in_darts39(new COMPLEX*[this->numThreads])
    , out_darts39(new COMPLEX*[this->numThreads])
    , W_darts39(new COMPLEX*[this->numThreads])
    , nW_darts39(new int[this->numThreads])
    , nWdn_darts39(new int[this->numThreads])
    , m_darts39(new int[this->numThreads])
    , ab_darts39(new int[this->numThreads])
    , i_darts39(new int[this->numThreads])
    , i1_0_darts39(new REAL[this->numThreads])
    , i1_1_darts39(new REAL[this->numThreads])
    , i1_10_darts39(new REAL[this->numThreads])
    , i1_11_darts39(new REAL[this->numThreads])
    , i1_12_darts39(new REAL[this->numThreads])
    , i1_13_darts39(new REAL[this->numThreads])
    , i1_14_darts39(new REAL[this->numThreads])
    , i1_15_darts39(new REAL[this->numThreads])
    , i1_2_darts39(new REAL[this->numThreads])
    , i1_3_darts39(new REAL[this->numThreads])
    , i1_4_darts39(new REAL[this->numThreads])
    , i1_5_darts39(new REAL[this->numThreads])
    , i1_6_darts39(new REAL[this->numThreads])
    , i1_7_darts39(new REAL[this->numThreads])
    , i1_8_darts39(new REAL[this->numThreads])
    , i1_9_darts39(new REAL[this->numThreads])
    , i2_0_darts39(new REAL[this->numThreads])
    , i2_1_darts39(new REAL[this->numThreads])
    , i2_10_darts39(new REAL[this->numThreads])
    , i2_11_darts39(new REAL[this->numThreads])
    , i2_12_darts39(new REAL[this->numThreads])
    , i2_13_darts39(new REAL[this->numThreads])
    , i2_14_darts39(new REAL[this->numThreads])
    , i2_15_darts39(new REAL[this->numThreads])
    , i2_2_darts39(new REAL[this->numThreads])
    , i2_3_darts39(new REAL[this->numThreads])
    , i2_4_darts39(new REAL[this->numThreads])
    , i2_5_darts39(new REAL[this->numThreads])
    , i2_6_darts39(new REAL[this->numThreads])
    , i2_7_darts39(new REAL[this->numThreads])
    , i2_8_darts39(new REAL[this->numThreads])
    , i2_9_darts39(new REAL[this->numThreads])
    , i3_0_darts39(new REAL[this->numThreads])
    , i3_1_darts39(new REAL[this->numThreads])
    , i3_10_darts39(new REAL[this->numThreads])
    , i3_11_darts39(new REAL[this->numThreads])
    , i3_12_darts39(new REAL[this->numThreads])
    , i3_13_darts39(new REAL[this->numThreads])
    , i3_14_darts39(new REAL[this->numThreads])
    , i3_15_darts39(new REAL[this->numThreads])
    , i3_2_darts39(new REAL[this->numThreads])
    , i3_3_darts39(new REAL[this->numThreads])
    , i3_4_darts39(new REAL[this->numThreads])
    , i3_5_darts39(new REAL[this->numThreads])
    , i3_6_darts39(new REAL[this->numThreads])
    , i3_7_darts39(new REAL[this->numThreads])
    , i3_8_darts39(new REAL[this->numThreads])
    , i3_9_darts39(new REAL[this->numThreads])
    , i4_0_darts39(new REAL[this->numThreads])
    , i4_1_darts39(new REAL[this->numThreads])
    , i4_10_darts39(new REAL[this->numThreads])
    , i4_11_darts39(new REAL[this->numThreads])
    , i4_12_darts39(new REAL[this->numThreads])
    , i4_13_darts39(new REAL[this->numThreads])
    , i4_14_darts39(new REAL[this->numThreads])
    , i4_15_darts39(new REAL[this->numThreads])
    , i4_2_darts39(new REAL[this->numThreads])
    , i4_3_darts39(new REAL[this->numThreads])
    , i4_4_darts39(new REAL[this->numThreads])
    , i4_5_darts39(new REAL[this->numThreads])
    , i4_6_darts39(new REAL[this->numThreads])
    , i4_7_darts39(new REAL[this->numThreads])
    , i4_8_darts39(new REAL[this->numThreads])
    , i4_9_darts39(new REAL[this->numThreads])
    , jp_darts39(new COMPLEX*[this->numThreads])
    , kp_darts39(new COMPLEX*[this->numThreads])
    , l1_darts39(new int[this->numThreads])
    , r1_0_darts39(new REAL[this->numThreads])
    , r1_1_darts39(new REAL[this->numThreads])
    , r1_10_darts39(new REAL[this->numThreads])
    , r1_11_darts39(new REAL[this->numThreads])
    , r1_12_darts39(new REAL[this->numThreads])
    , r1_13_darts39(new REAL[this->numThreads])
    , r1_14_darts39(new REAL[this->numThreads])
    , r1_15_darts39(new REAL[this->numThreads])
    , r1_2_darts39(new REAL[this->numThreads])
    , r1_3_darts39(new REAL[this->numThreads])
    , r1_4_darts39(new REAL[this->numThreads])
    , r1_5_darts39(new REAL[this->numThreads])
    , r1_6_darts39(new REAL[this->numThreads])
    , r1_7_darts39(new REAL[this->numThreads])
    , r1_8_darts39(new REAL[this->numThreads])
    , r1_9_darts39(new REAL[this->numThreads])
    , r2_0_darts39(new REAL[this->numThreads])
    , r2_1_darts39(new REAL[this->numThreads])
    , r2_10_darts39(new REAL[this->numThreads])
    , r2_11_darts39(new REAL[this->numThreads])
    , r2_12_darts39(new REAL[this->numThreads])
    , r2_13_darts39(new REAL[this->numThreads])
    , r2_14_darts39(new REAL[this->numThreads])
    , r2_15_darts39(new REAL[this->numThreads])
    , r2_2_darts39(new REAL[this->numThreads])
    , r2_3_darts39(new REAL[this->numThreads])
    , r2_4_darts39(new REAL[this->numThreads])
    , r2_5_darts39(new REAL[this->numThreads])
    , r2_6_darts39(new REAL[this->numThreads])
    , r2_7_darts39(new REAL[this->numThreads])
    , r2_8_darts39(new REAL[this->numThreads])
    , r2_9_darts39(new REAL[this->numThreads])
    , r3_0_darts39(new REAL[this->numThreads])
    , r3_1_darts39(new REAL[this->numThreads])
    , r3_10_darts39(new REAL[this->numThreads])
    , r3_11_darts39(new REAL[this->numThreads])
    , r3_12_darts39(new REAL[this->numThreads])
    , r3_13_darts39(new REAL[this->numThreads])
    , r3_14_darts39(new REAL[this->numThreads])
    , r3_15_darts39(new REAL[this->numThreads])
    , r3_2_darts39(new REAL[this->numThreads])
    , r3_3_darts39(new REAL[this->numThreads])
    , r3_4_darts39(new REAL[this->numThreads])
    , r3_5_darts39(new REAL[this->numThreads])
    , r3_6_darts39(new REAL[this->numThreads])
    , r3_7_darts39(new REAL[this->numThreads])
    , r3_8_darts39(new REAL[this->numThreads])
    , r3_9_darts39(new REAL[this->numThreads])
    , r4_0_darts39(new REAL[this->numThreads])
    , r4_1_darts39(new REAL[this->numThreads])
    , r4_10_darts39(new REAL[this->numThreads])
    , r4_11_darts39(new REAL[this->numThreads])
    , r4_12_darts39(new REAL[this->numThreads])
    , r4_13_darts39(new REAL[this->numThreads])
    , r4_14_darts39(new REAL[this->numThreads])
    , r4_15_darts39(new REAL[this->numThreads])
    , r4_2_darts39(new REAL[this->numThreads])
    , r4_3_darts39(new REAL[this->numThreads])
    , r4_4_darts39(new REAL[this->numThreads])
    , r4_5_darts39(new REAL[this->numThreads])
    , r4_6_darts39(new REAL[this->numThreads])
    , r4_7_darts39(new REAL[this->numThreads])
    , r4_8_darts39(new REAL[this->numThreads])
    , r4_9_darts39(new REAL[this->numThreads])
    , tmpi_darts39(new REAL[this->numThreads])
    , tmpr_darts39(new REAL[this->numThreads])
    , wi_darts39(new REAL[this->numThreads])
    , wr_darts39(new REAL[this->numThreads])
    , task3875Inputs(new _task3875Inputs*[this->numThreads])
    , task3877Inputs(new _task3877Inputs*[this->numThreads])
    , checkInCodelets2960(new _checkInCodelets2960[this->numThreads])
    , checkInCodelets2963(new _checkInCodelets2963[this->numThreads])
    , checkInCodelets3872(new _checkInCodelets3872[this->numThreads])
    , checkInCodelets3875(new _checkInCodelets3875[this->numThreads])
    , checkInCodelets3877(new _checkInCodelets3877[this->numThreads])
    , barrierCodelets3879(new _barrierCodelets3879[1])
{
    barrierCodelets3879[0] = _barrierCodelets3879(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets3877* checkInCodelets3877Ptr = (this->checkInCodelets3877);
    _checkInCodelets3875* checkInCodelets3875Ptr = (this->checkInCodelets3875);
    _checkInCodelets3872* checkInCodelets3872Ptr = (this->checkInCodelets3872);
    _checkInCodelets2963* checkInCodelets2963Ptr = (this->checkInCodelets2963);
    _checkInCodelets2960* checkInCodelets2960Ptr = (this->checkInCodelets2960);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets2960);
#endif
    for (size_t codeletCounter = 0; codeletCounter < this->numThreads; codeletCounter++) {
        (*checkInCodelets3877Ptr) = _checkInCodelets3877(1, 1, this, codeletCounter);
        checkInCodelets3877Ptr++;
        (*checkInCodelets3875Ptr) = _checkInCodelets3875(1, 1, this, codeletCounter);
        checkInCodelets3875Ptr++;
        (*checkInCodelets3872Ptr) = _checkInCodelets3872(1, 1, this, codeletCounter);
        checkInCodelets3872Ptr++;
        (*checkInCodelets2963Ptr) = _checkInCodelets2963(1, 1, this, codeletCounter);
        checkInCodelets2963Ptr++;
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets2960Ptr) = _checkInCodelets2960(2, 1, this, codeletCounter);
#else
        (*checkInCodelets2960Ptr) = _checkInCodelets2960(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets2960Ptr).decDep();
        checkInCodelets2960Ptr++;
    }
    if (this->numThreads == 1) {
        this->nextCodeletsfft_twiddle_16[0] = in_mainNextCodelet;
        this->nextSyncCodeletsfft_twiddle_16[0] = in_mainSyncCodelet;
        this->a_darts39[0] = in_a;
        this->b_darts39[0] = in_b;
        this->in_darts39[0] = in_in;
        this->out_darts39[0] = in_out;
        this->W_darts39[0] = in_W;
        this->nW_darts39[0] = in_nW;
        this->nWdn_darts39[0] = in_nWdn;
        this->m_darts39[0] = in_m;
        this->availableCodelets[0] = 1;
    } else {
        this->a_darts39[this->mainCodeletID] = in_a;
        this->b_darts39[this->mainCodeletID] = in_b;
        this->in_darts39[this->mainCodeletID] = in_in;
        this->out_darts39[this->mainCodeletID] = in_out;
        this->W_darts39[this->mainCodeletID] = in_W;
        this->nW_darts39[this->mainCodeletID] = in_nW;
        this->nWdn_darts39[this->mainCodeletID] = in_nWdn;
        this->m_darts39[this->mainCodeletID] = in_m;
        this->nextCodeletsfft_twiddle_16[in_mainCodeletID] = in_mainNextCodelet;
        this->nextSyncCodeletsfft_twiddle_16[in_mainCodeletID] = in_mainSyncCodelet;
#if USE_SPIN_CODELETS == 0
        this->firstCodelet[this->mainCodeletID].decDep();
#else
        this->availableCodelets[this->mainCodeletID] = 1;
#endif
        *(this->ptrToThisFunctionTP) = this;
    }
}
TP39::~TP39()
{
    delete[] task3875Inputs;
    delete[] task3877Inputs;
    delete[] barrierCodelets3879;
    delete[] checkInCodelets3877;
    delete[] checkInCodelets3875;
    delete[] checkInCodelets3872;
    delete[] checkInCodelets2963;
    delete[] checkInCodelets2960;
    delete[] nextCodeletsfft_twiddle_16;
    delete[] nextSyncCodeletsfft_twiddle_16;
    delete[] a_darts39;
    delete[] b_darts39;
    delete[] in_darts39;
    delete[] out_darts39;
    delete[] W_darts39;
    delete[] nW_darts39;
    delete[] nWdn_darts39;
    delete[] m_darts39;
    delete[] ab_darts39;
    delete[] i_darts39;
    delete[] i1_0_darts39;
    delete[] i1_1_darts39;
    delete[] i1_10_darts39;
    delete[] i1_11_darts39;
    delete[] i1_12_darts39;
    delete[] i1_13_darts39;
    delete[] i1_14_darts39;
    delete[] i1_15_darts39;
    delete[] i1_2_darts39;
    delete[] i1_3_darts39;
    delete[] i1_4_darts39;
    delete[] i1_5_darts39;
    delete[] i1_6_darts39;
    delete[] i1_7_darts39;
    delete[] i1_8_darts39;
    delete[] i1_9_darts39;
    delete[] i2_0_darts39;
    delete[] i2_1_darts39;
    delete[] i2_10_darts39;
    delete[] i2_11_darts39;
    delete[] i2_12_darts39;
    delete[] i2_13_darts39;
    delete[] i2_14_darts39;
    delete[] i2_15_darts39;
    delete[] i2_2_darts39;
    delete[] i2_3_darts39;
    delete[] i2_4_darts39;
    delete[] i2_5_darts39;
    delete[] i2_6_darts39;
    delete[] i2_7_darts39;
    delete[] i2_8_darts39;
    delete[] i2_9_darts39;
    delete[] i3_0_darts39;
    delete[] i3_1_darts39;
    delete[] i3_10_darts39;
    delete[] i3_11_darts39;
    delete[] i3_12_darts39;
    delete[] i3_13_darts39;
    delete[] i3_14_darts39;
    delete[] i3_15_darts39;
    delete[] i3_2_darts39;
    delete[] i3_3_darts39;
    delete[] i3_4_darts39;
    delete[] i3_5_darts39;
    delete[] i3_6_darts39;
    delete[] i3_7_darts39;
    delete[] i3_8_darts39;
    delete[] i3_9_darts39;
    delete[] i4_0_darts39;
    delete[] i4_1_darts39;
    delete[] i4_10_darts39;
    delete[] i4_11_darts39;
    delete[] i4_12_darts39;
    delete[] i4_13_darts39;
    delete[] i4_14_darts39;
    delete[] i4_15_darts39;
    delete[] i4_2_darts39;
    delete[] i4_3_darts39;
    delete[] i4_4_darts39;
    delete[] i4_5_darts39;
    delete[] i4_6_darts39;
    delete[] i4_7_darts39;
    delete[] i4_8_darts39;
    delete[] i4_9_darts39;
    delete[] jp_darts39;
    delete[] kp_darts39;
    delete[] l1_darts39;
    delete[] r1_0_darts39;
    delete[] r1_1_darts39;
    delete[] r1_10_darts39;
    delete[] r1_11_darts39;
    delete[] r1_12_darts39;
    delete[] r1_13_darts39;
    delete[] r1_14_darts39;
    delete[] r1_15_darts39;
    delete[] r1_2_darts39;
    delete[] r1_3_darts39;
    delete[] r1_4_darts39;
    delete[] r1_5_darts39;
    delete[] r1_6_darts39;
    delete[] r1_7_darts39;
    delete[] r1_8_darts39;
    delete[] r1_9_darts39;
    delete[] r2_0_darts39;
    delete[] r2_1_darts39;
    delete[] r2_10_darts39;
    delete[] r2_11_darts39;
    delete[] r2_12_darts39;
    delete[] r2_13_darts39;
    delete[] r2_14_darts39;
    delete[] r2_15_darts39;
    delete[] r2_2_darts39;
    delete[] r2_3_darts39;
    delete[] r2_4_darts39;
    delete[] r2_5_darts39;
    delete[] r2_6_darts39;
    delete[] r2_7_darts39;
    delete[] r2_8_darts39;
    delete[] r2_9_darts39;
    delete[] r3_0_darts39;
    delete[] r3_1_darts39;
    delete[] r3_10_darts39;
    delete[] r3_11_darts39;
    delete[] r3_12_darts39;
    delete[] r3_13_darts39;
    delete[] r3_14_darts39;
    delete[] r3_15_darts39;
    delete[] r3_2_darts39;
    delete[] r3_3_darts39;
    delete[] r3_4_darts39;
    delete[] r3_5_darts39;
    delete[] r3_6_darts39;
    delete[] r3_7_darts39;
    delete[] r3_8_darts39;
    delete[] r3_9_darts39;
    delete[] r4_0_darts39;
    delete[] r4_1_darts39;
    delete[] r4_10_darts39;
    delete[] r4_11_darts39;
    delete[] r4_12_darts39;
    delete[] r4_13_darts39;
    delete[] r4_14_darts39;
    delete[] r4_15_darts39;
    delete[] r4_2_darts39;
    delete[] r4_3_darts39;
    delete[] r4_4_darts39;
    delete[] r4_5_darts39;
    delete[] r4_6_darts39;
    delete[] r4_7_darts39;
    delete[] r4_8_darts39;
    delete[] r4_9_darts39;
    delete[] tmpi_darts39;
    delete[] tmpr_darts39;
    delete[] wi_darts39;
    delete[] wr_darts39;
}
void TP39::setNewInputs(int in_a, int in_b, COMPLEX* in_in, COMPLEX* in_out, COMPLEX* in_W,
    int in_nW, int in_nWdn, int in_m, size_t codeletID)
{
    this->a_darts39[codeletID] = in_a;
    this->b_darts39[codeletID] = in_b;
    this->in_darts39[codeletID] = in_in;
    this->out_darts39[codeletID] = in_out;
    this->W_darts39[codeletID] = in_W;
    this->nW_darts39[codeletID] = in_nW;
    this->nWdn_darts39[codeletID] = in_nWdn;
    this->m_darts39[codeletID] = in_m;
}
/*TP3875: OMPTaskDirective*/
void TP3875::_checkInCodelets3876::fire(void)
{
/*Init the vars for this region*/

/*printing node 3876: CallExpr*/
/*Finding the next codelet after the function call*/
#if 0
/*Signaling next codelet region: 3876 nextRegion: 11512 */
myTP->controlTPParent->checkInCodelets11512.decDep();
#endif
/*Finding the next sync point before making the function call*/
#if 0
/*Find the sync point for a function call*/
/*syncNode: OMPTaskwaitDirective 3879*/
/*No syncpoint found or outside task's boundaries, looking for syncCodelet*/
#endif
    /*Make the function call*/
    invoke<TP_fft_twiddle_16>(myTP, myTP->numThreads, this->getID(),
        &(myTP->controlTPParent->checkInCodelets11512),
        (myTP->controlTPParent->task3875Inputs->nextSyncCodelet), nullptr,
        (this->taskInputs->a_darts3875), (this->taskInputs->ab_darts3875),
        (this->taskInputs->in_darts3875), (this->taskInputs->out_darts3875),
        (this->taskInputs->W_darts3875), (this->taskInputs->nW_darts3875),
        (this->taskInputs->nWdn_darts3875), (this->taskInputs->m_darts3875));
}
void TP3875::_checkInCodelets11512::fire(void)
{

    /*printing node 11512: NullStmt*/
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task3875Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task3875Inputs->nextSyncCodelet->decDep();
}
TP3875::TP3875(
    int in_numThreads, int in_mainCodeletID, TP39* in_TPParent, _task3875Inputs* in_task3875Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task3875Inputs(in_task3875Inputs)
    , TP3876Ptr(nullptr)
    , TP3876_alreadyLaunched(0)
    , checkInCodelets3876(1, 1, this, this->mainCodeletID)
    , checkInCodelets11512(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets3876.decDep();
}
TP3875::~TP3875() { delete (task3875Inputs); }
/*TP3877: OMPTaskDirective*/
void TP3877::_checkInCodelets3878::fire(void)
{
/*Init the vars for this region*/

/*printing node 3878: CallExpr*/
/*Finding the next codelet after the function call*/
#if 0
/*Signaling next codelet region: 3878 nextRegion: 11513 */
myTP->controlTPParent->checkInCodelets11513.decDep();
#endif
/*Finding the next sync point before making the function call*/
#if 0
/*Find the sync point for a function call*/
/*syncNode: OMPTaskwaitDirective 3879*/
/*No syncpoint found or outside task's boundaries, looking for syncCodelet*/
#endif
    /*Make the function call*/
    invoke<TP_fft_twiddle_16>(myTP, myTP->numThreads, this->getID(),
        &(myTP->controlTPParent->checkInCodelets11513),
        (myTP->controlTPParent->task3877Inputs->nextSyncCodelet), nullptr,
        (this->taskInputs->ab_darts3877), (this->taskInputs->b_darts3877),
        (this->taskInputs->in_darts3877), (this->taskInputs->out_darts3877),
        (this->taskInputs->W_darts3877), (this->taskInputs->nW_darts3877),
        (this->taskInputs->nWdn_darts3877), (this->taskInputs->m_darts3877));
}
void TP3877::_checkInCodelets11513::fire(void)
{

    /*printing node 11513: NullStmt*/
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task3877Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task3877Inputs->nextSyncCodelet->decDep();
}
TP3877::TP3877(
    int in_numThreads, int in_mainCodeletID, TP39* in_TPParent, _task3877Inputs* in_task3877Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task3877Inputs(in_task3877Inputs)
    , TP3878Ptr(nullptr)
    , TP3878_alreadyLaunched(0)
    , checkInCodelets3878(1, 1, this, this->mainCodeletID)
    , checkInCodelets11513(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets3878.decDep();
}
TP3877::~TP3877() { delete (task3877Inputs); }
/*TP41: TP_fft_unshuffle_16*/
void TP41::_checkInCodelets4809::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    /*Init the vars for this region*/
    /*Printing conditional branch node 4809: inlining: 0*/
    if (((this->inputsTPParent->b_darts41[this->getID()])
            - (this->inputsTPParent->a_darts41[this->getID()]))
        < 128) {
        myTP->checkInCodelets4812[this->getID()].decDep();
    } else {
        /*elseRegion: DeclStmt 4893*/
        myTP->checkInCodelets4893[this->getID()].decDep();
    }
}
void TP41::_checkInCodelets4812::fire(void)
{

    /*printing node 4812: BinaryOperator*/
    (this->inputsTPParent->ip_darts41[this->getID()])
        = (this->inputsTPParent->in_darts41[this->getID()])
        + (this->inputsTPParent->a_darts41[this->getID()]) * 16;

    /*printing node 4815: ForStmt*/
    {
        int* b = &(this->inputsTPParent->b_darts41[this->getLocalID()]);
        (void)b /*PRIVATE*/;
        int* i = &(this->inputsTPParent->i_darts41[this->getLocalID()]);
        (void)i /*PRIVATE*/;
        COMPLEX** ip = &(this->inputsTPParent->ip_darts41[this->getLocalID()]);
        (void)ip /*PRIVATE*/;
        COMPLEX** jp = &(this->inputsTPParent->jp_darts41[this->getLocalID()]);
        (void)jp /*PRIVATE*/;
        int* m = &(this->inputsTPParent->m_darts41[this->getLocalID()]);
        (void)m /*PRIVATE*/;
        COMPLEX** out = &(this->inputsTPParent->out_darts41[this->getLocalID()]);
        (void)out /*PRIVATE*/;
        /*Loop's init*/
        (this->inputsTPParent->i_darts41[this->getID()])
            = (this->inputsTPParent->a_darts41[this->getID()]);
        int i_darts_counter_temp41 = (this->inputsTPParent->i_darts41[this->getID()]);
        for (; (i_darts_counter_temp41) < (*b); ++(i_darts_counter_temp41)) {
            (*jp) = (*out) + (i_darts_counter_temp41);
            (*jp)[0] = (*ip)[0];
            (*jp)[(*m)] = (*ip)[1];
            (*ip) += 2;
            (*jp) += 2 * (*m);
            (*jp)[0] = (*ip)[0];
            (*jp)[(*m)] = (*ip)[1];
            (*ip) += 2;
            (*jp) += 2 * (*m);
            (*jp)[0] = (*ip)[0];
            (*jp)[(*m)] = (*ip)[1];
            (*ip) += 2;
            (*jp) += 2 * (*m);
            (*jp)[0] = (*ip)[0];
            (*jp)[(*m)] = (*ip)[1];
            (*ip) += 2;
            (*jp) += 2 * (*m);
            (*jp)[0] = (*ip)[0];
            (*jp)[(*m)] = (*ip)[1];
            (*ip) += 2;
            (*jp) += 2 * (*m);
            (*jp)[0] = (*ip)[0];
            (*jp)[(*m)] = (*ip)[1];
            (*ip) += 2;
            (*jp) += 2 * (*m);
            (*jp)[0] = (*ip)[0];
            (*jp)[(*m)] = (*ip)[1];
            (*ip) += 2;
            (*jp) += 2 * (*m);
            (*jp)[0] = (*ip)[0];
            (*jp)[(*m)] = (*ip)[1];
            (*ip) += 2;
        }
        (this->inputsTPParent->i_darts41[this->getID()]) = i_darts_counter_temp41;
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/

    myTP->controlTPParent->nextCodeletsfft_unshuffle_16[this->getID()]->decDep();
}
void TP41::_checkInCodelets4893::fire(void)
{

    /*printing node 4893: DeclStmt*/
    this->inputsTPParent->ab_darts41[this->getID()]
        = ((this->inputsTPParent->a_darts41[this->getID()])
              + (this->inputsTPParent->b_darts41[this->getID()]))
        / 2;
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 4893 nextRegion: 4896 */
    myTP->controlTPParent->checkInCodelets4896[this->getID()].decDep();
}
void TP41::_checkInCodelets4896::fire(void)
{

    /*printing node 4896: OMPTaskDirective*/
    /*syncNode: OMPTaskwaitDirective 4900*/
    /*Find the TP to which the nextRegion belongs*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->barrierCodelets4900[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 4896*/
    _task4896Inputs* task4896Inputs
        = new _task4896Inputs(&((this->inputsTPParent->a_darts41[this->getID()])),
            &((this->inputsTPParent->ab_darts41[this->getID()])),
            &((this->inputsTPParent->in_darts41[this->getID()])),
            &((this->inputsTPParent->m_darts41[this->getID()])),
            &((this->inputsTPParent->out_darts41[this->getID()])), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task4896Inputs[this->getID()] = task4896Inputs;
    invoke<TP4896>(myTP, 1, this->getID(), myTP, task4896Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 4896 nextRegion: 4898 */
    myTP->controlTPParent->checkInCodelets4898[this->getID()].decDep();
}
void TP41::_checkInCodelets4898::fire(void)
{

    /*printing node 4898: OMPTaskDirective*/
    /*syncNode: OMPTaskwaitDirective 4900*/
    /*Find the TP to which the nextRegion belongs*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->barrierCodelets4900[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 4898*/
    _task4898Inputs* task4898Inputs
        = new _task4898Inputs(&((this->inputsTPParent->ab_darts41[this->getID()])),
            &((this->inputsTPParent->b_darts41[this->getID()])),
            &((this->inputsTPParent->in_darts41[this->getID()])),
            &((this->inputsTPParent->m_darts41[this->getID()])),
            &((this->inputsTPParent->out_darts41[this->getID()])), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task4898Inputs[this->getID()] = task4898Inputs;
    invoke<TP4898>(myTP, 1, this->getID(), myTP, task4898Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 4898 nextRegion: 4900 */
    myTP->controlTPParent->barrierCodelets4900[0].decDep();
}
void TP41::_barrierCodelets4900::fire(void)
{
    TP41* myTP = static_cast<TP41*>(myTP_);

    for (size_t codeletsCounter = 0; codeletsCounter < (size_t)myTP->numThreads;
         codeletsCounter++) {
        myTP->nextCodeletsfft_unshuffle_16[codeletsCounter]->decDep();
    }
}
TP41::TP41(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_mainNextCodelet,
    darts::Codelet* in_mainSyncCodelet, TP41** in_ptrToThisFunctionTP, int in_a, int in_b,
    COMPLEX* in_in, COMPLEX* in_out, int in_m)
    : ompTP(in_numThreads, in_mainCodeletID)
    , ptrToThisFunctionTP(in_ptrToThisFunctionTP)
    , inputsTPParent(this)
    , controlTPParent(this)
    , nextCodeletsfft_unshuffle_16(new Codelet*[in_numThreads])
    , nextSyncCodeletsfft_unshuffle_16(new Codelet*[in_numThreads])
    , a_darts41(new int[this->numThreads])
    , b_darts41(new int[this->numThreads])
    , in_darts41(new COMPLEX*[this->numThreads])
    , out_darts41(new COMPLEX*[this->numThreads])
    , m_darts41(new int[this->numThreads])
    , ab_darts41(new int[this->numThreads])
    , i_darts41(new int[this->numThreads])
    , ip_darts41(new COMPLEX*[this->numThreads])
    , jp_darts41(new COMPLEX*[this->numThreads])
    , task4896Inputs(new _task4896Inputs*[this->numThreads])
    , task4898Inputs(new _task4898Inputs*[this->numThreads])
    , checkInCodelets4809(new _checkInCodelets4809[this->numThreads])
    , checkInCodelets4812(new _checkInCodelets4812[this->numThreads])
    , checkInCodelets4893(new _checkInCodelets4893[this->numThreads])
    , checkInCodelets4896(new _checkInCodelets4896[this->numThreads])
    , checkInCodelets4898(new _checkInCodelets4898[this->numThreads])
    , barrierCodelets4900(new _barrierCodelets4900[1])
{
    barrierCodelets4900[0] = _barrierCodelets4900(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets4898* checkInCodelets4898Ptr = (this->checkInCodelets4898);
    _checkInCodelets4896* checkInCodelets4896Ptr = (this->checkInCodelets4896);
    _checkInCodelets4893* checkInCodelets4893Ptr = (this->checkInCodelets4893);
    _checkInCodelets4812* checkInCodelets4812Ptr = (this->checkInCodelets4812);
    _checkInCodelets4809* checkInCodelets4809Ptr = (this->checkInCodelets4809);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets4809);
#endif
    for (size_t codeletCounter = 0; codeletCounter < this->numThreads; codeletCounter++) {
        (*checkInCodelets4898Ptr) = _checkInCodelets4898(1, 1, this, codeletCounter);
        checkInCodelets4898Ptr++;
        (*checkInCodelets4896Ptr) = _checkInCodelets4896(1, 1, this, codeletCounter);
        checkInCodelets4896Ptr++;
        (*checkInCodelets4893Ptr) = _checkInCodelets4893(1, 1, this, codeletCounter);
        checkInCodelets4893Ptr++;
        (*checkInCodelets4812Ptr) = _checkInCodelets4812(1, 1, this, codeletCounter);
        checkInCodelets4812Ptr++;
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets4809Ptr) = _checkInCodelets4809(2, 1, this, codeletCounter);
#else
        (*checkInCodelets4809Ptr) = _checkInCodelets4809(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets4809Ptr).decDep();
        checkInCodelets4809Ptr++;
    }
    if (this->numThreads == 1) {
        this->nextCodeletsfft_unshuffle_16[0] = in_mainNextCodelet;
        this->nextSyncCodeletsfft_unshuffle_16[0] = in_mainSyncCodelet;
        this->a_darts41[0] = in_a;
        this->b_darts41[0] = in_b;
        this->in_darts41[0] = in_in;
        this->out_darts41[0] = in_out;
        this->m_darts41[0] = in_m;
        this->availableCodelets[0] = 1;
    } else {
        this->a_darts41[this->mainCodeletID] = in_a;
        this->b_darts41[this->mainCodeletID] = in_b;
        this->in_darts41[this->mainCodeletID] = in_in;
        this->out_darts41[this->mainCodeletID] = in_out;
        this->m_darts41[this->mainCodeletID] = in_m;
        this->nextCodeletsfft_unshuffle_16[in_mainCodeletID] = in_mainNextCodelet;
        this->nextSyncCodeletsfft_unshuffle_16[in_mainCodeletID] = in_mainSyncCodelet;
#if USE_SPIN_CODELETS == 0
        this->firstCodelet[this->mainCodeletID].decDep();
#else
        this->availableCodelets[this->mainCodeletID] = 1;
#endif
        *(this->ptrToThisFunctionTP) = this;
    }
}
TP41::~TP41()
{
    delete[] task4896Inputs;
    delete[] task4898Inputs;
    delete[] barrierCodelets4900;
    delete[] checkInCodelets4898;
    delete[] checkInCodelets4896;
    delete[] checkInCodelets4893;
    delete[] checkInCodelets4812;
    delete[] checkInCodelets4809;
    delete[] nextCodeletsfft_unshuffle_16;
    delete[] nextSyncCodeletsfft_unshuffle_16;
    delete[] a_darts41;
    delete[] b_darts41;
    delete[] in_darts41;
    delete[] out_darts41;
    delete[] m_darts41;
    delete[] ab_darts41;
    delete[] i_darts41;
    delete[] ip_darts41;
    delete[] jp_darts41;
}
void TP41::setNewInputs(
    int in_a, int in_b, COMPLEX* in_in, COMPLEX* in_out, int in_m, size_t codeletID)
{
    this->a_darts41[codeletID] = in_a;
    this->b_darts41[codeletID] = in_b;
    this->in_darts41[codeletID] = in_in;
    this->out_darts41[codeletID] = in_out;
    this->m_darts41[codeletID] = in_m;
}
/*TP4896: OMPTaskDirective*/
void TP4896::_checkInCodelets4897::fire(void)
{
/*Init the vars for this region*/

/*printing node 4897: CallExpr*/
/*Finding the next codelet after the function call*/
#if 0
/*Signaling next codelet region: 4897 nextRegion: 11522 */
myTP->controlTPParent->checkInCodelets11522.decDep();
#endif
/*Finding the next sync point before making the function call*/
#if 0
/*Find the sync point for a function call*/
/*syncNode: OMPTaskwaitDirective 4900*/
/*No syncpoint found or outside task's boundaries, looking for syncCodelet*/
#endif
    /*Make the function call*/
    invoke<TP_fft_unshuffle_16>(myTP, myTP->numThreads, this->getID(),
        &(myTP->controlTPParent->checkInCodelets11522),
        (myTP->controlTPParent->task4896Inputs->nextSyncCodelet), nullptr,
        (this->taskInputs->a_darts4896), (this->taskInputs->ab_darts4896),
        (this->taskInputs->in_darts4896), (this->taskInputs->out_darts4896),
        (this->taskInputs->m_darts4896));
}
void TP4896::_checkInCodelets11522::fire(void)
{

    /*printing node 11522: NullStmt*/
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task4896Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task4896Inputs->nextSyncCodelet->decDep();
}
TP4896::TP4896(
    int in_numThreads, int in_mainCodeletID, TP41* in_TPParent, _task4896Inputs* in_task4896Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task4896Inputs(in_task4896Inputs)
    , TP4897Ptr(nullptr)
    , TP4897_alreadyLaunched(0)
    , checkInCodelets4897(1, 1, this, this->mainCodeletID)
    , checkInCodelets11522(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets4897.decDep();
}
TP4896::~TP4896() { delete (task4896Inputs); }
/*TP4898: OMPTaskDirective*/
void TP4898::_checkInCodelets4899::fire(void)
{
/*Init the vars for this region*/

/*printing node 4899: CallExpr*/
/*Finding the next codelet after the function call*/
#if 0
/*Signaling next codelet region: 4899 nextRegion: 11523 */
myTP->controlTPParent->checkInCodelets11523.decDep();
#endif
/*Finding the next sync point before making the function call*/
#if 0
/*Find the sync point for a function call*/
/*syncNode: OMPTaskwaitDirective 4900*/
/*No syncpoint found or outside task's boundaries, looking for syncCodelet*/
#endif
    /*Make the function call*/
    invoke<TP_fft_unshuffle_16>(myTP, myTP->numThreads, this->getID(),
        &(myTP->controlTPParent->checkInCodelets11523),
        (myTP->controlTPParent->task4898Inputs->nextSyncCodelet), nullptr,
        (this->taskInputs->ab_darts4898), (this->taskInputs->b_darts4898),
        (this->taskInputs->in_darts4898), (this->taskInputs->out_darts4898),
        (this->taskInputs->m_darts4898));
}
void TP4898::_checkInCodelets11523::fire(void)
{

    /*printing node 11523: NullStmt*/
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task4898Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task4898Inputs->nextSyncCodelet->decDep();
}
TP4898::TP4898(
    int in_numThreads, int in_mainCodeletID, TP41* in_TPParent, _task4898Inputs* in_task4898Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task4898Inputs(in_task4898Inputs)
    , TP4899Ptr(nullptr)
    , TP4899_alreadyLaunched(0)
    , checkInCodelets4899(1, 1, this, this->mainCodeletID)
    , checkInCodelets11523(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets4899.decDep();
}
TP4898::~TP4898() { delete (task4898Inputs); }
/*TP44: TP_fft_twiddle_32*/
void TP44::_checkInCodelets6503::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    /*Init the vars for this region*/
    /*Printing conditional branch node 6503: inlining: 0*/
    if (((this->inputsTPParent->b_darts44[this->getID()])
            - (this->inputsTPParent->a_darts44[this->getID()]))
        < 128) {
        myTP->checkInCodelets6506[this->getID()].decDep();
    } else {
        /*elseRegion: DeclStmt 8647*/
        myTP->checkInCodelets8647[this->getID()].decDep();
    }
}
void TP44::_checkInCodelets6506::fire(void)
{

    /*printing node 6506: ForStmt*/
    {
        COMPLEX** W = &(this->inputsTPParent->W_darts44[this->getLocalID()]);
        (void)W /*PRIVATE*/;
        int* b = &(this->inputsTPParent->b_darts44[this->getLocalID()]);
        (void)b /*PRIVATE*/;
        int* i = &(this->inputsTPParent->i_darts44[this->getLocalID()]);
        (void)i /*PRIVATE*/;
        REAL* i1_0 = &(this->inputsTPParent->i1_0_darts44[this->getLocalID()]);
        (void)i1_0 /*PRIVATE*/;
        REAL* i1_1 = &(this->inputsTPParent->i1_1_darts44[this->getLocalID()]);
        (void)i1_1 /*PRIVATE*/;
        REAL* i1_10 = &(this->inputsTPParent->i1_10_darts44[this->getLocalID()]);
        (void)i1_10 /*PRIVATE*/;
        REAL* i1_11 = &(this->inputsTPParent->i1_11_darts44[this->getLocalID()]);
        (void)i1_11 /*PRIVATE*/;
        REAL* i1_12 = &(this->inputsTPParent->i1_12_darts44[this->getLocalID()]);
        (void)i1_12 /*PRIVATE*/;
        REAL* i1_13 = &(this->inputsTPParent->i1_13_darts44[this->getLocalID()]);
        (void)i1_13 /*PRIVATE*/;
        REAL* i1_14 = &(this->inputsTPParent->i1_14_darts44[this->getLocalID()]);
        (void)i1_14 /*PRIVATE*/;
        REAL* i1_15 = &(this->inputsTPParent->i1_15_darts44[this->getLocalID()]);
        (void)i1_15 /*PRIVATE*/;
        REAL* i1_16 = &(this->inputsTPParent->i1_16_darts44[this->getLocalID()]);
        (void)i1_16 /*PRIVATE*/;
        REAL* i1_17 = &(this->inputsTPParent->i1_17_darts44[this->getLocalID()]);
        (void)i1_17 /*PRIVATE*/;
        REAL* i1_18 = &(this->inputsTPParent->i1_18_darts44[this->getLocalID()]);
        (void)i1_18 /*PRIVATE*/;
        REAL* i1_19 = &(this->inputsTPParent->i1_19_darts44[this->getLocalID()]);
        (void)i1_19 /*PRIVATE*/;
        REAL* i1_2 = &(this->inputsTPParent->i1_2_darts44[this->getLocalID()]);
        (void)i1_2 /*PRIVATE*/;
        REAL* i1_20 = &(this->inputsTPParent->i1_20_darts44[this->getLocalID()]);
        (void)i1_20 /*PRIVATE*/;
        REAL* i1_21 = &(this->inputsTPParent->i1_21_darts44[this->getLocalID()]);
        (void)i1_21 /*PRIVATE*/;
        REAL* i1_22 = &(this->inputsTPParent->i1_22_darts44[this->getLocalID()]);
        (void)i1_22 /*PRIVATE*/;
        REAL* i1_23 = &(this->inputsTPParent->i1_23_darts44[this->getLocalID()]);
        (void)i1_23 /*PRIVATE*/;
        REAL* i1_24 = &(this->inputsTPParent->i1_24_darts44[this->getLocalID()]);
        (void)i1_24 /*PRIVATE*/;
        REAL* i1_25 = &(this->inputsTPParent->i1_25_darts44[this->getLocalID()]);
        (void)i1_25 /*PRIVATE*/;
        REAL* i1_26 = &(this->inputsTPParent->i1_26_darts44[this->getLocalID()]);
        (void)i1_26 /*PRIVATE*/;
        REAL* i1_27 = &(this->inputsTPParent->i1_27_darts44[this->getLocalID()]);
        (void)i1_27 /*PRIVATE*/;
        REAL* i1_28 = &(this->inputsTPParent->i1_28_darts44[this->getLocalID()]);
        (void)i1_28 /*PRIVATE*/;
        REAL* i1_29 = &(this->inputsTPParent->i1_29_darts44[this->getLocalID()]);
        (void)i1_29 /*PRIVATE*/;
        REAL* i1_3 = &(this->inputsTPParent->i1_3_darts44[this->getLocalID()]);
        (void)i1_3 /*PRIVATE*/;
        REAL* i1_30 = &(this->inputsTPParent->i1_30_darts44[this->getLocalID()]);
        (void)i1_30 /*PRIVATE*/;
        REAL* i1_31 = &(this->inputsTPParent->i1_31_darts44[this->getLocalID()]);
        (void)i1_31 /*PRIVATE*/;
        REAL* i1_4 = &(this->inputsTPParent->i1_4_darts44[this->getLocalID()]);
        (void)i1_4 /*PRIVATE*/;
        REAL* i1_5 = &(this->inputsTPParent->i1_5_darts44[this->getLocalID()]);
        (void)i1_5 /*PRIVATE*/;
        REAL* i1_6 = &(this->inputsTPParent->i1_6_darts44[this->getLocalID()]);
        (void)i1_6 /*PRIVATE*/;
        REAL* i1_7 = &(this->inputsTPParent->i1_7_darts44[this->getLocalID()]);
        (void)i1_7 /*PRIVATE*/;
        REAL* i1_8 = &(this->inputsTPParent->i1_8_darts44[this->getLocalID()]);
        (void)i1_8 /*PRIVATE*/;
        REAL* i1_9 = &(this->inputsTPParent->i1_9_darts44[this->getLocalID()]);
        (void)i1_9 /*PRIVATE*/;
        REAL* i2_0 = &(this->inputsTPParent->i2_0_darts44[this->getLocalID()]);
        (void)i2_0 /*PRIVATE*/;
        REAL* i2_1 = &(this->inputsTPParent->i2_1_darts44[this->getLocalID()]);
        (void)i2_1 /*PRIVATE*/;
        REAL* i2_10 = &(this->inputsTPParent->i2_10_darts44[this->getLocalID()]);
        (void)i2_10 /*PRIVATE*/;
        REAL* i2_11 = &(this->inputsTPParent->i2_11_darts44[this->getLocalID()]);
        (void)i2_11 /*PRIVATE*/;
        REAL* i2_12 = &(this->inputsTPParent->i2_12_darts44[this->getLocalID()]);
        (void)i2_12 /*PRIVATE*/;
        REAL* i2_13 = &(this->inputsTPParent->i2_13_darts44[this->getLocalID()]);
        (void)i2_13 /*PRIVATE*/;
        REAL* i2_14 = &(this->inputsTPParent->i2_14_darts44[this->getLocalID()]);
        (void)i2_14 /*PRIVATE*/;
        REAL* i2_15 = &(this->inputsTPParent->i2_15_darts44[this->getLocalID()]);
        (void)i2_15 /*PRIVATE*/;
        REAL* i2_16 = &(this->inputsTPParent->i2_16_darts44[this->getLocalID()]);
        (void)i2_16 /*PRIVATE*/;
        REAL* i2_17 = &(this->inputsTPParent->i2_17_darts44[this->getLocalID()]);
        (void)i2_17 /*PRIVATE*/;
        REAL* i2_18 = &(this->inputsTPParent->i2_18_darts44[this->getLocalID()]);
        (void)i2_18 /*PRIVATE*/;
        REAL* i2_19 = &(this->inputsTPParent->i2_19_darts44[this->getLocalID()]);
        (void)i2_19 /*PRIVATE*/;
        REAL* i2_2 = &(this->inputsTPParent->i2_2_darts44[this->getLocalID()]);
        (void)i2_2 /*PRIVATE*/;
        REAL* i2_20 = &(this->inputsTPParent->i2_20_darts44[this->getLocalID()]);
        (void)i2_20 /*PRIVATE*/;
        REAL* i2_21 = &(this->inputsTPParent->i2_21_darts44[this->getLocalID()]);
        (void)i2_21 /*PRIVATE*/;
        REAL* i2_22 = &(this->inputsTPParent->i2_22_darts44[this->getLocalID()]);
        (void)i2_22 /*PRIVATE*/;
        REAL* i2_23 = &(this->inputsTPParent->i2_23_darts44[this->getLocalID()]);
        (void)i2_23 /*PRIVATE*/;
        REAL* i2_24 = &(this->inputsTPParent->i2_24_darts44[this->getLocalID()]);
        (void)i2_24 /*PRIVATE*/;
        REAL* i2_25 = &(this->inputsTPParent->i2_25_darts44[this->getLocalID()]);
        (void)i2_25 /*PRIVATE*/;
        REAL* i2_26 = &(this->inputsTPParent->i2_26_darts44[this->getLocalID()]);
        (void)i2_26 /*PRIVATE*/;
        REAL* i2_27 = &(this->inputsTPParent->i2_27_darts44[this->getLocalID()]);
        (void)i2_27 /*PRIVATE*/;
        REAL* i2_28 = &(this->inputsTPParent->i2_28_darts44[this->getLocalID()]);
        (void)i2_28 /*PRIVATE*/;
        REAL* i2_29 = &(this->inputsTPParent->i2_29_darts44[this->getLocalID()]);
        (void)i2_29 /*PRIVATE*/;
        REAL* i2_3 = &(this->inputsTPParent->i2_3_darts44[this->getLocalID()]);
        (void)i2_3 /*PRIVATE*/;
        REAL* i2_30 = &(this->inputsTPParent->i2_30_darts44[this->getLocalID()]);
        (void)i2_30 /*PRIVATE*/;
        REAL* i2_31 = &(this->inputsTPParent->i2_31_darts44[this->getLocalID()]);
        (void)i2_31 /*PRIVATE*/;
        REAL* i2_4 = &(this->inputsTPParent->i2_4_darts44[this->getLocalID()]);
        (void)i2_4 /*PRIVATE*/;
        REAL* i2_5 = &(this->inputsTPParent->i2_5_darts44[this->getLocalID()]);
        (void)i2_5 /*PRIVATE*/;
        REAL* i2_6 = &(this->inputsTPParent->i2_6_darts44[this->getLocalID()]);
        (void)i2_6 /*PRIVATE*/;
        REAL* i2_7 = &(this->inputsTPParent->i2_7_darts44[this->getLocalID()]);
        (void)i2_7 /*PRIVATE*/;
        REAL* i2_8 = &(this->inputsTPParent->i2_8_darts44[this->getLocalID()]);
        (void)i2_8 /*PRIVATE*/;
        REAL* i2_9 = &(this->inputsTPParent->i2_9_darts44[this->getLocalID()]);
        (void)i2_9 /*PRIVATE*/;
        REAL* i3_0 = &(this->inputsTPParent->i3_0_darts44[this->getLocalID()]);
        (void)i3_0 /*PRIVATE*/;
        REAL* i3_1 = &(this->inputsTPParent->i3_1_darts44[this->getLocalID()]);
        (void)i3_1 /*PRIVATE*/;
        REAL* i3_10 = &(this->inputsTPParent->i3_10_darts44[this->getLocalID()]);
        (void)i3_10 /*PRIVATE*/;
        REAL* i3_11 = &(this->inputsTPParent->i3_11_darts44[this->getLocalID()]);
        (void)i3_11 /*PRIVATE*/;
        REAL* i3_12 = &(this->inputsTPParent->i3_12_darts44[this->getLocalID()]);
        (void)i3_12 /*PRIVATE*/;
        REAL* i3_13 = &(this->inputsTPParent->i3_13_darts44[this->getLocalID()]);
        (void)i3_13 /*PRIVATE*/;
        REAL* i3_14 = &(this->inputsTPParent->i3_14_darts44[this->getLocalID()]);
        (void)i3_14 /*PRIVATE*/;
        REAL* i3_15 = &(this->inputsTPParent->i3_15_darts44[this->getLocalID()]);
        (void)i3_15 /*PRIVATE*/;
        REAL* i3_16 = &(this->inputsTPParent->i3_16_darts44[this->getLocalID()]);
        (void)i3_16 /*PRIVATE*/;
        REAL* i3_17 = &(this->inputsTPParent->i3_17_darts44[this->getLocalID()]);
        (void)i3_17 /*PRIVATE*/;
        REAL* i3_18 = &(this->inputsTPParent->i3_18_darts44[this->getLocalID()]);
        (void)i3_18 /*PRIVATE*/;
        REAL* i3_19 = &(this->inputsTPParent->i3_19_darts44[this->getLocalID()]);
        (void)i3_19 /*PRIVATE*/;
        REAL* i3_2 = &(this->inputsTPParent->i3_2_darts44[this->getLocalID()]);
        (void)i3_2 /*PRIVATE*/;
        REAL* i3_20 = &(this->inputsTPParent->i3_20_darts44[this->getLocalID()]);
        (void)i3_20 /*PRIVATE*/;
        REAL* i3_21 = &(this->inputsTPParent->i3_21_darts44[this->getLocalID()]);
        (void)i3_21 /*PRIVATE*/;
        REAL* i3_22 = &(this->inputsTPParent->i3_22_darts44[this->getLocalID()]);
        (void)i3_22 /*PRIVATE*/;
        REAL* i3_23 = &(this->inputsTPParent->i3_23_darts44[this->getLocalID()]);
        (void)i3_23 /*PRIVATE*/;
        REAL* i3_24 = &(this->inputsTPParent->i3_24_darts44[this->getLocalID()]);
        (void)i3_24 /*PRIVATE*/;
        REAL* i3_25 = &(this->inputsTPParent->i3_25_darts44[this->getLocalID()]);
        (void)i3_25 /*PRIVATE*/;
        REAL* i3_26 = &(this->inputsTPParent->i3_26_darts44[this->getLocalID()]);
        (void)i3_26 /*PRIVATE*/;
        REAL* i3_27 = &(this->inputsTPParent->i3_27_darts44[this->getLocalID()]);
        (void)i3_27 /*PRIVATE*/;
        REAL* i3_28 = &(this->inputsTPParent->i3_28_darts44[this->getLocalID()]);
        (void)i3_28 /*PRIVATE*/;
        REAL* i3_29 = &(this->inputsTPParent->i3_29_darts44[this->getLocalID()]);
        (void)i3_29 /*PRIVATE*/;
        REAL* i3_3 = &(this->inputsTPParent->i3_3_darts44[this->getLocalID()]);
        (void)i3_3 /*PRIVATE*/;
        REAL* i3_30 = &(this->inputsTPParent->i3_30_darts44[this->getLocalID()]);
        (void)i3_30 /*PRIVATE*/;
        REAL* i3_31 = &(this->inputsTPParent->i3_31_darts44[this->getLocalID()]);
        (void)i3_31 /*PRIVATE*/;
        REAL* i3_4 = &(this->inputsTPParent->i3_4_darts44[this->getLocalID()]);
        (void)i3_4 /*PRIVATE*/;
        REAL* i3_5 = &(this->inputsTPParent->i3_5_darts44[this->getLocalID()]);
        (void)i3_5 /*PRIVATE*/;
        REAL* i3_6 = &(this->inputsTPParent->i3_6_darts44[this->getLocalID()]);
        (void)i3_6 /*PRIVATE*/;
        REAL* i3_7 = &(this->inputsTPParent->i3_7_darts44[this->getLocalID()]);
        (void)i3_7 /*PRIVATE*/;
        REAL* i3_8 = &(this->inputsTPParent->i3_8_darts44[this->getLocalID()]);
        (void)i3_8 /*PRIVATE*/;
        REAL* i3_9 = &(this->inputsTPParent->i3_9_darts44[this->getLocalID()]);
        (void)i3_9 /*PRIVATE*/;
        REAL* i4_0 = &(this->inputsTPParent->i4_0_darts44[this->getLocalID()]);
        (void)i4_0 /*PRIVATE*/;
        REAL* i4_1 = &(this->inputsTPParent->i4_1_darts44[this->getLocalID()]);
        (void)i4_1 /*PRIVATE*/;
        REAL* i4_10 = &(this->inputsTPParent->i4_10_darts44[this->getLocalID()]);
        (void)i4_10 /*PRIVATE*/;
        REAL* i4_11 = &(this->inputsTPParent->i4_11_darts44[this->getLocalID()]);
        (void)i4_11 /*PRIVATE*/;
        REAL* i4_12 = &(this->inputsTPParent->i4_12_darts44[this->getLocalID()]);
        (void)i4_12 /*PRIVATE*/;
        REAL* i4_13 = &(this->inputsTPParent->i4_13_darts44[this->getLocalID()]);
        (void)i4_13 /*PRIVATE*/;
        REAL* i4_14 = &(this->inputsTPParent->i4_14_darts44[this->getLocalID()]);
        (void)i4_14 /*PRIVATE*/;
        REAL* i4_15 = &(this->inputsTPParent->i4_15_darts44[this->getLocalID()]);
        (void)i4_15 /*PRIVATE*/;
        REAL* i4_16 = &(this->inputsTPParent->i4_16_darts44[this->getLocalID()]);
        (void)i4_16 /*PRIVATE*/;
        REAL* i4_17 = &(this->inputsTPParent->i4_17_darts44[this->getLocalID()]);
        (void)i4_17 /*PRIVATE*/;
        REAL* i4_18 = &(this->inputsTPParent->i4_18_darts44[this->getLocalID()]);
        (void)i4_18 /*PRIVATE*/;
        REAL* i4_19 = &(this->inputsTPParent->i4_19_darts44[this->getLocalID()]);
        (void)i4_19 /*PRIVATE*/;
        REAL* i4_2 = &(this->inputsTPParent->i4_2_darts44[this->getLocalID()]);
        (void)i4_2 /*PRIVATE*/;
        REAL* i4_20 = &(this->inputsTPParent->i4_20_darts44[this->getLocalID()]);
        (void)i4_20 /*PRIVATE*/;
        REAL* i4_21 = &(this->inputsTPParent->i4_21_darts44[this->getLocalID()]);
        (void)i4_21 /*PRIVATE*/;
        REAL* i4_22 = &(this->inputsTPParent->i4_22_darts44[this->getLocalID()]);
        (void)i4_22 /*PRIVATE*/;
        REAL* i4_23 = &(this->inputsTPParent->i4_23_darts44[this->getLocalID()]);
        (void)i4_23 /*PRIVATE*/;
        REAL* i4_24 = &(this->inputsTPParent->i4_24_darts44[this->getLocalID()]);
        (void)i4_24 /*PRIVATE*/;
        REAL* i4_25 = &(this->inputsTPParent->i4_25_darts44[this->getLocalID()]);
        (void)i4_25 /*PRIVATE*/;
        REAL* i4_26 = &(this->inputsTPParent->i4_26_darts44[this->getLocalID()]);
        (void)i4_26 /*PRIVATE*/;
        REAL* i4_27 = &(this->inputsTPParent->i4_27_darts44[this->getLocalID()]);
        (void)i4_27 /*PRIVATE*/;
        REAL* i4_28 = &(this->inputsTPParent->i4_28_darts44[this->getLocalID()]);
        (void)i4_28 /*PRIVATE*/;
        REAL* i4_29 = &(this->inputsTPParent->i4_29_darts44[this->getLocalID()]);
        (void)i4_29 /*PRIVATE*/;
        REAL* i4_3 = &(this->inputsTPParent->i4_3_darts44[this->getLocalID()]);
        (void)i4_3 /*PRIVATE*/;
        REAL* i4_30 = &(this->inputsTPParent->i4_30_darts44[this->getLocalID()]);
        (void)i4_30 /*PRIVATE*/;
        REAL* i4_31 = &(this->inputsTPParent->i4_31_darts44[this->getLocalID()]);
        (void)i4_31 /*PRIVATE*/;
        REAL* i4_4 = &(this->inputsTPParent->i4_4_darts44[this->getLocalID()]);
        (void)i4_4 /*PRIVATE*/;
        REAL* i4_5 = &(this->inputsTPParent->i4_5_darts44[this->getLocalID()]);
        (void)i4_5 /*PRIVATE*/;
        REAL* i4_6 = &(this->inputsTPParent->i4_6_darts44[this->getLocalID()]);
        (void)i4_6 /*PRIVATE*/;
        REAL* i4_7 = &(this->inputsTPParent->i4_7_darts44[this->getLocalID()]);
        (void)i4_7 /*PRIVATE*/;
        REAL* i4_8 = &(this->inputsTPParent->i4_8_darts44[this->getLocalID()]);
        (void)i4_8 /*PRIVATE*/;
        REAL* i4_9 = &(this->inputsTPParent->i4_9_darts44[this->getLocalID()]);
        (void)i4_9 /*PRIVATE*/;
        REAL* i5_0 = &(this->inputsTPParent->i5_0_darts44[this->getLocalID()]);
        (void)i5_0 /*PRIVATE*/;
        REAL* i5_1 = &(this->inputsTPParent->i5_1_darts44[this->getLocalID()]);
        (void)i5_1 /*PRIVATE*/;
        REAL* i5_10 = &(this->inputsTPParent->i5_10_darts44[this->getLocalID()]);
        (void)i5_10 /*PRIVATE*/;
        REAL* i5_11 = &(this->inputsTPParent->i5_11_darts44[this->getLocalID()]);
        (void)i5_11 /*PRIVATE*/;
        REAL* i5_12 = &(this->inputsTPParent->i5_12_darts44[this->getLocalID()]);
        (void)i5_12 /*PRIVATE*/;
        REAL* i5_13 = &(this->inputsTPParent->i5_13_darts44[this->getLocalID()]);
        (void)i5_13 /*PRIVATE*/;
        REAL* i5_14 = &(this->inputsTPParent->i5_14_darts44[this->getLocalID()]);
        (void)i5_14 /*PRIVATE*/;
        REAL* i5_15 = &(this->inputsTPParent->i5_15_darts44[this->getLocalID()]);
        (void)i5_15 /*PRIVATE*/;
        REAL* i5_16 = &(this->inputsTPParent->i5_16_darts44[this->getLocalID()]);
        (void)i5_16 /*PRIVATE*/;
        REAL* i5_17 = &(this->inputsTPParent->i5_17_darts44[this->getLocalID()]);
        (void)i5_17 /*PRIVATE*/;
        REAL* i5_18 = &(this->inputsTPParent->i5_18_darts44[this->getLocalID()]);
        (void)i5_18 /*PRIVATE*/;
        REAL* i5_19 = &(this->inputsTPParent->i5_19_darts44[this->getLocalID()]);
        (void)i5_19 /*PRIVATE*/;
        REAL* i5_2 = &(this->inputsTPParent->i5_2_darts44[this->getLocalID()]);
        (void)i5_2 /*PRIVATE*/;
        REAL* i5_20 = &(this->inputsTPParent->i5_20_darts44[this->getLocalID()]);
        (void)i5_20 /*PRIVATE*/;
        REAL* i5_21 = &(this->inputsTPParent->i5_21_darts44[this->getLocalID()]);
        (void)i5_21 /*PRIVATE*/;
        REAL* i5_22 = &(this->inputsTPParent->i5_22_darts44[this->getLocalID()]);
        (void)i5_22 /*PRIVATE*/;
        REAL* i5_23 = &(this->inputsTPParent->i5_23_darts44[this->getLocalID()]);
        (void)i5_23 /*PRIVATE*/;
        REAL* i5_24 = &(this->inputsTPParent->i5_24_darts44[this->getLocalID()]);
        (void)i5_24 /*PRIVATE*/;
        REAL* i5_25 = &(this->inputsTPParent->i5_25_darts44[this->getLocalID()]);
        (void)i5_25 /*PRIVATE*/;
        REAL* i5_26 = &(this->inputsTPParent->i5_26_darts44[this->getLocalID()]);
        (void)i5_26 /*PRIVATE*/;
        REAL* i5_27 = &(this->inputsTPParent->i5_27_darts44[this->getLocalID()]);
        (void)i5_27 /*PRIVATE*/;
        REAL* i5_28 = &(this->inputsTPParent->i5_28_darts44[this->getLocalID()]);
        (void)i5_28 /*PRIVATE*/;
        REAL* i5_29 = &(this->inputsTPParent->i5_29_darts44[this->getLocalID()]);
        (void)i5_29 /*PRIVATE*/;
        REAL* i5_3 = &(this->inputsTPParent->i5_3_darts44[this->getLocalID()]);
        (void)i5_3 /*PRIVATE*/;
        REAL* i5_30 = &(this->inputsTPParent->i5_30_darts44[this->getLocalID()]);
        (void)i5_30 /*PRIVATE*/;
        REAL* i5_31 = &(this->inputsTPParent->i5_31_darts44[this->getLocalID()]);
        (void)i5_31 /*PRIVATE*/;
        REAL* i5_4 = &(this->inputsTPParent->i5_4_darts44[this->getLocalID()]);
        (void)i5_4 /*PRIVATE*/;
        REAL* i5_5 = &(this->inputsTPParent->i5_5_darts44[this->getLocalID()]);
        (void)i5_5 /*PRIVATE*/;
        REAL* i5_6 = &(this->inputsTPParent->i5_6_darts44[this->getLocalID()]);
        (void)i5_6 /*PRIVATE*/;
        REAL* i5_7 = &(this->inputsTPParent->i5_7_darts44[this->getLocalID()]);
        (void)i5_7 /*PRIVATE*/;
        REAL* i5_8 = &(this->inputsTPParent->i5_8_darts44[this->getLocalID()]);
        (void)i5_8 /*PRIVATE*/;
        REAL* i5_9 = &(this->inputsTPParent->i5_9_darts44[this->getLocalID()]);
        (void)i5_9 /*PRIVATE*/;
        COMPLEX** in = &(this->inputsTPParent->in_darts44[this->getLocalID()]);
        (void)in /*PRIVATE*/;
        COMPLEX** jp = &(this->inputsTPParent->jp_darts44[this->getLocalID()]);
        (void)jp /*PRIVATE*/;
        COMPLEX** kp = &(this->inputsTPParent->kp_darts44[this->getLocalID()]);
        (void)kp /*PRIVATE*/;
        int* l1 = &(this->inputsTPParent->l1_darts44[this->getLocalID()]);
        (void)l1 /*PRIVATE*/;
        int* m = &(this->inputsTPParent->m_darts44[this->getLocalID()]);
        (void)m /*PRIVATE*/;
        int* nWdn = &(this->inputsTPParent->nWdn_darts44[this->getLocalID()]);
        (void)nWdn /*PRIVATE*/;
        COMPLEX** out = &(this->inputsTPParent->out_darts44[this->getLocalID()]);
        (void)out /*PRIVATE*/;
        REAL* r1_0 = &(this->inputsTPParent->r1_0_darts44[this->getLocalID()]);
        (void)r1_0 /*PRIVATE*/;
        REAL* r1_1 = &(this->inputsTPParent->r1_1_darts44[this->getLocalID()]);
        (void)r1_1 /*PRIVATE*/;
        REAL* r1_10 = &(this->inputsTPParent->r1_10_darts44[this->getLocalID()]);
        (void)r1_10 /*PRIVATE*/;
        REAL* r1_11 = &(this->inputsTPParent->r1_11_darts44[this->getLocalID()]);
        (void)r1_11 /*PRIVATE*/;
        REAL* r1_12 = &(this->inputsTPParent->r1_12_darts44[this->getLocalID()]);
        (void)r1_12 /*PRIVATE*/;
        REAL* r1_13 = &(this->inputsTPParent->r1_13_darts44[this->getLocalID()]);
        (void)r1_13 /*PRIVATE*/;
        REAL* r1_14 = &(this->inputsTPParent->r1_14_darts44[this->getLocalID()]);
        (void)r1_14 /*PRIVATE*/;
        REAL* r1_15 = &(this->inputsTPParent->r1_15_darts44[this->getLocalID()]);
        (void)r1_15 /*PRIVATE*/;
        REAL* r1_16 = &(this->inputsTPParent->r1_16_darts44[this->getLocalID()]);
        (void)r1_16 /*PRIVATE*/;
        REAL* r1_17 = &(this->inputsTPParent->r1_17_darts44[this->getLocalID()]);
        (void)r1_17 /*PRIVATE*/;
        REAL* r1_18 = &(this->inputsTPParent->r1_18_darts44[this->getLocalID()]);
        (void)r1_18 /*PRIVATE*/;
        REAL* r1_19 = &(this->inputsTPParent->r1_19_darts44[this->getLocalID()]);
        (void)r1_19 /*PRIVATE*/;
        REAL* r1_2 = &(this->inputsTPParent->r1_2_darts44[this->getLocalID()]);
        (void)r1_2 /*PRIVATE*/;
        REAL* r1_20 = &(this->inputsTPParent->r1_20_darts44[this->getLocalID()]);
        (void)r1_20 /*PRIVATE*/;
        REAL* r1_21 = &(this->inputsTPParent->r1_21_darts44[this->getLocalID()]);
        (void)r1_21 /*PRIVATE*/;
        REAL* r1_22 = &(this->inputsTPParent->r1_22_darts44[this->getLocalID()]);
        (void)r1_22 /*PRIVATE*/;
        REAL* r1_23 = &(this->inputsTPParent->r1_23_darts44[this->getLocalID()]);
        (void)r1_23 /*PRIVATE*/;
        REAL* r1_24 = &(this->inputsTPParent->r1_24_darts44[this->getLocalID()]);
        (void)r1_24 /*PRIVATE*/;
        REAL* r1_25 = &(this->inputsTPParent->r1_25_darts44[this->getLocalID()]);
        (void)r1_25 /*PRIVATE*/;
        REAL* r1_26 = &(this->inputsTPParent->r1_26_darts44[this->getLocalID()]);
        (void)r1_26 /*PRIVATE*/;
        REAL* r1_27 = &(this->inputsTPParent->r1_27_darts44[this->getLocalID()]);
        (void)r1_27 /*PRIVATE*/;
        REAL* r1_28 = &(this->inputsTPParent->r1_28_darts44[this->getLocalID()]);
        (void)r1_28 /*PRIVATE*/;
        REAL* r1_29 = &(this->inputsTPParent->r1_29_darts44[this->getLocalID()]);
        (void)r1_29 /*PRIVATE*/;
        REAL* r1_3 = &(this->inputsTPParent->r1_3_darts44[this->getLocalID()]);
        (void)r1_3 /*PRIVATE*/;
        REAL* r1_30 = &(this->inputsTPParent->r1_30_darts44[this->getLocalID()]);
        (void)r1_30 /*PRIVATE*/;
        REAL* r1_31 = &(this->inputsTPParent->r1_31_darts44[this->getLocalID()]);
        (void)r1_31 /*PRIVATE*/;
        REAL* r1_4 = &(this->inputsTPParent->r1_4_darts44[this->getLocalID()]);
        (void)r1_4 /*PRIVATE*/;
        REAL* r1_5 = &(this->inputsTPParent->r1_5_darts44[this->getLocalID()]);
        (void)r1_5 /*PRIVATE*/;
        REAL* r1_6 = &(this->inputsTPParent->r1_6_darts44[this->getLocalID()]);
        (void)r1_6 /*PRIVATE*/;
        REAL* r1_7 = &(this->inputsTPParent->r1_7_darts44[this->getLocalID()]);
        (void)r1_7 /*PRIVATE*/;
        REAL* r1_8 = &(this->inputsTPParent->r1_8_darts44[this->getLocalID()]);
        (void)r1_8 /*PRIVATE*/;
        REAL* r1_9 = &(this->inputsTPParent->r1_9_darts44[this->getLocalID()]);
        (void)r1_9 /*PRIVATE*/;
        REAL* r2_0 = &(this->inputsTPParent->r2_0_darts44[this->getLocalID()]);
        (void)r2_0 /*PRIVATE*/;
        REAL* r2_1 = &(this->inputsTPParent->r2_1_darts44[this->getLocalID()]);
        (void)r2_1 /*PRIVATE*/;
        REAL* r2_10 = &(this->inputsTPParent->r2_10_darts44[this->getLocalID()]);
        (void)r2_10 /*PRIVATE*/;
        REAL* r2_11 = &(this->inputsTPParent->r2_11_darts44[this->getLocalID()]);
        (void)r2_11 /*PRIVATE*/;
        REAL* r2_12 = &(this->inputsTPParent->r2_12_darts44[this->getLocalID()]);
        (void)r2_12 /*PRIVATE*/;
        REAL* r2_13 = &(this->inputsTPParent->r2_13_darts44[this->getLocalID()]);
        (void)r2_13 /*PRIVATE*/;
        REAL* r2_14 = &(this->inputsTPParent->r2_14_darts44[this->getLocalID()]);
        (void)r2_14 /*PRIVATE*/;
        REAL* r2_15 = &(this->inputsTPParent->r2_15_darts44[this->getLocalID()]);
        (void)r2_15 /*PRIVATE*/;
        REAL* r2_16 = &(this->inputsTPParent->r2_16_darts44[this->getLocalID()]);
        (void)r2_16 /*PRIVATE*/;
        REAL* r2_17 = &(this->inputsTPParent->r2_17_darts44[this->getLocalID()]);
        (void)r2_17 /*PRIVATE*/;
        REAL* r2_18 = &(this->inputsTPParent->r2_18_darts44[this->getLocalID()]);
        (void)r2_18 /*PRIVATE*/;
        REAL* r2_19 = &(this->inputsTPParent->r2_19_darts44[this->getLocalID()]);
        (void)r2_19 /*PRIVATE*/;
        REAL* r2_2 = &(this->inputsTPParent->r2_2_darts44[this->getLocalID()]);
        (void)r2_2 /*PRIVATE*/;
        REAL* r2_20 = &(this->inputsTPParent->r2_20_darts44[this->getLocalID()]);
        (void)r2_20 /*PRIVATE*/;
        REAL* r2_21 = &(this->inputsTPParent->r2_21_darts44[this->getLocalID()]);
        (void)r2_21 /*PRIVATE*/;
        REAL* r2_22 = &(this->inputsTPParent->r2_22_darts44[this->getLocalID()]);
        (void)r2_22 /*PRIVATE*/;
        REAL* r2_23 = &(this->inputsTPParent->r2_23_darts44[this->getLocalID()]);
        (void)r2_23 /*PRIVATE*/;
        REAL* r2_24 = &(this->inputsTPParent->r2_24_darts44[this->getLocalID()]);
        (void)r2_24 /*PRIVATE*/;
        REAL* r2_25 = &(this->inputsTPParent->r2_25_darts44[this->getLocalID()]);
        (void)r2_25 /*PRIVATE*/;
        REAL* r2_26 = &(this->inputsTPParent->r2_26_darts44[this->getLocalID()]);
        (void)r2_26 /*PRIVATE*/;
        REAL* r2_27 = &(this->inputsTPParent->r2_27_darts44[this->getLocalID()]);
        (void)r2_27 /*PRIVATE*/;
        REAL* r2_28 = &(this->inputsTPParent->r2_28_darts44[this->getLocalID()]);
        (void)r2_28 /*PRIVATE*/;
        REAL* r2_29 = &(this->inputsTPParent->r2_29_darts44[this->getLocalID()]);
        (void)r2_29 /*PRIVATE*/;
        REAL* r2_3 = &(this->inputsTPParent->r2_3_darts44[this->getLocalID()]);
        (void)r2_3 /*PRIVATE*/;
        REAL* r2_30 = &(this->inputsTPParent->r2_30_darts44[this->getLocalID()]);
        (void)r2_30 /*PRIVATE*/;
        REAL* r2_31 = &(this->inputsTPParent->r2_31_darts44[this->getLocalID()]);
        (void)r2_31 /*PRIVATE*/;
        REAL* r2_4 = &(this->inputsTPParent->r2_4_darts44[this->getLocalID()]);
        (void)r2_4 /*PRIVATE*/;
        REAL* r2_5 = &(this->inputsTPParent->r2_5_darts44[this->getLocalID()]);
        (void)r2_5 /*PRIVATE*/;
        REAL* r2_6 = &(this->inputsTPParent->r2_6_darts44[this->getLocalID()]);
        (void)r2_6 /*PRIVATE*/;
        REAL* r2_7 = &(this->inputsTPParent->r2_7_darts44[this->getLocalID()]);
        (void)r2_7 /*PRIVATE*/;
        REAL* r2_8 = &(this->inputsTPParent->r2_8_darts44[this->getLocalID()]);
        (void)r2_8 /*PRIVATE*/;
        REAL* r2_9 = &(this->inputsTPParent->r2_9_darts44[this->getLocalID()]);
        (void)r2_9 /*PRIVATE*/;
        REAL* r3_0 = &(this->inputsTPParent->r3_0_darts44[this->getLocalID()]);
        (void)r3_0 /*PRIVATE*/;
        REAL* r3_1 = &(this->inputsTPParent->r3_1_darts44[this->getLocalID()]);
        (void)r3_1 /*PRIVATE*/;
        REAL* r3_10 = &(this->inputsTPParent->r3_10_darts44[this->getLocalID()]);
        (void)r3_10 /*PRIVATE*/;
        REAL* r3_11 = &(this->inputsTPParent->r3_11_darts44[this->getLocalID()]);
        (void)r3_11 /*PRIVATE*/;
        REAL* r3_12 = &(this->inputsTPParent->r3_12_darts44[this->getLocalID()]);
        (void)r3_12 /*PRIVATE*/;
        REAL* r3_13 = &(this->inputsTPParent->r3_13_darts44[this->getLocalID()]);
        (void)r3_13 /*PRIVATE*/;
        REAL* r3_14 = &(this->inputsTPParent->r3_14_darts44[this->getLocalID()]);
        (void)r3_14 /*PRIVATE*/;
        REAL* r3_15 = &(this->inputsTPParent->r3_15_darts44[this->getLocalID()]);
        (void)r3_15 /*PRIVATE*/;
        REAL* r3_16 = &(this->inputsTPParent->r3_16_darts44[this->getLocalID()]);
        (void)r3_16 /*PRIVATE*/;
        REAL* r3_17 = &(this->inputsTPParent->r3_17_darts44[this->getLocalID()]);
        (void)r3_17 /*PRIVATE*/;
        REAL* r3_18 = &(this->inputsTPParent->r3_18_darts44[this->getLocalID()]);
        (void)r3_18 /*PRIVATE*/;
        REAL* r3_19 = &(this->inputsTPParent->r3_19_darts44[this->getLocalID()]);
        (void)r3_19 /*PRIVATE*/;
        REAL* r3_2 = &(this->inputsTPParent->r3_2_darts44[this->getLocalID()]);
        (void)r3_2 /*PRIVATE*/;
        REAL* r3_20 = &(this->inputsTPParent->r3_20_darts44[this->getLocalID()]);
        (void)r3_20 /*PRIVATE*/;
        REAL* r3_21 = &(this->inputsTPParent->r3_21_darts44[this->getLocalID()]);
        (void)r3_21 /*PRIVATE*/;
        REAL* r3_22 = &(this->inputsTPParent->r3_22_darts44[this->getLocalID()]);
        (void)r3_22 /*PRIVATE*/;
        REAL* r3_23 = &(this->inputsTPParent->r3_23_darts44[this->getLocalID()]);
        (void)r3_23 /*PRIVATE*/;
        REAL* r3_24 = &(this->inputsTPParent->r3_24_darts44[this->getLocalID()]);
        (void)r3_24 /*PRIVATE*/;
        REAL* r3_25 = &(this->inputsTPParent->r3_25_darts44[this->getLocalID()]);
        (void)r3_25 /*PRIVATE*/;
        REAL* r3_26 = &(this->inputsTPParent->r3_26_darts44[this->getLocalID()]);
        (void)r3_26 /*PRIVATE*/;
        REAL* r3_27 = &(this->inputsTPParent->r3_27_darts44[this->getLocalID()]);
        (void)r3_27 /*PRIVATE*/;
        REAL* r3_28 = &(this->inputsTPParent->r3_28_darts44[this->getLocalID()]);
        (void)r3_28 /*PRIVATE*/;
        REAL* r3_29 = &(this->inputsTPParent->r3_29_darts44[this->getLocalID()]);
        (void)r3_29 /*PRIVATE*/;
        REAL* r3_3 = &(this->inputsTPParent->r3_3_darts44[this->getLocalID()]);
        (void)r3_3 /*PRIVATE*/;
        REAL* r3_30 = &(this->inputsTPParent->r3_30_darts44[this->getLocalID()]);
        (void)r3_30 /*PRIVATE*/;
        REAL* r3_31 = &(this->inputsTPParent->r3_31_darts44[this->getLocalID()]);
        (void)r3_31 /*PRIVATE*/;
        REAL* r3_4 = &(this->inputsTPParent->r3_4_darts44[this->getLocalID()]);
        (void)r3_4 /*PRIVATE*/;
        REAL* r3_5 = &(this->inputsTPParent->r3_5_darts44[this->getLocalID()]);
        (void)r3_5 /*PRIVATE*/;
        REAL* r3_6 = &(this->inputsTPParent->r3_6_darts44[this->getLocalID()]);
        (void)r3_6 /*PRIVATE*/;
        REAL* r3_7 = &(this->inputsTPParent->r3_7_darts44[this->getLocalID()]);
        (void)r3_7 /*PRIVATE*/;
        REAL* r3_8 = &(this->inputsTPParent->r3_8_darts44[this->getLocalID()]);
        (void)r3_8 /*PRIVATE*/;
        REAL* r3_9 = &(this->inputsTPParent->r3_9_darts44[this->getLocalID()]);
        (void)r3_9 /*PRIVATE*/;
        REAL* r4_0 = &(this->inputsTPParent->r4_0_darts44[this->getLocalID()]);
        (void)r4_0 /*PRIVATE*/;
        REAL* r4_1 = &(this->inputsTPParent->r4_1_darts44[this->getLocalID()]);
        (void)r4_1 /*PRIVATE*/;
        REAL* r4_10 = &(this->inputsTPParent->r4_10_darts44[this->getLocalID()]);
        (void)r4_10 /*PRIVATE*/;
        REAL* r4_11 = &(this->inputsTPParent->r4_11_darts44[this->getLocalID()]);
        (void)r4_11 /*PRIVATE*/;
        REAL* r4_12 = &(this->inputsTPParent->r4_12_darts44[this->getLocalID()]);
        (void)r4_12 /*PRIVATE*/;
        REAL* r4_13 = &(this->inputsTPParent->r4_13_darts44[this->getLocalID()]);
        (void)r4_13 /*PRIVATE*/;
        REAL* r4_14 = &(this->inputsTPParent->r4_14_darts44[this->getLocalID()]);
        (void)r4_14 /*PRIVATE*/;
        REAL* r4_15 = &(this->inputsTPParent->r4_15_darts44[this->getLocalID()]);
        (void)r4_15 /*PRIVATE*/;
        REAL* r4_16 = &(this->inputsTPParent->r4_16_darts44[this->getLocalID()]);
        (void)r4_16 /*PRIVATE*/;
        REAL* r4_17 = &(this->inputsTPParent->r4_17_darts44[this->getLocalID()]);
        (void)r4_17 /*PRIVATE*/;
        REAL* r4_18 = &(this->inputsTPParent->r4_18_darts44[this->getLocalID()]);
        (void)r4_18 /*PRIVATE*/;
        REAL* r4_19 = &(this->inputsTPParent->r4_19_darts44[this->getLocalID()]);
        (void)r4_19 /*PRIVATE*/;
        REAL* r4_2 = &(this->inputsTPParent->r4_2_darts44[this->getLocalID()]);
        (void)r4_2 /*PRIVATE*/;
        REAL* r4_20 = &(this->inputsTPParent->r4_20_darts44[this->getLocalID()]);
        (void)r4_20 /*PRIVATE*/;
        REAL* r4_21 = &(this->inputsTPParent->r4_21_darts44[this->getLocalID()]);
        (void)r4_21 /*PRIVATE*/;
        REAL* r4_22 = &(this->inputsTPParent->r4_22_darts44[this->getLocalID()]);
        (void)r4_22 /*PRIVATE*/;
        REAL* r4_23 = &(this->inputsTPParent->r4_23_darts44[this->getLocalID()]);
        (void)r4_23 /*PRIVATE*/;
        REAL* r4_24 = &(this->inputsTPParent->r4_24_darts44[this->getLocalID()]);
        (void)r4_24 /*PRIVATE*/;
        REAL* r4_25 = &(this->inputsTPParent->r4_25_darts44[this->getLocalID()]);
        (void)r4_25 /*PRIVATE*/;
        REAL* r4_26 = &(this->inputsTPParent->r4_26_darts44[this->getLocalID()]);
        (void)r4_26 /*PRIVATE*/;
        REAL* r4_27 = &(this->inputsTPParent->r4_27_darts44[this->getLocalID()]);
        (void)r4_27 /*PRIVATE*/;
        REAL* r4_28 = &(this->inputsTPParent->r4_28_darts44[this->getLocalID()]);
        (void)r4_28 /*PRIVATE*/;
        REAL* r4_29 = &(this->inputsTPParent->r4_29_darts44[this->getLocalID()]);
        (void)r4_29 /*PRIVATE*/;
        REAL* r4_3 = &(this->inputsTPParent->r4_3_darts44[this->getLocalID()]);
        (void)r4_3 /*PRIVATE*/;
        REAL* r4_30 = &(this->inputsTPParent->r4_30_darts44[this->getLocalID()]);
        (void)r4_30 /*PRIVATE*/;
        REAL* r4_31 = &(this->inputsTPParent->r4_31_darts44[this->getLocalID()]);
        (void)r4_31 /*PRIVATE*/;
        REAL* r4_4 = &(this->inputsTPParent->r4_4_darts44[this->getLocalID()]);
        (void)r4_4 /*PRIVATE*/;
        REAL* r4_5 = &(this->inputsTPParent->r4_5_darts44[this->getLocalID()]);
        (void)r4_5 /*PRIVATE*/;
        REAL* r4_6 = &(this->inputsTPParent->r4_6_darts44[this->getLocalID()]);
        (void)r4_6 /*PRIVATE*/;
        REAL* r4_7 = &(this->inputsTPParent->r4_7_darts44[this->getLocalID()]);
        (void)r4_7 /*PRIVATE*/;
        REAL* r4_8 = &(this->inputsTPParent->r4_8_darts44[this->getLocalID()]);
        (void)r4_8 /*PRIVATE*/;
        REAL* r4_9 = &(this->inputsTPParent->r4_9_darts44[this->getLocalID()]);
        (void)r4_9 /*PRIVATE*/;
        REAL* r5_0 = &(this->inputsTPParent->r5_0_darts44[this->getLocalID()]);
        (void)r5_0 /*PRIVATE*/;
        REAL* r5_1 = &(this->inputsTPParent->r5_1_darts44[this->getLocalID()]);
        (void)r5_1 /*PRIVATE*/;
        REAL* r5_10 = &(this->inputsTPParent->r5_10_darts44[this->getLocalID()]);
        (void)r5_10 /*PRIVATE*/;
        REAL* r5_11 = &(this->inputsTPParent->r5_11_darts44[this->getLocalID()]);
        (void)r5_11 /*PRIVATE*/;
        REAL* r5_12 = &(this->inputsTPParent->r5_12_darts44[this->getLocalID()]);
        (void)r5_12 /*PRIVATE*/;
        REAL* r5_13 = &(this->inputsTPParent->r5_13_darts44[this->getLocalID()]);
        (void)r5_13 /*PRIVATE*/;
        REAL* r5_14 = &(this->inputsTPParent->r5_14_darts44[this->getLocalID()]);
        (void)r5_14 /*PRIVATE*/;
        REAL* r5_15 = &(this->inputsTPParent->r5_15_darts44[this->getLocalID()]);
        (void)r5_15 /*PRIVATE*/;
        REAL* r5_16 = &(this->inputsTPParent->r5_16_darts44[this->getLocalID()]);
        (void)r5_16 /*PRIVATE*/;
        REAL* r5_17 = &(this->inputsTPParent->r5_17_darts44[this->getLocalID()]);
        (void)r5_17 /*PRIVATE*/;
        REAL* r5_18 = &(this->inputsTPParent->r5_18_darts44[this->getLocalID()]);
        (void)r5_18 /*PRIVATE*/;
        REAL* r5_19 = &(this->inputsTPParent->r5_19_darts44[this->getLocalID()]);
        (void)r5_19 /*PRIVATE*/;
        REAL* r5_2 = &(this->inputsTPParent->r5_2_darts44[this->getLocalID()]);
        (void)r5_2 /*PRIVATE*/;
        REAL* r5_20 = &(this->inputsTPParent->r5_20_darts44[this->getLocalID()]);
        (void)r5_20 /*PRIVATE*/;
        REAL* r5_21 = &(this->inputsTPParent->r5_21_darts44[this->getLocalID()]);
        (void)r5_21 /*PRIVATE*/;
        REAL* r5_22 = &(this->inputsTPParent->r5_22_darts44[this->getLocalID()]);
        (void)r5_22 /*PRIVATE*/;
        REAL* r5_23 = &(this->inputsTPParent->r5_23_darts44[this->getLocalID()]);
        (void)r5_23 /*PRIVATE*/;
        REAL* r5_24 = &(this->inputsTPParent->r5_24_darts44[this->getLocalID()]);
        (void)r5_24 /*PRIVATE*/;
        REAL* r5_25 = &(this->inputsTPParent->r5_25_darts44[this->getLocalID()]);
        (void)r5_25 /*PRIVATE*/;
        REAL* r5_26 = &(this->inputsTPParent->r5_26_darts44[this->getLocalID()]);
        (void)r5_26 /*PRIVATE*/;
        REAL* r5_27 = &(this->inputsTPParent->r5_27_darts44[this->getLocalID()]);
        (void)r5_27 /*PRIVATE*/;
        REAL* r5_28 = &(this->inputsTPParent->r5_28_darts44[this->getLocalID()]);
        (void)r5_28 /*PRIVATE*/;
        REAL* r5_29 = &(this->inputsTPParent->r5_29_darts44[this->getLocalID()]);
        (void)r5_29 /*PRIVATE*/;
        REAL* r5_3 = &(this->inputsTPParent->r5_3_darts44[this->getLocalID()]);
        (void)r5_3 /*PRIVATE*/;
        REAL* r5_30 = &(this->inputsTPParent->r5_30_darts44[this->getLocalID()]);
        (void)r5_30 /*PRIVATE*/;
        REAL* r5_31 = &(this->inputsTPParent->r5_31_darts44[this->getLocalID()]);
        (void)r5_31 /*PRIVATE*/;
        REAL* r5_4 = &(this->inputsTPParent->r5_4_darts44[this->getLocalID()]);
        (void)r5_4 /*PRIVATE*/;
        REAL* r5_5 = &(this->inputsTPParent->r5_5_darts44[this->getLocalID()]);
        (void)r5_5 /*PRIVATE*/;
        REAL* r5_6 = &(this->inputsTPParent->r5_6_darts44[this->getLocalID()]);
        (void)r5_6 /*PRIVATE*/;
        REAL* r5_7 = &(this->inputsTPParent->r5_7_darts44[this->getLocalID()]);
        (void)r5_7 /*PRIVATE*/;
        REAL* r5_8 = &(this->inputsTPParent->r5_8_darts44[this->getLocalID()]);
        (void)r5_8 /*PRIVATE*/;
        REAL* r5_9 = &(this->inputsTPParent->r5_9_darts44[this->getLocalID()]);
        (void)r5_9 /*PRIVATE*/;
        REAL* tmpi = &(this->inputsTPParent->tmpi_darts44[this->getLocalID()]);
        (void)tmpi /*PRIVATE*/;
        REAL* tmpr = &(this->inputsTPParent->tmpr_darts44[this->getLocalID()]);
        (void)tmpr /*PRIVATE*/;
        REAL* wi = &(this->inputsTPParent->wi_darts44[this->getLocalID()]);
        (void)wi /*PRIVATE*/;
        REAL* wr = &(this->inputsTPParent->wr_darts44[this->getLocalID()]);
        (void)wr /*PRIVATE*/;
        /*Loop's init*/
        (this->inputsTPParent->i_darts44[this->getID()])
            = (this->inputsTPParent->a_darts44[this->getID()]);
        int i_darts_counter_temp44 = (this->inputsTPParent->i_darts44[this->getID()]);
        for (; (i_darts_counter_temp44) < (*b); (i_darts_counter_temp44)++) {
            (*l1) = (*nWdn) * (i_darts_counter_temp44);
            (*kp) = (*out) + (i_darts_counter_temp44);
            (*jp) = (*in) + (i_darts_counter_temp44);
            {
                {
                    {
                        {
                            {
                                (*r5_0) = (((*jp)[0 * (*m)]).re);
                                (*i5_0) = (((*jp)[0 * (*m)]).im);
                                (*wr) = (((*W)[16 * (*l1)]).re);
                                (*wi) = (((*W)[16 * (*l1)]).im);
                                (*tmpr) = (((*jp)[16 * (*m)]).re);
                                (*tmpi) = (((*jp)[16 * (*m)]).im);
                                (*r5_16) = (((*wr) * (*tmpr)) - ((*wi) * (*tmpi)));
                                (*i5_16) = (((*wi) * (*tmpr)) + ((*wr) * (*tmpi)));
                                (*r4_0) = ((*r5_0) + (*r5_16));
                                (*i4_0) = ((*i5_0) + (*i5_16));
                                (*r4_16) = ((*r5_0) - (*r5_16));
                                (*i4_16) = ((*i5_0) - (*i5_16));
                            }
                            {
                                (*wr) = (((*W)[8 * (*l1)]).re);
                                (*wi) = (((*W)[8 * (*l1)]).im);
                                (*tmpr) = (((*jp)[8 * (*m)]).re);
                                (*tmpi) = (((*jp)[8 * (*m)]).im);
                                (*r5_8) = (((*wr) * (*tmpr)) - ((*wi) * (*tmpi)));
                                (*i5_8) = (((*wi) * (*tmpr)) + ((*wr) * (*tmpi)));
                                (*wr) = (((*W)[24 * (*l1)]).re);
                                (*wi) = (((*W)[24 * (*l1)]).im);
                                (*tmpr) = (((*jp)[24 * (*m)]).re);
                                (*tmpi) = (((*jp)[24 * (*m)]).im);
                                (*r5_24) = (((*wr) * (*tmpr)) - ((*wi) * (*tmpi)));
                                (*i5_24) = (((*wi) * (*tmpr)) + ((*wr) * (*tmpi)));
                                (*r4_8) = ((*r5_8) + (*r5_24));
                                (*i4_8) = ((*i5_8) + (*i5_24));
                                (*r4_24) = ((*r5_8) - (*r5_24));
                                (*i4_24) = ((*i5_8) - (*i5_24));
                            }
                            (*r3_0) = ((*r4_0) + (*r4_8));
                            (*i3_0) = ((*i4_0) + (*i4_8));
                            (*r3_16) = ((*r4_0) - (*r4_8));
                            (*i3_16) = ((*i4_0) - (*i4_8));
                            (*r3_8) = ((*r4_16) + (*i4_24));
                            (*i3_8) = ((*i4_16) - (*r4_24));
                            (*r3_24) = ((*r4_16) - (*i4_24));
                            (*i3_24) = ((*i4_16) + (*r4_24));
                        }
                        {
                            {
                                (*wr) = (((*W)[4 * (*l1)]).re);
                                (*wi) = (((*W)[4 * (*l1)]).im);
                                (*tmpr) = (((*jp)[4 * (*m)]).re);
                                (*tmpi) = (((*jp)[4 * (*m)]).im);
                                (*r5_4) = (((*wr) * (*tmpr)) - ((*wi) * (*tmpi)));
                                (*i5_4) = (((*wi) * (*tmpr)) + ((*wr) * (*tmpi)));
                                (*wr) = (((*W)[20 * (*l1)]).re);
                                (*wi) = (((*W)[20 * (*l1)]).im);
                                (*tmpr) = (((*jp)[20 * (*m)]).re);
                                (*tmpi) = (((*jp)[20 * (*m)]).im);
                                (*r5_20) = (((*wr) * (*tmpr)) - ((*wi) * (*tmpi)));
                                (*i5_20) = (((*wi) * (*tmpr)) + ((*wr) * (*tmpi)));
                                (*r4_4) = ((*r5_4) + (*r5_20));
                                (*i4_4) = ((*i5_4) + (*i5_20));
                                (*r4_20) = ((*r5_4) - (*r5_20));
                                (*i4_20) = ((*i5_4) - (*i5_20));
                            }
                            {
                                (*wr) = (((*W)[12 * (*l1)]).re);
                                (*wi) = (((*W)[12 * (*l1)]).im);
                                (*tmpr) = (((*jp)[12 * (*m)]).re);
                                (*tmpi) = (((*jp)[12 * (*m)]).im);
                                (*r5_12) = (((*wr) * (*tmpr)) - ((*wi) * (*tmpi)));
                                (*i5_12) = (((*wi) * (*tmpr)) + ((*wr) * (*tmpi)));
                                (*wr) = (((*W)[28 * (*l1)]).re);
                                (*wi) = (((*W)[28 * (*l1)]).im);
                                (*tmpr) = (((*jp)[28 * (*m)]).re);
                                (*tmpi) = (((*jp)[28 * (*m)]).im);
                                (*r5_28) = (((*wr) * (*tmpr)) - ((*wi) * (*tmpi)));
                                (*i5_28) = (((*wi) * (*tmpr)) + ((*wr) * (*tmpi)));
                                (*r4_12) = ((*r5_12) + (*r5_28));
                                (*i4_12) = ((*i5_12) + (*i5_28));
                                (*r4_28) = ((*r5_12) - (*r5_28));
                                (*i4_28) = ((*i5_12) - (*i5_28));
                            }
                            (*r3_4) = ((*r4_4) + (*r4_12));
                            (*i3_4) = ((*i4_4) + (*i4_12));
                            (*r3_20) = ((*r4_4) - (*r4_12));
                            (*i3_20) = ((*i4_4) - (*i4_12));
                            (*r3_12) = ((*r4_20) + (*i4_28));
                            (*i3_12) = ((*i4_20) - (*r4_28));
                            (*r3_28) = ((*r4_20) - (*i4_28));
                            (*i3_28) = ((*i4_20) + (*r4_28));
                        }
                        (*r2_0) = ((*r3_0) + (*r3_4));
                        (*i2_0) = ((*i3_0) + (*i3_4));
                        (*r2_16) = ((*r3_0) - (*r3_4));
                        (*i2_16) = ((*i3_0) - (*i3_4));
                        (*tmpr) = (0.70710678118699999 * ((*r3_12) + (*i3_12)));
                        (*tmpi) = (0.70710678118699999 * ((*i3_12) - (*r3_12)));
                        (*r2_4) = ((*r3_8) + (*tmpr));
                        (*i2_4) = ((*i3_8) + (*tmpi));
                        (*r2_20) = ((*r3_8) - (*tmpr));
                        (*i2_20) = ((*i3_8) - (*tmpi));
                        (*r2_8) = ((*r3_16) + (*i3_20));
                        (*i2_8) = ((*i3_16) - (*r3_20));
                        (*r2_24) = ((*r3_16) - (*i3_20));
                        (*i2_24) = ((*i3_16) + (*r3_20));
                        (*tmpr) = (0.70710678118699999 * ((*i3_28) - (*r3_28)));
                        (*tmpi) = (0.70710678118699999 * ((*r3_28) + (*i3_28)));
                        (*r2_12) = ((*r3_24) + (*tmpr));
                        (*i2_12) = ((*i3_24) - (*tmpi));
                        (*r2_28) = ((*r3_24) - (*tmpr));
                        (*i2_28) = ((*i3_24) + (*tmpi));
                    }
                    {
                        {
                            {
                                (*wr) = (((*W)[2 * (*l1)]).re);
                                (*wi) = (((*W)[2 * (*l1)]).im);
                                (*tmpr) = (((*jp)[2 * (*m)]).re);
                                (*tmpi) = (((*jp)[2 * (*m)]).im);
                                (*r5_2) = (((*wr) * (*tmpr)) - ((*wi) * (*tmpi)));
                                (*i5_2) = (((*wi) * (*tmpr)) + ((*wr) * (*tmpi)));
                                (*wr) = (((*W)[18 * (*l1)]).re);
                                (*wi) = (((*W)[18 * (*l1)]).im);
                                (*tmpr) = (((*jp)[18 * (*m)]).re);
                                (*tmpi) = (((*jp)[18 * (*m)]).im);
                                (*r5_18) = (((*wr) * (*tmpr)) - ((*wi) * (*tmpi)));
                                (*i5_18) = (((*wi) * (*tmpr)) + ((*wr) * (*tmpi)));
                                (*r4_2) = ((*r5_2) + (*r5_18));
                                (*i4_2) = ((*i5_2) + (*i5_18));
                                (*r4_18) = ((*r5_2) - (*r5_18));
                                (*i4_18) = ((*i5_2) - (*i5_18));
                            }
                            {
                                (*wr) = (((*W)[10 * (*l1)]).re);
                                (*wi) = (((*W)[10 * (*l1)]).im);
                                (*tmpr) = (((*jp)[10 * (*m)]).re);
                                (*tmpi) = (((*jp)[10 * (*m)]).im);
                                (*r5_10) = (((*wr) * (*tmpr)) - ((*wi) * (*tmpi)));
                                (*i5_10) = (((*wi) * (*tmpr)) + ((*wr) * (*tmpi)));
                                (*wr) = (((*W)[26 * (*l1)]).re);
                                (*wi) = (((*W)[26 * (*l1)]).im);
                                (*tmpr) = (((*jp)[26 * (*m)]).re);
                                (*tmpi) = (((*jp)[26 * (*m)]).im);
                                (*r5_26) = (((*wr) * (*tmpr)) - ((*wi) * (*tmpi)));
                                (*i5_26) = (((*wi) * (*tmpr)) + ((*wr) * (*tmpi)));
                                (*r4_10) = ((*r5_10) + (*r5_26));
                                (*i4_10) = ((*i5_10) + (*i5_26));
                                (*r4_26) = ((*r5_10) - (*r5_26));
                                (*i4_26) = ((*i5_10) - (*i5_26));
                            }
                            (*r3_2) = ((*r4_2) + (*r4_10));
                            (*i3_2) = ((*i4_2) + (*i4_10));
                            (*r3_18) = ((*r4_2) - (*r4_10));
                            (*i3_18) = ((*i4_2) - (*i4_10));
                            (*r3_10) = ((*r4_18) + (*i4_26));
                            (*i3_10) = ((*i4_18) - (*r4_26));
                            (*r3_26) = ((*r4_18) - (*i4_26));
                            (*i3_26) = ((*i4_18) + (*r4_26));
                        }
                        {
                            {
                                (*wr) = (((*W)[6 * (*l1)]).re);
                                (*wi) = (((*W)[6 * (*l1)]).im);
                                (*tmpr) = (((*jp)[6 * (*m)]).re);
                                (*tmpi) = (((*jp)[6 * (*m)]).im);
                                (*r5_6) = (((*wr) * (*tmpr)) - ((*wi) * (*tmpi)));
                                (*i5_6) = (((*wi) * (*tmpr)) + ((*wr) * (*tmpi)));
                                (*wr) = (((*W)[22 * (*l1)]).re);
                                (*wi) = (((*W)[22 * (*l1)]).im);
                                (*tmpr) = (((*jp)[22 * (*m)]).re);
                                (*tmpi) = (((*jp)[22 * (*m)]).im);
                                (*r5_22) = (((*wr) * (*tmpr)) - ((*wi) * (*tmpi)));
                                (*i5_22) = (((*wi) * (*tmpr)) + ((*wr) * (*tmpi)));
                                (*r4_6) = ((*r5_6) + (*r5_22));
                                (*i4_6) = ((*i5_6) + (*i5_22));
                                (*r4_22) = ((*r5_6) - (*r5_22));
                                (*i4_22) = ((*i5_6) - (*i5_22));
                            }
                            {
                                (*wr) = (((*W)[14 * (*l1)]).re);
                                (*wi) = (((*W)[14 * (*l1)]).im);
                                (*tmpr) = (((*jp)[14 * (*m)]).re);
                                (*tmpi) = (((*jp)[14 * (*m)]).im);
                                (*r5_14) = (((*wr) * (*tmpr)) - ((*wi) * (*tmpi)));
                                (*i5_14) = (((*wi) * (*tmpr)) + ((*wr) * (*tmpi)));
                                (*wr) = (((*W)[30 * (*l1)]).re);
                                (*wi) = (((*W)[30 * (*l1)]).im);
                                (*tmpr) = (((*jp)[30 * (*m)]).re);
                                (*tmpi) = (((*jp)[30 * (*m)]).im);
                                (*r5_30) = (((*wr) * (*tmpr)) - ((*wi) * (*tmpi)));
                                (*i5_30) = (((*wi) * (*tmpr)) + ((*wr) * (*tmpi)));
                                (*r4_14) = ((*r5_14) + (*r5_30));
                                (*i4_14) = ((*i5_14) + (*i5_30));
                                (*r4_30) = ((*r5_14) - (*r5_30));
                                (*i4_30) = ((*i5_14) - (*i5_30));
                            }
                            (*r3_6) = ((*r4_6) + (*r4_14));
                            (*i3_6) = ((*i4_6) + (*i4_14));
                            (*r3_22) = ((*r4_6) - (*r4_14));
                            (*i3_22) = ((*i4_6) - (*i4_14));
                            (*r3_14) = ((*r4_22) + (*i4_30));
                            (*i3_14) = ((*i4_22) - (*r4_30));
                            (*r3_30) = ((*r4_22) - (*i4_30));
                            (*i3_30) = ((*i4_22) + (*r4_30));
                        }
                        (*r2_2) = ((*r3_2) + (*r3_6));
                        (*i2_2) = ((*i3_2) + (*i3_6));
                        (*r2_18) = ((*r3_2) - (*r3_6));
                        (*i2_18) = ((*i3_2) - (*i3_6));
                        (*tmpr) = (0.70710678118699999 * ((*r3_14) + (*i3_14)));
                        (*tmpi) = (0.70710678118699999 * ((*i3_14) - (*r3_14)));
                        (*r2_6) = ((*r3_10) + (*tmpr));
                        (*i2_6) = ((*i3_10) + (*tmpi));
                        (*r2_22) = ((*r3_10) - (*tmpr));
                        (*i2_22) = ((*i3_10) - (*tmpi));
                        (*r2_10) = ((*r3_18) + (*i3_22));
                        (*i2_10) = ((*i3_18) - (*r3_22));
                        (*r2_26) = ((*r3_18) - (*i3_22));
                        (*i2_26) = ((*i3_18) + (*r3_22));
                        (*tmpr) = (0.70710678118699999 * ((*i3_30) - (*r3_30)));
                        (*tmpi) = (0.70710678118699999 * ((*r3_30) + (*i3_30)));
                        (*r2_14) = ((*r3_26) + (*tmpr));
                        (*i2_14) = ((*i3_26) - (*tmpi));
                        (*r2_30) = ((*r3_26) - (*tmpr));
                        (*i2_30) = ((*i3_26) + (*tmpi));
                    }
                    (*r1_0) = ((*r2_0) + (*r2_2));
                    (*i1_0) = ((*i2_0) + (*i2_2));
                    (*r1_16) = ((*r2_0) - (*r2_2));
                    (*i1_16) = ((*i2_0) - (*i2_2));
                    (*tmpr) = ((0.92387953251099997 * (*r2_6)) + (0.38268343236500002 * (*i2_6)));
                    (*tmpi) = ((0.92387953251099997 * (*i2_6)) - (0.38268343236500002 * (*r2_6)));
                    (*r1_2) = ((*r2_4) + (*tmpr));
                    (*i1_2) = ((*i2_4) + (*tmpi));
                    (*r1_18) = ((*r2_4) - (*tmpr));
                    (*i1_18) = ((*i2_4) - (*tmpi));
                    (*tmpr) = (0.70710678118699999 * ((*r2_10) + (*i2_10)));
                    (*tmpi) = (0.70710678118699999 * ((*i2_10) - (*r2_10)));
                    (*r1_4) = ((*r2_8) + (*tmpr));
                    (*i1_4) = ((*i2_8) + (*tmpi));
                    (*r1_20) = ((*r2_8) - (*tmpr));
                    (*i1_20) = ((*i2_8) - (*tmpi));
                    (*tmpr) = ((0.38268343236500002 * (*r2_14)) + (0.92387953251099997 * (*i2_14)));
                    (*tmpi) = ((0.38268343236500002 * (*i2_14)) - (0.92387953251099997 * (*r2_14)));
                    (*r1_6) = ((*r2_12) + (*tmpr));
                    (*i1_6) = ((*i2_12) + (*tmpi));
                    (*r1_22) = ((*r2_12) - (*tmpr));
                    (*i1_22) = ((*i2_12) - (*tmpi));
                    (*r1_8) = ((*r2_16) + (*i2_18));
                    (*i1_8) = ((*i2_16) - (*r2_18));
                    (*r1_24) = ((*r2_16) - (*i2_18));
                    (*i1_24) = ((*i2_16) + (*r2_18));
                    (*tmpr) = ((0.92387953251099997 * (*i2_22)) - (0.38268343236500002 * (*r2_22)));
                    (*tmpi) = ((0.92387953251099997 * (*r2_22)) + (0.38268343236500002 * (*i2_22)));
                    (*r1_10) = ((*r2_20) + (*tmpr));
                    (*i1_10) = ((*i2_20) - (*tmpi));
                    (*r1_26) = ((*r2_20) - (*tmpr));
                    (*i1_26) = ((*i2_20) + (*tmpi));
                    (*tmpr) = (0.70710678118699999 * ((*i2_26) - (*r2_26)));
                    (*tmpi) = (0.70710678118699999 * ((*r2_26) + (*i2_26)));
                    (*r1_12) = ((*r2_24) + (*tmpr));
                    (*i1_12) = ((*i2_24) - (*tmpi));
                    (*r1_28) = ((*r2_24) - (*tmpr));
                    (*i1_28) = ((*i2_24) + (*tmpi));
                    (*tmpr) = ((0.38268343236500002 * (*i2_30)) - (0.92387953251099997 * (*r2_30)));
                    (*tmpi) = ((0.38268343236500002 * (*r2_30)) + (0.92387953251099997 * (*i2_30)));
                    (*r1_14) = ((*r2_28) + (*tmpr));
                    (*i1_14) = ((*i2_28) - (*tmpi));
                    (*r1_30) = ((*r2_28) - (*tmpr));
                    (*i1_30) = ((*i2_28) + (*tmpi));
                }
                {
                    {
                        {
                            {
                                (*wr) = (((*W)[1 * (*l1)]).re);
                                (*wi) = (((*W)[1 * (*l1)]).im);
                                (*tmpr) = (((*jp)[1 * (*m)]).re);
                                (*tmpi) = (((*jp)[1 * (*m)]).im);
                                (*r5_1) = (((*wr) * (*tmpr)) - ((*wi) * (*tmpi)));
                                (*i5_1) = (((*wi) * (*tmpr)) + ((*wr) * (*tmpi)));
                                (*wr) = (((*W)[17 * (*l1)]).re);
                                (*wi) = (((*W)[17 * (*l1)]).im);
                                (*tmpr) = (((*jp)[17 * (*m)]).re);
                                (*tmpi) = (((*jp)[17 * (*m)]).im);
                                (*r5_17) = (((*wr) * (*tmpr)) - ((*wi) * (*tmpi)));
                                (*i5_17) = (((*wi) * (*tmpr)) + ((*wr) * (*tmpi)));
                                (*r4_1) = ((*r5_1) + (*r5_17));
                                (*i4_1) = ((*i5_1) + (*i5_17));
                                (*r4_17) = ((*r5_1) - (*r5_17));
                                (*i4_17) = ((*i5_1) - (*i5_17));
                            }
                            {
                                (*wr) = (((*W)[9 * (*l1)]).re);
                                (*wi) = (((*W)[9 * (*l1)]).im);
                                (*tmpr) = (((*jp)[9 * (*m)]).re);
                                (*tmpi) = (((*jp)[9 * (*m)]).im);
                                (*r5_9) = (((*wr) * (*tmpr)) - ((*wi) * (*tmpi)));
                                (*i5_9) = (((*wi) * (*tmpr)) + ((*wr) * (*tmpi)));
                                (*wr) = (((*W)[25 * (*l1)]).re);
                                (*wi) = (((*W)[25 * (*l1)]).im);
                                (*tmpr) = (((*jp)[25 * (*m)]).re);
                                (*tmpi) = (((*jp)[25 * (*m)]).im);
                                (*r5_25) = (((*wr) * (*tmpr)) - ((*wi) * (*tmpi)));
                                (*i5_25) = (((*wi) * (*tmpr)) + ((*wr) * (*tmpi)));
                                (*r4_9) = ((*r5_9) + (*r5_25));
                                (*i4_9) = ((*i5_9) + (*i5_25));
                                (*r4_25) = ((*r5_9) - (*r5_25));
                                (*i4_25) = ((*i5_9) - (*i5_25));
                            }
                            (*r3_1) = ((*r4_1) + (*r4_9));
                            (*i3_1) = ((*i4_1) + (*i4_9));
                            (*r3_17) = ((*r4_1) - (*r4_9));
                            (*i3_17) = ((*i4_1) - (*i4_9));
                            (*r3_9) = ((*r4_17) + (*i4_25));
                            (*i3_9) = ((*i4_17) - (*r4_25));
                            (*r3_25) = ((*r4_17) - (*i4_25));
                            (*i3_25) = ((*i4_17) + (*r4_25));
                        }
                        {
                            {
                                (*wr) = (((*W)[5 * (*l1)]).re);
                                (*wi) = (((*W)[5 * (*l1)]).im);
                                (*tmpr) = (((*jp)[5 * (*m)]).re);
                                (*tmpi) = (((*jp)[5 * (*m)]).im);
                                (*r5_5) = (((*wr) * (*tmpr)) - ((*wi) * (*tmpi)));
                                (*i5_5) = (((*wi) * (*tmpr)) + ((*wr) * (*tmpi)));
                                (*wr) = (((*W)[21 * (*l1)]).re);
                                (*wi) = (((*W)[21 * (*l1)]).im);
                                (*tmpr) = (((*jp)[21 * (*m)]).re);
                                (*tmpi) = (((*jp)[21 * (*m)]).im);
                                (*r5_21) = (((*wr) * (*tmpr)) - ((*wi) * (*tmpi)));
                                (*i5_21) = (((*wi) * (*tmpr)) + ((*wr) * (*tmpi)));
                                (*r4_5) = ((*r5_5) + (*r5_21));
                                (*i4_5) = ((*i5_5) + (*i5_21));
                                (*r4_21) = ((*r5_5) - (*r5_21));
                                (*i4_21) = ((*i5_5) - (*i5_21));
                            }
                            {
                                (*wr) = (((*W)[13 * (*l1)]).re);
                                (*wi) = (((*W)[13 * (*l1)]).im);
                                (*tmpr) = (((*jp)[13 * (*m)]).re);
                                (*tmpi) = (((*jp)[13 * (*m)]).im);
                                (*r5_13) = (((*wr) * (*tmpr)) - ((*wi) * (*tmpi)));
                                (*i5_13) = (((*wi) * (*tmpr)) + ((*wr) * (*tmpi)));
                                (*wr) = (((*W)[29 * (*l1)]).re);
                                (*wi) = (((*W)[29 * (*l1)]).im);
                                (*tmpr) = (((*jp)[29 * (*m)]).re);
                                (*tmpi) = (((*jp)[29 * (*m)]).im);
                                (*r5_29) = (((*wr) * (*tmpr)) - ((*wi) * (*tmpi)));
                                (*i5_29) = (((*wi) * (*tmpr)) + ((*wr) * (*tmpi)));
                                (*r4_13) = ((*r5_13) + (*r5_29));
                                (*i4_13) = ((*i5_13) + (*i5_29));
                                (*r4_29) = ((*r5_13) - (*r5_29));
                                (*i4_29) = ((*i5_13) - (*i5_29));
                            }
                            (*r3_5) = ((*r4_5) + (*r4_13));
                            (*i3_5) = ((*i4_5) + (*i4_13));
                            (*r3_21) = ((*r4_5) - (*r4_13));
                            (*i3_21) = ((*i4_5) - (*i4_13));
                            (*r3_13) = ((*r4_21) + (*i4_29));
                            (*i3_13) = ((*i4_21) - (*r4_29));
                            (*r3_29) = ((*r4_21) - (*i4_29));
                            (*i3_29) = ((*i4_21) + (*r4_29));
                        }
                        (*r2_1) = ((*r3_1) + (*r3_5));
                        (*i2_1) = ((*i3_1) + (*i3_5));
                        (*r2_17) = ((*r3_1) - (*r3_5));
                        (*i2_17) = ((*i3_1) - (*i3_5));
                        (*tmpr) = (0.70710678118699999 * ((*r3_13) + (*i3_13)));
                        (*tmpi) = (0.70710678118699999 * ((*i3_13) - (*r3_13)));
                        (*r2_5) = ((*r3_9) + (*tmpr));
                        (*i2_5) = ((*i3_9) + (*tmpi));
                        (*r2_21) = ((*r3_9) - (*tmpr));
                        (*i2_21) = ((*i3_9) - (*tmpi));
                        (*r2_9) = ((*r3_17) + (*i3_21));
                        (*i2_9) = ((*i3_17) - (*r3_21));
                        (*r2_25) = ((*r3_17) - (*i3_21));
                        (*i2_25) = ((*i3_17) + (*r3_21));
                        (*tmpr) = (0.70710678118699999 * ((*i3_29) - (*r3_29)));
                        (*tmpi) = (0.70710678118699999 * ((*r3_29) + (*i3_29)));
                        (*r2_13) = ((*r3_25) + (*tmpr));
                        (*i2_13) = ((*i3_25) - (*tmpi));
                        (*r2_29) = ((*r3_25) - (*tmpr));
                        (*i2_29) = ((*i3_25) + (*tmpi));
                    }
                    {
                        {
                            {
                                (*wr) = (((*W)[3 * (*l1)]).re);
                                (*wi) = (((*W)[3 * (*l1)]).im);
                                (*tmpr) = (((*jp)[3 * (*m)]).re);
                                (*tmpi) = (((*jp)[3 * (*m)]).im);
                                (*r5_3) = (((*wr) * (*tmpr)) - ((*wi) * (*tmpi)));
                                (*i5_3) = (((*wi) * (*tmpr)) + ((*wr) * (*tmpi)));
                                (*wr) = (((*W)[19 * (*l1)]).re);
                                (*wi) = (((*W)[19 * (*l1)]).im);
                                (*tmpr) = (((*jp)[19 * (*m)]).re);
                                (*tmpi) = (((*jp)[19 * (*m)]).im);
                                (*r5_19) = (((*wr) * (*tmpr)) - ((*wi) * (*tmpi)));
                                (*i5_19) = (((*wi) * (*tmpr)) + ((*wr) * (*tmpi)));
                                (*r4_3) = ((*r5_3) + (*r5_19));
                                (*i4_3) = ((*i5_3) + (*i5_19));
                                (*r4_19) = ((*r5_3) - (*r5_19));
                                (*i4_19) = ((*i5_3) - (*i5_19));
                            }
                            {
                                (*wr) = (((*W)[11 * (*l1)]).re);
                                (*wi) = (((*W)[11 * (*l1)]).im);
                                (*tmpr) = (((*jp)[11 * (*m)]).re);
                                (*tmpi) = (((*jp)[11 * (*m)]).im);
                                (*r5_11) = (((*wr) * (*tmpr)) - ((*wi) * (*tmpi)));
                                (*i5_11) = (((*wi) * (*tmpr)) + ((*wr) * (*tmpi)));
                                (*wr) = (((*W)[27 * (*l1)]).re);
                                (*wi) = (((*W)[27 * (*l1)]).im);
                                (*tmpr) = (((*jp)[27 * (*m)]).re);
                                (*tmpi) = (((*jp)[27 * (*m)]).im);
                                (*r5_27) = (((*wr) * (*tmpr)) - ((*wi) * (*tmpi)));
                                (*i5_27) = (((*wi) * (*tmpr)) + ((*wr) * (*tmpi)));
                                (*r4_11) = ((*r5_11) + (*r5_27));
                                (*i4_11) = ((*i5_11) + (*i5_27));
                                (*r4_27) = ((*r5_11) - (*r5_27));
                                (*i4_27) = ((*i5_11) - (*i5_27));
                            }
                            (*r3_3) = ((*r4_3) + (*r4_11));
                            (*i3_3) = ((*i4_3) + (*i4_11));
                            (*r3_19) = ((*r4_3) - (*r4_11));
                            (*i3_19) = ((*i4_3) - (*i4_11));
                            (*r3_11) = ((*r4_19) + (*i4_27));
                            (*i3_11) = ((*i4_19) - (*r4_27));
                            (*r3_27) = ((*r4_19) - (*i4_27));
                            (*i3_27) = ((*i4_19) + (*r4_27));
                        }
                        {
                            {
                                (*wr) = (((*W)[7 * (*l1)]).re);
                                (*wi) = (((*W)[7 * (*l1)]).im);
                                (*tmpr) = (((*jp)[7 * (*m)]).re);
                                (*tmpi) = (((*jp)[7 * (*m)]).im);
                                (*r5_7) = (((*wr) * (*tmpr)) - ((*wi) * (*tmpi)));
                                (*i5_7) = (((*wi) * (*tmpr)) + ((*wr) * (*tmpi)));
                                (*wr) = (((*W)[23 * (*l1)]).re);
                                (*wi) = (((*W)[23 * (*l1)]).im);
                                (*tmpr) = (((*jp)[23 * (*m)]).re);
                                (*tmpi) = (((*jp)[23 * (*m)]).im);
                                (*r5_23) = (((*wr) * (*tmpr)) - ((*wi) * (*tmpi)));
                                (*i5_23) = (((*wi) * (*tmpr)) + ((*wr) * (*tmpi)));
                                (*r4_7) = ((*r5_7) + (*r5_23));
                                (*i4_7) = ((*i5_7) + (*i5_23));
                                (*r4_23) = ((*r5_7) - (*r5_23));
                                (*i4_23) = ((*i5_7) - (*i5_23));
                            }
                            {
                                (*wr) = (((*W)[15 * (*l1)]).re);
                                (*wi) = (((*W)[15 * (*l1)]).im);
                                (*tmpr) = (((*jp)[15 * (*m)]).re);
                                (*tmpi) = (((*jp)[15 * (*m)]).im);
                                (*r5_15) = (((*wr) * (*tmpr)) - ((*wi) * (*tmpi)));
                                (*i5_15) = (((*wi) * (*tmpr)) + ((*wr) * (*tmpi)));
                                (*wr) = (((*W)[31 * (*l1)]).re);
                                (*wi) = (((*W)[31 * (*l1)]).im);
                                (*tmpr) = (((*jp)[31 * (*m)]).re);
                                (*tmpi) = (((*jp)[31 * (*m)]).im);
                                (*r5_31) = (((*wr) * (*tmpr)) - ((*wi) * (*tmpi)));
                                (*i5_31) = (((*wi) * (*tmpr)) + ((*wr) * (*tmpi)));
                                (*r4_15) = ((*r5_15) + (*r5_31));
                                (*i4_15) = ((*i5_15) + (*i5_31));
                                (*r4_31) = ((*r5_15) - (*r5_31));
                                (*i4_31) = ((*i5_15) - (*i5_31));
                            }
                            (*r3_7) = ((*r4_7) + (*r4_15));
                            (*i3_7) = ((*i4_7) + (*i4_15));
                            (*r3_23) = ((*r4_7) - (*r4_15));
                            (*i3_23) = ((*i4_7) - (*i4_15));
                            (*r3_15) = ((*r4_23) + (*i4_31));
                            (*i3_15) = ((*i4_23) - (*r4_31));
                            (*r3_31) = ((*r4_23) - (*i4_31));
                            (*i3_31) = ((*i4_23) + (*r4_31));
                        }
                        (*r2_3) = ((*r3_3) + (*r3_7));
                        (*i2_3) = ((*i3_3) + (*i3_7));
                        (*r2_19) = ((*r3_3) - (*r3_7));
                        (*i2_19) = ((*i3_3) - (*i3_7));
                        (*tmpr) = (0.70710678118699999 * ((*r3_15) + (*i3_15)));
                        (*tmpi) = (0.70710678118699999 * ((*i3_15) - (*r3_15)));
                        (*r2_7) = ((*r3_11) + (*tmpr));
                        (*i2_7) = ((*i3_11) + (*tmpi));
                        (*r2_23) = ((*r3_11) - (*tmpr));
                        (*i2_23) = ((*i3_11) - (*tmpi));
                        (*r2_11) = ((*r3_19) + (*i3_23));
                        (*i2_11) = ((*i3_19) - (*r3_23));
                        (*r2_27) = ((*r3_19) - (*i3_23));
                        (*i2_27) = ((*i3_19) + (*r3_23));
                        (*tmpr) = (0.70710678118699999 * ((*i3_31) - (*r3_31)));
                        (*tmpi) = (0.70710678118699999 * ((*r3_31) + (*i3_31)));
                        (*r2_15) = ((*r3_27) + (*tmpr));
                        (*i2_15) = ((*i3_27) - (*tmpi));
                        (*r2_31) = ((*r3_27) - (*tmpr));
                        (*i2_31) = ((*i3_27) + (*tmpi));
                    }
                    (*r1_1) = ((*r2_1) + (*r2_3));
                    (*i1_1) = ((*i2_1) + (*i2_3));
                    (*r1_17) = ((*r2_1) - (*r2_3));
                    (*i1_17) = ((*i2_1) - (*i2_3));
                    (*tmpr) = ((0.92387953251099997 * (*r2_7)) + (0.38268343236500002 * (*i2_7)));
                    (*tmpi) = ((0.92387953251099997 * (*i2_7)) - (0.38268343236500002 * (*r2_7)));
                    (*r1_3) = ((*r2_5) + (*tmpr));
                    (*i1_3) = ((*i2_5) + (*tmpi));
                    (*r1_19) = ((*r2_5) - (*tmpr));
                    (*i1_19) = ((*i2_5) - (*tmpi));
                    (*tmpr) = (0.70710678118699999 * ((*r2_11) + (*i2_11)));
                    (*tmpi) = (0.70710678118699999 * ((*i2_11) - (*r2_11)));
                    (*r1_5) = ((*r2_9) + (*tmpr));
                    (*i1_5) = ((*i2_9) + (*tmpi));
                    (*r1_21) = ((*r2_9) - (*tmpr));
                    (*i1_21) = ((*i2_9) - (*tmpi));
                    (*tmpr) = ((0.38268343236500002 * (*r2_15)) + (0.92387953251099997 * (*i2_15)));
                    (*tmpi) = ((0.38268343236500002 * (*i2_15)) - (0.92387953251099997 * (*r2_15)));
                    (*r1_7) = ((*r2_13) + (*tmpr));
                    (*i1_7) = ((*i2_13) + (*tmpi));
                    (*r1_23) = ((*r2_13) - (*tmpr));
                    (*i1_23) = ((*i2_13) - (*tmpi));
                    (*r1_9) = ((*r2_17) + (*i2_19));
                    (*i1_9) = ((*i2_17) - (*r2_19));
                    (*r1_25) = ((*r2_17) - (*i2_19));
                    (*i1_25) = ((*i2_17) + (*r2_19));
                    (*tmpr) = ((0.92387953251099997 * (*i2_23)) - (0.38268343236500002 * (*r2_23)));
                    (*tmpi) = ((0.92387953251099997 * (*r2_23)) + (0.38268343236500002 * (*i2_23)));
                    (*r1_11) = ((*r2_21) + (*tmpr));
                    (*i1_11) = ((*i2_21) - (*tmpi));
                    (*r1_27) = ((*r2_21) - (*tmpr));
                    (*i1_27) = ((*i2_21) + (*tmpi));
                    (*tmpr) = (0.70710678118699999 * ((*i2_27) - (*r2_27)));
                    (*tmpi) = (0.70710678118699999 * ((*r2_27) + (*i2_27)));
                    (*r1_13) = ((*r2_25) + (*tmpr));
                    (*i1_13) = ((*i2_25) - (*tmpi));
                    (*r1_29) = ((*r2_25) - (*tmpr));
                    (*i1_29) = ((*i2_25) + (*tmpi));
                    (*tmpr) = ((0.38268343236500002 * (*i2_31)) - (0.92387953251099997 * (*r2_31)));
                    (*tmpi) = ((0.38268343236500002 * (*r2_31)) + (0.92387953251099997 * (*i2_31)));
                    (*r1_15) = ((*r2_29) + (*tmpr));
                    (*i1_15) = ((*i2_29) - (*tmpi));
                    (*r1_31) = ((*r2_29) - (*tmpr));
                    (*i1_31) = ((*i2_29) + (*tmpi));
                }
                (((*kp)[0 * (*m)]).re) = ((*r1_0) + (*r1_1));
                (((*kp)[0 * (*m)]).im) = ((*i1_0) + (*i1_1));
                (((*kp)[16 * (*m)]).re) = ((*r1_0) - (*r1_1));
                (((*kp)[16 * (*m)]).im) = ((*i1_0) - (*i1_1));
                (*tmpr) = ((0.98078528040299994 * (*r1_3)) + (0.19509032201599999 * (*i1_3)));
                (*tmpi) = ((0.98078528040299994 * (*i1_3)) - (0.19509032201599999 * (*r1_3)));
                (((*kp)[1 * (*m)]).re) = ((*r1_2) + (*tmpr));
                (((*kp)[1 * (*m)]).im) = ((*i1_2) + (*tmpi));
                (((*kp)[17 * (*m)]).re) = ((*r1_2) - (*tmpr));
                (((*kp)[17 * (*m)]).im) = ((*i1_2) - (*tmpi));
                (*tmpr) = ((0.92387953251099997 * (*r1_5)) + (0.38268343236500002 * (*i1_5)));
                (*tmpi) = ((0.92387953251099997 * (*i1_5)) - (0.38268343236500002 * (*r1_5)));
                (((*kp)[2 * (*m)]).re) = ((*r1_4) + (*tmpr));
                (((*kp)[2 * (*m)]).im) = ((*i1_4) + (*tmpi));
                (((*kp)[18 * (*m)]).re) = ((*r1_4) - (*tmpr));
                (((*kp)[18 * (*m)]).im) = ((*i1_4) - (*tmpi));
                (*tmpr) = ((0.83146961230299998 * (*r1_7)) + (0.55557023301999997 * (*i1_7)));
                (*tmpi) = ((0.83146961230299998 * (*i1_7)) - (0.55557023301999997 * (*r1_7)));
                (((*kp)[3 * (*m)]).re) = ((*r1_6) + (*tmpr));
                (((*kp)[3 * (*m)]).im) = ((*i1_6) + (*tmpi));
                (((*kp)[19 * (*m)]).re) = ((*r1_6) - (*tmpr));
                (((*kp)[19 * (*m)]).im) = ((*i1_6) - (*tmpi));
                (*tmpr) = (0.70710678118699999 * ((*r1_9) + (*i1_9)));
                (*tmpi) = (0.70710678118699999 * ((*i1_9) - (*r1_9)));
                (((*kp)[4 * (*m)]).re) = ((*r1_8) + (*tmpr));
                (((*kp)[4 * (*m)]).im) = ((*i1_8) + (*tmpi));
                (((*kp)[20 * (*m)]).re) = ((*r1_8) - (*tmpr));
                (((*kp)[20 * (*m)]).im) = ((*i1_8) - (*tmpi));
                (*tmpr) = ((0.55557023301999997 * (*r1_11)) + (0.83146961230299998 * (*i1_11)));
                (*tmpi) = ((0.55557023301999997 * (*i1_11)) - (0.83146961230299998 * (*r1_11)));
                (((*kp)[5 * (*m)]).re) = ((*r1_10) + (*tmpr));
                (((*kp)[5 * (*m)]).im) = ((*i1_10) + (*tmpi));
                (((*kp)[21 * (*m)]).re) = ((*r1_10) - (*tmpr));
                (((*kp)[21 * (*m)]).im) = ((*i1_10) - (*tmpi));
                (*tmpr) = ((0.38268343236500002 * (*r1_13)) + (0.92387953251099997 * (*i1_13)));
                (*tmpi) = ((0.38268343236500002 * (*i1_13)) - (0.92387953251099997 * (*r1_13)));
                (((*kp)[6 * (*m)]).re) = ((*r1_12) + (*tmpr));
                (((*kp)[6 * (*m)]).im) = ((*i1_12) + (*tmpi));
                (((*kp)[22 * (*m)]).re) = ((*r1_12) - (*tmpr));
                (((*kp)[22 * (*m)]).im) = ((*i1_12) - (*tmpi));
                (*tmpr) = ((0.19509032201599999 * (*r1_15)) + (0.98078528040299994 * (*i1_15)));
                (*tmpi) = ((0.19509032201599999 * (*i1_15)) - (0.98078528040299994 * (*r1_15)));
                (((*kp)[7 * (*m)]).re) = ((*r1_14) + (*tmpr));
                (((*kp)[7 * (*m)]).im) = ((*i1_14) + (*tmpi));
                (((*kp)[23 * (*m)]).re) = ((*r1_14) - (*tmpr));
                (((*kp)[23 * (*m)]).im) = ((*i1_14) - (*tmpi));
                (((*kp)[8 * (*m)]).re) = ((*r1_16) + (*i1_17));
                (((*kp)[8 * (*m)]).im) = ((*i1_16) - (*r1_17));
                (((*kp)[24 * (*m)]).re) = ((*r1_16) - (*i1_17));
                (((*kp)[24 * (*m)]).im) = ((*i1_16) + (*r1_17));
                (*tmpr) = ((0.98078528040299994 * (*i1_19)) - (0.19509032201599999 * (*r1_19)));
                (*tmpi) = ((0.98078528040299994 * (*r1_19)) + (0.19509032201599999 * (*i1_19)));
                (((*kp)[9 * (*m)]).re) = ((*r1_18) + (*tmpr));
                (((*kp)[9 * (*m)]).im) = ((*i1_18) - (*tmpi));
                (((*kp)[25 * (*m)]).re) = ((*r1_18) - (*tmpr));
                (((*kp)[25 * (*m)]).im) = ((*i1_18) + (*tmpi));
                (*tmpr) = ((0.92387953251099997 * (*i1_21)) - (0.38268343236500002 * (*r1_21)));
                (*tmpi) = ((0.92387953251099997 * (*r1_21)) + (0.38268343236500002 * (*i1_21)));
                (((*kp)[10 * (*m)]).re) = ((*r1_20) + (*tmpr));
                (((*kp)[10 * (*m)]).im) = ((*i1_20) - (*tmpi));
                (((*kp)[26 * (*m)]).re) = ((*r1_20) - (*tmpr));
                (((*kp)[26 * (*m)]).im) = ((*i1_20) + (*tmpi));
                (*tmpr) = ((0.83146961230299998 * (*i1_23)) - (0.55557023301999997 * (*r1_23)));
                (*tmpi) = ((0.83146961230299998 * (*r1_23)) + (0.55557023301999997 * (*i1_23)));
                (((*kp)[11 * (*m)]).re) = ((*r1_22) + (*tmpr));
                (((*kp)[11 * (*m)]).im) = ((*i1_22) - (*tmpi));
                (((*kp)[27 * (*m)]).re) = ((*r1_22) - (*tmpr));
                (((*kp)[27 * (*m)]).im) = ((*i1_22) + (*tmpi));
                (*tmpr) = (0.70710678118699999 * ((*i1_25) - (*r1_25)));
                (*tmpi) = (0.70710678118699999 * ((*r1_25) + (*i1_25)));
                (((*kp)[12 * (*m)]).re) = ((*r1_24) + (*tmpr));
                (((*kp)[12 * (*m)]).im) = ((*i1_24) - (*tmpi));
                (((*kp)[28 * (*m)]).re) = ((*r1_24) - (*tmpr));
                (((*kp)[28 * (*m)]).im) = ((*i1_24) + (*tmpi));
                (*tmpr) = ((0.55557023301999997 * (*i1_27)) - (0.83146961230299998 * (*r1_27)));
                (*tmpi) = ((0.55557023301999997 * (*r1_27)) + (0.83146961230299998 * (*i1_27)));
                (((*kp)[13 * (*m)]).re) = ((*r1_26) + (*tmpr));
                (((*kp)[13 * (*m)]).im) = ((*i1_26) - (*tmpi));
                (((*kp)[29 * (*m)]).re) = ((*r1_26) - (*tmpr));
                (((*kp)[29 * (*m)]).im) = ((*i1_26) + (*tmpi));
                (*tmpr) = ((0.38268343236500002 * (*i1_29)) - (0.92387953251099997 * (*r1_29)));
                (*tmpi) = ((0.38268343236500002 * (*r1_29)) + (0.92387953251099997 * (*i1_29)));
                (((*kp)[14 * (*m)]).re) = ((*r1_28) + (*tmpr));
                (((*kp)[14 * (*m)]).im) = ((*i1_28) - (*tmpi));
                (((*kp)[30 * (*m)]).re) = ((*r1_28) - (*tmpr));
                (((*kp)[30 * (*m)]).im) = ((*i1_28) + (*tmpi));
                (*tmpr) = ((0.19509032201599999 * (*i1_31)) - (0.98078528040299994 * (*r1_31)));
                (*tmpi) = ((0.19509032201599999 * (*r1_31)) + (0.98078528040299994 * (*i1_31)));
                (((*kp)[15 * (*m)]).re) = ((*r1_30) + (*tmpr));
                (((*kp)[15 * (*m)]).im) = ((*i1_30) - (*tmpi));
                (((*kp)[31 * (*m)]).re) = ((*r1_30) - (*tmpr));
                (((*kp)[31 * (*m)]).im) = ((*i1_30) + (*tmpi));
            }
            (*l1) += (*nWdn);
            (*kp)++;
        }
        (this->inputsTPParent->i_darts44[this->getID()]) = i_darts_counter_temp44;
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/

    myTP->controlTPParent->nextCodeletsfft_twiddle_32[this->getID()]->decDep();
}
void TP44::_checkInCodelets8647::fire(void)
{

    /*printing node 8647: DeclStmt*/
    this->inputsTPParent->ab_darts44[this->getID()]
        = ((this->inputsTPParent->a_darts44[this->getID()])
              + (this->inputsTPParent->b_darts44[this->getID()]))
        / 2;
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 8647 nextRegion: 8650 */
    myTP->controlTPParent->checkInCodelets8650[this->getID()].decDep();
}
void TP44::_checkInCodelets8650::fire(void)
{

    /*printing node 8650: OMPTaskDirective*/
    /*syncNode: OMPTaskwaitDirective 8654*/
    /*Find the TP to which the nextRegion belongs*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->barrierCodelets8654[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 8650*/
    _task8650Inputs* task8650Inputs
        = new _task8650Inputs(&((this->inputsTPParent->W_darts44[this->getID()])),
            &((this->inputsTPParent->a_darts44[this->getID()])),
            &((this->inputsTPParent->ab_darts44[this->getID()])),
            &((this->inputsTPParent->in_darts44[this->getID()])),
            &((this->inputsTPParent->m_darts44[this->getID()])),
            &((this->inputsTPParent->nW_darts44[this->getID()])),
            &((this->inputsTPParent->nWdn_darts44[this->getID()])),
            &((this->inputsTPParent->out_darts44[this->getID()])), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task8650Inputs[this->getID()] = task8650Inputs;
    invoke<TP8650>(myTP, 1, this->getID(), myTP, task8650Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 8650 nextRegion: 8652 */
    myTP->controlTPParent->checkInCodelets8652[this->getID()].decDep();
}
void TP44::_checkInCodelets8652::fire(void)
{

    /*printing node 8652: OMPTaskDirective*/
    /*syncNode: OMPTaskwaitDirective 8654*/
    /*Find the TP to which the nextRegion belongs*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->barrierCodelets8654[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 8652*/
    _task8652Inputs* task8652Inputs
        = new _task8652Inputs(&((this->inputsTPParent->W_darts44[this->getID()])),
            &((this->inputsTPParent->ab_darts44[this->getID()])),
            &((this->inputsTPParent->b_darts44[this->getID()])),
            &((this->inputsTPParent->in_darts44[this->getID()])),
            &((this->inputsTPParent->m_darts44[this->getID()])),
            &((this->inputsTPParent->nW_darts44[this->getID()])),
            &((this->inputsTPParent->nWdn_darts44[this->getID()])),
            &((this->inputsTPParent->out_darts44[this->getID()])), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task8652Inputs[this->getID()] = task8652Inputs;
    invoke<TP8652>(myTP, 1, this->getID(), myTP, task8652Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 8652 nextRegion: 8654 */
    myTP->controlTPParent->barrierCodelets8654[0].decDep();
}
void TP44::_barrierCodelets8654::fire(void)
{
    TP44* myTP = static_cast<TP44*>(myTP_);

    for (size_t codeletsCounter = 0; codeletsCounter < (size_t)myTP->numThreads;
         codeletsCounter++) {
        myTP->nextCodeletsfft_twiddle_32[codeletsCounter]->decDep();
    }
}
TP44::TP44(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_mainNextCodelet,
    darts::Codelet* in_mainSyncCodelet, TP44** in_ptrToThisFunctionTP, int in_a, int in_b,
    COMPLEX* in_in, COMPLEX* in_out, COMPLEX* in_W, int in_nW, int in_nWdn, int in_m)
    : ompTP(in_numThreads, in_mainCodeletID)
    , ptrToThisFunctionTP(in_ptrToThisFunctionTP)
    , inputsTPParent(this)
    , controlTPParent(this)
    , nextCodeletsfft_twiddle_32(new Codelet*[in_numThreads])
    , nextSyncCodeletsfft_twiddle_32(new Codelet*[in_numThreads])
    , a_darts44(new int[this->numThreads])
    , b_darts44(new int[this->numThreads])
    , in_darts44(new COMPLEX*[this->numThreads])
    , out_darts44(new COMPLEX*[this->numThreads])
    , W_darts44(new COMPLEX*[this->numThreads])
    , nW_darts44(new int[this->numThreads])
    , nWdn_darts44(new int[this->numThreads])
    , m_darts44(new int[this->numThreads])
    , ab_darts44(new int[this->numThreads])
    , i_darts44(new int[this->numThreads])
    , i1_0_darts44(new REAL[this->numThreads])
    , i1_1_darts44(new REAL[this->numThreads])
    , i1_10_darts44(new REAL[this->numThreads])
    , i1_11_darts44(new REAL[this->numThreads])
    , i1_12_darts44(new REAL[this->numThreads])
    , i1_13_darts44(new REAL[this->numThreads])
    , i1_14_darts44(new REAL[this->numThreads])
    , i1_15_darts44(new REAL[this->numThreads])
    , i1_16_darts44(new REAL[this->numThreads])
    , i1_17_darts44(new REAL[this->numThreads])
    , i1_18_darts44(new REAL[this->numThreads])
    , i1_19_darts44(new REAL[this->numThreads])
    , i1_2_darts44(new REAL[this->numThreads])
    , i1_20_darts44(new REAL[this->numThreads])
    , i1_21_darts44(new REAL[this->numThreads])
    , i1_22_darts44(new REAL[this->numThreads])
    , i1_23_darts44(new REAL[this->numThreads])
    , i1_24_darts44(new REAL[this->numThreads])
    , i1_25_darts44(new REAL[this->numThreads])
    , i1_26_darts44(new REAL[this->numThreads])
    , i1_27_darts44(new REAL[this->numThreads])
    , i1_28_darts44(new REAL[this->numThreads])
    , i1_29_darts44(new REAL[this->numThreads])
    , i1_3_darts44(new REAL[this->numThreads])
    , i1_30_darts44(new REAL[this->numThreads])
    , i1_31_darts44(new REAL[this->numThreads])
    , i1_4_darts44(new REAL[this->numThreads])
    , i1_5_darts44(new REAL[this->numThreads])
    , i1_6_darts44(new REAL[this->numThreads])
    , i1_7_darts44(new REAL[this->numThreads])
    , i1_8_darts44(new REAL[this->numThreads])
    , i1_9_darts44(new REAL[this->numThreads])
    , i2_0_darts44(new REAL[this->numThreads])
    , i2_1_darts44(new REAL[this->numThreads])
    , i2_10_darts44(new REAL[this->numThreads])
    , i2_11_darts44(new REAL[this->numThreads])
    , i2_12_darts44(new REAL[this->numThreads])
    , i2_13_darts44(new REAL[this->numThreads])
    , i2_14_darts44(new REAL[this->numThreads])
    , i2_15_darts44(new REAL[this->numThreads])
    , i2_16_darts44(new REAL[this->numThreads])
    , i2_17_darts44(new REAL[this->numThreads])
    , i2_18_darts44(new REAL[this->numThreads])
    , i2_19_darts44(new REAL[this->numThreads])
    , i2_2_darts44(new REAL[this->numThreads])
    , i2_20_darts44(new REAL[this->numThreads])
    , i2_21_darts44(new REAL[this->numThreads])
    , i2_22_darts44(new REAL[this->numThreads])
    , i2_23_darts44(new REAL[this->numThreads])
    , i2_24_darts44(new REAL[this->numThreads])
    , i2_25_darts44(new REAL[this->numThreads])
    , i2_26_darts44(new REAL[this->numThreads])
    , i2_27_darts44(new REAL[this->numThreads])
    , i2_28_darts44(new REAL[this->numThreads])
    , i2_29_darts44(new REAL[this->numThreads])
    , i2_3_darts44(new REAL[this->numThreads])
    , i2_30_darts44(new REAL[this->numThreads])
    , i2_31_darts44(new REAL[this->numThreads])
    , i2_4_darts44(new REAL[this->numThreads])
    , i2_5_darts44(new REAL[this->numThreads])
    , i2_6_darts44(new REAL[this->numThreads])
    , i2_7_darts44(new REAL[this->numThreads])
    , i2_8_darts44(new REAL[this->numThreads])
    , i2_9_darts44(new REAL[this->numThreads])
    , i3_0_darts44(new REAL[this->numThreads])
    , i3_1_darts44(new REAL[this->numThreads])
    , i3_10_darts44(new REAL[this->numThreads])
    , i3_11_darts44(new REAL[this->numThreads])
    , i3_12_darts44(new REAL[this->numThreads])
    , i3_13_darts44(new REAL[this->numThreads])
    , i3_14_darts44(new REAL[this->numThreads])
    , i3_15_darts44(new REAL[this->numThreads])
    , i3_16_darts44(new REAL[this->numThreads])
    , i3_17_darts44(new REAL[this->numThreads])
    , i3_18_darts44(new REAL[this->numThreads])
    , i3_19_darts44(new REAL[this->numThreads])
    , i3_2_darts44(new REAL[this->numThreads])
    , i3_20_darts44(new REAL[this->numThreads])
    , i3_21_darts44(new REAL[this->numThreads])
    , i3_22_darts44(new REAL[this->numThreads])
    , i3_23_darts44(new REAL[this->numThreads])
    , i3_24_darts44(new REAL[this->numThreads])
    , i3_25_darts44(new REAL[this->numThreads])
    , i3_26_darts44(new REAL[this->numThreads])
    , i3_27_darts44(new REAL[this->numThreads])
    , i3_28_darts44(new REAL[this->numThreads])
    , i3_29_darts44(new REAL[this->numThreads])
    , i3_3_darts44(new REAL[this->numThreads])
    , i3_30_darts44(new REAL[this->numThreads])
    , i3_31_darts44(new REAL[this->numThreads])
    , i3_4_darts44(new REAL[this->numThreads])
    , i3_5_darts44(new REAL[this->numThreads])
    , i3_6_darts44(new REAL[this->numThreads])
    , i3_7_darts44(new REAL[this->numThreads])
    , i3_8_darts44(new REAL[this->numThreads])
    , i3_9_darts44(new REAL[this->numThreads])
    , i4_0_darts44(new REAL[this->numThreads])
    , i4_1_darts44(new REAL[this->numThreads])
    , i4_10_darts44(new REAL[this->numThreads])
    , i4_11_darts44(new REAL[this->numThreads])
    , i4_12_darts44(new REAL[this->numThreads])
    , i4_13_darts44(new REAL[this->numThreads])
    , i4_14_darts44(new REAL[this->numThreads])
    , i4_15_darts44(new REAL[this->numThreads])
    , i4_16_darts44(new REAL[this->numThreads])
    , i4_17_darts44(new REAL[this->numThreads])
    , i4_18_darts44(new REAL[this->numThreads])
    , i4_19_darts44(new REAL[this->numThreads])
    , i4_2_darts44(new REAL[this->numThreads])
    , i4_20_darts44(new REAL[this->numThreads])
    , i4_21_darts44(new REAL[this->numThreads])
    , i4_22_darts44(new REAL[this->numThreads])
    , i4_23_darts44(new REAL[this->numThreads])
    , i4_24_darts44(new REAL[this->numThreads])
    , i4_25_darts44(new REAL[this->numThreads])
    , i4_26_darts44(new REAL[this->numThreads])
    , i4_27_darts44(new REAL[this->numThreads])
    , i4_28_darts44(new REAL[this->numThreads])
    , i4_29_darts44(new REAL[this->numThreads])
    , i4_3_darts44(new REAL[this->numThreads])
    , i4_30_darts44(new REAL[this->numThreads])
    , i4_31_darts44(new REAL[this->numThreads])
    , i4_4_darts44(new REAL[this->numThreads])
    , i4_5_darts44(new REAL[this->numThreads])
    , i4_6_darts44(new REAL[this->numThreads])
    , i4_7_darts44(new REAL[this->numThreads])
    , i4_8_darts44(new REAL[this->numThreads])
    , i4_9_darts44(new REAL[this->numThreads])
    , i5_0_darts44(new REAL[this->numThreads])
    , i5_1_darts44(new REAL[this->numThreads])
    , i5_10_darts44(new REAL[this->numThreads])
    , i5_11_darts44(new REAL[this->numThreads])
    , i5_12_darts44(new REAL[this->numThreads])
    , i5_13_darts44(new REAL[this->numThreads])
    , i5_14_darts44(new REAL[this->numThreads])
    , i5_15_darts44(new REAL[this->numThreads])
    , i5_16_darts44(new REAL[this->numThreads])
    , i5_17_darts44(new REAL[this->numThreads])
    , i5_18_darts44(new REAL[this->numThreads])
    , i5_19_darts44(new REAL[this->numThreads])
    , i5_2_darts44(new REAL[this->numThreads])
    , i5_20_darts44(new REAL[this->numThreads])
    , i5_21_darts44(new REAL[this->numThreads])
    , i5_22_darts44(new REAL[this->numThreads])
    , i5_23_darts44(new REAL[this->numThreads])
    , i5_24_darts44(new REAL[this->numThreads])
    , i5_25_darts44(new REAL[this->numThreads])
    , i5_26_darts44(new REAL[this->numThreads])
    , i5_27_darts44(new REAL[this->numThreads])
    , i5_28_darts44(new REAL[this->numThreads])
    , i5_29_darts44(new REAL[this->numThreads])
    , i5_3_darts44(new REAL[this->numThreads])
    , i5_30_darts44(new REAL[this->numThreads])
    , i5_31_darts44(new REAL[this->numThreads])
    , i5_4_darts44(new REAL[this->numThreads])
    , i5_5_darts44(new REAL[this->numThreads])
    , i5_6_darts44(new REAL[this->numThreads])
    , i5_7_darts44(new REAL[this->numThreads])
    , i5_8_darts44(new REAL[this->numThreads])
    , i5_9_darts44(new REAL[this->numThreads])
    , jp_darts44(new COMPLEX*[this->numThreads])
    , kp_darts44(new COMPLEX*[this->numThreads])
    , l1_darts44(new int[this->numThreads])
    , r1_0_darts44(new REAL[this->numThreads])
    , r1_1_darts44(new REAL[this->numThreads])
    , r1_10_darts44(new REAL[this->numThreads])
    , r1_11_darts44(new REAL[this->numThreads])
    , r1_12_darts44(new REAL[this->numThreads])
    , r1_13_darts44(new REAL[this->numThreads])
    , r1_14_darts44(new REAL[this->numThreads])
    , r1_15_darts44(new REAL[this->numThreads])
    , r1_16_darts44(new REAL[this->numThreads])
    , r1_17_darts44(new REAL[this->numThreads])
    , r1_18_darts44(new REAL[this->numThreads])
    , r1_19_darts44(new REAL[this->numThreads])
    , r1_2_darts44(new REAL[this->numThreads])
    , r1_20_darts44(new REAL[this->numThreads])
    , r1_21_darts44(new REAL[this->numThreads])
    , r1_22_darts44(new REAL[this->numThreads])
    , r1_23_darts44(new REAL[this->numThreads])
    , r1_24_darts44(new REAL[this->numThreads])
    , r1_25_darts44(new REAL[this->numThreads])
    , r1_26_darts44(new REAL[this->numThreads])
    , r1_27_darts44(new REAL[this->numThreads])
    , r1_28_darts44(new REAL[this->numThreads])
    , r1_29_darts44(new REAL[this->numThreads])
    , r1_3_darts44(new REAL[this->numThreads])
    , r1_30_darts44(new REAL[this->numThreads])
    , r1_31_darts44(new REAL[this->numThreads])
    , r1_4_darts44(new REAL[this->numThreads])
    , r1_5_darts44(new REAL[this->numThreads])
    , r1_6_darts44(new REAL[this->numThreads])
    , r1_7_darts44(new REAL[this->numThreads])
    , r1_8_darts44(new REAL[this->numThreads])
    , r1_9_darts44(new REAL[this->numThreads])
    , r2_0_darts44(new REAL[this->numThreads])
    , r2_1_darts44(new REAL[this->numThreads])
    , r2_10_darts44(new REAL[this->numThreads])
    , r2_11_darts44(new REAL[this->numThreads])
    , r2_12_darts44(new REAL[this->numThreads])
    , r2_13_darts44(new REAL[this->numThreads])
    , r2_14_darts44(new REAL[this->numThreads])
    , r2_15_darts44(new REAL[this->numThreads])
    , r2_16_darts44(new REAL[this->numThreads])
    , r2_17_darts44(new REAL[this->numThreads])
    , r2_18_darts44(new REAL[this->numThreads])
    , r2_19_darts44(new REAL[this->numThreads])
    , r2_2_darts44(new REAL[this->numThreads])
    , r2_20_darts44(new REAL[this->numThreads])
    , r2_21_darts44(new REAL[this->numThreads])
    , r2_22_darts44(new REAL[this->numThreads])
    , r2_23_darts44(new REAL[this->numThreads])
    , r2_24_darts44(new REAL[this->numThreads])
    , r2_25_darts44(new REAL[this->numThreads])
    , r2_26_darts44(new REAL[this->numThreads])
    , r2_27_darts44(new REAL[this->numThreads])
    , r2_28_darts44(new REAL[this->numThreads])
    , r2_29_darts44(new REAL[this->numThreads])
    , r2_3_darts44(new REAL[this->numThreads])
    , r2_30_darts44(new REAL[this->numThreads])
    , r2_31_darts44(new REAL[this->numThreads])
    , r2_4_darts44(new REAL[this->numThreads])
    , r2_5_darts44(new REAL[this->numThreads])
    , r2_6_darts44(new REAL[this->numThreads])
    , r2_7_darts44(new REAL[this->numThreads])
    , r2_8_darts44(new REAL[this->numThreads])
    , r2_9_darts44(new REAL[this->numThreads])
    , r3_0_darts44(new REAL[this->numThreads])
    , r3_1_darts44(new REAL[this->numThreads])
    , r3_10_darts44(new REAL[this->numThreads])
    , r3_11_darts44(new REAL[this->numThreads])
    , r3_12_darts44(new REAL[this->numThreads])
    , r3_13_darts44(new REAL[this->numThreads])
    , r3_14_darts44(new REAL[this->numThreads])
    , r3_15_darts44(new REAL[this->numThreads])
    , r3_16_darts44(new REAL[this->numThreads])
    , r3_17_darts44(new REAL[this->numThreads])
    , r3_18_darts44(new REAL[this->numThreads])
    , r3_19_darts44(new REAL[this->numThreads])
    , r3_2_darts44(new REAL[this->numThreads])
    , r3_20_darts44(new REAL[this->numThreads])
    , r3_21_darts44(new REAL[this->numThreads])
    , r3_22_darts44(new REAL[this->numThreads])
    , r3_23_darts44(new REAL[this->numThreads])
    , r3_24_darts44(new REAL[this->numThreads])
    , r3_25_darts44(new REAL[this->numThreads])
    , r3_26_darts44(new REAL[this->numThreads])
    , r3_27_darts44(new REAL[this->numThreads])
    , r3_28_darts44(new REAL[this->numThreads])
    , r3_29_darts44(new REAL[this->numThreads])
    , r3_3_darts44(new REAL[this->numThreads])
    , r3_30_darts44(new REAL[this->numThreads])
    , r3_31_darts44(new REAL[this->numThreads])
    , r3_4_darts44(new REAL[this->numThreads])
    , r3_5_darts44(new REAL[this->numThreads])
    , r3_6_darts44(new REAL[this->numThreads])
    , r3_7_darts44(new REAL[this->numThreads])
    , r3_8_darts44(new REAL[this->numThreads])
    , r3_9_darts44(new REAL[this->numThreads])
    , r4_0_darts44(new REAL[this->numThreads])
    , r4_1_darts44(new REAL[this->numThreads])
    , r4_10_darts44(new REAL[this->numThreads])
    , r4_11_darts44(new REAL[this->numThreads])
    , r4_12_darts44(new REAL[this->numThreads])
    , r4_13_darts44(new REAL[this->numThreads])
    , r4_14_darts44(new REAL[this->numThreads])
    , r4_15_darts44(new REAL[this->numThreads])
    , r4_16_darts44(new REAL[this->numThreads])
    , r4_17_darts44(new REAL[this->numThreads])
    , r4_18_darts44(new REAL[this->numThreads])
    , r4_19_darts44(new REAL[this->numThreads])
    , r4_2_darts44(new REAL[this->numThreads])
    , r4_20_darts44(new REAL[this->numThreads])
    , r4_21_darts44(new REAL[this->numThreads])
    , r4_22_darts44(new REAL[this->numThreads])
    , r4_23_darts44(new REAL[this->numThreads])
    , r4_24_darts44(new REAL[this->numThreads])
    , r4_25_darts44(new REAL[this->numThreads])
    , r4_26_darts44(new REAL[this->numThreads])
    , r4_27_darts44(new REAL[this->numThreads])
    , r4_28_darts44(new REAL[this->numThreads])
    , r4_29_darts44(new REAL[this->numThreads])
    , r4_3_darts44(new REAL[this->numThreads])
    , r4_30_darts44(new REAL[this->numThreads])
    , r4_31_darts44(new REAL[this->numThreads])
    , r4_4_darts44(new REAL[this->numThreads])
    , r4_5_darts44(new REAL[this->numThreads])
    , r4_6_darts44(new REAL[this->numThreads])
    , r4_7_darts44(new REAL[this->numThreads])
    , r4_8_darts44(new REAL[this->numThreads])
    , r4_9_darts44(new REAL[this->numThreads])
    , r5_0_darts44(new REAL[this->numThreads])
    , r5_1_darts44(new REAL[this->numThreads])
    , r5_10_darts44(new REAL[this->numThreads])
    , r5_11_darts44(new REAL[this->numThreads])
    , r5_12_darts44(new REAL[this->numThreads])
    , r5_13_darts44(new REAL[this->numThreads])
    , r5_14_darts44(new REAL[this->numThreads])
    , r5_15_darts44(new REAL[this->numThreads])
    , r5_16_darts44(new REAL[this->numThreads])
    , r5_17_darts44(new REAL[this->numThreads])
    , r5_18_darts44(new REAL[this->numThreads])
    , r5_19_darts44(new REAL[this->numThreads])
    , r5_2_darts44(new REAL[this->numThreads])
    , r5_20_darts44(new REAL[this->numThreads])
    , r5_21_darts44(new REAL[this->numThreads])
    , r5_22_darts44(new REAL[this->numThreads])
    , r5_23_darts44(new REAL[this->numThreads])
    , r5_24_darts44(new REAL[this->numThreads])
    , r5_25_darts44(new REAL[this->numThreads])
    , r5_26_darts44(new REAL[this->numThreads])
    , r5_27_darts44(new REAL[this->numThreads])
    , r5_28_darts44(new REAL[this->numThreads])
    , r5_29_darts44(new REAL[this->numThreads])
    , r5_3_darts44(new REAL[this->numThreads])
    , r5_30_darts44(new REAL[this->numThreads])
    , r5_31_darts44(new REAL[this->numThreads])
    , r5_4_darts44(new REAL[this->numThreads])
    , r5_5_darts44(new REAL[this->numThreads])
    , r5_6_darts44(new REAL[this->numThreads])
    , r5_7_darts44(new REAL[this->numThreads])
    , r5_8_darts44(new REAL[this->numThreads])
    , r5_9_darts44(new REAL[this->numThreads])
    , tmpi_darts44(new REAL[this->numThreads])
    , tmpr_darts44(new REAL[this->numThreads])
    , wi_darts44(new REAL[this->numThreads])
    , wr_darts44(new REAL[this->numThreads])
    , task8650Inputs(new _task8650Inputs*[this->numThreads])
    , task8652Inputs(new _task8652Inputs*[this->numThreads])
    , checkInCodelets6503(new _checkInCodelets6503[this->numThreads])
    , checkInCodelets6506(new _checkInCodelets6506[this->numThreads])
    , checkInCodelets8647(new _checkInCodelets8647[this->numThreads])
    , checkInCodelets8650(new _checkInCodelets8650[this->numThreads])
    , checkInCodelets8652(new _checkInCodelets8652[this->numThreads])
    , barrierCodelets8654(new _barrierCodelets8654[1])
{
    barrierCodelets8654[0] = _barrierCodelets8654(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets8652* checkInCodelets8652Ptr = (this->checkInCodelets8652);
    _checkInCodelets8650* checkInCodelets8650Ptr = (this->checkInCodelets8650);
    _checkInCodelets8647* checkInCodelets8647Ptr = (this->checkInCodelets8647);
    _checkInCodelets6506* checkInCodelets6506Ptr = (this->checkInCodelets6506);
    _checkInCodelets6503* checkInCodelets6503Ptr = (this->checkInCodelets6503);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets6503);
#endif
    for (size_t codeletCounter = 0; codeletCounter < this->numThreads; codeletCounter++) {
        (*checkInCodelets8652Ptr) = _checkInCodelets8652(1, 1, this, codeletCounter);
        checkInCodelets8652Ptr++;
        (*checkInCodelets8650Ptr) = _checkInCodelets8650(1, 1, this, codeletCounter);
        checkInCodelets8650Ptr++;
        (*checkInCodelets8647Ptr) = _checkInCodelets8647(1, 1, this, codeletCounter);
        checkInCodelets8647Ptr++;
        (*checkInCodelets6506Ptr) = _checkInCodelets6506(1, 1, this, codeletCounter);
        checkInCodelets6506Ptr++;
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets6503Ptr) = _checkInCodelets6503(2, 1, this, codeletCounter);
#else
        (*checkInCodelets6503Ptr) = _checkInCodelets6503(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets6503Ptr).decDep();
        checkInCodelets6503Ptr++;
    }
    if (this->numThreads == 1) {
        this->nextCodeletsfft_twiddle_32[0] = in_mainNextCodelet;
        this->nextSyncCodeletsfft_twiddle_32[0] = in_mainSyncCodelet;
        this->a_darts44[0] = in_a;
        this->b_darts44[0] = in_b;
        this->in_darts44[0] = in_in;
        this->out_darts44[0] = in_out;
        this->W_darts44[0] = in_W;
        this->nW_darts44[0] = in_nW;
        this->nWdn_darts44[0] = in_nWdn;
        this->m_darts44[0] = in_m;
        this->availableCodelets[0] = 1;
    } else {
        this->a_darts44[this->mainCodeletID] = in_a;
        this->b_darts44[this->mainCodeletID] = in_b;
        this->in_darts44[this->mainCodeletID] = in_in;
        this->out_darts44[this->mainCodeletID] = in_out;
        this->W_darts44[this->mainCodeletID] = in_W;
        this->nW_darts44[this->mainCodeletID] = in_nW;
        this->nWdn_darts44[this->mainCodeletID] = in_nWdn;
        this->m_darts44[this->mainCodeletID] = in_m;
        this->nextCodeletsfft_twiddle_32[in_mainCodeletID] = in_mainNextCodelet;
        this->nextSyncCodeletsfft_twiddle_32[in_mainCodeletID] = in_mainSyncCodelet;
#if USE_SPIN_CODELETS == 0
        this->firstCodelet[this->mainCodeletID].decDep();
#else
        this->availableCodelets[this->mainCodeletID] = 1;
#endif
        *(this->ptrToThisFunctionTP) = this;
    }
}
TP44::~TP44()
{
    delete[] task8650Inputs;
    delete[] task8652Inputs;
    delete[] barrierCodelets8654;
    delete[] checkInCodelets8652;
    delete[] checkInCodelets8650;
    delete[] checkInCodelets8647;
    delete[] checkInCodelets6506;
    delete[] checkInCodelets6503;
    delete[] nextCodeletsfft_twiddle_32;
    delete[] nextSyncCodeletsfft_twiddle_32;
    delete[] a_darts44;
    delete[] b_darts44;
    delete[] in_darts44;
    delete[] out_darts44;
    delete[] W_darts44;
    delete[] nW_darts44;
    delete[] nWdn_darts44;
    delete[] m_darts44;
    delete[] ab_darts44;
    delete[] i_darts44;
    delete[] i1_0_darts44;
    delete[] i1_1_darts44;
    delete[] i1_10_darts44;
    delete[] i1_11_darts44;
    delete[] i1_12_darts44;
    delete[] i1_13_darts44;
    delete[] i1_14_darts44;
    delete[] i1_15_darts44;
    delete[] i1_16_darts44;
    delete[] i1_17_darts44;
    delete[] i1_18_darts44;
    delete[] i1_19_darts44;
    delete[] i1_2_darts44;
    delete[] i1_20_darts44;
    delete[] i1_21_darts44;
    delete[] i1_22_darts44;
    delete[] i1_23_darts44;
    delete[] i1_24_darts44;
    delete[] i1_25_darts44;
    delete[] i1_26_darts44;
    delete[] i1_27_darts44;
    delete[] i1_28_darts44;
    delete[] i1_29_darts44;
    delete[] i1_3_darts44;
    delete[] i1_30_darts44;
    delete[] i1_31_darts44;
    delete[] i1_4_darts44;
    delete[] i1_5_darts44;
    delete[] i1_6_darts44;
    delete[] i1_7_darts44;
    delete[] i1_8_darts44;
    delete[] i1_9_darts44;
    delete[] i2_0_darts44;
    delete[] i2_1_darts44;
    delete[] i2_10_darts44;
    delete[] i2_11_darts44;
    delete[] i2_12_darts44;
    delete[] i2_13_darts44;
    delete[] i2_14_darts44;
    delete[] i2_15_darts44;
    delete[] i2_16_darts44;
    delete[] i2_17_darts44;
    delete[] i2_18_darts44;
    delete[] i2_19_darts44;
    delete[] i2_2_darts44;
    delete[] i2_20_darts44;
    delete[] i2_21_darts44;
    delete[] i2_22_darts44;
    delete[] i2_23_darts44;
    delete[] i2_24_darts44;
    delete[] i2_25_darts44;
    delete[] i2_26_darts44;
    delete[] i2_27_darts44;
    delete[] i2_28_darts44;
    delete[] i2_29_darts44;
    delete[] i2_3_darts44;
    delete[] i2_30_darts44;
    delete[] i2_31_darts44;
    delete[] i2_4_darts44;
    delete[] i2_5_darts44;
    delete[] i2_6_darts44;
    delete[] i2_7_darts44;
    delete[] i2_8_darts44;
    delete[] i2_9_darts44;
    delete[] i3_0_darts44;
    delete[] i3_1_darts44;
    delete[] i3_10_darts44;
    delete[] i3_11_darts44;
    delete[] i3_12_darts44;
    delete[] i3_13_darts44;
    delete[] i3_14_darts44;
    delete[] i3_15_darts44;
    delete[] i3_16_darts44;
    delete[] i3_17_darts44;
    delete[] i3_18_darts44;
    delete[] i3_19_darts44;
    delete[] i3_2_darts44;
    delete[] i3_20_darts44;
    delete[] i3_21_darts44;
    delete[] i3_22_darts44;
    delete[] i3_23_darts44;
    delete[] i3_24_darts44;
    delete[] i3_25_darts44;
    delete[] i3_26_darts44;
    delete[] i3_27_darts44;
    delete[] i3_28_darts44;
    delete[] i3_29_darts44;
    delete[] i3_3_darts44;
    delete[] i3_30_darts44;
    delete[] i3_31_darts44;
    delete[] i3_4_darts44;
    delete[] i3_5_darts44;
    delete[] i3_6_darts44;
    delete[] i3_7_darts44;
    delete[] i3_8_darts44;
    delete[] i3_9_darts44;
    delete[] i4_0_darts44;
    delete[] i4_1_darts44;
    delete[] i4_10_darts44;
    delete[] i4_11_darts44;
    delete[] i4_12_darts44;
    delete[] i4_13_darts44;
    delete[] i4_14_darts44;
    delete[] i4_15_darts44;
    delete[] i4_16_darts44;
    delete[] i4_17_darts44;
    delete[] i4_18_darts44;
    delete[] i4_19_darts44;
    delete[] i4_2_darts44;
    delete[] i4_20_darts44;
    delete[] i4_21_darts44;
    delete[] i4_22_darts44;
    delete[] i4_23_darts44;
    delete[] i4_24_darts44;
    delete[] i4_25_darts44;
    delete[] i4_26_darts44;
    delete[] i4_27_darts44;
    delete[] i4_28_darts44;
    delete[] i4_29_darts44;
    delete[] i4_3_darts44;
    delete[] i4_30_darts44;
    delete[] i4_31_darts44;
    delete[] i4_4_darts44;
    delete[] i4_5_darts44;
    delete[] i4_6_darts44;
    delete[] i4_7_darts44;
    delete[] i4_8_darts44;
    delete[] i4_9_darts44;
    delete[] i5_0_darts44;
    delete[] i5_1_darts44;
    delete[] i5_10_darts44;
    delete[] i5_11_darts44;
    delete[] i5_12_darts44;
    delete[] i5_13_darts44;
    delete[] i5_14_darts44;
    delete[] i5_15_darts44;
    delete[] i5_16_darts44;
    delete[] i5_17_darts44;
    delete[] i5_18_darts44;
    delete[] i5_19_darts44;
    delete[] i5_2_darts44;
    delete[] i5_20_darts44;
    delete[] i5_21_darts44;
    delete[] i5_22_darts44;
    delete[] i5_23_darts44;
    delete[] i5_24_darts44;
    delete[] i5_25_darts44;
    delete[] i5_26_darts44;
    delete[] i5_27_darts44;
    delete[] i5_28_darts44;
    delete[] i5_29_darts44;
    delete[] i5_3_darts44;
    delete[] i5_30_darts44;
    delete[] i5_31_darts44;
    delete[] i5_4_darts44;
    delete[] i5_5_darts44;
    delete[] i5_6_darts44;
    delete[] i5_7_darts44;
    delete[] i5_8_darts44;
    delete[] i5_9_darts44;
    delete[] jp_darts44;
    delete[] kp_darts44;
    delete[] l1_darts44;
    delete[] r1_0_darts44;
    delete[] r1_1_darts44;
    delete[] r1_10_darts44;
    delete[] r1_11_darts44;
    delete[] r1_12_darts44;
    delete[] r1_13_darts44;
    delete[] r1_14_darts44;
    delete[] r1_15_darts44;
    delete[] r1_16_darts44;
    delete[] r1_17_darts44;
    delete[] r1_18_darts44;
    delete[] r1_19_darts44;
    delete[] r1_2_darts44;
    delete[] r1_20_darts44;
    delete[] r1_21_darts44;
    delete[] r1_22_darts44;
    delete[] r1_23_darts44;
    delete[] r1_24_darts44;
    delete[] r1_25_darts44;
    delete[] r1_26_darts44;
    delete[] r1_27_darts44;
    delete[] r1_28_darts44;
    delete[] r1_29_darts44;
    delete[] r1_3_darts44;
    delete[] r1_30_darts44;
    delete[] r1_31_darts44;
    delete[] r1_4_darts44;
    delete[] r1_5_darts44;
    delete[] r1_6_darts44;
    delete[] r1_7_darts44;
    delete[] r1_8_darts44;
    delete[] r1_9_darts44;
    delete[] r2_0_darts44;
    delete[] r2_1_darts44;
    delete[] r2_10_darts44;
    delete[] r2_11_darts44;
    delete[] r2_12_darts44;
    delete[] r2_13_darts44;
    delete[] r2_14_darts44;
    delete[] r2_15_darts44;
    delete[] r2_16_darts44;
    delete[] r2_17_darts44;
    delete[] r2_18_darts44;
    delete[] r2_19_darts44;
    delete[] r2_2_darts44;
    delete[] r2_20_darts44;
    delete[] r2_21_darts44;
    delete[] r2_22_darts44;
    delete[] r2_23_darts44;
    delete[] r2_24_darts44;
    delete[] r2_25_darts44;
    delete[] r2_26_darts44;
    delete[] r2_27_darts44;
    delete[] r2_28_darts44;
    delete[] r2_29_darts44;
    delete[] r2_3_darts44;
    delete[] r2_30_darts44;
    delete[] r2_31_darts44;
    delete[] r2_4_darts44;
    delete[] r2_5_darts44;
    delete[] r2_6_darts44;
    delete[] r2_7_darts44;
    delete[] r2_8_darts44;
    delete[] r2_9_darts44;
    delete[] r3_0_darts44;
    delete[] r3_1_darts44;
    delete[] r3_10_darts44;
    delete[] r3_11_darts44;
    delete[] r3_12_darts44;
    delete[] r3_13_darts44;
    delete[] r3_14_darts44;
    delete[] r3_15_darts44;
    delete[] r3_16_darts44;
    delete[] r3_17_darts44;
    delete[] r3_18_darts44;
    delete[] r3_19_darts44;
    delete[] r3_2_darts44;
    delete[] r3_20_darts44;
    delete[] r3_21_darts44;
    delete[] r3_22_darts44;
    delete[] r3_23_darts44;
    delete[] r3_24_darts44;
    delete[] r3_25_darts44;
    delete[] r3_26_darts44;
    delete[] r3_27_darts44;
    delete[] r3_28_darts44;
    delete[] r3_29_darts44;
    delete[] r3_3_darts44;
    delete[] r3_30_darts44;
    delete[] r3_31_darts44;
    delete[] r3_4_darts44;
    delete[] r3_5_darts44;
    delete[] r3_6_darts44;
    delete[] r3_7_darts44;
    delete[] r3_8_darts44;
    delete[] r3_9_darts44;
    delete[] r4_0_darts44;
    delete[] r4_1_darts44;
    delete[] r4_10_darts44;
    delete[] r4_11_darts44;
    delete[] r4_12_darts44;
    delete[] r4_13_darts44;
    delete[] r4_14_darts44;
    delete[] r4_15_darts44;
    delete[] r4_16_darts44;
    delete[] r4_17_darts44;
    delete[] r4_18_darts44;
    delete[] r4_19_darts44;
    delete[] r4_2_darts44;
    delete[] r4_20_darts44;
    delete[] r4_21_darts44;
    delete[] r4_22_darts44;
    delete[] r4_23_darts44;
    delete[] r4_24_darts44;
    delete[] r4_25_darts44;
    delete[] r4_26_darts44;
    delete[] r4_27_darts44;
    delete[] r4_28_darts44;
    delete[] r4_29_darts44;
    delete[] r4_3_darts44;
    delete[] r4_30_darts44;
    delete[] r4_31_darts44;
    delete[] r4_4_darts44;
    delete[] r4_5_darts44;
    delete[] r4_6_darts44;
    delete[] r4_7_darts44;
    delete[] r4_8_darts44;
    delete[] r4_9_darts44;
    delete[] r5_0_darts44;
    delete[] r5_1_darts44;
    delete[] r5_10_darts44;
    delete[] r5_11_darts44;
    delete[] r5_12_darts44;
    delete[] r5_13_darts44;
    delete[] r5_14_darts44;
    delete[] r5_15_darts44;
    delete[] r5_16_darts44;
    delete[] r5_17_darts44;
    delete[] r5_18_darts44;
    delete[] r5_19_darts44;
    delete[] r5_2_darts44;
    delete[] r5_20_darts44;
    delete[] r5_21_darts44;
    delete[] r5_22_darts44;
    delete[] r5_23_darts44;
    delete[] r5_24_darts44;
    delete[] r5_25_darts44;
    delete[] r5_26_darts44;
    delete[] r5_27_darts44;
    delete[] r5_28_darts44;
    delete[] r5_29_darts44;
    delete[] r5_3_darts44;
    delete[] r5_30_darts44;
    delete[] r5_31_darts44;
    delete[] r5_4_darts44;
    delete[] r5_5_darts44;
    delete[] r5_6_darts44;
    delete[] r5_7_darts44;
    delete[] r5_8_darts44;
    delete[] r5_9_darts44;
    delete[] tmpi_darts44;
    delete[] tmpr_darts44;
    delete[] wi_darts44;
    delete[] wr_darts44;
}
void TP44::setNewInputs(int in_a, int in_b, COMPLEX* in_in, COMPLEX* in_out, COMPLEX* in_W,
    int in_nW, int in_nWdn, int in_m, size_t codeletID)
{
    this->a_darts44[codeletID] = in_a;
    this->b_darts44[codeletID] = in_b;
    this->in_darts44[codeletID] = in_in;
    this->out_darts44[codeletID] = in_out;
    this->W_darts44[codeletID] = in_W;
    this->nW_darts44[codeletID] = in_nW;
    this->nWdn_darts44[codeletID] = in_nWdn;
    this->m_darts44[codeletID] = in_m;
}
/*TP8650: OMPTaskDirective*/
void TP8650::_checkInCodelets8651::fire(void)
{
/*Init the vars for this region*/

/*printing node 8651: CallExpr*/
/*Finding the next codelet after the function call*/
#if 0
/*Signaling next codelet region: 8651 nextRegion: 11514 */
myTP->controlTPParent->checkInCodelets11514.decDep();
#endif
/*Finding the next sync point before making the function call*/
#if 0
/*Find the sync point for a function call*/
/*syncNode: OMPTaskwaitDirective 8654*/
/*No syncpoint found or outside task's boundaries, looking for syncCodelet*/
#endif
    /*Make the function call*/
    invoke<TP_fft_twiddle_32>(myTP, myTP->numThreads, this->getID(),
        &(myTP->controlTPParent->checkInCodelets11514),
        (myTP->controlTPParent->task8650Inputs->nextSyncCodelet), nullptr,
        (this->taskInputs->a_darts8650), (this->taskInputs->ab_darts8650),
        (this->taskInputs->in_darts8650), (this->taskInputs->out_darts8650),
        (this->taskInputs->W_darts8650), (this->taskInputs->nW_darts8650),
        (this->taskInputs->nWdn_darts8650), (this->taskInputs->m_darts8650));
}
void TP8650::_checkInCodelets11514::fire(void)
{

    /*printing node 11514: NullStmt*/
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task8650Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task8650Inputs->nextSyncCodelet->decDep();
}
TP8650::TP8650(
    int in_numThreads, int in_mainCodeletID, TP44* in_TPParent, _task8650Inputs* in_task8650Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task8650Inputs(in_task8650Inputs)
    , TP8651Ptr(nullptr)
    , TP8651_alreadyLaunched(0)
    , checkInCodelets8651(1, 1, this, this->mainCodeletID)
    , checkInCodelets11514(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets8651.decDep();
}
TP8650::~TP8650() { delete (task8650Inputs); }
/*TP8652: OMPTaskDirective*/
void TP8652::_checkInCodelets8653::fire(void)
{
/*Init the vars for this region*/

/*printing node 8653: CallExpr*/
/*Finding the next codelet after the function call*/
#if 0
/*Signaling next codelet region: 8653 nextRegion: 11515 */
myTP->controlTPParent->checkInCodelets11515.decDep();
#endif
/*Finding the next sync point before making the function call*/
#if 0
/*Find the sync point for a function call*/
/*syncNode: OMPTaskwaitDirective 8654*/
/*No syncpoint found or outside task's boundaries, looking for syncCodelet*/
#endif
    /*Make the function call*/
    invoke<TP_fft_twiddle_32>(myTP, myTP->numThreads, this->getID(),
        &(myTP->controlTPParent->checkInCodelets11515),
        (myTP->controlTPParent->task8652Inputs->nextSyncCodelet), nullptr,
        (this->taskInputs->ab_darts8652), (this->taskInputs->b_darts8652),
        (this->taskInputs->in_darts8652), (this->taskInputs->out_darts8652),
        (this->taskInputs->W_darts8652), (this->taskInputs->nW_darts8652),
        (this->taskInputs->nWdn_darts8652), (this->taskInputs->m_darts8652));
}
void TP8652::_checkInCodelets11515::fire(void)
{

    /*printing node 11515: NullStmt*/
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task8652Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task8652Inputs->nextSyncCodelet->decDep();
}
TP8652::TP8652(
    int in_numThreads, int in_mainCodeletID, TP44* in_TPParent, _task8652Inputs* in_task8652Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task8652Inputs(in_task8652Inputs)
    , TP8653Ptr(nullptr)
    , TP8653_alreadyLaunched(0)
    , checkInCodelets8653(1, 1, this, this->mainCodeletID)
    , checkInCodelets11515(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets8653.decDep();
}
TP8652::~TP8652() { delete (task8652Inputs); }
/*TP46: TP_fft_unshuffle_32*/
void TP46::_checkInCodelets10816::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    /*Init the vars for this region*/
    /*Printing conditional branch node 10816: inlining: 0*/
    if (((this->inputsTPParent->b_darts46[this->getID()])
            - (this->inputsTPParent->a_darts46[this->getID()]))
        < 128) {
        myTP->checkInCodelets10819[this->getID()].decDep();
    } else {
        /*elseRegion: DeclStmt 10972*/
        myTP->checkInCodelets10972[this->getID()].decDep();
    }
}
void TP46::_checkInCodelets10819::fire(void)
{

    /*printing node 10819: BinaryOperator*/
    (this->inputsTPParent->ip_darts46[this->getID()])
        = (this->inputsTPParent->in_darts46[this->getID()])
        + (this->inputsTPParent->a_darts46[this->getID()]) * 32;

    /*printing node 10822: ForStmt*/
    {
        int* b = &(this->inputsTPParent->b_darts46[this->getLocalID()]);
        (void)b /*PRIVATE*/;
        int* i = &(this->inputsTPParent->i_darts46[this->getLocalID()]);
        (void)i /*PRIVATE*/;
        COMPLEX** ip = &(this->inputsTPParent->ip_darts46[this->getLocalID()]);
        (void)ip /*PRIVATE*/;
        COMPLEX** jp = &(this->inputsTPParent->jp_darts46[this->getLocalID()]);
        (void)jp /*PRIVATE*/;
        int* m = &(this->inputsTPParent->m_darts46[this->getLocalID()]);
        (void)m /*PRIVATE*/;
        COMPLEX** out = &(this->inputsTPParent->out_darts46[this->getLocalID()]);
        (void)out /*PRIVATE*/;
        /*Loop's init*/
        (this->inputsTPParent->i_darts46[this->getID()])
            = (this->inputsTPParent->a_darts46[this->getID()]);
        int i_darts_counter_temp46 = (this->inputsTPParent->i_darts46[this->getID()]);
        for (; (i_darts_counter_temp46) < (*b); ++(i_darts_counter_temp46)) {
            (*jp) = (*out) + (i_darts_counter_temp46);
            (*jp)[0] = (*ip)[0];
            (*jp)[(*m)] = (*ip)[1];
            (*ip) += 2;
            (*jp) += 2 * (*m);
            (*jp)[0] = (*ip)[0];
            (*jp)[(*m)] = (*ip)[1];
            (*ip) += 2;
            (*jp) += 2 * (*m);
            (*jp)[0] = (*ip)[0];
            (*jp)[(*m)] = (*ip)[1];
            (*ip) += 2;
            (*jp) += 2 * (*m);
            (*jp)[0] = (*ip)[0];
            (*jp)[(*m)] = (*ip)[1];
            (*ip) += 2;
            (*jp) += 2 * (*m);
            (*jp)[0] = (*ip)[0];
            (*jp)[(*m)] = (*ip)[1];
            (*ip) += 2;
            (*jp) += 2 * (*m);
            (*jp)[0] = (*ip)[0];
            (*jp)[(*m)] = (*ip)[1];
            (*ip) += 2;
            (*jp) += 2 * (*m);
            (*jp)[0] = (*ip)[0];
            (*jp)[(*m)] = (*ip)[1];
            (*ip) += 2;
            (*jp) += 2 * (*m);
            (*jp)[0] = (*ip)[0];
            (*jp)[(*m)] = (*ip)[1];
            (*ip) += 2;
            (*jp) += 2 * (*m);
            (*jp)[0] = (*ip)[0];
            (*jp)[(*m)] = (*ip)[1];
            (*ip) += 2;
            (*jp) += 2 * (*m);
            (*jp)[0] = (*ip)[0];
            (*jp)[(*m)] = (*ip)[1];
            (*ip) += 2;
            (*jp) += 2 * (*m);
            (*jp)[0] = (*ip)[0];
            (*jp)[(*m)] = (*ip)[1];
            (*ip) += 2;
            (*jp) += 2 * (*m);
            (*jp)[0] = (*ip)[0];
            (*jp)[(*m)] = (*ip)[1];
            (*ip) += 2;
            (*jp) += 2 * (*m);
            (*jp)[0] = (*ip)[0];
            (*jp)[(*m)] = (*ip)[1];
            (*ip) += 2;
            (*jp) += 2 * (*m);
            (*jp)[0] = (*ip)[0];
            (*jp)[(*m)] = (*ip)[1];
            (*ip) += 2;
            (*jp) += 2 * (*m);
            (*jp)[0] = (*ip)[0];
            (*jp)[(*m)] = (*ip)[1];
            (*ip) += 2;
            (*jp) += 2 * (*m);
            (*jp)[0] = (*ip)[0];
            (*jp)[(*m)] = (*ip)[1];
            (*ip) += 2;
        }
        (this->inputsTPParent->i_darts46[this->getID()]) = i_darts_counter_temp46;
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/

    myTP->controlTPParent->nextCodeletsfft_unshuffle_32[this->getID()]->decDep();
}
void TP46::_checkInCodelets10972::fire(void)
{

    /*printing node 10972: DeclStmt*/
    this->inputsTPParent->ab_darts46[this->getID()]
        = ((this->inputsTPParent->a_darts46[this->getID()])
              + (this->inputsTPParent->b_darts46[this->getID()]))
        / 2;
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 10972 nextRegion: 10975 */
    myTP->controlTPParent->checkInCodelets10975[this->getID()].decDep();
}
void TP46::_checkInCodelets10975::fire(void)
{

    /*printing node 10975: OMPTaskDirective*/
    /*syncNode: OMPTaskwaitDirective 10979*/
    /*Find the TP to which the nextRegion belongs*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->barrierCodelets10979[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 10975*/
    _task10975Inputs* task10975Inputs
        = new _task10975Inputs(&((this->inputsTPParent->a_darts46[this->getID()])),
            &((this->inputsTPParent->ab_darts46[this->getID()])),
            &((this->inputsTPParent->in_darts46[this->getID()])),
            &((this->inputsTPParent->m_darts46[this->getID()])),
            &((this->inputsTPParent->out_darts46[this->getID()])), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task10975Inputs[this->getID()] = task10975Inputs;
    invoke<TP10975>(myTP, 1, this->getID(), myTP, task10975Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 10975 nextRegion: 10977 */
    myTP->controlTPParent->checkInCodelets10977[this->getID()].decDep();
}
void TP46::_checkInCodelets10977::fire(void)
{

    /*printing node 10977: OMPTaskDirective*/
    /*syncNode: OMPTaskwaitDirective 10979*/
    /*Find the TP to which the nextRegion belongs*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->barrierCodelets10979[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 10977*/
    _task10977Inputs* task10977Inputs
        = new _task10977Inputs(&((this->inputsTPParent->ab_darts46[this->getID()])),
            &((this->inputsTPParent->b_darts46[this->getID()])),
            &((this->inputsTPParent->in_darts46[this->getID()])),
            &((this->inputsTPParent->m_darts46[this->getID()])),
            &((this->inputsTPParent->out_darts46[this->getID()])), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task10977Inputs[this->getID()] = task10977Inputs;
    invoke<TP10977>(myTP, 1, this->getID(), myTP, task10977Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 10977 nextRegion: 10979 */
    myTP->controlTPParent->barrierCodelets10979[0].decDep();
}
void TP46::_barrierCodelets10979::fire(void)
{
    TP46* myTP = static_cast<TP46*>(myTP_);

    for (size_t codeletsCounter = 0; codeletsCounter < (size_t)myTP->numThreads;
         codeletsCounter++) {
        myTP->nextCodeletsfft_unshuffle_32[codeletsCounter]->decDep();
    }
}
TP46::TP46(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_mainNextCodelet,
    darts::Codelet* in_mainSyncCodelet, TP46** in_ptrToThisFunctionTP, int in_a, int in_b,
    COMPLEX* in_in, COMPLEX* in_out, int in_m)
    : ompTP(in_numThreads, in_mainCodeletID)
    , ptrToThisFunctionTP(in_ptrToThisFunctionTP)
    , inputsTPParent(this)
    , controlTPParent(this)
    , nextCodeletsfft_unshuffle_32(new Codelet*[in_numThreads])
    , nextSyncCodeletsfft_unshuffle_32(new Codelet*[in_numThreads])
    , a_darts46(new int[this->numThreads])
    , b_darts46(new int[this->numThreads])
    , in_darts46(new COMPLEX*[this->numThreads])
    , out_darts46(new COMPLEX*[this->numThreads])
    , m_darts46(new int[this->numThreads])
    , ab_darts46(new int[this->numThreads])
    , i_darts46(new int[this->numThreads])
    , ip_darts46(new COMPLEX*[this->numThreads])
    , jp_darts46(new COMPLEX*[this->numThreads])
    , task10975Inputs(new _task10975Inputs*[this->numThreads])
    , task10977Inputs(new _task10977Inputs*[this->numThreads])
    , checkInCodelets10816(new _checkInCodelets10816[this->numThreads])
    , checkInCodelets10819(new _checkInCodelets10819[this->numThreads])
    , checkInCodelets10972(new _checkInCodelets10972[this->numThreads])
    , checkInCodelets10975(new _checkInCodelets10975[this->numThreads])
    , checkInCodelets10977(new _checkInCodelets10977[this->numThreads])
    , barrierCodelets10979(new _barrierCodelets10979[1])
{
    barrierCodelets10979[0] = _barrierCodelets10979(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets10977* checkInCodelets10977Ptr = (this->checkInCodelets10977);
    _checkInCodelets10975* checkInCodelets10975Ptr = (this->checkInCodelets10975);
    _checkInCodelets10972* checkInCodelets10972Ptr = (this->checkInCodelets10972);
    _checkInCodelets10819* checkInCodelets10819Ptr = (this->checkInCodelets10819);
    _checkInCodelets10816* checkInCodelets10816Ptr = (this->checkInCodelets10816);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets10816);
#endif
    for (size_t codeletCounter = 0; codeletCounter < this->numThreads; codeletCounter++) {
        (*checkInCodelets10977Ptr) = _checkInCodelets10977(1, 1, this, codeletCounter);
        checkInCodelets10977Ptr++;
        (*checkInCodelets10975Ptr) = _checkInCodelets10975(1, 1, this, codeletCounter);
        checkInCodelets10975Ptr++;
        (*checkInCodelets10972Ptr) = _checkInCodelets10972(1, 1, this, codeletCounter);
        checkInCodelets10972Ptr++;
        (*checkInCodelets10819Ptr) = _checkInCodelets10819(1, 1, this, codeletCounter);
        checkInCodelets10819Ptr++;
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets10816Ptr) = _checkInCodelets10816(2, 1, this, codeletCounter);
#else
        (*checkInCodelets10816Ptr) = _checkInCodelets10816(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets10816Ptr).decDep();
        checkInCodelets10816Ptr++;
    }
    if (this->numThreads == 1) {
        this->nextCodeletsfft_unshuffle_32[0] = in_mainNextCodelet;
        this->nextSyncCodeletsfft_unshuffle_32[0] = in_mainSyncCodelet;
        this->a_darts46[0] = in_a;
        this->b_darts46[0] = in_b;
        this->in_darts46[0] = in_in;
        this->out_darts46[0] = in_out;
        this->m_darts46[0] = in_m;
        this->availableCodelets[0] = 1;
    } else {
        this->a_darts46[this->mainCodeletID] = in_a;
        this->b_darts46[this->mainCodeletID] = in_b;
        this->in_darts46[this->mainCodeletID] = in_in;
        this->out_darts46[this->mainCodeletID] = in_out;
        this->m_darts46[this->mainCodeletID] = in_m;
        this->nextCodeletsfft_unshuffle_32[in_mainCodeletID] = in_mainNextCodelet;
        this->nextSyncCodeletsfft_unshuffle_32[in_mainCodeletID] = in_mainSyncCodelet;
#if USE_SPIN_CODELETS == 0
        this->firstCodelet[this->mainCodeletID].decDep();
#else
        this->availableCodelets[this->mainCodeletID] = 1;
#endif
        *(this->ptrToThisFunctionTP) = this;
    }
}
TP46::~TP46()
{
    delete[] task10975Inputs;
    delete[] task10977Inputs;
    delete[] barrierCodelets10979;
    delete[] checkInCodelets10977;
    delete[] checkInCodelets10975;
    delete[] checkInCodelets10972;
    delete[] checkInCodelets10819;
    delete[] checkInCodelets10816;
    delete[] nextCodeletsfft_unshuffle_32;
    delete[] nextSyncCodeletsfft_unshuffle_32;
    delete[] a_darts46;
    delete[] b_darts46;
    delete[] in_darts46;
    delete[] out_darts46;
    delete[] m_darts46;
    delete[] ab_darts46;
    delete[] i_darts46;
    delete[] ip_darts46;
    delete[] jp_darts46;
}
void TP46::setNewInputs(
    int in_a, int in_b, COMPLEX* in_in, COMPLEX* in_out, int in_m, size_t codeletID)
{
    this->a_darts46[codeletID] = in_a;
    this->b_darts46[codeletID] = in_b;
    this->in_darts46[codeletID] = in_in;
    this->out_darts46[codeletID] = in_out;
    this->m_darts46[codeletID] = in_m;
}
/*TP10975: OMPTaskDirective*/
void TP10975::_checkInCodelets10976::fire(void)
{
/*Init the vars for this region*/

/*printing node 10976: CallExpr*/
/*Finding the next codelet after the function call*/
#if 0
/*Signaling next codelet region: 10976 nextRegion: 11510 */
myTP->controlTPParent->checkInCodelets11510.decDep();
#endif
/*Finding the next sync point before making the function call*/
#if 0
/*Find the sync point for a function call*/
/*syncNode: OMPTaskwaitDirective 10979*/
/*No syncpoint found or outside task's boundaries, looking for syncCodelet*/
#endif
    /*Make the function call*/
    invoke<TP_fft_unshuffle_32>(myTP, myTP->numThreads, this->getID(),
        &(myTP->controlTPParent->checkInCodelets11510),
        (myTP->controlTPParent->task10975Inputs->nextSyncCodelet), nullptr,
        (this->taskInputs->a_darts10975), (this->taskInputs->ab_darts10975),
        (this->taskInputs->in_darts10975), (this->taskInputs->out_darts10975),
        (this->taskInputs->m_darts10975));
}
void TP10975::_checkInCodelets11510::fire(void)
{

    /*printing node 11510: NullStmt*/
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task10975Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task10975Inputs->nextSyncCodelet->decDep();
}
TP10975::TP10975(int in_numThreads, int in_mainCodeletID, TP46* in_TPParent,
    _task10975Inputs* in_task10975Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task10975Inputs(in_task10975Inputs)
    , TP10976Ptr(nullptr)
    , TP10976_alreadyLaunched(0)
    , checkInCodelets10976(1, 1, this, this->mainCodeletID)
    , checkInCodelets11510(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets10976.decDep();
}
TP10975::~TP10975() { delete (task10975Inputs); }
/*TP10977: OMPTaskDirective*/
void TP10977::_checkInCodelets10978::fire(void)
{
/*Init the vars for this region*/

/*printing node 10978: CallExpr*/
/*Finding the next codelet after the function call*/
#if 0
/*Signaling next codelet region: 10978 nextRegion: 11511 */
myTP->controlTPParent->checkInCodelets11511.decDep();
#endif
/*Finding the next sync point before making the function call*/
#if 0
/*Find the sync point for a function call*/
/*syncNode: OMPTaskwaitDirective 10979*/
/*No syncpoint found or outside task's boundaries, looking for syncCodelet*/
#endif
    /*Make the function call*/
    invoke<TP_fft_unshuffle_32>(myTP, myTP->numThreads, this->getID(),
        &(myTP->controlTPParent->checkInCodelets11511),
        (myTP->controlTPParent->task10977Inputs->nextSyncCodelet), nullptr,
        (this->taskInputs->ab_darts10977), (this->taskInputs->b_darts10977),
        (this->taskInputs->in_darts10977), (this->taskInputs->out_darts10977),
        (this->taskInputs->m_darts10977));
}
void TP10977::_checkInCodelets11511::fire(void)
{

    /*printing node 11511: NullStmt*/
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task10977Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task10977Inputs->nextSyncCodelet->decDep();
}
TP10977::TP10977(int in_numThreads, int in_mainCodeletID, TP46* in_TPParent,
    _task10977Inputs* in_task10977Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task10977Inputs(in_task10977Inputs)
    , TP10978Ptr(nullptr)
    , TP10978_alreadyLaunched(0)
    , checkInCodelets10978(1, 1, this, this->mainCodeletID)
    , checkInCodelets11511(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets10978.decDep();
}
TP10977::~TP10977() { delete (task10977Inputs); }
/*TP48: TP_fft_aux*/
void TP48::_checkInCodelets11146::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    /*Init the vars for this region*/

    /*printing node 11146: DeclStmt*/

    /*printing node 11147: DeclStmt*/

    /*printing node 11148: IfStmt*/
    if ((this->inputsTPParent->n_darts48[this->getID()]) == 32) {
        fft_base_32((this->inputsTPParent->in_darts48[this->getID()]),
            (this->inputsTPParent->out_darts48[this->getID()]));
        /*return*/
        myTP->controlTPParent->nextCodeletsfft_aux[this->getID()]->decDep();
        return;
    }

    /*printing node 11153: IfStmt*/
    if ((this->inputsTPParent->n_darts48[this->getID()]) == 16) {
        fft_base_16((this->inputsTPParent->in_darts48[this->getID()]),
            (this->inputsTPParent->out_darts48[this->getID()]));
        /*return*/
        myTP->controlTPParent->nextCodeletsfft_aux[this->getID()]->decDep();
        return;
    }

    /*printing node 11158: IfStmt*/
    if ((this->inputsTPParent->n_darts48[this->getID()]) == 8) {
        fft_base_8((this->inputsTPParent->in_darts48[this->getID()]),
            (this->inputsTPParent->out_darts48[this->getID()]));
        /*return*/
        myTP->controlTPParent->nextCodeletsfft_aux[this->getID()]->decDep();
        return;
    }

    /*printing node 11163: IfStmt*/
    if ((this->inputsTPParent->n_darts48[this->getID()]) == 4) {
        fft_base_4((this->inputsTPParent->in_darts48[this->getID()]),
            (this->inputsTPParent->out_darts48[this->getID()]));
        /*return*/
        myTP->controlTPParent->nextCodeletsfft_aux[this->getID()]->decDep();
        return;
    }

    /*printing node 11168: IfStmt*/
    if ((this->inputsTPParent->n_darts48[this->getID()]) == 2) {
        fft_base_2((this->inputsTPParent->in_darts48[this->getID()]),
            (this->inputsTPParent->out_darts48[this->getID()]));
        /*return*/
        myTP->controlTPParent->nextCodeletsfft_aux[this->getID()]->decDep();
        return;
    }

    /*printing node 11173: BinaryOperator*/
    (this->inputsTPParent->r_darts48[this->getID()])
        = *(this->inputsTPParent->factors_darts48[this->getID()]);

    /*printing node 11175: BinaryOperator*/
    (this->inputsTPParent->m_darts48[this->getID()])
        = (this->inputsTPParent->n_darts48[this->getID()])
        / (this->inputsTPParent->r_darts48[this->getID()]);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 11146 nextRegion: 11177 */
    myTP->controlTPParent->checkInCodelets11177[this->getID()].decDep();
}
void TP48::_checkInCodelets11177::fire(void)
{
    /*Printing conditional branch node 11177: inlining: 0*/
    if ((this->inputsTPParent->r_darts48[this->getID()])
        < (this->inputsTPParent->n_darts48[this->getID()])) {
        myTP->checkInCodelets11180[this->getID()].decDep();
    } else {
        /*Signaling the region after the if stmt*/
        /*Signaling next codelet region: 11177 nextRegion: 11223 */
        myTP->controlTPParent->checkInCodelets11223[this->getID()].decDep();
    }
}
void TP48::_checkInCodelets11223::fire(void)
{
    /*Printing conditional branch node 11223: inlining: 0*/
    if ((this->inputsTPParent->r_darts48[this->getID()]) == 2) {
        myTP->checkInCodelets11226[this->getID()].decDep();
    } else if ((this->inputsTPParent->r_darts48[this->getID()]) == 4) {
        myTP->checkInCodelets11232[this->getID()].decDep();
    } else if ((this->inputsTPParent->r_darts48[this->getID()]) == 8) {
        myTP->checkInCodelets11238[this->getID()].decDep();
    } else if ((this->inputsTPParent->r_darts48[this->getID()]) == 16) {
        myTP->checkInCodelets11244[this->getID()].decDep();
    } else if ((this->inputsTPParent->r_darts48[this->getID()]) == 32) {
        myTP->checkInCodelets11250[this->getID()].decDep();
    } else {
        /*elseRegion: OMPTaskDirective 11254*/
        myTP->checkInCodelets11254[this->getID()].decDep();
    }
}
void TP48::_barrierCodelets11261::fire(void)
{
    TP48* myTP = static_cast<TP48*>(myTP_);
    {
        for (size_t codeletsCounter = 0; codeletsCounter < myTP->numThreads; codeletsCounter++) {
            myTP->checkInCodelets11262[codeletsCounter].decDep();
        }
    }
}
void TP48::_checkInCodelets11262::fire(void)
{

    /*printing node 11262: ReturnStmt*/
    /*return*/
    myTP->controlTPParent->nextCodeletsfft_aux[this->getID()]->decDep();
    return;
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/

    myTP->controlTPParent->nextCodeletsfft_aux[this->getID()]->decDep();
}
void TP48::_checkInCodelets11180::fire(void)
{
    /*Printing conditional branch node 11180: inlining: 0*/
    if ((this->inputsTPParent->r_darts48[this->getID()]) == 32) {
        myTP->checkInCodelets11183[this->getID()].decDep();
    } else if ((this->inputsTPParent->r_darts48[this->getID()]) == 16) {
        myTP->checkInCodelets11188[this->getID()].decDep();
    } else if ((this->inputsTPParent->r_darts48[this->getID()]) == 8) {
        myTP->checkInCodelets11193[this->getID()].decDep();
    } else if ((this->inputsTPParent->r_darts48[this->getID()]) == 4) {
        myTP->checkInCodelets11198[this->getID()].decDep();
    } else if ((this->inputsTPParent->r_darts48[this->getID()]) == 2) {
        myTP->checkInCodelets11203[this->getID()].decDep();
    } else {
        /*elseRegion: CallExpr 11205*/
        myTP->checkInCodelets11205[this->getID()].decDep();
    }
}
void TP48::_barrierCodelets11211::fire(void)
{
    TP48* myTP = static_cast<TP48*>(myTP_);
    {
        for (size_t codeletsCounter = 0; codeletsCounter < myTP->numThreads; codeletsCounter++) {
            myTP->checkInCodelets11213[codeletsCounter].decDep();
        }
    }
}
void TP48::_checkInCodelets11213::fire(void)
{

    /*printing node 11213: BinaryOperator*/
    (this->inputsTPParent->k_darts48[this->getID()]) = 0;

    /*printing node 11214: BinaryOperator*/
    /*Print the code for a condition node in a complex loop stmt */
    if ((this->inputsTPParent->k_darts48[this->getID()])
        < (this->inputsTPParent->n_darts48[this->getID()])) {
        /*Signal the first codelet in the loop*/
        myTP->checkInCodelets11212[this->getID()].decDep();
        return;
    } else {
        /*Signal the codelet after the loop from the end condional node.*/
        /*Signaling next codelet region: 11215 nextRegion: 11222 */
        myTP->controlTPParent->barrierCodelets11222[0].decDep();
        return;
    }
}
void TP48::_checkInCodelets11212::fire(void)
{

    /*printing node 11212: ForStmt*/
    bool haveToLaunch = __sync_bool_compare_and_swap(&(myTP->controlTPParent->TP11212_LoopCounter),
        myTP->controlTPParent->TP11212_LoopCounterPerThread[this->getID()],
        myTP->controlTPParent->TP11212_LoopCounterPerThread[this->getID()] + 1);
    unsigned int iterIdx = myTP->controlTPParent->TP11212_LoopCounterPerThread[this->getID()];
    if (haveToLaunch) {
        this->resetCodelet();
        myTP->controlTPParent->TP11212PtrVec.push_back(nullptr);
        myTP->controlTPParent->TP11212_LoopCounterPerThread[this->getID()] += 1;
        invoke<TP11212>(myTP, myTP->numThreads, this->getID(), myTP, myTP->inputsTPParent,
            &(myTP->controlTPParent->TP11212PtrVec.back()));
    } else {
        if (myTP->controlTPParent->TP11212PtrVec.size() == 0) {
            this->resetCodelet();
            this->decDep();
            return;
        } else if (myTP->controlTPParent->TP11212PtrVec.size() < (iterIdx + 1)) {
            this->resetCodelet();
            this->decDep();
            return;
        } else if (myTP->controlTPParent->TP11212PtrVec[iterIdx] == nullptr) {
            this->resetCodelet();
            this->decDep();
            return;
        } else {
            this->resetCodelet();
#if USE_SPIN_CODELETS == 0
            myTP->controlTPParent->TP11212PtrVec[iterIdx]->firstCodelet[this->getID()].decDep();
#else
            myTP->controlTPParent->TP11212PtrVec[iterIdx]->availableCodelets[this->getID()] = 1;
#endif
            myTP->controlTPParent->TP11212_LoopCounterPerThread[this->getID()] += 1;
        }
    }
}
void TP48::_checkInCodelets11215::fire(void)
{

    /*printing node 11215: CompoundAssignOperator*/
    (this->inputsTPParent->k_darts48[this->getID()])
        += (this->inputsTPParent->m_darts48[this->getID()]);

    /*printing node 11530: BinaryOperator*/
    /*Print the code for a condition node in a complex loop stmt */
    if ((this->inputsTPParent->k_darts48[this->getID()])
        < (this->inputsTPParent->n_darts48[this->getID()])) {
        this->resetCodelet();
        /*Signal the first codelet in the loop*/
        myTP->checkInCodelets11212[this->getID()].decDep();
        return;
    } else {
        /*Signal the codelet after the loop from the condtional node.*/
        /*Signaling next codelet region: 11215 nextRegion: 11222 */
        myTP->controlTPParent->barrierCodelets11222[0].decDep();
        return;
    }
}
void TP48::_barrierCodelets11222::fire(void)
{
    TP48* myTP = static_cast<TP48*>(myTP_);
    {
        for (size_t codeletsCounter = 0; codeletsCounter < myTP->numThreads; codeletsCounter++) {
            myTP->checkInCodelets11223[codeletsCounter].decDep();
        }
    }
}
void TP48::_checkInCodelets11183::fire(void)
{

    /*printing node 11183: OMPTaskDirective*/
    /*syncNode: OMPTaskwaitDirective 11211*/
    /*Find the TP to which the nextRegion belongs*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->barrierCodelets11211[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 11183*/
    _task11183Inputs* task11183Inputs
        = new _task11183Inputs(&((this->inputsTPParent->in_darts48[this->getID()])),
            &((this->inputsTPParent->m_darts48[this->getID()])),
            &((this->inputsTPParent->out_darts48[this->getID()])), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task11183Inputs[this->getID()] = task11183Inputs;
    invoke<TP11183>(myTP, 1, this->getID(), myTP, task11183Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 11183 nextRegion: 11211 */
    myTP->controlTPParent->barrierCodelets11211[0].decDep();
}
void TP48::_checkInCodelets11185::fire(void)
{
    /*Printing conditional branch node 11185: inlining: 0*/
    if ((this->inputsTPParent->r_darts48[this->getID()]) == 16) {
        myTP->checkInCodelets11188[this->getID()].decDep();
    } else if ((this->inputsTPParent->r_darts48[this->getID()]) == 8) {
        myTP->checkInCodelets11193[this->getID()].decDep();
    } else if ((this->inputsTPParent->r_darts48[this->getID()]) == 4) {
        myTP->checkInCodelets11198[this->getID()].decDep();
    } else if ((this->inputsTPParent->r_darts48[this->getID()]) == 2) {
        myTP->checkInCodelets11203[this->getID()].decDep();
    } else {
        /*elseRegion: CallExpr 11205*/
        myTP->checkInCodelets11205[this->getID()].decDep();
    }
}
void TP48::_checkInCodelets11188::fire(void)
{

    /*printing node 11188: OMPTaskDirective*/
    /*syncNode: OMPTaskwaitDirective 11211*/
    /*Find the TP to which the nextRegion belongs*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->barrierCodelets11211[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 11188*/
    _task11188Inputs* task11188Inputs
        = new _task11188Inputs(&((this->inputsTPParent->in_darts48[this->getID()])),
            &((this->inputsTPParent->m_darts48[this->getID()])),
            &((this->inputsTPParent->out_darts48[this->getID()])), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task11188Inputs[this->getID()] = task11188Inputs;
    invoke<TP11188>(myTP, 1, this->getID(), myTP, task11188Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 11188 nextRegion: 11211 */
    myTP->controlTPParent->barrierCodelets11211[0].decDep();
}
void TP48::_checkInCodelets11190::fire(void)
{
    /*Printing conditional branch node 11190: inlining: 0*/
    if ((this->inputsTPParent->r_darts48[this->getID()]) == 8) {
        myTP->checkInCodelets11193[this->getID()].decDep();
    } else if ((this->inputsTPParent->r_darts48[this->getID()]) == 4) {
        myTP->checkInCodelets11198[this->getID()].decDep();
    } else if ((this->inputsTPParent->r_darts48[this->getID()]) == 2) {
        myTP->checkInCodelets11203[this->getID()].decDep();
    } else {
        /*elseRegion: CallExpr 11205*/
        myTP->checkInCodelets11205[this->getID()].decDep();
    }
}
void TP48::_checkInCodelets11193::fire(void)
{

    /*printing node 11193: OMPTaskDirective*/
    /*syncNode: OMPTaskwaitDirective 11211*/
    /*Find the TP to which the nextRegion belongs*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->barrierCodelets11211[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 11193*/
    _task11193Inputs* task11193Inputs
        = new _task11193Inputs(&((this->inputsTPParent->in_darts48[this->getID()])),
            &((this->inputsTPParent->m_darts48[this->getID()])),
            &((this->inputsTPParent->out_darts48[this->getID()])), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task11193Inputs[this->getID()] = task11193Inputs;
    invoke<TP11193>(myTP, 1, this->getID(), myTP, task11193Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 11193 nextRegion: 11211 */
    myTP->controlTPParent->barrierCodelets11211[0].decDep();
}
void TP48::_checkInCodelets11195::fire(void)
{
    /*Printing conditional branch node 11195: inlining: 0*/
    if ((this->inputsTPParent->r_darts48[this->getID()]) == 4) {
        myTP->checkInCodelets11198[this->getID()].decDep();
    } else if ((this->inputsTPParent->r_darts48[this->getID()]) == 2) {
        myTP->checkInCodelets11203[this->getID()].decDep();
    } else {
        /*elseRegion: CallExpr 11205*/
        myTP->checkInCodelets11205[this->getID()].decDep();
    }
}
void TP48::_checkInCodelets11198::fire(void)
{

    /*printing node 11198: OMPTaskDirective*/
    /*syncNode: OMPTaskwaitDirective 11211*/
    /*Find the TP to which the nextRegion belongs*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->barrierCodelets11211[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 11198*/
    _task11198Inputs* task11198Inputs
        = new _task11198Inputs(&((this->inputsTPParent->in_darts48[this->getID()])),
            &((this->inputsTPParent->m_darts48[this->getID()])),
            &((this->inputsTPParent->out_darts48[this->getID()])), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task11198Inputs[this->getID()] = task11198Inputs;
    invoke<TP11198>(myTP, 1, this->getID(), myTP, task11198Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 11198 nextRegion: 11211 */
    myTP->controlTPParent->barrierCodelets11211[0].decDep();
}
void TP48::_checkInCodelets11200::fire(void)
{
    /*Printing conditional branch node 11200: inlining: 0*/
    if ((this->inputsTPParent->r_darts48[this->getID()]) == 2) {
        myTP->checkInCodelets11203[this->getID()].decDep();
    } else {
        /*elseRegion: CallExpr 11205*/
        myTP->checkInCodelets11205[this->getID()].decDep();
    }
}
void TP48::_checkInCodelets11203::fire(void)
{

    /*printing node 11203: OMPTaskDirective*/
    /*syncNode: OMPTaskwaitDirective 11211*/
    /*Find the TP to which the nextRegion belongs*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->barrierCodelets11211[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 11203*/
    _task11203Inputs* task11203Inputs
        = new _task11203Inputs(&((this->inputsTPParent->in_darts48[this->getID()])),
            &((this->inputsTPParent->m_darts48[this->getID()])),
            &((this->inputsTPParent->out_darts48[this->getID()])), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task11203Inputs[this->getID()] = task11203Inputs;
    invoke<TP11203>(myTP, 1, this->getID(), myTP, task11203Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 11203 nextRegion: 11211 */
    myTP->controlTPParent->barrierCodelets11211[0].decDep();
}
void TP48::_checkInCodelets11205::fire(void)
{

    /*printing node 11205: CallExpr*/
    if (!__sync_val_compare_and_swap(&(myTP->controlTPParent->TP11205_alreadyLaunched), 0, 1)) {
/*Finding the next codelet after the function call*/
#if 0
/*Signaling next codelet region: 11205 nextRegion: 11211 */
myTP->controlTPParent->barrierCodelets11211[0].decDep();
#endif
/*Finding the next sync point before making the function call*/
#if 0
/*Find the sync point for a function call*/
/*syncNode: OMPTaskwaitDirective 11211*/
/*Find the TP to which the nextRegion belongs*/
#endif
        /*Make the function call*/
        invoke<TP_unshuffle>(myTP, myTP->numThreads, this->getID(),
            &(myTP->controlTPParent->barrierCodelets11211[0]),
            &(myTP->controlTPParent->barrierCodelets11211[0]), &(myTP->controlTPParent->TP11205Ptr),
            0, (this->inputsTPParent->m_darts48[this->getID()]),
            (this->inputsTPParent->in_darts48[this->getID()]),
            (this->inputsTPParent->out_darts48[this->getID()]),
            (this->inputsTPParent->r_darts48[this->getID()]),
            (this->inputsTPParent->m_darts48[this->getID()]));
    } else {
        if (myTP->controlTPParent->TP11205Ptr == nullptr) {
            myTP->add(this);
            return;
        } else {
            myTP->controlTPParent->TP11205Ptr->setNewInputs(0,
                (this->inputsTPParent->m_darts48[this->getID()]),
                (this->inputsTPParent->in_darts48[this->getID()]),
                (this->inputsTPParent->out_darts48[this->getID()]),
                (this->inputsTPParent->r_darts48[this->getID()]),
                (this->inputsTPParent->m_darts48[this->getID()]), this->getID());
            myTP->controlTPParent->TP11205Ptr->nextCodeletsunshuffle[this->getID()]
                = &(myTP->controlTPParent->barrierCodelets11211[0]);
            myTP->controlTPParent->TP11205Ptr->nextSyncCodeletsunshuffle[this->getID()]
                = &(myTP->controlTPParent->barrierCodelets11211[0]);
#if USE_SPIN_CODELETS == 0
            myTP->controlTPParent->TP11205Ptr->firstCodelet[this->getID()].decDep();
#else
            myTP->controlTPParent->TP11205Ptr->availableCodelets[this->getID()] = 1;
#endif
        }
    }
}
void TP48::_checkInCodelets11226::fire(void)
{

    /*printing node 11226: OMPTaskDirective*/
    /*syncNode: OMPTaskwaitDirective 11261*/
    /*Find the TP to which the nextRegion belongs*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->barrierCodelets11261[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 11226*/
    _task11226Inputs* task11226Inputs
        = new _task11226Inputs(&((this->inputsTPParent->W_darts48[this->getID()])),
            &((this->inputsTPParent->in_darts48[this->getID()])),
            &((this->inputsTPParent->m_darts48[this->getID()])),
            &((this->inputsTPParent->n_darts48[this->getID()])),
            &((this->inputsTPParent->nW_darts48[this->getID()])),
            &((this->inputsTPParent->out_darts48[this->getID()])), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task11226Inputs[this->getID()] = task11226Inputs;
    invoke<TP11226>(myTP, 1, this->getID(), myTP, task11226Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 11226 nextRegion: 11261 */
    myTP->controlTPParent->barrierCodelets11261[0].decDep();
}
void TP48::_checkInCodelets11229::fire(void)
{
    /*Printing conditional branch node 11229: inlining: 0*/
    if ((this->inputsTPParent->r_darts48[this->getID()]) == 4) {
        myTP->checkInCodelets11232[this->getID()].decDep();
    } else if ((this->inputsTPParent->r_darts48[this->getID()]) == 8) {
        myTP->checkInCodelets11238[this->getID()].decDep();
    } else if ((this->inputsTPParent->r_darts48[this->getID()]) == 16) {
        myTP->checkInCodelets11244[this->getID()].decDep();
    } else if ((this->inputsTPParent->r_darts48[this->getID()]) == 32) {
        myTP->checkInCodelets11250[this->getID()].decDep();
    } else {
        /*elseRegion: OMPTaskDirective 11254*/
        myTP->checkInCodelets11254[this->getID()].decDep();
    }
}
void TP48::_checkInCodelets11232::fire(void)
{

    /*printing node 11232: OMPTaskDirective*/
    /*syncNode: OMPTaskwaitDirective 11261*/
    /*Find the TP to which the nextRegion belongs*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->barrierCodelets11261[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 11232*/
    _task11232Inputs* task11232Inputs
        = new _task11232Inputs(&((this->inputsTPParent->W_darts48[this->getID()])),
            &((this->inputsTPParent->in_darts48[this->getID()])),
            &((this->inputsTPParent->m_darts48[this->getID()])),
            &((this->inputsTPParent->n_darts48[this->getID()])),
            &((this->inputsTPParent->nW_darts48[this->getID()])),
            &((this->inputsTPParent->out_darts48[this->getID()])), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task11232Inputs[this->getID()] = task11232Inputs;
    invoke<TP11232>(myTP, 1, this->getID(), myTP, task11232Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 11232 nextRegion: 11261 */
    myTP->controlTPParent->barrierCodelets11261[0].decDep();
}
void TP48::_checkInCodelets11235::fire(void)
{
    /*Printing conditional branch node 11235: inlining: 0*/
    if ((this->inputsTPParent->r_darts48[this->getID()]) == 8) {
        myTP->checkInCodelets11238[this->getID()].decDep();
    } else if ((this->inputsTPParent->r_darts48[this->getID()]) == 16) {
        myTP->checkInCodelets11244[this->getID()].decDep();
    } else if ((this->inputsTPParent->r_darts48[this->getID()]) == 32) {
        myTP->checkInCodelets11250[this->getID()].decDep();
    } else {
        /*elseRegion: OMPTaskDirective 11254*/
        myTP->checkInCodelets11254[this->getID()].decDep();
    }
}
void TP48::_checkInCodelets11238::fire(void)
{

    /*printing node 11238: OMPTaskDirective*/
    /*syncNode: OMPTaskwaitDirective 11261*/
    /*Find the TP to which the nextRegion belongs*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->barrierCodelets11261[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 11238*/
    _task11238Inputs* task11238Inputs
        = new _task11238Inputs(&((this->inputsTPParent->W_darts48[this->getID()])),
            &((this->inputsTPParent->in_darts48[this->getID()])),
            &((this->inputsTPParent->m_darts48[this->getID()])),
            &((this->inputsTPParent->n_darts48[this->getID()])),
            &((this->inputsTPParent->nW_darts48[this->getID()])),
            &((this->inputsTPParent->out_darts48[this->getID()])), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task11238Inputs[this->getID()] = task11238Inputs;
    invoke<TP11238>(myTP, 1, this->getID(), myTP, task11238Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 11238 nextRegion: 11261 */
    myTP->controlTPParent->barrierCodelets11261[0].decDep();
}
void TP48::_checkInCodelets11241::fire(void)
{
    /*Printing conditional branch node 11241: inlining: 0*/
    if ((this->inputsTPParent->r_darts48[this->getID()]) == 16) {
        myTP->checkInCodelets11244[this->getID()].decDep();
    } else if ((this->inputsTPParent->r_darts48[this->getID()]) == 32) {
        myTP->checkInCodelets11250[this->getID()].decDep();
    } else {
        /*elseRegion: OMPTaskDirective 11254*/
        myTP->checkInCodelets11254[this->getID()].decDep();
    }
}
void TP48::_checkInCodelets11244::fire(void)
{

    /*printing node 11244: OMPTaskDirective*/
    /*syncNode: OMPTaskwaitDirective 11261*/
    /*Find the TP to which the nextRegion belongs*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->barrierCodelets11261[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 11244*/
    _task11244Inputs* task11244Inputs
        = new _task11244Inputs(&((this->inputsTPParent->W_darts48[this->getID()])),
            &((this->inputsTPParent->in_darts48[this->getID()])),
            &((this->inputsTPParent->m_darts48[this->getID()])),
            &((this->inputsTPParent->n_darts48[this->getID()])),
            &((this->inputsTPParent->nW_darts48[this->getID()])),
            &((this->inputsTPParent->out_darts48[this->getID()])), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task11244Inputs[this->getID()] = task11244Inputs;
    invoke<TP11244>(myTP, 1, this->getID(), myTP, task11244Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 11244 nextRegion: 11261 */
    myTP->controlTPParent->barrierCodelets11261[0].decDep();
}
void TP48::_checkInCodelets11247::fire(void)
{
    /*Printing conditional branch node 11247: inlining: 0*/
    if ((this->inputsTPParent->r_darts48[this->getID()]) == 32) {
        myTP->checkInCodelets11250[this->getID()].decDep();
    } else {
        /*elseRegion: OMPTaskDirective 11254*/
        myTP->checkInCodelets11254[this->getID()].decDep();
    }
}
void TP48::_checkInCodelets11250::fire(void)
{

    /*printing node 11250: OMPTaskDirective*/
    /*syncNode: OMPTaskwaitDirective 11261*/
    /*Find the TP to which the nextRegion belongs*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->barrierCodelets11261[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 11250*/
    _task11250Inputs* task11250Inputs
        = new _task11250Inputs(&((this->inputsTPParent->W_darts48[this->getID()])),
            &((this->inputsTPParent->in_darts48[this->getID()])),
            &((this->inputsTPParent->m_darts48[this->getID()])),
            &((this->inputsTPParent->n_darts48[this->getID()])),
            &((this->inputsTPParent->nW_darts48[this->getID()])),
            &((this->inputsTPParent->out_darts48[this->getID()])), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task11250Inputs[this->getID()] = task11250Inputs;
    invoke<TP11250>(myTP, 1, this->getID(), myTP, task11250Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 11250 nextRegion: 11261 */
    myTP->controlTPParent->barrierCodelets11261[0].decDep();
}
void TP48::_checkInCodelets11254::fire(void)
{

    /*printing node 11254: OMPTaskDirective*/
    /*syncNode: OMPTaskwaitDirective 11261*/
    /*Find the TP to which the nextRegion belongs*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->barrierCodelets11261[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 11254*/
    _task11254Inputs* task11254Inputs
        = new _task11254Inputs(&((this->inputsTPParent->W_darts48[this->getID()])),
            &((this->inputsTPParent->in_darts48[this->getID()])),
            &((this->inputsTPParent->m_darts48[this->getID()])),
            &((this->inputsTPParent->n_darts48[this->getID()])),
            &((this->inputsTPParent->nW_darts48[this->getID()])),
            &((this->inputsTPParent->out_darts48[this->getID()])),
            &((this->inputsTPParent->r_darts48[this->getID()])), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task11254Inputs[this->getID()] = task11254Inputs;
    invoke<TP11254>(myTP, 1, this->getID(), myTP, task11254Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 11254 nextRegion: 11261 */
    myTP->controlTPParent->barrierCodelets11261[0].decDep();
}
TP48::TP48(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_mainNextCodelet,
    darts::Codelet* in_mainSyncCodelet, TP48** in_ptrToThisFunctionTP, int in_n, COMPLEX* in_in,
    COMPLEX* in_out, int* in_factors, COMPLEX* in_W, int in_nW)
    : ompTP(in_numThreads, in_mainCodeletID)
    , ptrToThisFunctionTP(in_ptrToThisFunctionTP)
    , inputsTPParent(this)
    , controlTPParent(this)
    , nextCodeletsfft_aux(new Codelet*[in_numThreads])
    , nextSyncCodeletsfft_aux(new Codelet*[in_numThreads])
    , n_darts48(new int[this->numThreads])
    , in_darts48(new COMPLEX*[this->numThreads])
    , out_darts48(new COMPLEX*[this->numThreads])
    , factors_darts48(new int*[this->numThreads])
    , W_darts48(new COMPLEX*[this->numThreads])
    , nW_darts48(new int[this->numThreads])
    , k_darts48(new int[this->numThreads])
    , m_darts48(new int[this->numThreads])
    , r_darts48(new int[this->numThreads])
    , TP11212_LoopCounter(0)
    , TP11212_LoopCounterPerThread(new unsigned int[this->numThreads])
    , TP11205Ptr(nullptr)
    , TP11205_alreadyLaunched(0)
    , task11183Inputs(new _task11183Inputs*[this->numThreads])
    , task11188Inputs(new _task11188Inputs*[this->numThreads])
    , task11193Inputs(new _task11193Inputs*[this->numThreads])
    , task11198Inputs(new _task11198Inputs*[this->numThreads])
    , task11203Inputs(new _task11203Inputs*[this->numThreads])
    , task11217Inputs(new _task11217Inputs*[this->numThreads])
    , task11226Inputs(new _task11226Inputs*[this->numThreads])
    , task11232Inputs(new _task11232Inputs*[this->numThreads])
    , task11238Inputs(new _task11238Inputs*[this->numThreads])
    , task11244Inputs(new _task11244Inputs*[this->numThreads])
    , task11250Inputs(new _task11250Inputs*[this->numThreads])
    , task11254Inputs(new _task11254Inputs*[this->numThreads])
    , checkInCodelets11146(new _checkInCodelets11146[this->numThreads])
    , checkInCodelets11177(new _checkInCodelets11177[this->numThreads])
    , checkInCodelets11223(new _checkInCodelets11223[this->numThreads])
    , barrierCodelets11261(new _barrierCodelets11261[1])
    , checkInCodelets11262(new _checkInCodelets11262[this->numThreads])
    , checkInCodelets11180(new _checkInCodelets11180[this->numThreads])
    , barrierCodelets11211(new _barrierCodelets11211[1])
    , checkInCodelets11213(new _checkInCodelets11213[this->numThreads])
    , checkInCodelets11212(new _checkInCodelets11212[this->numThreads])
    , checkInCodelets11215(new _checkInCodelets11215[this->numThreads])
    , barrierCodelets11222(new _barrierCodelets11222[1])
    , checkInCodelets11183(new _checkInCodelets11183[this->numThreads])
    , checkInCodelets11185(new _checkInCodelets11185[this->numThreads])
    , checkInCodelets11188(new _checkInCodelets11188[this->numThreads])
    , checkInCodelets11190(new _checkInCodelets11190[this->numThreads])
    , checkInCodelets11193(new _checkInCodelets11193[this->numThreads])
    , checkInCodelets11195(new _checkInCodelets11195[this->numThreads])
    , checkInCodelets11198(new _checkInCodelets11198[this->numThreads])
    , checkInCodelets11200(new _checkInCodelets11200[this->numThreads])
    , checkInCodelets11203(new _checkInCodelets11203[this->numThreads])
    , checkInCodelets11205(new _checkInCodelets11205[this->numThreads])
    , checkInCodelets11226(new _checkInCodelets11226[this->numThreads])
    , checkInCodelets11229(new _checkInCodelets11229[this->numThreads])
    , checkInCodelets11232(new _checkInCodelets11232[this->numThreads])
    , checkInCodelets11235(new _checkInCodelets11235[this->numThreads])
    , checkInCodelets11238(new _checkInCodelets11238[this->numThreads])
    , checkInCodelets11241(new _checkInCodelets11241[this->numThreads])
    , checkInCodelets11244(new _checkInCodelets11244[this->numThreads])
    , checkInCodelets11247(new _checkInCodelets11247[this->numThreads])
    , checkInCodelets11250(new _checkInCodelets11250[this->numThreads])
    , checkInCodelets11254(new _checkInCodelets11254[this->numThreads])
{
    memset((void*)TP11212_LoopCounterPerThread, 0, this->numThreads * sizeof(unsigned int));
    barrierCodelets11222[0] = _barrierCodelets11222(this->numThreads, this->numThreads, this, 0);
    barrierCodelets11211[0] = _barrierCodelets11211(this->numThreads, this->numThreads, this, 0);
    barrierCodelets11261[0] = _barrierCodelets11261(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets11254* checkInCodelets11254Ptr = (this->checkInCodelets11254);
    _checkInCodelets11250* checkInCodelets11250Ptr = (this->checkInCodelets11250);
    _checkInCodelets11247* checkInCodelets11247Ptr = (this->checkInCodelets11247);
    _checkInCodelets11244* checkInCodelets11244Ptr = (this->checkInCodelets11244);
    _checkInCodelets11241* checkInCodelets11241Ptr = (this->checkInCodelets11241);
    _checkInCodelets11238* checkInCodelets11238Ptr = (this->checkInCodelets11238);
    _checkInCodelets11235* checkInCodelets11235Ptr = (this->checkInCodelets11235);
    _checkInCodelets11232* checkInCodelets11232Ptr = (this->checkInCodelets11232);
    _checkInCodelets11229* checkInCodelets11229Ptr = (this->checkInCodelets11229);
    _checkInCodelets11226* checkInCodelets11226Ptr = (this->checkInCodelets11226);
    _checkInCodelets11205* checkInCodelets11205Ptr = (this->checkInCodelets11205);
    _checkInCodelets11203* checkInCodelets11203Ptr = (this->checkInCodelets11203);
    _checkInCodelets11200* checkInCodelets11200Ptr = (this->checkInCodelets11200);
    _checkInCodelets11198* checkInCodelets11198Ptr = (this->checkInCodelets11198);
    _checkInCodelets11195* checkInCodelets11195Ptr = (this->checkInCodelets11195);
    _checkInCodelets11193* checkInCodelets11193Ptr = (this->checkInCodelets11193);
    _checkInCodelets11190* checkInCodelets11190Ptr = (this->checkInCodelets11190);
    _checkInCodelets11188* checkInCodelets11188Ptr = (this->checkInCodelets11188);
    _checkInCodelets11185* checkInCodelets11185Ptr = (this->checkInCodelets11185);
    _checkInCodelets11183* checkInCodelets11183Ptr = (this->checkInCodelets11183);
    _checkInCodelets11215* checkInCodelets11215Ptr = (this->checkInCodelets11215);
    _checkInCodelets11212* checkInCodelets11212Ptr = (this->checkInCodelets11212);
    _checkInCodelets11213* checkInCodelets11213Ptr = (this->checkInCodelets11213);
    _checkInCodelets11180* checkInCodelets11180Ptr = (this->checkInCodelets11180);
    _checkInCodelets11262* checkInCodelets11262Ptr = (this->checkInCodelets11262);
    _checkInCodelets11223* checkInCodelets11223Ptr = (this->checkInCodelets11223);
    _checkInCodelets11177* checkInCodelets11177Ptr = (this->checkInCodelets11177);
    _checkInCodelets11146* checkInCodelets11146Ptr = (this->checkInCodelets11146);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets11146);
#endif
    for (size_t codeletCounter = 0; codeletCounter < this->numThreads; codeletCounter++) {
        (*checkInCodelets11254Ptr) = _checkInCodelets11254(1, 1, this, codeletCounter);
        checkInCodelets11254Ptr++;
        (*checkInCodelets11250Ptr) = _checkInCodelets11250(1, 1, this, codeletCounter);
        checkInCodelets11250Ptr++;
        (*checkInCodelets11247Ptr) = _checkInCodelets11247(1, 1, this, codeletCounter);
        checkInCodelets11247Ptr++;
        (*checkInCodelets11244Ptr) = _checkInCodelets11244(1, 1, this, codeletCounter);
        checkInCodelets11244Ptr++;
        (*checkInCodelets11241Ptr) = _checkInCodelets11241(1, 1, this, codeletCounter);
        checkInCodelets11241Ptr++;
        (*checkInCodelets11238Ptr) = _checkInCodelets11238(1, 1, this, codeletCounter);
        checkInCodelets11238Ptr++;
        (*checkInCodelets11235Ptr) = _checkInCodelets11235(1, 1, this, codeletCounter);
        checkInCodelets11235Ptr++;
        (*checkInCodelets11232Ptr) = _checkInCodelets11232(1, 1, this, codeletCounter);
        checkInCodelets11232Ptr++;
        (*checkInCodelets11229Ptr) = _checkInCodelets11229(1, 1, this, codeletCounter);
        checkInCodelets11229Ptr++;
        (*checkInCodelets11226Ptr) = _checkInCodelets11226(1, 1, this, codeletCounter);
        checkInCodelets11226Ptr++;
        (*checkInCodelets11205Ptr) = _checkInCodelets11205(1, 1, this, codeletCounter);
        checkInCodelets11205Ptr++;
        (*checkInCodelets11203Ptr) = _checkInCodelets11203(1, 1, this, codeletCounter);
        checkInCodelets11203Ptr++;
        (*checkInCodelets11200Ptr) = _checkInCodelets11200(1, 1, this, codeletCounter);
        checkInCodelets11200Ptr++;
        (*checkInCodelets11198Ptr) = _checkInCodelets11198(1, 1, this, codeletCounter);
        checkInCodelets11198Ptr++;
        (*checkInCodelets11195Ptr) = _checkInCodelets11195(1, 1, this, codeletCounter);
        checkInCodelets11195Ptr++;
        (*checkInCodelets11193Ptr) = _checkInCodelets11193(1, 1, this, codeletCounter);
        checkInCodelets11193Ptr++;
        (*checkInCodelets11190Ptr) = _checkInCodelets11190(1, 1, this, codeletCounter);
        checkInCodelets11190Ptr++;
        (*checkInCodelets11188Ptr) = _checkInCodelets11188(1, 1, this, codeletCounter);
        checkInCodelets11188Ptr++;
        (*checkInCodelets11185Ptr) = _checkInCodelets11185(1, 1, this, codeletCounter);
        checkInCodelets11185Ptr++;
        (*checkInCodelets11183Ptr) = _checkInCodelets11183(1, 1, this, codeletCounter);
        checkInCodelets11183Ptr++;
        (*checkInCodelets11215Ptr) = _checkInCodelets11215(1, 1, this, codeletCounter);
        checkInCodelets11215Ptr++;
        (*checkInCodelets11212Ptr) = _checkInCodelets11212(1, 1, this, codeletCounter);
        checkInCodelets11212Ptr++;
        (*checkInCodelets11213Ptr) = _checkInCodelets11213(1, 1, this, codeletCounter);
        checkInCodelets11213Ptr++;
        (*checkInCodelets11180Ptr) = _checkInCodelets11180(1, 1, this, codeletCounter);
        checkInCodelets11180Ptr++;
        (*checkInCodelets11262Ptr) = _checkInCodelets11262(1, 1, this, codeletCounter);
        checkInCodelets11262Ptr++;
        (*checkInCodelets11223Ptr) = _checkInCodelets11223(1, 1, this, codeletCounter);
        checkInCodelets11223Ptr++;
        (*checkInCodelets11177Ptr) = _checkInCodelets11177(1, 1, this, codeletCounter);
        checkInCodelets11177Ptr++;
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets11146Ptr) = _checkInCodelets11146(2, 1, this, codeletCounter);
#else
        (*checkInCodelets11146Ptr) = _checkInCodelets11146(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets11146Ptr).decDep();
        checkInCodelets11146Ptr++;
    }
    if (this->numThreads == 1) {
        this->nextCodeletsfft_aux[0] = in_mainNextCodelet;
        this->nextSyncCodeletsfft_aux[0] = in_mainSyncCodelet;
        this->n_darts48[0] = in_n;
        this->in_darts48[0] = in_in;
        this->out_darts48[0] = in_out;
        this->factors_darts48[0] = in_factors;
        this->W_darts48[0] = in_W;
        this->nW_darts48[0] = in_nW;
        this->availableCodelets[0] = 1;
    } else {
        this->n_darts48[this->mainCodeletID] = in_n;
        this->in_darts48[this->mainCodeletID] = in_in;
        this->out_darts48[this->mainCodeletID] = in_out;
        this->factors_darts48[this->mainCodeletID] = in_factors;
        this->W_darts48[this->mainCodeletID] = in_W;
        this->nW_darts48[this->mainCodeletID] = in_nW;
        this->nextCodeletsfft_aux[in_mainCodeletID] = in_mainNextCodelet;
        this->nextSyncCodeletsfft_aux[in_mainCodeletID] = in_mainSyncCodelet;
#if USE_SPIN_CODELETS == 0
        this->firstCodelet[this->mainCodeletID].decDep();
#else
        this->availableCodelets[this->mainCodeletID] = 1;
#endif
        *(this->ptrToThisFunctionTP) = this;
    }
}
TP48::~TP48()
{
    delete[] TP11212_LoopCounterPerThread;
    delete[] task11183Inputs;
    delete[] task11188Inputs;
    delete[] task11193Inputs;
    delete[] task11198Inputs;
    delete[] task11203Inputs;
    delete[] task11217Inputs;
    delete[] task11226Inputs;
    delete[] task11232Inputs;
    delete[] task11238Inputs;
    delete[] task11244Inputs;
    delete[] task11250Inputs;
    delete[] task11254Inputs;
    delete[] checkInCodelets11254;
    delete[] checkInCodelets11250;
    delete[] checkInCodelets11247;
    delete[] checkInCodelets11244;
    delete[] checkInCodelets11241;
    delete[] checkInCodelets11238;
    delete[] checkInCodelets11235;
    delete[] checkInCodelets11232;
    delete[] checkInCodelets11229;
    delete[] checkInCodelets11226;
    delete[] checkInCodelets11205;
    delete[] checkInCodelets11203;
    delete[] checkInCodelets11200;
    delete[] checkInCodelets11198;
    delete[] checkInCodelets11195;
    delete[] checkInCodelets11193;
    delete[] checkInCodelets11190;
    delete[] checkInCodelets11188;
    delete[] checkInCodelets11185;
    delete[] checkInCodelets11183;
    delete[] barrierCodelets11222;
    delete[] checkInCodelets11215;
    delete[] checkInCodelets11212;
    delete[] checkInCodelets11213;
    delete[] barrierCodelets11211;
    delete[] checkInCodelets11180;
    delete[] checkInCodelets11262;
    delete[] barrierCodelets11261;
    delete[] checkInCodelets11223;
    delete[] checkInCodelets11177;
    delete[] checkInCodelets11146;
    delete[] nextCodeletsfft_aux;
    delete[] nextSyncCodeletsfft_aux;
    delete[] n_darts48;
    delete[] in_darts48;
    delete[] out_darts48;
    delete[] factors_darts48;
    delete[] W_darts48;
    delete[] nW_darts48;
    delete[] k_darts48;
    delete[] m_darts48;
    delete[] r_darts48;
}
void TP48::setNewInputs(int in_n, COMPLEX* in_in, COMPLEX* in_out, int* in_factors, COMPLEX* in_W,
    int in_nW, size_t codeletID)
{
    this->n_darts48[codeletID] = in_n;
    this->in_darts48[codeletID] = in_in;
    this->out_darts48[codeletID] = in_out;
    this->factors_darts48[codeletID] = in_factors;
    this->W_darts48[codeletID] = in_W;
    this->nW_darts48[codeletID] = in_nW;
}
/*TP11212: ForStmt*/
void TP11212::_checkInCodelets11217::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif

    /*printing node 11217: OMPTaskDirective*/
    /*syncNode: OMPTaskwaitDirective 11222*/
    /*Find the TP to which the nextRegion belongs*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->TPParent->barrierCodelets11222[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 11217*/
    _task11217Inputs* task11217Inputs
        = new _task11217Inputs(&((this->inputsTPParent->W_darts48[this->getID()])),
            &((this->inputsTPParent->factors_darts48[this->getID()])),
            &((this->inputsTPParent->in_darts48[this->getID()])),
            &((this->inputsTPParent->k_darts48[this->getID()])),
            &((this->inputsTPParent->m_darts48[this->getID()])),
            &((this->inputsTPParent->nW_darts48[this->getID()])),
            &((this->inputsTPParent->out_darts48[this->getID()])), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task11217Inputs[this->getID()] = task11217Inputs;
    invoke<TP11217>(myTP, 1, this->getID(), myTP, task11217Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*The node is the last one in a complex loop, so signal the inc node*/
    /*Find the TP to which the nextRegion belongs*/
    myTP->controlTPParent->TPParent->checkInCodelets11215[this->getID()].decDep();
}
TP11212::TP11212(int in_numThreads, int in_mainCodeletID, TP48* in_TPParent,
    TP48* in_inputsTPParent, TP11212** in_ptrToThisTP)
    : ompTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(in_inputsTPParent)
    , ptrToThisTP(in_ptrToThisTP)
    , checkInCodelets11217(new _checkInCodelets11217[this->numThreads])
{
    /*Initialize Codelets*/
    _checkInCodelets11217* checkInCodelets11217Ptr = (this->checkInCodelets11217);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets11217);
#endif
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets11217Ptr) = _checkInCodelets11217(2, 1, this, codeletCounter);
#else
        (*checkInCodelets11217Ptr) = _checkInCodelets11217(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets11217Ptr).decDep();
        checkInCodelets11217Ptr++;
    }
    *(this->ptrToThisTP) = this;
#if USE_SPIN_CODELETS == 0
    this->firstCodelet[this->getID()].decDep();
#else
    this->availableCodelets[this->getID()] = 1;
#endif
}
TP11212::~TP11212() { delete[] checkInCodelets11217; }
/*TP11217: OMPTaskDirective*/
void TP11217::_checkInCodelets11218::fire(void)
{
/*Init the vars for this region*/

/*printing node 11218: CallExpr*/
/*Finding the next codelet after the function call*/
#if 0
/*Signaling next codelet region: 11218 nextRegion: 11529 */
myTP->controlTPParent->checkInCodelets11529.decDep();
#endif
/*Finding the next sync point before making the function call*/
#if 0
/*Find the sync point for a function call*/
/*syncNode: OMPTaskwaitDirective 11222*/
/*No syncpoint found or outside task's boundaries, looking for syncCodelet*/
#endif
    /*Make the function call*/
    invoke<TP_fft_aux>(myTP, myTP->numThreads, this->getID(),
        &(myTP->controlTPParent->checkInCodelets11529),
        (myTP->controlTPParent->task11217Inputs->nextSyncCodelet), nullptr,
        (this->taskInputs->m_darts11217),
        (this->taskInputs->out_darts11217) + (this->taskInputs->k_darts11217),
        (this->taskInputs->in_darts11217) + (this->taskInputs->k_darts11217),
        (this->taskInputs->factors_darts11217) + 1, (this->taskInputs->W_darts11217),
        (this->taskInputs->nW_darts11217));
}
void TP11217::_checkInCodelets11529::fire(void)
{

    /*printing node 11529: NullStmt*/
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task11217Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task11217Inputs->nextSyncCodelet->decDep();
}
TP11217::TP11217(int in_numThreads, int in_mainCodeletID, TP11212* in_TPParent,
    _task11217Inputs* in_task11217Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task11217Inputs(in_task11217Inputs)
    , TP11218Ptr(nullptr)
    , TP11218_alreadyLaunched(0)
    , checkInCodelets11218(1, 1, this, this->mainCodeletID)
    , checkInCodelets11529(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets11218.decDep();
}
TP11217::~TP11217() { delete (task11217Inputs); }
/*TP11183: OMPTaskDirective*/
void TP11183::_checkInCodelets11184::fire(void)
{
/*Init the vars for this region*/

/*printing node 11184: CallExpr*/
/*Finding the next codelet after the function call*/
#if 0
/*Signaling next codelet region: 11184 nextRegion: 11524 */
myTP->controlTPParent->checkInCodelets11524.decDep();
#endif
/*Finding the next sync point before making the function call*/
#if 0
/*Find the sync point for a function call*/
/*syncNode: OMPTaskwaitDirective 11211*/
/*No syncpoint found or outside task's boundaries, looking for syncCodelet*/
#endif
    /*Make the function call*/
    invoke<TP_fft_unshuffle_32>(myTP, myTP->numThreads, this->getID(),
        &(myTP->controlTPParent->checkInCodelets11524),
        (myTP->controlTPParent->task11183Inputs->nextSyncCodelet), nullptr, 0,
        (this->taskInputs->m_darts11183), (this->taskInputs->in_darts11183),
        (this->taskInputs->out_darts11183), (this->taskInputs->m_darts11183));
}
void TP11183::_checkInCodelets11524::fire(void)
{

    /*printing node 11524: NullStmt*/
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task11183Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task11183Inputs->nextSyncCodelet->decDep();
}
TP11183::TP11183(int in_numThreads, int in_mainCodeletID, TP48* in_TPParent,
    _task11183Inputs* in_task11183Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task11183Inputs(in_task11183Inputs)
    , TP11184Ptr(nullptr)
    , TP11184_alreadyLaunched(0)
    , checkInCodelets11184(1, 1, this, this->mainCodeletID)
    , checkInCodelets11524(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets11184.decDep();
}
TP11183::~TP11183() { delete (task11183Inputs); }
/*TP11188: OMPTaskDirective*/
void TP11188::_checkInCodelets11189::fire(void)
{
/*Init the vars for this region*/

/*printing node 11189: CallExpr*/
/*Finding the next codelet after the function call*/
#if 0
/*Signaling next codelet region: 11189 nextRegion: 11525 */
myTP->controlTPParent->checkInCodelets11525.decDep();
#endif
/*Finding the next sync point before making the function call*/
#if 0
/*Find the sync point for a function call*/
/*syncNode: OMPTaskwaitDirective 11211*/
/*No syncpoint found or outside task's boundaries, looking for syncCodelet*/
#endif
    /*Make the function call*/
    invoke<TP_fft_unshuffle_16>(myTP, myTP->numThreads, this->getID(),
        &(myTP->controlTPParent->checkInCodelets11525),
        (myTP->controlTPParent->task11188Inputs->nextSyncCodelet), nullptr, 0,
        (this->taskInputs->m_darts11188), (this->taskInputs->in_darts11188),
        (this->taskInputs->out_darts11188), (this->taskInputs->m_darts11188));
}
void TP11188::_checkInCodelets11525::fire(void)
{

    /*printing node 11525: NullStmt*/
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task11188Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task11188Inputs->nextSyncCodelet->decDep();
}
TP11188::TP11188(int in_numThreads, int in_mainCodeletID, TP48* in_TPParent,
    _task11188Inputs* in_task11188Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task11188Inputs(in_task11188Inputs)
    , TP11189Ptr(nullptr)
    , TP11189_alreadyLaunched(0)
    , checkInCodelets11189(1, 1, this, this->mainCodeletID)
    , checkInCodelets11525(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets11189.decDep();
}
TP11188::~TP11188() { delete (task11188Inputs); }
/*TP11193: OMPTaskDirective*/
void TP11193::_checkInCodelets11194::fire(void)
{
/*Init the vars for this region*/

/*printing node 11194: CallExpr*/
/*Finding the next codelet after the function call*/
#if 0
/*Signaling next codelet region: 11194 nextRegion: 11526 */
myTP->controlTPParent->checkInCodelets11526.decDep();
#endif
/*Finding the next sync point before making the function call*/
#if 0
/*Find the sync point for a function call*/
/*syncNode: OMPTaskwaitDirective 11211*/
/*No syncpoint found or outside task's boundaries, looking for syncCodelet*/
#endif
    /*Make the function call*/
    invoke<TP_fft_unshuffle_8>(myTP, myTP->numThreads, this->getID(),
        &(myTP->controlTPParent->checkInCodelets11526),
        (myTP->controlTPParent->task11193Inputs->nextSyncCodelet), nullptr, 0,
        (this->taskInputs->m_darts11193), (this->taskInputs->in_darts11193),
        (this->taskInputs->out_darts11193), (this->taskInputs->m_darts11193));
}
void TP11193::_checkInCodelets11526::fire(void)
{

    /*printing node 11526: NullStmt*/
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task11193Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task11193Inputs->nextSyncCodelet->decDep();
}
TP11193::TP11193(int in_numThreads, int in_mainCodeletID, TP48* in_TPParent,
    _task11193Inputs* in_task11193Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task11193Inputs(in_task11193Inputs)
    , TP11194Ptr(nullptr)
    , TP11194_alreadyLaunched(0)
    , checkInCodelets11194(1, 1, this, this->mainCodeletID)
    , checkInCodelets11526(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets11194.decDep();
}
TP11193::~TP11193() { delete (task11193Inputs); }
/*TP11198: OMPTaskDirective*/
void TP11198::_checkInCodelets11199::fire(void)
{
/*Init the vars for this region*/

/*printing node 11199: CallExpr*/
/*Finding the next codelet after the function call*/
#if 0
/*Signaling next codelet region: 11199 nextRegion: 11527 */
myTP->controlTPParent->checkInCodelets11527.decDep();
#endif
/*Finding the next sync point before making the function call*/
#if 0
/*Find the sync point for a function call*/
/*syncNode: OMPTaskwaitDirective 11211*/
/*No syncpoint found or outside task's boundaries, looking for syncCodelet*/
#endif
    /*Make the function call*/
    invoke<TP_fft_unshuffle_4>(myTP, myTP->numThreads, this->getID(),
        &(myTP->controlTPParent->checkInCodelets11527),
        (myTP->controlTPParent->task11198Inputs->nextSyncCodelet), nullptr, 0,
        (this->taskInputs->m_darts11198), (this->taskInputs->in_darts11198),
        (this->taskInputs->out_darts11198), (this->taskInputs->m_darts11198));
}
void TP11198::_checkInCodelets11527::fire(void)
{

    /*printing node 11527: NullStmt*/
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task11198Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task11198Inputs->nextSyncCodelet->decDep();
}
TP11198::TP11198(int in_numThreads, int in_mainCodeletID, TP48* in_TPParent,
    _task11198Inputs* in_task11198Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task11198Inputs(in_task11198Inputs)
    , TP11199Ptr(nullptr)
    , TP11199_alreadyLaunched(0)
    , checkInCodelets11199(1, 1, this, this->mainCodeletID)
    , checkInCodelets11527(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets11199.decDep();
}
TP11198::~TP11198() { delete (task11198Inputs); }
/*TP11203: OMPTaskDirective*/
void TP11203::_checkInCodelets11204::fire(void)
{
/*Init the vars for this region*/

/*printing node 11204: CallExpr*/
/*Finding the next codelet after the function call*/
#if 0
/*Signaling next codelet region: 11204 nextRegion: 11528 */
myTP->controlTPParent->checkInCodelets11528.decDep();
#endif
/*Finding the next sync point before making the function call*/
#if 0
/*Find the sync point for a function call*/
/*syncNode: OMPTaskwaitDirective 11211*/
/*No syncpoint found or outside task's boundaries, looking for syncCodelet*/
#endif
    /*Make the function call*/
    invoke<TP_fft_unshuffle_2>(myTP, myTP->numThreads, this->getID(),
        &(myTP->controlTPParent->checkInCodelets11528),
        (myTP->controlTPParent->task11203Inputs->nextSyncCodelet), nullptr, 0,
        (this->taskInputs->m_darts11203), (this->taskInputs->in_darts11203),
        (this->taskInputs->out_darts11203), (this->taskInputs->m_darts11203));
}
void TP11203::_checkInCodelets11528::fire(void)
{

    /*printing node 11528: NullStmt*/
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task11203Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task11203Inputs->nextSyncCodelet->decDep();
}
TP11203::TP11203(int in_numThreads, int in_mainCodeletID, TP48* in_TPParent,
    _task11203Inputs* in_task11203Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task11203Inputs(in_task11203Inputs)
    , TP11204Ptr(nullptr)
    , TP11204_alreadyLaunched(0)
    , checkInCodelets11204(1, 1, this, this->mainCodeletID)
    , checkInCodelets11528(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets11204.decDep();
}
TP11203::~TP11203() { delete (task11203Inputs); }
/*TP11226: OMPTaskDirective*/
void TP11226::_checkInCodelets11227::fire(void)
{
/*Init the vars for this region*/

/*printing node 11227: CallExpr*/
/*Finding the next codelet after the function call*/
#if 0
/*Signaling next codelet region: 11227 nextRegion: 11531 */
myTP->controlTPParent->checkInCodelets11531.decDep();
#endif
/*Finding the next sync point before making the function call*/
#if 0
/*Find the sync point for a function call*/
/*syncNode: OMPTaskwaitDirective 11261*/
/*No syncpoint found or outside task's boundaries, looking for syncCodelet*/
#endif
    /*Make the function call*/
    invoke<TP_fft_twiddle_2>(myTP, myTP->numThreads, this->getID(),
        &(myTP->controlTPParent->checkInCodelets11531),
        (myTP->controlTPParent->task11226Inputs->nextSyncCodelet), nullptr, 0,
        (this->taskInputs->m_darts11226), (this->taskInputs->in_darts11226),
        (this->taskInputs->out_darts11226), (this->taskInputs->W_darts11226),
        (this->taskInputs->nW_darts11226),
        (this->taskInputs->nW_darts11226) / (this->taskInputs->n_darts11226),
        (this->taskInputs->m_darts11226));
}
void TP11226::_checkInCodelets11531::fire(void)
{

    /*printing node 11531: NullStmt*/
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task11226Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task11226Inputs->nextSyncCodelet->decDep();
}
TP11226::TP11226(int in_numThreads, int in_mainCodeletID, TP48* in_TPParent,
    _task11226Inputs* in_task11226Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task11226Inputs(in_task11226Inputs)
    , TP11227Ptr(nullptr)
    , TP11227_alreadyLaunched(0)
    , checkInCodelets11227(1, 1, this, this->mainCodeletID)
    , checkInCodelets11531(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets11227.decDep();
}
TP11226::~TP11226() { delete (task11226Inputs); }
/*TP11232: OMPTaskDirective*/
void TP11232::_checkInCodelets11233::fire(void)
{
/*Init the vars for this region*/

/*printing node 11233: CallExpr*/
/*Finding the next codelet after the function call*/
#if 0
/*Signaling next codelet region: 11233 nextRegion: 11532 */
myTP->controlTPParent->checkInCodelets11532.decDep();
#endif
/*Finding the next sync point before making the function call*/
#if 0
/*Find the sync point for a function call*/
/*syncNode: OMPTaskwaitDirective 11261*/
/*No syncpoint found or outside task's boundaries, looking for syncCodelet*/
#endif
    /*Make the function call*/
    invoke<TP_fft_twiddle_4>(myTP, myTP->numThreads, this->getID(),
        &(myTP->controlTPParent->checkInCodelets11532),
        (myTP->controlTPParent->task11232Inputs->nextSyncCodelet), nullptr, 0,
        (this->taskInputs->m_darts11232), (this->taskInputs->in_darts11232),
        (this->taskInputs->out_darts11232), (this->taskInputs->W_darts11232),
        (this->taskInputs->nW_darts11232),
        (this->taskInputs->nW_darts11232) / (this->taskInputs->n_darts11232),
        (this->taskInputs->m_darts11232));
}
void TP11232::_checkInCodelets11532::fire(void)
{

    /*printing node 11532: NullStmt*/
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task11232Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task11232Inputs->nextSyncCodelet->decDep();
}
TP11232::TP11232(int in_numThreads, int in_mainCodeletID, TP48* in_TPParent,
    _task11232Inputs* in_task11232Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task11232Inputs(in_task11232Inputs)
    , TP11233Ptr(nullptr)
    , TP11233_alreadyLaunched(0)
    , checkInCodelets11233(1, 1, this, this->mainCodeletID)
    , checkInCodelets11532(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets11233.decDep();
}
TP11232::~TP11232() { delete (task11232Inputs); }
/*TP11238: OMPTaskDirective*/
void TP11238::_checkInCodelets11239::fire(void)
{
/*Init the vars for this region*/

/*printing node 11239: CallExpr*/
/*Finding the next codelet after the function call*/
#if 0
/*Signaling next codelet region: 11239 nextRegion: 11533 */
myTP->controlTPParent->checkInCodelets11533.decDep();
#endif
/*Finding the next sync point before making the function call*/
#if 0
/*Find the sync point for a function call*/
/*syncNode: OMPTaskwaitDirective 11261*/
/*No syncpoint found or outside task's boundaries, looking for syncCodelet*/
#endif
    /*Make the function call*/
    invoke<TP_fft_twiddle_8>(myTP, myTP->numThreads, this->getID(),
        &(myTP->controlTPParent->checkInCodelets11533),
        (myTP->controlTPParent->task11238Inputs->nextSyncCodelet), nullptr, 0,
        (this->taskInputs->m_darts11238), (this->taskInputs->in_darts11238),
        (this->taskInputs->out_darts11238), (this->taskInputs->W_darts11238),
        (this->taskInputs->nW_darts11238),
        (this->taskInputs->nW_darts11238) / (this->taskInputs->n_darts11238),
        (this->taskInputs->m_darts11238));
}
void TP11238::_checkInCodelets11533::fire(void)
{

    /*printing node 11533: NullStmt*/
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task11238Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task11238Inputs->nextSyncCodelet->decDep();
}
TP11238::TP11238(int in_numThreads, int in_mainCodeletID, TP48* in_TPParent,
    _task11238Inputs* in_task11238Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task11238Inputs(in_task11238Inputs)
    , TP11239Ptr(nullptr)
    , TP11239_alreadyLaunched(0)
    , checkInCodelets11239(1, 1, this, this->mainCodeletID)
    , checkInCodelets11533(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets11239.decDep();
}
TP11238::~TP11238() { delete (task11238Inputs); }
/*TP11244: OMPTaskDirective*/
void TP11244::_checkInCodelets11245::fire(void)
{
/*Init the vars for this region*/

/*printing node 11245: CallExpr*/
/*Finding the next codelet after the function call*/
#if 0
/*Signaling next codelet region: 11245 nextRegion: 11534 */
myTP->controlTPParent->checkInCodelets11534.decDep();
#endif
/*Finding the next sync point before making the function call*/
#if 0
/*Find the sync point for a function call*/
/*syncNode: OMPTaskwaitDirective 11261*/
/*No syncpoint found or outside task's boundaries, looking for syncCodelet*/
#endif
    /*Make the function call*/
    invoke<TP_fft_twiddle_16>(myTP, myTP->numThreads, this->getID(),
        &(myTP->controlTPParent->checkInCodelets11534),
        (myTP->controlTPParent->task11244Inputs->nextSyncCodelet), nullptr, 0,
        (this->taskInputs->m_darts11244), (this->taskInputs->in_darts11244),
        (this->taskInputs->out_darts11244), (this->taskInputs->W_darts11244),
        (this->taskInputs->nW_darts11244),
        (this->taskInputs->nW_darts11244) / (this->taskInputs->n_darts11244),
        (this->taskInputs->m_darts11244));
}
void TP11244::_checkInCodelets11534::fire(void)
{

    /*printing node 11534: NullStmt*/
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task11244Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task11244Inputs->nextSyncCodelet->decDep();
}
TP11244::TP11244(int in_numThreads, int in_mainCodeletID, TP48* in_TPParent,
    _task11244Inputs* in_task11244Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task11244Inputs(in_task11244Inputs)
    , TP11245Ptr(nullptr)
    , TP11245_alreadyLaunched(0)
    , checkInCodelets11245(1, 1, this, this->mainCodeletID)
    , checkInCodelets11534(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets11245.decDep();
}
TP11244::~TP11244() { delete (task11244Inputs); }
/*TP11250: OMPTaskDirective*/
void TP11250::_checkInCodelets11251::fire(void)
{
/*Init the vars for this region*/

/*printing node 11251: CallExpr*/
/*Finding the next codelet after the function call*/
#if 0
/*Signaling next codelet region: 11251 nextRegion: 11535 */
myTP->controlTPParent->checkInCodelets11535.decDep();
#endif
/*Finding the next sync point before making the function call*/
#if 0
/*Find the sync point for a function call*/
/*syncNode: OMPTaskwaitDirective 11261*/
/*No syncpoint found or outside task's boundaries, looking for syncCodelet*/
#endif
    /*Make the function call*/
    invoke<TP_fft_twiddle_32>(myTP, myTP->numThreads, this->getID(),
        &(myTP->controlTPParent->checkInCodelets11535),
        (myTP->controlTPParent->task11250Inputs->nextSyncCodelet), nullptr, 0,
        (this->taskInputs->m_darts11250), (this->taskInputs->in_darts11250),
        (this->taskInputs->out_darts11250), (this->taskInputs->W_darts11250),
        (this->taskInputs->nW_darts11250),
        (this->taskInputs->nW_darts11250) / (this->taskInputs->n_darts11250),
        (this->taskInputs->m_darts11250));
}
void TP11250::_checkInCodelets11535::fire(void)
{

    /*printing node 11535: NullStmt*/
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task11250Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task11250Inputs->nextSyncCodelet->decDep();
}
TP11250::TP11250(int in_numThreads, int in_mainCodeletID, TP48* in_TPParent,
    _task11250Inputs* in_task11250Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task11250Inputs(in_task11250Inputs)
    , TP11251Ptr(nullptr)
    , TP11251_alreadyLaunched(0)
    , checkInCodelets11251(1, 1, this, this->mainCodeletID)
    , checkInCodelets11535(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets11251.decDep();
}
TP11250::~TP11250() { delete (task11250Inputs); }
/*TP11254: OMPTaskDirective*/
void TP11254::_checkInCodelets11255::fire(void)
{
/*Init the vars for this region*/

/*printing node 11255: CallExpr*/
/*Finding the next codelet after the function call*/
#if 0
/*Signaling next codelet region: 11255 nextRegion: 11536 */
myTP->controlTPParent->checkInCodelets11536.decDep();
#endif
/*Finding the next sync point before making the function call*/
#if 0
/*Find the sync point for a function call*/
/*syncNode: OMPTaskwaitDirective 11261*/
/*No syncpoint found or outside task's boundaries, looking for syncCodelet*/
#endif
    /*Make the function call*/
    invoke<TP_fft_twiddle_gen>(myTP, myTP->numThreads, this->getID(),
        &(myTP->controlTPParent->checkInCodelets11536),
        (myTP->controlTPParent->task11254Inputs->nextSyncCodelet), nullptr, 0,
        (this->taskInputs->m_darts11254), (this->taskInputs->in_darts11254),
        (this->taskInputs->out_darts11254), (this->taskInputs->W_darts11254),
        (this->taskInputs->nW_darts11254),
        (this->taskInputs->nW_darts11254) / (this->taskInputs->n_darts11254),
        (this->taskInputs->r_darts11254), (this->taskInputs->m_darts11254));
}
void TP11254::_checkInCodelets11536::fire(void)
{

    /*printing node 11536: NullStmt*/
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task11254Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task11254Inputs->nextSyncCodelet->decDep();
}
TP11254::TP11254(int in_numThreads, int in_mainCodeletID, TP48* in_TPParent,
    _task11254Inputs* in_task11254Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task11254Inputs(in_task11254Inputs)
    , TP11255Ptr(nullptr)
    , TP11255_alreadyLaunched(0)
    , checkInCodelets11255(1, 1, this, this->mainCodeletID)
    , checkInCodelets11536(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets11255.decDep();
}
TP11254::~TP11254() { delete (task11254Inputs); }
/*TP11383: OMPParallelDirective*/
void TP11383::_barrierCodelets11383::fire(void)
{
    TP11383* myTP = static_cast<TP11383*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
void TP11383::_checkInCodelets11384::fire(void)
{
    /*Select the thread executing OMPSingleDirective 11384*/
    if (!__sync_val_compare_and_swap(&(myTP->TP11384_alreadyLaunched), 0, 1)) {
        invoke<TP11384>(myTP, 1, this->getID(), myTP, &(*(this->inputsTPParent->W_darts11383)),
            &(*(this->inputsTPParent->n_darts11383)));
    } else {
        myTP->barrierCodelets11384[0].decDep();
    }
}
void TP11383::_barrierCodelets11384::fire(void)
{
    TP11383* myTP = static_cast<TP11383*>(myTP_);
    myTP->TPParent->barrierCodelets11383[0].setDep(0);
    myTP->add(&(myTP->TPParent->barrierCodelets11383[0]));
}
TP11383::TP11383(
    int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet, COMPLEX** in_W, int* in_n)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , W_darts11383(in_W) /*OMP_SHARED - INPUT*/
    , n_darts11383(in_n) /*OMP_SHARED - INPUT*/
    , TP11384Ptr(nullptr)
    , TP11384_alreadyLaunched(0)
    , task11385Inputs(new _task11385Inputs*[this->numThreads])
    , barrierCodelets11383(new _barrierCodelets11383[1])
    , checkInCodelets11384(new _checkInCodelets11384[this->numThreads])
    , barrierCodelets11384(new _barrierCodelets11384[1])
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets11383[0] = _barrierCodelets11383(ompNumThreads, ompNumThreads, this, 0);
    barrierCodelets11384[0] = _barrierCodelets11384(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets11384* checkInCodelets11384Ptr = (this->checkInCodelets11384);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets11384Ptr) = _checkInCodelets11384(1, 1, this, codeletCounter);
        (*checkInCodelets11384Ptr).decDep();
        checkInCodelets11384Ptr++;
    }
}
TP11383::~TP11383()
{
    delete[] task11385Inputs;
    delete[] barrierCodelets11383;
    delete[] barrierCodelets11384;
    delete[] checkInCodelets11384;
}
/*TP11384: OMPSingleDirective*/
void TP11384::_checkInCodelets11385::fire(void)
{

    /*printing node 11385: OMPTaskDirective*/
    /*syncNode: OMPSingleDirective 11384*/
    /*Find the TP to which the nextRegion belongs*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->TPParent->barrierCodelets11384[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 11385*/
    _task11385Inputs* task11385Inputs
        = new _task11385Inputs(&(*(this->inputsTPParent->W_darts11384)),
            &(*(this->inputsTPParent->n_darts11384)), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task11385Inputs[0] = task11385Inputs;
    invoke<TP11385>(myTP, 1, this->getID(), myTP, task11385Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets11384[0].decDep();
}
TP11384::TP11384(
    int in_numThreads, int in_mainCodeletID, TP11383* in_TPParent, COMPLEX** in_W, int* in_n)
    : ompOMPSingleDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , W_darts11384(in_W) /*OMP_SHARED - INPUT*/
    , n_darts11384(in_n) /*OMP_SHARED - INPUT*/
    , task11385Inputs(new _task11385Inputs*[this->numThreads])
    , checkInCodelets11385(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets11385.decDep();
}
TP11384::~TP11384() { delete[] task11385Inputs; }
/*TP11385: OMPTaskDirective*/
void TP11385::_checkInCodelets11386::fire(void)
{
/*Init the vars for this region*/

/*printing node 11386: CallExpr*/
/*Finding the next codelet after the function call*/
#if 0
/*Signaling next codelet region: 11386 nextRegion: 11518 */
myTP->controlTPParent->checkInCodelets11518.decDep();
#endif
/*Finding the next sync point before making the function call*/
#if 0
/*Find the sync point for a function call*/
/*syncNode: OMPSingleDirective 11384*/
/*No syncpoint found or outside task's boundaries, looking for syncCodelet*/
#endif
    /*Make the function call*/
    invoke<TP_compute_w_coefficients>(myTP, myTP->numThreads, this->getID(),
        &(myTP->controlTPParent->checkInCodelets11518),
        (myTP->controlTPParent->task11385Inputs->nextSyncCodelet), nullptr,
        (*(this->taskInputs->n_darts11385)), 0, (*(this->taskInputs->n_darts11385)) / 2,
        (*(this->taskInputs->W_darts11385)));
}
void TP11385::_checkInCodelets11518::fire(void)
{

    /*printing node 11518: NullStmt*/
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task11385Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task11385Inputs->nextSyncCodelet->decDep();
}
TP11385::TP11385(int in_numThreads, int in_mainCodeletID, TP11384* in_TPParent,
    _task11385Inputs* in_task11385Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task11385Inputs(in_task11385Inputs)
    , TP11386Ptr(nullptr)
    , TP11386_alreadyLaunched(0)
    , checkInCodelets11386(1, 1, this, this->mainCodeletID)
    , checkInCodelets11518(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets11386.decDep();
}
TP11385::~TP11385() { delete (task11385Inputs); }
/*TP11407: OMPParallelDirective*/
void TP11407::_barrierCodelets11407::fire(void)
{
    TP11407* myTP = static_cast<TP11407*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
void TP11407::_checkInCodelets11408::fire(void)
{
    /*Select the thread executing OMPSingleDirective 11408*/
    if (!__sync_val_compare_and_swap(&(myTP->TP11408_alreadyLaunched), 0, 1)) {
        invoke<TP11408>(myTP, 1, this->getID(), myTP, &(*(this->inputsTPParent->W_darts11407)),
            ((this->inputsTPParent->factors_darts11407)),
            (this->inputsTPParent->factors_outer11407_size),
            &(*(this->inputsTPParent->in_darts11407)), &(*(this->inputsTPParent->n_darts11407)),
            &(*(this->inputsTPParent->out_darts11407)));
    } else {
        myTP->barrierCodelets11408[0].decDep();
    }
}
void TP11407::_barrierCodelets11408::fire(void)
{
    TP11407* myTP = static_cast<TP11407*>(myTP_);
    myTP->TPParent->barrierCodelets11407[0].setDep(0);
    myTP->add(&(myTP->TPParent->barrierCodelets11407[0]));
}
TP11407::TP11407(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet, COMPLEX** in_W,
    int* in_factors, int in_factors_outer11407_size, COMPLEX** in_in, int* in_n, COMPLEX** in_out)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , W_darts11407(in_W) /*OMP_SHARED - INPUT*/
    , factors_darts11407(in_factors) /*OMP_SHARED - INPUT*/
    , factors_outer11407_size(in_factors_outer11407_size)
    , in_darts11407(in_in) /*OMP_SHARED - INPUT*/
    , n_darts11407(in_n) /*OMP_SHARED - INPUT*/
    , out_darts11407(in_out) /*OMP_SHARED - INPUT*/
    , TP11408Ptr(nullptr)
    , TP11408_alreadyLaunched(0)
    , task11409Inputs(new _task11409Inputs*[this->numThreads])
    , barrierCodelets11407(new _barrierCodelets11407[1])
    , checkInCodelets11408(new _checkInCodelets11408[this->numThreads])
    , barrierCodelets11408(new _barrierCodelets11408[1])
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets11407[0] = _barrierCodelets11407(ompNumThreads, ompNumThreads, this, 0);
    barrierCodelets11408[0] = _barrierCodelets11408(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets11408* checkInCodelets11408Ptr = (this->checkInCodelets11408);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets11408Ptr) = _checkInCodelets11408(1, 1, this, codeletCounter);
        (*checkInCodelets11408Ptr).decDep();
        checkInCodelets11408Ptr++;
    }
}
TP11407::~TP11407()
{
    delete[] task11409Inputs;
    delete[] barrierCodelets11407;
    delete[] barrierCodelets11408;
    delete[] checkInCodelets11408;
}
/*TP11408: OMPSingleDirective*/
void TP11408::_checkInCodelets11409::fire(void)
{

    /*printing node 11409: OMPTaskDirective*/
    /*syncNode: OMPSingleDirective 11408*/
    /*Find the TP to which the nextRegion belongs*/
    Codelet* nextSyncCodelet = &(myTP->controlTPParent->TPParent->barrierCodelets11408[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 11409*/
    _task11409Inputs* task11409Inputs = new _task11409Inputs(
        &(*(this->inputsTPParent->W_darts11408)), ((this->inputsTPParent->factors_darts11408)),
        (this->inputsTPParent->factors_outer11408_size), &(*(this->inputsTPParent->in_darts11408)),
        &(*(this->inputsTPParent->n_darts11408)), &(*(this->inputsTPParent->out_darts11408)),
        nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task11409Inputs[0] = task11409Inputs;
    invoke<TP11409>(myTP, 1, this->getID(), myTP, task11409Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets11408[0].decDep();
}
TP11408::TP11408(int in_numThreads, int in_mainCodeletID, TP11407* in_TPParent, COMPLEX** in_W,
    int* in_factors, int in_factors_outer11408_size, COMPLEX** in_in, int* in_n, COMPLEX** in_out)
    : ompOMPSingleDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , W_darts11408(in_W) /*OMP_SHARED - INPUT*/
    , factors_darts11408(in_factors) /*OMP_SHARED - INPUT*/
    , factors_outer11408_size(in_factors_outer11408_size)
    , in_darts11408(in_in) /*OMP_SHARED - INPUT*/
    , n_darts11408(in_n) /*OMP_SHARED - INPUT*/
    , out_darts11408(in_out) /*OMP_SHARED - INPUT*/
    , task11409Inputs(new _task11409Inputs*[this->numThreads])
    , checkInCodelets11409(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets11409.decDep();
}
TP11408::~TP11408() { delete[] task11409Inputs; }
/*TP11409: OMPTaskDirective*/
void TP11409::_checkInCodelets11410::fire(void)
{
/*Init the vars for this region*/

/*printing node 11410: CallExpr*/
/*Finding the next codelet after the function call*/
#if 0
/*Signaling next codelet region: 11410 nextRegion: 11519 */
myTP->controlTPParent->checkInCodelets11519.decDep();
#endif
/*Finding the next sync point before making the function call*/
#if 0
/*Find the sync point for a function call*/
/*syncNode: OMPSingleDirective 11408*/
/*No syncpoint found or outside task's boundaries, looking for syncCodelet*/
#endif
    /*Make the function call*/
    invoke<TP_fft_aux>(myTP, myTP->numThreads, this->getID(),
        &(myTP->controlTPParent->checkInCodelets11519),
        (myTP->controlTPParent->task11409Inputs->nextSyncCodelet), nullptr,
        (*(this->taskInputs->n_darts11409)), (*(this->taskInputs->in_darts11409)),
        (*(this->taskInputs->out_darts11409)), ((this->taskInputs->factors_darts11409)),
        (*(this->taskInputs->W_darts11409)), (*(this->taskInputs->n_darts11409)));
}
void TP11409::_checkInCodelets11519::fire(void)
{

    /*printing node 11519: NullStmt*/
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task11409Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task11409Inputs->nextSyncCodelet->decDep();
}
TP11409::TP11409(int in_numThreads, int in_mainCodeletID, TP11408* in_TPParent,
    _task11409Inputs* in_task11409Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task11409Inputs(in_task11409Inputs)
    , TP11410Ptr(nullptr)
    , TP11410_alreadyLaunched(0)
    , checkInCodelets11410(1, 1, this, this->mainCodeletID)
    , checkInCodelets11519(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets11410.decDep();
}
TP11409::~TP11409() { delete (task11409Inputs); }
