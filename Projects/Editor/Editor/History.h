#pragma once

#include <Engine/System.h>

#include <entt/entity/entity.hpp>

namespace editor
{
	class History final : public core::System
	{
	public:
		History();
		~History();

		void Initialize(entt::registry& registry) override;
		void Destroy(entt::registry& registry) override;

		void Update(entt::registry& registry, const sf::Time& time) override;
		void Render(entt::registry& registry);

		void SetVisible(const bool value) { m_IsVisible = value; }

	private:
		bool m_IsVisible = true;
	};
}