#include "Editor/EditorPCH.h"
#include "Editor/AssetBrowser.h"

#include "Editor/FlipbookEditor.h"
#include "Editor/SpriteEditor.h"
#include "Editor/SpriteExtractor.h"

#include <Engine/AssetHelpers.h>
#include <Engine/AssetManager.h>
#include <Engine/FileHelpers.h>
#include <Engine/JsonHelpers.h>
#include <Engine/SortHelpers.h>

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui-sfml/imgui-SFML.h>

// #todo: preview icons
// #todo: select assets
// #todo: multi-select actions
// #todo: import via drag/drop -> inspector
// #todo: renaming of files
// #todo: ensure unique filename
//		- create/rename/import

namespace
{
	const str::String strSounds = "*.ogg";
	const str::String strTextures = "*.png";
	const str::String strAll = strSounds + ";" + strTextures;

	sf::Texture* iconBack = nullptr;
	sf::Texture* iconFile = nullptr;
	sf::Texture* iconFolder = nullptr;

	enum class EResult
	{
		None,
		LeftClick,
		LeftClick_x2,
		RightClick,
	};

	static const std::vector<std::string> s_AssetFilters =
	{
		"All Files (" + strAll + ")", strAll,
		"Sounds (" + strSounds + ")", strSounds,
		"Textures (" + strTextures + ")", strTextures,
	};

	bool MenuBarItem(const char* text, const sf::Texture& texture)
	{
		const ImVec2 itemSize = { 100.f, 40.f };

		ImGui::PushID(text);
		const ImVec2 cursorBefore = ImGui::GetCursorPos();
		const bool result = ImGui::Selectable("", false, 0, itemSize);
		const ImVec2 cursorAfter = ImGui::GetCursorPos();

		// image
		{
			ImGui::SetCursorPosX(cursorBefore.x);
			ImGui::SetCursorPosY(cursorBefore.y + (itemSize.y * 0.5f) - (texture.getSize().y * 0.5f));
			ImGui::Image(texture);
		}

		// text
		{
			const float fontHeight = ImGui::GetFontSize();
			ImGui::SetCursorPosY(cursorBefore.y + (itemSize.y * 0.5f) - (fontHeight * 0.5f));
			ImGui::Text(text);
		}

		ImGui::SetCursorPos(cursorAfter);
		ImGui::PopID();
		return result;
	}
}

bool editor::DirectoryEntry::operator<(const DirectoryEntry& rhs) const
{
	if (m_IsDirectory != rhs.m_IsDirectory)
		return m_IsDirectory;

	return sort::AlphaNumeric(m_Name, rhs.m_Name);
}

editor::AssetBrowser::AssetBrowser
(
	core::AssetManager& assetManager
	, editor::FlipbookEditor& flipbookEditor
	, editor::SpriteEditor& spriteEditor
	, editor::SpriteExtractor& spriteExtractor
)
	: m_AssetManager(assetManager)
	, m_FlipbookEditor(flipbookEditor)
	, m_SpriteEditor(spriteEditor)
	, m_SpriteExtractor(spriteExtractor)
{
}

editor::AssetBrowser::~AssetBrowser()
{
}

void editor::AssetBrowser::Initialize(entt::registry& registry)
{
	iconBack = new sf::Texture();
	iconFile = new sf::Texture();
	iconFolder = new sf::Texture();

	iconBack->loadFromFile("D:\\Projects\\C++\\entt_template\\Projects\\Editor\\Icons\\arrow_left.png");
	iconFile->loadFromFile("D:\\Projects\\C++\\entt_template\\Projects\\Editor\\Icons\\file.png");
	iconFolder->loadFromFile("D:\\Projects\\C++\\entt_template\\Projects\\Editor\\Icons\\folder.png");
}

void editor::AssetBrowser::Destroy(entt::registry& registry)
{
}

void editor::AssetBrowser::Update(entt::registry& registry, const core::GameTime& gameTime)
{
	m_Entries.clear();
	for (const auto& entry : std::filesystem::directory_iterator(m_Directory))
	{
		if (!entry.is_directory() && entry.path().extension() != L".asset")
			return;

		DirectoryEntry directoryEntry = 
		{ 
			entry.path().stem().u8string()
			, entry.path()
			, entry.is_directory() 
		};
		m_Entries.emplace(std::move(directoryEntry));
	}

	Render(registry);
}

void editor::AssetBrowser::Import()
{
	core::SelectFileSettings fileSettings;
	fileSettings.m_Filters = s_AssetFilters;
	fileSettings.m_IsMultiSelect = true;

	core::SelectFolderSettings folderSettings;

	std::vector<str::Path> inputFilepaths = core::SelectFileDialog(fileSettings);
	if (inputFilepaths.empty())
		return;

	str::Path outputFolder = core::SelectFolderDialog(folderSettings);
	if (outputFolder.empty())
		return;

	for (const str::Path& inputFilepath : inputFilepaths)
	{
		str::Path outputFilepath = outputFolder;
		outputFilepath += "\\";
		outputFilepath += inputFilepath.stem().string();
		outputFilepath += ".asset";

		const str::String extension = inputFilepath.extension().string();
		if (str::ContainsAny_NoCase(extension, { ".png" }))
		{
			m_AssetManager.ImportAsset<render::TextureAsset>(inputFilepath, outputFilepath);
		}
		else if (str::ContainsAny_NoCase(extension, { ".ogg" }))
		{
			m_AssetManager.ImportAsset<audio::SoundAsset>(inputFilepath, outputFilepath);
		}

		m_AssetManager.LoadFilepath(outputFilepath, false);
	}
}

void editor::AssetBrowser::Open(const DirectoryEntry& entry)
{
	if (entry.m_IsDirectory)
	{
		m_Directory = entry.m_Filepath;
	}
	else
	{
		rapidjson::Document document;
		json::LoadDocument(entry.m_Filepath, document);

		const char* asset_guid = json::ParseString(document, "asset_guid", nullptr);
		const char* asset_type = json::ParseString(document, "asset_type", nullptr);
		const str::Name assetGuid = str::Name::Create(asset_guid);
		const core::EAssetType assetType = core::ToAssetType(asset_type);

		switch (assetType)
		{
		case core::EAssetType::Flipbook:
			m_FlipbookEditor.OpenEditor(assetGuid);
			break;
		case core::EAssetType::Sprite:
			m_SpriteEditor.OpenEditor(assetGuid);
			break;
		}
	}
}

void editor::AssetBrowser::Render(entt::registry& registry)
{
	if (!m_IsVisible)
		return;

	ImGui::Begin("Asset Browser", &m_IsVisible);
	{
		Render_MenuBar();

		if (ImGui::ImageButton(*iconBack, { 16.f, 16.f }) && !m_Directory.parent_path().empty())
			m_Directory = m_Directory.parent_path();

		ImGui::SameLine();

		ImGui::SetWindowFontScale(1.5f);
		ImGui::Text(m_Directory.string().c_str());
		ImGui::SetWindowFontScale(1.f);

		ImGui::Separator();

		ImGui::Columns(m_Columns, "columns", false);

		for (const DirectoryEntry& entry : m_Entries)
		{
			Render_Entry(entry);
			ImGui::NextColumn();
		}

		ImGui::Columns(1);
	}
	ImGui::End();
}

void editor::AssetBrowser::Render_Entry(const editor::DirectoryEntry& entry)
{
	const ImGuiWindowFlags flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
	const ImVec2 itemSize = { ImGui::GetColumnWidth(), 40.f };
	const float framePadding = 4.f;
	const sf::Texture& texture = entry.m_IsDirectory ? *iconFolder : *iconFile;

	EResult result = EResult::None;
	if (ImGui::BeginChild(entry.m_Name.c_str(), itemSize, false, flags))
	{
		const ImVec2 cursorBefore = ImGui::GetCursorPos();
		ImGui::Selectable("", false, 0, itemSize);
		const ImVec2 cursorAfter = ImGui::GetCursorPos();

		if (ImGui::IsItemHovered())
		{
			if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
				result = EResult::LeftClick;
			if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
				result = EResult::LeftClick_x2;
			if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
				result = EResult::RightClick;
		}

		// image
		{
			ImGui::SetCursorPosX(cursorBefore.x + framePadding);
			ImGui::SetCursorPosY(cursorBefore.y + (itemSize.y * 0.5f) - (texture.getSize().y * 0.5f));
			ImGui::Image(texture);
		}

		// text
		{
			const float fontHeight = ImGui::GetFontSize();
			ImGui::SameLine();
			ImGui::SetCursorPosY(cursorBefore.y + (itemSize.y * 0.5f) - (fontHeight * 0.5f));
			ImGui::Text(entry.m_Name.c_str());
		}

		ImGui::SetCursorPos(cursorAfter);
	}
	ImGui::EndChild();

	switch (result)
	{
	case EResult::LeftClick:
		break;
	case EResult::LeftClick_x2:
		Open(entry);
		break;
	case EResult::RightClick:
		break;
	}
}

void editor::AssetBrowser::Render_MenuBar()
{
	if (ImGui::BeginChild("Menu Bar", { ImGui::GetWindowWidth(), 40.f } ))
	{
		ImGui::GetCurrentWindow()->DC.LayoutType = ImGuiLayoutType_Horizontal;

		if (MenuBarItem("Create", *iconFile))
			ImGui::OpenPopup("Create");

		if (MenuBarItem("Import...", *iconFolder))
			Import();

		if (ImGui::BeginPopup("Create"))
		{
			if (ImGui::BeginMenu("Physics\t"))
			{
				if (ImGui::MenuItem("Material"))
					m_AssetManager.CreateAsset(physics::MaterialAsset{}, "Assets\\Example.asset");
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Render\t"))
			{
				if (ImGui::MenuItem("Flipbook"))
					m_AssetManager.CreateAsset(render::FlipbookAsset{}, "Assets\\Example.asset");
				if (ImGui::MenuItem("Sprite"))
					m_AssetManager.CreateAsset(render::SpriteAsset{}, "Assets\\Example.asset");
				ImGui::EndMenu();
			}
			ImGui::EndPopup();
		}

	}
	ImGui::EndChild();
}