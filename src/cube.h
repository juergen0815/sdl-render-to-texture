/*
 * cube.h
 *
 *  Created on: 2013-01-15
 *      Author: jurgens
 */

#ifndef CUBE_H_
#define CUBE_H_

#include "entity.h"
#include "brush.h"
#include "texture.h"

#include <GL/glew.h>

class Cube : public Entity
{
public:
    enum
    {
        BLEND_COLOR_F = 1<<RenderState::USER_B
    };
private:
	GLuint m_VboID;

	bool   m_IsInitialized;
	bool   m_HasVBO;

	int    m_NormalArrayEnabled;
	int    m_ColorArrayEnabled;
	int    m_VertexArrayEnabled;
	int    m_TexCoordArrayEnabled;
protected:
	BrushPtr    m_Asset;
	TexturePtr  m_Texture;
public:
	Cube();

	Cube( BrushPtr brush );

	virtual ~Cube();
protected:
	virtual bool DoInitialize( Renderer* renderer );

	virtual void DoRender();

	virtual void DoUpdate( float ticks ) {}

};


#endif /* CUBE_H_ */
