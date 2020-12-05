#pragma once

#include "Editor/PropertyHelpers.h"

#include <Engine/String.h>
#include <Engine/StringTable.h>

#include <variant>
#include <entt/meta/meta.hpp>
#include <entt/meta/resolve.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
template<class... Ts> overload(Ts...)->overload<Ts...>;

template<class Type>
void editor::PropertyWidget<Type>(editor::Any propertyData, const entt::meta_data& metaData)
{
	static_assert("Undefined Property Type");
}

template<>
void editor::PropertyWidget<bool>(editor::Any propertyData, const entt::meta_data& metaData)
{
	const char* name = PropertyName(metaData, "");
	bool& value = propertyData.data.cast<bool>();
	ImGui::Checkbox(name, &value);
	propertyData.data = value;
}

template<>
void editor::PropertyWidget<int>(editor::Any propertyData, const entt::meta_data& metaData)
{
	const char* name = PropertyName(metaData, "");
	int& value = propertyData.data.cast<int>();
	ImGui::DragInt(name, &value);
	propertyData.data = value;
}

template<>
void editor::PropertyWidget<float>(editor::Any propertyData, const entt::meta_data& metaData)
{
	const char* name = PropertyName(metaData, "");
	float& value = propertyData.data.cast<float>();
	ImGui::DragFloat(name, &value);
	propertyData.data = value;
}

template<>
void editor::PropertyWidget<sf::Vector3f>(editor::Any propertyData, const entt::meta_data& metaData)
{
	const char* name = PropertyName(metaData, "");
	sf::Vector3f& value = propertyData.data.cast<sf::Vector3f>();
	ImGui::DragFloat3(name, &value.x);
	propertyData.data = value;
}

void editor::PropertyWidget_Child(entt::meta_any propertyData, const entt::meta_data& metaData /*= nullptr*/)
{
	const entt::meta_type& metaType = propertyData.type();
	if (!metaType)
		return;

	const char* name = editor::PropertyName(metaData, nullptr);
	const bool isSkipHeader = !name;

	if (const entt::meta_func& funcCustom = metaType.func(core::strInspector))
	{
		// #todo, entt doesn't handle entt::meta_any as an arg?
		entt::meta_handle handle = { };
		editor::Any wrapper = { propertyData };
		funcCustom.invoke(handle, wrapper, metaData);
	}
	else if (metaType.is_array())
	{
		// #todo: multi-dimensional arrays?
		const entt::meta_type& childMetaType = metaType.remove_extent();
		const bool isChildAClass = childMetaType.is_class();

		if (name && !ImGui::CollapsingHeader(name))
			return;

		int i = 0;
		ImGui::Indent();
		for (entt::meta_any childData : propertyData.as_sequence_container())
		{
			ImGui::PushID(++i);
			PropertyWidget_Child(childData);
			ImGui::PopID();
		}
		ImGui::Unindent();
	}
	else if (metaType.is_associative_container())
	{
		editor::PropertyWidget_AssociativeContainer(propertyData, metaData);
	}
	else if (metaType.is_sequence_container())
	{
		editor::PropertyWidget_SequenceContainer(propertyData, metaData);
	}
	else if (metaType.is_class())
	{
		if (isSkipHeader || ImGui::CollapsingHeader(name))
		{
			for (const entt::meta_data& childMetaData : metaType.data())
			{
				entt::meta_any childData = childMetaData.get(propertyData);
				entt::meta_type childMetaType = childMetaData.type();

				const char* childName = editor::PropertyName(childMetaData);

				ImGui::PushID(childName);
				ImGui::Indent();
				PropertyWidget_Child(childData, childMetaData);
				ImGui::Unindent();
				ImGui::PopID();
			}
		}
	}
}

void editor::PropertyWidget_AssociativeContainer(entt::meta_any propertyData, const entt::meta_data& metaData /*= nullptr*/)
{
	using meta_container = entt::meta_associative_container;
	using meta_iterator = meta_container::iterator;
	using meta_node = meta_iterator::reference;

	const char* name = editor::PropertyName(metaData, nullptr);
	if (name && !ImGui::CollapsingHeader(name))
		return;

	meta_container container = propertyData.as_associative_container();
	entt::meta_any toRemove = {};

	ImGui::Indent();
	{
		if (ImGui::Button("++"))
		{
			// #note: mapped_type == value (not value_type).
			entt::meta_ctor ctorK = container.key_type().ctor<>();
			entt::meta_ctor ctorV = container.mapped_type().ctor<>();
			container.insert(ctorK.invoke(), ctorV.invoke());
		}

		meta_iterator itr = container.begin();
		meta_iterator end = container.end();
		for (int i = 0; itr != end; ++itr)
		{
			meta_node node = *itr;

			ImGui::PushID(++i);
			ImGui::Text("Key:   ");
			ImGui::SameLine();
			PropertyWidget_Child(node.first);
			ImGui::PopID();

			ImGui::PushID(++i);
			ImGui::SameLine();
			ImGui::SameLine();
			if (ImGui::Button("--"))
				toRemove = node.first;
			ImGui::PopID();

			ImGui::PushID(++i);
			ImGui::Text("Value: ");
			ImGui::SameLine();
			PropertyWidget_Child(node.second);
			ImGui::PopID();

			ImGui::Separator();
		}
	}
	ImGui::Unindent();

	if (toRemove)
		container.erase(toRemove);
}

void editor::PropertyWidget_SequenceContainer(entt::meta_any propertyData, const entt::meta_data& metaData /*= nullptr*/)
{
	struct None { };
	struct Delete { int i; };
	struct DragDrop { int source, target; };
	struct Duplicate { int i; };
	struct Insert { int i; };
	struct RemoveAll { };
	using Command = std::variant<None, Delete, DragDrop, Duplicate, Insert, RemoveAll>;

	using meta_container = entt::meta_sequence_container;
	using meta_iterator = meta_container::iterator;

	Command command = None();
	meta_container container = propertyData.as_sequence_container();

	const char* name = editor::PropertyName(metaData, "?????");
	if (ImGui::CollapsingHeader(name))
	{
		ImGui::Indent();
		ImGui::Text("%d Array Elements", container.size());
		ImGui::SameLine(ImGui::GetColumnWidth() - 100.f);
		if (ImGui::Button("Append"))
			command = Insert{ 0 };
		ImGui::SameLine();
		if (ImGui::Button("RemoveAll"))
			command = RemoveAll();

		ImGui::Columns(2, "tree", true);
		{
			meta_iterator itr = container.begin();
			meta_iterator end = container.end();
			for (int i = 0; itr != end; ++itr, ++i)
			{
				entt::meta_any childData = *itr;
				entt::meta_type childMetaType = childData.type();
				const str::String label = std::to_string(i);

				ImGui::PushID(i + 1);
				{
					ImGui::Button("::");
					if (ImGui::BeginDragDropSource())
					{
						ImGui::SetDragDropPayload("DRAG", &i, sizeof(int));
						ImGui::Text(label.c_str());
						ImGui::EndDragDropSource();
					}
					if (ImGui::BeginDragDropTarget())
					{
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DRAG"))
						{
							const int data = *(const int*)payload->Data;
							command = DragDrop{ data, i };
						}
						ImGui::EndDragDropTarget();
					}

					ImGui::SameLine();
					ImGui::Selectable(label.c_str());
					ImGui::NextColumn();
					PropertyWidget_Child(childData);
					ImGui::SameLine();
					if (ImGui::ArrowButton("_", ImGuiDir_Down))
						ImGui::OpenPopup("Menu");
					if (ImGui::BeginPopup("Menu"))
					{
						if (ImGui::MenuItem("Insert"))
							command = Insert{ i };
						if (ImGui::MenuItem("Delete"))
							command = Delete{ i };
						if (ImGui::MenuItem("Duplicate"))
							command = Duplicate{ i };
						ImGui::EndPopup();
					}
				}
				ImGui::PopID();
				ImGui::NextColumn();
			}
		}
		ImGui::Columns(1);
		ImGui::Unindent();
	}

	std::visit(overload
		{
			[&](None&) {},
			[&](Delete& arg)
			{
				meta_iterator itr = container.begin();
				std::advance(itr, arg.i);
				container.erase(itr);
			},
			[&](DragDrop& arg) 
			{
				const bool isUnderneath = arg.source < arg.target;

				meta_iterator source = container.begin();
				meta_iterator target = container.begin();
				std::advance(source, arg.source);
				std::advance(target, isUnderneath ? arg.target + 1 : arg.target);

				entt::meta_any data = *source;
				container.insert(target, data);

				// iterators were invalidated
				source = container.begin();
				std::advance(source, !isUnderneath ? arg.source + 1 : arg.source);
				container.erase(source);

			},
			[&](Duplicate& arg) 
			{
				meta_iterator itr = container.begin();
				std::advance(itr, arg.i);
				entt::meta_any data = *itr;
				container.insert(itr, data);
			},
			[&](Insert& arg)
			{
				entt::meta_ctor ctor = container.value_type().ctor<>();
				meta_iterator itr = container.begin();
				std::advance(itr, arg.i);
				container.insert(itr, ctor.invoke());
			},
			[&](RemoveAll&) { container.clear(); },
		}, command);
}
