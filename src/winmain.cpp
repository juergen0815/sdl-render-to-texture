/************************************************************************/
/* pei::EngineToolkit (©)Copyright 2021 Jürgen Schober                  */
/*                                                                      */
/* This Software is distributed as Freeware. It may be copied and       */
/* distributed (even as a companion to a commercial product) as long as */
/* the archive stays complete. No further agreement is necessary.       */
/************************************************************************/

#if defined (_WIN32) && !defined (__GNUC__)

#include <Windows.h>

extern "C" int main( int argc, char* argv[] );

PCHAR* CommandLineToArgvA( PCHAR CmdLine, int* _argc )
{
    PCHAR* argv;
    PCHAR  _argv;
    ULONG   len;
    ULONG   argc;
    CHAR   a;
    ULONG   i, j;

    BOOLEAN  in_QM;
    BOOLEAN  in_TEXT;
    BOOLEAN  in_SPACE;

    len = strlen(CmdLine);
    i = ((len+2)/2)*sizeof(PVOID) + sizeof(PVOID);

    argv = (PCHAR*)GlobalAlloc(GMEM_FIXED,
        i + (len+2)*sizeof(CHAR));

    _argv = (PCHAR)(((PUCHAR)argv)+i);

    argc = 0;
    argv[argc] = _argv;
    in_QM = FALSE;
    in_TEXT = FALSE;
    in_SPACE = TRUE;
    i = 0;
    j = 0;

    while( (a = CmdLine[i]) != '\0' ) {
        if(in_QM) {
            if(a == '\"') {
                in_QM = FALSE;
            } else {
                _argv[j] = a;
                j++;
            }
        } else {
            switch(a) {
                case '\"':
                    in_QM = TRUE;
                    in_TEXT = TRUE;
                    if(in_SPACE) {
                        argv[argc] = _argv+j;
                        argc++;
                    }
                    in_SPACE = FALSE;
                    break;
                case ' ':
                case '\t':
                case '\n':
                case '\r':
                    if(in_TEXT) {
                        _argv[j] = '\0';
                        j++;
                    }
                    in_TEXT = FALSE;
                    in_SPACE = TRUE;
                    break;
                default:
                    in_TEXT = TRUE;
                    if(in_SPACE) {
                        argv[argc] = _argv+j;
                        argc++;
                    }
                    _argv[j] = a;
                    j++;
                    in_SPACE = FALSE;
                    break;
            }
        }
        i++;
    }
    _argv[j] = '\0';
    argv[argc] = NULL;

    (*_argc) = argc;
    return argv;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLin, int nShowCmd )
{
    int r = ERROR_FILE_NOT_FOUND;
    try {
        int argc;

        LPSTR wcmdline = GetCommandLineA();
        PCHAR *argv = CommandLineToArgvA( wcmdline, &argc );
        main( argc, argv );

        LocalFree(argv);
    } catch ( ... ) {
        // exception from boost
    }
    return r;
}

#elif defined (_WIN32) && defined (__GNUC__) // mingw

#include <stddef.h>  // NULL
#include <ctype.h>   // ispace
#include <stdarg.h>
#include <windef.h>  // WINAPI
#include <winbase.h>

#include <vector>
#include <string>
#include <algorithm>

static void UnEscapeQuotes( char *arg )
{
    char *last = NULL;

    while( *arg ) {
        if( *arg == '"' && *last == '\\' ) {
            char *c_curr = arg;
            char *c_last = last;

            while( *c_curr ) {
                *c_last = *c_curr;
                c_last = c_curr;
                c_curr++;
            }
            *c_last = '\0';
        }
        last = arg;
        arg++;
    }
}

static std::vector< const char* > ParseCommandLine(char *cmdline)
{
    char *bufp;
    char *lastp = NULL;
    int argc, last_argc;

    std::vector< const char * > args;
    // make sure array is big enough - avoid reallocs
    char *arg = NULL;
    argc = last_argc = 0;
    for ( bufp = cmdline; *bufp; ) {
        /* Skip leading whitespace */
        while ( isspace(*bufp) ) {
            ++bufp;
        }
        /* Skip over argument */
        if ( *bufp == '"' ) {
            ++bufp;
            if ( *bufp ) {
                arg = bufp;
                ++argc;
            }
            /* Skip over word */
            while ( *bufp && ( *bufp != '"' || *lastp == '\\' ) ) {
                lastp = bufp;
                ++bufp;
            }
        } else {
            if ( *bufp ) {
                arg = bufp;
                ++argc;
            }
            /* Skip over word */
            while ( *bufp && ! isspace(*bufp) ) {
                ++bufp;
            }
        }
        if ( *bufp ) {
            if ( arg ) {
                *bufp = '\0';
            }
            ++bufp;
        }
        /* Strip out \ from \" sequences */
        if( arg && last_argc != argc ) {
            UnEscapeQuotes( arg );
        }
        if ( arg ) {
            args.push_back( arg );
        }
        last_argc = argc;
    }
    return args;
}

#include <iostream>

extern "C" int main( int argc, const char* argv[] );

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLin, int nShowCmd )
{
    char *bufp;
    size_t nLen;

    bufp = GetCommandLine();
    nLen = strlen( bufp );

    // must copy to terminate strings
    char* cmdline = new char[nLen+1];
    strncpy( cmdline, bufp, nLen );
    cmdline[nLen] = 0;
    std::vector< const char* > args = ParseCommandLine( cmdline );
    return main( args.size(), &args[0] );
}

#endif // _WIN32 && __GNUC__ (== __MING32W__)
