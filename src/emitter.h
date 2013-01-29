
//**************************************************************************
// File: emitter.h
// Date: Jan 17, 2013
// Author: Jurgen
//**************************************************************************

#ifndef __emitter_h__
#define __emitter_h__

#include "err.h"
#include "particle.h"
#include "texture.h"
#include "allocator.h"

#include <vector>

typedef std::vector< Particle, Allocator<Particle> > ParticleList;

//typedef std::list< ParticlePtr > ParticleList;

class Renderer;
class MemoryPool;

class Emitter : public Entity
{
    TexturePtr   m_Texture;
    Renderer    *m_Renderer;    // keep a week link to the renderer
//    Particle    *m_Particles;
    boost::shared_ptr<MemoryPool> m_MemoryPool;
    ParticleList m_Particles;   // list to all particles
public:
    Emitter( Renderer* renderer ) throw();

    ~Emitter();

    bool HandleEvent( const SDL_Event& event );

private:
    virtual bool DoInitialize( Renderer* renderer );

    virtual void DoUpdate( float ticks );

    virtual void DoRender();
};

#endif // __emitter_h__
