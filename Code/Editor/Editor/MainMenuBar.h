#pragma once

#include <Engine/System.h>

#include <entt/fwd.hpp>

namespace editor
{
	class AssetBrowser;
	class EntityBrowser;
	class GameWindow;
	class Historian;
	class Inspector;

	class MainMenuBar final : public core::System
	{
	public:
		MainMenuBar
		(
			editor::AssetBrowser& assetBrowser
			, editor::EntityBrowser& entityBrowser
			, editor::Historian& historian
			, editor::Inspector& inspector
		);
		~MainMenuBar();

		void Initialize(entt::registry& registry) override;
		void Destroy(entt::registry& registry) override;

		void Update(entt::registry& registry, const core::GameTime& gameTime) override;
		void Render(entt::registry& registry);

	private:
		editor::AssetBrowser& m_AssetBrowser;
		editor::EntityBrowser& m_EntityBrowser;
		editor::Historian& m_Historian;
		editor::Inspector& m_Inspector;
	};
}