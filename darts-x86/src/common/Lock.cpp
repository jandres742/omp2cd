#include "Lock.h"

using namespace darts;

//TODO: Comment Code

Lock::Lock(void)
{
#ifndef _MSC_VER
    pthread_mutex_init(&mutex_, NULL);
#else
    mutex_ = CreateMutex(NULL, FALSE, NULL);
#endif
}

Lock::~Lock(void)
{
#ifndef _MSC_VER
    pthread_mutex_destroy(&mutex_);
#else
    CloseHandle(mutex_);
#endif
}

void Lock::lock(void)
{
#ifndef _MSC_VER
    pthread_mutex_lock(&mutex_);
#else
    WaitForSingleObject(mutex_, INFINITE);
#endif
}

bool Lock::trylock(void)
{

    bool lockSuccess = false;

#ifndef _MSC_VER
    int result;

    result = pthread_mutex_trylock(&mutex_);

    if (result == 0)
        lockSuccess = true;

#else
    DWORD result;

    result = WaitForSingleObject(mutex_, 1); // 1 Millisecond

    if (result == WAIT_OBJECT_0)
        lockSuccess = true;

#endif

    return lockSuccess;
}

void Lock::unlock(void)
{
#ifndef _MSC_VER
    pthread_mutex_unlock(&mutex_);
#else
    ReleaseMutex(mutex_);
#endif
}
