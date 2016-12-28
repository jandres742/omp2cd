#ifndef TPLOOPCLOSURE_H
#define TPLOOPCLOSURE_H

namespace darts {
class ThreadedProcedure;
class ABCScheduler;
struct TPLoopClosure;

typedef ThreadedProcedure* (*tpLoopFactory)(void*, TPLoopClosure*);

struct TPLoopClosure {
    tpLoopFactory factory;
    unsigned int mainID;
    Codelet* toSignal;
    int64_t totalTPLoops;
    int64_t codeletsPerTP;

    TPLoopClosure(void)
    {
    }

    TPLoopClosure(tpLoopFactory tpf, unsigned int it, Codelet* toSig, int64_t totalTPLoops, int64_t codeletsPerTP)
        : factory(tpf)
        , mainID(it)
        , toSignal(toSig)
        , totalTPLoops(totalTPLoops)
        , codeletsPerTP(codeletsPerTP)
    {
    }

    virtual TPLoopClosure* clone(void) { return new TPLoopClosure(*this); }
};

template <class arg1>
struct TPLoopClosure1 : TPLoopClosure {
    arg1 a1;

    TPLoopClosure1(void)
    {
    }

    TPLoopClosure1(tpLoopFactory tpf, unsigned int it, Codelet* toSig, int64_t totalTPLoops, int64_t codeletsPerTP,
        arg1 A1)
        : TPLoopClosure(tpf, it, toSig, totalTPLoops, codeletsPerTP)
        , a1(A1)
    {
    }

    virtual TPLoopClosure1* clone(void) { return new TPLoopClosure1(*this); }
};

template <class arg1, class arg2>
struct TPLoopClosure2 : TPLoopClosure {
    arg1 a1;
    arg2 a2;

    TPLoopClosure2(void)
    {
    }

    TPLoopClosure2(tpLoopFactory tpf, unsigned int it, Codelet* toSig, int64_t totalTPLoops, int64_t codeletsPerTP,
        arg1 A1, arg2 A2)
        : TPLoopClosure(tpf, it, toSig, totalTPLoops, codeletsPerTP)
        , a1(A1)
        , a2(A2)
    {
    }

    virtual TPLoopClosure2* clone(void) { return new TPLoopClosure2(*this); }
};

template <class arg1, class arg2, class arg3>
struct TPLoopClosure3 : TPLoopClosure {
    arg1 a1;
    arg2 a2;
    arg3 a3;

    TPLoopClosure3(void)
    {
    }

    TPLoopClosure3(tpLoopFactory tpf, unsigned int it, Codelet* toSig, int64_t totalTPLoops, int64_t codeletsPerTP,
        arg1 A1, arg2 A2, arg3 A3)
        : TPLoopClosure(tpf, it, toSig, totalTPLoops, codeletsPerTP)
        , a1(A1)
        , a2(A2)
        , a3(A3)
    {
    }

    virtual TPLoopClosure3* clone(void) { return new TPLoopClosure3(*this); }
};

template <class arg1, class arg2, class arg3, class arg4>
struct TPLoopClosure4 : TPLoopClosure {
    arg1 a1;
    arg2 a2;
    arg3 a3;
    arg4 a4;

    TPLoopClosure4(void)
    {
    }

    TPLoopClosure4(tpLoopFactory tpf, unsigned int it, Codelet* toSig, int64_t totalTPLoops, int64_t codeletsPerTP,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4)
        : TPLoopClosure(tpf, it, toSig, totalTPLoops, codeletsPerTP)
        , a1(A1)
        , a2(A2)
        , a3(A3)
        , a4(A4)
    {
    }

    virtual TPLoopClosure4* clone(void) { return new TPLoopClosure4(*this); }
};

template <class arg1, class arg2, class arg3, class arg4, class arg5>
struct TPLoopClosure5 : TPLoopClosure {
    arg1 a1;
    arg2 a2;
    arg3 a3;
    arg4 a4;
    arg5 a5;

    TPLoopClosure5(void)
    {
    }

    TPLoopClosure5(tpLoopFactory tpf, unsigned int it, Codelet* toSig, int64_t totalTPLoops, int64_t codeletsPerTP,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5)
        : TPLoopClosure(tpf, it, toSig, totalTPLoops, codeletsPerTP)
        , a1(A1)
        , a2(A2)
        , a3(A3)
        , a4(A4)
        , a5(A5)
    {
    }

    virtual TPLoopClosure5* clone(void) { return new TPLoopClosure5(*this); }
};

template <class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6>
struct TPLoopClosure6 : TPLoopClosure {
    arg1 a1;
    arg2 a2;
    arg3 a3;
    arg4 a4;
    arg5 a5;
    arg6 a6;

    TPLoopClosure6(void)
    {
    }

    TPLoopClosure6(tpLoopFactory tpf, unsigned int it, Codelet* toSig, int64_t totalTPLoops, int64_t codeletsPerTP,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6)
        : TPLoopClosure(tpf, it, toSig, totalTPLoops, codeletsPerTP)
        , a1(A1)
        , a2(A2)
        , a3(A3)
        , a4(A4)
        , a5(A5)
        , a6(A6)
    {
    }

    virtual TPLoopClosure6* clone(void) { return new TPLoopClosure6(*this); }
};

template <class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7>
struct TPLoopClosure7 : TPLoopClosure {
    arg1 a1;
    arg2 a2;
    arg3 a3;
    arg4 a4;
    arg5 a5;
    arg6 a6;
    arg7 a7;

    TPLoopClosure7(void)
    {
    }

    TPLoopClosure7(tpLoopFactory tpf, unsigned int it, Codelet* toSig, int64_t totalTPLoops, int64_t codeletsPerTP,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7)
        : TPLoopClosure(tpf, it, toSig, totalTPLoops, codeletsPerTP)
        , a1(A1)
        , a2(A2)
        , a3(A3)
        , a4(A4)
        , a5(A5)
        , a6(A6)
        , a7(A7)
    {
    }

    virtual TPLoopClosure7* clone(void) { return new TPLoopClosure7(*this); }
};

template <class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8>
struct TPLoopClosure8 : TPLoopClosure {
    arg1 a1;
    arg2 a2;
    arg3 a3;
    arg4 a4;
    arg5 a5;
    arg6 a6;
    arg7 a7;
    arg8 a8;

    TPLoopClosure8(void)
    {
    }

    TPLoopClosure8(tpLoopFactory tpf, unsigned int it, Codelet* toSig, int64_t totalTPLoops, int64_t codeletsPerTP,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8)
        : TPLoopClosure(tpf, it, toSig, totalTPLoops, codeletsPerTP)
        , a1(A1)
        , a2(A2)
        , a3(A3)
        , a4(A4)
        , a5(A5)
        , a6(A6)
        , a7(A7)
        , a8(A8)
    {
    }

    virtual TPLoopClosure8* clone(void) { return new TPLoopClosure8(*this); }
};

template <class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9>
struct TPLoopClosure9 : TPLoopClosure {
    arg1 a1;
    arg2 a2;
    arg3 a3;
    arg4 a4;
    arg5 a5;
    arg6 a6;
    arg7 a7;
    arg8 a8;
    arg9 a9;

    TPLoopClosure9(void)
    {
    }

    TPLoopClosure9(tpLoopFactory tpf, unsigned int it, Codelet* toSig, int64_t totalTPLoops, int64_t codeletsPerTP,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9)
        : TPLoopClosure(tpf, it, toSig, totalTPLoops, codeletsPerTP)
        , a1(A1)
        , a2(A2)
        , a3(A3)
        , a4(A4)
        , a5(A5)
        , a6(A6)
        , a7(A7)
        , a8(A8)
        , a9(A9)
    {
    }

    virtual TPLoopClosure9* clone(void) { return new TPLoopClosure9(*this); }
};

template <class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10>
struct TPLoopClosure10 : TPLoopClosure {
    arg1 a1;
    arg2 a2;
    arg3 a3;
    arg4 a4;
    arg5 a5;
    arg6 a6;
    arg7 a7;
    arg8 a8;
    arg9 a9;
    arg10 a10;

    TPLoopClosure10(void)
    {
    }

    TPLoopClosure10(tpLoopFactory tpf, unsigned int it, Codelet* toSig, int64_t totalTPLoops, int64_t codeletsPerTP,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10)
        : TPLoopClosure(tpf, it, toSig, totalTPLoops, codeletsPerTP)
        , a1(A1)
        , a2(A2)
        , a3(A3)
        , a4(A4)
        , a5(A5)
        , a6(A6)
        , a7(A7)
        , a8(A8)
        , a9(A9)
        , a10(A10)
    {
    }

    virtual TPLoopClosure10* clone(void) { return new TPLoopClosure10(*this); }
};

template <class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11>
struct TPLoopClosure11 : TPLoopClosure {
    arg1 a1;
    arg2 a2;
    arg3 a3;
    arg4 a4;
    arg5 a5;
    arg6 a6;
    arg7 a7;
    arg8 a8;
    arg9 a9;
    arg10 a10;
    arg11 a11;

    TPLoopClosure11(void)
    {
    }

    TPLoopClosure11(tpLoopFactory tpf, unsigned int it, Codelet* toSig, int64_t totalTPLoops, int64_t codeletsPerTP,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11)
        : TPLoopClosure(tpf, it, toSig, totalTPLoops, codeletsPerTP)
        , a1(A1)
        , a2(A2)
        , a3(A3)
        , a4(A4)
        , a5(A5)
        , a6(A6)
        , a7(A7)
        , a8(A8)
        , a9(A9)
        , a10(A10)
        , a11(A11)
    {
    }

    virtual TPLoopClosure11* clone(void) { return new TPLoopClosure11(*this); }
};

template <class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11, class arg12>
struct TPLoopClosure12 : TPLoopClosure {
    arg1 a1;
    arg2 a2;
    arg3 a3;
    arg4 a4;
    arg5 a5;
    arg6 a6;
    arg7 a7;
    arg8 a8;
    arg9 a9;
    arg10 a10;
    arg11 a11;
    arg12 a12;

    TPLoopClosure12(void)
    {
    }

    TPLoopClosure12(tpLoopFactory tpf, unsigned int it, Codelet* toSig, int64_t totalTPLoops, int64_t codeletsPerTP,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12)
        : TPLoopClosure(tpf, it, toSig, totalTPLoops, codeletsPerTP)
        , a1(A1)
        , a2(A2)
        , a3(A3)
        , a4(A4)
        , a5(A5)
        , a6(A6)
        , a7(A7)
        , a8(A8)
        , a9(A9)
        , a10(A10)
        , a11(A11)
        , a12(A12)
    {
    }

    virtual TPLoopClosure12* clone(void) { return new TPLoopClosure12(*this); }
};

template <class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11, class arg12, class arg13>
struct TPLoopClosure13 : TPLoopClosure {
    arg1 a1;
    arg2 a2;
    arg3 a3;
    arg4 a4;
    arg5 a5;
    arg6 a6;
    arg7 a7;
    arg8 a8;
    arg9 a9;
    arg10 a10;
    arg11 a11;
    arg12 a12;
    arg13 a13;

    TPLoopClosure13(void)
    {
    }

    TPLoopClosure13(tpLoopFactory tpf, unsigned int it, Codelet* toSig, int64_t totalTPLoops, int64_t codeletsPerTP,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13)
        : TPLoopClosure(tpf, it, toSig, totalTPLoops, codeletsPerTP)
        , a1(A1)
        , a2(A2)
        , a3(A3)
        , a4(A4)
        , a5(A5)
        , a6(A6)
        , a7(A7)
        , a8(A8)
        , a9(A9)
        , a10(A10)
        , a11(A11)
        , a12(A12)
        , a13(A13)
    {
    }

    virtual TPLoopClosure13* clone(void) { return new TPLoopClosure13(*this); }
};

template <class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11, class arg12, class arg13, class arg14>
struct TPLoopClosure14 : TPLoopClosure {
    arg1 a1;
    arg2 a2;
    arg3 a3;
    arg4 a4;
    arg5 a5;
    arg6 a6;
    arg7 a7;
    arg8 a8;
    arg9 a9;
    arg10 a10;
    arg11 a11;
    arg12 a12;
    arg13 a13;
    arg14 a14;

    TPLoopClosure14(void)
    {
    }

    TPLoopClosure14(tpLoopFactory tpf, unsigned int it, Codelet* toSig, int64_t totalTPLoops, int64_t codeletsPerTP,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14)
        : TPLoopClosure(tpf, it, toSig, totalTPLoops, codeletsPerTP)
        , a1(A1)
        , a2(A2)
        , a3(A3)
        , a4(A4)
        , a5(A5)
        , a6(A6)
        , a7(A7)
        , a8(A8)
        , a9(A9)
        , a10(A10)
        , a11(A11)
        , a12(A12)
        , a13(A13)
        , a14(A14)
    {
    }

    virtual TPLoopClosure14* clone(void) { return new TPLoopClosure14(*this); }
};

template <class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11, class arg12, class arg13, class arg14, class arg15>
struct TPLoopClosure15 : TPLoopClosure {
    arg1 a1;
    arg2 a2;
    arg3 a3;
    arg4 a4;
    arg5 a5;
    arg6 a6;
    arg7 a7;
    arg8 a8;
    arg9 a9;
    arg10 a10;
    arg11 a11;
    arg12 a12;
    arg13 a13;
    arg14 a14;
    arg15 a15;

    TPLoopClosure15(void)
    {
    }

    TPLoopClosure15(tpLoopFactory tpf, unsigned int it, Codelet* toSig, int64_t totalTPLoops, int64_t codeletsPerTP,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15)
        : TPLoopClosure(tpf, it, toSig, totalTPLoops, codeletsPerTP)
        , a1(A1)
        , a2(A2)
        , a3(A3)
        , a4(A4)
        , a5(A5)
        , a6(A6)
        , a7(A7)
        , a8(A8)
        , a9(A9)
        , a10(A10)
        , a11(A11)
        , a12(A12)
        , a13(A13)
        , a14(A14)
        , a15(A15)
    {
    }

    virtual TPLoopClosure15* clone(void) { return new TPLoopClosure15(*this); }
};

template <class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11, class arg12, class arg13, class arg14, class arg15,
    class arg16>
struct TPLoopClosure16 : TPLoopClosure {
    arg1 a1;
    arg2 a2;
    arg3 a3;
    arg4 a4;
    arg5 a5;
    arg6 a6;
    arg7 a7;
    arg8 a8;
    arg9 a9;
    arg10 a10;
    arg11 a11;
    arg12 a12;
    arg13 a13;
    arg14 a14;
    arg15 a15;
    arg16 a16;

    TPLoopClosure16(void)
    {
    }

    TPLoopClosure16(tpLoopFactory tpf, unsigned int it, Codelet* toSig, int64_t totalTPLoops, int64_t codeletsPerTP,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16)
        : TPLoopClosure(tpf, it, toSig, totalTPLoops, codeletsPerTP)
        , a1(A1)
        , a2(A2)
        , a3(A3)
        , a4(A4)
        , a5(A5)
        , a6(A6)
        , a7(A7)
        , a8(A8)
        , a9(A9)
        , a10(A10)
        , a11(A11)
        , a12(A12)
        , a13(A13)
        , a14(A14)
        , a15(A15)
        , a16(A16)
    {
    }

    virtual TPLoopClosure16* clone(void) { return new TPLoopClosure16(*this); }
};

template <class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11, class arg12, class arg13, class arg14, class arg15,
    class arg16, class arg17>
struct TPLoopClosure17 : TPLoopClosure {
    arg1 a1;
    arg2 a2;
    arg3 a3;
    arg4 a4;
    arg5 a5;
    arg6 a6;
    arg7 a7;
    arg8 a8;
    arg9 a9;
    arg10 a10;
    arg11 a11;
    arg12 a12;
    arg13 a13;
    arg14 a14;
    arg15 a15;
    arg16 a16;
    arg17 a17;

    TPLoopClosure17(void)
    {
    }

    TPLoopClosure17(tpLoopFactory tpf, unsigned int it, Codelet* toSig, int64_t totalTPLoops, int64_t codeletsPerTP,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17)
        : TPLoopClosure(tpf, it, toSig, totalTPLoops, codeletsPerTP)
        , a1(A1)
        , a2(A2)
        , a3(A3)
        , a4(A4)
        , a5(A5)
        , a6(A6)
        , a7(A7)
        , a8(A8)
        , a9(A9)
        , a10(A10)
        , a11(A11)
        , a12(A12)
        , a13(A13)
        , a14(A14)
        , a15(A15)
        , a16(A16)
        , a17(A17)
    {
    }

    virtual TPLoopClosure17* clone(void) { return new TPLoopClosure17(*this); }
};

template <class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11, class arg12, class arg13, class arg14, class arg15,
    class arg16, class arg17, class arg18>
struct TPLoopClosure18 : TPLoopClosure {
    arg1 a1;
    arg2 a2;
    arg3 a3;
    arg4 a4;
    arg5 a5;
    arg6 a6;
    arg7 a7;
    arg8 a8;
    arg9 a9;
    arg10 a10;
    arg11 a11;
    arg12 a12;
    arg13 a13;
    arg14 a14;
    arg15 a15;
    arg16 a16;
    arg17 a17;
    arg18 a18;

    TPLoopClosure18(void)
    {
    }

    TPLoopClosure18(tpLoopFactory tpf, unsigned int it, Codelet* toSig, int64_t totalTPLoops, int64_t codeletsPerTP,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18)
        : TPLoopClosure(tpf, it, toSig, totalTPLoops, codeletsPerTP)
        , a1(A1)
        , a2(A2)
        , a3(A3)
        , a4(A4)
        , a5(A5)
        , a6(A6)
        , a7(A7)
        , a8(A8)
        , a9(A9)
        , a10(A10)
        , a11(A11)
        , a12(A12)
        , a13(A13)
        , a14(A14)
        , a15(A15)
        , a16(A16)
        , a17(A17)
        , a18(A18)
    {
    }

    virtual TPLoopClosure18* clone(void) { return new TPLoopClosure18(*this); }
};

template <class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11, class arg12, class arg13, class arg14, class arg15,
    class arg16, class arg17, class arg18, class arg19>
struct TPLoopClosure19 : TPLoopClosure {
    arg1 a1;
    arg2 a2;
    arg3 a3;
    arg4 a4;
    arg5 a5;
    arg6 a6;
    arg7 a7;
    arg8 a8;
    arg9 a9;
    arg10 a10;
    arg11 a11;
    arg12 a12;
    arg13 a13;
    arg14 a14;
    arg15 a15;
    arg16 a16;
    arg17 a17;
    arg18 a18;
    arg19 a19;

    TPLoopClosure19(void)
    {
    }

    TPLoopClosure19(tpLoopFactory tpf, unsigned int it, Codelet* toSig, int64_t totalTPLoops, int64_t codeletsPerTP,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19)
        : TPLoopClosure(tpf, it, toSig, totalTPLoops, codeletsPerTP)
        , a1(A1)
        , a2(A2)
        , a3(A3)
        , a4(A4)
        , a5(A5)
        , a6(A6)
        , a7(A7)
        , a8(A8)
        , a9(A9)
        , a10(A10)
        , a11(A11)
        , a12(A12)
        , a13(A13)
        , a14(A14)
        , a15(A15)
        , a16(A16)
        , a17(A17)
        , a18(A18)
        , a19(A19)
    {
    }

    virtual TPLoopClosure19* clone(void) { return new TPLoopClosure19(*this); }
};

template <class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11, class arg12, class arg13, class arg14, class arg15,
    class arg16, class arg17, class arg18, class arg19, class arg20>
struct TPLoopClosure20 : TPLoopClosure {
    arg1 a1;
    arg2 a2;
    arg3 a3;
    arg4 a4;
    arg5 a5;
    arg6 a6;
    arg7 a7;
    arg8 a8;
    arg9 a9;
    arg10 a10;
    arg11 a11;
    arg12 a12;
    arg13 a13;
    arg14 a14;
    arg15 a15;
    arg16 a16;
    arg17 a17;
    arg18 a18;
    arg19 a19;
    arg20 a20;

    TPLoopClosure20(void)
    {
    }

    TPLoopClosure20(tpLoopFactory tpf, unsigned int it, Codelet* toSig, int64_t totalTPLoops, int64_t codeletsPerTP,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20)
        : TPLoopClosure(tpf, it, toSig, totalTPLoops, codeletsPerTP)
        , a1(A1)
        , a2(A2)
        , a3(A3)
        , a4(A4)
        , a5(A5)
        , a6(A6)
        , a7(A7)
        , a8(A8)
        , a9(A9)
        , a10(A10)
        , a11(A11)
        , a12(A12)
        , a13(A13)
        , a14(A14)
        , a15(A15)
        , a16(A16)
        , a17(A17)
        , a18(A18)
        , a19(A19)
        , a20(A20)
    {
    }

    virtual TPLoopClosure20* clone(void) { return new TPLoopClosure20(*this); }
};

template <class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11, class arg12, class arg13, class arg14, class arg15,
    class arg16, class arg17, class arg18, class arg19, class arg20,
    class arg21>
struct TPLoopClosure21 : TPLoopClosure {
    arg1 a1;
    arg2 a2;
    arg3 a3;
    arg4 a4;
    arg5 a5;
    arg6 a6;
    arg7 a7;
    arg8 a8;
    arg9 a9;
    arg10 a10;
    arg11 a11;
    arg12 a12;
    arg13 a13;
    arg14 a14;
    arg15 a15;
    arg16 a16;
    arg17 a17;
    arg18 a18;
    arg19 a19;
    arg20 a20;
    arg21 a21;

    TPLoopClosure21(void)
    {
    }

    TPLoopClosure21(tpLoopFactory tpf, unsigned int it, Codelet* toSig, int64_t totalTPLoops, int64_t codeletsPerTP,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20,
        arg21 A21)
        : TPLoopClosure(tpf, it, toSig, totalTPLoops, codeletsPerTP)
        , a1(A1)
        , a2(A2)
        , a3(A3)
        , a4(A4)
        , a5(A5)
        , a6(A6)
        , a7(A7)
        , a8(A8)
        , a9(A9)
        , a10(A10)
        , a11(A11)
        , a12(A12)
        , a13(A13)
        , a14(A14)
        , a15(A15)
        , a16(A16)
        , a17(A17)
        , a18(A18)
        , a19(A19)
        , a20(A20)
        , a21(A21)
    {
    }

    virtual TPLoopClosure21* clone(void) { return new TPLoopClosure21(*this); }
};

template <class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11, class arg12, class arg13, class arg14, class arg15,
    class arg16, class arg17, class arg18, class arg19, class arg20,
    class arg21, class arg22>
struct TPLoopClosure22 : TPLoopClosure {
    arg1 a1;
    arg2 a2;
    arg3 a3;
    arg4 a4;
    arg5 a5;
    arg6 a6;
    arg7 a7;
    arg8 a8;
    arg9 a9;
    arg10 a10;
    arg11 a11;
    arg12 a12;
    arg13 a13;
    arg14 a14;
    arg15 a15;
    arg16 a16;
    arg17 a17;
    arg18 a18;
    arg19 a19;
    arg20 a20;
    arg21 a21;
    arg22 a22;

    TPLoopClosure22(void)
    {
    }

    TPLoopClosure22(tpLoopFactory tpf, unsigned int it, Codelet* toSig, int64_t totalTPLoops, int64_t codeletsPerTP,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20,
        arg21 A21, arg22 A22)
        : TPLoopClosure(tpf, it, toSig, totalTPLoops, codeletsPerTP)
        , a1(A1)
        , a2(A2)
        , a3(A3)
        , a4(A4)
        , a5(A5)
        , a6(A6)
        , a7(A7)
        , a8(A8)
        , a9(A9)
        , a10(A10)
        , a11(A11)
        , a12(A12)
        , a13(A13)
        , a14(A14)
        , a15(A15)
        , a16(A16)
        , a17(A17)
        , a18(A18)
        , a19(A19)
        , a20(A20)
        , a21(A21)
        , a22(A22)
    {
    }

    virtual TPLoopClosure22* clone(void) { return new TPLoopClosure22(*this); }
};

template <class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11, class arg12, class arg13, class arg14, class arg15,
    class arg16, class arg17, class arg18, class arg19, class arg20,
    class arg21, class arg22, class arg23>
struct TPLoopClosure23 : TPLoopClosure {
    arg1 a1;
    arg2 a2;
    arg3 a3;
    arg4 a4;
    arg5 a5;
    arg6 a6;
    arg7 a7;
    arg8 a8;
    arg9 a9;
    arg10 a10;
    arg11 a11;
    arg12 a12;
    arg13 a13;
    arg14 a14;
    arg15 a15;
    arg16 a16;
    arg17 a17;
    arg18 a18;
    arg19 a19;
    arg20 a20;
    arg21 a21;
    arg22 a22;
    arg23 a23;

    TPLoopClosure23(void)
    {
    }

    TPLoopClosure23(tpLoopFactory tpf, unsigned int it, Codelet* toSig, int64_t totalTPLoops, int64_t codeletsPerTP,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20,
        arg21 A21, arg22 A22, arg23 A23)
        : TPLoopClosure(tpf, it, toSig, totalTPLoops, codeletsPerTP)
        , a1(A1)
        , a2(A2)
        , a3(A3)
        , a4(A4)
        , a5(A5)
        , a6(A6)
        , a7(A7)
        , a8(A8)
        , a9(A9)
        , a10(A10)
        , a11(A11)
        , a12(A12)
        , a13(A13)
        , a14(A14)
        , a15(A15)
        , a16(A16)
        , a17(A17)
        , a18(A18)
        , a19(A19)
        , a20(A20)
        , a21(A21)
        , a22(A22)
        , a23(A23)
    {
    }

    virtual TPLoopClosure23* clone(void) { return new TPLoopClosure23(*this); }
};

template <class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11, class arg12, class arg13, class arg14, class arg15,
    class arg16, class arg17, class arg18, class arg19, class arg20,
    class arg21, class arg22, class arg23, class arg24>
struct TPLoopClosure24 : TPLoopClosure {
    arg1 a1;
    arg2 a2;
    arg3 a3;
    arg4 a4;
    arg5 a5;
    arg6 a6;
    arg7 a7;
    arg8 a8;
    arg9 a9;
    arg10 a10;
    arg11 a11;
    arg12 a12;
    arg13 a13;
    arg14 a14;
    arg15 a15;
    arg16 a16;
    arg17 a17;
    arg18 a18;
    arg19 a19;
    arg20 a20;
    arg21 a21;
    arg22 a22;
    arg23 a23;
    arg24 a24;

    TPLoopClosure24(void)
    {
    }

    TPLoopClosure24(tpLoopFactory tpf, unsigned int it, Codelet* toSig, int64_t totalTPLoops, int64_t codeletsPerTP,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20,
        arg21 A21, arg22 A22, arg23 A23, arg24 A24)
        : TPLoopClosure(tpf, it, toSig, totalTPLoops, codeletsPerTP)
        , a1(A1)
        , a2(A2)
        , a3(A3)
        , a4(A4)
        , a5(A5)
        , a6(A6)
        , a7(A7)
        , a8(A8)
        , a9(A9)
        , a10(A10)
        , a11(A11)
        , a12(A12)
        , a13(A13)
        , a14(A14)
        , a15(A15)
        , a16(A16)
        , a17(A17)
        , a18(A18)
        , a19(A19)
        , a20(A20)
        , a21(A21)
        , a22(A22)
        , a23(A23)
        , a24(A24)
    {
    }

    virtual TPLoopClosure24* clone(void) { return new TPLoopClosure24(*this); }
};

template <class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11, class arg12, class arg13, class arg14, class arg15,
    class arg16, class arg17, class arg18, class arg19, class arg20,
    class arg21, class arg22, class arg23, class arg24, class arg25>
struct TPLoopClosure25 : TPLoopClosure {
    arg1 a1;
    arg2 a2;
    arg3 a3;
    arg4 a4;
    arg5 a5;
    arg6 a6;
    arg7 a7;
    arg8 a8;
    arg9 a9;
    arg10 a10;
    arg11 a11;
    arg12 a12;
    arg13 a13;
    arg14 a14;
    arg15 a15;
    arg16 a16;
    arg17 a17;
    arg18 a18;
    arg19 a19;
    arg20 a20;
    arg21 a21;
    arg22 a22;
    arg23 a23;
    arg24 a24;
    arg25 a25;

    TPLoopClosure25(void)
    {
    }

    TPLoopClosure25(tpLoopFactory tpf, unsigned int it, Codelet* toSig, int64_t totalTPLoops, int64_t codeletsPerTP,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20,
        arg21 A21, arg22 A22, arg23 A23, arg24 A24, arg25 A25)
        : TPLoopClosure(tpf, it, toSig, totalTPLoops, codeletsPerTP)
        , a1(A1)
        , a2(A2)
        , a3(A3)
        , a4(A4)
        , a5(A5)
        , a6(A6)
        , a7(A7)
        , a8(A8)
        , a9(A9)
        , a10(A10)
        , a11(A11)
        , a12(A12)
        , a13(A13)
        , a14(A14)
        , a15(A15)
        , a16(A16)
        , a17(A17)
        , a18(A18)
        , a19(A19)
        , a20(A20)
        , a21(A21)
        , a22(A22)
        , a23(A23)
        , a24(A24)
        , a25(A25)
    {
    }

    virtual TPLoopClosure25* clone(void) { return new TPLoopClosure25(*this); }
};

template <class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11, class arg12, class arg13, class arg14, class arg15,
    class arg16, class arg17, class arg18, class arg19, class arg20,
    class arg21, class arg22, class arg23, class arg24, class arg25,
    class arg26>
struct TPLoopClosure26 : TPLoopClosure {
    arg1 a1;
    arg2 a2;
    arg3 a3;
    arg4 a4;
    arg5 a5;
    arg6 a6;
    arg7 a7;
    arg8 a8;
    arg9 a9;
    arg10 a10;
    arg11 a11;
    arg12 a12;
    arg13 a13;
    arg14 a14;
    arg15 a15;
    arg16 a16;
    arg17 a17;
    arg18 a18;
    arg19 a19;
    arg20 a20;
    arg21 a21;
    arg22 a22;
    arg23 a23;
    arg24 a24;
    arg25 a25;
    arg26 a26;

    TPLoopClosure26(void)
    {
    }

    TPLoopClosure26(tpLoopFactory tpf, unsigned int it, Codelet* toSig, int64_t totalTPLoops, int64_t codeletsPerTP,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20,
        arg21 A21, arg22 A22, arg23 A23, arg24 A24, arg25 A25,
        arg26 A26)
        : TPLoopClosure(tpf, it, toSig, totalTPLoops, codeletsPerTP)
        , a1(A1)
        , a2(A2)
        , a3(A3)
        , a4(A4)
        , a5(A5)
        , a6(A6)
        , a7(A7)
        , a8(A8)
        , a9(A9)
        , a10(A10)
        , a11(A11)
        , a12(A12)
        , a13(A13)
        , a14(A14)
        , a15(A15)
        , a16(A16)
        , a17(A17)
        , a18(A18)
        , a19(A19)
        , a20(A20)
        , a21(A21)
        , a22(A22)
        , a23(A23)
        , a24(A24)
        , a25(A25)
        , a26(A26)
    {
    }

    virtual TPLoopClosure26* clone(void) { return new TPLoopClosure26(*this); }
};

template <class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11, class arg12, class arg13, class arg14, class arg15,
    class arg16, class arg17, class arg18, class arg19, class arg20,
    class arg21, class arg22, class arg23, class arg24, class arg25,
    class arg26, class arg27>
struct TPLoopClosure27 : TPLoopClosure {
    arg1 a1;
    arg2 a2;
    arg3 a3;
    arg4 a4;
    arg5 a5;
    arg6 a6;
    arg7 a7;
    arg8 a8;
    arg9 a9;
    arg10 a10;
    arg11 a11;
    arg12 a12;
    arg13 a13;
    arg14 a14;
    arg15 a15;
    arg16 a16;
    arg17 a17;
    arg18 a18;
    arg19 a19;
    arg20 a20;
    arg21 a21;
    arg22 a22;
    arg23 a23;
    arg24 a24;
    arg25 a25;
    arg26 a26;
    arg27 a27;

    TPLoopClosure27(void)
    {
    }

    TPLoopClosure27(tpLoopFactory tpf, unsigned int it, Codelet* toSig, int64_t totalTPLoops, int64_t codeletsPerTP,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20,
        arg21 A21, arg22 A22, arg23 A23, arg24 A24, arg25 A25,
        arg26 A26, arg27 A27)
        : TPLoopClosure(tpf, it, toSig, totalTPLoops, codeletsPerTP)
        , a1(A1)
        , a2(A2)
        , a3(A3)
        , a4(A4)
        , a5(A5)
        , a6(A6)
        , a7(A7)
        , a8(A8)
        , a9(A9)
        , a10(A10)
        , a11(A11)
        , a12(A12)
        , a13(A13)
        , a14(A14)
        , a15(A15)
        , a16(A16)
        , a17(A17)
        , a18(A18)
        , a19(A19)
        , a20(A20)
        , a21(A21)
        , a22(A22)
        , a23(A23)
        , a24(A24)
        , a25(A25)
        , a26(A26)
        , a27(A27)
    {
    }

    virtual TPLoopClosure27* clone(void) { return new TPLoopClosure27(*this); }
};

template <class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11, class arg12, class arg13, class arg14, class arg15,
    class arg16, class arg17, class arg18, class arg19, class arg20,
    class arg21, class arg22, class arg23, class arg24, class arg25,
    class arg26, class arg27, class arg28>
struct TPLoopClosure28 : TPLoopClosure {
    arg1 a1;
    arg2 a2;
    arg3 a3;
    arg4 a4;
    arg5 a5;
    arg6 a6;
    arg7 a7;
    arg8 a8;
    arg9 a9;
    arg10 a10;
    arg11 a11;
    arg12 a12;
    arg13 a13;
    arg14 a14;
    arg15 a15;
    arg16 a16;
    arg17 a17;
    arg18 a18;
    arg19 a19;
    arg20 a20;
    arg21 a21;
    arg22 a22;
    arg23 a23;
    arg24 a24;
    arg25 a25;
    arg26 a26;
    arg27 a27;
    arg28 a28;

    TPLoopClosure28(void)
    {
    }

    TPLoopClosure28(tpLoopFactory tpf, unsigned int it, Codelet* toSig, int64_t totalTPLoops, int64_t codeletsPerTP,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20,
        arg21 A21, arg22 A22, arg23 A23, arg24 A24, arg25 A25,
        arg26 A26, arg27 A27, arg28 A28)
        : TPLoopClosure(tpf, it, toSig, totalTPLoops, codeletsPerTP)
        , a1(A1)
        , a2(A2)
        , a3(A3)
        , a4(A4)
        , a5(A5)
        , a6(A6)
        , a7(A7)
        , a8(A8)
        , a9(A9)
        , a10(A10)
        , a11(A11)
        , a12(A12)
        , a13(A13)
        , a14(A14)
        , a15(A15)
        , a16(A16)
        , a17(A17)
        , a18(A18)
        , a19(A19)
        , a20(A20)
        , a21(A21)
        , a22(A22)
        , a23(A23)
        , a24(A24)
        , a25(A25)
        , a26(A26)
        , a27(A27)
        , a28(A28)
    {
    }

    virtual TPLoopClosure28* clone(void) { return new TPLoopClosure28(*this); }
};

template <class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11, class arg12, class arg13, class arg14, class arg15,
    class arg16, class arg17, class arg18, class arg19, class arg20,
    class arg21, class arg22, class arg23, class arg24, class arg25,
    class arg26, class arg27, class arg28, class arg29>
struct TPLoopClosure29 : TPLoopClosure {
    arg1 a1;
    arg2 a2;
    arg3 a3;
    arg4 a4;
    arg5 a5;
    arg6 a6;
    arg7 a7;
    arg8 a8;
    arg9 a9;
    arg10 a10;
    arg11 a11;
    arg12 a12;
    arg13 a13;
    arg14 a14;
    arg15 a15;
    arg16 a16;
    arg17 a17;
    arg18 a18;
    arg19 a19;
    arg20 a20;
    arg21 a21;
    arg22 a22;
    arg23 a23;
    arg24 a24;
    arg25 a25;
    arg26 a26;
    arg27 a27;
    arg28 a28;
    arg29 a29;

    TPLoopClosure29(void)
    {
    }

    TPLoopClosure29(tpLoopFactory tpf, unsigned int it, Codelet* toSig, int64_t totalTPLoops, int64_t codeletsPerTP,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20,
        arg21 A21, arg22 A22, arg23 A23, arg24 A24, arg25 A25,
        arg26 A26, arg27 A27, arg28 A28, arg29 A29)
        : TPLoopClosure(tpf, it, toSig, totalTPLoops, codeletsPerTP)
        , a1(A1)
        , a2(A2)
        , a3(A3)
        , a4(A4)
        , a5(A5)
        , a6(A6)
        , a7(A7)
        , a8(A8)
        , a9(A9)
        , a10(A10)
        , a11(A11)
        , a12(A12)
        , a13(A13)
        , a14(A14)
        , a15(A15)
        , a16(A16)
        , a17(A17)
        , a18(A18)
        , a19(A19)
        , a20(A20)
        , a21(A21)
        , a22(A22)
        , a23(A23)
        , a24(A24)
        , a25(A25)
        , a26(A26)
        , a27(A27)
        , a28(A28)
        , a29(A29)
    {
    }

    virtual TPLoopClosure29* clone(void) { return new TPLoopClosure29(*this); }
};

template <class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11, class arg12, class arg13, class arg14, class arg15,
    class arg16, class arg17, class arg18, class arg19, class arg20,
    class arg21, class arg22, class arg23, class arg24, class arg25,
    class arg26, class arg27, class arg28, class arg29, class arg30>
struct TPLoopClosure30 : TPLoopClosure {
    arg1 a1;
    arg2 a2;
    arg3 a3;
    arg4 a4;
    arg5 a5;
    arg6 a6;
    arg7 a7;
    arg8 a8;
    arg9 a9;
    arg10 a10;
    arg11 a11;
    arg12 a12;
    arg13 a13;
    arg14 a14;
    arg15 a15;
    arg16 a16;
    arg17 a17;
    arg18 a18;
    arg19 a19;
    arg20 a20;
    arg21 a21;
    arg22 a22;
    arg23 a23;
    arg24 a24;
    arg25 a25;
    arg26 a26;
    arg27 a27;
    arg28 a28;
    arg29 a29;
    arg30 a30;

    TPLoopClosure30(void)
    {
    }

    TPLoopClosure30(tpLoopFactory tpf, unsigned int it, Codelet* toSig, int64_t totalTPLoops, int64_t codeletsPerTP,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20,
        arg21 A21, arg22 A22, arg23 A23, arg24 A24, arg25 A25,
        arg26 A26, arg27 A27, arg28 A28, arg29 A29, arg30 A30)
        : TPLoopClosure(tpf, it, toSig, totalTPLoops, codeletsPerTP)
        , a1(A1)
        , a2(A2)
        , a3(A3)
        , a4(A4)
        , a5(A5)
        , a6(A6)
        , a7(A7)
        , a8(A8)
        , a9(A9)
        , a10(A10)
        , a11(A11)
        , a12(A12)
        , a13(A13)
        , a14(A14)
        , a15(A15)
        , a16(A16)
        , a17(A17)
        , a18(A18)
        , a19(A19)
        , a20(A20)
        , a21(A21)
        , a22(A22)
        , a23(A23)
        , a24(A24)
        , a25(A25)
        , a26(A26)
        , a27(A27)
        , a28(A28)
        , a29(A29)
        , a30(A30)
    {
    }

    virtual TPLoopClosure30* clone(void) { return new TPLoopClosure30(*this); }
};

template <class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11, class arg12, class arg13, class arg14, class arg15,
    class arg16, class arg17, class arg18, class arg19, class arg20,
    class arg21, class arg22, class arg23, class arg24, class arg25,
    class arg26, class arg27, class arg28, class arg29, class arg30,
    class arg31>
struct TPLoopClosure31 : TPLoopClosure {
    arg1 a1;
    arg2 a2;
    arg3 a3;
    arg4 a4;
    arg5 a5;
    arg6 a6;
    arg7 a7;
    arg8 a8;
    arg9 a9;
    arg10 a10;
    arg11 a11;
    arg12 a12;
    arg13 a13;
    arg14 a14;
    arg15 a15;
    arg16 a16;
    arg17 a17;
    arg18 a18;
    arg19 a19;
    arg20 a20;
    arg21 a21;
    arg22 a22;
    arg23 a23;
    arg24 a24;
    arg25 a25;
    arg26 a26;
    arg27 a27;
    arg28 a28;
    arg29 a29;
    arg30 a30;
    arg31 a31;

    TPLoopClosure31(void)
    {
    }

    TPLoopClosure31(tpLoopFactory tpf, unsigned int it, Codelet* toSig, int64_t totalTPLoops, int64_t codeletsPerTP,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20,
        arg21 A21, arg22 A22, arg23 A23, arg24 A24, arg25 A25,
        arg26 A26, arg27 A27, arg28 A28, arg29 A29, arg30 A30,
        arg31 A31)
        : TPLoopClosure(tpf, it, toSig, totalTPLoops, codeletsPerTP)
        , a1(A1)
        , a2(A2)
        , a3(A3)
        , a4(A4)
        , a5(A5)
        , a6(A6)
        , a7(A7)
        , a8(A8)
        , a9(A9)
        , a10(A10)
        , a11(A11)
        , a12(A12)
        , a13(A13)
        , a14(A14)
        , a15(A15)
        , a16(A16)
        , a17(A17)
        , a18(A18)
        , a19(A19)
        , a20(A20)
        , a21(A21)
        , a22(A22)
        , a23(A23)
        , a24(A24)
        , a25(A25)
        , a26(A26)
        , a27(A27)
        , a28(A28)
        , a29(A29)
        , a30(A30)
        , a31(A31)
    {
    }

    virtual TPLoopClosure31* clone(void) { return new TPLoopClosure31(*this); }
};

template <class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11, class arg12, class arg13, class arg14, class arg15,
    class arg16, class arg17, class arg18, class arg19, class arg20,
    class arg21, class arg22, class arg23, class arg24, class arg25,
    class arg26, class arg27, class arg28, class arg29, class arg30,
    class arg31, class arg32>
struct TPLoopClosure32 : TPLoopClosure {
    arg1 a1;
    arg2 a2;
    arg3 a3;
    arg4 a4;
    arg5 a5;
    arg6 a6;
    arg7 a7;
    arg8 a8;
    arg9 a9;
    arg10 a10;
    arg11 a11;
    arg12 a12;
    arg13 a13;
    arg14 a14;
    arg15 a15;
    arg16 a16;
    arg17 a17;
    arg18 a18;
    arg19 a19;
    arg20 a20;
    arg21 a21;
    arg22 a22;
    arg23 a23;
    arg24 a24;
    arg25 a25;
    arg26 a26;
    arg27 a27;
    arg28 a28;
    arg29 a29;
    arg30 a30;
    arg31 a31;
    arg32 a32;

    TPLoopClosure32(void)
    {
    }

    TPLoopClosure32(tpLoopFactory tpf, unsigned int it, Codelet* toSig, int64_t totalTPLoops, int64_t codeletsPerTP,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20,
        arg21 A21, arg22 A22, arg23 A23, arg24 A24, arg25 A25,
        arg26 A26, arg27 A27, arg28 A28, arg29 A29, arg30 A30,
        arg31 A31, arg32 A32)
        : TPLoopClosure(tpf, it, toSig, totalTPLoops, codeletsPerTP)
        , a1(A1)
        , a2(A2)
        , a3(A3)
        , a4(A4)
        , a5(A5)
        , a6(A6)
        , a7(A7)
        , a8(A8)
        , a9(A9)
        , a10(A10)
        , a11(A11)
        , a12(A12)
        , a13(A13)
        , a14(A14)
        , a15(A15)
        , a16(A16)
        , a17(A17)
        , a18(A18)
        , a19(A19)
        , a20(A20)
        , a21(A21)
        , a22(A22)
        , a23(A23)
        , a24(A24)
        , a25(A25)
        , a26(A26)
        , a27(A27)
        , a28(A28)
        , a29(A29)
        , a30(A30)
        , a31(A31)
        , a32(A32)
    {
    }

    virtual TPLoopClosure32* clone(void) { return new TPLoopClosure32(*this); }
};
}

#endif /* LOOPCLOSURE_H */
