/*
 * buffered.cpp
 *
 *  Created on: 2013-01-24
 *      Author: jurgens
 */

#include "buffered.h"
#include "viewport.h"
#include "camera.h"
#include "emitter.h"

Buffered::Buffered()
{
}

Buffered::~Buffered()
{
}

bool Buffered::Initialize( Renderer* renderer )
{
    // create offscreen
    int width(512);
    int height(512);
    bool r = m_FrameBuffer.Allocate( width, height );
    ASSERT( r, "Cannot allocate offscreen buffer!");
    m_Texture = m_FrameBuffer.GetTexture();

    // Add a cube
    Viewport *viewport(new Viewport(width, height));
    viewport->SetClearColor( {1.0f, 1.0f, 1.0f } );
    AddEntity( EntityPtr(viewport) );

    EntityPtr particleEmitter( new Emitter(renderer) );
    particleEmitter->GetRenderState()->Translate( Vector(0.0, 0.0, -60), Vector(1.0f, 1.0f, 1.0f) );
    viewport->AddEntity( particleEmitter );

    // Subtree will be initialized here:
    return Cube::Initialize( renderer );
}

bool Buffered::DoInitialize( Renderer* renderer )
{
    // Disable background color
    GetRenderState()->ClearFlag( BLEND_COLOR_F );
    return Cube::DoInitialize( renderer );
}

void Buffered::Render( float ticks )
{
    // backup viewport
    GLint vp[4];
    glGetIntegerv( GL_VIEWPORT, vp );
    GLfloat depth[2];
    glGetFloatv( GL_DEPTH_RANGE, depth );

    // backup projection matrix for internal storage
    Matrix modelview;
    glGetFloatv( GL_MODELVIEW_MATRIX, (float*)modelview );
    Matrix projection;
    glGetFloatv( GL_PROJECTION_MATRIX, (float*)projection );

    m_FrameBuffer.Enable();

    // background color
    glClearColor( 1,1,1,1 );
    // clear buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    for( auto it = m_RenderList.begin(); it != m_RenderList.end(); ) {
        EntityPtr entity = *it;
        if ( entity->AreFlagsSet( Entity::F_ENABLE ) ) {
            entity->Render(ticks);
        }
        if ( entity->AreFlagsSet( Entity::F_DELETE ) ) {
            it = m_RenderList.erase( it );
            continue;
        }
        ++it;
    }
    m_FrameBuffer.Disable();

    // restore view port
    glViewport( vp[0], vp[1], vp[2], vp[3]);
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf( projection );
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf( modelview );

    DoUpdate( ticks );

    // render local list into texture
    glPushMatrix();
    glMultMatrixf(GetRenderState()->GetMatrix());
    Cube::DoRender();
    glPopMatrix();
}

void Buffered::DoUpdate( float ticks )
{
    GetRenderState()->GetMatrix().RotateZ( 360.0f / 5000.0f * ticks );
}

