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

class MapChunkBlockData
{
public:

    Map* map;
    ls::Vec3I pos;
    uint32_t seed;
    ls::Array3<BlockContainer> blocks;

    MapChunkBlockData(Map& map, MapGenerator& mapGenerator, const ls::Vec3I& pos);

    MapChunkBlockData(const MapChunkBlockData&) = delete;
    MapChunkBlockData& operator=(const MapChunkBlockData&) = delete;
    MapChunkBlockData(MapChunkBlockData&&) noexcept = default;
    MapChunkBlockData& operator=(MapChunkBlockData&&) noexcept = default;

    ls::Vec3I firstBlockPosition() const;
};

class MapChunk
{
    friend class MapChunkBlockData;
public:
    MapChunk(Map& map, const ls::Vec3I& pos, const MapChunkNeighbours& neighbours);
    MapChunk(MapChunkBlockData&& chunkBlockData, const MapChunkNeighbours& neighbours);

    MapChunk(const MapChunk&) = delete;
    MapChunk& operator=(const MapChunk&) = delete;
    MapChunk(MapChunk&& other) noexcept;
    MapChunk& operator=(MapChunk&& other) noexcept;

    const ls::Vec3I& pos() const;

    ls::Vec3I firstBlockPosition() const;

    void onAdjacentChunkPlaced(MapChunk& placedChunk, const ls::Vec3I& placedChunkPos);

    void emplaceBlock(const BlockFactory& blockFactory, const ls::Vec3I& localPos, bool doUpdate = true);

    void placeBlock(BlockContainer&& block, const ls::Vec3I& localPos, bool doUpdate = true);

    BlockContainer removeBlock(const ls::Vec3I& localPos, bool doUpdate = true);

    void updateBlockOnAdjacentBlockChanged(const ls::Vec3I& blockToUpdateMapPos, Block& changedBlock, const ls::Vec3I& changedBlockMapPos);

    void updateAllAsIfPlaced();

    BlockContainer& at(const ls::Vec3I& localPos);
    const BlockContainer& at(const ls::Vec3I& localPos) const;

    const ls::Sphere3F& boundingSphere() const;

    const ls::Array3<BlockContainer>& blocks() const;
    const ls::Array3<BlockSideOpacity>& outsideOpacityCache() const;

    void draw(float dt, int& numUpdatedChunksOnDraw);
    void tooFarToDraw(float dt);
    void culled(float dt, int& numUpdatedChunksOnCull);

    uint32_t seed() const;

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
    uint32_t m_seed;
    ls::Vec3I m_pos;
    ls::Sphere3F m_boundingSphere;
    MapChunkRenderer m_renderer;
    ls::Array3<BlockContainer> m_blocks;
    ls::Array3<BlockSideOpacity> m_outsideOpacityCache;

    static constexpr int m_width = 32;
    static constexpr int m_height = 32;
    static constexpr int m_depth = 32;

    ls::Vec3I mapToLocalPos(const ls::Vec3I& mapPos) const;
    ls::Sphere3F computeBoundingSphere();
    void updateOutsideOpacity(const MapChunkNeighbours& neighbours);
    void updateOutsideOpacityOnChunkBorders(const MapChunkNeighbours& neighbours);
    void updateOutsideOpacityOnChunkBorder(const MapChunk& other, const ls::Vec3I& otherPos);
    void updateOutsideOpacityOnAdjacentBlockChanged(const ls::Vec3I& blockToUpdateMapPos, Block& changedBlock, const ls::Vec3I& changedBlockMapPos);

    BlockSideOpacity computeOutsideOpacity(ls::Vec3I blockPos, const ls::Array3<BlockSideOpacity>& cache);
    // created cache has padding on each side, so the coords are shifted by 1
    ls::Array3<BlockSideOpacity> createBlockOpacityCache();
};
