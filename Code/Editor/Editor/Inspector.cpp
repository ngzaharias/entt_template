#include "EditorPCH.h"
#include "Editor/Inspector.h"

#include "Editor/ComponentList.h"
#include "Editor/HistorianComponents.h"
#include "Editor/InspectorTypes.h"
#include "Editor/InspectorWidgets.h"
#include "Editor/SceneEditor.h"

#include <Engine/TypeList.h>

#include <iostream>
#include <entt/entt.hpp>
#include <imgui/imgui.h>
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>

namespace editor
{
	// #todo: multiple windows
	// #todo: multi-edit
	// #todo: reset to default 
	// #todo: reset to parent
	// #todo: undo/redo

	static float s_DividerOffset = 250.f;

	void PrintDocument(const json::Document& document)
	{
		rapidjson::StringBuffer buffer;
		rapidjson::PrettyWriter writer(buffer);
		document.Accept(writer);
		std::cout << buffer.GetString() << "\n";
		std::cout << "=========================\n";
	}

	template<typename Component>
	void InspectComponent(entt::registry& registry, ecs::Entity entity, InspectorInfo& info)
	{
		if (Component* component = registry.try_get<Component>(entity))
		{
			constexpr refl::type_descriptor descriptor = refl::reflect<Component>();
			const char* name = get_display_name(descriptor);

			if (ImGui::CollapsingHeader(name, ImGuiTreeNodeFlags_DefaultOpen))
			{
				info.m_Address.Push(name);
				
				imgui::InspectorBegin();
				editor::InspectType(*component, info);
				imgui::InspectorEnd();

				info.m_Address.Pop();
			}
		}
	}

	template <typename ...Types>
	void InspectComponents(entt::registry& registry, ecs::Entity entity, InspectorInfo& info, core::TypeList<Types...> typeList)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 20.f);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 8.f, 8.f });

		(InspectComponent<Types>(registry, entity, info), ...);

		ImGui::PopStyleVar(2);
	}
}

editor::Inspector::Inspector(
	editor::Historian& historian,
	editor::SceneEditor& sceneEditor)
	: m_Historian(historian)
	, m_SceneEditor(sceneEditor)
{
}

void editor::Inspector::Update(const core::GameTime& gameTime)
{
	PROFILE_FUNCTION();

	auto& registry = m_World->m_Registry;


	{
		{
			m_Historian.CopyToRecord(registry, m_Record, s_ComponentList);
		}
	}

	Render();
}

void editor::Inspector::Render()
{
	auto& registry = m_World->m_Registry;

	if (!IsVisible())
		return;

	if (ImGui::Begin("Inspector", &m_IsVisible, ImGuiWindowFlags_MenuBar))
	{
		Render_MenuBar(registry);
	}
	ImGui::End();
}

void editor::Inspector::Render_MenuBar(entt::registry& registry)
{
	if (ImGui::BeginMenuBar())
	{
		ImGui::EndMenuBar();
	}
}

{
	if (ImGui::BeginChild("entity"))
	{
		{

			InspectorInfo info;

			if (!info.m_Transactions.empty())
			{
				editor::Record recordOld = m_Record;
				for (const auto& transaction : info.m_Transactions)
					transaction.ApplyTo(m_Record.m_Document);

				m_Historian.PushRecord(recordOld, m_Record);
			}

			ImGui::PopID();
		}
	}
	ImGui::EndChild();
}
