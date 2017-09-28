#pragma once

#include <map>

#include "MapChunk.h"
#include "MapRenderer.h"
#include "MapGenerator.h"

#include "../LibS/Shapes/Vec3.h"
#include "../LibS/OpenGL/Camera.h"

class Game;

class Map
{
public:
    Map(uint32_t seed);

    std::map<ls::Vec3I, MapChunk>& chunks();
    const std::map<ls::Vec3I, MapChunk>& chunks() const;

    void draw(const ls::gl::Camera& camera, float dt);

    void update(Game& game, float dt);

    ls::Vec3I worldToChunk(const ls::Vec3F& worldPos) const;

    bool isValidChunkPos(const ls::Vec3I& pos) const;

    MapChunk* chunkAt(const ls::Vec3I& pos);

    MapChunkNeighbours chunkNeighbours(const ls::Vec3I& pos);

    uint32_t seed() const;

private:
    MapRenderer m_renderer;
    MapGenerator m_generator;
    uint32_t m_seed;
    float m_timeSinceLastChunkUnloadPass;
    std::map<ls::Vec3I, MapChunk> m_chunks;

    float m_timeSinceLastMissingChunkPosCacheUpdate;
    std::vector<ls::Vec3I> m_missingChunkPosCache;
    int m_missingChunkPosCacheCurrentPosition;
    ls::Vec3I m_missingChunkPosCacheLastOrigin;

    static constexpr int m_maxWorldHeight = 256;
    static_assert(m_maxWorldHeight % MapChunk::height() == 0);

    static constexpr int m_minChunkDistanceToUnload = 20;

    static constexpr float m_timeBetweenChunkUnloadingPasses = 1.0f;
    static constexpr float m_timeBetweenMissingChunkPosCacheUpdates = 1.0f;

    static constexpr int m_chunkLoadingRange = 10;

    static constexpr int m_maxChunksSpawnedPerUpdate = 16;

    void trySpawnNewChunk(const ls::Vec3I& currentChunk);
    void spawnChunk(const ls::Vec3I& pos);
    void unloadFarChunks(const ls::Vec3I& currentChunk);
    int distanceBetweenChunks(const ls::Vec3I& lhs, const ls::Vec3I& rhs);
    std::map<ls::Vec3I, MapChunk>::iterator unloadChunk(const std::map<ls::Vec3I, MapChunk>::iterator& iter);

    void updateMissingChunkPosCache(const ls::Vec3I& pos);
};
