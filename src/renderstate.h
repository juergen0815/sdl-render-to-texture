/*
 * renderstate.h
 *
 *  Created on: Jan 24, 2013
 *      Author: Jurgen
 */

#ifndef RENDERSTATE_H_
#define RENDERSTATE_H_

#include "matrix.h"
#include "vector.h"

class RenderState
{
public:
    enum {
       ALPHA_F = 1<<0,
       BLEND_F = 1<<1,
       DEPTH_TEST_F = 1<<2,

       USER_B  = 20,                // above bit 20 is user space
       SYS_MASK = (1<<USER_B)-1,
    };
protected:
    uint32_t m_Flags;
    Matrix   m_Matrix;
public:
    RenderState();

    virtual ~RenderState();

    void SetMatrix( const Matrix& matrix );

    Matrix& GetMatrix();

    const Matrix& GetMatrix() const;

    RenderState& Translate( const Vector& position, const Vector& scale = Vector(1.0f, 1.0f, 1.0f ) );

    RenderState& Scale( const Vector& scale );

    RenderState& Rotate( const Vector& rotation );

    uint32_t GetFlags() const { return m_Flags; }

    void SetFlag( uint32_t flag ) { m_Flags |= flag; }

    void ClearFlag( uint32_t flag ) { m_Flags &= ~flag; }

};

#endif /* RENDERSTATE_H_ */
