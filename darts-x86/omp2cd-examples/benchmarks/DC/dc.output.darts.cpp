#include "dc.output.darts.h"
using namespace darts;
using namespace std;
std::mutex TP3488mutex;
char CLASS;
long long attrnum;
long long checksumAhi = 7141688;
long long checksumAlo = 178042;
long long checksumBhi = 9348365;
long long checksumBlo = 700453;
long long checksumS = 464620213;
long long checksumWhi = 1401796;
long long checksumWlo = 434318;
long long input_tuples;
/*Function: main, ID: 20*/
int main(int argc, char* argv[])
{
    getOMPNumThreads();
    getOMPSchedulePolicy();
    getTPLoopThresholds();
    getNumTPs();
    affin = new ThreadAffinity(
        ompNumThreads / NUMTPS - 1, NUMTPS, COMPACT, getDARTSTPPolicy(), getDARTSMCPolicy());
    affinMaskRes = affin->generateMask();
    myDARTSRuntime = new Runtime(affin);
    RuntimeFinalCodelet = &(myDARTSRuntime->finalSignal);
    /*main:20*/
    /*CompoundStmt:3263*/
    ADC_PAR* parp;
    ADC_VIEW_PARS* adcpp;
    int32 retCode;
    fprintf(stdout, "\n\n NAS Parallel Benchmarks (NPB3.3-OMP) - DC Benchmark\n\n");
    if (argc == 2) {
        CLASS = argv[1][0];
    } else if (argc > 3
        || (argc > 1 && !((*__ctype_b_loc())[(int)((argv[1][0]))] & (unsigned short)_ISdigit))
        || argc == 1) {
        fprintf(stderr, "Usage: <program name> <amount of memory> <file of parameters>\n");
        fprintf(stderr, "Example: bin/dc.exe 1000000 DC/ADC.par\n");
        fprintf(stderr, "The last argument, (a parameter file) can be skipped\n");
        fprintf(stderr, "Usage: <program name> <scale>\n");
        fprintf(stderr, "Example: bin/dc.exe S\n");
        exit(1);
    }
    if (CLASS == 'S') {
        input_tuples = 1000;
        attrnum = 5;
    } else if (CLASS == 'W') {
        input_tuples = 100000;
        attrnum = 10;
    } else if (CLASS == 'A') {
        input_tuples = 1000000;
        attrnum = 15;
    } else if (CLASS == 'B') {
        input_tuples = 10000000;
        attrnum = 20;
    }
    if (!(parp = (ADC_PAR*)malloc(sizeof(ADC_PAR)))
        || !(adcpp = (ADC_VIEW_PARS*)malloc(sizeof(ADC_VIEW_PARS)))) {
        {
            fprintf(stderr, " %s, errno = %d\n", "main: malloc failed", (*__errno_location()));
        }
        exit(1);
    }
    initADCpar(parp);
    parp->clss = CLASS;
    if (argc != 3) {
        parp->dim = attrnum;
        parp->tuplenum = input_tuples;
    } else if ((argc == 3) && (!ParseParFile(argv[2], parp))) {
        {
            fprintf(stderr, " %s, errno = %d\n", "main.ParseParFile failed", (*__errno_location()));
        }
        exit(1);
    }
    ShowADCPar(parp);
    if (!GenerateADC(parp)) {
        {
            fprintf(stderr, " %s, errno = %d\n", "main.GenerateAdc failed", (*__errno_location()));
        }
        exit(1);
    }
    adcpp->ndid = parp->ndid;
    adcpp->clss = parp->clss;
    adcpp->nd = parp->dim;
    adcpp->nm = parp->mnum;
    adcpp->nTasks = 1;
    if (argc >= 2)
        adcpp->memoryLimit = atoi(argv[1]);
    else
        adcpp->memoryLimit = 0;
    if (adcpp->memoryLimit <= 0) {
        adcpp->memoryLimit = parp->tuplenum * (50 + 5 * parp->dim);
        fprintf(stdout, "Estimated rb-tree size = %d \n", adcpp->memoryLimit);
    }
    adcpp->nInputRecs = parp->tuplenum;
    strcpy(adcpp->adcName, parp->filename);
    strcpy(adcpp->adcInpFileName, parp->filename);
    if ((retCode = DC(adcpp))) {
        {
            fprintf(stderr, " %s, errno = %d\n", "main.DC failed", (*__errno_location()));
        }
        fprintf(stderr, "main.ParRun failed: retcode = %d\n", retCode);
        exit(1);
    }
    if (parp) {
        free(parp);
        parp = 0;
    }
    if (adcpp) {
        free(adcpp);
        adcpp = 0;
    }
    return 0;
}
/*Function: DC, ID: 21*/
int32 DC(ADC_VIEW_PARS* adcpp)
{
    /*DC:21*/
    /*CompoundStmt:3408*/
    int32 itsk = 0;
    double t_total = 0.;
    int verified;
    PAR_VIEW_ST* pvstp;
    pvstp = (PAR_VIEW_ST*)malloc(sizeof(PAR_VIEW_ST));
    pvstp->verificationFailed = 0;
    pvstp->totalViewTuples = 0;
    pvstp->totalViewSizesInBytes = 0;
    pvstp->totalNumberOfMadeViews = 0;
    pvstp->checksum = 0;
    adcpp->nTasks = omp_get_max_threads();
    fprintf(stdout, "\nNumber of available threads:  %d\n", adcpp->nTasks);
    if (adcpp->nTasks > 256) {
        adcpp->nTasks = 256;
        fprintf(stdout, "Warning: Maximum number of tasks reached: %d\n", adcpp->nTasks);
    }
    if (affinMaskRes) {
        myDARTSRuntime->run(launch<TP3440>(ompNumThreads * DARTS_CODELETS_MULT, 0,
            RuntimeFinalCodelet, (ADC_VIEW_PARS**)&((adcpp)), (PAR_VIEW_ST**)&((pvstp))));
    }
    t_total = pvstp->tm_max;
    pvstp->verificationFailed = Verify(pvstp->checksum, adcpp);
    verified = (pvstp->verificationFailed == -1) ? -1 : (pvstp->verificationFailed == 0) ? 1 : 0;
    fprintf(stdout, "\n*** DC Benchmark Results:\n");
    fprintf(stdout, " Benchmark Time   = %20.3f\n", t_total);
    fprintf(stdout, " Input Tuples     =         %12d\n", (int)adcpp->nInputRecs);
    fprintf(stdout, " Number of Views  =         %12d\n", (int)pvstp->totalNumberOfMadeViews);
    fprintf(stdout, " Number of Tasks  =         %12d\n", (int)adcpp->nTasks);
    fprintf(stdout, " Tuples Generated = %20.0f\n", (double)pvstp->totalViewTuples);
    fprintf(stdout, " Tuples/s         = %20.2f\n", (double)pvstp->totalViewTuples / t_total);
    fprintf(stdout, " Checksum         = %20.12e\n", (double)pvstp->checksum);
    if (pvstp->verificationFailed)
        fprintf(stdout, " Verification failed\n");
    c_print_results("DC", adcpp->clss, (int)adcpp->nInputRecs, 0, 0, 1, t_total,
        (double)pvstp->totalViewTuples * 9.9999999999999995E-7 / t_total, "Tuples generated",
        verified, "3.3.1", "24 Jul 2016", "gcc", "$(CC)",
        "-lm -L/home/jaime/phd/clang-llvm/openmp/run...",
        "-I/home/jaime/phd/clang-llvm/openmp/runtime...", "-O3 -fopenmp", "-O3 -fopenmp");
    return 0;
}
/*Function: Verify, ID: 22*/
int Verify(long long checksum, ADC_VIEW_PARS* adcpp)
{
    /*Verify:22*/
    /*CompoundStmt:3573*/
    switch (adcpp->clss) {
    case 'S':
        if (checksum == checksumS)
            return 0;
        break;
    case 'W':
        if (checksum == checksumWlo + 1000000 * checksumWhi)
            return 0;
        break;
    case 'A':
        if (checksum == checksumAlo + 1000000 * checksumAhi)
            return 0;
        break;
    case 'B':
        if (checksum == checksumBlo + 1000000 * checksumBhi)
            return 0;
        break;
    default:
        return -1;
    }
    return 1;
}
/*TP3440: OMPParallelDirective*/
void TP3440::_barrierCodelets3440::fire(void)
{
    TP3440* myTP = static_cast<TP3440*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
void TP3440::_checkInCodelets3442::fire(void)
{
    /*Init the vars for this region*/

    /*printing node 3442: DeclStmt*/
    this->inputsTPParent->tm0_darts3440[this->getID()] = 0;

    /*printing node 3443: DeclStmt*/
    this->inputsTPParent->itimer_darts3440[this->getID()] = 0;

    /*printing node 3444: DeclStmt*/

    /*printing node 3445: BinaryOperator*/
    (this->inputsTPParent->itsk_darts3440[this->getID()]) = omp_get_thread_num();

    /*printing node 3447: BinaryOperator*/
    (this->inputsTPParent->adccntlp_darts3440[this->getID()])
        = NewAdcViewCntl((*(this->inputsTPParent->adcpp_darts3440)),
            (this->inputsTPParent->itsk_darts3440[this->getID()]));

    /*printing node 3449: IfStmt*/
    if (!(this->inputsTPParent->adccntlp_darts3440[this->getID()])) {
        {
            fprintf(stderr, " %s, errno = %d\n", "ParRun.NewAdcViewCntl: returned NULL",
                (*__errno_location()));
        }
        (this->inputsTPParent->adccntlp_darts3440[this->getID()])->verificationFailed = 1;
    } else {
        (this->inputsTPParent->adccntlp_darts3440[this->getID()])->verificationFailed = 0;
        if ((this->inputsTPParent->adccntlp_darts3440[this->getID()])->retCode != 0) {
            fprintf(stderr, "DC.NewAdcViewCntl: return code = %d\n",
                (this->inputsTPParent->adccntlp_darts3440[this->getID()])->retCode);
        }
    }

    /*printing node 3466: IfStmt*/
    if (!(this->inputsTPParent->adccntlp_darts3440[this->getID()])->verificationFailed) {
        if (PartitionCube((this->inputsTPParent->adccntlp_darts3440[this->getID()]))) {
            {
                fprintf(
                    stderr, " %s, errno = %d\n", "DC.PartitionCube failed", (*__errno_location()));
            }
        }
        timer_clear((this->inputsTPParent->itimer_darts3440[this->getID()]));
        timer_start((this->inputsTPParent->itimer_darts3440[this->getID()]));
        if (ComputeGivenGroupbys((this->inputsTPParent->adccntlp_darts3440[this->getID()]))) {
            {
                fprintf(stderr, " %s, errno = %d\n", "DC.ComputeGivenGroupbys failed",
                    (*__errno_location()));
            }
        }
        timer_stop((this->inputsTPParent->itimer_darts3440[this->getID()]));
        (this->inputsTPParent->tm0_darts3440[this->getID()])
            = timer_read((this->inputsTPParent->itimer_darts3440[this->getID()]));
    }

    /*printing node 3490: IfStmt*/
    TP3488mutex.lock();
    if ((*(this->inputsTPParent->pvstp_darts3440))->tm_max
        < (this->inputsTPParent->tm0_darts3440[this->getID()])) {
        (*(this->inputsTPParent->pvstp_darts3440))->tm_max
            = (this->inputsTPParent->tm0_darts3440[this->getID()]);
    }

    /*printing node 3495: CompoundAssignOperator*/
    (*(this->inputsTPParent->pvstp_darts3440))->verificationFailed
        += (this->inputsTPParent->adccntlp_darts3440[this->getID()])->verificationFailed;

    /*printing node 3498: IfStmt*/
    if (!(this->inputsTPParent->adccntlp_darts3440[this->getID()])->verificationFailed) {
        (*(this->inputsTPParent->pvstp_darts3440))->totalNumberOfMadeViews
            += (this->inputsTPParent->adccntlp_darts3440[this->getID()])->numberOfMadeViews;
        (*(this->inputsTPParent->pvstp_darts3440))->totalViewSizesInBytes
            += (this->inputsTPParent->adccntlp_darts3440[this->getID()])->totalViewFileSize;
        (*(this->inputsTPParent->pvstp_darts3440))->totalViewTuples
            += (this->inputsTPParent->adccntlp_darts3440[this->getID()])->totalOfViewRows;
        (*(this->inputsTPParent->pvstp_darts3440))->checksum
            += (this->inputsTPParent->adccntlp_darts3440[this->getID()])->totchs[0];
    }
    TP3488mutex.unlock();

    /*printing node 3514: IfStmt*/
    if (CloseAdcView((this->inputsTPParent->adccntlp_darts3440[this->getID()]))) {
        {
            fprintf(stderr, " %s, errno = %d\n", "ParRun.CloseAdcView: is failed",
                (*__errno_location()));
        }
        (this->inputsTPParent->adccntlp_darts3440[this->getID()])->verificationFailed = 1;
    }
    /*Signaling next codelet from last stmt in the codelet*/
    /*Find and signal the next codelet*/
    myTP->controlTPParent->TPParent->barrierCodelets3440[0].decDep();
}
TP3440::TP3440(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet,
    ADC_VIEW_PARS** in_adcpp, PAR_VIEW_ST** in_pvstp)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , adcpp_darts3440(in_adcpp) /*OMP_SHARED - INPUT*/
    , itsk_darts3440(new int32[this->numThreads]) /*OMP_PRIVATE - INPUT*/
    , pvstp_darts3440(in_pvstp) /*OMP_SHARED - INPUT*/
    , adccntlp_darts3440(new ADC_VIEW_CNTL*[this->numThreads]) /*VARIABLE*/
    , itimer_darts3440(new int[this->numThreads]) /*VARIABLE*/
    , tm0_darts3440(new double[this->numThreads]) /*VARIABLE*/
    , barrierCodelets3440(new _barrierCodelets3440[1])
    , checkInCodelets3442(new _checkInCodelets3442[this->numThreads])
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets3440[0] = _barrierCodelets3440(ompNumThreads, ompNumThreads, this, 0);
    _checkInCodelets3442* checkInCodelets3442Ptr = (this->checkInCodelets3442);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets3442Ptr) = _checkInCodelets3442(1, 1, this, codeletCounter);
        (*checkInCodelets3442Ptr).decDep();
        checkInCodelets3442Ptr++;
    }
}
TP3440::~TP3440()
{
    delete[] adccntlp_darts3440;
    delete[] itimer_darts3440;
    delete[] tm0_darts3440;
    delete[] barrierCodelets3440;
    delete[] checkInCodelets3442;
}
