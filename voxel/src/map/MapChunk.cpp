#include "map/MapChunk.h"

#include "map/Map.h"
#include "map/MapGenerator.h"

#include "block/Block.h"

MapChunk::MapChunk(Map& map, const ls::Vec3I& pos, const MapChunkNeighbours& neighbours) :
    m_map(&map),
    m_pos(pos),
    m_blocks(m_width, m_height, m_depth, nullptr),
    m_outsideOpacityCache(m_width, m_height, m_depth, BlockSideOpacity::none())
{
    m_boundingSphere = computeBoundingSphere();
    updateOutsideOpacityOnChunkBorders(neighbours);
}

MapChunk::MapChunk(Map& map, MapGenerator& mapGenerator, const ls::Vec3I& pos, const MapChunkNeighbours& neighbours) :
    m_map(&map),
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

void MapChunk::emplaceBlock(const BlockFactory& blockFactory, const ls::Vec3I& localPos)
{
    placeBlock(blockFactory.instantiate(), localPos);
}

void MapChunk::placeBlock(BlockContainer&& block, const ls::Vec3I& localPos)
{
    BlockContainer& dest = at(localPos);
    dest = std::move(block);
    dest.block().onBlockPlaced(*m_map, localPos);

    m_renderer.scheduleUpdate();
}

BlockContainer MapChunk::removeBlock(const ls::Vec3I& localPos)
{
    BlockContainer block = std::move(at(localPos));
    block.block().onBlockRemoved(*m_map, localPos);

    m_renderer.scheduleUpdate();

    return block;
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
void MapChunk::draw()
{
    m_renderer.draw(*this);
}
void MapChunk::noLongerRendered()
{
    m_renderer.noLongerRendered(*this);
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

}
void MapChunk::updateOutsideOpacityOnChunkBorders(const MapChunkNeighbours& neighbours)
{

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