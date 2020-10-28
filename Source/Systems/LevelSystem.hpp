#pragma once

#include <entt/fwd.hpp>

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
		LevelSystem();
		~LevelSystem();

		void Update(entt::registry& registry, const sf::Time& time);

		bool Load(entt::registry& registry);
		void Unload(entt::registry& registry);
	};
};