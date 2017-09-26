#pragma once

#include "../LibS/Fwd.h"

class Map;
class MapChunk;

class MapRenderer
{
public:
    void draw(Map& map, const ls::gl::Camera& camera);
private:

    static constexpr float m_maxDistanceToRenderedChunk = 80.0f;

    // expects normalized planes in frustum
    static bool shouldDrawChunk(const ls::gl::Camera& camera, const ls::Frustum3F& frustum, const MapChunk& chunk);
    static bool shouldForgetChunk(const ls::gl::Camera& camera, const MapChunk& chunk);
    static float estimateDistanceToChunk(const ls::Vec3F& cameraPos, const MapChunk& chunkPos);

    // expects normalized plane
    static float distance(const ls::Plane3F& plane, const ls::Vec3F& point);

    // expects normalized planes in frustum
    static bool intersect(const ls::Frustum3F& frustum, const ls::Sphere3F& sphere);
};
