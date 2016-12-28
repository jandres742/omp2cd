#pragma once
#ifndef _MSC_VER

#include <pthread.h>
#define thread_local __thread
#else

#include <windows.h>
#include <tchar.h>
#include <strsafe.h>

#define thread_local __declspec(thread)
#endif
namespace darts {
//forward declaration
class ABCScheduler;
class ThreadedProcedure;

struct ThreadLocalInfo {
    ABCScheduler* threadTPsched;
    char pad1[64 - sizeof(ABCScheduler*)];
    ABCScheduler* threadMCsched;
    char pad2[64 - sizeof(ABCScheduler*)];
    ThreadedProcedure* tempParent;
};

extern thread_local ThreadLocalInfo myThread;
}
