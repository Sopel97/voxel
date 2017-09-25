#include "MapChunk.h"

#include "Map.h"

#include "block/Block.h"

MapChunk::MapChunk(Map& map, const ls::Vec3I& pos, const MapChunkNeighbours& neighbours) :
    m_map(&map),
    m_pos(pos),
    m_blocks(m_width, m_height, m_depth)
{

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

}

void MapChunk::emplaceBlock(const BlockFactory& blockFactory, const ls::Vec3I& mapPos)
{
    placeBlock(blockFactory.instantiate(), mapPos);
}

void MapChunk::placeBlock(BlockContainer&& block, const ls::Vec3I& mapPos)
{
    BlockContainer& dest = at(mapPos);
    dest = std::move(block);
    dest.block().onBlockPlaced(*m_map, mapPos);
}

BlockContainer MapChunk::removeBlock(const ls::Vec3I& mapPos)
{
    BlockContainer block = std::move(at(mapPos));
    block.block().onBlockRemoved(*m_map, mapPos);
    return block;
}

void MapChunk::updateBlockOnAdjacentBlockPlaced(const ls::Vec3I& blockToUpdateMapPos, Block& placedBlock, const ls::Vec3I& placedBlockMapPos)
{
    at(blockToUpdateMapPos).block().onAdjacentBlockPlaced(*m_map, blockToUpdateMapPos, placedBlock, placedBlockMapPos);
}

void MapChunk::updateBlockOnAdjacentBlockRemoved(const ls::Vec3I& blockToUpdateMapPos, const ls::Vec3I& removedBlockMapPos)
{
    at(blockToUpdateMapPos).block().onAdjacentBlockRemoved(*m_map, blockToUpdateMapPos, removedBlockMapPos);
}

BlockContainer& MapChunk::at(const ls::Vec3I& mapPos)
{
    return atLocal(mapToLocalPos(mapPos));
}
const BlockContainer& MapChunk::at(const ls::Vec3I& mapPos) const
{
    return atLocal(mapToLocalPos(mapPos));
}

BlockContainer& MapChunk::atLocal(const ls::Vec3I& localPos)
{
    return m_blocks(localPos.x, localPos.y, localPos.z);
}
const BlockContainer& MapChunk::atLocal(const ls::Vec3I& localPos) const
{
    return m_blocks(localPos.x, localPos.y, localPos.z);
}

ls::Vec3I MapChunk::mapToLocalPos(const ls::Vec3I& mapPos) const
{
    return mapPos - firstBlockPosition();
}
