#pragma once

class Block;

class BlockContainer
{
public:
    BlockContainer(Block* block);

    BlockContainer();
    BlockContainer(const BlockContainer& other);

    BlockContainer(BlockContainer&& other);

    BlockContainer& operator=(const BlockContainer& other);
    BlockContainer& operator=(BlockContainer&& other);

    const Block& block() const;

    ~BlockContainer();

private:
    Block* m_block;
};
