#include "sprite/Spritesheet.h"

Spritesheet::Spritesheet(const std::string& path, const ls::Vec2I& gridSize, const ls::Vec2I& padding) :
    m_texture{},
    m_gridSize(gridSize),
    m_padding(padding)
{
    m_texture.load(path.c_str(), GL_RGBA);
    m_textureSize = textureSize();
}

ls::Vec2I Spritesheet::gridCoordsToTexCoords(const ls::Vec2I& gridCoords) const
{
    return gridCoords * (m_gridSize + m_padding);
}
ls::Vec2I Spritesheet::gridSizeToTexSize(const ls::Vec2I& onGridSize) const
{
    return onGridSize * m_gridSize;
}
ls::Vec2F Spritesheet::gridCoordsToTexCoordsF(const ls::Vec2I& gridCoords) const
{
    return gridCoordsToTexCoords(gridCoords) / m_textureSize;
}
ls::Vec2F Spritesheet::gridSizeToTexSizeF(const ls::Vec2I& onGridSize) const
{
    return gridSizeToTexSize(onGridSize) / m_textureSize;
}
ls::Vec2F Spritesheet::textureSize() const
{
    return ls::Vec2F(static_cast<float>(m_texture.width()), static_cast<float>(m_texture.height()));
}

const ls::gl::Texture2& Spritesheet::texture() const
{
    return m_texture;
}
ls::Vec2I Spritesheet::gridSize() const
{
    return m_gridSize;
}
ls::Vec2I Spritesheet::padding() const
{
    return m_padding;
}
ls::Vec2F Spritesheet::gridSizeF() const
{
    return m_gridSize / m_textureSize;
}
ls::Vec2F Spritesheet::paddingF() const
{
    return m_padding / m_textureSize;
}

void Spritesheet::setRepeated(bool repeated)
{
    if (repeated)
    {
        m_texture.setVerticalWrap(GL_REPEAT);
        m_texture.setHorizontalWrap(GL_REPEAT);
    }
    else
    {
        m_texture.setVerticalWrap(GL_CLAMP_TO_EDGE);
        m_texture.setHorizontalWrap(GL_CLAMP_TO_EDGE);
    }
}