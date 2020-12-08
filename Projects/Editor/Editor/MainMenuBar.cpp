#include "Editor/MainMenuBar.h"

#include "Editor/AssetBrowser.h"
#include "Editor/History.h"
#include "Editor/Inspector.h"

#include <imgui/imgui.h>
#include <SFML/System/Time.hpp>

namespace
{
	const ImVec4 s_Black = ImVec4(0.f, 0.f, 0.f, 1.f);
	const ImVec4 s_Gray = ImVec4(0.09f, 0.09f, 0.09f, 1.f);
	const ImVec4 s_White = ImVec4(1.f, 1.f, 1.f, 1.f);

	const ImVec4 s_PurpleSelected = ImVec4(0.68f, 0.55f, 0.96f, 1.f);
	const ImVec4 s_PurpleHovered = ImVec4(0.55f, 0.36f, 0.95f, 1.f);
	const ImVec4 s_PurpleEnabled = ImVec4(0.37f, 0.23f, 0.69f, 1.f);
	const ImVec4 s_PurpleDisabled = ImVec4(0.27f, 0.19f, 0.46f, 1.f);

	const ImVec4 s_Valid = ImVec4(0.71f, 0.92f, 0.14f, 1.f);
	const ImVec4 s_Invalid = ImVec4(0.92f, 0.14f, 0.25f, 1.f);
}

editor::MainMenuBar::MainMenuBar
(
	editor::AssetBrowser& assetBrowser
	, editor::History& history
	, editor::Inspector& inspector
)
	: m_AssetBrowser(assetBrowser)
	, m_History(history)
	, m_Inspector(inspector)
{
}

editor::MainMenuBar::~MainMenuBar()
{
}

void editor::MainMenuBar::Initialize(entt::registry& registry)
{
	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_Header] = s_PurpleEnabled;
	style.Colors[ImGuiCol_HeaderActive] = s_PurpleSelected;
	style.Colors[ImGuiCol_HeaderHovered] = s_PurpleHovered;
	style.Colors[ImGuiCol_ModalWindowDimBg] = s_Gray;
	style.Colors[ImGuiCol_ResizeGrip] = s_PurpleEnabled;
	style.Colors[ImGuiCol_ResizeGripActive] = s_PurpleSelected;
	style.Colors[ImGuiCol_ResizeGripHovered] = s_PurpleHovered;
	style.Colors[ImGuiCol_Separator] = s_PurpleHovered;
	style.Colors[ImGuiCol_TitleBg] = s_PurpleEnabled;
	style.Colors[ImGuiCol_TitleBgActive] = s_PurpleEnabled;
	style.Colors[ImGuiCol_TitleBgCollapsed] = s_PurpleEnabled;
}

void editor::MainMenuBar::Destroy(entt::registry& registry)
{
}

void editor::MainMenuBar::Update(entt::registry& registry, const sf::Time& time)
{
	Render(registry);
}

void editor::MainMenuBar::Render(entt::registry& registry)
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::MenuItem("New Level...", "Ctrl+N");
			ImGui::MenuItem("Open Level...", "Ctrl+O");
			ImGui::MenuItem("Save Level", "Ctrl+S");
			ImGui::MenuItem("Save Level As...", "Ctrl+Shift+S");
			ImGui::Separator();

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit"))
		{
			ImGui::Text("History");
			ImGui::MenuItem("Undo\t", "Ctrl+Z");
			ImGui::MenuItem("Redo\t", "Ctrl+Y");
			ImGui::Separator();

			ImGui::Text("Edit");
			ImGui::MenuItem("Cut\t", "Ctrl+X");
			ImGui::MenuItem("Copy\t", "Ctrl+C");
			ImGui::MenuItem("Paste\t", "Ctrl+V");
			ImGui::MenuItem("Duplicate\t", "Ctrl+D");
			ImGui::MenuItem("Delete\t", "Delete");
			ImGui::Separator();

			ImGui::Text("Configuration");
			ImGui::MenuItem("Settings");

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Window"))
		{
			ImGui::Text("General");

			if (ImGui::MenuItem("Asset Browser"))
				m_AssetBrowser.SetVisible(true);
			if (ImGui::MenuItem("History"))
				m_History.SetVisible(true);
			if (ImGui::MenuItem("Inspector"))
				m_Inspector.SetVisible(true);

			ImGui::Separator();

			ImGui::Text("Debug");
			ImGui::MenuItem("Entt Debugger");

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}