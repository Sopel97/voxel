#include "map/MapChunk.h"

#include "map/Map.h"
#include "map/MapGenerator.h"

#include "block/Block.h"

#include "CubeSide.h"

MapChunkBlockData::MapChunkBlockData(Map& map, MapGenerator& mapGenerator, const ls::Vec3I& pos) :
    map(&map),
    pos(pos),
    seed(map.seed()),
    blocks(MapChunkStorageReserve::instance().loadBlockArray())
{
    mapGenerator.generateChunk(*this);
}

MapChunkBlockData::~MapChunkBlockData()
{
    if (!blocks.isEmpty())
    {
        MapChunkStorageReserve::instance().storeBlockArray(std::move(blocks));
    }
}
ls::Vec3I MapChunkBlockData::firstBlockPosition() const
{
    return pos * ls::Vec3I(static_cast<int>(MapChunk::m_width), static_cast<int>(MapChunk::m_height), static_cast<int>(MapChunk::m_depth));
}
MapChunk::MapChunk(Map& map, const ls::Vec3I& pos, const MapChunkNeighbours& neighbours) :
    m_map(&map),
    m_seed(map.seed()),
    m_pos(pos),
    m_blocks(MapChunkStorageReserve::instance().loadBlockArray()),
    m_outsideOpacityCache(MapChunkStorageReserve::instance().loadOpacityArray())
{
    m_boundingSphere = computeBoundingSphere();
    updateOutsideOpacityOnChunkBorders(neighbours);
}
MapChunk::MapChunk(MapChunkBlockData&& chunkBlockData, const MapChunkNeighbours& neighbours) :
    m_map(chunkBlockData.map),
    m_seed(chunkBlockData.seed),
    m_pos(chunkBlockData.pos),
    m_blocks(std::move(chunkBlockData.blocks)),
    m_outsideOpacityCache(MapChunkStorageReserve::instance().loadOpacityArray())
{
    m_boundingSphere = computeBoundingSphere();
    updateOutsideOpacity(neighbours);
    updateAllAsIfPlaced();
}
MapChunk::MapChunk(MapChunk&& other) noexcept :
    m_map(std::move(other.m_map)),
    m_seed(std::move(other.m_seed)),
    m_pos(std::move(other.m_pos)),
    m_boundingSphere(std::move(other.m_boundingSphere)),
    m_renderer(std::move(other.m_renderer)),
    m_blocks(std::move(other.m_blocks)),
    m_outsideOpacityCache(std::move(other.m_outsideOpacityCache))
{

}

MapChunk::~MapChunk()
{
    if (!m_blocks.isEmpty())
    {
        MapChunkStorageReserve::instance().storeBlockArray(std::move(m_blocks));
    }
    if (!m_outsideOpacityCache.isEmpty())
    {
        MapChunkStorageReserve::instance().storeOpacityArray(std::move(m_outsideOpacityCache));
    }
}
MapChunk& MapChunk::operator=(MapChunk&& other) noexcept
{
    m_map = std::move(other.m_map);
    m_seed = std::move(other.m_seed);
    m_pos = std::move(other.m_pos);
    m_boundingSphere = std::move(other.m_boundingSphere);
    m_renderer = std::move(other.m_renderer);
    m_blocks = std::move(other.m_blocks);
    m_outsideOpacityCache = std::move(other.m_outsideOpacityCache);

    return *this;
}

const ls::Vec3I& MapChunk::pos() const
{
    return m_pos;
}

ls::Vec3I MapChunk::firstBlockPosition() const
{
    return m_pos * ls::Vec3I(static_cast<int>(m_width), static_cast<int>(m_height), static_cast<int>(m_depth));
}

void MapChunk::onAdjacentChunkPlaced(MapChunk& placedChunk, const ls::Vec3I& placedChunkPos)
{
    updateOutsideOpacityOnChunkBorder(placedChunk, placedChunkPos);

    m_renderer.scheduleUpdate();
}

void MapChunk::emplaceBlock(const BlockFactory& blockFactory, const ls::Vec3I& localPos, bool doUpdate)
{
    placeBlock(blockFactory.instantiate(), localPos, doUpdate);
}

void MapChunk::placeBlock(BlockContainer&& block, const ls::Vec3I& localPos, bool doUpdate)
{
    BlockContainer& dest = at(localPos);
    dest = std::move(block);
    if (doUpdate)
    {
        dest.block().onBlockPlaced(*m_map, localPos);
    }

    m_renderer.scheduleUpdate();
}

BlockContainer MapChunk::removeBlock(const ls::Vec3I& localPos, bool doUpdate)
{
    BlockContainer& block = at(localPos);
    if (doUpdate)
    {
        block.block().onBlockRemoved(*m_map, localPos);
    }

    block = m_map->instantiateAirBlock();

    m_renderer.scheduleUpdate();

    return block;
}

uint32_t MapChunk::seed() const
{
    return m_seed;
}

void MapChunk::updateBlockOnAdjacentBlockChanged(const ls::Vec3I& blockToUpdateMapPos, Block& changedBlock, const ls::Vec3I& changedBlockMapPos)
{
    at(blockToUpdateMapPos).block().onAdjacentBlockChanged(*m_map, blockToUpdateMapPos, changedBlock, changedBlockMapPos);
    updateOutsideOpacityOnAdjacentBlockChanged(blockToUpdateMapPos, changedBlock, changedBlockMapPos);

    m_renderer.scheduleUpdate();
}

void MapChunk::updateAllAsIfPlaced()
{
    for (size_t x = 0; x < MapChunk::width(); ++x)
    {
        for (size_t y = 0; y < MapChunk::height(); ++y)
        {
            for (size_t z = 0; z < MapChunk::depth(); ++z)
            {
                auto& block = m_blocks.at(x, y, z);

                const ls::Vec3I pos = m_pos + ls::Vec3I(static_cast<int>(x), static_cast<int>(y), static_cast<int>(z));
                block.block().onBlockPlaced(*m_map, pos);
            }
        }
    }
}

BlockContainer& MapChunk::at(const ls::Vec3I& localPos)
{
    return m_blocks(localPos.x, localPos.y, localPos.z);
}
const BlockContainer& MapChunk::at(const ls::Vec3I& localPos) const
{
    return m_blocks(localPos.x, localPos.y, localPos.z);
}

const ls::Sphere3F& MapChunk::boundingSphere() const
{
    return m_boundingSphere;
}
void MapChunk::draw(float dt, int& numUpdatedChunksOnDraw)
{
    m_renderer.draw(*this, dt, numUpdatedChunksOnDraw);
}
void MapChunk::tooFarToDraw(float dt)
{
    m_renderer.tooFarToDraw(*this, dt);
}
void MapChunk::culled(float dt, int& numUpdatedChunksOnCull)
{
    m_renderer.culled(*this, dt, numUpdatedChunksOnCull);
}

ls::Vec3I MapChunk::mapToLocalPos(const ls::Vec3I& mapPos) const
{
    return mapPos - firstBlockPosition();
}

ls::Sphere3F MapChunk::computeBoundingSphere()
{
    static constexpr ls::Vec3F chunkSizeF(static_cast<float>(MapChunk::width()), static_cast<float>(MapChunk::height()), static_cast<float>(MapChunk::depth()));

    ls::Sphere3F sphere;

    sphere.origin = (static_cast<ls::Vec3F>(m_pos) + ls::Vec3F(0.5f, 0.5f, 0.5f)) * chunkSizeF;
    sphere.radius = chunkSizeF.length() / 2.0f;

    return sphere;
}

const MapChunk::BlockArray& MapChunk::blocks() const
{
    return m_blocks;
}
const MapChunk::BlockSideOpacityArray& MapChunk::outsideOpacityCache() const
{
    return m_outsideOpacityCache;
}
void MapChunk::updateOutsideOpacity(const MapChunkNeighbours& neighbours)
{
    // do the interior now, borders later
    ls::Array3<BlockSideOpacity> blockOpacityCache = createBlockOpacityCache();

    for (size_t x = 0; x < MapChunk::width(); ++x)
    {
        for (size_t y = 0; y < MapChunk::height(); ++y)
        {
            for (size_t z = 0; z < MapChunk::depth(); ++z)
            {
                const ls::Vec3I pos{ static_cast<int>(x), static_cast<int>(y), static_cast<int>(z) };

                m_outsideOpacityCache(x, y, z) = computeOutsideOpacity(pos, blockOpacityCache);
            }
        }
    }

    updateOutsideOpacityOnChunkBorders(neighbours);
}
void MapChunk::updateOutsideOpacityOnChunkBorders(const MapChunkNeighbours& neighbours)
{
    if (neighbours.east)
    {
        updateOutsideOpacityOnChunkBorder(*neighbours.east, m_pos + CubeSide::makeEast().direction());
    }
    if (neighbours.west)
    {
        updateOutsideOpacityOnChunkBorder(*neighbours.west, m_pos + CubeSide::makeWest().direction());
    }
    if (neighbours.top)
    {
        updateOutsideOpacityOnChunkBorder(*neighbours.top, m_pos + CubeSide::makeTop().direction());
    }
    if (neighbours.bottom)
    {
        updateOutsideOpacityOnChunkBorder(*neighbours.bottom, m_pos + CubeSide::makeBottom().direction());
    }
    if (neighbours.south)
    {
        updateOutsideOpacityOnChunkBorder(*neighbours.south, m_pos + CubeSide::makeSouth().direction());
    }
    if (neighbours.north)
    {
        updateOutsideOpacityOnChunkBorder(*neighbours.north, m_pos + CubeSide::makeNorth().direction());
    }
}
void MapChunk::updateOutsideOpacityOnChunkBorder(const MapChunk& other, const ls::Vec3I& otherPos)
{
    const ls::Vec3I diff = otherPos - m_pos;
    const auto dir = CubeSide::fromDirection(diff);

    // calculate bounding coords of a 16x16 plane of blocks
    // on the side next to the other chunk
    int minX = diff.x == 1 ? m_width - 1 : 0;
    int minY = diff.y == 1 ? m_height - 1 : 0;
    int minZ = diff.z == 1 ? m_depth - 1 : 0;

    int maxX = std::abs(diff.x) == 1 ? minX : m_width - 1;
    int maxY = std::abs(diff.y) == 1 ? minY : m_height - 1;
    int maxZ = std::abs(diff.z) == 1 ? minZ : m_depth - 1;

    for (int x = minX; x <= maxX; ++x)
    {
        for (int y = minY; y <= maxY; ++y)
        {
            for (int z = minZ; z <= maxZ; ++z)
            {
                const ls::Vec3I pos{ x, y, z };

                // calculate position of a block next to this one but in the other chunk
                // avoid modulo with negative number
                const ls::Vec3I otherPos{ (pos.x + diff.x + m_width) % m_width, (pos.y + diff.y + m_height) % m_height, (pos.z + diff.z + m_depth) % m_depth };
                auto& selfOpacity = m_outsideOpacityCache(x, y, z);
                const auto& otherBlock = other.m_blocks(otherPos.x, otherPos.y, otherPos.z);
                const auto otherOpacity = otherBlock.block().sideOpacity();
                switch (dir)
                {
                case CubeSide::East:
                    selfOpacity.east = otherOpacity.west;
                    break;
                case CubeSide::West:
                    selfOpacity.west = otherOpacity.east;
                    break;
                case CubeSide::Top:
                    selfOpacity.top = otherOpacity.bottom;
                    break;
                case CubeSide::Bottom:
                    selfOpacity.bottom = otherOpacity.top;
                    break;
                case CubeSide::South:
                    selfOpacity.south = otherOpacity.north;
                    break;
                case CubeSide::North:
                    selfOpacity.north = otherOpacity.south;
                    break;
                default:
                    break;
                }
            }
        }
    }
}
void MapChunk::updateOutsideOpacityOnAdjacentBlockChanged(const ls::Vec3I& blockToUpdateMapPos, Block& changedBlock, const ls::Vec3I& changedBlockMapPos)
{
    const ls::Vec3I localPos = mapToLocalPos(blockToUpdateMapPos);
    auto& selfOpacity = m_outsideOpacityCache(localPos.x, localPos.y, localPos.z);
    const auto otherOpacity = changedBlock.sideOpacity();
    const ls::Vec3I diff = changedBlockMapPos - blockToUpdateMapPos;
    const auto dir = CubeSide::fromDirection(diff);
    switch (dir)
    {
    case CubeSide::East:
        selfOpacity.east = otherOpacity.west;
        break;
    case CubeSide::West:
        selfOpacity.west = otherOpacity.east;
        break;
    case CubeSide::Top:
        selfOpacity.top = otherOpacity.bottom;
        break;
    case CubeSide::Bottom:
        selfOpacity.bottom = otherOpacity.top;
        break;
    case CubeSide::South:
        selfOpacity.south = otherOpacity.north;
        break;
    case CubeSide::North:
        selfOpacity.north = otherOpacity.south;
        break;
    default:
        break;
    }
}
BlockSideOpacity MapChunk::computeOutsideOpacity(ls::Vec3I blockPos, const ls::Array3<BlockSideOpacity>& cache)
{
    blockPos += ls::Vec3I(1, 1, 1); // because cache is offseted
    const ls::Vec3I eastPos = blockPos + CubeSide::makeEast().direction();
    const ls::Vec3I westPos = blockPos + CubeSide::makeWest().direction();
    const ls::Vec3I topPos = blockPos + CubeSide::makeTop().direction();
    const ls::Vec3I bottomPos = blockPos + CubeSide::makeBottom().direction();
    const ls::Vec3I southPos = blockPos + CubeSide::makeSouth().direction();
    const ls::Vec3I northPos = blockPos + CubeSide::makeNorth().direction();

    auto eastBlockOpacity = cache(eastPos.x, eastPos.y, eastPos.z);
    auto westBlockOpacity = cache(westPos.x, westPos.y, westPos.z);
    auto topBlockOpacity = cache(topPos.x, topPos.y, topPos.z);
    auto bottomBlockOpacity = cache(bottomPos.x, bottomPos.y, bottomPos.z);
    auto southBlockOpacity = cache(southPos.x, southPos.y, southPos.z);
    auto northBlockOpacity = cache(northPos.x, northPos.y, northPos.z);

    // we are interested from outside view on opacities
    // so the sides are switched
    return BlockSideOpacity{
        eastBlockOpacity.west,
        westBlockOpacity.east,
        topBlockOpacity.bottom,
        bottomBlockOpacity.top,
        southBlockOpacity.north,
        northBlockOpacity.south
    };
}
ls::Array3<BlockSideOpacity> MapChunk::createBlockOpacityCache()
{
    ls::Array3<BlockSideOpacity> cache(m_width + 2, m_height + 2, m_depth + 2, BlockSideOpacity::none());

    for (int x = 1; x < m_width + 1; ++x)
    {
        for (int y = 1; y < m_height + 1; ++y)
        {
            for (int z = 1; z < m_depth + 1; ++z)
            {
                const auto& block = m_blocks.at(x - 1, y - 1, z - 1);

                cache(x, y, z) = block.block().sideOpacity();
            }
        }
    }

    return cache;
}