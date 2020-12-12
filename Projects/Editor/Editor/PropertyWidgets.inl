#pragma once

#include <Engine/AttributeHelpers.h>
#include <Engine/TypeHelpers.h>
#include <Engine/VariantHelpers.h>

#include <imgui/GroupPanel.h>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

//////////////////////////////////////////////////////////////////////////

template<typename Descriptor, typename Type>
void editor::Field(const Descriptor descriptor, Type& value)
{
	constexpr bool isReflectable = refl::trait::is_reflectable<Type>::value;
	constexpr const char* name = reflect::GetFieldName(descriptor);

	ImGui::PushID(name);
	if constexpr (isReflectable && std::is_class<Type>::value)
	{
		editor::FieldAsClass(descriptor, value);
	}
	else if constexpr (core::IsVariant<Type>::value)
	{
		editor::FieldAsVariant(descriptor, value);
	}
	// #note: containers aren't reflectable, so they will need to check themselves
	else if constexpr (refl::trait::is_container<Type>::value)
	{
		editor::FieldAsContainer(descriptor, value);
	}
	else
	{
		editor::PropertyWidget(name, value);
	}
	ImGui::PopID();
}

//////////////////////////////////////////////////////////////////////////

template<class Descriptor, class Type>
void editor::FieldAsClass(const Descriptor descriptor, Type& value)
{
	constexpr const char* name = reflect::GetFieldName(descriptor);

	ImGui::Unindent(20.f);
	bool isExpanded = false;
	isExpanded |= ImGui::TreeNodeEx(name, ImGuiTreeNodeFlags_AllowItemOverlap);
	ImGui::Indent(20.f);

	ImGui::NextColumn();
	ImGui::Spacing();
	ImGui::NextColumn();

	if (isExpanded)
	{
		int32 i = 1;
		ImGui::Indent();
		for_each(refl::reflect<Type>().members, [&](auto field)
		{
			constexpr const char* childName = reflect::GetFieldName(field);
			auto& childDescriptor = field;
			auto& childValue = field(value);

			ImGui::PushID(i++);
			Field(childDescriptor, childValue);
			ImGui::PopID();
		});
		ImGui::Unindent();
		ImGui::TreePop();
	}
}

//////////////////////////////////////////////////////////////////////////

template<class Descriptor, class Container>
void editor::FieldAsContainer(const Descriptor descriptor, Container& container)
{
	ImGui::Text("Unsupported Container Type!");
}

template<class Descriptor, class Key, class Value>
void editor::FieldAsContainer(const Descriptor descriptor, std::map<Key, Value>& container)
{
	constexpr const char* name = reflect::GetFieldName(descriptor);
	if constexpr (!refl::trait::is_reflectable<Key>::value || !refl::trait::is_reflectable<Value>::value)
		return;

	if (ImGui::CollapsingHeader(name))
	{
		ImGui::Indent();
		ImGui::Columns(2, "", true);
		{
			auto itr = container.begin();
			auto end = container.end();
			for (int i = 0; itr != end; ++itr)
			{
				Key key = itr->first;
				Value& value = itr->second;
				const str::String label = std::to_string(i);

				ImGui::PushID(++i);
				ImGui::Text("Key: ");
				ImGui::SameLine();
				Field(descriptor, key);
				ImGui::PopID();

				//////////////////////////////////////////////////////////////////////////
				ImGui::NextColumn();
				//////////////////////////////////////////////////////////////////////////

				ImGui::PushID(++i);
				ImGui::Text("Value: ");
				ImGui::SameLine();
				Field(descriptor, value);
				ImGui::PopID();

				//////////////////////////////////////////////////////////////////////////
				ImGui::NextColumn();
				//////////////////////////////////////////////////////////////////////////
			}
		}
		ImGui::Columns(1);
		ImGui::Unindent();
	}
}

template<class Descriptor, class Type>
void editor::FieldAsContainer(const Descriptor descriptor, std::vector<Type>& container)
{
	constexpr const char* name = reflect::GetFieldName(descriptor);
	if constexpr (!refl::trait::is_reflectable<Type>::value)
		return;

	if (ImGui::CollapsingHeader(name))
	{
		ImGui::Indent();
		ImGui::Columns(2, "", true);
		{
			auto itr = container.begin();
			auto end = container.end();
			for (int i = 0; itr != end; ++itr, ++i)
			{
				Type& value = *itr;
				const str::String label = std::to_string(i);

				ImGui::PushID(i + 1);
				{
					ImGui::Text(label.c_str());

					//////////////////////////////////////////////////////////////////////////
					ImGui::NextColumn();
					//////////////////////////////////////////////////////////////////////////

					Field(descriptor, value);
				}

				ImGui::PopID();
				ImGui::NextColumn();
			}
		}
		ImGui::Columns(1);
		ImGui::Unindent();
	}
}

//////////////////////////////////////////////////////////////////////////

template<class Descriptor, class Variant>
void editor::FieldAsVariant(const Descriptor descriptor, Variant& variant)
{
	ImGui::Text("Unsupported Variant Type!");
}

template<class Descriptor, typename ...Types>
void editor::FieldAsVariant(const Descriptor descriptor, std::variant<Types...>& variant)
{
	using Variant = std::variant<Types...>;
	using Builder = typename core::VariantBuilder<Variant>::type;

	// #todo: build this compile time
	static std::vector<const char*> names = core::TypeNames<Types...>();
	constexpr const char* name = reflect::GetFieldName(descriptor);

	bool isExpanded = false;
	isExpanded |= ImGui::TreeNodeEx(name, ImGuiTreeNodeFlags_AllowItemOverlap);

	ImGui::NextColumn();
	ImGui::Spacing();
	ImGui::NextColumn();

	if (isExpanded)
	{
		int index = static_cast<int>(variant.index());
		int size = static_cast<int>(sizeof...(Types));

		ImGui::Indent();
		ImGui::Text("Type");
		ImGui::NextColumn();
		if (ImGui::Combo("", &index, &names[0], size))
			variant = Builder::variants[index];
		ImGui::NextColumn();
		std::visit([&](auto&& typeValue)
		{
			editor::Field(descriptor, typeValue);
		}, variant);
		ImGui::Unindent();
		ImGui::TreePop();
	}
}

//////////////////////////////////////////////////////////////////////////

template<class Type>
void editor::PropertyWidget(const char* name, Type& value)
{
	ImGui::Text("Unsupported Type!");
}

template<>
void editor::PropertyWidget(const char* name, bool& value)
{
	ImGui::Text(name);
	ImGui::NextColumn();
	ImGui::Checkbox("", &value);
	ImGui::NextColumn();
}

template<>
void editor::PropertyWidget(const char* name, int& value)
{
	ImGui::Text(name);
	ImGui::NextColumn();
	ImGui::DragInt("", &value);
	ImGui::NextColumn();
}

template<>
void editor::PropertyWidget(const char* name, float& value)
{
	ImGui::Text(name);
	ImGui::NextColumn();
	ImGui::DragFloat("", &value);
	ImGui::NextColumn();
}

template<>
void editor::PropertyWidget(const char* name, sf::Vector3f& value)
{
	ImGui::Text(name);
	ImGui::NextColumn();
	ImGui::DragFloat3("", &value.x);
	ImGui::NextColumn();
}
