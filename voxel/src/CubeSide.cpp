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

const std::string& CubeSide::toString() const
{
    static const std::array<std::string, 6> dict = {
        "East",
        "West",
        "Top",
        "Bottom",
        "South",
        "North"
    };

    return dict[static_cast<unsigned>(m_value)];
}