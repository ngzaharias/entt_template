#pragma once

#include "Editor/InspectorWidgets.h"

#include <Engine/AttributeHelpers.h>
#include <Engine/TypeHelpers.h>
#include <Engine/VariantHelpers.h>

#include <imgui/Custom.h>
#include <imgui/Helpers.h>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

namespace
{
	template<typename Type>
	void TypeAsKey_TrivialL(Type& key)
	{
		using TypeDescriptor = refl::type_descriptor<Type>;
		constexpr TypeDescriptor typeDescriptor = refl::reflect<Type>();

		// type
		imgui::SetColumnIndex(0);
		ImGui::Indent();
		editor::InspectorType(key);
		ImGui::Unindent();
	}

	template<typename Type>
	void TypeAsKey_TrivialLR(Type& key)
	{
		using TypeDescriptor = refl::type_descriptor<Type>;
		constexpr TypeDescriptor typeDescriptor = refl::reflect<Type>();

		// label
		imgui::SetColumnIndex(0);
		ImGui::Indent();
		ImGui::Text("Key   (%s)", typeDescriptor.name.c_str());
		ImGui::Unindent();

		// type
		imgui::SetColumnIndex(1);
		editor::InspectorType(key);
	}

	template<typename Type>
	void TypeAsKey_ComplexLR(Type& key)
	{
		using TypeDescriptor = refl::type_descriptor<Type>;
		constexpr TypeDescriptor typeDescriptor = refl::reflect<Type>();

		// label
		imgui::SetColumnIndex(0);
		bool isExpanded = widget::CollapsingHeader("Key   (%s)", typeDescriptor.name.c_str());

		// info
		imgui::SetColumnIndex(1);
		ImGui::Text("%d Members", typeDescriptor.members.size);

		// type
		if (isExpanded)
		{
			imgui::SetColumnIndex(0);
			ImGui::Indent();
			editor::InspectorType(key);
			ImGui::Unindent();
		}
	}

	template<typename Type>
	void TypeAsValue_TrivialR(Type& value)
	{
		using TypeDescriptor = refl::type_descriptor<Type>;
		constexpr TypeDescriptor typeDescriptor = refl::reflect<Type>();

		// type
		imgui::SetColumnIndex(1);
		editor::InspectorType(value);

		// #todo: add dropdown
	}

	template<typename Type>
	void TypeAsValue_TrivialLR(Type& value)
	{
		using TypeDescriptor = refl::type_descriptor<Type>;
		constexpr TypeDescriptor typeDescriptor = refl::reflect<Type>();

		// label
		imgui::SetColumnIndex(0);
		ImGui::Indent();
		ImGui::Text("Value (%s)", typeDescriptor.name.c_str());
		ImGui::Unindent();

		// type
		imgui::SetColumnIndex(1);
		editor::InspectorType(value);

		// #todo: add dropdown
	}

	template<typename Type>
	void TypeAsValue_ComplexLR(Type& value)
	{
		using TypeDescriptor = refl::type_descriptor<Type>;
		constexpr TypeDescriptor typeDescriptor = refl::reflect<Type>();

		// label
		imgui::SetColumnIndex(0);
		bool isExpanded = widget::CollapsingHeader("Value (%s)", typeDescriptor.name.c_str());

		// info
		imgui::SetColumnIndex(1);
		ImGui::Text("%d Members", typeDescriptor.members.size);
		// #todo: add dropdown

		// type
		if (isExpanded)
		{
			imgui::SetColumnIndex(0);
			ImGui::Indent();
			editor::InspectorType(value);
			ImGui::Unindent();
		}
	}

	template<typename FieldDescriptor, typename Type>
	void FieldAsClass(const FieldDescriptor fieldDescriptor, Type& value)
	{
		using TypeDescriptor = refl::type_descriptor<Type>;
		constexpr TypeDescriptor typeDescriptor = refl::reflect<Type>();
		constexpr const char* fieldName = reflect::GetFieldName(fieldDescriptor);

		imgui::SetColumnIndex(0);
		bool isExpanded = widget::CollapsingHeader(fieldName);

		imgui::SetColumnIndex(1);
		ImGui::Text("%d Members", typeDescriptor.members.size);

		if (isExpanded)
		{
			imgui::SetColumnIndex(0);
			ImGui::Indent();

			int32 i = 0;
			for_each(typeDescriptor.members, [&](auto member)
			{
				auto& childDescriptor = member;
				auto& childValue = member(value);

				ImGui::PushID(++i);
				editor::InspectorField(childDescriptor, childValue);
				ImGui::PopID();
			});

			imgui::SetColumnIndex(0);
			ImGui::Unindent();
		}
	}

	//////////////////////////////////////////////////////////////////////////

	template<typename FieldDescriptor, class Container>
	void FieldAsContainer(const FieldDescriptor fieldDescriptor, Container& container)
	{
		ImGui::Text("Unsupported Container Type!");
	}

	template<typename FieldDescriptor, class Key, class Value>
	void FieldAsContainer(const FieldDescriptor fieldDescriptor, std::map<Key, Value>& container)
	{
		constexpr const char* fieldName = reflect::GetFieldName(fieldDescriptor);
		if constexpr (!refl::trait::is_reflectable<Key>::value || !refl::trait::is_reflectable<Value>::value)
			return;

		imgui::SetColumnIndex(0);
		bool isExpanded = widget::CollapsingHeader(fieldName);

		imgui::SetColumnIndex(1);
		ImGui::Text("%d Map Elements", container.size());
		ImGui::SameLine();
		ImGui::Button("+"); // #todo
		ImGui::SameLine();
		ImGui::Button("x"); // #todo

		if (isExpanded)
		{
			const bool isKeyInlined = !std::is_class<Key>::value;
			const bool isValInlined = !std::is_class<Value>::value;

			auto itr = container.begin();
			auto end = container.end();
			for (int i = 0, id = 0; itr != end; ++itr, ++i)
			{
				Key key = itr->first;
				Value& value = itr->second;

				bool isElementExpanded = true;
				if (!isKeyInlined || !isValInlined)
				{
					imgui::SetColumnIndex(0);
					ImGui::Indent();
					isElementExpanded = widget::CollapsingHeader("Element %d", i);
					ImGui::Unindent();
				}

				if (isElementExpanded)
				{
					imgui::SetColumnIndex(0);
					ImGui::Indent();

					// key
					ImGui::PushID(++id);
					if (isKeyInlined && isValInlined)
						TypeAsKey_TrivialL(key);
					if (isKeyInlined && !isValInlined)
						TypeAsKey_TrivialLR(key);
					if (!isKeyInlined && isValInlined)
						TypeAsKey_ComplexLR(key);
					if (!isKeyInlined && !isValInlined)
						TypeAsKey_ComplexLR(key);
					ImGui::PopID();

					// value
					ImGui::PushID(++id);
					if (isKeyInlined && isValInlined)
						TypeAsValue_TrivialR(value);
					if (isKeyInlined && !isValInlined)
						TypeAsValue_ComplexLR(value);
					if (!isKeyInlined && isValInlined)
						TypeAsValue_TrivialLR(value);
					if (!isKeyInlined && !isValInlined)
						TypeAsValue_ComplexLR(value);
					ImGui::PopID();

					imgui::SetColumnIndex(0);
					ImGui::Unindent();
				}

				// #todo: itr->first != key, remove and replace
			}
		}
	}

	template<typename FieldDescriptor, typename Type>
	void FieldAsContainer(const FieldDescriptor fieldDescriptor, std::vector<Type>& container)
	{
		using TypeDescriptor = refl::type_descriptor<Type>;
		constexpr TypeDescriptor typeDescriptor = refl::reflect<Type>();
		constexpr const char* fieldName = reflect::GetFieldName(fieldDescriptor);

		if constexpr (!refl::trait::is_reflectable<Type>::value)
			return;

		imgui::SetColumnIndex(0);
		bool isExpanded = widget::CollapsingHeader(fieldName);

		imgui::SetColumnIndex(1);
		ImGui::Text("%d Array Elements", container.size());
		ImGui::SameLine();
		ImGui::Button("+"); // #todo
		ImGui::SameLine();
		ImGui::Button("x"); // #todo

		if (isExpanded)
		{
			const bool isInlined = !std::is_class<Type>::value;

			auto itr = container.begin();
			auto end = container.end();
			for (int i = 0; itr != end; ++itr, ++i)
			{
				Type& value = *itr;

				// #todo: re-orderable

				imgui::SetColumnIndex(0);
				ImGui::Indent();

				if (isInlined)
				{
					// label
					imgui::SetColumnIndex(0);
					ImGui::Indent();
					ImGui::Text("%d", i);
					ImGui::Unindent();

					//////////////////////////////////////////////////////////////////////////

					// value
					imgui::SetColumnIndex(1);
					ImGui::PushID(i + 1);
					editor::InspectorType(value);
					ImGui::PopID();

					// #todo: add settings dropdown
				}
				else
				{
					const str::String label = std::to_string(i);

					// label
					imgui::SetColumnIndex(0);
					bool isOpen = widget::CollapsingHeader(label.c_str());

					//////////////////////////////////////////////////////////////////////////

					// info
					imgui::SetColumnIndex(1);
					ImGui::Text("%d Members", typeDescriptor.members.size);

					// #todo: add settings dropdown

					//////////////////////////////////////////////////////////////////////////

					// value
					if (isOpen)
					{
						imgui::SetColumnIndex(0);
						editor::InspectorType<Type>(value);
					}
				}

				imgui::SetColumnIndex(0);
				ImGui::Unindent();
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////

	template<typename FieldDescriptor, typename Variant>
	void FieldAsVariant(const FieldDescriptor fieldDescriptor, Variant& variant)
	{
		ImGui::Text("Unsupported Variant Type!");
	}

	template<typename FieldDescriptor, typename ...Types>
	void FieldAsVariant(const FieldDescriptor fieldDescriptor, std::variant<Types...>& variant)
	{
		using Variant = std::variant<Types...>;
		using Builder = typename core::VariantBuilder<Variant>::type;

		// #todo: build this compile time
		static std::vector<const char*> names = core::TypeNames<Types...>();
		constexpr const char* name = reflect::GetFieldName(fieldDescriptor);

		int index = static_cast<int>(variant.index());
		int size = static_cast<int>(sizeof...(Types));

		imgui::SetColumnIndex(0);
		bool isExpanded = widget::CollapsingHeader(name);

		imgui::SetColumnIndex(1);
		if (ImGui::Combo("", &index, &names[0], size))
			variant = Builder::variants[index];

		if (isExpanded)
		{
			imgui::SetColumnIndex(0);
			ImGui::Indent();

			std::visit([&](auto&& typeValue)
			{
				using Type = decltype(typeValue);
				const bool isTrivial = !std::is_class<Type>::value;
				if (isTrivial)
				{
					TypeAsValue_TrivialR(typeValue);
				}
				else
				{
					TypeAsValue_ComplexLR(typeValue);
				}
			}, variant);

			imgui::SetColumnIndex(0);
			ImGui::Unindent();
		}
	}
}

//////////////////////////////////////////////////////////////////////////

template<typename FieldDescriptor, typename Type>
void editor::InspectorField(const FieldDescriptor fieldDescriptor, Type& value)
{
	constexpr bool isReflectable = refl::trait::is_reflectable<Type>::value;
	constexpr const char* name = reflect::GetFieldName(fieldDescriptor);

	ImGui::PushID(name);
	if constexpr (isReflectable && std::is_class<Type>::value)
	{
		FieldAsClass(fieldDescriptor, value);
	}
	else if constexpr (core::IsVariant<Type>::value)
	{
		FieldAsVariant(fieldDescriptor, value);
	}
	// #note: containers aren't reflectable, so they will need to check internally
	else if constexpr (refl::trait::is_container<Type>::value)
	{
		FieldAsContainer(fieldDescriptor, value);
	}
	else
	{
		// #todo: override base

		imgui::SetColumnIndex(0);
		ImGui::Indent();
		ImGui::Text(name);
		ImGui::Unindent();

		imgui::SetColumnIndex(1);
		editor::InspectorType(value);

		// #todo: revert to default
	}
	ImGui::PopID();
}

//////////////////////////////////////////////////////////////////////////

template<typename Type>
void editor::InspectorType(Type& value)
{
	if (std::is_class<Type>::value)
	{
		for_each(refl::reflect<Type>().members, [&](auto field)
		{
			auto& fieldDescriptor = field;
			auto& fieldValue = field(value);
			editor::InspectorField(fieldDescriptor, fieldValue);
		});
	}
	else
	{
		ImGui::Text("Unsupported Type!");
	}
}

template<>
void editor::InspectorType(bool& value)
{
	ImGui::Checkbox("", &value);
}

template<>
void editor::InspectorType(int& value)
{
	ImGui::DragInt("", &value);
}

template<>
void editor::InspectorType(float& value)
{
	ImGui::DragFloat("", &value);
}

template<>
void editor::InspectorType(sf::Vector3f& value)
{
	ImGui::DragFloat3("", &value.x);
}
