#include "map/MapChunkRenderer.h"

#include "block/BlockVertex.h"
#include "block/Block.h"
#include "block/BlockContainer.h"

#include "map/MapChunk.h"

MapChunkRenderer::MapChunkRenderer() :
    m_timeOutsideDrawingRange(0.0f),
    m_iboSize(0),
    m_needsUpdate(true)
{
    m_vbo = &m_vao.createVertexBufferObject();
    m_vao.setVertexAttribute(*m_vbo, 0, &BlockVertex::pos, 3, GL_FLOAT, GL_FALSE);
    m_vao.setVertexAttribute(*m_vbo, 1, &BlockVertex::uv, 2, GL_FLOAT, GL_FALSE);

    m_ibo = &m_vao.createIndexBufferObject();
}
void MapChunkRenderer::draw(MapChunk& chunk, float dt, int& numUpdatedChunksOnDraw)
{
    if (m_needsUpdate && numUpdatedChunksOnDraw < m_maxChunksUpdatedOnDrawPerFrame)
    {
        update(chunk);
        m_needsUpdate = false;

        ++numUpdatedChunksOnDraw;
    }

    if (m_iboSize > 0)
    {
        m_vao.drawElements(GL_TRIANGLES, m_iboSize, GL_UNSIGNED_INT);
    }

    m_timeOutsideDrawingRange = 0.0f;
}

void MapChunkRenderer::scheduleUpdate()
{
    m_needsUpdate = true;
}
void MapChunkRenderer::tooFarToDraw(MapChunk& chunk, float dt)
{
    m_timeOutsideDrawingRange += dt;
    if (m_iboSize > 0 && m_timeOutsideDrawingRange > m_maxTimeOutsideDrawingRange)
    {
        // TODO: better emptying of the buffers
        m_vbo->reset<char>(nullptr, 1, GL_STATIC_DRAW);
        m_ibo->reset<char>(nullptr, 1, GL_STATIC_DRAW);

        m_iboSize = 0;
        m_needsUpdate = true;
    }
}
void MapChunkRenderer::culled(MapChunk& chunk, float dt, int& numUpdatedChunksOnCull)
{
    if (m_needsUpdate && numUpdatedChunksOnCull < m_maxChunksUpdatedOnCullPerFrame)
    {
        update(chunk);
        m_needsUpdate = false;

        ++numUpdatedChunksOnCull;
    }

    m_timeOutsideDrawingRange = 0.0f;
}
void MapChunkRenderer::update(MapChunk& chunk)
{
    std::vector<BlockVertex> vertices;
    std::vector<unsigned> indices;
    
    const ls::Vec3I firstBlockPos = chunk.firstBlockPosition();
    const auto& blocks = chunk.blocks();
    const auto& opacity = chunk.outsideOpacityCache();
    for (int x = 0; x < MapChunk::width(); ++x)
    {
        for (int y = 0; y < MapChunk::height(); ++y)
        {
            for (int z = 0; z < MapChunk::depth(); ++z)
            {
                const auto& blockCont = blocks(x, y, z);

                const ls::Vec3I pos = firstBlockPos + ls::Vec3I(x, y, z);
                blockCont.block().draw(vertices, indices, pos, opacity(x, y, z));
            }
        }
    }

    m_iboSize = indices.size();
    if (m_iboSize > 0)
    {
        m_vbo->reset(vertices.data(), vertices.size(), GL_DYNAMIC_DRAW);
        m_ibo->reset(indices.data(), indices.size(), GL_DYNAMIC_DRAW);
    }
}
