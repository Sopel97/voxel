#include "MapChunkRenderer.h"

#include "block/BlockVertex.h"

MapChunkRenderer::MapChunkRenderer() :
    m_needsUpdate(true)
{
    m_vbo = &m_vao.createVertexBufferObject();
    m_vao.setVertexAttribute(*m_vbo, 0, &BlockVertex::pos, 3, GL_FLOAT, GL_FALSE);
    m_vao.setVertexAttribute(*m_vbo, 1, &BlockVertex::uv, 2, GL_FLOAT, GL_FALSE);

    m_ibo = &m_vao.createIndexBufferObject();
}
void MapChunkRenderer::draw(MapChunk& chunk)
{
    if (m_needsUpdate)
    {
        update();
        m_needsUpdate = false;
    }
}
void MapChunkRenderer::noLongerRendered(MapChunk& chunk)
{
    // TODO: better emptying of the buffers
    m_vbo->reset<void>(nullptr, 1, GL_STATIC_DRAW);
    m_ibo->reset<void>(nullptr, 1, GL_STATIC_DRAW);
}
void MapChunkRenderer::update()
{

}
