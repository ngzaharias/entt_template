#pragma once

#include "Editor/ContainerWidgets.h"
#include "Editor/InspectorWidgets.h"
#include "Editor/TrivialWidgets.h"

#include <Engine/AttributeHelpers.h>
#include <Engine/TypeHelpers.h>
#include <Engine/VariantHelpers.h>

#include <imgui/Custom.h>
#include <imgui/Helpers.h>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

// #todo: vector:
// - re-orderable elements
// - append new
// - clear contents
// - element options: insert, delete

// #todo: map:
// - customize isKeyInlined && !isValInlined
// - append new
// - clear contents
// - element options: insert, delete
// - rename keys
// - maintain key order after rename

namespace
{
	// inline
	// left only
	template<typename Type>
	void TypeInContainer_v1(const int column, Type& value)
	{
		using TypeDescriptor = refl::type_descriptor<Type>;
		constexpr TypeDescriptor typeDescriptor = refl::reflect<Type>();

		// type
		imgui::SetColumnIndex(column);
		widget::TypeAsIs(value);
	}

	// inline
	// left and right
	template<typename Type>
	void TypeInContainer_v2(const char* name, Type& value)
	{
		using TypeDescriptor = refl::type_descriptor<Type>;
		constexpr TypeDescriptor typeDescriptor = refl::reflect<Type>();

		// label
		imgui::SetColumnIndex(0);
		ImGui::Text("%s (%s)", name, typeDescriptor.name.c_str());

		// type
		imgui::SetColumnIndex(1);
		widget::TypeAsIs(value);
	}

	// not inline
	// left and right
	template<typename Type>
	void TypeInContainer_v3(const char* name, Type& key)
	{
		using TypeDescriptor = refl::type_descriptor<Type>;
		constexpr TypeDescriptor typeDescriptor = refl::reflect<Type>();

		// left
		imgui::SetColumnIndex(0);
		if (imgui::FieldHeader("%s (%s)", name, typeDescriptor.name.c_str()))
		{
			widget::TypeAsIs(key);
		}

		// right
		imgui::SetColumnIndex(1);
		ImGui::Text("%d Members", typeDescriptor.members.size);
	}
}

//////////////////////////////////////////////////////////////////////////

template<class Container>
void widget::FieldAsContainer(const char* text, Container& container)
{
	ImGui::Text("Unsupported Container Type!");
}

template<class Key, class Value>
void widget::FieldAsContainer(const char* text, std::map<Key, Value>& container)
{
	using Iterator = typename std::map<Key, Value>::iterator;
	struct Rename { Iterator itr; Key key; };

	if constexpr (!refl::trait::is_reflectable<Key>::value || !refl::trait::is_reflectable<Value>::value)
		return;

	Rename rename = { container.end(), Key() };

	imgui::SetColumnIndex(0);
	bool isExpanded = imgui::FieldHeader(text);

	imgui::SetColumnIndex(1);
	ImGui::Text("%d Map Elements", container.size());

	if (isExpanded)
	{
		imgui::SetColumnIndex(0);
		imgui::Indent_x(2);

		constexpr bool isKeyInlined = !std::is_class<Key>::value;
		constexpr bool isValInlined = !std::is_class<Value>::value;

		auto itr = container.begin();
		auto end = container.end();
		for (int i = 0, id = 0; itr != end; ++itr, ++i)
		{
			Key key = itr->first;
			Value& value = itr->second;

			// key
			ImGui::PushID(++id);
			if (isKeyInlined && isValInlined)
			{
				imgui::SetColumnIndex(0);
				widget::TypeAsIs(key);
			}
			if (isKeyInlined && !isValInlined)
				editor::Field("Key: ", key);
			if (!isKeyInlined && isValInlined)
				editor::Field("Key: ", key);
			if (!isKeyInlined && !isValInlined)
				editor::Field("Key: ", key);
			ImGui::PopID();

			// value
			ImGui::PushID(++id);
			if (isKeyInlined && isValInlined)
			{
				imgui::SetColumnIndex(1);
				widget::TypeAsIs(value);
			}
			if (isKeyInlined && !isValInlined)
				editor::Field("Value: ", value);
			if (!isKeyInlined && isValInlined)
				editor::Field("Value: ", value);
			if (!isKeyInlined && !isValInlined)
				editor::Field("Value: ", value);
			ImGui::PopID();

			if (key != itr->first)
				rename = { itr, key };
		}

		imgui::SetColumnIndex(0);
		imgui::Unindent_x(2);
	}
}

template<typename Type>
void widget::FieldAsContainer(const char* text, std::vector<Type>& container)
{
	using TypeDescriptor = refl::type_descriptor<Type>;
	constexpr TypeDescriptor typeDescriptor = refl::reflect<Type>();

	if constexpr (!refl::trait::is_reflectable<Type>::value)
		return;

	imgui::SetColumnIndex(0);
	bool isExpanded = imgui::FieldHeader(text);

	imgui::SetColumnIndex(1);
	ImGui::Text("%d Array Elements", container.size());

	if (isExpanded)
	{
		imgui::SetColumnIndex(0);
		imgui::Indent_x(2);

		auto itr = container.begin();
		auto end = container.end();
		for (int i = 0; itr != end; ++itr, ++i)
		{
			Type& value = *itr;
			str::String label = std::to_string(i);

			editor::Field(label.c_str(), value);
		}

		imgui::SetColumnIndex(0);
		imgui::Unindent_x(2);
	}
}
