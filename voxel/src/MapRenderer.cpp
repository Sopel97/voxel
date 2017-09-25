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
    static constexpr ls::Vec3F chunkSizeF(static_cast<float>(MapChunk::width()), static_cast<float>(MapChunk::height()), static_cast<float>(MapChunk::depth()));

    const ls::Vec3F chunkCenter = (ls::Vec3F(chunkPos) + ls::Vec3F(0.5f, 0.5f, 0.5f)) * chunkSizeF;
    return cameraPos.distance(chunkCenter);
}