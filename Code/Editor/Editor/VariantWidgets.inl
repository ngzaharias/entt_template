#pragma once

#include "Editor/TrivialWidgets.h"
#include "Editor/VariantWidgets.h"

#include <Engine/TypeHelpers.h>
#include <Engine/VariantHelpers.h>

#include <imgui/Custom.h>
#include <imgui/imgui.h>

template<typename Type, typename Variant>
void editor::InspectVariant(Variant& variant, Type& value, InspectorInfo& info)
{
	ImGui::Text("Unsupported type of Variant!");
}

template<typename Type, typename ...Types>
void editor::InspectVariant(std::variant<Types...>& variant, Type& value, InspectorInfo& info)
{
	using Variant = std::variant<Types...>;
	using Builder = typename core::VariantBuilder<Variant>::type;
	using TypeDescriptor = refl::type_descriptor<Type>;
	constexpr TypeDescriptor typeDescriptor = refl::reflect<Type>();

	// #todo: build this compile time
	static std::vector<const char*> names = core::TypeNames<Types...>(); 
	const char* name = info.m_Address.GetLast();

	int index = static_cast<int>(variant.index());
	int size = static_cast<int>(sizeof...(Types));

	ImGui::TableSetColumnIndex(0);
	const bool isExpanded = imgui::InspectHeader(name);

	ImGui::TableSetColumnIndex(1);
	ImGui::SetNextItemWidth(-1);
	if (ImGui::Combo("##combo", &index, &names[0], size))
	{
		rapidjson::Document document;
		rapidjson::Pointer("/0").Set(document, index);
		info.m_Transactions.emplace_back(info.m_Address, document);

		variant = Builder::variants[index];
	}

	if (isExpanded)
	{
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::Indent();

		// #todo: can this be done compile time?
		if (std::is_class<Type>::value)
		{
			ImGui::TableSetColumnIndex(0);
			bool isClassExpanded = imgui::InspectHeader("");

			ImGui::TableSetColumnIndex(1);
			ImGui::Text("%d Members", typeDescriptor.members.size);

			if (isClassExpanded)
			{
				info.m_Address.Push("1");

				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Indent();
				editor::InspectType(value, info);
				ImGui::TableSetColumnIndex(0);
				ImGui::Unindent();

				info.m_Address.Pop();
			}
		}
		else
		{
			info.m_Address.Push("1");

			ImGui::TableSetColumnIndex(1);
			editor::InspectType(value, info);

			info.m_Address.Pop();
		}

		ImGui::TableSetColumnIndex(0);
		ImGui::Unindent();
	}
}
