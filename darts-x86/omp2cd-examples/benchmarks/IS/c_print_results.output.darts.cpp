#include "c_print_results.output.darts.h"
using namespace darts;
using namespace std;
/*Function: c_print_results, ID: 9*/
void c_print_results(char* name, char class_name, int n1, int n2, int n3, int niter, double t,
    double mops, char* optype, int passed_verification, char* npbversion, char* compiletime,
    char* cc, char* clink, char* c_lib, char* c_inc, char* cflags, char* clinkflags)
{
    /*c_print_results:9*/
    /*CompoundStmt:1026*/
    int num_threads, max_threads;
    max_threads = 1;
    num_threads = 1;
    max_threads = omp_get_max_threads();
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP1032>(
            ompNumThreads * DARTS_CODELETS_MULT, 0, RuntimeFinalCodelet, (int*)&((num_threads))));
    }
    printf("\n\n %s Benchmark Completed\n", name);
    printf(" Class           =                        %c\n", class_name);
    if (n3 == 0) {
        long nn = n1;
        if (n2 != 0)
            nn *= n2;
        printf(" Size            =             %12ld\n", nn);
    } else
        printf(" Size            =             %4dx%4dx%4d\n", n1, n2, n3);
    printf(" Iterations      =             %12d\n", niter);
    printf(" Time in seconds =             %12.2f\n", t);
    printf(" Total threads   =             %12d\n", num_threads);
    printf(" Avail threads   =             %12d\n", max_threads);
    if (num_threads != max_threads)
        printf(" Warning: Threads used differ from threads available\n");
    printf(" Mop/s total     =             %12.2f\n", mops);
    printf(" Mop/s/thread    =             %12.2f\n", mops / (double)num_threads);
    printf(" Operation type  = %24s\n", optype);
    if (passed_verification < 0)
        printf(" Verification    =            NOT PERFORMED\n");
    else if (passed_verification)
        printf(" Verification    =               SUCCESSFUL\n");
    else
        printf(" Verification    =             UNSUCCESSFUL\n");
    printf(" Version         =             %12s\n", npbversion);
    printf(" Compile date    =             %12s\n", compiletime);
    printf("\n Compile options:\n");
    printf("    CC           = %s\n", cc);
    printf("    CLINK        = %s\n", clink);
    printf("    C_LIB        = %s\n", c_lib);
    printf("    C_INC        = %s\n", c_inc);
    printf("    CFLAGS       = %s\n", cflags);
    printf("    CLINKFLAGS   = %s\n", clinkflags);
    printf("\n\n");
    printf(" Please send all errors/feedbacks to:\n\n");
    printf(" NPB Development Team\n");
    printf(" npb@nas.nasa.gov\n\n\n");
}
/*TP1032: OMPParallelDirective*/
void TP1032::_barrierCodelets1032::fire(void)
{
    TP1032* myTP = static_cast<TP1032*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
void TP1032::_checkInCodelets1034::fire(void)
{
    if (this->getID() == 0) {
        /*Init the vars for this region*/
        /*Initialize the vars of the inlined region*/
        this->inputsTPParent->num_threads_darts1034
            = (this->inputsTPParent->num_threads_darts1032) /*OMP_SHARED - VAR INLINED*/;

        /*printing node 1035: BinaryOperator*/
        (*(this->inputsTPParent->num_threads_darts1034)) = omp_get_num_threads();
        /*Signaling next codelet from last stmt in the codelet*/
        /*Find and signal the next codelet*/
        myTP->controlTPParent->TPParent->barrierCodelets1032[0].decDep();
    } else {
        /*Find and signal the next codelet*/
        myTP->TPParent->barrierCodelets1032[0].decDep();
    }
}
TP1032::TP1032(
    int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet, int* in_num_threads)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , num_threads_darts1032(in_num_threads) /*OMP_SHARED - INPUT*/
    , TP1034_alreadyLaunched(0)
    , barrierCodelets1032(new _barrierCodelets1032[1])
    , checkInCodelets1034(new _checkInCodelets1034[this->numThreads])
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets1032[0] = _barrierCodelets1032(ompNumThreads, ompNumThreads, this, 0);
    _checkInCodelets1034* checkInCodelets1034Ptr = (this->checkInCodelets1034);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets1034Ptr) = _checkInCodelets1034(1, 1, this, codeletCounter);
        (*checkInCodelets1034Ptr).decDep();
        checkInCodelets1034Ptr++;
    }
}
TP1032::~TP1032()
{
    delete[] barrierCodelets1032;
    delete[] checkInCodelets1034;
}
