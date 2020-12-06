#pragma once

#include <Engine/AttributeHelpers.h>

#include <imgui/imgui.h>

template<class Descriptor, class Type>
void editor::PropertyWidget(const Descriptor& descriptor, Type& value)
{
	const char* name = reflect::GetName(descriptor);
	ImGui::Text("%s : ", name);
	ImGui::SameLine();
	ImGui::Text("Unsupported Type!");
}

template<class Descriptor>
void editor::PropertyWidget(const Descriptor& descriptor, bool& value)
{
	const char* name = reflect::GetName(descriptor);
	ImGui::Text("%s : ", name);
	ImGui::SameLine();
	ImGui::Checkbox("", &value);
}

template<class Descriptor>
void editor::PropertyWidget(const Descriptor& descriptor, int& value)
{
	const char* name = reflect::GetName(descriptor);
	ImGui::Text("%s : ", name);
	ImGui::SameLine();
	ImGui::DragInt("", &value);
}

template<class Descriptor>
void editor::PropertyWidget(const Descriptor& descriptor, float& value)
{
	const char* name = reflect::GetName(descriptor);
	ImGui::Text("%s : ", name);
	ImGui::SameLine();
	ImGui::DragFloat("", &value);
}

template<class Descriptor>
void editor::PropertyWidget(const Descriptor& descriptor, sf::Vector3f& value)
{
	const char* name = reflect::GetName(descriptor);
	ImGui::Text("%s : ", name);
	ImGui::SameLine();
	ImGui::DragFloat3("", &value.x);
}