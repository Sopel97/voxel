#include "block/type/PlainBlock.h"

#include "block/BlockResourceLoader.h"
#include "block/BlockVertex.h"

#include "sprite/Spritesheet.h"

#include "CubeSide.h"

#include "../LibS/Json.h"

#include <array>

REGISTER_BLOCK_TYPE(PlainBlock);

PlainBlock::SharedData::SharedData(SpecificBlockFactory<PlainBlock>& blockFactory, const ls::json::Value& config) :
    BlockSharedData<PlainBlock>(blockFactory, config)
{
    const Spritesheet& texture = ResourceManager<Spritesheet>::instance().get("Spritesheet").get();

    texCoords[CubeSide::East] = texture.gridCoordsToTexCoordsF(ls::Vec2I(
        static_cast<int>(config["eastTexCoords"][0].getInt()),
        static_cast<int>(config["eastTexCoords"][1].getInt())
    ));

    texCoords[CubeSide::West] = texture.gridCoordsToTexCoordsF(ls::Vec2I(
        static_cast<int>(config["westTexCoords"][0].getInt()),
        static_cast<int>(config["westTexCoords"][1].getInt())
    ));

    texCoords[CubeSide::Bottom] = texture.gridCoordsToTexCoordsF(ls::Vec2I(
        static_cast<int>(config["bottomTexCoords"][0].getInt()),
        static_cast<int>(config["bottomTexCoords"][1].getInt())
    ));

    texCoords[CubeSide::Top] = texture.gridCoordsToTexCoordsF(ls::Vec2I(
        static_cast<int>(config["topTexCoords"][0].getInt()),
        static_cast<int>(config["topTexCoords"][1].getInt())
    ));

    texCoords[CubeSide::South] = texture.gridCoordsToTexCoordsF(ls::Vec2I(
        static_cast<int>(config["southTexCoords"][0].getInt()),
        static_cast<int>(config["southTexCoords"][1].getInt())
    ));

    texCoords[CubeSide::North] = texture.gridCoordsToTexCoordsF(ls::Vec2I(
        static_cast<int>(config["northTexCoords"][0].getInt()),
        static_cast<int>(config["northTexCoords"][1].getInt())
    ));

    texSize = texture.gridSizeToTexSizeF({ 1, 1 });

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
    if (!outsideOpacity.east)
    {
        drawFace(vertices, indices, position, CubeSide::East);
    }
    if (!outsideOpacity.west)
    {
        drawFace(vertices, indices, position, CubeSide::West);
    }
    if (!outsideOpacity.top)
    {
        drawFace(vertices, indices, position, CubeSide::Top);
    }
    if (!outsideOpacity.bottom)
    {
        drawFace(vertices, indices, position, CubeSide::Bottom);
    }
    if (!outsideOpacity.south)
    {
        drawFace(vertices, indices, position, CubeSide::South);
    }
    if (!outsideOpacity.north)
    {
        drawFace(vertices, indices, position, CubeSide::North);
    }
}
void PlainBlock::drawFace(std::vector<BlockVertex>& vertices, std::vector<unsigned>& indices, const ls::Vec3I& position, CubeSide side) const
{
    static const std::array<unsigned, 6> faceIndices = CubeSide::faceIndices();

    const ls::Vec3F positionF(position);

    const int lastIndex = vertices.size();
    const auto& face = side.faceVertices();

    for (const auto& v : face)
    {
        vertices.push_back(
            BlockVertex{
                v.pos + positionF,
                m_sharedData->texCoords[static_cast<int>(side)] + m_sharedData->texSize * v.uv
            }
        );
    }

    for (const auto& i : faceIndices)
    {
        indices.emplace_back(lastIndex + i);
    }
}
BlockSideOpacity PlainBlock::sideOpacity() const
{
    return m_sharedData->opacity;
}

std::unique_ptr<Block> PlainBlock::clone() const
{
    return std::make_unique<PlainBlock>(*this);
}
