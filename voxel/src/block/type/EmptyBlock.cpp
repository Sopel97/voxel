#include "block/type/EmptyBlock.h"

#include "block/BlockResourceLoader.h"

REGISTER_BLOCK_TYPE(EmptyBlock);

EmptyBlock::EmptyBlock(const SharedData& sharedData) :
    m_sharedData(&sharedData)
{

}

BlockSideOpacity EmptyBlock::sideOpacity() const
{
    return BlockSideOpacity::none();
}

std::unique_ptr<Block> EmptyBlock::clone() const
{
    return std::make_unique<EmptyBlock>(*this);
}