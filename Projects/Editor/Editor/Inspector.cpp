#include "Editor/EditorPCH.h"
#include "Editor/Inspector.h"

#include "Editor/InspectorExamples.h"
#include "Editor/InspectorWidgets.h"

#include <Engine/FlipbookComponent.h>
#include <Engine/TypeList.h>
#include <Engine/TransformComponent.h>

#include <entt/entt.hpp>
#include <imgui/imgui.h>

namespace
{
	// #todo: support multiple inspectors
	// #todo: support different types of inspectors

	static float s_DividerOffset = 250.f;

	template<typename Component>
	void CloneComponent(entt::registry& oldRegistry, entt::entity oldEntity, entt::registry& newRegistry, entt::entity newEntity)
	{
		if (!oldRegistry.has<Component>(oldEntity))
			return;

		Component& oldComponent = oldRegistry.get<Component>(oldEntity);
		Component& newComponent = newRegistry.emplace<Component>(newEntity);
		newComponent = oldComponent;
	}

	template <typename ...Types>
	void CloneEntity(entt::registry& oldRegistry, entt::entity oldEntity, entt::registry& newRegistry, entt::entity newEntity, core::TypeList<Types...> typeList)
	{
		(CloneComponent<Types>(oldRegistry, oldEntity, newRegistry, newEntity), ...);
	}

	template<typename Component>
	void InspectComponent(entt::registry& registry, entt::entity entity)
	{
		if (Component* component = registry.try_get<Component>(entity))
		{
			constexpr refl::type_descriptor descriptor = refl::reflect<Component>();
			const char* name = get_display_name(descriptor);

			if (ImGui::CollapsingHeader(name, ImGuiTreeNodeFlags_DefaultOpen))
			{
				imgui::InspectorBegin();
				editor::InspectType(*component);
				imgui::InspectorEnd();
			}
		}
	}

	template <typename ...Types>
	void InspectComponents(entt::registry& registry, entt::entity entity, core::TypeList<Types...> typeList)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 20.f);
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
}

void editor::Inspector::Destroy(entt::registry& registry)
{
}

void editor::Inspector::Update(entt::registry& registry, const core::GameTime& gameTime)
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
	using ComponentsList = core::TypeList<example::Component, render::FlipbookComponent, core::TransformComponent>;
	ComponentsList components;

	if (ImGui::BeginChild("entity"))
	{
		if (registry.valid(m_Entity))
		{
			ImGui::PushID(static_cast<int>(m_Entity));

			InspectComponents(registry, m_Entity, components);

			ImGui::PopID();
		}
	}
	ImGui::EndChild();
}
