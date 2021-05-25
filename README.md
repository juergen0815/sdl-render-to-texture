Render to Texture

Author:

	Jurgen Schober
	
Date:
   
	Initial: April, 2012

    Updated: May, 2021
	
Short:
  
	Example to handle render to texture in a very lean OOP way (like a surface)

Description:

	This is an example how to render into a texture in an OOP way. This scene 
    renders a couple of primitives and particles into a "Texture" objects (kind of a 
    surface) and then uses this surface as a texture mapped onto a cube. 

    This demo also uses a couple of other tricks where it uses memory mapped files 
    as textures. Bascially it "streams" bmp images directly from the file system 
    into a pixel buffer of the surface (texture).

    The particle engine uses custom memory allocators to pool particles to avoid 
    large memory fragmentation per particles. Each particle is an "Entity" (those 
    form the scene graph). As such those a true 3D entities in the scene graph and 
    this demonstrates limited live time of 3D objects with a lot of add/remove in 
    the scene graph.

	This is 100% portable C++ code using boost libraries. Other than windows 
    bootstrappers this code does not contain any OS specific code.

Libs used:

    SDL
	boost_thread
	boost_system
	boost_io
    boost_filesystem

	using timers (IOService), boost threads and stdc++ queues for event handling (custom event queue).

License:

	Use as is. No license other then the ones included with third party libraries are required.

Compiler used:

	Tested on Windows with Visual Studio Code, C++ plugin, VSC++ Community Edition.
    Mingw64 and Linux (and mac) should work or need minor adjustments.

Have fun
Jurgen
