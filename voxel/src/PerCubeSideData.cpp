#include "PerCubeSideData.h"

#include "../LibS/Json.h"

#include "CubeSide.h"

PerCubeSideData<bool> PerCubeSideData<bool>::fromJson(const ls::json::Value& config)
{
    PerCubeSideData<bool> res = none();
    const size_t size = config.size();
    for (size_t i = 0; i < size; ++i)
    {
        const CubeSide side = CubeSide::fromString(config[i].getString());
        switch (side)
        {
        case CubeSide::East:
            res.east = true;
            break;
        case CubeSide::West:
            res.west = true;
            break;
        case CubeSide::Top:
            res.top = true;
            break;
        case CubeSide::Bottom:
            res.bottom = true;
            break;
        case CubeSide::South:
            res.south = true;
            break;
        case CubeSide::North:
            res.north = true;
            break;
        default:
            break;
        }
    }

    return res;
}

bool PerCubeSideData<bool>::get(CubeSide side) const
{
    switch (side)
    {
    case CubeSide::East:
        return east;
    case CubeSide::West:
        return west;
    case CubeSide::Top:
        return top;
    case CubeSide::Bottom:
        return bottom;
    case CubeSide::South:
        return south;
    case CubeSide::North:
        return north;
    }
}
void PerCubeSideData<bool>::set(CubeSide side, bool value)
{
    switch (side)
    {
    case CubeSide::East:
        east = value;
        break;
    case CubeSide::West:
        west = value;
        break;
    case CubeSide::Top:
        top = value;
        break;
    case CubeSide::Bottom:
        bottom = value;
        break;
    case CubeSide::South:
        south = value;
        break;
    case CubeSide::North:
        north = value;
        break;
    }
}
