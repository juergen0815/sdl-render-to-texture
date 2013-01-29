/*
 * entitypool.cpp
 *
 *  Created on: 2013-01-21
 *      Author: jurgens
 */

#include "entitypool.h"

#include <cstdlib>
#include <cstring>

class MemoryPool
{
    friend class EnityPool;

    struct Block
    {
        char *m_Data;
    };
    Block       *m_Data;
    std::size_t  m_NumBlocks;
    long         m_BlockSize;
    std::size_t  m_BlocksAvailable;
    char        *m_FirstFree;
    std::size_t  m_HeaderSize;
    char        *m_Begin;
    char        *m_End;

    MemoryPool  *m_NextPool;
public:
    void* Allocate( ) throw ();

    void* Allocate( std::size_t num ) throw ();

    void Free( void* p );

    void Free( void* p, std::size_t num );

    MemoryPool( std::size_t num, std::size_t size );

    MemoryPool( const MemoryPool& other );

    ~MemoryPool();

};

MemoryPool::MemoryPool( std::size_t num, std::size_t size )
    : m_Data( nullptr )
    , m_NumBlocks(num)
    , m_BlockSize(size)
    , m_BlocksAvailable(num)
    , m_NextPool(nullptr)
{
    m_HeaderSize = sizeof( Block ) * num;
    // Allocate memory for num amount of blocks
    m_Data = static_cast<Block*>(std::malloc( m_HeaderSize + num * size ));
    // clear the header (only)
    std::memset( m_Data, 0x00, m_HeaderSize );
    m_Begin = ((char*)m_Data) + m_HeaderSize;
    m_FirstFree = m_Begin;
    m_End       = m_Begin + num * size;
}

MemoryPool::MemoryPool( const MemoryPool& o )
    : m_NumBlocks(o.m_NumBlocks)
    , m_BlockSize(o.m_BlockSize)
    , m_BlocksAvailable(o.m_BlocksAvailable)
    , m_FirstFree(o.m_FirstFree)
    , m_HeaderSize(o.m_HeaderSize)
    , m_Begin(o.m_Begin)
    , m_End(o.m_End)
    , m_NextPool(o.m_NextPool)
{
    if ( m_Data ) {
        delete m_Data;
    }
    m_Data = o.m_Data;
}

MemoryPool::~MemoryPool()
{
    if ( m_NextPool ) {
        delete m_NextPool;
    }
    if ( m_Data ) {
        // delete the whole block
        std::free( m_Data );
    }
    m_FirstFree = (char*)0xdeadbeef;
}

void* MemoryPool::Allocate() throw ()
{
    // TODO: Make thread safe
    void *p = nullptr;
    MemoryPool *pool = this;
    do {
        if ( pool->m_BlocksAvailable > 0 ) {
            ASSERT( pool->m_FirstFree >= pool->m_Begin &&
                    pool->m_FirstFree <  pool->m_End, "Error! Free block points to invalid addr" );
            p = static_cast<void*>(pool->m_FirstFree);
            long offset = ( pool->m_FirstFree - pool->m_Begin ) / m_BlockSize;
            pool->m_Data[ offset ].m_Data =  static_cast<char*>(p);
            // search for next free block - do not just increment!
            pool->m_FirstFree += m_BlockSize;
            --pool->m_BlocksAvailable;
        } else {
            pool = m_NextPool;
            if ( !pool ) {
                pool = new MemoryPool( m_NumBlocks, m_BlockSize );
                m_NextPool = pool;
            }
        }
    } while ( p == nullptr ) ;
    return p;
}

void* MemoryPool::Allocate( std::size_t num ) throw ()
{
    // TODO: Make thread safe

    MemoryPool *pool = this;
    // allocate a new block if the request size is bigger than available blocks
    // - This pool must be empty to hold a complete array of data - else allocate a new pool
    if ( pool->m_BlocksAvailable != m_NumBlocks || m_NumBlocks < num ) {
        while ( pool->m_NextPool ) {
            pool = pool->m_NextPool;
        }
        pool = new MemoryPool( num, m_BlockSize );
        m_NextPool = pool;
    }
    pool->m_BlocksAvailable = 0;
    void *p = static_cast<void*>(pool->m_FirstFree);
    char *addr = pool->m_FirstFree;
    for ( std::size_t i = 0; i < num; ++i, addr += pool->m_BlockSize ) {
        pool->m_Data[ i ].m_Data = addr;
    }
    pool->m_FirstFree = pool->m_End;
    return p;
}

void MemoryPool::Free( void* p )
{
    // TODO: Make thread safe
    ASSERT( p, "Error! Trying to delete a nullptr!");
    MemoryPool *pool = this;
    do {
        if ( p >= pool->m_Begin && p < pool->m_End ) {
            long offset = ( static_cast<char*>(p) - pool->m_Begin ) /  m_BlockSize;
            // make this block available
            m_FirstFree = pool->m_Data[ offset ].m_Data;
            // mark block as available
            pool->m_Data[ offset ].m_Data = 0;
            // If pool is empty but we have another one attached, make next current
            if ( (++pool->m_BlocksAvailable == pool->m_NumBlocks) && pool->m_NextPool ) {
                MemoryPool *tPool = pool->m_NextPool;
                *this = *tPool;
                tPool->m_NextPool = NULL;
                tPool->m_Data = NULL;
                delete tPool;
            }
            return;
        }
        else {
            pool = pool->m_NextPool;
            ASSERT( pool, "Error! Pointer 0x%0x is not in memory pool!", p );
        }
    } while ( pool );
}

void MemoryPool::Free( void* p, std::size_t num )
{
    // TODO: Make thread safe

    MemoryPool *pool = this;
    do {
        // is this address in this pool?
        if ( p >= pool->m_Begin && p < pool->m_End ) {
            std::size_t offset = ( static_cast<char*>(p) - pool->m_Begin ) /  m_BlockSize;
            // if for some reason someone frees more blocks than size of buffer...
            if ( (m_NumBlocks - offset) < num ) num = m_NumBlocks - offset;
            // first in new freed block
            m_FirstFree = pool->m_Data[ offset ].m_Data;
            pool->m_BlocksAvailable += num;
            for ( std::size_t i = 0; i < num; ++i, ++offset ) {
                // mark block as available
                pool->m_Data[ offset ].m_Data = 0;
            }
            // If pool is empty but we have another one attached, make next current
            if ( (pool->m_BlocksAvailable == pool->m_NumBlocks) && pool->m_NextPool ) {
                MemoryPool *tPool = pool->m_NextPool;
                *this = *tPool;
                tPool->m_NextPool = NULL;
                tPool->m_Data = NULL;
                delete tPool;
            }
            return;
        }
        else {
            pool = pool->m_NextPool;
            ASSERT( pool, "Error! Pointer 0x%0x is not in memory pool!", p );
        }
    } while ( pool );

}

EntityPool::EntityPool()
{
}

EntityPool::~EntityPool()
{
}

MemoryPool* EntityPool::CreatePool( std::size_t num, std::size_t objSize )
{
    MemoryPool* pool = new MemoryPool( num, objSize);
    return pool;
}

void EntityPool::DestroyPool( MemoryPool *pool )
{
    ASSERT( pool, "Invalid pool!" );
    delete pool;
}

void EntityPool::Free( MemoryPool* pool, Entity* p, std::size_t num )
{
    num > 1 ? pool->Free( p, num ) : pool->Free( p );
}

void EntityPool::Free( MemoryPool* pool, Entity* p )
{
    pool->Free( p );
}

Entity* EntityPool::Allocate( MemoryPool* pool, std::size_t num ) throw()
{
    void *p = nullptr;
    if ( num > 1 ) {
        p = pool->Allocate( num );
    } else {
        p = pool->Allocate( );
    }
    return static_cast<Entity*>(p);
}
