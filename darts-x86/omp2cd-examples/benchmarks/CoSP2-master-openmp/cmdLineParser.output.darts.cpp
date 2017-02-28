#include "cmdLineParser.output.darts.h"
using namespace darts;
using namespace std;
static int longest = 1;
static MyOption* myargs = ((void*)0);
int iBase_darts2;
bool iBase_darts2_initFlag;
static MyOption* findOption(MyOption* o, unsigned char shortArg);
static MyOption* lastOption(MyOption* o);
static MyOption* myOptionFree(MyOption* o);
static MyOption* myOptionAlloc(const char* longOption, const char shortOption, int has_arg,
    const char type, void* dataPtr, int dataSize, const char* help);
static char* dupString(const char* s);
/*Function: dupString, ID: 1*/
static char* dupString(const char* s)
{
    /*dupString:1*/
    /*CompoundStmt:1039*/
    char* d;
    if (!s)
        s = "";
    d = (char*)calloc((strlen(s) + 1), sizeof(char));
    strcpy(d, s);
    return d;
}
/*Function: myOptionAlloc, ID: 2*/
static MyOption* myOptionAlloc(const char* longOption, const char shortOption, int has_arg,
    const char type, void* dataPtr, int dataSize, const char* help)
{
    /*myOptionAlloc:2*/
    /*CompoundStmt:1053*/
    static int iBase = 129;
    MyOption* o = (MyOption*)calloc(1, sizeof(MyOption));
    o->help = dupString(help);
    o->longArg = dupString(longOption);
    if (shortOption)
        o->shortArg[0] = (unsigned char)shortOption;
    else {
        o->shortArg[0] = iBase;
        iBase++;
    }
    o->argFlag = has_arg;
    o->type = type;
    o->ptr = dataPtr;
    o->sz = dataSize;
    if (longOption)
        longest = (longest > strlen(longOption) ? longest : strlen(longOption));
    return o;
}
/*Function: myOptionFree, ID: 3*/
static MyOption* myOptionFree(MyOption* o)
{
    /*myOptionFree:3*/
    /*CompoundStmt:1094*/
    MyOption* r;
    if (!o)
        return ((void*)0);
    r = ((MyOption*)o->next);
    if (o->longArg)
        free(o->longArg);
    if (o->help)
        free(o->help);
    free(o);
    return r;
}
/*Function: lastOption, ID: 4*/
static MyOption* lastOption(MyOption* o)
{
    /*lastOption:4*/
    /*CompoundStmt:1116*/
    if (!o)
        return o;
    while (((MyOption*)o->next))
        o = ((MyOption*)o->next);
    return o;
}
/*Function: findOption, ID: 5*/
static MyOption* findOption(MyOption* o, unsigned char shortArg)
{
    /*findOption:5*/
    /*CompoundStmt:1129*/
    while (o) {
        if (o->shortArg[0] == shortArg)
            return o;
        o = ((MyOption*)o->next);
    }
    return o;
}
/*Function: addArg, ID: 6*/
int addArg(const char* longOption, const char shortOption, int has_arg, const char type,
    void* dataPtr, int dataSize, const char* help)
{
    /*addArg:6*/
    /*CompoundStmt:1141*/
    MyOption* o;
    MyOption* p;
    o = myOptionAlloc(longOption, shortOption, has_arg, type, dataPtr, dataSize, help);
    if (!o)
        return 1;
    if (!myargs)
        myargs = o;
    else {
        p = lastOption(myargs);
        p->next = (void*)o;
    }
    return 0;
}
/*Function: freeArgs, ID: 7*/
void freeArgs()
{
    /*freeArgs:7*/
    /*CompoundStmt:1161*/
    while (myargs) {
        myargs = myOptionFree(myargs);
    }
    return;
}
/*Function: printArgs, ID: 8*/
void printArgs()
{
    /*printArgs:8*/
    /*CompoundStmt:1168*/
    MyOption* o = myargs;
    char s[4096];
    unsigned char* shortArg;
    fprintf(stdout, "\n  Arguments are: \n");
    sprintf(s, "   --%%-%ds", longest);
    while (o) {
        if (o->shortArg[0] < 255)
            shortArg = o->shortArg;
        else
            shortArg = (unsigned char*)"---";
        fprintf(stdout, s, o->longArg);
        fprintf(stdout, " -%c  arg=%1d type=%c  %s\n", shortArg[0], o->argFlag, o->type, o->help);
        o = ((MyOption*)o->next);
    }
    fprintf(stdout, "\n\n");
    return;
}
/*Function: processArgs, ID: 9*/
void processArgs(int argc, char** argv)
{
    /*processArgs:9*/
    /*CompoundStmt:1197*/
    MyOption* o;
    int n = 0;
    int i;
    struct option* opts;
    char* sArgs;
    int c;
    if (!myargs)
        return;
    o = myargs;
    while (o) {
        n++, o = ((MyOption*)o->next);
    }
    o = myargs;
    sArgs = (char*)calloc(2 * (n + 2), sizeof(char));
    opts = (struct option*)calloc(n, sizeof(struct option));
    for (i = 0; i < n; i++) {
        opts[i].name = o->longArg;
        opts[i].has_arg = o->argFlag;
        opts[i].flag = 0;
        opts[i].val = o->shortArg[0];
        strcat(sArgs, (char*)o->shortArg);
        if (o->argFlag)
            strcat(sArgs, ":");
        o = ((MyOption*)o->next);
    }
    while (1) {
        int option_index = 0;
        c = getopt_long(argc, argv, sArgs, opts, &option_index);
        if (c == -1)
            break;
        o = findOption(myargs, c);
        if (!o) {
            fprintf(stdout, "\n\n    invalid switch : -%c in getopt()\n\n\n", c);
            break;
        }
        if (!o->argFlag) {
            int* i = (int*)o->ptr;
            *i = 1;
        } else {
            switch (o->type) {
            case 'i':
                sscanf(optarg, "%d", (int*)o->ptr);
                break;
            case 'f':
                sscanf(optarg, "%f", (float*)o->ptr);
                break;
            case 'd':
                sscanf(optarg, "%lf", (double*)o->ptr);
                break;
            case 's':
                strncpy((char*)o->ptr, (char*)optarg, o->sz);
                ((char*)o->ptr)[o->sz - 1] = '\x00';
                break;
            case 'c':
                sscanf(optarg, "%c", (char*)o->ptr);
                break;
            default:
                fprintf(stdout, "\n\n    invalid type : %c in getopt()\n    valid values are 'e', "
                                "'z'. 'i','d','f','s', and 'c'\n\n\n",
                    c);
            }
        }
    }
    free(opts);
    free(sArgs);
    return;
}
