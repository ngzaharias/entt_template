#include "EditorPCH.h"
#include "Editor/SpriteExtractor.h"

#include "Editor/InspectorWidgets.h"

#include <Engine/AssetManager.h>
#include <Engine/FileHelpers.h>

#include <functional>

#include <imgui/imgui.h>
#include <imgui-sfml/imgui-SFML.h>

namespace
{
	constexpr const char* s_Label = "Sprite Extractor";

	template<typename Type>
	void Inspect(Type& value)
	{
		imgui::InspectorBegin();
		//editor::InspectType(value);
		imgui::InspectorEnd();
	}

	void ForEachSprite(const editor::GridSettings& settings, std::function<bool(const Vector2u&)> callback)
	{
		Vector2u position = { 0, 0 };
		position.y = settings.m_Margin.y;

		for (uint32 y = 0; y < settings.m_Count.y; ++y)
		{
			position.x = settings.m_Margin.x;
			for (uint32 x = 0; x < settings.m_Count.x; ++x)
			{
				if (!callback(position))
					return;
				position.x += settings.m_Size.x;
				position.x += settings.m_Spacing.x;
			}
			position.y += settings.m_Size.y;
			position.y += settings.m_Spacing.y;
		}
	}
}

editor::SpriteExtractor::SpriteExtractor()
{
}

editor::SpriteExtractor::~SpriteExtractor()
{
}

void editor::SpriteExtractor::Initialize(entt::registry& registry)
{
}

void editor::SpriteExtractor::Destroy(entt::registry& registry)
{
}

void editor::SpriteExtractor::Update(entt::registry& registry, const core::GameTime& gameTime)
{
	Render(registry);
}

void editor::SpriteExtractor::OpenDialog(const str::Name& guid)
{
	core::AssetManager& assetManager = core::AssetManager::Instance();
	m_Texture = assetManager.LoadAsset<render::TextureAsset>(guid);
}

void editor::SpriteExtractor::CloseDialog()
{
	m_Texture = { };
	ImGui::CloseCurrentPopup();
}

void editor::SpriteExtractor::Render(entt::registry& registry)
{
	constexpr float s_SettingsWidth = 400.f;
	constexpr float s_SettingsBotHeight = 30.f;

	core::AssetManager& assetManager = core::AssetManager::Instance();

	if (!m_Texture)
		return;

	const render::TextureAsset& textureAsset = m_Texture.get();
	if (!ImGui::IsPopupOpen(s_Label))
	{
		ImGui::OpenPopup(s_Label);
		m_GridSettings.m_Count = { 1, 1 };
		m_GridSettings.m_Size = textureAsset.m_Texture.getSize();
	}

	const ImVec2 position = { ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f };
	ImGui::SetNextWindowPos(position, ImGuiCond_Once, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal(s_Label))
	{
		if (ImGui::BeginChild("texture", { ImGui::GetContentRegionAvail().x - s_SettingsWidth, 0 }, false))
		{
			ImGui::Text(textureAsset.m_Filepath.string().c_str());

			const ImVec2 cursorPos = ImGui::GetCursorScreenPos();
			ImGui::Image(textureAsset.m_Texture);

			ImDrawList* drawList = ImGui::GetWindowDrawList();
			ForEachSprite(m_GridSettings, [&](const Vector2u& position)
			{
				const ImVec2 min = { cursorPos.x + position.x, cursorPos.y + position.y };
				const ImVec2 max = { min.x + m_GridSettings.m_Size.x, min.y + m_GridSettings.m_Size.y };
				const ImU32 col = ImColor(ImVec4(1.0f, 1.0f, 0.4f, 1.0f));
				drawList->AddRect(min, max, col, 0.0f, ImDrawCornerFlags_All, 1.f);
				return true;
			});
		}
		ImGui::EndChild();

		ImGui::SameLine();

		if (ImGui::BeginChild("settings", { ImGui::GetContentRegionAvail().x, 0 }, false))
		{
			const float topWidth = s_SettingsWidth;
			const float topHeight = ImGui::GetWindowHeight() - s_SettingsBotHeight;
			if (ImGui::BeginChild("settings_top", { topWidth, topHeight }, false))
			{
				if (ImGui::CollapsingHeader("Naming", ImGuiTreeNodeFlags_DefaultOpen))
					Inspect(m_NamingSettings);

				if (ImGui::CollapsingHeader("Grid", ImGuiTreeNodeFlags_DefaultOpen))
					Inspect(m_GridSettings);
			}
			ImGui::EndChild();

			const float botWidth = s_SettingsWidth;
			const float botHeight = s_SettingsBotHeight;
			if (ImGui::BeginChild("settings_bottom", { botWidth, botHeight }, false))
			{
				if (ImGui::Button("Extract"))
				{
					Extract();
					CloseDialog();
				}
				ImGui::SameLine();
				if (ImGui::Button("Cancel"))
					CloseDialog();
			}
			ImGui::EndChild();
		}
		ImGui::EndChild();

		ImGui::EndPopup();
	}
}

void editor::SpriteExtractor::Extract()
{
	core::SelectFolderSettings folderSettings;
	str::Path outputFolder = core::SelectFolderDialog(folderSettings);
	if (outputFolder.empty())
		return;

	int32 namingIndex = m_NamingSettings.m_StartIndex;

	core::AssetManager& assetManager = core::AssetManager::Instance();
	ForEachSprite(m_GridSettings, [&](const Vector2u& position)
	{
		str::Path outputFilepath = outputFolder;
		outputFilepath += "\\";
		outputFilepath += m_NamingSettings.m_Template;
		outputFilepath += std::to_string(namingIndex++);
		outputFilepath += ".asset";

		render::SpriteAsset asset;
		asset.m_RectanglePos = position;
		asset.m_RectangleSize = m_GridSettings.m_Size;
		asset.m_Texture = m_Texture;

		assetManager.CreateAsset(asset, outputFilepath);
		return true;
	});
}
