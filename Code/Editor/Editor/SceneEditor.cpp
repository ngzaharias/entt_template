#include "EditorPCH.h"
#include "Editor/SceneEditor.h"

#include <Engine/Screen.h>

#include <imgui/imgui.h>
#include <imgui-sfml/imgui-SFML.h>
#include <ImGuizmo/ImGuizmo.h>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Transform.hpp>
#include <SFML/Graphics/View.hpp>

// #todo: editor camera
// #todo: multiple cameras
// #todo: multiple windows
// #todo: dropdown select camera

editor::SceneEditor::SceneEditor(sf::RenderTexture& renderTexture)
	: m_RenderTexture(renderTexture)
{
}

editor::SceneEditor::~SceneEditor()
{
}

void editor::SceneEditor::Initialize(entt::registry& registry)
{
}

void editor::SceneEditor::Destroy(entt::registry& registry)
{
}

void editor::SceneEditor::Update(entt::registry& registry, const core::GameTime& gameTime)
{
	Render(registry);
}

void editor::SceneEditor::Render(entt::registry& registry)
{
	if (!m_IsVisible)
		return;

	ImGuiWindowFlags flags = 0;
	flags |= ImGuiWindowFlags_MenuBar;
	flags |= ImGuiWindowFlags_NoCollapse;
	flags |= ImGuiWindowFlags_NoScrollbar;
	flags |= ImGuiWindowFlags_NoScrollWithMouse;

	if (ImGui::Begin("Scene", &m_IsVisible, flags))
	{
		if (ImGui::BeginMenuBar())
			ImGui::EndMenuBar();

		// #fixme: update the camera size
		const Vector2f regionSize = { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y };
		const Vector2f viewSize = m_RenderTexture.getView().getSize();
		Screen::width = regionSize.x;
		Screen::height = regionSize.y;

		ImGui::Image(m_RenderTexture.getTexture(), viewSize);
	}
	ImGui::End();
}