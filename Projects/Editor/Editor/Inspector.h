#pragma once

#include <Engine/System.h>

#include <entt/entity/entity.hpp>

namespace editor
{
	class Inspector final : public core::System
	{
	public:
		Inspector();
		~Inspector();

		void Initialize(entt::registry& registry) override;
		void Destroy(entt::registry& registry) override;

		void Update(entt::registry& registry, const sf::Time& time) override;
		void Render(entt::registry& registry);

		void SetEntity(const entt::entity value) { m_Entity = value; }
		void SetVisible(const bool value) { m_IsVisible = value; }

	private:
		void Render_MenuBar(entt::registry& registry);
		void Render_Selected(entt::registry& registry);

	private:
		entt::entity m_Entity = entt::null;
		bool m_IsVisible = true;
	};
}