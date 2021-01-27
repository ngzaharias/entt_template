#pragma once

#include <Engine/System.h>

#include <entt/entity/entity.hpp>

namespace editor
{
	class Inspector;

	struct EntityEntry
	{
		bool operator==(const EntityEntry& rhs) const;
		bool operator<(const EntityEntry& rhs) const;

		entt::entity m_Entity = entt::null;
		str::String m_Name = { };
	};

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
		void Render(entt::registry& registry);

		void Command_CreateCamera(entt::registry& registry);
		void Command_CreateEmpty(entt::registry& registry);
		void Command_CreateFlipbook(entt::registry& registry);
		void Command_CreateSprite(entt::registry& registry);

	private:
		editor::Inspector& m_Inspector;

		std::set<EntityEntry> m_Entries;
		str::String m_Filter;
		bool m_IsVisible = true;
	};
}