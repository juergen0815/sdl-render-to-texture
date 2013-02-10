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
#include "cube.h"

#include <bmp_loader.h>

Buffered::Buffered()
{
}

Buffered::~Buffered()
{
}

static int rec = 3;

bool Buffered::Initialize( Renderer* renderer )
{
    // create offscreen
    int width(512);
    int height(512);
    bool r = m_FrameBuffer.Allocate( width, height );
    ASSERT( r, "Cannot allocate offscreen buffer!");
    m_Texture = m_FrameBuffer.GetTexture();

#if 0
    // Add a cube
    Viewport *viewport(new Viewport(width, height));
    viewport->SetClearColor( {1.0f, 1.0f, 1.0f } );
    AddEntity( EntityPtr(viewport) );

    EntityPtr particleEmitter( new Emitter(renderer) );
    particleEmitter->GetRenderState()->Translate( Vector(0.0, 0.0, -60), Vector(1.0f, 1.0f, 1.0f) );
    viewport->AddEntity( particleEmitter );
#endif
    boost::shared_ptr<BmpBrush> bmpBrush( new BmpBrush );
    ASSERT( bmpBrush->Load( "data/Floor_D.bmp"), "BMP Load error!" );

    ////////////////////////////////////////////////////////////////////////////
    // Compose our scene

    // Add a cube
    EntityPtr viewport(new Viewport(width, height));
    // this entity renders
    AddEntity(viewport);

    // Add the camera
    EntityPtr camera(new Camera(NULL));
    // this entity renders
    viewport->AddEntity(camera, 0);

    EntityPtr cube( new Cube() );
    cube->GetRenderState()->Translate( Vector(-8.0, 0, 10), Vector(2.0f, 2.0f, 2.0f) );
    cube->GetRenderState()->Rotate( Vector(0.0f, 0.0f, 0.0f ) );
    // this entity renders
    camera->AddEntity(cube, 20 );

    EntityPtr cube2( new Cube( bmpBrush ) );
    cube2->GetRenderState()->Translate( Vector(+8.0, 0, 10), Vector(2.0f, 2.0f, 2.0f) );
    cube2->GetRenderState()->Rotate( Vector(0.0f, 0.0f, 0.0f ) );
    // this entity renders
    camera->AddEntity(cube2, 20 );

    if ( --rec > 0 ) {
        // this creates a recursion
        EntityPtr buffered( new Cube );
        buffered->GetRenderState()->Translate( Vector(0.0, 0.0, 15.0), Vector(3.0f, 3.0f, 3.0f) );
        // this entity renders
        camera->AddEntity(buffered, 30 );

    }

#define PARTICLES
#ifdef PARTICLES
    EntityPtr particleEmitter( new Emitter( renderer ) );
    particleEmitter->GetRenderState()->Translate( Vector(0.0, 0.0, -30.0), Vector(1.0f, 1.0f, 1.0f) );
    // this entity renders
    camera->AddEntity(particleEmitter, 100 );
#endif

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

