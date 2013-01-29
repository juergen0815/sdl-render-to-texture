/*
 * app.h
 *
 *  Created on: 2013-01-15
 *      Author: jurgens
 */

#ifndef APP_H_
#define APP_H_

#include "worker.h"
#include "entity.h"
#include "joystick.h"

#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>

#include <SDL/SDL_events.h>

typedef boost::function< bool( const SDL_Event& event ) > HandleEventFunction;
typedef std::list< HandleEventFunction > EventHandlerList;


class App
{
	boost::shared_ptr< Worker > m_Worker;

    EntityList       m_EntityEventHandlerList;
	EventHandlerList m_EventHandlerList;
	SDL_Joystick    *m_Joystick;
public:
	App();

	~App();

	void Init( int argc, char *argv[] );

	int Run();

private:
	void InitScene( int width, int height );

};


#endif /* APP_H_ */
