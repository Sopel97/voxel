#pragma once

#include <vector>
#include <string>

#include "GameRenderer.h"

class Game
{
public:
    Game();

    void run();

private:
    GameRenderer m_renderer;

    static constexpr float m_tickTime = 1.0f / 20.0f;
};