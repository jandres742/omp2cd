#include "Timer.h"

using namespace darts;

//TODO: Comment Code

#ifndef _MSC_VER
Timer::Timer(void)
{
}

void Timer::start()
{
    //clock_gettime( CLOCK_THREAD_CPUTIME_ID, &counterStart );
    clock_gettime(CLOCK_REALTIME, &counterStart);
    // gettimeofday(&counterStart, NULL);
}

double Timer::get()
{

    struct timespec counterStop;
    //struct timeval counterStop;

    //clock_gettime( CLOCK_THREAD_CPUTIME_ID, &counterStop );
    clock_gettime(CLOCK_REALTIME, &counterStop);
    //	gettimeofday(&counterStop, NULL);

    return double(counterStop.tv_sec - counterStart.tv_sec) + double(counterStop.tv_nsec - counterStart.tv_nsec) / 1E9;
    //return double( counterStop.tv_sec - counterStart.tv_sec ) + double( counterStop.tv_usec - counterStart.tv_usec ) / 1E6;
}

void Timer::reset()
{
}

#else

Timer::Timer(void)
    : counterStart(0)
    , freq(0)
{
}

void Timer::start()
{
    LARGE_INTEGER li;
    if (!QueryPerformanceFrequency(&li))
        return;

    freq = double(li.QuadPart);

    QueryPerformanceCounter(&li);
    counterStart = li.QuadPart;
}

double Timer::get()
{
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    return double(li.QuadPart - counterStart) / freq;
}

void Timer::reset()
{
    counterStart = 0;
    freq = 0;
}

#endif
