#include "map/MapGenerator.h"

#include "map/MapChunk.h"

#include "ResourceManager.h"
#include "block/BlockFactory.h"
#include "block/Block.h"
#include "block/BlockContainer.h"

#include <cstdlib>

MapGenerator::MapGenerator(Map& map) :
    m_map(&map)
{

}

void MapGenerator::generateChunk(MapChunk& chunk) const
{
    const auto& grassFactory = ResourceManager<BlockFactory>::instance().get("Grass");

    for (int x = 0; x < MapChunk::width(); ++x)
    {
        for (int y = 0; y < MapChunk::height(); ++y)
        {
            for (int z = 0; z < MapChunk::depth(); ++z)
            {
                float r = rand() / static_cast<float>(RAND_MAX);

                if (r < 0.1)
                {
                    chunk.emplaceBlock(grassFactory.get(), { x, y, z }, false);
                }
            }
        }
    }

    chunk.emplaceBlock(grassFactory.get(), { 0, 0, 0 }, false);

    chunk.updateAllAsIfPlaced();
}
