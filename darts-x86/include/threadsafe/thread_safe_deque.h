#ifndef THREAD_SAFE_DEQUE_H_INCLUDED
#define THREAD_SAFE_DEQUE_H_INCLUDED

#include "AutoLock.h"
#include "Lock.h"
#include <deque>

namespace thread_safe {

template <class T, class Allocator = std::allocator<T> >
class deque {
public:
    typedef typename std::deque<T, Allocator>::iterator iterator;
    typedef typename std::deque<T, Allocator>::const_iterator const_iterator;
    typedef typename std::deque<T, Allocator>::reverse_iterator reverse_iterator;
    typedef typename std::deque<T, Allocator>::const_reverse_iterator const_reverse_iterator;
    typedef typename std::deque<T, Allocator>::allocator_type allocator_type;
    typedef typename std::deque<T, Allocator>::value_type value_type;
    typedef typename std::deque<T, Allocator>::size_type size_type;
    typedef typename std::deque<T, Allocator>::difference_type difference_type;

    //Constructors
    explicit deque(const Allocator& alloc = Allocator())
        : storage(alloc)
    {
    }
    explicit deque(size_type n, const T& value = T(), const Allocator& alloc = Allocator())
        : storage(n, value, alloc)
    {
    }
    template <class InputIterator>
    deque(InputIterator first, InputIterator last, const Allocator& alloc = Allocator())
        : storage(first, last, alloc)
    {
    }
    deque(const thread_safe::deque<T, Allocator>& x)
    {
        darts::AutoLock lock(x.mutex);
        storage = x.storage;
    }

    //Copy
    thread_safe::deque<T, Allocator>& operator=(const thread_safe::deque<T, Allocator>& x)
    {
        darts::AutoLock lock(&mutex);
        darts::AutoLock lock2(x.mutex);
        storage = x.storage;
        return *this;
    }

    //Destructor
    ~deque<T, Allocator>(void) {}

    //Iterators
    iterator begin(void)
    {
        darts::AutoLock lock(&mutex);
        return storage.begin();
    }
    const_iterator begin(void) const
    {
        darts::AutoLock lock(&mutex);
        return storage.begin();
    }

    iterator end(void)
    {
        darts::AutoLock lock(&mutex);
        return storage.end();
    }
    const_iterator end(void) const
    {
        darts::AutoLock lock(&mutex);
        return storage.end();
    }

    reverse_iterator rbegin(void)
    {
        darts::AutoLock lock(&mutex);
        return storage.rbegin();
    }
    const_reverse_iterator rbegin(void) const
    {
        darts::AutoLock lock(&mutex);
        return storage.rbegin();
    }

    reverse_iterator rend(void)
    {
        darts::AutoLock lock(&mutex);
        return storage.rend();
    }
    const_reverse_iterator rend(void) const
    {
        darts::AutoLock lock(&mutex);
        return storage.rend();
    }

    //Capacity
    size_type size(void) const
    {
        darts::AutoLock lock(&mutex);
        return storage.size();
    }

    size_type max_size(void) const
    {
        darts::AutoLock lock(&mutex);
        return storage.max_size();
    }

    void resize(size_type n, T c = T())
    {
        darts::AutoLock lock(&mutex);
        storage.resize(n, c);
    }

    bool empty(void) const
    {
        darts::AutoLock lock(&mutex);
        return storage.empty();
    }

    //Element access
    T& operator[](size_type n)
    {
        darts::AutoLock lock(&mutex);
        return storage[n];
    }
    const T& operator[](size_type n) const
    {
        darts::AutoLock lock(&mutex);
        return storage[n];
    }

    T& at(size_type n)
    {
        darts::AutoLock lock(&mutex);
        return storage.at(n);
    }
    const T& at(size_type n) const
    {
        darts::AutoLock lock(&mutex);
        return storage.at(n);
    }

    T& front(void)
    {
        darts::AutoLock lock(&mutex);
        return storage.front();
    }
    const T& front(void) const
    {
        darts::AutoLock lock(&mutex);
        return storage.back();
    }

    T& back(void)
    {
        darts::AutoLock lock(&mutex);
        return storage.back();
    }
    const T& back(void) const
    {
        darts::AutoLock lock(&mutex);
        return storage.back();
    }

    //Modifiers
    void assign(size_type n, T& u)
    {
        darts::AutoLock lock(&mutex);
        storage.assign(n, u);
    }
    template <class InputIterator>
    void assign(InputIterator begin, InputIterator end)
    {
        darts::AutoLock lock(&mutex);
        storage.assign(begin, end);
    }

    void push_back(const T& u)
    {
        darts::AutoLock lock(&mutex);
        storage.push_back(u);
    }

    void pop_back(void)
    {
        darts::AutoLock lock(&mutex);
        storage.pop_back();
    }

    T popBack(void)
    {
        darts::AutoLock lock(&mutex);
        T u = 0;
        if (!storage.empty()) {
            u = storage.back();
            storage.pop_back();
        }
        return u;
    }

    void push_front(const T& u)
    {
        darts::AutoLock lock(&mutex);
        storage.push_front(u);
    }

    void pop_front(void)
    {
        darts::AutoLock lock(&mutex);
        storage.pop_front();
    }

    T popFront(void)
    {
        darts::AutoLock lock(&mutex);
        T u = 0;
        if (!storage.empty()) {
            //Assuming pointers otherwise a bug
            u = storage.front();
            storage.pop_front();
        }

        return u;
    }

    iterator insert(iterator pos, const T& u)
    {
        darts::AutoLock lock(&mutex);
        return storage.insert(pos, u);
    }
    void insert(iterator pos, size_type n, const T& u)
    {
        darts::AutoLock lock(&mutex);
        storage.insert(pos, n, u);
    }
    template <class InputIterator>
    void insert(iterator pos, InputIterator begin, InputIterator end)
    {
        darts::AutoLock lock(&mutex);
        storage.insert(pos, begin, end);
    }

    iterator erase(iterator pos)
    {
        darts::AutoLock lock(&mutex);
        return storage.erase(pos);
    }
    iterator erase(iterator begin, iterator end)
    {
        darts::AutoLock lock(&mutex);
        return storage.erase(begin, end);
    }

    void swap(thread_safe::deque<T, Allocator>& x)
    {
        darts::AutoLock lock(&mutex);
        darts::AutoLock lock2(x.mutex);
        storage.swap(x.storage);
    }

    void clear(void)
    {
        darts::AutoLock lock(&mutex);
        storage.clear();
    }

    //Allocator
    allocator_type get_allocator(void) const
    {
        darts::AutoLock lock(&mutex);
        return storage.get_allocator();
    }

private:
    std::deque<T, Allocator> storage;
    mutable darts::Lock mutex;
};

} //namespace thread_safe

#endif // THREAD_SAFE_DEQUE_H_INCLUDED
