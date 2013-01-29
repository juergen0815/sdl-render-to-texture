/*
 * render.cpp
 *
 *  Created on: 2013-01-15
 *      Author: jurgens
 */

#include "renderer.h"
#include "err.h"
#include "joystick.h"

#include <SDL/SDL.h>

#include <boost/bind.hpp>
//#include <map>

#include <GL/glew.h>

Renderer::Renderer()
	: m_Terminate(false)
    , m_CurrentContext( nullptr )
    , m_CurrentDC( nullptr )
    , m_TimeBase(1.0f)
    , m_Pause(1)
{
}

Renderer::~Renderer()
{
}

void Renderer::Init()
{
    m_CurrentContext = wglGetCurrentContext();
    m_CurrentDC      = wglGetCurrentDC();

    // release current context
    wglMakeCurrent( NULL, NULL );
}

void Renderer::AddEntity( EntityPtr entity, int priority /*= 0*/  )
{
    entity->SetOrder( priority );
    m_InitList.push_back( entity );
}

void Renderer::RemoveEntity( EntityPtr entity )
{
    entity->SetFlag( Entity::F_DELETE );
}

EntityPtr Renderer::FindEntity( const std::string& name )
{

    return EntityPtr();
}

long Renderer::RegisterUpdateFunction( const UpdateFunction& func )
{
    // This is unique
    long key = SDL_GetTicks();
    m_Updaters.insert( std::pair< long, UpdateFunction >(key, func) );
    return key;
}

void Renderer::UnRegisterUpdateFunction( long key )
{
    m_Updaters.erase( key );
}

void Renderer::Terminate()
{
	m_Terminate = true;
}

bool Renderer::HandleEvent( const SDL_Event& event )
{
    switch (event.type)
    {
    case SDL_KEYDOWN:
        switch (event.key.keysym.sym)
        {
        case SDLK_SPACE:
            m_Pause ^= 1;
            break;
        case SDLK_KP_PLUS: {
            m_TimeBase += 0.1f;
            } break;
        case SDLK_KP_MINUS: {
            m_TimeBase -= 0.1f;
            } break;
        case SDLK_KP0:
            m_TimeBase = 0.0f;
            break;
        case SDLK_RETURN:
        case SDLK_KP_ENTER:
        case SDLK_KP1:
            m_TimeBase = 1.0f;
            break;
        case SDLK_KP2:
            m_TimeBase = 2.0f;
            break;
        case SDLK_KP3:
            m_TimeBase = 3.0f;
            break;
        case SDLK_KP7:
            m_TimeBase = 0.75f;
            break;
        case SDLK_KP8:
            m_TimeBase = 0.5f;
            break;
        case SDLK_KP9:
            m_TimeBase = 0.25f;
            break;
        default: break;
        }
        break;
    case SDL_JOYBUTTONUP:
        switch ( event.jbutton.button ) {
        case JOY_BUTTONS::SELECT:
            m_Pause ^= 1;
            break;
        case JOY_BUTTONS::TRIANGLE:
            m_TimeBase += 0.1f;
            break;
        case JOY_BUTTONS::CIRCLE:
            m_TimeBase -= 0.1f;
            break;
        case JOY_BUTTONS::SQUARE:
            m_TimeBase = 1.0f;
            break;
        }
        break;
    default: break;
    }
    return false;
}

void Renderer::InitGL()
{
    // This is important! Our renderer runs its own render thread
    // All
    wglMakeCurrent(m_CurrentDC,m_CurrentContext);

    // Init GLEW - we need this to use OGL extensions (e.g. for VBOs)
    GLenum err = glewInit();
    ASSERT( GLEW_OK == err, "Error: %s\n", glewGetErrorString(err) );

    glShadeModel(GL_SMOOTH);                    // shading mathod: GL_SMOOTH or GL_FLAT
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);      // 4-byte pixel alignment

    // enable /disable features
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
//    glEnable(GL_TEXTURE_2D);
//    glEnable(GL_CULL_FACE);

     // track material ambient and diffuse from surface color, call it before glEnable(GL_COLOR_MATERIAL)
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    glClearStencil(0);                          // clear stencil buffer
    glClearDepth(1.0f);                         // 0 is near, 1 is far
    glDepthFunc(GL_LEQUAL);

    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_ALPHA_TEST);
    glEnable( GL_BLEND );

    GLfloat lightKa[] = {.2f, .2f, .2f, 1.0f};  // ambient light
    GLfloat lightKd[] = {.7f, .7f, .7f, 1.0f};  // diffuse light
    GLfloat lightKs[] = {  1,   1,   1,    1};  // specular light
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightKa);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightKd);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightKs);

    // position the light
    float lightPos[4] = {0, 0, 20, 1}; // positional light
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    glEnable(GL_LIGHT0);                        // MUST enable each light source after configuration

    glClearColor(0, 0, 0, 0);                   // background color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

bool Renderer::CompareEntityPriorities( const EntityPtr& a, const EntityPtr& b ) {
    return a->GetOrder() < b->GetOrder();
}

static void SendTerminate()
{
    // Send QUIT event to main thread
    SDL_Event quit;
    quit.type = SDL_QUIT;
    SDL_PushEvent( &quit );
}

static void HandleUnexpected()
{
    ShowError( "Renderer failed with unexpected Exception!", "Unexpected Exception" );
    exit(-20);
}

void Renderer::Run()
{
    std::set_terminate( SendTerminate );
    std::set_unexpected( HandleUnexpected );
    try {
        InitGL();

        float timeStamp = float(SDL_GetTicks());
        do {
            // first step: iterate through a list of newly added entities and initialize them properly
            //             Must be done in the context of the render thread.
            //             Limit the number of initializations to 5 to not stall the render loop

            int resort(0);
            // max init 5 entities at one time to not stall the render loop forever
            while ( m_InitList.size() > 0 ) {
                auto& entity = m_InitList.front();
                entity->Initialize( this );
                // should be a sorted insert...but would probably slow the container (change to set/multiset??)
                m_RenderList.push_back(entity);
                m_InitList.pop_front();
                ++resort;
            }

            // second step: If we added more than 1 entity, resort the render list

            // only resort if we added more than one entity
            if ( resort > 0 ) {
                m_RenderList.sort( boost::bind( &Renderer::CompareEntityPriorities, this, _1, _2) );
            }

            // third step: render all entities

            float ticks = SDL_GetTicks();

            // Call all updater callbacks (once per frame)
            for ( auto doUpdate = m_Updaters.begin(); doUpdate != m_Updaters.end(); ) {
                bool remove = doUpdate->second( (timeStamp - ticks)*m_TimeBase*float(m_Pause) );
                if ( remove ) {
                    doUpdate = m_Updaters.erase( doUpdate );
                    continue;
                }
                ++doUpdate;
            }

            // No Scene graph, no nested objects, no tree...must unroll in reverse order (see below)
            // DONT DO THIS. Just to keep it simple! Use a scene graph instead!

            // run list
            for( auto it = m_RenderList.begin(); it != m_RenderList.end(); ) {
                EntityPtr entity = *it;
                if ( entity->AreFlagsSet( Entity::F_ENABLE ) ) {
                    entity->Render( (timeStamp - ticks)*m_TimeBase*float(m_Pause) );
                }
                if ( entity->AreFlagsSet( Entity::F_DELETE ) ) {
                    it = m_RenderList.erase( it );
                    continue;
                }
                ++it;
            }

            // fourth: swap the buffers
            // Swap the buffer
            SDL_GL_SwapBuffers();
            // Store timestamp after we have rendered all entities
            timeStamp = ticks;

        } while (!m_Terminate);

        m_Updaters.clear();
        m_RenderList.clear();
    }
    catch ( std::bad_alloc & ex ) {
        ShowError( ex.what(), "Renderer failed with Memory Exception!" );
        m_Updaters.clear();
        m_RenderList.clear();
        m_Terminate = true;
        SendTerminate();
    }
    catch ( std::exception &ex ) {
        ShowError( ex.what(), "Renderer failed with Exception!" );
        m_Updaters.clear();
        m_RenderList.clear();
        m_Terminate = true;
        SendTerminate();
    }
    catch ( ... ) {
        ShowError( "Unknown Error!", "Renderer failed with unknown Exception!" );
        m_Updaters.clear();
        m_RenderList.clear();
        m_Terminate = true;
        SendTerminate();
    }
}

