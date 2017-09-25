#pragma once

#include <memory>
#include <iostream>

#include "../LibS/Json.h"

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
