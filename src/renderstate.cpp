/*
 * renderstate.cpp
 *
 *  Created on: Jan 24, 2013
 *      Author: Jurgen
 */

#include "renderstate.h"

RenderState::RenderState()
    : m_Flags( ALPHA_F|BLEND_F|DEPTH_TEST_F )
{
}

RenderState::~RenderState()
{
}

void RenderState::SetMatrix(const Matrix& matrix)
{
    m_Matrix = matrix;
}

Matrix& RenderState::GetMatrix()
{
    return m_Matrix;
}

const Matrix& RenderState::GetMatrix() const
{
    return m_Matrix;
}

RenderState& RenderState::Translate( const Vector& position, const Vector& scale /* = Vector(1.0f, 1.0f, 1.0f ) */ )
{
    Matrix trans( scale[ Vector::X ],   0.0f,                 0.0f,                 0.0f,
                  0.0f,                 scale[ Vector::Y ],   0.0f,                 0.0f,
                  0.0f,                 0.0f,                 scale[ Vector::Z ],   0.0f,
                  position[ Vector::X ],position[ Vector::Y ],position[ Vector::Z ],1.0f );
    m_Matrix.Mul( trans );
    return *this;
}

RenderState& RenderState::Scale( const Vector& scale )
{
    m_Matrix.Scale( scale );
    return *this;
}

RenderState& RenderState::Rotate( const Vector& rotation )
{
    m_Matrix.RotateX( rotation[ Vector::X ] );
    m_Matrix.RotateY( rotation[ Vector::Y ] );
    m_Matrix.RotateZ( rotation[ Vector::Z ] );
    // need to fix this
//    m_Matrix.Rotate( rotation );
    return *this;
}

