#pragma once

#include <Editor/Historian.h>

#include <Engine/System.h>

namespace editor
{
	class Historian;
	class SceneEditor;

	class Inspector final : public ecs::System
	{

	public:
		Inspector(
			editor::Historian& historian,
			editor::SceneEditor& sceneEditor);

		void Update(const core::GameTime& gameTime) override;

		bool IsVisible() { return m_IsVisible; }
		void SetVisible(const bool value) { m_IsVisible = value; }

	private:
		void Render();
		void Render_MenuBar(entt::registry& registry);

	private:
		editor::Historian& m_Historian;
		editor::SceneEditor& m_SceneEditor;

		editor::Record m_Record = { };

		bool m_IsVisible = true;
	};
}