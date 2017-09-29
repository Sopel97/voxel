#pragma once

#include "../LibS/OpenGL/VertexArrayObject.h"

#include "block/BlockVertex.h"

class MapChunk;

class MapChunkRenderer
{
public:
    MapChunkRenderer();

    void draw(MapChunk& chunk, float dt, int& numUpdatedChunksOnDraw);
    void tooFarToDraw(MapChunk& chunk, float dt);
    void culled(MapChunk& chunk, float dt, int& numUpdatedChunksOnCull);

    void scheduleUpdate();

private:
    ls::gl::VertexArrayObject m_vao;
    ls::gl::VertexBufferObject* m_vbo;
    ls::gl::IndexBufferObject* m_ibo;
    float m_timeOutsideDrawingRange;
    int m_iboSize;
    bool m_needsUpdate; 
    
    static constexpr float m_maxTimeOutsideDrawingRange = 30.0f;
    static constexpr int m_maxChunksUpdatedOnDrawPerFrame = 2;
    static constexpr int m_maxChunksUpdatedOnCullPerFrame = 1;

    void update(MapChunk& chunk);
};
