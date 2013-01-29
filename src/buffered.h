/*
 * buffered.h
 *
 *  Created on: 2013-01-24
 *      Author: jurgens
 */

#ifndef BUFFERED_H_
#define BUFFERED_H_

#include "err.h"
#include "entity.h"
#include "framebuffer.h"
#include "vector.h"
#include "brush.h"
#include "cube.h"

class Buffered : public Cube
{
    FrameBuffer m_FrameBuffer;
public:
    Buffered( );

    virtual ~Buffered();

    virtual bool Initialize( Renderer* renderer );

protected:
    virtual bool DoInitialize( Renderer* renderer );

    virtual void Render( float ticks );

    void DoUpdate( float ticks );
};


#endif /* BUFFERED_H_ */
