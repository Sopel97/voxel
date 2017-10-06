#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include "../LibS/Fwd.h"
#include "../LibS/OpenGL/Shader.h"

#include <vector>

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

    //static constexpr int m_maxDistanceToRenderedChunk = 20;
    static constexpr int m_maxDistanceToRenderedChunk = 12;

    // expects normalized planes in frustum
    static bool shouldDrawChunk(const ls::gl::Camera& camera, const ls::Frustum3F& frustum, const MapChunk& chunk);
    static bool shouldForgetChunk(const MapChunk& chunk, int dist);
    // expects normalized plane
    static float distance(const ls::Plane3F& plane, const ls::Vec3F& point);

    // expects normalized planes in frustum
    static bool intersect(const ls::Frustum3F& frustum, const ls::Sphere3F& sphere);
};
