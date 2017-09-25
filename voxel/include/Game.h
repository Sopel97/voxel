#pragma once

#include <vector>
#include <string>

#include "GameRenderer.h"

#include "Map.h"

class Game
{
public:
    Game();

    void run();

    Map& map();
    const Map& map() const;

private:
    GameRenderer m_renderer;
    Map m_map;

    static constexpr float m_tickTime = 1.0f / 20.0f;
};