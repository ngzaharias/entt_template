#pragma once

#include "Editor/ContainerWidgets.h"
#include "Editor/InspectorWidgets.h"
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
	template <typename T>
	using overload = decltype(widget::TypeAsIs(std::declval<T&>()));

	template <typename T, typename = void>
	struct is_overloaded : std::false_type {};

	template <typename T>
	struct is_overloaded<T, std::void_t<decltype(widget::TypeAsIs(std::declval<T&>()))>> : std::true_type {};
}

template<typename Type>
void editor::InspectType(Type& value)
{
	constexpr bool isClass = std::is_class<Type>::value;
	constexpr bool isReflectable = refl::trait::is_reflectable<Type>::value;
	constexpr bool isOverloaded = is_overloaded<Type>();
	if constexpr (isClass && isReflectable)
	{
		for_each(refl::reflect<Type>().members, [&](auto field)
		{
			constexpr const char* name = reflect::GetFieldName(field);
			editor::InspectField(name, field(value));
			ImGui::TableNextRow();
		});
	}
	else if constexpr (isOverloaded)
	{
		widget::TypeAsIs(value);
	}
	else
	{
		ImGui::Text("Unsupported Type!");
	}
}

template<typename Type>
void editor::InspectField(const char* text, Type& value)
{
	ImGui::PushID(text);

	constexpr bool isClass = std::is_class<Type>::value;
	constexpr bool isContainer = refl::trait::is_container<Type>::value;
	constexpr bool isOverloaded = is_overloaded<Type>();
	constexpr bool isReflectable = refl::trait::is_reflectable<Type>::value;
	constexpr bool isVariant = core::IsVariant<Type>::value;

	if constexpr (isOverloaded)
	{
		ImGui::TableSetColumnIndex(0);
		imgui::Bullet();
		ImGui::Text(text);

		ImGui::TableSetColumnIndex(1);
		widget::TypeAsIs(value);
	}
	else if constexpr (isClass && isReflectable)
	{
		editor::FieldAsClass(text, value);
	}
	else if constexpr (isVariant)
	{
		std::visit([&](auto& subValue)
		{
			widget::FieldAsVariant(text, value, subValue);
		}, value);
	}
	else if constexpr (isContainer)
	{
		// #note: we only reflect the container content type which is done internally
		widget::FieldAsContainer(text, value);
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
template<typename Type>
void editor::FieldAsClass(const char* text, Type& value)
{
	using TypeDescriptor = refl::type_descriptor<Type>;
	constexpr TypeDescriptor typeDescriptor = refl::reflect<Type>();

	ImGui::TableSetColumnIndex(0);
	bool isExpanded = imgui::FieldHeader(text);

	ImGui::TableSetColumnIndex(1);
	ImGui::Text("%d Members", typeDescriptor.members.size);

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