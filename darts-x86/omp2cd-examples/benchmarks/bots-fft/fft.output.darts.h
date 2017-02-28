#ifndef _fft_output_darts_h_
#define _fft_output_darts_h_
#ifndef __DARTS_
#define __DARTS_
#endif
#include "app-desc.h"
#include "bots.h"
#include "darts.h"
#include "fft.h"
#include "ompTP.h"
#include "tbb/concurrent_vector.h"
#include "utils.h"
#include <limits.h>
#include <math.h>
#include <mutex>
#include <numa.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
void fft_unshuffle_32_seq(int a, int b, COMPLEX* in, COMPLEX* out, int m);
void fft_twiddle_16_seq(
    int a, int b, COMPLEX* in, COMPLEX* out, COMPLEX* W, int nW, int nWdn, int m);
void fft_base_16(COMPLEX* in, COMPLEX* out);
void fft_aux_seq(int n, COMPLEX* in, COMPLEX* out, int* factors, COMPLEX* W, int nW);
void fft_unshuffle_8_seq(int a, int b, COMPLEX* in, COMPLEX* out, int m);
void fft_twiddle_gen_seq(
    int i, int i1, COMPLEX* in, COMPLEX* out, COMPLEX* W, int nW, int nWdn, int r, int m);
void unshuffle_seq(int a, int b, COMPLEX* in, COMPLEX* out, int r, int m);
void fft(int n, COMPLEX* in, COMPLEX* out);
void fft_unshuffle_2_seq(int a, int b, COMPLEX* in, COMPLEX* out, int m);
int test_correctness(int n, COMPLEX* out1, COMPLEX* out2);
void fft_base_4(COMPLEX* in, COMPLEX* out);
void fft_unshuffle_16_seq(int a, int b, COMPLEX* in, COMPLEX* out, int m);
void fft_twiddle_gen1(
    COMPLEX* in, COMPLEX* out, COMPLEX* W, int r, int m, int nW, int nWdnti, int nWdntm);
void fft_twiddle_32_seq(
    int a, int b, COMPLEX* in, COMPLEX* out, COMPLEX* W, int nW, int nWdn, int m);
void compute_w_coefficients_seq(int n, int a, int b, COMPLEX* W);
void fft_seq(int n, COMPLEX* in, COMPLEX* out);
int factor(int n);
void fft_base_2(COMPLEX* in, COMPLEX* out);
void fft_twiddle_2_seq(
    int a, int b, COMPLEX* in, COMPLEX* out, COMPLEX* W, int nW, int nWdn, int m);
void fft_base_32(COMPLEX* in, COMPLEX* out);
void fft_unshuffle_4_seq(int a, int b, COMPLEX* in, COMPLEX* out, int m);
void fft_base_8(COMPLEX* in, COMPLEX* out);
void fft_twiddle_4_seq(
    int a, int b, COMPLEX* in, COMPLEX* out, COMPLEX* W, int nW, int nWdn, int m);
void fft_twiddle_8_seq(
    int a, int b, COMPLEX* in, COMPLEX* out, COMPLEX* W, int nW, int nWdn, int m);
class TP15;
typedef TP15 TP_compute_w_coefficients;
/*Class containing the inputs passed to task149*/
class _task149Inputs {
public:
    COMPLEX* W_darts149 /*OMP_FIRSTPRIVATE*/;
    int a_darts149 /*OMP_FIRSTPRIVATE*/;
    int ab_darts149 /*OMP_FIRSTPRIVATE*/;
    int n_darts149 /*OMP_FIRSTPRIVATE*/;
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task149Inputs() {}
    _task149Inputs(
        COMPLEX** in_W, int* in_a, int* in_ab, int* in_n, darts::Codelet* in_nextSyncCodelet)
        : taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
        this->W_darts149 = *in_W;
        this->a_darts149 = *in_a;
        this->ab_darts149 = *in_ab;
        this->n_darts149 = *in_n;
    }
    ~_task149Inputs() {}
};
class TP149;
/*Class containing the inputs passed to task151*/
class _task151Inputs {
public:
    COMPLEX* W_darts151 /*OMP_FIRSTPRIVATE*/;
    int ab_darts151 /*OMP_FIRSTPRIVATE*/;
    int b_darts151 /*OMP_FIRSTPRIVATE*/;
    int n_darts151 /*OMP_FIRSTPRIVATE*/;
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task151Inputs() {}
    _task151Inputs(
        COMPLEX** in_W, int* in_ab, int* in_b, int* in_n, darts::Codelet* in_nextSyncCodelet)
        : taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
        this->W_darts151 = *in_W;
        this->ab_darts151 = *in_ab;
        this->b_darts151 = *in_b;
        this->n_darts151 = *in_n;
    }
    ~_task151Inputs() {}
};
class TP151;
class TP18;
typedef TP18 TP_unshuffle;
/*Class containing the inputs passed to task290*/
class _task290Inputs {
public:
    int a_darts290 /*OMP_FIRSTPRIVATE*/;
    int ab_darts290 /*OMP_FIRSTPRIVATE*/;
    COMPLEX* in_darts290 /*OMP_FIRSTPRIVATE*/;
    int m_darts290 /*OMP_FIRSTPRIVATE*/;
    COMPLEX* out_darts290 /*OMP_FIRSTPRIVATE*/;
    int r_darts290 /*OMP_FIRSTPRIVATE*/;
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task290Inputs() {}
    _task290Inputs(int* in_a, int* in_ab, COMPLEX** in_in, int* in_m, COMPLEX** in_out, int* in_r,
        darts::Codelet* in_nextSyncCodelet)
        : taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
        this->a_darts290 = *in_a;
        this->ab_darts290 = *in_ab;
        this->in_darts290 = *in_in;
        this->m_darts290 = *in_m;
        this->out_darts290 = *in_out;
        this->r_darts290 = *in_r;
    }
    ~_task290Inputs() {}
};
class TP290;
/*Class containing the inputs passed to task292*/
class _task292Inputs {
public:
    int ab_darts292 /*OMP_FIRSTPRIVATE*/;
    int b_darts292 /*OMP_FIRSTPRIVATE*/;
    COMPLEX* in_darts292 /*OMP_FIRSTPRIVATE*/;
    int m_darts292 /*OMP_FIRSTPRIVATE*/;
    COMPLEX* out_darts292 /*OMP_FIRSTPRIVATE*/;
    int r_darts292 /*OMP_FIRSTPRIVATE*/;
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task292Inputs() {}
    _task292Inputs(int* in_ab, int* in_b, COMPLEX** in_in, int* in_m, COMPLEX** in_out, int* in_r,
        darts::Codelet* in_nextSyncCodelet)
        : taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
        this->ab_darts292 = *in_ab;
        this->b_darts292 = *in_b;
        this->in_darts292 = *in_in;
        this->m_darts292 = *in_m;
        this->out_darts292 = *in_out;
        this->r_darts292 = *in_r;
    }
    ~_task292Inputs() {}
};
class TP292;
class TP21;
typedef TP21 TP_fft_twiddle_gen;
/*Class containing the inputs passed to task418*/
class _task418Inputs {
public:
    COMPLEX* W_darts418 /*OMP_FIRSTPRIVATE*/;
    int i_darts418 /*OMP_FIRSTPRIVATE*/;
    COMPLEX* in_darts418 /*OMP_FIRSTPRIVATE*/;
    int m_darts418 /*OMP_FIRSTPRIVATE*/;
    int nW_darts418 /*OMP_FIRSTPRIVATE*/;
    int nWdn_darts418 /*OMP_FIRSTPRIVATE*/;
    COMPLEX* out_darts418 /*OMP_FIRSTPRIVATE*/;
    int r_darts418 /*OMP_FIRSTPRIVATE*/;
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task418Inputs() {}
    _task418Inputs(COMPLEX** in_W, int* in_i, COMPLEX** in_in, int* in_m, int* in_nW, int* in_nWdn,
        COMPLEX** in_out, int* in_r, darts::Codelet* in_nextSyncCodelet)
        : taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
        this->W_darts418 = *in_W;
        this->i_darts418 = *in_i;
        this->in_darts418 = *in_in;
        this->m_darts418 = *in_m;
        this->nW_darts418 = *in_nW;
        this->nWdn_darts418 = *in_nWdn;
        this->out_darts418 = *in_out;
        this->r_darts418 = *in_r;
    }
    ~_task418Inputs() {}
};
class TP418;
/*Class containing the inputs passed to task428*/
class _task428Inputs {
public:
    COMPLEX* W_darts428 /*OMP_FIRSTPRIVATE*/;
    int i_darts428 /*OMP_FIRSTPRIVATE*/;
    int i2_darts428 /*OMP_FIRSTPRIVATE*/;
    COMPLEX* in_darts428 /*OMP_FIRSTPRIVATE*/;
    int m_darts428 /*OMP_FIRSTPRIVATE*/;
    int nW_darts428 /*OMP_FIRSTPRIVATE*/;
    int nWdn_darts428 /*OMP_FIRSTPRIVATE*/;
    COMPLEX* out_darts428 /*OMP_FIRSTPRIVATE*/;
    int r_darts428 /*OMP_FIRSTPRIVATE*/;
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task428Inputs() {}
    _task428Inputs(COMPLEX** in_W, int* in_i, int* in_i2, COMPLEX** in_in, int* in_m, int* in_nW,
        int* in_nWdn, COMPLEX** in_out, int* in_r, darts::Codelet* in_nextSyncCodelet)
        : taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
        this->W_darts428 = *in_W;
        this->i_darts428 = *in_i;
        this->i2_darts428 = *in_i2;
        this->in_darts428 = *in_in;
        this->m_darts428 = *in_m;
        this->nW_darts428 = *in_nW;
        this->nWdn_darts428 = *in_nWdn;
        this->out_darts428 = *in_out;
        this->r_darts428 = *in_r;
    }
    ~_task428Inputs() {}
};
class TP428;
/*Class containing the inputs passed to task430*/
class _task430Inputs {
public:
    COMPLEX* W_darts430 /*OMP_FIRSTPRIVATE*/;
    int i1_darts430 /*OMP_FIRSTPRIVATE*/;
    int i2_darts430 /*OMP_FIRSTPRIVATE*/;
    COMPLEX* in_darts430 /*OMP_FIRSTPRIVATE*/;
    int m_darts430 /*OMP_FIRSTPRIVATE*/;
    int nW_darts430 /*OMP_FIRSTPRIVATE*/;
    int nWdn_darts430 /*OMP_FIRSTPRIVATE*/;
    COMPLEX* out_darts430 /*OMP_FIRSTPRIVATE*/;
    int r_darts430 /*OMP_FIRSTPRIVATE*/;
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task430Inputs() {}
    _task430Inputs(COMPLEX** in_W, int* in_i1, int* in_i2, COMPLEX** in_in, int* in_m, int* in_nW,
        int* in_nWdn, COMPLEX** in_out, int* in_r, darts::Codelet* in_nextSyncCodelet)
        : taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
        this->W_darts430 = *in_W;
        this->i1_darts430 = *in_i1;
        this->i2_darts430 = *in_i2;
        this->in_darts430 = *in_in;
        this->m_darts430 = *in_m;
        this->nW_darts430 = *in_nW;
        this->nWdn_darts430 = *in_nWdn;
        this->out_darts430 = *in_out;
        this->r_darts430 = *in_r;
    }
    ~_task430Inputs() {}
};
class TP430;
class TP24;
typedef TP24 TP_fft_twiddle_2;
/*Class containing the inputs passed to task558*/
class _task558Inputs {
public:
    COMPLEX* W_darts558 /*OMP_FIRSTPRIVATE*/;
    int a_darts558 /*OMP_FIRSTPRIVATE*/;
    int ab_darts558 /*OMP_FIRSTPRIVATE*/;
    COMPLEX* in_darts558 /*OMP_FIRSTPRIVATE*/;
    int m_darts558 /*OMP_FIRSTPRIVATE*/;
    int nW_darts558 /*OMP_FIRSTPRIVATE*/;
    int nWdn_darts558 /*OMP_FIRSTPRIVATE*/;
    COMPLEX* out_darts558 /*OMP_FIRSTPRIVATE*/;
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task558Inputs() {}
    _task558Inputs(COMPLEX** in_W, int* in_a, int* in_ab, COMPLEX** in_in, int* in_m, int* in_nW,
        int* in_nWdn, COMPLEX** in_out, darts::Codelet* in_nextSyncCodelet)
        : taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
        this->W_darts558 = *in_W;
        this->a_darts558 = *in_a;
        this->ab_darts558 = *in_ab;
        this->in_darts558 = *in_in;
        this->m_darts558 = *in_m;
        this->nW_darts558 = *in_nW;
        this->nWdn_darts558 = *in_nWdn;
        this->out_darts558 = *in_out;
    }
    ~_task558Inputs() {}
};
class TP558;
/*Class containing the inputs passed to task560*/
class _task560Inputs {
public:
    COMPLEX* W_darts560 /*OMP_FIRSTPRIVATE*/;
    int ab_darts560 /*OMP_FIRSTPRIVATE*/;
    int b_darts560 /*OMP_FIRSTPRIVATE*/;
    COMPLEX* in_darts560 /*OMP_FIRSTPRIVATE*/;
    int m_darts560 /*OMP_FIRSTPRIVATE*/;
    int nW_darts560 /*OMP_FIRSTPRIVATE*/;
    int nWdn_darts560 /*OMP_FIRSTPRIVATE*/;
    COMPLEX* out_darts560 /*OMP_FIRSTPRIVATE*/;
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task560Inputs() {}
    _task560Inputs(COMPLEX** in_W, int* in_ab, int* in_b, COMPLEX** in_in, int* in_m, int* in_nW,
        int* in_nWdn, COMPLEX** in_out, darts::Codelet* in_nextSyncCodelet)
        : taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
        this->W_darts560 = *in_W;
        this->ab_darts560 = *in_ab;
        this->b_darts560 = *in_b;
        this->in_darts560 = *in_in;
        this->m_darts560 = *in_m;
        this->nW_darts560 = *in_nW;
        this->nWdn_darts560 = *in_nWdn;
        this->out_darts560 = *in_out;
    }
    ~_task560Inputs() {}
};
class TP560;
class TP26;
typedef TP26 TP_fft_unshuffle_2;
/*Class containing the inputs passed to task676*/
class _task676Inputs {
public:
    int a_darts676 /*OMP_FIRSTPRIVATE*/;
    int ab_darts676 /*OMP_FIRSTPRIVATE*/;
    COMPLEX* in_darts676 /*OMP_FIRSTPRIVATE*/;
    int m_darts676 /*OMP_FIRSTPRIVATE*/;
    COMPLEX* out_darts676 /*OMP_FIRSTPRIVATE*/;
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task676Inputs() {}
    _task676Inputs(int* in_a, int* in_ab, COMPLEX** in_in, int* in_m, COMPLEX** in_out,
        darts::Codelet* in_nextSyncCodelet)
        : taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
        this->a_darts676 = *in_a;
        this->ab_darts676 = *in_ab;
        this->in_darts676 = *in_in;
        this->m_darts676 = *in_m;
        this->out_darts676 = *in_out;
    }
    ~_task676Inputs() {}
};
class TP676;
/*Class containing the inputs passed to task678*/
class _task678Inputs {
public:
    int ab_darts678 /*OMP_FIRSTPRIVATE*/;
    int b_darts678 /*OMP_FIRSTPRIVATE*/;
    COMPLEX* in_darts678 /*OMP_FIRSTPRIVATE*/;
    int m_darts678 /*OMP_FIRSTPRIVATE*/;
    COMPLEX* out_darts678 /*OMP_FIRSTPRIVATE*/;
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task678Inputs() {}
    _task678Inputs(int* in_ab, int* in_b, COMPLEX** in_in, int* in_m, COMPLEX** in_out,
        darts::Codelet* in_nextSyncCodelet)
        : taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
        this->ab_darts678 = *in_ab;
        this->b_darts678 = *in_b;
        this->in_darts678 = *in_in;
        this->m_darts678 = *in_m;
        this->out_darts678 = *in_out;
    }
    ~_task678Inputs() {}
};
class TP678;
class TP29;
typedef TP29 TP_fft_twiddle_4;
/*Class containing the inputs passed to task965*/
class _task965Inputs {
public:
    COMPLEX* W_darts965 /*OMP_FIRSTPRIVATE*/;
    int a_darts965 /*OMP_FIRSTPRIVATE*/;
    int ab_darts965 /*OMP_FIRSTPRIVATE*/;
    COMPLEX* in_darts965 /*OMP_FIRSTPRIVATE*/;
    int m_darts965 /*OMP_FIRSTPRIVATE*/;
    int nW_darts965 /*OMP_FIRSTPRIVATE*/;
    int nWdn_darts965 /*OMP_FIRSTPRIVATE*/;
    COMPLEX* out_darts965 /*OMP_FIRSTPRIVATE*/;
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task965Inputs() {}
    _task965Inputs(COMPLEX** in_W, int* in_a, int* in_ab, COMPLEX** in_in, int* in_m, int* in_nW,
        int* in_nWdn, COMPLEX** in_out, darts::Codelet* in_nextSyncCodelet)
        : taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
        this->W_darts965 = *in_W;
        this->a_darts965 = *in_a;
        this->ab_darts965 = *in_ab;
        this->in_darts965 = *in_in;
        this->m_darts965 = *in_m;
        this->nW_darts965 = *in_nW;
        this->nWdn_darts965 = *in_nWdn;
        this->out_darts965 = *in_out;
    }
    ~_task965Inputs() {}
};
class TP965;
/*Class containing the inputs passed to task967*/
class _task967Inputs {
public:
    COMPLEX* W_darts967 /*OMP_FIRSTPRIVATE*/;
    int ab_darts967 /*OMP_FIRSTPRIVATE*/;
    int b_darts967 /*OMP_FIRSTPRIVATE*/;
    COMPLEX* in_darts967 /*OMP_FIRSTPRIVATE*/;
    int m_darts967 /*OMP_FIRSTPRIVATE*/;
    int nW_darts967 /*OMP_FIRSTPRIVATE*/;
    int nWdn_darts967 /*OMP_FIRSTPRIVATE*/;
    COMPLEX* out_darts967 /*OMP_FIRSTPRIVATE*/;
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task967Inputs() {}
    _task967Inputs(COMPLEX** in_W, int* in_ab, int* in_b, COMPLEX** in_in, int* in_m, int* in_nW,
        int* in_nWdn, COMPLEX** in_out, darts::Codelet* in_nextSyncCodelet)
        : taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
        this->W_darts967 = *in_W;
        this->ab_darts967 = *in_ab;
        this->b_darts967 = *in_b;
        this->in_darts967 = *in_in;
        this->m_darts967 = *in_m;
        this->nW_darts967 = *in_nW;
        this->nWdn_darts967 = *in_nWdn;
        this->out_darts967 = *in_out;
    }
    ~_task967Inputs() {}
};
class TP967;
class TP31;
typedef TP31 TP_fft_unshuffle_4;
/*Class containing the inputs passed to task1184*/
class _task1184Inputs {
public:
    int a_darts1184 /*OMP_FIRSTPRIVATE*/;
    int ab_darts1184 /*OMP_FIRSTPRIVATE*/;
    COMPLEX* in_darts1184 /*OMP_FIRSTPRIVATE*/;
    int m_darts1184 /*OMP_FIRSTPRIVATE*/;
    COMPLEX* out_darts1184 /*OMP_FIRSTPRIVATE*/;
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task1184Inputs() {}
    _task1184Inputs(int* in_a, int* in_ab, COMPLEX** in_in, int* in_m, COMPLEX** in_out,
        darts::Codelet* in_nextSyncCodelet)
        : taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
        this->a_darts1184 = *in_a;
        this->ab_darts1184 = *in_ab;
        this->in_darts1184 = *in_in;
        this->m_darts1184 = *in_m;
        this->out_darts1184 = *in_out;
    }
    ~_task1184Inputs() {}
};
class TP1184;
/*Class containing the inputs passed to task1186*/
class _task1186Inputs {
public:
    int ab_darts1186 /*OMP_FIRSTPRIVATE*/;
    int b_darts1186 /*OMP_FIRSTPRIVATE*/;
    COMPLEX* in_darts1186 /*OMP_FIRSTPRIVATE*/;
    int m_darts1186 /*OMP_FIRSTPRIVATE*/;
    COMPLEX* out_darts1186 /*OMP_FIRSTPRIVATE*/;
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task1186Inputs() {}
    _task1186Inputs(int* in_ab, int* in_b, COMPLEX** in_in, int* in_m, COMPLEX** in_out,
        darts::Codelet* in_nextSyncCodelet)
        : taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
        this->ab_darts1186 = *in_ab;
        this->b_darts1186 = *in_b;
        this->in_darts1186 = *in_in;
        this->m_darts1186 = *in_m;
        this->out_darts1186 = *in_out;
    }
    ~_task1186Inputs() {}
};
class TP1186;
class TP34;
typedef TP34 TP_fft_twiddle_8;
/*Class containing the inputs passed to task1844*/
class _task1844Inputs {
public:
    COMPLEX* W_darts1844 /*OMP_FIRSTPRIVATE*/;
    int a_darts1844 /*OMP_FIRSTPRIVATE*/;
    int ab_darts1844 /*OMP_FIRSTPRIVATE*/;
    COMPLEX* in_darts1844 /*OMP_FIRSTPRIVATE*/;
    int m_darts1844 /*OMP_FIRSTPRIVATE*/;
    int nW_darts1844 /*OMP_FIRSTPRIVATE*/;
    int nWdn_darts1844 /*OMP_FIRSTPRIVATE*/;
    COMPLEX* out_darts1844 /*OMP_FIRSTPRIVATE*/;
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task1844Inputs() {}
    _task1844Inputs(COMPLEX** in_W, int* in_a, int* in_ab, COMPLEX** in_in, int* in_m, int* in_nW,
        int* in_nWdn, COMPLEX** in_out, darts::Codelet* in_nextSyncCodelet)
        : taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
        this->W_darts1844 = *in_W;
        this->a_darts1844 = *in_a;
        this->ab_darts1844 = *in_ab;
        this->in_darts1844 = *in_in;
        this->m_darts1844 = *in_m;
        this->nW_darts1844 = *in_nW;
        this->nWdn_darts1844 = *in_nWdn;
        this->out_darts1844 = *in_out;
    }
    ~_task1844Inputs() {}
};
class TP1844;
/*Class containing the inputs passed to task1846*/
class _task1846Inputs {
public:
    COMPLEX* W_darts1846 /*OMP_FIRSTPRIVATE*/;
    int ab_darts1846 /*OMP_FIRSTPRIVATE*/;
    int b_darts1846 /*OMP_FIRSTPRIVATE*/;
    COMPLEX* in_darts1846 /*OMP_FIRSTPRIVATE*/;
    int m_darts1846 /*OMP_FIRSTPRIVATE*/;
    int nW_darts1846 /*OMP_FIRSTPRIVATE*/;
    int nWdn_darts1846 /*OMP_FIRSTPRIVATE*/;
    COMPLEX* out_darts1846 /*OMP_FIRSTPRIVATE*/;
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task1846Inputs() {}
    _task1846Inputs(COMPLEX** in_W, int* in_ab, int* in_b, COMPLEX** in_in, int* in_m, int* in_nW,
        int* in_nWdn, COMPLEX** in_out, darts::Codelet* in_nextSyncCodelet)
        : taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
        this->W_darts1846 = *in_W;
        this->ab_darts1846 = *in_ab;
        this->b_darts1846 = *in_b;
        this->in_darts1846 = *in_in;
        this->m_darts1846 = *in_m;
        this->nW_darts1846 = *in_nW;
        this->nWdn_darts1846 = *in_nWdn;
        this->out_darts1846 = *in_out;
    }
    ~_task1846Inputs() {}
};
class TP1846;
class TP36;
typedef TP36 TP_fft_unshuffle_8;
/*Class containing the inputs passed to task2301*/
class _task2301Inputs {
public:
    int a_darts2301 /*OMP_FIRSTPRIVATE*/;
    int ab_darts2301 /*OMP_FIRSTPRIVATE*/;
    COMPLEX* in_darts2301 /*OMP_FIRSTPRIVATE*/;
    int m_darts2301 /*OMP_FIRSTPRIVATE*/;
    COMPLEX* out_darts2301 /*OMP_FIRSTPRIVATE*/;
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task2301Inputs() {}
    _task2301Inputs(int* in_a, int* in_ab, COMPLEX** in_in, int* in_m, COMPLEX** in_out,
        darts::Codelet* in_nextSyncCodelet)
        : taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
        this->a_darts2301 = *in_a;
        this->ab_darts2301 = *in_ab;
        this->in_darts2301 = *in_in;
        this->m_darts2301 = *in_m;
        this->out_darts2301 = *in_out;
    }
    ~_task2301Inputs() {}
};
class TP2301;
/*Class containing the inputs passed to task2303*/
class _task2303Inputs {
public:
    int ab_darts2303 /*OMP_FIRSTPRIVATE*/;
    int b_darts2303 /*OMP_FIRSTPRIVATE*/;
    COMPLEX* in_darts2303 /*OMP_FIRSTPRIVATE*/;
    int m_darts2303 /*OMP_FIRSTPRIVATE*/;
    COMPLEX* out_darts2303 /*OMP_FIRSTPRIVATE*/;
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task2303Inputs() {}
    _task2303Inputs(int* in_ab, int* in_b, COMPLEX** in_in, int* in_m, COMPLEX** in_out,
        darts::Codelet* in_nextSyncCodelet)
        : taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
        this->ab_darts2303 = *in_ab;
        this->b_darts2303 = *in_b;
        this->in_darts2303 = *in_in;
        this->m_darts2303 = *in_m;
        this->out_darts2303 = *in_out;
    }
    ~_task2303Inputs() {}
};
class TP2303;
class TP39;
typedef TP39 TP_fft_twiddle_16;
/*Class containing the inputs passed to task3875*/
class _task3875Inputs {
public:
    COMPLEX* W_darts3875 /*OMP_FIRSTPRIVATE*/;
    int a_darts3875 /*OMP_FIRSTPRIVATE*/;
    int ab_darts3875 /*OMP_FIRSTPRIVATE*/;
    COMPLEX* in_darts3875 /*OMP_FIRSTPRIVATE*/;
    int m_darts3875 /*OMP_FIRSTPRIVATE*/;
    int nW_darts3875 /*OMP_FIRSTPRIVATE*/;
    int nWdn_darts3875 /*OMP_FIRSTPRIVATE*/;
    COMPLEX* out_darts3875 /*OMP_FIRSTPRIVATE*/;
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task3875Inputs() {}
    _task3875Inputs(COMPLEX** in_W, int* in_a, int* in_ab, COMPLEX** in_in, int* in_m, int* in_nW,
        int* in_nWdn, COMPLEX** in_out, darts::Codelet* in_nextSyncCodelet)
        : taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
        this->W_darts3875 = *in_W;
        this->a_darts3875 = *in_a;
        this->ab_darts3875 = *in_ab;
        this->in_darts3875 = *in_in;
        this->m_darts3875 = *in_m;
        this->nW_darts3875 = *in_nW;
        this->nWdn_darts3875 = *in_nWdn;
        this->out_darts3875 = *in_out;
    }
    ~_task3875Inputs() {}
};
class TP3875;
/*Class containing the inputs passed to task3877*/
class _task3877Inputs {
public:
    COMPLEX* W_darts3877 /*OMP_FIRSTPRIVATE*/;
    int ab_darts3877 /*OMP_FIRSTPRIVATE*/;
    int b_darts3877 /*OMP_FIRSTPRIVATE*/;
    COMPLEX* in_darts3877 /*OMP_FIRSTPRIVATE*/;
    int m_darts3877 /*OMP_FIRSTPRIVATE*/;
    int nW_darts3877 /*OMP_FIRSTPRIVATE*/;
    int nWdn_darts3877 /*OMP_FIRSTPRIVATE*/;
    COMPLEX* out_darts3877 /*OMP_FIRSTPRIVATE*/;
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task3877Inputs() {}
    _task3877Inputs(COMPLEX** in_W, int* in_ab, int* in_b, COMPLEX** in_in, int* in_m, int* in_nW,
        int* in_nWdn, COMPLEX** in_out, darts::Codelet* in_nextSyncCodelet)
        : taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
        this->W_darts3877 = *in_W;
        this->ab_darts3877 = *in_ab;
        this->b_darts3877 = *in_b;
        this->in_darts3877 = *in_in;
        this->m_darts3877 = *in_m;
        this->nW_darts3877 = *in_nW;
        this->nWdn_darts3877 = *in_nWdn;
        this->out_darts3877 = *in_out;
    }
    ~_task3877Inputs() {}
};
class TP3877;
class TP41;
typedef TP41 TP_fft_unshuffle_16;
/*Class containing the inputs passed to task4896*/
class _task4896Inputs {
public:
    int a_darts4896 /*OMP_FIRSTPRIVATE*/;
    int ab_darts4896 /*OMP_FIRSTPRIVATE*/;
    COMPLEX* in_darts4896 /*OMP_FIRSTPRIVATE*/;
    int m_darts4896 /*OMP_FIRSTPRIVATE*/;
    COMPLEX* out_darts4896 /*OMP_FIRSTPRIVATE*/;
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task4896Inputs() {}
    _task4896Inputs(int* in_a, int* in_ab, COMPLEX** in_in, int* in_m, COMPLEX** in_out,
        darts::Codelet* in_nextSyncCodelet)
        : taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
        this->a_darts4896 = *in_a;
        this->ab_darts4896 = *in_ab;
        this->in_darts4896 = *in_in;
        this->m_darts4896 = *in_m;
        this->out_darts4896 = *in_out;
    }
    ~_task4896Inputs() {}
};
class TP4896;
/*Class containing the inputs passed to task4898*/
class _task4898Inputs {
public:
    int ab_darts4898 /*OMP_FIRSTPRIVATE*/;
    int b_darts4898 /*OMP_FIRSTPRIVATE*/;
    COMPLEX* in_darts4898 /*OMP_FIRSTPRIVATE*/;
    int m_darts4898 /*OMP_FIRSTPRIVATE*/;
    COMPLEX* out_darts4898 /*OMP_FIRSTPRIVATE*/;
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task4898Inputs() {}
    _task4898Inputs(int* in_ab, int* in_b, COMPLEX** in_in, int* in_m, COMPLEX** in_out,
        darts::Codelet* in_nextSyncCodelet)
        : taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
        this->ab_darts4898 = *in_ab;
        this->b_darts4898 = *in_b;
        this->in_darts4898 = *in_in;
        this->m_darts4898 = *in_m;
        this->out_darts4898 = *in_out;
    }
    ~_task4898Inputs() {}
};
class TP4898;
class TP44;
typedef TP44 TP_fft_twiddle_32;
/*Class containing the inputs passed to task8650*/
class _task8650Inputs {
public:
    COMPLEX* W_darts8650 /*OMP_FIRSTPRIVATE*/;
    int a_darts8650 /*OMP_FIRSTPRIVATE*/;
    int ab_darts8650 /*OMP_FIRSTPRIVATE*/;
    COMPLEX* in_darts8650 /*OMP_FIRSTPRIVATE*/;
    int m_darts8650 /*OMP_FIRSTPRIVATE*/;
    int nW_darts8650 /*OMP_FIRSTPRIVATE*/;
    int nWdn_darts8650 /*OMP_FIRSTPRIVATE*/;
    COMPLEX* out_darts8650 /*OMP_FIRSTPRIVATE*/;
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task8650Inputs() {}
    _task8650Inputs(COMPLEX** in_W, int* in_a, int* in_ab, COMPLEX** in_in, int* in_m, int* in_nW,
        int* in_nWdn, COMPLEX** in_out, darts::Codelet* in_nextSyncCodelet)
        : taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
        this->W_darts8650 = *in_W;
        this->a_darts8650 = *in_a;
        this->ab_darts8650 = *in_ab;
        this->in_darts8650 = *in_in;
        this->m_darts8650 = *in_m;
        this->nW_darts8650 = *in_nW;
        this->nWdn_darts8650 = *in_nWdn;
        this->out_darts8650 = *in_out;
    }
    ~_task8650Inputs() {}
};
class TP8650;
/*Class containing the inputs passed to task8652*/
class _task8652Inputs {
public:
    COMPLEX* W_darts8652 /*OMP_FIRSTPRIVATE*/;
    int ab_darts8652 /*OMP_FIRSTPRIVATE*/;
    int b_darts8652 /*OMP_FIRSTPRIVATE*/;
    COMPLEX* in_darts8652 /*OMP_FIRSTPRIVATE*/;
    int m_darts8652 /*OMP_FIRSTPRIVATE*/;
    int nW_darts8652 /*OMP_FIRSTPRIVATE*/;
    int nWdn_darts8652 /*OMP_FIRSTPRIVATE*/;
    COMPLEX* out_darts8652 /*OMP_FIRSTPRIVATE*/;
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task8652Inputs() {}
    _task8652Inputs(COMPLEX** in_W, int* in_ab, int* in_b, COMPLEX** in_in, int* in_m, int* in_nW,
        int* in_nWdn, COMPLEX** in_out, darts::Codelet* in_nextSyncCodelet)
        : taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
        this->W_darts8652 = *in_W;
        this->ab_darts8652 = *in_ab;
        this->b_darts8652 = *in_b;
        this->in_darts8652 = *in_in;
        this->m_darts8652 = *in_m;
        this->nW_darts8652 = *in_nW;
        this->nWdn_darts8652 = *in_nWdn;
        this->out_darts8652 = *in_out;
    }
    ~_task8652Inputs() {}
};
class TP8652;
class TP46;
typedef TP46 TP_fft_unshuffle_32;
/*Class containing the inputs passed to task10975*/
class _task10975Inputs {
public:
    int a_darts10975 /*OMP_FIRSTPRIVATE*/;
    int ab_darts10975 /*OMP_FIRSTPRIVATE*/;
    COMPLEX* in_darts10975 /*OMP_FIRSTPRIVATE*/;
    int m_darts10975 /*OMP_FIRSTPRIVATE*/;
    COMPLEX* out_darts10975 /*OMP_FIRSTPRIVATE*/;
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task10975Inputs() {}
    _task10975Inputs(int* in_a, int* in_ab, COMPLEX** in_in, int* in_m, COMPLEX** in_out,
        darts::Codelet* in_nextSyncCodelet)
        : taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
        this->a_darts10975 = *in_a;
        this->ab_darts10975 = *in_ab;
        this->in_darts10975 = *in_in;
        this->m_darts10975 = *in_m;
        this->out_darts10975 = *in_out;
    }
    ~_task10975Inputs() {}
};
class TP10975;
/*Class containing the inputs passed to task10977*/
class _task10977Inputs {
public:
    int ab_darts10977 /*OMP_FIRSTPRIVATE*/;
    int b_darts10977 /*OMP_FIRSTPRIVATE*/;
    COMPLEX* in_darts10977 /*OMP_FIRSTPRIVATE*/;
    int m_darts10977 /*OMP_FIRSTPRIVATE*/;
    COMPLEX* out_darts10977 /*OMP_FIRSTPRIVATE*/;
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task10977Inputs() {}
    _task10977Inputs(int* in_ab, int* in_b, COMPLEX** in_in, int* in_m, COMPLEX** in_out,
        darts::Codelet* in_nextSyncCodelet)
        : taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
        this->ab_darts10977 = *in_ab;
        this->b_darts10977 = *in_b;
        this->in_darts10977 = *in_in;
        this->m_darts10977 = *in_m;
        this->out_darts10977 = *in_out;
    }
    ~_task10977Inputs() {}
};
class TP10977;
class TP48;
typedef TP48 TP_fft_aux;
class TP11212;
/*Class containing the inputs passed to task11217*/
class _task11217Inputs {
public:
    COMPLEX* W_darts11217 /*OMP_FIRSTPRIVATE*/;
    int* factors_darts11217 /*OMP_FIRSTPRIVATE*/;
    COMPLEX* in_darts11217 /*OMP_FIRSTPRIVATE*/;
    int k_darts11217 /*OMP_FIRSTPRIVATE*/;
    int m_darts11217 /*OMP_FIRSTPRIVATE*/;
    int nW_darts11217 /*OMP_FIRSTPRIVATE*/;
    COMPLEX* out_darts11217 /*OMP_FIRSTPRIVATE*/;
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task11217Inputs() {}
    _task11217Inputs(COMPLEX** in_W, int** in_factors, COMPLEX** in_in, int* in_k, int* in_m,
        int* in_nW, COMPLEX** in_out, darts::Codelet* in_nextSyncCodelet)
        : taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
        this->W_darts11217 = *in_W;
        this->factors_darts11217 = *in_factors;
        this->in_darts11217 = *in_in;
        this->k_darts11217 = *in_k;
        this->m_darts11217 = *in_m;
        this->nW_darts11217 = *in_nW;
        this->out_darts11217 = *in_out;
    }
    ~_task11217Inputs() {}
};
class TP11217;
/*Class containing the inputs passed to task11183*/
class _task11183Inputs {
public:
    COMPLEX* in_darts11183 /*OMP_FIRSTPRIVATE*/;
    int m_darts11183 /*OMP_FIRSTPRIVATE*/;
    COMPLEX* out_darts11183 /*OMP_FIRSTPRIVATE*/;
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task11183Inputs() {}
    _task11183Inputs(
        COMPLEX** in_in, int* in_m, COMPLEX** in_out, darts::Codelet* in_nextSyncCodelet)
        : taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
        this->in_darts11183 = *in_in;
        this->m_darts11183 = *in_m;
        this->out_darts11183 = *in_out;
    }
    ~_task11183Inputs() {}
};
class TP11183;
/*Class containing the inputs passed to task11188*/
class _task11188Inputs {
public:
    COMPLEX* in_darts11188 /*OMP_FIRSTPRIVATE*/;
    int m_darts11188 /*OMP_FIRSTPRIVATE*/;
    COMPLEX* out_darts11188 /*OMP_FIRSTPRIVATE*/;
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task11188Inputs() {}
    _task11188Inputs(
        COMPLEX** in_in, int* in_m, COMPLEX** in_out, darts::Codelet* in_nextSyncCodelet)
        : taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
        this->in_darts11188 = *in_in;
        this->m_darts11188 = *in_m;
        this->out_darts11188 = *in_out;
    }
    ~_task11188Inputs() {}
};
class TP11188;
/*Class containing the inputs passed to task11193*/
class _task11193Inputs {
public:
    COMPLEX* in_darts11193 /*OMP_FIRSTPRIVATE*/;
    int m_darts11193 /*OMP_FIRSTPRIVATE*/;
    COMPLEX* out_darts11193 /*OMP_FIRSTPRIVATE*/;
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task11193Inputs() {}
    _task11193Inputs(
        COMPLEX** in_in, int* in_m, COMPLEX** in_out, darts::Codelet* in_nextSyncCodelet)
        : taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
        this->in_darts11193 = *in_in;
        this->m_darts11193 = *in_m;
        this->out_darts11193 = *in_out;
    }
    ~_task11193Inputs() {}
};
class TP11193;
/*Class containing the inputs passed to task11198*/
class _task11198Inputs {
public:
    COMPLEX* in_darts11198 /*OMP_FIRSTPRIVATE*/;
    int m_darts11198 /*OMP_FIRSTPRIVATE*/;
    COMPLEX* out_darts11198 /*OMP_FIRSTPRIVATE*/;
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task11198Inputs() {}
    _task11198Inputs(
        COMPLEX** in_in, int* in_m, COMPLEX** in_out, darts::Codelet* in_nextSyncCodelet)
        : taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
        this->in_darts11198 = *in_in;
        this->m_darts11198 = *in_m;
        this->out_darts11198 = *in_out;
    }
    ~_task11198Inputs() {}
};
class TP11198;
/*Class containing the inputs passed to task11203*/
class _task11203Inputs {
public:
    COMPLEX* in_darts11203 /*OMP_FIRSTPRIVATE*/;
    int m_darts11203 /*OMP_FIRSTPRIVATE*/;
    COMPLEX* out_darts11203 /*OMP_FIRSTPRIVATE*/;
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task11203Inputs() {}
    _task11203Inputs(
        COMPLEX** in_in, int* in_m, COMPLEX** in_out, darts::Codelet* in_nextSyncCodelet)
        : taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
        this->in_darts11203 = *in_in;
        this->m_darts11203 = *in_m;
        this->out_darts11203 = *in_out;
    }
    ~_task11203Inputs() {}
};
class TP11203;
/*Class containing the inputs passed to task11226*/
class _task11226Inputs {
public:
    COMPLEX* W_darts11226 /*OMP_FIRSTPRIVATE*/;
    COMPLEX* in_darts11226 /*OMP_FIRSTPRIVATE*/;
    int m_darts11226 /*OMP_FIRSTPRIVATE*/;
    int n_darts11226 /*OMP_FIRSTPRIVATE*/;
    int nW_darts11226 /*OMP_FIRSTPRIVATE*/;
    COMPLEX* out_darts11226 /*OMP_FIRSTPRIVATE*/;
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task11226Inputs() {}
    _task11226Inputs(COMPLEX** in_W, COMPLEX** in_in, int* in_m, int* in_n, int* in_nW,
        COMPLEX** in_out, darts::Codelet* in_nextSyncCodelet)
        : taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
        this->W_darts11226 = *in_W;
        this->in_darts11226 = *in_in;
        this->m_darts11226 = *in_m;
        this->n_darts11226 = *in_n;
        this->nW_darts11226 = *in_nW;
        this->out_darts11226 = *in_out;
    }
    ~_task11226Inputs() {}
};
class TP11226;
/*Class containing the inputs passed to task11232*/
class _task11232Inputs {
public:
    COMPLEX* W_darts11232 /*OMP_FIRSTPRIVATE*/;
    COMPLEX* in_darts11232 /*OMP_FIRSTPRIVATE*/;
    int m_darts11232 /*OMP_FIRSTPRIVATE*/;
    int n_darts11232 /*OMP_FIRSTPRIVATE*/;
    int nW_darts11232 /*OMP_FIRSTPRIVATE*/;
    COMPLEX* out_darts11232 /*OMP_FIRSTPRIVATE*/;
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task11232Inputs() {}
    _task11232Inputs(COMPLEX** in_W, COMPLEX** in_in, int* in_m, int* in_n, int* in_nW,
        COMPLEX** in_out, darts::Codelet* in_nextSyncCodelet)
        : taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
        this->W_darts11232 = *in_W;
        this->in_darts11232 = *in_in;
        this->m_darts11232 = *in_m;
        this->n_darts11232 = *in_n;
        this->nW_darts11232 = *in_nW;
        this->out_darts11232 = *in_out;
    }
    ~_task11232Inputs() {}
};
class TP11232;
/*Class containing the inputs passed to task11238*/
class _task11238Inputs {
public:
    COMPLEX* W_darts11238 /*OMP_FIRSTPRIVATE*/;
    COMPLEX* in_darts11238 /*OMP_FIRSTPRIVATE*/;
    int m_darts11238 /*OMP_FIRSTPRIVATE*/;
    int n_darts11238 /*OMP_FIRSTPRIVATE*/;
    int nW_darts11238 /*OMP_FIRSTPRIVATE*/;
    COMPLEX* out_darts11238 /*OMP_FIRSTPRIVATE*/;
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task11238Inputs() {}
    _task11238Inputs(COMPLEX** in_W, COMPLEX** in_in, int* in_m, int* in_n, int* in_nW,
        COMPLEX** in_out, darts::Codelet* in_nextSyncCodelet)
        : taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
        this->W_darts11238 = *in_W;
        this->in_darts11238 = *in_in;
        this->m_darts11238 = *in_m;
        this->n_darts11238 = *in_n;
        this->nW_darts11238 = *in_nW;
        this->out_darts11238 = *in_out;
    }
    ~_task11238Inputs() {}
};
class TP11238;
/*Class containing the inputs passed to task11244*/
class _task11244Inputs {
public:
    COMPLEX* W_darts11244 /*OMP_FIRSTPRIVATE*/;
    COMPLEX* in_darts11244 /*OMP_FIRSTPRIVATE*/;
    int m_darts11244 /*OMP_FIRSTPRIVATE*/;
    int n_darts11244 /*OMP_FIRSTPRIVATE*/;
    int nW_darts11244 /*OMP_FIRSTPRIVATE*/;
    COMPLEX* out_darts11244 /*OMP_FIRSTPRIVATE*/;
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task11244Inputs() {}
    _task11244Inputs(COMPLEX** in_W, COMPLEX** in_in, int* in_m, int* in_n, int* in_nW,
        COMPLEX** in_out, darts::Codelet* in_nextSyncCodelet)
        : taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
        this->W_darts11244 = *in_W;
        this->in_darts11244 = *in_in;
        this->m_darts11244 = *in_m;
        this->n_darts11244 = *in_n;
        this->nW_darts11244 = *in_nW;
        this->out_darts11244 = *in_out;
    }
    ~_task11244Inputs() {}
};
class TP11244;
/*Class containing the inputs passed to task11250*/
class _task11250Inputs {
public:
    COMPLEX* W_darts11250 /*OMP_FIRSTPRIVATE*/;
    COMPLEX* in_darts11250 /*OMP_FIRSTPRIVATE*/;
    int m_darts11250 /*OMP_FIRSTPRIVATE*/;
    int n_darts11250 /*OMP_FIRSTPRIVATE*/;
    int nW_darts11250 /*OMP_FIRSTPRIVATE*/;
    COMPLEX* out_darts11250 /*OMP_FIRSTPRIVATE*/;
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task11250Inputs() {}
    _task11250Inputs(COMPLEX** in_W, COMPLEX** in_in, int* in_m, int* in_n, int* in_nW,
        COMPLEX** in_out, darts::Codelet* in_nextSyncCodelet)
        : taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
        this->W_darts11250 = *in_W;
        this->in_darts11250 = *in_in;
        this->m_darts11250 = *in_m;
        this->n_darts11250 = *in_n;
        this->nW_darts11250 = *in_nW;
        this->out_darts11250 = *in_out;
    }
    ~_task11250Inputs() {}
};
class TP11250;
/*Class containing the inputs passed to task11254*/
class _task11254Inputs {
public:
    COMPLEX* W_darts11254 /*OMP_FIRSTPRIVATE*/;
    COMPLEX* in_darts11254 /*OMP_FIRSTPRIVATE*/;
    int m_darts11254 /*OMP_FIRSTPRIVATE*/;
    int n_darts11254 /*OMP_FIRSTPRIVATE*/;
    int nW_darts11254 /*OMP_FIRSTPRIVATE*/;
    COMPLEX* out_darts11254 /*OMP_FIRSTPRIVATE*/;
    int r_darts11254 /*OMP_FIRSTPRIVATE*/;
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task11254Inputs() {}
    _task11254Inputs(COMPLEX** in_W, COMPLEX** in_in, int* in_m, int* in_n, int* in_nW,
        COMPLEX** in_out, int* in_r, darts::Codelet* in_nextSyncCodelet)
        : taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
        this->W_darts11254 = *in_W;
        this->in_darts11254 = *in_in;
        this->m_darts11254 = *in_m;
        this->n_darts11254 = *in_n;
        this->nW_darts11254 = *in_nW;
        this->out_darts11254 = *in_out;
        this->r_darts11254 = *in_r;
    }
    ~_task11254Inputs() {}
};
class TP11254;
class TP11383;
class TP11384;
/*Class containing the inputs passed to task11385*/
class _task11385Inputs {
public:
    COMPLEX** W_darts11385; /*OMP_SHARED*/
    int* n_darts11385; /*OMP_SHARED*/
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task11385Inputs() {}
    _task11385Inputs(COMPLEX** in_W, int* in_n, darts::Codelet* in_nextSyncCodelet)
        : W_darts11385(in_W)
        , n_darts11385(in_n)
        , taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
    }
    ~_task11385Inputs() {}
};
class TP11385;
class TP11407;
class TP11408;
/*Class containing the inputs passed to task11409*/
class _task11409Inputs {
public:
    COMPLEX** W_darts11409; /*OMP_SHARED*/
    int* factors_darts11409; /*OMP_SHARED*/
    int factors_outer11409_size;
    COMPLEX** in_darts11409; /*OMP_SHARED*/
    int* n_darts11409; /*OMP_SHARED*/
    COMPLEX** out_darts11409; /*OMP_SHARED*/
    /*Termination flag to be used by in the depend clauses*/
    bool taskCompleted;
    /*Synchronization codelet for this task*/
    darts::Codelet* nextSyncCodelet;
    _task11409Inputs() {}
    _task11409Inputs(COMPLEX** in_W, int* in_factors, int in_factors_outer_size, COMPLEX** in_in,
        int* in_n, COMPLEX** in_out, darts::Codelet* in_nextSyncCodelet)
        : W_darts11409(in_W)
        , factors_darts11409(in_factors)
        , factors_outer11409_size(in_factors_outer_size)
        , in_darts11409(in_in)
        , n_darts11409(in_n)
        , out_darts11409(in_out)
        , taskCompleted(false)
        , nextSyncCodelet(in_nextSyncCodelet)
    {
    }
    ~_task11409Inputs() {}
};
class TP11409;
extern int DARTS_CODELETS_MULT;
extern int NUMTPS;
extern size_t numOfCUs;
extern darts::Codelet* RuntimeFinalCodelet;
extern darts::ThreadAffinity* affin;
extern bool affinMaskRes;
extern darts::Runtime* myDARTSRuntime;
extern std::vector<std::vector<void*> > threadFunctionStack;
extern size_t ompNumThreads;
extern int ompSchedulePolicy;
extern int ompScheduleChunk;
extern void omp_set_num_threads(unsigned long numThreadsToSet);
extern int omp_get_num_threads();
extern int omp_get_max_threads();
extern int omp_get_num_procs();
extern double omp_get_wtime();
extern void omp_init_lock(omp_lock_t* lock);
extern void omp_destroy_lock(omp_lock_t* lock);
extern void omp_set_lock(omp_lock_t* lock);
extern void omp_unset_lock(omp_lock_t* lock);
/*TP15: compute_w_coefficients*/
class TP15 : public ompTP {
public:
    class _checkInCodelets111 : public darts::Codelet {
    public:
        TP15* myTP;
        TP15* inputsTPParent;
        _checkInCodelets111()
            : darts::Codelet()
        {
        }
        _checkInCodelets111(uint32_t dep, uint32_t res, TP15* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets114 : public darts::Codelet {
    public:
        TP15* myTP;
        TP15* inputsTPParent;
        _checkInCodelets114()
            : darts::Codelet()
        {
        }
        _checkInCodelets114(uint32_t dep, uint32_t res, TP15* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets146 : public darts::Codelet {
    public:
        TP15* myTP;
        TP15* inputsTPParent;
        _checkInCodelets146()
            : darts::Codelet()
        {
        }
        _checkInCodelets146(uint32_t dep, uint32_t res, TP15* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets149 : public darts::Codelet {
    public:
        TP15* myTP;
        TP15* inputsTPParent;
        _checkInCodelets149()
            : darts::Codelet()
        {
        }
        _checkInCodelets149(uint32_t dep, uint32_t res, TP15* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets151 : public darts::Codelet {
    public:
        TP15* myTP;
        TP15* inputsTPParent;
        _checkInCodelets151()
            : darts::Codelet()
        {
        }
        _checkInCodelets151(uint32_t dep, uint32_t res, TP15* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets154 : public darts::Codelet {
    public:
        TP15* inputsTPParent;
        _barrierCodelets154()
            : darts::Codelet()
        {
        }
        _barrierCodelets154(uint32_t dep, uint32_t res, TP15* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP15** ptrToThisFunctionTP;
    TP15* inputsTPParent;
    TP15* controlTPParent;
    darts::Codelet** nextCodeletscompute_w_coefficients;
    darts::Codelet** nextSyncCodeletscompute_w_coefficients;
    int* n_darts15 /*VARIABLE*/;
    int* a_darts15 /*VARIABLE*/;
    int* b_darts15 /*VARIABLE*/;
    COMPLEX** W_darts15 /*VARIABLE*/;
    int* ab_darts15 /*VARIABLE*/;
    REAL* c_darts15 /*VARIABLE*/;
    int* k_darts15 /*VARIABLE*/;
    REAL* s_darts15 /*VARIABLE*/;
    double* twoPiOverN_darts15 /*VARIABLE*/;
    _task149Inputs** task149Inputs;
    _task151Inputs** task151Inputs;
    _checkInCodelets111* checkInCodelets111;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets111* firstCodelet;
#endif
    _checkInCodelets114* checkInCodelets114;
    _checkInCodelets146* checkInCodelets146;
    _checkInCodelets149* checkInCodelets149;
    _checkInCodelets151* checkInCodelets151;
    _barrierCodelets154* barrierCodelets154;
    TP15(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_mainNextCodelet,
        darts::Codelet* in_mainSyncCodelet, TP15** in_ptrToThisFunctionTP, int in_n, int in_a,
        int in_b, COMPLEX* in_W);
    ~TP15();
    void setNewInputs(int in_n, int in_a, int in_b, COMPLEX* in_W, size_t codeletID);
};
/*TP149: OMPTaskDirective*/
class TP149 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets150 : public darts::Codelet {
    public:
        TP149* myTP;
        TP149* inputsTPParent;
        _task149Inputs* taskInputs;
        _checkInCodelets150()
            : darts::Codelet()
        {
        }
        _checkInCodelets150(uint32_t dep, uint32_t res, TP149* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task149Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets11539 : public darts::Codelet {
    public:
        TP149* myTP;
        TP149* inputsTPParent;
        _task149Inputs* taskInputs;
        _checkInCodelets11539()
            : darts::Codelet()
        {
        }
        _checkInCodelets11539(uint32_t dep, uint32_t res, TP149* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task149Inputs)
        {
        }
        void fire(void);
    };
    TP15* TPParent;
    TP149* controlTPParent;
    TP149* inputsTPParent;
    _task149Inputs* task149Inputs;
    TP_compute_w_coefficients* TP150Ptr;
    int TP150_alreadyLaunched;
    _checkInCodelets150 checkInCodelets150;
    _checkInCodelets11539 checkInCodelets11539;
    TP149(int in_numThreads, int in_mainCodeletID, TP15* in_TPParent,
        _task149Inputs* in_task149Inputs);
    ~TP149();
};
/*TP151: OMPTaskDirective*/
class TP151 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets152 : public darts::Codelet {
    public:
        TP151* myTP;
        TP151* inputsTPParent;
        _task151Inputs* taskInputs;
        _checkInCodelets152()
            : darts::Codelet()
        {
        }
        _checkInCodelets152(uint32_t dep, uint32_t res, TP151* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task151Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets11540 : public darts::Codelet {
    public:
        TP151* myTP;
        TP151* inputsTPParent;
        _task151Inputs* taskInputs;
        _checkInCodelets11540()
            : darts::Codelet()
        {
        }
        _checkInCodelets11540(uint32_t dep, uint32_t res, TP151* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task151Inputs)
        {
        }
        void fire(void);
    };
    TP15* TPParent;
    TP151* controlTPParent;
    TP151* inputsTPParent;
    _task151Inputs* task151Inputs;
    TP_compute_w_coefficients* TP152Ptr;
    int TP152_alreadyLaunched;
    _checkInCodelets152 checkInCodelets152;
    _checkInCodelets11540 checkInCodelets11540;
    TP151(int in_numThreads, int in_mainCodeletID, TP15* in_TPParent,
        _task151Inputs* in_task151Inputs);
    ~TP151();
};
/*TP18: unshuffle*/
class TP18 : public ompTP {
public:
    class _checkInCodelets236 : public darts::Codelet {
    public:
        TP18* myTP;
        TP18* inputsTPParent;
        _checkInCodelets236()
            : darts::Codelet()
        {
        }
        _checkInCodelets236(uint32_t dep, uint32_t res, TP18* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets242 : public darts::Codelet {
    public:
        TP18* myTP;
        TP18* inputsTPParent;
        _checkInCodelets242()
            : darts::Codelet()
        {
        }
        _checkInCodelets242(uint32_t dep, uint32_t res, TP18* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets245 : public darts::Codelet {
    public:
        TP18* myTP;
        TP18* inputsTPParent;
        _checkInCodelets245()
            : darts::Codelet()
        {
        }
        _checkInCodelets245(uint32_t dep, uint32_t res, TP18* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets287 : public darts::Codelet {
    public:
        TP18* myTP;
        TP18* inputsTPParent;
        _checkInCodelets287()
            : darts::Codelet()
        {
        }
        _checkInCodelets287(uint32_t dep, uint32_t res, TP18* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets290 : public darts::Codelet {
    public:
        TP18* myTP;
        TP18* inputsTPParent;
        _checkInCodelets290()
            : darts::Codelet()
        {
        }
        _checkInCodelets290(uint32_t dep, uint32_t res, TP18* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets292 : public darts::Codelet {
    public:
        TP18* myTP;
        TP18* inputsTPParent;
        _checkInCodelets292()
            : darts::Codelet()
        {
        }
        _checkInCodelets292(uint32_t dep, uint32_t res, TP18* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets294 : public darts::Codelet {
    public:
        TP18* inputsTPParent;
        _barrierCodelets294()
            : darts::Codelet()
        {
        }
        _barrierCodelets294(uint32_t dep, uint32_t res, TP18* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP18** ptrToThisFunctionTP;
    TP18* inputsTPParent;
    TP18* controlTPParent;
    darts::Codelet** nextCodeletsunshuffle;
    darts::Codelet** nextSyncCodeletsunshuffle;
    int* a_darts18 /*VARIABLE*/;
    int* b_darts18 /*VARIABLE*/;
    COMPLEX** in_darts18 /*VARIABLE*/;
    COMPLEX** out_darts18 /*VARIABLE*/;
    int* r_darts18 /*VARIABLE*/;
    int* m_darts18 /*VARIABLE*/;
    int* ab_darts18 /*VARIABLE*/;
    int* i_darts18 /*VARIABLE*/;
    COMPLEX** ip_darts18 /*VARIABLE*/;
    int* j_darts18 /*VARIABLE*/;
    COMPLEX** jp_darts18 /*VARIABLE*/;
    int* r4_darts18 /*VARIABLE*/;
    _task290Inputs** task290Inputs;
    _task292Inputs** task292Inputs;
    _checkInCodelets236* checkInCodelets236;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets236* firstCodelet;
#endif
    _checkInCodelets242* checkInCodelets242;
    _checkInCodelets245* checkInCodelets245;
    _checkInCodelets287* checkInCodelets287;
    _checkInCodelets290* checkInCodelets290;
    _checkInCodelets292* checkInCodelets292;
    _barrierCodelets294* barrierCodelets294;
    TP18(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_mainNextCodelet,
        darts::Codelet* in_mainSyncCodelet, TP18** in_ptrToThisFunctionTP, int in_a, int in_b,
        COMPLEX* in_in, COMPLEX* in_out, int in_r, int in_m);
    ~TP18();
    void setNewInputs(
        int in_a, int in_b, COMPLEX* in_in, COMPLEX* in_out, int in_r, int in_m, size_t codeletID);
};
/*TP290: OMPTaskDirective*/
class TP290 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets291 : public darts::Codelet {
    public:
        TP290* myTP;
        TP290* inputsTPParent;
        _task290Inputs* taskInputs;
        _checkInCodelets291()
            : darts::Codelet()
        {
        }
        _checkInCodelets291(uint32_t dep, uint32_t res, TP290* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task290Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets11520 : public darts::Codelet {
    public:
        TP290* myTP;
        TP290* inputsTPParent;
        _task290Inputs* taskInputs;
        _checkInCodelets11520()
            : darts::Codelet()
        {
        }
        _checkInCodelets11520(uint32_t dep, uint32_t res, TP290* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task290Inputs)
        {
        }
        void fire(void);
    };
    TP18* TPParent;
    TP290* controlTPParent;
    TP290* inputsTPParent;
    _task290Inputs* task290Inputs;
    TP_unshuffle* TP291Ptr;
    int TP291_alreadyLaunched;
    _checkInCodelets291 checkInCodelets291;
    _checkInCodelets11520 checkInCodelets11520;
    TP290(int in_numThreads, int in_mainCodeletID, TP18* in_TPParent,
        _task290Inputs* in_task290Inputs);
    ~TP290();
};
/*TP292: OMPTaskDirective*/
class TP292 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets293 : public darts::Codelet {
    public:
        TP292* myTP;
        TP292* inputsTPParent;
        _task292Inputs* taskInputs;
        _checkInCodelets293()
            : darts::Codelet()
        {
        }
        _checkInCodelets293(uint32_t dep, uint32_t res, TP292* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task292Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets11521 : public darts::Codelet {
    public:
        TP292* myTP;
        TP292* inputsTPParent;
        _task292Inputs* taskInputs;
        _checkInCodelets11521()
            : darts::Codelet()
        {
        }
        _checkInCodelets11521(uint32_t dep, uint32_t res, TP292* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task292Inputs)
        {
        }
        void fire(void);
    };
    TP18* TPParent;
    TP292* controlTPParent;
    TP292* inputsTPParent;
    _task292Inputs* task292Inputs;
    TP_unshuffle* TP293Ptr;
    int TP293_alreadyLaunched;
    _checkInCodelets293 checkInCodelets293;
    _checkInCodelets11521 checkInCodelets11521;
    TP292(int in_numThreads, int in_mainCodeletID, TP18* in_TPParent,
        _task292Inputs* in_task292Inputs);
    ~TP292();
};
/*TP21: fft_twiddle_gen*/
class TP21 : public ompTP {
public:
    class _checkInCodelets415 : public darts::Codelet {
    public:
        TP21* myTP;
        TP21* inputsTPParent;
        _checkInCodelets415()
            : darts::Codelet()
        {
        }
        _checkInCodelets415(uint32_t dep, uint32_t res, TP21* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets432 : public darts::Codelet {
    public:
        TP21* inputsTPParent;
        _barrierCodelets432()
            : darts::Codelet()
        {
        }
        _barrierCodelets432(uint32_t dep, uint32_t res, TP21* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets418 : public darts::Codelet {
    public:
        TP21* myTP;
        TP21* inputsTPParent;
        _checkInCodelets418()
            : darts::Codelet()
        {
        }
        _checkInCodelets418(uint32_t dep, uint32_t res, TP21* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets425 : public darts::Codelet {
    public:
        TP21* myTP;
        TP21* inputsTPParent;
        _checkInCodelets425()
            : darts::Codelet()
        {
        }
        _checkInCodelets425(uint32_t dep, uint32_t res, TP21* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets428 : public darts::Codelet {
    public:
        TP21* myTP;
        TP21* inputsTPParent;
        _checkInCodelets428()
            : darts::Codelet()
        {
        }
        _checkInCodelets428(uint32_t dep, uint32_t res, TP21* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets430 : public darts::Codelet {
    public:
        TP21* myTP;
        TP21* inputsTPParent;
        _checkInCodelets430()
            : darts::Codelet()
        {
        }
        _checkInCodelets430(uint32_t dep, uint32_t res, TP21* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP21** ptrToThisFunctionTP;
    TP21* inputsTPParent;
    TP21* controlTPParent;
    darts::Codelet** nextCodeletsfft_twiddle_gen;
    darts::Codelet** nextSyncCodeletsfft_twiddle_gen;
    int* i_darts21 /*VARIABLE*/;
    int* i1_darts21 /*VARIABLE*/;
    COMPLEX** in_darts21 /*VARIABLE*/;
    COMPLEX** out_darts21 /*VARIABLE*/;
    COMPLEX** W_darts21 /*VARIABLE*/;
    int* nW_darts21 /*VARIABLE*/;
    int* nWdn_darts21 /*VARIABLE*/;
    int* r_darts21 /*VARIABLE*/;
    int* m_darts21 /*VARIABLE*/;
    int* i2_darts21 /*VARIABLE*/;
    _task418Inputs** task418Inputs;
    _task428Inputs** task428Inputs;
    _task430Inputs** task430Inputs;
    _checkInCodelets415* checkInCodelets415;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets415* firstCodelet;
#endif
    _barrierCodelets432* barrierCodelets432;
    _checkInCodelets418* checkInCodelets418;
    _checkInCodelets425* checkInCodelets425;
    _checkInCodelets428* checkInCodelets428;
    _checkInCodelets430* checkInCodelets430;
    TP21(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_mainNextCodelet,
        darts::Codelet* in_mainSyncCodelet, TP21** in_ptrToThisFunctionTP, int in_i, int in_i1,
        COMPLEX* in_in, COMPLEX* in_out, COMPLEX* in_W, int in_nW, int in_nWdn, int in_r, int in_m);
    ~TP21();
    void setNewInputs(int in_i, int in_i1, COMPLEX* in_in, COMPLEX* in_out, COMPLEX* in_W,
        int in_nW, int in_nWdn, int in_r, int in_m, size_t codeletID);
};
/*TP418: OMPTaskDirective*/
class TP418 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets419 : public darts::Codelet {
    public:
        TP418* myTP;
        TP418* inputsTPParent;
        _task418Inputs* taskInputs;
        _checkInCodelets419()
            : darts::Codelet()
        {
        }
        _checkInCodelets419(uint32_t dep, uint32_t res, TP418* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task418Inputs)
        {
        }
        void fire(void);
    };
    TP21* TPParent;
    TP418* controlTPParent;
    TP418* inputsTPParent;
    _task418Inputs* task418Inputs;
    _checkInCodelets419 checkInCodelets419;
    TP418(int in_numThreads, int in_mainCodeletID, TP21* in_TPParent,
        _task418Inputs* in_task418Inputs);
    ~TP418();
};
/*TP428: OMPTaskDirective*/
class TP428 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets429 : public darts::Codelet {
    public:
        TP428* myTP;
        TP428* inputsTPParent;
        _task428Inputs* taskInputs;
        _checkInCodelets429()
            : darts::Codelet()
        {
        }
        _checkInCodelets429(uint32_t dep, uint32_t res, TP428* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task428Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets11516 : public darts::Codelet {
    public:
        TP428* myTP;
        TP428* inputsTPParent;
        _task428Inputs* taskInputs;
        _checkInCodelets11516()
            : darts::Codelet()
        {
        }
        _checkInCodelets11516(uint32_t dep, uint32_t res, TP428* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task428Inputs)
        {
        }
        void fire(void);
    };
    TP21* TPParent;
    TP428* controlTPParent;
    TP428* inputsTPParent;
    _task428Inputs* task428Inputs;
    TP_fft_twiddle_gen* TP429Ptr;
    int TP429_alreadyLaunched;
    _checkInCodelets429 checkInCodelets429;
    _checkInCodelets11516 checkInCodelets11516;
    TP428(int in_numThreads, int in_mainCodeletID, TP21* in_TPParent,
        _task428Inputs* in_task428Inputs);
    ~TP428();
};
/*TP430: OMPTaskDirective*/
class TP430 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets431 : public darts::Codelet {
    public:
        TP430* myTP;
        TP430* inputsTPParent;
        _task430Inputs* taskInputs;
        _checkInCodelets431()
            : darts::Codelet()
        {
        }
        _checkInCodelets431(uint32_t dep, uint32_t res, TP430* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task430Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets11517 : public darts::Codelet {
    public:
        TP430* myTP;
        TP430* inputsTPParent;
        _task430Inputs* taskInputs;
        _checkInCodelets11517()
            : darts::Codelet()
        {
        }
        _checkInCodelets11517(uint32_t dep, uint32_t res, TP430* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task430Inputs)
        {
        }
        void fire(void);
    };
    TP21* TPParent;
    TP430* controlTPParent;
    TP430* inputsTPParent;
    _task430Inputs* task430Inputs;
    TP_fft_twiddle_gen* TP431Ptr;
    int TP431_alreadyLaunched;
    _checkInCodelets431 checkInCodelets431;
    _checkInCodelets11517 checkInCodelets11517;
    TP430(int in_numThreads, int in_mainCodeletID, TP21* in_TPParent,
        _task430Inputs* in_task430Inputs);
    ~TP430();
};
/*TP24: fft_twiddle_2*/
class TP24 : public ompTP {
public:
    class _checkInCodelets483 : public darts::Codelet {
    public:
        TP24* myTP;
        TP24* inputsTPParent;
        _checkInCodelets483()
            : darts::Codelet()
        {
        }
        _checkInCodelets483(uint32_t dep, uint32_t res, TP24* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets486 : public darts::Codelet {
    public:
        TP24* myTP;
        TP24* inputsTPParent;
        _checkInCodelets486()
            : darts::Codelet()
        {
        }
        _checkInCodelets486(uint32_t dep, uint32_t res, TP24* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets555 : public darts::Codelet {
    public:
        TP24* myTP;
        TP24* inputsTPParent;
        _checkInCodelets555()
            : darts::Codelet()
        {
        }
        _checkInCodelets555(uint32_t dep, uint32_t res, TP24* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets558 : public darts::Codelet {
    public:
        TP24* myTP;
        TP24* inputsTPParent;
        _checkInCodelets558()
            : darts::Codelet()
        {
        }
        _checkInCodelets558(uint32_t dep, uint32_t res, TP24* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets560 : public darts::Codelet {
    public:
        TP24* myTP;
        TP24* inputsTPParent;
        _checkInCodelets560()
            : darts::Codelet()
        {
        }
        _checkInCodelets560(uint32_t dep, uint32_t res, TP24* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets562 : public darts::Codelet {
    public:
        TP24* inputsTPParent;
        _barrierCodelets562()
            : darts::Codelet()
        {
        }
        _barrierCodelets562(uint32_t dep, uint32_t res, TP24* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP24** ptrToThisFunctionTP;
    TP24* inputsTPParent;
    TP24* controlTPParent;
    darts::Codelet** nextCodeletsfft_twiddle_2;
    darts::Codelet** nextSyncCodeletsfft_twiddle_2;
    int* a_darts24 /*VARIABLE*/;
    int* b_darts24 /*VARIABLE*/;
    COMPLEX** in_darts24 /*VARIABLE*/;
    COMPLEX** out_darts24 /*VARIABLE*/;
    COMPLEX** W_darts24 /*VARIABLE*/;
    int* nW_darts24 /*VARIABLE*/;
    int* nWdn_darts24 /*VARIABLE*/;
    int* m_darts24 /*VARIABLE*/;
    int* ab_darts24 /*VARIABLE*/;
    int* i_darts24 /*VARIABLE*/;
    REAL* i1_0_darts24 /*VARIABLE*/;
    REAL* i1_1_darts24 /*VARIABLE*/;
    COMPLEX** jp_darts24 /*VARIABLE*/;
    COMPLEX** kp_darts24 /*VARIABLE*/;
    int* l1_darts24 /*VARIABLE*/;
    REAL* r1_0_darts24 /*VARIABLE*/;
    REAL* r1_1_darts24 /*VARIABLE*/;
    REAL* tmpi_darts24 /*VARIABLE*/;
    REAL* tmpr_darts24 /*VARIABLE*/;
    REAL* wi_darts24 /*VARIABLE*/;
    REAL* wr_darts24 /*VARIABLE*/;
    _task558Inputs** task558Inputs;
    _task560Inputs** task560Inputs;
    _checkInCodelets483* checkInCodelets483;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets483* firstCodelet;
#endif
    _checkInCodelets486* checkInCodelets486;
    _checkInCodelets555* checkInCodelets555;
    _checkInCodelets558* checkInCodelets558;
    _checkInCodelets560* checkInCodelets560;
    _barrierCodelets562* barrierCodelets562;
    TP24(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_mainNextCodelet,
        darts::Codelet* in_mainSyncCodelet, TP24** in_ptrToThisFunctionTP, int in_a, int in_b,
        COMPLEX* in_in, COMPLEX* in_out, COMPLEX* in_W, int in_nW, int in_nWdn, int in_m);
    ~TP24();
    void setNewInputs(int in_a, int in_b, COMPLEX* in_in, COMPLEX* in_out, COMPLEX* in_W, int in_nW,
        int in_nWdn, int in_m, size_t codeletID);
};
/*TP558: OMPTaskDirective*/
class TP558 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets559 : public darts::Codelet {
    public:
        TP558* myTP;
        TP558* inputsTPParent;
        _task558Inputs* taskInputs;
        _checkInCodelets559()
            : darts::Codelet()
        {
        }
        _checkInCodelets559(uint32_t dep, uint32_t res, TP558* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task558Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets11541 : public darts::Codelet {
    public:
        TP558* myTP;
        TP558* inputsTPParent;
        _task558Inputs* taskInputs;
        _checkInCodelets11541()
            : darts::Codelet()
        {
        }
        _checkInCodelets11541(uint32_t dep, uint32_t res, TP558* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task558Inputs)
        {
        }
        void fire(void);
    };
    TP24* TPParent;
    TP558* controlTPParent;
    TP558* inputsTPParent;
    _task558Inputs* task558Inputs;
    TP_fft_twiddle_2* TP559Ptr;
    int TP559_alreadyLaunched;
    _checkInCodelets559 checkInCodelets559;
    _checkInCodelets11541 checkInCodelets11541;
    TP558(int in_numThreads, int in_mainCodeletID, TP24* in_TPParent,
        _task558Inputs* in_task558Inputs);
    ~TP558();
};
/*TP560: OMPTaskDirective*/
class TP560 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets561 : public darts::Codelet {
    public:
        TP560* myTP;
        TP560* inputsTPParent;
        _task560Inputs* taskInputs;
        _checkInCodelets561()
            : darts::Codelet()
        {
        }
        _checkInCodelets561(uint32_t dep, uint32_t res, TP560* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task560Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets11542 : public darts::Codelet {
    public:
        TP560* myTP;
        TP560* inputsTPParent;
        _task560Inputs* taskInputs;
        _checkInCodelets11542()
            : darts::Codelet()
        {
        }
        _checkInCodelets11542(uint32_t dep, uint32_t res, TP560* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task560Inputs)
        {
        }
        void fire(void);
    };
    TP24* TPParent;
    TP560* controlTPParent;
    TP560* inputsTPParent;
    _task560Inputs* task560Inputs;
    TP_fft_twiddle_2* TP561Ptr;
    int TP561_alreadyLaunched;
    _checkInCodelets561 checkInCodelets561;
    _checkInCodelets11542 checkInCodelets11542;
    TP560(int in_numThreads, int in_mainCodeletID, TP24* in_TPParent,
        _task560Inputs* in_task560Inputs);
    ~TP560();
};
/*TP26: fft_unshuffle_2*/
class TP26 : public ompTP {
public:
    class _checkInCodelets652 : public darts::Codelet {
    public:
        TP26* myTP;
        TP26* inputsTPParent;
        _checkInCodelets652()
            : darts::Codelet()
        {
        }
        _checkInCodelets652(uint32_t dep, uint32_t res, TP26* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets655 : public darts::Codelet {
    public:
        TP26* myTP;
        TP26* inputsTPParent;
        _checkInCodelets655()
            : darts::Codelet()
        {
        }
        _checkInCodelets655(uint32_t dep, uint32_t res, TP26* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets673 : public darts::Codelet {
    public:
        TP26* myTP;
        TP26* inputsTPParent;
        _checkInCodelets673()
            : darts::Codelet()
        {
        }
        _checkInCodelets673(uint32_t dep, uint32_t res, TP26* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets676 : public darts::Codelet {
    public:
        TP26* myTP;
        TP26* inputsTPParent;
        _checkInCodelets676()
            : darts::Codelet()
        {
        }
        _checkInCodelets676(uint32_t dep, uint32_t res, TP26* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets678 : public darts::Codelet {
    public:
        TP26* myTP;
        TP26* inputsTPParent;
        _checkInCodelets678()
            : darts::Codelet()
        {
        }
        _checkInCodelets678(uint32_t dep, uint32_t res, TP26* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets680 : public darts::Codelet {
    public:
        TP26* inputsTPParent;
        _barrierCodelets680()
            : darts::Codelet()
        {
        }
        _barrierCodelets680(uint32_t dep, uint32_t res, TP26* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP26** ptrToThisFunctionTP;
    TP26* inputsTPParent;
    TP26* controlTPParent;
    darts::Codelet** nextCodeletsfft_unshuffle_2;
    darts::Codelet** nextSyncCodeletsfft_unshuffle_2;
    int* a_darts26 /*VARIABLE*/;
    int* b_darts26 /*VARIABLE*/;
    COMPLEX** in_darts26 /*VARIABLE*/;
    COMPLEX** out_darts26 /*VARIABLE*/;
    int* m_darts26 /*VARIABLE*/;
    int* ab_darts26 /*VARIABLE*/;
    int* i_darts26 /*VARIABLE*/;
    COMPLEX** ip_darts26 /*VARIABLE*/;
    COMPLEX** jp_darts26 /*VARIABLE*/;
    _task676Inputs** task676Inputs;
    _task678Inputs** task678Inputs;
    _checkInCodelets652* checkInCodelets652;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets652* firstCodelet;
#endif
    _checkInCodelets655* checkInCodelets655;
    _checkInCodelets673* checkInCodelets673;
    _checkInCodelets676* checkInCodelets676;
    _checkInCodelets678* checkInCodelets678;
    _barrierCodelets680* barrierCodelets680;
    TP26(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_mainNextCodelet,
        darts::Codelet* in_mainSyncCodelet, TP26** in_ptrToThisFunctionTP, int in_a, int in_b,
        COMPLEX* in_in, COMPLEX* in_out, int in_m);
    ~TP26();
    void setNewInputs(
        int in_a, int in_b, COMPLEX* in_in, COMPLEX* in_out, int in_m, size_t codeletID);
};
/*TP676: OMPTaskDirective*/
class TP676 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets677 : public darts::Codelet {
    public:
        TP676* myTP;
        TP676* inputsTPParent;
        _task676Inputs* taskInputs;
        _checkInCodelets677()
            : darts::Codelet()
        {
        }
        _checkInCodelets677(uint32_t dep, uint32_t res, TP676* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task676Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets11545 : public darts::Codelet {
    public:
        TP676* myTP;
        TP676* inputsTPParent;
        _task676Inputs* taskInputs;
        _checkInCodelets11545()
            : darts::Codelet()
        {
        }
        _checkInCodelets11545(uint32_t dep, uint32_t res, TP676* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task676Inputs)
        {
        }
        void fire(void);
    };
    TP26* TPParent;
    TP676* controlTPParent;
    TP676* inputsTPParent;
    _task676Inputs* task676Inputs;
    TP_fft_unshuffle_2* TP677Ptr;
    int TP677_alreadyLaunched;
    _checkInCodelets677 checkInCodelets677;
    _checkInCodelets11545 checkInCodelets11545;
    TP676(int in_numThreads, int in_mainCodeletID, TP26* in_TPParent,
        _task676Inputs* in_task676Inputs);
    ~TP676();
};
/*TP678: OMPTaskDirective*/
class TP678 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets679 : public darts::Codelet {
    public:
        TP678* myTP;
        TP678* inputsTPParent;
        _task678Inputs* taskInputs;
        _checkInCodelets679()
            : darts::Codelet()
        {
        }
        _checkInCodelets679(uint32_t dep, uint32_t res, TP678* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task678Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets11546 : public darts::Codelet {
    public:
        TP678* myTP;
        TP678* inputsTPParent;
        _task678Inputs* taskInputs;
        _checkInCodelets11546()
            : darts::Codelet()
        {
        }
        _checkInCodelets11546(uint32_t dep, uint32_t res, TP678* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task678Inputs)
        {
        }
        void fire(void);
    };
    TP26* TPParent;
    TP678* controlTPParent;
    TP678* inputsTPParent;
    _task678Inputs* task678Inputs;
    TP_fft_unshuffle_2* TP679Ptr;
    int TP679_alreadyLaunched;
    _checkInCodelets679 checkInCodelets679;
    _checkInCodelets11546 checkInCodelets11546;
    TP678(int in_numThreads, int in_mainCodeletID, TP26* in_TPParent,
        _task678Inputs* in_task678Inputs);
    ~TP678();
};
/*TP29: fft_twiddle_4*/
class TP29 : public ompTP {
public:
    class _checkInCodelets798 : public darts::Codelet {
    public:
        TP29* myTP;
        TP29* inputsTPParent;
        _checkInCodelets798()
            : darts::Codelet()
        {
        }
        _checkInCodelets798(uint32_t dep, uint32_t res, TP29* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets801 : public darts::Codelet {
    public:
        TP29* myTP;
        TP29* inputsTPParent;
        _checkInCodelets801()
            : darts::Codelet()
        {
        }
        _checkInCodelets801(uint32_t dep, uint32_t res, TP29* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets962 : public darts::Codelet {
    public:
        TP29* myTP;
        TP29* inputsTPParent;
        _checkInCodelets962()
            : darts::Codelet()
        {
        }
        _checkInCodelets962(uint32_t dep, uint32_t res, TP29* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets965 : public darts::Codelet {
    public:
        TP29* myTP;
        TP29* inputsTPParent;
        _checkInCodelets965()
            : darts::Codelet()
        {
        }
        _checkInCodelets965(uint32_t dep, uint32_t res, TP29* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets967 : public darts::Codelet {
    public:
        TP29* myTP;
        TP29* inputsTPParent;
        _checkInCodelets967()
            : darts::Codelet()
        {
        }
        _checkInCodelets967(uint32_t dep, uint32_t res, TP29* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets969 : public darts::Codelet {
    public:
        TP29* inputsTPParent;
        _barrierCodelets969()
            : darts::Codelet()
        {
        }
        _barrierCodelets969(uint32_t dep, uint32_t res, TP29* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP29** ptrToThisFunctionTP;
    TP29* inputsTPParent;
    TP29* controlTPParent;
    darts::Codelet** nextCodeletsfft_twiddle_4;
    darts::Codelet** nextSyncCodeletsfft_twiddle_4;
    int* a_darts29 /*VARIABLE*/;
    int* b_darts29 /*VARIABLE*/;
    COMPLEX** in_darts29 /*VARIABLE*/;
    COMPLEX** out_darts29 /*VARIABLE*/;
    COMPLEX** W_darts29 /*VARIABLE*/;
    int* nW_darts29 /*VARIABLE*/;
    int* nWdn_darts29 /*VARIABLE*/;
    int* m_darts29 /*VARIABLE*/;
    int* ab_darts29 /*VARIABLE*/;
    int* i_darts29 /*VARIABLE*/;
    REAL* i1_0_darts29 /*VARIABLE*/;
    REAL* i1_1_darts29 /*VARIABLE*/;
    REAL* i1_2_darts29 /*VARIABLE*/;
    REAL* i1_3_darts29 /*VARIABLE*/;
    REAL* i2_0_darts29 /*VARIABLE*/;
    REAL* i2_1_darts29 /*VARIABLE*/;
    REAL* i2_2_darts29 /*VARIABLE*/;
    REAL* i2_3_darts29 /*VARIABLE*/;
    COMPLEX** jp_darts29 /*VARIABLE*/;
    COMPLEX** kp_darts29 /*VARIABLE*/;
    int* l1_darts29 /*VARIABLE*/;
    REAL* r1_0_darts29 /*VARIABLE*/;
    REAL* r1_1_darts29 /*VARIABLE*/;
    REAL* r1_2_darts29 /*VARIABLE*/;
    REAL* r1_3_darts29 /*VARIABLE*/;
    REAL* r2_0_darts29 /*VARIABLE*/;
    REAL* r2_1_darts29 /*VARIABLE*/;
    REAL* r2_2_darts29 /*VARIABLE*/;
    REAL* r2_3_darts29 /*VARIABLE*/;
    REAL* tmpi_darts29 /*VARIABLE*/;
    REAL* tmpr_darts29 /*VARIABLE*/;
    REAL* wi_darts29 /*VARIABLE*/;
    REAL* wr_darts29 /*VARIABLE*/;
    _task965Inputs** task965Inputs;
    _task967Inputs** task967Inputs;
    _checkInCodelets798* checkInCodelets798;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets798* firstCodelet;
#endif
    _checkInCodelets801* checkInCodelets801;
    _checkInCodelets962* checkInCodelets962;
    _checkInCodelets965* checkInCodelets965;
    _checkInCodelets967* checkInCodelets967;
    _barrierCodelets969* barrierCodelets969;
    TP29(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_mainNextCodelet,
        darts::Codelet* in_mainSyncCodelet, TP29** in_ptrToThisFunctionTP, int in_a, int in_b,
        COMPLEX* in_in, COMPLEX* in_out, COMPLEX* in_W, int in_nW, int in_nWdn, int in_m);
    ~TP29();
    void setNewInputs(int in_a, int in_b, COMPLEX* in_in, COMPLEX* in_out, COMPLEX* in_W, int in_nW,
        int in_nWdn, int in_m, size_t codeletID);
};
/*TP965: OMPTaskDirective*/
class TP965 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets966 : public darts::Codelet {
    public:
        TP965* myTP;
        TP965* inputsTPParent;
        _task965Inputs* taskInputs;
        _checkInCodelets966()
            : darts::Codelet()
        {
        }
        _checkInCodelets966(uint32_t dep, uint32_t res, TP965* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task965Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets11543 : public darts::Codelet {
    public:
        TP965* myTP;
        TP965* inputsTPParent;
        _task965Inputs* taskInputs;
        _checkInCodelets11543()
            : darts::Codelet()
        {
        }
        _checkInCodelets11543(uint32_t dep, uint32_t res, TP965* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task965Inputs)
        {
        }
        void fire(void);
    };
    TP29* TPParent;
    TP965* controlTPParent;
    TP965* inputsTPParent;
    _task965Inputs* task965Inputs;
    TP_fft_twiddle_4* TP966Ptr;
    int TP966_alreadyLaunched;
    _checkInCodelets966 checkInCodelets966;
    _checkInCodelets11543 checkInCodelets11543;
    TP965(int in_numThreads, int in_mainCodeletID, TP29* in_TPParent,
        _task965Inputs* in_task965Inputs);
    ~TP965();
};
/*TP967: OMPTaskDirective*/
class TP967 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets968 : public darts::Codelet {
    public:
        TP967* myTP;
        TP967* inputsTPParent;
        _task967Inputs* taskInputs;
        _checkInCodelets968()
            : darts::Codelet()
        {
        }
        _checkInCodelets968(uint32_t dep, uint32_t res, TP967* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task967Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets11544 : public darts::Codelet {
    public:
        TP967* myTP;
        TP967* inputsTPParent;
        _task967Inputs* taskInputs;
        _checkInCodelets11544()
            : darts::Codelet()
        {
        }
        _checkInCodelets11544(uint32_t dep, uint32_t res, TP967* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task967Inputs)
        {
        }
        void fire(void);
    };
    TP29* TPParent;
    TP967* controlTPParent;
    TP967* inputsTPParent;
    _task967Inputs* task967Inputs;
    TP_fft_twiddle_4* TP968Ptr;
    int TP968_alreadyLaunched;
    _checkInCodelets968 checkInCodelets968;
    _checkInCodelets11544 checkInCodelets11544;
    TP967(int in_numThreads, int in_mainCodeletID, TP29* in_TPParent,
        _task967Inputs* in_task967Inputs);
    ~TP967();
};
/*TP31: fft_unshuffle_4*/
class TP31 : public ompTP {
public:
    class _checkInCodelets1151 : public darts::Codelet {
    public:
        TP31* myTP;
        TP31* inputsTPParent;
        _checkInCodelets1151()
            : darts::Codelet()
        {
        }
        _checkInCodelets1151(uint32_t dep, uint32_t res, TP31* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets1154 : public darts::Codelet {
    public:
        TP31* myTP;
        TP31* inputsTPParent;
        _checkInCodelets1154()
            : darts::Codelet()
        {
        }
        _checkInCodelets1154(uint32_t dep, uint32_t res, TP31* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets1181 : public darts::Codelet {
    public:
        TP31* myTP;
        TP31* inputsTPParent;
        _checkInCodelets1181()
            : darts::Codelet()
        {
        }
        _checkInCodelets1181(uint32_t dep, uint32_t res, TP31* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets1184 : public darts::Codelet {
    public:
        TP31* myTP;
        TP31* inputsTPParent;
        _checkInCodelets1184()
            : darts::Codelet()
        {
        }
        _checkInCodelets1184(uint32_t dep, uint32_t res, TP31* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets1186 : public darts::Codelet {
    public:
        TP31* myTP;
        TP31* inputsTPParent;
        _checkInCodelets1186()
            : darts::Codelet()
        {
        }
        _checkInCodelets1186(uint32_t dep, uint32_t res, TP31* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets1188 : public darts::Codelet {
    public:
        TP31* inputsTPParent;
        _barrierCodelets1188()
            : darts::Codelet()
        {
        }
        _barrierCodelets1188(uint32_t dep, uint32_t res, TP31* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP31** ptrToThisFunctionTP;
    TP31* inputsTPParent;
    TP31* controlTPParent;
    darts::Codelet** nextCodeletsfft_unshuffle_4;
    darts::Codelet** nextSyncCodeletsfft_unshuffle_4;
    int* a_darts31 /*VARIABLE*/;
    int* b_darts31 /*VARIABLE*/;
    COMPLEX** in_darts31 /*VARIABLE*/;
    COMPLEX** out_darts31 /*VARIABLE*/;
    int* m_darts31 /*VARIABLE*/;
    int* ab_darts31 /*VARIABLE*/;
    int* i_darts31 /*VARIABLE*/;
    COMPLEX** ip_darts31 /*VARIABLE*/;
    COMPLEX** jp_darts31 /*VARIABLE*/;
    _task1184Inputs** task1184Inputs;
    _task1186Inputs** task1186Inputs;
    _checkInCodelets1151* checkInCodelets1151;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets1151* firstCodelet;
#endif
    _checkInCodelets1154* checkInCodelets1154;
    _checkInCodelets1181* checkInCodelets1181;
    _checkInCodelets1184* checkInCodelets1184;
    _checkInCodelets1186* checkInCodelets1186;
    _barrierCodelets1188* barrierCodelets1188;
    TP31(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_mainNextCodelet,
        darts::Codelet* in_mainSyncCodelet, TP31** in_ptrToThisFunctionTP, int in_a, int in_b,
        COMPLEX* in_in, COMPLEX* in_out, int in_m);
    ~TP31();
    void setNewInputs(
        int in_a, int in_b, COMPLEX* in_in, COMPLEX* in_out, int in_m, size_t codeletID);
};
/*TP1184: OMPTaskDirective*/
class TP1184 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets1185 : public darts::Codelet {
    public:
        TP1184* myTP;
        TP1184* inputsTPParent;
        _task1184Inputs* taskInputs;
        _checkInCodelets1185()
            : darts::Codelet()
        {
        }
        _checkInCodelets1185(uint32_t dep, uint32_t res, TP1184* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task1184Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets11547 : public darts::Codelet {
    public:
        TP1184* myTP;
        TP1184* inputsTPParent;
        _task1184Inputs* taskInputs;
        _checkInCodelets11547()
            : darts::Codelet()
        {
        }
        _checkInCodelets11547(uint32_t dep, uint32_t res, TP1184* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task1184Inputs)
        {
        }
        void fire(void);
    };
    TP31* TPParent;
    TP1184* controlTPParent;
    TP1184* inputsTPParent;
    _task1184Inputs* task1184Inputs;
    TP_fft_unshuffle_4* TP1185Ptr;
    int TP1185_alreadyLaunched;
    _checkInCodelets1185 checkInCodelets1185;
    _checkInCodelets11547 checkInCodelets11547;
    TP1184(int in_numThreads, int in_mainCodeletID, TP31* in_TPParent,
        _task1184Inputs* in_task1184Inputs);
    ~TP1184();
};
/*TP1186: OMPTaskDirective*/
class TP1186 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets1187 : public darts::Codelet {
    public:
        TP1186* myTP;
        TP1186* inputsTPParent;
        _task1186Inputs* taskInputs;
        _checkInCodelets1187()
            : darts::Codelet()
        {
        }
        _checkInCodelets1187(uint32_t dep, uint32_t res, TP1186* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task1186Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets11548 : public darts::Codelet {
    public:
        TP1186* myTP;
        TP1186* inputsTPParent;
        _task1186Inputs* taskInputs;
        _checkInCodelets11548()
            : darts::Codelet()
        {
        }
        _checkInCodelets11548(uint32_t dep, uint32_t res, TP1186* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task1186Inputs)
        {
        }
        void fire(void);
    };
    TP31* TPParent;
    TP1186* controlTPParent;
    TP1186* inputsTPParent;
    _task1186Inputs* task1186Inputs;
    TP_fft_unshuffle_4* TP1187Ptr;
    int TP1187_alreadyLaunched;
    _checkInCodelets1187 checkInCodelets1187;
    _checkInCodelets11548 checkInCodelets11548;
    TP1186(int in_numThreads, int in_mainCodeletID, TP31* in_TPParent,
        _task1186Inputs* in_task1186Inputs);
    ~TP1186();
};
/*TP34: fft_twiddle_8*/
class TP34 : public ompTP {
public:
    class _checkInCodelets1457 : public darts::Codelet {
    public:
        TP34* myTP;
        TP34* inputsTPParent;
        _checkInCodelets1457()
            : darts::Codelet()
        {
        }
        _checkInCodelets1457(uint32_t dep, uint32_t res, TP34* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets1460 : public darts::Codelet {
    public:
        TP34* myTP;
        TP34* inputsTPParent;
        _checkInCodelets1460()
            : darts::Codelet()
        {
        }
        _checkInCodelets1460(uint32_t dep, uint32_t res, TP34* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets1841 : public darts::Codelet {
    public:
        TP34* myTP;
        TP34* inputsTPParent;
        _checkInCodelets1841()
            : darts::Codelet()
        {
        }
        _checkInCodelets1841(uint32_t dep, uint32_t res, TP34* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets1844 : public darts::Codelet {
    public:
        TP34* myTP;
        TP34* inputsTPParent;
        _checkInCodelets1844()
            : darts::Codelet()
        {
        }
        _checkInCodelets1844(uint32_t dep, uint32_t res, TP34* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets1846 : public darts::Codelet {
    public:
        TP34* myTP;
        TP34* inputsTPParent;
        _checkInCodelets1846()
            : darts::Codelet()
        {
        }
        _checkInCodelets1846(uint32_t dep, uint32_t res, TP34* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets1848 : public darts::Codelet {
    public:
        TP34* inputsTPParent;
        _barrierCodelets1848()
            : darts::Codelet()
        {
        }
        _barrierCodelets1848(uint32_t dep, uint32_t res, TP34* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP34** ptrToThisFunctionTP;
    TP34* inputsTPParent;
    TP34* controlTPParent;
    darts::Codelet** nextCodeletsfft_twiddle_8;
    darts::Codelet** nextSyncCodeletsfft_twiddle_8;
    int* a_darts34 /*VARIABLE*/;
    int* b_darts34 /*VARIABLE*/;
    COMPLEX** in_darts34 /*VARIABLE*/;
    COMPLEX** out_darts34 /*VARIABLE*/;
    COMPLEX** W_darts34 /*VARIABLE*/;
    int* nW_darts34 /*VARIABLE*/;
    int* nWdn_darts34 /*VARIABLE*/;
    int* m_darts34 /*VARIABLE*/;
    int* ab_darts34 /*VARIABLE*/;
    int* i_darts34 /*VARIABLE*/;
    REAL* i1_0_darts34 /*VARIABLE*/;
    REAL* i1_1_darts34 /*VARIABLE*/;
    REAL* i1_2_darts34 /*VARIABLE*/;
    REAL* i1_3_darts34 /*VARIABLE*/;
    REAL* i1_4_darts34 /*VARIABLE*/;
    REAL* i1_5_darts34 /*VARIABLE*/;
    REAL* i1_6_darts34 /*VARIABLE*/;
    REAL* i1_7_darts34 /*VARIABLE*/;
    REAL* i2_0_darts34 /*VARIABLE*/;
    REAL* i2_1_darts34 /*VARIABLE*/;
    REAL* i2_2_darts34 /*VARIABLE*/;
    REAL* i2_3_darts34 /*VARIABLE*/;
    REAL* i2_4_darts34 /*VARIABLE*/;
    REAL* i2_5_darts34 /*VARIABLE*/;
    REAL* i2_6_darts34 /*VARIABLE*/;
    REAL* i2_7_darts34 /*VARIABLE*/;
    REAL* i3_0_darts34 /*VARIABLE*/;
    REAL* i3_1_darts34 /*VARIABLE*/;
    REAL* i3_2_darts34 /*VARIABLE*/;
    REAL* i3_3_darts34 /*VARIABLE*/;
    REAL* i3_4_darts34 /*VARIABLE*/;
    REAL* i3_5_darts34 /*VARIABLE*/;
    REAL* i3_6_darts34 /*VARIABLE*/;
    REAL* i3_7_darts34 /*VARIABLE*/;
    COMPLEX** jp_darts34 /*VARIABLE*/;
    COMPLEX** kp_darts34 /*VARIABLE*/;
    int* l1_darts34 /*VARIABLE*/;
    REAL* r1_0_darts34 /*VARIABLE*/;
    REAL* r1_1_darts34 /*VARIABLE*/;
    REAL* r1_2_darts34 /*VARIABLE*/;
    REAL* r1_3_darts34 /*VARIABLE*/;
    REAL* r1_4_darts34 /*VARIABLE*/;
    REAL* r1_5_darts34 /*VARIABLE*/;
    REAL* r1_6_darts34 /*VARIABLE*/;
    REAL* r1_7_darts34 /*VARIABLE*/;
    REAL* r2_0_darts34 /*VARIABLE*/;
    REAL* r2_1_darts34 /*VARIABLE*/;
    REAL* r2_2_darts34 /*VARIABLE*/;
    REAL* r2_3_darts34 /*VARIABLE*/;
    REAL* r2_4_darts34 /*VARIABLE*/;
    REAL* r2_5_darts34 /*VARIABLE*/;
    REAL* r2_6_darts34 /*VARIABLE*/;
    REAL* r2_7_darts34 /*VARIABLE*/;
    REAL* r3_0_darts34 /*VARIABLE*/;
    REAL* r3_1_darts34 /*VARIABLE*/;
    REAL* r3_2_darts34 /*VARIABLE*/;
    REAL* r3_3_darts34 /*VARIABLE*/;
    REAL* r3_4_darts34 /*VARIABLE*/;
    REAL* r3_5_darts34 /*VARIABLE*/;
    REAL* r3_6_darts34 /*VARIABLE*/;
    REAL* r3_7_darts34 /*VARIABLE*/;
    REAL* tmpi_darts34 /*VARIABLE*/;
    REAL* tmpr_darts34 /*VARIABLE*/;
    REAL* wi_darts34 /*VARIABLE*/;
    REAL* wr_darts34 /*VARIABLE*/;
    _task1844Inputs** task1844Inputs;
    _task1846Inputs** task1846Inputs;
    _checkInCodelets1457* checkInCodelets1457;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets1457* firstCodelet;
#endif
    _checkInCodelets1460* checkInCodelets1460;
    _checkInCodelets1841* checkInCodelets1841;
    _checkInCodelets1844* checkInCodelets1844;
    _checkInCodelets1846* checkInCodelets1846;
    _barrierCodelets1848* barrierCodelets1848;
    TP34(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_mainNextCodelet,
        darts::Codelet* in_mainSyncCodelet, TP34** in_ptrToThisFunctionTP, int in_a, int in_b,
        COMPLEX* in_in, COMPLEX* in_out, COMPLEX* in_W, int in_nW, int in_nWdn, int in_m);
    ~TP34();
    void setNewInputs(int in_a, int in_b, COMPLEX* in_in, COMPLEX* in_out, COMPLEX* in_W, int in_nW,
        int in_nWdn, int in_m, size_t codeletID);
};
/*TP1844: OMPTaskDirective*/
class TP1844 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets1845 : public darts::Codelet {
    public:
        TP1844* myTP;
        TP1844* inputsTPParent;
        _task1844Inputs* taskInputs;
        _checkInCodelets1845()
            : darts::Codelet()
        {
        }
        _checkInCodelets1845(uint32_t dep, uint32_t res, TP1844* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task1844Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets11549 : public darts::Codelet {
    public:
        TP1844* myTP;
        TP1844* inputsTPParent;
        _task1844Inputs* taskInputs;
        _checkInCodelets11549()
            : darts::Codelet()
        {
        }
        _checkInCodelets11549(uint32_t dep, uint32_t res, TP1844* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task1844Inputs)
        {
        }
        void fire(void);
    };
    TP34* TPParent;
    TP1844* controlTPParent;
    TP1844* inputsTPParent;
    _task1844Inputs* task1844Inputs;
    TP_fft_twiddle_8* TP1845Ptr;
    int TP1845_alreadyLaunched;
    _checkInCodelets1845 checkInCodelets1845;
    _checkInCodelets11549 checkInCodelets11549;
    TP1844(int in_numThreads, int in_mainCodeletID, TP34* in_TPParent,
        _task1844Inputs* in_task1844Inputs);
    ~TP1844();
};
/*TP1846: OMPTaskDirective*/
class TP1846 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets1847 : public darts::Codelet {
    public:
        TP1846* myTP;
        TP1846* inputsTPParent;
        _task1846Inputs* taskInputs;
        _checkInCodelets1847()
            : darts::Codelet()
        {
        }
        _checkInCodelets1847(uint32_t dep, uint32_t res, TP1846* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task1846Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets11550 : public darts::Codelet {
    public:
        TP1846* myTP;
        TP1846* inputsTPParent;
        _task1846Inputs* taskInputs;
        _checkInCodelets11550()
            : darts::Codelet()
        {
        }
        _checkInCodelets11550(uint32_t dep, uint32_t res, TP1846* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task1846Inputs)
        {
        }
        void fire(void);
    };
    TP34* TPParent;
    TP1846* controlTPParent;
    TP1846* inputsTPParent;
    _task1846Inputs* task1846Inputs;
    TP_fft_twiddle_8* TP1847Ptr;
    int TP1847_alreadyLaunched;
    _checkInCodelets1847 checkInCodelets1847;
    _checkInCodelets11550 checkInCodelets11550;
    TP1846(int in_numThreads, int in_mainCodeletID, TP34* in_TPParent,
        _task1846Inputs* in_task1846Inputs);
    ~TP1846();
};
/*TP36: fft_unshuffle_8*/
class TP36 : public ompTP {
public:
    class _checkInCodelets2250 : public darts::Codelet {
    public:
        TP36* myTP;
        TP36* inputsTPParent;
        _checkInCodelets2250()
            : darts::Codelet()
        {
        }
        _checkInCodelets2250(uint32_t dep, uint32_t res, TP36* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets2253 : public darts::Codelet {
    public:
        TP36* myTP;
        TP36* inputsTPParent;
        _checkInCodelets2253()
            : darts::Codelet()
        {
        }
        _checkInCodelets2253(uint32_t dep, uint32_t res, TP36* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets2298 : public darts::Codelet {
    public:
        TP36* myTP;
        TP36* inputsTPParent;
        _checkInCodelets2298()
            : darts::Codelet()
        {
        }
        _checkInCodelets2298(uint32_t dep, uint32_t res, TP36* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets2301 : public darts::Codelet {
    public:
        TP36* myTP;
        TP36* inputsTPParent;
        _checkInCodelets2301()
            : darts::Codelet()
        {
        }
        _checkInCodelets2301(uint32_t dep, uint32_t res, TP36* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets2303 : public darts::Codelet {
    public:
        TP36* myTP;
        TP36* inputsTPParent;
        _checkInCodelets2303()
            : darts::Codelet()
        {
        }
        _checkInCodelets2303(uint32_t dep, uint32_t res, TP36* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets2305 : public darts::Codelet {
    public:
        TP36* inputsTPParent;
        _barrierCodelets2305()
            : darts::Codelet()
        {
        }
        _barrierCodelets2305(uint32_t dep, uint32_t res, TP36* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP36** ptrToThisFunctionTP;
    TP36* inputsTPParent;
    TP36* controlTPParent;
    darts::Codelet** nextCodeletsfft_unshuffle_8;
    darts::Codelet** nextSyncCodeletsfft_unshuffle_8;
    int* a_darts36 /*VARIABLE*/;
    int* b_darts36 /*VARIABLE*/;
    COMPLEX** in_darts36 /*VARIABLE*/;
    COMPLEX** out_darts36 /*VARIABLE*/;
    int* m_darts36 /*VARIABLE*/;
    int* ab_darts36 /*VARIABLE*/;
    int* i_darts36 /*VARIABLE*/;
    COMPLEX** ip_darts36 /*VARIABLE*/;
    COMPLEX** jp_darts36 /*VARIABLE*/;
    _task2301Inputs** task2301Inputs;
    _task2303Inputs** task2303Inputs;
    _checkInCodelets2250* checkInCodelets2250;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets2250* firstCodelet;
#endif
    _checkInCodelets2253* checkInCodelets2253;
    _checkInCodelets2298* checkInCodelets2298;
    _checkInCodelets2301* checkInCodelets2301;
    _checkInCodelets2303* checkInCodelets2303;
    _barrierCodelets2305* barrierCodelets2305;
    TP36(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_mainNextCodelet,
        darts::Codelet* in_mainSyncCodelet, TP36** in_ptrToThisFunctionTP, int in_a, int in_b,
        COMPLEX* in_in, COMPLEX* in_out, int in_m);
    ~TP36();
    void setNewInputs(
        int in_a, int in_b, COMPLEX* in_in, COMPLEX* in_out, int in_m, size_t codeletID);
};
/*TP2301: OMPTaskDirective*/
class TP2301 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets2302 : public darts::Codelet {
    public:
        TP2301* myTP;
        TP2301* inputsTPParent;
        _task2301Inputs* taskInputs;
        _checkInCodelets2302()
            : darts::Codelet()
        {
        }
        _checkInCodelets2302(uint32_t dep, uint32_t res, TP2301* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task2301Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets11537 : public darts::Codelet {
    public:
        TP2301* myTP;
        TP2301* inputsTPParent;
        _task2301Inputs* taskInputs;
        _checkInCodelets11537()
            : darts::Codelet()
        {
        }
        _checkInCodelets11537(uint32_t dep, uint32_t res, TP2301* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task2301Inputs)
        {
        }
        void fire(void);
    };
    TP36* TPParent;
    TP2301* controlTPParent;
    TP2301* inputsTPParent;
    _task2301Inputs* task2301Inputs;
    TP_fft_unshuffle_8* TP2302Ptr;
    int TP2302_alreadyLaunched;
    _checkInCodelets2302 checkInCodelets2302;
    _checkInCodelets11537 checkInCodelets11537;
    TP2301(int in_numThreads, int in_mainCodeletID, TP36* in_TPParent,
        _task2301Inputs* in_task2301Inputs);
    ~TP2301();
};
/*TP2303: OMPTaskDirective*/
class TP2303 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets2304 : public darts::Codelet {
    public:
        TP2303* myTP;
        TP2303* inputsTPParent;
        _task2303Inputs* taskInputs;
        _checkInCodelets2304()
            : darts::Codelet()
        {
        }
        _checkInCodelets2304(uint32_t dep, uint32_t res, TP2303* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task2303Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets11538 : public darts::Codelet {
    public:
        TP2303* myTP;
        TP2303* inputsTPParent;
        _task2303Inputs* taskInputs;
        _checkInCodelets11538()
            : darts::Codelet()
        {
        }
        _checkInCodelets11538(uint32_t dep, uint32_t res, TP2303* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task2303Inputs)
        {
        }
        void fire(void);
    };
    TP36* TPParent;
    TP2303* controlTPParent;
    TP2303* inputsTPParent;
    _task2303Inputs* task2303Inputs;
    TP_fft_unshuffle_8* TP2304Ptr;
    int TP2304_alreadyLaunched;
    _checkInCodelets2304 checkInCodelets2304;
    _checkInCodelets11538 checkInCodelets11538;
    TP2303(int in_numThreads, int in_mainCodeletID, TP36* in_TPParent,
        _task2303Inputs* in_task2303Inputs);
    ~TP2303();
};
/*TP39: fft_twiddle_16*/
class TP39 : public ompTP {
public:
    class _checkInCodelets2960 : public darts::Codelet {
    public:
        TP39* myTP;
        TP39* inputsTPParent;
        _checkInCodelets2960()
            : darts::Codelet()
        {
        }
        _checkInCodelets2960(uint32_t dep, uint32_t res, TP39* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets2963 : public darts::Codelet {
    public:
        TP39* myTP;
        TP39* inputsTPParent;
        _checkInCodelets2963()
            : darts::Codelet()
        {
        }
        _checkInCodelets2963(uint32_t dep, uint32_t res, TP39* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets3872 : public darts::Codelet {
    public:
        TP39* myTP;
        TP39* inputsTPParent;
        _checkInCodelets3872()
            : darts::Codelet()
        {
        }
        _checkInCodelets3872(uint32_t dep, uint32_t res, TP39* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets3875 : public darts::Codelet {
    public:
        TP39* myTP;
        TP39* inputsTPParent;
        _checkInCodelets3875()
            : darts::Codelet()
        {
        }
        _checkInCodelets3875(uint32_t dep, uint32_t res, TP39* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets3877 : public darts::Codelet {
    public:
        TP39* myTP;
        TP39* inputsTPParent;
        _checkInCodelets3877()
            : darts::Codelet()
        {
        }
        _checkInCodelets3877(uint32_t dep, uint32_t res, TP39* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets3879 : public darts::Codelet {
    public:
        TP39* inputsTPParent;
        _barrierCodelets3879()
            : darts::Codelet()
        {
        }
        _barrierCodelets3879(uint32_t dep, uint32_t res, TP39* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP39** ptrToThisFunctionTP;
    TP39* inputsTPParent;
    TP39* controlTPParent;
    darts::Codelet** nextCodeletsfft_twiddle_16;
    darts::Codelet** nextSyncCodeletsfft_twiddle_16;
    int* a_darts39 /*VARIABLE*/;
    int* b_darts39 /*VARIABLE*/;
    COMPLEX** in_darts39 /*VARIABLE*/;
    COMPLEX** out_darts39 /*VARIABLE*/;
    COMPLEX** W_darts39 /*VARIABLE*/;
    int* nW_darts39 /*VARIABLE*/;
    int* nWdn_darts39 /*VARIABLE*/;
    int* m_darts39 /*VARIABLE*/;
    int* ab_darts39 /*VARIABLE*/;
    int* i_darts39 /*VARIABLE*/;
    REAL* i1_0_darts39 /*VARIABLE*/;
    REAL* i1_1_darts39 /*VARIABLE*/;
    REAL* i1_10_darts39 /*VARIABLE*/;
    REAL* i1_11_darts39 /*VARIABLE*/;
    REAL* i1_12_darts39 /*VARIABLE*/;
    REAL* i1_13_darts39 /*VARIABLE*/;
    REAL* i1_14_darts39 /*VARIABLE*/;
    REAL* i1_15_darts39 /*VARIABLE*/;
    REAL* i1_2_darts39 /*VARIABLE*/;
    REAL* i1_3_darts39 /*VARIABLE*/;
    REAL* i1_4_darts39 /*VARIABLE*/;
    REAL* i1_5_darts39 /*VARIABLE*/;
    REAL* i1_6_darts39 /*VARIABLE*/;
    REAL* i1_7_darts39 /*VARIABLE*/;
    REAL* i1_8_darts39 /*VARIABLE*/;
    REAL* i1_9_darts39 /*VARIABLE*/;
    REAL* i2_0_darts39 /*VARIABLE*/;
    REAL* i2_1_darts39 /*VARIABLE*/;
    REAL* i2_10_darts39 /*VARIABLE*/;
    REAL* i2_11_darts39 /*VARIABLE*/;
    REAL* i2_12_darts39 /*VARIABLE*/;
    REAL* i2_13_darts39 /*VARIABLE*/;
    REAL* i2_14_darts39 /*VARIABLE*/;
    REAL* i2_15_darts39 /*VARIABLE*/;
    REAL* i2_2_darts39 /*VARIABLE*/;
    REAL* i2_3_darts39 /*VARIABLE*/;
    REAL* i2_4_darts39 /*VARIABLE*/;
    REAL* i2_5_darts39 /*VARIABLE*/;
    REAL* i2_6_darts39 /*VARIABLE*/;
    REAL* i2_7_darts39 /*VARIABLE*/;
    REAL* i2_8_darts39 /*VARIABLE*/;
    REAL* i2_9_darts39 /*VARIABLE*/;
    REAL* i3_0_darts39 /*VARIABLE*/;
    REAL* i3_1_darts39 /*VARIABLE*/;
    REAL* i3_10_darts39 /*VARIABLE*/;
    REAL* i3_11_darts39 /*VARIABLE*/;
    REAL* i3_12_darts39 /*VARIABLE*/;
    REAL* i3_13_darts39 /*VARIABLE*/;
    REAL* i3_14_darts39 /*VARIABLE*/;
    REAL* i3_15_darts39 /*VARIABLE*/;
    REAL* i3_2_darts39 /*VARIABLE*/;
    REAL* i3_3_darts39 /*VARIABLE*/;
    REAL* i3_4_darts39 /*VARIABLE*/;
    REAL* i3_5_darts39 /*VARIABLE*/;
    REAL* i3_6_darts39 /*VARIABLE*/;
    REAL* i3_7_darts39 /*VARIABLE*/;
    REAL* i3_8_darts39 /*VARIABLE*/;
    REAL* i3_9_darts39 /*VARIABLE*/;
    REAL* i4_0_darts39 /*VARIABLE*/;
    REAL* i4_1_darts39 /*VARIABLE*/;
    REAL* i4_10_darts39 /*VARIABLE*/;
    REAL* i4_11_darts39 /*VARIABLE*/;
    REAL* i4_12_darts39 /*VARIABLE*/;
    REAL* i4_13_darts39 /*VARIABLE*/;
    REAL* i4_14_darts39 /*VARIABLE*/;
    REAL* i4_15_darts39 /*VARIABLE*/;
    REAL* i4_2_darts39 /*VARIABLE*/;
    REAL* i4_3_darts39 /*VARIABLE*/;
    REAL* i4_4_darts39 /*VARIABLE*/;
    REAL* i4_5_darts39 /*VARIABLE*/;
    REAL* i4_6_darts39 /*VARIABLE*/;
    REAL* i4_7_darts39 /*VARIABLE*/;
    REAL* i4_8_darts39 /*VARIABLE*/;
    REAL* i4_9_darts39 /*VARIABLE*/;
    COMPLEX** jp_darts39 /*VARIABLE*/;
    COMPLEX** kp_darts39 /*VARIABLE*/;
    int* l1_darts39 /*VARIABLE*/;
    REAL* r1_0_darts39 /*VARIABLE*/;
    REAL* r1_1_darts39 /*VARIABLE*/;
    REAL* r1_10_darts39 /*VARIABLE*/;
    REAL* r1_11_darts39 /*VARIABLE*/;
    REAL* r1_12_darts39 /*VARIABLE*/;
    REAL* r1_13_darts39 /*VARIABLE*/;
    REAL* r1_14_darts39 /*VARIABLE*/;
    REAL* r1_15_darts39 /*VARIABLE*/;
    REAL* r1_2_darts39 /*VARIABLE*/;
    REAL* r1_3_darts39 /*VARIABLE*/;
    REAL* r1_4_darts39 /*VARIABLE*/;
    REAL* r1_5_darts39 /*VARIABLE*/;
    REAL* r1_6_darts39 /*VARIABLE*/;
    REAL* r1_7_darts39 /*VARIABLE*/;
    REAL* r1_8_darts39 /*VARIABLE*/;
    REAL* r1_9_darts39 /*VARIABLE*/;
    REAL* r2_0_darts39 /*VARIABLE*/;
    REAL* r2_1_darts39 /*VARIABLE*/;
    REAL* r2_10_darts39 /*VARIABLE*/;
    REAL* r2_11_darts39 /*VARIABLE*/;
    REAL* r2_12_darts39 /*VARIABLE*/;
    REAL* r2_13_darts39 /*VARIABLE*/;
    REAL* r2_14_darts39 /*VARIABLE*/;
    REAL* r2_15_darts39 /*VARIABLE*/;
    REAL* r2_2_darts39 /*VARIABLE*/;
    REAL* r2_3_darts39 /*VARIABLE*/;
    REAL* r2_4_darts39 /*VARIABLE*/;
    REAL* r2_5_darts39 /*VARIABLE*/;
    REAL* r2_6_darts39 /*VARIABLE*/;
    REAL* r2_7_darts39 /*VARIABLE*/;
    REAL* r2_8_darts39 /*VARIABLE*/;
    REAL* r2_9_darts39 /*VARIABLE*/;
    REAL* r3_0_darts39 /*VARIABLE*/;
    REAL* r3_1_darts39 /*VARIABLE*/;
    REAL* r3_10_darts39 /*VARIABLE*/;
    REAL* r3_11_darts39 /*VARIABLE*/;
    REAL* r3_12_darts39 /*VARIABLE*/;
    REAL* r3_13_darts39 /*VARIABLE*/;
    REAL* r3_14_darts39 /*VARIABLE*/;
    REAL* r3_15_darts39 /*VARIABLE*/;
    REAL* r3_2_darts39 /*VARIABLE*/;
    REAL* r3_3_darts39 /*VARIABLE*/;
    REAL* r3_4_darts39 /*VARIABLE*/;
    REAL* r3_5_darts39 /*VARIABLE*/;
    REAL* r3_6_darts39 /*VARIABLE*/;
    REAL* r3_7_darts39 /*VARIABLE*/;
    REAL* r3_8_darts39 /*VARIABLE*/;
    REAL* r3_9_darts39 /*VARIABLE*/;
    REAL* r4_0_darts39 /*VARIABLE*/;
    REAL* r4_1_darts39 /*VARIABLE*/;
    REAL* r4_10_darts39 /*VARIABLE*/;
    REAL* r4_11_darts39 /*VARIABLE*/;
    REAL* r4_12_darts39 /*VARIABLE*/;
    REAL* r4_13_darts39 /*VARIABLE*/;
    REAL* r4_14_darts39 /*VARIABLE*/;
    REAL* r4_15_darts39 /*VARIABLE*/;
    REAL* r4_2_darts39 /*VARIABLE*/;
    REAL* r4_3_darts39 /*VARIABLE*/;
    REAL* r4_4_darts39 /*VARIABLE*/;
    REAL* r4_5_darts39 /*VARIABLE*/;
    REAL* r4_6_darts39 /*VARIABLE*/;
    REAL* r4_7_darts39 /*VARIABLE*/;
    REAL* r4_8_darts39 /*VARIABLE*/;
    REAL* r4_9_darts39 /*VARIABLE*/;
    REAL* tmpi_darts39 /*VARIABLE*/;
    REAL* tmpr_darts39 /*VARIABLE*/;
    REAL* wi_darts39 /*VARIABLE*/;
    REAL* wr_darts39 /*VARIABLE*/;
    _task3875Inputs** task3875Inputs;
    _task3877Inputs** task3877Inputs;
    _checkInCodelets2960* checkInCodelets2960;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets2960* firstCodelet;
#endif
    _checkInCodelets2963* checkInCodelets2963;
    _checkInCodelets3872* checkInCodelets3872;
    _checkInCodelets3875* checkInCodelets3875;
    _checkInCodelets3877* checkInCodelets3877;
    _barrierCodelets3879* barrierCodelets3879;
    TP39(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_mainNextCodelet,
        darts::Codelet* in_mainSyncCodelet, TP39** in_ptrToThisFunctionTP, int in_a, int in_b,
        COMPLEX* in_in, COMPLEX* in_out, COMPLEX* in_W, int in_nW, int in_nWdn, int in_m);
    ~TP39();
    void setNewInputs(int in_a, int in_b, COMPLEX* in_in, COMPLEX* in_out, COMPLEX* in_W, int in_nW,
        int in_nWdn, int in_m, size_t codeletID);
};
/*TP3875: OMPTaskDirective*/
class TP3875 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets3876 : public darts::Codelet {
    public:
        TP3875* myTP;
        TP3875* inputsTPParent;
        _task3875Inputs* taskInputs;
        _checkInCodelets3876()
            : darts::Codelet()
        {
        }
        _checkInCodelets3876(uint32_t dep, uint32_t res, TP3875* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task3875Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets11512 : public darts::Codelet {
    public:
        TP3875* myTP;
        TP3875* inputsTPParent;
        _task3875Inputs* taskInputs;
        _checkInCodelets11512()
            : darts::Codelet()
        {
        }
        _checkInCodelets11512(uint32_t dep, uint32_t res, TP3875* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task3875Inputs)
        {
        }
        void fire(void);
    };
    TP39* TPParent;
    TP3875* controlTPParent;
    TP3875* inputsTPParent;
    _task3875Inputs* task3875Inputs;
    TP_fft_twiddle_16* TP3876Ptr;
    int TP3876_alreadyLaunched;
    _checkInCodelets3876 checkInCodelets3876;
    _checkInCodelets11512 checkInCodelets11512;
    TP3875(int in_numThreads, int in_mainCodeletID, TP39* in_TPParent,
        _task3875Inputs* in_task3875Inputs);
    ~TP3875();
};
/*TP3877: OMPTaskDirective*/
class TP3877 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets3878 : public darts::Codelet {
    public:
        TP3877* myTP;
        TP3877* inputsTPParent;
        _task3877Inputs* taskInputs;
        _checkInCodelets3878()
            : darts::Codelet()
        {
        }
        _checkInCodelets3878(uint32_t dep, uint32_t res, TP3877* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task3877Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets11513 : public darts::Codelet {
    public:
        TP3877* myTP;
        TP3877* inputsTPParent;
        _task3877Inputs* taskInputs;
        _checkInCodelets11513()
            : darts::Codelet()
        {
        }
        _checkInCodelets11513(uint32_t dep, uint32_t res, TP3877* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task3877Inputs)
        {
        }
        void fire(void);
    };
    TP39* TPParent;
    TP3877* controlTPParent;
    TP3877* inputsTPParent;
    _task3877Inputs* task3877Inputs;
    TP_fft_twiddle_16* TP3878Ptr;
    int TP3878_alreadyLaunched;
    _checkInCodelets3878 checkInCodelets3878;
    _checkInCodelets11513 checkInCodelets11513;
    TP3877(int in_numThreads, int in_mainCodeletID, TP39* in_TPParent,
        _task3877Inputs* in_task3877Inputs);
    ~TP3877();
};
/*TP41: fft_unshuffle_16*/
class TP41 : public ompTP {
public:
    class _checkInCodelets4809 : public darts::Codelet {
    public:
        TP41* myTP;
        TP41* inputsTPParent;
        _checkInCodelets4809()
            : darts::Codelet()
        {
        }
        _checkInCodelets4809(uint32_t dep, uint32_t res, TP41* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets4812 : public darts::Codelet {
    public:
        TP41* myTP;
        TP41* inputsTPParent;
        _checkInCodelets4812()
            : darts::Codelet()
        {
        }
        _checkInCodelets4812(uint32_t dep, uint32_t res, TP41* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets4893 : public darts::Codelet {
    public:
        TP41* myTP;
        TP41* inputsTPParent;
        _checkInCodelets4893()
            : darts::Codelet()
        {
        }
        _checkInCodelets4893(uint32_t dep, uint32_t res, TP41* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets4896 : public darts::Codelet {
    public:
        TP41* myTP;
        TP41* inputsTPParent;
        _checkInCodelets4896()
            : darts::Codelet()
        {
        }
        _checkInCodelets4896(uint32_t dep, uint32_t res, TP41* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets4898 : public darts::Codelet {
    public:
        TP41* myTP;
        TP41* inputsTPParent;
        _checkInCodelets4898()
            : darts::Codelet()
        {
        }
        _checkInCodelets4898(uint32_t dep, uint32_t res, TP41* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets4900 : public darts::Codelet {
    public:
        TP41* inputsTPParent;
        _barrierCodelets4900()
            : darts::Codelet()
        {
        }
        _barrierCodelets4900(uint32_t dep, uint32_t res, TP41* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP41** ptrToThisFunctionTP;
    TP41* inputsTPParent;
    TP41* controlTPParent;
    darts::Codelet** nextCodeletsfft_unshuffle_16;
    darts::Codelet** nextSyncCodeletsfft_unshuffle_16;
    int* a_darts41 /*VARIABLE*/;
    int* b_darts41 /*VARIABLE*/;
    COMPLEX** in_darts41 /*VARIABLE*/;
    COMPLEX** out_darts41 /*VARIABLE*/;
    int* m_darts41 /*VARIABLE*/;
    int* ab_darts41 /*VARIABLE*/;
    int* i_darts41 /*VARIABLE*/;
    COMPLEX** ip_darts41 /*VARIABLE*/;
    COMPLEX** jp_darts41 /*VARIABLE*/;
    _task4896Inputs** task4896Inputs;
    _task4898Inputs** task4898Inputs;
    _checkInCodelets4809* checkInCodelets4809;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets4809* firstCodelet;
#endif
    _checkInCodelets4812* checkInCodelets4812;
    _checkInCodelets4893* checkInCodelets4893;
    _checkInCodelets4896* checkInCodelets4896;
    _checkInCodelets4898* checkInCodelets4898;
    _barrierCodelets4900* barrierCodelets4900;
    TP41(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_mainNextCodelet,
        darts::Codelet* in_mainSyncCodelet, TP41** in_ptrToThisFunctionTP, int in_a, int in_b,
        COMPLEX* in_in, COMPLEX* in_out, int in_m);
    ~TP41();
    void setNewInputs(
        int in_a, int in_b, COMPLEX* in_in, COMPLEX* in_out, int in_m, size_t codeletID);
};
/*TP4896: OMPTaskDirective*/
class TP4896 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets4897 : public darts::Codelet {
    public:
        TP4896* myTP;
        TP4896* inputsTPParent;
        _task4896Inputs* taskInputs;
        _checkInCodelets4897()
            : darts::Codelet()
        {
        }
        _checkInCodelets4897(uint32_t dep, uint32_t res, TP4896* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task4896Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets11522 : public darts::Codelet {
    public:
        TP4896* myTP;
        TP4896* inputsTPParent;
        _task4896Inputs* taskInputs;
        _checkInCodelets11522()
            : darts::Codelet()
        {
        }
        _checkInCodelets11522(uint32_t dep, uint32_t res, TP4896* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task4896Inputs)
        {
        }
        void fire(void);
    };
    TP41* TPParent;
    TP4896* controlTPParent;
    TP4896* inputsTPParent;
    _task4896Inputs* task4896Inputs;
    TP_fft_unshuffle_16* TP4897Ptr;
    int TP4897_alreadyLaunched;
    _checkInCodelets4897 checkInCodelets4897;
    _checkInCodelets11522 checkInCodelets11522;
    TP4896(int in_numThreads, int in_mainCodeletID, TP41* in_TPParent,
        _task4896Inputs* in_task4896Inputs);
    ~TP4896();
};
/*TP4898: OMPTaskDirective*/
class TP4898 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets4899 : public darts::Codelet {
    public:
        TP4898* myTP;
        TP4898* inputsTPParent;
        _task4898Inputs* taskInputs;
        _checkInCodelets4899()
            : darts::Codelet()
        {
        }
        _checkInCodelets4899(uint32_t dep, uint32_t res, TP4898* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task4898Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets11523 : public darts::Codelet {
    public:
        TP4898* myTP;
        TP4898* inputsTPParent;
        _task4898Inputs* taskInputs;
        _checkInCodelets11523()
            : darts::Codelet()
        {
        }
        _checkInCodelets11523(uint32_t dep, uint32_t res, TP4898* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task4898Inputs)
        {
        }
        void fire(void);
    };
    TP41* TPParent;
    TP4898* controlTPParent;
    TP4898* inputsTPParent;
    _task4898Inputs* task4898Inputs;
    TP_fft_unshuffle_16* TP4899Ptr;
    int TP4899_alreadyLaunched;
    _checkInCodelets4899 checkInCodelets4899;
    _checkInCodelets11523 checkInCodelets11523;
    TP4898(int in_numThreads, int in_mainCodeletID, TP41* in_TPParent,
        _task4898Inputs* in_task4898Inputs);
    ~TP4898();
};
/*TP44: fft_twiddle_32*/
class TP44 : public ompTP {
public:
    class _checkInCodelets6503 : public darts::Codelet {
    public:
        TP44* myTP;
        TP44* inputsTPParent;
        _checkInCodelets6503()
            : darts::Codelet()
        {
        }
        _checkInCodelets6503(uint32_t dep, uint32_t res, TP44* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets6506 : public darts::Codelet {
    public:
        TP44* myTP;
        TP44* inputsTPParent;
        _checkInCodelets6506()
            : darts::Codelet()
        {
        }
        _checkInCodelets6506(uint32_t dep, uint32_t res, TP44* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets8647 : public darts::Codelet {
    public:
        TP44* myTP;
        TP44* inputsTPParent;
        _checkInCodelets8647()
            : darts::Codelet()
        {
        }
        _checkInCodelets8647(uint32_t dep, uint32_t res, TP44* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets8650 : public darts::Codelet {
    public:
        TP44* myTP;
        TP44* inputsTPParent;
        _checkInCodelets8650()
            : darts::Codelet()
        {
        }
        _checkInCodelets8650(uint32_t dep, uint32_t res, TP44* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets8652 : public darts::Codelet {
    public:
        TP44* myTP;
        TP44* inputsTPParent;
        _checkInCodelets8652()
            : darts::Codelet()
        {
        }
        _checkInCodelets8652(uint32_t dep, uint32_t res, TP44* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets8654 : public darts::Codelet {
    public:
        TP44* inputsTPParent;
        _barrierCodelets8654()
            : darts::Codelet()
        {
        }
        _barrierCodelets8654(uint32_t dep, uint32_t res, TP44* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP44** ptrToThisFunctionTP;
    TP44* inputsTPParent;
    TP44* controlTPParent;
    darts::Codelet** nextCodeletsfft_twiddle_32;
    darts::Codelet** nextSyncCodeletsfft_twiddle_32;
    int* a_darts44 /*VARIABLE*/;
    int* b_darts44 /*VARIABLE*/;
    COMPLEX** in_darts44 /*VARIABLE*/;
    COMPLEX** out_darts44 /*VARIABLE*/;
    COMPLEX** W_darts44 /*VARIABLE*/;
    int* nW_darts44 /*VARIABLE*/;
    int* nWdn_darts44 /*VARIABLE*/;
    int* m_darts44 /*VARIABLE*/;
    int* ab_darts44 /*VARIABLE*/;
    int* i_darts44 /*VARIABLE*/;
    REAL* i1_0_darts44 /*VARIABLE*/;
    REAL* i1_1_darts44 /*VARIABLE*/;
    REAL* i1_10_darts44 /*VARIABLE*/;
    REAL* i1_11_darts44 /*VARIABLE*/;
    REAL* i1_12_darts44 /*VARIABLE*/;
    REAL* i1_13_darts44 /*VARIABLE*/;
    REAL* i1_14_darts44 /*VARIABLE*/;
    REAL* i1_15_darts44 /*VARIABLE*/;
    REAL* i1_16_darts44 /*VARIABLE*/;
    REAL* i1_17_darts44 /*VARIABLE*/;
    REAL* i1_18_darts44 /*VARIABLE*/;
    REAL* i1_19_darts44 /*VARIABLE*/;
    REAL* i1_2_darts44 /*VARIABLE*/;
    REAL* i1_20_darts44 /*VARIABLE*/;
    REAL* i1_21_darts44 /*VARIABLE*/;
    REAL* i1_22_darts44 /*VARIABLE*/;
    REAL* i1_23_darts44 /*VARIABLE*/;
    REAL* i1_24_darts44 /*VARIABLE*/;
    REAL* i1_25_darts44 /*VARIABLE*/;
    REAL* i1_26_darts44 /*VARIABLE*/;
    REAL* i1_27_darts44 /*VARIABLE*/;
    REAL* i1_28_darts44 /*VARIABLE*/;
    REAL* i1_29_darts44 /*VARIABLE*/;
    REAL* i1_3_darts44 /*VARIABLE*/;
    REAL* i1_30_darts44 /*VARIABLE*/;
    REAL* i1_31_darts44 /*VARIABLE*/;
    REAL* i1_4_darts44 /*VARIABLE*/;
    REAL* i1_5_darts44 /*VARIABLE*/;
    REAL* i1_6_darts44 /*VARIABLE*/;
    REAL* i1_7_darts44 /*VARIABLE*/;
    REAL* i1_8_darts44 /*VARIABLE*/;
    REAL* i1_9_darts44 /*VARIABLE*/;
    REAL* i2_0_darts44 /*VARIABLE*/;
    REAL* i2_1_darts44 /*VARIABLE*/;
    REAL* i2_10_darts44 /*VARIABLE*/;
    REAL* i2_11_darts44 /*VARIABLE*/;
    REAL* i2_12_darts44 /*VARIABLE*/;
    REAL* i2_13_darts44 /*VARIABLE*/;
    REAL* i2_14_darts44 /*VARIABLE*/;
    REAL* i2_15_darts44 /*VARIABLE*/;
    REAL* i2_16_darts44 /*VARIABLE*/;
    REAL* i2_17_darts44 /*VARIABLE*/;
    REAL* i2_18_darts44 /*VARIABLE*/;
    REAL* i2_19_darts44 /*VARIABLE*/;
    REAL* i2_2_darts44 /*VARIABLE*/;
    REAL* i2_20_darts44 /*VARIABLE*/;
    REAL* i2_21_darts44 /*VARIABLE*/;
    REAL* i2_22_darts44 /*VARIABLE*/;
    REAL* i2_23_darts44 /*VARIABLE*/;
    REAL* i2_24_darts44 /*VARIABLE*/;
    REAL* i2_25_darts44 /*VARIABLE*/;
    REAL* i2_26_darts44 /*VARIABLE*/;
    REAL* i2_27_darts44 /*VARIABLE*/;
    REAL* i2_28_darts44 /*VARIABLE*/;
    REAL* i2_29_darts44 /*VARIABLE*/;
    REAL* i2_3_darts44 /*VARIABLE*/;
    REAL* i2_30_darts44 /*VARIABLE*/;
    REAL* i2_31_darts44 /*VARIABLE*/;
    REAL* i2_4_darts44 /*VARIABLE*/;
    REAL* i2_5_darts44 /*VARIABLE*/;
    REAL* i2_6_darts44 /*VARIABLE*/;
    REAL* i2_7_darts44 /*VARIABLE*/;
    REAL* i2_8_darts44 /*VARIABLE*/;
    REAL* i2_9_darts44 /*VARIABLE*/;
    REAL* i3_0_darts44 /*VARIABLE*/;
    REAL* i3_1_darts44 /*VARIABLE*/;
    REAL* i3_10_darts44 /*VARIABLE*/;
    REAL* i3_11_darts44 /*VARIABLE*/;
    REAL* i3_12_darts44 /*VARIABLE*/;
    REAL* i3_13_darts44 /*VARIABLE*/;
    REAL* i3_14_darts44 /*VARIABLE*/;
    REAL* i3_15_darts44 /*VARIABLE*/;
    REAL* i3_16_darts44 /*VARIABLE*/;
    REAL* i3_17_darts44 /*VARIABLE*/;
    REAL* i3_18_darts44 /*VARIABLE*/;
    REAL* i3_19_darts44 /*VARIABLE*/;
    REAL* i3_2_darts44 /*VARIABLE*/;
    REAL* i3_20_darts44 /*VARIABLE*/;
    REAL* i3_21_darts44 /*VARIABLE*/;
    REAL* i3_22_darts44 /*VARIABLE*/;
    REAL* i3_23_darts44 /*VARIABLE*/;
    REAL* i3_24_darts44 /*VARIABLE*/;
    REAL* i3_25_darts44 /*VARIABLE*/;
    REAL* i3_26_darts44 /*VARIABLE*/;
    REAL* i3_27_darts44 /*VARIABLE*/;
    REAL* i3_28_darts44 /*VARIABLE*/;
    REAL* i3_29_darts44 /*VARIABLE*/;
    REAL* i3_3_darts44 /*VARIABLE*/;
    REAL* i3_30_darts44 /*VARIABLE*/;
    REAL* i3_31_darts44 /*VARIABLE*/;
    REAL* i3_4_darts44 /*VARIABLE*/;
    REAL* i3_5_darts44 /*VARIABLE*/;
    REAL* i3_6_darts44 /*VARIABLE*/;
    REAL* i3_7_darts44 /*VARIABLE*/;
    REAL* i3_8_darts44 /*VARIABLE*/;
    REAL* i3_9_darts44 /*VARIABLE*/;
    REAL* i4_0_darts44 /*VARIABLE*/;
    REAL* i4_1_darts44 /*VARIABLE*/;
    REAL* i4_10_darts44 /*VARIABLE*/;
    REAL* i4_11_darts44 /*VARIABLE*/;
    REAL* i4_12_darts44 /*VARIABLE*/;
    REAL* i4_13_darts44 /*VARIABLE*/;
    REAL* i4_14_darts44 /*VARIABLE*/;
    REAL* i4_15_darts44 /*VARIABLE*/;
    REAL* i4_16_darts44 /*VARIABLE*/;
    REAL* i4_17_darts44 /*VARIABLE*/;
    REAL* i4_18_darts44 /*VARIABLE*/;
    REAL* i4_19_darts44 /*VARIABLE*/;
    REAL* i4_2_darts44 /*VARIABLE*/;
    REAL* i4_20_darts44 /*VARIABLE*/;
    REAL* i4_21_darts44 /*VARIABLE*/;
    REAL* i4_22_darts44 /*VARIABLE*/;
    REAL* i4_23_darts44 /*VARIABLE*/;
    REAL* i4_24_darts44 /*VARIABLE*/;
    REAL* i4_25_darts44 /*VARIABLE*/;
    REAL* i4_26_darts44 /*VARIABLE*/;
    REAL* i4_27_darts44 /*VARIABLE*/;
    REAL* i4_28_darts44 /*VARIABLE*/;
    REAL* i4_29_darts44 /*VARIABLE*/;
    REAL* i4_3_darts44 /*VARIABLE*/;
    REAL* i4_30_darts44 /*VARIABLE*/;
    REAL* i4_31_darts44 /*VARIABLE*/;
    REAL* i4_4_darts44 /*VARIABLE*/;
    REAL* i4_5_darts44 /*VARIABLE*/;
    REAL* i4_6_darts44 /*VARIABLE*/;
    REAL* i4_7_darts44 /*VARIABLE*/;
    REAL* i4_8_darts44 /*VARIABLE*/;
    REAL* i4_9_darts44 /*VARIABLE*/;
    REAL* i5_0_darts44 /*VARIABLE*/;
    REAL* i5_1_darts44 /*VARIABLE*/;
    REAL* i5_10_darts44 /*VARIABLE*/;
    REAL* i5_11_darts44 /*VARIABLE*/;
    REAL* i5_12_darts44 /*VARIABLE*/;
    REAL* i5_13_darts44 /*VARIABLE*/;
    REAL* i5_14_darts44 /*VARIABLE*/;
    REAL* i5_15_darts44 /*VARIABLE*/;
    REAL* i5_16_darts44 /*VARIABLE*/;
    REAL* i5_17_darts44 /*VARIABLE*/;
    REAL* i5_18_darts44 /*VARIABLE*/;
    REAL* i5_19_darts44 /*VARIABLE*/;
    REAL* i5_2_darts44 /*VARIABLE*/;
    REAL* i5_20_darts44 /*VARIABLE*/;
    REAL* i5_21_darts44 /*VARIABLE*/;
    REAL* i5_22_darts44 /*VARIABLE*/;
    REAL* i5_23_darts44 /*VARIABLE*/;
    REAL* i5_24_darts44 /*VARIABLE*/;
    REAL* i5_25_darts44 /*VARIABLE*/;
    REAL* i5_26_darts44 /*VARIABLE*/;
    REAL* i5_27_darts44 /*VARIABLE*/;
    REAL* i5_28_darts44 /*VARIABLE*/;
    REAL* i5_29_darts44 /*VARIABLE*/;
    REAL* i5_3_darts44 /*VARIABLE*/;
    REAL* i5_30_darts44 /*VARIABLE*/;
    REAL* i5_31_darts44 /*VARIABLE*/;
    REAL* i5_4_darts44 /*VARIABLE*/;
    REAL* i5_5_darts44 /*VARIABLE*/;
    REAL* i5_6_darts44 /*VARIABLE*/;
    REAL* i5_7_darts44 /*VARIABLE*/;
    REAL* i5_8_darts44 /*VARIABLE*/;
    REAL* i5_9_darts44 /*VARIABLE*/;
    COMPLEX** jp_darts44 /*VARIABLE*/;
    COMPLEX** kp_darts44 /*VARIABLE*/;
    int* l1_darts44 /*VARIABLE*/;
    REAL* r1_0_darts44 /*VARIABLE*/;
    REAL* r1_1_darts44 /*VARIABLE*/;
    REAL* r1_10_darts44 /*VARIABLE*/;
    REAL* r1_11_darts44 /*VARIABLE*/;
    REAL* r1_12_darts44 /*VARIABLE*/;
    REAL* r1_13_darts44 /*VARIABLE*/;
    REAL* r1_14_darts44 /*VARIABLE*/;
    REAL* r1_15_darts44 /*VARIABLE*/;
    REAL* r1_16_darts44 /*VARIABLE*/;
    REAL* r1_17_darts44 /*VARIABLE*/;
    REAL* r1_18_darts44 /*VARIABLE*/;
    REAL* r1_19_darts44 /*VARIABLE*/;
    REAL* r1_2_darts44 /*VARIABLE*/;
    REAL* r1_20_darts44 /*VARIABLE*/;
    REAL* r1_21_darts44 /*VARIABLE*/;
    REAL* r1_22_darts44 /*VARIABLE*/;
    REAL* r1_23_darts44 /*VARIABLE*/;
    REAL* r1_24_darts44 /*VARIABLE*/;
    REAL* r1_25_darts44 /*VARIABLE*/;
    REAL* r1_26_darts44 /*VARIABLE*/;
    REAL* r1_27_darts44 /*VARIABLE*/;
    REAL* r1_28_darts44 /*VARIABLE*/;
    REAL* r1_29_darts44 /*VARIABLE*/;
    REAL* r1_3_darts44 /*VARIABLE*/;
    REAL* r1_30_darts44 /*VARIABLE*/;
    REAL* r1_31_darts44 /*VARIABLE*/;
    REAL* r1_4_darts44 /*VARIABLE*/;
    REAL* r1_5_darts44 /*VARIABLE*/;
    REAL* r1_6_darts44 /*VARIABLE*/;
    REAL* r1_7_darts44 /*VARIABLE*/;
    REAL* r1_8_darts44 /*VARIABLE*/;
    REAL* r1_9_darts44 /*VARIABLE*/;
    REAL* r2_0_darts44 /*VARIABLE*/;
    REAL* r2_1_darts44 /*VARIABLE*/;
    REAL* r2_10_darts44 /*VARIABLE*/;
    REAL* r2_11_darts44 /*VARIABLE*/;
    REAL* r2_12_darts44 /*VARIABLE*/;
    REAL* r2_13_darts44 /*VARIABLE*/;
    REAL* r2_14_darts44 /*VARIABLE*/;
    REAL* r2_15_darts44 /*VARIABLE*/;
    REAL* r2_16_darts44 /*VARIABLE*/;
    REAL* r2_17_darts44 /*VARIABLE*/;
    REAL* r2_18_darts44 /*VARIABLE*/;
    REAL* r2_19_darts44 /*VARIABLE*/;
    REAL* r2_2_darts44 /*VARIABLE*/;
    REAL* r2_20_darts44 /*VARIABLE*/;
    REAL* r2_21_darts44 /*VARIABLE*/;
    REAL* r2_22_darts44 /*VARIABLE*/;
    REAL* r2_23_darts44 /*VARIABLE*/;
    REAL* r2_24_darts44 /*VARIABLE*/;
    REAL* r2_25_darts44 /*VARIABLE*/;
    REAL* r2_26_darts44 /*VARIABLE*/;
    REAL* r2_27_darts44 /*VARIABLE*/;
    REAL* r2_28_darts44 /*VARIABLE*/;
    REAL* r2_29_darts44 /*VARIABLE*/;
    REAL* r2_3_darts44 /*VARIABLE*/;
    REAL* r2_30_darts44 /*VARIABLE*/;
    REAL* r2_31_darts44 /*VARIABLE*/;
    REAL* r2_4_darts44 /*VARIABLE*/;
    REAL* r2_5_darts44 /*VARIABLE*/;
    REAL* r2_6_darts44 /*VARIABLE*/;
    REAL* r2_7_darts44 /*VARIABLE*/;
    REAL* r2_8_darts44 /*VARIABLE*/;
    REAL* r2_9_darts44 /*VARIABLE*/;
    REAL* r3_0_darts44 /*VARIABLE*/;
    REAL* r3_1_darts44 /*VARIABLE*/;
    REAL* r3_10_darts44 /*VARIABLE*/;
    REAL* r3_11_darts44 /*VARIABLE*/;
    REAL* r3_12_darts44 /*VARIABLE*/;
    REAL* r3_13_darts44 /*VARIABLE*/;
    REAL* r3_14_darts44 /*VARIABLE*/;
    REAL* r3_15_darts44 /*VARIABLE*/;
    REAL* r3_16_darts44 /*VARIABLE*/;
    REAL* r3_17_darts44 /*VARIABLE*/;
    REAL* r3_18_darts44 /*VARIABLE*/;
    REAL* r3_19_darts44 /*VARIABLE*/;
    REAL* r3_2_darts44 /*VARIABLE*/;
    REAL* r3_20_darts44 /*VARIABLE*/;
    REAL* r3_21_darts44 /*VARIABLE*/;
    REAL* r3_22_darts44 /*VARIABLE*/;
    REAL* r3_23_darts44 /*VARIABLE*/;
    REAL* r3_24_darts44 /*VARIABLE*/;
    REAL* r3_25_darts44 /*VARIABLE*/;
    REAL* r3_26_darts44 /*VARIABLE*/;
    REAL* r3_27_darts44 /*VARIABLE*/;
    REAL* r3_28_darts44 /*VARIABLE*/;
    REAL* r3_29_darts44 /*VARIABLE*/;
    REAL* r3_3_darts44 /*VARIABLE*/;
    REAL* r3_30_darts44 /*VARIABLE*/;
    REAL* r3_31_darts44 /*VARIABLE*/;
    REAL* r3_4_darts44 /*VARIABLE*/;
    REAL* r3_5_darts44 /*VARIABLE*/;
    REAL* r3_6_darts44 /*VARIABLE*/;
    REAL* r3_7_darts44 /*VARIABLE*/;
    REAL* r3_8_darts44 /*VARIABLE*/;
    REAL* r3_9_darts44 /*VARIABLE*/;
    REAL* r4_0_darts44 /*VARIABLE*/;
    REAL* r4_1_darts44 /*VARIABLE*/;
    REAL* r4_10_darts44 /*VARIABLE*/;
    REAL* r4_11_darts44 /*VARIABLE*/;
    REAL* r4_12_darts44 /*VARIABLE*/;
    REAL* r4_13_darts44 /*VARIABLE*/;
    REAL* r4_14_darts44 /*VARIABLE*/;
    REAL* r4_15_darts44 /*VARIABLE*/;
    REAL* r4_16_darts44 /*VARIABLE*/;
    REAL* r4_17_darts44 /*VARIABLE*/;
    REAL* r4_18_darts44 /*VARIABLE*/;
    REAL* r4_19_darts44 /*VARIABLE*/;
    REAL* r4_2_darts44 /*VARIABLE*/;
    REAL* r4_20_darts44 /*VARIABLE*/;
    REAL* r4_21_darts44 /*VARIABLE*/;
    REAL* r4_22_darts44 /*VARIABLE*/;
    REAL* r4_23_darts44 /*VARIABLE*/;
    REAL* r4_24_darts44 /*VARIABLE*/;
    REAL* r4_25_darts44 /*VARIABLE*/;
    REAL* r4_26_darts44 /*VARIABLE*/;
    REAL* r4_27_darts44 /*VARIABLE*/;
    REAL* r4_28_darts44 /*VARIABLE*/;
    REAL* r4_29_darts44 /*VARIABLE*/;
    REAL* r4_3_darts44 /*VARIABLE*/;
    REAL* r4_30_darts44 /*VARIABLE*/;
    REAL* r4_31_darts44 /*VARIABLE*/;
    REAL* r4_4_darts44 /*VARIABLE*/;
    REAL* r4_5_darts44 /*VARIABLE*/;
    REAL* r4_6_darts44 /*VARIABLE*/;
    REAL* r4_7_darts44 /*VARIABLE*/;
    REAL* r4_8_darts44 /*VARIABLE*/;
    REAL* r4_9_darts44 /*VARIABLE*/;
    REAL* r5_0_darts44 /*VARIABLE*/;
    REAL* r5_1_darts44 /*VARIABLE*/;
    REAL* r5_10_darts44 /*VARIABLE*/;
    REAL* r5_11_darts44 /*VARIABLE*/;
    REAL* r5_12_darts44 /*VARIABLE*/;
    REAL* r5_13_darts44 /*VARIABLE*/;
    REAL* r5_14_darts44 /*VARIABLE*/;
    REAL* r5_15_darts44 /*VARIABLE*/;
    REAL* r5_16_darts44 /*VARIABLE*/;
    REAL* r5_17_darts44 /*VARIABLE*/;
    REAL* r5_18_darts44 /*VARIABLE*/;
    REAL* r5_19_darts44 /*VARIABLE*/;
    REAL* r5_2_darts44 /*VARIABLE*/;
    REAL* r5_20_darts44 /*VARIABLE*/;
    REAL* r5_21_darts44 /*VARIABLE*/;
    REAL* r5_22_darts44 /*VARIABLE*/;
    REAL* r5_23_darts44 /*VARIABLE*/;
    REAL* r5_24_darts44 /*VARIABLE*/;
    REAL* r5_25_darts44 /*VARIABLE*/;
    REAL* r5_26_darts44 /*VARIABLE*/;
    REAL* r5_27_darts44 /*VARIABLE*/;
    REAL* r5_28_darts44 /*VARIABLE*/;
    REAL* r5_29_darts44 /*VARIABLE*/;
    REAL* r5_3_darts44 /*VARIABLE*/;
    REAL* r5_30_darts44 /*VARIABLE*/;
    REAL* r5_31_darts44 /*VARIABLE*/;
    REAL* r5_4_darts44 /*VARIABLE*/;
    REAL* r5_5_darts44 /*VARIABLE*/;
    REAL* r5_6_darts44 /*VARIABLE*/;
    REAL* r5_7_darts44 /*VARIABLE*/;
    REAL* r5_8_darts44 /*VARIABLE*/;
    REAL* r5_9_darts44 /*VARIABLE*/;
    REAL* tmpi_darts44 /*VARIABLE*/;
    REAL* tmpr_darts44 /*VARIABLE*/;
    REAL* wi_darts44 /*VARIABLE*/;
    REAL* wr_darts44 /*VARIABLE*/;
    _task8650Inputs** task8650Inputs;
    _task8652Inputs** task8652Inputs;
    _checkInCodelets6503* checkInCodelets6503;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets6503* firstCodelet;
#endif
    _checkInCodelets6506* checkInCodelets6506;
    _checkInCodelets8647* checkInCodelets8647;
    _checkInCodelets8650* checkInCodelets8650;
    _checkInCodelets8652* checkInCodelets8652;
    _barrierCodelets8654* barrierCodelets8654;
    TP44(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_mainNextCodelet,
        darts::Codelet* in_mainSyncCodelet, TP44** in_ptrToThisFunctionTP, int in_a, int in_b,
        COMPLEX* in_in, COMPLEX* in_out, COMPLEX* in_W, int in_nW, int in_nWdn, int in_m);
    ~TP44();
    void setNewInputs(int in_a, int in_b, COMPLEX* in_in, COMPLEX* in_out, COMPLEX* in_W, int in_nW,
        int in_nWdn, int in_m, size_t codeletID);
};
/*TP8650: OMPTaskDirective*/
class TP8650 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets8651 : public darts::Codelet {
    public:
        TP8650* myTP;
        TP8650* inputsTPParent;
        _task8650Inputs* taskInputs;
        _checkInCodelets8651()
            : darts::Codelet()
        {
        }
        _checkInCodelets8651(uint32_t dep, uint32_t res, TP8650* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task8650Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets11514 : public darts::Codelet {
    public:
        TP8650* myTP;
        TP8650* inputsTPParent;
        _task8650Inputs* taskInputs;
        _checkInCodelets11514()
            : darts::Codelet()
        {
        }
        _checkInCodelets11514(uint32_t dep, uint32_t res, TP8650* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task8650Inputs)
        {
        }
        void fire(void);
    };
    TP44* TPParent;
    TP8650* controlTPParent;
    TP8650* inputsTPParent;
    _task8650Inputs* task8650Inputs;
    TP_fft_twiddle_32* TP8651Ptr;
    int TP8651_alreadyLaunched;
    _checkInCodelets8651 checkInCodelets8651;
    _checkInCodelets11514 checkInCodelets11514;
    TP8650(int in_numThreads, int in_mainCodeletID, TP44* in_TPParent,
        _task8650Inputs* in_task8650Inputs);
    ~TP8650();
};
/*TP8652: OMPTaskDirective*/
class TP8652 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets8653 : public darts::Codelet {
    public:
        TP8652* myTP;
        TP8652* inputsTPParent;
        _task8652Inputs* taskInputs;
        _checkInCodelets8653()
            : darts::Codelet()
        {
        }
        _checkInCodelets8653(uint32_t dep, uint32_t res, TP8652* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task8652Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets11515 : public darts::Codelet {
    public:
        TP8652* myTP;
        TP8652* inputsTPParent;
        _task8652Inputs* taskInputs;
        _checkInCodelets11515()
            : darts::Codelet()
        {
        }
        _checkInCodelets11515(uint32_t dep, uint32_t res, TP8652* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task8652Inputs)
        {
        }
        void fire(void);
    };
    TP44* TPParent;
    TP8652* controlTPParent;
    TP8652* inputsTPParent;
    _task8652Inputs* task8652Inputs;
    TP_fft_twiddle_32* TP8653Ptr;
    int TP8653_alreadyLaunched;
    _checkInCodelets8653 checkInCodelets8653;
    _checkInCodelets11515 checkInCodelets11515;
    TP8652(int in_numThreads, int in_mainCodeletID, TP44* in_TPParent,
        _task8652Inputs* in_task8652Inputs);
    ~TP8652();
};
/*TP46: fft_unshuffle_32*/
class TP46 : public ompTP {
public:
    class _checkInCodelets10816 : public darts::Codelet {
    public:
        TP46* myTP;
        TP46* inputsTPParent;
        _checkInCodelets10816()
            : darts::Codelet()
        {
        }
        _checkInCodelets10816(uint32_t dep, uint32_t res, TP46* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets10819 : public darts::Codelet {
    public:
        TP46* myTP;
        TP46* inputsTPParent;
        _checkInCodelets10819()
            : darts::Codelet()
        {
        }
        _checkInCodelets10819(uint32_t dep, uint32_t res, TP46* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets10972 : public darts::Codelet {
    public:
        TP46* myTP;
        TP46* inputsTPParent;
        _checkInCodelets10972()
            : darts::Codelet()
        {
        }
        _checkInCodelets10972(uint32_t dep, uint32_t res, TP46* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets10975 : public darts::Codelet {
    public:
        TP46* myTP;
        TP46* inputsTPParent;
        _checkInCodelets10975()
            : darts::Codelet()
        {
        }
        _checkInCodelets10975(uint32_t dep, uint32_t res, TP46* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets10977 : public darts::Codelet {
    public:
        TP46* myTP;
        TP46* inputsTPParent;
        _checkInCodelets10977()
            : darts::Codelet()
        {
        }
        _checkInCodelets10977(uint32_t dep, uint32_t res, TP46* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets10979 : public darts::Codelet {
    public:
        TP46* inputsTPParent;
        _barrierCodelets10979()
            : darts::Codelet()
        {
        }
        _barrierCodelets10979(uint32_t dep, uint32_t res, TP46* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP46** ptrToThisFunctionTP;
    TP46* inputsTPParent;
    TP46* controlTPParent;
    darts::Codelet** nextCodeletsfft_unshuffle_32;
    darts::Codelet** nextSyncCodeletsfft_unshuffle_32;
    int* a_darts46 /*VARIABLE*/;
    int* b_darts46 /*VARIABLE*/;
    COMPLEX** in_darts46 /*VARIABLE*/;
    COMPLEX** out_darts46 /*VARIABLE*/;
    int* m_darts46 /*VARIABLE*/;
    int* ab_darts46 /*VARIABLE*/;
    int* i_darts46 /*VARIABLE*/;
    COMPLEX** ip_darts46 /*VARIABLE*/;
    COMPLEX** jp_darts46 /*VARIABLE*/;
    _task10975Inputs** task10975Inputs;
    _task10977Inputs** task10977Inputs;
    _checkInCodelets10816* checkInCodelets10816;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets10816* firstCodelet;
#endif
    _checkInCodelets10819* checkInCodelets10819;
    _checkInCodelets10972* checkInCodelets10972;
    _checkInCodelets10975* checkInCodelets10975;
    _checkInCodelets10977* checkInCodelets10977;
    _barrierCodelets10979* barrierCodelets10979;
    TP46(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_mainNextCodelet,
        darts::Codelet* in_mainSyncCodelet, TP46** in_ptrToThisFunctionTP, int in_a, int in_b,
        COMPLEX* in_in, COMPLEX* in_out, int in_m);
    ~TP46();
    void setNewInputs(
        int in_a, int in_b, COMPLEX* in_in, COMPLEX* in_out, int in_m, size_t codeletID);
};
/*TP10975: OMPTaskDirective*/
class TP10975 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets10976 : public darts::Codelet {
    public:
        TP10975* myTP;
        TP10975* inputsTPParent;
        _task10975Inputs* taskInputs;
        _checkInCodelets10976()
            : darts::Codelet()
        {
        }
        _checkInCodelets10976(uint32_t dep, uint32_t res, TP10975* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task10975Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets11510 : public darts::Codelet {
    public:
        TP10975* myTP;
        TP10975* inputsTPParent;
        _task10975Inputs* taskInputs;
        _checkInCodelets11510()
            : darts::Codelet()
        {
        }
        _checkInCodelets11510(uint32_t dep, uint32_t res, TP10975* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task10975Inputs)
        {
        }
        void fire(void);
    };
    TP46* TPParent;
    TP10975* controlTPParent;
    TP10975* inputsTPParent;
    _task10975Inputs* task10975Inputs;
    TP_fft_unshuffle_32* TP10976Ptr;
    int TP10976_alreadyLaunched;
    _checkInCodelets10976 checkInCodelets10976;
    _checkInCodelets11510 checkInCodelets11510;
    TP10975(int in_numThreads, int in_mainCodeletID, TP46* in_TPParent,
        _task10975Inputs* in_task10975Inputs);
    ~TP10975();
};
/*TP10977: OMPTaskDirective*/
class TP10977 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets10978 : public darts::Codelet {
    public:
        TP10977* myTP;
        TP10977* inputsTPParent;
        _task10977Inputs* taskInputs;
        _checkInCodelets10978()
            : darts::Codelet()
        {
        }
        _checkInCodelets10978(uint32_t dep, uint32_t res, TP10977* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task10977Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets11511 : public darts::Codelet {
    public:
        TP10977* myTP;
        TP10977* inputsTPParent;
        _task10977Inputs* taskInputs;
        _checkInCodelets11511()
            : darts::Codelet()
        {
        }
        _checkInCodelets11511(uint32_t dep, uint32_t res, TP10977* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task10977Inputs)
        {
        }
        void fire(void);
    };
    TP46* TPParent;
    TP10977* controlTPParent;
    TP10977* inputsTPParent;
    _task10977Inputs* task10977Inputs;
    TP_fft_unshuffle_32* TP10978Ptr;
    int TP10978_alreadyLaunched;
    _checkInCodelets10978 checkInCodelets10978;
    _checkInCodelets11511 checkInCodelets11511;
    TP10977(int in_numThreads, int in_mainCodeletID, TP46* in_TPParent,
        _task10977Inputs* in_task10977Inputs);
    ~TP10977();
};
/*TP48: fft_aux*/
class TP48 : public ompTP {
public:
    class _checkInCodelets11146 : public darts::Codelet {
    public:
        TP48* myTP;
        TP48* inputsTPParent;
        _checkInCodelets11146()
            : darts::Codelet()
        {
        }
        _checkInCodelets11146(uint32_t dep, uint32_t res, TP48* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets11177 : public darts::Codelet {
    public:
        TP48* myTP;
        TP48* inputsTPParent;
        _checkInCodelets11177()
            : darts::Codelet()
        {
        }
        _checkInCodelets11177(uint32_t dep, uint32_t res, TP48* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets11223 : public darts::Codelet {
    public:
        TP48* myTP;
        TP48* inputsTPParent;
        _checkInCodelets11223()
            : darts::Codelet()
        {
        }
        _checkInCodelets11223(uint32_t dep, uint32_t res, TP48* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets11261 : public darts::Codelet {
    public:
        TP48* inputsTPParent;
        _barrierCodelets11261()
            : darts::Codelet()
        {
        }
        _barrierCodelets11261(uint32_t dep, uint32_t res, TP48* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets11262 : public darts::Codelet {
    public:
        TP48* myTP;
        TP48* inputsTPParent;
        _checkInCodelets11262()
            : darts::Codelet()
        {
        }
        _checkInCodelets11262(uint32_t dep, uint32_t res, TP48* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets11180 : public darts::Codelet {
    public:
        TP48* myTP;
        TP48* inputsTPParent;
        _checkInCodelets11180()
            : darts::Codelet()
        {
        }
        _checkInCodelets11180(uint32_t dep, uint32_t res, TP48* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets11211 : public darts::Codelet {
    public:
        TP48* inputsTPParent;
        _barrierCodelets11211()
            : darts::Codelet()
        {
        }
        _barrierCodelets11211(uint32_t dep, uint32_t res, TP48* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets11213 : public darts::Codelet {
    public:
        TP48* myTP;
        TP48* inputsTPParent;
        _checkInCodelets11213()
            : darts::Codelet()
        {
        }
        _checkInCodelets11213(uint32_t dep, uint32_t res, TP48* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets11212 : public darts::Codelet {
    public:
        TP48* myTP;
        TP48* inputsTPParent;
        _checkInCodelets11212()
            : darts::Codelet()
        {
        }
        _checkInCodelets11212(uint32_t dep, uint32_t res, TP48* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets11215 : public darts::Codelet {
    public:
        TP48* myTP;
        TP48* inputsTPParent;
        _checkInCodelets11215()
            : darts::Codelet()
        {
        }
        _checkInCodelets11215(uint32_t dep, uint32_t res, TP48* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets11222 : public darts::Codelet {
    public:
        TP48* inputsTPParent;
        _barrierCodelets11222()
            : darts::Codelet()
        {
        }
        _barrierCodelets11222(uint32_t dep, uint32_t res, TP48* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets11183 : public darts::Codelet {
    public:
        TP48* myTP;
        TP48* inputsTPParent;
        _checkInCodelets11183()
            : darts::Codelet()
        {
        }
        _checkInCodelets11183(uint32_t dep, uint32_t res, TP48* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets11185 : public darts::Codelet {
    public:
        TP48* myTP;
        TP48* inputsTPParent;
        _checkInCodelets11185()
            : darts::Codelet()
        {
        }
        _checkInCodelets11185(uint32_t dep, uint32_t res, TP48* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets11188 : public darts::Codelet {
    public:
        TP48* myTP;
        TP48* inputsTPParent;
        _checkInCodelets11188()
            : darts::Codelet()
        {
        }
        _checkInCodelets11188(uint32_t dep, uint32_t res, TP48* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets11190 : public darts::Codelet {
    public:
        TP48* myTP;
        TP48* inputsTPParent;
        _checkInCodelets11190()
            : darts::Codelet()
        {
        }
        _checkInCodelets11190(uint32_t dep, uint32_t res, TP48* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets11193 : public darts::Codelet {
    public:
        TP48* myTP;
        TP48* inputsTPParent;
        _checkInCodelets11193()
            : darts::Codelet()
        {
        }
        _checkInCodelets11193(uint32_t dep, uint32_t res, TP48* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets11195 : public darts::Codelet {
    public:
        TP48* myTP;
        TP48* inputsTPParent;
        _checkInCodelets11195()
            : darts::Codelet()
        {
        }
        _checkInCodelets11195(uint32_t dep, uint32_t res, TP48* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets11198 : public darts::Codelet {
    public:
        TP48* myTP;
        TP48* inputsTPParent;
        _checkInCodelets11198()
            : darts::Codelet()
        {
        }
        _checkInCodelets11198(uint32_t dep, uint32_t res, TP48* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets11200 : public darts::Codelet {
    public:
        TP48* myTP;
        TP48* inputsTPParent;
        _checkInCodelets11200()
            : darts::Codelet()
        {
        }
        _checkInCodelets11200(uint32_t dep, uint32_t res, TP48* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets11203 : public darts::Codelet {
    public:
        TP48* myTP;
        TP48* inputsTPParent;
        _checkInCodelets11203()
            : darts::Codelet()
        {
        }
        _checkInCodelets11203(uint32_t dep, uint32_t res, TP48* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets11205 : public darts::Codelet {
    public:
        TP48* myTP;
        TP48* inputsTPParent;
        _checkInCodelets11205()
            : darts::Codelet()
        {
        }
        _checkInCodelets11205(uint32_t dep, uint32_t res, TP48* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets11226 : public darts::Codelet {
    public:
        TP48* myTP;
        TP48* inputsTPParent;
        _checkInCodelets11226()
            : darts::Codelet()
        {
        }
        _checkInCodelets11226(uint32_t dep, uint32_t res, TP48* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets11229 : public darts::Codelet {
    public:
        TP48* myTP;
        TP48* inputsTPParent;
        _checkInCodelets11229()
            : darts::Codelet()
        {
        }
        _checkInCodelets11229(uint32_t dep, uint32_t res, TP48* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets11232 : public darts::Codelet {
    public:
        TP48* myTP;
        TP48* inputsTPParent;
        _checkInCodelets11232()
            : darts::Codelet()
        {
        }
        _checkInCodelets11232(uint32_t dep, uint32_t res, TP48* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets11235 : public darts::Codelet {
    public:
        TP48* myTP;
        TP48* inputsTPParent;
        _checkInCodelets11235()
            : darts::Codelet()
        {
        }
        _checkInCodelets11235(uint32_t dep, uint32_t res, TP48* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets11238 : public darts::Codelet {
    public:
        TP48* myTP;
        TP48* inputsTPParent;
        _checkInCodelets11238()
            : darts::Codelet()
        {
        }
        _checkInCodelets11238(uint32_t dep, uint32_t res, TP48* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets11241 : public darts::Codelet {
    public:
        TP48* myTP;
        TP48* inputsTPParent;
        _checkInCodelets11241()
            : darts::Codelet()
        {
        }
        _checkInCodelets11241(uint32_t dep, uint32_t res, TP48* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets11244 : public darts::Codelet {
    public:
        TP48* myTP;
        TP48* inputsTPParent;
        _checkInCodelets11244()
            : darts::Codelet()
        {
        }
        _checkInCodelets11244(uint32_t dep, uint32_t res, TP48* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets11247 : public darts::Codelet {
    public:
        TP48* myTP;
        TP48* inputsTPParent;
        _checkInCodelets11247()
            : darts::Codelet()
        {
        }
        _checkInCodelets11247(uint32_t dep, uint32_t res, TP48* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets11250 : public darts::Codelet {
    public:
        TP48* myTP;
        TP48* inputsTPParent;
        _checkInCodelets11250()
            : darts::Codelet()
        {
        }
        _checkInCodelets11250(uint32_t dep, uint32_t res, TP48* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets11254 : public darts::Codelet {
    public:
        TP48* myTP;
        TP48* inputsTPParent;
        _checkInCodelets11254()
            : darts::Codelet()
        {
        }
        _checkInCodelets11254(uint32_t dep, uint32_t res, TP48* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP48** ptrToThisFunctionTP;
    TP48* inputsTPParent;
    TP48* controlTPParent;
    darts::Codelet** nextCodeletsfft_aux;
    darts::Codelet** nextSyncCodeletsfft_aux;
    int* n_darts48 /*VARIABLE*/;
    COMPLEX** in_darts48 /*VARIABLE*/;
    COMPLEX** out_darts48 /*VARIABLE*/;
    int** factors_darts48 /*VARIABLE*/;
    COMPLEX** W_darts48 /*VARIABLE*/;
    int* nW_darts48 /*VARIABLE*/;
    int* k_darts48 /*VARIABLE*/;
    int* m_darts48 /*VARIABLE*/;
    int* r_darts48 /*VARIABLE*/;
    unsigned int TP11212_LoopCounter;
    unsigned int* TP11212_LoopCounterPerThread;
    tbb::concurrent_vector<TP11212*> TP11212PtrVec;
    TP_unshuffle* TP11205Ptr;
    int TP11205_alreadyLaunched;
    _task11183Inputs** task11183Inputs;
    _task11188Inputs** task11188Inputs;
    _task11193Inputs** task11193Inputs;
    _task11198Inputs** task11198Inputs;
    _task11203Inputs** task11203Inputs;
    _task11217Inputs** task11217Inputs;
    _task11226Inputs** task11226Inputs;
    _task11232Inputs** task11232Inputs;
    _task11238Inputs** task11238Inputs;
    _task11244Inputs** task11244Inputs;
    _task11250Inputs** task11250Inputs;
    _task11254Inputs** task11254Inputs;
    _checkInCodelets11146* checkInCodelets11146;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets11146* firstCodelet;
#endif
    _checkInCodelets11177* checkInCodelets11177;
    _checkInCodelets11223* checkInCodelets11223;
    _barrierCodelets11261* barrierCodelets11261;
    _checkInCodelets11262* checkInCodelets11262;
    _checkInCodelets11180* checkInCodelets11180;
    _barrierCodelets11211* barrierCodelets11211;
    _checkInCodelets11213* checkInCodelets11213;
    _checkInCodelets11212* checkInCodelets11212;
    _checkInCodelets11215* checkInCodelets11215;
    _barrierCodelets11222* barrierCodelets11222;
    _checkInCodelets11183* checkInCodelets11183;
    _checkInCodelets11185* checkInCodelets11185;
    _checkInCodelets11188* checkInCodelets11188;
    _checkInCodelets11190* checkInCodelets11190;
    _checkInCodelets11193* checkInCodelets11193;
    _checkInCodelets11195* checkInCodelets11195;
    _checkInCodelets11198* checkInCodelets11198;
    _checkInCodelets11200* checkInCodelets11200;
    _checkInCodelets11203* checkInCodelets11203;
    _checkInCodelets11205* checkInCodelets11205;
    _checkInCodelets11226* checkInCodelets11226;
    _checkInCodelets11229* checkInCodelets11229;
    _checkInCodelets11232* checkInCodelets11232;
    _checkInCodelets11235* checkInCodelets11235;
    _checkInCodelets11238* checkInCodelets11238;
    _checkInCodelets11241* checkInCodelets11241;
    _checkInCodelets11244* checkInCodelets11244;
    _checkInCodelets11247* checkInCodelets11247;
    _checkInCodelets11250* checkInCodelets11250;
    _checkInCodelets11254* checkInCodelets11254;
    TP48(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_mainNextCodelet,
        darts::Codelet* in_mainSyncCodelet, TP48** in_ptrToThisFunctionTP, int in_n, COMPLEX* in_in,
        COMPLEX* in_out, int* in_factors, COMPLEX* in_W, int in_nW);
    ~TP48();
    void setNewInputs(int in_n, COMPLEX* in_in, COMPLEX* in_out, int* in_factors, COMPLEX* in_W,
        int in_nW, size_t codeletID);
};
/*TP11212: ForStmt*/
class TP11212 : public ompTP {
public:
    class _checkInCodelets11217 : public darts::Codelet {
    public:
        TP11212* myTP;
        TP48* inputsTPParent;
        _checkInCodelets11217()
            : darts::Codelet()
        {
        }
        _checkInCodelets11217(uint32_t dep, uint32_t res, TP11212* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP48* TPParent;
    TP11212* controlTPParent;
    TP48* inputsTPParent;
    TP11212** ptrToThisTP;
    _checkInCodelets11217* checkInCodelets11217;
#if USE_SPIN_CODELETS == 0
    _checkInCodelets11217* firstCodelet;
#endif
    TP11212(int in_numThreads, int in_mainCodeletID, TP48* in_TPParent, TP48* in_inputsTPParent,
        TP11212** in_ptrToThisTP);
    ~TP11212();
};
/*TP11217: OMPTaskDirective*/
class TP11217 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets11218 : public darts::Codelet {
    public:
        TP11217* myTP;
        TP11217* inputsTPParent;
        _task11217Inputs* taskInputs;
        _checkInCodelets11218()
            : darts::Codelet()
        {
        }
        _checkInCodelets11218(uint32_t dep, uint32_t res, TP11217* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task11217Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets11529 : public darts::Codelet {
    public:
        TP11217* myTP;
        TP11217* inputsTPParent;
        _task11217Inputs* taskInputs;
        _checkInCodelets11529()
            : darts::Codelet()
        {
        }
        _checkInCodelets11529(uint32_t dep, uint32_t res, TP11217* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task11217Inputs)
        {
        }
        void fire(void);
    };
    TP11212* TPParent;
    TP11217* controlTPParent;
    TP11217* inputsTPParent;
    _task11217Inputs* task11217Inputs;
    TP_fft_aux* TP11218Ptr;
    int TP11218_alreadyLaunched;
    _checkInCodelets11218 checkInCodelets11218;
    _checkInCodelets11529 checkInCodelets11529;
    TP11217(int in_numThreads, int in_mainCodeletID, TP11212* in_TPParent,
        _task11217Inputs* in_task11217Inputs);
    ~TP11217();
};
/*TP11183: OMPTaskDirective*/
class TP11183 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets11184 : public darts::Codelet {
    public:
        TP11183* myTP;
        TP11183* inputsTPParent;
        _task11183Inputs* taskInputs;
        _checkInCodelets11184()
            : darts::Codelet()
        {
        }
        _checkInCodelets11184(uint32_t dep, uint32_t res, TP11183* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task11183Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets11524 : public darts::Codelet {
    public:
        TP11183* myTP;
        TP11183* inputsTPParent;
        _task11183Inputs* taskInputs;
        _checkInCodelets11524()
            : darts::Codelet()
        {
        }
        _checkInCodelets11524(uint32_t dep, uint32_t res, TP11183* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task11183Inputs)
        {
        }
        void fire(void);
    };
    TP48* TPParent;
    TP11183* controlTPParent;
    TP11183* inputsTPParent;
    _task11183Inputs* task11183Inputs;
    TP_fft_unshuffle_32* TP11184Ptr;
    int TP11184_alreadyLaunched;
    _checkInCodelets11184 checkInCodelets11184;
    _checkInCodelets11524 checkInCodelets11524;
    TP11183(int in_numThreads, int in_mainCodeletID, TP48* in_TPParent,
        _task11183Inputs* in_task11183Inputs);
    ~TP11183();
};
/*TP11188: OMPTaskDirective*/
class TP11188 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets11189 : public darts::Codelet {
    public:
        TP11188* myTP;
        TP11188* inputsTPParent;
        _task11188Inputs* taskInputs;
        _checkInCodelets11189()
            : darts::Codelet()
        {
        }
        _checkInCodelets11189(uint32_t dep, uint32_t res, TP11188* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task11188Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets11525 : public darts::Codelet {
    public:
        TP11188* myTP;
        TP11188* inputsTPParent;
        _task11188Inputs* taskInputs;
        _checkInCodelets11525()
            : darts::Codelet()
        {
        }
        _checkInCodelets11525(uint32_t dep, uint32_t res, TP11188* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task11188Inputs)
        {
        }
        void fire(void);
    };
    TP48* TPParent;
    TP11188* controlTPParent;
    TP11188* inputsTPParent;
    _task11188Inputs* task11188Inputs;
    TP_fft_unshuffle_16* TP11189Ptr;
    int TP11189_alreadyLaunched;
    _checkInCodelets11189 checkInCodelets11189;
    _checkInCodelets11525 checkInCodelets11525;
    TP11188(int in_numThreads, int in_mainCodeletID, TP48* in_TPParent,
        _task11188Inputs* in_task11188Inputs);
    ~TP11188();
};
/*TP11193: OMPTaskDirective*/
class TP11193 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets11194 : public darts::Codelet {
    public:
        TP11193* myTP;
        TP11193* inputsTPParent;
        _task11193Inputs* taskInputs;
        _checkInCodelets11194()
            : darts::Codelet()
        {
        }
        _checkInCodelets11194(uint32_t dep, uint32_t res, TP11193* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task11193Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets11526 : public darts::Codelet {
    public:
        TP11193* myTP;
        TP11193* inputsTPParent;
        _task11193Inputs* taskInputs;
        _checkInCodelets11526()
            : darts::Codelet()
        {
        }
        _checkInCodelets11526(uint32_t dep, uint32_t res, TP11193* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task11193Inputs)
        {
        }
        void fire(void);
    };
    TP48* TPParent;
    TP11193* controlTPParent;
    TP11193* inputsTPParent;
    _task11193Inputs* task11193Inputs;
    TP_fft_unshuffle_8* TP11194Ptr;
    int TP11194_alreadyLaunched;
    _checkInCodelets11194 checkInCodelets11194;
    _checkInCodelets11526 checkInCodelets11526;
    TP11193(int in_numThreads, int in_mainCodeletID, TP48* in_TPParent,
        _task11193Inputs* in_task11193Inputs);
    ~TP11193();
};
/*TP11198: OMPTaskDirective*/
class TP11198 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets11199 : public darts::Codelet {
    public:
        TP11198* myTP;
        TP11198* inputsTPParent;
        _task11198Inputs* taskInputs;
        _checkInCodelets11199()
            : darts::Codelet()
        {
        }
        _checkInCodelets11199(uint32_t dep, uint32_t res, TP11198* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task11198Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets11527 : public darts::Codelet {
    public:
        TP11198* myTP;
        TP11198* inputsTPParent;
        _task11198Inputs* taskInputs;
        _checkInCodelets11527()
            : darts::Codelet()
        {
        }
        _checkInCodelets11527(uint32_t dep, uint32_t res, TP11198* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task11198Inputs)
        {
        }
        void fire(void);
    };
    TP48* TPParent;
    TP11198* controlTPParent;
    TP11198* inputsTPParent;
    _task11198Inputs* task11198Inputs;
    TP_fft_unshuffle_4* TP11199Ptr;
    int TP11199_alreadyLaunched;
    _checkInCodelets11199 checkInCodelets11199;
    _checkInCodelets11527 checkInCodelets11527;
    TP11198(int in_numThreads, int in_mainCodeletID, TP48* in_TPParent,
        _task11198Inputs* in_task11198Inputs);
    ~TP11198();
};
/*TP11203: OMPTaskDirective*/
class TP11203 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets11204 : public darts::Codelet {
    public:
        TP11203* myTP;
        TP11203* inputsTPParent;
        _task11203Inputs* taskInputs;
        _checkInCodelets11204()
            : darts::Codelet()
        {
        }
        _checkInCodelets11204(uint32_t dep, uint32_t res, TP11203* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task11203Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets11528 : public darts::Codelet {
    public:
        TP11203* myTP;
        TP11203* inputsTPParent;
        _task11203Inputs* taskInputs;
        _checkInCodelets11528()
            : darts::Codelet()
        {
        }
        _checkInCodelets11528(uint32_t dep, uint32_t res, TP11203* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task11203Inputs)
        {
        }
        void fire(void);
    };
    TP48* TPParent;
    TP11203* controlTPParent;
    TP11203* inputsTPParent;
    _task11203Inputs* task11203Inputs;
    TP_fft_unshuffle_2* TP11204Ptr;
    int TP11204_alreadyLaunched;
    _checkInCodelets11204 checkInCodelets11204;
    _checkInCodelets11528 checkInCodelets11528;
    TP11203(int in_numThreads, int in_mainCodeletID, TP48* in_TPParent,
        _task11203Inputs* in_task11203Inputs);
    ~TP11203();
};
/*TP11226: OMPTaskDirective*/
class TP11226 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets11227 : public darts::Codelet {
    public:
        TP11226* myTP;
        TP11226* inputsTPParent;
        _task11226Inputs* taskInputs;
        _checkInCodelets11227()
            : darts::Codelet()
        {
        }
        _checkInCodelets11227(uint32_t dep, uint32_t res, TP11226* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task11226Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets11531 : public darts::Codelet {
    public:
        TP11226* myTP;
        TP11226* inputsTPParent;
        _task11226Inputs* taskInputs;
        _checkInCodelets11531()
            : darts::Codelet()
        {
        }
        _checkInCodelets11531(uint32_t dep, uint32_t res, TP11226* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task11226Inputs)
        {
        }
        void fire(void);
    };
    TP48* TPParent;
    TP11226* controlTPParent;
    TP11226* inputsTPParent;
    _task11226Inputs* task11226Inputs;
    TP_fft_twiddle_2* TP11227Ptr;
    int TP11227_alreadyLaunched;
    _checkInCodelets11227 checkInCodelets11227;
    _checkInCodelets11531 checkInCodelets11531;
    TP11226(int in_numThreads, int in_mainCodeletID, TP48* in_TPParent,
        _task11226Inputs* in_task11226Inputs);
    ~TP11226();
};
/*TP11232: OMPTaskDirective*/
class TP11232 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets11233 : public darts::Codelet {
    public:
        TP11232* myTP;
        TP11232* inputsTPParent;
        _task11232Inputs* taskInputs;
        _checkInCodelets11233()
            : darts::Codelet()
        {
        }
        _checkInCodelets11233(uint32_t dep, uint32_t res, TP11232* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task11232Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets11532 : public darts::Codelet {
    public:
        TP11232* myTP;
        TP11232* inputsTPParent;
        _task11232Inputs* taskInputs;
        _checkInCodelets11532()
            : darts::Codelet()
        {
        }
        _checkInCodelets11532(uint32_t dep, uint32_t res, TP11232* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task11232Inputs)
        {
        }
        void fire(void);
    };
    TP48* TPParent;
    TP11232* controlTPParent;
    TP11232* inputsTPParent;
    _task11232Inputs* task11232Inputs;
    TP_fft_twiddle_4* TP11233Ptr;
    int TP11233_alreadyLaunched;
    _checkInCodelets11233 checkInCodelets11233;
    _checkInCodelets11532 checkInCodelets11532;
    TP11232(int in_numThreads, int in_mainCodeletID, TP48* in_TPParent,
        _task11232Inputs* in_task11232Inputs);
    ~TP11232();
};
/*TP11238: OMPTaskDirective*/
class TP11238 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets11239 : public darts::Codelet {
    public:
        TP11238* myTP;
        TP11238* inputsTPParent;
        _task11238Inputs* taskInputs;
        _checkInCodelets11239()
            : darts::Codelet()
        {
        }
        _checkInCodelets11239(uint32_t dep, uint32_t res, TP11238* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task11238Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets11533 : public darts::Codelet {
    public:
        TP11238* myTP;
        TP11238* inputsTPParent;
        _task11238Inputs* taskInputs;
        _checkInCodelets11533()
            : darts::Codelet()
        {
        }
        _checkInCodelets11533(uint32_t dep, uint32_t res, TP11238* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task11238Inputs)
        {
        }
        void fire(void);
    };
    TP48* TPParent;
    TP11238* controlTPParent;
    TP11238* inputsTPParent;
    _task11238Inputs* task11238Inputs;
    TP_fft_twiddle_8* TP11239Ptr;
    int TP11239_alreadyLaunched;
    _checkInCodelets11239 checkInCodelets11239;
    _checkInCodelets11533 checkInCodelets11533;
    TP11238(int in_numThreads, int in_mainCodeletID, TP48* in_TPParent,
        _task11238Inputs* in_task11238Inputs);
    ~TP11238();
};
/*TP11244: OMPTaskDirective*/
class TP11244 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets11245 : public darts::Codelet {
    public:
        TP11244* myTP;
        TP11244* inputsTPParent;
        _task11244Inputs* taskInputs;
        _checkInCodelets11245()
            : darts::Codelet()
        {
        }
        _checkInCodelets11245(uint32_t dep, uint32_t res, TP11244* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task11244Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets11534 : public darts::Codelet {
    public:
        TP11244* myTP;
        TP11244* inputsTPParent;
        _task11244Inputs* taskInputs;
        _checkInCodelets11534()
            : darts::Codelet()
        {
        }
        _checkInCodelets11534(uint32_t dep, uint32_t res, TP11244* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task11244Inputs)
        {
        }
        void fire(void);
    };
    TP48* TPParent;
    TP11244* controlTPParent;
    TP11244* inputsTPParent;
    _task11244Inputs* task11244Inputs;
    TP_fft_twiddle_16* TP11245Ptr;
    int TP11245_alreadyLaunched;
    _checkInCodelets11245 checkInCodelets11245;
    _checkInCodelets11534 checkInCodelets11534;
    TP11244(int in_numThreads, int in_mainCodeletID, TP48* in_TPParent,
        _task11244Inputs* in_task11244Inputs);
    ~TP11244();
};
/*TP11250: OMPTaskDirective*/
class TP11250 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets11251 : public darts::Codelet {
    public:
        TP11250* myTP;
        TP11250* inputsTPParent;
        _task11250Inputs* taskInputs;
        _checkInCodelets11251()
            : darts::Codelet()
        {
        }
        _checkInCodelets11251(uint32_t dep, uint32_t res, TP11250* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task11250Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets11535 : public darts::Codelet {
    public:
        TP11250* myTP;
        TP11250* inputsTPParent;
        _task11250Inputs* taskInputs;
        _checkInCodelets11535()
            : darts::Codelet()
        {
        }
        _checkInCodelets11535(uint32_t dep, uint32_t res, TP11250* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task11250Inputs)
        {
        }
        void fire(void);
    };
    TP48* TPParent;
    TP11250* controlTPParent;
    TP11250* inputsTPParent;
    _task11250Inputs* task11250Inputs;
    TP_fft_twiddle_32* TP11251Ptr;
    int TP11251_alreadyLaunched;
    _checkInCodelets11251 checkInCodelets11251;
    _checkInCodelets11535 checkInCodelets11535;
    TP11250(int in_numThreads, int in_mainCodeletID, TP48* in_TPParent,
        _task11250Inputs* in_task11250Inputs);
    ~TP11250();
};
/*TP11254: OMPTaskDirective*/
class TP11254 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets11255 : public darts::Codelet {
    public:
        TP11254* myTP;
        TP11254* inputsTPParent;
        _task11254Inputs* taskInputs;
        _checkInCodelets11255()
            : darts::Codelet()
        {
        }
        _checkInCodelets11255(uint32_t dep, uint32_t res, TP11254* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task11254Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets11536 : public darts::Codelet {
    public:
        TP11254* myTP;
        TP11254* inputsTPParent;
        _task11254Inputs* taskInputs;
        _checkInCodelets11536()
            : darts::Codelet()
        {
        }
        _checkInCodelets11536(uint32_t dep, uint32_t res, TP11254* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task11254Inputs)
        {
        }
        void fire(void);
    };
    TP48* TPParent;
    TP11254* controlTPParent;
    TP11254* inputsTPParent;
    _task11254Inputs* task11254Inputs;
    TP_fft_twiddle_gen* TP11255Ptr;
    int TP11255_alreadyLaunched;
    _checkInCodelets11255 checkInCodelets11255;
    _checkInCodelets11536 checkInCodelets11536;
    TP11254(int in_numThreads, int in_mainCodeletID, TP48* in_TPParent,
        _task11254Inputs* in_task11254Inputs);
    ~TP11254();
};
/*TP11383: OMPParallelDirective*/
class TP11383 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets11383 : public darts::Codelet {
    public:
        TP11383* inputsTPParent;
        _barrierCodelets11383()
            : darts::Codelet()
        {
        }
        _barrierCodelets11383(uint32_t dep, uint32_t res, TP11383* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets11384 : public darts::Codelet {
    public:
        TP11383* myTP;
        TP11383* inputsTPParent;
        _checkInCodelets11384()
            : darts::Codelet()
        {
        }
        _checkInCodelets11384(uint32_t dep, uint32_t res, TP11383* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets11384 : public darts::Codelet {
    public:
        TP11383* inputsTPParent;
        _barrierCodelets11384()
            : darts::Codelet()
        {
        }
        _barrierCodelets11384(uint32_t dep, uint32_t res, TP11383* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP11383* TPParent;
    TP11383* controlTPParent;
    TP11383* inputsTPParent;
    COMPLEX** W_darts11383; /*OMP_SHARED - INPUT*/
    int* n_darts11383; /*OMP_SHARED - INPUT*/
    TP11384* TP11384Ptr;
    size_t TP11384_alreadyLaunched;
    _task11385Inputs** task11385Inputs;
    _barrierCodelets11383* barrierCodelets11383;
    _checkInCodelets11384* checkInCodelets11384;
    _barrierCodelets11384* barrierCodelets11384;
    TP11383(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet, COMPLEX** in_W,
        int* in_n);
    ~TP11383();
};
/*TP11384: OMPSingleDirective*/
class TP11384 : public ompOMPSingleDirectiveTP {
public:
    class _checkInCodelets11385 : public darts::Codelet {
    public:
        TP11384* myTP;
        TP11384* inputsTPParent;
        _checkInCodelets11385()
            : darts::Codelet()
        {
        }
        _checkInCodelets11385(uint32_t dep, uint32_t res, TP11384* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP11383* TPParent;
    TP11384* controlTPParent;
    TP11384* inputsTPParent;
    COMPLEX** W_darts11384; /*OMP_SHARED - INPUT*/
    int* n_darts11384; /*OMP_SHARED - INPUT*/
    _task11385Inputs** task11385Inputs;
    _checkInCodelets11385 checkInCodelets11385;
    TP11384(
        int in_numThreads, int in_mainCodeletID, TP11383* in_TPParent, COMPLEX** in_W, int* in_n);
    ~TP11384();
};
/*TP11385: OMPTaskDirective*/
class TP11385 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets11386 : public darts::Codelet {
    public:
        TP11385* myTP;
        TP11385* inputsTPParent;
        _task11385Inputs* taskInputs;
        _checkInCodelets11386()
            : darts::Codelet()
        {
        }
        _checkInCodelets11386(uint32_t dep, uint32_t res, TP11385* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task11385Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets11518 : public darts::Codelet {
    public:
        TP11385* myTP;
        TP11385* inputsTPParent;
        _task11385Inputs* taskInputs;
        _checkInCodelets11518()
            : darts::Codelet()
        {
        }
        _checkInCodelets11518(uint32_t dep, uint32_t res, TP11385* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task11385Inputs)
        {
        }
        void fire(void);
    };
    TP11384* TPParent;
    TP11385* controlTPParent;
    TP11385* inputsTPParent;
    _task11385Inputs* task11385Inputs;
    TP_compute_w_coefficients* TP11386Ptr;
    int TP11386_alreadyLaunched;
    _checkInCodelets11386 checkInCodelets11386;
    _checkInCodelets11518 checkInCodelets11518;
    TP11385(int in_numThreads, int in_mainCodeletID, TP11384* in_TPParent,
        _task11385Inputs* in_task11385Inputs);
    ~TP11385();
};
/*TP11407: OMPParallelDirective*/
class TP11407 : public darts::ThreadedProcedure {
public:
    class _barrierCodelets11407 : public darts::Codelet {
    public:
        TP11407* inputsTPParent;
        _barrierCodelets11407()
            : darts::Codelet()
        {
        }
        _barrierCodelets11407(uint32_t dep, uint32_t res, TP11407* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _checkInCodelets11408 : public darts::Codelet {
    public:
        TP11407* myTP;
        TP11407* inputsTPParent;
        _checkInCodelets11408()
            : darts::Codelet()
        {
        }
        _checkInCodelets11408(uint32_t dep, uint32_t res, TP11407* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    class _barrierCodelets11408 : public darts::Codelet {
    public:
        TP11407* inputsTPParent;
        _barrierCodelets11408()
            : darts::Codelet()
        {
        }
        _barrierCodelets11408(uint32_t dep, uint32_t res, TP11407* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    darts::Codelet* nextCodelet;
    TP11407* TPParent;
    TP11407* controlTPParent;
    TP11407* inputsTPParent;
    COMPLEX** W_darts11407; /*OMP_SHARED - INPUT*/
    int* factors_darts11407; /*OMP_SHARED - INPUT*/
    uint64_t factors_outer11407_size;
    COMPLEX** in_darts11407; /*OMP_SHARED - INPUT*/
    int* n_darts11407; /*OMP_SHARED - INPUT*/
    COMPLEX** out_darts11407; /*OMP_SHARED - INPUT*/
    TP11408* TP11408Ptr;
    size_t TP11408_alreadyLaunched;
    _task11409Inputs** task11409Inputs;
    _barrierCodelets11407* barrierCodelets11407;
    _checkInCodelets11408* checkInCodelets11408;
    _barrierCodelets11408* barrierCodelets11408;
    TP11407(int in_numThreads, int in_mainCodeletID, darts::Codelet* in_nextCodelet, COMPLEX** in_W,
        int* in_factors, int in_factors_outer11407_size, COMPLEX** in_in, int* in_n,
        COMPLEX** in_out);
    ~TP11407();
};
/*TP11408: OMPSingleDirective*/
class TP11408 : public ompOMPSingleDirectiveTP {
public:
    class _checkInCodelets11409 : public darts::Codelet {
    public:
        TP11408* myTP;
        TP11408* inputsTPParent;
        _checkInCodelets11409()
            : darts::Codelet()
        {
        }
        _checkInCodelets11409(uint32_t dep, uint32_t res, TP11408* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
        {
        }
        void fire(void);
    };
    TP11407* TPParent;
    TP11408* controlTPParent;
    TP11408* inputsTPParent;
    COMPLEX** W_darts11408; /*OMP_SHARED - INPUT*/
    int* factors_darts11408; /*OMP_SHARED - INPUT*/
    uint64_t factors_outer11408_size;
    COMPLEX** in_darts11408; /*OMP_SHARED - INPUT*/
    int* n_darts11408; /*OMP_SHARED - INPUT*/
    COMPLEX** out_darts11408; /*OMP_SHARED - INPUT*/
    _task11409Inputs** task11409Inputs;
    _checkInCodelets11409 checkInCodelets11409;
    TP11408(int in_numThreads, int in_mainCodeletID, TP11407* in_TPParent, COMPLEX** in_W,
        int* in_factors, int in_factors_outer11408_size, COMPLEX** in_in, int* in_n,
        COMPLEX** in_out);
    ~TP11408();
};
/*TP11409: OMPTaskDirective*/
class TP11409 : public ompOMPTaskDirectiveTP {
public:
    class _checkInCodelets11410 : public darts::Codelet {
    public:
        TP11409* myTP;
        TP11409* inputsTPParent;
        _task11409Inputs* taskInputs;
        _checkInCodelets11410()
            : darts::Codelet()
        {
        }
        _checkInCodelets11410(uint32_t dep, uint32_t res, TP11409* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task11409Inputs)
        {
        }
        void fire(void);
    };
    class _checkInCodelets11519 : public darts::Codelet {
    public:
        TP11409* myTP;
        TP11409* inputsTPParent;
        _task11409Inputs* taskInputs;
        _checkInCodelets11519()
            : darts::Codelet()
        {
        }
        _checkInCodelets11519(uint32_t dep, uint32_t res, TP11409* myTP, uint32_t id)
            : darts::Codelet(dep, res, myTP, LONGWAIT, id)
            , myTP(myTP)
            , inputsTPParent(myTP->inputsTPParent)
            , taskInputs(inputsTPParent->task11409Inputs)
        {
        }
        void fire(void);
    };
    TP11408* TPParent;
    TP11409* controlTPParent;
    TP11409* inputsTPParent;
    _task11409Inputs* task11409Inputs;
    TP_fft_aux* TP11410Ptr;
    int TP11410_alreadyLaunched;
    _checkInCodelets11410 checkInCodelets11410;
    _checkInCodelets11519 checkInCodelets11519;
    TP11409(int in_numThreads, int in_mainCodeletID, TP11408* in_TPParent,
        _task11409Inputs* in_task11409Inputs);
    ~TP11409();
};
#endif
