#pragma once

#include <Engine/AttributeHelpers.h>
#include <Engine/TypeHelpers.h>
#include <Engine/VariantHelpers.h>

#include <imgui/GroupPanel.h>
#include <imgui/imgui.h>

//////////////////////////////////////////////////////////////////////////

template<typename Descriptor, typename Type>
void editor::Field(const Descriptor descriptor, Type& value)
{
	constexpr bool isReflectable = refl::trait::is_reflectable<Type>::value;
	constexpr const char* name = reflect::GetName(descriptor);

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
		editor::PropertyWidget(value);
	}
	ImGui::PopID();
}

//////////////////////////////////////////////////////////////////////////

template<class Descriptor, class Type>
void editor::FieldAsClass(const Descriptor descriptor, Type& value)
{
	constexpr const char* name = reflect::GetName(descriptor);
	if (ImGui::CollapsingHeader(name))
	{
		int32 i = 1;
		ImGui::Indent();
		for_each(refl::reflect<Type>().members, [&](auto field)
		{
			auto& childDescriptor = field;
			auto& childValue = field(value);

			ImGui::PushID(i++);
			ImGui::Text("%s: ", name);
			ImGui::SameLine();
			Field(childDescriptor, childValue);
			ImGui::PopID();
		});
		ImGui::Unindent();
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
	constexpr const char* name = reflect::GetName(descriptor);
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
	constexpr const char* name = reflect::GetName(descriptor);
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

	int index = static_cast<int>(variant.index());
	int size = static_cast<int>(sizeof...(Types));
	if (ImGui::Combo("Variant", &index, &names[0], size))
		variant = Builder::variants[index];

	std::visit([&](auto&& typeValue)
	{
		editor::Field(descriptor, typeValue);
	}, variant);
}

//////////////////////////////////////////////////////////////////////////

template<class Type>
void editor::PropertyWidget(Type& value)
{
	ImGui::Text("Unsupported Type!");
}

template<>
void editor::PropertyWidget(bool& value)
{
	ImGui::Checkbox("", &value);
}

template<>
void editor::PropertyWidget(int& value)
{
	ImGui::DragInt("", &value);
}

template<>
void editor::PropertyWidget(float& value)
{
	ImGui::DragFloat("", &value);
}

template<>
void editor::PropertyWidget(sf::Vector3f& value)
{
	ImGui::DragFloat3("", &value.x);
}
