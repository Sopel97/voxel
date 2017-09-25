#pragma once

#include <map>

#include "MapChunk.h"
#include "MapRenderer.h"

#include "../LibS/Shapes/Vec3.h"
#include "../LibS/OpenGL/Camera.h"

class Map
{
public:
    Map() = default;

    std::map<ls::Vec3I, MapChunk>& chunks();
    const std::map<ls::Vec3I, MapChunk>& chunks() const;

    void draw(const ls::gl::Camera& camera);

private:
    MapRenderer m_renderer;
    std::map<ls::Vec3I, MapChunk> m_chunks;

    static constexpr int m_maxWorldHeight = 256;
    static_assert(m_maxWorldHeight % MapChunk::height() == 0);
};
