#pragma once

#include <Engine/Path.h>
#include <Engine/System.h>

#include <entt/fwd.hpp>

namespace audio
{
	class SoundSystem;
}

namespace core
{
	class ResourceManager;
}

namespace physics
{
	class PhysicsSystem;
}

namespace sf
{
	class RenderWindow;
	class Time;
}

namespace core
{
	class LevelSystem final : public core::System
	{
	public:
		LevelSystem
		(
			core::ResourceManager& resourceManager
			, physics::PhysicsSystem& physicsSystem
			, audio::SoundSystem& soundSystem
		);
		~LevelSystem();

		void Initialize(entt::registry& registry) override;
		void Destroy(entt::registry& registry) override;

		void Update(entt::registry& registry, const sf::Time& time) override;

		bool Load(entt::registry& registry, const str::Path& directory);
		void Unload(entt::registry& registry);

	private:
		entt::entity CreateEntity(entt::registry& registry, const char* filepath);

		void MyFunction();

	private:
		core::ResourceManager& m_ResourceManager;

		physics::PhysicsSystem& m_PhysicsSystem;
		audio::SoundSystem& m_SoundSystem;
	};
};