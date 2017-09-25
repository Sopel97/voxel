#include "MapRenderer.h"

#include "MapChunk.h"
#include "Map.h"

void MapRenderer::draw(Map& map, const ls::gl::Camera& camera)
{
    for (auto& p : map.chunks())
    {
        auto& chunk = p.second;
        if (shouldDrawChunk(camera, chunk))
        {
            chunk.draw();
        }
        else
        {
            chunk.noLongerRendered();
        }
    }
}

bool MapRenderer::shouldDrawChunk(const ls::gl::Camera& camera, const MapChunk& chunk)
{
    return estimateDistanceToChunk(camera.position(), chunk.pos()) < m_maxDistanceToRenderedChunk;
}
float MapRenderer::estimateDistanceToChunk(const ls::Vec3F& cameraPos, const ls::Vec3I& chunkPos)
{
    const ls::Vec3F chunkCenter = (ls::Vec3F(chunkPos) + ls::Vec3F(0.5f, 0.5f, 0.5f)) * ls::Vec3F(MapChunk::width(), MapChunk::height(), MapChunk::depth());
    return cameraPos.distance(chunkCenter);
}