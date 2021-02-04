#pragma once

#include <Engine/System.h>

namespace sf
{
	class RenderTexture;
}

namespace editor
{
	class GameWindow final : public core::System
	{
	public:
		GameWindow(sf::RenderTexture& renderTexture);
		~GameWindow();

		void Initialize(entt::registry& registry) override;
		void Destroy(entt::registry& registry) override;

		void Update(entt::registry& registry, const core::GameTime& gameTime) override;
		void Render(entt::registry& registry);

		bool IsVisible() { return m_IsVisible; }
		void SetVisible(const bool value) { m_IsVisible = value; }

	private:
		sf::RenderTexture& m_RenderTexture;

		bool m_IsVisible = true;
	};
}