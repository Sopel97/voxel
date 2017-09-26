#include "MapRenderer.h"

#include "MapChunk.h"
#include "Map.h"

#include "../LibS/OpenGL/Camera.h"
#include "../LibS/Shapes/Vec3.h"
#include "../LibS/Shapes/Plane3.h"
#include "../LibS/Shapes/Frustum3.h"
#include "../LibS/Shapes/Sphere3.h"

void MapRenderer::draw(Map& map, const ls::gl::Camera& camera)
{
    ls::Frustum3F frustum = ls::Frustum3F::fromMatrix(camera.projectionMatrix() * camera.viewMatrix());
    for (auto& p : frustum.planes)
    {
        p.normalize();
    }

    for (auto& p : map.chunks())
    {
        auto& chunk = p.second;
        if (shouldForgetChunk(camera, chunk))
        {
            chunk.noLongerRendered();
        }
        else if(shouldDrawChunk(camera, frustum, chunk))
        {
            chunk.draw();
        }
    }
}

bool MapRenderer::shouldDrawChunk(const ls::gl::Camera& camera, const ls::Frustum3F& frustum, const MapChunk& chunk)
{
    return intersect(frustum, chunk.boundingSphere());
}
bool MapRenderer::shouldForgetChunk(const ls::gl::Camera& camera, const MapChunk& chunk)
{
    return estimateDistanceToChunk(camera.position(), chunk) > m_maxDistanceToRenderedChunk;
}
float MapRenderer::estimateDistanceToChunk(const ls::Vec3F& cameraPos, const MapChunk& chunkPos)
{
    return cameraPos.distance(chunkPos.boundingSphere().origin);
}
float MapRenderer::distance(const ls::Plane3F& plane, const ls::Vec3F& point)
{
    return plane.a*point.x + plane.b*point.y + plane.c*point.z + plane.d;
}
bool MapRenderer::intersect(const ls::Frustum3F& frustum, const ls::Sphere3F& sphere)
{
    for (const auto& plane : frustum.planes)
    {
        if (distance(plane, sphere.origin) < -sphere.radius) // signedness due to orientation of the planes
            return false;
    }
    return true;
}