
#include "app.h"
#include "err.h"

#include <exception>
#include <iostream>

#ifdef __linux__
#include <gtk/gtk.h>
#endif

static void HandleUnexpected()
{
    ShowError( "Application failed with unexpected Exception!", "Unexpected Exception" );
}

//extern "C" int SDL_main( int argc, char* argv[] )
int main( int argc, char* argv[] )
{
    std::set_unexpected( HandleUnexpected );
	try {
#ifdef __linux__
		gtk_init(&argc,&argv);
#endif
		App app;
		app.Init( argc, argv );
		return app.Run();
	}
	catch ( std::bad_alloc &ex ) {
		ShowError( ex.what(), "Memory Exception in Application" );
	}
    catch ( std::exception &ex ) {
        ShowError( ex.what(), "Exception in Application" );
    }
    catch ( ... ) {
        ShowError( "Unknown Error!", "Unknown Error in Application" );
    }
	return -1;
}
