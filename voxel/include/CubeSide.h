#pragma once

#include <string>
#include <array>

#include "../LibS/Shapes/Vec3.h"

#include "block/BlockVertex.h"

class CubeSide
{
private:
    enum class Value
    {
        East,
        West,
        Top,
        Bottom,
        South,
        North
    } m_value;

public:
    static constexpr Value East = Value::East;
    static constexpr Value West = Value::West;
    static constexpr Value Top = Value::Top;
    static constexpr Value Bottom = Value::Bottom;
    static constexpr Value South = Value::South;
    static constexpr Value North = Value::North;

    static CubeSide fromString(const std::string& s);

    static CubeSide fromDirection(const ls::Vec3I& dir);

    const std::string& toString();

    const ls::Vec3I& direction();
    const std::array<BlockVertex, 4>& faceVertices();
    static const std::array<unsigned, 6>& faceIndices();

    CubeSide() = default;
    CubeSide(const CubeSide&) = default;
    CubeSide(CubeSide&&) = default;
    CubeSide(Value val);

    CubeSide& operator=(const CubeSide&) = default;
    CubeSide& operator=(CubeSide&&) = default;
    CubeSide& operator=(Value val);

    operator Value();
    explicit operator int();
};
