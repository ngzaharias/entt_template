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

#define metaid(type) entt::type_info<type>::id()
#define typeof_member(member) std::remove_all_extents<decltype(member)>::type
#define typeid_member(member) entt::type_info<typeof_member(member)>::id()
#define sizeof_member(member) sizeof(typeof_member(member))

namespace
{
	struct POD
	{
		bool m_Bool = true;
		int m_Int = 666;
		float m_Float = 1337.f;
	};

	struct CContainers
	{
		static constexpr POD s_Sample = { };

		POD m_Array[3] = { s_Sample, s_Sample, s_Sample };
		CircularBuffer<POD, 3> m_CircleBuffer;
		std::map<int, POD> m_Map = { { 1, s_Sample }, { 2, s_Sample }, { 3, s_Sample } };
		std::vector<POD> m_Vector = { s_Sample, s_Sample, s_Sample };
	};

	struct TContainers
	{
		float m_Array[3] = { 1.f, 2.f, 3.f };
		CircularBuffer<float, 3> m_CircleBuffer;
		std::map<int, float> m_Map = { { 1, 1.f }, { 2, 2.f }, { 3, 3.f } };
		std::vector<float> m_Vector = { 1.f, 2.f, 3.f };
	};

	struct Combined
	{
		POD m_Pod;
		CContainers m_CContainers;
		TContainers m_TContainers;
	};

	bool HasComponent(const entt::registry& registry, const entt::entity& entity, const entt::id_type& typeId)
	{
		entt::id_type types[] = { typeId };
		return registry.runtime_view(std::cbegin(types), std::cend(types)).contains(entity);
	}

	entt::meta_any GetComponent(entt::registry& registry, const entt::entity& entity, const entt::id_type& componentId)
	{
		switch (componentId)
		{
		case entt::type_info<core::TransformComponent>::id():
			return registry.get<core::TransformComponent>(entity);
		case entt::type_info<POD>::id():
			return registry.get<POD>(entity);
		case entt::type_info<Combined>::id():
			return registry.get<Combined>(entity);
		case entt::type_info<CContainers>::id():
			return registry.get<CContainers>(entity);
		case entt::type_info<TContainers>::id():
			return registry.get<TContainers>(entity);
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
	//registry.emplace<Combined>(m_Entity);
	registry.emplace<core::TransformComponent>(m_Entity);

	entt::meta<POD>()
		.type("POD"_hs)
		.prop(core::strName, "POD")
	.data<&POD::m_Float>("POD::m_Float"_hs)
		.prop(core::strName, "m_Float")
	.data<&POD::m_Int>("POD::m_Int"_hs)
		.prop(core::strName, "m_Int")
	.data<&POD::m_Bool>("POD::m_Bool"_hs)
		.prop(core::strName, "m_Bool");

	entt::meta<CContainers>()
		.type("CContainers"_hs)
		.prop(core::strName, "CContainers")
	.data<&CContainers::m_Array>("CContainers::m_Array"_hs)
		.prop(core::strName, "m_Array");
	/*.data<&CContainers::m_Map>("CContainers::m_Map"_hs)
		.prop(core::strName, "m_Map");*/
	//.data<&CContainers::m_Vector>("CContainers::m_Vector"_hs)
	//	.prop(core::strName, "m_Vector");

	entt::meta<TContainers>()
		.type("TContainers"_hs)
		.prop(core::strName, "TContainers")
		// #todo: arrays aren't working, 
		// issue within entt when converting meta_any -> meta_handle
	.data<&TContainers::m_Array>("TContainers::m_Array"_hs)
		.prop(core::strName, "m_Array")
	//.data<&TContainers::m_CircleBuffer>("TContainers::m_CircleBuffer"_hs)
	//	.prop(core::strName, "m_CircleBuffer")
	.data<&TContainers::m_Map>("TContainers::m_Map"_hs)
		.prop(core::strName, "m_Map")
	.data<&TContainers::m_Vector>("TContainers::m_Vector"_hs)
		.prop(core::strName, "m_Vector");

	entt::meta<Combined>()
		.type("Combined"_hs)
		.prop(core::strName, "Combined")
	.data<&Combined::m_Pod>("Combined::m_Pod"_hs)
		.prop(core::strName, "m_Pod")
	.data<&Combined::m_CContainers>("Combined::m_CContainers"_hs)
		.prop(core::strName, "m_CContainers")
	.data<&Combined::m_TContainers>("Combined::m_TContainers"_hs)
		.prop(core::strName, "m_TContainers");

	entt::meta<sf::Vector3f>()
		.type("sf::Vector3f"_hs)
		.prop(core::strName, "Vector3")
		.func<&editor::PropertyWidget<sf::Vector3f>>(core::strCustomInspector)
	.data<&sf::Vector3f::x>("&sf::Vector3f::x"_hs)
		.prop(core::strName, "X")
	.data<&sf::Vector3f::y>("&sf::Vector3f::y"_hs)
		.prop(core::strName, "Y")
	.data<&sf::Vector3f::z>("&sf::Vector3f::z"_hs)
		.prop(core::strName, "Z");

	entt::meta<core::TransformComponent>()
		.type("core::TransformComponent"_hs)
		.prop(core::strName, "Transform Component")
	.data<&core::TransformComponent::m_Scale>("&core::TransformComponent::m_Scale"_hs)
		.prop(core::strName, "Scale")
	.data<&core::TransformComponent::m_Rotate>("&core::TransformComponent::m_Rotate"_hs)
		.prop(core::strName, "Rotate")
	.data<&core::TransformComponent::m_Translate>("&core::TransformComponent::m_Translate"_hs)
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

void Render_Child(entt::meta_any data, const entt::meta_data& metaData = nullptr)
{
	const entt::meta_type& metaType = data.type();
	if (!metaType)
		return;

	const char* name = editor::PropertyName(metaData, nullptr);
	const bool isSkipHeader = !name;

	if (const entt::meta_func& funcCustom = metaType.func(core::strCustomInspector))
	{
		// #todo, entt doesn't handle entt::meta_any as an arg?
		entt::meta_handle handle = { };
		editor::Any wrapper = { data };
		funcCustom.invoke(handle, wrapper, metaData);
	}
	else if (metaType.is_array())
	{
		// #todo: multi-dimensional arrays, use entt::meta_type::rank
		const entt::meta_type& childMetaType = metaType.remove_extent();
		const bool isChildAClass = childMetaType.is_class();

		if (isSkipHeader || ImGui::CollapsingHeader(name))
		{
			int i = 0;
			ImGui::Indent();
			for (auto itr : data.as_sequence_container())
			{
				ImGui::PushID(++i);
				Render_Child(itr);
				ImGui::PopID();
			}
			ImGui::Unindent();
		}
	}
	else if (metaType.is_associative_container())
	{
		const entt::meta_type& childMetaType = metaType.remove_extent();
		const bool isChildAClass = childMetaType.is_class();

		if (isSkipHeader || ImGui::CollapsingHeader(name))
		{
			int i = 0;
			ImGui::Indent();
			for (auto itr : data.as_associative_container())
			{
				ImGui::PushID(++i);
				ImGui::Text("Key:   ");
				ImGui::SameLine();
				Render_Child(itr.first);
				ImGui::PopID();

				ImGui::PushID(++i);
				ImGui::Text("Value: ");
				ImGui::SameLine();
				Render_Child(itr.second);
				ImGui::PopID();

				ImGui::Separator();
			}
			ImGui::Unindent();
		}
	}
	else if (metaType.is_sequence_container())
	{
		const entt::meta_type& childMetaType = metaType.remove_extent();
		const bool isChildAClass = childMetaType.is_class();

		if (isSkipHeader || ImGui::CollapsingHeader(name))
		{
			ImGui::Indent();

			int i = 0;
			for (auto itr : data.as_sequence_container())
			{
				ImGui::PushID(++i);
				if (isChildAClass)
				{
					const str::String index = std::to_string(i);
					if (ImGui::CollapsingHeader(index.c_str()))
					Render_Child(itr);
				}
				else
				{
					const str::String index = std::to_string(i) + ": ";
					ImGui::Text(index.c_str());
					ImGui::SameLine();
					Render_Child(itr);
				}
				ImGui::PopID();
			}
			ImGui::Unindent();
		}
	}
	else if (metaType.is_class())
	{
		if (isSkipHeader || ImGui::CollapsingHeader(name))
		{
			for (const entt::meta_data& childMetaData : metaType.data())
			{
				entt::meta_any childData = childMetaData.get(data);
				entt::meta_type childMetaType = childMetaData.type();

				const char* childName = editor::PropertyName(childMetaData);

				ImGui::PushID(childName);
				ImGui::Indent();
				Render_Child(childData, childMetaData);
				ImGui::Unindent();
				ImGui::PopID();
			}
		}
	}
	else if (metaType.has_boolean())
	{
		editor::PropertyWidget<bool>({ data }, metaData);
	}
	else if (metaType.has_integer())
	{
		editor::PropertyWidget<int>({ data }, metaData);
	}
	else if (metaType.has_float())
	{
		editor::PropertyWidget<float>({ data }, metaData);
	}
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
						Render_Child(data);
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
