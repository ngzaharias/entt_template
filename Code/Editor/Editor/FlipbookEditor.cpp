#include "EditorPCH.h"
#include "Editor/FlipbookEditor.h"

#include "Editor/InspectorWidgets.h"

#include <Engine/AssetManager.h>
#include <Engine/FlipbookComponent.h>

#include <entt/entity/registry.hpp>
#include <imgui/imgui.h>
#include <imgui-sfml/imgui-SFML.h>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Window/Keyboard.hpp>

namespace
{
	constexpr float s_TimeMaxEpsilon = 0.000001f;

	template<typename Type>
	void Inspect(Type& value)
	{
		imgui::InspectorBegin();

		editor::InspectorInfo info;
		editor::InspectType(value, info);

		imgui::InspectorEnd();
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
	PROFILE_FUNCTION();

	if (m_Guid && m_Entity == entt::null)
	{
		m_Entity = registry.create();
		registry.emplace<render::FlipbookComponent>(m_Entity);
	}
	else if (!m_Guid && m_Entity != entt::null)
	{
		registry.destroy(m_Entity);
		m_Entity = entt::null;
	}

	if (m_Entity != entt::null)
	{
		core::AssetManager& assetManager = core::AssetManager::Instance();
		render::FlipbookHandle flipbookHandle = assetManager.LoadAsset<render::FlipbookAsset>(*m_Guid);
		render::FlipbookAsset& flipbookAsset = flipbookHandle.get();

		auto& component = registry.get<render::FlipbookComponent>(m_Entity);
		component.m_Flipbook = flipbookHandle;
		component.m_FPS = flipbookAsset.m_FPS;

		// #todo: only trigger once per press/release
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			component.m_IsPlaying = !component.m_IsPlaying;

		Render(component);
	}
}

void editor::FlipbookEditor::OpenEditor(const str::Name& guid)
{
	m_Guid = guid;
}

void editor::FlipbookEditor::CloseEditor()
{
	m_Guid = { };
	ImGui::CloseCurrentPopup();
}

void editor::FlipbookEditor::Render(render::FlipbookComponent& component)
{
	constexpr float s_PlaybackHeight = 100.f;
	constexpr float s_SettingsWidth = 400.f;
	const Vector2f s_FramePadding = { 8.f, 8.f };

	render::FlipbookAsset& flipbookAsset = component.m_Flipbook.get();

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
					core::AssetManager::Instance().SaveAsset(flipbookAsset);

				// #todo
				ImGui::MenuItem("Save As...");
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		const float s_PreviewWidth = ImGui::GetColumnWidth() - s_SettingsWidth - s_FramePadding.x;
		const float s_PreviewHeight = ImGui::GetContentRegionAvail().y - s_PlaybackHeight;
		if (ImGui::BeginChild("preview", { s_PreviewWidth, s_PreviewHeight }, true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
		{
			ImGui::Text(flipbookAsset.m_Filepath.string().c_str());
			ImGui::Separator();

			Render_Preview(component);
		}
		ImGui::EndChild();

		ImGui::SameLine();

		if (ImGui::BeginChild("settings", { s_SettingsWidth, s_PreviewHeight }, true))
		{
			if (ImGui::CollapsingHeader("Flipbook", ImGuiTreeNodeFlags_DefaultOpen))
			{
				const bool wasLooping = flipbookAsset.m_IsLooping;
				Inspect(flipbookAsset);

				if (wasLooping != flipbookAsset.m_IsLooping)
					component.m_IsLooping = flipbookAsset.m_IsLooping;
			}
		}
		ImGui::EndChild();

		if (ImGui::BeginChild("playback", { 0, s_PlaybackHeight - 8.f }, true))
		{
			Render_Playback(component);
		}
		ImGui::EndChild();
	}
	ImGui::End();

	if (!isOpen)
		CloseEditor();
}

void editor::FlipbookEditor::Render_Playback(render::FlipbookComponent& component)
{
	if (!component.m_Flipbook)
		return;

	const render::FlipbookAsset& flipbookAsset = component.m_Flipbook.get();

	const int32 indexCount = static_cast<int32>(flipbookAsset.m_Frames.size());
	const int32 indexMax = std::max(0, indexCount - 1);
	const float timeFrame = 1.f / flipbookAsset.m_FPS;
	const float timeMax = indexCount / flipbookAsset.m_FPS;

	std::optional<float> timeNew = { };
	std::optional<int32> indexNew = { };

	ImGui::PushItemWidth(ImGui::GetWindowWidth() - 16.f);
	if (ImGui::SliderInt("##index", &component.m_Index, 0, indexMax))
		indexNew = component.m_Index;
	if (ImGui::SliderFloat("##time", &component.m_Time, 0.f, timeMax))
		timeNew = component.m_Time;
	ImGui::PopItemWidth();

	if (ImGui::Button("Frame First"))
		indexNew = 0;

	ImGui::SameLine();

	if (ImGui::Button("Frame Previous"))
	{
		component.m_IsPlaying = false;
		indexNew = std::max(0, component.m_Index - 1);
	}

	ImGui::SameLine();

	{
		if (component.m_IsPlaying && ImGui::Button("Pause"))
			component.m_IsPlaying = false;
		if (!component.m_IsPlaying && ImGui::Button("Play"))
			component.m_IsPlaying = true;
	}

	ImGui::SameLine();

	if (ImGui::Button("Frame Next"))
	{
		component.m_IsPlaying = false;
		indexNew = std::min(indexMax, component.m_Index + 1);
	}

	ImGui::SameLine();

	if (ImGui::Button("Frame Last"))
		indexNew = indexMax;

	ImGui::SameLine();

	{
		if (component.m_IsLooping && ImGui::Button("Unloop"))
			component.m_IsLooping = false;
		if (!component.m_IsLooping && ImGui::Button("Loop"))
			component.m_IsLooping = true;
	}

	if (timeNew)
		component.m_Time = std::clamp(timeNew.value(), 0.f, timeMax);
	if (indexNew)
		component.m_Time = indexNew.value() * timeFrame;
}

void editor::FlipbookEditor::Render_Preview(render::FlipbookComponent& component)
{
	render::FlipbookAsset& flipbookAsset = component.m_Flipbook.get();
	if (flipbookAsset.m_Frames.empty())
		return;

	const render::FlipbookFrame& flipbookFrame = flipbookAsset.m_Frames[component.m_Index];
	if (!flipbookFrame.m_Sprite)
		return;

	const render::SpriteAsset& spriteAsset = flipbookFrame.m_Sprite.get();
	if (!spriteAsset.m_Texture)
		return;

	const render::TextureAsset& textureAsset = spriteAsset.m_Texture.get();
	const sf::Texture& texture = textureAsset.m_Texture;

	sf::Sprite sprite;
	sprite.setTexture(texture);
	sprite.setTextureRect(sf::IntRect
	(
		spriteAsset.m_RectanglePos.x
		, spriteAsset.m_RectanglePos.y
		, spriteAsset.m_RectangleSize.x
		, spriteAsset.m_RectangleSize.y
	));

	const Vector2f size = Vector2f(texture.getSize());
	ImGui::Image(sprite, { size.x, size.y });
}
