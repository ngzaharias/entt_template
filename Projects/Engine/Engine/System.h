#pragma once

#include <entt/fwd.hpp>

namespace sf
{
	class Time;
}

namespace core
{
	class System
	{
	public:
		virtual void Initialize(entt::registry& registry) { }
		virtual void Destroy(entt::registry& registry) { }

		virtual void Update(entt::registry& registry, const sf::Time& time) { }
	};
}