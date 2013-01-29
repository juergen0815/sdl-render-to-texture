/*
 * texture.cpp
 *
 *  Created on: Jan 19, 2013
 *      Author: Jurgen
 */

#include "texture.h"

#include <GL/glew.h>

Texture::Texture()
    : m_TextID( -1 )
    , m_Width(0)
    , m_Height(0)
{
}

Texture::~Texture()
{
    if ( m_TextID > -1 ) {
        glDeleteTextures(1,(GLuint*)&m_TextID);
    }
}

bool Texture::Allocate( int width, int height, int type )
{
    bool r(false);
    m_Width  = width;
    m_Height = height;
//    GLenum code = glGetError();
    glGenTextures( 1, (GLuint*)&m_TextID );
    ASSERT( m_TextID > 0, "Error generating texture!" );
//    code = glGetError();
//    GL_ASSERT( code, "Error generating texture!" );
//    if ( code == GL_NO_ERROR )
    {
        glBindTexture( GL_TEXTURE_2D, m_TextID );

        /* Linear Filtering */
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

        // TODO: use flags to set wrap mode
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // TODO: use flags to switch mip maps on/off
        glTexParameterf(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

        // generate pixel buffer in vmem
        glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, NULL);
        r = true;
    }
    return r;
}

void Texture::Load( const char* pixels, int width, int height, int type )
{
    if ( m_TextID <= 0 ) {
        Allocate( width, height, type );
    }
    Enable();
    // load pixels into texture memory
    if ( width > m_Width || height > m_Height ) {
        // Size increased
        glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, NULL);
        m_Width  = width;
        m_Height = height;
    }
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, type, GL_UNSIGNED_BYTE, pixels);
}

void Texture::Load( const Brush& brush )
{
    ASSERT( brush.m_Pixels && ( brush.m_BytesPerPixel == 3 || brush.m_BytesPerPixel == 4 ),
            "Invalid brush for texture!" );

    Load( brush.m_Pixels, brush.m_Width, brush.m_Height, brush.m_BytesPerPixel == 3 ? GL_RGB : GL_RGBA );
}

unsigned int Texture::GetTextureId() const
{
    return m_TextID;
}

void Texture::Bind() const
{
    ASSERT( m_TextID > -1, "Invalid Texture ID" );

    /* Typical Texture Generation Using Data From The Bitmap */
    glBindTexture( GL_TEXTURE_2D, m_TextID );
}

void Texture::Enable() const
{
    glEnable( GL_TEXTURE_2D );
    Bind();
}

void Texture::Disable() const
{
    glDisable( GL_TEXTURE_2D );
}

