/*
 * entity.cpp
 *
 *  Created on: 2013-01-21
 *      Author: jurgens
 */

#include "entity.h"
#include "renderer.h"

#include <GL/glew.h>

#include <boost/bind.hpp>

Entity::Entity() throw ()
    : m_Flags(F_ENABLE)
    , m_OrderNum(0)
{
}

void Entity::CheckDestroy()
{
    delete this;
}

bool Entity::CompareEntityPriorities( const EntityPtr& a, const EntityPtr& b ) {
    return a->GetOrder() < b->GetOrder();
}

RenderState* Entity::GetRenderState()
{
    // TODO: This must be pointer. Can be subclassed!
    return &m_RenderState;
}

void Entity::AddEntity( EntityPtr entity, int priority /*= 0*/  )
{
    entity->SetOrder( priority );
    m_InitList.push_back( entity );
}

bool Entity::Initialize( Renderer* renderer )
{
    bool r(true);
    bool sortChildred( false );
    while ( m_InitList.size() ) {
        auto& entity = m_InitList.front();
        entity->Initialize( renderer );
        // should be a sorted insert...but would probably slow the container (change to set/multiset??)
        m_RenderList.push_back(entity);
        m_InitList.pop_front();
        sortChildred = true;
    }
    if ( sortChildred ) {
        m_RenderList.sort( boost::bind( &Entity::CompareEntityPriorities, this, _1, _2) );
    }
    DoInitialize( renderer );
    return r;
}

void Entity::Render( float ticks )
{
    DoUpdate(ticks);
    glPushMatrix();
    glMultMatrixf( GetRenderState()->GetMatrix() );

    uint32_t flags = GetRenderState()->GetFlags();

    int gl_blend_src(0), gl_blend_dst(0);
    int alpha_enabled;
    glGetIntegerv(GL_ALPHA_TEST, &alpha_enabled);
    if (!alpha_enabled && (flags & RenderState::ALPHA_F) )
    {
        glGetIntegerv(GL_BLEND_SRC, &gl_blend_src);
        glGetIntegerv(GL_BLEND_DST, &gl_blend_dst);
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_ALPHA_TEST);
    }
    else if (alpha_enabled && (flags & RenderState::ALPHA_F) == 0 )
    {
        glDisable(GL_ALPHA_TEST);
    }
    int blend_enabled;
    glGetIntegerv(GL_BLEND, &blend_enabled);
    if (!blend_enabled && (flags & RenderState::BLEND_F) )
    {
        glEnable(GL_BLEND);
    }
    else if ( blend_enabled && (flags & RenderState::BLEND_F) == 0 )
    {
        glDisable(GL_BLEND);
    }

    for( auto it = m_RenderList.begin(); it != m_RenderList.end(); ) {
        EntityPtr entity = *it;
        if ( entity->AreFlagsSet( Entity::F_ENABLE ) ) {
            entity->Render( ticks );
        }
        if ( entity->AreFlagsSet( Entity::F_DELETE ) ) {
            it = m_RenderList.erase( it );
            continue;
        }
        ++it;
    }
    DoRender();

    if (!alpha_enabled && (flags & RenderState::ALPHA_F) )
    {
        glBlendFunc( gl_blend_src, gl_blend_dst);
    }
    glPopMatrix();
}
