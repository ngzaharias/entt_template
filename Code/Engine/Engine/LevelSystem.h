#pragma once

#include <Engine/Path.h>
#include <Engine/System.h>

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
	class AssetManager;

	class LevelSystem final : public ecs::System
	{
	public:
		LevelSystem(
			core::AssetManager& assetManager, 
			physics::PhysicsManager& physicsManager);

		void Update(const core::GameTime& gameTime) override;

		bool Load(const str::Path& directory);
		void Unload();

	private:
		ecs::Entity CreateEntity(entt::registry& registry, const str::Path& filepath);

	private:
		core::AssetManager& m_AssetManager;
		physics::PhysicsManager& m_PhysicsManager;
	};
};