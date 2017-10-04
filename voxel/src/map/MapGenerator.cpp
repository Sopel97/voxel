#include "map/MapGenerator.h"

#include "map/MapChunk.h"

#include "ResourceManager.h"
#include "block/BlockFactory.h"
#include "block/Block.h"
#include "block/BlockContainer.h"

#include "../LibS/Noise/NoiseSampler.h"
#include "../LibS/Noise/SimplexNoise.h"
#include "../LibS/Array3.h"
#include "../LibS/Shapes/Vec3.h"

#include "map\Map.h"

#include <cstdlib>
#include <utility>

MapGenerator::MapGenerator(Map& map) :
    m_map(&map)
{

}

MapGenerator::CaveMapType MapGenerator::generateCaveMap(const ls::Vec3I& chunkPos, uint32_t seed) const
{
    static constexpr int simulationRadiusInChunks = 1;
    static constexpr float stepSize = 2.0f;
    static constexpr float wormSize = (MapChunk::width() + MapChunk::height() + MapChunk::width()) * (0.5f * 1.5f); // average times 1.5
    static constexpr int maxWormsPerChunk = 3;
    static constexpr int minWormsPerChunk = 0;
    static constexpr std::array<float, 3> wormRadii{ 1.8f, 2.5f, 3.5f };
    static constexpr int maxWormStartHeight = 100;

    Hasher hasher(seed);

    static const std::vector<ls::Array3<bool>> carvingTemplates = []() -> std::vector<ls::Array3<bool>> {

        std::vector<ls::Array3<bool>> templates;
        templates.reserve(wormRadii.size());
        for (const auto wormRadius : wormRadii)
        {
            const int dim = static_cast<int>(std::round(wormRadius * 2 + 1));
            ls::Array3<bool> result(dim, dim, dim, false);
            for (int x = 0; x < dim; ++x)
            {
                for (int y = 0; y < dim; ++y)
                {
                    for (int z = 0; z < dim; ++z)
                    {
                        const float dx = static_cast<float>(x) - wormRadius;
                        const float dy = static_cast<float>(y) - wormRadius;
                        const float dz = static_cast<float>(z) - wormRadius;
                        if (dx*dx + dy*dy + dz*dz <= wormRadius * wormRadius)
                        {
                            result(x, y, z) = true;
                        }
                    }
                }
            }
            templates.emplace_back(std::move(result));
        }

        return templates;
    }();

    auto applyTemplate = [](CaveMapType& map, const ls::Array3<bool>& t, const ls::Vec3I& center)
    {
        const int w = static_cast<int>(t.width());
        const int h = static_cast<int>(t.height());
        const int d = static_cast<int>(t.depth());
        const ls::Vec3I pos{ center.x - w / 2, center.y - h / 2, center.z - d / 2 };
        const int minX = std::max(0, pos.x);
        const int minY = std::max(0, pos.y);
        const int minZ = std::max(0, pos.z);
        const int maxX = std::min(static_cast<int>(MapChunk::width()), pos.x + w) - 1;
        const int maxY = std::min(static_cast<int>(MapChunk::height()), pos.y + h) - 1;
        const int maxZ = std::min(static_cast<int>(MapChunk::depth()), pos.z + d) - 1;
        for (int x = minX; x <= maxX; ++x)
        {
            for (int y = minY; y <= maxY; ++y)
            {
                for (int z = minZ; z <= maxZ; ++z)
                {
                    auto& current = map(x, y, z);
                    current = t(x - pos.x, y - pos.y, z - pos.z) || current;
                }
            }
        }
    };

    ls::SimplexNoise<float, Hasher> noiseGen(Hasher{ seed });
    ls::NoiseSampler2F sampler;
    sampler.setLowerBound(-1.0f);
    sampler.setUpperBound(1.0f);
    sampler.setOctaves(3);
    sampler.setScale({ 0.01f, 0.01f });

    auto simulateWorm = [&sampler, &noiseGen, &applyTemplate](CaveMapType& map, const ls::Array3<bool>& carvingTemplate, ls::Vec3F pos, uint32_t chunkHash)
    {
        constexpr float xSampleOffset = 0.0f;
        constexpr float ySampleOffset = wormSize * 16.0f;
        constexpr float zSampleOffset = wormSize * 32.0f;

        const float x = static_cast<float>(chunkHash & 0xFFFF);

        float length = 0.0f;
        while (length < wormSize)
        {
            const float dx = sampler.sample({ x , length + xSampleOffset }, noiseGen);
            const float dy = sampler.sample({ x , length + ySampleOffset }, noiseGen);
            const float dz = sampler.sample({ x , length + zSampleOffset }, noiseGen);

            const ls::Vec3F step = ls::Vec3F(dx, dy * 0.7f, dz).normalized() * stepSize; // 0.5 factor for y coord to make caves less vertical
            pos += step;
            applyTemplate(map, carvingTemplate, static_cast<ls::Vec3I>(pos));

            length += stepSize;
        }
    };

    CaveMapType result(false);

    for (int cdx = -simulationRadiusInChunks; cdx <= simulationRadiusInChunks; ++cdx)
    {
        for (int cdy = -simulationRadiusInChunks; cdy <= simulationRadiusInChunks; ++cdy)
        {
            for (int cdz = -simulationRadiusInChunks; cdz <= simulationRadiusInChunks; ++cdz)
            {
                const ls::Vec3I chunkPos(chunkPos.x + cdx, chunkPos.y + cdy, chunkPos.z + cdz);
                const uint32_t chunkHash = hasher(chunkPos.x, chunkPos.y, chunkPos.z);
                const int numWorms = chunkHash % (maxWormsPerChunk - minWormsPerChunk + 1) + minWormsPerChunk;
                for (int i = 0; i < numWorms; ++i)
                {
                    const uint32_t xh = hasher(chunkHash, i);
                    const uint32_t yh = hasher(xh, i);
                    if (yh % MapChunk::height() + chunkPos.y * MapChunk::height() > maxWormStartHeight) continue; // to reduce amount of caves on teh surface

                    const uint32_t zh = hasher(yh, i);
                    const uint32_t carvingTemplateId = hasher(zh, i) % carvingTemplates.size();
                    ls::Vec3F wormPos;
                    wormPos.x = static_cast<float>(static_cast<int>(xh % MapChunk::width()) + cdx * MapChunk::width());
                    wormPos.y = static_cast<float>(static_cast<int>(yh % MapChunk::height()) + cdy * MapChunk::height());
                    wormPos.z = static_cast<float>(static_cast<int>(zh % MapChunk::depth()) + cdz * MapChunk::depth());

                    simulateWorm(result, carvingTemplates[carvingTemplateId], wormPos, chunkHash);
                }
            }
        }
    }

    return result;
}

void MapGenerator::generateChunk(MapChunkBlockData& chunk) const
{
    const auto& grassFactory = ResourceManager<BlockFactory>::instance().get("Grass");
    const auto& dirtFactory = ResourceManager<BlockFactory>::instance().get("Dirt");
    const auto& stoneFactory = ResourceManager<BlockFactory>::instance().get("Stone");

    ls::NoiseSampler2D sampler;
    sampler.setLowerBound(0.0);
    sampler.setUpperBound(1.0);
    sampler.setOctaves(4);
    sampler.setScale({ 0.01, 0.01 });

    ls::SimplexNoise<double, Hasher> simplexNoise(Hasher(chunk.seed));

    const ls::Vec3I firstBlockPos = chunk.firstBlockPosition();

    const auto& caveMap = generateCaveMap(chunk.pos, chunk.seed);

    for (size_t x = 0; x < MapChunk::width(); ++x)
    {
        for (size_t z = 0; z < MapChunk::depth(); ++z)
        {
            const double r = sampler.sample({ static_cast<double>(static_cast<int>(x) + firstBlockPos.x), static_cast<double>(static_cast<int>(z) + firstBlockPos.z) }, simplexNoise);

            const int stoneLayerTop = 110 + static_cast<int>(r * 5.0) - firstBlockPos.y;
            const int dirtLayerTop = stoneLayerTop + static_cast<int>(r * 2.0) + 2;
            const int grassLayerTop = dirtLayerTop + 1;

            int y = 0;
            while (y < MapChunk::height() && y <= stoneLayerTop)
            {
                chunk.blocks(x, y, z) = caveMap(x, y, z) ? m_map->instantiateAirBlock() : stoneFactory.get().instantiate();
                 ++y;
            }
            while (y < MapChunk::height() && y <= dirtLayerTop)
            {
                chunk.blocks(x, y, z) = caveMap(x, y, z) ? m_map->instantiateAirBlock() : dirtFactory.get().instantiate();;
                ++y;
            }
            while (y < MapChunk::height() && y <= grassLayerTop)
            {
                chunk.blocks(x, y, z) = caveMap(x, y, z) ? m_map->instantiateAirBlock() : grassFactory.get().instantiate();
                ++y;
            }
            while (y < MapChunk::height())
            {
                chunk.blocks(x, y, z) = m_map->instantiateAirBlock();
                ++y;
            }
        }
    }
}
