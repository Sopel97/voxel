#pragma once

#include "../LibS/OpenGL/VertexArrayObject.h"

class MapChunk;

class MapChunkRenderer
{
public:
    MapChunkRenderer();

    void draw(MapChunk& chunk);
    void noLongerRendered(MapChunk& chunk);

private:
    ls::gl::VertexArrayObject m_vao;
    ls::gl::VertexBufferObject* m_vbo;
    ls::gl::IndexBufferObject* m_ibo;
    bool m_needsUpdate;

    void update();
};
