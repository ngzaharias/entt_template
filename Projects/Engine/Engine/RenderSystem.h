#pragma once

#include <Engine/System.h>

#include <entt/fwd.hpp>

namespace core
{
	class AssetManager;
}

namespace sf
{
	class RenderWindow;
}

namespace render
{
	class RenderSystem : public core::System
	{
	public:
		RenderSystem(sf::RenderWindow& window);
		~RenderSystem();

		void Update(entt::registry& registry, const core::GameTime& gameTime) override;

	private:
		sf::RenderWindow& m_Window;
	};
};