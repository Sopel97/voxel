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
void MapChunk::noLongerRendered(float dt)
{
    m_renderer.noLongerRendered(*this, dt);
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

}
void MapChunk::updateOutsideOpacityOnAdjacentBlockPlaced(const ls::Vec3I& blockToUpdateMapPos, Block& placedBlock, const ls::Vec3I& placedBlockMapPos)
{

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
        bottomBlockOpacity.top,
        topBlockOpacity.bottom,
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
                auto& block = m_blocks.at(x-1, y-1, z-1);
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