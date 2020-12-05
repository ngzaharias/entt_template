#include "Editor/Inspector.h"

#include "Editor/PropertyDefines.h"
#include "Editor/PropertyWidgets.h"

#include <Engine/CircularBuffer.h>
#include <Engine/LevelComponent.h>
#include <Engine/NameComponent.h>
#include <Engine/ResourceManager.h>
#include <Engine/StringTable.h>
#include <Engine/TransformComponent.h>

#include <iostream>
#include <entt/entt.hpp>
#include <entt/meta/container.hpp>
#include <imgui/imgui.h>
#include <SFML/System/Time.hpp>

// #todo: move somewhere else? they could be quite useful 
#define metaid(type) entt::type_info<type>::id()
#define typeof_member(member) std::remove_all_extents<decltype(member)>::type
#define typeid_member(member) entt::type_info<typeof_member(member)>::id()
#define sizeof_member(member) sizeof(typeof_member(member))

namespace
{
	struct ExampleStruct
	{
		bool m_Bool;
		int m_Int;
		float m_Float;

		bool operator==(const ExampleStruct&) const { return false; }
	};

	struct ExampleComponent
	{
		std::map<int, float> m_MapA = { {1, 1.f}, {2, 2.f}, {3, 3.f} };
		std::map<int, ExampleStruct> m_MapB = { {1, ExampleStruct()} };
		std::vector<int> m_VectorA = { 0, 1, 2, 3, 4, 5 };
		std::vector<ExampleStruct> m_VectorB = { ExampleStruct() };
	};

	// #todo: we should be able to get this as a func or as a part of the register
	// 
	//	registry.visit(m_Entity, [&](auto comp)
	//	{
	//		entt::resolve(comp).func("get"_hs).invoke({}, registry, entity);
	//	}
	// 
	entt::meta_any GetComponent(entt::registry& registry, const entt::entity& entity, const entt::id_type& componentId)
	{
		switch (componentId)
		{
		case entt::type_info<ExampleComponent>::id():
			return std::ref(registry.get<ExampleComponent>(entity));
		case entt::type_info<core::TransformComponent>::id():
			return std::ref(registry.get<core::TransformComponent>(entity));
		}

		return { };
	};
}

editor::Inspector::Inspector(core::ResourceManager& resourceManager)
	: m_ResourceManager(resourceManager)
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

	entt::meta<bool>().ctor<>()
		.type("bool"_hs)
		.prop(core::strName, "Boolean")
		.func<&editor::PropertyWidget<bool>>(core::strInspector);

	entt::meta<int>().ctor<>()
		.type("int"_hs)
		.prop(core::strName, "Integer")
		.func<&editor::PropertyWidget<int>>(core::strInspector);

	entt::meta<float>().ctor<>()
		.type("float"_hs)
		.prop(core::strName, "Floating Point")
		.func<&editor::PropertyWidget<float>>(core::strInspector);

	entt::meta<sf::Vector3f>().ctor<>()
		.type("sf::Vector3f"_hs)
		.prop(core::strName, "Vector3")
		.func<&editor::PropertyWidget<sf::Vector3f>>(core::strInspector);

	entt::meta<ExampleStruct>().ctor<>()
		.type("ExampleStruct"_hs)
		.prop(core::strName, "Example Struct")
	.data<&ExampleStruct::m_Bool, entt::as_ref_t>("ExampleStruct::m_Bool"_hs)
		.prop(core::strName, "m_Bool")
	.data<&ExampleStruct::m_Int, entt::as_ref_t>("ExampleStruct::m_Int"_hs)
		.prop(core::strName, "m_Int")
	.data<&ExampleStruct::m_Float, entt::as_ref_t>("ExampleStruct::m_Float"_hs)
		.prop(core::strName, "m_Float");

	entt::meta<ExampleComponent>().ctor<>()
		.type("ExampleComponent"_hs)
		.prop(core::strName, "Example Component")
	.data<&ExampleComponent::m_MapA, entt::as_ref_t>("ExampleComponent::m_MapA"_hs)
		.prop(core::strName, "m_MapA")
	.data<&ExampleComponent::m_MapB, entt::as_ref_t>("ExampleComponent::m_MapB"_hs)
		.prop(core::strName, "m_MapB")
	.data<&ExampleComponent::m_VectorA, entt::as_ref_t>("ExampleComponent::m_VectorA"_hs)
		.prop(core::strName, "m_VectorA")
	.data<&ExampleComponent::m_VectorB, entt::as_ref_t>("ExampleComponent::m_VectorB"_hs)
		.prop(core::strName, "m_VectorB");

	entt::meta<core::TransformComponent>().ctor<>()
		.type("core::TransformComponent"_hs)
		.prop(core::strName, "Transform Component")
	.data<&core::TransformComponent::m_Scale, entt::as_ref_t>("core::TransformComponent::m_Scale"_hs)
		.prop(core::strName, "Scale")
	.data<&core::TransformComponent::m_Rotate, entt::as_ref_t>("core::TransformComponent::m_Rotate"_hs)
		.prop(core::strName, "Rotate")
	.data<&core::TransformComponent::m_Translate, entt::as_ref_t>("core::TransformComponent::m_Translate"_hs)
		.prop(core::strName, "Translate");
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
	Render_Entity(registry);

	ImGui::End();
}

void editor::Inspector::Render_Entity(entt::registry& registry)
{
	if (ImGui::BeginChild("entity"))
	{
		if (registry.valid(m_Entity))
		{
			ImGui::PushID(static_cast<int>(m_Entity));

			registry.visit(m_Entity, [&](const entt::id_type& componentId)
			{
				if (const entt::meta_type& metaType = entt::resolve_type(componentId))
				{
					const char* name = editor::PropertyName(metaType);
					entt::meta_any data = GetComponent(registry, m_Entity, componentId);

					ImGui::PushID(static_cast<int>(componentId));
					if (!name || ImGui::CollapsingHeader(name))
						PropertyWidget_Child(data);
					ImGui::PopID();
				}
			});

			ImGui::PopID();
		}
		ImGui::EndChild();
	}
}

void editor::Inspector::Render_MenuBar(entt::registry& registry)
{
	if (ImGui::BeginMenuBar())
	{
		ImGui::EndMenuBar();
	}
}
