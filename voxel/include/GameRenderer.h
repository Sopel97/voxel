#pragma once

#include <SFML/Window.hpp>

#include "../LibS/OpenGL.h"

class Game;

class GameRenderer
{
public:
    GameRenderer();

    void draw(Game& game, float dt);

    bool pollWindowEvent(sf::Event& event);

    const ls::gl::Camera& camera() const;

    void handleInput(float dt);

private:
    sf::Window m_window;
    ls::gl::Camera m_camera;
    float m_timeSinceLastFpsMeasure;
    int m_lastMeasuredFps;
    int m_currentFpsCounter;

    static constexpr int m_defaultWindowWidth = 1024;
    static constexpr int m_defaultWindowHeight = 768;
    static constexpr float m_defaultFov = 45.0f;

    void onWindowResized(const sf::Event& e);
    void updateFpsMeasures(float dt);
};