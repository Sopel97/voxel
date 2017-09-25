#pragma once

#include "BlockFactory.h"

class BlockFactoryFactory
{
public:
    virtual std::unique_ptr<BlockFactory> createBlockFactory(const ls::json::Value& config) const = 0;
};

template <class BlockType>
class SpecificBlockFactoryFactory : public BlockFactoryFactory // these will be registered compile time
{
public:
    std::unique_ptr<BlockFactory> createBlockFactory(const ls::json::Value& config) const override
    {
        return std::make_unique<SpecificBlockFactory<BlockType>>(config);
    }
};
