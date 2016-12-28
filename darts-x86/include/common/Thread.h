#pragma once

#ifndef _MSC_VER

#include <pthread.h>
#else

#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#endif

#include <hwloc.h>

namespace darts {

/*
    Class: Thread

    A class used to provide a common interface for running threads across
    different OSs.
    */
class Thread {
    // Section: Private ->
private:
    /*
        Variable: function_
        Function to start as a thread.
        */
    void* (*function_)(void*);

    /*
        Variable: argument_
        Argument to the thread.
        */
    void* argument_;

/*
        Variable: thread_
        Natively repesents the thread.
        */
#ifndef _MSC_VER
    pthread_t thread_;
    pthread_attr_t attr;
#else
    DWORD thread_;
    HANDLE threadHandle_;
#endif

public:
    //Section: Public ->

    /*
        Constructor: Thread

        Creates a thread object to run on natively as a thread
        on the architecture.

        Parameters:
        functionToSet - function to run as a thread.
        argToSet - argument pass to the thread.

        */
    Thread(void* (*functionToSet)(void*), void* argToSet);
    Thread(void* (*functionToSet)(void*));
    Thread(void);

    /*
        Function: run

        Starts the thread to run.

        Returns:

        True if the thread was created succesfully.
        */
    bool run();

    /*
        Function: join

        Waits for the thread to get done running.

        Returns:

        True if the thread exited succesfully.
        */
    bool join();

    void resetArgument(void* resetToMe);

    void resetFunction(void* (*functionToSet)(void*));

    bool setAffinity(int cpu);
    bool setAffinity(int cpu, hwloc_topology_t _topology);

    ~Thread(void);
};

} // namespace darts
