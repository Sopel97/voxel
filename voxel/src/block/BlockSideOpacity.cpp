#include "block/BlockSideOpacity.h"

#include "../LibS/Json.h"

#include "CubeSide.h"

BlockSideOpacity BlockSideOpacity::fromJson(const ls::json::Value& config)
{
    BlockSideOpacity opacity = none();
    const size_t size = config.size();
    for (size_t i = 0; i < size; ++i)
    {
        const CubeSide side = CubeSide::fromString(config[i].getString());
        switch (side)
        {
        case CubeSide::East:
            opacity.east = true;
            break;
        case CubeSide::West:
            opacity.west = true;
            break;
        case CubeSide::Top:
            opacity.top = true;
            break;
        case CubeSide::Bottom:
            opacity.bottom = true;
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

    return opacity;
}
