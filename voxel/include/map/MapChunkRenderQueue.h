#pragma once

#include <vector>

class MapChunk;

class MapChunkRenderQueue
{
public:
    MapChunkRenderQueue(int maxDistance);

    void enqueueDraw(MapChunk& chunk, int distance);

    void enqueueCull(MapChunk& chunk);

    void draw(float dt);

private:
    int m_maxDistance;
    std::vector<std::vector<MapChunk*>> m_drawQueue;
    std::vector<MapChunk*> m_cullQueue;
};
