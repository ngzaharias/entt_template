#pragma once

#include <Engine/AssetTypes.h>
#include <Engine/System.h>

#include <filesystem>
#include <set>
#include <vector>

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

	struct DirectoryEntry
	{
		bool operator==(const DirectoryEntry& rhs) const;
		bool operator<(const DirectoryEntry& rhs) const;

		str::Name m_Guid;
		str::Path m_Filepath;
		str::String m_Name;
		core::EAssetType m_Type;
		bool m_IsDirectory;
	};

	class AssetBrowser final : public core::System
	{
		using EntrySet = std::set<DirectoryEntry>;
		using Selection = std::vector<int32>;

	public:
		AssetBrowser
		(
			core::AssetManager&
			, editor::FlipbookEditor&
			, editor::SpriteEditor&
			, editor::SpriteExtractor&
		);
		~AssetBrowser();

		void Initialize(entt::registry& registry) override;
		void Destroy(entt::registry& registry) override;

		void Update(entt::registry& registry, const core::GameTime& gameTime) override;

		bool IsVisible() { return m_IsVisible; }
		void SetVisible(const bool value) { m_IsVisible = value; }

	private:
		void Command_ContextMenu();
		void Command_Import();
		void Command_Open(const int32 index);
		void Command_Select(const int32 index);

		void ContextMenu_Common(const Selection& selection);
		void ContextMenu_Sprite(const Selection& selection);
		void ContextMenu_Texture(const Selection& selection);

		void Render(entt::registry& registry);
		void Render_ContextMenu();
		void Render_Entry(const int32 index);
		void Render_MenuBar();

	private:
		core::AssetManager& m_AssetManager;
		editor::FlipbookEditor& m_FlipbookEditor;
		editor::SpriteEditor& m_SpriteEditor;
		editor::SpriteExtractor& m_SpriteExtractor;

		str::Path m_Directory = "Assets";

		EntrySet m_Entries = { };
		Selection m_Selection = { };

		int32 m_Columns = 3;
		bool m_IsVisible = true;
	};
}