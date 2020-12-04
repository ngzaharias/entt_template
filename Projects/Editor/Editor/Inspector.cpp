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
	entt::meta_any GetComponent(entt::registry& registry, const entt::entity& entity, const entt::id_type& componentId)
	{
		switch (componentId)
		{
		case entt::type_info<core::TransformComponent>::id():
			return registry.get<core::TransformComponent>(entity);
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
	registry.emplace<core::TransformComponent>(m_Entity);

	entt::meta<bool>()
		.type("bool"_hs)
		.prop(core::strName, "Boolean")
		.func<&editor::PropertyWidget<bool>>(core::strInspector);

	entt::meta<int>()
		.type("int"_hs)
		.prop(core::strName, "Integer")
		.func<&editor::PropertyWidget<int>>(core::strInspector);

	entt::meta<float>()
		.type("float"_hs)
		.prop(core::strName, "Floating Point")
		.func<&editor::PropertyWidget<float>>(core::strInspector);

	entt::meta<sf::Vector3f>()
		.type("sf::Vector3f"_hs)
		.prop(core::strName, "Vector3")
		.func<&editor::PropertyWidget<sf::Vector3f>>(core::strInspector);

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

	if (const entt::meta_func& funcCustom = metaType.func(core::strInspector))
	{
		// #todo, entt doesn't handle entt::meta_any as an arg?
		entt::meta_handle handle = { };
		editor::Any wrapper = { data };
		funcCustom.invoke(handle, wrapper, metaData);
	}
	else if (metaType.is_array())
	{
		// #todo: multi-dimensional arrays?
		const entt::meta_type& childMetaType = metaType.remove_extent();
		const bool isChildAClass = childMetaType.is_class();

		if (isSkipHeader || ImGui::CollapsingHeader(name))
		{
			int i = 0;
			ImGui::Indent();
			for (entt::meta_any childData : data.as_sequence_container())
			{
				ImGui::PushID(++i);
				Render_Child(childData);
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
			for (auto node : data.as_associative_container())
			{
				ImGui::PushID(++i);
				ImGui::Text("Key:   ");
				ImGui::SameLine();
				Render_Child(node.first);
				ImGui::PopID();

				ImGui::PushID(++i);
				ImGui::Text("Value: ");
				ImGui::SameLine();
				Render_Child(node.second);
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
			for (auto childData : data.as_sequence_container())
			{
				ImGui::PushID(++i);
				if (isChildAClass)
				{
					const str::String index = std::to_string(i);
					if (ImGui::CollapsingHeader(index.c_str()))
					Render_Child(childData);
				}
				else
				{
					const str::String index = std::to_string(i) + ": ";
					ImGui::Text(index.c_str());
					ImGui::SameLine();
					Render_Child(childData);
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
