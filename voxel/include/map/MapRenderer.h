#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include "../LibS/Fwd.h"
#include "../LibS/OpenGL/Shader.h"

class Map;
class MapChunk;

class MapRenderer
{
public:
    MapRenderer();

    void draw(Map& map, const ls::gl::Camera& camera, float dt);
private:
    const ls::gl::Texture2* m_texture;
    const ls::gl::ShaderProgram* m_shader;
    ls::gl::ProgramUniformView m_uModelViewProjection;

    static constexpr float m_maxDistanceToRenderedChunk = 16.0f * 16.0f;

    // expects normalized planes in frustum
    static bool shouldDrawChunk(const ls::gl::Camera& camera, const ls::Frustum3F& frustum, const MapChunk& chunk);
    static bool shouldForgetChunk(const ls::gl::Camera& camera, const MapChunk& chunk);
    static float estimateDistanceToChunk(const ls::Vec3F& cameraPos, const MapChunk& chunkPos);

    // expects normalized plane
    static float distance(const ls::Plane3F& plane, const ls::Vec3F& point);

    // expects normalized planes in frustum
    static bool intersect(const ls::Frustum3F& frustum, const ls::Sphere3F& sphere);
};
