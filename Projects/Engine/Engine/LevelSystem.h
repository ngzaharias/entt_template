#pragma once

#include <Engine/Path.h>
#include <Engine/System.h>

#include <entt/fwd.hpp>

namespace audio
{
	class SoundSystem;
}

namespace physics
{
	class PhysicsManager;
	class PhysicsSystem;
}

namespace sf
{
	class RenderWindow;
	class Time;
}

namespace core
{
	class ResourceManager;

	class LevelSystem final : public core::System
	{
	public:
		LevelSystem
		(
			physics::PhysicsManager& physicsManager
			, core::ResourceManager& resourceManager
		);
		~LevelSystem();

		void Initialize(entt::registry& registry) override;
		void Destroy(entt::registry& registry) override;

		void Update(entt::registry& registry, const sf::Time& time) override;

		bool Load(entt::registry& registry, const str::Path& directory);
		void Unload(entt::registry& registry);

	private:
		entt::entity CreateEntity(entt::registry& registry, const char* filepath);

	private:
		physics::PhysicsManager& m_PhysicsManager;
		core::ResourceManager& m_ResourceManager;
	};
};