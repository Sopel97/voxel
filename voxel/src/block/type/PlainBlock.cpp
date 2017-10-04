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

    texCoords[CubeSide::East] = texture.gridCoordsToTexCoordsF(ls::json::fromJson<ls::Vec2I>(config["eastTexCoords"]));
    texCoords[CubeSide::West] = texture.gridCoordsToTexCoordsF(ls::json::fromJson<ls::Vec2I>(config["westTexCoords"]));
    texCoords[CubeSide::Top] = texture.gridCoordsToTexCoordsF(ls::json::fromJson<ls::Vec2I>(config["topTexCoords"]));
    texCoords[CubeSide::Bottom] = texture.gridCoordsToTexCoordsF(ls::json::fromJson<ls::Vec2I>(config["bottomTexCoords"]));
    texCoords[CubeSide::South] = texture.gridCoordsToTexCoordsF(ls::json::fromJson<ls::Vec2I>(config["southTexCoords"]));
    texCoords[CubeSide::North] = texture.gridCoordsToTexCoordsF(ls::json::fromJson<ls::Vec2I>(config["northTexCoords"]));

    texSize = texture.gridSizeToTexSizeF({ 1, 1 });

    opacity = BlockSideOpacity::fromJson(config["opacity"]);
}
PlainBlock::PlainBlock(const SharedData& sharedData) :
    m_sharedData(&sharedData)
{

}

void PlainBlock::draw(std::vector<BlockVertex>& vertices, std::vector<uint32_t>& indices, const ls::Vec3I& position, BlockSideOpacity outsideOpacity) const
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
void PlainBlock::drawFace(std::vector<BlockVertex>& vertices, std::vector<uint32_t>& indices, const ls::Vec3I& position, CubeSide side) const
{
    static const std::array<unsigned, 6> faceIndices = CubeSide::faceIndices();

    const ls::Vec3F positionF(position);

    const uint32_t lastIndex = static_cast<uint32_t>(vertices.size());
    const auto& face = side.faceVertices();

    for (const auto& v : face)
    {
        vertices.push_back(
            BlockVertex{
                v.pos + positionF,
                m_sharedData->texCoords[static_cast<unsigned>(side)] + m_sharedData->texSize * v.uv
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
