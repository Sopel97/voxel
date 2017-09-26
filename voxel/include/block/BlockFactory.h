#pragma once

#include "BlockContainer.h"

#include "../LibS/Json.h"

class BlockFactory
{
public:
    virtual BlockContainer instantiate() const = 0;

    virtual int typeId() const = 0;

    virtual ~BlockFactory() {};

protected:
    int getNextTypeId() const
    {
        static int prevId = -1;

        return ++prevId;
    }
};

template <class BlockType>
class SpecificBlockFactory : public BlockFactory // these will be created runtime from config files
{
public:
    using BlockSharedDataType = typename BlockType::SharedData;

    SpecificBlockFactory(const ls::json::Value& config) : // or some other way of passing the config
        m_typeId(getNextTypeId()),
        m_sharedData(std::make_unique<const BlockSharedDataType>(*this, config)),
        m_singleton(nullptr)
    {
        if (!BlockType::isStatefulStatic())
        {
            m_singleton = std::make_unique<BlockType>(*m_sharedData);
        }
    }

    BlockContainer instantiate() const override
    {
        if (BlockType::isStatefulStatic())
        {
            return BlockContainer(new BlockType(*m_sharedData));
        }
        else
        {
            return BlockContainer(m_singleton.get());
        }
    }

    int typeId() const override
    {
        return m_typeId;
    }

    ~SpecificBlockFactory() override = default;

private:
    int m_typeId;
    std::unique_ptr<const BlockSharedDataType> m_sharedData;
    std::unique_ptr<BlockType> m_singleton;
};
