#ifndef GETCLOCK_H
#define GETCLOCK_H
#include <iomanip>
#include <iostream>
#include <stdint.h>
#include <time.h>

inline uint64_t getTime(void)
{
    timespec time;
    clock_gettime(CLOCK_REALTIME, &time);

/*TODO: Determine which one of these is the best.*/
#if 0
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time);
#endif
#if 0
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &time);
#endif
    return ((uint64_t)time.tv_sec) * 1000000000 + time.tv_nsec;
}

inline void getTime(timespec* time)
{
    clock_gettime(CLOCK_REALTIME, time);
}

const timespec operator-(const timespec& end, const timespec& start);

void printTime(timespec theTime, std::ostream& out = std::cout);

double seconds(timespec theTime);

#endif /* GETCLOCK_H */
