#include "Editor/Inspector.h"

#include "Editor/PropertyDefines.h"
#include "Editor/PropertyWidgets.h"

#include <Engine/LevelComponent.h>
#include <Engine/NameComponent.h>
#include <Engine/ResourceManager.h>
#include <Engine/TransformComponent.h>

#include <iostream>
#include <entt/entt.hpp>
#include <imgui/imgui.h>
#include <SFML/System/Time.hpp>

//#define REFLECT_CLASS(class_name, pretty_name) \
//entt::meta<class_name>() \
//	.type(#class_name""_hs) \
//	.prop(strName, pretty_name) \
//
//#define REFLECT_MEMBER(class_name, member_name, pretty_name) \
//.data<&class_name::member_name>(#class_name"::"#member_name""_hs) \
//	.prop(strName, pretty_name) \
//	.prop(strOffset, offsetof(class_name, member_name)) \

#define metaid(type) entt::type_info<type>::id()

#define typeof_member(member) std::remove_all_extents<decltype(member)>::type
#define typeid_member(member) entt::type_info<typeof_member(member)>::id()

#define sizeof_member(member) sizeof(typeof_member(member))

#define void_pointer(value) (*((char**)(&value)))

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
		std::vector<POD> m_Vector = { s_Sample, s_Sample, s_Sample };
	};

	struct TContainers
	{
		float m_Array[3] = { 1.f, 2.f, 3.f };
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

	void* GetVoidComponent(entt::registry& registry, const entt::entity& entity, const entt::id_type& componentId)
	{
		switch (componentId)
		{
		case entt::type_info<core::TransformComponent>::id():
			return &registry.get<core::TransformComponent>(entity);
		case entt::type_info<POD>::id():
			return &registry.get<POD>(entity);
		case entt::type_info<Combined>::id():
			return &registry.get<Combined>(entity);
		case entt::type_info<CContainers>::id():
			return &registry.get<CContainers>(entity);
		case entt::type_info<TContainers>::id():
			return &registry.get<TContainers>(entity);
		}

		return nullptr;
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
	registry.emplace<Combined>(m_Entity);
	registry.emplace<core::TransformComponent>(m_Entity);

	entt::meta<POD>()
		.type("POD"_hs)
		.prop(strName, "POD")
	.data<&POD::m_Float>("POD::m_Float"_hs)
		.prop(strName, "m_Float")
		.prop(strOffset, offsetof(POD, m_Float))
	.data<&POD::m_Int>("POD::m_Int"_hs)
		.prop(strName, "m_Int")
		.prop(strOffset, offsetof(POD, m_Int))
	.data<&POD::m_Bool>("POD::m_Bool"_hs)
		.prop(strName, "m_Bool")
		.prop(strOffset, offsetof(POD, m_Bool));

	entt::meta<CContainers>()
		.type("CContainers"_hs)
		.prop(strName, "CContainers")
	.data<&CContainers::m_Array>("CContainers::m_Array"_hs)
		.prop(strName, "m_Array")
		.prop(strOffset, offsetof(CContainers, m_Array))
		.prop(strStride, sizeof_member(CContainers::m_Array));

	entt::meta<TContainers>()
		.type("TContainers"_hs)
		.prop(strName, "TContainers")
	.data<&TContainers::m_Array>("TContainers::m_Array"_hs)
		.prop(strName, "m_Array")
		.prop(strOffset, offsetof(TContainers, m_Array))
		.prop(strStride, sizeof_member(TContainers::m_Array))
	.data<&TContainers::m_Vector>("TContainers::m_Vector"_hs)
		.prop(strName, "m_Vector")
		.prop(strOffset, offsetof(TContainers, m_Vector))
		.prop(strStride, sizeof_member(TContainers::m_Vector));

	entt::meta<Combined>()
		.type("Combined"_hs)
		.prop(strName, "Combined")
	.data<&Combined::m_Pod>("Combined::m_Pod"_hs)
		.prop(strName, "m_Pod")
		.prop(strOffset, offsetof(Combined, m_Pod))
	.data<&Combined::m_CContainers>("Combined::m_CContainers"_hs)
		.prop(strName, "m_CContainers")
		.prop(strOffset, offsetof(Combined, m_CContainers))
	.data<&Combined::m_TContainers>("Combined::m_TContainers"_hs)
		.prop(strName, "m_TContainers")
		.prop(strOffset, offsetof(Combined, m_TContainers));

	entt::meta<sf::Vector3f>()
		.type("sf::Vector3f"_hs)
		.prop(strName, "Vector3")
		.func<&editor::PropertyWidget<sf::Vector3f>>(strCustom)
	.data<&sf::Vector3f::z>("&sf::Vector3f::z"_hs)
		.prop(strName, "Z")
		.prop(strOffset, offsetof(sf::Vector3f, z))
	.data<&sf::Vector3f::y>("&sf::Vector3f::y"_hs)
		.prop(strName, "Y")
		.prop(strOffset, offsetof(sf::Vector3f, y))
	.data<&sf::Vector3f::x>("&sf::Vector3f::x"_hs)
		.prop(strName, "X")
		.prop(strOffset, offsetof(sf::Vector3f, x));

	entt::meta<core::TransformComponent>()
		.type("core::TransformComponent"_hs)
		.prop(strName, "Transform Component")
	.data<&core::TransformComponent::m_Scale>("&core::TransformComponent::m_Scale"_hs)
		.prop(strName, "Scale")
		.prop(strOffset, offsetof(core::TransformComponent, m_Scale))
	.data<&core::TransformComponent::m_Rotate>("&core::TransformComponent::m_Rotate"_hs)
		.prop(strName, "Rotate")
		.prop(strOffset, offsetof(core::TransformComponent, m_Rotate))
	.data<&core::TransformComponent::m_Translate>("&core::TransformComponent::m_Translate"_hs)
		.prop(strName, "Translate")
		.prop(strOffset, offsetof(core::TransformComponent, m_Translate));
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

void Render_Child(void* data, const entt::meta_type& metaType, const entt::meta_data& metaData = nullptr)
{
	if (!metaType)
		return;

	const char* name = editor::PropertyName(metaData, nullptr);
	const bool isSkipHeader = !name;

	if (const entt::meta_func& funcCustom = metaType.func(editor::strCustom))
	{
		editor::Void wrapper = { data };
		entt::meta_handle handle = { };
		funcCustom.invoke(handle, wrapper, metaData);
	}
	else if (metaType.is_array())
	{
		// #todo: multi-dimensional arrays, use entt::meta_type::rank
		const size_t stride = metaData.prop(editor::strStride).value().cast<size_t>();
		const size_t extent = metaType.extent();
		const entt::meta_type& childMetaType = metaType.remove_extent();

		const bool isChildAClass = childMetaType.is_class();

		if (isSkipHeader || ImGui::CollapsingHeader(name))
		{
			ImGui::Indent();
			for (int i = 0; i < extent; ++i)
			{
				// #todo: multi-dimensional arrays, also advance by array dimension
				void* childData = void_pointer(data) + (stride * i);

				ImGui::PushID(i);
				if (isChildAClass)
				{
					const str::String index = std::to_string(i);
					if (ImGui::CollapsingHeader(index.c_str()))
						Render_Child(childData, childMetaType);
				}
				else
				{
					const str::String index = std::to_string(i) + ": ";
					ImGui::Text(index.c_str());
					ImGui::SameLine();
					Render_Child(childData, childMetaType);
				}
				ImGui::PopID();
			}
			ImGui::Unindent();
		}
	}
	else if (metaType.is_associative_container())
	{
		ImGui::CollapsingHeader("is_associative_container", ImGuiTreeNodeFlags_Bullet);
	}
	else if (metaType.is_sequence_container())
	{
		ImGui::CollapsingHeader("is_sequence_container", ImGuiTreeNodeFlags_Bullet);
	}
	else if (metaType.is_class())
	{
		if (isSkipHeader || ImGui::CollapsingHeader(name))
		{
			for (const entt::meta_data& childMetaData : metaType.data())
			{
				const char* childName = editor::PropertyName(childMetaData);
				const size_t childOffset = editor::PropertyOffset(childMetaData);
				void* childData = void_pointer(data) + childOffset;

				ImGui::PushID(childName);
				ImGui::Indent();
				Render_Child(childData, childMetaData.type(), childMetaData);
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

void Render_Component(entt::registry& registry, const entt::entity& entity, const entt::id_type& componentId)
{
	if (const entt::meta_type& metaType = entt::resolve_type(componentId))
	{
		const char* name = editor::PropertyName(metaType);
		void* data = GetVoidComponent(registry, entity, componentId);

		ImGui::PushID(static_cast<int>(componentId));
		if (!name || ImGui::CollapsingHeader(name))
			Render_Child(data, metaType);
		ImGui::PopID();
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
				Render_Component(registry, m_Entity, componentId);
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
		if (ImGui::BeginMenu("(dummy)"))
		{
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}
}
