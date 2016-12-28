#pragma once
#include "ABCScheduler.h"
#include "Codelet.h"
#include "ThreadedProcedure.h"

#ifdef TBB
#include <tbb/concurrent_queue.h>
#else
#include "thread_safe_deque.h"
#endif

namespace darts {

/*Scheduler:
The Scheduler class is templated and 
wrappes the ABCScheduler allow us to 
add templated ready deques which each
scheduler needs.  The reason for this
wrapping is to avoid a recursive definition
when adding pointers to parent and children
schedulers.
*/

template <class T>
class Scheduler : public ABCScheduler {
public:

private:
/*This protected not private so the 
        inheriting class can iterate on it
        This is a pool of T ready things 
        which are running or ready to run*/

#ifdef TBB
    tbb::concurrent_queue<T> ready_;
#else
    thread_safe::deque<T> ready_;
#endif

public:
    //Constructors
    Scheduler(ABCScheduler* parent, std::vector<ABCScheduler*> child)
        : ABCScheduler(parent, child)
    {
    }

    Scheduler(void)
        : ABCScheduler()
    {
    }

    //The following are wrappers to the ready deque

    //Pushes to the head of the deque
    void pushReadyHead(T const& toAdd);

    //Pops the head and returns the value
    T popReadyHead(void);

    //Pushes to the tail of the deque
    void pushReadyTail(T const& toAdd);

    //Pops tail and returns the value
    T popReadyTail(void);

    /*The following are virtual functions which
        the highest level schedulers must implement*/

    //The function which will actually run the scheduling procedure
    virtual void policy(void) = 0;
    //Tells the scheduler how to push a TP
    virtual bool placeTP(tpClosure* input) = 0;
    //Tell the scheduler how to push a Codelet
    virtual bool placeCodelet(Codelet* input) = 0;

    size_t getReadySize();

}; //class

template <class T>
void Scheduler<T>::pushReadyHead(T const& toAdd)
{
#ifdef TBB
    ready_.push(toAdd);
#else
    ready_.push_front(toAdd);
#endif
}

template <class T>
T Scheduler<T>::popReadyHead(void)
{
#ifdef TBB
    T temp;
    if (ready_.try_pop(temp)) {
        return temp;
    } else
        return 0;
#else
    return ready_.popFront();
#endif
}

template <class T>
void Scheduler<T>::pushReadyTail(T const& toAdd)
{
#ifdef TBB
    ready_.push(toAdd);
#else
    ready_.push_back(toAdd);
#endif
}

template <class T>
T Scheduler<T>::popReadyTail(void)
{
#ifdef TBB
    T temp;
    if (ready_.try_pop(temp))
        return temp;
    else
        return 0;
#else
    return ready_.popBack();
#endif
}

template <class T>
size_t Scheduler<T>::getReadySize()
{
    return ready_.unsafe_size();
}

} // namespace darts
