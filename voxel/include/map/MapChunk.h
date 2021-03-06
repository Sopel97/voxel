#pragma once

#include "../LibS/Shapes/Vec3.h"
#include "../LibS/Shapes/Sphere3.h"
#include "../LibS/Array3.h"

#include "block/BlockContainer.h"
#include "block/BlockFactory.h"
#include "block/BlockSideOpacity.h"

#include "MapChunkRenderer.h"

#include <queue>
#include <vector>
#include <mutex>

class MapChunk;
class Map;
class MapGenerator;

namespace detail
{
    constexpr size_t m_chunkWidth = 32;
    constexpr size_t m_chunkHeight = 32;
    constexpr size_t m_chunkDepth = 32;

    using BlockArray = ls::Array3<BlockContainer, m_chunkWidth, m_chunkHeight, m_chunkDepth>;
    using BlockSideOpacityArray = ls::Array3<BlockSideOpacity, m_chunkWidth, m_chunkHeight, m_chunkDepth>;

    template<class Data>
    class concurrent_queue
    {
    private:
        std::queue<Data> the_queue;
        mutable std::mutex the_mutex;
    public:
        template <class... Args>
        void emplace(Args&&... args)
        {
            std::unique_lock<std::mutex> lock(the_mutex);
            the_queue.emplace(std::forward<Args>(args)...);
            lock.unlock();
        }
        void push(Data&& data)
        {
            std::unique_lock<std::mutex> lock(the_mutex);
            the_queue.emplace(std::move(data));
            lock.unlock();
        }

        bool empty() const
        {
            return the_queue.empty();
        }

        Data pop()
        {
            std::unique_lock<std::mutex> lock(the_mutex);
            Data d = std::move(the_queue.front());
            the_queue.pop();
            lock.unlock();
            return d;
        }
    };
}

using MapChunkNeighbours = PerCubeSideData<MapChunk*>;

class MapChunkStorageReserve
{
public:
    static MapChunkStorageReserve& instance()
    {
        static MapChunkStorageReserve singleton;

        return singleton;
    }

    void storeBlockArray(detail::BlockArray&& arr)
    {
        arr.fill(nullptr);
        m_blockArrays.emplace(std::move(arr));
    }
    detail::BlockArray loadBlockArray()
    {
        if (m_blockArrays.empty()) createBlockArray();
        return m_blockArrays.pop();
    }
    void storeOpacityArray(detail::BlockSideOpacityArray&& arr)
    {
        arr.fill(BlockSideOpacity::none());
        m_opacityArrays.emplace(std::move(arr));
    }
    detail::BlockSideOpacityArray loadOpacityArray()
    {
        if (m_opacityArrays.empty()) createOpacityArray();
        return m_opacityArrays.pop();
    }

    void createBlockArray()
    {
        m_blockArrays.emplace(nullptr);
    }
    void createOpacityArray()
    {
        m_opacityArrays.emplace(BlockSideOpacity::none());
    }

private:
    detail::concurrent_queue<detail::BlockArray> m_blockArrays;
    detail::concurrent_queue<detail::BlockSideOpacityArray> m_opacityArrays;
};

class MapChunkBlockData
{
    using BlockArray = detail::BlockArray;
public:
    Map* map;
    ls::Vec3I pos;
    uint32_t seed;
    BlockArray blocks;

    MapChunkBlockData(Map& map, MapGenerator& mapGenerator, const ls::Vec3I& pos);

    MapChunkBlockData(const MapChunkBlockData&) = delete;
    MapChunkBlockData& operator=(const MapChunkBlockData&) = delete;
    MapChunkBlockData(MapChunkBlockData&&) noexcept = default;
    MapChunkBlockData& operator=(MapChunkBlockData&&) noexcept = default;

    ~MapChunkBlockData();

    ls::Vec3I firstBlockPosition() const;
};

class MapChunk
{
    friend class MapChunkBlockData;

    static constexpr size_t m_width = detail::m_chunkWidth;
    static constexpr size_t m_height = detail::m_chunkHeight;
    static constexpr size_t m_depth = detail::m_chunkDepth;

    using BlockArray = detail::BlockArray;
    using BlockSideOpacityArray = detail::BlockSideOpacityArray;

public:
    MapChunk(Map& map, const ls::Vec3I& pos, const MapChunkNeighbours& neighbours);
    MapChunk(MapChunkBlockData&& chunkBlockData, const MapChunkNeighbours& neighbours);

    MapChunk(const MapChunk&) = delete;
    MapChunk& operator=(const MapChunk&) = delete;
    MapChunk(MapChunk&& other) noexcept;
    MapChunk& operator=(MapChunk&& other) noexcept;

    ~MapChunk();

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

    const BlockArray& blocks() const;
    const BlockSideOpacityArray& outsideOpacityCache() const;

    void draw(float dt, int& numUpdatedChunksOnDraw);
    void tooFarToDraw(float dt);
    void culled(float dt, int& numUpdatedChunksOnCull);

    uint32_t seed() const;

    static constexpr size_t width()
    {
        return m_width;
    }
    static constexpr size_t height()
    {
        return m_height;
    }
    static constexpr size_t depth()
    {
        return m_depth;
    }

private:
    Map* m_map;
    uint32_t m_seed;
    ls::Vec3I m_pos;
    ls::Sphere3F m_boundingSphere;
    MapChunkRenderer m_renderer;
    BlockArray m_blocks;
    BlockSideOpacityArray m_outsideOpacityCache;

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
