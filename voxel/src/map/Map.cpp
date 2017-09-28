#include "map/Map.h"

#include "Game.h"

#include "CubeSide.h"

#include <cstdlib>

Map::Map(uint32_t seed) :
    m_renderer{},
    m_generator(*this),
    m_seed(seed), 
    m_timeSinceLastChunkUnloadPass(0.0f),
    m_timeSinceLastMissingChunkPosCacheUpdate(0.0f),
    m_missingChunkPosCacheCurrentPosition(0)
{
}
std::map<ls::Vec3I, MapChunk>& Map::chunks()
{
    return m_chunks;
}
const std::map<ls::Vec3I, MapChunk>& Map::chunks() const
{
    return m_chunks;
}

void Map::draw(const ls::gl::Camera& camera, float dt)
{
    m_renderer.draw(*this, camera, dt);
}

void Map::update(Game& game, float dt)
{
    const auto& cameraPos = game.camera().position();

    const auto currentChunk = worldToChunk(cameraPos);

    trySpawnNewChunk(currentChunk);

    m_timeSinceLastChunkUnloadPass += dt;
    if (m_timeSinceLastChunkUnloadPass >= m_timeBetweenChunkUnloadingPasses)
    {
        unloadFarChunks(currentChunk);
        m_timeSinceLastChunkUnloadPass = 0.0f;
    }

    m_timeSinceLastMissingChunkPosCacheUpdate += dt;
    if (m_timeSinceLastMissingChunkPosCacheUpdate >= m_timeBetweenMissingChunkPosCacheUpdates)
    {
        updateMissingChunkPosCache(currentChunk);
        m_timeSinceLastMissingChunkPosCacheUpdate = 0.0f;
    }
}

ls::Vec3I Map::worldToChunk(const ls::Vec3F& worldPos) const
{
    static constexpr ls::Vec3F chunkSizeF(static_cast<float>(MapChunk::width()), static_cast<float>(MapChunk::height()), static_cast<float>(MapChunk::depth()));

    return ls::floorToInt(worldPos / chunkSizeF);
}

bool Map::isValidChunkPos(const ls::Vec3I& pos) const
{
    if (pos.y < 0) return false;
    if (pos.y >= m_maxWorldHeight / MapChunk::height()) return false;

    return true;
}

void Map::trySpawnNewChunk(const ls::Vec3I& currentChunk)
{
    const int numMissingChunks = m_missingChunkPosCache.size();
    for (int i = 0; i < m_maxChunksSpawnedPerUpdate && m_missingChunkPosCacheCurrentPosition < numMissingChunks; ++i)
    {
        spawnChunk(m_missingChunkPosCache[m_missingChunkPosCacheCurrentPosition]);
        ++m_missingChunkPosCacheCurrentPosition;
    }
}

uint32_t Map::seed() const
{
    return m_seed;
}

MapChunk* Map::chunkAt(const ls::Vec3I& pos)
{
    if (!isValidChunkPos(pos)) return nullptr;

    auto iter = m_chunks.find(pos);
    if (iter != m_chunks.end())
    {
        return &(iter->second);
    }
    
    return nullptr;
}
MapChunkNeighbours Map::chunkNeighbours(const ls::Vec3I& pos)
{
    MapChunk* east = chunkAt(pos + CubeSide::makeEast().direction());
    MapChunk* west = chunkAt(pos + CubeSide::makeWest().direction());
    MapChunk* top = chunkAt(pos + CubeSide::makeTop().direction());
    MapChunk* bottom = chunkAt(pos + CubeSide::makeBottom().direction());
    MapChunk* south = chunkAt(pos + CubeSide::makeSouth().direction());
    MapChunk* north = chunkAt(pos + CubeSide::makeNorth().direction());

    return MapChunkNeighbours{ east, west, top, bottom, south, north };
}
void Map::spawnChunk(const ls::Vec3I& pos)
{
    const auto neighbours = chunkNeighbours(pos);
    auto p = m_chunks.emplace(std::make_pair(pos, MapChunk(*this, m_generator, pos, neighbours)));
    auto& placedChunk = p.first->second;

    if (neighbours.east) neighbours.east->onAdjacentChunkPlaced(placedChunk, pos);
    if (neighbours.west) neighbours.west->onAdjacentChunkPlaced(placedChunk, pos);
    if (neighbours.top) neighbours.top->onAdjacentChunkPlaced(placedChunk, pos);
    if (neighbours.bottom) neighbours.bottom->onAdjacentChunkPlaced(placedChunk, pos);
    if (neighbours.south) neighbours.south->onAdjacentChunkPlaced(placedChunk, pos);
    if (neighbours.north) neighbours.north->onAdjacentChunkPlaced(placedChunk, pos);
}
void Map::unloadFarChunks(const ls::Vec3I& currentChunk)
{
    for (auto iter = m_chunks.begin(); iter != m_chunks.end();)
    {
        auto& pos = iter->first;
        if (distanceBetweenChunks(currentChunk, pos) >= m_minChunkDistanceToUnload)
        {
            iter = unloadChunk(iter);
        }
        else
        {
            ++iter;
        }
    }
}
std::map<ls::Vec3I, MapChunk>::iterator Map::unloadChunk(const std::map<ls::Vec3I, MapChunk>::iterator& iter)
{
    return m_chunks.erase(iter);
}
int Map::distanceBetweenChunks(const ls::Vec3I& lhs, const ls::Vec3I& rhs)
{
    return std::max({ std::abs(lhs.x - rhs.x), std::abs(lhs.y - rhs.y), std::abs(lhs.z - rhs.z) });
}

void Map::updateMissingChunkPosCache(const ls::Vec3I& currentChunkPos)
{
    const static std::vector<ls::Vec3I> order = []() ->std::vector<ls::Vec3I> 
    {
        std::vector<ls::Vec3I> result;
        const int dim = m_chunkLoadingRange * 2 + 1;
        result.reserve(dim * dim * dim);
        for (int dx = -m_chunkLoadingRange; dx <= m_chunkLoadingRange; ++dx)
        {
            for (int dy = -m_chunkLoadingRange; dy <= m_chunkLoadingRange; ++dy)
            {
                for (int dz = -m_chunkLoadingRange; dz <= m_chunkLoadingRange; ++dz)
                {
                    result.emplace_back(dx, dy, dz);
                }
            }
        }

        std::sort(result.begin(), result.end(), [](const ls::Vec3I& lhs, const ls::Vec3I& rhs) {
            return std::abs(lhs.x) + std::abs(lhs.y) + std::abs(lhs.z) < std::abs(rhs.x) + std::abs(rhs.y) + std::abs(rhs.z);
        });

        return result;
    }();

    m_missingChunkPosCache.clear();

    for (const auto& offset : order)
    {
        const auto pos = currentChunkPos + offset;
        if (!isValidChunkPos(pos)) continue;

        if (m_chunks.count(pos) == 0)
        {
            m_missingChunkPosCache.emplace_back(pos);
        }
    }

    m_missingChunkPosCacheCurrentPosition = 0;
}
