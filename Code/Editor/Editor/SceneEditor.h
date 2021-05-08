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
	public:
		SceneEditor(sf::RenderTexture& renderTexture);
		~SceneEditor();

		void Initialise() override;
		void Destroy() override;

		void Update(const core::GameTime& gameTime) override;
		void Render();

		bool IsVisible() { return m_IsVisible; }
		void SetVisible(const bool value) { m_IsVisible = value; }

	private:
		sf::RenderTexture& m_RenderTexture;

		bool m_IsVisible = true;
	};
}