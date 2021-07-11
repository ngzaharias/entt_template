#pragma once

#include <Editor/Historian.h>

#include <Engine/Entity.h>
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

		void SetSelection(const entt::entity value) { m_Selection = value; m_HasChanged = true; }

	private:
		void Render();
		void Render_MenuBar(entt::registry& registry);
		void Render_Selection(entt::registry& registry);

	private:
		editor::Historian& m_Historian;
		editor::SceneEditor& m_SceneEditor;

		editor::Record m_Record = { };

		ecs::Entity m_Selection = entt::null;

		bool m_HasChanged = false;
		bool m_IsVisible = true;
	};
}