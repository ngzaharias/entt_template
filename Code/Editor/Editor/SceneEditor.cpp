#include "EditorPCH.h"
#include "Editor/SceneEditor.h"

#include "Editor/HistorianComponents.h"

#include <Engine/Screen.h>
#include <Engine/TransformComponent.h>

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

namespace
{
}

editor::SceneEditor::SceneEditor(
	sf::RenderTexture& renderTexture)
	: m_RenderTexture(renderTexture)
{
}

void editor::SceneEditor::Update(const core::GameTime& gameTime)
{
	PROFILE_FUNCTION();

	Render();
}

void editor::SceneEditor::Render()
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
		{
			ImGui::EndMenuBar();
		}

		// #fixme: update the camera size
		const Vector2f regionSize = { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y };
		const Vector2f viewSize = m_RenderTexture.getView().getSize();
		Screen::width = regionSize.x;
		Screen::height = regionSize.y;

		ImGui::Image(m_RenderTexture.getTexture(), viewSize);
	}
	ImGui::End();
}