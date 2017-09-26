#include "Game.h"

#include <iostream>

#include <SFML/Window.hpp>

#include "GameResourceLoader.h"

Game::Game() :
    m_renderer{}
{
    GameResourceLoader::loadAssets();

    m_map = std::make_unique<Map>();
}


void Game::run()
{
    sf::Clock clock;
    clock.restart();
    float lastTick = 0.0f;
    float lastDraw = 0.0f;
    bool running = true;
    while (running)
    {
        sf::Event event;

        const sf::Time elapsedTime = clock.getElapsedTime();
        const float currentTime = elapsedTime.asSeconds();
        const float dtTick = currentTime - lastTick;
        const float dtDraw = currentTime - lastDraw;

        while (m_renderer.pollWindowEvent(event))
        {
            if (event.type == sf::Event::EventType::Closed)
            {
                running = false;
            }
        }

        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            handleInput(dtDraw);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            running = false;
        }

        if (dtTick >= m_tickTime)
        {
            m_map->update(*this, m_tickTime);
            lastTick = currentTime;
        }

        {
            m_renderer.draw(*this, dtDraw);
            lastDraw = currentTime;
        }
    }
}

Map& Game::map()
{
    return *m_map;
}
const Map& Game::map() const
{
    return *m_map;
}

const ls::gl::Camera& Game::camera() const
{
    return m_renderer.camera();
}
void Game::handleInput(float dt)
{
    m_renderer.handleInput(dt);
}
