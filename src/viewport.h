/*
 * viewport.h
 *
 *  Created on: 2013-01-16
 *      Author: jurgens
 */

#ifndef VIEWPORT_H_
#define VIEWPORT_H_

#include "err.h"
#include "entity.h"

struct Frustum
{
    float m_Left;
    float m_Right;
    float m_Bottom;
    float m_Top;
    float m_ZNear,m_ZFar;

    void Set();
};

class Viewport : public Entity
{
public:
private:
    int     m_Width;
    int     m_Height;
    Vector  m_ClearColor;
    Frustum m_Frustum;
public:
    Viewport( int width, int height );

    virtual ~Viewport();

    void SetClearColor( const Vector& col );

    int GetWidth() const { return m_Width; }

    int GetHeight() const { return m_Height; }
private:
    virtual bool HandleEvent( const SDL_Event& event );

    virtual bool DoInitialize( Renderer* renderer );

    virtual void DoUpdate( float ticks );

    virtual void DoRender() {}
};


#endif /* VIEWPORT_H_ */
