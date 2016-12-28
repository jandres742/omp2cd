#ifndef AUTOLOCK_H
#define AUTOLOCK_H

#include "Lock.h"

namespace darts {

/**
 * Class: AutoLock
 *
 * \brief Lock containter which ensures that the lock is acquired at the end of
 * the object's construction, and that the lock is freed when the object is
 * destroyed.
 */
class AutoLock {
private:
    /** \brief lock to be used */
    Lock* lock_;

public:
    /** 
         * \brief Constructor which takes a <Lock> as input, and attempts to
         * be its owner.
         */
    AutoLock(Lock* lock)
        : lock_(lock)
    {
        if (lock_)
            lock_->lock();
    }

    AutoLock(Lock& lock)
        : lock_(&lock)
    {
        if (lock_)
            lock_->lock();
    }

    /**
         * \brief Destructor which releases the lock the object contains.
         */
    ~AutoLock()
    {
        if (lock_)
            lock_->unlock();
    }
};

} // namespace darts

#endif
