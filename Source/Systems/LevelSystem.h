#pragma once

#include <Strings/Path.h>

#include <entt/fwd.hpp>

namespace audio
{
	class SoundSystem;
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
	class LevelSystem
	{
	public:
		LevelSystem(physics::PhysicsSystem& physicsSystem, audio::SoundSystem& soundSystem);
		~LevelSystem();

		void Initialize(entt::registry& registry);
		void Destroy(entt::registry& registry);

		void Update(entt::registry& registry, const sf::Time& time);

		bool Load(entt::registry& registry, const str::Path& directory);
		void Unload(entt::registry& registry);

	private:
		entt::entity CreateEntity(entt::registry& registry, const char* filepath);

		void MyFunction();

	private:
		physics::PhysicsSystem& m_PhysicsSystem;
		audio::SoundSystem& m_SoundSystem;
	};
};