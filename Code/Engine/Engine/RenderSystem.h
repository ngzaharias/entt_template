#pragma once

#include <Engine/System.h>

#include <entt/fwd.hpp>

namespace sf
{
	class RenderTarget;
}

namespace render
{
	class RenderSystem : public core::System
	{
	public:
		RenderSystem(sf::RenderTarget& renderTarget);
		~RenderSystem();

		void Update(entt::registry& registry, const core::GameTime& gameTime) override;

	private:
		sf::RenderTarget& m_RenderTarget;
	};
};