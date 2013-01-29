/*
 * particle.h
 *
 *  Created on: 2013-01-17
 *      Author: jurgens
 */

#ifndef PARTICLE_H_
#define PARTICLE_H_

// http://nehe.gamedev.net/tutorial/particle_engine_using_triangle_strips/21001/

#include "err.h"
#include "vector.h"
#include "entity.h"
#include "texture.h"

#include <boost/shared_ptr.hpp>

class Particle : public Entity
{
    enum {
        VERTEX_ARRAY_SIZE = 6*3*sizeof(float), // 6 vertices, 3 components, float
        COLOR_ARRAY_SIZE  = 6*3*sizeof(float), // 6 vertices, 3 components, float
        TEX_COORD_SIZE    = 6*2*sizeof(float)  // 6 vertices, 2 components, float
    };

    float  m_LifeTime;
    float  m_FadeTime;

    Vector m_Color;
    Vector m_Position;
    Vector m_Direction;
    Vector m_Gravity;

    Vector m_Speed;
    float  m_Slowdown;

    float m_ColorArray[6*3];

    int    m_VboID;
public:
    Particle( const Vector& color = Vector( 1.0f, 0.3f,0.5f ) );

    ~Particle();

    void SetColor(const Vector& color);
protected:
    virtual bool DoInitialize( Renderer* renderer );

    virtual void DoRender();
private:
    void Reset();

    void DoUpdate( float ticks );

    friend class Emitter;
};

typedef boost::shared_ptr<Particle> ParticlePtr;

#endif /* PARTICLE_H_ */
