#include "EditorPCH.h"
#include "Editor/SceneWindow.h"

#include <Engine/Screen.h>

#include <imgui/imgui.h>
#include <imgui-sfml/imgui-SFML.h>
#include <ImGuizmo/ImGuizmo.h>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Transform.hpp>
#include <SFML/Graphics/View.hpp>

// #todo: multiple windows
// #todo: dropdown select camera

namespace
{
	void EditTransform(const sf::View& view, sf::Transform& transform)
	{
		constexpr ImGuizmo::OPERATION operation = ImGuizmo::TRANSLATE;
		constexpr ImGuizmo::MODE mode = ImGuizmo::WORLD;
		const ImGuiIO& io = ImGui::GetIO();

		float matrixProj[16];
		float* matrixView = const_cast<float*>(view.getTransform().getMatrix());
		float* matrixTran = const_cast<float*>(transform.getMatrix());

		ImGuizmo::Enable(true);
		ImGuizmo::SetOrthographic(true);
		ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
		ImGuizmo::Manipulate(matrixView, matrixProj, operation, mode, matrixTran);
	}
}

editor::SceneWindow::SceneWindow(sf::RenderTexture& renderTexture)
	: m_RenderTexture(renderTexture)
{
}

editor::SceneWindow::~SceneWindow()
{
}

void editor::SceneWindow::Initialize(entt::registry& registry)
{
}

void editor::SceneWindow::Destroy(entt::registry& registry)
{
}

void editor::SceneWindow::Update(entt::registry& registry, const core::GameTime& gameTime)
{
	Render(registry);
}

void editor::SceneWindow::Render(entt::registry& registry)
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

		// #todo: get render texture from camera
		ImGui::Image(m_RenderTexture.getTexture(), viewSize);
	}
	ImGui::End();
}