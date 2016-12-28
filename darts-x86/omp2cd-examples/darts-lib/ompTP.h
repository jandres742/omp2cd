#ifndef _OMPTP_H_
#define _OMPTP_H_

#include "darts.h"
#include <algorithm>
#include <unordered_map>
#include <utility>
#include <vector>

extern size_t numOfCUs;
extern size_t ompNumThreads;

class ompTP : public darts::ThreadedProcedure {
public:
    int* availableCodelets;

    ompTP(int in_numThreads, int in_mainCodeletID)
        : ThreadedProcedure(in_numThreads, in_mainCodeletID)
        , availableCodelets(new int[in_numThreads])
    {
        for (int i = 0; i < in_numThreads; i++) {
            this->availableCodelets[i] = 0;
        }

    }
    ~ompTP()
    {
        delete[] availableCodelets;
    }
};

class ompOMPParallelDirectiveTP : public ompTP {
public:
    bool firstTPLaunched;

    ompOMPParallelDirectiveTP(int in_numThreads, int in_mainCodeletID, bool in_firstTPLaunched)
        : ompTP(in_numThreads, in_mainCodeletID)
        , firstTPLaunched(in_firstTPLaunched)
    {
        this->availableCodelets[mainCodeletID] = 1;

        if (this->firstTPLaunched) {
            for (int i = 0; i < in_numThreads; i++) {
                this->availableCodelets[i] = (int)(this->firstTPLaunched);
            }
        }
    }
};

class ompOMPParallelForDirectiveTP : public ompTP {
public:
    bool firstTPLaunched;

    ompOMPParallelForDirectiveTP(int in_numThreads, int in_mainCodeletID, bool in_firstTPLaunched)
        : ompTP(in_numThreads, in_mainCodeletID)
        , firstTPLaunched(in_firstTPLaunched)
    {
        this->availableCodelets[mainCodeletID] = 1;

        if (this->firstTPLaunched) {
            for (int i = 0; i < in_numThreads; i++) {
                this->availableCodelets[i] = (int)(this->firstTPLaunched);
            }
        }
    }
};

class ompOMPSingleDirectiveTP : public ompTP {
public:
    ompOMPSingleDirectiveTP(int in_numThreads, int in_mainCodeletID)
        : ompTP(in_numThreads, in_mainCodeletID)
    {
        this->availableCodelets[0] = 1;
    }
};

class ompOMPMasterDirectiveTP : public ompTP {
public:
    ompOMPMasterDirectiveTP(int in_numThreads, int in_mainCodeletID)
        : ompTP(in_numThreads, in_mainCodeletID)
    {
        this->availableCodelets[0] = 1;
    }
};

class ompOMPTaskDirectiveTP : public ompTP {
public:
    ompOMPTaskDirectiveTP(int in_numThreads, int in_mainCodeletID)
        : ompTP(in_numThreads, in_mainCodeletID)
    {
        this->availableCodelets[0] = 1;
    }
};

class ompOMPSectionsDirectiveTP : public ompTP {
public:
    ompOMPSectionsDirectiveTP(int in_numThreads, int in_mainCodeletID)
        : ompTP(in_numThreads, in_mainCodeletID)
    {
        this->availableCodelets[mainCodeletID] = 1;
    }
};

class ompOMPSectionDirectiveTP : public ompTP {
public:
    ompOMPSectionDirectiveTP(int in_numThreads, int in_mainCodeletID)
        : ompTP(in_numThreads, in_mainCodeletID)
    {
        this->availableCodelets[0] = 1;
    }
};

class ompOMPCriticalDirectiveTP : public ompTP {
public:
    ompOMPCriticalDirectiveTP(int in_numThreads, int in_mainCodeletID)
        : ompTP(in_numThreads, in_mainCodeletID)
    {
        this->availableCodelets[mainCodeletID] = 1;
    }
};

class ompOMPAtomicDirectiveTP : public ompTP {
public:
    ompOMPAtomicDirectiveTP(int in_numThreads, int in_mainCodeletID)
        : ompTP(in_numThreads, in_mainCodeletID)
    {
        this->availableCodelets[mainCodeletID] = 1;
    }
};

class ompOMPForDirectiveTP : public ompTP {
public:
    ompOMPForDirectiveTP(int in_numThreads, int in_mainCodeletID)
        : ompTP(in_numThreads, in_mainCodeletID)
    {
        this->availableCodelets[mainCodeletID] = 1;
    }
};

#endif
