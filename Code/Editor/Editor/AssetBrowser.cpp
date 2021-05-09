#include "EditorPCH.h"
#include "Editor/AssetBrowser.h"

#include "Editor/FlipbookEditor.h"
#include "Editor/SpriteEditor.h"
#include "Editor/SpriteExtractor.h"

#include <Engine/AssetHelpers.h>
#include <Engine/AssetManager.h>
#include <Engine/FileHelpers.h>
#include <Engine/JsonHelpers.h>
#include <Engine/JsonTypes.h>
#include <Engine/SortHelpers.h>

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui-sfml/imgui-SFML.h>
#include <SFML/Window/Keyboard.hpp>

// #todo: preview icons
// #todo: drag/drop -> inspector/scene
// #todo: ensure unique filename
// #todo: create/rename/import

namespace
{
	constexpr float s_IconSize = 10.f;
	constexpr float s_RowHeight = 20.f;

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

	static const Array<std::string> s_AssetFilters =
	{
		"All Files (" + strAll + ")", strAll,
		"Sounds (" + strSounds + ")", strSounds,
		"Textures (" + strTextures + ")", strTextures,
	};

	template<typename Type>
	bool Contains(const Array<Type>& container, const Type& value)
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

bool editor::AssetBrowser::Entry::operator==(const Entry& rhs) const
{
	return m_Name == rhs.m_Name;
}

bool editor::AssetBrowser::Entry::operator<(const Entry& rhs) const
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

void editor::AssetBrowser::Initialise()
{
	iconBack = new sf::Texture();
	iconFile = new sf::Texture();
	iconFolder = new sf::Texture();

	iconBack->loadFromFile("Code\\Editor\\Icons\\arrow_left.png");
	iconFile->loadFromFile("Code\\Editor\\Icons\\file.png");
	iconFolder->loadFromFile("Code\\Editor\\Icons\\folder.png");
}

void editor::AssetBrowser::Destroy()
{
}

void editor::AssetBrowser::Update(const core::GameTime& gameTime)
{
	PROFILE_FUNCTION();

	Set<core::Hash> directoryHash;
	for (const auto& entry : std::filesystem::directory_iterator(m_DirectoryPath))
	{
		const str::Path& path = entry.path();
		if (!entry.is_directory() && path.extension() != L".asset")
			break;

		const str::String name = path.stem().u8string();
		const core::Hash hash = str::ToHash(name);
		directoryHash.emplace(hash);
	}

	Set<core::Hash> difference;
	core::Difference(directoryHash, m_DirectoryHash, difference);
	core::Difference(m_DirectoryHash, directoryHash, difference);

	if (!difference.empty())
	{
		m_DirectoryHash = directoryHash;
		Command_ReloadAll();
	}

	Render();
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

	Array<str::Path> inputFilepaths = core::SelectFileDialog(fileSettings);
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
		auto entry = m_DirectoryEntries.begin();
		std::advance(entry, index);

		if (entry->m_IsDirectory)
		{
			m_DirectoryPath = entry->m_Filepath;
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

void editor::AssetBrowser::Command_ReloadAll()
{
	m_DirectoryEntries.clear();
	for (const auto& entry : std::filesystem::directory_iterator(m_DirectoryPath))
	{
		const str::Path& path = entry.path();
		const str::String name = path.stem().u8string();
		if (!entry.is_directory() && path.extension() != L".asset")
			break;

		str::Guid guid = str::strNullGuid;
		core::EAssetType type = core::EAssetType::Unknown;
		if (!entry.is_directory())
		{
			json::Document document;
			json::LoadDocument(entry.path(), document);

			const str::String asset_guid = json::ParseString(document, "asset_guid", { });
			const str::String asset_type = json::ParseString(document, "asset_type", { });

			guid = GUID(asset_guid);
			type = core::ToAssetType(asset_type);
		}

		Entry directoryEntry =
		{
			guid
			, path
			, name
			, type
			, entry.is_directory()
		};
		m_DirectoryEntries.emplace(std::move(directoryEntry));
	}
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
		const str::Guid flipbookGuid = m_AssetManager.CreateAsset(render::FlipbookAsset{}, "Assets\\Example.asset");
		if (flipbookGuid != str::strNullGuid)
		{
			render::FlipbookHandle flipbookHandle = m_AssetManager.LoadAsset<render::FlipbookAsset>(flipbookGuid);
			render::FlipbookAsset& flipbookAsset = flipbookHandle.get();
			for (const int32& index : selection)
			{
				auto entry = m_DirectoryEntries.begin();
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
		auto entry = m_DirectoryEntries.begin();
		std::advance(entry, m_Selection[0]);
		m_SpriteExtractor.OpenDialog(entry->m_Guid);
	}
}

void editor::AssetBrowser::Render()
{
	if (!m_IsVisible)
		return;

	if (ImGui::Begin("Asset Browser", &m_IsVisible))
	{
		Render_MenuBar();

		if (ImGui::ImageButton(*iconBack, { 16.f, 16.f }) && !m_DirectoryPath.parent_path().empty())
			m_DirectoryPath = m_DirectoryPath.parent_path();

		ImGui::SameLine();

		ImGui::SetWindowFontScale(1.5f);
		ImGui::Text(m_DirectoryPath.string().c_str());
		ImGui::SetWindowFontScale(1.f);

		ImGui::Separator();
		ImGui::Separator();

		if (ImGui::BeginChild("Entries"))
		{
			ImGui::BeginTable("Table", 1, ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_Resizable);
			ImGui::TableNextRow();
			//ImGui::TableSetColumnIndex(1);
			//ImGui::PushItemWidth(-FLT_MIN);

			ImGui::TableSetColumnIndex(0);
			ImGui::Text("Name");
			ImGui::Separator();
			ImGui::TableNextRow();

			for (int32 index = 0; index < m_DirectoryEntries.size(); ++index)
			{
				ImGui::TableSetColumnIndex(0);
				Render_Entry(index);
				ImGui::TableNextRow();
			}

			Render_ContextMenu();

			ImGui::EndTable();
		}
		ImGui::EndChild();
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
			auto entry = m_DirectoryEntries.begin();
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
			case core::EAssetType::Template:
				ImGui::TextDisabled("Template Actions");
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
	auto entry = m_DirectoryEntries.begin();
	std::advance(entry, index);

	const ImGuiWindowFlags flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
	const ImVec2 itemSize = { ImGui::GetColumnWidth(), s_RowHeight };
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
			float positionX = cursorBefore.x + framePadding;
			float positionY = cursorBefore.y + (itemSize.y * 0.5f) - (texture.getSize().y * 0.5f);
			positionY += s_IconSize * 0.7f;
			ImGui::SetCursorPosX(positionX);
			ImGui::SetCursorPosY(positionY);
			ImGui::Image(texture, { s_IconSize, s_IconSize });
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