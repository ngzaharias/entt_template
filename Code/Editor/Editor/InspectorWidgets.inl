#pragma once

#include "Editor/InspectorWidgets.h"

#include "Editor/ContainerWidgets.h"
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
	// #note: must be declared after the function
	template <typename A, typename B, typename = void>
	struct HasOverload : std::false_type {};
	template <typename A, typename B>
	struct HasOverload<A, B, std::void_t<decltype(editor::TypeOverload(std::declval<A>(), std::declval<B>()))>> : std::true_type {};
}

template<typename Type>
void editor::InspectType(Type& value, InspectorInfo& info)
{
	constexpr bool isClass = std::is_class<Type>::value;
	constexpr bool isReflectable = refl::trait::is_reflectable<Type>::value;
	constexpr bool isOverloaded = HasOverload<Type&, editor::InspectorInfo>();
	if constexpr (isOverloaded)
	{
		editor::TypeOverload(value, info);
	}
	else if constexpr (isClass && isReflectable)
	{
		for_each(refl::reflect<Type>().members, [&](auto field)
		{
			constexpr const char* name = reflect::GetFieldName(field);
			info.m_Address.Push(name);

			editor::InspectProperty(field(value), info);
			ImGui::TableNextRow();

			info.m_Address.Pop();
		});
	}
	else
	{
		ImGui::Text("Unsupported Type!");
	}
}

template<typename Type>
void editor::InspectProperty(Type& value, InspectorInfo& info)
{
	const char* name = info.m_Address.GetLast();
	ImGui::PushID(name);

	constexpr bool isClass = std::is_class<Type>::value;
	constexpr bool isContainer = refl::trait::is_container<Type>::value;
	constexpr bool isOverloaded = HasOverload<Type&, editor::InspectorInfo&>();
	constexpr bool isReflectable = refl::trait::is_reflectable<Type>::value;
	constexpr bool isVariant = core::IsVariant<Type>::value;

	if constexpr (isOverloaded)
	{
		ImGui::TableSetColumnIndex(0);
		imgui::Bullet();
		ImGui::Text(name);

		ImGui::TableSetColumnIndex(1);
		editor::TypeOverload(value, info);
	}
	else if constexpr (isClass && isReflectable)
	{
		editor::InspectClass(value, info);
	}
	else if constexpr (isVariant)
	{
		std::visit([&](auto& subValue)
		{
			editor::InspectVariant(value, subValue, info);
		}, value);
	}
	else if constexpr (isContainer)
	{
		// #note: we only reflect the container content type which is done internally
		editor::InspectContainer(value, info);
	}
	else
	{
		ImGui::TableSetColumnIndex(0);
		imgui::Bullet();
		ImGui::Text(name);

		ImGui::TableSetColumnIndex(1);
		ImGui::Text("Unsupported Type!");
	}

	ImGui::PopID();
}

template<typename Type>
void editor::InspectClass(Type& value, InspectorInfo& info)
{
	const char* name = info.m_Address.GetLast();

	ImGui::TableSetColumnIndex(0);
	const bool isExpanded = imgui::InspectHeader(name);

	ImGui::TableSetColumnIndex(1);
	ImGui::Text("%d Members", refl::reflect<Type>().members.size);

	if (isExpanded)
	{
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::Indent();

		editor::InspectType(value, info);

		ImGui::TableSetColumnIndex(0);
		ImGui::Unindent();
	}
}