#pragma once

#include <memory>
#include <iostream>

#include "../LibS/Json.h"
#include "../LibS/Shapes/Vec3.h"

class Map;

template <class BlockType>
class SpecificBlockFactory;

template <class BlockType>
struct BlockSharedData
{
    BlockSharedData(SpecificBlockFactory<BlockType>* blockFactory, const ls::json::Value& config) :
        blockFactory(blockFactory)
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
