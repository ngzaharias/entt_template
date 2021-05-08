#pragma once

#include <Engine/System.h>

namespace sf
{
	class RenderTarget;
}

namespace render
{
	class RenderSystem : public ecs::System
	{
	public:
		RenderSystem(sf::RenderTarget& renderTarget);
		~RenderSystem();

		void Update(const core::GameTime& gameTime) override;

	private:
		sf::RenderTarget& m_RenderTarget;
	};
};