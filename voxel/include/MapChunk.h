#pragma once

#include "../LibS/Shapes/Vec3.h"
#include "../LibS/Shapes/Sphere3.h"
#include "../LibS/Array3.h"

#include "block/BlockContainer.h"
#include "block/BlockFactory.h"
#include "block/BlockSideOpacity.h"

#include "MapChunkRenderer.h"

class MapChunk;
class Map;
class MapGenerator;

struct MapChunkNeighbours
{
    MapChunk* east;
    MapChunk* west;
    MapChunk* top;
    MapChunk* bottom;
    MapChunk* south;
    MapChunk* north;
};



class MapChunk
{
public:
    MapChunk(Map& map, const ls::Vec3I& pos, const MapChunkNeighbours& neighbours);
    MapChunk(Map& map, MapGenerator& mapGenerator, const ls::Vec3I& pos, const MapChunkNeighbours& neighbours);

    const ls::Vec3I& pos() const;

    ls::Vec3I firstBlockPosition() const;

    void onAdjacentChunkPlaced(MapChunk& placedChunk, const ls::Vec3I& placedChunkPos);

    void emplaceBlock(const BlockFactory& blockFactory, const ls::Vec3I& localPos);

    void placeBlock(BlockContainer&& block, const ls::Vec3I& localPos);

    BlockContainer removeBlock(const ls::Vec3I& localPos);

    void updateBlockOnAdjacentBlockPlaced(const ls::Vec3I& blockToUpdateMapPos, Block& placedBlock, const ls::Vec3I& placedBlockMapPos);

    void updateBlockOnAdjacentBlockRemoved(const ls::Vec3I& blockToUpdateMapPos, const ls::Vec3I& removedBlockMapPos);

    BlockContainer& at(const ls::Vec3I& localPos);
    const BlockContainer& at(const ls::Vec3I& localPos) const;

    const ls::Sphere3F& boundingSphere() const;

    const ls::Array3<BlockContainer>& blocks() const;
    const ls::Array3<BlockSideOpacity>& outsideOpacityCache() const;

    void draw();
    void noLongerRendered();

    static constexpr int width()
    {
        return m_width;
    }
    static constexpr int height()
    {
        return m_height;
    }
    static constexpr int depth()
    {
        return m_depth;
    }

private:
    Map* m_map;
    ls::Vec3I m_pos;
    ls::Sphere3F m_boundingSphere;
    MapChunkRenderer m_renderer;
    ls::Array3<BlockContainer> m_blocks;
    ls::Array3<BlockSideOpacity> m_outsideOpacityCache;

    static constexpr int m_width = 16;
    static constexpr int m_height = 16;
    static constexpr int m_depth = 16;

    ls::Vec3I mapToLocalPos(const ls::Vec3I& mapPos) const;
    ls::Sphere3F computeBoundingSphere();
    void updateOutsideOpacity(const MapChunkNeighbours& neighbours);
    void updateOutsideOpacityOnChunkBorders(const MapChunkNeighbours& neighbours);
    void updateOutsideOpacityOnChunkBorder(const MapChunk& other, const ls::Vec3I& otherPos);
    void updateOutsideOpacityOnAdjacentBlockPlaced(const ls::Vec3I& blockToUpdateMapPos, Block& placedBlock, const ls::Vec3I& placedBlockMapPos);
    void updateOutsideOpacityOnAdjacentBlockRemoved(const ls::Vec3I& blockToUpdateMapPos, const ls::Vec3I& removedBlockMapPos);
};
