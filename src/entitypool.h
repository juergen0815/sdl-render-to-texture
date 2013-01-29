/*
 * entitypool.h
 *
 *  Created on: 2013-01-21
 *      Author: jurgens
 */

#ifndef ENTITYPOOL_H_
#define ENTITYPOOL_H_

#include "err.h"
#include "entity.h"

#include <cstdlib>

class MemoryPool; // abstract class! Used as a handle

class EntityPool
{
    // Static class no need for c'tor/d'tor
    EntityPool();

    ~EntityPool();
public:
    template< class T = Entity >
    static MemoryPool* CreatePool( std::size_t num = 16 );

    static void DestroyPool( MemoryPool *pool );

    template< class T = Entity >
    static T* Construct( MemoryPool* pool ) throw();

    template< class T = Entity >
    static T* Construct( MemoryPool* pool, int num ) throw();

    template< class T = Entity >
    static void Delete( MemoryPool* pool, T *p ) throw();

    template< class T = Entity >
    static void Delete( MemoryPool* pool, T *p, int num ) throw();

    static Entity* Allocate( MemoryPool* pool, std::size_t num = 1 ) throw( );

    static void Free( MemoryPool *pool, Entity* p, std::size_t num );

    static void Free( MemoryPool *pool, Entity* p );
private:
    static MemoryPool* CreatePool( std::size_t num, std::size_t objSize );

    friend class Entity;
};

template< class T >
MemoryPool* EntityPool::CreatePool( std::size_t num )
{
    return CreatePool( num, sizeof(T) );
}

template< class T >
T* EntityPool::Construct( MemoryPool* pool ) throw()
{
    T* p = static_cast<T*>(EntityPool::Allocate( pool ));
    new ( p ) T();
    return p;
}

template< class T >
T* EntityPool::Construct( MemoryPool* pool, int num ) throw()
{
    T* p = static_cast<T*>(EntityPool::Allocate( pool, num ));
    for ( int i = 0; i < num; ++i ) {
        new ( &p[i] ) T();
    }
    return p;
}

template< class T >
void EntityPool::Delete( MemoryPool* pool, T *p ) throw()
{
    p->~T();
    EntityPool::Free( pool, p );
}

template< class T >
void EntityPool::Delete( MemoryPool* pool, T *p, int num ) throw()
{
    for ( int i = 0; i < num; ++i ) {
        p[i].~T();
        EntityPool::Free( pool, &p[i] );
    }
//    EntityPool::Free( pool, p, num );
}

#endif /* ENTITYPOOL_H_ */
