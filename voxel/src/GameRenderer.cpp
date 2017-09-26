#include "GameRenderer.h"

#define GLEW_STATIC
#include <GL/glew.h>

#include "Logger.h"

#include "Game.h"

GameRenderer::GameRenderer() :
    m_window(sf::VideoMode(m_defaultWindowWidth, m_defaultWindowHeight), "Voxel", 7u, sf::ContextSettings(24, 8)),
    m_camera(m_defaultFov, static_cast<float>(m_defaultWindowWidth)/static_cast<float>(m_defaultWindowHeight)),
    m_timeSinceLastFpsMeasure(0.0f),
    m_lastMeasuredFps(0),
    m_currentFpsCounter(0)
{
    glewInit();
}

void GameRenderer::draw(Game& game, float dt)
{
    updateFpsMeasures(dt);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    game.map().draw(m_camera);

    m_window.display();
}
bool GameRenderer::pollWindowEvent(sf::Event& event)
{
    while (m_window.pollEvent(event))
    {
        if (event.type == sf::Event::Resized)
        {
            onWindowResized(event);
        }

        return true;
    }
    return false;
}
void GameRenderer::onWindowResized(const sf::Event& e)
{
    glViewport(0, 0, e.size.width, e.size.height);
    m_camera.setAspect(static_cast<float>(e.size.width) / static_cast<float>(e.size.height));
}

void GameRenderer::updateFpsMeasures(float dt)
{
    m_timeSinceLastFpsMeasure += dt;
    ++m_currentFpsCounter;

    if (m_timeSinceLastFpsMeasure >= 1.0f)
    {
        m_lastMeasuredFps = m_currentFpsCounter;
        m_currentFpsCounter = 0;
        m_timeSinceLastFpsMeasure = 0.0f;

        Logger::instance().log(Logger::Priority::Info, std::string("Avg frame time (ms): ") + std::to_string(1000.0 / m_lastMeasuredFps) + " (" + std::to_string(m_lastMeasuredFps) + " fps)");
    }
}