#pragma once

#include "Editor/ContainerWidgets.h"
#include "Editor/InspectorWidgets.h"
#include "Editor/ResourceWidgets.h"
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

template<typename Type>
void editor::Field(const char* text, Type& value)
{
	ImGui::PushID(text);
	if constexpr (std::is_class<Type>::value && refl::trait::is_reflectable<Type>::value)
	{
		editor::FieldAsClass(text, value);
	}
	else if constexpr (core::IsVariant<Type>::value)
	{
		std::visit([&](auto& subValue)
		{
			widget::FieldAsVariant(text, value, subValue);
		}, value);
	}
	else if constexpr (refl::trait::is_container<Type>::value)
	{
		// #note: containers aren't reflectable, so they will need to check internally
		widget::FieldAsContainer(text, value);
	}
	else
	{
		imgui::SetColumnIndex(0);
		imgui::Bullet();
		ImGui::Text(text);

		imgui::SetColumnIndex(1);
		widget::TypeAsIs(value);
	}
	ImGui::PopID();
}

// variations:
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

	imgui::SetColumnIndex(0);
	bool isExpanded = imgui::FieldHeader(text);

	imgui::SetColumnIndex(1);
	ImGui::Text("%d Members", typeDescriptor.members.size);

	if (isExpanded)
	{
		imgui::SetColumnIndex(0);
		ImGui::Indent();

		widget::TypeAsIs(value);

		imgui::SetColumnIndex(0);
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