//************************************************************************
// File: emitter.cpp
// Date: Jan 17, 2013
// Author: Jurgen
//************************************************************************

#include "emitter.h"
#include "entitypool.h"
#include "renderer.h"
#include "brush.h"

#include <bmp_loader.h>

#include <GL/glew.h>

#include <SDL/SDL.h>

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>

extern struct Brush sParticleTexture;

const std::size_t NUM_PARTICLES = 10000;
const static std::size_t NUM_COLORS = 12;
static Vector sColors[NUM_COLORS] = // Rainbow Of Colors
{
    {1.0f,0.5f,0.5f},{1.0f,0.75f,0.5f},{1.0f,1.0f,0.5f},{0.75f,1.0f,0.5f},
    {0.5f,1.0f,0.5f},{0.5f,1.0f,0.75f},{0.5f,1.0f,1.0f},{0.5f,0.75f,1.0f},
    {0.5f,0.5f,1.0f},{0.75f,0.5f,1.0f},{1.0f,0.5f,1.0f},{1.0f,0.5f,0.75f}
};

struct PoolDeleter
{
    void operator()(void const *p) const
    {
        EntityPool::DestroyPool( (MemoryPool*)p );
    }
};

Emitter::Emitter(Renderer* renderer) throw()
    : m_Texture( new Texture() )
    , m_Renderer( renderer )
    , m_MemoryPool( EntityPool::CreatePool<Particle>( NUM_PARTICLES ), PoolDeleter() )
    , m_Particles( m_MemoryPool )
{
    ASSERT( renderer, "Error! Renderer is invalid!" );
//    renderer->RegisterUpdateFunction( boost::bind( &Emitter::OnUpdate, this, _1 ) );
    m_Particles.resize( NUM_PARTICLES );
    int i(0);
    for ( auto& particle : m_Particles ) {
        particle.SetFlag(Entity::F_ENABLE);
        particle.SetColor(sColors[i*NUM_COLORS/NUM_PARTICLES]); ++i;
    }
}

Emitter::~Emitter()
{
    m_Particles.clear();
}

bool Emitter::HandleEvent( const SDL_Event& event )
{
    switch (event.type)
    {
    case SDL_KEYDOWN:
        switch (event.key.keysym.sym)
        {
        case SDLK_SPACE:
            break;
        default:
            break;
        }
        break;
    }

    return false;
}

bool Emitter::DoInitialize( Renderer* renderer )
{
    m_Texture->Allocate( sParticleTexture.m_Width, sParticleTexture.m_Height, GL_RGBA );
    BmpBrush brush;
    if (!brush.Load( "../../data/particle.bmp" )) {
        m_Texture->Load( sParticleTexture.m_Pixels, sParticleTexture.m_Width, sParticleTexture.m_Height, GL_RGBA );
    } else {
        m_Texture->Load( brush );
    }
    for ( auto& particle : m_Particles ) {
        particle.DoInitialize( renderer );
    }
    return true;
}

void Emitter::DoUpdate( float ticks )
{
    for ( auto& particle : m_Particles ) {
        particle.DoUpdate(ticks);
    }
}

void Emitter::DoRender()
{
    ASSERT( m_Texture, "Missing texture! Bailing out!" );
    m_Texture->Enable();
    // This could be moved into the emitter, too
    int colorArrayEnabled;
    glGetIntegerv( GL_COLOR_ARRAY, &colorArrayEnabled );
    if ( !colorArrayEnabled) {
        glEnableClientState(GL_COLOR_ARRAY);
    }
    int vertexArrayEnabled;
    glGetIntegerv( GL_VERTEX_ARRAY, &vertexArrayEnabled );
    if (!vertexArrayEnabled) {
        glEnableClientState(GL_VERTEX_ARRAY);
    }
    int texCoordArrayEnabled;
    glGetIntegerv( GL_TEXTURE_COORD_ARRAY, &texCoordArrayEnabled );
    if (!texCoordArrayEnabled) {
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    }

    int depthTestEnabled;
    glGetIntegerv( GL_DEPTH_TEST, &depthTestEnabled );
    if ( depthTestEnabled && ((GetRenderState()->GetFlags() & RenderState::DEPTH_TEST_F) == 0) ) {
        glDisable(GL_DEPTH_TEST);
    } else if ( !depthTestEnabled && (GetRenderState()->GetFlags() & RenderState::DEPTH_TEST_F) ) {
        glEnable(GL_DEPTH_TEST);
    }

    ////////////////////////////////////////////////////////////////////////////
    // Render us
    for ( auto& particle : m_Particles ) {
        particle.DoRender();
    }
    ////////////////////////////////////////////////////////////////////////////

    if ( depthTestEnabled ) {
        glEnable(GL_DEPTH_TEST);
    }

    // This could be moved into the emitter, too
    if (!vertexArrayEnabled)  {
        glDisableClientState(GL_VERTEX_ARRAY);  // disable vertex arrays
    }
    if (!colorArrayEnabled)   {
        glDisableClientState(GL_COLOR_ARRAY);
    }
    if (!texCoordArrayEnabled) {
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }
    m_Texture->Disable();
}
