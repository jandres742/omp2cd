#include "common.output.darts.h"
using namespace darts;
using namespace std;
int delaylength = -1;
double delaytime = -1.;
unsigned long innerreps;
int nthreads = -1;
int outerreps = -1;
double referencesd;
double referencetime;
double targettesttime = 0.;
double testsd;
double testtime;
double* times;
/*Function: usage, ID: 8*/
void usage(char* argv[])
{
    /*usage:8*/
    /*CompoundStmt:65*/
    printf("Usage: %s.x \n\t--outer-repetitions <outer-repetitions> (default %d)\n\t--test-time "
           "<target-test-time> (default %0.2f microseconds)\n\t--delay-time <delay-time> (default "
           "%0.4f microseconds)\n\t--delay-length <delay-length> (default auto-generated based on "
           "processor speed)\n",
        argv[0], 20, 1000., 0.10000000000000001);
}
/*Function: parse_args, ID: 9*/
void parse_args(int argc, char* argv[])
{
    /*parse_args:9*/
    /*CompoundStmt:70*/
    int arg;
    for (arg = 1; arg < argc; arg++) {
        if (strcmp(argv[arg], "--delay-time") == 0.) {
            delaytime = atof(argv[++arg]);
            if (delaytime == 0.) {
                printf("Invalid float:--delay-time: %s\n", argv[arg]);
                usage(argv);
                exit(1);
            }
        } else if (strcmp(argv[arg], "--outer-repetitions") == 0) {
            outerreps = atoi(argv[++arg]);
            if (outerreps == 0) {
                printf("Invalid integer:--outer-repetitions: %s\n", argv[arg]);
                usage(argv);
                exit(1);
            }
        } else if (strcmp(argv[arg], "--test-time") == 0) {
            targettesttime = atof(argv[++arg]);
            if (targettesttime == 0) {
                printf("Invalid integer:--test-time: %s\n", argv[arg]);
                usage(argv);
                exit(1);
            }
        } else if (strcmp(argv[arg], "-h") == 0) {
            usage(argv);
            exit(0);
        } else {
            printf("Invalid parameters: %s\n", argv[arg]);
            usage(argv);
            exit(1);
        }
    }
}
/*Function: getdelaylengthfromtime, ID: 10*/
int getdelaylengthfromtime(double delaytime)
{
    /*getdelaylengthfromtime:10*/
    /*CompoundStmt:132*/
    int i, reps;
    double lapsedtime, starttime;
    reps = 1000;
    lapsedtime = 0.;
    delaytime = delaytime / 1.0E+6;
    delaylength = 0;
    delay(delaylength);
    while (lapsedtime < delaytime) {
        delaylength = delaylength * 1.1000000000000001 + 1;
        starttime = getclock();
        for (i = 0; i < reps; i++) {
            delay(delaylength);
        }
        lapsedtime = (getclock() - starttime) / (double)reps;
    }
    return delaylength;
}
/*Function: getinnerreps, ID: 11*/
unsigned long getinnerreps(void (*test)(void))
{
    /*getinnerreps:11*/
    /*CompoundStmt:164*/
    innerreps = 10L;
    double time = 0.;
    while (time < targettesttime) {
        double start = getclock();
        test();
        time = (getclock() - start) * 1.0E+6;
        innerreps *= 2;
        if (innerreps > (targettesttime * 1.0E+15)) {
            printf("Compiler has optimised reference loop away, STOP! \n");
            printf("Try recompiling with lower optimisation level \n");
            exit(1);
        }
    }
    return innerreps;
}
/*Function: printheader, ID: 12*/
void printheader(char* name)
{
    /*printheader:12*/
    /*CompoundStmt:187*/
    printf("\n");
    printf("--------------------------------------------------------\n");
    printf("Computing %s time using %lu reps\n", name, innerreps);
}
/*Function: stats, ID: 13*/
void stats(double* mtp, double* sdp)
{
    /*stats:13*/
    /*CompoundStmt:191*/
    double meantime, totaltime, sumsq, mintime, maxtime, sd, cutoff;
    int i, nr;
    mintime = 1.0E+10;
    maxtime = 0.;
    totaltime = 0.;
    for (i = 1; i <= outerreps; i++) {
        mintime = (mintime < times[i]) ? mintime : times[i];
        maxtime = (maxtime > times[i]) ? maxtime : times[i];
        totaltime += times[i];
    }
    meantime = totaltime / outerreps;
    sumsq = 0;
    for (i = 1; i <= outerreps; i++) {
        sumsq += (times[i] - meantime) * (times[i] - meantime);
    }
    sd = sqrt(sumsq / (outerreps - 1));
    cutoff = 3. * sd;
    nr = 0;
    for (i = 1; i <= outerreps; i++) {
        if (fabs(times[i] - meantime) > cutoff)
            nr++;
    }
    printf("\n");
    printf("Sample_size       Average     Min         Max          S.D.          Outliers\n");
    printf(" %d                %f   %f   %f    %f      %d\n", outerreps, meantime, mintime, maxtime,
        sd, nr);
    printf("\n");
    *mtp = meantime;
    *sdp = sd;
}
/*Function: printfooter, ID: 14*/
void printfooter(char* name, double testtime, double testsd, double referencetime, double refsd)
{
    /*printfooter:14*/
    /*CompoundStmt:256*/
    printf("%s time     = %f microseconds +/- %f\n", name, testtime, 1.96 * testsd);
    printf("%s overhead = %f microseconds +/- %f\n", name, testtime - referencetime,
        1.96 * (testsd + referencesd));
}
/*Function: printreferencefooter, ID: 15*/
void printreferencefooter(char* name, double referencetime, double referencesd)
{
    /*printreferencefooter:15*/
    /*CompoundStmt:265*/
    printf("%s time     = %f microseconds +/- %f\n", name, referencetime, 1.96 * referencesd);
}
/*Function: init, ID: 16*/
void init(int argc, char** argv)
{
    /*init:16*/
    /*CompoundStmt:269*/
    if (affinMaskRes) {
        myDARTSRuntime->run(
            launch<TP270>(ompNumThreads * DARTS_CODELETS_MULT, 0, RuntimeFinalCodelet));
    }
    parse_args(argc, argv);
    if (outerreps == -1) {
        outerreps = 20;
    }
    if (targettesttime == 0.) {
        targettesttime = 1000.;
    }
    if (delaytime == -1.) {
        delaytime = 0.10000000000000001;
    }
    delaylength = getdelaylengthfromtime(delaytime);
    times = malloc((outerreps + 1) * sizeof(double));
    printf("Running OpenMP benchmark version 3.0\n\t%d thread(s)\n\t%d outer repetitions\n\t%0.2f "
           "test time (microseconds)\n\t%d delay length (iterations) \n\t%f delay time "
           "(microseconds)\n",
        nthreads, outerreps, targettesttime, delaylength, delaytime);
}
/*Function: finalise, ID: 17*/
void finalise()
{
    /*finalise:17*/
    /*CompoundStmt:299*/
    free(times);
}
/*Function: initreference, ID: 18*/
void initreference(char* name)
{
    /*initreference:18*/
    /*CompoundStmt:301*/
    printheader(name);
}
/*Function: reference, ID: 19*/
void reference(char* name, void (*refer)(void))
{
    /*reference:19*/
    /*CompoundStmt:303*/
    int k;
    double start;
    innerreps = getinnerreps(refer);
    initreference(name);
    for (k = 0; k <= outerreps; k++) {
        start = getclock();
        refer();
        times[k] = (getclock() - start) * 1.0E+6 / (double)innerreps;
    }
    finalisereference(name);
}
/*Function: finalisereference, ID: 20*/
void finalisereference(char* name)
{
    /*finalisereference:20*/
    /*CompoundStmt:326*/
    stats(&referencetime, &referencesd);
    printreferencefooter(name, referencetime, referencesd);
}
/*Function: intitest, ID: 21*/
void intitest(char* name)
{
    /*intitest:21*/
    /*CompoundStmt:331*/
    printheader(name);
}
/*Function: finalisetest, ID: 22*/
void finalisetest(char* name)
{
    /*finalisetest:22*/
    /*CompoundStmt:333*/
    stats(&testtime, &testsd);
    printfooter(name, testtime, testsd, referencetime, referencesd);
}
/*Function: benchmark, ID: 23*/
void benchmark(char* name, void (*test)(void))
{
    /*benchmark:23*/
    /*CompoundStmt:338*/
    int k;
    double start;
    innerreps = getinnerreps(test);
    intitest(name);
    for (k = 0; k <= outerreps; k++) {
        start = getclock();
        test();
        times[k] = (getclock() - start) * 1.0E+6 / (double)innerreps;
    }
    finalisetest(name);
}
/*Function: delay, ID: 24*/
void delay(int delaylength)
{
    /*delay:24*/
    /*CompoundStmt:361*/
    int i;
    float a = 0.;
    for (i = 0; i < delaylength; i++)
        a += i;
    if (a < 0)
        printf("%f \n", a);
}
/*Function: array_delay, ID: 25*/
void array_delay(int delaylength, double* a)
{
    /*array_delay:25*/
    /*CompoundStmt:374*/
    int i;
    a[0] = 1.;
    for (i = 0; i < delaylength; i++)
        a[0] += i;
    if (a[0] < 0)
        printf("%f \n", a[0]);
}
/*Function: getclock, ID: 26*/
double getclock()
{
    /*getclock:26*/
    /*CompoundStmt:390*/
    double time;
    double omp_get_wtime();
    time = omp_get_wtime();
    return time;
}
/*Function: returnfalse, ID: 28*/
int returnfalse()
{
    /*returnfalse:28*/
    /*CompoundStmt:396*/
    return 0;
}
/*TP270: OMPParallelDirective*/
void TP270::_barrierCodelets270::fire(void)
{
    TP270* myTP = static_cast<TP270*>(myTP_);
    myTP->controlTPParent->nextCodelet->decDep();
}
void TP270::_checkInCodelets272::fire(void)
{
    if (this->getID() == 0) {
        /*Init the vars for this region*/

        /*printing node 274: BinaryOperator*/
        nthreads = omp_get_num_threads();
        /*Signaling next codelet from last stmt in the codelet*/
        /*Find and signal the next codelet*/
        myTP->controlTPParent->TPParent->barrierCodelets270[0].decDep();
    } else {
        /*Find and signal the next codelet*/
        myTP->TPParent->barrierCodelets270[0].decDep();
    }
}
TP270::TP270(int in_numThreads, int in_mainCodeletID, Codelet* in_nextCodelet)
    : ThreadedProcedure(in_numThreads, in_mainCodeletID)
    , nextCodelet(in_nextCodelet)
    , TPParent(this)
    , controlTPParent(this)
    , inputsTPParent(this)
    , TP272_alreadyLaunched(0)
    , barrierCodelets270(new _barrierCodelets270[1])
    , checkInCodelets272(new _checkInCodelets272[this->numThreads])
{
    /*Initialize inputs and vars.*/
    /*Initialize Codelets*/
    barrierCodelets270[0] = _barrierCodelets270(ompNumThreads, ompNumThreads, this, 0);
    _checkInCodelets272* checkInCodelets272Ptr = (this->checkInCodelets272);
    for (size_t codeletCounter = 0; codeletCounter < (size_t)this->numThreads; codeletCounter++) {
        (*checkInCodelets272Ptr) = _checkInCodelets272(1, 1, this, codeletCounter);
        (*checkInCodelets272Ptr).decDep();
        checkInCodelets272Ptr++;
    }
}
TP270::~TP270()
{
    delete[] barrierCodelets270;
    delete[] checkInCodelets272;
}
