#include "block/BlockContainer.h"

#include "block/Block.h"

BlockContainer::BlockContainer(Block* block) :
    m_block(block)
{

}

BlockContainer::BlockContainer() :
    m_block(nullptr)
{

}
BlockContainer::BlockContainer(const BlockContainer& other)
{
    if (other.m_block && other.m_block->isStateful())
    {
        m_block = other.m_block->clone().release();
    }
    else
    {
        m_block = other.m_block;
    }
}

BlockContainer::BlockContainer(BlockContainer&& other) :
    m_block(other.m_block)
{
    other.m_block = nullptr;
}

BlockContainer& BlockContainer::operator=(const BlockContainer& other)
{
    if (m_block && m_block->isStateful())
    {
        delete m_block;
        m_block = other.m_block->clone().release();
    }
    else
    {
        m_block = other.m_block;
    }

    return *this;
}
BlockContainer& BlockContainer::operator=(BlockContainer&& other)
{
    m_block = other.m_block;
    other.m_block = nullptr;

    return *this;
}

const Block& BlockContainer::block() const
{
    return *m_block;
}

BlockContainer::~BlockContainer()
{
    if (m_block && m_block->isStateful())
    {
        delete m_block;
    }
}
