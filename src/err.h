/*
 * err.h
 *
 *  Created on: 2013-01-15
 *      Author: jurgens
 */

#ifndef ERR_H_
#define ERR_H_

#include <exception>
#include <stdexcept>
#include <cstdio>
#include <boost/assert.hpp>

const char* glErrMessage( int code );

void ShowWindowsError( const char* msg, unsigned long err, const char* header = "SystemError!" );

void ShowError( const char* msg, const char* header = "Error!" );

#define THROW( ... ) \
{                                           \
    char msg[ 256 ];                     \
    snprintf( msg, 256, __VA_ARGS__ );   \
    char buffer[ 480 ];                     \
    std::snprintf( buffer, 480, "%s\nFile: %s\nLine %d\nIn Function: %s", msg, __FILE__, __LINE__,BOOST_CURRENT_FUNCTION );   \
    throw std::runtime_error( buffer );     \
}

#define ASSERT( cond, ... ) if ( !(cond) ) THROW( __VA_ARGS__ );

#define GL_ASSERT( code, ... ) \
    if ( !(code) ) {              \
        char b1[ 256 ];                     \
        std::snprintf( b1, 256, __VA_ARGS__ );   \
        char b2[ 480 ];                     \
        std::snprintf( b2, 480, "%s\nGL_ERROR: %d\n%s\nFile: %s\nLine: %d", b1, ((int)glGetError()), glErrMessage(code), __FILE__, __LINE__ ); \
        throw std::runtime_error( b2 );     \
    }

#endif /* ERR_H_ */
