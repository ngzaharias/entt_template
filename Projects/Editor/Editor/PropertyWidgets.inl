#pragma once

#include "Editor/PropertyHelpers.h"

#include <entt/meta/meta.hpp>
#include <imgui/imgui.h>

template<class Type>
void editor::PropertyWidget<Type>(editor::Any propertyData, const entt::meta_data& metaData)
{
	static_assert("Undefined Property Type");
}

template<>
void editor::PropertyWidget<bool>(editor::Any propertyData, const entt::meta_data& metaData)
{
	const char* name = PropertyName(metaData, "");
	bool* value = reinterpret_cast<bool*>(propertyData.data.data());
	ImGui::Checkbox(name, value);
}

template<>
void editor::PropertyWidget<int>(editor::Any propertyData, const entt::meta_data& metaData)
{
	const char* name = PropertyName(metaData, "");
	int* value = reinterpret_cast<int*>(propertyData.data.data());
	ImGui::DragInt(name, value);
}

template<>
void editor::PropertyWidget<float>(editor::Any propertyData, const entt::meta_data& metaData)
{
	const char* name = PropertyName(metaData, "");
	float* value = reinterpret_cast<float*>(propertyData.data.data());
	ImGui::DragFloat(name, value);
}

template<>
void editor::PropertyWidget<sf::Vector3f>(editor::Any propertyData, const entt::meta_data& metaData)
{
	const char* name = PropertyName(metaData, "");
	sf::Vector3f* value = reinterpret_cast<sf::Vector3f*>(propertyData.data.data());
	ImGui::DragFloat3(name, &value->x);
}