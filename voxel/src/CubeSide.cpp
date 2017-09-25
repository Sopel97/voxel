#include "CubeSide.h"

CubeSide CubeSide::fromString(const std::string& s)
{
    switch (s[0])
    {
    case 'E':
        return CubeSide(Value::East);
    case 'W':
        return CubeSide(Value::West);
    case 'T':
        return CubeSide(Value::Top);
    case 'B':
        return CubeSide(Value::Bottom);
    case 'S':
        return CubeSide(Value::South);
    case 'N':
        return CubeSide(Value::North);
    default:
        return CubeSide{};
    }
}

CubeSide CubeSide::fromDirection(const ls::Vec3I& dir)
{
    if (dir.x > 0) return CubeSide(Value::East);
    if (dir.x < 0) return CubeSide(Value::West);

    if (dir.y > 0) return CubeSide(Value::Top);
    if (dir.y < 0) return CubeSide(Value::Bottom);

    if (dir.z > 0) return CubeSide(Value::South);
    if (dir.z < 0) return CubeSide(Value::North);

    return CubeSide{};
}

const std::string& CubeSide::toString()
{
    static const std::array<std::string, 6> dict = {
        "East",
        "West",
        "Top",
        "Bottom",
        "South",
        "North"
    };

    return dict[static_cast<int>(m_value)];
}

const ls::Vec3I& CubeSide::direction()
{
    static const std::array<ls::Vec3I, 6> dict = {
        ls::Vec3I{ 1, 0, 0 },
        ls::Vec3I{ -1, 0, 0 },
        ls::Vec3I{ 0, 1, 0 },
        ls::Vec3I{ 0, -1, 0 },
        ls::Vec3I{ 0, 0, 1 },
        ls::Vec3I{ 0, 0, -1 }
    };

    return dict[static_cast<int>(m_value)];
}

CubeSide::CubeSide(Value val) :
    m_value(val)
{
}

CubeSide& CubeSide::operator=(Value val)
{
    m_value = val;
    return *this;
}

CubeSide::operator Value()
{
    return m_value;
}
CubeSide::operator int()
{
    return static_cast<int>(m_value);
}