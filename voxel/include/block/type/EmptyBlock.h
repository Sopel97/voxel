#pragma once

#include "block/Block.h"

#include "../LibS/Shapes/Vec2.h"

#include "CubeSide.h"

#include <array>

class EmptyBlock : public StatelessBlock
{
public:
    using SharedData = BlockSharedData<EmptyBlock>;

    EmptyBlock(const SharedData& sharedData);

    BlockSideOpacity sideOpacity() const override;

    ~EmptyBlock() override = default;

    std::unique_ptr<Block> clone() const override;

private:
    const SharedData* m_sharedData;
};
