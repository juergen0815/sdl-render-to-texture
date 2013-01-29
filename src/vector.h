/*
 * vector.h
 *
 *  Created on: 2013-01-17
 *      Author: jurgens
 */

#ifndef VECTOR_H_
#define VECTOR_H_

#include "err.h"

#include <cmath>
#include <cstring>

class Vector
{
public:
    enum Coord {
        X, Y, Z, W
    };
    enum Color {
        R, G, B, A
    };
private:
    float vec[4];

public:
    Vector() : vec({0,0,0,1})
    {
    }

    Vector( const Vector& v )
    {
        std::memcpy(vec,v.vec, 4*sizeof(float));
    }

    Vector( const float v[4] )
    {
        std::memcpy(vec,v, 4*sizeof(float));
    }

    Vector( float x, float y, float z )
    {
        vec[0] = x;
        vec[1] = y;
        vec[2] = z;
        vec[3] = 1;
        // That's kind a cool...but rather slow...so test only
//        std::memcpy(vec,(float[4]){ x,y,z,1 }, 4*sizeof(float));
    }

    Vector( float r, float g, float b, float a )
    {
        vec[0] = r;
        vec[1] = g;
        vec[2] = b;
        vec[3] = a;
    }

    operator float* () { return vec; }

    operator const float* () const { return vec; }

    float operator[]( Vector::Coord coord ) const {
        return vec[ (int)coord ];
    }

    float operator[]( Vector::Color color ) const {
        return vec[ (int)color ];
    }

    float& operator[]( Vector::Coord coord ) {
        return vec[ (int)coord ];
    }

    float& operator[]( Vector::Color color ) {
        return vec[ (int)color ];
    }

    void operator+=( float v ) {
        vec[0] += v;
        vec[1] += v;
        vec[2] += v;
        vec[3] += v;
    }

    void operator-=( float v ) {
        vec[0] -= v;
        vec[1] -= v;
        vec[2] -= v;
        vec[3] -= v;
    }

    void operator*=( float v ) {
        vec[0] *= v;
        vec[1] *= v;
        vec[2] *= v;
        vec[3] *= v;
    }

    void operator+=( const Vector& v )
    {
        vec[0] += v.vec[0];
        vec[1] += v.vec[1];
        vec[2] += v.vec[2];
        vec[3] += v.vec[3];
    }

    void operator-=( const Vector& v )
    {
        vec[0] -= v.vec[0];
        vec[1] -= v.vec[1];
        vec[2] -= v.vec[2];
        vec[3] -= v.vec[3];
    }

    void operator*=( const Vector& v )
    {
        vec[0] *= v.vec[0];
        vec[1] *= v.vec[1];
        vec[2] *= v.vec[2];
        vec[3] *= v.vec[3];
    }

    Vector& Add( const Vector& v )
    {
        *this += v;
        return *this;
    }

    Vector operator+( const Vector& v ) const
    {
        return Vector(*this).Add(v);
    }

    Vector& Sub( const Vector& v )
    {
        *this -= v;
        return *this;
    }

    Vector operator-( const Vector& v ) const
    {
        return Vector(*this).Sub( v );
    }

    Vector& Mul( const Vector& v )
    {
        *this *= v;
        return *this;
    }

    Vector operator*( const Vector& v ) const
    {
        return Vector(*this).Mul( v );
    }

    Vector& Add( float v )
    {
        *this += v;
        return *this;
    }

    Vector operator+( float v ) const
    {
        return Vector(*this).Add( v );
    }

    Vector& Sub( float v )
    {
        *this -= v;
        return *this;
    }

    Vector operator-( float v ) const
    {
        return Vector(*this).Sub( v );
    }

    Vector& Mul( float v )
    {
        *this *= v;
        return *this;
    }

    Vector operator*( float v ) const
    {
        return Vector(*this).Mul( v );
    }

    float Magnitude() const
    {
        // only x,y,z values - ignore w. Always positive. There is no neg sqrt
        return std::sqrt( vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2] );
    }

    float Dot( const Vector& v ) const
    {
        return vec[0]*v.vec[0] + vec[1]*v.vec[1] + vec[2]*v.vec[2];
    }

    Vector& Cross( const Vector& v )
    {
        float x = vec[Y] * v.vec[Z] - vec[Z] * v.vec[Y];
        float y = vec[Z] * v.vec[X] - vec[X] * v.vec[Z];
        float z = vec[X] * v.vec[Y] - vec[Y] * v.vec[X];

        vec[X] = x;
        vec[Y] = y;
        vec[Z] = z;
        vec[W] = 0;
        return *this;
    }

    Vector& Normalize()
    {
        // Magnitude is always positive!
        float length = Magnitude();
        if ( length > 0 ) {
            Mul( 1/length );
        }
        return *this;
    }

    Vector Normalized() const
    {
        return Vector( *this ).Normalize();
    }
};


#endif /* VECTOR_H_ */
