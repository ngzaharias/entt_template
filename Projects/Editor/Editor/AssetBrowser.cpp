#include "Editor/EditorPCH.h"
#include "Editor/AssetBrowser.h"

#include <Engine/ResourceManager.h>

#include <imgui/imgui.h>
#include <imgui/misc/cpp/imgui_stdlib.h>
#include <SFML/System/Time.hpp>

editor::AssetBrowser::AssetBrowser(core::ResourceManager& resourceManager)
	: m_ResourceManager(resourceManager)
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

	Render_MenuBar();

	for (auto&& [guid, entry] : m_ResourceManager.GetEntries())
	{
		ImGui::CollapsingHeader(entry.m_Filepath.ToChar(), ImGuiTreeNodeFlags_Bullet);
	}
	ImGui::End();
}

void editor::AssetBrowser::Render_MenuBar()
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Create"))
		{
			if (ImGui::BeginMenu("Physics\t"))
			{
				// #todo: folder path and unique filename
				if (ImGui::MenuItem("Material"))
					m_ResourceManager.CreateResource<physics::MaterialResource>("Assets\\Temp\\Example.asset");
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}

		if (ImGui::Button("Import..."))
			ImGui::OpenPopup("Import...");
		if (ImGui::BeginPopupModal("Import...", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
		{
			std::string inputPath;
			std::string outputPath;

			// #todo: file browser
			ImGui::Text("Input Path:  ");
			ImGui::SameLine();
			ImGui::InputTextWithHint("##input", "C:\\MyFolder\\MyFile.png", &inputPath);

			ImGui::Text("Output Path: ");
			ImGui::SameLine();
			ImGui::InputTextWithHint("##output", "Assets\\MyFolder\\MyFile.asset", &outputPath);

			//////////////////////////////////////////////////////////////////////////
			ImGui::Separator();
			//////////////////////////////////////////////////////////////////////////

			if (ImGui::Button("OK", ImVec2(120, 0)))
			{
				ImGui::CloseCurrentPopup();
			}
			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(120, 0))) 
				ImGui::CloseCurrentPopup();

			ImGui::EndPopup();
		}

		ImGui::EndMenuBar();
	}
}
