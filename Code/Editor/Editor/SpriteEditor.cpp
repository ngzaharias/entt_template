#include "EditorPCH.h"
#include "Editor/SpriteEditor.h"

#include "Editor/InspectorWidgets.h"

#include <Engine/AssetManager.h>

#include <imgui/imgui.h>
#include <imgui-sfml/imgui-SFML.h>

namespace
{
	template<typename Type>
	void Inspect(Type& value)
	{
		imgui::InspectorBegin();

		editor::InspectorInfo info;
		editor::InspectType(value, info);
		
		imgui::InspectorEnd();
	}
}

editor::SpriteEditor::SpriteEditor()
{
}

editor::SpriteEditor::~SpriteEditor()
{
}

void editor::SpriteEditor::Initialize(entt::registry& registry)
{
}

void editor::SpriteEditor::Destroy(entt::registry& registry)
{
}

void editor::SpriteEditor::Update(entt::registry& registry, const core::GameTime& gameTime)
{
	PROFILE_FUNCTION();

	Render();
}

void editor::SpriteEditor::OpenEditor(const str::Name& guid)
{
	core::AssetManager& assetManager = core::AssetManager::Instance();
	m_Sprite = assetManager.LoadAsset<render::SpriteAsset>(guid);
}

void editor::SpriteEditor::CloseEditor()
{
	m_Sprite = { };
	ImGui::CloseCurrentPopup();
}

void editor::SpriteEditor::Render()
{
	constexpr float s_SettingsWidth = 400.f;

	core::AssetManager& assetManager = core::AssetManager::Instance();

	if (!m_Sprite)
		return;

	render::SpriteAsset& spriteAsset = m_Sprite.get();

	const ImVec2 position = { ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f };
	ImGui::SetNextWindowPos(position, ImGuiCond_Once, ImVec2(0.5f, 0.5f));

	bool isOpen = true;
	ImGui::Begin("Sprite Editor", &isOpen, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse);
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Save"))
					assetManager.SaveAsset(spriteAsset);

				ImGui::MenuItem("Save As...");
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		ImGui::Text(spriteAsset.m_Filepath.string().c_str());

		ImGuiWindowFlags flags = 0;
		flags |= ImGuiWindowFlags_NoScrollbar;
		flags |= ImGuiWindowFlags_NoScrollWithMouse;

		const float s_TextureWidth = ImGui::GetContentRegionAvail().x - s_SettingsWidth;
		const float s_TextureHeight = ImGui::GetContentRegionAvail().y;
		if (ImGui::BeginChild("texture", { s_TextureWidth, s_TextureHeight }, true, flags))
		{
			if (spriteAsset.m_Texture)
			{
				const ImVec2 cursorPos = ImGui::GetCursorScreenPos();

				const render::TextureAsset& textureAsset = spriteAsset.m_Texture.get();
				ImGui::Image(textureAsset.m_Texture);

				ImDrawList* draw_list = ImGui::GetWindowDrawList();
				const ImVec2 min =
				{
					cursorPos.x + spriteAsset.m_RectanglePos.x
					, cursorPos.y + spriteAsset.m_RectanglePos.y
				};
				const ImVec2 max =
				{
					min.x + spriteAsset.m_RectangleSize.x
					, min.y + spriteAsset.m_RectangleSize.y
				};
				const ImU32 col = ImColor(ImVec4(1.0f, 1.0f, 0.4f, 1.0f));
				draw_list->AddRect(min, max, col, 0.0f, ImDrawCornerFlags_All, 1.f);
			}
		}
		ImGui::EndChild();

		ImGui::SameLine();

		if (ImGui::BeginChild("settings", { s_SettingsWidth, 0 }, true))
		{
			if (ImGui::CollapsingHeader("Sprite", ImGuiTreeNodeFlags_DefaultOpen))
				Inspect(spriteAsset);
		}
		ImGui::EndChild();
	}
	ImGui::End();

	if (!isOpen)
		CloseEditor();
}