/*
 * render.h
 *
 *  Created on: 2013-01-15
 *      Author: jurgens
 */

#ifndef RENDER_H_
#define RENDER_H_

#include "worker.h"
#include "entity.h"

#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/unordered_map.hpp>

typedef boost::function<bool(long)> UpdateFunction;

class Renderer : public Worker
{
private:
	bool        m_Terminate;

    EntityList  m_InitList;
	EntityList  m_RenderList;
#ifdef _WIN32
	HGLRC       m_CurrentContext;
	HDC         m_CurrentDC;
#endif
	float       m_TimeBase;
	int         m_Pause;

	boost::unordered_map< long, UpdateFunction > m_Updaters;
public:
	Renderer();

	virtual ~Renderer();

	void Init();

	void AddEntity( EntityPtr entity, int priority = 0 );

	void RemoveEntity( EntityPtr entity );

	EntityPtr FindEntity( const std::string& name );

	long RegisterUpdateFunction( const UpdateFunction& func );

    void UnRegisterUpdateFunction( long key );

    virtual bool HandleEvent( const SDL_Event& event );

private:
	void InitGL();

	// No direct access
	virtual void Terminate();

	virtual void Run();

private:
	// Need this in scope of class to have access to Entity::GetPriority() method
	bool CompareEntityPriorities( const EntityPtr& a, const EntityPtr& b );

};

#endif /* RENDER_H_ */
