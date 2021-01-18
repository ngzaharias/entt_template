#include "Editor/EditorPCH.h"
#include "Editor/FlipbookEditor.h"

#include "Editor/InspectorWidgets.h"

#include <Engine/AssetManager.h>

#include <imgui/imgui.h>
#include <imgui-sfml/imgui-SFML.h>

namespace
{
	template<typename Type>
	void Inspect(Type& value)
	{
		ImGui::Columns(2, "Columns");

		editor::InspectType(value);

		ImGui::Columns(1);
	}
}

editor::FlipbookEditor::FlipbookEditor()
{
}

editor::FlipbookEditor::~FlipbookEditor()
{
}

void editor::FlipbookEditor::Initialize(entt::registry& registry)
{
}

void editor::FlipbookEditor::Destroy(entt::registry& registry)
{
}

void editor::FlipbookEditor::Update(entt::registry& registry, const core::GameTime& gameTime)
{
	Render();
}

void editor::FlipbookEditor::OpenEditor(const str::Name& guid)
{
	core::AssetManager& assetManager = core::AssetManager::Instance();
	m_Flipbook = assetManager.LoadAsset<render::FlipbookAsset>(guid);
}

void editor::FlipbookEditor::CloseEditor()
{
	m_Flipbook = { };
	ImGui::CloseCurrentPopup();
}

void editor::FlipbookEditor::Render()
{
	constexpr float s_SettingsWidth = 400.f;

	core::AssetManager& assetManager = core::AssetManager::Instance();

	if (!m_Flipbook)
		return;

	render::FlipbookAsset& flipbookAsset = m_Flipbook.get();

	const ImVec2 position = { ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f };
	ImGui::SetNextWindowPos(position, ImGuiCond_Once, ImVec2(0.5f, 0.5f));

	bool isOpen = true;
	ImGui::Begin("Flipbook Editor", &isOpen, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse);
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Save"))
					assetManager.SaveAsset(flipbookAsset);

				ImGui::MenuItem("Save As...");
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		if (ImGui::BeginChild("texture", { ImGui::GetWindowWidth() - s_SettingsWidth, 0 }, false))
		{
			ImGui::Text(flipbookAsset.m_Filepath.string().c_str());
		}
		ImGui::EndChild();

		ImGui::SameLine();

		if (ImGui::BeginChild("settings", { s_SettingsWidth, 0 }, false))
		{
			if (ImGui::CollapsingHeader("Flipbook", ImGuiTreeNodeFlags_DefaultOpen))
				::Inspect(flipbookAsset);
		}
		ImGui::EndChild();
	}
	ImGui::End();

	if (!isOpen)
		CloseEditor();
}