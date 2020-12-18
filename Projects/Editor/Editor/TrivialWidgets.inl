#pragma once

#include "Editor/TrivialWidgets.h"

#include <Engine/AttributeHelpers.h>

#include <imgui/imgui.h>

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
void widget::TypeAsIs(Type& value)
{
	constexpr bool isClass = std::is_class<Type>::value;
	constexpr bool isReflectable = refl::trait::is_reflectable<Type>::value;

	if constexpr (isClass && isReflectable)
	{
		for_each(refl::reflect<Type>().members, [&](auto field)
		{
			constexpr const char* name = reflect::GetFieldName(field);
			editor::Field(name, field(value));
		});
	}
	else
	{
		ImGui::Text("Unsupported Type!");
	}
}

template<>
void widget::TypeAsIs(bool& value)
{
	ImGui::Checkbox("", &value);
}

template<>
void widget::TypeAsIs(int& value)
{
	ImGui::DragInt("", &value);
}

template<>
void widget::TypeAsIs(float& value)
{
	ImGui::DragFloat("", &value);
}

template<>
void widget::TypeAsIs(Vector3f& value)
{
	ImGui::DragFloat3("", &value.x);
}
