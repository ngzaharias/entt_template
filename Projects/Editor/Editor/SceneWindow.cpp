#include "Editor/EditorPCH.h"
#include "Editor/SceneWindow.h"

#include <Engine/Screen.h>

#include <imgui/imgui.h>
#include <imgui-sfml/imgui-SFML.h>
#include <ImGuizmo/ImGuizmo.h>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Transform.hpp>
#include <SFML/Graphics/View.hpp>

namespace
{
	void Orthographic(const float l, float r, float b, const float t, float zn, const float zf, float* m16)
	{
		m16[0] = 2 / (r - l);
		m16[1] = 0.0f;
		m16[2] = 0.0f;
		m16[3] = 0.0f;
		m16[4] = 0.0f;
		m16[5] = 2 / (t - b);
		m16[6] = 0.0f;
		m16[7] = 0.0f;
		m16[8] = 0.0f;
		m16[9] = 0.0f;
		m16[10] = 1.0f / (zf - zn);
		m16[11] = 0.0f;
		m16[12] = (l + r) / (l - r);
		m16[13] = (t + b) / (b - t);
		m16[14] = zn / (zn - zf);
		m16[15] = 1.0f;
	}

	void EditTransform(const sf::View& view, sf::Transform& transform)
	{
		constexpr float fov = 27.f;
		constexpr float viewWidth = 10.f; // for orthographic
		constexpr float camYAngle = 165.f / 180.f * 3.14159f;
		constexpr float camXAngle = 32.f / 180.f * 3.14159f;

		static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::ROTATE);
		static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);

		if (ImGui::IsKeyPressed(90))
			mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
		if (ImGui::IsKeyPressed(69))
			mCurrentGizmoOperation = ImGuizmo::ROTATE;
		if (ImGui::IsKeyPressed(82)) // r Key
			mCurrentGizmoOperation = ImGuizmo::SCALE;

		float* matrixTran = const_cast<float*>(transform.getMatrix());
		const float* matrixView = view.getTransform().getMatrix();

		ImGuiIO& io = ImGui::GetIO();
		float viewHeight = viewWidth * io.DisplaySize.y / io.DisplaySize.x;
		float matrixProj[16];
		Orthographic(-viewWidth, viewWidth, -viewHeight, viewHeight, 1000.f, -1000.f, matrixProj);

		ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
		ImGuizmo::Manipulate(matrixView, matrixProj, mCurrentGizmoOperation, mCurrentGizmoMode, matrixTran);
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
		sf::View view;
		view.setCenter(0.f, 0.f);
		view.setRotation(0.f);
		view.setSize(Screen::width, Screen::height);

		sf::Transform transform = sf::Transform::Identity;
		EditTransform(view, transform);

		if (ImGui::BeginMenuBar())
		{
			// #todo: dropdown select camera

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