#pragma once

#include <Engine/System.h>

#include <entt/fwd.hpp>

namespace editor
{
	class AssetBrowser;
	class Inspector;

	class MainMenuBar final : public core::System
	{
	public:
		MainMenuBar(editor::AssetBrowser& assetBrowser, editor::Inspector& inspector);
		~MainMenuBar();

		void Initialize(entt::registry& registry) override;
		void Destroy(entt::registry& registry) override;

		void Update(entt::registry& registry, const sf::Time& time) override;
		void Render(entt::registry& registry);

	private:
		editor::AssetBrowser& m_AssetBrowser;
		editor::Inspector& m_Inspector;
	};
}