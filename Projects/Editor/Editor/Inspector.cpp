#include "Editor/Inspector.h"

#include "Editor/PropertyWidgets.h"

#include <Engine/AttributeHelpers.h>
#include <Engine/FieldAttributes.h>
#include <Engine/TransformComponent.h>
#include <Engine/TypeList.h>

#include <map>
#include <vector>
#include <entt/entt.hpp>
#include <imgui/imgui.h>
#include <refl/refl.hpp>
#include <SFML/System/Time.hpp>

namespace
{
	struct ExampleComponent
	{
		bool m_Bool = true;
		int m_Int = 1337;
		float m_Float = 0.666f;
		sf::Vector3f m_Vector3 = { 1.f, 2.f, 3.f };

		std::map<int, int> m_Map = { {1,1}, {2,2}, {3,3} };
		std::vector<int> m_Vector = { 1, 2, 3, 4, 5 };
	};

	template<typename Component>
	void InspectComponent(entt::registry& registry, entt::entity entity)
	{
		if (Component* value = registry.try_get<Component>(entity))
		{
			constexpr refl::type_descriptor descriptor = refl::reflect<Component>();
			const char* name = get_display_name(descriptor);

			if (ImGui::CollapsingHeader(name))
			{
				ImGui::Indent();
				for_each(refl::reflect<Component>().members, [&](auto field)
				{
					auto& fieldDescriptor = field;
					auto& fieldValue = field(*value);
					editor::Field(fieldDescriptor, fieldValue);
				});
				ImGui::Unindent();
			}
		}
	}

	template <typename ...Types>
	void InspectComponents(entt::registry& registry, entt::entity entity, core::TypeList<Types...> typeList)
	{
		(InspectComponent<Types>(registry, entity), ...);
	}
}

REFL_AUTO
(
	type(ExampleComponent)
	, field(m_Bool, field::Name("Boolean"))
	, field(m_Int, field::Name("Integer"))
	, field(m_Float, field::Name("Float"))
	, field(m_Vector3, field::Name("Vector3"))
	, field(m_Map, field::Name("Map"))
	, field(m_Vector, field::Name("Vector"))
)

editor::Inspector::Inspector()
{
}

editor::Inspector::~Inspector()
{
}

void editor::Inspector::Initialize(entt::registry& registry)
{
	m_Entity = registry.create();
	registry.emplace<ExampleComponent>(m_Entity);
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
		ExampleComponent
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
