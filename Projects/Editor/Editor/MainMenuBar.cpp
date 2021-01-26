#include "Editor/EditorPCH.h"
#include "Editor/MainMenuBar.h"

#include "Editor/AssetBrowser.h"
#include "Editor/EntityBrowser.h"
#include "Editor/History.h"
#include "Editor/Inspector.h"

#include <Engine/EnttDebugger.h>

#include <imgui/imgui.h>

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
	debug::EnttDebugger& enttDebugger
	, editor::AssetBrowser& assetBrowser
	, editor::EntityBrowser& entityBrowser
	, editor::History& history
	, editor::Inspector& inspector
)
	: m_EnttDebugger(enttDebugger)
	, m_AssetBrowser(assetBrowser)
	, m_EntityBrowser(entityBrowser)
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

void editor::MainMenuBar::Update(entt::registry& registry, const core::GameTime& gameTime)
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
			ImGui::MenuItem("History", nullptr, false, false);
			ImGui::MenuItem("Undo\t", "Ctrl+Z");
			ImGui::MenuItem("Redo\t", "Ctrl+Y");
			ImGui::Separator();

			ImGui::MenuItem("Edit", nullptr, false, false);
			ImGui::MenuItem("Cut\t", "Ctrl+X");
			ImGui::MenuItem("Copy\t", "Ctrl+C");
			ImGui::MenuItem("Paste\t", "Ctrl+V");
			ImGui::MenuItem("Duplicate\t", "Ctrl+D");
			ImGui::MenuItem("Delete\t", "Delete");
			ImGui::Separator();

			ImGui::MenuItem("Configuration", nullptr, false, false);
			ImGui::MenuItem("Settings");

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Window"))
		{
			ImGui::MenuItem("General", nullptr, false, false);

			bool assetBrowser = m_AssetBrowser.IsVisible();
			if (ImGui::MenuItem("Asset Browser", nullptr, &assetBrowser))
				m_AssetBrowser.SetVisible(assetBrowser);
			bool entityBrowser = m_EntityBrowser.IsVisible();
			if (ImGui::MenuItem("Entity Browser", nullptr, &entityBrowser))
				m_EntityBrowser.SetVisible(entityBrowser);
			bool history = m_History.IsVisible();
			if (ImGui::MenuItem("History", nullptr, &history))
				m_History.SetVisible(history);
			bool inspector = m_Inspector.IsVisible();
			if (ImGui::MenuItem("Inspector", nullptr, &inspector))
				m_Inspector.SetVisible(inspector);

			ImGui::Separator();

			ImGui::MenuItem("Debug", nullptr, false, false);
			bool enttDebugger = m_EnttDebugger.IsVisible();
			if (ImGui::MenuItem("Entity Debugger", nullptr, &enttDebugger))
				m_EnttDebugger.SetVisible(enttDebugger);

			ImGui::EndMenu();
		}


		ImGui::EndMainMenuBar();
	}
}