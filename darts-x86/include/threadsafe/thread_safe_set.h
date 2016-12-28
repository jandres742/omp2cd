#ifndef THREAD_SAFE_SET_H_INCLUDED
#define THREAD_SAFE_SET_H_INCLUDED

#include "AutoLock.h"
#include "Lock.h"
#include <set>

namespace thread_safe {

template <class Key, class Compare = std::less<Key>, class Allocator = std::allocator<Key> >
class set {
public:
    typedef typename std::set<Key, Compare, Allocator>::iterator iterator;
    typedef typename std::set<Key, Compare, Allocator>::const_iterator const_iterator;
    typedef typename std::set<Key, Compare, Allocator>::reverse_iterator reverse_iterator;
    typedef typename std::set<Key, Compare, Allocator>::const_reverse_iterator const_reverse_iterator;
    typedef typename std::set<Key, Compare, Allocator>::allocator_type allocator_type;
    typedef typename std::set<Key, Compare, Allocator>::size_type size_type;
    typedef typename std::set<Key, Compare, Allocator>::key_compare key_compare;
    typedef typename std::set<Key, Compare, Allocator>::value_compare value_compare;

    //Constructors
    explicit set(const Compare& comp = Compare(), const Allocator& alloc = Allocator())
        : storage(comp, alloc)
    {
    }
    template <class InputIterator>
    set(InputIterator first, InputIterator last, const Compare& comp = Compare(), const Allocator& alloc = Allocator())
        : storage(first, last, comp, alloc)
    {
    }
    set(const thread_safe::set<Key, Compare, Allocator>& x)
        : storage(x.storage)
    {
    }

    //Copy
    thread_safe::set<Key, Compare, Allocator>& operator=(const thread_safe::set<Key, Compare, Allocator>& x)
    {
        darts::AutoLock lock(&mutex);
        darts::AutoLock lock2(x.mutex);
        storage = x.storage;
        return *this;
    }

    //Destructor
    ~set(void) {}

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

    bool empty(void) const
    {
        darts::AutoLock lock(&mutex);
        return storage.empty();
    }

    //Modifiers
    std::pair<iterator, bool> insert(const Key& x)
    {
        darts::AutoLock lock(&mutex);
        return storage.insert(x);
    }
    iterator insert(iterator position, const Key& x)
    {
        darts::AutoLock lock(&mutex);
        return storage.insert(position, x);
    }
    template <class InputIterator>
    void insert(InputIterator first, InputIterator last)
    {
        darts::AutoLock lock(&mutex);
        storage.insert(first, last);
    }

    void erase(iterator pos)
    {
        darts::AutoLock lock(&mutex);
        storage.erase(pos);
    }
    size_type erase(const Key& x)
    {
        darts::AutoLock lock(&mutex);
        return storage.erase(x);
    }
    void erase(iterator begin, iterator end)
    {
        darts::AutoLock lock(&mutex);
        storage.erase(begin, end);
    }

    void swap(thread_safe::set<Key, Compare, Allocator>& x)
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

    //Observers
    key_compare key_comp(void) const
    {
        darts::AutoLock lock(&mutex);
        return storage.key_comp();
    }
    value_compare value_comp(void) const
    {
        darts::AutoLock lock(&mutex);
        return storage.value_comp();
    }

    //Operations
    const_iterator find(const Key& x) const
    {
        darts::AutoLock lock(&mutex);
        return storage.find(x);
    }
    iterator find(const Key& x)
    {
        darts::AutoLock lock(&mutex);
        return storage.find(x);
    }

    size_type count(const Key& x) const
    {
        darts::AutoLock lock(&mutex);
        return storage.count(x);
    }

    const_iterator lower_bound(const Key& x) const
    {
        darts::AutoLock lock(&mutex);
        return storage.lower_bound(x);
    }
    iterator lower_bound(const Key& x)
    {
        darts::AutoLock lock(&mutex);
        return storage.lower_bound(x);
    }

    const_iterator upper_bound(const Key& x) const
    {
        darts::AutoLock lock(&mutex);
        return storage.upper_bound(x);
    }
    iterator upper_bound(const Key& x)
    {
        darts::AutoLock lock(&mutex);
        return storage.upper_bound(x);
    }

    std::pair<const_iterator, const_iterator> equal_range(const Key& x) const
    {
        darts::AutoLock lock(&mutex);
        return storage.equal_range(x);
    }
    std::pair<iterator, iterator> equal_range(const Key& x)
    {
        darts::AutoLock lock(&mutex);
        return storage.equal_range(x);
    }

    //Allocator
    allocator_type get_allocator(void) const
    {
        darts::AutoLock lock(&mutex);
        return storage.get_allocator();
    }

private:
    std::set<Key, Compare, Allocator> storage;
    mutable darts::Lock mutex;
};

template <class Key, class Compare = std::less<Key>, class Allocator = std::allocator<Key> >
class multiset {
public:
    typedef typename std::multiset<Key, Compare, Allocator>::iterator iterator;
    typedef typename std::multiset<Key, Compare, Allocator>::const_iterator const_iterator;
    typedef typename std::multiset<Key, Compare, Allocator>::reverse_iterator reverse_iterator;
    typedef typename std::multiset<Key, Compare, Allocator>::const_reverse_iterator const_reverse_iterator;
    typedef typename std::multiset<Key, Compare, Allocator>::allocator_type allocator_type;
    typedef typename std::multiset<Key, Compare, Allocator>::size_type size_type;
    typedef typename std::multiset<Key, Compare, Allocator>::key_compare key_compare;
    typedef typename std::multiset<Key, Compare, Allocator>::value_compare value_compare;

    //Constructors
    explicit multiset(const Compare& comp = Compare(), const Allocator& alloc = Allocator())
        : storage(comp, alloc)
    {
    }
    template <class InputIterator>
    multiset(InputIterator first, InputIterator last, const Compare& comp = Compare(), const Allocator& alloc = Allocator())
        : storage(first, last, comp, alloc)
    {
    }
    multiset(const thread_safe::multiset<Key, Compare, Allocator>& x)
        : storage(x.storage)
    {
    }

    //Copy
    thread_safe::multiset<Key, Compare, Allocator>& operator=(const thread_safe::multiset<Key, Compare, Allocator>& x)
    {
        darts::AutoLock lock(&mutex);
        darts::AutoLock lock2(x.mutex);
        storage = x.storage;
        return *this;
    }

    //Destructor
    ~multiset(void) {}

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

    bool empty(void) const
    {
        darts::AutoLock lock(&mutex);
        return storage.empty();
    }

    //Modifiers
    std::pair<iterator, bool> insert(const Key& x)
    {
        darts::AutoLock lock(&mutex);
        return storage.insert(x);
    }
    iterator insert(iterator position, const Key& x)
    {
        darts::AutoLock lock(&mutex);
        return storage.insert(position, x);
    }
    template <class InputIterator>
    void insert(InputIterator first, InputIterator last)
    {
        darts::AutoLock lock(&mutex);
        storage.insert(first, last);
    }

    void erase(iterator pos)
    {
        darts::AutoLock lock(&mutex);
        storage.erase(pos);
    }
    size_type erase(const Key& x)
    {
        darts::AutoLock lock(&mutex);
        return storage.erase(x);
    }
    void erase(iterator begin, iterator end)
    {
        darts::AutoLock lock(&mutex);
        storage.erase(begin, end);
    }

    void swap(thread_safe::multiset<Key, Compare, Allocator>& x)
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

    //Observers
    key_compare key_comp(void) const
    {
        darts::AutoLock lock(&mutex);
        return storage.key_comp();
    }
    value_compare value_comp(void) const
    {
        darts::AutoLock lock(&mutex);
        return storage.value_comp();
    }

    //Operations
    const_iterator find(const Key& x) const
    {
        darts::AutoLock lock(&mutex);
        return storage.find(x);
    }
    iterator find(const Key& x)
    {
        darts::AutoLock lock(&mutex);
        return storage.find(x);
    }

    size_type count(const Key& x) const
    {
        darts::AutoLock lock(&mutex);
        return storage.count(x);
    }

    const_iterator lower_bound(const Key& x) const
    {
        darts::AutoLock lock(&mutex);
        return storage.lower_bound(x);
    }
    iterator lower_bound(const Key& x)
    {
        darts::AutoLock lock(&mutex);
        return storage.lower_bound(x);
    }

    const_iterator upper_bound(const Key& x) const
    {
        darts::AutoLock lock(&mutex);
        return storage.upper_bound(x);
    }
    iterator upper_bound(const Key& x)
    {
        darts::AutoLock lock(&mutex);
        return storage.upper_bound(x);
    }

    std::pair<const_iterator, const_iterator> equal_range(const Key& x) const
    {
        darts::AutoLock lock(&mutex);
        return storage.equal_range(x);
    }
    std::pair<iterator, iterator> equal_range(const Key& x)
    {
        darts::AutoLock lock(&mutex);
        return storage.equal_range(x);
    }

    //Allocator
    allocator_type get_allocator(void) const
    {
        darts::AutoLock lock(&mutex);
        return storage.get_allocator();
    }

private:
    std::multiset<Key, Compare, Allocator> storage;
    mutable darts::Lock mutex;
};

} // namespace thread_safe

#endif // THREAD_SAFE_SET_H_INCLUDED
