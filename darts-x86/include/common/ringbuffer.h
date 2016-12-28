#ifndef RINGBUFFER_H
#define RINGBUFFER_H
#include "Atomics.h"
#include <cstdio>

namespace darts {
template <class T, size_t num>
class ringBuffer {
private:
    unsigned int produceCount;
    char pad1[64 - sizeof(int)];
    unsigned int consumeCount;
    char pad2[64 - sizeof(int)];
    T buffer_[num];

public:
    ringBuffer(void)
        : produceCount(0)
        , consumeCount(0)
    {
    }

    bool push(T toAdd)
    {
        if (produceCount - consumeCount == num)
            return false;
        buffer_[produceCount & (num - 1)] = toAdd;
        //produceCount++;
        Atomics::fetchAdd(produceCount, 1U);
        return true;
    }

    T pull(void)
    {
        if (produceCount - consumeCount == 0)
            return 0;
        T temp = buffer_[consumeCount & (num - 1)];
        //consumeCount++;
        Atomics::fetchAdd(consumeCount, 1U);
        return temp;
    }
};
}

#endif /* RINGBUFFER_H */
