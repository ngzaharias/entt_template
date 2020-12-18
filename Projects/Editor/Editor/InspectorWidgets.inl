#pragma once

#include "Editor/ContainerWidgets.h"
#include "Editor/InspectorWidgets.h"
#include "Editor/TrivialWidgets.h"
#include "Editor/VariantWidgets.h"

#include <imgui/Custom.h>
#include <imgui/Helpers.h>
#include <imgui/imgui.h>
#include <refl/refl.hpp>

// #todo: contains:
// - re-orderable elements
// - append new
// - clear contents
// - element options: insert, delete

// #todo: maps:
// - maintain key order after rename

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
	constexpr bool isClass = std::is_class<Type>::value;
	constexpr bool isContainer = refl::trait::is_container<Type>::value;
	constexpr bool isReflectable = refl::trait::is_reflectable<Type>::value;
	constexpr bool isVariant = core::IsVariant<Type>::value;

	ImGui::PushID(text);
	if constexpr (isClass && isReflectable)
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
		// #note: containers aren't reflectable, so they will need to check internally
		widget::FieldAsContainer(text, value);
	}
	else
	{
		imgui::SetColumnIndex(0);
		ImGui::Text(text);

		imgui::SetColumnIndex(1);
		widget::TypeAsIs(value);
	}
	ImGui::PopID();
}

template<typename Type>
void editor::FieldAsClass(const char* text, Type& value)
{
	// variations:
	//
	// v Element X	: X Members
	// |   m_Member : Value
	//
	// v FieldName	: X Members
	// |   m_Member : Value

	using TypeDescriptor = refl::type_descriptor<Type>;
	constexpr TypeDescriptor typeDescriptor = refl::reflect<Type>();

	imgui::SetColumnIndex(0);
	bool isExpanded = imgui::FieldHeader(text);

	imgui::SetColumnIndex(1);
	ImGui::Text("%d Members", typeDescriptor.members.size);

	if (isExpanded)
	{
		imgui::SetColumnIndex(0);
		imgui::Indent_x(2);

		widget::TypeAsIs(value);

		imgui::SetColumnIndex(0);
		imgui::Unindent_x(2);
	}
}


/*

MyComponent
   m_Bool
   m_Struct
 |   m_Int 
 v m_Other
 |   m_Float
 v m_VectorA
 | v Element 0
 | |   m_Bool
 | |   m_Int
 | v Element 1
 | |   m_Bool
 | |   m_Int
 v m_VectorB
 |   Element 0
 |   Element 1
 |   Element 2
 v m_Map
 |   Key
 |   Value
 
 
 
 
 
 


 
 
 
 
 
*/