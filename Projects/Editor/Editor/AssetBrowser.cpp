#include "Editor/EditorPCH.h"
#include "Editor/AssetBrowser.h"

#include <Engine/AssetManager.h>
#include <Engine/FileHelpers.h>

#include <imgui/imgui.h>
#include <imgui/misc/cpp/imgui_stdlib.h>
#include <SFML/System/Time.hpp>

namespace
{
	static const str::String strSounds = "*.ogg";
	static const str::String strTextures = "*.png";
	static const str::String strAll = strSounds + ";" + strTextures;

	static const std::vector<std::string> s_AssetFilters =
	{
		"All Files (" + strAll + ")", strAll,
		"Sounds (" + strSounds + ")", strSounds,
		"Textures (" + strTextures + ")", strTextures,
	};
}

editor::AssetBrowser::AssetBrowser(core::AssetManager& assetManager)
	: m_AssetManager(assetManager)
{
}

editor::AssetBrowser::~AssetBrowser()
{
}

void editor::AssetBrowser::Initialize(entt::registry& registry)
{
}

void editor::AssetBrowser::Destroy(entt::registry& registry)
{
}

void editor::AssetBrowser::Update(entt::registry& registry, const sf::Time& time)
{
	Render(registry);
}

void editor::AssetBrowser::Render(entt::registry& registry)
{
	if (!m_IsVisible)
		return;

	ImGui::Begin("Asset Browser", &m_IsVisible, ImGuiWindowFlags_MenuBar);
	{
		Render_MenuBar();

		// #todo: show directories
		// #todo: sort alphabetically
		// #todo: drag/drop -> inspector
		ImGui::Columns(3, "columns", false);
		for (auto&& [guid, entry] : m_AssetManager.GetEntries())
		{
			const str::String filename = str::String(entry.m_Filepath.GetFileNameNoExtension());
			ImGui::CollapsingHeader(filename.c_str(), ImGuiTreeNodeFlags_Bullet);
			ImGui::NextColumn();

		}
		ImGui::Columns(1);
	}
	ImGui::End();
}

void editor::AssetBrowser::Render_MenuBar()
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Create"))
		{
			// #todo: folder path and unique filename
			if (ImGui::BeginMenu("Physics\t"))
			{
				if (ImGui::MenuItem("Material"))
					m_AssetManager.CreateAsset<physics::MaterialAsset>("Assets\\Example.asset");
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Render\t"))
			{
				if (ImGui::MenuItem("Flipbook"))
					m_AssetManager.CreateAsset<render::FlipbookAsset>("Assets\\Example.asset");
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}

		if (ImGui::Button("Import..."))
			Import();

		ImGui::EndMenuBar();
	}
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
	if (outputFolder.IsEmpty())
		return;

	for (const str::Path& inputFilepath : inputFilepaths)
	{
		str::Path outputFilepath = outputFolder;
		outputFilepath += "\\";
		outputFilepath += inputFilepath.GetFileNameNoExtension();
		outputFilepath += ".asset";

		// #todo: check output directory for filename conflict

		const str::StringView& extension = inputFilepath.GetFileExtension();
		if (str::ContainsAny_NoCase(extension, { ".png" }))
		{
			m_AssetManager.ImportAsset<render::TextureAsset>(inputFilepath, outputFilepath);
		}
		else if (str::ContainsAny_NoCase(extension, { ".ogg" }))
		{
			m_AssetManager.ImportAsset<audio::SoundAsset>(inputFilepath, outputFilepath);
		}

		m_AssetManager.LoadFile(outputFilepath.ToChar());
	}
}

