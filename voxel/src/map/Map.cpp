#include "map/Map.h"

#include "Game.h"

#include "CubeSide.h"

#include <cstdlib>

Map::Map(uint32_t seed) :
    m_renderer{},
    m_generator(*this),
    m_seed(seed),
    m_airFactory(ResourceManager<BlockFactory>::instance().get("Air")),
    m_timeSinceLastMissingChunkPosCacheUpdate(0.0f),
    m_missingChunkPosCacheCurrentPosition(0),
    m_missingChunkPosCacheLastOrigin(1, 1, 1) // must not be the starting chunk
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

BlockContainer Map::instantiateAirBlock() const
{
    return m_airFactory.get().instantiate();
}

void Map::update(Game& game, float dt)
{
    const auto& cameraPos = game.camera().position();

    const auto currentChunk = worldToChunk(cameraPos);

    trySpawnNewChunks(currentChunk);
    unloadFarChunks(currentChunk);

    m_timeSinceLastMissingChunkPosCacheUpdate += dt;
    if (m_timeSinceLastMissingChunkPosCacheUpdate >= m_timeBetweenMissingChunkPosCacheUpdates && m_missingChunkPosCacheLastOrigin != currentChunk)
    {
        updateMissingChunkPosCache(currentChunk);
        m_timeSinceLastMissingChunkPosCacheUpdate = 0.0f;
        m_missingChunkPosCacheLastOrigin = currentChunk;
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
    if (pos.y >= m_maxWorldHeight / static_cast<int>(MapChunk::height())) return false;

    return true;
}


std::vector<std::pair<ls::Vec3I, MapChunkBlockData>> Map::generateChunksIsolated(const std::vector<ls::Vec3I>& positions)
{
    std::vector<std::pair<ls::Vec3I, MapChunkBlockData>> chunks;

    for (const auto& pos : positions)
    {
        chunks.emplace_back(generateChunkIsolated(pos));
    }

    return chunks;
}
std::pair<ls::Vec3I, MapChunkBlockData> Map::generateChunkIsolated(const ls::Vec3I& pos)
{
    return std::make_pair(pos, MapChunkBlockData(*this, m_generator, pos));
}
void Map::trySpawnNewChunks(const ls::Vec3I& currentChunk)
{
    bool ready = false;
    if (m_generatedChunks.valid() && m_generatedChunks.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
    {
        ready = true;

        auto chunks = m_generatedChunks.get();
        for (auto& chunk : chunks)
        {
            spawnChunk(chunk.first, std::move(chunk.second));
        }
        m_missingChunksInGeneration.clear();
    }

    if(!m_generatedChunks.valid() || ready)
    {
        const size_t numMissingChunks = m_missingChunkPosCache.size();
        for (size_t i = 0; i < m_maxChunksSpawnedPerUpdate && m_missingChunkPosCacheCurrentPosition < numMissingChunks; ++i)
        {
            m_missingChunksInGeneration.emplace_back(m_missingChunkPosCache[m_missingChunkPosCacheCurrentPosition]);
            ++m_missingChunkPosCacheCurrentPosition;
        }

        m_generatedChunks = std::async(std::launch::async, [this](const std::vector<ls::Vec3I>& p) {return generateChunksIsolated(p); }, m_missingChunksInGeneration);
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
    MapChunkBlockData blockData(*this, m_generator, pos);
    spawnChunk(pos, std::move(blockData));
}
void Map::spawnChunk(const ls::Vec3I& pos, MapChunkBlockData&& chunk)
{
    const auto neighbours = chunkNeighbours(pos);
    auto p = m_chunks.emplace(std::make_pair(pos, MapChunk(std::move(chunk), neighbours)));
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
    int numRemovedChunks = 0;
    for (auto iter = m_chunks.begin(); iter != m_chunks.end() && numRemovedChunks < m_maxChunksRemovedPerUpdate;)
    {
        auto& pos = iter->first;
        if (distanceBetweenChunks(currentChunk, pos) >= m_minChunkDistanceToUnload)
        {
            iter = unloadChunk(iter);
            ++numRemovedChunks;
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
            return std::abs(lhs.x) + std::abs(lhs.y * 2) + std::abs(lhs.z) < std::abs(rhs.x) + std::abs(rhs.y * 2) + std::abs(rhs.z); // value horizontal directions more
        });

        return result;
    }();

    m_missingChunkPosCache.clear();

    for (const auto& offset : order)
    {
        const auto pos = currentChunkPos + offset;
        if (!isValidChunkPos(pos)) continue;

        if (m_chunks.count(pos) == 0 && std::find(m_missingChunksInGeneration.begin(), m_missingChunksInGeneration.end(), pos) == m_missingChunksInGeneration.end())
        {
            m_missingChunkPosCache.emplace_back(pos);
        }
    }

    m_missingChunkPosCacheCurrentPosition = 0;
}
