#include "block/type/PlainBlock.h"

#include "block/BlockResourceLoader.h"

#include "CubeSide.h"

#include "../LibS/Json.h"

REGISTER_BLOCK_TYPE(PlainBlock);

PlainBlock::SharedData::SharedData(SpecificBlockFactory<PlainBlock>& blockFactory, const ls::json::Value& config) :
    BlockSharedData<PlainBlock>(blockFactory, config)
{
    eastTexCoords = ls::Vec2I(
        config["eastTexCoords"][0].getInt(),
        config["eastTexCoords"][1].getInt()
    );

    westTexCoords = ls::Vec2I(
        config["westTexCoords"][0].getInt(),
        config["westTexCoords"][1].getInt()
    );

    bottomTexCoords = ls::Vec2I(
        config["bottomTexCoords"][0].getInt(),
        config["bottomTexCoords"][1].getInt()
    );

    topTexCoords = ls::Vec2I(
        config["topTexCoords"][0].getInt(),
        config["topTexCoords"][1].getInt()
    );

    southTexCoords = ls::Vec2I(
        config["southTexCoords"][0].getInt(),
        config["southTexCoords"][1].getInt()
    );

    northTexCoords = ls::Vec2I(
        config["northTexCoords"][0].getInt(),
        config["northTexCoords"][1].getInt()
    );

    opacity = BlockSideOpacity::none();
    const auto& opacityConfig = config["opacity"];
    const int size = opacityConfig.size();
    for (int i = 0; i < size; ++i)
    {
        CubeSide side = CubeSide::fromString(opacityConfig[i].getString());
        switch (side)
        {
        case CubeSide::East:
            opacity.east = true;
            break;
        case CubeSide::West:
            opacity.west = true;
            break;
        case CubeSide::Bottom:
            opacity.bottom = true;
            break;
        case CubeSide::Top:
            opacity.top = true;
            break;
        case CubeSide::South:
            opacity.south = true;
            break;
        case CubeSide::North:
            opacity.north = true;
            break;
        default:
            break;
        }
    }
}
PlainBlock::PlainBlock(const SharedData& sharedData) :
    m_sharedData(&sharedData)
{

}

void PlainBlock::draw(std::vector<BlockVertex>& vertices, std::vector<unsigned>& indices, const ls::Vec3I& position, BlockSideOpacity outsideOpacity) const
{

}
BlockSideOpacity PlainBlock::sideOpacity() const
{
    return m_sharedData->opacity;
}

std::unique_ptr<Block> PlainBlock::clone() const
{
    return std::make_unique<PlainBlock>(*this);
}
