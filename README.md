Sequencer

Author:

	Jurgen Schober
	
Date:
   
	April, 2012
	
Short:
  
	Event driven sequencer example.

Description:

	A short example which demonstrates time based events triggering "Effects" as a linear sequence. 
	An Effect itself can send events. Also contains a custom boost based event queue. 
	This is 100% portable C++ code using boost libraries.

Libs used:

	boost_thread
	boost_system

	using timers (IOSerive), boost threads and stdc++ queues for event handling (custom event queue).

License:

	Use as is. No license other then the ones included with third party libraries are required.

Compiler used:

	MinGW with Eclipse 3.7 Indigo (Mac and Linux targets should work, too).

TODO:

	* Move this to CMake and decouple from Eclipse. I use VSC as of late.

Have fun
Jurgen
