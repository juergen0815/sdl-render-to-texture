/*
 * framebuffer.h
 *
 *  Created on: 2013-01-24
 *      Author: jurgens
 */

#ifndef FRAMEBUFFER_H_
#define FRAMEBUFFER_H_

#include "err.h"
#include "texture.h"

class FrameBuffer : public Texture
{
public:
    enum enFBFLAGS
    {
        F_ENABLE_DEPTH_BUFFER_F = 1 << 0
    };
private:
    uint32_t   m_Flags;
    int        m_FrameBufferID;
    int        m_DepthBufferID;
    TexturePtr m_Texture;
public:
    FrameBuffer( uint32_t flags = F_ENABLE_DEPTH_BUFFER_F );

    virtual ~FrameBuffer();

    virtual bool Allocate( int width, int height, int type = GL_RGBA );

    void Enable();

    void Disable();

    TexturePtr GetTexture() const;
};


#endif /* FRAMEBUFFER_H_ */
