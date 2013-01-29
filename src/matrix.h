/*
 * matrix.h
 *
 *  Created on: Jan 24, 2013
 *      Author: Jurgen
 */

#ifndef MATRIX_H_
#define MATRIX_H_

#include "err.h"

#include <cstdlib>
#include <cstring>
#include <cmath>

class Matrix
{
private:
    float m[16]; // 12/13/14 -> x/y/z
public:
    inline Matrix()
    {
        LoadIdentity();
    }

    inline Matrix( const float ma[16] )
    {
        std::memcpy( m, ma, sizeof(m) );
    }

    inline Matrix( const Matrix& matrix )
    {
        std::memcpy( m, matrix.m, sizeof(m) );
    }

    inline Matrix( float m0, float m1, float m2, float m3,
                   float m4, float m5, float m6, float m7,
                   float m8, float m9, float m10,float m11,
                   float m12,float m13,float m14,float m15)
    {
        m[0]  = m0;  m[1]  = m1;  m[2]  = m2;  m[3]  = m3;
        m[4]  = m4;  m[5]  = m5;  m[6]  = m6;  m[7]  = m7;
        m[8]  = m8;  m[9]  = m9;  m[10] = m10; m[11] = m11;
        m[12] = m12; m[13] = m13; m[14] = m14; m[15] = m15;
    }

    inline Matrix& LoadIdentity( )
    {
        m[0] = 1; m[1] = 0; m[2]  = 0; m[3] = 0;
        m[4] = 0; m[5] = 1; m[6]  = 0; m[7] = 0;
        m[8] = 0; m[9] = 0; m[10] = 1; m[11]= 0;
        m[12]= 0; m[13]= 0; m[14] = 0; m[15]= 1;
        return *this;
    }

    inline Matrix& Set( float m0, float m1, float m2, float m3,
                        float m4, float m5, float m6, float m7,
                        float m8, float m9, float m10,float m11,
                        float m12,float m13,float m14,float m15)
    {
        m[0]  = m0;  m[1]  = m1;  m[2]  = m2;  m[3]  = m3;
        m[4]  = m4;  m[5]  = m5;  m[6]  = m6;  m[7]  = m7;
        m[8]  = m8;  m[9]  = m9;  m[10] = m10; m[11] = m11;
        m[12] = m12; m[13] = m13; m[14] = m14; m[15] = m15;
        return *this;
    }

    inline Matrix& Add( const float mat[16] )
    {
        for(int i = 0;i < 4;i++) {
            for(int j = 0;j < 4;j++) {
                m[i + j * 4] += mat[i + j * 4];
            }
        }
        return *this;
    }

    inline Matrix& Sub( const float mat[16] )
    {
        for(int i = 0;i < 4;i++) {
            for(int j = 0;j < 4;j++) {
                m[i + j * 4] -= mat[i + j * 4];
            }
        }
        return *this;
    }

    inline Matrix& Mul( const float mat[16] )
    {
        for (int i = 0; i < 4; i++)
        {
            // rotate m1
            float a = m[ i * 4 + 0 ];
            float b = m[ i * 4 + 1 ];
            float c = m[ i * 4 + 2 ];
            float d = m[ i * 4 + 3 ];

            m[i * 4 + 0] = a * mat[0] + b * mat[4] + c * mat[8]  + d*mat[12];
            m[i * 4 + 1] = a * mat[1] + b * mat[5] + c * mat[9]  + d*mat[13];
            m[i * 4 + 2] = a * mat[2] + b * mat[6] + c * mat[10] + d*mat[14];
            m[i * 4 + 3] = a * mat[3] + b * mat[7] + c * mat[11] + d*mat[15];
        }
        return *this;
    }

    inline Matrix& Mul3( const float mat[16] )
    {
        for (int i = 0; i < 3; i++)
        {
            // rotate m1
            float a = m[ i * 4 + 0 ];
            float b = m[ i * 4 + 1 ];
            float c = m[ i * 4 + 2 ];

            m[i * 4 + 0] = a * mat[0] + b * mat[4] + c * mat[8];
            m[i * 4 + 1] = a * mat[1] + b * mat[5] + c * mat[9];
            m[i * 4 + 2] = a * mat[2] + b * mat[6] + c * mat[10];
        }
        return *this;
    }

    inline Matrix& Transpose()
    {
        return Set( m[0],  m[4],  m[8],  m[12],
                    m[1],  m[5],  m[9],  m[13],
                    m[2],  m[6],  m[10], m[14],
                    m[3],  m[7],  m[11], m[15]);
    }

    inline Matrix& Transpose( const Matrix& mat )
    {
        return Set( mat.m[0],  mat.m[4],  mat.m[8],  mat.m[12],
                    mat.m[1],  mat.m[5],  mat.m[9],  mat.m[13],
                    mat.m[2],  mat.m[6],  mat.m[10], mat.m[14],
                    mat.m[3],  mat.m[7],  mat.m[11], mat.m[15]);
        return *this;
    }

    inline Matrix& Translate( const float vector[4] )
    {
        // OpenGL style translation
        float x = vector[0];
        float y = vector[1];
        float z = vector[2];

        m[12] += m[0]*x + m[4]*y + m[8] *z;
        m[13] += m[1]*x + m[5]*y + m[9] *z;
        m[14] += m[2]*x + m[6]*y + m[10]*z;
        return *this;
    }

    inline Matrix& Scale( const float s[4] )
    {
        float sx(s[0]), sy(s[1]), sz(s[2]);
        m[0] *= sx; m[4] *= sy; m[8]  *= sz;
        m[1] *= sx; m[5] *= sy; m[9]  *= sz;
        m[2] *= sx; m[6] *= sy; m[10] *= sz;
        return *this;
    }

    inline Matrix& Scale( float s )
    {
        m[0] *= s; m[4] *= s; m[8]  *= s;
        m[1] *= s; m[5] *= s; m[9]  *= s;
        m[2] *= s; m[6] *= s; m[10] *= s;
        return *this;
    }

    inline Matrix& RotateZ( float angle )
    {
        static const float RAD = 0.017453292519943295769236907684886f;

        float alpha = angle * RAD;
        float sina = std::sin( alpha );
        float cosa = std::cos( alpha );

        Matrix rot ( cosa,  sina,  0.0f,  0.0f,
                    -sina,  cosa,  0.0f,  0.0f,
                     0.0f,  0.0f,  1.0f,  0.0f,
                     0.0f,  0.0f,  0.0f,  1.0f);
//            *this = rot.Mul3( *this );
//            return *this;
        return Mul3( rot );
    }

    inline Matrix& RotateX( float angle )
    {
        static const float RAD = 0.017453292519943295769236907684886f;

        float alpha = angle * RAD;
        float sina = std::sin( alpha );
        float cosa = std::cos( alpha );

        Matrix rot (1.0f,  0.0f,  0.0f,   0.0f,
                    0.0f,  cosa,  sina,   0.0f,
                    0.0f, -sina,  cosa,   0.0f,
                    0.0f,  0.0f,  0.0f,   1.0f);
//        *this = rot.Mul3( *this );
//        return *this;
        return Mul3( rot );
    }

    inline Matrix& RotateY( float angle )
    {
        static const float RAD = 0.017453292519943295769236907684886f;

        float alpha = angle * RAD;
        float sina = std::sin( alpha );
        float cosa = std::cos( alpha );

        Matrix rot ( cosa,  0.0f, -sina,   0.0f,
                     0.0f,  1.0f,  0.0f,   0.0f,
                     sina,  0.0f,  cosa,   0.0f,
                     0.0f,  0.0f,  0.0f,   1.0f );
//            *this = rot.Mul3( *this );
//            return *this;
        return Mul3( rot );

    }

    inline Matrix& Rotate( const float v[4] )
    {
        // see: http://www.sjbaker.org/steve/omniv/matrices_can_be_your_friends.html
        /* Rotation around Z axis : c = cos, s = sin
        c -s 0 0
        s c 0 0
        0 0 1 0
        0 0 0 1

        in OpenGL format (column-major order)

        c s 0 0 -s c 0 0 0 0 1 0 0 0 0 1
        */

        static const float RAD = 0.017453292519943295769236907684886f;

#if 0
        if ( std::fabs(v[0]) > 0 ) {
            return RotateX( v[0] );
        }
        if ( std::fabs(v[1]) > 0 ) {
            return RotateY( v[1] );
        }
        if ( std::fabs(v[2]) > 0 ) {
            return RotateZ( v[2] );
        }
#else
        if ( v[0] == 0 ) {
            if ( v[1] == 0 ) {
                if ( v[2] != 0 ) {
                    return RotateZ( v[2] );
                }
            } else if ( v[2] == 0 ) {
                return RotateY( v[1] );
            }
        } else if ( v[1] == 0 ) {
            if ( v[2] == 0 ) {
                return RotateX( v[0] );
            }
        } else {

            float x_rad = v[0] * RAD;
            float y_rad = v[1] * RAD;
            float z_rad = v[2] * RAD;

            float sx = std::sin( x_rad );
            float cx = std::cos( x_rad );
            float sy = std::sin( y_rad );
            float cy = std::cos( y_rad );
            float sz = std::sin( z_rad );
            float cz = std::cos( z_rad );

            Matrix rot((cy * cz - sy * sx * sz), (cy * sz + sy * sx * cz), (-sy * cx), 0.0f,
                          (cx * -sz),               (cx * cz),                sx,         0.0f,
                          (sy * cz + cy * sx * sz), (sy * sz - cy * sx * cz), (cy * cx),  0.0f,
                           0.0f,                     0.0f,                     0.0f,      1.0f);
            return Mul3( rot );
//                *this = rot.Mul3( *this );
        }
#endif
        return *this;
    }

    inline Matrix& operator=( const float ma[16] )
    {
        std::memcpy( m, ma, sizeof(m) );
        return *this;
    }

    inline Matrix& operator=( const Matrix& matrix )
    {
        std::memcpy( m, matrix.m, sizeof(m) );
        return *this;
    }

    inline Matrix operator!()
    {
        return Matrix(*this).Transpose();
    }

    inline float operator[](unsigned int i) const
    {
        ASSERT( i < 16, "Matrix index out of bounds: %d", i );
        return m[i];
    }

    inline float& operator[](unsigned int i)
    {
        ASSERT( i < 16, "Matrix index out of bounds: %d", i );
        return m[i];
    }

    inline operator float* () {
        return m;
    }

    inline operator const float* () const {
        return m;
    }

    inline bool operator==( const Matrix& o ) const {
        for ( int i = 0; i < 16; i++ ) {
            if ( m[i] != o.m[i] ) {
                return false;
            }
        }
        return true;
    }

    inline bool operator!=( const Matrix& o ) const {
        return !((*this)==o);
    }
};


#endif /* MATRIX_H_ */
