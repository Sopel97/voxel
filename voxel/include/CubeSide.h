#pragma once

#include <string>
#include <array>

#include "../LibS/Shapes/Vec3.h"

#include "block/BlockVertex.h"

class CubeSide
{
private:
public:
    enum Value
    {
        East,
        West,
        Top,
        Bottom,
        South,
        North
    };

    static constexpr CubeSide makeEast()
    {
        return CubeSide(East);
    }
    static constexpr CubeSide makeWest()
    {
        return CubeSide(West);
    }
    static constexpr CubeSide makeTop()
    {
        return CubeSide(Top);
    }
    static constexpr CubeSide makeBottom()
    {
        return CubeSide(Bottom);
    }
    static constexpr CubeSide makeSouth()
    {
        return CubeSide(South);
    }
    static constexpr CubeSide makeNorth()
    {
        return CubeSide(North);
    }

    static CubeSide fromString(const std::string& s);

    static constexpr CubeSide fromDirection(const ls::Vec3I& dir)
    {
        if (dir.x > 0) return CubeSide(East);
        if (dir.x < 0) return CubeSide(West);

        if (dir.y > 0) return CubeSide(Top);
        if (dir.y < 0) return CubeSide(Bottom);

        if (dir.z > 0) return CubeSide(South);
        if (dir.z < 0) return CubeSide(North);

        return CubeSide{};
    }

    const std::string& toString();

    constexpr const ls::Vec3I& direction()
    {
        static constexpr std::array<ls::Vec3I, 6> dict{
            ls::Vec3I{ 1, 0, 0 },
            ls::Vec3I{ -1, 0, 0 },
            ls::Vec3I{ 0, 1, 0 },
            ls::Vec3I{ 0, -1, 0 },
            ls::Vec3I{ 0, 0, 1 },
            ls::Vec3I{ 0, 0, -1 }
        };

        return dict[static_cast<int>(m_value)];
    }
    constexpr const std::array<BlockVertex, 4>& faceVertices()
    {
        static const std::array<std::array<BlockVertex, 4>, 6> vertices{
            std::array<BlockVertex, 4>{
                BlockVertex{ { 1.0f, 0.0f, 1.0f },{ 0.0f, 0.0f } },
                BlockVertex{ { 1.0f, 0.0f, 0.0f },{ 1.0f, 0.0f } },
                BlockVertex{ { 1.0f, 1.0f, 0.0f },{ 1.0f, 1.0f } },
                BlockVertex{ { 1.0f, 1.0f, 1.0f },{ 0.0f, 1.0f } }
            },
            std::array<BlockVertex, 4>{
                BlockVertex{ { 0.0f, 0.0f, 0.0f },{ 0.0f, 0.0f } },
                BlockVertex{ { 0.0f, 0.0f, 1.0f },{ 1.0f, 0.0f } },
                BlockVertex{ { 0.0f, 1.0f, 1.0f },{ 1.0f, 1.0f } },
                BlockVertex{ { 0.0f, 1.0f, 0.0f },{ 0.0f, 1.0f } }
            },
            std::array<BlockVertex, 4>{
                BlockVertex{ { 0.0f, 1.0f, 1.0f },{ 0.0f, 0.0f } },
                BlockVertex{ { 1.0f, 1.0f, 1.0f },{ 1.0f, 0.0f } },
                BlockVertex{ { 1.0f, 1.0f, 0.0f },{ 1.0f, 1.0f } },
                BlockVertex{ { 0.0f, 1.0f, 0.0f },{ 0.0f, 1.0f } }
            },
            std::array<BlockVertex, 4>{
                BlockVertex{ { 0.0f, 0.0f, 0.0f },{ 0.0f, 0.0f } },
                BlockVertex{ { 1.0f, 0.0f, 0.0f },{ 1.0f, 0.0f } },
                BlockVertex{ { 1.0f, 0.0f, 1.0f },{ 1.0f, 1.0f } },
                BlockVertex{ { 0.0f, 0.0f, 1.0f },{ 0.0f, 1.0f } }
            },
            std::array<BlockVertex, 4>{
                BlockVertex{ { 0.0f, 0.0f, 1.0f },{ 0.0f, 0.0f } },
                BlockVertex{ { 1.0f, 0.0f, 1.0f },{ 1.0f, 0.0f } },
                BlockVertex{ { 1.0f, 1.0f, 1.0f },{ 1.0f, 1.0f } },
                BlockVertex{ { 0.0f, 1.0f, 1.0f },{ 0.0f, 1.0f } }
            },
            std::array<BlockVertex, 4>{
                BlockVertex{ { 1.0f, 0.0f, 0.0f },{ 0.0f, 0.0f } },
                BlockVertex{ { 0.0f, 0.0f, 0.0f },{ 1.0f, 0.0f } },
                BlockVertex{ { 0.0f, 1.0f, 0.0f },{ 1.0f, 1.0f } },
                BlockVertex{ { 1.0f, 1.0f, 0.0f },{ 0.0f, 1.0f } }
            },
        };

        return vertices[static_cast<int>(m_value)];
    }
    static constexpr const std::array<unsigned, 6>& faceIndices()
    {
        static const std::array<unsigned, 6> indices{ 0, 1, 2, 0, 2, 3 };

        return indices;
    }

    constexpr CubeSide() noexcept :
        m_value(East)
    {

    }
    constexpr CubeSide(const CubeSide&) = default;
    constexpr CubeSide(CubeSide&&) noexcept = default;
    constexpr CubeSide(Value val) :
        m_value(val)
    {
    }

    constexpr CubeSide& operator=(const CubeSide&) = default;
    constexpr CubeSide& operator=(CubeSide&&) = default;
    constexpr CubeSide& operator=(Value val)
    {
        m_value = val;
        return *this;
    }

    constexpr operator Value() const
    {
        return m_value;
    }

private:
    Value m_value;
};
