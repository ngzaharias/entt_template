#include "Editor/EditorPCH.h"
#include "Editor/FlipbookEditor.h"

#include "Editor/InspectorWidgets.h"

#include <Engine/AssetManager.h>

#include <imgui/imgui.h>
#include <imgui-sfml/imgui-SFML.h>
#include <SFML/Graphics/Sprite.hpp>

namespace
{
	constexpr float s_TimeMaxEpsilon = 0.0000001f;

	sf::Texture* iconBack = nullptr;
	sf::Texture* iconFile = nullptr;
	sf::Texture* iconFolder = nullptr;

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
	if (m_Component && m_Component->m_Flipbook)
	{
		const render::FlipbookAsset& flipbookAsset = m_Component->m_Flipbook.get();
		const int32 indexCount = static_cast<int32>(flipbookAsset.m_Frames.size());

		const float timeMax = indexCount / flipbookAsset.m_FPS;

		if (m_IsPlaying)
		{
			const float timeOld = m_Component->m_Time;
			const float timeNew = m_Component->m_Time + gameTime.asSeconds();

			if (timeNew < timeMax)
			{
				m_Component->m_Time = timeNew;
			}
			else if (m_IsLooping)
			{
				m_Component->m_Time = timeNew - timeMax;
			}
			else
			{
				m_Component->m_Time = timeMax;
			}
		}

		// #note: we subtract the epsilon because otherwise it will wrap back to 0 when time == timeMax
		m_Component->m_Time = std::clamp(m_Component->m_Time, 0.f, timeMax - s_TimeMaxEpsilon);
		m_Component->m_Index = static_cast<int32>(m_Component->m_Time * flipbookAsset.m_FPS);
		m_Component->m_Index %= indexCount;
	}

	Render();
}

void editor::FlipbookEditor::OpenEditor(const str::Name& guid)
{
	core::AssetManager& assetManager = core::AssetManager::Instance();

	m_Component = render::FlipbookComponent();
	m_Component->m_Time = 0.f;
	m_Component->m_Index = 0;
	m_Component->m_Flipbook = assetManager.LoadAsset<render::FlipbookAsset>(guid);
}

void editor::FlipbookEditor::CloseEditor()
{
	m_Component = { };
	ImGui::CloseCurrentPopup();
}

void editor::FlipbookEditor::Render()
{
	constexpr float s_PlaybackHeight = 100.f;
	constexpr float s_SettingsWidth = 400.f;
	const Vector2f s_FramePadding = { 8.f, 8.f };

	core::AssetManager& assetManager = core::AssetManager::Instance();

	if (!m_Component)
		return;

	render::FlipbookAsset& flipbookAsset = m_Component->m_Flipbook.get();

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

		const float s_PreviewHeight = ImGui::GetWindowHeight() - s_PlaybackHeight;
		const float s_PreviewWidth = ImGui::GetColumnWidth() - s_SettingsWidth - s_FramePadding.x;
		if (ImGui::BeginChild("preview", { s_PreviewWidth, s_PreviewHeight }, true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
		{
			ImGui::Text(flipbookAsset.m_Filepath.string().c_str());
			ImGui::Separator();

			Render_Preview();
		}
		ImGui::EndChild();

		ImGui::SameLine();

		if (ImGui::BeginChild("settings", { s_SettingsWidth, s_PreviewHeight }, true))
		{
			if (ImGui::CollapsingHeader("Flipbook", ImGuiTreeNodeFlags_DefaultOpen))
				Inspect(flipbookAsset);
		}
		ImGui::EndChild();

		if (ImGui::BeginChild("playback", { 0, s_PlaybackHeight - 8.f }, true))
		{
			Render_Playback();
		}
		ImGui::EndChild();
	}
	ImGui::End();

	if (!isOpen)
		CloseEditor();
}

void editor::FlipbookEditor::Render_Playback()
{
	if (!m_Component || !m_Component->m_Flipbook)
		return;

	const render::FlipbookAsset& flipbookAsset = m_Component->m_Flipbook.get();

	const int32 indexCount = static_cast<int32>(flipbookAsset.m_Frames.size());
	const int32 indexMax = std::max(0, indexCount - 1);
	const float timeFrame = 1.f / flipbookAsset.m_FPS;
	const float timeMax = indexCount / flipbookAsset.m_FPS;

	std::optional<float> timeNew = { };
	std::optional<int32> indexNew = { };

	ImGui::PushItemWidth(ImGui::GetWindowWidth() - 16.f);
	if (ImGui::SliderInt("##index", &m_Component->m_Index, 0, indexMax))
		indexNew = m_Component->m_Index;
	if (ImGui::SliderFloat("##time", &m_Component->m_Time, 0.f, timeMax))
		timeNew = m_Component->m_Time;
	ImGui::PopItemWidth();

	if (ImGui::Button("Frame First"))
		indexNew = 0;

	ImGui::SameLine();

	if (ImGui::Button("Frame Previous"))
	{
		m_IsPlaying = false;
		indexNew = std::max(0, m_Component->m_Index - 1);
	}

	ImGui::SameLine();

	{
		if (m_IsPlaying && ImGui::Button("Pause"))
			m_IsPlaying = false;
		if (!m_IsPlaying && ImGui::Button("Play"))
			m_IsPlaying = true;
	}

	ImGui::SameLine();

	if (ImGui::Button("Frame Next"))
	{
		m_IsPlaying = false;
		indexNew = std::min(indexMax, m_Component->m_Index + 1);
	}

	ImGui::SameLine();

	if (ImGui::Button("Frame Last"))
		indexNew = indexMax;

	ImGui::SameLine();

	{
		if (m_IsLooping && ImGui::Button("Unloop"))
			m_IsLooping = false;
		if (!m_IsLooping && ImGui::Button("Loop"))
			m_IsLooping = true;
	}

	if (timeNew)
		m_Component->m_Time = std::clamp(timeNew.value(), 0.f, timeMax);
	if (indexNew)
		m_Component->m_Time = indexNew.value() * timeFrame;
}

void editor::FlipbookEditor::Render_Preview()
{
	if (!m_Component)
		return;

	render::FlipbookAsset& flipbookAsset = m_Component->m_Flipbook.get();
	if (flipbookAsset.m_Frames.empty())
		return;

	const render::FlipbookFrame& flipbookFrame = flipbookAsset.m_Frames[m_Component->m_Index];
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
		spriteAsset.m_Position.x
		, spriteAsset.m_Position.y
		, spriteAsset.m_Size.x
		, spriteAsset.m_Size.y
	));

	const Vector2f size = Vector2f(texture.getSize());
	ImGui::Image(sprite, { size.x, size.y });
}
