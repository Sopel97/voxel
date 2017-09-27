#include "map/MapChunk.h"

#include "map/Map.h"
#include "map/MapGenerator.h"

#include "block/Block.h"

#include "CubeSide.h"

MapChunk::MapChunk(Map& map, const ls::Vec3I& pos, const MapChunkNeighbours& neighbours) :
    m_map(&map),
    m_seed(map.seed()),
    m_pos(pos),
    m_blocks(m_width, m_height, m_depth, nullptr),
    m_outsideOpacityCache(m_width, m_height, m_depth, BlockSideOpacity::none())
{
    m_boundingSphere = computeBoundingSphere();
    updateOutsideOpacityOnChunkBorders(neighbours);
}

MapChunk::MapChunk(Map& map, MapGenerator& mapGenerator, const ls::Vec3I& pos, const MapChunkNeighbours& neighbours) :
    m_map(&map),
    m_seed(map.seed()),
    m_pos(pos),
    m_blocks(m_width, m_height, m_depth, nullptr),
    m_outsideOpacityCache(m_width, m_height, m_depth, BlockSideOpacity::none())
{
    m_boundingSphere = computeBoundingSphere();

    mapGenerator.generateChunk(*this);

    updateOutsideOpacity(neighbours);
}

const ls::Vec3I& MapChunk::pos() const
{
    return m_pos;
}

ls::Vec3I MapChunk::firstBlockPosition() const
{
    return m_pos * ls::Vec3I(m_width, m_height, m_depth);
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
    BlockContainer block = std::move(at(localPos));
    if (doUpdate)
    {
        block.block().onBlockRemoved(*m_map, localPos);
    }

    m_renderer.scheduleUpdate();

    return block;
}

uint32_t MapChunk::seed() const
{
    return m_seed;
}

void MapChunk::updateBlockOnAdjacentBlockPlaced(const ls::Vec3I& blockToUpdateMapPos, Block& placedBlock, const ls::Vec3I& placedBlockMapPos)
{
    at(blockToUpdateMapPos).block().onAdjacentBlockPlaced(*m_map, blockToUpdateMapPos, placedBlock, placedBlockMapPos);
    updateOutsideOpacityOnAdjacentBlockPlaced(blockToUpdateMapPos, placedBlock, placedBlockMapPos);

    m_renderer.scheduleUpdate();
}

void MapChunk::updateBlockOnAdjacentBlockRemoved(const ls::Vec3I& blockToUpdateMapPos, const ls::Vec3I& removedBlockMapPos)
{
    at(blockToUpdateMapPos).block().onAdjacentBlockRemoved(*m_map, blockToUpdateMapPos, removedBlockMapPos);
    updateOutsideOpacityOnAdjacentBlockRemoved(blockToUpdateMapPos, removedBlockMapPos);

    m_renderer.scheduleUpdate();
}

void MapChunk::updateAllAsIfPlaced()
{
    for (int x = 0; x < MapChunk::width(); ++x)
    {
        for (int y = 0; y < MapChunk::height(); ++y)
        {
            for (int z = 0; z < MapChunk::depth(); ++z)
            {
                auto& block = m_blocks.at(x, y, z);
                if (block.isEmpty())
                {
                    continue;
                }

                const ls::Vec3I pos = m_pos + ls::Vec3I(x, y, z);
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
void MapChunk::draw(float dt)
{
    m_renderer.draw(*this, dt);
}
void MapChunk::tooFarToDraw(float dt)
{
    m_renderer.tooFarToDraw(*this, dt);
}
void MapChunk::culled(float dt)
{
    m_renderer.culled(*this, dt);
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

const ls::Array3<BlockContainer>& MapChunk::blocks() const
{
    return m_blocks;
}
const ls::Array3<BlockSideOpacity>& MapChunk::outsideOpacityCache() const
{
    return m_outsideOpacityCache;
}
void MapChunk::updateOutsideOpacity(const MapChunkNeighbours& neighbours)
{
    // do the interior now, borders later
    ls::Array3<BlockSideOpacity> blockOpacityCache = createBlockOpacityCache();

    for (int x = 0; x < MapChunk::width(); ++x)
    {
        for (int y = 0; y < MapChunk::height(); ++y)
        {
            for (int z = 0; z < MapChunk::depth(); ++z)
            {
                const ls::Vec3I pos{ x, y, z };

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
    if (neighbours.bottom)
    {
        updateOutsideOpacityOnChunkBorder(*neighbours.bottom, m_pos + CubeSide::makeBottom().direction());
    }
    if (neighbours.top)
    {
        updateOutsideOpacityOnChunkBorder(*neighbours.top, m_pos + CubeSide::makeTop().direction());
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
                const auto otherOpacity = otherBlock.isEmpty() ? BlockSideOpacity::none() : otherBlock.block().sideOpacity();
                switch (dir)
                {
                case CubeSide::East:
                    selfOpacity.east = otherOpacity.west;
                    break;
                case CubeSide::West:
                    selfOpacity.west = otherOpacity.east;
                    break;
                case CubeSide::Bottom:
                    selfOpacity.top = selfOpacity.bottom;
                    break;
                case CubeSide::Top:
                    selfOpacity.bottom = selfOpacity.top;
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
void MapChunk::updateOutsideOpacityOnAdjacentBlockPlaced(const ls::Vec3I& blockToUpdateMapPos, Block& placedBlock, const ls::Vec3I& placedBlockMapPos)
{
    const ls::Vec3I localPos = mapToLocalPos(blockToUpdateMapPos);
    auto& selfOpacity = m_outsideOpacityCache(localPos.x, localPos.y, localPos.z);
    const auto otherOpacity = placedBlock.sideOpacity();
    const ls::Vec3I diff = placedBlockMapPos - blockToUpdateMapPos;
    const auto dir = CubeSide::fromDirection(diff);
    switch (dir)
    {
    case CubeSide::East:
        selfOpacity.east = otherOpacity.west;
        break;
    case CubeSide::West:
        selfOpacity.west = otherOpacity.east;
        break;
    case CubeSide::Bottom:
        selfOpacity.top = otherOpacity.bottom;
        break;
    case CubeSide::Top:
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
void MapChunk::updateOutsideOpacityOnAdjacentBlockRemoved(const ls::Vec3I& blockToUpdateMapPos, const ls::Vec3I& removedBlockMapPos)
{

}
BlockSideOpacity MapChunk::computeOutsideOpacity(ls::Vec3I blockPos, const ls::Array3<BlockSideOpacity>& cache)
{
    blockPos += ls::Vec3I(1, 1, 1);
    const ls::Vec3I eastPos = blockPos + CubeSide::makeEast().direction();
    const ls::Vec3I westPos = blockPos + CubeSide::makeWest().direction();
    const ls::Vec3I bottomPos = blockPos + CubeSide::makeBottom().direction();
    const ls::Vec3I topPos = blockPos + CubeSide::makeTop().direction();
    const ls::Vec3I southPos = blockPos + CubeSide::makeSouth().direction();
    const ls::Vec3I northPos = blockPos + CubeSide::makeNorth().direction();

    auto eastBlockOpacity = cache(eastPos.x, eastPos.y, eastPos.z);
    auto westBlockOpacity = cache(westPos.x, westPos.y, westPos.z);
    auto bottomBlockOpacity = cache(bottomPos.x, bottomPos.y, bottomPos.z);
    auto topBlockOpacity = cache(topPos.x, topPos.y, topPos.z);
    auto southBlockOpacity = cache(southPos.x, southPos.y, southPos.z);
    auto northBlockOpacity = cache(northPos.x, northPos.y, northPos.z);

    // we are interested from outside view on opacities
    // so the sides are switched
    return BlockSideOpacity{
        eastBlockOpacity.west,
        westBlockOpacity.east,
        bottomBlockOpacity.bottom,
        topBlockOpacity.top,
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
                auto& block = m_blocks.at(x - 1, y - 1, z - 1);
                if (block.isEmpty())
                {
                    continue;
                }

                cache(x, y, z) = block.block().sideOpacity();
            }
        }
    }

    return cache;
}