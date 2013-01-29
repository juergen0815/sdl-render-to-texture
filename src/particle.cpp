/*
 * particle.cpp
 *
 *  Created on: 2013-01-17
 *      Author: jurgens
 */

#include "particle.h"

#include <GL/glew.h>

#include <cstdlib>

// for VBOs use a shared vertex buffer. We need to multiply the pos with matrix
// anyway unless we update positions per vertex which makes VBOs to much overhead

//#define USE_VBO
//#define USE_INDIRECT

float sVertexArray[6*3] = {-1.0f,-1.0f,0.0f, +1.0f,-1.0f,0.0f, +1.0f,+1.0f,0.0f,
                           -1.0f,-1.0f,0.0f, +1.0f,+1.0f,0.0f, -1.0f,+1.0f,0.0f};
float sTexCoordArray[6*2] = { 0,0, 1,0, 1,1,
                              0,0, 1,1, 0,1 };

Particle::Particle( const Vector& color /* = Vector(1.0f, 0.3f,0.5f) */ )
    : m_LifeTime( 1.0f )
    , m_Gravity ( 0.0f, -0.8f, 0.0f )
    , m_Speed( 0.0f, 0.0f, 0.0f )
    , m_ColorArray({0})
    , m_VboID(-1)
{
    SetColor( color );
    Reset();
}

Particle::~Particle()
{
#ifdef USE_VBO
    if ( m_VboID > 0 ) {
        glDeleteBuffers(1, (GLuint*)&m_VboID);
    }
#endif
}

void Particle::SetColor(const Vector& color)
{
    m_Color = color;
    float colors[COLOR_ARRAY_SIZE] =
        { m_Color[ Vector::R], m_Color[ Vector::G], m_Color[ Vector::B],
          m_Color[ Vector::R], m_Color[ Vector::G], m_Color[ Vector::B],
          m_Color[ Vector::R], m_Color[ Vector::G], m_Color[ Vector::B],
          m_Color[ Vector::R], m_Color[ Vector::G], m_Color[ Vector::B],
          m_Color[ Vector::R], m_Color[ Vector::G], m_Color[ Vector::B],
          m_Color[ Vector::R], m_Color[ Vector::G], m_Color[ Vector::B] };
    std::memcpy(m_ColorArray, colors, COLOR_ARRAY_SIZE );
}

void Particle::Reset()
{
    SetFlag( Entity::F_ENABLE );
    m_Slowdown = 10.0f;
    m_LifeTime = 1.0f;
    m_FadeTime = float( rand( ) %100 ) / 1000.0f + 0.003f;
    m_Direction = { m_Speed[Vector::X] + float((std::rand()%50)-26.0f)*10.0f ,
                    m_Speed[Vector::Y] + float((std::rand()%50)-25.0f)*10.0f ,
                    m_Speed[Vector::Z] + float((std::rand()%50)-25.0f)*10.0f };
    m_Position = { 0,0,0,0 };
}

bool Particle::DoInitialize( Renderer* renderer )
{
#ifdef USE_VBO
    // We'd load our texture here
    bool hasVBO  = glewGetExtension("GL_ARB_vertex_buffer_object");
    ASSERT( hasVBO, "VBOs not supported!" );

    glGenBuffers(1, (GLuint*)&m_VboID);
    GL_ASSERT( m_VboID > 0, "Error allocating vertex buffer object!" );

    // generate VBO
    glBindBuffer(GL_ARRAY_BUFFER, m_VboID);

    int VBO_SIZE = VERTEX_ARRAY_SIZE + COLOR_ARRAY_SIZE + TEX_COORD_SIZE;
    glBufferData(GL_ARRAY_BUFFER, VBO_SIZE, 0, GL_STATIC_DRAW_ARB);
    // keep static arrays at the bottom
    std::size_t offset(0);
    glBufferSubData(GL_ARRAY_BUFFER, offset, VERTEX_ARRAY_SIZE, sVertexArray);                             // copy vertices starting from 0 offest
    offset += VERTEX_ARRAY_SIZE;
    glBufferSubData(GL_ARRAY_BUFFER, offset, COLOR_ARRAY_SIZE, m_ColorArray);  // copy colors after normals
    offset += COLOR_ARRAY_SIZE;
    glBufferSubData(GL_ARRAY_BUFFER, offset, TEX_COORD_SIZE, sTexCoordArray);  // copy colors after normals
#endif
    return true;
}

void Particle::DoRender()
{
#ifndef USE_INDIRECT
    float *m = GetRenderState()->GetMatrix();
    m[12] = m_Position[Vector::X]; m[13] = m_Position[Vector::Y]; m[14] = m_Position[Vector::Z];

    glPushMatrix();
    glMultMatrixf( m );
#ifdef USE_VBO
    glBindBuffer(GL_ARRAY_BUFFER, m_VboID);
    std::size_t offset(0);
    glVertexPointer(3, GL_FLOAT, 0, (GLvoid*)offset);
    offset += VERTEX_ARRAY_SIZE;
    glColorPointer (3, GL_FLOAT, 0, (GLvoid*)offset);
    offset += COLOR_ARRAY_SIZE;
    glTexCoordPointer(2, GL_FLOAT, 0, (GLvoid*)offset);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
#else // N_USE_VBO
    glVertexPointer(3, GL_FLOAT, 0, sVertexArray);
    glColorPointer (3, GL_FLOAT, 0, m_ColorArray);
    glTexCoordPointer(2, GL_FLOAT, 0, sTexCoordArray);
    glDrawArrays(GL_TRIANGLES, 0, 6);
#endif // N_USE_VBO
    glPopMatrix();
#else
    float x = m_Position[Vector::X];
    float y = m_Position[Vector::Y];
    float z = m_Position[Vector::Z];
    // make this vertex arrays...
    glColor4f( m_Color[ Vector::R], m_Color[ Vector::G], m_Color[ Vector::B], m_Color[ Vector::A] );
    glBegin( GL_TRIANGLE_STRIP );
        glTexCoord2s(1,1); glVertex3f(x+1.0f,y+1.0f, z);
        glTexCoord2s(0,1); glVertex3f(x-1.0f,y+1.0f, z);
        glTexCoord2s(1,0); glVertex3f(x+1.0f,y-1.0f, z);
        glTexCoord2s(0,0); glVertex3f(x-1.0f,y-1.0f, z);
    glEnd();
#endif

}

void Particle::DoUpdate( float ticks )
{
    if ( ticks != 0.0f ) {
        /* Move On The X Axis By X Speed */
        m_Position  += (m_Direction * ( ticks / (m_Slowdown * 1000.0f)) );
        m_Direction += m_Gravity;
        m_LifeTime  -= m_FadeTime;
        if ( m_LifeTime < 0.0f )
        {
            Reset();
        }
    }
}
