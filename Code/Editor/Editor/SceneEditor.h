#pragma once

#include <Engine/System.h>

namespace sf
{
	class RenderTexture;
}

namespace editor
{
	class SceneEditor final : public ecs::System
	{
		using Selection = ecs::Entity;

	public:
		SceneEditor(sf::RenderTexture& renderTexture);

		void Update(const core::GameTime& gameTime) override;

		bool IsVisible() { return m_IsVisible; }
		void SetVisible(const bool value) { m_IsVisible = value; }

	private:
		void Render();
	private:
		sf::RenderTexture& m_RenderTexture;

		bool m_IsVisible = true;
	};
}