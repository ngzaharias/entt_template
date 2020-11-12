#pragma once

#include <Engine/EnttDebugger.h>
#include <Engine/LevelSystem.h>
#include <Engine/PhysicsSystem.h>
#include <Engine/RenderSystem.h>
#include <Engine/SoundSystem.h>

#include <entt/fwd.hpp>

namespace core
{
	class ResourceManager;
}

namespace sf
{
	class RenderWindow;
}

class Map;

class Game final
{
public:
	static Game& Instance();

	Game();
	~Game();

	void Initialise(entt::registry& registry);
	void Destroy(entt::registry& registry);

	void Update(entt::registry& registry, const sf::Time& time);
	void Render(entt::registry& registry, sf::RenderWindow* window);

public:
	entt::registry* m_Registry;

	// managers
	core::ResourceManager* m_ResourceManager;

	// systems
	core::LevelSystem* m_LevelSystem;
	physics::PhysicsSystem* m_PhysicsSystem;
	render::RenderSystem* m_RenderSystem;
	audio::SoundSystem* m_SoundSystem;

	// debug
	debug::EnttDebugger* m_EnttDebugger;
};
