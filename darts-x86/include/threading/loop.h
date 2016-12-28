#ifndef LOOP_H
#define LOOP_H
#include "Codelet.h"
#include "TPLoopClosure.h"
#include "ThreadedProcedure.h"
#include "codeletDefines.h"
#include "doLoop.h"
#include "doTP.h"
#include "doTPLoop.h"
#include "loopClosure.h"
#include "threadlocal.h"
#include "tpClosure.h"
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <string.h>
namespace darts {
class loop : public ThreadedProcedure {
public:
    Codelet* toSignal;
    unsigned int iter;

    loop(Codelet* toSig)
        : toSignal(toSig)
    {
    }

    loop(unsigned int it, Codelet* toSig)
        : toSignal(toSig)
        , iter(it)
    {
    }
};

template <class LP>
class paraFor : public Codelet {
private:
    uint32_t Dep;
    uint32_t Res;
    uint32_t Stat;
    bool fired;
    unsigned int iterations_;
    Codelet* toSignal_;
    tpClosure* closure;
    unsigned int* itPtr;

public:
    ~paraFor(void)
    {
        delete closure;
    }

    void setIterations(unsigned int it) { iterations_ = it; }

    paraFor()
        : Codelet()
    {
    }

    paraFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it)
        : Codelet(dep, res * it, myTP, stat)
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , fired(false)
        , iterations_(it)
        , toSignal_(toSig)
    {
        tpfactory funct = &TPFactory<LP, unsigned int, Codelet*>;
        tpClosure2<unsigned int, Codelet*>* temp = new tpClosure2<unsigned int, Codelet*>(funct, myTP, 0, this);
        itPtr = &temp->a1;
        closure = temp;
    }

    template <class arg1>
    paraFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1)
        : Codelet(dep, res * it, myTP, stat)
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , fired(false)
        , iterations_(it)
        , toSignal_(toSig)
    {
        tpfactory funct = &TPFactory<LP, unsigned int, Codelet*, arg1>;
        tpClosure3<unsigned int, Codelet*, arg1>* temp = new tpClosure3<unsigned int, Codelet*, arg1>(funct, myTP, 0, this, A1);
        itPtr = &temp->a1;
        closure = temp;
    }

    template <class arg1, class arg2>
    paraFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2)
        : Codelet(dep, res * it, myTP, stat)
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , fired(false)
        , iterations_(it)
        , toSignal_(toSig)
    {
        tpfactory funct = &TPFactory<LP, unsigned int, Codelet*, arg1, arg2>;
        tpClosure4<unsigned int, Codelet*, arg1, arg2>* temp = new tpClosure4<unsigned int, Codelet*, arg1, arg2>(funct, myTP, 0, this, A1, A2);
        itPtr = &temp->a1;
        closure = temp;
    }

    template <class arg1, class arg2, class arg3>
    paraFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3)
        : Codelet(dep, res * it, myTP, stat)
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , fired(false)
        , iterations_(it)
        , toSignal_(toSig)
    {
        tpfactory funct = &TPFactory<LP, unsigned int, Codelet*, arg1, arg2, arg3>;
        tpClosure5<unsigned int, Codelet*, arg1, arg2, arg3>* temp = new tpClosure5<unsigned int, Codelet*, arg1, arg2, arg3>(funct, myTP, 0, this, A1, A2, A3);
        itPtr = &temp->a1;
        closure = temp;
    }

    template <class arg1, class arg2, class arg3, class arg4>
    paraFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4)
        : Codelet(dep, res * it, myTP, stat)
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , fired(false)
        , iterations_(it)
        , toSignal_(toSig)
    {
        tpfactory funct = &TPFactory<LP, unsigned int, Codelet*, arg1, arg2, arg3, arg4>;
        tpClosure6<unsigned int, Codelet*, arg1, arg2, arg3, arg4>* temp = new tpClosure6<unsigned int, Codelet*, arg1, arg2, arg3, arg4>(funct, myTP, 0, this, A1, A2, A3, A4);
        itPtr = &temp->a1;
        closure = temp;
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5>
    paraFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5)
        : Codelet(dep, res * it, myTP, stat)
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , fired(false)
        , iterations_(it)
        , toSignal_(toSig)
    {
        tpfactory funct = &TPFactory<LP, unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5>;
        tpClosure7<unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5>* temp = new tpClosure7<unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5>(funct, myTP, 0, this, A1, A2, A3, A4, A5);
        itPtr = &temp->a1;
        closure = temp;
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6>
    paraFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6)
        : Codelet(dep, res * it, myTP, stat)
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , fired(false)
        , iterations_(it)
        , toSignal_(toSig)
    {
        tpfactory funct = &TPFactory<LP, unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
            arg6>;
        tpClosure8<unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
            arg6>* temp
            = new tpClosure8<unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
                arg6>(funct, myTP, 0, this, A1, A2, A3, A4, A5,
                A6);
        itPtr = &temp->a1;
        closure = temp;
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7>
    paraFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7)
        : Codelet(dep, res * it, myTP, stat)
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , fired(false)
        , iterations_(it)
        , toSignal_(toSig)
    {
        tpfactory funct = &TPFactory<LP, unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7>;
        tpClosure9<unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7>* temp
            = new tpClosure9<unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
                arg6, arg7>(funct, myTP, 0, this, A1, A2, A3, A4, A5,
                A6, A7);
        itPtr = &temp->a1;
        closure = temp;
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8>
    paraFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8)
        : Codelet(dep, res * it, myTP, stat)
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , fired(false)
        , iterations_(it)
        , toSignal_(toSig)
    {
        tpfactory funct = &TPFactory<LP, unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8>;
        tpClosure10<unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8>* temp
            = new tpClosure10<unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
                arg6, arg7, arg8>(funct, myTP, 0, this, A1, A2, A3, A4, A5,
                A6, A7, A8);
        itPtr = &temp->a1;
        closure = temp;
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9>
    paraFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9)
        : Codelet(dep, res * it, myTP, stat)
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , fired(false)
        , iterations_(it)
        , toSignal_(toSig)
    {
        tpfactory funct = &TPFactory<LP, unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9>;
        tpClosure11<unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9>* temp
            = new tpClosure11<unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
                arg6, arg7, arg8, arg9>(funct, myTP, 0, this, A1, A2, A3, A4, A5,
                A6, A7, A8, A9);
        itPtr = &temp->a1;
        closure = temp;
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10>
    paraFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10)
        : Codelet(dep, res * it, myTP, stat)
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , fired(false)
        , iterations_(it)
        , toSignal_(toSig)
    {
        tpfactory funct = &TPFactory<LP, unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10>;
        tpClosure12<unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10>* temp
            = new tpClosure12<unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
                arg6, arg7, arg8, arg9, arg10>(funct, myTP, 0, this, A1, A2, A3, A4, A5,
                A6, A7, A8, A9, A10);
        itPtr = &temp->a1;
        closure = temp;
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11>
    paraFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11)
        : Codelet(dep, res * it, myTP, stat)
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , fired(false)
        , iterations_(it)
        , toSignal_(toSig)
    {
        tpfactory funct = &TPFactory<LP, unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11>;
        tpClosure13<unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11>* temp
            = new tpClosure13<unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
                arg6, arg7, arg8, arg9, arg10,
                arg11>(funct, myTP, 0, this, A1, A2, A3, A4, A5,
                A6, A7, A8, A9, A10,
                A11);
        itPtr = &temp->a1;
        closure = temp;
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12>
    paraFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12)
        : Codelet(dep, res * it, myTP, stat)
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , fired(false)
        , iterations_(it)
        , toSignal_(toSig)
    {
        tpfactory funct = &TPFactory<LP, unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12>;
        tpClosure14<unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12>* temp
            = new tpClosure14<unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
                arg6, arg7, arg8, arg9, arg10,
                arg11, arg12>(funct, myTP, 0, this, A1, A2, A3, A4, A5,
                A6, A7, A8, A9, A10,
                A11, A12);
        itPtr = &temp->a1;
        closure = temp;
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13>
    paraFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13)
        : Codelet(dep, res * it, myTP, stat)
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , fired(false)
        , iterations_(it)
        , toSignal_(toSig)
    {
        tpfactory funct = &TPFactory<LP, unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13>;
        tpClosure15<unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13>* temp
            = new tpClosure15<unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
                arg6, arg7, arg8, arg9, arg10,
                arg11, arg12, arg13>(funct, myTP, 0, this, A1, A2, A3, A4, A5,
                A6, A7, A8, A9, A10,
                A11, A12, A13);
        itPtr = &temp->a1;
        closure = temp;
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14>
    paraFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14)
        : Codelet(dep, res * it, myTP, stat)
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , fired(false)
        , iterations_(it)
        , toSignal_(toSig)
    {
        tpfactory funct = &TPFactory<LP, unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14>;
        tpClosure16<unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14>* temp
            = new tpClosure16<unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
                arg6, arg7, arg8, arg9, arg10,
                arg11, arg12, arg13, arg14>(funct, myTP, 0, this, A1, A2, A3, A4, A5,
                A6, A7, A8, A9, A10,
                A11, A12, A13, A14);
        itPtr = &temp->a1;
        closure = temp;
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14, class arg15>
    paraFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15)
        : Codelet(dep, res * it, myTP, stat)
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , fired(false)
        , iterations_(it)
        , toSignal_(toSig)
    {
        tpfactory funct = &TPFactory<LP, unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15>;
        tpClosure17<unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15>* temp
            = new tpClosure17<unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
                arg6, arg7, arg8, arg9, arg10,
                arg11, arg12, arg13, arg14, arg15>(funct, myTP, 0, this, A1, A2, A3, A4, A5,
                A6, A7, A8, A9, A10,
                A11, A12, A13, A14, A15);
        itPtr = &temp->a1;
        closure = temp;
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14, class arg15,
        class arg16>
    paraFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16)
        : Codelet(dep, res * it, myTP, stat)
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , fired(false)
        , iterations_(it)
        , toSignal_(toSig)
    {
        tpfactory funct = &TPFactory<LP, unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16>;
        tpClosure18<unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16>* temp
            = new tpClosure18<unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
                arg6, arg7, arg8, arg9, arg10,
                arg11, arg12, arg13, arg14, arg15,
                arg16>(funct, myTP, 0, this, A1, A2, A3, A4, A5,
                A6, A7, A8, A9, A10,
                A11, A12, A13, A14, A15,
                A16);
        itPtr = &temp->a1;
        closure = temp;
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14, class arg15,
        class arg16, class arg17>
    paraFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17)
        : Codelet(dep, res * it, myTP, stat)
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , fired(false)
        , iterations_(it)
        , toSignal_(toSig)
    {
        tpfactory funct = &TPFactory<LP, unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17>;
        tpClosure19<unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17>* temp
            = new tpClosure19<unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
                arg6, arg7, arg8, arg9, arg10,
                arg11, arg12, arg13, arg14, arg15,
                arg16, arg17>(funct, myTP, 0, this, A1, A2, A3, A4, A5,
                A6, A7, A8, A9, A10,
                A11, A12, A13, A14, A15,
                A16, A17);
        itPtr = &temp->a1;
        closure = temp;
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14, class arg15,
        class arg16, class arg17, class arg18>
    paraFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18)
        : Codelet(dep, res * it, myTP, stat)
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , fired(false)
        , iterations_(it)
        , toSignal_(toSig)
    {
        tpfactory funct = &TPFactory<LP, unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18>;
        tpClosure20<unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18>* temp
            = new tpClosure20<unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
                arg6, arg7, arg8, arg9, arg10,
                arg11, arg12, arg13, arg14, arg15,
                arg16, arg17, arg18>(funct, myTP, 0, this, A1, A2, A3, A4, A5,
                A6, A7, A8, A9, A10,
                A11, A12, A13, A14, A15,
                A16, A17, A18);
        itPtr = &temp->a1;
        closure = temp;
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14, class arg15,
        class arg16, class arg17, class arg18, class arg19>
    paraFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19)
        : Codelet(dep, res * it, myTP, stat)
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , fired(false)
        , iterations_(it)
        , toSignal_(toSig)
    {
        tpfactory funct = &TPFactory<LP, unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19>;
        tpClosure21<unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19>* temp
            = new tpClosure21<unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
                arg6, arg7, arg8, arg9, arg10,
                arg11, arg12, arg13, arg14, arg15,
                arg16, arg17, arg18, arg19>(funct, myTP, 0, this, A1, A2, A3, A4, A5,
                A6, A7, A8, A9, A10,
                A11, A12, A13, A14, A15,
                A16, A17, A18, A19);
        itPtr = &temp->a1;
        closure = temp;
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14, class arg15,
        class arg16, class arg17, class arg18, class arg19, class arg20>
    paraFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20)
        : Codelet(dep, res * it, myTP, stat)
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , fired(false)
        , iterations_(it)
        , toSignal_(toSig)
    {
        tpfactory funct = &TPFactory<LP, unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20>;
        tpClosure22<unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20>* temp
            = new tpClosure22<unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
                arg6, arg7, arg8, arg9, arg10,
                arg11, arg12, arg13, arg14, arg15,
                arg16, arg17, arg18, arg19, arg20>(funct, myTP, 0, this, A1, A2, A3, A4, A5,
                A6, A7, A8, A9, A10,
                A11, A12, A13, A14, A15,
                A16, A17, A18, A19, A20);
        itPtr = &temp->a1;
        closure = temp;
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14, class arg15,
        class arg16, class arg17, class arg18, class arg19, class arg20,
        class arg21>
    paraFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20,
        arg21 A21)
        : Codelet(dep, res * it, myTP, stat)
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , fired(false)
        , iterations_(it)
        , toSignal_(toSig)
    {
        tpfactory funct = &TPFactory<LP, unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21>;
        tpClosure23<unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21>* temp
            = new tpClosure23<unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
                arg6, arg7, arg8, arg9, arg10,
                arg11, arg12, arg13, arg14, arg15,
                arg16, arg17, arg18, arg19, arg20,
                arg21>(funct, myTP, 0, this, A1, A2, A3, A4, A5,
                A6, A7, A8, A9, A10,
                A11, A12, A13, A14, A15,
                A16, A17, A18, A19, A20,
                A21);
        itPtr = &temp->a1;
        closure = temp;
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14, class arg15,
        class arg16, class arg17, class arg18, class arg19, class arg20,
        class arg21, class arg22>
    paraFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20,
        arg21 A21, arg22 A22)
        : Codelet(dep, res * it, myTP, stat)
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , fired(false)
        , iterations_(it)
        , toSignal_(toSig)
    {
        tpfactory funct = &TPFactory<LP, unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22>;
        tpClosure24<unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22>* temp
            = new tpClosure24<unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
                arg6, arg7, arg8, arg9, arg10,
                arg11, arg12, arg13, arg14, arg15,
                arg16, arg17, arg18, arg19, arg20,
                arg21, arg22>(funct, myTP, 0, this, A1, A2, A3, A4, A5,
                A6, A7, A8, A9, A10,
                A11, A12, A13, A14, A15,
                A16, A17, A18, A19, A20,
                A21, A22);
        itPtr = &temp->a1;
        closure = temp;
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14, class arg15,
        class arg16, class arg17, class arg18, class arg19, class arg20,
        class arg21, class arg22, class arg23>
    paraFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20,
        arg21 A21, arg22 A22, arg23 A23)
        : Codelet(dep, res * it, myTP, stat)
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , fired(false)
        , iterations_(it)
        , toSignal_(toSig)
    {
        tpfactory funct = &TPFactory<LP, unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23>;
        tpClosure25<unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23>* temp
            = new tpClosure25<unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
                arg6, arg7, arg8, arg9, arg10,
                arg11, arg12, arg13, arg14, arg15,
                arg16, arg17, arg18, arg19, arg20,
                arg21, arg22, arg23>(funct, myTP, 0, this, A1, A2, A3, A4, A5,
                A6, A7, A8, A9, A10,
                A11, A12, A13, A14, A15,
                A16, A17, A18, A19, A20,
                A21, A22, A23);
        itPtr = &temp->a1;
        closure = temp;
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14, class arg15,
        class arg16, class arg17, class arg18, class arg19, class arg20,
        class arg21, class arg22, class arg23, class arg24>
    paraFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20,
        arg21 A21, arg22 A22, arg23 A23, arg24 A24)
        : Codelet(dep, res * it, myTP, stat)
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , fired(false)
        , iterations_(it)
        , toSignal_(toSig)
    {
        tpfactory funct = &TPFactory<LP, unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24>;
        tpClosure26<unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24>* temp
            = new tpClosure26<unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
                arg6, arg7, arg8, arg9, arg10,
                arg11, arg12, arg13, arg14, arg15,
                arg16, arg17, arg18, arg19, arg20,
                arg21, arg22, arg23, arg24>(funct, myTP, 0, this, A1, A2, A3, A4, A5,
                A6, A7, A8, A9, A10,
                A11, A12, A13, A14, A15,
                A16, A17, A18, A19, A20,
                A21, A22, A23, A24);
        itPtr = &temp->a1;
        closure = temp;
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14, class arg15,
        class arg16, class arg17, class arg18, class arg19, class arg20,
        class arg21, class arg22, class arg23, class arg24, class arg25>
    paraFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20,
        arg21 A21, arg22 A22, arg23 A23, arg24 A24, arg25 A25)
        : Codelet(dep, res * it, myTP, stat)
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , fired(false)
        , iterations_(it)
        , toSignal_(toSig)
    {
        tpfactory funct = &TPFactory<LP, unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25>;
        tpClosure27<unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25>* temp
            = new tpClosure27<unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
                arg6, arg7, arg8, arg9, arg10,
                arg11, arg12, arg13, arg14, arg15,
                arg16, arg17, arg18, arg19, arg20,
                arg21, arg22, arg23, arg24, arg25>(funct, myTP, 0, this, A1, A2, A3, A4, A5,
                A6, A7, A8, A9, A10,
                A11, A12, A13, A14, A15,
                A16, A17, A18, A19, A20,
                A21, A22, A23, A24, A25);
        itPtr = &temp->a1;
        closure = temp;
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14, class arg15,
        class arg16, class arg17, class arg18, class arg19, class arg20,
        class arg21, class arg22, class arg23, class arg24, class arg25,
        class arg26>
    paraFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20,
        arg21 A21, arg22 A22, arg23 A23, arg24 A24, arg25 A25,
        arg26 A26)
        : Codelet(dep, res * it, myTP, stat)
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , fired(false)
        , iterations_(it)
        , toSignal_(toSig)
    {
        tpfactory funct = &TPFactory<LP, unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25,
            arg26>;
        tpClosure28<unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25,
            arg26>* temp
            = new tpClosure28<unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
                arg6, arg7, arg8, arg9, arg10,
                arg11, arg12, arg13, arg14, arg15,
                arg16, arg17, arg18, arg19, arg20,
                arg21, arg22, arg23, arg24, arg25,
                arg26>(funct, myTP, 0, this, A1, A2, A3, A4, A5,
                A6, A7, A8, A9, A10,
                A11, A12, A13, A14, A15,
                A16, A17, A18, A19, A20,
                A21, A22, A23, A24, A25,
                A26);
        itPtr = &temp->a1;
        closure = temp;
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14, class arg15,
        class arg16, class arg17, class arg18, class arg19, class arg20,
        class arg21, class arg22, class arg23, class arg24, class arg25,
        class arg26, class arg27>
    paraFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20,
        arg21 A21, arg22 A22, arg23 A23, arg24 A24, arg25 A25,
        arg26 A26, arg27 A27)
        : Codelet(dep, res * it, myTP, stat)
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , fired(false)
        , iterations_(it)
        , toSignal_(toSig)
    {
        tpfactory funct = &TPFactory<LP, unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25,
            arg26, arg27>;
        tpClosure29<unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25,
            arg26, arg27>* temp
            = new tpClosure29<unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
                arg6, arg7, arg8, arg9, arg10,
                arg11, arg12, arg13, arg14, arg15,
                arg16, arg17, arg18, arg19, arg20,
                arg21, arg22, arg23, arg24, arg25,
                arg26, arg27>(funct, myTP, 0, this, A1, A2, A3, A4, A5,
                A6, A7, A8, A9, A10,
                A11, A12, A13, A14, A15,
                A16, A17, A18, A19, A20,
                A21, A22, A23, A24, A25,
                A26, A27);
        itPtr = &temp->a1;
        closure = temp;
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14, class arg15,
        class arg16, class arg17, class arg18, class arg19, class arg20,
        class arg21, class arg22, class arg23, class arg24, class arg25,
        class arg26, class arg27, class arg28>
    paraFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20,
        arg21 A21, arg22 A22, arg23 A23, arg24 A24, arg25 A25,
        arg26 A26, arg27 A27, arg28 A28)
        : Codelet(dep, res * it, myTP, stat)
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , fired(false)
        , iterations_(it)
        , toSignal_(toSig)
    {
        tpfactory funct = &TPFactory<LP, unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25,
            arg26, arg27, arg28>;
        tpClosure30<unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25,
            arg26, arg27, arg28>* temp
            = new tpClosure30<unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
                arg6, arg7, arg8, arg9, arg10,
                arg11, arg12, arg13, arg14, arg15,
                arg16, arg17, arg18, arg19, arg20,
                arg21, arg22, arg23, arg24, arg25,
                arg26, arg27, arg28>(funct, myTP, 0, this, A1, A2, A3, A4, A5,
                A6, A7, A8, A9, A10,
                A11, A12, A13, A14, A15,
                A16, A17, A18, A19, A20,
                A21, A22, A23, A24, A25,
                A26, A27, A28);
        itPtr = &temp->a1;
        closure = temp;
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14, class arg15,
        class arg16, class arg17, class arg18, class arg19, class arg20,
        class arg21, class arg22, class arg23, class arg24, class arg25,
        class arg26, class arg27, class arg28, class arg29>
    paraFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20,
        arg21 A21, arg22 A22, arg23 A23, arg24 A24, arg25 A25,
        arg26 A26, arg27 A27, arg28 A28, arg29 A29)
        : Codelet(dep, res * it, myTP, stat)
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , fired(false)
        , iterations_(it)
        , toSignal_(toSig)
    {
        tpfactory funct = &TPFactory<LP, unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25,
            arg26, arg27, arg28, arg29>;
        tpClosure31<unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25,
            arg26, arg27, arg28, arg29>* temp
            = new tpClosure31<unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
                arg6, arg7, arg8, arg9, arg10,
                arg11, arg12, arg13, arg14, arg15,
                arg16, arg17, arg18, arg19, arg20,
                arg21, arg22, arg23, arg24, arg25,
                arg26, arg27, arg28, arg29>(funct, myTP, 0, this, A1, A2, A3, A4, A5,
                A6, A7, A8, A9, A10,
                A11, A12, A13, A14, A15,
                A16, A17, A18, A19, A20,
                A21, A22, A23, A24, A25,
                A26, A27, A28, A29);
        itPtr = &temp->a1;
        closure = temp;
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14, class arg15,
        class arg16, class arg17, class arg18, class arg19, class arg20,
        class arg21, class arg22, class arg23, class arg24, class arg25,
        class arg26, class arg27, class arg28, class arg29, class arg30>
    paraFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20,
        arg21 A21, arg22 A22, arg23 A23, arg24 A24, arg25 A25,
        arg26 A26, arg27 A27, arg28 A28, arg29 A29, arg30 A30)
        : Codelet(dep, res * it, myTP, stat)
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , fired(false)
        , iterations_(it)
        , toSignal_(toSig)
    {
        tpfactory funct = &TPFactory<LP, unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25,
            arg26, arg27, arg28, arg29, arg30>;
        tpClosure32<unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25,
            arg26, arg27, arg28, arg29, arg30>* temp
            = new tpClosure32<unsigned int, Codelet*, arg1, arg2, arg3, arg4, arg5,
                arg6, arg7, arg8, arg9, arg10,
                arg11, arg12, arg13, arg14, arg15,
                arg16, arg17, arg18, arg19, arg20,
                arg21, arg22, arg23, arg24, arg25,
                arg26, arg27, arg28, arg29, arg30>(funct, myTP, 0, this, A1, A2, A3, A4, A5,
                A6, A7, A8, A9, A10,
                A11, A12, A13, A14, A15,
                A16, A17, A18, A19, A20,
                A21, A22, A23, A24, A25,
                A26, A27, A28, A29, A30);
        itPtr = &temp->a1;
        closure = temp;
    }

    void fire(void)
    {
        fired = !fired;
        //inverted
        if (fired) {
            initCodelet(Res * iterations_, Res, myTP_, Stat);
            for (unsigned int i = 0; i < iterations_; i++) {
                (*itPtr) = i;
                myTP_->incRef();
                myThread.threadTPsched->placeTP(closure->clone());
            }
        } else {
            initCodelet(Dep, Res, myTP_, Stat);
            toSignal_->decDep();
        }
    }
};

template <class LP, class alloc = std::allocator<LP> >
class serialFor : public Codelet, public alloc {
private:
    uint32_t Dep;
    uint32_t Res;
    uint32_t Stat;
    unsigned int curIterations_;
    unsigned int iterations_;
    Codelet* toSignal_;
    lpClosure* closure;
    LP* loop_;

public:
    ~serialFor(void)
    {
        delete closure;
        alloc::deallocate(loop_, 1);
    }

    void setIterations(unsigned int it) { iterations_ = it; }

    serialFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it)
        : Codelet(dep, res, myTP, stat)
        , alloc()
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , curIterations_(0)
        , iterations_(it)
        , toSignal_(toSig)
        , loop_(alloc::allocate(1))
    {
        lpfactory funct = &LPFactory<LP>;
        closure = new lpClosure(funct, 0, this);
    }

    template <class arg1>
    serialFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1)
        : Codelet(dep, res, myTP, stat)
        , alloc()
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , curIterations_(0)
        , iterations_(it)
        , toSignal_(toSig)
        , loop_(alloc::allocate(1))
    {
        lpfactory funct = &LPFactory<LP, arg1>;
        closure = new lpClosure1<arg1>(funct, 0, this, A1);
    }

    template <class arg1, class arg2>
    serialFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2)
        : Codelet(dep, res, myTP, stat)
        , alloc()
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , curIterations_(0)
        , iterations_(it)
        , toSignal_(toSig)
        , loop_(alloc::allocate(1))
    {
        lpfactory funct = &LPFactory<LP, arg1, arg2>;
        closure = new lpClosure2<arg1, arg2>(funct, 0, this, A1, A2);
    }

    template <class arg1, class arg2, class arg3>
    serialFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3)
        : Codelet(dep, res, myTP, stat)
        , alloc()
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , curIterations_(0)
        , iterations_(it)
        , toSignal_(toSig)
        , loop_(alloc::allocate(1))
    {
        lpfactory funct = &LPFactory<LP, arg1, arg2, arg3>;
        closure = new lpClosure3<arg1, arg2, arg3>(funct, 0, this, A1, A2, A3);
    }

    template <class arg1, class arg2, class arg3, class arg4>
    serialFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4)
        : Codelet(dep, res, myTP, stat)
        , alloc()
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , curIterations_(0)
        , iterations_(it)
        , toSignal_(toSig)
        , loop_(alloc::allocate(1))
    {
        lpfactory funct = &LPFactory<LP, arg1, arg2, arg3, arg4>;
        closure = new lpClosure4<arg1, arg2, arg3, arg4>(funct, 0, this, A1, A2, A3, A4);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5>
    serialFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5)
        : Codelet(dep, res, myTP, stat)
        , alloc()
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , curIterations_(0)
        , iterations_(it)
        , toSignal_(toSig)
        , loop_(alloc::allocate(1))
    {
        lpfactory funct = &LPFactory<LP, arg1, arg2, arg3, arg4, arg5>;
        closure = new lpClosure5<arg1, arg2, arg3, arg4, arg5>(funct, 0, this, A1, A2, A3, A4, A5);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6>
    serialFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6)
        : Codelet(dep, res, myTP, stat)
        , alloc()
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , curIterations_(0)
        , iterations_(it)
        , toSignal_(toSig)
        , loop_(alloc::allocate(1))
    {
        lpfactory funct = &LPFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6>;
        closure = new lpClosure6<arg1, arg2, arg3, arg4, arg5,
            arg6>(funct, 0, this, A1, A2, A3, A4, A5,
            A6);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7>
    serialFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7)
        : Codelet(dep, res, myTP, stat)
        , alloc()
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , curIterations_(0)
        , iterations_(it)
        , toSignal_(toSig)
        , loop_(alloc::allocate(1))
    {
        lpfactory funct = &LPFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7>;
        closure = new lpClosure7<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7>(funct, 0, this, A1, A2, A3, A4, A5,
            A6, A7);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8>
    serialFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8)
        : Codelet(dep, res, myTP, stat)
        , alloc()
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , curIterations_(0)
        , iterations_(it)
        , toSignal_(toSig)
        , loop_(alloc::allocate(1))
    {
        lpfactory funct = &LPFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8>;
        closure = new lpClosure8<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8>(funct, 0, this, A1, A2, A3, A4, A5,
            A6, A7, A8);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9>
    serialFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9)
        : Codelet(dep, res, myTP, stat)
        , alloc()
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , curIterations_(0)
        , iterations_(it)
        , toSignal_(toSig)
        , loop_(alloc::allocate(1))
    {
        lpfactory funct = &LPFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9>;
        closure = new lpClosure9<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9>(funct, 0, this, A1, A2, A3, A4, A5,
            A6, A7, A8, A9);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10>
    serialFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10)
        : Codelet(dep, res, myTP, stat)
        , alloc()
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , curIterations_(0)
        , iterations_(it)
        , toSignal_(toSig)
        , loop_(alloc::allocate(1))
    {
        lpfactory funct = &LPFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10>;
        closure = new lpClosure10<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10>(funct, 0, this, A1, A2, A3, A4, A5,
            A6, A7, A8, A9, A10);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11>
    serialFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11)
        : Codelet(dep, res, myTP, stat)
        , alloc()
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , curIterations_(0)
        , iterations_(it)
        , toSignal_(toSig)
        , loop_(alloc::allocate(1))
    {
        lpfactory funct = &LPFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11>;
        closure = new lpClosure11<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11>(funct, 0, this, A1, A2, A3, A4, A5,
            A6, A7, A8, A9, A10,
            A11);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12>
    serialFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12)
        : Codelet(dep, res, myTP, stat)
        , alloc()
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , curIterations_(0)
        , iterations_(it)
        , toSignal_(toSig)
        , loop_(alloc::allocate(1))
    {
        lpfactory funct = &LPFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12>;
        closure = new lpClosure12<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12>(funct, 0, this, A1, A2, A3, A4, A5,
            A6, A7, A8, A9, A10,
            A11, A12);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13>
    serialFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13)
        : Codelet(dep, res, myTP, stat)
        , alloc()
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , curIterations_(0)
        , iterations_(it)
        , toSignal_(toSig)
        , loop_(alloc::allocate(1))
    {
        lpfactory funct = &LPFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13>;
        closure = new lpClosure13<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13>(funct, 0, this, A1, A2, A3, A4, A5,
            A6, A7, A8, A9, A10,
            A11, A12, A13);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14>
    serialFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14)
        : Codelet(dep, res, myTP, stat)
        , alloc()
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , curIterations_(0)
        , iterations_(it)
        , toSignal_(toSig)
        , loop_(alloc::allocate(1))
    {
        lpfactory funct = &LPFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14>;
        closure = new lpClosure14<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14>(funct, 0, this, A1, A2, A3, A4, A5,
            A6, A7, A8, A9, A10,
            A11, A12, A13, A14);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14, class arg15>
    serialFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15)
        : Codelet(dep, res, myTP, stat)
        , alloc()
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , curIterations_(0)
        , iterations_(it)
        , toSignal_(toSig)
        , loop_(alloc::allocate(1))
    {
        lpfactory funct = &LPFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15>;
        closure = new lpClosure15<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15>(funct, 0, this, A1, A2, A3, A4, A5,
            A6, A7, A8, A9, A10,
            A11, A12, A13, A14, A15);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14, class arg15,
        class arg16>
    serialFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16)
        : Codelet(dep, res, myTP, stat)
        , alloc()
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , curIterations_(0)
        , iterations_(it)
        , toSignal_(toSig)
        , loop_(alloc::allocate(1))
    {
        lpfactory funct = &LPFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16>;
        closure = new lpClosure16<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16>(funct, 0, this, A1, A2, A3, A4, A5,
            A6, A7, A8, A9, A10,
            A11, A12, A13, A14, A15,
            A16);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14, class arg15,
        class arg16, class arg17>
    serialFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17)
        : Codelet(dep, res, myTP, stat)
        , alloc()
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , curIterations_(0)
        , iterations_(it)
        , toSignal_(toSig)
        , loop_(alloc::allocate(1))
    {
        lpfactory funct = &LPFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17>;
        closure = new lpClosure17<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17>(funct, 0, this, A1, A2, A3, A4, A5,
            A6, A7, A8, A9, A10,
            A11, A12, A13, A14, A15,
            A16, A17);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14, class arg15,
        class arg16, class arg17, class arg18>
    serialFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18)
        : Codelet(dep, res, myTP, stat)
        , alloc()
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , curIterations_(0)
        , iterations_(it)
        , toSignal_(toSig)
        , loop_(alloc::allocate(1))
    {
        lpfactory funct = &LPFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18>;
        closure = new lpClosure18<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18>(funct, 0, this, A1, A2, A3, A4, A5,
            A6, A7, A8, A9, A10,
            A11, A12, A13, A14, A15,
            A16, A17, A18);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14, class arg15,
        class arg16, class arg17, class arg18, class arg19>
    serialFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19)
        : Codelet(dep, res, myTP, stat)
        , alloc()
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , curIterations_(0)
        , iterations_(it)
        , toSignal_(toSig)
        , loop_(alloc::allocate(1))
    {
        lpfactory funct = &LPFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19>;
        closure = new lpClosure19<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19>(funct, 0, this, A1, A2, A3, A4, A5,
            A6, A7, A8, A9, A10,
            A11, A12, A13, A14, A15,
            A16, A17, A18, A19);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14, class arg15,
        class arg16, class arg17, class arg18, class arg19, class arg20>
    serialFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20)
        : Codelet(dep, res, myTP, stat)
        , alloc()
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , curIterations_(0)
        , iterations_(it)
        , toSignal_(toSig)
        , loop_(alloc::allocate(1))
    {
        lpfactory funct = &LPFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20>;
        closure = new lpClosure20<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20>(funct, 0, this, A1, A2, A3, A4, A5,
            A6, A7, A8, A9, A10,
            A11, A12, A13, A14, A15,
            A16, A17, A18, A19, A20);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14, class arg15,
        class arg16, class arg17, class arg18, class arg19, class arg20,
        class arg21>
    serialFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20,
        arg21 A21)
        : Codelet(dep, res, myTP, stat)
        , alloc()
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , curIterations_(0)
        , iterations_(it)
        , toSignal_(toSig)
        , loop_(alloc::allocate(1))
    {
        lpfactory funct = &LPFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21>;
        closure = new lpClosure21<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21>(funct, 0, this, A1, A2, A3, A4, A5,
            A6, A7, A8, A9, A10,
            A11, A12, A13, A14, A15,
            A16, A17, A18, A19, A20,
            A21);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14, class arg15,
        class arg16, class arg17, class arg18, class arg19, class arg20,
        class arg21, class arg22>
    serialFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20,
        arg21 A21, arg22 A22)
        : Codelet(dep, res, myTP, stat)
        , alloc()
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , curIterations_(0)
        , iterations_(it)
        , toSignal_(toSig)
        , loop_(alloc::allocate(1))
    {
        lpfactory funct = &LPFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22>;
        closure = new lpClosure22<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22>(funct, 0, this, A1, A2, A3, A4, A5,
            A6, A7, A8, A9, A10,
            A11, A12, A13, A14, A15,
            A16, A17, A18, A19, A20,
            A21, A22);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14, class arg15,
        class arg16, class arg17, class arg18, class arg19, class arg20,
        class arg21, class arg22, class arg23>
    serialFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20,
        arg21 A21, arg22 A22, arg23 A23)
        : Codelet(dep, res, myTP, stat)
        , alloc()
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , curIterations_(0)
        , iterations_(it)
        , toSignal_(toSig)
        , loop_(alloc::allocate(1))
    {
        lpfactory funct = &LPFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23>;
        closure = new lpClosure23<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23>(funct, 0, this, A1, A2, A3, A4, A5,
            A6, A7, A8, A9, A10,
            A11, A12, A13, A14, A15,
            A16, A17, A18, A19, A20,
            A21, A22, A23);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14, class arg15,
        class arg16, class arg17, class arg18, class arg19, class arg20,
        class arg21, class arg22, class arg23, class arg24>
    serialFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20,
        arg21 A21, arg22 A22, arg23 A23, arg24 A24)
        : Codelet(dep, res, myTP, stat)
        , alloc()
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , curIterations_(0)
        , iterations_(it)
        , toSignal_(toSig)
        , loop_(alloc::allocate(1))
    {
        lpfactory funct = &LPFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24>;
        closure = new lpClosure24<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24>(funct, 0, this, A1, A2, A3, A4, A5,
            A6, A7, A8, A9, A10,
            A11, A12, A13, A14, A15,
            A16, A17, A18, A19, A20,
            A21, A22, A23, A24);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14, class arg15,
        class arg16, class arg17, class arg18, class arg19, class arg20,
        class arg21, class arg22, class arg23, class arg24, class arg25>
    serialFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20,
        arg21 A21, arg22 A22, arg23 A23, arg24 A24, arg25 A25)
        : Codelet(dep, res, myTP, stat)
        , alloc()
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , curIterations_(0)
        , iterations_(it)
        , toSignal_(toSig)
        , loop_(alloc::allocate(1))
    {
        lpfactory funct = &LPFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25>;
        closure = new lpClosure25<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25>(funct, 0, this, A1, A2, A3, A4, A5,
            A6, A7, A8, A9, A10,
            A11, A12, A13, A14, A15,
            A16, A17, A18, A19, A20,
            A21, A22, A23, A24, A25);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14, class arg15,
        class arg16, class arg17, class arg18, class arg19, class arg20,
        class arg21, class arg22, class arg23, class arg24, class arg25,
        class arg26>
    serialFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20,
        arg21 A21, arg22 A22, arg23 A23, arg24 A24, arg25 A25,
        arg26 A26)
        : Codelet(dep, res, myTP, stat)
        , alloc()
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , curIterations_(0)
        , iterations_(it)
        , toSignal_(toSig)
        , loop_(alloc::allocate(1))
    {
        lpfactory funct = &LPFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25,
            arg26>;
        closure = new lpClosure26<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25,
            arg26>(funct, 0, this, A1, A2, A3, A4, A5,
            A6, A7, A8, A9, A10,
            A11, A12, A13, A14, A15,
            A16, A17, A18, A19, A20,
            A21, A22, A23, A24, A25,
            A26);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14, class arg15,
        class arg16, class arg17, class arg18, class arg19, class arg20,
        class arg21, class arg22, class arg23, class arg24, class arg25,
        class arg26, class arg27>
    serialFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20,
        arg21 A21, arg22 A22, arg23 A23, arg24 A24, arg25 A25,
        arg26 A26, arg27 A27)
        : Codelet(dep, res, myTP, stat)
        , alloc()
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , curIterations_(0)
        , iterations_(it)
        , toSignal_(toSig)
        , loop_(alloc::allocate(1))
    {
        lpfactory funct = &LPFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25,
            arg26, arg27>;
        closure = new lpClosure27<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25,
            arg26, arg27>(funct, 0, this, A1, A2, A3, A4, A5,
            A6, A7, A8, A9, A10,
            A11, A12, A13, A14, A15,
            A16, A17, A18, A19, A20,
            A21, A22, A23, A24, A25,
            A26, A27);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14, class arg15,
        class arg16, class arg17, class arg18, class arg19, class arg20,
        class arg21, class arg22, class arg23, class arg24, class arg25,
        class arg26, class arg27, class arg28>
    serialFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20,
        arg21 A21, arg22 A22, arg23 A23, arg24 A24, arg25 A25,
        arg26 A26, arg27 A27, arg28 A28)
        : Codelet(dep, res, myTP, stat)
        , alloc()
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , curIterations_(0)
        , iterations_(it)
        , toSignal_(toSig)
        , loop_(alloc::allocate(1))
    {
        lpfactory funct = &LPFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25,
            arg26, arg27, arg28>;
        closure = new lpClosure28<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25,
            arg26, arg27, arg28>(funct, 0, this, A1, A2, A3, A4, A5,
            A6, A7, A8, A9, A10,
            A11, A12, A13, A14, A15,
            A16, A17, A18, A19, A20,
            A21, A22, A23, A24, A25,
            A26, A27, A28);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14, class arg15,
        class arg16, class arg17, class arg18, class arg19, class arg20,
        class arg21, class arg22, class arg23, class arg24, class arg25,
        class arg26, class arg27, class arg28, class arg29>
    serialFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20,
        arg21 A21, arg22 A22, arg23 A23, arg24 A24, arg25 A25,
        arg26 A26, arg27 A27, arg28 A28, arg29 A29)
        : Codelet(dep, res, myTP, stat)
        , alloc()
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , curIterations_(0)
        , iterations_(it)
        , toSignal_(toSig)
        , loop_(alloc::allocate(1))
    {
        lpfactory funct = &LPFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25,
            arg26, arg27, arg28, arg29>;
        closure = new lpClosure29<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25,
            arg26, arg27, arg28, arg29>(funct, 0, this, A1, A2, A3, A4, A5,
            A6, A7, A8, A9, A10,
            A11, A12, A13, A14, A15,
            A16, A17, A18, A19, A20,
            A21, A22, A23, A24, A25,
            A26, A27, A28, A29);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14, class arg15,
        class arg16, class arg17, class arg18, class arg19, class arg20,
        class arg21, class arg22, class arg23, class arg24, class arg25,
        class arg26, class arg27, class arg28, class arg29, class arg30>
    serialFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20,
        arg21 A21, arg22 A22, arg23 A23, arg24 A24, arg25 A25,
        arg26 A26, arg27 A27, arg28 A28, arg29 A29, arg30 A30)
        : Codelet(dep, res, myTP, stat)
        , alloc()
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , curIterations_(0)
        , iterations_(it)
        , toSignal_(toSig)
        , loop_(alloc::allocate(1))
    {
        lpfactory funct = &LPFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25,
            arg26, arg27, arg28, arg29, arg30>;
        closure = new lpClosure30<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25,
            arg26, arg27, arg28, arg29, arg30>(funct, 0, this, A1, A2, A3, A4, A5,
            A6, A7, A8, A9, A10,
            A11, A12, A13, A14, A15,
            A16, A17, A18, A19, A20,
            A21, A22, A23, A24, A25,
            A26, A27, A28, A29, A30);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14, class arg15,
        class arg16, class arg17, class arg18, class arg19, class arg20,
        class arg21, class arg22, class arg23, class arg24, class arg25,
        class arg26, class arg27, class arg28, class arg29, class arg30,
        class arg31>
    serialFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20,
        arg21 A21, arg22 A22, arg23 A23, arg24 A24, arg25 A25,
        arg26 A26, arg27 A27, arg28 A28, arg29 A29, arg30 A30,
        arg31 A31)
        : Codelet(dep, res, myTP, stat)
        , alloc()
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , curIterations_(0)
        , iterations_(it)
        , toSignal_(toSig)
        , loop_(alloc::allocate(1))
    {
        lpfactory funct = &LPFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25,
            arg26, arg27, arg28, arg29, arg30,
            arg31>;
        closure = new lpClosure31<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25,
            arg26, arg27, arg28, arg29, arg30,
            arg31>(funct, 0, this, A1, A2, A3, A4, A5,
            A6, A7, A8, A9, A10,
            A11, A12, A13, A14, A15,
            A16, A17, A18, A19, A20,
            A21, A22, A23, A24, A25,
            A26, A27, A28, A29, A30,
            A31);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14, class arg15,
        class arg16, class arg17, class arg18, class arg19, class arg20,
        class arg21, class arg22, class arg23, class arg24, class arg25,
        class arg26, class arg27, class arg28, class arg29, class arg30,
        class arg31, class arg32>
    serialFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20,
        arg21 A21, arg22 A22, arg23 A23, arg24 A24, arg25 A25,
        arg26 A26, arg27 A27, arg28 A28, arg29 A29, arg30 A30,
        arg31 A31, arg32 A32)
        : Codelet(dep, res, myTP, stat)
        , alloc()
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , curIterations_(0)
        , iterations_(it)
        , toSignal_(toSig)
        , loop_(alloc::allocate(1))
    {
        lpfactory funct = &LPFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25,
            arg26, arg27, arg28, arg29, arg30,
            arg31, arg32>;
        closure = new lpClosure32<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25,
            arg26, arg27, arg28, arg29, arg30,
            arg31, arg32>(funct, 0, this, A1, A2, A3, A4, A5,
            A6, A7, A8, A9, A10,
            A11, A12, A13, A14, A15,
            A16, A17, A18, A19, A20,
            A21, A22, A23, A24, A25,
            A26, A27, A28, A29, A30,
            A31, A32);
    }

    void fire(void)
    {
        if (curIterations_ < iterations_) {
            if (!curIterations_) {
                myTP_->incRef();
            } else {
                alloc::destroy(loop_);
            }
            closure->iter = curIterations_;
            curIterations_++;
            //initCodelet(Res*iterations_,Res*iterations_,myTP_,Stat);
            resetCodelet();
            closure->factory(loop_, closure);
        } else if (curIterations_ == iterations_) {
            if (iterations_) {
                alloc::destroy(loop_);
                myTP_->decRef();
            }
            curIterations_ = 0;
            initCodelet(Dep, Res, myTP_, Stat);
            toSignal_->decDep();
        }
    }
};

//other loop
template <class LP, class alloc = std::allocator<LP> >
class codeletFor : public Codelet, public alloc {
private:
    uint32_t Dep;
    uint32_t Res;
    uint32_t Stat;
    bool fired;
    unsigned int iterations_;
    Codelet* toSignal_;
    LP* loop_;
    int64_t totalLoops;
    int64_t codeletsPerTP;

    lpClosure* closure;

public:
    ~codeletFor(void)
    {
        delete closure;
        if (iterations_ && loop_) {
            alloc::deallocate(loop_, iterations_);
            loop_ = NULL;
        }
    }

    void setIterations(unsigned int it)
    {
        if (iterations_ && loop_) {
            alloc::deallocate(loop_, iterations_);
        }
        iterations_ = it;
        loop_ = alloc::allocate(iterations_);
    }

    void fire(void)
    {
        if (iterations_) {
            if (!(this->fired)) {
                myTP_->incRef();
                initCodelet(Res * iterations_, Res, myTP_, Stat);
                for (unsigned int i = 0; i < iterations_; i++) {
                    closure->iter = i;
                    closure->factory(&loop_[i], closure);
                }
            } else {
                for (unsigned int i = 0; i < iterations_; i++) {
                    alloc::destroy(&loop_[i]);
                }
                initCodelet(Dep, Res, myTP_, Stat);
                myTP_->decRef();
                toSignal_->decDep();
            }

            this->fired = !(this->fired);
        } else {
            toSignal_->decDep();
        }
    }

    codeletFor()
        : Codelet()
    {
    }

    codeletFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it)
        : Codelet(dep, res * it, myTP, stat)
        , alloc()
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , fired(0)
        , iterations_(it)
        , toSignal_(toSig)
        , loop_(alloc::allocate(it))
    {
        lpfactory funct = &LPFactory<LP>;
        closure = new lpClosure(funct, 0, this);
    }

    template <class arg1>
    codeletFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1)
        : Codelet(dep, res * it, myTP, stat)
        , alloc()
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , fired(0)
        , iterations_(it)
        , toSignal_(toSig)
        , loop_(alloc::allocate(it))
    {
        lpfactory funct = &LPFactory<LP, arg1>;
        closure = new lpClosure1<arg1>(funct, 0, this, A1);
    }

    template <class arg1, class arg2>
    codeletFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2)
        : Codelet(dep, res * it, myTP, stat)
        , alloc()
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , fired(0)
        , iterations_(it)
        , toSignal_(toSig)
        , loop_(alloc::allocate(it))
    {
        lpfactory funct = &LPFactory<LP, arg1, arg2>;
        closure = new lpClosure2<arg1, arg2>(funct, 0, this, A1, A2);
    }

    template <class arg1, class arg2, class arg3>
    codeletFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3)
        : Codelet(dep, res * it, myTP, stat)
        , alloc()
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , fired(0)
        , iterations_(it)
        , toSignal_(toSig)
        , loop_(alloc::allocate(it))
    {
        lpfactory funct = &LPFactory<LP, arg1, arg2, arg3>;
        closure = new lpClosure3<arg1, arg2, arg3>(funct, 0, this, A1, A2, A3);
    }

    template <class arg1, class arg2, class arg3, class arg4>
    codeletFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4)
        : Codelet(dep, res * it, myTP, stat)
        , alloc()
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , fired(0)
        , iterations_(it)
        , toSignal_(toSig)
        , loop_(alloc::allocate(it))
    {
        lpfactory funct = &LPFactory<LP, arg1, arg2, arg3, arg4>;
        closure = new lpClosure4<arg1, arg2, arg3, arg4>(funct, 0, this, A1, A2, A3, A4);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5>
    codeletFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5)
        : Codelet(dep, res * it, myTP, stat)
        , alloc()
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , fired(0)
        , iterations_(it)
        , toSignal_(toSig)
        , loop_(alloc::allocate(it))
    {
        lpfactory funct = &LPFactory<LP, arg1, arg2, arg3, arg4, arg5>;
        closure = new lpClosure5<arg1, arg2, arg3, arg4, arg5>(funct, 0, this, A1, A2, A3, A4, A5);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6>
    codeletFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6)
        : Codelet(dep, res * it, myTP, stat)
        , alloc()
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , fired(0)
        , iterations_(it)
        , toSignal_(toSig)
        , loop_(alloc::allocate(it))
    //~ loop_(alloc::allocate(totalTPLoops))
    {
        lpfactory funct = &LPFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6>;
        closure = new lpClosure6<arg1, arg2, arg3, arg4, arg5,
            arg6>(funct, 0, this, A1, A2, A3, A4, A5,
            A6);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7>
    codeletFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7)
        : Codelet(dep, res * it, myTP, stat)
        , alloc()
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , fired(0)
        , iterations_(it)
        , toSignal_(toSig)
        , loop_(alloc::allocate(it))
    {
        lpfactory funct = &LPFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7>;
        closure = new lpClosure7<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7>(funct, 0, this, A1, A2, A3, A4, A5,
            A6, A7);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8>
    codeletFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8)
        : Codelet(dep, res * it, myTP, stat)
        , alloc()
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , fired(0)
        , iterations_(it)
        , toSignal_(toSig)
        , loop_(alloc::allocate(it))
    {
        lpfactory funct = &LPFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8>;
        closure = new lpClosure8<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8>(funct, 0, this, A1, A2, A3, A4, A5,
            A6, A7, A8);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9>
    codeletFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9)
        : Codelet(dep, res * it, myTP, stat)
        , alloc()
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , fired(0)
        , iterations_(it)
        , toSignal_(toSig)
        , loop_(alloc::allocate(it))
    {
        lpfactory funct = &LPFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9>;
        closure = new lpClosure9<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9>(funct, 0, this, A1, A2, A3, A4, A5,
            A6, A7, A8, A9);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10>
    codeletFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10)
        : Codelet(dep, res * it, myTP, stat)
        , alloc()
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , fired(0)
        , iterations_(it)
        , toSignal_(toSig)
        , loop_(alloc::allocate(it))
    {
        lpfactory funct = &LPFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10>;
        closure = new lpClosure10<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10>(funct, 0, this, A1, A2, A3, A4, A5,
            A6, A7, A8, A9, A10);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11>
    codeletFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11)
        : Codelet(dep, res * it, myTP, stat)
        , alloc()
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , fired(0)
        , iterations_(it)
        , toSignal_(toSig)
        , loop_(alloc::allocate(it))
    {
        lpfactory funct = &LPFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11>;
        closure = new lpClosure11<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11>(funct, 0, this, A1, A2, A3, A4, A5,
            A6, A7, A8, A9, A10,
            A11);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12>
    codeletFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12)
        : Codelet(dep, res * it, myTP, stat)
        , alloc()
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , fired(0)
        , iterations_(it)
        , toSignal_(toSig)
        , loop_(alloc::allocate(it))
    {
        lpfactory funct = &LPFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12>;
        closure = new lpClosure12<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12>(funct, 0, this, A1, A2, A3, A4, A5,
            A6, A7, A8, A9, A10,
            A11, A12);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13>
    codeletFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13)
        : Codelet(dep, res * it, myTP, stat)
        , alloc()
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , fired(0)
        , iterations_(it)
        , toSignal_(toSig)
        , loop_(alloc::allocate(it))
    {
        lpfactory funct = &LPFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13>;
        closure = new lpClosure13<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13>(funct, 0, this, A1, A2, A3, A4, A5,
            A6, A7, A8, A9, A10,
            A11, A12, A13);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14>
    codeletFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14)
        : Codelet(dep, res * it, myTP, stat)
        , alloc()
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , fired(0)
        , iterations_(it)
        , toSignal_(toSig)
        , loop_(alloc::allocate(it))
    {
        lpfactory funct = &LPFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14>;
        closure = new lpClosure14<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14>(funct, 0, this, A1, A2, A3, A4, A5,
            A6, A7, A8, A9, A10,
            A11, A12, A13, A14);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14, class arg15>
    codeletFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15)
        : Codelet(dep, res * it, myTP, stat)
        , alloc()
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , fired(0)
        , iterations_(it)
        , toSignal_(toSig)
        , loop_(alloc::allocate(it))
    {
        lpfactory funct = &LPFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15>;
        closure = new lpClosure15<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15>(funct, 0, this, A1, A2, A3, A4, A5,
            A6, A7, A8, A9, A10,
            A11, A12, A13, A14, A15);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14, class arg15,
        class arg16>
    codeletFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16)
        : Codelet(dep, res * it, myTP, stat)
        , alloc()
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , fired(0)
        , iterations_(it)
        , toSignal_(toSig)
        , loop_(alloc::allocate(it))
    {
        lpfactory funct = &LPFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16>;
        closure = new lpClosure16<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16>(funct, 0, this, A1, A2, A3, A4, A5,
            A6, A7, A8, A9, A10,
            A11, A12, A13, A14, A15,
            A16);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14, class arg15,
        class arg16, class arg17>
    codeletFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17)
        : Codelet(dep, res * it, myTP, stat)
        , alloc()
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , fired(0)
        , iterations_(it)
        , toSignal_(toSig)
        , loop_(alloc::allocate(it))
    {
        lpfactory funct = &LPFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17>;
        closure = new lpClosure17<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17>(funct, 0, this, A1, A2, A3, A4, A5,
            A6, A7, A8, A9, A10,
            A11, A12, A13, A14, A15,
            A16, A17);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14, class arg15,
        class arg16, class arg17, class arg18>
    codeletFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18)
        : Codelet(dep, res * it, myTP, stat)
        , alloc()
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , fired(0)
        , iterations_(it)
        , toSignal_(toSig)
        , loop_(alloc::allocate(it))
    {
        lpfactory funct = &LPFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18>;
        closure = new lpClosure18<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18>(funct, 0, this, A1, A2, A3, A4, A5,
            A6, A7, A8, A9, A10,
            A11, A12, A13, A14, A15,
            A16, A17, A18);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14, class arg15,
        class arg16, class arg17, class arg18, class arg19>
    codeletFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19)
        : Codelet(dep, res * it, myTP, stat)
        , alloc()
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , fired(0)
        , iterations_(it)
        , toSignal_(toSig)
        , loop_(alloc::allocate(it))
    {
        lpfactory funct = &LPFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19>;
        closure = new lpClosure19<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19>(funct, 0, this, A1, A2, A3, A4, A5,
            A6, A7, A8, A9, A10,
            A11, A12, A13, A14, A15,
            A16, A17, A18, A19);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14, class arg15,
        class arg16, class arg17, class arg18, class arg19, class arg20>
    codeletFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20)
        : Codelet(dep, res * it, myTP, stat)
        , alloc()
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , fired(0)
        , iterations_(it)
        , toSignal_(toSig)
        , loop_(alloc::allocate(it))
    {
        lpfactory funct = &LPFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20>;
        closure = new lpClosure20<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20>(funct, 0, this, A1, A2, A3, A4, A5,
            A6, A7, A8, A9, A10,
            A11, A12, A13, A14, A15,
            A16, A17, A18, A19, A20);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14, class arg15,
        class arg16, class arg17, class arg18, class arg19, class arg20,
        class arg21>
    codeletFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20,
        arg21 A21)
        : Codelet(dep, res * it, myTP, stat)
        , alloc()
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , fired(0)
        , iterations_(it)
        , toSignal_(toSig)
        , loop_(alloc::allocate(it))
    {
        lpfactory funct = &LPFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21>;
        closure = new lpClosure21<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21>(funct, 0, this, A1, A2, A3, A4, A5,
            A6, A7, A8, A9, A10,
            A11, A12, A13, A14, A15,
            A16, A17, A18, A19, A20,
            A21);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14, class arg15,
        class arg16, class arg17, class arg18, class arg19, class arg20,
        class arg21, class arg22>
    codeletFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20,
        arg21 A21, arg22 A22)
        : Codelet(dep, res * it, myTP, stat)
        , alloc()
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , fired(0)
        , iterations_(it)
        , toSignal_(toSig)
        , loop_(alloc::allocate(it))
    {
        lpfactory funct = &LPFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22>;
        closure = new lpClosure22<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22>(funct, 0, this, A1, A2, A3, A4, A5,
            A6, A7, A8, A9, A10,
            A11, A12, A13, A14, A15,
            A16, A17, A18, A19, A20,
            A21, A22);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14, class arg15,
        class arg16, class arg17, class arg18, class arg19, class arg20,
        class arg21, class arg22, class arg23>
    codeletFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20,
        arg21 A21, arg22 A22, arg23 A23)
        : Codelet(dep, res * it, myTP, stat)
        , alloc()
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , fired(0)
        , iterations_(it)
        , toSignal_(toSig)
        , loop_(alloc::allocate(it))
    {
        lpfactory funct = &LPFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23>;
        closure = new lpClosure23<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23>(funct, 0, this, A1, A2, A3, A4, A5,
            A6, A7, A8, A9, A10,
            A11, A12, A13, A14, A15,
            A16, A17, A18, A19, A20,
            A21, A22, A23);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14, class arg15,
        class arg16, class arg17, class arg18, class arg19, class arg20,
        class arg21, class arg22, class arg23, class arg24>
    codeletFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20,
        arg21 A21, arg22 A22, arg23 A23, arg24 A24)
        : Codelet(dep, res * it, myTP, stat)
        , alloc()
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , fired(0)
        , iterations_(it)
        , toSignal_(toSig)
        , loop_(alloc::allocate(it))
    {
        lpfactory funct = &LPFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24>;
        closure = new lpClosure24<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24>(funct, 0, this, A1, A2, A3, A4, A5,
            A6, A7, A8, A9, A10,
            A11, A12, A13, A14, A15,
            A16, A17, A18, A19, A20,
            A21, A22, A23, A24);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14, class arg15,
        class arg16, class arg17, class arg18, class arg19, class arg20,
        class arg21, class arg22, class arg23, class arg24, class arg25>
    codeletFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20,
        arg21 A21, arg22 A22, arg23 A23, arg24 A24, arg25 A25)
        : Codelet(dep, res * it, myTP, stat)
        , alloc()
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , fired(0)
        , iterations_(it)
        , toSignal_(toSig)
        , loop_(alloc::allocate(it))
    {
        lpfactory funct = &LPFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25>;
        closure = new lpClosure25<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25>(funct, 0, this, A1, A2, A3, A4, A5,
            A6, A7, A8, A9, A10,
            A11, A12, A13, A14, A15,
            A16, A17, A18, A19, A20,
            A21, A22, A23, A24, A25);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14, class arg15,
        class arg16, class arg17, class arg18, class arg19, class arg20,
        class arg21, class arg22, class arg23, class arg24, class arg25,
        class arg26>
    codeletFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20,
        arg21 A21, arg22 A22, arg23 A23, arg24 A24, arg25 A25,
        arg26 A26)
        : Codelet(dep, res * it, myTP, stat)
        , alloc()
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , fired(0)
        , iterations_(it)
        , toSignal_(toSig)
        , loop_(alloc::allocate(it))
    {
        lpfactory funct = &LPFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25,
            arg26>;
        closure = new lpClosure26<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25,
            arg26>(funct, 0, this, A1, A2, A3, A4, A5,
            A6, A7, A8, A9, A10,
            A11, A12, A13, A14, A15,
            A16, A17, A18, A19, A20,
            A21, A22, A23, A24, A25,
            A26);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14, class arg15,
        class arg16, class arg17, class arg18, class arg19, class arg20,
        class arg21, class arg22, class arg23, class arg24, class arg25,
        class arg26, class arg27>
    codeletFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20,
        arg21 A21, arg22 A22, arg23 A23, arg24 A24, arg25 A25,
        arg26 A26, arg27 A27)
        : Codelet(dep, res * it, myTP, stat)
        , alloc()
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , fired(0)
        , iterations_(it)
        , toSignal_(toSig)
        , loop_(alloc::allocate(it))
    {
        lpfactory funct = &LPFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25,
            arg26, arg27>;
        closure = new lpClosure27<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25,
            arg26, arg27>(funct, 0, this, A1, A2, A3, A4, A5,
            A6, A7, A8, A9, A10,
            A11, A12, A13, A14, A15,
            A16, A17, A18, A19, A20,
            A21, A22, A23, A24, A25,
            A26, A27);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14, class arg15,
        class arg16, class arg17, class arg18, class arg19, class arg20,
        class arg21, class arg22, class arg23, class arg24, class arg25,
        class arg26, class arg27, class arg28>
    codeletFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20,
        arg21 A21, arg22 A22, arg23 A23, arg24 A24, arg25 A25,
        arg26 A26, arg27 A27, arg28 A28)
        : Codelet(dep, res * it, myTP, stat)
        , alloc()
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , fired(0)
        , iterations_(it)
        , toSignal_(toSig)
        , loop_(alloc::allocate(it))
    {
        lpfactory funct = &LPFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25,
            arg26, arg27, arg28>;
        closure = new lpClosure28<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25,
            arg26, arg27, arg28>(funct, 0, this, A1, A2, A3, A4, A5,
            A6, A7, A8, A9, A10,
            A11, A12, A13, A14, A15,
            A16, A17, A18, A19, A20,
            A21, A22, A23, A24, A25,
            A26, A27, A28);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14, class arg15,
        class arg16, class arg17, class arg18, class arg19, class arg20,
        class arg21, class arg22, class arg23, class arg24, class arg25,
        class arg26, class arg27, class arg28, class arg29>
    codeletFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20,
        arg21 A21, arg22 A22, arg23 A23, arg24 A24, arg25 A25,
        arg26 A26, arg27 A27, arg28 A28, arg29 A29)
        : Codelet(dep, res * it, myTP, stat)
        , alloc()
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , fired(0)
        , iterations_(it)
        , toSignal_(toSig)
        , loop_(alloc::allocate(it))
    {
        lpfactory funct = &LPFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25,
            arg26, arg27, arg28, arg29>;
        closure = new lpClosure29<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25,
            arg26, arg27, arg28, arg29>(funct, 0, this, A1, A2, A3, A4, A5,
            A6, A7, A8, A9, A10,
            A11, A12, A13, A14, A15,
            A16, A17, A18, A19, A20,
            A21, A22, A23, A24, A25,
            A26, A27, A28, A29);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14, class arg15,
        class arg16, class arg17, class arg18, class arg19, class arg20,
        class arg21, class arg22, class arg23, class arg24, class arg25,
        class arg26, class arg27, class arg28, class arg29, class arg30>
    codeletFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20,
        arg21 A21, arg22 A22, arg23 A23, arg24 A24, arg25 A25,
        arg26 A26, arg27 A27, arg28 A28, arg29 A29, arg30 A30)
        : Codelet(dep, res * it, myTP, stat)
        , alloc()
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , fired(0)
        , iterations_(it)
        , toSignal_(toSig)
        , loop_(alloc::allocate(it))
    {
        lpfactory funct = &LPFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25,
            arg26, arg27, arg28, arg29, arg30>;
        closure = new lpClosure30<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25,
            arg26, arg27, arg28, arg29, arg30>(funct, 0, this, A1, A2, A3, A4, A5,
            A6, A7, A8, A9, A10,
            A11, A12, A13, A14, A15,
            A16, A17, A18, A19, A20,
            A21, A22, A23, A24, A25,
            A26, A27, A28, A29, A30);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14, class arg15,
        class arg16, class arg17, class arg18, class arg19, class arg20,
        class arg21, class arg22, class arg23, class arg24, class arg25,
        class arg26, class arg27, class arg28, class arg29, class arg30,
        class arg31>
    codeletFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20,
        arg21 A21, arg22 A22, arg23 A23, arg24 A24, arg25 A25,
        arg26 A26, arg27 A27, arg28 A28, arg29 A29, arg30 A30,
        arg31 A31)
        : Codelet(dep, res * it, myTP, stat)
        , alloc()
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , fired(0)
        , iterations_(it)
        , toSignal_(toSig)
        , loop_(alloc::allocate(it))
    {
        lpfactory funct = &LPFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25,
            arg26, arg27, arg28, arg29, arg30,
            arg31>;
        closure = new lpClosure31<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25,
            arg26, arg27, arg28, arg29, arg30,
            arg31>(funct, 0, this, A1, A2, A3, A4, A5,
            A6, A7, A8, A9, A10,
            A11, A12, A13, A14, A15,
            A16, A17, A18, A19, A20,
            A21, A22, A23, A24, A25,
            A26, A27, A28, A29, A30,
            A31);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14, class arg15,
        class arg16, class arg17, class arg18, class arg19, class arg20,
        class arg21, class arg22, class arg23, class arg24, class arg25,
        class arg26, class arg27, class arg28, class arg29, class arg30,
        class arg31, class arg32>
    codeletFor(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, unsigned int it,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20,
        arg21 A21, arg22 A22, arg23 A23, arg24 A24, arg25 A25,
        arg26 A26, arg27 A27, arg28 A28, arg29 A29, arg30 A30,
        arg31 A31, arg32 A32)
        : Codelet(dep, res * it, myTP, stat)
        , alloc()
        , Dep(dep)
        , Res(res)
        , Stat(stat)
        , fired(0)
        , iterations_(it)
        , toSignal_(toSig)
        , loop_(alloc::allocate(it))
    {
        lpfactory funct = &LPFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25,
            arg26, arg27, arg28, arg29, arg30,
            arg31, arg32>;
        closure = new lpClosure32<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25,
            arg26, arg27, arg28, arg29, arg30,
            arg31, arg32>(funct, 0, this, A1, A2, A3, A4, A5,
            A6, A7, A8, A9, A10,
            A11, A12, A13, A14, A15,
            A16, A17, A18, A19, A20,
            A21, A22, A23, A24, A25,
            A26, A27, A28, A29, A30,
            A31, A32);
    }
};

/*==============================================*/
/* Similar to class loop but automatically calculates the itearation
     * ranges.*/
template <typename Iterations_t = int64_t>
class TPLoop : public ThreadedProcedure {
public:
    unsigned int totalTPLoops;
    unsigned int codeletsPerTP;
    Iterations_t initIteration;
    Iterations_t lastIteration;
    Codelet* toSignal;

    unsigned int totalCodelets;
    Iterations_t range;
    Iterations_t rangePerCodelet;
    Iterations_t minIteration;
    Iterations_t remainderRange;

    TPLoop()
        : ThreadedProcedure(1, 0)
        , initIteration(-1)
        , lastIteration(-1)
        , toSignal(nullptr)
    {
    }

    TPLoop(unsigned int in_totalTPLoops, unsigned int in_codeletsPerTP, unsigned int in_mainCodeletID,
        Iterations_t in_initIteration, Iterations_t in_lastIteration,
        Codelet* in_toSig)
        : ThreadedProcedure(in_codeletsPerTP, in_mainCodeletID)
        , totalTPLoops(in_totalTPLoops)
        , codeletsPerTP(in_codeletsPerTP)
        , initIteration(in_initIteration)
        , lastIteration(in_lastIteration)
        , toSignal(in_toSig)
    {
    }
};
/*==============================================*/

/*==============================================*/
/* Similar to class loop but automatically calculates the itearation
     * ranges.*/
template <class LP, class alloc = std::allocator<LP>, typename Iterations_t = int64_t>
class TPLoopMasterCodelet : public Codelet, public alloc {
private:
    bool fired;
    int64_t totalTPLoops;
    int64_t codeletsPerTP;
    Codelet* toSignal_;

    Iterations_t initIteration;
    Iterations_t lastIteration;

    TPLoopClosure* closure;
    LP* loop_;

public:
    ~TPLoopMasterCodelet(void)
    {
        delete closure;
        if (totalTPLoops > 0 && loop_) {
            alloc::deallocate(loop_, totalTPLoops);
            loop_ = NULL;
        }
    }

    void setupTPLoop(int in_initIteration, int in_lastIteration)
    {
        if (totalTPLoops > 0 && loop_) {
            alloc::deallocate(loop_, totalTPLoops);
        }

        this->initIteration = in_initIteration;
        this->lastIteration = in_lastIteration;
        loop_ = alloc::allocate(this->totalTPLoops);
    }

    void setupTPLoop(int in_initIteration, int in_lastIteration, int64_t in_totalTPLoops, int64_t in_codeletsPerTP)
    {
        if (totalTPLoops > 0 && loop_) {
            alloc::deallocate(loop_, totalTPLoops);
        }

        this->totalTPLoops = in_totalTPLoops;
        this->codeletsPerTP = in_codeletsPerTP;
        this->initIteration = in_initIteration;
        this->lastIteration = in_lastIteration;
        loop_ = alloc::allocate(this->totalTPLoops);
    }

    void fire(void)
    {
        if (totalTPLoops > 0) {
            if (!(this->fired)) {
                myTP_->incRef();
                setDep(totalTPLoops * codeletsPerTP);
                for (unsigned int i = 0; i < totalTPLoops; i++) {
                    closure->mainID = i;
                    closure->totalTPLoops = this->totalTPLoops;
                    closure->codeletsPerTP = this->codeletsPerTP;
                    closure->factory(&loop_[i], closure);
                }
            } else {
                for (unsigned int i = 0; i < totalTPLoops; i++) {
                    alloc::destroy(&loop_[i]);
                }
                resetCodelet();
                myTP_->decRef();
                toSignal_->decDep();
            }

            this->fired = !(this->fired);
        } else {
            toSignal_->decDep();
        }
    }

    TPLoopMasterCodelet()
        : Codelet()
    {
    }

    TPLoopMasterCodelet(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, int64_t totalTPLoops, int64_t codeletsPerTP)
        : Codelet(dep, res, myTP, stat)
        , alloc()
        ,

        fired(0)
        , totalTPLoops(totalTPLoops)
        , codeletsPerTP(codeletsPerTP)
        , toSignal_(toSig)
        , loop_(alloc::allocate(totalTPLoops))
    {
        tpLoopFactory funct = &TPLoopFactory<LP>;
        closure = new TPLoopClosure(funct, 0, this, totalTPLoops, codeletsPerTP);
    }

    template <class arg1>
    TPLoopMasterCodelet(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, int64_t totalTPLoops, int64_t codeletsPerTP,
        arg1 A1)
        : Codelet(dep, res, myTP, stat)
        , alloc()
        ,

        fired(0)
        , totalTPLoops(totalTPLoops)
        , codeletsPerTP(codeletsPerTP)
        , toSignal_(toSig)
        , loop_(alloc::allocate(totalTPLoops))
    {
        tpLoopFactory funct = &TPLoopFactory<LP, arg1>;
        closure = new TPLoopClosure1<arg1>(funct, 0, this, totalTPLoops, codeletsPerTP, A1);
    }

    template <class arg1, class arg2>
    TPLoopMasterCodelet(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, int64_t totalTPLoops, int64_t codeletsPerTP,
        arg1 A1, arg2 A2)
        : Codelet(dep, res, myTP, stat)
        , alloc()
        ,

        fired(0)
        , totalTPLoops(totalTPLoops)
        , codeletsPerTP(codeletsPerTP)
        , toSignal_(toSig)
        , loop_(alloc::allocate(totalTPLoops))
    {
        tpLoopFactory funct = &TPLoopFactory<LP, arg1, arg2>;
        closure = new TPLoopClosure2<arg1, arg2>(funct, 0, this, totalTPLoops, codeletsPerTP, A1, A2);
    }

    template <class arg1, class arg2, class arg3>
    TPLoopMasterCodelet(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, int64_t totalTPLoops, int64_t codeletsPerTP,
        arg1 A1, arg2 A2, arg3 A3)
        : Codelet(dep, res, myTP, stat)
        , alloc()
        ,

        fired(0)
        , totalTPLoops(totalTPLoops)
        , codeletsPerTP(codeletsPerTP)
        , toSignal_(toSig)
        , loop_(alloc::allocate(totalTPLoops))
    {
        tpLoopFactory funct = &TPLoopFactory<LP, arg1, arg2, arg3>;
        closure = new TPLoopClosure3<arg1, arg2, arg3>(funct, 0, this, totalTPLoops, codeletsPerTP, A1, A2, A3);
    }

    template <class arg1, class arg2, class arg3, class arg4>
    TPLoopMasterCodelet(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, int64_t totalTPLoops, int64_t codeletsPerTP,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4)
        : Codelet(dep, res, myTP, stat)
        , alloc()
        ,

        fired(0)
        , totalTPLoops(totalTPLoops)
        , codeletsPerTP(codeletsPerTP)
        , toSignal_(toSig)
        , loop_(alloc::allocate(totalTPLoops))
    {
        tpLoopFactory funct = &TPLoopFactory<LP, arg1, arg2, arg3, arg4>;
        closure = new TPLoopClosure4<arg1, arg2, arg3, arg4>(funct, 0, this, totalTPLoops, codeletsPerTP, A1, A2, A3, A4);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5>
    TPLoopMasterCodelet(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, int64_t totalTPLoops, int64_t codeletsPerTP,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5)
        : Codelet(dep, res, myTP, stat)
        , alloc()
        ,

        fired(0)
        , totalTPLoops(totalTPLoops)
        , codeletsPerTP(codeletsPerTP)
        , toSignal_(toSig)
        , loop_(alloc::allocate(totalTPLoops))
    {
        tpLoopFactory funct = &TPLoopFactory<LP, arg1, arg2, arg3, arg4, arg5>;
        closure = new TPLoopClosure5<arg1, arg2, arg3, arg4, arg5>(funct, 0, this, totalTPLoops, codeletsPerTP, A1, A2, A3, A4, A5);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6>
    TPLoopMasterCodelet(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, int64_t totalTPLoops, int64_t codeletsPerTP,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6)
        : Codelet(dep, res, myTP, stat)
        , alloc()
        ,

        fired(0)
        , totalTPLoops(totalTPLoops)
        , codeletsPerTP(codeletsPerTP)
        , toSignal_(toSig)
        , loop_(alloc::allocate(totalTPLoops))
    {
        tpLoopFactory funct = &TPLoopFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6>;
        closure = new TPLoopClosure6<arg1, arg2, arg3, arg4, arg5,
            arg6>(funct, 0, this, totalTPLoops, codeletsPerTP, A1, A2, A3, A4, A5,
            A6);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7>
    TPLoopMasterCodelet(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, int64_t totalTPLoops, int64_t codeletsPerTP,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7)
        : Codelet(dep, res, myTP, stat)
        , alloc()
        ,

        fired(0)
        , totalTPLoops(totalTPLoops)
        , codeletsPerTP(codeletsPerTP)
        , toSignal_(toSig)
        , loop_(alloc::allocate(totalTPLoops))
    {
        tpLoopFactory funct = &TPLoopFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7>;
        closure = new TPLoopClosure7<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7>(funct, 0, this, totalTPLoops, codeletsPerTP, A1, A2, A3, A4, A5,
            A6, A7);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8>
    TPLoopMasterCodelet(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, int64_t totalTPLoops, int64_t codeletsPerTP,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8)
        : Codelet(dep, res, myTP, stat)
        , alloc()
        ,

        fired(0)
        , totalTPLoops(totalTPLoops)
        , codeletsPerTP(codeletsPerTP)
        , toSignal_(toSig)
        , loop_(alloc::allocate(totalTPLoops))
    {
        tpLoopFactory funct = &TPLoopFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8>;
        closure = new TPLoopClosure8<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8>(funct, 0, this, totalTPLoops, codeletsPerTP, A1, A2, A3, A4, A5,
            A6, A7, A8);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9>
    TPLoopMasterCodelet(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, int64_t totalTPLoops, int64_t codeletsPerTP,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9)
        : Codelet(dep, res, myTP, stat)
        , alloc()
        ,

        fired(0)
        , totalTPLoops(totalTPLoops)
        , codeletsPerTP(codeletsPerTP)
        , toSignal_(toSig)
        , loop_(alloc::allocate(totalTPLoops))
    {
        tpLoopFactory funct = &TPLoopFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9>;
        closure = new TPLoopClosure9<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9>(funct, 0, this, totalTPLoops, codeletsPerTP, A1, A2, A3, A4, A5,
            A6, A7, A8, A9);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10>
    TPLoopMasterCodelet(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, int64_t totalTPLoops, int64_t codeletsPerTP,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10)
        : Codelet(dep, res, myTP, stat)
        , alloc()
        ,

        fired(0)
        , totalTPLoops(totalTPLoops)
        , codeletsPerTP(codeletsPerTP)
        , toSignal_(toSig)
        , loop_(alloc::allocate(totalTPLoops))
    {
        tpLoopFactory funct = &TPLoopFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10>;
        closure = new TPLoopClosure10<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10>(funct, 0, this, totalTPLoops, codeletsPerTP, A1, A2, A3, A4, A5,
            A6, A7, A8, A9, A10);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11>
    TPLoopMasterCodelet(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, int64_t totalTPLoops, int64_t codeletsPerTP,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11)
        : Codelet(dep, res, myTP, stat)
        , alloc()
        ,

        fired(0)
        , totalTPLoops(totalTPLoops)
        , codeletsPerTP(codeletsPerTP)
        , toSignal_(toSig)
        , loop_(alloc::allocate(totalTPLoops))
    {
        tpLoopFactory funct = &TPLoopFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11>;
        closure = new TPLoopClosure11<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11>(funct, 0, this, totalTPLoops, codeletsPerTP, A1, A2, A3, A4, A5,
            A6, A7, A8, A9, A10,
            A11);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12>
    TPLoopMasterCodelet(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, int64_t totalTPLoops, int64_t codeletsPerTP,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12)
        : Codelet(dep, res, myTP, stat)
        , alloc()
        ,

        fired(0)
        , totalTPLoops(totalTPLoops)
        , codeletsPerTP(codeletsPerTP)
        , toSignal_(toSig)
        , loop_(alloc::allocate(totalTPLoops))
    {
        tpLoopFactory funct = &TPLoopFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12>;
        closure = new TPLoopClosure12<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12>(funct, 0, this, totalTPLoops, codeletsPerTP, A1, A2, A3, A4, A5,
            A6, A7, A8, A9, A10,
            A11, A12);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13>
    TPLoopMasterCodelet(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, int64_t totalTPLoops, int64_t codeletsPerTP,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13)
        : Codelet(dep, res, myTP, stat)
        , alloc()
        ,

        fired(0)
        , totalTPLoops(totalTPLoops)
        , codeletsPerTP(codeletsPerTP)
        , toSignal_(toSig)
        , loop_(alloc::allocate(totalTPLoops))
    {
        tpLoopFactory funct = &TPLoopFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13>;
        closure = new TPLoopClosure13<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13>(funct, 0, this, totalTPLoops, codeletsPerTP, A1, A2, A3, A4, A5,
            A6, A7, A8, A9, A10,
            A11, A12, A13);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14>
    TPLoopMasterCodelet(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, int64_t totalTPLoops, int64_t codeletsPerTP,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14)
        : Codelet(dep, res, myTP, stat)
        , alloc()
        ,

        fired(0)
        , totalTPLoops(totalTPLoops)
        , codeletsPerTP(codeletsPerTP)
        , toSignal_(toSig)
        , loop_(alloc::allocate(totalTPLoops))
    {
        tpLoopFactory funct = &TPLoopFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14>;
        closure = new TPLoopClosure14<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14>(funct, 0, this, totalTPLoops, codeletsPerTP, A1, A2, A3, A4, A5,
            A6, A7, A8, A9, A10,
            A11, A12, A13, A14);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14, class arg15>
    TPLoopMasterCodelet(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, int64_t totalTPLoops, int64_t codeletsPerTP,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15)
        : Codelet(dep, res, myTP, stat)
        , alloc()
        ,

        fired(0)
        , totalTPLoops(totalTPLoops)
        , codeletsPerTP(codeletsPerTP)
        , toSignal_(toSig)
        , loop_(alloc::allocate(totalTPLoops))
    {
        tpLoopFactory funct = &TPLoopFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15>;
        closure = new TPLoopClosure15<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15>(funct, 0, this, totalTPLoops, codeletsPerTP, A1, A2, A3, A4, A5,
            A6, A7, A8, A9, A10,
            A11, A12, A13, A14, A15);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14, class arg15,
        class arg16>
    TPLoopMasterCodelet(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, int64_t totalTPLoops, int64_t codeletsPerTP,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16)
        : Codelet(dep, res, myTP, stat)
        , alloc()
        ,

        fired(0)
        , totalTPLoops(totalTPLoops)
        , codeletsPerTP(codeletsPerTP)
        , toSignal_(toSig)
        , loop_(alloc::allocate(totalTPLoops))
    {
        tpLoopFactory funct = &TPLoopFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16>;
        closure = new TPLoopClosure16<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16>(funct, 0, this, totalTPLoops, codeletsPerTP, A1, A2, A3, A4, A5,
            A6, A7, A8, A9, A10,
            A11, A12, A13, A14, A15,
            A16);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14, class arg15,
        class arg16, class arg17>
    TPLoopMasterCodelet(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, int64_t totalTPLoops, int64_t codeletsPerTP,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17)
        : Codelet(dep, res, myTP, stat)
        , alloc()
        ,

        fired(0)
        , totalTPLoops(totalTPLoops)
        , codeletsPerTP(codeletsPerTP)
        , toSignal_(toSig)
        , loop_(alloc::allocate(totalTPLoops))
    {
        tpLoopFactory funct = &TPLoopFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17>;
        closure = new TPLoopClosure17<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17>(funct, 0, this, totalTPLoops, codeletsPerTP, A1, A2, A3, A4, A5,
            A6, A7, A8, A9, A10,
            A11, A12, A13, A14, A15,
            A16, A17);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14, class arg15,
        class arg16, class arg17, class arg18>
    TPLoopMasterCodelet(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, int64_t totalTPLoops, int64_t codeletsPerTP,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18)
        : Codelet(dep, res, myTP, stat)
        , alloc()
        ,

        fired(0)
        , totalTPLoops(totalTPLoops)
        , codeletsPerTP(codeletsPerTP)
        , toSignal_(toSig)
        , loop_(alloc::allocate(totalTPLoops))
    {
        tpLoopFactory funct = &TPLoopFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18>;
        closure = new TPLoopClosure18<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18>(funct, 0, this, totalTPLoops, codeletsPerTP, A1, A2, A3, A4, A5,
            A6, A7, A8, A9, A10,
            A11, A12, A13, A14, A15,
            A16, A17, A18);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14, class arg15,
        class arg16, class arg17, class arg18, class arg19>
    TPLoopMasterCodelet(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, int64_t totalTPLoops, int64_t codeletsPerTP,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19)
        : Codelet(dep, res, myTP, stat)
        , alloc()
        ,

        fired(0)
        , totalTPLoops(totalTPLoops)
        , codeletsPerTP(codeletsPerTP)
        , toSignal_(toSig)
        , loop_(alloc::allocate(totalTPLoops))
    {
        tpLoopFactory funct = &TPLoopFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19>;
        closure = new TPLoopClosure19<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19>(funct, 0, this, totalTPLoops, codeletsPerTP, A1, A2, A3, A4, A5,
            A6, A7, A8, A9, A10,
            A11, A12, A13, A14, A15,
            A16, A17, A18, A19);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14, class arg15,
        class arg16, class arg17, class arg18, class arg19, class arg20>
    TPLoopMasterCodelet(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, int64_t totalTPLoops, int64_t codeletsPerTP,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20)
        : Codelet(dep, res, myTP, stat)
        , alloc()
        ,

        fired(0)
        , totalTPLoops(totalTPLoops)
        , codeletsPerTP(codeletsPerTP)
        , toSignal_(toSig)
        , loop_(alloc::allocate(totalTPLoops))
    {
        tpLoopFactory funct = &TPLoopFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20>;
        closure = new TPLoopClosure20<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20>(funct, 0, this, totalTPLoops, codeletsPerTP, A1, A2, A3, A4, A5,
            A6, A7, A8, A9, A10,
            A11, A12, A13, A14, A15,
            A16, A17, A18, A19, A20);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14, class arg15,
        class arg16, class arg17, class arg18, class arg19, class arg20,
        class arg21>
    TPLoopMasterCodelet(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, int64_t totalTPLoops, int64_t codeletsPerTP,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20,
        arg21 A21)
        : Codelet(dep, res, myTP, stat)
        , alloc()
        ,

        fired(0)
        , totalTPLoops(totalTPLoops)
        , codeletsPerTP(codeletsPerTP)
        , toSignal_(toSig)
        , loop_(alloc::allocate(totalTPLoops))
    {
        tpLoopFactory funct = &TPLoopFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21>;
        closure = new TPLoopClosure21<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21>(funct, 0, this, totalTPLoops, codeletsPerTP, A1, A2, A3, A4, A5,
            A6, A7, A8, A9, A10,
            A11, A12, A13, A14, A15,
            A16, A17, A18, A19, A20,
            A21);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14, class arg15,
        class arg16, class arg17, class arg18, class arg19, class arg20,
        class arg21, class arg22>
    TPLoopMasterCodelet(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, int64_t totalTPLoops, int64_t codeletsPerTP,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20,
        arg21 A21, arg22 A22)
        : Codelet(dep, res, myTP, stat)
        , alloc()
        ,

        fired(0)
        , totalTPLoops(totalTPLoops)
        , codeletsPerTP(codeletsPerTP)
        , toSignal_(toSig)
        , loop_(alloc::allocate(totalTPLoops))
    {
        tpLoopFactory funct = &TPLoopFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22>;
        closure = new TPLoopClosure22<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22>(funct, 0, this, totalTPLoops, codeletsPerTP, A1, A2, A3, A4, A5,
            A6, A7, A8, A9, A10,
            A11, A12, A13, A14, A15,
            A16, A17, A18, A19, A20,
            A21, A22);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14, class arg15,
        class arg16, class arg17, class arg18, class arg19, class arg20,
        class arg21, class arg22, class arg23>
    TPLoopMasterCodelet(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, int64_t totalTPLoops, int64_t codeletsPerTP,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20,
        arg21 A21, arg22 A22, arg23 A23)
        : Codelet(dep, res, myTP, stat)
        , alloc()
        ,

        fired(0)
        , totalTPLoops(totalTPLoops)
        , codeletsPerTP(codeletsPerTP)
        , toSignal_(toSig)
        , loop_(alloc::allocate(totalTPLoops))
    {
        tpLoopFactory funct = &TPLoopFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23>;
        closure = new TPLoopClosure23<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23>(funct, 0, this, totalTPLoops, codeletsPerTP, A1, A2, A3, A4, A5,
            A6, A7, A8, A9, A10,
            A11, A12, A13, A14, A15,
            A16, A17, A18, A19, A20,
            A21, A22, A23);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14, class arg15,
        class arg16, class arg17, class arg18, class arg19, class arg20,
        class arg21, class arg22, class arg23, class arg24>
    TPLoopMasterCodelet(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, int64_t totalTPLoops, int64_t codeletsPerTP,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20,
        arg21 A21, arg22 A22, arg23 A23, arg24 A24)
        : Codelet(dep, res, myTP, stat)
        , alloc()
        ,

        fired(0)
        , totalTPLoops(totalTPLoops)
        , codeletsPerTP(codeletsPerTP)
        , toSignal_(toSig)
        , loop_(alloc::allocate(totalTPLoops))
    {
        tpLoopFactory funct = &TPLoopFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24>;
        closure = new TPLoopClosure24<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24>(funct, 0, this, totalTPLoops, codeletsPerTP, A1, A2, A3, A4, A5,
            A6, A7, A8, A9, A10,
            A11, A12, A13, A14, A15,
            A16, A17, A18, A19, A20,
            A21, A22, A23, A24);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14, class arg15,
        class arg16, class arg17, class arg18, class arg19, class arg20,
        class arg21, class arg22, class arg23, class arg24, class arg25>
    TPLoopMasterCodelet(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, int64_t totalTPLoops, int64_t codeletsPerTP,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20,
        arg21 A21, arg22 A22, arg23 A23, arg24 A24, arg25 A25)
        : Codelet(dep, res, myTP, stat)
        , alloc()
        ,

        fired(0)
        , totalTPLoops(totalTPLoops)
        , codeletsPerTP(codeletsPerTP)
        , toSignal_(toSig)
        , loop_(alloc::allocate(totalTPLoops))
    {
        tpLoopFactory funct = &TPLoopFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25>;
        closure = new TPLoopClosure25<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25>(funct, 0, this, totalTPLoops, codeletsPerTP, A1, A2, A3, A4, A5,
            A6, A7, A8, A9, A10,
            A11, A12, A13, A14, A15,
            A16, A17, A18, A19, A20,
            A21, A22, A23, A24, A25);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14, class arg15,
        class arg16, class arg17, class arg18, class arg19, class arg20,
        class arg21, class arg22, class arg23, class arg24, class arg25,
        class arg26>
    TPLoopMasterCodelet(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, int64_t totalTPLoops, int64_t codeletsPerTP,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20,
        arg21 A21, arg22 A22, arg23 A23, arg24 A24, arg25 A25,
        arg26 A26)
        : Codelet(dep, res, myTP, stat)
        , alloc()
        ,

        fired(0)
        , totalTPLoops(totalTPLoops)
        , codeletsPerTP(codeletsPerTP)
        , toSignal_(toSig)
        , loop_(alloc::allocate(totalTPLoops))
    {
        tpLoopFactory funct = &TPLoopFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25,
            arg26>;
        closure = new TPLoopClosure26<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25,
            arg26>(funct, 0, this, totalTPLoops, codeletsPerTP, A1, A2, A3, A4, A5,
            A6, A7, A8, A9, A10,
            A11, A12, A13, A14, A15,
            A16, A17, A18, A19, A20,
            A21, A22, A23, A24, A25,
            A26);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14, class arg15,
        class arg16, class arg17, class arg18, class arg19, class arg20,
        class arg21, class arg22, class arg23, class arg24, class arg25,
        class arg26, class arg27>
    TPLoopMasterCodelet(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, int64_t totalTPLoops, int64_t codeletsPerTP,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20,
        arg21 A21, arg22 A22, arg23 A23, arg24 A24, arg25 A25,
        arg26 A26, arg27 A27)
        : Codelet(dep, res, myTP, stat)
        , alloc()
        ,

        fired(0)
        , totalTPLoops(totalTPLoops)
        , codeletsPerTP(codeletsPerTP)
        , toSignal_(toSig)
        , loop_(alloc::allocate(totalTPLoops))
    {
        tpLoopFactory funct = &TPLoopFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25,
            arg26, arg27>;
        closure = new TPLoopClosure27<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25,
            arg26, arg27>(funct, 0, this, totalTPLoops, codeletsPerTP, A1, A2, A3, A4, A5,
            A6, A7, A8, A9, A10,
            A11, A12, A13, A14, A15,
            A16, A17, A18, A19, A20,
            A21, A22, A23, A24, A25,
            A26, A27);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14, class arg15,
        class arg16, class arg17, class arg18, class arg19, class arg20,
        class arg21, class arg22, class arg23, class arg24, class arg25,
        class arg26, class arg27, class arg28>
    TPLoopMasterCodelet(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, int64_t totalTPLoops, int64_t codeletsPerTP,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20,
        arg21 A21, arg22 A22, arg23 A23, arg24 A24, arg25 A25,
        arg26 A26, arg27 A27, arg28 A28)
        : Codelet(dep, res, myTP, stat)
        , alloc()
        ,

        fired(0)
        , totalTPLoops(totalTPLoops)
        , codeletsPerTP(codeletsPerTP)
        , toSignal_(toSig)
        , loop_(alloc::allocate(totalTPLoops))
    {
        tpLoopFactory funct = &TPLoopFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25,
            arg26, arg27, arg28>;
        closure = new TPLoopClosure28<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25,
            arg26, arg27, arg28>(funct, 0, this, totalTPLoops, codeletsPerTP, A1, A2, A3, A4, A5,
            A6, A7, A8, A9, A10,
            A11, A12, A13, A14, A15,
            A16, A17, A18, A19, A20,
            A21, A22, A23, A24, A25,
            A26, A27, A28);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14, class arg15,
        class arg16, class arg17, class arg18, class arg19, class arg20,
        class arg21, class arg22, class arg23, class arg24, class arg25,
        class arg26, class arg27, class arg28, class arg29>
    TPLoopMasterCodelet(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, int64_t totalTPLoops, int64_t codeletsPerTP,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20,
        arg21 A21, arg22 A22, arg23 A23, arg24 A24, arg25 A25,
        arg26 A26, arg27 A27, arg28 A28, arg29 A29)
        : Codelet(dep, res, myTP, stat)
        , alloc()
        ,

        fired(0)
        , totalTPLoops(totalTPLoops)
        , codeletsPerTP(codeletsPerTP)
        , toSignal_(toSig)
        , loop_(alloc::allocate(totalTPLoops))
    {
        tpLoopFactory funct = &TPLoopFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25,
            arg26, arg27, arg28, arg29>;
        closure = new TPLoopClosure29<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25,
            arg26, arg27, arg28, arg29>(funct, 0, this, totalTPLoops, codeletsPerTP, A1, A2, A3, A4, A5,
            A6, A7, A8, A9, A10,
            A11, A12, A13, A14, A15,
            A16, A17, A18, A19, A20,
            A21, A22, A23, A24, A25,
            A26, A27, A28, A29);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14, class arg15,
        class arg16, class arg17, class arg18, class arg19, class arg20,
        class arg21, class arg22, class arg23, class arg24, class arg25,
        class arg26, class arg27, class arg28, class arg29, class arg30>
    TPLoopMasterCodelet(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, int64_t totalTPLoops, int64_t codeletsPerTP,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20,
        arg21 A21, arg22 A22, arg23 A23, arg24 A24, arg25 A25,
        arg26 A26, arg27 A27, arg28 A28, arg29 A29, arg30 A30)
        : Codelet(dep, res, myTP, stat)
        , alloc()
        ,

        fired(0)
        , totalTPLoops(totalTPLoops)
        , codeletsPerTP(codeletsPerTP)
        , toSignal_(toSig)
        , loop_(alloc::allocate(totalTPLoops))
    {
        tpLoopFactory funct = &TPLoopFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25,
            arg26, arg27, arg28, arg29, arg30>;
        closure = new TPLoopClosure30<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25,
            arg26, arg27, arg28, arg29, arg30>(funct, 0, this, totalTPLoops, codeletsPerTP, A1, A2, A3, A4, A5,
            A6, A7, A8, A9, A10,
            A11, A12, A13, A14, A15,
            A16, A17, A18, A19, A20,
            A21, A22, A23, A24, A25,
            A26, A27, A28, A29, A30);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14, class arg15,
        class arg16, class arg17, class arg18, class arg19, class arg20,
        class arg21, class arg22, class arg23, class arg24, class arg25,
        class arg26, class arg27, class arg28, class arg29, class arg30,
        class arg31>
    TPLoopMasterCodelet(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, int64_t totalTPLoops, int64_t codeletsPerTP,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20,
        arg21 A21, arg22 A22, arg23 A23, arg24 A24, arg25 A25,
        arg26 A26, arg27 A27, arg28 A28, arg29 A29, arg30 A30,
        arg31 A31)
        : Codelet(dep, res, myTP, stat)
        , alloc()
        ,

        fired(0)
        , totalTPLoops(totalTPLoops)
        , codeletsPerTP(codeletsPerTP)
        , toSignal_(toSig)
        , loop_(alloc::allocate(totalTPLoops))
    {
        tpLoopFactory funct = &TPLoopFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25,
            arg26, arg27, arg28, arg29, arg30,
            arg31>;
        closure = new TPLoopClosure31<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25,
            arg26, arg27, arg28, arg29, arg30,
            arg31>(funct, 0, this, totalTPLoops, codeletsPerTP, A1, A2, A3, A4, A5,
            A6, A7, A8, A9, A10,
            A11, A12, A13, A14, A15,
            A16, A17, A18, A19, A20,
            A21, A22, A23, A24, A25,
            A26, A27, A28, A29, A30,
            A31);
    }

    template <class arg1, class arg2, class arg3, class arg4, class arg5,
        class arg6, class arg7, class arg8, class arg9, class arg10,
        class arg11, class arg12, class arg13, class arg14, class arg15,
        class arg16, class arg17, class arg18, class arg19, class arg20,
        class arg21, class arg22, class arg23, class arg24, class arg25,
        class arg26, class arg27, class arg28, class arg29, class arg30,
        class arg31, class arg32>
    TPLoopMasterCodelet(uint32_t dep, uint32_t res, ThreadedProcedure* myTP, uint32_t stat, Codelet* toSig, int64_t totalTPLoops, int64_t codeletsPerTP,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20,
        arg21 A21, arg22 A22, arg23 A23, arg24 A24, arg25 A25,
        arg26 A26, arg27 A27, arg28 A28, arg29 A29, arg30 A30,
        arg31 A31, arg32 A32)
        : Codelet(dep, res, myTP, stat)
        , alloc()
        ,

        fired(0)
        , totalTPLoops(totalTPLoops)
        , codeletsPerTP(codeletsPerTP)
        , toSignal_(toSig)
        , loop_(alloc::allocate(totalTPLoops))
    {
        tpLoopFactory funct = &TPLoopFactory<LP, arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25,
            arg26, arg27, arg28, arg29, arg30,
            arg31, arg32>;
        closure = new TPLoopClosure32<arg1, arg2, arg3, arg4, arg5,
            arg6, arg7, arg8, arg9, arg10,
            arg11, arg12, arg13, arg14, arg15,
            arg16, arg17, arg18, arg19, arg20,
            arg21, arg22, arg23, arg24, arg25,
            arg26, arg27, arg28, arg29, arg30,
            arg31, arg32>(funct, 0, this, totalTPLoops, codeletsPerTP, A1, A2, A3, A4, A5,
            A6, A7, A8, A9, A10,
            A11, A12, A13, A14, A15,
            A16, A17, A18, A19, A20,
            A21, A22, A23, A24, A25,
            A26, A27, A28, A29, A30,
            A31, A32);
    }
};
/*==============================================*/

} //namespace

#endif /* LOOP_H */
