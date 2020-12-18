#include "Editor/EditorPCH.h"
#include "Editor/Inspector.h"

#include "Editor/InspectorExamples.h"
#include "Editor/InspectorWidgets.h"

#include <Engine/TransformComponent.h>
#include <Engine/TypeList.h>

#include <entt/entt.hpp>
#include <imgui/imgui.h>
#include <SFML/System/Time.hpp>

namespace
{
	// #todo: support multiple inspectors
	// #todo: support different types of inspectors

	static float s_DividerOffset = 250.f;

	template<typename Component>
	void InspectComponent(entt::registry& registry, entt::entity entity)
	{
		if (Component* value = registry.try_get<Component>(entity))
		{
			constexpr refl::type_descriptor descriptor = refl::reflect<Component>();
			const char* name = get_display_name(descriptor);

			if (ImGui::CollapsingHeader(name, ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::Columns(2, "Columns");
				ImGui::SetColumnOffset(1, s_DividerOffset);

				imgui::Indent_x(2);
				widget::TypeAsIs(*value);
				imgui::Unindent_x(2);

				ImGui::Columns(1);
				s_DividerOffset = imgui::GetColumnOffset("Columns", 2, 1);
			}
		}
	}

	template <typename ...Types>
	void InspectComponents(entt::registry& registry, entt::entity entity, core::TypeList<Types...> typeList)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 13.f);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 8.f, 8.f });

		(InspectComponent<Types>(registry, entity), ...);

		ImGui::PopStyleVar(2);
	}
}

editor::Inspector::Inspector()
{
}

editor::Inspector::~Inspector()
{
}

void editor::Inspector::Initialize(entt::registry& registry)
{
	m_Entity = registry.create();
	registry.emplace<example::Component>(m_Entity);
	registry.emplace<core::TransformComponent>(m_Entity);
}

void editor::Inspector::Destroy(entt::registry& registry)
{
}

void editor::Inspector::Update(entt::registry& registry, const sf::Time& time)
{
	Render(registry);
}

void editor::Inspector::Render(entt::registry& registry)
{
	if (!m_IsVisible)
		return;

	ImGui::Begin("Inspector", &m_IsVisible, ImGuiWindowFlags_MenuBar);

	Render_MenuBar(registry);
	Render_Selected(registry);

	ImGui::End();
}

void editor::Inspector::Render_MenuBar(entt::registry& registry)
{
	if (ImGui::BeginMenuBar())
	{
		ImGui::EndMenuBar();
	}
}

void editor::Inspector::Render_Selected(entt::registry& registry)
{
	using ComponentsList = core::TypeList
		<
		example::Component
		, core::TransformComponent
		>;

	if (ImGui::BeginChild("entity"))
	{
		if (registry.valid(m_Entity))
		{
			ImGui::PushID(static_cast<int>(m_Entity));

			ComponentsList components;
			InspectComponents(registry, m_Entity, components);

			ImGui::PopID();
		}
		ImGui::EndChild();
	}
}
