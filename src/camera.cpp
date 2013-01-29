
/*
 * camera.cpp
 *
 *  Created on: 2013-01-16
 *      Author: jurgens
 */

#include "camera.h"
#include "joystick.h"

#include <GL/glew.h>

#include <SDL/SDL.h>

Camera::Camera( SDL_Joystick* joystick )
    : m_MouseLeftDown(false)
    , m_MouseMiddleDown(false)
    , m_MouseRightDown(false)
    , m_MouseX(0), m_MouseY(0)
    , m_CameraSpeed(1.0)
    , m_CameraAngle( { 0,0,0})
    , m_CameraPosition( {0,0,30} )
    , m_Joystick(joystick)
{
}

Camera::~Camera()
{
}

bool Camera::DoInitialize( Renderer* renderer )
{
    return true;
}

float Camera::GetJoystickAxisValue( int index )
{
    float axisValue = float( SDL_JoystickGetAxis( m_Joystick, index )) * JOY_AXIS_SCALE;
    // always process! otherwise it'll stop if we move to max!
    if ( axisValue > JOY_AXIS_THRESHOLD ) {
        axisValue = JOY_AXIS_THRESHOLD;
    } else if ( axisValue < -JOY_AXIS_THRESHOLD ) {
        // keep signed! But top it off at max
        axisValue = -(JOY_AXIS_THRESHOLD);
    }
    else if ( std::fabs( axisValue ) < JOY_AXIS_DEADZONE) {
        // must reset
        axisValue = 0.0f;
    }
    return axisValue;
}

bool Camera::HandleEvent(const SDL_Event& event)
{
    bool processed(false);
    switch (event.type) {
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
            case SDLK_F5:
                m_MouseLeftDown  = false;
                m_MouseRightDown = false;
                m_MouseX = 0;
                m_MouseY = 0;
                m_CameraPosition = { 0, 0, 30 };
                m_CameraAngle = { 0, 0, 0 };
                processed = true;
                break;
            default: break;
            }
            break;
        case SDL_MOUSEMOTION:
            if ( m_MouseLeftDown ) {
                m_CameraAngle[ Vector::X ] += (event.motion.y - m_MouseY);
                m_CameraAngle[ Vector::Y ] += (event.motion.x - m_MouseX);
                m_MouseX = event.motion.x;
                m_MouseY = event.motion.y;
                processed = true;
            }
            if ( m_MouseRightDown ) {
                m_CameraPosition[ Vector::Z ] -= (event.motion.y - m_MouseY) * 0.2f;
                m_MouseY = event.motion.y;
                processed = true;
            }
            if ( m_MouseMiddleDown ) {
                m_CameraPosition[ Vector::X ] -= (event.motion.x - m_MouseX) * 0.2f;
                m_CameraPosition[ Vector::Y ] -= (event.motion.y - m_MouseY) * 0.2f;
                m_MouseX = event.motion.x;
                m_MouseY = event.motion.y;
                processed = true;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            m_MouseX = event.button.x;
            m_MouseY = event.button.y;

            switch (event.button.button)
            {
            case 1: m_MouseLeftDown   = true; break;
            case 2: m_MouseMiddleDown = true; break;
            case 3: m_MouseRightDown  = true; break;
            default: break;
            }
            break;
        case SDL_MOUSEBUTTONUP:
            switch (event.button.button)
            {
            case 1: m_MouseLeftDown   = false; break;
            case 2: m_MouseMiddleDown = false; break;
            case 3: m_MouseRightDown  = false; break;
            default: break;
            }
            break;

        case SDL_JOYHATMOTION:
            switch ( event.jhat.value )
            {
            case SDL_HAT_LEFT:
                break;
            case SDL_HAT_RIGHT:
                break;
            case SDL_HAT_UP:
                break;
            case SDL_HAT_DOWN:
                break;
            }
            break;
        case SDL_JOYAXISMOTION: {
            m_JoyStickMotionAxis      = { -GetJoystickAxisValue(JOY_AXIS::X_MOTION),-GetJoystickAxisValue(JOY_AXIS::Z_TRIGGER),GetJoystickAxisValue(JOY_AXIS::Y_MOTION)*m_CameraSpeed  };
            m_JoystickOrientationAxis = {  GetJoystickAxisValue(JOY_AXIS::X_ROTATION)*1.75f, GetJoystickAxisValue(JOY_AXIS::Y_ROTATION)*1.75f, 0 };
            } break;
        default: break;
    }
    return processed;
}

void Camera::DoUpdate( float ticks )
{
    m_CameraPosition += m_JoyStickMotionAxis;
    m_CameraAngle    += m_JoystickOrientationAxis;
    Matrix &m = GetRenderState()->GetMatrix();
    // reset matrix to object world
    m.LoadIdentity();
    m.Rotate( m_CameraAngle );
    m.Translate( (const float[4]){m_CameraPosition[Vector::X], -m_CameraPosition[Vector::Y], -m_CameraPosition[Vector::Z], 1.0f});
}
