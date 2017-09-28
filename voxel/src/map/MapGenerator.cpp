#include "map/MapGenerator.h"

#include "map/MapChunk.h"

#include "ResourceManager.h"
#include "block/BlockFactory.h"
#include "block/Block.h"
#include "block/BlockContainer.h"

#include "../LibS/Noise/NoiseSampler.h"
#include "../LibS/Noise/SimplexNoise.h"

#include "map\Map.h"

#include <cstdlib>
#include <utility>

MapGenerator::MapGenerator(Map& map) :
    m_map(&map)
{

}

void MapGenerator::generateChunk(MapChunk& chunk) const
{
    struct Hasher
    {
        Hasher(uint32_t seed) :
            m_seed(seed)
        {

        }

        uint32_t operator()(uint32_t x) const
        {
            return m_seed ^ (std::hash<uint32_t>()(x) + 0x9e3779b9u + (m_seed << 6) + (m_seed >> 2));
        }

    private:
        uint32_t m_seed;
    };

    const auto& grassFactory = ResourceManager<BlockFactory>::instance().get("Grass");
    const auto& dirtFactory = ResourceManager<BlockFactory>::instance().get("Dirt");
    const auto& stoneFactory = ResourceManager<BlockFactory>::instance().get("Stone");

    ls::NoiseSampler2D sampler;
    sampler.setLowerBound(0.0);
    sampler.setUpperBound(1.0);
    sampler.setOctaves(4);
    sampler.setScale({0.01, 0.01});

    ls::SimplexNoise<double, Hasher> simplexNoise(Hasher(chunk.seed()));

    const ls::Vec3I firstBlockPos = chunk.firstBlockPosition();

    for (int x = 0; x < MapChunk::width(); ++x)
    {
        for (int z = 0; z < MapChunk::depth(); ++z)
        {
            const double r = sampler.sample({ static_cast<double>(x + firstBlockPos.x), static_cast<double>(z + firstBlockPos.z) }, simplexNoise);

            const int stoneLayerTop = 110 + static_cast<int>(r * 5.0) - firstBlockPos.y;
            const int dirtLayerTop = stoneLayerTop + static_cast<int>(r * 2.0) + 2;
            const int grassLayerTop = dirtLayerTop + 1;

            int y = 0;
            while (y < MapChunk::height() && y <= stoneLayerTop)
            {
                chunk.emplaceBlock(stoneFactory.get(), { x, y, z }, false);
                ++y;
            }
            while (y < MapChunk::height() && y <= dirtLayerTop)
            {
                chunk.emplaceBlock(dirtFactory.get(), { x, y, z }, false);
                ++y;
            }
            while (y < MapChunk::height() && y <= grassLayerTop)
            {
                chunk.emplaceBlock(grassFactory.get(), { x, y, z }, false);
                ++y;
            }
            while (y < MapChunk::height())
            {
                chunk.placeBlock(m_map->instantiateAirBlock(), { x, y, z }, false);
                ++y;
            }
        }
    }

    chunk.updateAllAsIfPlaced();
}
