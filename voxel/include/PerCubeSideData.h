#pragma once

#include "../LibS/Fwd.h"

class CubeSide;

template <class T>
class PerCubeSideData
{
public:
    T east;
    T west;
    T top;
    T bottom;
    T south;
    T north;
};

template <>
class PerCubeSideData<bool>
{
public:
    bool east : 1;
    bool west : 1;
    bool top : 1;
    bool bottom : 1;
    bool south : 1;
    bool north : 1;

    static constexpr PerCubeSideData<bool> none()
    {
        return { false, false, false, false, false, false };
    }
    static constexpr PerCubeSideData<bool> all()
    {
        return { true, true, true, true, true, true };
    }

    static PerCubeSideData<bool> fromJson(const ls::json::Value& config);

    bool get(CubeSide side) const;
    void set(CubeSide side, bool value);
};
