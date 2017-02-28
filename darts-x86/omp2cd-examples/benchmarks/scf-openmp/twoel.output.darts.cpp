#include "twoel.output.darts.h"
using namespace darts;
using namespace std;
/*Function: twoel_i_j_k_l_all_different, ID: 39*/
void twoel_i_j_k_l_all_different(double tol2e_over_schwmax)
{
    /*twoel_i_j_k_l_all_different:39*/
    /*CompoundStmt:93*/
    long long temp_icut1 = 0;
    long long temp_icut2 = 0;
    long long temp_icut3 = 0;
    long long temp_icut4 = 0;
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP98>(ompNumThreads * DARTS_CODELETS_MULT, 0,
            RuntimeFinalCodelet, (long long*)&((temp_icut1)), (long long*)&((temp_icut2)),
            (long long*)&((temp_icut3)), (long long*)&((temp_icut4)),
            (double*)&((tol2e_over_schwmax))));
    }
    icut1 += temp_icut1;
    icut2 += temp_icut2;
    icut3 += temp_icut3;
    icut4 += temp_icut4;
}
/*Function: twoel_i_eq_j, ID: 40*/
void twoel_i_eq_j(double tol2e_over_schwmax)
{
    /*twoel_i_eq_j:40*/
    /*CompoundStmt:401*/
    long long temp_icut1 = 0;
    long long temp_icut2 = 0;
    long long temp_icut3 = 0;
    long long temp_icut4 = 0;
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP406>(ompNumThreads * DARTS_CODELETS_MULT, 0,
            RuntimeFinalCodelet, (long long*)&((temp_icut1)), (long long*)&((temp_icut2)),
            (long long*)&((temp_icut3)), (long long*)&((temp_icut4)),
            (double*)&((tol2e_over_schwmax))));
    }
    icut1 += temp_icut1;
    icut2 += temp_icut2;
    icut3 += temp_icut3;
    icut4 += temp_icut4;
}
/*Function: twoel_k_eq_l, ID: 41*/
void twoel_k_eq_l(double tol2e_over_schwmax)
{
    /*twoel_k_eq_l:41*/
    /*CompoundStmt:535*/
    long long temp_icut1 = 0;
    long long temp_icut2 = 0;
    long long temp_icut3 = 0;
    long long temp_icut4 = 0;
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP540>(ompNumThreads * DARTS_CODELETS_MULT, 0,
            RuntimeFinalCodelet, (long long*)&((temp_icut1)), (long long*)&((temp_icut2)),
            (long long*)&((temp_icut3)), (long long*)&((temp_icut4)),
            (double*)&((tol2e_over_schwmax))));
    }
    icut1 += temp_icut1;
    icut2 += temp_icut2;
    icut3 += temp_icut3;
    icut4 += temp_icut4;
}
/*Function: twoel_ij_eq_kl, ID: 42*/
void twoel_ij_eq_kl(double tol2e_over_schwmax)
{
    /*twoel_ij_eq_kl:42*/
    /*CompoundStmt:658*/
    long long temp_icut1 = 0;
    long long temp_icut2 = 0;
    long long temp_icut3 = 0;
    long long temp_icut4 = 0;
    int i, j;
    int i_off, j_off;
    double* fock = g_fock;
    i_off = -nbfn;
    for (i = 0; i < nbfn; i++) {
        i_off += nbfn;
        j_off = i_off;
        double g_dens_i_i = g_dens[((i_off) + (i))];
        for (j = i + 1; j < nbfn; j++) {
            j_off += nbfn;
            double g_dens_j_i = g_dens[((j_off) + (i))];
            double g_dens_i_j = g_dens_j_i;
            double g_dens_j_j = g_dens[((j_off) + (j))];
            double tol2e_over_g_schwarz_i_j = 9.9999999999999995E-7 / g_schwarz[((i_off) + (j))];
            if (g_schwarz[((i_off) + (j))] < tol2e_over_g_schwarz_i_j) {
                temp_icut1 += 4;
                continue;
            }
            temp_icut3 += 4;
            double gg = g_fast(((i_off) + (j)), ((i_off) + (j)));
            fock[((i_off) + (j))] += (gg * (g_dens_i_j));
            fock[((i_off) + (i))] -= (0.5 * gg * (g_dens_j_j));
            ;
            fock[((i_off) + (j))] += (gg * (g_dens_j_i));
            fock[((i_off) + (j))] -= (0.5 * gg * (g_dens_j_i));
            ;
            fock[((j_off) + (i))] += (gg * (g_dens_i_j));
            fock[((j_off) + (i))] -= (0.5 * gg * (g_dens_i_j));
            ;
            fock[((j_off) + (i))] += (gg * (g_dens_j_i));
            fock[((j_off) + (j))] -= (0.5 * gg * (g_dens_i_i));
            ;
        }
    }
    icut1 += temp_icut1;
    icut2 += temp_icut2;
    icut3 += temp_icut3;
    icut4 += temp_icut4;
}
/*Function: twoel_i_eq_j_and_k_eq_l, ID: 43*/
void twoel_i_eq_j_and_k_eq_l(double tol2e_over_schwmax)
{
    /*twoel_i_eq_j_and_k_eq_l:43*/
    /*CompoundStmt:751*/
    long long temp_icut1 = 0;
    long long temp_icut2 = 0;
    long long temp_icut3 = 0;
    long long temp_icut4 = 0;
    int i, k;
    int i_off, k_off;
    double* fock = g_fock;
    i_off = -nbfn;
    for (i = 0; i < nbfn; i++) {
        i_off += nbfn;
        double g_dens_i_i = g_dens[((i_off) + (i))];
        double tol2e_over_g_schwarz_i_i = 9.9999999999999995E-7 / g_schwarz[((i_off) + (i))];
        if (g_schwarz[((i_off) + (i))] < tol2e_over_schwmax) {
            temp_icut1 += 2 * (nbfn - i - 1);
            continue;
        }
        k_off = i_off;
        for (k = i + 1; k < nbfn; k++) {
            k_off += nbfn;
            double g_dens_k_i = g_dens[((k_off) + (i))];
            double g_dens_i_k = g_dens_k_i;
            double g_dens_k_k = g_dens[((k_off) + (k))];
            if (g_schwarz[((k_off) + (k))] < tol2e_over_g_schwarz_i_i) {
                temp_icut2 += 2;
                continue;
            }
            temp_icut3 += 2;
            double gg = g_fast(((i_off) + (i)), ((k_off) + (k)));
            fock[((i_off) + (i))] += (gg * (g_dens_k_k));
            fock[((i_off) + (k))] -= (0.5 * gg * (g_dens_i_k));
            ;
            fock[((k_off) + (k))] += (gg * (g_dens_i_i));
            fock[((k_off) + (i))] -= (0.5 * gg * (g_dens_k_i));
            ;
        }
    }
    icut1 += temp_icut1;
    icut2 += temp_icut2;
    icut3 += temp_icut3;
    icut4 += temp_icut4;
}
/*Function: twoel_i_eq_j_eq_k_eq_l, ID: 44*/
void twoel_i_eq_j_eq_k_eq_l(double tol2e_over_schwmax)
{
    /*twoel_i_eq_j_eq_k_eq_l:44*/
    /*CompoundStmt:832*/
    long long temp_icut1 = 0;
    long long temp_icut2 = 0;
    long long temp_icut3 = 0;
    long long temp_icut4 = 0;
    int i;
    int i_off;
    double* fock = g_fock;
    i_off = -nbfn;
    for (i = 0; i < nbfn; i++) {
        i_off += nbfn;
        double g_dens_i_i = g_dens[((i_off) + (i))];
        double tol2e_over_g_schwarz_i_i = 9.9999999999999995E-7 / g_schwarz[((i_off) + (i))];
        if (g_schwarz[((i_off) + (i))] < tol2e_over_g_schwarz_i_i) {
            temp_icut2 += 1;
            continue;
        }
        temp_icut3 += 1;
        double gg = g_fast(((i_off) + (i)), ((i_off) + (i)));
        fock[((i_off) + (i))] += (gg * (g_dens_i_i));
        fock[((i_off) + (i))] -= (0.5 * gg * (g_dens_i_i));
        ;
    }
    icut1 += temp_icut1;
    icut2 += temp_icut2;
    icut3 += temp_icut3;
    icut4 += temp_icut4;
}
/*Function: twoel_fast, ID: 45*/
double twoel_fast(double schwmax)
{
    /*twoel_fast:45*/
    /*CompoundStmt:880*/
    double tol2e_over_schwmax = 9.9999999999999995E-7 / schwmax;
    twoel_ij_eq_kl(tol2e_over_schwmax);
    twoel_i_eq_j_and_k_eq_l(tol2e_over_schwmax);
    twoel_i_eq_j_eq_k_eq_l(tol2e_over_schwmax);
    twoel_i_eq_j(tol2e_over_schwmax);
    twoel_k_eq_l(tol2e_over_schwmax);
    twoel_i_j_k_l_all_different(tol2e_over_schwmax);
    return (0.5 * contract_matrices(g_fock, g_dens));
}
/*TP98: OMPParallelDirective*/
void TP98::_barrierCodelets98::fire(void)
{
    TP98* myTP = static_cast<TP98*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
void TP98::_checkInCodelets100::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 100: DeclStmt*/
    this->inputsTPParent->fock_darts98[this->getID()]
        = &g_thread_fock[omp_get_thread_num() * nbfn * nbfn];
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 100 nextRegion: 106 */
    myTP->controlTPParent->checkInCodelets106[this->getID()].decDep();
}
void TP98::_checkInCodelets106::fire(void)
{
    /*printOMPForCodeletFireCode, inlined: 0*/
    /*region 106 0*/
    /*Determine the TP to which this codelet belongs and check if this codelet spawns the TP or if
     * it signals it using dispatchCodelet()*/
    size_t idx = this->getID() / myTP->codeletsPerTP106;
    if (idx < myTP->TPsToUse106) {
        if (!__sync_val_compare_and_swap(&(myTP->TP106_alreadyLaunched[idx]), 0, 1)) {
            int range = abs(nbfn - 0) / 1;
            int rangePerCodelet = range / myTP->TPsToUse106;
            int minIteration = min<int>(nbfn, 0);
            int remainderRange = range % myTP->TPsToUse106;
            int initIteration = rangePerCodelet * idx;
            int lastIteration = rangePerCodelet * (idx + 1);
            if (remainderRange != 0) {
                if (idx < (uint32_t)remainderRange) {
                    initIteration += idx;
                    lastIteration += (idx + 1);
                } else {
                    initIteration += remainderRange;
                    lastIteration += remainderRange;
                }
            }
            initIteration = initIteration * 1 + minIteration;
            lastIteration = lastIteration * 1 + minIteration;
            if (0 < nbfn) {
                initIteration = min(initIteration, lastIteration);
                lastIteration = max(initIteration, lastIteration);
            } else {
                initIteration = max(initIteration, lastIteration);
                lastIteration = min(initIteration, lastIteration);
            }
            if (idx == myTP->TPsToUse106 - 1) {
                lastIteration = nbfn;
            }
#if USEINVOKE == 1
            invoke<TP106>(myTP, myTP->codeletsPerTP106 * DARTS_CODELETS_MULT, this->getID(), myTP,
                initIteration, lastIteration, &(*(this->inputsTPParent->temp_icut1_darts98)),
                &(*(this->inputsTPParent->temp_icut2_darts98)),
                &(*(this->inputsTPParent->temp_icut3_darts98)),
                &(*(this->inputsTPParent->temp_icut4_darts98)),
                &(*(this->inputsTPParent->tol2e_over_schwmax_darts98)), &(myTP->TP106Ptr[idx]));
#else
            place<TP106>(idx, myTP, myTP->codeletsPerTP106 * DARTS_CODELETS_MULT, this->getID(),
                myTP, initIteration, lastIteration, &(*(this->inputsTPParent->temp_icut1_darts98)),
                &(*(this->inputsTPParent->temp_icut2_darts98)),
                &(*(this->inputsTPParent->temp_icut3_darts98)),
                &(*(this->inputsTPParent->temp_icut4_darts98)),
                &(*(this->inputsTPParent->tol2e_over_schwmax_darts98)), &(myTP->TP106Ptr[idx]));
#endif
        } else {
            if (myTP->TP106Ptr[idx] != nullptr) {
                myTP->TP106Ptr[idx]->dispatchCodelet(this->getID());
            } else {
                this->resetCodelet();
                this->decDep();
            }
        }
    }
}
void TP98::_barrierCodelets106::fire(void)
{
    TP98* myTP = static_cast<TP98*>(myTP_);
    {
        for (size_t codeletsCounter = 0; codeletsCounter < myTP->numThreads; codeletsCounter++) {
            myTP->checkInCodelets271[codeletsCounter].decDep();
        }
    }
}
void TP98::_checkInCodelets271::fire(void)
{

    /*printing node 271: DeclStmt*/
    this->inputsTPParent->fock_2_darts98[this->getID()] = g_partial_fock;

    /*printing node 272: DeclStmt*/
    this->inputsTPParent->nbfn_squared_darts98[this->getID()] = nbfn * nbfn;

    /*printing node 274: DeclStmt*/
    this->inputsTPParent->numthreads_darts98[this->getID()] = omp_get_max_threads();
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 271 nextRegion: 276 */
    myTP->controlTPParent->checkInCodelets276[this->getID()].decDep();
}
void TP98::_checkInCodelets276::fire(void)
{
    /*printOMPForCodeletFireCode, inlined: 0*/
    /*region 276 0*/
    /*Determine the TP to which this codelet belongs and check if this codelet spawns the TP or if
     * it signals it using dispatchCodelet()*/
    size_t idx = this->getID() / myTP->codeletsPerTP276;
    if (idx < myTP->TPsToUse276) {
        if (!__sync_val_compare_and_swap(&(myTP->TP276_alreadyLaunched[idx]), 0, 1)) {
            int range = abs((this->inputsTPParent->nbfn_squared_darts98[this->getID()]) - 0) / 1;
            int rangePerCodelet = range / myTP->TPsToUse276;
            int minIteration
                = min<int>((this->inputsTPParent->nbfn_squared_darts98[this->getID()]), 0);
            int remainderRange = range % myTP->TPsToUse276;
            int initIteration = rangePerCodelet * idx;
            int lastIteration = rangePerCodelet * (idx + 1);
            if (remainderRange != 0) {
                if (idx < (uint32_t)remainderRange) {
                    initIteration += idx;
                    lastIteration += (idx + 1);
                } else {
                    initIteration += remainderRange;
                    lastIteration += remainderRange;
                }
            }
            initIteration = initIteration * 1 + minIteration;
            lastIteration = lastIteration * 1 + minIteration;
            if (0 < (this->inputsTPParent->nbfn_squared_darts98[this->getID()])) {
                initIteration = min(initIteration, lastIteration);
                lastIteration = max(initIteration, lastIteration);
            } else {
                initIteration = max(initIteration, lastIteration);
                lastIteration = min(initIteration, lastIteration);
            }
            if (idx == myTP->TPsToUse276 - 1) {
                lastIteration = (this->inputsTPParent->nbfn_squared_darts98[this->getID()]);
            }
#if USEINVOKE == 1
            invoke<TP276>(myTP, myTP->codeletsPerTP276 * DARTS_CODELETS_MULT, this->getID(), myTP,
                initIteration, lastIteration, &(myTP->TP276Ptr[idx]));
#else
            place<TP276>(idx, myTP, myTP->codeletsPerTP276 * DARTS_CODELETS_MULT, this->getID(),
                myTP, initIteration, lastIteration, &(myTP->TP276Ptr[idx]));
#endif
        } else {
            if (myTP->TP276Ptr[idx] != nullptr) {
                myTP->TP276Ptr[idx]->dispatchCodelet(this->getID());
            } else {
                this->resetCodelet();
                this->decDep();
            }
        }
    }
}
void TP98::_barrierCodelets276::fire(void)
{
    TP98* myTP = static_cast<TP98*>(myTP_);
    {
        for (size_t codeletsCounter = 0; codeletsCounter < myTP->numThreads; codeletsCounter++) {
            myTP->checkInCodelets325[codeletsCounter].decDep();
        }
    }
}
void TP98::_checkInCodelets325::fire(void)
{
    /*printOMPForCodeletFireCode, inlined: 0*/
    /*region 325 0*/
    /*Determine the TP to which this codelet belongs and check if this codelet spawns the TP or if
     * it signals it using dispatchCodelet()*/
    size_t idx = this->getID() / myTP->codeletsPerTP325;
    if (idx < myTP->TPsToUse325) {
        if (!__sync_val_compare_and_swap(&(myTP->TP325_alreadyLaunched[idx]), 0, 1)) {
            int range = abs(nbfn - 0) / 1;
            int rangePerCodelet = range / myTP->TPsToUse325;
            int minIteration = min<int>(nbfn, 0);
            int remainderRange = range % myTP->TPsToUse325;
            int initIteration = rangePerCodelet * idx;
            int lastIteration = rangePerCodelet * (idx + 1);
            if (remainderRange != 0) {
                if (idx < (uint32_t)remainderRange) {
                    initIteration += idx;
                    lastIteration += (idx + 1);
                } else {
                    initIteration += remainderRange;
                    lastIteration += remainderRange;
                }
            }
            initIteration = initIteration * 1 + minIteration;
            lastIteration = lastIteration * 1 + minIteration;
            if (0 < nbfn) {
                initIteration = min(initIteration, lastIteration);
                lastIteration = max(initIteration, lastIteration);
            } else {
                initIteration = max(initIteration, lastIteration);
                lastIteration = min(initIteration, lastIteration);
            }
            if (idx == myTP->TPsToUse325 - 1) {
                lastIteration = nbfn;
            }
#if USEINVOKE == 1
            invoke<TP325>(myTP, myTP->codeletsPerTP325 * DARTS_CODELETS_MULT, this->getID(), myTP,
                initIteration, lastIteration, &(myTP->TP325Ptr[idx]));
#else
            place<TP325>(idx, myTP, myTP->codeletsPerTP325 * DARTS_CODELETS_MULT, this->getID(),
                myTP, initIteration, lastIteration, &(myTP->TP325Ptr[idx]));
#endif
        } else {
            if (myTP->TP325Ptr[idx] != nullptr) {
                myTP->TP325Ptr[idx]->dispatchCodelet(this->getID());
            } else {
                this->resetCodelet();
                this->decDep();
            }
        }
    }
}
void TP98::_barrierCodelets325::fire(void)
{
    TP98* myTP = static_cast<TP98*>(myTP_);
    myTP->TPParent->barrierCodelets98[0].setDep(0);
    myTP->add(&(myTP->TPParent->barrierCodelets98[0]));
}
TP98::TP98(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet,
    long long* in_temp_icut1, long long* in_temp_icut2, long long* in_temp_icut3,
    long long* in_temp_icut4, double* in_tol2e_over_schwmax)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , temp_icut1_darts98(in_temp_icut1) /*OMP_SHARED - INPUT*/
    , temp_icut2_darts98(in_temp_icut2) /*OMP_SHARED - INPUT*/
    , temp_icut3_darts98(in_temp_icut3) /*OMP_SHARED - INPUT*/
    , temp_icut4_darts98(in_temp_icut4) /*OMP_SHARED - INPUT*/
    , tol2e_over_schwmax_darts98(in_tol2e_over_schwmax) /*OMP_SHARED - INPUT*/
    , fock_darts98(new double*[this->numThreads]) /*VARIABLE*/
    , fock_2_darts98(new double*[this->numThreads]) /*VARIABLE*/
    , nbfn_squared_darts98(new long long[this->numThreads]) /*VARIABLE*/
    , numthreads_darts98(new int[this->numThreads]) /*VARIABLE*/
    , TP106Ptr(new TP106*[NUMTPS106])
    , TP106_alreadyLaunched(new size_t[NUMTPS106])
    , numTPsSet106(0)
    , numTPsReady106(0)
    , TPsToUse106(NUMTPS106)
    , codeletsPerTP106(this->numThreads / NUMTPS106)
    , totalCodelets106(this->TPsToUse106 * this->codeletsPerTP106)
    , TP276Ptr(new TP276*[NUMTPS276])
    , TP276_alreadyLaunched(new size_t[NUMTPS276])
    , numTPsSet276(0)
    , numTPsReady276(0)
    , TPsToUse276(NUMTPS276)
    , codeletsPerTP276(this->numThreads / NUMTPS276)
    , totalCodelets276(this->TPsToUse276 * this->codeletsPerTP276)
    , TP325Ptr(new TP325*[NUMTPS325])
    , TP325_alreadyLaunched(new size_t[NUMTPS325])
    , numTPsSet325(0)
    , numTPsReady325(0)
    , TPsToUse325(NUMTPS325)
    , codeletsPerTP325(this->numThreads / NUMTPS325)
    , totalCodelets325(this->TPsToUse325 * this->codeletsPerTP325)
    , barrierCodelets98(new _barrierCodelets98[1])
    , checkInCodelets100(new _checkInCodelets100[this->numThreads])
    , checkInCodelets106(new _checkInCodelets106[this->numThreads])
    , barrierCodelets106(new _barrierCodelets106[1])
    , checkInCodelets271(new _checkInCodelets271[this->numThreads])
    , checkInCodelets276(new _checkInCodelets276[this->numThreads])
    , barrierCodelets276(new _barrierCodelets276[1])
    , checkInCodelets325(new _checkInCodelets325[this->numThreads])
    , barrierCodelets325(new _barrierCodelets325[1])
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets98[0] = _barrierCodelets98(ompNumThreads, ompNumThreads, this, 0);
    barrierCodelets325[0] = _barrierCodelets325(NUMTPS325, NUMTPS325, this, 0);
    barrierCodelets276[0] = _barrierCodelets276(NUMTPS276, NUMTPS276, this, 0);
    barrierCodelets106[0] = _barrierCodelets106(NUMTPS106, NUMTPS106, this, 0);
    _checkInCodelets325* checkInCodelets325Ptr = (this->checkInCodelets325);
    for (int i = 0; i < NUMTPS325; i++) {
        TP325Ptr[i] = nullptr;
        TP325_alreadyLaunched[i] = 0;
    }
    _checkInCodelets276* checkInCodelets276Ptr = (this->checkInCodelets276);
    for (int i = 0; i < NUMTPS276; i++) {
        TP276Ptr[i] = nullptr;
        TP276_alreadyLaunched[i] = 0;
    }
    _checkInCodelets271* checkInCodelets271Ptr = (this->checkInCodelets271);
    _checkInCodelets106* checkInCodelets106Ptr = (this->checkInCodelets106);
    for (int i = 0; i < NUMTPS106; i++) {
        TP106Ptr[i] = nullptr;
        TP106_alreadyLaunched[i] = 0;
    }
    _checkInCodelets100* checkInCodelets100Ptr = (this->checkInCodelets100);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets325Ptr) = _checkInCodelets325(1, 1, this, codeletCounter);
        checkInCodelets325Ptr++;
        (*checkInCodelets276Ptr) = _checkInCodelets276(1, 1, this, codeletCounter);
        checkInCodelets276Ptr++;
        (*checkInCodelets271Ptr) = _checkInCodelets271(1, 1, this, codeletCounter);
        checkInCodelets271Ptr++;
        (*checkInCodelets106Ptr) = _checkInCodelets106(1, 1, this, codeletCounter);
        checkInCodelets106Ptr++;
        (*checkInCodelets100Ptr) = _checkInCodelets100(1, 1, this, codeletCounter);
        (*checkInCodelets100Ptr).decDep();
        checkInCodelets100Ptr++;
    }
}
TP98::~TP98()
{
    delete[] fock_darts98;
    delete[] fock_2_darts98;
    delete[] nbfn_squared_darts98;
    delete[] numthreads_darts98;
    delete[] barrierCodelets98;
    delete[] barrierCodelets325;
    delete[] checkInCodelets325;
    delete[] barrierCodelets276;
    delete[] checkInCodelets276;
    delete[] checkInCodelets271;
    delete[] barrierCodelets106;
    delete[] checkInCodelets106;
    delete[] checkInCodelets100;
}
/*TP106: OMPForDirective*/
void TP106::_barrierCodelets106::fire(void)
{
    TP106* myTP = static_cast<TP106*>(myTP_);
    myTP->controlTPParent->TPParent->barrierCodelets106[0].decDep();
}
bool TP106::requestNewRangeIterations106(int* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Runtime */
    /*Chunk = 1*/
    bool isThereNewIteration = false;
    if (ompLoopSched106 == 1) {
        /*Static Scheduling*/
        int tempStartRange = rangePerCodelet106 * codeletID;
        int tempEndRange = rangePerCodelet106 * (codeletID + 1);
        if (remainderRange106 != 0) {
            if (codeletID < (uint32_t)remainderRange106) {
                tempStartRange += codeletID;
                tempEndRange += (codeletID + 1);
            } else {
                tempStartRange += remainderRange106;
                tempEndRange += remainderRange106;
            }
        }
        tempStartRange = tempStartRange * 1 + minIteration106;
        tempEndRange = tempEndRange * 1 + minIteration106;
        if (tempStartRange != tempEndRange) {
            isThereNewIteration = true;
        }
        if (initIteration106 < lastIteration106) {
            (this->inputsTPParent->i_darts106[codeletID]) = min(tempStartRange, tempEndRange);
            *endRange = max(tempStartRange, tempEndRange);
        } else {
            (this->inputsTPParent->i_darts106[codeletID]) = max(tempStartRange, tempEndRange);
            *endRange = min(tempStartRange, tempEndRange);
        }
        if (codeletID == this->numThreads - 1) {
            *endRange = lastIteration106;
        }
    } else if (ompLoopSched106 == 2) {
        /*Dynamic Scheduling*/
        if (initIteration106 < lastIteration106) {
            (this->inputsTPParent->i_darts106[codeletID])
                = __sync_fetch_and_add(&(nextIteration106), ompLoopChunk106 * 1);
            *endRange = (this->inputsTPParent->i_darts106[codeletID]) + ompLoopChunk106 * 1;
        } else {
            (this->inputsTPParent->i_darts106[codeletID])
                = __sync_fetch_and_sub(&(nextIteration106), ompLoopChunk106 * 1);
            *endRange = (this->inputsTPParent->i_darts106[codeletID]) - ompLoopChunk106 * 1;
        }
        if ((this->inputsTPParent->i_darts106[codeletID]) < lastIteration106) {
            isThereNewIteration = true;
        }
    }
    return isThereNewIteration;
}
void TP106::_checkInCodelets107::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    /*Init the vars for this region*/
    /*Get pointer from parent for variable
     with shared scope in this region but private
     in the enclosing one.*/
    this->inputsTPParent->fock_darts106[this->getLocalID()]
        = (double**)&(myTP->TPParent->inputsTPParent->fock_darts98[this->getID()]);

    /*printing node 107: ForStmt*/
    /*var: fock*/
    /*var: temp_icut1*/
    /*var: temp_icut2*/
    /*var: temp_icut3*/
    /*var: temp_icut4*/
    /*var: tol2e_over_schwmax*/
    /*Declare local reduction vars*/
    long long temp_icut3_LocalReductionVar = (long long)0;
    long long temp_icut4_LocalReductionVar = (long long)0;
    long long temp_icut2_LocalReductionVar = (long long)0;
    long long temp_icut1_LocalReductionVar = (long long)0;
    double*** fock = &(this->inputsTPParent->fock_darts106[this->getLocalID()]);
    (void)fock /*OMP_SHARED_PRIVATE*/;
    double* tol2e_over_schwmax = (this->inputsTPParent->tol2e_over_schwmax_darts106);
    (void)tol2e_over_schwmax /*OMP_SHARED*/;
    int* i = &(this->inputsTPParent->i_darts106[this->getLocalID()]);
    (void)i /*PRIVATE*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations106(
        (int*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        goto updateReduceVars;
    }
    while (isThereNewIteration) {
        for (int i_darts_counter_temp106 = (*i); i_darts_counter_temp106 < endRange
             && i_darts_counter_temp106 < this->inputsTPParent->lastIteration106;
             i_darts_counter_temp106++) {
            {
                int i_off = (i_darts_counter_temp106)*nbfn;
                {
                    /*Loop's init*/
                    int j = (i_darts_counter_temp106) + 1;
                    for (; j < nbfn; j++) {
                        int j_off = j * nbfn;
                        double g_dens_j_i = g_dens[((j_off) + ((i_darts_counter_temp106)))];
                        double g_dens_i_j = g_dens_j_i;
                        double tol2e_over_g_schwarz_i_j
                            = 9.9999999999999995E-7 / g_schwarz[((i_off) + (j))];
                        if (g_schwarz[((i_off) + (j))] < (*(tol2e_over_schwmax))) {
                            (*(&(temp_icut1_LocalReductionVar))) += 8
                                * (-2 * j + (i_darts_counter_temp106) * (3 - 2 * nbfn)
                                      + (i_darts_counter_temp106) * (i_darts_counter_temp106)
                                      + nbfn * (nbfn - 1))
                                / 2;
                            continue;
                        }
                        int k_off = i_off - nbfn;
                        {
                            /*Loop's init*/
                            int k = (i_darts_counter_temp106);
                            for (; k < nbfn; k++) {
                                k_off += nbfn;
                                double g_dens_k_i = g_dens[((k_off) + ((i_darts_counter_temp106)))];
                                double g_dens_i_k = g_dens_k_i;
                                double g_dens_k_j = g_dens[((k_off) + (j))];
                                double g_dens_j_k = g_dens_k_j;
                                if (g_schwarz_max_j[k] < tol2e_over_g_schwarz_i_j) {
                                    (*(&(temp_icut4_LocalReductionVar)))
                                        += 8 * ((k == (i_darts_counter_temp106)) ? nbfn - j - 1
                                                                                 : nbfn - k - 1);
                                    continue;
                                }
                                int l_off = ((k == (i_darts_counter_temp106)) ? j_off : k_off);
                                {
                                    /*Loop's init*/
                                    int l = 1 + ((k == (i_darts_counter_temp106)) ? j : k);
                                    for (; l < nbfn; l++) {
                                        l_off += nbfn;
                                        if (g_schwarz[((k_off) + (l))] < tol2e_over_g_schwarz_i_j) {
                                            (*(&(temp_icut2_LocalReductionVar))) += 8;
                                            continue;
                                        }
                                        double g_dens_l_i
                                            = g_dens[((l_off) + ((i_darts_counter_temp106)))];
                                        double g_dens_i_l = g_dens_l_i;
                                        double g_dens_l_j = g_dens[((l_off) + (j))];
                                        double g_dens_j_l = g_dens_l_j;
                                        double g_dens_l_k = g_dens[((l_off) + (k))];
                                        double g_dens_k_l = g_dens_l_k;
                                        (*(&(temp_icut3_LocalReductionVar))) += 8;
                                        double gg = g_fast(((i_off) + (j)), ((k_off) + (l)));
                                        (*(*fock))[((i_off) + (j))]
                                            += (gg * (g_dens_k_l)) + (gg * (g_dens_k_l));
                                        (*(*fock))[((k_off) + (l))]
                                            += (gg * (g_dens_i_j)) + (gg * (g_dens_i_j));
                                        (*(*fock))[((i_off) + (k))] -= (0.5 * gg * (g_dens_j_l));
                                        (*(*fock))[((i_off) + (l))] -= (0.5 * gg * (g_dens_j_k));
                                        (*(*fock))[((j_off) + (k))] -= (0.5 * gg * (g_dens_i_l));
                                        (*(*fock))[((j_off) + (l))] -= (0.5 * gg * (g_dens_i_k));
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        /*dont ask for more iterations if the scheduling selected at runtime is static*/
        if (this->inputsTPParent->ompLoopSched106 == 1)
            break;
        isThereNewIteration = this->inputsTPParent->requestNewRangeIterations106(
            &(this->endRange), this->getLocalID());
    }
updateReduceVars:
    /*Before signaling next codelet, check if all the threads in the TP
    * performed the reduction locally and if so, update the unique
    * reduction variable.*/
    /*Reducing for operator '+'*/
    this->inputsTPParent->temp_icut3_darts106_mutex->lock();
    *(this->inputsTPParent->temp_icut3_darts106) += (temp_icut3_LocalReductionVar);
    this->inputsTPParent->temp_icut3_darts106_mutex->unlock();
    /*Reducing for operator '+'*/
    this->inputsTPParent->temp_icut4_darts106_mutex->lock();
    *(this->inputsTPParent->temp_icut4_darts106) += (temp_icut4_LocalReductionVar);
    this->inputsTPParent->temp_icut4_darts106_mutex->unlock();
    /*Reducing for operator '+'*/
    this->inputsTPParent->temp_icut2_darts106_mutex->lock();
    *(this->inputsTPParent->temp_icut2_darts106) += (temp_icut2_LocalReductionVar);
    this->inputsTPParent->temp_icut2_darts106_mutex->unlock();
    /*Reducing for operator '+'*/
    this->inputsTPParent->temp_icut1_darts106_mutex->lock();
    *(this->inputsTPParent->temp_icut1_darts106) += (temp_icut1_LocalReductionVar);
    this->inputsTPParent->temp_icut1_darts106_mutex->unlock();
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling omp for stmt's barrier*/
    myTP->controlTPParent->barrierCodelets106[0].decDep();
}
TP106::TP106(int in_numThreads, int in_mainCodeletID, TP98* in_TPParent, int in_initIteration,
    int in_lastIteration, long long* in_temp_icut1, long long* in_temp_icut2,
    long long* in_temp_icut3, long long* in_temp_icut4, double* in_tol2e_over_schwmax,
    TP106** in_ptrToThisTP)
    : ompTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , fock_darts106(new double**[this->numThreads])
    , temp_icut1_darts106(in_temp_icut1) /*OMP_SHARED - INPUT*/
    , temp_icut2_darts106(in_temp_icut2) /*OMP_SHARED - INPUT*/
    , temp_icut3_darts106(in_temp_icut3) /*OMP_SHARED - INPUT*/
    , temp_icut4_darts106(in_temp_icut4) /*OMP_SHARED - INPUT*/
    , tol2e_over_schwmax_darts106(in_tol2e_over_schwmax) /*OMP_SHARED - INPUT*/
    , i_darts106(new int[this->numThreads]) /*VARIABLE*/
    , initIteration106(in_initIteration)
    , lastIteration106(in_lastIteration)
    , ompLoopSched106(1) /*Static by default with runtime scheduling*/
    , ompLoopChunk106(1) /*Chunk =1 by default with runtime scheduling*/
    , nextIteration106(INT_MAX)
    , loop106alreadySetUp(0)
    , readyCodelets(0)
    , baseNumThreads(this->numThreads / DARTS_CODELETS_MULT)
    , temp_icut3_darts106_mutex(&(this->TPParent->temp_icut3_darts106_mutex))
    , temp_icut4_darts106_mutex(&(this->TPParent->temp_icut4_darts106_mutex))
    , temp_icut2_darts106_mutex(&(this->TPParent->temp_icut2_darts106_mutex))
    , temp_icut1_darts106_mutex(&(this->TPParent->temp_icut1_darts106_mutex))
    , barrierCodelets106(new _barrierCodelets106[1])
    , checkInCodelets107(new _checkInCodelets107[this->numThreads])
{
    /*Initialize the loop parameters*/
    ompLoopSched106 = ompSchedulePolicy;
    ompLoopChunk106 = ompScheduleChunk;
    range106 = abs(lastIteration106 - initIteration106) / 1;
    rangePerCodelet106 = range106 / numThreads;
    minIteration106 = min<int>(lastIteration106, initIteration106);
    remainderRange106 = range106 % numThreads;
    nextIteration106 = initIteration106;
    /*Initialize inputs and vars.*/
    this->fock_darts106
        = (double***)malloc(sizeof(double**) * this->numThreads) /*OMP_SHARED_PRIVATE*/;
    /*Initialize Codelets*/
    barrierCodelets106[0] = _barrierCodelets106(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets107* checkInCodelets107Ptr = (this->checkInCodelets107);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets107);
#endif
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->baseNumThreads;
         codeletCounter++) {
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets107Ptr) = _checkInCodelets107(2, 1, this, codeletCounter);
#else
        (*checkInCodelets107Ptr) = _checkInCodelets107(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets107Ptr).decDep();
        checkInCodelets107Ptr++;
    }
    this->dispatchCodelet(this->getID());
    *(in_ptrToThisTP) = this;
}
void TP106::dispatchCodelet(size_t codeletID)
{
    int idx = codeletID / this->baseNumThreads;
    int localID = codeletID - this->baseNumThreads * idx;
    this->checkInCodelets107[localID].setID(codeletID);
    this->checkInCodelets107[localID].setLocalID(localID);
    /*Check if we want to replicate codelets*/
    for (size_t i = 1; i < (size_t)DARTS_CODELETS_MULT; i++) {
#if USE_SPIN_CODELETS == 0
        this->checkInCodelets107[localID + this->baseNumThreads * i]
            = _checkInCodelets107(2, 1, this, localID + this->baseNumThreads * i);
#else
        this->checkInCodelets107[localID + this->baseNumThreads * i]
            = _checkInCodelets107(1, 1, this, localID + this->baseNumThreads * i);
#endif
        this->checkInCodelets107[localID + this->baseNumThreads * i].setID(codeletID);
        this->checkInCodelets107[localID + this->baseNumThreads * i].decDep();
#if USE_SPIN_CODELETS == 0
        this->firstCodelet[localID + this->baseNumThreads * i].decDep();
#else
        this->availableCodelets[localID + this->baseNumThreads * i] = 1;
#endif
    }
#if USE_SPIN_CODELETS == 0
    this->firstCodelet[localID].decDep();
#else
    this->availableCodelets[localID] = 1;
#endif
}
TP106::~TP106()
{
    delete[] fock_darts106;
    delete[] i_darts106;
    delete[] barrierCodelets106;
    delete[] checkInCodelets107;
}
/*TP276: OMPForDirective*/
void TP276::_barrierCodelets276::fire(void)
{
    TP276* myTP = static_cast<TP276*>(myTP_);
    myTP->controlTPParent->TPParent->barrierCodelets276[0].decDep();
}
bool TP276::requestNewRangeIterations276(int* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Static */
    /*Chunk = 1*/
    bool isThereNewIteration = false;
    {
        /*Static Scheduling*/
        int tempStartRange = rangePerCodelet276 * codeletID;
        int tempEndRange = rangePerCodelet276 * (codeletID + 1);
        if (remainderRange276 != 0) {
            if (codeletID < (uint32_t)remainderRange276) {
                tempStartRange += codeletID;
                tempEndRange += (codeletID + 1);
            } else {
                tempStartRange += remainderRange276;
                tempEndRange += remainderRange276;
            }
        }
        tempStartRange = tempStartRange * 1 + minIteration276;
        tempEndRange = tempEndRange * 1 + minIteration276;
        if (tempStartRange != tempEndRange) {
            isThereNewIteration = true;
        }
        if (initIteration276 < lastIteration276) {
            (this->inputsTPParent->i_darts276[codeletID]) = min(tempStartRange, tempEndRange);
            *endRange = max(tempStartRange, tempEndRange);
        } else {
            (this->inputsTPParent->i_darts276[codeletID]) = max(tempStartRange, tempEndRange);
            *endRange = min(tempStartRange, tempEndRange);
        }
        if (codeletID == this->numThreads - 1) {
            *endRange = lastIteration276;
        }
    }
    return isThereNewIteration;
}
void TP276::_checkInCodelets277::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    /*Init the vars for this region*/
    /*Get pointer from parent for variable
     with shared scope in this region but private
     in the enclosing one.*/
    this->inputsTPParent->fock_2_darts276[this->getLocalID()]
        = (double**)&(myTP->TPParent->inputsTPParent->fock_2_darts98[this->getID()]);
    /*Get pointer from parent for variable
     with shared scope in this region but private
     in the enclosing one.*/
    this->inputsTPParent->nbfn_squared_darts276[this->getLocalID()]
        = (long long*)&(myTP->TPParent->inputsTPParent->nbfn_squared_darts98[this->getID()]);
    /*Get pointer from parent for variable
     with shared scope in this region but private
     in the enclosing one.*/
    this->inputsTPParent->numthreads_darts276[this->getLocalID()]
        = (int*)&(myTP->TPParent->inputsTPParent->numthreads_darts98[this->getID()]);

    /*printing node 277: ForStmt*/
    /*var: fock_2*/
    /*var: nbfn_squared*/
    /*var: numthreads*/
    double*** fock_2 = &(this->inputsTPParent->fock_2_darts276[this->getLocalID()]);
    (void)fock_2 /*OMP_SHARED_PRIVATE*/;
    long long** nbfn_squared = &(this->inputsTPParent->nbfn_squared_darts276[this->getLocalID()]);
    (void)nbfn_squared /*OMP_SHARED_PRIVATE*/;
    int** numthreads = &(this->inputsTPParent->numthreads_darts276[this->getLocalID()]);
    (void)numthreads /*OMP_SHARED_PRIVATE*/;
    int* i = &(this->inputsTPParent->i_darts276[this->getLocalID()]);
    (void)i /*PRIVATE*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations276(
        (int*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        /*Signaling omp for stmt's barrier*/
        myTP->controlTPParent->barrierCodelets276[0].decDep();
        return;
    }
    for (int i_darts_counter_temp276 = (*i); i_darts_counter_temp276 < endRange
         && i_darts_counter_temp276 < this->inputsTPParent->lastIteration276;
         i_darts_counter_temp276++) {
        {
            double* tmp = &g_thread_fock[(i_darts_counter_temp276)];
            double sum = 0.;
            {
                /*Loop's init*/
                int j = 0;
                for (; j < (*(*numthreads)); j++) {
                    sum += tmp[0];
                    tmp[0] = 0;
                    tmp = &tmp[(*(*nbfn_squared))];
                }
            }
            (*(*fock_2))[(i_darts_counter_temp276)] += sum;
        }
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling omp for stmt's barrier*/
    myTP->controlTPParent->barrierCodelets276[0].decDep();
}
TP276::TP276(int in_numThreads, int in_mainCodeletID, TP98* in_TPParent, int in_initIteration,
    int in_lastIteration, TP276** in_ptrToThisTP)
    : ompTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , fock_2_darts276(new double**[this->numThreads])
    , nbfn_squared_darts276(new long long*[this->numThreads])
    , numthreads_darts276(new int*[this->numThreads])
    , i_darts276(new int[this->numThreads]) /*VARIABLE*/
    , initIteration276(in_initIteration)
    , lastIteration276(in_lastIteration)
    , readyCodelets(0)
    , baseNumThreads(this->numThreads / DARTS_CODELETS_MULT)
    , barrierCodelets276(new _barrierCodelets276[1])
    , checkInCodelets277(new _checkInCodelets277[this->numThreads])
{
    /*Initialize the loop parameters*/
    range276 = abs(lastIteration276 - initIteration276) / 1;
    rangePerCodelet276 = range276 / numThreads;
    minIteration276 = min<int>(lastIteration276, initIteration276);
    remainderRange276 = range276 % numThreads;
    /*Initialize inputs and vars.*/
    this->fock_2_darts276
        = (double***)malloc(sizeof(double**) * this->numThreads) /*OMP_SHARED_PRIVATE*/;
    this->nbfn_squared_darts276
        = (long long**)malloc(sizeof(long long*) * this->numThreads) /*OMP_SHARED_PRIVATE*/;
    this->numthreads_darts276
        = (int**)malloc(sizeof(int*) * this->numThreads) /*OMP_SHARED_PRIVATE*/;
    /*Initialize Codelets*/
    barrierCodelets276[0] = _barrierCodelets276(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets277* checkInCodelets277Ptr = (this->checkInCodelets277);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets277);
#endif
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->baseNumThreads;
         codeletCounter++) {
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets277Ptr) = _checkInCodelets277(2, 1, this, codeletCounter);
#else
        (*checkInCodelets277Ptr) = _checkInCodelets277(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets277Ptr).decDep();
        checkInCodelets277Ptr++;
    }
    this->dispatchCodelet(this->getID());
    *(in_ptrToThisTP) = this;
}
void TP276::dispatchCodelet(size_t codeletID)
{
    int idx = codeletID / this->baseNumThreads;
    int localID = codeletID - this->baseNumThreads * idx;
    this->checkInCodelets277[localID].setID(codeletID);
    this->checkInCodelets277[localID].setLocalID(localID);
    /*Check if we want to replicate codelets*/
    for (size_t i = 1; i < (size_t)DARTS_CODELETS_MULT; i++) {
#if USE_SPIN_CODELETS == 0
        this->checkInCodelets277[localID + this->baseNumThreads * i]
            = _checkInCodelets277(2, 1, this, localID + this->baseNumThreads * i);
#else
        this->checkInCodelets277[localID + this->baseNumThreads * i]
            = _checkInCodelets277(1, 1, this, localID + this->baseNumThreads * i);
#endif
        this->checkInCodelets277[localID + this->baseNumThreads * i].setID(codeletID);
        this->checkInCodelets277[localID + this->baseNumThreads * i].decDep();
#if USE_SPIN_CODELETS == 0
        this->firstCodelet[localID + this->baseNumThreads * i].decDep();
#else
        this->availableCodelets[localID + this->baseNumThreads * i] = 1;
#endif
    }
#if USE_SPIN_CODELETS == 0
    this->firstCodelet[localID].decDep();
#else
    this->availableCodelets[localID] = 1;
#endif
}
TP276::~TP276()
{
    delete[] fock_2_darts276;
    delete[] nbfn_squared_darts276;
    delete[] numthreads_darts276;
    delete[] i_darts276;
    delete[] barrierCodelets276;
    delete[] checkInCodelets277;
}
/*TP325: OMPForDirective*/
void TP325::_barrierCodelets325::fire(void)
{
    TP325* myTP = static_cast<TP325*>(myTP_);
    myTP->controlTPParent->TPParent->barrierCodelets325[0].decDep();
}
bool TP325::requestNewRangeIterations325(int* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Runtime */
    /*Chunk = 1*/
    bool isThereNewIteration = false;
    if (ompLoopSched325 == 1) {
        /*Static Scheduling*/
        int tempStartRange = rangePerCodelet325 * codeletID;
        int tempEndRange = rangePerCodelet325 * (codeletID + 1);
        if (remainderRange325 != 0) {
            if (codeletID < (uint32_t)remainderRange325) {
                tempStartRange += codeletID;
                tempEndRange += (codeletID + 1);
            } else {
                tempStartRange += remainderRange325;
                tempEndRange += remainderRange325;
            }
        }
        tempStartRange = tempStartRange * 1 + minIteration325;
        tempEndRange = tempEndRange * 1 + minIteration325;
        if (tempStartRange != tempEndRange) {
            isThereNewIteration = true;
        }
        if (initIteration325 < lastIteration325) {
            (this->inputsTPParent->i_darts325[codeletID]) = min(tempStartRange, tempEndRange);
            *endRange = max(tempStartRange, tempEndRange);
        } else {
            (this->inputsTPParent->i_darts325[codeletID]) = max(tempStartRange, tempEndRange);
            *endRange = min(tempStartRange, tempEndRange);
        }
        if (codeletID == this->numThreads - 1) {
            *endRange = lastIteration325;
        }
    } else if (ompLoopSched325 == 2) {
        /*Dynamic Scheduling*/
        if (initIteration325 < lastIteration325) {
            (this->inputsTPParent->i_darts325[codeletID])
                = __sync_fetch_and_add(&(nextIteration325), ompLoopChunk325 * 1);
            *endRange = (this->inputsTPParent->i_darts325[codeletID]) + ompLoopChunk325 * 1;
        } else {
            (this->inputsTPParent->i_darts325[codeletID])
                = __sync_fetch_and_sub(&(nextIteration325), ompLoopChunk325 * 1);
            *endRange = (this->inputsTPParent->i_darts325[codeletID]) - ompLoopChunk325 * 1;
        }
        if ((this->inputsTPParent->i_darts325[codeletID]) < lastIteration325) {
            isThereNewIteration = true;
        }
    }
    return isThereNewIteration;
}
void TP325::_checkInCodelets326::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    /*Init the vars for this region*/

    /*printing node 326: ForStmt*/
    int* i = &(this->inputsTPParent->i_darts325[this->getLocalID()]);
    (void)i /*PRIVATE*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations325(
        (int*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        /*Signaling omp for stmt's barrier*/
        myTP->controlTPParent->barrierCodelets325[0].decDep();
        return;
    }
    while (isThereNewIteration) {
        for (int i_darts_counter_temp325 = (*i); i_darts_counter_temp325 < endRange
             && i_darts_counter_temp325 < this->inputsTPParent->lastIteration325;
             i_darts_counter_temp325++) {
            {
                int i_off = (i_darts_counter_temp325)*nbfn;
                double fock_val = g_partial_fock[i_off + (i_darts_counter_temp325)];
                g_fock[i_off + (i_darts_counter_temp325)] += fock_val + fock_val;
                g_partial_fock[i_off + (i_darts_counter_temp325)] = 0.;
                int j_off = i_off + nbfn;
                {
                    /*Loop's init*/
                    int j = (i_darts_counter_temp325) + 1;
                    for (; j < nbfn; j++) {
                        fock_val = g_partial_fock[i_off + j]
                            + g_partial_fock[j_off + (i_darts_counter_temp325)];
                        g_fock[i_off + j] += fock_val;
                        g_fock[j_off + (i_darts_counter_temp325)] += fock_val;
                        g_partial_fock[i_off + j] = 0.;
                        g_partial_fock[j_off + (i_darts_counter_temp325)] = 0.;
                        j_off += nbfn;
                    }
                }
            }
        }
        /*dont ask for more iterations if the scheduling selected at runtime is static*/
        if (this->inputsTPParent->ompLoopSched325 == 1)
            break;
        isThereNewIteration = this->inputsTPParent->requestNewRangeIterations325(
            &(this->endRange), this->getLocalID());
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling omp for stmt's barrier*/
    myTP->controlTPParent->barrierCodelets325[0].decDep();
}
TP325::TP325(int in_numThreads, int in_mainCodeletID, TP98* in_TPParent, int in_initIteration,
    int in_lastIteration, TP325** in_ptrToThisTP)
    : ompTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , i_darts325(new int[this->numThreads]) /*VARIABLE*/
    , initIteration325(in_initIteration)
    , lastIteration325(in_lastIteration)
    , ompLoopSched325(1) /*Static by default with runtime scheduling*/
    , ompLoopChunk325(1) /*Chunk =1 by default with runtime scheduling*/
    , nextIteration325(INT_MAX)
    , loop325alreadySetUp(0)
    , readyCodelets(0)
    , baseNumThreads(this->numThreads / DARTS_CODELETS_MULT)
    , barrierCodelets325(new _barrierCodelets325[1])
    , checkInCodelets326(new _checkInCodelets326[this->numThreads])
{
    /*Initialize the loop parameters*/
    ompLoopSched325 = ompSchedulePolicy;
    ompLoopChunk325 = ompScheduleChunk;
    range325 = abs(lastIteration325 - initIteration325) / 1;
    rangePerCodelet325 = range325 / numThreads;
    minIteration325 = min<int>(lastIteration325, initIteration325);
    remainderRange325 = range325 % numThreads;
    nextIteration325 = initIteration325;
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets325[0] = _barrierCodelets325(this->numThreads, this->numThreads, this, 0);
    _checkInCodelets326* checkInCodelets326Ptr = (this->checkInCodelets326);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets326);
#endif
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->baseNumThreads;
         codeletCounter++) {
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets326Ptr) = _checkInCodelets326(2, 1, this, codeletCounter);
#else
        (*checkInCodelets326Ptr) = _checkInCodelets326(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets326Ptr).decDep();
        checkInCodelets326Ptr++;
    }
    this->dispatchCodelet(this->getID());
    *(in_ptrToThisTP) = this;
}
void TP325::dispatchCodelet(size_t codeletID)
{
    int idx = codeletID / this->baseNumThreads;
    int localID = codeletID - this->baseNumThreads * idx;
    this->checkInCodelets326[localID].setID(codeletID);
    this->checkInCodelets326[localID].setLocalID(localID);
    /*Check if we want to replicate codelets*/
    for (size_t i = 1; i < (size_t)DARTS_CODELETS_MULT; i++) {
#if USE_SPIN_CODELETS == 0
        this->checkInCodelets326[localID + this->baseNumThreads * i]
            = _checkInCodelets326(2, 1, this, localID + this->baseNumThreads * i);
#else
        this->checkInCodelets326[localID + this->baseNumThreads * i]
            = _checkInCodelets326(1, 1, this, localID + this->baseNumThreads * i);
#endif
        this->checkInCodelets326[localID + this->baseNumThreads * i].setID(codeletID);
        this->checkInCodelets326[localID + this->baseNumThreads * i].decDep();
#if USE_SPIN_CODELETS == 0
        this->firstCodelet[localID + this->baseNumThreads * i].decDep();
#else
        this->availableCodelets[localID + this->baseNumThreads * i] = 1;
#endif
    }
#if USE_SPIN_CODELETS == 0
    this->firstCodelet[localID].decDep();
#else
    this->availableCodelets[localID] = 1;
#endif
}
TP325::~TP325()
{
    delete[] i_darts325;
    delete[] barrierCodelets325;
    delete[] checkInCodelets326;
}
/*TP406: OMPParallelDirective*/
void TP406::_barrierCodelets406::fire(void)
{
    TP406* myTP = static_cast<TP406*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
void TP406::_checkInCodelets408::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 408: DeclStmt*/
    this->inputsTPParent->fock_darts406[this->getID()]
        = &g_thread_fock[omp_get_thread_num() * nbfn * nbfn];
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 408 nextRegion: 414 */
    myTP->controlTPParent->checkInCodelets414[this->getID()].decDep();
}
void TP406::_checkInCodelets414::fire(void)
{
    /*printOMPForCodeletFireCode, inlined: 0*/
    /*region 414 0*/
    /*Determine the TP to which this codelet belongs and check if this codelet spawns the TP or if
     * it signals it using dispatchCodelet()*/
    size_t idx = this->getID() / myTP->codeletsPerTP414;
    if (idx < myTP->TPsToUse414) {
        if (!__sync_val_compare_and_swap(&(myTP->TP414_alreadyLaunched[idx]), 0, 1)) {
            int range = abs(nbfn - 0) / 1;
            int rangePerCodelet = range / myTP->TPsToUse414;
            int minIteration = min<int>(nbfn, 0);
            int remainderRange = range % myTP->TPsToUse414;
            int initIteration = rangePerCodelet * idx;
            int lastIteration = rangePerCodelet * (idx + 1);
            if (remainderRange != 0) {
                if (idx < (uint32_t)remainderRange) {
                    initIteration += idx;
                    lastIteration += (idx + 1);
                } else {
                    initIteration += remainderRange;
                    lastIteration += remainderRange;
                }
            }
            initIteration = initIteration * 1 + minIteration;
            lastIteration = lastIteration * 1 + minIteration;
            if (0 < nbfn) {
                initIteration = min(initIteration, lastIteration);
                lastIteration = max(initIteration, lastIteration);
            } else {
                initIteration = max(initIteration, lastIteration);
                lastIteration = min(initIteration, lastIteration);
            }
            if (idx == myTP->TPsToUse414 - 1) {
                lastIteration = nbfn;
            }
#if USEINVOKE == 1
            invoke<TP414>(myTP, myTP->codeletsPerTP414 * DARTS_CODELETS_MULT, this->getID(), myTP,
                initIteration, lastIteration, &(*(this->inputsTPParent->temp_icut1_darts406)),
                &(*(this->inputsTPParent->temp_icut2_darts406)),
                &(*(this->inputsTPParent->temp_icut3_darts406)),
                &(*(this->inputsTPParent->temp_icut4_darts406)),
                &(*(this->inputsTPParent->tol2e_over_schwmax_darts406)), &(myTP->TP414Ptr[idx]));
#else
            place<TP414>(idx, myTP, myTP->codeletsPerTP414 * DARTS_CODELETS_MULT, this->getID(),
                myTP, initIteration, lastIteration, &(*(this->inputsTPParent->temp_icut1_darts406)),
                &(*(this->inputsTPParent->temp_icut2_darts406)),
                &(*(this->inputsTPParent->temp_icut3_darts406)),
                &(*(this->inputsTPParent->temp_icut4_darts406)),
                &(*(this->inputsTPParent->tol2e_over_schwmax_darts406)), &(myTP->TP414Ptr[idx]));
#endif
        } else {
            if (myTP->TP414Ptr[idx] != nullptr) {
                myTP->TP414Ptr[idx]->dispatchCodelet(this->getID());
            } else {
                this->resetCodelet();
                this->decDep();
            }
        }
    } else {
        /*Find and signal the next codelet*/
        myTP->controlTPParent->TPParent->barrierCodelets406[0].decDep();
    }
}
TP406::TP406(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet,
    long long* in_temp_icut1, long long* in_temp_icut2, long long* in_temp_icut3,
    long long* in_temp_icut4, double* in_tol2e_over_schwmax)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , temp_icut1_darts406(in_temp_icut1) /*OMP_SHARED - INPUT*/
    , temp_icut2_darts406(in_temp_icut2) /*OMP_SHARED - INPUT*/
    , temp_icut3_darts406(in_temp_icut3) /*OMP_SHARED - INPUT*/
    , temp_icut4_darts406(in_temp_icut4) /*OMP_SHARED - INPUT*/
    , tol2e_over_schwmax_darts406(in_tol2e_over_schwmax) /*OMP_SHARED - INPUT*/
    , fock_darts406(new double*[this->numThreads]) /*VARIABLE*/
    , TP414Ptr(new TP414*[NUMTPS414])
    , TP414_alreadyLaunched(new size_t[NUMTPS414])
    , numTPsSet414(0)
    , numTPsReady414(0)
    , TPsToUse414(NUMTPS414)
    , codeletsPerTP414(this->numThreads / NUMTPS414)
    , totalCodelets414(this->TPsToUse414 * this->codeletsPerTP414)
    , barrierCodelets406(new _barrierCodelets406[1])
    , checkInCodelets408(new _checkInCodelets408[this->numThreads])
    , checkInCodelets414(new _checkInCodelets414[this->numThreads])
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets406[0] = _barrierCodelets406(ompNumThreads, ompNumThreads, this, 0);
    _checkInCodelets414* checkInCodelets414Ptr = (this->checkInCodelets414);
    for (int i = 0; i < NUMTPS414; i++) {
        TP414Ptr[i] = nullptr;
        TP414_alreadyLaunched[i] = 0;
    }
    _checkInCodelets408* checkInCodelets408Ptr = (this->checkInCodelets408);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets414Ptr) = _checkInCodelets414(1, 1, this, codeletCounter);
        checkInCodelets414Ptr++;
        (*checkInCodelets408Ptr) = _checkInCodelets408(1, 1, this, codeletCounter);
        (*checkInCodelets408Ptr).decDep();
        checkInCodelets408Ptr++;
    }
}
TP406::~TP406()
{
    delete[] fock_darts406;
    delete[] barrierCodelets406;
    delete[] checkInCodelets414;
    delete[] checkInCodelets408;
}
/*TP414: OMPForDirective*/
bool TP414::requestNewRangeIterations414(int* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Runtime */
    /*Chunk = 1*/
    bool isThereNewIteration = false;
    if (ompLoopSched414 == 1) {
        /*Static Scheduling*/
        int tempStartRange = rangePerCodelet414 * codeletID;
        int tempEndRange = rangePerCodelet414 * (codeletID + 1);
        if (remainderRange414 != 0) {
            if (codeletID < (uint32_t)remainderRange414) {
                tempStartRange += codeletID;
                tempEndRange += (codeletID + 1);
            } else {
                tempStartRange += remainderRange414;
                tempEndRange += remainderRange414;
            }
        }
        tempStartRange = tempStartRange * 1 + minIteration414;
        tempEndRange = tempEndRange * 1 + minIteration414;
        if (tempStartRange != tempEndRange) {
            isThereNewIteration = true;
        }
        if (initIteration414 < lastIteration414) {
            (this->inputsTPParent->i_darts414[codeletID]) = min(tempStartRange, tempEndRange);
            *endRange = max(tempStartRange, tempEndRange);
        } else {
            (this->inputsTPParent->i_darts414[codeletID]) = max(tempStartRange, tempEndRange);
            *endRange = min(tempStartRange, tempEndRange);
        }
        if (codeletID == this->numThreads - 1) {
            *endRange = lastIteration414;
        }
    } else if (ompLoopSched414 == 2) {
        /*Dynamic Scheduling*/
        if (initIteration414 < lastIteration414) {
            (this->inputsTPParent->i_darts414[codeletID])
                = __sync_fetch_and_add(&(nextIteration414), ompLoopChunk414 * 1);
            *endRange = (this->inputsTPParent->i_darts414[codeletID]) + ompLoopChunk414 * 1;
        } else {
            (this->inputsTPParent->i_darts414[codeletID])
                = __sync_fetch_and_sub(&(nextIteration414), ompLoopChunk414 * 1);
            *endRange = (this->inputsTPParent->i_darts414[codeletID]) - ompLoopChunk414 * 1;
        }
        if ((this->inputsTPParent->i_darts414[codeletID]) < lastIteration414) {
            isThereNewIteration = true;
        }
    }
    return isThereNewIteration;
}
void TP414::_checkInCodelets415::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    /*Init the vars for this region*/
    /*Get pointer from parent for variable
     with shared scope in this region but private
     in the enclosing one.*/
    this->inputsTPParent->fock_darts414[this->getLocalID()]
        = (double**)&(myTP->TPParent->inputsTPParent->fock_darts406[this->getID()]);

    /*printing node 415: ForStmt*/
    /*var: fock*/
    /*var: temp_icut1*/
    /*var: temp_icut2*/
    /*var: temp_icut3*/
    /*var: temp_icut4*/
    /*var: tol2e_over_schwmax*/
    /*Declare local reduction vars*/
    long long temp_icut3_LocalReductionVar = (long long)0;
    long long temp_icut4_LocalReductionVar = (long long)0;
    long long temp_icut2_LocalReductionVar = (long long)0;
    long long temp_icut1_LocalReductionVar = (long long)0;
    double*** fock = &(this->inputsTPParent->fock_darts414[this->getLocalID()]);
    (void)fock /*OMP_SHARED_PRIVATE*/;
    double* tol2e_over_schwmax = (this->inputsTPParent->tol2e_over_schwmax_darts414);
    (void)tol2e_over_schwmax /*OMP_SHARED*/;
    int* i = &(this->inputsTPParent->i_darts414[this->getLocalID()]);
    (void)i /*PRIVATE*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations414(
        (int*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        goto updateReduceVars;
    }
    while (isThereNewIteration) {
        for (int i_darts_counter_temp414 = (*i); i_darts_counter_temp414 < endRange
             && i_darts_counter_temp414 < this->inputsTPParent->lastIteration414;
             i_darts_counter_temp414++) {
            {
                int i_off = (i_darts_counter_temp414)*nbfn;
                double g_dens_i_i = g_dens[((i_off) + ((i_darts_counter_temp414)))];
                double tol2e_over_g_schwarz_i_i
                    = 9.9999999999999995E-7 / g_schwarz[((i_off) + ((i_darts_counter_temp414)))];
                if (g_schwarz[((i_off) + ((i_darts_counter_temp414)))] < (*(tol2e_over_schwmax))) {
                    (*(&(temp_icut1_LocalReductionVar))) += 4 * (1 + (i_darts_counter_temp414)-nbfn)
                        * ((i_darts_counter_temp414)-nbfn) / 2;
                    continue;
                }
                int k_off = i_off - nbfn;
                {
                    /*Loop's init*/
                    int k = (i_darts_counter_temp414);
                    for (; k < nbfn; k++) {
                        k_off += nbfn;
                        double g_dens_k_i = g_dens[((k_off) + ((i_darts_counter_temp414)))];
                        double g_dens_i_k = g_dens_k_i;
                        if (g_schwarz_max_j[k] < tol2e_over_g_schwarz_i_i) {
                            (*(&(temp_icut4_LocalReductionVar))) += 4 * (nbfn - k - 1);
                            continue;
                        }
                        int l_off = k_off;
                        {
                            /*Loop's init*/
                            int l = 1 + k;
                            for (; l < nbfn; l++) {
                                l_off += nbfn;
                                if (g_schwarz[((k_off) + (l))] < tol2e_over_g_schwarz_i_i) {
                                    (*(&(temp_icut2_LocalReductionVar))) += 4;
                                    continue;
                                }
                                double g_dens_l_i = g_dens[((l_off) + ((i_darts_counter_temp414)))];
                                double g_dens_i_l = g_dens_l_i;
                                double g_dens_l_k = g_dens[((l_off) + (k))];
                                double g_dens_k_l = g_dens_l_k;
                                (*(&(temp_icut3_LocalReductionVar))) += 4;
                                double gg = g_fast(
                                    ((i_off) + ((i_darts_counter_temp414))), ((k_off) + (l)));
                                (*(*fock))[((i_off) + ((i_darts_counter_temp414)))]
                                    += (gg * (g_dens_k_l));
                                (*(*fock))[((k_off) + (l))] += (gg * (g_dens_i_i));
                                (*(*fock))[((i_off) + (k))] -= (0.5 * gg * (g_dens_i_l));
                                (*(*fock))[((i_off) + (l))] -= (0.5 * gg * (g_dens_i_k));
                            }
                        }
                    }
                }
            }
        }
        /*dont ask for more iterations if the scheduling selected at runtime is static*/
        if (this->inputsTPParent->ompLoopSched414 == 1)
            break;
        isThereNewIteration = this->inputsTPParent->requestNewRangeIterations414(
            &(this->endRange), this->getLocalID());
    }
updateReduceVars:
    /*Before signaling next codelet, check if all the threads in the TP
    * performed the reduction locally and if so, update the unique
    * reduction variable.*/
    /*Reducing for operator '+'*/
    this->inputsTPParent->temp_icut3_darts414_mutex->lock();
    *(this->inputsTPParent->temp_icut3_darts414) += (temp_icut3_LocalReductionVar);
    this->inputsTPParent->temp_icut3_darts414_mutex->unlock();
    /*Reducing for operator '+'*/
    this->inputsTPParent->temp_icut4_darts414_mutex->lock();
    *(this->inputsTPParent->temp_icut4_darts414) += (temp_icut4_LocalReductionVar);
    this->inputsTPParent->temp_icut4_darts414_mutex->unlock();
    /*Reducing for operator '+'*/
    this->inputsTPParent->temp_icut2_darts414_mutex->lock();
    *(this->inputsTPParent->temp_icut2_darts414) += (temp_icut2_LocalReductionVar);
    this->inputsTPParent->temp_icut2_darts414_mutex->unlock();
    /*Reducing for operator '+'*/
    this->inputsTPParent->temp_icut1_darts414_mutex->lock();
    *(this->inputsTPParent->temp_icut1_darts414) += (temp_icut1_LocalReductionVar);
    this->inputsTPParent->temp_icut1_darts414_mutex->unlock();
    /*If this omp for has no barrier,
    check if all the codelets
    replicated from the same
    global ID has finished and
    signal the next codelet.
    Otherwise, return.*/
    uint32_t completedMultCodelet = __sync_fetch_and_add(
        &(myTP->signalNextReady[this->getLocalID() % myTP->baseNumThreads]), 1);
    if (completedMultCodelet < (uint32_t)(DARTS_CODELETS_MULT - 1))
        return;
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->TPParent->barrierCodelets406[0].decDep();
}
TP414::TP414(int in_numThreads, int in_mainCodeletID, TP406* in_TPParent, int in_initIteration,
    int in_lastIteration, long long* in_temp_icut1, long long* in_temp_icut2,
    long long* in_temp_icut3, long long* in_temp_icut4, double* in_tol2e_over_schwmax,
    TP414** in_ptrToThisTP)
    : ompTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , fock_darts414(new double**[this->numThreads])
    , temp_icut1_darts414(in_temp_icut1) /*OMP_SHARED - INPUT*/
    , temp_icut2_darts414(in_temp_icut2) /*OMP_SHARED - INPUT*/
    , temp_icut3_darts414(in_temp_icut3) /*OMP_SHARED - INPUT*/
    , temp_icut4_darts414(in_temp_icut4) /*OMP_SHARED - INPUT*/
    , tol2e_over_schwmax_darts414(in_tol2e_over_schwmax) /*OMP_SHARED - INPUT*/
    , i_darts414(new int[this->numThreads]) /*VARIABLE*/
    , initIteration414(in_initIteration)
    , lastIteration414(in_lastIteration)
    , ompLoopSched414(1) /*Static by default with runtime scheduling*/
    , ompLoopChunk414(1) /*Chunk =1 by default with runtime scheduling*/
    , nextIteration414(INT_MAX)
    , loop414alreadySetUp(0)
    , readyCodelets(0)
    , baseNumThreads(this->numThreads / DARTS_CODELETS_MULT)
    , signalNextReady(new int[baseNumThreads])
    , temp_icut3_darts414_mutex(&(this->TPParent->temp_icut3_darts414_mutex))
    , temp_icut4_darts414_mutex(&(this->TPParent->temp_icut4_darts414_mutex))
    , temp_icut2_darts414_mutex(&(this->TPParent->temp_icut2_darts414_mutex))
    , temp_icut1_darts414_mutex(&(this->TPParent->temp_icut1_darts414_mutex))
    , checkInCodelets415(new _checkInCodelets415[this->numThreads])
{
    /*Initialize the loop parameters*/
    ompLoopSched414 = ompSchedulePolicy;
    ompLoopChunk414 = ompScheduleChunk;
    range414 = abs(lastIteration414 - initIteration414) / 1;
    rangePerCodelet414 = range414 / numThreads;
    minIteration414 = min<int>(lastIteration414, initIteration414);
    remainderRange414 = range414 % numThreads;
    nextIteration414 = initIteration414;
    /*Initialize inputs and vars.*/
    this->fock_darts414
        = (double***)malloc(sizeof(double**) * this->numThreads) /*OMP_SHARED_PRIVATE*/;
    /*Initialize Codelets*/
    _checkInCodelets415* checkInCodelets415Ptr = (this->checkInCodelets415);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets415);
#endif
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->baseNumThreads;
         codeletCounter++) {
        this->signalNextReady[codeletCounter] = 0;
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets415Ptr) = _checkInCodelets415(2, 1, this, codeletCounter);
#else
        (*checkInCodelets415Ptr) = _checkInCodelets415(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets415Ptr).decDep();
        checkInCodelets415Ptr++;
    }
    this->dispatchCodelet(this->getID());
    *(in_ptrToThisTP) = this;
}
void TP414::dispatchCodelet(size_t codeletID)
{
    int idx = codeletID / this->baseNumThreads;
    int localID = codeletID - this->baseNumThreads * idx;
    this->checkInCodelets415[localID].setID(codeletID);
    this->checkInCodelets415[localID].setLocalID(localID);
    /*Check if we want to replicate codelets*/
    for (size_t i = 1; i < (size_t)DARTS_CODELETS_MULT; i++) {
#if USE_SPIN_CODELETS == 0
        this->checkInCodelets415[localID + this->baseNumThreads * i]
            = _checkInCodelets415(2, 1, this, localID + this->baseNumThreads * i);
#else
        this->checkInCodelets415[localID + this->baseNumThreads * i]
            = _checkInCodelets415(1, 1, this, localID + this->baseNumThreads * i);
#endif
        this->checkInCodelets415[localID + this->baseNumThreads * i].setID(codeletID);
        this->checkInCodelets415[localID + this->baseNumThreads * i].decDep();
#if USE_SPIN_CODELETS == 0
        this->firstCodelet[localID + this->baseNumThreads * i].decDep();
#else
        this->availableCodelets[localID + this->baseNumThreads * i] = 1;
#endif
    }
#if USE_SPIN_CODELETS == 0
    this->firstCodelet[localID].decDep();
#else
    this->availableCodelets[localID] = 1;
#endif
}
TP414::~TP414()
{
    delete[] fock_darts414;
    delete[] i_darts414;
    delete[] checkInCodelets415;
}
/*TP540: OMPParallelDirective*/
void TP540::_barrierCodelets540::fire(void)
{
    TP540* myTP = static_cast<TP540*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
void TP540::_checkInCodelets542::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 542: DeclStmt*/
    this->inputsTPParent->fock_darts540[this->getID()]
        = &g_thread_fock[omp_get_thread_num() * nbfn * nbfn];
    /*Signaling next codelet from last stmt in the codelet*/
    /*Signaling next codelet region: 542 nextRegion: 548 */
    myTP->controlTPParent->checkInCodelets548[this->getID()].decDep();
}
void TP540::_checkInCodelets548::fire(void)
{
    /*printOMPForCodeletFireCode, inlined: 0*/
    /*region 548 0*/
    /*Determine the TP to which this codelet belongs and check if this codelet spawns the TP or if
     * it signals it using dispatchCodelet()*/
    size_t idx = this->getID() / myTP->codeletsPerTP548;
    if (idx < myTP->TPsToUse548) {
        if (!__sync_val_compare_and_swap(&(myTP->TP548_alreadyLaunched[idx]), 0, 1)) {
            int range = abs(nbfn - 0) / 1;
            int rangePerCodelet = range / myTP->TPsToUse548;
            int minIteration = min<int>(nbfn, 0);
            int remainderRange = range % myTP->TPsToUse548;
            int initIteration = rangePerCodelet * idx;
            int lastIteration = rangePerCodelet * (idx + 1);
            if (remainderRange != 0) {
                if (idx < (uint32_t)remainderRange) {
                    initIteration += idx;
                    lastIteration += (idx + 1);
                } else {
                    initIteration += remainderRange;
                    lastIteration += remainderRange;
                }
            }
            initIteration = initIteration * 1 + minIteration;
            lastIteration = lastIteration * 1 + minIteration;
            if (0 < nbfn) {
                initIteration = min(initIteration, lastIteration);
                lastIteration = max(initIteration, lastIteration);
            } else {
                initIteration = max(initIteration, lastIteration);
                lastIteration = min(initIteration, lastIteration);
            }
            if (idx == myTP->TPsToUse548 - 1) {
                lastIteration = nbfn;
            }
#if USEINVOKE == 1
            invoke<TP548>(myTP, myTP->codeletsPerTP548 * DARTS_CODELETS_MULT, this->getID(), myTP,
                initIteration, lastIteration, &(*(this->inputsTPParent->temp_icut1_darts540)),
                &(*(this->inputsTPParent->temp_icut2_darts540)),
                &(*(this->inputsTPParent->temp_icut3_darts540)),
                &(*(this->inputsTPParent->temp_icut4_darts540)),
                &(*(this->inputsTPParent->tol2e_over_schwmax_darts540)), &(myTP->TP548Ptr[idx]));
#else
            place<TP548>(idx, myTP, myTP->codeletsPerTP548 * DARTS_CODELETS_MULT, this->getID(),
                myTP, initIteration, lastIteration, &(*(this->inputsTPParent->temp_icut1_darts540)),
                &(*(this->inputsTPParent->temp_icut2_darts540)),
                &(*(this->inputsTPParent->temp_icut3_darts540)),
                &(*(this->inputsTPParent->temp_icut4_darts540)),
                &(*(this->inputsTPParent->tol2e_over_schwmax_darts540)), &(myTP->TP548Ptr[idx]));
#endif
        } else {
            if (myTP->TP548Ptr[idx] != nullptr) {
                myTP->TP548Ptr[idx]->dispatchCodelet(this->getID());
            } else {
                this->resetCodelet();
                this->decDep();
            }
        }
    } else {
        /*Find and signal the next codelet*/
        myTP->controlTPParent->TPParent->barrierCodelets540[0].decDep();
    }
}
TP540::TP540(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet,
    long long* in_temp_icut1, long long* in_temp_icut2, long long* in_temp_icut3,
    long long* in_temp_icut4, double* in_tol2e_over_schwmax)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , temp_icut1_darts540(in_temp_icut1) /*OMP_SHARED - INPUT*/
    , temp_icut2_darts540(in_temp_icut2) /*OMP_SHARED - INPUT*/
    , temp_icut3_darts540(in_temp_icut3) /*OMP_SHARED - INPUT*/
    , temp_icut4_darts540(in_temp_icut4) /*OMP_SHARED - INPUT*/
    , tol2e_over_schwmax_darts540(in_tol2e_over_schwmax) /*OMP_SHARED - INPUT*/
    , fock_darts540(new double*[this->numThreads]) /*VARIABLE*/
    , TP548Ptr(new TP548*[NUMTPS548])
    , TP548_alreadyLaunched(new size_t[NUMTPS548])
    , numTPsSet548(0)
    , numTPsReady548(0)
    , TPsToUse548(NUMTPS548)
    , codeletsPerTP548(this->numThreads / NUMTPS548)
    , totalCodelets548(this->TPsToUse548 * this->codeletsPerTP548)
    , barrierCodelets540(new _barrierCodelets540[1])
    , checkInCodelets542(new _checkInCodelets542[this->numThreads])
    , checkInCodelets548(new _checkInCodelets548[this->numThreads])
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets540[0] = _barrierCodelets540(ompNumThreads, ompNumThreads, this, 0);
    _checkInCodelets548* checkInCodelets548Ptr = (this->checkInCodelets548);
    for (int i = 0; i < NUMTPS548; i++) {
        TP548Ptr[i] = nullptr;
        TP548_alreadyLaunched[i] = 0;
    }
    _checkInCodelets542* checkInCodelets542Ptr = (this->checkInCodelets542);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets548Ptr) = _checkInCodelets548(1, 1, this, codeletCounter);
        checkInCodelets548Ptr++;
        (*checkInCodelets542Ptr) = _checkInCodelets542(1, 1, this, codeletCounter);
        (*checkInCodelets542Ptr).decDep();
        checkInCodelets542Ptr++;
    }
}
TP540::~TP540()
{
    delete[] fock_darts540;
    delete[] barrierCodelets540;
    delete[] checkInCodelets548;
    delete[] checkInCodelets542;
}
/*TP548: OMPForDirective*/
bool TP548::requestNewRangeIterations548(int* endRange, uint32_t codeletID)
{
    /*Scheduling Policy = Runtime */
    /*Chunk = 1*/
    bool isThereNewIteration = false;
    if (ompLoopSched548 == 1) {
        /*Static Scheduling*/
        int tempStartRange = rangePerCodelet548 * codeletID;
        int tempEndRange = rangePerCodelet548 * (codeletID + 1);
        if (remainderRange548 != 0) {
            if (codeletID < (uint32_t)remainderRange548) {
                tempStartRange += codeletID;
                tempEndRange += (codeletID + 1);
            } else {
                tempStartRange += remainderRange548;
                tempEndRange += remainderRange548;
            }
        }
        tempStartRange = tempStartRange * 1 + minIteration548;
        tempEndRange = tempEndRange * 1 + minIteration548;
        if (tempStartRange != tempEndRange) {
            isThereNewIteration = true;
        }
        if (initIteration548 < lastIteration548) {
            (this->inputsTPParent->i_darts548[codeletID]) = min(tempStartRange, tempEndRange);
            *endRange = max(tempStartRange, tempEndRange);
        } else {
            (this->inputsTPParent->i_darts548[codeletID]) = max(tempStartRange, tempEndRange);
            *endRange = min(tempStartRange, tempEndRange);
        }
        if (codeletID == this->numThreads - 1) {
            *endRange = lastIteration548;
        }
    } else if (ompLoopSched548 == 2) {
        /*Dynamic Scheduling*/
        if (initIteration548 < lastIteration548) {
            (this->inputsTPParent->i_darts548[codeletID])
                = __sync_fetch_and_add(&(nextIteration548), ompLoopChunk548 * 1);
            *endRange = (this->inputsTPParent->i_darts548[codeletID]) + ompLoopChunk548 * 1;
        } else {
            (this->inputsTPParent->i_darts548[codeletID])
                = __sync_fetch_and_sub(&(nextIteration548), ompLoopChunk548 * 1);
            *endRange = (this->inputsTPParent->i_darts548[codeletID]) - ompLoopChunk548 * 1;
        }
        if ((this->inputsTPParent->i_darts548[codeletID]) < lastIteration548) {
            isThereNewIteration = true;
        }
    }
    return isThereNewIteration;
}
void TP548::_checkInCodelets549::fire(void)
{
#if USE_SPIN_CODELETS == 1
    /*Wait until the codelet with the same ID finishes in the previous TP*/
    if (myTP->availableCodelets[this->getLocalID()] == 0) {
        myTP->add(this);
        return;
    }
#endif
    /*Init the vars for this region*/
    /*Get pointer from parent for variable
     with shared scope in this region but private
     in the enclosing one.*/
    this->inputsTPParent->fock_darts548[this->getLocalID()]
        = (double**)&(myTP->TPParent->inputsTPParent->fock_darts540[this->getID()]);

    /*printing node 549: ForStmt*/
    /*var: fock*/
    /*var: temp_icut1*/
    /*var: temp_icut2*/
    /*var: temp_icut3*/
    /*var: temp_icut4*/
    /*var: tol2e_over_schwmax*/
    /*Declare local reduction vars*/
    long long temp_icut3_LocalReductionVar = (long long)0;
    long long temp_icut4_LocalReductionVar = (long long)0;
    long long temp_icut2_LocalReductionVar = (long long)0;
    long long temp_icut1_LocalReductionVar = (long long)0;
    double*** fock = &(this->inputsTPParent->fock_darts548[this->getLocalID()]);
    (void)fock /*OMP_SHARED_PRIVATE*/;
    double* tol2e_over_schwmax = (this->inputsTPParent->tol2e_over_schwmax_darts548);
    (void)tol2e_over_schwmax /*OMP_SHARED*/;
    int* i = &(this->inputsTPParent->i_darts548[this->getLocalID()]);
    (void)i /*PRIVATE*/;
    bool isThereNewIteration = this->inputsTPParent->requestNewRangeIterations548(
        (int*)&(this->endRange), this->getLocalID());
    if (isThereNewIteration == false) {
        goto updateReduceVars;
    }
    while (isThereNewIteration) {
        for (int i_darts_counter_temp548 = (*i); i_darts_counter_temp548 < endRange
             && i_darts_counter_temp548 < this->inputsTPParent->lastIteration548;
             i_darts_counter_temp548++) {
            {
                int i_off = (i_darts_counter_temp548)*nbfn;
                int j_off = i_off;
                {
                    /*Loop's init*/
                    int j = (i_darts_counter_temp548) + 1;
                    for (; j < nbfn; j++) {
                        j_off += nbfn;
                        double g_dens_j_i = g_dens[((j_off) + ((i_darts_counter_temp548)))];
                        double g_dens_i_j = g_dens_j_i;
                        double tol2e_over_g_schwarz_i_j
                            = 9.9999999999999995E-7 / g_schwarz[((i_off) + (j))];
                        if (g_schwarz[((i_off) + (j))] < (*(tol2e_over_schwmax))) {
                            (*(&(temp_icut1_LocalReductionVar)))
                                += 4 * (nbfn - (i_darts_counter_temp548)-1);
                            continue;
                        }
                        int k_off = i_off;
                        {
                            /*Loop's init*/
                            int k = (i_darts_counter_temp548) + 1;
                            for (; k < nbfn; k++) {
                                k_off += nbfn;
                                double g_dens_k_i = g_dens[((k_off) + ((i_darts_counter_temp548)))];
                                double g_dens_i_k = g_dens_k_i;
                                double g_dens_k_j = g_dens[((k_off) + (j))];
                                double g_dens_j_k = g_dens_k_j;
                                double g_dens_k_k = g_dens[((k_off) + (k))];
                                if (g_schwarz[((k_off) + (k))] < tol2e_over_g_schwarz_i_j) {
                                    (*(&(temp_icut2_LocalReductionVar))) += 4;
                                    continue;
                                }
                                (*(&(temp_icut3_LocalReductionVar))) += 4;
                                double gg = g_fast(((i_off) + (j)), ((k_off) + (k)));
                                (*(*fock))[((i_off) + (j))] += (gg * (g_dens_k_k));
                                (*(*fock))[((k_off) + (k))] += (gg * (g_dens_i_j));
                                (*(*fock))[((i_off) + (k))] -= (0.5 * gg * (g_dens_j_k));
                                (*(*fock))[((j_off) + (k))] -= (0.5 * gg * (g_dens_i_k));
                            }
                        }
                    }
                }
            }
        }
        /*dont ask for more iterations if the scheduling selected at runtime is static*/
        if (this->inputsTPParent->ompLoopSched548 == 1)
            break;
        isThereNewIteration = this->inputsTPParent->requestNewRangeIterations548(
            &(this->endRange), this->getLocalID());
    }
updateReduceVars:
    /*Before signaling next codelet, check if all the threads in the TP
    * performed the reduction locally and if so, update the unique
    * reduction variable.*/
    /*Reducing for operator '+'*/
    this->inputsTPParent->temp_icut3_darts548_mutex->lock();
    *(this->inputsTPParent->temp_icut3_darts548) += (temp_icut3_LocalReductionVar);
    this->inputsTPParent->temp_icut3_darts548_mutex->unlock();
    /*Reducing for operator '+'*/
    this->inputsTPParent->temp_icut4_darts548_mutex->lock();
    *(this->inputsTPParent->temp_icut4_darts548) += (temp_icut4_LocalReductionVar);
    this->inputsTPParent->temp_icut4_darts548_mutex->unlock();
    /*Reducing for operator '+'*/
    this->inputsTPParent->temp_icut2_darts548_mutex->lock();
    *(this->inputsTPParent->temp_icut2_darts548) += (temp_icut2_LocalReductionVar);
    this->inputsTPParent->temp_icut2_darts548_mutex->unlock();
    /*Reducing for operator '+'*/
    this->inputsTPParent->temp_icut1_darts548_mutex->lock();
    *(this->inputsTPParent->temp_icut1_darts548) += (temp_icut1_LocalReductionVar);
    this->inputsTPParent->temp_icut1_darts548_mutex->unlock();
    /*If this omp for has no barrier,
    check if all the codelets
    replicated from the same
    global ID has finished and
    signal the next codelet.
    Otherwise, return.*/
    uint32_t completedMultCodelet = __sync_fetch_and_add(
        &(myTP->signalNextReady[this->getLocalID() % myTP->baseNumThreads]), 1);
    if (completedMultCodelet < (uint32_t)(DARTS_CODELETS_MULT - 1))
        return;
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->TPParent->barrierCodelets540[0].decDep();
}
TP548::TP548(int in_numThreads, int in_mainCodeletID, TP540* in_TPParent, int in_initIteration,
    int in_lastIteration, long long* in_temp_icut1, long long* in_temp_icut2,
    long long* in_temp_icut3, long long* in_temp_icut4, double* in_tol2e_over_schwmax,
    TP548** in_ptrToThisTP)
    : ompTP(in_numThreads, in_mainCodeletID)
    , TPParent(in_TPParent)
    , controlTPParent(this)
    , inputsTPParent(this)
    , fock_darts548(new double**[this->numThreads])
    , temp_icut1_darts548(in_temp_icut1) /*OMP_SHARED - INPUT*/
    , temp_icut2_darts548(in_temp_icut2) /*OMP_SHARED - INPUT*/
    , temp_icut3_darts548(in_temp_icut3) /*OMP_SHARED - INPUT*/
    , temp_icut4_darts548(in_temp_icut4) /*OMP_SHARED - INPUT*/
    , tol2e_over_schwmax_darts548(in_tol2e_over_schwmax) /*OMP_SHARED - INPUT*/
    , i_darts548(new int[this->numThreads]) /*VARIABLE*/
    , initIteration548(in_initIteration)
    , lastIteration548(in_lastIteration)
    , ompLoopSched548(1) /*Static by default with runtime scheduling*/
    , ompLoopChunk548(1) /*Chunk =1 by default with runtime scheduling*/
    , nextIteration548(INT_MAX)
    , loop548alreadySetUp(0)
    , readyCodelets(0)
    , baseNumThreads(this->numThreads / DARTS_CODELETS_MULT)
    , signalNextReady(new int[baseNumThreads])
    , temp_icut3_darts548_mutex(&(this->TPParent->temp_icut3_darts548_mutex))
    , temp_icut4_darts548_mutex(&(this->TPParent->temp_icut4_darts548_mutex))
    , temp_icut2_darts548_mutex(&(this->TPParent->temp_icut2_darts548_mutex))
    , temp_icut1_darts548_mutex(&(this->TPParent->temp_icut1_darts548_mutex))
    , checkInCodelets549(new _checkInCodelets549[this->numThreads])
{
    /*Initialize the loop parameters*/
    ompLoopSched548 = ompSchedulePolicy;
    ompLoopChunk548 = ompScheduleChunk;
    range548 = abs(lastIteration548 - initIteration548) / 1;
    rangePerCodelet548 = range548 / numThreads;
    minIteration548 = min<int>(lastIteration548, initIteration548);
    remainderRange548 = range548 % numThreads;
    nextIteration548 = initIteration548;
    /*Initialize inputs and vars.*/
    this->fock_darts548
        = (double***)malloc(sizeof(double**) * this->numThreads) /*OMP_SHARED_PRIVATE*/;
    /*Initialize Codelets*/
    _checkInCodelets549* checkInCodelets549Ptr = (this->checkInCodelets549);
#if USE_SPIN_CODELETS == 0
    firstCodelet = (this->checkInCodelets549);
#endif
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->baseNumThreads;
         codeletCounter++) {
        this->signalNextReady[codeletCounter] = 0;
#if USE_SPIN_CODELETS == 0
        (*checkInCodelets549Ptr) = _checkInCodelets549(2, 1, this, codeletCounter);
#else
        (*checkInCodelets549Ptr) = _checkInCodelets549(1, 1, this, codeletCounter);
#endif
        (*checkInCodelets549Ptr).decDep();
        checkInCodelets549Ptr++;
    }
    this->dispatchCodelet(this->getID());
    *(in_ptrToThisTP) = this;
}
void TP548::dispatchCodelet(size_t codeletID)
{
    int idx = codeletID / this->baseNumThreads;
    int localID = codeletID - this->baseNumThreads * idx;
    this->checkInCodelets549[localID].setID(codeletID);
    this->checkInCodelets549[localID].setLocalID(localID);
    /*Check if we want to replicate codelets*/
    for (size_t i = 1; i < (size_t)DARTS_CODELETS_MULT; i++) {
#if USE_SPIN_CODELETS == 0
        this->checkInCodelets549[localID + this->baseNumThreads * i]
            = _checkInCodelets549(2, 1, this, localID + this->baseNumThreads * i);
#else
        this->checkInCodelets549[localID + this->baseNumThreads * i]
            = _checkInCodelets549(1, 1, this, localID + this->baseNumThreads * i);
#endif
        this->checkInCodelets549[localID + this->baseNumThreads * i].setID(codeletID);
        this->checkInCodelets549[localID + this->baseNumThreads * i].decDep();
#if USE_SPIN_CODELETS == 0
        this->firstCodelet[localID + this->baseNumThreads * i].decDep();
#else
        this->availableCodelets[localID + this->baseNumThreads * i] = 1;
#endif
    }
#if USE_SPIN_CODELETS == 0
    this->firstCodelet[localID].decDep();
#else
    this->availableCodelets[localID] = 1;
#endif
}
TP548::~TP548()
{
    delete[] fock_darts548;
    delete[] i_darts548;
    delete[] checkInCodelets549;
}
