#ifndef DOTPLOOP_H
#define DOTPLOOP_H
#include "TPLoopClosure.h"
#include "threadlocal.h"

namespace darts {

template <class newTP>
ThreadedProcedure*
TPLoopFactory(void* space, TPLoopClosure* args)
{
    myThread.tempParent = (ThreadedProcedure*)space;
    return new (space) newTP(args->mainID, args->toSignal, args->totalTPLoops, args->codeletsPerTP);
}

template <class newTP, class arg1>
ThreadedProcedure*
TPLoopFactory(void* space, TPLoopClosure* closure)
{
    TPLoopClosure1<arg1>* args = static_cast<TPLoopClosure1<arg1>*>(closure);
    myThread.tempParent = (ThreadedProcedure*)space;
    return new (space) newTP(args->mainID, args->toSignal, args->totalTPLoops, args->codeletsPerTP, args->a1);
}

template <class newTP, class arg1, class arg2>
ThreadedProcedure*
TPLoopFactory(void* space, TPLoopClosure* closure)
{
    TPLoopClosure2<arg1, arg2>* args = static_cast<TPLoopClosure2<arg1, arg2>*>(closure);
    myThread.tempParent = (ThreadedProcedure*)space;
    return new (space) newTP(args->mainID, args->toSignal, args->totalTPLoops, args->codeletsPerTP, args->a1, args->a2);
}

template <class newTP, class arg1, class arg2, class arg3>
ThreadedProcedure*
TPLoopFactory(void* space, TPLoopClosure* closure)
{
    TPLoopClosure3<arg1, arg2, arg3>* args = static_cast<TPLoopClosure3<arg1, arg2, arg3>*>(closure);
    myThread.tempParent = (ThreadedProcedure*)space;
    return new (space) newTP(args->mainID, args->toSignal, args->totalTPLoops, args->codeletsPerTP, args->a1, args->a2, args->a3);
}

template <class newTP, class arg1, class arg2, class arg3, class arg4>
ThreadedProcedure*
TPLoopFactory(void* space, TPLoopClosure* closure)
{
    TPLoopClosure4<arg1, arg2, arg3, arg4>* args = static_cast<TPLoopClosure4<arg1, arg2, arg3, arg4>*>(closure);
    myThread.tempParent = (ThreadedProcedure*)space;
    return new (space) newTP(args->mainID, args->toSignal, args->totalTPLoops, args->codeletsPerTP, args->a1, args->a2, args->a3, args->a4);
}

template <class newTP, class arg1, class arg2, class arg3, class arg4, class arg5>
ThreadedProcedure*
TPLoopFactory(void* space, TPLoopClosure* closure)
{
    TPLoopClosure5<arg1, arg2, arg3, arg4, arg5>* args = static_cast<TPLoopClosure5<arg1, arg2, arg3, arg4, arg5>*>(closure);
    myThread.tempParent = (ThreadedProcedure*)space;
    return new (space) newTP(args->mainID, args->toSignal, args->totalTPLoops, args->codeletsPerTP, args->a1, args->a2, args->a3, args->a4, args->a5);
}

template <class newTP, class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6>
ThreadedProcedure*
TPLoopFactory(void* space, TPLoopClosure* closure)
{
    TPLoopClosure6<arg1, arg2, arg3, arg4, arg5,
        arg6>* args
        = static_cast<TPLoopClosure6<arg1, arg2, arg3, arg4, arg5,
            arg6>*>(closure);
    myThread.tempParent = (ThreadedProcedure*)space;
    return new (space) newTP(args->mainID, args->toSignal, args->totalTPLoops, args->codeletsPerTP, args->a1, args->a2, args->a3, args->a4, args->a5,
        args->a6);
}

template <class newTP, class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7>
ThreadedProcedure*
TPLoopFactory(void* space, TPLoopClosure* closure)
{
    TPLoopClosure7<arg1, arg2, arg3, arg4, arg5,
        arg6, arg7>* args
        = static_cast<TPLoopClosure7<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7>*>(closure);
    myThread.tempParent = (ThreadedProcedure*)space;
    return new (space) newTP(args->mainID, args->toSignal, args->totalTPLoops, args->codeletsPerTP, args->a1, args->a2, args->a3, args->a4, args->a5,
        args->a6, args->a7);
}

template <class newTP, class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8>
ThreadedProcedure*
TPLoopFactory(void* space, TPLoopClosure* closure)
{
    TPLoopClosure8<arg1, arg2, arg3, arg4, arg5,
        arg6, arg7, arg8>* args
        = static_cast<TPLoopClosure8<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8>*>(closure);
    myThread.tempParent = (ThreadedProcedure*)space;
    return new (space) newTP(args->mainID, args->toSignal, args->totalTPLoops, args->codeletsPerTP, args->a1, args->a2, args->a3, args->a4, args->a5,
        args->a6, args->a7, args->a8);
}

template <class newTP, class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9>
ThreadedProcedure*
TPLoopFactory(void* space, TPLoopClosure* closure)
{
    TPLoopClosure9<arg1, arg2, arg3, arg4, arg5,
        arg6, arg7, arg8, arg9>* args
        = static_cast<TPLoopClosure9<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9>*>(closure);
    myThread.tempParent = (ThreadedProcedure*)space;
    return new (space) newTP(args->mainID, args->toSignal, args->totalTPLoops, args->codeletsPerTP, args->a1, args->a2, args->a3, args->a4, args->a5,
        args->a6, args->a7, args->a8, args->a9);
}

template <class newTP, class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10>
ThreadedProcedure*
TPLoopFactory(void* space, TPLoopClosure* closure)
{
    TPLoopClosure10<arg1, arg2, arg3, arg4, arg5,
        arg6, arg7, arg8, arg9, arg10>* args
        = static_cast<TPLoopClosure10<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10>*>(closure);
    myThread.tempParent = (ThreadedProcedure*)space;
    return new (space) newTP(args->mainID, args->toSignal, args->totalTPLoops, args->codeletsPerTP, args->a1, args->a2, args->a3, args->a4, args->a5,
        args->a6, args->a7, args->a8, args->a9, args->a10);
}

template <class newTP, class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11>
ThreadedProcedure*
TPLoopFactory(void* space, TPLoopClosure* closure)
{
    TPLoopClosure11<arg1, arg2, arg3, arg4, arg5,
        arg6, arg7, arg8, arg9, arg10,
        arg11>* args
        = static_cast<TPLoopClosure11<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11>*>(closure);
    myThread.tempParent = (ThreadedProcedure*)space;
    return new (space) newTP(args->mainID, args->toSignal, args->totalTPLoops, args->codeletsPerTP, args->a1, args->a2, args->a3, args->a4, args->a5,
        args->a6, args->a7, args->a8, args->a9, args->a10,
        args->a11);
}

template <class newTP, class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11, class arg12>
ThreadedProcedure*
TPLoopFactory(void* space, TPLoopClosure* closure)
{
    TPLoopClosure12<arg1, arg2, arg3, arg4, arg5,
        arg6, arg7, arg8, arg9, arg10,
        arg11, arg12>* args
        = static_cast<TPLoopClosure12<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12>*>(closure);
    myThread.tempParent = (ThreadedProcedure*)space;
    return new (space) newTP(args->mainID, args->toSignal, args->totalTPLoops, args->codeletsPerTP, args->a1, args->a2, args->a3, args->a4, args->a5,
        args->a6, args->a7, args->a8, args->a9, args->a10,
        args->a11, args->a12);
}

template <class newTP, class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11, class arg12, class arg13>
ThreadedProcedure*
TPLoopFactory(void* space, TPLoopClosure* closure)
{
    TPLoopClosure13<arg1, arg2, arg3, arg4, arg5,
        arg6, arg7, arg8, arg9, arg10,
        arg11, arg12, arg13>* args
        = static_cast<TPLoopClosure13<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13>*>(closure);
    myThread.tempParent = (ThreadedProcedure*)space;
    return new (space) newTP(args->mainID, args->toSignal, args->totalTPLoops, args->codeletsPerTP, args->a1, args->a2, args->a3, args->a4, args->a5,
        args->a6, args->a7, args->a8, args->a9, args->a10,
        args->a11, args->a12, args->a13);
}

template <class newTP, class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11, class arg12, class arg13, class arg14>
ThreadedProcedure*
TPLoopFactory(void* space, TPLoopClosure* closure)
{
    TPLoopClosure14<arg1, arg2, arg3, arg4, arg5,
        arg6, arg7, arg8, arg9, arg10,
        arg11, arg12, arg13, arg14>* args
        = static_cast<TPLoopClosure14<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14>*>(closure);
    myThread.tempParent = (ThreadedProcedure*)space;
    return new (space) newTP(args->mainID, args->toSignal, args->totalTPLoops, args->codeletsPerTP, args->a1, args->a2, args->a3, args->a4, args->a5,
        args->a6, args->a7, args->a8, args->a9, args->a10,
        args->a11, args->a12, args->a13, args->a14);
}

template <class newTP, class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11, class arg12, class arg13, class arg14, class arg15>
ThreadedProcedure*
TPLoopFactory(void* space, TPLoopClosure* closure)
{
    TPLoopClosure15<arg1, arg2, arg3, arg4, arg5,
        arg6, arg7, arg8, arg9, arg10,
        arg11, arg12, arg13, arg14, arg15>* args
        = static_cast<TPLoopClosure15<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15>*>(closure);
    myThread.tempParent = (ThreadedProcedure*)space;
    return new (space) newTP(args->mainID, args->toSignal, args->totalTPLoops, args->codeletsPerTP, args->a1, args->a2, args->a3, args->a4, args->a5,
        args->a6, args->a7, args->a8, args->a9, args->a10,
        args->a11, args->a12, args->a13, args->a14, args->a15);
}

template <class newTP, class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11, class arg12, class arg13, class arg14, class arg15,
    class arg16>
ThreadedProcedure*
TPLoopFactory(void* space, TPLoopClosure* closure)
{
    TPLoopClosure16<arg1, arg2, arg3, arg4, arg5,
        arg6, arg7, arg8, arg9, arg10,
        arg11, arg12, arg13, arg14, arg15,
        arg16>* args
        = static_cast<TPLoopClosure16<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16>*>(closure);
    myThread.tempParent = (ThreadedProcedure*)space;
    return new (space) newTP(args->mainID, args->toSignal, args->totalTPLoops, args->codeletsPerTP, args->a1, args->a2, args->a3, args->a4, args->a5,
        args->a6, args->a7, args->a8, args->a9, args->a10,
        args->a11, args->a12, args->a13, args->a14, args->a15,
        args->a16);
}

template <class newTP, class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11, class arg12, class arg13, class arg14, class arg15,
    class arg16, class arg17>
ThreadedProcedure*
TPLoopFactory(void* space, TPLoopClosure* closure)
{
    TPLoopClosure17<arg1, arg2, arg3, arg4, arg5,
        arg6, arg7, arg8, arg9, arg10,
        arg11, arg12, arg13, arg14, arg15,
        arg16, arg17>* args
        = static_cast<TPLoopClosure17<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17>*>(closure);
    myThread.tempParent = (ThreadedProcedure*)space;
    return new (space) newTP(args->mainID, args->toSignal, args->totalTPLoops, args->codeletsPerTP, args->a1, args->a2, args->a3, args->a4, args->a5,
        args->a6, args->a7, args->a8, args->a9, args->a10,
        args->a11, args->a12, args->a13, args->a14, args->a15,
        args->a16, args->a17);
}

template <class newTP, class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11, class arg12, class arg13, class arg14, class arg15,
    class arg16, class arg17, class arg18>
ThreadedProcedure*
TPLoopFactory(void* space, TPLoopClosure* closure)
{
    TPLoopClosure18<arg1, arg2, arg3, arg4, arg5,
        arg6, arg7, arg8, arg9, arg10,
        arg11, arg12, arg13, arg14, arg15,
        arg16, arg17, arg18>* args
        = static_cast<TPLoopClosure18<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18>*>(closure);
    myThread.tempParent = (ThreadedProcedure*)space;
    return new (space) newTP(args->mainID, args->toSignal, args->totalTPLoops, args->codeletsPerTP, args->a1, args->a2, args->a3, args->a4, args->a5,
        args->a6, args->a7, args->a8, args->a9, args->a10,
        args->a11, args->a12, args->a13, args->a14, args->a15,
        args->a16, args->a17, args->a18);
}

template <class newTP, class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11, class arg12, class arg13, class arg14, class arg15,
    class arg16, class arg17, class arg18, class arg19>
ThreadedProcedure*
TPLoopFactory(void* space, TPLoopClosure* closure)
{
    TPLoopClosure19<arg1, arg2, arg3, arg4, arg5,
        arg6, arg7, arg8, arg9, arg10,
        arg11, arg12, arg13, arg14, arg15,
        arg16, arg17, arg18, arg19>* args
        = static_cast<TPLoopClosure19<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19>*>(closure);
    myThread.tempParent = (ThreadedProcedure*)space;
    return new (space) newTP(args->mainID, args->toSignal, args->totalTPLoops, args->codeletsPerTP, args->a1, args->a2, args->a3, args->a4, args->a5,
        args->a6, args->a7, args->a8, args->a9, args->a10,
        args->a11, args->a12, args->a13, args->a14, args->a15,
        args->a16, args->a17, args->a18, args->a19);
}

template <class newTP, class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11, class arg12, class arg13, class arg14, class arg15,
    class arg16, class arg17, class arg18, class arg19, class arg20>
ThreadedProcedure*
TPLoopFactory(void* space, TPLoopClosure* closure)
{
    TPLoopClosure20<arg1, arg2, arg3, arg4, arg5,
        arg6, arg7, arg8, arg9, arg10,
        arg11, arg12, arg13, arg14, arg15,
        arg16, arg17, arg18, arg19, arg20>* args
        = static_cast<TPLoopClosure20<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20>*>(closure);
    myThread.tempParent = (ThreadedProcedure*)space;
    return new (space) newTP(args->mainID, args->toSignal, args->totalTPLoops, args->codeletsPerTP, args->a1, args->a2, args->a3, args->a4, args->a5,
        args->a6, args->a7, args->a8, args->a9, args->a10,
        args->a11, args->a12, args->a13, args->a14, args->a15,
        args->a16, args->a17, args->a18, args->a19, args->a20);
}

template <class newTP, class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11, class arg12, class arg13, class arg14, class arg15,
    class arg16, class arg17, class arg18, class arg19, class arg20,
    class arg21>
ThreadedProcedure*
TPLoopFactory(void* space, TPLoopClosure* closure)
{
    TPLoopClosure21<arg1, arg2, arg3, arg4, arg5,
        arg6, arg7, arg8, arg9, arg10,
        arg11, arg12, arg13, arg14, arg15,
        arg16, arg17, arg18, arg19, arg20,
        arg21>* args
        = static_cast<TPLoopClosure21<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21>*>(closure);
    myThread.tempParent = (ThreadedProcedure*)space;
    return new (space) newTP(args->mainID, args->toSignal, args->totalTPLoops, args->codeletsPerTP, args->a1, args->a2, args->a3, args->a4, args->a5,
        args->a6, args->a7, args->a8, args->a9, args->a10,
        args->a11, args->a12, args->a13, args->a14, args->a15,
        args->a16, args->a17, args->a18, args->a19, args->a20,
        args->a21);
}

template <class newTP, class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11, class arg12, class arg13, class arg14, class arg15,
    class arg16, class arg17, class arg18, class arg19, class arg20,
    class arg21, class arg22>
ThreadedProcedure*
TPLoopFactory(void* space, TPLoopClosure* closure)
{
    TPLoopClosure22<arg1, arg2, arg3, arg4, arg5,
        arg6, arg7, arg8, arg9, arg10,
        arg11, arg12, arg13, arg14, arg15,
        arg16, arg17, arg18, arg19, arg20,
        arg21, arg22>* args
        = static_cast<TPLoopClosure22<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22>*>(closure);
    myThread.tempParent = (ThreadedProcedure*)space;
    return new (space) newTP(args->mainID, args->toSignal, args->totalTPLoops, args->codeletsPerTP, args->a1, args->a2, args->a3, args->a4, args->a5,
        args->a6, args->a7, args->a8, args->a9, args->a10,
        args->a11, args->a12, args->a13, args->a14, args->a15,
        args->a16, args->a17, args->a18, args->a19, args->a20,
        args->a21, args->a22);
}

template <class newTP, class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11, class arg12, class arg13, class arg14, class arg15,
    class arg16, class arg17, class arg18, class arg19, class arg20,
    class arg21, class arg22, class arg23>
ThreadedProcedure*
TPLoopFactory(void* space, TPLoopClosure* closure)
{
    TPLoopClosure23<arg1, arg2, arg3, arg4, arg5,
        arg6, arg7, arg8, arg9, arg10,
        arg11, arg12, arg13, arg14, arg15,
        arg16, arg17, arg18, arg19, arg20,
        arg21, arg22, arg23>* args
        = static_cast<TPLoopClosure23<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23>*>(closure);
    myThread.tempParent = (ThreadedProcedure*)space;
    return new (space) newTP(args->mainID, args->toSignal, args->totalTPLoops, args->codeletsPerTP, args->a1, args->a2, args->a3, args->a4, args->a5,
        args->a6, args->a7, args->a8, args->a9, args->a10,
        args->a11, args->a12, args->a13, args->a14, args->a15,
        args->a16, args->a17, args->a18, args->a19, args->a20,
        args->a21, args->a22, args->a23);
}

template <class newTP, class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11, class arg12, class arg13, class arg14, class arg15,
    class arg16, class arg17, class arg18, class arg19, class arg20,
    class arg21, class arg22, class arg23, class arg24>
ThreadedProcedure*
TPLoopFactory(void* space, TPLoopClosure* closure)
{
    TPLoopClosure24<arg1, arg2, arg3, arg4, arg5,
        arg6, arg7, arg8, arg9, arg10,
        arg11, arg12, arg13, arg14, arg15,
        arg16, arg17, arg18, arg19, arg20,
        arg21, arg22, arg23, arg24>* args
        = static_cast<TPLoopClosure24<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24>*>(closure);
    myThread.tempParent = (ThreadedProcedure*)space;
    return new (space) newTP(args->mainID, args->toSignal, args->totalTPLoops, args->codeletsPerTP, args->a1, args->a2, args->a3, args->a4, args->a5,
        args->a6, args->a7, args->a8, args->a9, args->a10,
        args->a11, args->a12, args->a13, args->a14, args->a15,
        args->a16, args->a17, args->a18, args->a19, args->a20,
        args->a21, args->a22, args->a23, args->a24);
}

template <class newTP, class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11, class arg12, class arg13, class arg14, class arg15,
    class arg16, class arg17, class arg18, class arg19, class arg20,
    class arg21, class arg22, class arg23, class arg24, class arg25>
ThreadedProcedure*
TPLoopFactory(void* space, TPLoopClosure* closure)
{
    TPLoopClosure25<arg1, arg2, arg3, arg4, arg5,
        arg6, arg7, arg8, arg9, arg10,
        arg11, arg12, arg13, arg14, arg15,
        arg16, arg17, arg18, arg19, arg20,
        arg21, arg22, arg23, arg24, arg25>* args
        = static_cast<TPLoopClosure25<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25>*>(closure);
    myThread.tempParent = (ThreadedProcedure*)space;
    return new (space) newTP(args->mainID, args->toSignal, args->totalTPLoops, args->codeletsPerTP, args->a1, args->a2, args->a3, args->a4, args->a5,
        args->a6, args->a7, args->a8, args->a9, args->a10,
        args->a11, args->a12, args->a13, args->a14, args->a15,
        args->a16, args->a17, args->a18, args->a19, args->a20,
        args->a21, args->a22, args->a23, args->a24, args->a25);
}

template <class newTP, class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11, class arg12, class arg13, class arg14, class arg15,
    class arg16, class arg17, class arg18, class arg19, class arg20,
    class arg21, class arg22, class arg23, class arg24, class arg25,
    class arg26>
ThreadedProcedure*
TPLoopFactory(void* space, TPLoopClosure* closure)
{
    TPLoopClosure26<arg1, arg2, arg3, arg4, arg5,
        arg6, arg7, arg8, arg9, arg10,
        arg11, arg12, arg13, arg14, arg15,
        arg16, arg17, arg18, arg19, arg20,
        arg21, arg22, arg23, arg24, arg25,
        arg26>* args
        = static_cast<TPLoopClosure26<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25,
            arg26>*>(closure);
    myThread.tempParent = (ThreadedProcedure*)space;
    return new (space) newTP(args->mainID, args->toSignal, args->totalTPLoops, args->codeletsPerTP, args->a1, args->a2, args->a3, args->a4, args->a5,
        args->a6, args->a7, args->a8, args->a9, args->a10,
        args->a11, args->a12, args->a13, args->a14, args->a15,
        args->a16, args->a17, args->a18, args->a19, args->a20,
        args->a21, args->a22, args->a23, args->a24, args->a25,
        args->a26);
}

template <class newTP, class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11, class arg12, class arg13, class arg14, class arg15,
    class arg16, class arg17, class arg18, class arg19, class arg20,
    class arg21, class arg22, class arg23, class arg24, class arg25,
    class arg26, class arg27>
ThreadedProcedure*
TPLoopFactory(void* space, TPLoopClosure* closure)
{
    TPLoopClosure27<arg1, arg2, arg3, arg4, arg5,
        arg6, arg7, arg8, arg9, arg10,
        arg11, arg12, arg13, arg14, arg15,
        arg16, arg17, arg18, arg19, arg20,
        arg21, arg22, arg23, arg24, arg25,
        arg26, arg27>* args
        = static_cast<TPLoopClosure27<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25,
            arg26, arg27>*>(closure);
    myThread.tempParent = (ThreadedProcedure*)space;
    return new (space) newTP(args->mainID, args->toSignal, args->totalTPLoops, args->codeletsPerTP, args->a1, args->a2, args->a3, args->a4, args->a5,
        args->a6, args->a7, args->a8, args->a9, args->a10,
        args->a11, args->a12, args->a13, args->a14, args->a15,
        args->a16, args->a17, args->a18, args->a19, args->a20,
        args->a21, args->a22, args->a23, args->a24, args->a25,
        args->a26, args->a27);
}

template <class newTP, class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11, class arg12, class arg13, class arg14, class arg15,
    class arg16, class arg17, class arg18, class arg19, class arg20,
    class arg21, class arg22, class arg23, class arg24, class arg25,
    class arg26, class arg27, class arg28>
ThreadedProcedure*
TPLoopFactory(void* space, TPLoopClosure* closure)
{
    TPLoopClosure28<arg1, arg2, arg3, arg4, arg5,
        arg6, arg7, arg8, arg9, arg10,
        arg11, arg12, arg13, arg14, arg15,
        arg16, arg17, arg18, arg19, arg20,
        arg21, arg22, arg23, arg24, arg25,
        arg26, arg27, arg28>* args
        = static_cast<TPLoopClosure28<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25,
            arg26, arg27, arg28>*>(closure);
    myThread.tempParent = (ThreadedProcedure*)space;
    return new (space) newTP(args->mainID, args->toSignal, args->totalTPLoops, args->codeletsPerTP, args->a1, args->a2, args->a3, args->a4, args->a5,
        args->a6, args->a7, args->a8, args->a9, args->a10,
        args->a11, args->a12, args->a13, args->a14, args->a15,
        args->a16, args->a17, args->a18, args->a19, args->a20,
        args->a21, args->a22, args->a23, args->a24, args->a25,
        args->a26, args->a27, args->a28);
}

template <class newTP, class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11, class arg12, class arg13, class arg14, class arg15,
    class arg16, class arg17, class arg18, class arg19, class arg20,
    class arg21, class arg22, class arg23, class arg24, class arg25,
    class arg26, class arg27, class arg28, class arg29>
ThreadedProcedure*
TPLoopFactory(void* space, TPLoopClosure* closure)
{
    TPLoopClosure29<arg1, arg2, arg3, arg4, arg5,
        arg6, arg7, arg8, arg9, arg10,
        arg11, arg12, arg13, arg14, arg15,
        arg16, arg17, arg18, arg19, arg20,
        arg21, arg22, arg23, arg24, arg25,
        arg26, arg27, arg28, arg29>* args
        = static_cast<TPLoopClosure29<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25,
            arg26, arg27, arg28, arg29>*>(closure);
    myThread.tempParent = (ThreadedProcedure*)space;
    return new (space) newTP(args->mainID, args->toSignal, args->totalTPLoops, args->codeletsPerTP, args->a1, args->a2, args->a3, args->a4, args->a5,
        args->a6, args->a7, args->a8, args->a9, args->a10,
        args->a11, args->a12, args->a13, args->a14, args->a15,
        args->a16, args->a17, args->a18, args->a19, args->a20,
        args->a21, args->a22, args->a23, args->a24, args->a25,
        args->a26, args->a27, args->a28, args->a29);
}

template <class newTP, class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11, class arg12, class arg13, class arg14, class arg15,
    class arg16, class arg17, class arg18, class arg19, class arg20,
    class arg21, class arg22, class arg23, class arg24, class arg25,
    class arg26, class arg27, class arg28, class arg29, class arg30>
ThreadedProcedure*
TPLoopFactory(void* space, TPLoopClosure* closure)
{
    TPLoopClosure30<arg1, arg2, arg3, arg4, arg5,
        arg6, arg7, arg8, arg9, arg10,
        arg11, arg12, arg13, arg14, arg15,
        arg16, arg17, arg18, arg19, arg20,
        arg21, arg22, arg23, arg24, arg25,
        arg26, arg27, arg28, arg29, arg30>* args
        = static_cast<TPLoopClosure30<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25,
            arg26, arg27, arg28, arg29, arg30>*>(closure);
    myThread.tempParent = (ThreadedProcedure*)space;
    return new (space) newTP(args->mainID, args->toSignal, args->totalTPLoops, args->codeletsPerTP, args->a1, args->a2, args->a3, args->a4, args->a5,
        args->a6, args->a7, args->a8, args->a9, args->a10,
        args->a11, args->a12, args->a13, args->a14, args->a15,
        args->a16, args->a17, args->a18, args->a19, args->a20,
        args->a21, args->a22, args->a23, args->a24, args->a25,
        args->a26, args->a27, args->a28, args->a29, args->a30);
}

template <class newTP, class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11, class arg12, class arg13, class arg14, class arg15,
    class arg16, class arg17, class arg18, class arg19, class arg20,
    class arg21, class arg22, class arg23, class arg24, class arg25,
    class arg26, class arg27, class arg28, class arg29, class arg30,
    class arg31>
ThreadedProcedure*
TPLoopFactory(void* space, TPLoopClosure* closure)
{
    TPLoopClosure31<arg1, arg2, arg3, arg4, arg5,
        arg6, arg7, arg8, arg9, arg10,
        arg11, arg12, arg13, arg14, arg15,
        arg16, arg17, arg18, arg19, arg20,
        arg21, arg22, arg23, arg24, arg25,
        arg26, arg27, arg28, arg29, arg30,
        arg31>* args
        = static_cast<TPLoopClosure31<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25,
            arg26, arg27, arg28, arg29, arg30,
            arg31>*>(closure);
    myThread.tempParent = (ThreadedProcedure*)space;
    return new (space) newTP(args->mainID, args->toSignal, args->totalTPLoops, args->codeletsPerTP, args->a1, args->a2, args->a3, args->a4, args->a5,
        args->a6, args->a7, args->a8, args->a9, args->a10,
        args->a11, args->a12, args->a13, args->a14, args->a15,
        args->a16, args->a17, args->a18, args->a19, args->a20,
        args->a21, args->a22, args->a23, args->a24, args->a25,
        args->a26, args->a27, args->a28, args->a29, args->a30,
        args->a31);
}

template <class newTP, class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11, class arg12, class arg13, class arg14, class arg15,
    class arg16, class arg17, class arg18, class arg19, class arg20,
    class arg21, class arg22, class arg23, class arg24, class arg25,
    class arg26, class arg27, class arg28, class arg29, class arg30,
    class arg31, class arg32>
ThreadedProcedure*
TPLoopFactory(void* space, TPLoopClosure* closure)
{
    TPLoopClosure32<arg1, arg2, arg3, arg4, arg5,
        arg6, arg7, arg8, arg9, arg10,
        arg11, arg12, arg13, arg14, arg15,
        arg16, arg17, arg18, arg19, arg20,
        arg21, arg22, arg23, arg24, arg25,
        arg26, arg27, arg28, arg29, arg30,
        arg31, arg32>* args
        = static_cast<TPLoopClosure32<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25,
            arg26, arg27, arg28, arg29, arg30,
            arg31, arg32>*>(closure);
    myThread.tempParent = (ThreadedProcedure*)space;
    return new (space) newTP(args->mainID, args->toSignal, args->totalTPLoops, args->codeletsPerTP, args->a1, args->a2, args->a3, args->a4, args->a5,
        args->a6, args->a7, args->a8, args->a9, args->a10,
        args->a11, args->a12, args->a13, args->a14, args->a15,
        args->a16, args->a17, args->a18, args->a19, args->a20,
        args->a21, args->a22, args->a23, args->a24, args->a25,
        args->a26, args->a27, args->a28, args->a29, args->a30,
        args->a31, args->a32);
}
}

#endif /* DOLOOP_H */
