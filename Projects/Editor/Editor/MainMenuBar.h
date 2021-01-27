#pragma once

#include <Engine/System.h>

#include <entt/fwd.hpp>

namespace debug
{
	class EnttDebugger;
}

namespace editor
{
	class AssetBrowser;
	class EntityBrowser;
	class GameWindow;
	class History;
	class Inspector;

	class MainMenuBar final : public core::System
	{
	public:
		MainMenuBar
		(
			debug::EnttDebugger& enttDebugger
			, editor::AssetBrowser& assetBrowser
			, editor::EntityBrowser& entityBrowser
			, editor::GameWindow& gameWindow
			, editor::History& history
			, editor::Inspector& inspector
		);
		~MainMenuBar();

		void Initialize(entt::registry& registry) override;
		void Destroy(entt::registry& registry) override;

		void Update(entt::registry& registry, const core::GameTime& gameTime) override;
		void Render(entt::registry& registry);

	private:
		debug::EnttDebugger& m_EnttDebugger;
		editor::AssetBrowser& m_AssetBrowser;
		editor::EntityBrowser& m_EntityBrowser;
		editor::GameWindow& m_GameWindow;
		editor::History& m_History;
		editor::Inspector& m_Inspector;
	};
}