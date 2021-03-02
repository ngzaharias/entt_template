#include "EditorPCH.h"
#include "Editor/Inspector.h"

#include "Editor/ComponentList.h"
#include "Editor/Historian.h"
#include "Editor/HistorianComponents.h"
#include "Editor/InspectorTypes.h"
#include "Editor/InspectorWidgets.h"

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

	void PrintDocument(const rapidjson::Document& document)
	{
		rapidjson::StringBuffer buffer;
		rapidjson::PrettyWriter writer(buffer);
		document.Accept(writer);
		std::cout << buffer.GetString() << "\n";
		std::cout << "=========================\n";
	}

	template<typename Component>
	void InspectComponent(entt::registry& registry, entt::entity entity, InspectorInfo& info)
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
	void InspectComponents(entt::registry& registry, entt::entity entity, InspectorInfo& info, core::TypeList<Types...> typeList)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 20.f);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 8.f, 8.f });

		(InspectComponent<Types>(registry, entity, info), ...);

		ImGui::PopStyleVar(2);
	}
}

editor::Inspector::Inspector(editor::Historian& historian)
	: m_Historian(historian)
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
	m_HasChanged |= !registry.view<editor::RedoEventComponent>().empty();
	m_HasChanged |= !registry.view<editor::UndoEventComponent>().empty();

	if (m_HasChanged)
	{
		m_HasChanged = false;
		if (registry.valid(m_Entity))
		{
			m_Record.m_Entity = m_Entity;
			m_Historian.CopyToRecord(registry, m_Record, s_ComponentList);
		}
	}

	Render(registry);
}

void editor::Inspector::Render(entt::registry& registry)
{
	if (!IsVisible())
		return;

	if (ImGui::Begin("Inspector", &m_IsVisible, ImGuiWindowFlags_MenuBar))
	{
		Render_MenuBar(registry);
		Render_Selected(registry);
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

void editor::Inspector::Render_Selected(entt::registry& registry)
{
	if (ImGui::BeginChild("entity"))
	{
		if (registry.valid(m_Entity))
		{
			ImGui::PushID(static_cast<int>(m_Entity));

			InspectorInfo info;
			InspectComponents(registry, m_Entity, info, s_ComponentList);

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
