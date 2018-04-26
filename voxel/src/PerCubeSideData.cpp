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
        res[side] = true;
    }

    return res;
}

bool PerCubeSideData<bool>::operator[](CubeSide side) const
{
    return (m_values >> side.ordinal()) & 1;
}
PerCubeSideData<bool>::Reference PerCubeSideData<bool>::operator[](CubeSide side)
{
    return Reference(m_values, side.ordinal());
}
