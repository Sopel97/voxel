#pragma once

#include <initializer_list>
#include <utility>
#include <cstdint>

#include "../LibS/Fwd.h"

#include "../LibS/Array3.h"

#include "MapChunk.h"

class Map;
class MapChunkBlockData;

class MapGenerator
{
public:
    MapGenerator(Map& map);

    void generateChunk(MapChunkBlockData& chunk) const;

private:
    using CaveMapType = ls::Array3<bool, MapChunk::width(), MapChunk::height(), MapChunk::depth()>;

    struct Hasher
    {
        Hasher(uint32_t seed) :
            m_seed(seed)
        {

        }

        uint32_t operator()(uint32_t x) const
        {
            return combine(m_seed, x);
        }

        uint32_t operator()(uint32_t x, uint32_t y) const
        {
            return combine({ m_seed, x, y });
        }

        uint32_t operator()(uint32_t x, uint32_t y, uint32_t z) const
        {
            return combine({ m_seed, x, y, z });
        }

        uint32_t operator()(uint32_t x, uint32_t y, uint32_t z, uint32_t w) const
        {
            return combine({ m_seed, x, y, z, w });
        }

    private:
        static uint32_t combine(std::initializer_list<uint32_t> list)
        {
            if (list.size() == 0) return 0;
            auto iter = list.begin();
            auto end = list.end();
            uint32_t hash = *iter;
            ++iter;

            while (iter != end)
            {
                hash = combine(hash, *iter);
                ++iter;
            }

            return hash;
        }

        static uint32_t combine(uint32_t seed, uint32_t x)
        {
            return seed ^ (std::hash<uint32_t>()(x) + 0x9e3779b9u + (seed << 6) + (seed >> 2));
        }

        uint32_t m_seed;
    };

    Map* m_map;

    CaveMapType generateCaveMap(const ls::Vec3I& chunkPos, uint32_t seed) const;
};
