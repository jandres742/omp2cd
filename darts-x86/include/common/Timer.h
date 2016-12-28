#pragma once

#ifndef _MSC_VER

#include <time.h>
#include <sys/time.h>

#else

#include <windows.h>

#endif

namespace darts {
/*
	Class: Lock

	A class used to provide generic locking objects works for windows
	and any OS with pthreads.
*/
class Timer {
    // Section: Private ->
private:
#ifndef _MSC_VER

    struct timespec counterStart;
//struct timeval counterStart;

#else

    double freq;
    double overhead;
    __int64 counterStart;

#endif

public:
    //Section: Public ->

    Timer(void);

    void start();

    double get();

    void reset();
};
}
