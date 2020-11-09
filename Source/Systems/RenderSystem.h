#pragma once

#include <entt/fwd.hpp>

namespace sf
{
	class RenderWindow;
}

namespace render
{
	class RenderSystem
	{
	public:
		RenderSystem();
		~RenderSystem();

		void Update(entt::registry& registry, sf::RenderWindow* window);
	};
};