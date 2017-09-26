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

    m_camera.setNear(0.1f);
    m_camera.setFar(200.0f);
    m_camera.setPosition({ 0, 128.0f, 0 });

    glViewport(0, 0, m_defaultWindowWidth, m_defaultWindowHeight);

    sf::Mouse::setPosition(sf::Vector2i(m_window.getSize().x / 2, m_window.getSize().y / 2), m_window);
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

const ls::gl::Camera& GameRenderer::camera() const
{
    return m_camera;
}

void GameRenderer::handleInput(float dt)
{
    static constexpr float mouseSens = 0.0015f;
    static constexpr float moveSpeed = 16.0f;

    ls::Vec2F moveInput(0, 0);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
    {
        moveInput.y += dt * moveSpeed;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
    {
        moveInput.y -= dt * moveSpeed;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
    {
        moveInput.x -= dt * moveSpeed;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
    {
        moveInput.x += dt * moveSpeed;
    }

    if (moveInput.lengthSquared() > 0.0f)
    {
        const ls::Vec3F forward = m_camera.forward();
        const ls::Vec3F right = m_camera.right();

        m_camera.move(forward * moveInput.y + right * moveInput.x);
    }

    const int windowWidth = m_window.getSize().x;
    const int windowHeight = m_window.getSize().y;
    const ls::Vec2F mousePos(static_cast<float>(sf::Mouse::getPosition(m_window).x), static_cast<float>(sf::Mouse::getPosition(m_window).y));
    const ls::Vec2F windowCenter(windowWidth / 2.0f, windowHeight / 2.0f);
    const ls::Vec2F deltaRot = (mousePos - windowCenter) * mouseSens;
    m_camera.addAngles(ls::Angle2F::radians(deltaRot.x), ls::Angle2F::radians(deltaRot.y));

    sf::Mouse::setPosition(sf::Vector2i(static_cast<int>(windowCenter.x), static_cast<int>(windowCenter.y)), m_window);
}