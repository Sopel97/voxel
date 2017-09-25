#pragma once

#include "../LibS/OpenGL/Camera.h"

class Map;
class MapChunk;

class MapRenderer
{
public:
    void draw(Map& map, const ls::gl::Camera& camera);
private:

    static constexpr float m_maxDistanceToRenderedChunk = 80.0f;

    static bool shouldDrawChunk(const ls::gl::Camera& camera, const MapChunk& chunk);
    static float estimateDistanceToChunk(const ls::Vec3F& cameraPos, const ls::Vec3I& chunkPos);
};
