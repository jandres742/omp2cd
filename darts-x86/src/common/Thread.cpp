#include "Thread.h"
#include <iostream>

using namespace darts;

Thread::Thread(void)
    : function_(0)
    , argument_(0)
{
#ifndef _MSC_VER
    pthread_attr_init(&attr);
#endif
}

Thread::Thread(void* (*functionToSet)(void*), void* argToSet)
    : function_(functionToSet)
    , argument_(argToSet)
{
#ifndef _MSC_VER
    pthread_attr_init(&attr);
#endif
}

Thread::Thread(void* (*functionToSet)(void*))
    : function_(functionToSet)
{
#ifndef _MSC_VER
    pthread_attr_init(&attr);
#endif
}

bool Thread::run()
{
#ifndef _MSC_VER

    int rc = pthread_create(&thread_, &attr, function_, argument_);
    //int rc = pthread_create( &thread_, NULL, function_, argument_ );
    if (rc != 0)
        return false;

    return true;
#else

    LPVOID argument = static_cast<LPVOID>(argument_);

    LPTHREAD_START_ROUTINE function = reinterpret_cast<LPTHREAD_START_ROUTINE>(function_);

    threadHandle_ = CreateThread(0, 0, function,
        argument, 0, &thread_);

    if (threadHandle_ == 0)
        return false;

    return true;

#endif
}

bool Thread::join()
{
#ifndef _MSC_VER

    //FIXME
    pthread_join(thread_, 0);

    return true;

#else

    DWORD result;

    result = WaitForSingleObject(threadHandle_, INFINITE);

    if (result == WAIT_OBJECT_0)
        return true;

    return false;

#endif
}

void Thread::resetArgument(void* resetToMe)
{
    argument_ = resetToMe;
}

void Thread::resetFunction(void* (*functionToSet)(void*))
{
    function_ = functionToSet;
}

bool Thread::setAffinity(int cpu)
{
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(cpu, &cpuset);
    return (pthread_attr_setaffinity_np(&attr, sizeof(cpuset), &cpuset));
}

bool Thread::setAffinity(int cpu, hwloc_topology_t _topology)
{
#if 0
    /*Allocate a new empty CPU set*/
    hwloc_cpuset_t set = hwloc_bitmap_alloc();
    /*Add CPU cpu in CPU set set.*/
    hwloc_cpuset_cpu(set, cpu);
    int ret = hwloc_set_cpubind(_topology, set, 0);
    char *str;
    hwloc_bitmap_asprintf(&str, set);
    printf("\t\tcpuset = %s\n", str);
    free(str);
    return ret;
#else 
    return false;
#endif
}

Thread::~Thread(void)
{
#ifndef _MSC_VER
    pthread_attr_destroy(&attr);
#endif
}
