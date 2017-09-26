#pragma once

#include "../LibS/OpenGL/VertexArrayObject.h"

class MapChunk;

class MapChunkRenderer
{
public:
    MapChunkRenderer();

    void draw(MapChunk& chunk);
    void noLongerRendered(MapChunk& chunk);

    void scheduleUpdate();

private:
    ls::gl::VertexArrayObject m_vao;
    ls::gl::VertexBufferObject* m_vbo;
    ls::gl::IndexBufferObject* m_ibo;
    int m_iboSize;
    bool m_needsUpdate;

    void update(MapChunk& chunk);
};
