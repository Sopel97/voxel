#pragma once

#include "block/Block.h"

#include "../LibS/Shapes/Vec2.h"

#include "CubeSide.h"

#include <array>

class PlainBlock : public StatelessBlock
{
public:
    class SharedData : public BlockSharedData<PlainBlock>
    {
    public:
        SharedData(SpecificBlockFactory<PlainBlock>& blockFactory, const ls::json::Value& config);

        std::array<ls::Vec2F, 6> texCoords;
        ls::Vec2F texSize;

        BlockSideOpacity opacity;
    };

    PlainBlock(const SharedData& sharedData);

    void draw(std::vector<BlockVertex>& vertices, std::vector<unsigned>& indices, const ls::Vec3I& position, BlockSideOpacity outsideOpacity) const override;
    BlockSideOpacity sideOpacity() const override;

    ~PlainBlock() override = default;

    std::unique_ptr<Block> clone() const override;

private:
    const SharedData* m_sharedData;

    void drawFace(std::vector<BlockVertex>& vertices, std::vector<unsigned>& indices, const ls::Vec3I& position, CubeSide side) const;
};
