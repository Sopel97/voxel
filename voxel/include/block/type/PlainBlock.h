#pragma once

#include "block/Block.h"

#include "../LibS/Shapes/Vec2.h"

class PlainBlock : public StatelessBlock
{
public:
    class SharedData : public BlockSharedData<PlainBlock>
    {
    public:
        SharedData(SpecificBlockFactory<PlainBlock>& blockFactory, const ls::json::Value& config);

        ls::Vec2I eastTexCoords;
        ls::Vec2I westTexCoords;
        ls::Vec2I bottomTexCoords;
        ls::Vec2I topTexCoords;
        ls::Vec2I southTexCoords;
        ls::Vec2I northTexCoords;

        BlockSideOpacity opacity;
    };

    PlainBlock(const SharedData& sharedData);

    void draw(std::vector<BlockVertex>& vertices, std::vector<unsigned>& indices, const ls::Vec3I& position, BlockSideOpacity outsideOpacity) const override;
    BlockSideOpacity sideOpacity() const override;

    ~PlainBlock() override = default;

    std::unique_ptr<Block> clone() const override;

private:
    const SharedData* m_sharedData;
};
