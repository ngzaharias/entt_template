#pragma once

#include <Engine/System.h>

namespace editor
{
	class Inspector;

	class EntityBrowser final : public core::System
	{
	public:
		EntityBrowser(editor::Inspector& inspector);
		~EntityBrowser();

		void Initialize(entt::registry& registry) override;
		void Destroy(entt::registry& registry) override;

		void Update(entt::registry& registry, const core::GameTime& gameTime) override;

		bool IsVisible() { return m_IsVisible; }
		void SetVisible(const bool value) { m_IsVisible = value; }

	private:
		editor::Inspector& m_Inspector;

		bool m_IsVisible = true;
	};
}