#ifndef TPCLOSURE_H
#define TPCLOSURE_H
namespace darts {
class ThreadedProcedure;
class ABCScheduler;
class tpClosure;

typedef ThreadedProcedure* (*tpfactory)(tpClosure*);

class tpClosure {
public:
    tpfactory factory;
    ThreadedProcedure* parent;
    uint8_t* newTPptr;

    tpClosure(void)
    {
    }

    tpClosure(tpfactory tpf, ThreadedProcedure* daddy, uint8_t* in_newTPptr = nullptr)
        : factory(tpf)
        , parent(daddy)
        , newTPptr(in_newTPptr)
    {
    }

    virtual tpClosure* clone(void) { return new tpClosure(*this); }

    virtual ~tpClosure() {}
};

template <class arg1>
class tpClosure1 : public tpClosure {
public:
    arg1 a1;

    tpClosure1(void)
    {
    }

    tpClosure1(tpfactory tpf, ThreadedProcedure* daddy,
        arg1 A1, uint8_t* in_newTPptr = nullptr)
        : tpClosure(tpf, daddy, in_newTPptr)
        , a1(A1)
    {
    }

    virtual tpClosure1* clone(void) { return new tpClosure1(*this); }

    virtual ~tpClosure1() {}
};

template <class arg1, class arg2>
class tpClosure2 : public tpClosure {
public:
    arg1 a1;
    arg2 a2;

    tpClosure2(void)
    {
    }

    tpClosure2(tpfactory tpf, ThreadedProcedure* daddy,
        arg1 A1, arg2 A2, uint8_t* in_newTPptr = nullptr)
        : tpClosure(tpf, daddy, in_newTPptr)
        , a1(A1)
        , a2(A2)
    {
    }

    virtual tpClosure2* clone(void) { return new tpClosure2(*this); }

    virtual ~tpClosure2() {}
};

template <class arg1, class arg2, class arg3>
class tpClosure3 : public tpClosure {
public:
    arg1 a1;
    arg2 a2;
    arg3 a3;

    tpClosure3(void)
    {
    }

    tpClosure3(tpfactory tpf, ThreadedProcedure* daddy,
        arg1 A1, arg2 A2, arg3 A3, uint8_t* in_newTPptr = nullptr)
        : tpClosure(tpf, daddy, in_newTPptr)
        , a1(A1)
        , a2(A2)
        , a3(A3)
    {
    }

    virtual tpClosure3* clone(void) { return new tpClosure3(*this); }

    virtual ~tpClosure3() {}
};

template <class arg1, class arg2, class arg3, class arg4>
class tpClosure4 : public tpClosure {
public:
    arg1 a1;
    arg2 a2;
    arg3 a3;
    arg4 a4;

    tpClosure4(void)
    {
    }

    tpClosure4(tpfactory tpf, ThreadedProcedure* daddy,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, uint8_t* in_newTPptr = nullptr)
        : tpClosure(tpf, daddy, in_newTPptr)
        , a1(A1)
        , a2(A2)
        , a3(A3)
        , a4(A4)
    {
    }

    virtual tpClosure4* clone(void) { return new tpClosure4(*this); }

    virtual ~tpClosure4() {}
};

template <class arg1, class arg2, class arg3, class arg4, class arg5>
class tpClosure5 : public tpClosure {
public:
    arg1 a1;
    arg2 a2;
    arg3 a3;
    arg4 a4;
    arg5 a5;

    tpClosure5(void)
    {
    }

    tpClosure5(tpfactory tpf, ThreadedProcedure* daddy,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5, uint8_t* in_newTPptr = nullptr)
        : tpClosure(tpf, daddy, in_newTPptr)
        , a1(A1)
        , a2(A2)
        , a3(A3)
        , a4(A4)
        , a5(A5)
    {
    }

    virtual tpClosure5* clone(void) { return new tpClosure5(*this); }

    virtual ~tpClosure5() {}
};

template <class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6>
class tpClosure6 : public tpClosure {
public:
    arg1 a1;
    arg2 a2;
    arg3 a3;
    arg4 a4;
    arg5 a5;
    arg6 a6;

    tpClosure6(void)
    {
    }

    tpClosure6(tpfactory tpf, ThreadedProcedure* daddy,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, uint8_t* in_newTPptr = nullptr)
        : tpClosure(tpf, daddy, in_newTPptr)
        , a1(A1)
        , a2(A2)
        , a3(A3)
        , a4(A4)
        , a5(A5)
        , a6(A6)
    {
    }

    virtual tpClosure6* clone(void) { return new tpClosure6(*this); }

    virtual ~tpClosure6() {}
};

template <class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7>
class tpClosure7 : public tpClosure {
public:
    arg1 a1;
    arg2 a2;
    arg3 a3;
    arg4 a4;
    arg5 a5;
    arg6 a6;
    arg7 a7;

    tpClosure7(void)
    {
    }

    tpClosure7(tpfactory tpf, ThreadedProcedure* daddy,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, uint8_t* in_newTPptr = nullptr)
        : tpClosure(tpf, daddy, in_newTPptr)
        , a1(A1)
        , a2(A2)
        , a3(A3)
        , a4(A4)
        , a5(A5)
        , a6(A6)
        , a7(A7)
    {
    }

    virtual tpClosure7* clone(void) { return new tpClosure7(*this); }

    virtual ~tpClosure7() {}
};

template <class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8>
class tpClosure8 : public tpClosure {
public:
    arg1 a1;
    arg2 a2;
    arg3 a3;
    arg4 a4;
    arg5 a5;
    arg6 a6;
    arg7 a7;
    arg8 a8;

    tpClosure8(void)
    {
    }

    tpClosure8(tpfactory tpf, ThreadedProcedure* daddy,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, uint8_t* in_newTPptr = nullptr)
        : tpClosure(tpf, daddy, in_newTPptr)
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

    virtual tpClosure8* clone(void) { return new tpClosure8(*this); }

    virtual ~tpClosure8() {}
};

template <class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9>
class tpClosure9 : public tpClosure {
public:
    arg1 a1;
    arg2 a2;
    arg3 a3;
    arg4 a4;
    arg5 a5;
    arg6 a6;
    arg7 a7;
    arg8 a8;
    arg9 a9;

    tpClosure9(void)
    {
    }

    tpClosure9(tpfactory tpf, ThreadedProcedure* daddy,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, uint8_t* in_newTPptr = nullptr)
        : tpClosure(tpf, daddy, in_newTPptr)
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

    virtual tpClosure9* clone(void) { return new tpClosure9(*this); }

    virtual ~tpClosure9() {}
};

template <class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10>
class tpClosure10 : public tpClosure {
public:
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

    tpClosure10(void)
    {
    }

    tpClosure10(tpfactory tpf, ThreadedProcedure* daddy,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10, uint8_t* in_newTPptr = nullptr)
        : tpClosure(tpf, daddy, in_newTPptr)
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

    virtual tpClosure10* clone(void) { return new tpClosure10(*this); }

    virtual ~tpClosure10() {}
};

template <class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11>
class tpClosure11 : public tpClosure {
public:
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

    tpClosure11(void)
    {
    }

    tpClosure11(tpfactory tpf, ThreadedProcedure* daddy,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, uint8_t* in_newTPptr = nullptr)
        : tpClosure(tpf, daddy, in_newTPptr)
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

    virtual tpClosure11* clone(void) { return new tpClosure11(*this); }

    virtual ~tpClosure11() {}
};

template <class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11, class arg12>
class tpClosure12 : public tpClosure {
public:
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

    tpClosure12(void)
    {
    }

    tpClosure12(tpfactory tpf, ThreadedProcedure* daddy,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, uint8_t* in_newTPptr = nullptr)
        : tpClosure(tpf, daddy, in_newTPptr)
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

    virtual tpClosure12* clone(void) { return new tpClosure12(*this); }

    virtual ~tpClosure12() {}
};

template <class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11, class arg12, class arg13>
class tpClosure13 : public tpClosure {
public:
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

    tpClosure13(void)
    {
    }

    tpClosure13(tpfactory tpf, ThreadedProcedure* daddy,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, uint8_t* in_newTPptr = nullptr)
        : tpClosure(tpf, daddy, in_newTPptr)
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

    virtual tpClosure13* clone(void) { return new tpClosure13(*this); }

    virtual ~tpClosure13() {}
};

template <class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11, class arg12, class arg13, class arg14>
class tpClosure14 : public tpClosure {
public:
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

    tpClosure14(void)
    {
    }

    tpClosure14(tpfactory tpf, ThreadedProcedure* daddy,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, uint8_t* in_newTPptr = nullptr)
        : tpClosure(tpf, daddy, in_newTPptr)
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

    virtual tpClosure14* clone(void) { return new tpClosure14(*this); }

    virtual ~tpClosure14() {}
};

template <class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11, class arg12, class arg13, class arg14, class arg15>
class tpClosure15 : public tpClosure {
public:
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

    tpClosure15(void)
    {
    }

    tpClosure15(tpfactory tpf, ThreadedProcedure* daddy,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15, uint8_t* in_newTPptr = nullptr)
        : tpClosure(tpf, daddy, in_newTPptr)
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

    virtual tpClosure15* clone(void) { return new tpClosure15(*this); }

    virtual ~tpClosure15() {}
};

template <class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11, class arg12, class arg13, class arg14, class arg15,
    class arg16>
class tpClosure16 : public tpClosure {
public:
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

    tpClosure16(void)
    {
    }

    tpClosure16(tpfactory tpf, ThreadedProcedure* daddy,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, uint8_t* in_newTPptr = nullptr)
        : tpClosure(tpf, daddy, in_newTPptr)
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

    virtual tpClosure16* clone(void) { return new tpClosure16(*this); }

    virtual ~tpClosure16() {}
};

template <class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11, class arg12, class arg13, class arg14, class arg15,
    class arg16, class arg17>
class tpClosure17 : public tpClosure {
public:
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

    tpClosure17(void)
    {
    }

    tpClosure17(tpfactory tpf, ThreadedProcedure* daddy,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, uint8_t* in_newTPptr = nullptr)
        : tpClosure(tpf, daddy, in_newTPptr)
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

    virtual tpClosure17* clone(void) { return new tpClosure17(*this); }

    virtual ~tpClosure17() {}
};

template <class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11, class arg12, class arg13, class arg14, class arg15,
    class arg16, class arg17, class arg18>
class tpClosure18 : public tpClosure {
public:
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

    tpClosure18(void)
    {
    }

    tpClosure18(tpfactory tpf, ThreadedProcedure* daddy,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, uint8_t* in_newTPptr = nullptr)
        : tpClosure(tpf, daddy, in_newTPptr)
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

    virtual tpClosure18* clone(void) { return new tpClosure18(*this); }

    virtual ~tpClosure18() {}
};

template <class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11, class arg12, class arg13, class arg14, class arg15,
    class arg16, class arg17, class arg18, class arg19>
class tpClosure19 : public tpClosure {
public:
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

    tpClosure19(void)
    {
    }

    tpClosure19(tpfactory tpf, ThreadedProcedure* daddy,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, uint8_t* in_newTPptr = nullptr)
        : tpClosure(tpf, daddy, in_newTPptr)
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

    virtual tpClosure19* clone(void) { return new tpClosure19(*this); }

    virtual ~tpClosure19() {}
};

template <class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11, class arg12, class arg13, class arg14, class arg15,
    class arg16, class arg17, class arg18, class arg19, class arg20>
class tpClosure20 : public tpClosure {
public:
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

    tpClosure20(void)
    {
    }

    tpClosure20(tpfactory tpf, ThreadedProcedure* daddy,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20, uint8_t* in_newTPptr = nullptr)
        : tpClosure(tpf, daddy, in_newTPptr)
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

    virtual tpClosure20* clone(void) { return new tpClosure20(*this); }

    virtual ~tpClosure20() {}
};

template <class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11, class arg12, class arg13, class arg14, class arg15,
    class arg16, class arg17, class arg18, class arg19, class arg20,
    class arg21>
class tpClosure21 : public tpClosure {
public:
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

    tpClosure21(void)
    {
    }

    tpClosure21(tpfactory tpf, ThreadedProcedure* daddy,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20,
        arg21 A21, uint8_t* in_newTPptr = nullptr)
        : tpClosure(tpf, daddy, in_newTPptr)
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

    virtual tpClosure21* clone(void) { return new tpClosure21(*this); }

    virtual ~tpClosure21() {}
};

template <class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11, class arg12, class arg13, class arg14, class arg15,
    class arg16, class arg17, class arg18, class arg19, class arg20,
    class arg21, class arg22>
class tpClosure22 : public tpClosure {
public:
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

    tpClosure22(void)
    {
    }

    tpClosure22(tpfactory tpf, ThreadedProcedure* daddy,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20,
        arg21 A21, arg22 A22, uint8_t* in_newTPptr = nullptr)
        : tpClosure(tpf, daddy, in_newTPptr)
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

    virtual tpClosure22* clone(void) { return new tpClosure22(*this); }

    virtual ~tpClosure22() {}
};

template <class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11, class arg12, class arg13, class arg14, class arg15,
    class arg16, class arg17, class arg18, class arg19, class arg20,
    class arg21, class arg22, class arg23>
class tpClosure23 : public tpClosure {
public:
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

    tpClosure23(void)
    {
    }

    tpClosure23(tpfactory tpf, ThreadedProcedure* daddy,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20,
        arg21 A21, arg22 A22, arg23 A23, uint8_t* in_newTPptr = nullptr)
        : tpClosure(tpf, daddy, in_newTPptr)
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

    virtual tpClosure23* clone(void) { return new tpClosure23(*this); }

    virtual ~tpClosure23() {}
};

template <class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11, class arg12, class arg13, class arg14, class arg15,
    class arg16, class arg17, class arg18, class arg19, class arg20,
    class arg21, class arg22, class arg23, class arg24>
class tpClosure24 : public tpClosure {
public:
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

    tpClosure24(void)
    {
    }

    tpClosure24(tpfactory tpf, ThreadedProcedure* daddy,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20,
        arg21 A21, arg22 A22, arg23 A23, arg24 A24, uint8_t* in_newTPptr = nullptr)
        : tpClosure(tpf, daddy, in_newTPptr)
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

    virtual tpClosure24* clone(void) { return new tpClosure24(*this); }

    virtual ~tpClosure24() {}
};

template <class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11, class arg12, class arg13, class arg14, class arg15,
    class arg16, class arg17, class arg18, class arg19, class arg20,
    class arg21, class arg22, class arg23, class arg24, class arg25>
class tpClosure25 : public tpClosure {
public:
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

    tpClosure25(void)
    {
    }

    tpClosure25(tpfactory tpf, ThreadedProcedure* daddy,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20,
        arg21 A21, arg22 A22, arg23 A23, arg24 A24, arg25 A25, uint8_t* in_newTPptr = nullptr)
        : tpClosure(tpf, daddy, in_newTPptr)
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

    virtual tpClosure25* clone(void) { return new tpClosure25(*this); }

    virtual ~tpClosure25() {}
};

template <class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11, class arg12, class arg13, class arg14, class arg15,
    class arg16, class arg17, class arg18, class arg19, class arg20,
    class arg21, class arg22, class arg23, class arg24, class arg25,
    class arg26>
class tpClosure26 : public tpClosure {
public:
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

    tpClosure26(void)
    {
    }

    tpClosure26(tpfactory tpf, ThreadedProcedure* daddy,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20,
        arg21 A21, arg22 A22, arg23 A23, arg24 A24, arg25 A25,
        arg26 A26, uint8_t* in_newTPptr = nullptr)
        : tpClosure(tpf, daddy, in_newTPptr)
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

    virtual tpClosure26* clone(void) { return new tpClosure26(*this); }

    virtual ~tpClosure26() {}
};

template <class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11, class arg12, class arg13, class arg14, class arg15,
    class arg16, class arg17, class arg18, class arg19, class arg20,
    class arg21, class arg22, class arg23, class arg24, class arg25,
    class arg26, class arg27>
class tpClosure27 : public tpClosure {
public:
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

    tpClosure27(void)
    {
    }

    tpClosure27(tpfactory tpf, ThreadedProcedure* daddy,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20,
        arg21 A21, arg22 A22, arg23 A23, arg24 A24, arg25 A25,
        arg26 A26, arg27 A27, uint8_t* in_newTPptr = nullptr)
        : tpClosure(tpf, daddy, in_newTPptr)
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

    virtual tpClosure27* clone(void) { return new tpClosure27(*this); }

    virtual ~tpClosure27() {}
};

template <class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11, class arg12, class arg13, class arg14, class arg15,
    class arg16, class arg17, class arg18, class arg19, class arg20,
    class arg21, class arg22, class arg23, class arg24, class arg25,
    class arg26, class arg27, class arg28>
class tpClosure28 : public tpClosure {
public:
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

    tpClosure28(void)
    {
    }

    tpClosure28(tpfactory tpf, ThreadedProcedure* daddy,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20,
        arg21 A21, arg22 A22, arg23 A23, arg24 A24, arg25 A25,
        arg26 A26, arg27 A27, arg28 A28, uint8_t* in_newTPptr = nullptr)
        : tpClosure(tpf, daddy, in_newTPptr)
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

    virtual tpClosure28* clone(void) { return new tpClosure28(*this); }

    virtual ~tpClosure28() {}
};

template <class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11, class arg12, class arg13, class arg14, class arg15,
    class arg16, class arg17, class arg18, class arg19, class arg20,
    class arg21, class arg22, class arg23, class arg24, class arg25,
    class arg26, class arg27, class arg28, class arg29>
class tpClosure29 : public tpClosure {
public:
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

    tpClosure29(void)
    {
    }

    tpClosure29(tpfactory tpf, ThreadedProcedure* daddy,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20,
        arg21 A21, arg22 A22, arg23 A23, arg24 A24, arg25 A25,
        arg26 A26, arg27 A27, arg28 A28, arg29 A29, uint8_t* in_newTPptr = nullptr)
        : tpClosure(tpf, daddy, in_newTPptr)
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

    virtual tpClosure29* clone(void) { return new tpClosure29(*this); }

    virtual ~tpClosure29() {}
};

template <class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11, class arg12, class arg13, class arg14, class arg15,
    class arg16, class arg17, class arg18, class arg19, class arg20,
    class arg21, class arg22, class arg23, class arg24, class arg25,
    class arg26, class arg27, class arg28, class arg29, class arg30>
class tpClosure30 : public tpClosure {
public:
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

    tpClosure30(void)
    {
    }

    tpClosure30(tpfactory tpf, ThreadedProcedure* daddy,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20,
        arg21 A21, arg22 A22, arg23 A23, arg24 A24, arg25 A25,
        arg26 A26, arg27 A27, arg28 A28, arg29 A29, arg30 A30, uint8_t* in_newTPptr = nullptr)
        : tpClosure(tpf, daddy, in_newTPptr)
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

    virtual tpClosure30* clone(void) { return new tpClosure30(*this); }

    virtual ~tpClosure30() {}
};

template <class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11, class arg12, class arg13, class arg14, class arg15,
    class arg16, class arg17, class arg18, class arg19, class arg20,
    class arg21, class arg22, class arg23, class arg24, class arg25,
    class arg26, class arg27, class arg28, class arg29, class arg30,
    class arg31>
class tpClosure31 : public tpClosure {
public:
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

    tpClosure31(void)
    {
    }

    tpClosure31(tpfactory tpf, ThreadedProcedure* daddy,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20,
        arg21 A21, arg22 A22, arg23 A23, arg24 A24, arg25 A25,
        arg26 A26, arg27 A27, arg28 A28, arg29 A29, arg30 A30,
        arg31 A31, uint8_t* in_newTPptr = nullptr)
        : tpClosure(tpf, daddy, in_newTPptr)
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

    virtual tpClosure31* clone(void) { return new tpClosure31(*this); }

    virtual ~tpClosure31() {}
};

template <class arg1, class arg2, class arg3, class arg4, class arg5,
    class arg6, class arg7, class arg8, class arg9, class arg10,
    class arg11, class arg12, class arg13, class arg14, class arg15,
    class arg16, class arg17, class arg18, class arg19, class arg20,
    class arg21, class arg22, class arg23, class arg24, class arg25,
    class arg26, class arg27, class arg28, class arg29, class arg30,
    class arg31, class arg32>
class tpClosure32 : public tpClosure {
public:
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

    tpClosure32(void)
    {
    }

    tpClosure32(tpfactory tpf, ThreadedProcedure* daddy,
        arg1 A1, arg2 A2, arg3 A3, arg4 A4, arg5 A5,
        arg6 A6, arg7 A7, arg8 A8, arg9 A9, arg10 A10,
        arg11 A11, arg12 A12, arg13 A13, arg14 A14, arg15 A15,
        arg16 A16, arg17 A17, arg18 A18, arg19 A19, arg20 A20,
        arg21 A21, arg22 A22, arg23 A23, arg24 A24, arg25 A25,
        arg26 A26, arg27 A27, arg28 A28, arg29 A29, arg30 A30,
        arg31 A31, arg32 A32, uint8_t* in_newTPptr = nullptr)
        : tpClosure(tpf, daddy, in_newTPptr)
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

    virtual tpClosure32* clone(void) { return new tpClosure32(*this); }

    virtual ~tpClosure32() {}
};
}

#endif /* TPCLOSURE_H */
