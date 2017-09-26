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

    const ls::gl::Camera& camera() const;

private:
    GameRenderer m_renderer;
    std::unique_ptr<Map> m_map;

    static constexpr float m_tickTime = 1.0f / 20.0f;

    void handleInput(float dt);
};