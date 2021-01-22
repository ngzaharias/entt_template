#pragma once

#include <Engine/System.h>

#include <entt/fwd.hpp>

namespace render
{
	class FlipbookSystem : public core::System
	{
	public:
		FlipbookSystem();
		~FlipbookSystem();

		void Update(entt::registry& registry, const core::GameTime& gameTime) override;
	};
};