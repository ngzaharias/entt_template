#include "Game.h"

#include "Resources/ResourceManager.h"

#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

namespace
{
	Game* s_Instance = nullptr;
}

Game::Game()
	: m_Registry(nullptr)
	, m_EnttDebugger()
	, m_LevelSystem(m_PhysicsSystem, m_SoundSystem)
	, m_PhysicsSystem()
	, m_RenderSystem()
	, m_SoundSystem()
	, m_ResourceManager(nullptr)
{
	m_ResourceManager = new core::ResourceManager();
	s_Instance = this;
}

Game::~Game()
{
	delete m_ResourceManager;
	s_Instance = nullptr;
}

void Game::Initialise(entt::registry& registry)
{
	m_Registry = &registry;

	// managers
	m_ResourceManager->Initialize();

	// systems
	m_PhysicsSystem.Initialize(registry);
	m_LevelSystem.Initialize(registry);
	m_SoundSystem.Initialize(registry);

	// debuggers
	m_EnttDebugger.Initialize(registry);
}

void Game::Destroy(entt::registry& registry)
{
	// systems
	m_PhysicsSystem.Destroy(registry);
	m_LevelSystem.Destroy(registry);
	m_SoundSystem.Destroy(registry);

	// debuggers
	m_EnttDebugger.Destroy(registry);

	// managers
	m_ResourceManager->Destroy();
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

