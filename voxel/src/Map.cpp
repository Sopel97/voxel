#include "Map.h"

std::map<ls::Vec3I, MapChunk> Map::chunks()
{
    return m_chunks;
}
const std::map<ls::Vec3I, MapChunk> Map::chunks() const
{
    return m_chunks;
}

void Map::draw()
{
    m_renderer.draw(*this);
}