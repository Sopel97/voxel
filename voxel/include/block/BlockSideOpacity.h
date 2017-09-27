#pragma once

struct BlockSideOpacity
{
    bool east : 1;
    bool west : 1;
    bool top : 1;
    bool bottom : 1;
    bool south : 1;
    bool north : 1;

    static constexpr BlockSideOpacity none()
    {
        return { false, false, false, false, false, false };
    }
    static constexpr BlockSideOpacity full()
    {
        return { true, true, true, true, true, true };
    }
};
