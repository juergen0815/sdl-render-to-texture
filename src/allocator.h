/*
 * allocator.h
 *
 *  Created on: 2013-01-21
 *      Author: jurgens
 */

#ifndef ALLOCATOR_H_
#define ALLOCATOR_H_

#include "err.h"
#include "entitypool.h"

#include <limits>
#include <iostream>

#include <boost/shared_ptr.hpp>

template<class T>
class Allocator
{
public:
    // type definitions
    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

    // weak link to memory pool
    boost::shared_ptr<MemoryPool> m_Pool;

    // rebind allocator to type U
    template<class U>
    struct rebind
    {
        typedef Allocator<U> other;
    };

    pointer address(reference value) const
    {
        return &value;
    }
    const_pointer address(const_reference value) const
    {
        return &value;
    }

    Allocator( boost::shared_ptr<MemoryPool> pool ) throw ()
        : m_Pool(pool)
    {
    }

    Allocator(const Allocator& o) throw ()
        : m_Pool( o.m_Pool )
    {
    }

    template<class U>
    Allocator(const Allocator<U>& o) throw ()
        : m_Pool( o.m_Pool )
    {
    }

    ~Allocator() throw ()
    {
    }

    size_type max_size() const throw ()
    {
        return std::numeric_limits<std::size_t>::max() / sizeof(T);
    }

    pointer allocate(size_type num, const void* p = 0)
    {
        return (pointer) EntityPool::Allocate( m_Pool.get(), num );
    }

    // copy c'tor
    void construct(pointer p, const T& value)
    {
        new ((void*) p) T(value);
    }

    // default c'tor
    void construct(pointer p)
    {
        new ((void*) p) T();
    }

    void destroy(pointer p)
    {
        p->~T();
    }

    void deallocate(pointer p, size_type num)
    {
        EntityPool::Free( m_Pool.get(), (Entity*)p, num );
    }
};

// return that all specializations of this allocator are interchangeable
template<class T1, class T2>
bool operator==(const Allocator<T1>&, const Allocator<T2>&) throw ()
{
    return true;
}

template<class T1, class T2>
bool operator!=(const Allocator<T1>&, const Allocator<T2>&) throw ()
{
    return false;
}


#endif /* ALLOCATOR_H_ */
