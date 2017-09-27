#pragma once

#include "../LibS/OpenGL/VertexArrayObject.h"

#include "block/BlockVertex.h"

class MapChunk;

class MapChunkRenderer
{
public:
    MapChunkRenderer();

    void draw(MapChunk& chunk, float dt);
    void noLongerRendered(MapChunk& chunk, float dt);

    void scheduleUpdate();

private:
    ls::gl::VertexArrayObject m_vao;
    ls::gl::VertexBufferObject* m_vbo;
    ls::gl::IndexBufferObject* m_ibo;
    float m_timeSinceLastRender;
    int m_iboSize;
    bool m_needsUpdate; 
    
    static constexpr float m_freeChunkBufferTimeout = 30.0f;

    void update(MapChunk& chunk);
};
