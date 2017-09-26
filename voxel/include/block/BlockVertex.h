#pragma once

#include "../LibS/Shapes/Vec2.h"
#include "../LibS/Shapes/Vec3.h"

struct BlockVertex
{
    constexpr BlockVertex() :
        pos(0, 0, 0),
        uv(0, 0)
    {

    }

    constexpr BlockVertex(const ls::Vec3F& pos, const ls::Vec2F& uv) :
        pos(pos),
        uv(uv)
    {

    }

    constexpr BlockVertex(const BlockVertex&) = default;
    constexpr BlockVertex(BlockVertex&&) noexcept = default;
    constexpr BlockVertex& operator=(const BlockVertex&) = default;
    constexpr BlockVertex& operator=(BlockVertex&&) noexcept = default;

    ls::Vec3F pos;
    ls::Vec2F uv;
};
