#include "Game.hpp"

#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

Game* Game::s_Instance = nullptr;

Game::Game()
{
	s_Instance = this;
}

Game::~Game()
{
	s_Instance = nullptr;
}

void Game::Initialise(entt::registry& registry)
{
	m_Registry = &registry;

	m_EnttDebugger.Initialise(registry);
	m_SoundSystem.Initialize(registry);
}

void Game::Destroy(entt::registry& registry)
{
	m_EnttDebugger.Destroy(registry);
	m_SoundSystem.Destroy(registry);
}

void Game::Update(entt::registry& registry, const sf::Time& time)
{
	sf::Time timeScaled = time;

	// #todo: fixed update
	m_LevelSystem.Update(registry, timeScaled);
	m_PhysicsSystem.Update(registry, timeScaled);
	m_SoundSystem.Update(registry, time);

	m_EnttDebugger.Update(registry, time);
}

void Game::Draw(entt::registry& registry, sf::RenderWindow* window)
{
	m_RenderSystem.Update(registry, window);
	m_EnttDebugger.Render(registry);
}

Game& Game::Instance()
{
	return *s_Instance;
}

