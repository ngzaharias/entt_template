#pragma once

#include "Editor/TrivialWidgets.h"
#include "Editor/VariantWidgets.h"

#include <Engine/TypeHelpers.h>
#include <Engine/VariantHelpers.h>

#include <imgui/Custom.h>
#include <imgui/imgui.h>

template<typename Type, typename Variant>
void widget::FieldAsVariant(const char* text, Variant& variant, Type& value)
{
	ImGui::Text("Unsupported type of Variant!");
}

template<typename Type, typename ...Types>
void widget::FieldAsVariant(const char* text, std::variant<Types...>& variant, Type& value)
{
	using Variant = std::variant<Types...>;
	using Builder = typename core::VariantBuilder<Variant>::type;
	using TypeDescriptor = refl::type_descriptor<Type>;
	constexpr TypeDescriptor typeDescriptor = refl::reflect<Type>();

	// #todo: build this compile time
	static std::vector<const char*> names = core::TypeNames<Types...>(); 

	int index = static_cast<int>(variant.index());
	int size = static_cast<int>(sizeof...(Types));

	imgui::SetColumnIndex(0);
	bool isExpanded = imgui::FieldHeader(text);

	imgui::SetColumnIndex(1);
	ImGui::Text("");

	if (isExpanded)
	{
		// #todo: can this be done compile time?
		if (std::is_class<Type>::value)
		{
			imgui::SetColumnIndex(0);
			bool isClassExpanded = imgui::ArrowButton("##button");
			ImGui::SameLine();
			if (ImGui::Combo("##combo", &index, &names[0], size))
				variant = Builder::variants[index];

			imgui::SetColumnIndex(1);
			ImGui::Text("%d Members", typeDescriptor.members.size);

			if (isClassExpanded)
			{
				imgui::SetColumnIndex(0);
				imgui::Indent_x(4);

				widget::TypeAsIs(value);

				imgui::SetColumnIndex(0);
				imgui::Unindent_x(4);
			}
		}
		else
		{
			imgui::SetColumnIndex(0);
			imgui::Indent_x(2);
			if (ImGui::Combo("##combo", &index, &names[0], size))
				variant = Builder::variants[index];
			imgui::Unindent_x(2);

			imgui::SetColumnIndex(1);
			widget::TypeAsIs(value);
		}
	}
}
