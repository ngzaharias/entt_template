#include "EditorPCH.h"
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
#include <SFML/Window/Keyboard.hpp>

// #todo: preview icons
// #todo: import via drag/drop -> inspector
// #todo: renaming of files
// #todo: ensure unique filename
//		- create/rename/import
// #todo: fill guid into directory entry
// #todo: refresh entries only on directory change

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

	template<typename Type>
	bool Contains(const std::vector<Type>& container, const Type& value)
	{
		return std::find(container.begin(), container.end(), value) != container.end();
	}

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

bool editor::DirectoryEntry::operator==(const DirectoryEntry& rhs) const
{
	return m_Name == rhs.m_Name;
}

bool editor::DirectoryEntry::operator<(const DirectoryEntry& rhs) const
{
	if (m_IsDirectory != rhs.m_IsDirectory)
		return m_IsDirectory;

	return sort::AlphaNumeric(m_Name, rhs.m_Name) < 0;
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

	iconBack->loadFromFile("Code\\Editor\\Icons\\arrow_left.png");
	iconFile->loadFromFile("Code\\Editor\\Icons\\file.png");
	iconFolder->loadFromFile("Code\\Editor\\Icons\\folder.png");
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
			break;

		str::Name assetGuid = str::strNullGuid;
		core::EAssetType assetType = core::EAssetType::Unknown;

		if (!entry.is_directory())
		{
			// #todo: don't load documents every frame
			// #fixme: loading texture assets is super slow
			rapidjson::Document document;
			json::LoadDocument(entry.path(), document);

			const char* asset_guid = json::ParseString(document, "asset_guid", nullptr);
			const char* asset_type = json::ParseString(document, "asset_type", nullptr);

			assetGuid = NAME(asset_guid);
			assetType = core::ToAssetType(asset_type);
		}

		DirectoryEntry directoryEntry =
		{
			assetGuid
			, entry.path()
			, entry.path().stem().u8string()
			, assetType
			, entry.is_directory() 
		};
		m_Entries.emplace(std::move(directoryEntry));
	}

	Render(registry);
}

void editor::AssetBrowser::Command_ContextMenu()
{
	ImGui::OpenPopup("ContextMenu");
}

void editor::AssetBrowser::Command_Import()
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

void editor::AssetBrowser::Command_Open()
{
	for (const auto& index : m_Selection)
	{
		auto entry = m_Entries.begin();
		std::advance(entry, index);

		if (entry->m_IsDirectory)
		{
			m_Directory = entry->m_Filepath;
		}
		else
		{
			switch (entry->m_Type)
			{
			case core::EAssetType::Flipbook:
				m_FlipbookEditor.OpenEditor(entry->m_Guid);
				break;
			case core::EAssetType::Sprite:
				m_SpriteEditor.OpenEditor(entry->m_Guid);
				break;
			}
		}
	}
	m_Selection.clear();
}

void editor::AssetBrowser::Command_Select(const int32 index)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) && !m_Selection.empty())
	{
		const int32 first = std::min(index, m_Selection.back());
		const int32 last = std::max(index, m_Selection.back());

		m_Selection.pop_back();
		for (int32 i = first; i <= last; ++i)
			m_Selection.push_back(i);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
	{
		m_Selection.push_back(index);
	}
	else
	{
		m_Selection.clear();
		m_Selection.push_back(index);
	}
}

void editor::AssetBrowser::ContextMenu_Common(const Selection& selection)
{
	ImGui::TextDisabled("Common Actions");
}

void editor::AssetBrowser::ContextMenu_Sprite(const Selection& selection)
{
	ImGui::TextDisabled("Sprite Actions");
	if (ImGui::MenuItem("Create Flipbook..."))
	{
		const str::Name flipbookGuid = m_AssetManager.CreateAsset(render::FlipbookAsset{}, "Assets\\Example.asset");
		if (flipbookGuid != str::strNullGuid)
		{
			render::FlipbookHandle flipbookHandle = m_AssetManager.LoadAsset<render::FlipbookAsset>(flipbookGuid);
			render::FlipbookAsset& flipbookAsset = flipbookHandle.get();
			for (const int32& index : selection)
			{
				auto entry = m_Entries.begin();
				std::advance(entry, index);

				render::SpriteHandle spriteHandle = m_AssetManager.LoadAsset<render::SpriteAsset>(entry->m_Guid);
				flipbookAsset.m_Frames.push_back({ spriteHandle, 1 });
			}
		}
	}
}

void editor::AssetBrowser::ContextMenu_Texture(const Selection& selection)
{
	ImGui::TextDisabled("Texture Actions");
	if (ImGui::MenuItem("Extract Sprites..."))
	{
		auto entry = m_Entries.begin();
		std::advance(entry, m_Selection[0]);
		m_SpriteExtractor.OpenDialog(entry->m_Guid);
	}
}

void editor::AssetBrowser::Render(entt::registry& registry)
{
	if (!m_IsVisible)
		return;

	if (ImGui::Begin("Asset Browser", &m_IsVisible))
	{
		Render_MenuBar();

		if (ImGui::ImageButton(*iconBack, { 16.f, 16.f }) && !m_Directory.parent_path().empty())
			m_Directory = m_Directory.parent_path();

		ImGui::SameLine();

		ImGui::SetWindowFontScale(1.5f);
		ImGui::Text(m_Directory.string().c_str());
		ImGui::SetWindowFontScale(1.f);

		ImGui::Separator();

		ImGui::BeginTable("Table", m_Columns);
		ImGui::TableNextRow();
		ImGui::TableNextColumn();

		for (int32 index = 0; index < m_Entries.size(); ++index)
		{
			Render_Entry(index);
			ImGui::TableNextColumn();
		}

		Render_ContextMenu();

		ImGui::EndTable();
	}
	ImGui::End();
}

void editor::AssetBrowser::Render_ContextMenu()
{
	if (ImGui::BeginPopup("ContextMenu"))
	{
		bool isDirectory = false;
		core::EAssetType assetType = core::EAssetType::Unknown;
		for (const auto& index : m_Selection)
		{
			auto entry = m_Entries.begin();
			std::advance(entry, index);

			if (assetType != core::EAssetType::Unknown && entry->m_Type != assetType)
			{
				assetType = core::EAssetType::Unknown;
				break;
			}
			assetType = entry->m_Type;
		}

		if (isDirectory)
		{
			ImGui::TextDisabled("Directory Actions");
		}
		else
		{
			switch (assetType)
			{
			case core::EAssetType::EntityTemplate:
				ImGui::TextDisabled("Template Actions");
				break;
			case core::EAssetType::Flipbook:
				ImGui::TextDisabled("Flipbook Actions");
				break;
			case core::EAssetType::PhysicsMaterial:
				ImGui::TextDisabled("Physics Material Actions");
				break;
			case core::EAssetType::Sound:
				ImGui::TextDisabled("Sound Actions");
				break;
			case core::EAssetType::Sprite:
				ContextMenu_Sprite(m_Selection);
				break;
			case core::EAssetType::Texture:
				ContextMenu_Texture(m_Selection);
				break;
			}

			ContextMenu_Common(m_Selection);
		}
		ImGui::EndPopup();
	}
}

void editor::AssetBrowser::Render_Entry(const int32 index)
{
	auto entry = m_Entries.begin();
	std::advance(entry, index);

	const ImGuiWindowFlags flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
	const ImVec2 itemSize = { ImGui::GetColumnWidth(), 40.f };
	const float framePadding = 4.f;
	const sf::Texture& texture = entry->m_IsDirectory ? *iconFolder : *iconFile;

	EResult result = EResult::None;
	if (ImGui::BeginChild(entry->m_Name.c_str(), itemSize, false, flags))
	{
		const bool isSelected = std::find_if(m_Selection.begin(), m_Selection.end(),
			[&](const auto& selected) -> bool { return selected == index; }) != m_Selection.end();

		const ImVec2 cursorBefore = ImGui::GetCursorPos();
		ImGui::Selectable("", isSelected, 0, itemSize);
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
			ImGui::Text(entry->m_Name.c_str());
		}

		ImGui::SetCursorPos(cursorAfter);
	}
	ImGui::EndChild();

	switch (result)
	{
	case EResult::LeftClick:
		Command_Select(index);
		break;
	case EResult::LeftClick_x2:
		if (!Contains(m_Selection, index))
			Command_Select(index);
		Command_Open();
		break;
	case EResult::RightClick:
		if (!Contains(m_Selection, index))
			Command_Select(index);
		Command_ContextMenu();
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
			Command_Import();

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