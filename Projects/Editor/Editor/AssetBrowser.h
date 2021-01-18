#pragma once

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

	struct DirectoryEntry
	{
		bool operator<(const DirectoryEntry& rhs) const;

		str::String m_Name;
		str::Path m_Filepath;
		bool m_IsDirectory;
	};

	class AssetBrowser final : public core::System
	{
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
		void Import();
		void Open(const DirectoryEntry& entry);

		void Render(entt::registry& registry);
		void Render_Entry(const DirectoryEntry& entry);
		void Render_MenuBar();

	private:
		core::AssetManager& m_AssetManager;
		editor::FlipbookEditor& m_FlipbookEditor;
		editor::SpriteEditor& m_SpriteEditor;
		editor::SpriteExtractor& m_SpriteExtractor;

		str::Path m_Directory = "Assets";

		std::set<DirectoryEntry> m_Entries;

		int32 m_Columns = 3;
		bool m_IsVisible = true;
	};
}