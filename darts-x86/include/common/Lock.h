#pragma once

#ifndef _MSC_VER

#include <pthread.h>

#else

#include <windows.h>

#endif

namespace darts {

/*
	Class: Lock

	A class used to provide generic locking objects works for windows
	and any OS with pthreads.
*/
class Lock {
    // Section: Private ->
private:
#ifndef _MSC_VER

    /*
			Variable: mutex_

			Locking object with pthreads.
		*/
    pthread_mutex_t mutex_;

#else

    /*
			Variable: mutex_

			Locking object on windows.
		*/
    HANDLE mutex_;

#endif

public:
    //Section: Public ->

    /*
			Constructor: Lock

			Creates the lock for use.
	*/
    Lock(void);

    /*
			Destructor: Lock

			Destroys the lock.
	*/
    ~Lock(void);

    /*
			Function: lock

			Waits until the lock is secured.
	*/
    void lock(void);

    /*
			Function: trylock

			Tries to lock otherwise returns.

			Returns:

				True if the lock was secured.
	*/
    bool trylock(void);

    /*
			Function: unlock

			Yields the lock for others to use.
	*/
    void unlock(void);
};

} // namespace darts
