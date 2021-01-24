#pragma once

#include "Editor/InspectorWidgets.h"

#include "Editor/ContainerWidgets.h"
#include "Editor/AssetWidgets.h"
#include "Editor/TrivialWidgets.h"
#include "Editor/VariantWidgets.h"

#include <imgui/Custom.h>
#include <imgui/Helpers.h>
#include <imgui/imgui.h>
#include <refl/refl.hpp>

// #todo: override check box if inherited

// #todo: undo/redo:
// - WindowID
// - Entity
// - ComponentID
// - Field ? stack of field id?
// - Value ? std::any?

namespace
{
	// #note: must be declared after the function
	template <typename A, typename B, typename = void>
	struct HasOverload : std::false_type {};
	template <typename A, typename B>
	struct HasOverload<A, B, std::void_t<decltype(widget::TypeOverload(std::declval<A>(), std::declval<B>()))>> : std::true_type {};

	template <typename Descriptor>
	widget::Properties GetProperties(Descriptor descriptor)
	{
		using namespace refl::descriptor;

		widget::Properties properties;
		if constexpr (has_attribute<prop::Name>(descriptor))
			properties.m_Name = get_attribute<prop::Name>(descriptor);
		if constexpr (has_attribute<prop::Range>(descriptor))
			properties.m_Range = get_attribute<prop::Range>(descriptor);
		return properties;
	}
}

template<typename Type>
void editor::InspectType(Type& value)
{
	constexpr bool isClass = std::is_class<Type>::value;
	constexpr bool isReflectable = refl::trait::is_reflectable<Type>::value;
	constexpr bool isOverloaded = HasOverload<Type&, widget::Properties>();
	if constexpr (isOverloaded)
	{
		widget::TypeOverload(value);
	}
	else if constexpr (isClass && isReflectable)
	{
		for_each(refl::reflect<Type>().members, [&](auto field)
		{
			constexpr const char* name = reflect::GetFieldName(field);
			editor::InspectMember(name, field(value), field);
			ImGui::TableNextRow();
		});
	}
	else
	{
		ImGui::Text("Unsupported Type!");
	}
}

template<typename Type, typename Descriptor>
void editor::InspectMember(const char* text, Type& value, Descriptor descriptor)
{
	ImGui::PushID(text);

	constexpr bool isClass = std::is_class<Type>::value;
	constexpr bool isContainer = refl::trait::is_container<Type>::value;
	constexpr bool isOverloaded = HasOverload<Type&, widget::Properties>();
	constexpr bool isReflectable = refl::trait::is_reflectable<Type>::value;
	constexpr bool isVariant = core::IsVariant<Type>::value;

	if constexpr (isOverloaded)
	{
		ImGui::TableSetColumnIndex(0);
		imgui::Bullet();
		ImGui::Text(text);

		ImGui::TableSetColumnIndex(1);
		widget::TypeOverload(value, GetProperties(descriptor));
	}
	else if constexpr (isClass && isReflectable)
	{
		editor::InspectClass(text, value, descriptor);
	}
	else if constexpr (isVariant)
	{
		std::visit([&](auto& subValue)
		{
			editor::InspectVariant(text, descriptor, value, subValue);
		}, value);
	}
	else if constexpr (isContainer)
	{
		// #note: we only reflect the container content type which is done internally
		editor::InspectContainer(text, descriptor, value);
	}
	else
	{
		ImGui::TableSetColumnIndex(0);
		imgui::Bullet();
		ImGui::Text(text);

		ImGui::TableSetColumnIndex(1);
		ImGui::Text("Unsupported Type!");
	}

	ImGui::PopID();
}

// Variations
//
// v Element X	: X Members
// |   m_Member : Value
//
// v FieldName	: X Members
// |   m_Member : Value
template<typename Type, typename Descriptor>
void editor::InspectClass(const char* text, Type& value, Descriptor descriptor)
{
	ImGui::TableSetColumnIndex(0);
	const bool isExpanded = imgui::InspectHeader(text);

	ImGui::TableSetColumnIndex(1);
	ImGui::Text("%d Members", refl::reflect<Type>().members.size);

	if (isExpanded)
	{
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::Indent();

		editor::InspectType(value);

		ImGui::TableSetColumnIndex(0);
		ImGui::Unindent();
	}
}

/*
MyComponent
- m_Bool
- m_Struct
- m_Int
v m_Other
 - m_Float
v m_VectorA
 v Element 0
  - m_Bool
  - m_Int
 > Element 1
 > Element 2
v m_VectorB
 - Element 0
 - Element 1
 - Element 2
v m_MapA
 - [Key]	| [Value]
v m_MapB
 - Key		| [Key]
 > Value
*/