#ifndef DOLOOP_H
#define DOLOOP_H
#include "loopClosure.h"
#include "threadlocal.h"

namespace darts {

template <class newTP>
ThreadedProcedure*
LPFactory(void* space, lpClosure* args)
{
    myThread.tempParent = (ThreadedProcedure*)space;
    return new (space) newTP(args->iter, args->toSignal);
}

template <class newTP, class arg1>
ThreadedProcedure*
LPFactory(void* space, lpClosure* closure)
{
    lpClosure1<arg1>* args = static_cast<lpClosure1<arg1>*>(closure);
    myThread.tempParent = (ThreadedProcedure*)space;
    return new (space) newTP(args->iter, args->toSignal, args->a1);
}

template <class newTP, class arg1, class arg2>
ThreadedProcedure*
LPFactory(void* space, lpClosure* closure)
{
    lpClosure2<arg1, arg2>* args = static_cast<lpClosure2<arg1, arg2>*>(closure);
    myThread.tempParent = (ThreadedProcedure*)space;
    return new (space) newTP(args->iter, args->toSignal, args->a1, args->a2);
}

template <class newTP, class arg1, class arg2, class arg3>
ThreadedProcedure*
LPFactory(void* space, lpClosure* closure)
{
    lpClosure3<arg1, arg2, arg3>* args = static_cast<lpClosure3<arg1, arg2, arg3>*>(closure);
    myThread.tempParent = (ThreadedProcedure*)space;
    return new (space) newTP(args->iter, args->toSignal, args->a1, args->a2, args->a3);
}

template <class newTP, class arg1, class arg2, class arg3, class arg4>
ThreadedProcedure*
LPFactory(void* space, lpClosure* closure)
{
    lpClosure4<arg1, arg2, arg3, arg4>* args = static_cast<lpClosure4<arg1, arg2, arg3, arg4>*>(closure);
    myThread.tempParent = (ThreadedProcedure*)space;
    return new (space) newTP(args->iter, args->toSignal, args->a1, args->a2, args->a3, args->a4);
}

template <class newTP, class arg1, class arg2, class arg3, class arg4, class arg5>
ThreadedProcedure*
LPFactory(void* space, lpClosure* closure)
{
    lpClosure5<arg1, arg2, arg3, arg4, arg5>* args = static_cast<lpClosure5<arg1, arg2, arg3, arg4, arg5>*>(closure);
    myThread.tempParent = (ThreadedProcedure*)space;
    return new (space) newTP(args->iter, args->toSignal, args->a1, args->a2, args->a3, args->a4, args->a5);
}

template <class newTP, class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6>
ThreadedProcedure*
LPFactory(void* space, lpClosure* closure)
{
    lpClosure6<arg1, arg2, arg3, arg4, arg5,
        arg6>* args
        = static_cast<lpClosure6<arg1, arg2, arg3, arg4, arg5,
            arg6>*>(closure);
    myThread.tempParent = (ThreadedProcedure*)space;
    return new (space) newTP(args->iter, args->toSignal, args->a1, args->a2, args->a3, args->a4, args->a5,
        args->a6);
}

template <class newTP, class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7>
ThreadedProcedure*
LPFactory(void* space, lpClosure* closure)
{
    lpClosure7<arg1, arg2, arg3, arg4, arg5,
        arg6, arg7>* args
        = static_cast<lpClosure7<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7>*>(closure);
    myThread.tempParent = (ThreadedProcedure*)space;
    return new (space) newTP(args->iter, args->toSignal, args->a1, args->a2, args->a3, args->a4, args->a5,
        args->a6, args->a7);
}

template <class newTP, class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8>
ThreadedProcedure*
LPFactory(void* space, lpClosure* closure)
{
    lpClosure8<arg1, arg2, arg3, arg4, arg5,
        arg6, arg7, arg8>* args
        = static_cast<lpClosure8<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8>*>(closure);
    myThread.tempParent = (ThreadedProcedure*)space;
    return new (space) newTP(args->iter, args->toSignal, args->a1, args->a2, args->a3, args->a4, args->a5,
        args->a6, args->a7, args->a8);
}

template <class newTP, class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9>
ThreadedProcedure*
LPFactory(void* space, lpClosure* closure)
{
    lpClosure9<arg1, arg2, arg3, arg4, arg5,
        arg6, arg7, arg8, arg9>* args
        = static_cast<lpClosure9<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9>*>(closure);
    myThread.tempParent = (ThreadedProcedure*)space;
    return new (space) newTP(args->iter, args->toSignal, args->a1, args->a2, args->a3, args->a4, args->a5,
        args->a6, args->a7, args->a8, args->a9);
}

template <class newTP, class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10>
ThreadedProcedure*
LPFactory(void* space, lpClosure* closure)
{
    lpClosure10<arg1, arg2, arg3, arg4, arg5,
        arg6, arg7, arg8, arg9, arg10>* args
        = static_cast<lpClosure10<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10>*>(closure);
    myThread.tempParent = (ThreadedProcedure*)space;
    return new (space) newTP(args->iter, args->toSignal, args->a1, args->a2, args->a3, args->a4, args->a5,
        args->a6, args->a7, args->a8, args->a9, args->a10);
}

template <class newTP, class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11>
ThreadedProcedure*
LPFactory(void* space, lpClosure* closure)
{
    lpClosure11<arg1, arg2, arg3, arg4, arg5,
        arg6, arg7, arg8, arg9, arg10,
        arg11>* args
        = static_cast<lpClosure11<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11>*>(closure);
    myThread.tempParent = (ThreadedProcedure*)space;
    return new (space) newTP(args->iter, args->toSignal, args->a1, args->a2, args->a3, args->a4, args->a5,
        args->a6, args->a7, args->a8, args->a9, args->a10,
        args->a11);
}

template <class newTP, class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11, class arg12>
ThreadedProcedure*
LPFactory(void* space, lpClosure* closure)
{
    lpClosure12<arg1, arg2, arg3, arg4, arg5,
        arg6, arg7, arg8, arg9, arg10,
        arg11, arg12>* args
        = static_cast<lpClosure12<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12>*>(closure);
    myThread.tempParent = (ThreadedProcedure*)space;
    return new (space) newTP(args->iter, args->toSignal, args->a1, args->a2, args->a3, args->a4, args->a5,
        args->a6, args->a7, args->a8, args->a9, args->a10,
        args->a11, args->a12);
}

template <class newTP, class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11, class arg12, class arg13>
ThreadedProcedure*
LPFactory(void* space, lpClosure* closure)
{
    lpClosure13<arg1, arg2, arg3, arg4, arg5,
        arg6, arg7, arg8, arg9, arg10,
        arg11, arg12, arg13>* args
        = static_cast<lpClosure13<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13>*>(closure);
    myThread.tempParent = (ThreadedProcedure*)space;
    return new (space) newTP(args->iter, args->toSignal, args->a1, args->a2, args->a3, args->a4, args->a5,
        args->a6, args->a7, args->a8, args->a9, args->a10,
        args->a11, args->a12, args->a13);
}

template <class newTP, class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11, class arg12, class arg13, class arg14>
ThreadedProcedure*
LPFactory(void* space, lpClosure* closure)
{
    lpClosure14<arg1, arg2, arg3, arg4, arg5,
        arg6, arg7, arg8, arg9, arg10,
        arg11, arg12, arg13, arg14>* args
        = static_cast<lpClosure14<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14>*>(closure);
    myThread.tempParent = (ThreadedProcedure*)space;
    return new (space) newTP(args->iter, args->toSignal, args->a1, args->a2, args->a3, args->a4, args->a5,
        args->a6, args->a7, args->a8, args->a9, args->a10,
        args->a11, args->a12, args->a13, args->a14);
}

template <class newTP, class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11, class arg12, class arg13, class arg14, class arg15>
ThreadedProcedure*
LPFactory(void* space, lpClosure* closure)
{
    lpClosure15<arg1, arg2, arg3, arg4, arg5,
        arg6, arg7, arg8, arg9, arg10,
        arg11, arg12, arg13, arg14, arg15>* args
        = static_cast<lpClosure15<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15>*>(closure);
    myThread.tempParent = (ThreadedProcedure*)space;
    return new (space) newTP(args->iter, args->toSignal, args->a1, args->a2, args->a3, args->a4, args->a5,
        args->a6, args->a7, args->a8, args->a9, args->a10,
        args->a11, args->a12, args->a13, args->a14, args->a15);
}

template <class newTP, class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11, class arg12, class arg13, class arg14, class arg15,
    class arg16>
ThreadedProcedure*
LPFactory(void* space, lpClosure* closure)
{
    lpClosure16<arg1, arg2, arg3, arg4, arg5,
        arg6, arg7, arg8, arg9, arg10,
        arg11, arg12, arg13, arg14, arg15,
        arg16>* args
        = static_cast<lpClosure16<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16>*>(closure);
    myThread.tempParent = (ThreadedProcedure*)space;
    return new (space) newTP(args->iter, args->toSignal, args->a1, args->a2, args->a3, args->a4, args->a5,
        args->a6, args->a7, args->a8, args->a9, args->a10,
        args->a11, args->a12, args->a13, args->a14, args->a15,
        args->a16);
}

template <class newTP, class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11, class arg12, class arg13, class arg14, class arg15,
    class arg16, class arg17>
ThreadedProcedure*
LPFactory(void* space, lpClosure* closure)
{
    lpClosure17<arg1, arg2, arg3, arg4, arg5,
        arg6, arg7, arg8, arg9, arg10,
        arg11, arg12, arg13, arg14, arg15,
        arg16, arg17>* args
        = static_cast<lpClosure17<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17>*>(closure);
    myThread.tempParent = (ThreadedProcedure*)space;
    return new (space) newTP(args->iter, args->toSignal, args->a1, args->a2, args->a3, args->a4, args->a5,
        args->a6, args->a7, args->a8, args->a9, args->a10,
        args->a11, args->a12, args->a13, args->a14, args->a15,
        args->a16, args->a17);
}

template <class newTP, class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11, class arg12, class arg13, class arg14, class arg15,
    class arg16, class arg17, class arg18>
ThreadedProcedure*
LPFactory(void* space, lpClosure* closure)
{
    lpClosure18<arg1, arg2, arg3, arg4, arg5,
        arg6, arg7, arg8, arg9, arg10,
        arg11, arg12, arg13, arg14, arg15,
        arg16, arg17, arg18>* args
        = static_cast<lpClosure18<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18>*>(closure);
    myThread.tempParent = (ThreadedProcedure*)space;
    return new (space) newTP(args->iter, args->toSignal, args->a1, args->a2, args->a3, args->a4, args->a5,
        args->a6, args->a7, args->a8, args->a9, args->a10,
        args->a11, args->a12, args->a13, args->a14, args->a15,
        args->a16, args->a17, args->a18);
}

template <class newTP, class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11, class arg12, class arg13, class arg14, class arg15,
    class arg16, class arg17, class arg18, class arg19>
ThreadedProcedure*
LPFactory(void* space, lpClosure* closure)
{
    lpClosure19<arg1, arg2, arg3, arg4, arg5,
        arg6, arg7, arg8, arg9, arg10,
        arg11, arg12, arg13, arg14, arg15,
        arg16, arg17, arg18, arg19>* args
        = static_cast<lpClosure19<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19>*>(closure);
    myThread.tempParent = (ThreadedProcedure*)space;
    return new (space) newTP(args->iter, args->toSignal, args->a1, args->a2, args->a3, args->a4, args->a5,
        args->a6, args->a7, args->a8, args->a9, args->a10,
        args->a11, args->a12, args->a13, args->a14, args->a15,
        args->a16, args->a17, args->a18, args->a19);
}

template <class newTP, class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11, class arg12, class arg13, class arg14, class arg15,
    class arg16, class arg17, class arg18, class arg19, class arg20>
ThreadedProcedure*
LPFactory(void* space, lpClosure* closure)
{
    lpClosure20<arg1, arg2, arg3, arg4, arg5,
        arg6, arg7, arg8, arg9, arg10,
        arg11, arg12, arg13, arg14, arg15,
        arg16, arg17, arg18, arg19, arg20>* args
        = static_cast<lpClosure20<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20>*>(closure);
    myThread.tempParent = (ThreadedProcedure*)space;
    return new (space) newTP(args->iter, args->toSignal, args->a1, args->a2, args->a3, args->a4, args->a5,
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
LPFactory(void* space, lpClosure* closure)
{
    lpClosure21<arg1, arg2, arg3, arg4, arg5,
        arg6, arg7, arg8, arg9, arg10,
        arg11, arg12, arg13, arg14, arg15,
        arg16, arg17, arg18, arg19, arg20,
        arg21>* args
        = static_cast<lpClosure21<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21>*>(closure);
    myThread.tempParent = (ThreadedProcedure*)space;
    return new (space) newTP(args->iter, args->toSignal, args->a1, args->a2, args->a3, args->a4, args->a5,
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
LPFactory(void* space, lpClosure* closure)
{
    lpClosure22<arg1, arg2, arg3, arg4, arg5,
        arg6, arg7, arg8, arg9, arg10,
        arg11, arg12, arg13, arg14, arg15,
        arg16, arg17, arg18, arg19, arg20,
        arg21, arg22>* args
        = static_cast<lpClosure22<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22>*>(closure);
    myThread.tempParent = (ThreadedProcedure*)space;
    return new (space) newTP(args->iter, args->toSignal, args->a1, args->a2, args->a3, args->a4, args->a5,
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
LPFactory(void* space, lpClosure* closure)
{
    lpClosure23<arg1, arg2, arg3, arg4, arg5,
        arg6, arg7, arg8, arg9, arg10,
        arg11, arg12, arg13, arg14, arg15,
        arg16, arg17, arg18, arg19, arg20,
        arg21, arg22, arg23>* args
        = static_cast<lpClosure23<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23>*>(closure);
    myThread.tempParent = (ThreadedProcedure*)space;
    return new (space) newTP(args->iter, args->toSignal, args->a1, args->a2, args->a3, args->a4, args->a5,
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
LPFactory(void* space, lpClosure* closure)
{
    lpClosure24<arg1, arg2, arg3, arg4, arg5,
        arg6, arg7, arg8, arg9, arg10,
        arg11, arg12, arg13, arg14, arg15,
        arg16, arg17, arg18, arg19, arg20,
        arg21, arg22, arg23, arg24>* args
        = static_cast<lpClosure24<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24>*>(closure);
    myThread.tempParent = (ThreadedProcedure*)space;
    return new (space) newTP(args->iter, args->toSignal, args->a1, args->a2, args->a3, args->a4, args->a5,
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
LPFactory(void* space, lpClosure* closure)
{
    lpClosure25<arg1, arg2, arg3, arg4, arg5,
        arg6, arg7, arg8, arg9, arg10,
        arg11, arg12, arg13, arg14, arg15,
        arg16, arg17, arg18, arg19, arg20,
        arg21, arg22, arg23, arg24, arg25>* args
        = static_cast<lpClosure25<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25>*>(closure);
    myThread.tempParent = (ThreadedProcedure*)space;
    return new (space) newTP(args->iter, args->toSignal, args->a1, args->a2, args->a3, args->a4, args->a5,
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
LPFactory(void* space, lpClosure* closure)
{
    lpClosure26<arg1, arg2, arg3, arg4, arg5,
        arg6, arg7, arg8, arg9, arg10,
        arg11, arg12, arg13, arg14, arg15,
        arg16, arg17, arg18, arg19, arg20,
        arg21, arg22, arg23, arg24, arg25,
        arg26>* args
        = static_cast<lpClosure26<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25,
            arg26>*>(closure);
    myThread.tempParent = (ThreadedProcedure*)space;
    return new (space) newTP(args->iter, args->toSignal, args->a1, args->a2, args->a3, args->a4, args->a5,
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
LPFactory(void* space, lpClosure* closure)
{
    lpClosure27<arg1, arg2, arg3, arg4, arg5,
        arg6, arg7, arg8, arg9, arg10,
        arg11, arg12, arg13, arg14, arg15,
        arg16, arg17, arg18, arg19, arg20,
        arg21, arg22, arg23, arg24, arg25,
        arg26, arg27>* args
        = static_cast<lpClosure27<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25,
            arg26, arg27>*>(closure);
    myThread.tempParent = (ThreadedProcedure*)space;
    return new (space) newTP(args->iter, args->toSignal, args->a1, args->a2, args->a3, args->a4, args->a5,
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
LPFactory(void* space, lpClosure* closure)
{
    lpClosure28<arg1, arg2, arg3, arg4, arg5,
        arg6, arg7, arg8, arg9, arg10,
        arg11, arg12, arg13, arg14, arg15,
        arg16, arg17, arg18, arg19, arg20,
        arg21, arg22, arg23, arg24, arg25,
        arg26, arg27, arg28>* args
        = static_cast<lpClosure28<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25,
            arg26, arg27, arg28>*>(closure);
    myThread.tempParent = (ThreadedProcedure*)space;
    return new (space) newTP(args->iter, args->toSignal, args->a1, args->a2, args->a3, args->a4, args->a5,
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
LPFactory(void* space, lpClosure* closure)
{
    lpClosure29<arg1, arg2, arg3, arg4, arg5,
        arg6, arg7, arg8, arg9, arg10,
        arg11, arg12, arg13, arg14, arg15,
        arg16, arg17, arg18, arg19, arg20,
        arg21, arg22, arg23, arg24, arg25,
        arg26, arg27, arg28, arg29>* args
        = static_cast<lpClosure29<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25,
            arg26, arg27, arg28, arg29>*>(closure);
    myThread.tempParent = (ThreadedProcedure*)space;
    return new (space) newTP(args->iter, args->toSignal, args->a1, args->a2, args->a3, args->a4, args->a5,
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
LPFactory(void* space, lpClosure* closure)
{
    lpClosure30<arg1, arg2, arg3, arg4, arg5,
        arg6, arg7, arg8, arg9, arg10,
        arg11, arg12, arg13, arg14, arg15,
        arg16, arg17, arg18, arg19, arg20,
        arg21, arg22, arg23, arg24, arg25,
        arg26, arg27, arg28, arg29, arg30>* args
        = static_cast<lpClosure30<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25,
            arg26, arg27, arg28, arg29, arg30>*>(closure);
    myThread.tempParent = (ThreadedProcedure*)space;
    return new (space) newTP(args->iter, args->toSignal, args->a1, args->a2, args->a3, args->a4, args->a5,
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
LPFactory(void* space, lpClosure* closure)
{
    lpClosure31<arg1, arg2, arg3, arg4, arg5,
        arg6, arg7, arg8, arg9, arg10,
        arg11, arg12, arg13, arg14, arg15,
        arg16, arg17, arg18, arg19, arg20,
        arg21, arg22, arg23, arg24, arg25,
        arg26, arg27, arg28, arg29, arg30,
        arg31>* args
        = static_cast<lpClosure31<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25,
            arg26, arg27, arg28, arg29, arg30,
            arg31>*>(closure);
    myThread.tempParent = (ThreadedProcedure*)space;
    return new (space) newTP(args->iter, args->toSignal, args->a1, args->a2, args->a3, args->a4, args->a5,
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
LPFactory(void* space, lpClosure* closure)
{
    lpClosure32<arg1, arg2, arg3, arg4, arg5,
        arg6, arg7, arg8, arg9, arg10,
        arg11, arg12, arg13, arg14, arg15,
        arg16, arg17, arg18, arg19, arg20,
        arg21, arg22, arg23, arg24, arg25,
        arg26, arg27, arg28, arg29, arg30,
        arg31, arg32>* args
        = static_cast<lpClosure32<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25,
            arg26, arg27, arg28, arg29, arg30,
            arg31, arg32>*>(closure);
    myThread.tempParent = (ThreadedProcedure*)space;
    return new (space) newTP(args->iter, args->toSignal, args->a1, args->a2, args->a3, args->a4, args->a5,
        args->a6, args->a7, args->a8, args->a9, args->a10,
        args->a11, args->a12, args->a13, args->a14, args->a15,
        args->a16, args->a17, args->a18, args->a19, args->a20,
        args->a21, args->a22, args->a23, args->a24, args->a25,
        args->a26, args->a27, args->a28, args->a29, args->a30,
        args->a31, args->a32);
}
}

#endif /* DOLOOP_H */
