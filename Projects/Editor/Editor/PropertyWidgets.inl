#pragma once

#include "Editor/PropertyHelpers.h"

#include <entt/meta/meta.hpp>
#include <imgui/imgui.h>

template<class Type>
void editor::PropertyWidget<Type>(Void propertyData, const entt::meta_data& metaData)
{
	static_assert("Undefined Property Type");
}

template<>
void editor::PropertyWidget<bool>(Void propertyData, const entt::meta_data& metaData)
{
	const char* name = PropertyName(metaData, "");
	bool* value = reinterpret_cast<bool*>(propertyData.p);
	ImGui::Checkbox(name, value);
}

template<>
void editor::PropertyWidget<int>(Void propertyData, const entt::meta_data& metaData)
{
	const char* name = PropertyName(metaData, "");
	int* value = reinterpret_cast<int*>(propertyData.p);
	ImGui::DragInt(name, value);
}

template<>
void editor::PropertyWidget<float>(Void propertyData, const entt::meta_data& metaData)
{
	const char* name = PropertyName(metaData, "");
	float* value = reinterpret_cast<float*>(propertyData.p);
	ImGui::DragFloat(name, value);
}

template<>
void editor::PropertyWidget<sf::Vector3f>(Void propertyData, const entt::meta_data& metaData)
{
	const char* name = PropertyName(metaData, "");
	sf::Vector3f* value = reinterpret_cast<sf::Vector3f*>(propertyData.p);
	ImGui::DragFloat3(name, &value->x);
}