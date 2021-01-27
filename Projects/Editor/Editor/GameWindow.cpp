#include "Editor/EditorPCH.h"
#include "Editor/GameWindow.h"

#include <Engine/Screen.h>

#include <imgui/imgui.h>
#include <imgui-sfml/imgui-SFML.h>
#include <SFML/Graphics/RenderTexture.hpp>

editor::GameWindow::GameWindow(sf::RenderTexture& renderTexture)
	: m_RenderTexture(renderTexture)
{
}

editor::GameWindow::~GameWindow()
{
}

void editor::GameWindow::Initialize(entt::registry& registry)
{
}

void editor::GameWindow::Destroy(entt::registry& registry)
{
}

void editor::GameWindow::Update(entt::registry& registry, const core::GameTime& gameTime)
{
	Render(registry);
}

void editor::GameWindow::Render(entt::registry& registry)
{
	if (!m_IsVisible)
		return;

	ImGuiWindowFlags flags = 0;
	flags |= ImGuiWindowFlags_MenuBar;
	flags |= ImGuiWindowFlags_NoCollapse;
	flags |= ImGuiWindowFlags_NoScrollbar;
	flags |= ImGuiWindowFlags_NoScrollWithMouse;

	if (ImGui::Begin("Game", &m_IsVisible, flags))
	{
		if (ImGui::BeginMenuBar())
		{
			// #todo: dropdown select camera
			static int cameraIndex = 0;
			const char* items[] = { "Camera 1", "Camera 2" };
			ImGui::Combo("", &cameraIndex, items, 2);

			ImGui::EndMenuBar();
		}

		// #fixme: update the camera size
		const Vector2f regionSize = { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y };
		const Vector2f viewSize = m_RenderTexture.getView().getSize();
		Screen::width = regionSize.x;
		Screen::height = regionSize.y;

		// #todo: get render texture from camera
		ImGui::Image(m_RenderTexture.getTexture(), viewSize);
	}
	ImGui::End();
}