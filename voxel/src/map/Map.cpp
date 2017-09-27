#include "map/Map.h"

#include "Game.h"

#include "CubeSide.h"

#include <cstdlib>

Map::Map(uint32_t seed) :
    m_renderer{},
    m_generator(*this),
    m_seed(seed)
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
    unloadFarChunks(currentChunk);
    trySpawnNewChunk(currentChunk);
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
    constexpr int numTries = 100;
    constexpr int maxRange = 10;
    constexpr int maxChunksSpawned = 10;

    int numChunksSpawned = 0;
    for (int i = 0; i < numTries && numChunksSpawned < maxChunksSpawned; ++i)
    {
        ls::Vec3I offset(
            rand() % (maxRange * 2 + 1) - maxRange,
            rand() % (maxRange * 2 + 1) - maxRange,
            rand() % (maxRange * 2 + 1) - maxRange
        );

        ls::Vec3I sgn(offset.x > 0 ? 1 : -1, offset.y > 0 ? 1 : -1, offset.z > 0 ? 1 : -1);
        offset *= offset;
        offset /= 10;
        offset *= sgn;

        const ls::Vec3I pos = currentChunk + offset;
        if (isValidChunkPos(pos))
        {
            if (m_chunks.count(offset) == 0)
            {
                spawnChunk(pos);
                ++numChunksSpawned;
            }
        }
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
    MapChunk* bottom = chunkAt(pos + CubeSide::makeBottom().direction());
    MapChunk* top = chunkAt(pos + CubeSide::makeTop().direction());
    MapChunk* south = chunkAt(pos + CubeSide::makeSouth().direction());
    MapChunk* north = chunkAt(pos + CubeSide::makeNorth().direction());

    return MapChunkNeighbours{ east, west, bottom, top, south, north };
}
void Map::spawnChunk(const ls::Vec3I& pos)
{
    const auto neighbours = chunkNeighbours(pos);
    auto p = m_chunks.emplace(std::make_pair(pos, MapChunk(*this, m_generator, pos, neighbours)));
    auto& placedChunk = p.first->second;

    if (neighbours.east) neighbours.east->onAdjacentChunkPlaced(placedChunk, pos);
    if (neighbours.west) neighbours.west->onAdjacentChunkPlaced(placedChunk, pos);
    if (neighbours.bottom) neighbours.bottom->onAdjacentChunkPlaced(placedChunk, pos);
    if (neighbours.top) neighbours.top->onAdjacentChunkPlaced(placedChunk, pos);
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