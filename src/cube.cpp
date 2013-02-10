
/*
 * cube.cpp
 *
 *  Created on: 2013-01-15
 *      Author: jurgens
 */

#include "cube.h"
#include "err.h"

// cube ///////////////////////////////////////////////////////////////////////
//    v6----- v5
//   /|      /|
//  v1------v0|
//  | |     | |
//  | |v7---|-|v4
//  |/      |/
//  v2------v3

// normal array
GLfloat normals[]   = { 0, 0, 1,   0, 0, 1,   0, 0, 1,      // v0-v1-v2 (front)
                        0, 0, 1,   0, 0, 1,   0, 0, 1,      // v2-v3-v0

                        1, 0, 0,   1, 0, 0,   1, 0, 0,      // v0-v3-v4 (right)
                        1, 0, 0,   1, 0, 0,   1, 0, 0,      // v4-v5-v0

                        0, 0,-1,   0, 0,-1,   0, 0,-1,      // v4-v7-v6 (back)
                        0, 0,-1,   0, 0,-1,   0, 0,-1,      // v6-v5-v4

                       -1, 0, 0,  -1, 0, 0,  -1, 0, 0,      // v1-v6-v7 (left)
                       -1, 0, 0,  -1, 0, 0,  -1, 0, 0,      // v7-v2-v1

                        0, 1, 0,   0, 1, 0,   0, 1, 0,      // v0-v5-v6 (top)
                        0, 1, 0,   0, 1, 0,   0, 1, 0,      // v6-v1-v0

                        0,-1, 0,   0,-1, 0,   0,-1, 0,      // v7-v4-v3 (bottom)
                        0,-1, 0,   0,-1, 0,   0,-1, 0,      // v3-v2-v7
};

// vertex coords array for glDrawArrays() =====================================
// A cube has 6 sides and each side has 2 triangles, therefore, a cube consists
// of 36 vertices (6 sides * 2 tris * 3 vertices = 36 vertices). And, each
// vertex is 3 components (x,y,z) of floats, therefore, the size of vertex
// array is 108 floats (36 * 3 = 108).
GLfloat vertices[]  = { 1, 1, 1,  -1, 1, 1,  -1,-1, 1,      // v0-v1-v2 (front)
                       -1,-1, 1,   1,-1, 1,   1, 1, 1,      // v2-v3-v0

                        1, 1, 1,   1,-1, 1,   1,-1,-1,      // v0-v3-v4 (right)
                        1,-1,-1,   1, 1,-1,   1, 1, 1,      // v4-v5-v0

                        1,-1,-1,  -1,-1,-1,  -1, 1,-1,      // v4-v7-v6 (back)
                       -1, 1,-1,   1, 1,-1,   1,-1,-1,      // v6-v5-v4

                       -1, 1, 1,  -1, 1,-1,  -1,-1,-1,      // v1-v6-v7 (left)
                       -1,-1,-1,  -1,-1, 1,  -1, 1, 1,      // v7-v2-v1

                        1, 1, 1,   1, 1,-1,  -1, 1,-1,      // v0-v5-v6 (top)
                       -1, 1,-1,  -1, 1, 1,   1, 1, 1,      // v6-v1-v0

                       -1,-1,-1,   1,-1,-1,   1,-1, 1,      // v7-v4-v3 (bottom)
                        1,-1, 1,  -1,-1, 1,  -1,-1,-1,      // v3-v2-v7
};
// color array
GLfloat colors[]    = { 1, 1, 1, 1,   1, 1, 0, 1,   1, 0, 0, 1,      // v0-v1-v2 (front)
                        1, 0, 0, 1,   1, 0, 1, 1,   1, 1, 1, 1,      // v2-v3-v0

                        1, 1, 1, 1,   1, 0, 1, 1,   0, 0, 1, 1,      // v0-v3-v4 (right)
                        0, 0, 1, 1,   0, 1, 1, 1,   1, 1, 1, 1,      // v4-v5-v0

                        0, 0, 1, 1,   0, 0, 0, 1,   0, 1, 0, 1,      // v4-v7-v6 (back)
                        0, 1, 0, 1,   0, 1, 1, 1,   0, 0, 1, 1,      // v6-v5-v4

                        1, 1, 0, 1,   0, 1, 0, 1,   0, 0, 0, 1,      // v1-v6-v7 (left)
                        0, 0, 0, 1,   1, 0, 0, 1,   1, 1, 0, 1,      // v7-v2-v1

                        1, 1, 1, 1,   0, 1, 1, 1,   0, 1, 0, 1,      // v0-v5-v6 (top)
                        0, 1, 0, 1,   1, 1, 0, 1,   1, 1, 1, 1,      // v6-v1-v0

                        0, 0, 0, 1,   0, 0, 1, 1,   1, 0, 1, 1,      // v7-v4-v3 (bottom)
                        1, 0, 1, 1,   1, 0, 0, 1,   0, 0, 0, 1,      // v3-v2-v7
};
GLfloat texCoords[] = { 1,1,  0,1,  0,0,      // v0-v1-v2 (front)
                        0,0,  1,0,  1,1,      // v2-v3-v0

                        0,1,  0,0,  1,0,      // v0-v3-v4 (right)
                        1,0,  1,1,  0,1,      // v4-v5-v0

                        0,0,  1,0,  1,1,      // v4-v7-v6 (back)
                        1,1,  0,1,  0,0,      // v6-v5-v4

                        1,1,  0,1,  0,0,      // v1-v6-v7 (left)
                        0,0,  1,0,  1,1,      // v7-v2-v1

                        1,0,  1,1,  0,1,      // v0-v5-v6 (top)
                        0,1,  0,0,  1,0,      // v6-v1-v0

                        1,1,  0,1,  0,0,      // v7-v4-v3 (bottom)
                        0,0,  1,0,  1,1,      // v3-v2-v7
};

Cube::Cube()
    : m_VboID(0)
    , m_IsInitialized(false)
    , m_HasVBO(false)

    , m_NormalArrayEnabled(0)
    , m_ColorArrayEnabled(0)
    , m_VertexArrayEnabled(0)
    , m_TexCoordArrayEnabled(0)
{
    GetRenderState()->SetFlag( BLEND_COLOR_F );
}

Cube::Cube( BrushPtr brush )
    : m_VboID(0)
    , m_IsInitialized(false)
    , m_HasVBO(false)

    , m_NormalArrayEnabled(0)
    , m_ColorArrayEnabled(0)
    , m_VertexArrayEnabled(0)
    , m_TexCoordArrayEnabled(0)

    , m_Asset( brush )
{
    GetRenderState()->SetFlag( BLEND_COLOR_F );
}

Cube::~Cube()
{
    if ( m_VboID > 0 ) {
        glDeleteBuffers(1, &m_VboID);
    }
}

bool Cube::DoInitialize( Renderer* renderer )
{
    if ( m_Asset ) {
        m_Texture = TexturePtr( new Texture );
        m_Texture->Load( *m_Asset.get() );
        GetRenderState()->ClearFlag( BLEND_COLOR_F );
    }

    m_HasVBO  = glewGetExtension("GL_ARB_vertex_buffer_object");
    ASSERT( m_HasVBO, "VBOs not supported!" );
    if ( m_HasVBO ) {
        glGenBuffers(1, &m_VboID);
        glBindBuffer(GL_ARRAY_BUFFER, m_VboID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices)+sizeof(normals)+sizeof(colors)+sizeof(texCoords), 0, GL_STATIC_DRAW_ARB);
        std::size_t offset(0);
        glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(vertices), vertices);
        offset += sizeof(vertices);
        glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(normals), normals);
        offset += sizeof(normals);
        glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(colors), colors);
        if ( m_Texture ) {
            offset += sizeof(texCoords);
            glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(texCoords), texCoords);
        }
    }
    m_IsInitialized = true;
    return m_IsInitialized;
}

void Cube::DoRender( )
{
    // bind VBOs with IDs and set the buffer offsets of the bound VBOs
    // When buffer object is bound with its ID, all pointers in gl*Pointer()
    // are treated as offset instead of real pointer.

    // enable vertex arrays
    glGetIntegerv( GL_NORMAL_ARRAY, &m_NormalArrayEnabled );
    if ( !m_NormalArrayEnabled )  {
        glEnableClientState(GL_NORMAL_ARRAY);
    }
    glGetIntegerv( GL_COLOR_ARRAY, &m_ColorArrayEnabled );
    if ( !m_ColorArrayEnabled && (GetRenderState()->GetFlags() & BLEND_COLOR_F) ) {
        glEnableClientState(GL_COLOR_ARRAY);
    }
    glGetIntegerv( GL_VERTEX_ARRAY, &m_VertexArrayEnabled );
    if (!m_VertexArrayEnabled) {
        glEnableClientState(GL_VERTEX_ARRAY);
    }
    if ( m_Texture && !m_TexCoordArrayEnabled ) {
        glGetIntegerv( GL_TEXTURE_COORD_ARRAY, &m_TexCoordArrayEnabled );
        if (!m_TexCoordArrayEnabled) {
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        }
    }

    int blend_enabled;
    glGetIntegerv(GL_BLEND, &blend_enabled);

    // Render with VBO - if available
    if ( m_HasVBO ) {
        glBindBuffer(GL_ARRAY_BUFFER, m_VboID);
        // before draw, specify vertex and index arrays with their offsets
        std::size_t offset(0);
        glVertexPointer(3, GL_FLOAT, 0, (void*)offset); offset += sizeof(vertices);
        glNormalPointer(   GL_FLOAT, 0, (void*)offset); offset += sizeof(normals);
        glColorPointer (4, GL_FLOAT, 0, (void*)offset);
        if ( m_Texture) {
            m_Texture->Enable();
            offset += sizeof(texCoords);
            glTexCoordPointer(2, GL_FLOAT, 0, (void*)offset );
            if (blend_enabled) {
                glDisable( GL_BLEND );
            }
        }
    } else {
        // before draw, specify vertex arrays
        glVertexPointer(3, GL_FLOAT, 0, vertices);
        glNormalPointer(  GL_FLOAT, 0, normals);
        glColorPointer(4, GL_FLOAT, 0, colors);
        if ( m_Texture) {
            m_Texture->Enable();
            glTexCoordPointer(2, GL_FLOAT, 0, texCoords );
        }
    }
    glDrawArrays(GL_TRIANGLES, 0, 36);

    if ( m_HasVBO ){
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    if (!m_VertexArrayEnabled)  {
        glDisableClientState(GL_VERTEX_ARRAY);  // disable vertex arrays
    }
    if (!m_ColorArrayEnabled)   {
        glDisableClientState(GL_COLOR_ARRAY);
    }
    if (!m_NormalArrayEnabled) {
        glDisableClientState(GL_NORMAL_ARRAY);
    }
    if ( m_Texture ) {
        m_Texture->Disable();
        if (!m_TexCoordArrayEnabled) {
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        }
        if (blend_enabled) {
            glEnable( GL_BLEND );
        }
    }

}
