/*
 * err.cpp
 *
 *  Created on: 2013-01-15
 *      Author: jurgens
 */

#include <windows.h>

#include <GL/glew.h>

#include <cctype>
#include <cstring>

#include <iostream>
#include <sstream>

void ShowWindowsError( const char* msg, unsigned long err, const char* header /*= "Error!"*/ )
{
    const UINT bufSize(2048);
    LPSTR lpBuffer = (LPSTR)LocalAlloc( 0, bufSize + 1);
    if ( lpBuffer ) {
        if ( std::strlen( header) == 0 ) { header = "Error!"; }
        DWORD len = FormatMessageA( FORMAT_MESSAGE_FROM_SYSTEM,
                   NULL,
                   err,
                   LANG_SYSTEM_DEFAULT,
                   lpBuffer,
                   bufSize,
                   NULL );
        if ( len > 0 ) {
    		std::stringstream text;
    		text << msg << "\nCode: " << err << " - " << lpBuffer << std::ends;
    		std::stringbuf *buf = text.rdbuf();
    		MessageBoxA(HWND_DESKTOP, (const char*)buf->str().c_str(), header, MB_OK);
        }
        LocalFree( lpBuffer );
    }
}

void ShowError( const char* msg, const char* header /*= "Error!"*/ )
{
	MessageBoxA(HWND_DESKTOP, msg, header, MB_OK);
}

const char* glErrMessage( int code )
{
    switch ( code )
    {
    case GL_NO_ERROR:
        return "No error has been recorded. The value of this symbolic constant is guaranteed to be 0.";
    case GL_INVALID_ENUM:
        return "An unacceptable value is specified for an enumerated argument.\nThe offending command is ignored and has no other side effect than to set the error flag.";
    case GL_INVALID_VALUE:
        return "A numeric argument is out of range.\nThe offending command is ignored and has no other side effect than to set the error flag.";
    case GL_INVALID_OPERATION:
        return "The specified operation is not allowed in the current state.\nThe offending command is ignored and has no other side effect than to set the error flag.";
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        return "The framebuffer object is not complete.\nThe offending command is ignored and has no other side effect than to set the error flag.";
    case GL_OUT_OF_MEMORY:
        return "There is not enough memory left to execute the command.\nThe state of the GL is undefined, except for the state of the error flags, after this error is recorded.";
    case GL_STACK_UNDERFLOW:
        return "An attempt has been made to perform an operation that would cause an internal stack to underflow.";
    case GL_STACK_OVERFLOW:
        return "An attempt has been made to perform an operation that would cause an internal stack to overflow.";
    default:
        return "Unknown GL Error!";
    }
}
