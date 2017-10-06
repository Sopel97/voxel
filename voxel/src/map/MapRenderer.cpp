#include "map/MapRenderer.h"

#include "map/MapChunk.h"
#include "map/Map.h"
#include "map/MapChunkRenderQueue.h"

#include "../LibS/OpenGL/Camera.h"
#include "../LibS/Shapes/Vec3.h"
#include "../LibS/Shapes/Plane3.h"
#include "../LibS/Shapes/Frustum3.h"
#include "../LibS/Shapes/Sphere3.h"

#include "ResourceManager.h"
#include "sprite/Spritesheet.h"

MapRenderer::MapRenderer()
{
    m_texture = &(ResourceManager<Spritesheet>::instance().get("Spritesheet").get().texture());
    m_shader = &(ResourceManager<ls::gl::ShaderProgram>::instance().get("Terrain").get());
    m_uModelViewProjection = m_shader->uniformView("uModelViewProjection");    
    m_shader->uniformView("tex0").set(0);
}
void MapRenderer::draw(Map& map, const ls::gl::Camera& camera, float dt)
{
    glEnable(GL_DEPTH_TEST);    
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);
    
    m_texture->bind(GL_TEXTURE0);
    m_shader->bind();
    m_uModelViewProjection.set(camera.projectionMatrix() * camera.viewMatrix());
    
    ls::Frustum3F frustum = ls::Frustum3F::fromMatrix(camera.projectionMatrix() * camera.viewMatrix());
    for (auto& p : frustum.planes)
    {
        p.normalize();
    }

    const ls::Vec3I cameraChunk = map.worldToChunk(camera.position());

    MapChunkRenderQueue queue(m_maxDistanceToRenderedChunk);
    for (auto& p : map.chunks())
    {
        auto& chunk = p.second;
        const int dist = Map::distanceBetweenChunks(cameraChunk, chunk.pos());
        if (shouldForgetChunk(chunk, dist))
        {
            chunk.tooFarToDraw(dt);
        }
        else if(shouldDrawChunk(camera, frustum, chunk))
        {
            queue.enqueueDraw(chunk, dist);
        }
        else
        {
            queue.enqueueCull(chunk);
        }
    }

    queue.draw(dt);

    //std::cout << "Rendered chunks: " << numRenderedChunks << '/' << map.chunks().size() << '\n';
}

bool MapRenderer::shouldDrawChunk(const ls::gl::Camera& camera, const ls::Frustum3F& frustum, const MapChunk& chunk)
{
    return intersect(frustum, chunk.boundingSphere());
}
bool MapRenderer::shouldForgetChunk(const MapChunk& chunk, int dist)
{
    return dist > m_maxDistanceToRenderedChunk;
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
