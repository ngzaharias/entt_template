#pragma once

#include <Engine/GameTime.h>

#include <entt/fwd.hpp>

namespace core
{
	class System
	{
	public:
		virtual void Initialize(entt::registry& registry) { }
		virtual void Destroy(entt::registry& registry) { }

		virtual void Update(entt::registry& registry, const core::GameTime& gameTime) { }
	};
}