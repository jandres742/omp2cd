#include "getClock.h"

const timespec operator-(const timespec& end, const timespec& start)
{
    timespec temp;
    if ((end.tv_nsec - start.tv_nsec) < 0) {
        temp.tv_sec = end.tv_sec - start.tv_sec - 1;
        temp.tv_nsec = 1000000000 + end.tv_nsec - start.tv_nsec;
    } else {
        temp.tv_sec = end.tv_sec - start.tv_sec;
        temp.tv_nsec = end.tv_nsec - start.tv_nsec;
    }
    return temp;
}

void printTime(timespec theTime, std::ostream& out)
{
    out << std::fixed;
    out << std::setprecision(9) << (((double)theTime.tv_sec) * 1000000000 + theTime.tv_nsec) / (double)1000000000;
}

double seconds(timespec theTime)
{
    return (((double)theTime.tv_sec) * 1000000000 + theTime.tv_nsec) / (double)1000000000;
}
