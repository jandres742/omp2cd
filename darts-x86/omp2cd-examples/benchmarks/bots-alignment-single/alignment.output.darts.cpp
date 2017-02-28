#include "alignment.output.darts.h"
using namespace darts;
using namespace std;
char* amino_acid_codes = "ABCDEFGHIKLMNPQRSTUVWXYZ-";
char* amino_acid_order = "ABCDEFGHIKLMNPQRSTVWXYZ";
char** args;
int* bench_output;
int clustalw = 0;
int def_aa_xref[33];
int dnaFlag = 0;
double gap_extend;
double gap_extend_scale;
double gap_open;
double gap_open_scale;
int gap_pos1;
int gap_pos2;
int gon250mt[276] = { 24, 0, 0, 5, 0, 115, -3, 0, -32, 47, 0, 0, -30, 27, 36, -23, 0, -8, -45, -39,
    70, 5, 0, -20, 1, -8, -52, 66, -8, 0, -13, 4, 4, -1, -14, 60, -8, 0, -11, -38, -27, 10, -45,
    -22, 40, -4, 0, -28, 5, 12, -33, -11, 6, -21, 32, -12, 0, -15, -40, -28, 20, -44, -19, 28, -21,
    40, -7, 0, -9, -30, -20, 16, -35, -13, 25, -14, 28, 43, -3, 0, -18, 22, 9, -31, 4, 12, -28, 8,
    -30, -22, 38, 3, 0, -31, -7, -5, -38, -16, -11, -26, -6, -23, -24, -9, 76, -2, 0, -24, 9, 17,
    -26, -10, 12, -19, 15, -16, -10, 7, -2, 27, -6, 0, -22, -3, 4, -32, -10, 6, -24, 27, -22, -17,
    3, -9, 15, 47, 11, 0, 1, 5, 2, -28, 4, -2, -18, 1, -21, -14, 9, 4, 2, -2, 22, 6, 0, -5, 0, -1,
    -22, -11, -3, -6, 1, -13, -6, 5, 1, 0, -2, 15, 25, 1, 0, 0, -29, -19, 1, -33, -20, 31, -17, 18,
    16, -22, -18, -15, -20, -10, 0, 34, -36, 0, -10, -52, -43, 36, -40, -8, -18, -35, -7, -10, -36,
    -50, -27, -16, -33, -35, -26, 142, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, -22, 0, -5, -28, -27, 51, -40, 22, -7, -21, 0, -2, -14, -31, -17, -18, -19, -19, -11, 41, 0,
    78, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int ktup;
int mat_avscore;
int matrix[32][32];
int max_aa;
char** names;
int nseqs;
double prot_gap_extend;
double prot_gap_open;
int prot_ktup;
double prot_pw_ge_penalty;
double prot_pw_go_penalty;
int prot_signif;
int prot_window;
double pw_ge_penalty;
double pw_go_penalty;
char** seq_array;
int* seq_output;
int* seqlen_array;
int signif;
int window;
/*Function: del, ID: 22*/
void del(int k, int* print_ptr, int* last_print, int* displ)
{
    /*del:22*/
    /*CompoundStmt:343*/
    if (*last_print < 0)
        *last_print = displ[(*print_ptr) - 1] -= k;
    else
        *last_print = displ[(*print_ptr)++] = -k;
}
/*Function: add, ID: 23*/
void add(int v, int* print_ptr, int* last_print, int* displ)
{
    /*add:23*/
    /*CompoundStmt:361*/
    if (*last_print < 0) {
        displ[(*print_ptr) - 1] = v;
        displ[(*print_ptr)++] = *last_print;
    } else {
        *last_print = displ[(*print_ptr)++] = v;
    }
}
/*Function: calc_score, ID: 24*/
int calc_score(int iat, int jat, int v1, int v2, int seq1, int seq2)
{
    /*calc_score:24*/
    /*CompoundStmt:381*/
    int i, j, ipos, jpos;
    ipos = v1 + iat;
    jpos = v2 + jat;
    i = seq_array[seq1][ipos];
    j = seq_array[seq2][jpos];
    return (matrix[i][j]);
}
/*Function: get_matrix, ID: 25*/
int get_matrix(int* matptr, int* xref, int scale)
{
    /*get_matrix:25*/
    /*CompoundStmt:396*/
    int gg_score = 0;
    int gr_score = 0;
    int i, j, k, ti, tj, ix;
    int av1, av2, av3, min, max, maxres;
    for (i = 0; i <= max_aa; i++)
        for (j = 0; j <= max_aa; j++)
            matrix[i][j] = 0;
    ix = 0;
    maxres = 0;
    for (i = 0; i <= max_aa; i++) {
        ti = xref[i];
        for (j = 0; j <= i; j++) {
            tj = xref[j];
            if ((ti != -1) && (tj != -1)) {
                k = matptr[ix];
                if (ti == tj) {
                    matrix[ti][ti] = k * scale;
                    maxres++;
                } else {
                    matrix[ti][tj] = k * scale;
                    matrix[tj][ti] = k * scale;
                }
                ix++;
            }
        }
    }
    maxres--;
    av1 = av2 = av3 = 0;
    for (i = 0; i <= max_aa; i++) {
        for (j = 0; j <= i; j++) {
            av1 += matrix[i][j];
            if (i == j)
                av2 += matrix[i][j];
            else
                av3 += matrix[i][j];
        }
    }
    av1 /= (maxres * maxres) / 2;
    av2 /= maxres;
    av3 /= (int)(((double)(maxres * maxres - maxres)) / 2);
    mat_avscore = -av3;
    min = max = matrix[0][0];
    for (i = 0; i <= max_aa; i++)
        for (j = 1; j <= i; j++) {
            if (matrix[i][j] < min)
                min = matrix[i][j];
            if (matrix[i][j] > max)
                max = matrix[i][j];
        }
    for (i = 0; i < gap_pos1; i++) {
        matrix[i][gap_pos1] = gr_score;
        matrix[gap_pos1][i] = gr_score;
        matrix[i][gap_pos2] = gr_score;
        matrix[gap_pos2][i] = gr_score;
    }
    matrix[gap_pos1][gap_pos1] = gg_score;
    matrix[gap_pos2][gap_pos2] = gg_score;
    matrix[gap_pos2][gap_pos1] = gg_score;
    matrix[gap_pos1][gap_pos2] = gg_score;
    maxres += 2;
    return (maxres);
}
/*Function: forward_pass, ID: 26*/
void forward_pass(
    char* ia, char* ib, int n, int m, int* se1, int* se2, int* maxscore, int g, int gh)
{
    /*forward_pass:26*/
    /*CompoundStmt:546*/
    int i, j, f, p, t, hh;
    int HH[5000];
    int DD[5000];
    *maxscore = 0;
    *se1 = *se2 = 0;
    for (i = 0; i <= m; i++) {
        HH[i] = 0;
        DD[i] = -g;
    }
    for (i = 1; i <= n; i++) {
        hh = p = 0;
        f = -g;
        for (j = 1; j <= m; j++) {
            f -= gh;
            t = hh - g - gh;
            if (f < t)
                f = t;
            DD[j] -= gh;
            t = HH[j] - g - gh;
            if (DD[j] < t)
                DD[j] = t;
            hh = p + matrix[(int)ia[i]][(int)ib[j]];
            if (hh < f)
                hh = f;
            if (hh < DD[j])
                hh = DD[j];
            if (hh < 0)
                hh = 0;
            p = HH[j];
            HH[j] = hh;
            if (hh > *maxscore) {
                *maxscore = hh;
                *se1 = i;
                *se2 = j;
            }
        }
    }
}
/*Function: reverse_pass, ID: 27*/
void reverse_pass(
    char* ia, char* ib, int se1, int se2, int* sb1, int* sb2, int maxscore, int g, int gh)
{
    /*reverse_pass:27*/
    /*CompoundStmt:633*/
    int i, j, f, p, t, hh, cost;
    int HH[5000];
    int DD[5000];
    cost = 0;
    *sb1 = *sb2 = 1;
    for (i = se2; i > 0; i--) {
        HH[i] = -1;
        DD[i] = -1;
    }
    for (i = se1; i > 0; i--) {
        hh = f = -1;
        if (i == se1)
            p = 0;
        else
            p = -1;
        for (j = se2; j > 0; j--) {
            f -= gh;
            t = hh - g - gh;
            if (f < t)
                f = t;
            DD[j] -= gh;
            t = HH[j] - g - gh;
            if (DD[j] < t)
                DD[j] = t;
            hh = p + matrix[(int)ia[i]][(int)ib[j]];
            if (hh < f)
                hh = f;
            if (hh < DD[j])
                hh = DD[j];
            p = HH[j];
            HH[j] = hh;
            if (hh > cost) {
                cost = hh;
                *sb1 = i;
                *sb2 = j;
                if (cost >= maxscore)
                    break;
            }
        }
        if (cost >= maxscore)
            break;
    }
}
/*Function: diff, ID: 28*/
int diff(int A, int B, int M, int N, int tb, int te, int* print_ptr, int* last_print, int* displ,
    int seq1, int seq2, int g, int gh)
{
    /*diff:28*/
    /*CompoundStmt:729*/
    int i, j, f, e, s, t, hh;
    int midi, midj, midh, type;
    int HH[5000];
    int DD[5000];
    int RR[5000];
    int SS[5000];
    if (N <= 0) {
        if (M > 0)
            del(M, print_ptr, last_print, displ);
        return (-(int)((M) <= 0 ? 0 : tb + gh * (M)));
    }
    if (M <= 1) {
        if (M <= 0) {
            add(N, print_ptr, last_print, displ);
            return (-(int)((N) <= 0 ? 0 : tb + gh * (N)));
        }
        midh = -(tb + gh) - ((N) <= 0 ? 0 : te + gh * (N));
        hh = -(te + gh) - ((N) <= 0 ? 0 : tb + gh * (N));
        if (hh > midh)
            midh = hh;
        midj = 0;
        for (j = 1; j <= N; j++) {
            hh = calc_score(1, j, A, B, seq1, seq2) - ((N - j) <= 0 ? 0 : te + gh * (N - j))
                - ((j - 1) <= 0 ? 0 : tb + gh * (j - 1));
            if (hh > midh) {
                midh = hh;
                midj = j;
            }
        }
        if (midj == 0) {
            del(1, print_ptr, last_print, displ);
            add(N, print_ptr, last_print, displ);
        } else {
            if (midj > 1)
                add(midj - 1, print_ptr, last_print, displ);
            displ[(*print_ptr)++] = *last_print = 0;
            if (midj < N)
                add(N - midj, print_ptr, last_print, displ);
        }
        return midh;
    }
    midi = M / 2;
    HH[0] = 0.;
    t = -tb;
    for (j = 1; j <= N; j++) {
        HH[j] = t = t - gh;
        DD[j] = t - g;
    }
    t = -tb;
    for (i = 1; i <= midi; i++) {
        s = HH[0];
        HH[0] = hh = t = t - gh;
        f = t - g;
        for (j = 1; j <= N; j++) {
            if ((hh = hh - g - gh) > (f = f - gh))
                f = hh;
            if ((hh = HH[j] - g - gh) > (e = DD[j] - gh))
                e = hh;
            hh = s + calc_score(i, j, A, B, seq1, seq2);
            if (f > hh)
                hh = f;
            if (e > hh)
                hh = e;
            s = HH[j];
            HH[j] = hh;
            DD[j] = e;
        }
    }
    DD[0] = HH[0];
    RR[N] = 0;
    t = -te;
    for (j = N - 1; j >= 0; j--) {
        RR[j] = t = t - gh;
        SS[j] = t - g;
    }
    t = -te;
    for (i = M - 1; i >= midi; i--) {
        s = RR[N];
        RR[N] = hh = t = t - gh;
        f = t - g;
        for (j = N - 1; j >= 0; j--) {
            if ((hh = hh - g - gh) > (f = f - gh))
                f = hh;
            if ((hh = RR[j] - g - gh) > (e = SS[j] - gh))
                e = hh;
            hh = s + calc_score(i + 1, j + 1, A, B, seq1, seq2);
            if (f > hh)
                hh = f;
            if (e > hh)
                hh = e;
            s = RR[j];
            RR[j] = hh;
            SS[j] = e;
        }
    }
    SS[N] = RR[N];
    midh = HH[0] + RR[0];
    midj = 0;
    type = 1;
    for (j = 0; j <= N; j++) {
        hh = HH[j] + RR[j];
        if (hh >= midh)
            if (hh > midh || (HH[j] != DD[j] && RR[j] == SS[j])) {
                midh = hh;
                midj = j;
            }
    }
    for (j = N; j >= 0; j--) {
        hh = DD[j] + SS[j] + g;
        if (hh > midh) {
            midh = hh;
            midj = j;
            type = 2;
        }
    }
    if (type == 1) {
        diff(A, B, midi, midj, tb, g, print_ptr, last_print, displ, seq1, seq2, g, gh);
        diff(A + midi, B + midj, M - midi, N - midj, g, te, print_ptr, last_print, displ, seq1,
            seq2, g, gh);
    } else {
        diff(A, B, midi - 1, midj, tb, 0., print_ptr, last_print, displ, seq1, seq2, g, gh);
        del(2, print_ptr, last_print, displ);
        diff(A + midi + 1, B + midj, M - midi - 1, N - midj, 0., te, print_ptr, last_print, displ,
            seq1, seq2, g, gh);
    }
    return midh;
}
/*Function: tracepath, ID: 29*/
double tracepath(int tsb1, int tsb2, int* print_ptr, int* displ, int seq1, int seq2)
{
    /*tracepath:29*/
    /*CompoundStmt:1034*/
    int i, k;
    int i1 = tsb1;
    int i2 = tsb2;
    int pos = 0;
    int count = 0;
    for (i = 1; i <= *print_ptr - 1; ++i) {
        if (displ[i] == 0) {
            char c1 = seq_array[seq1][i1];
            char c2 = seq_array[seq2][i2];
            if ((c1 != gap_pos1) && (c1 != gap_pos2) && (c1 == c2))
                count++;
            ++i1;
            ++i2;
            ++pos;
        } else if ((k = displ[i]) > 0) {
            i2 += k;
            pos += k;
        } else {
            i1 -= k;
            pos -= k;
        }
    }
    return (100. * (double)count);
}
/*Function: pairalign, ID: 30*/
int pairalign()
{
    /*pairalign:30*/
    /*CompoundStmt:1076*/
    int i, n, m, si, sj;
    (void)i;
    (void)n;
    (void)m;
    (void)si;
    (void)sj;
    int len1, len2, maxres;
    (void)len1;
    (void)len2;
    double gg, mm_score;
    (void)gg;
    (void)mm_score;
    int *mat_xref, *matptr;
    matptr = gon250mt;
    mat_xref = def_aa_xref;
    maxres = get_matrix(matptr, mat_xref, 10);
    if (maxres == 0)
        return (-1);
    {
        /*CompoundStmt:1099*/
        if (bots_verbose_mode >= BOTS_VERBOSE_DEFAULT) {
            fprintf(stdout, "Start aligning ");
        }
    }
    if (affinMaskRes) {
        myDARTSRuntime->run(
            launch<TP1104>(ompNumThreads * DARTS_CODELETS_MULT, 0, RuntimeFinalCodelet));
    }
    {
        /*CompoundStmt:1252*/
        if (bots_verbose_mode >= BOTS_VERBOSE_DEFAULT) {
            fprintf(stdout, " completed!\n");
        }
    }
    return 0;
}
/*Function: pairalign_seq, ID: 31*/
int pairalign_seq()
{
    /*pairalign_seq:31*/
    /*CompoundStmt:1258*/
    int i, n, m, si, sj;
    int len1, len2, maxres;
    double gg, mm_score;
    int *mat_xref, *matptr;
    matptr = gon250mt;
    mat_xref = def_aa_xref;
    maxres = get_matrix(matptr, mat_xref, 10);
    if (maxres == 0)
        return (-1);
    for (si = 0; si < nseqs; si++) {
        n = seqlen_array[si + 1];
        for (i = 1, len1 = 0; i <= n; i++) {
            char c = seq_array[si + 1][i];
            if ((c != gap_pos1) && (c != gap_pos2))
                len1++;
        }
        for (sj = si + 1; sj < nseqs; sj++) {
            m = seqlen_array[sj + 1];
            if (n == 0 || m == 0) {
                seq_output[si * nseqs + sj] = (int)1.;
            } else {
                int se1, se2, sb1, sb2, maxscore, seq1, seq2, g, gh;
                int displ[10001];
                int print_ptr, last_print;
                for (i = 1, len2 = 0; i <= m; i++) {
                    char c = seq_array[sj + 1][i];
                    if ((c != gap_pos1) && (c != gap_pos2))
                        len2++;
                }
                if (dnaFlag == 1) {
                    g = (int)(2 * 100 * pw_go_penalty * gap_open_scale);
                    gh = (int)(100 * pw_ge_penalty * gap_extend_scale);
                } else {
                    gg = pw_go_penalty + log((double)((n) < (m) ? (n) : (m)));
                    g = (int)((mat_avscore <= 0) ? (2 * 100 * gg)
                                                 : (2 * mat_avscore * gg * gap_open_scale));
                    gh = (int)(100 * pw_ge_penalty);
                }
                seq1 = si + 1;
                seq2 = sj + 1;
                forward_pass(
                    &seq_array[seq1][0], &seq_array[seq2][0], n, m, &se1, &se2, &maxscore, g, gh);
                reverse_pass(&seq_array[seq1][0], &seq_array[seq2][0], se1, se2, &sb1, &sb2,
                    maxscore, g, gh);
                print_ptr = 1;
                last_print = 0;
                diff(sb1 - 1, sb2 - 1, se1 - sb1 + 1, se2 - sb2 + 1, 0, 0, &print_ptr, &last_print,
                    displ, seq1, seq2, g, gh);
                mm_score = tracepath(sb1, sb2, &print_ptr, displ, seq1, seq2);
                if (len1 == 0 || len2 == 0)
                    mm_score = 0.;
                else
                    mm_score /= (double)((len1) < (len2) ? (len1) : (len2));
                seq_output[si * nseqs + sj] = (int)mm_score;
            }
        }
    }
    return 0;
}
/*Function: init_matrix, ID: 32*/
void init_matrix()
{
    /*init_matrix:32*/
    /*CompoundStmt:1418*/
    int i, j;
    char c1, c2;
    gap_pos1 = 32 - 2;
    gap_pos2 = 32 - 1;
    max_aa = strlen(amino_acid_codes) - 2;
    for (i = 0; i < 32; i++)
        def_aa_xref[i] = -1;
    for (i = 0; (c1 = amino_acid_order[i]); i++)
        for (j = 0; (c2 = amino_acid_codes[j]); j++)
            if (c1 == c2) {
                def_aa_xref[i] = j;
                break;
            }
}
/*Function: pairalign_init, ID: 33*/
void pairalign_init(char* filename)
{
    /*pairalign_init:33*/
    /*CompoundStmt:1453*/
    int i;
    if (!filename || !filename[0]) {
        bots_error(0, "Please specify an input file with the -f option\n");
    }
    init_matrix();
    nseqs = readseqs(filename);
    {
        /*CompoundStmt:1462*/
        if (bots_verbose_mode >= BOTS_VERBOSE_DEFAULT) {
            fprintf(stdout, "Multiple Pairwise Alignment (%d sequences)\n", nseqs);
        }
    }
    for (i = 1; i <= nseqs; i++)
        ;
    if (clustalw == 1) {
        gap_open_scale = 0.66669999999999996;
        gap_extend_scale = 0.751;
    } else {
        gap_open_scale = 1.;
        gap_extend_scale = 1.;
    }
    if (dnaFlag == 1) {
        ktup = 2;
        window = 4;
        signif = 4;
        gap_open = 15.;
        gap_extend = 6.6600000000000001;
        pw_go_penalty = 15.;
        pw_ge_penalty = 6.6600000000000001;
    } else {
        ktup = 1;
        window = 5;
        signif = 5;
        gap_open = 10.;
        gap_extend = 0.20000000000000001;
        pw_go_penalty = 10.;
        pw_ge_penalty = 0.10000000000000001;
    }
}
/*Function: align_init, ID: 34*/
void align_init()
{
    /*align_init:34*/
    /*CompoundStmt:1509*/
    int i, j;
    bench_output = (int*)malloc(sizeof(int) * nseqs * nseqs);
    for (i = 0; i < nseqs; i++)
        for (j = 0; j < nseqs; j++)
            bench_output[i * nseqs + j] = 0;
}
/*Function: align, ID: 35*/
void align()
{
    /*align:35*/
    /*CompoundStmt:1529*/
    pairalign();
}
/*Function: align_seq_init, ID: 36*/
void align_seq_init()
{
    /*align_seq_init:36*/
    /*CompoundStmt:1531*/
    int i, j;
    seq_output = (int*)malloc(sizeof(int) * nseqs * nseqs);
    bench_output = (int*)malloc(sizeof(int) * nseqs * nseqs);
    for (i = 0; i < nseqs; i++)
        for (j = 0; j < nseqs; j++)
            seq_output[i * nseqs + j] = 0;
}
/*Function: align_seq, ID: 37*/
void align_seq()
{
    /*align_seq:37*/
    /*CompoundStmt:1557*/
    pairalign_seq();
}
/*Function: align_end, ID: 38*/
void align_end()
{
    /*align_end:38*/
    /*CompoundStmt:1559*/
    int i, j;
    for (i = 0; i < nseqs; i++)
        for (j = 0; j < nseqs; j++)
            if (bench_output[i * nseqs + j] != 0)
                ;
}
/*Function: align_verify, ID: 39*/
int align_verify()
{
    /*align_verify:39*/
    /*CompoundStmt:1571*/
    int i, j;
    int result = 1;
    for (i = 0; i < nseqs; i++) {
        for (j = 0; j < nseqs; j++) {
            if (bench_output[i * nseqs + j] != seq_output[i * nseqs + j]) {
                {
                    if (bots_verbose_mode >= BOTS_VERBOSE_DEFAULT) {
                        fprintf(stdout,
                            "Error: Optimized prot. (%3d:%3d)=%5d Sequential prot. (%3d:%3d)=%5d\n",
                            i + 1, j + 1, (int)bench_output[i * nseqs + j], i + 1, j + 1,
                            (int)seq_output[i * nseqs + j]);
                    }
                };
                result = 2;
            }
        }
    }
    return result;
}
/*TP1104: OMPParallelDirective*/
void TP1104::_barrierCodelets1104::fire(void)
{
    TP1104* myTP = static_cast<TP1104*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
void TP1104::_checkInCodelets1106::fire(void)
{
    /*Select the thread executing OMPSingleDirective 1106*/
    if (!__sync_val_compare_and_swap(&(myTP->TP1106_alreadyLaunched), 0, 1)) {
        invoke<TP1106>(myTP, 1, this->getID(), myTP);
    } else {
        myTP->barrierCodelets1106[0].decDep();
    }
}
void TP1104::_barrierCodelets1106::fire(void)
{
    TP1104* myTP = static_cast<TP1104*>(myTP_);
    myTP->TPParent->barrierCodelets1104[0].setDep(0);
    myTP->add(&(myTP->TPParent->barrierCodelets1104[0]));
}
TP1104::TP1104(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , TP1106Ptr(nullptr)
    , TP1106_alreadyLaunched(0)
    , task1148Inputs(new _task1148Inputs*[this->numThreads])
    , barrierCodelets1104(new _barrierCodelets1104[1])
    , checkInCodelets1106(new _checkInCodelets1106[this->numThreads])
    , barrierCodelets1106(new _barrierCodelets1106[1])
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets1104[0] = _barrierCodelets1104(ompNumThreads, ompNumThreads, this, 0);
    barrierCodelets1106[0] = _barrierCodelets1106(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets1106* checkInCodelets1106Ptr = (this->checkInCodelets1106);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets1106Ptr) = _checkInCodelets1106(1, 1, this, codeletCounter);
        (*checkInCodelets1106Ptr).decDep();
        checkInCodelets1106Ptr++;
    }
}
TP1104::~TP1104()
{
    delete[] task1148Inputs;
    delete[] barrierCodelets1104;
    delete[] barrierCodelets1106;
    delete[] checkInCodelets1106;
}
/*TP1106: OMPSingleDirective*/
void TP1106::_checkInCodelets1108::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 1108: BinaryOperator*/
    (this->inputsTPParent->si_darts1106) = 0;

    /*printing node 1109: BinaryOperator*/
    /*Print the code for a condition node in a complex loop stmt */
    if ((this->inputsTPParent->si_darts1106) < nseqs) {
        /*Signal the first codelet in the loop*/
        myTP->checkInCodelets1107.decDep();
        return;
    }
    /*Signal the codelet after the loop from the end's condional node.*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets1106[0].decDep();
}
void TP1106::_checkInCodelets1107::fire(void)
{

    /*printing node 1107: ForStmt*/
    bool haveToLaunch = __sync_bool_compare_and_swap(&(myTP->controlTPParent->TP1107_LoopCounter),
        myTP->controlTPParent->TP1107_LoopCounterPerThread[this->getID()],
        myTP->controlTPParent->TP1107_LoopCounterPerThread[this->getID()] + 1);
    unsigned int iterIdx = myTP->controlTPParent->TP1107_LoopCounterPerThread[this->getID()];
    if (haveToLaunch) {
        this->resetCodelet();
        myTP->controlTPParent->TP1107PtrVec.push_back(nullptr);
        myTP->controlTPParent->TP1107_LoopCounterPerThread[this->getID()] += 1;
        invoke<TP1107>(myTP, myTP->numThreads, this->getID(), myTP, myTP->inputsTPParent,
            &(myTP->controlTPParent->TP1107PtrVec.back()));
    } else {
        if (myTP->controlTPParent->TP1107PtrVec.size() == 0) {
            this->resetCodelet();
            this->decDep();
            return;
        } else if (myTP->controlTPParent->TP1107PtrVec.size() < (iterIdx + 1)) {
            this->resetCodelet();
            this->decDep();
            return;
        } else if (myTP->controlTPParent->TP1107PtrVec[iterIdx] == nullptr) {
            this->resetCodelet();
            this->decDep();
            return;
        } else {
            this->resetCodelet();
#if USE_SPIN_CODELETS == 0
            myTP->controlTPParent->TP1107PtrVec[iterIdx]->firstCodelet[this->getID()].decDep();
#else
            myTP->controlTPParent->TP1107PtrVec[iterIdx]->availableCodelets[this->getID()] = 1;
#endif
            myTP->controlTPParent->TP1107_LoopCounterPerThread[this->getID()] += 1;
        }
    }
}
void TP1106::_checkInCodelets1110::fire(void)
{

    /*printing node 1110: UnaryOperator*/
    (this->inputsTPParent->si_darts1106)++;

    /*printing node 1607: BinaryOperator*/
    /*Print the code for a condition node in a complex loop stmt */
    if ((this->inputsTPParent->si_darts1106) < nseqs) {
        this->resetCodelet();
        /*Signal the first codelet in the loop*/
        myTP->checkInCodelets1107.decDep();
        return;
    }
    /*Signal the codelet after the loop from the condtional node.*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets1106[0].decDep();
}
TP1106::TP1106(int in_numThreads, int in_mainCodeletID, TP1104* in_TPParent)
    : ompOMPSingleDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , TP1107_LoopCounter(0)
    , TP1107_LoopCounterPerThread(new unsigned int[this->numThreads])
    , task1148Inputs(new _task1148Inputs*[this->numThreads])
    , checkInCodelets1108(1, 1, this, this->mainCodeletID)
    , checkInCodelets1107(1, 1, this, this->mainCodeletID)
    , checkInCodelets1110(1, 1, this, this->mainCodeletID)
{
    memset((void*)TP1107_LoopCounterPerThread, 0, this->numThreads * sizeof(unsigned int));
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets1108.decDep();
}
TP1106::~TP1106()
{
    delete[] TP1107_LoopCounterPerThread;
    delete[] task1148Inputs;
}
/*TP1107: ForStmt*/
void TP1107::_checkInCodelets1112::fire(void)
{

    /*printing node 1112: BinaryOperator*/
    (this->inputsTPParent->n_darts1106) = seqlen_array[(this->inputsTPParent->si_darts1106) + 1];

    /*printing node 1115: BinaryOperator*/
    (this->inputsTPParent->len1_darts1106) = 0;

    /*printing node 1116: ForStmt*/
    {
        int* i = &(this->inputsTPParent->i_darts1106);
        (void)i /*OMP_PRIVATE*/;
        int* len1 = &(this->inputsTPParent->len1_darts1106);
        (void)len1 /*OMP_PRIVATE*/;
        int* n = &(this->inputsTPParent->n_darts1106);
        (void)n /*OMP_PRIVATE*/;
        int* si = &(this->inputsTPParent->si_darts1106);
        (void)si /*OMP_PRIVATE*/;
        char* c = &(this->inputsTPParent->c_darts1106);
        (void)c /*PRIVATE*/;
        /*Loop's init*/
        (this->inputsTPParent->i_darts1106) = 1;
        int i_darts_counter_temp1106 = (this->inputsTPParent->i_darts1106);
        for (; (i_darts_counter_temp1106) <= (*n); (i_darts_counter_temp1106)++) {
            *c = seq_array[(*si) + 1][(i_darts_counter_temp1106)];
            if (((*c) != gap_pos1) && ((*c) != gap_pos2)) {
                (*len1)++;
            }
        }
        (this->inputsTPParent->i_darts1106) = i_darts_counter_temp1106;
    }

    /*printing node 1130: BinaryOperator*/
    (this->inputsTPParent->sj_darts1106) = (this->inputsTPParent->si_darts1106) + 1;

    /*printing node 1132: BinaryOperator*/
    /*Print the code for a condition node in a complex loop stmt */
    if ((this->inputsTPParent->sj_darts1106) < nseqs) {
        /*Signal the first codelet in the loop*/
        myTP->checkInCodelets1129.decDep();
        return;
    }
    /*Signal the codelet after the loop from the end's condional node.*/
    /*The node is the last one in a complex loop, so signal the inc node*/
    /*Find the TP to which the nextRegion belongs*/
    myTP->controlTPParent->TPParent->checkInCodelets1110.decDep();
}
void TP1107::_checkInCodelets1129::fire(void)
{

    /*printing node 1129: ForStmt*/
    bool haveToLaunch = __sync_bool_compare_and_swap(&(myTP->controlTPParent->TP1129_LoopCounter),
        myTP->controlTPParent->TP1129_LoopCounterPerThread[this->getID()],
        myTP->controlTPParent->TP1129_LoopCounterPerThread[this->getID()] + 1);
    unsigned int iterIdx = myTP->controlTPParent->TP1129_LoopCounterPerThread[this->getID()];
    if (haveToLaunch) {
        this->resetCodelet();
        myTP->controlTPParent->TP1129PtrVec.push_back(nullptr);
        myTP->controlTPParent->TP1129_LoopCounterPerThread[this->getID()] += 1;
        invoke<TP1129>(myTP, myTP->numThreads, this->getID(), myTP, myTP->inputsTPParent,
            &(myTP->controlTPParent->TP1129PtrVec.back()));
    } else {
        if (myTP->controlTPParent->TP1129PtrVec.size() == 0) {
            this->resetCodelet();
            this->decDep();
            return;
        } else if (myTP->controlTPParent->TP1129PtrVec.size() < (iterIdx + 1)) {
            this->resetCodelet();
            this->decDep();
            return;
        } else if (myTP->controlTPParent->TP1129PtrVec[iterIdx] == nullptr) {
            this->resetCodelet();
            this->decDep();
            return;
        } else {
            this->resetCodelet();
#if USE_SPIN_CODELETS == 0
            myTP->controlTPParent->TP1129PtrVec[iterIdx]->firstCodelet[this->getID()].decDep();
#else
            myTP->controlTPParent->TP1129PtrVec[iterIdx]->availableCodelets[this->getID()] = 1;
#endif
            myTP->controlTPParent->TP1129_LoopCounterPerThread[this->getID()] += 1;
        }
    }
}
void TP1107::_checkInCodelets1133::fire(void)
{

    /*printing node 1133: UnaryOperator*/
    (this->inputsTPParent->sj_darts1106)++;

    /*printing node 1606: BinaryOperator*/
    /*Print the code for a condition node in a complex loop stmt */
    if ((this->inputsTPParent->sj_darts1106) < nseqs) {
        this->resetCodelet();
        /*Signal the first codelet in the loop*/
        myTP->checkInCodelets1129.decDep();
        return;
    }
    /*Signal the codelet after the loop from the condtional node.*/
    /*The node is the last one in a complex loop, so signal the inc node*/
    /*Find the TP to which the nextRegion belongs*/
    myTP->controlTPParent->TPParent->checkInCodelets1110.decDep();
}
TP1107::TP1107(int in_numThreads, int in_mainCodeletID, TP1106* in_TPParent,
    TP1106* in_inputsTPParent, TP1107** in_ptrToThisTP)
    : ompTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(in_inputsTPParent)
    , ptrToThisTP(in_ptrToThisTP)
    , TP1129_LoopCounter(0)
    , TP1129_LoopCounterPerThread(new unsigned int[this->numThreads])
    , checkInCodelets1112(1, 1, this, this->mainCodeletID)
    , checkInCodelets1129(1, 1, this, this->mainCodeletID)
    , checkInCodelets1133(1, 1, this, this->mainCodeletID)
{
    memset((void*)TP1129_LoopCounterPerThread, 0, this->numThreads * sizeof(unsigned int));
    /*Initialize Codelets*/
    checkInCodelets1112.decDep();
    *(this->ptrToThisTP) = this;
#if USE_SPIN_CODELETS == 0
    this->firstCodelet[this->getID()].decDep();
#else
    this->availableCodelets[this->getID()] = 1;
#endif
}
TP1107::~TP1107() { delete[] TP1129_LoopCounterPerThread; }
/*TP1129: ForStmt*/
void TP1129::_checkInCodelets1135::fire(void)
{

    /*printing node 1135: BinaryOperator*/
    (this->inputsTPParent->m_darts1106) = seqlen_array[(this->inputsTPParent->sj_darts1106) + 1];
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 1135 nextRegion: 1138 */
    myTP->controlTPParent->checkInCodelets1138.decDep();
}
void TP1129::_checkInCodelets1138::fire(void)
{
    /*Printing conditional branch node 1138: inlining: 0*/
    if ((this->inputsTPParent->n_darts1106) == 0 || (this->inputsTPParent->m_darts1106) == 0) {
        myTP->checkInCodelets1141.decDep();
    } else {
        /*elseRegion: OMPTaskDirective 1148*/
        myTP->checkInCodelets1148.decDep();
    }
}
void TP1129::_checkInCodelets1141::fire(void)
{

    /*printing node 1141: BinaryOperator*/
    bench_output[(this->inputsTPParent->si_darts1106) * nseqs
        + (this->inputsTPParent->sj_darts1106)]
        = (int)1.;
    /*Signaling next codelet from last stmt in the codelet*/
    /*The node is the last one in a complex loop, so signal the inc node*/
    /*Find the TP to which the nextRegion belongs*/
    myTP->controlTPParent->TPParent->checkInCodelets1133.decDep();
}
void TP1129::_checkInCodelets1148::fire(void)
{

    /*printing node 1148: OMPTaskDirective*/
    /*syncNode: OMPSingleDirective 1106*/
    /*Find the TP to which the nextRegion belongs*/
    Codelet* nextSyncCodelet
        = &(myTP->controlTPParent->TPParent->TPParent->TPParent->barrierCodelets1106[0]);
    /*Increment sync point's dependency to account for the task to be launched*/
    nextSyncCodelet->incDep();
    /*Encapsulating data for task 1148*/
    _task1148Inputs* task1148Inputs = new _task1148Inputs(&((this->inputsTPParent->i_darts1106)),
        &((this->inputsTPParent->len1_darts1106)), &((this->inputsTPParent->m_darts1106)),
        &((this->inputsTPParent->n_darts1106)), &((this->inputsTPParent->si_darts1106)),
        &((this->inputsTPParent->sj_darts1106)), nextSyncCodelet);
    /*Save the pointer to the recently created task's data*/
    this->inputsTPParent->task1148Inputs[0] = task1148Inputs;
    invoke<TP1148>(myTP, 1, this->getID(), myTP, task1148Inputs);
    /*Signaling next codelet from last stmt in the codelet*/
    /*The node is the last one in a complex loop, so signal the inc node*/
    /*Find the TP to which the nextRegion belongs*/
    myTP->controlTPParent->TPParent->checkInCodelets1133.decDep();
}
TP1129::TP1129(int in_numThreads, int in_mainCodeletID, TP1107* in_TPParent,
    TP1106* in_inputsTPParent, TP1129** in_ptrToThisTP)
    : ompTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(in_inputsTPParent)
    , ptrToThisTP(in_ptrToThisTP)
    , checkInCodelets1135(1, 1, this, this->mainCodeletID)
    , checkInCodelets1138(1, 1, this, this->mainCodeletID)
    , checkInCodelets1141(1, 1, this, this->mainCodeletID)
    , checkInCodelets1148(1, 1, this, this->mainCodeletID)
{
    /*Initialize Codelets*/
    checkInCodelets1135.decDep();
    *(this->ptrToThisTP) = this;
#if USE_SPIN_CODELETS == 0
    this->firstCodelet[this->getID()].decDep();
#else
    this->availableCodelets[this->getID()] = 1;
#endif
}
TP1129::~TP1129() {}
/*TP1148: OMPTaskDirective*/
void TP1148::_checkInCodelets1150::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 1150: DeclStmt*/

    /*printing node 1151: DeclStmt*/

    /*printing node 1154: DeclStmt*/

    /*printing node 1155: BinaryOperator*/
    (this->taskInputs->len2_darts1148) = 0;

    /*printing node 1156: ForStmt*/
    {
        int* i = &(this->taskInputs->i_darts1148);
        (void)i /*OMP_FIRSTPRIVATE*/;
        int* len2 = &(this->taskInputs->len2_darts1148);
        (void)len2 /*OMP_PRIVATE*/;
        int* m = &(this->taskInputs->m_darts1148);
        (void)m /*OMP_FIRSTPRIVATE*/;
        int* sj = &(this->taskInputs->sj_darts1148);
        (void)sj /*OMP_FIRSTPRIVATE*/;
        char* c = &(this->taskInputs->c_darts1148);
        (void)c /*PRIVATE*/;
        /*Loop's init*/
        (this->taskInputs->i_darts1148) = 1;
        int i_darts_counter_temp1148 = (this->taskInputs->i_darts1148);
        for (; (i_darts_counter_temp1148) <= (*m); (i_darts_counter_temp1148)++) {
            *c = seq_array[(*sj) + 1][(i_darts_counter_temp1148)];
            if (((*c) != gap_pos1) && ((*c) != gap_pos2)) {
                (*len2)++;
            }
        }
        (this->taskInputs->i_darts1148) = i_darts_counter_temp1148;
    }

    /*printing node 1169: IfStmt*/
    if (dnaFlag == 1) {
        (this->taskInputs->g_darts1148) = (int)(2 * 100 * pw_go_penalty * gap_open_scale);
        (this->taskInputs->gh_darts1148) = (int)(100 * pw_ge_penalty * gap_extend_scale);
    } else {
        (this->taskInputs->gg_darts1148) = pw_go_penalty
            + log((double)(((this->taskInputs->n_darts1148)) < ((this->taskInputs->m_darts1148))
                      ? ((this->taskInputs->n_darts1148))
                      : ((this->taskInputs->m_darts1148))));
        (this->taskInputs->g_darts1148) = (int)((mat_avscore <= 0)
                ? (2 * 100 * (this->taskInputs->gg_darts1148))
                : (2 * mat_avscore * (this->taskInputs->gg_darts1148) * gap_open_scale));
        (this->taskInputs->gh_darts1148) = (int)(100 * pw_ge_penalty);
    }

    /*printing node 1200: BinaryOperator*/
    (this->taskInputs->seq1_darts1148) = (this->taskInputs->si_darts1148) + 1;

    /*printing node 1202: BinaryOperator*/
    (this->taskInputs->seq2_darts1148) = (this->taskInputs->sj_darts1148) + 1;

    /*printing node 1204: CallExpr*/
    forward_pass(&seq_array[(this->taskInputs->seq1_darts1148)][0],
        &seq_array[(this->taskInputs->seq2_darts1148)][0], (this->taskInputs->n_darts1148),
        (this->taskInputs->m_darts1148), &(this->taskInputs->se1_darts1148),
        &(this->taskInputs->se2_darts1148), &(this->taskInputs->maxscore_darts1148),
        (this->taskInputs->g_darts1148), (this->taskInputs->gh_darts1148));

    /*printing node 1214: CallExpr*/
    reverse_pass(&seq_array[(this->taskInputs->seq1_darts1148)][0],
        &seq_array[(this->taskInputs->seq2_darts1148)][0], (this->taskInputs->se1_darts1148),
        (this->taskInputs->se2_darts1148), &(this->taskInputs->sb1_darts1148),
        &(this->taskInputs->sb2_darts1148), (this->taskInputs->maxscore_darts1148),
        (this->taskInputs->g_darts1148), (this->taskInputs->gh_darts1148));

    /*printing node 1223: BinaryOperator*/
    (this->taskInputs->print_ptr_darts1148) = 1;

    /*printing node 1224: BinaryOperator*/
    (this->taskInputs->last_print_darts1148) = 0;

    /*printing node 1225: CallExpr*/
    diff((this->taskInputs->sb1_darts1148) - 1, (this->taskInputs->sb2_darts1148) - 1,
        (this->taskInputs->se1_darts1148) - (this->taskInputs->sb1_darts1148) + 1,
        (this->taskInputs->se2_darts1148) - (this->taskInputs->sb2_darts1148) + 1, 0, 0,
        &(this->taskInputs->print_ptr_darts1148), &(this->taskInputs->last_print_darts1148),
        (this->taskInputs->displ_darts1148), (this->taskInputs->seq1_darts1148),
        (this->taskInputs->seq2_darts1148), (this->taskInputs->g_darts1148),
        (this->taskInputs->gh_darts1148));

    /*printing node 1234: BinaryOperator*/
    (this->taskInputs->mm_score_darts1148)
        = tracepath((this->taskInputs->sb1_darts1148), (this->taskInputs->sb2_darts1148),
            &(this->taskInputs->print_ptr_darts1148), (this->taskInputs->displ_darts1148),
            (this->taskInputs->seq1_darts1148), (this->taskInputs->seq2_darts1148));

    /*printing node 1237: IfStmt*/
    if ((this->taskInputs->len1_darts1148) == 0 || (this->taskInputs->len2_darts1148) == 0) {
        (this->taskInputs->mm_score_darts1148) = 0.;
    } else {
        (this->taskInputs->mm_score_darts1148)
            /= (double)(((this->taskInputs->len1_darts1148)) < ((this->taskInputs->len2_darts1148))
                    ? ((this->taskInputs->len1_darts1148))
                    : ((this->taskInputs->len2_darts1148)));
    }

    /*printing node 1247: BinaryOperator*/
    bench_output[(this->taskInputs->si_darts1148) * nseqs + (this->taskInputs->sj_darts1148)]
        = (int)(this->taskInputs->mm_score_darts1148);
    /*Signaling next codelet from last stmt in the codelet*/
    /*Mark this task as completed*/
    myTP->controlTPParent->task1148Inputs->taskCompleted = true;
    /*Signal the task's synchronization point*/
    myTP->controlTPParent->task1148Inputs->nextSyncCodelet->decDep();
}
TP1148::TP1148(int in_numThreads, int in_mainCodeletID, TP1129* in_TPParent,
    _task1148Inputs* in_task1148Inputs)
    : ompOMPTaskDirectiveTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , task1148Inputs(in_task1148Inputs)
    , checkInCodelets1150(1, 1, this, this->mainCodeletID)
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    checkInCodelets1150.decDep();
}
TP1148::~TP1148() { delete (task1148Inputs); }
