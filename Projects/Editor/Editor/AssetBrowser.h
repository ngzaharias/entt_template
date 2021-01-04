#pragma once

#include <Engine/System.h>

namespace core
{
	class AssetManager;
}

namespace editor
{
	class AssetBrowser final : public core::System
	{
	public:
		AssetBrowser(core::AssetManager& assetManager);
		~AssetBrowser();

		void Initialize(entt::registry& registry) override;
		void Destroy(entt::registry& registry) override;

		void Update(entt::registry& registry, const sf::Time& time) override;
		void Render(entt::registry& registry);

		bool IsVisible() { return m_IsVisible; }
		void SetVisible(const bool value) { m_IsVisible = value; }

	private:
		void Render_MenuBar();

		void Import();

	private:
		core::AssetManager& m_AssetManager;

		bool m_IsVisible = true;
	};
}