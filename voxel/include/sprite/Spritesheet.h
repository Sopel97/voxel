#pragma once

#include <string>

#include "../LibS/Shapes/Vec2.h"
#include "../LibS/Json.h"
#include "../LibS/OpenGL/Texture2.h"

class Spritesheet
{
public:
    Spritesheet(const std::string& path, const ls::Vec2I& gridSize, const ls::Vec2I& padding);

    ls::Vec2I gridCoordsToTexCoords(const ls::Vec2I& gridCoords) const;
    ls::Vec2I gridSizeToTexSize(const ls::Vec2I& onGridSize) const;
    ls::Vec2F gridCoordsToTexCoordsF(const ls::Vec2I& gridCoords) const;
    ls::Vec2F gridSizeToTexSizeF(const ls::Vec2I& onGridSize) const;

    const ls::gl::Texture2& texture() const;
    ls::Vec2I gridSize() const;
    ls::Vec2F gridSizeF() const;
    ls::Vec2I padding() const;
    ls::Vec2F paddingF() const;

    void setRepeated(bool repeated = true);

private:
    ls::gl::Texture2 m_texture;
    ls::Vec2I m_gridSize;
    ls::Vec2I m_padding;
    ls::Vec2F m_textureSize;

    ls::Vec2F textureSize() const;
};