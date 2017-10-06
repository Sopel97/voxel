#include "map/MapChunkRenderQueue.h"

#include "map/MapChunk.h"

MapChunkRenderQueue::MapChunkRenderQueue(int maxDistance) :
    m_maxDistance(maxDistance),
    m_drawQueue(maxDistance + 1)
{

}

void MapChunkRenderQueue::enqueueDraw(MapChunk& chunk, int distance)
{
    if (distance > m_maxDistance) distance = m_maxDistance;
    m_drawQueue[distance].emplace_back(&chunk);
}

void MapChunkRenderQueue::enqueueCull(MapChunk& chunk)
{
    m_cullQueue.emplace_back(&chunk);
}

void MapChunkRenderQueue::draw(float dt)
{
    int drawCounter = 0;
    for (auto& queue : m_drawQueue)
    {
        for (MapChunk* chunk : queue)
        {
            chunk->draw(dt, drawCounter);
        }
    }

    int cullCounter = 0;
    for (MapChunk* chunk : m_cullQueue)
    {
        chunk->culled(dt, cullCounter);
    }
}
