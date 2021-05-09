#pragma once

#include <Engine/AssetTypes.h>
#include <Engine/Hash.h>
#include <Engine/System.h>

#include <filesystem>

namespace core
{
	struct AssetEntry;

	class AssetManager;
}

namespace editor
{
	class FlipbookEditor;
	class SpriteEditor;
	class SpriteExtractor;

	class AssetBrowser final : public ecs::System
	{
		struct Entry
		{
			bool operator==(const Entry& rhs) const;
			bool operator<(const Entry& rhs) const;

			str::Name m_Guid;
			str::Path m_Filepath;
			str::String m_Name;
			core::EAssetType m_Type;
			bool m_IsDirectory;
		};

		using Selection = Array<int32>;

	public:
		AssetBrowser
		(
			core::AssetManager&
			, editor::FlipbookEditor&
			, editor::SpriteEditor&
			, editor::SpriteExtractor&
		);
		~AssetBrowser();

		void Initialise() override;
		void Destroy() override;

		void Update(const core::GameTime& gameTime) override;

		bool IsVisible() { return m_IsVisible; }
		void SetVisible(const bool value) { m_IsVisible = value; }

	private:
		void Command_ContextMenu();
		void Command_Import();
		void Command_Open();
		void Command_ReloadAll();
		void Command_Select(const int32 index);

		void ContextMenu_Common(const Selection& selection);
		void ContextMenu_Sprite(const Selection& selection);
		void ContextMenu_Texture(const Selection& selection);

		void Render();
		void Render_ContextMenu();
		void Render_Entry(const int32 index);
		void Render_MenuBar();


	private:
		core::AssetManager& m_AssetManager;
		editor::FlipbookEditor& m_FlipbookEditor;
		editor::SpriteEditor& m_SpriteEditor;
		editor::SpriteExtractor& m_SpriteExtractor;

		str::Path m_DirectoryPath = "Assets";
		Set<Entry> m_DirectoryEntries = { };
		Set<core::Hash> m_DirectoryHash = { };

		Selection m_Selection = { };

		bool m_IsVisible = true;
	};
}