#pragma once

#include <SFML/Window.hpp>

class Game;

class GameRenderer
{
public:
    GameRenderer();

    void draw(Game& game, float dt);

    bool pollWindowEvent(sf::Event& event);

private:
    sf::Window m_window;
    float m_timeSinceLastFpsMeasure;
    int m_lastMeasuredFps;
    int m_currentFpsCounter;

    static constexpr int m_defaultWindowWidth = 1024;
    static constexpr int m_defaultWindowHeight = 768;

    void onWindowResized(const sf::Event& e);
    void updateFpsMeasures(float dt);
};