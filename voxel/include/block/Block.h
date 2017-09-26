#pragma once

#include <memory>
#include <iostream>

#include "../LibS/Json.h"
#include "../LibS/Shapes/Vec3.h"

#include "BlockSideOpacity.h"

class Map;
struct BlockVertex;

template <class BlockType>
class SpecificBlockFactory;

template <class BlockType>
struct BlockSharedData
{
    BlockSharedData(SpecificBlockFactory<BlockType>& blockFactory, const ls::json::Value& config) :
        blockFactory(&blockFactory)
    {
    }

    SpecificBlockFactory<BlockType>* blockFactory;
};

class Block
{
public:
    virtual bool isStateful() = 0;

    virtual void onBlockPlaced(Map& map, const ls::Vec3I& pos)
    {
    }
    virtual void onBlockRemoved(Map& map, const ls::Vec3I& pos)
    {
    }
    virtual void onAdjacentBlockPlaced(Map& map, const ls::Vec3I& thisPos, Block& placedBlock, const ls::Vec3I& placedBlockPos)
    {
    }
    virtual void onAdjacentBlockRemoved(Map& map, const ls::Vec3I& thisPos, const ls::Vec3I& removedBlockPos)
    {
    }
    virtual void draw(std::vector<BlockVertex>& vertices, std::vector<unsigned>& indices, const ls::Vec3I& position, BlockSideOpacity outsideOpacity) const
    {
    }
    virtual BlockSideOpacity sideOpacity() const
    {
        return BlockSideOpacity::none();
    }

    virtual ~Block()
    {
    }

    virtual std::unique_ptr<Block> clone() const = 0;
};

class StatelessBlock : public Block
{
public:
    virtual bool isStateful() final
    {
        return false;
    }
    static constexpr bool isStatefulStatic()
    {
        return false;
    }
};

class StatefulBlock : public Block
{
public:
    virtual bool isStateful() final
    {
        return true;
    }
    static constexpr bool isStatefulStatic()
    {
        return true;
    }
};
