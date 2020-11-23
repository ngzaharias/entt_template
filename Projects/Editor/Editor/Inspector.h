#pragma once

#include <Engine/System.h>

#include <entt/entity/entity.hpp>

namespace core
{
	class ResourceManager;
}

namespace editor
{
	class Inspector final : public core::System
	{
	public:
		Inspector(core::ResourceManager& resourceManager);
		~Inspector();

		void Initialize(entt::registry& registry) override;
		void Destroy(entt::registry& registry) override;

		void Update(entt::registry& registry, const sf::Time& time) override;
		void Render(entt::registry& registry);

		void SetEntity(const entt::entity value) { m_Entity = value; }
		void SetVisible(const bool value) { m_IsVisible = value; }

	private:
		void Render_Entity(entt::registry& registry);
		void Render_MenuBar(entt::registry& registry);

	private:
		core::ResourceManager& m_ResourceManager;

		entt::entity m_Entity = entt::null;
		bool m_IsVisible = true;
	};
}