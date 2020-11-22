#pragma once

#include <Engine/System.h>

#include <entt/fwd.hpp>

namespace core
{
	class ResourceManager;
}

namespace sf
{
	class Time;
}

namespace editor
{
	class AssetBrowser final : public core::System
	{
	public:
		AssetBrowser(core::ResourceManager& resourceManager);
		~AssetBrowser();

		void Initialize(entt::registry& registry) override;
		void Destroy(entt::registry& registry) override;

		void Update(entt::registry& registry, const sf::Time& time) override;
		void Render(entt::registry& registry);

		void SetVisible(const bool value) { m_IsVisible = value; }

	private:
		void Render_MenuBar();

	private:
		core::ResourceManager& m_ResourceManager;

		bool m_IsVisible = true;
	};
}