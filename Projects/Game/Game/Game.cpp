#include "Game/Game.h"

#include <Engine/ResourceManager.h>

#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

namespace
{
	Game* s_Instance = nullptr;
}

Game::Game()
	: m_Registry(nullptr)
	, m_ResourceManager(nullptr)
	, m_LevelSystem(nullptr)
	, m_PhysicsSystem(nullptr)
	, m_RenderSystem(nullptr)
	, m_SoundSystem(nullptr)
	, m_EnttDebugger(nullptr)
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

	// managers
	m_ResourceManager = new core::ResourceManager();

	// systems
	m_PhysicsSystem = new physics::PhysicsSystem();
	m_SoundSystem = new audio::SoundSystem(*m_ResourceManager);
	m_LevelSystem = new core::LevelSystem(*m_ResourceManager, *m_PhysicsSystem, *m_SoundSystem);

	// debuggers
	m_EnttDebugger = new debug::EnttDebugger();

	//////////////////////////////////////////////////////////////////////////
	// managers
	m_ResourceManager->Initialize();

	// systems
	m_PhysicsSystem->Initialize(registry);
	m_SoundSystem->Initialize(registry);
	m_LevelSystem->Initialize(registry);

	// debuggers
	m_EnttDebugger->Initialize(registry);
}

void Game::Destroy(entt::registry& registry)
{
	// debuggers
	m_EnttDebugger->Destroy(registry);

	// systems
	m_LevelSystem->Destroy(registry);
	m_SoundSystem->Destroy(registry);
	m_PhysicsSystem->Destroy(registry);

	// managers
	m_ResourceManager->Destroy();

	//////////////////////////////////////////////////////////////////////////

	delete m_ResourceManager;
	delete m_PhysicsSystem;
	delete m_SoundSystem;
	delete m_LevelSystem;
	delete m_EnttDebugger;
}

void Game::Update(entt::registry& registry, const sf::Time& time)
{
	sf::Time timeScaled = time;

	// #todo: fixed update
	m_LevelSystem->Update(registry, timeScaled);
	m_PhysicsSystem->Update(registry, timeScaled);
	m_SoundSystem->Update(registry, time);

	m_EnttDebugger->Update(registry, time);
}

void Game::Render(entt::registry& registry, sf::RenderWindow* window)
{
	m_RenderSystem->Update(registry, window);
	m_EnttDebugger->Render(registry);
}

Game& Game::Instance()
{
	return *s_Instance;
}

