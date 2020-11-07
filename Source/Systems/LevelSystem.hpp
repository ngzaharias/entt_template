#pragma once

#include <entt/fwd.hpp>
#include <SFML/Graphics/Texture.hpp>

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
		LevelSystem(physics::PhysicsSystem& physicsSystem);
		~LevelSystem();

		void Initialize(entt::registry& registry);
		void Destroy(entt::registry& registry);

		void Update(entt::registry& registry, const sf::Time& time);

		bool Load(entt::registry& registry);
		void Unload(entt::registry& registry);

	private:
		void CreateEntity(entt::registry& registry, const char* filepath);

	private:
		physics::PhysicsSystem& m_PhysicsSystem;

		std::vector<sf::Texture> m_Textures;

		sf::Texture m_TextureDark;
		sf::Texture m_TextureLight;
	};
};