
/*
 * viewport.cpp
 *
 *  Created on: 2013-01-16
 *      Author: jurgens
 */

#include "viewport.h"

#include <GL/glew.h>

void Frustum::Set()
{
    glFrustum( m_Left, m_Right, m_Bottom, m_Top, m_ZNear, m_ZFar );
}

Viewport::Viewport( int width, int height )
    : m_Width(width)
    , m_Height(height)
{
    const float pi = 3.1415926535897932384626433832795f;
    float fov = 60.0f;
    float aspect = float(m_Width)/float(m_Height);
    float zNear = 1.0f;
    float zFar = 1000.0f;
    float fH = std::tan( fov / 360.0f * pi ) * zNear;
    float fW = fH * aspect;
    m_Frustum = { -fW, fW, -fH, fH, zNear, zFar };
}

Viewport::~Viewport()
{
}

bool Viewport::DoInitialize( Renderer* renderer )
{
    return true;
}

bool Viewport::HandleEvent(const SDL_Event& event)
{
    switch (event.type) {
    case SDL_VIDEORESIZE:
        m_Width  = event.resize.w;
        m_Height = event.resize.h;
        break;
    default: break;
    }
    return false;
}

void Viewport::SetClearColor( const Vector& col )
{
    m_ClearColor = col;
}


void Viewport::DoUpdate( float ticks )
{
    // Only 3D we do not care about overlays - just yet
    glViewport(0, 0, (GLsizei)m_Width, (GLsizei)m_Height);

    // set perspective viewing frustum
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    m_Frustum.Set();
    // switch to modelview matrix in order to set scene
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClearColor( m_ClearColor[ Vector::R ],
                  m_ClearColor[ Vector::G ],
                  m_ClearColor[ Vector::B ],
                  m_ClearColor[ Vector::A ] );                   // background color
    // clear buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}



