#pragma once

#include "EnttDebugger.h"
#include "Systems/LevelSystem.h"
#include "Systems/PhysicsSystem.h"
#include "Systems/RenderSystem.h"
#include "Systems/SoundSystem.h"

#include <entt/fwd.hpp>

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
	void Draw(entt::registry& registry, sf::RenderWindow* window);

public:
	entt::registry* m_Registry;

	debug::EnttDebugger m_EnttDebugger;
	core::LevelSystem m_LevelSystem;
	physics::PhysicsSystem m_PhysicsSystem;
	render::RenderSystem m_RenderSystem;
	audio::SoundSystem m_SoundSystem;
};
