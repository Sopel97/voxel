#include "MapGenerator.h"

#include "MapChunk.h"

#include "ResourceManager.h"

#include <cstdlib>

MapGenerator::MapGenerator(Map& map) :
    m_map(&map)
{

}

void MapGenerator::generateChunk(MapChunk& chunk) const
{
    for (int x = 0; x < MapChunk::width(); ++x)
    {
        for (int y = 0; y < MapChunk::height(); ++y)
        {
            for (int z = 0; z < MapChunk::depth(); ++z)
            {
                float r = rand() / static_cast<float>(RAND_MAX);
            }
        }
    }
}
