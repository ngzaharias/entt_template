#pragma once

#include <Editor/Historian.h>

#include <Engine/AssetManager.h>
#include <Engine/System.h>

#include <entt/entity/entity.hpp>
#include <rapidjson/document.h>

namespace editor
{
	class Inspector final : public core::System
	{
	public:
		Inspector(editor::Historian& historian);
		~Inspector();

		void Initialize(entt::registry& registry) override;
		void Destroy(entt::registry& registry) override;

		void Update(entt::registry& registry, const core::GameTime& gameTime) override;

		bool IsVisible() { return m_IsVisible; }

		void SetEntity(const entt::entity value) { m_Entity = value; m_HasChanged = true; }
		void SetVisible(const bool value) { m_IsVisible = value; }

	private:
		void Render(entt::registry& registry);
		void Render_MenuBar(entt::registry& registry);
		void Render_Selected(entt::registry& registry);

	private:
		editor::Historian& m_Historian;

		entt::entity m_Entity = entt::null;
		editor::Record m_Record = { };

		bool m_HasChanged = false;
		bool m_IsVisible = true;
	};
}