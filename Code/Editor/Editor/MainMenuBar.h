#pragma once

#include <Engine/System.h>

namespace editor
{
	class AssetBrowser;
	class EntityBrowser;
	class GameWindow;
	class Historian;
	class Inspector;

	class MainMenuBar final : public ecs::System
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

		void Initialise() override;
		void Destroy() override;

		void Update(const core::GameTime& gameTime) override;
		void Render();

	private:
		editor::AssetBrowser& m_AssetBrowser;
		editor::EntityBrowser& m_EntityBrowser;
		editor::Historian& m_Historian;
		editor::Inspector& m_Inspector;
	};
}