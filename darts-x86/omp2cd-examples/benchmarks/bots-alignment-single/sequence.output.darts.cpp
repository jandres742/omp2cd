#include "sequence.output.darts.h"
using namespace darts;
using namespace std;
char line_darts19[513];
/*Function: strlcpy, ID: 15*/
unsigned long strlcpy(char* dst, const char* src, size_t siz)
{
    /*strlcpy:15*/
    /*CompoundStmt:1609*/
    char* d = dst;
    const char* s = src;
    size_t n = siz;
    if (n != 0) {
        while (--n != 0) {
            if ((*d++ = *s++) == '\x00')
                break;
        }
    }
    if (n == 0) {
        if (siz != 0)
            *d = '\x00';
        while (*s++)
            ;
    }
    return (s - src - 1);
}
/*Function: fill_chartab, ID: 16*/
void fill_chartab(char* chartab)
{
    /*fill_chartab:16*/
    /*CompoundStmt:1640*/
    int i;
    for (i = 0; i < 128; i++)
        chartab[i] = 0;
    for (i = 0; i < 25; i++) {
        char c = amino_acid_codes[i];
        chartab[(int)c] = chartab[tolower(c)] = c;
    }
}
/*Function: encode, ID: 17*/
void encode(char* seq, char* naseq, int l)
{
    /*encode:17*/
    /*CompoundStmt:1661*/
    int i, j;
    char c, *t;
    for (i = 1; i <= l; i++)
        if (seq[i] == '-') {
            naseq[i] = (char)gap_pos2;
        } else {
            j = 0;
            c = seq[i];
            t = (char*)amino_acid_codes;
            naseq[i] = -1;
            while (t[j]) {
                if (t[j] == c) {
                    naseq[i] = (char)j;
                    break;
                }
                j++;
            }
        }
    naseq[l + 1] = -3;
}
/*Function: alloc_aln, ID: 18*/
void alloc_aln(int nseqs)
{
    /*alloc_aln:18*/
    /*CompoundStmt:1699*/
    int i;
    names = (char**)malloc((nseqs + 1) * sizeof(char*));
    seq_array = (char**)malloc((nseqs + 1) * sizeof(char*));
    seqlen_array = (int*)malloc((nseqs + 1) * sizeof(int));
    for (i = 0; i < nseqs + 1; i++) {
        names[i] = (char*)malloc((30 + 1) * sizeof(char));
        seq_array[i] = (char*)((void*)0);
    }
}
/*Function: get_seq, ID: 19*/
char* get_seq(char* sname, int* len, char* chartab, FILE* fin)
{
    /*get_seq:19*/
    /*CompoundStmt:1736*/
    unsigned int i, j;
    char c, *seq;
    static char line[513];
    *len = 0;
    seq = (char*)((void*)0);
    while (*line != '>' && fgets(line, 512 + 1, fin) != ((void*)0))
        ;
    for (i = 1; i <= strlen(line); i++)
        if (line[i] != ' ')
            break;
    for (j = i; j <= strlen(line); j++)
        if (line[j] == ' ')
            break;
    strlcpy(sname, line + i, j - i + 1);
    sname[j - i] = '\x00';
    while (fgets(line, 512 + 1, fin) != ((void*)0)) {
        if (seq == (char*)((void*)0))
            seq = (char*)malloc((512 + 2) * sizeof(char));
        else
            seq = (char*)realloc(seq, ((*len) + 512 + 2) * sizeof(char));
        for (i = 0; i <= 512; i++) {
            c = line[i];
            if (c == '\n' || c == '\x00' || c == '>')
                break;
            if (c == chartab[(int)c]) {
                *len += 1;
                seq[*len] = c;
            }
        }
        if (c == '>')
            break;
    }
    seq[*len + 1] = '\x00';
    return seq;
}
/*Function: readseqs, ID: 20*/
int readseqs(char* filename)
{
    /*readseqs:20*/
    /*CompoundStmt:1834*/
    int i, l1, no_seqs;
    FILE* fin;
    char *seq1, chartab[128];
    if ((fin = fopen(filename, "r")) == ((void*)0)) {
        {
            if (bots_verbose_mode >= BOTS_VERBOSE_DEFAULT) {
                fprintf(stdout, "Could not open sequence file (%s)\n", filename);
            }
        };
        exit(-1);
    }
    if (fscanf(fin, "Number of sequences is %d", &no_seqs) == (-1)) {
        {
            if (bots_verbose_mode >= BOTS_VERBOSE_DEFAULT) {
                fprintf(stdout, "Sequence file is bogus (%s)\n", filename);
            }
        };
        exit(-1);
    }
    fill_chartab(chartab);
    {
        /*CompoundStmt:1859*/
        if (bots_verbose_mode >= BOTS_VERBOSE_DEFAULT) {
            fprintf(stdout, "Sequence format is Pearson\n");
        }
    }
    alloc_aln(no_seqs);
    for (i = 1; i <= no_seqs; i++) {
        seq1 = get_seq(names[i], &l1, chartab, fin);
        seqlen_array[i] = l1;
        seq_array[i] = (char*)malloc((l1 + 2) * sizeof(char));
        encode(seq1, seq_array[i], l1);
        free(seq1);
    }
    return no_seqs;
}
