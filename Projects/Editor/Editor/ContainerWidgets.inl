#pragma once

#include "Editor/ContainerWidgets.h"
#include "Editor/InspectorWidgets.h"
#include "Editor/TrivialWidgets.h"

#include <Engine/AttributeHelpers.h>
#include <Engine/TypeHelpers.h>
#include <Engine/VariantHelpers.h>

#include <imgui/Custom.h>
#include <imgui/Helpers.h>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

// #todo: vector:
// - re-orderable elements
// - append new
// - clear contents
// - element options: insert, delete

// #todo: map:
// - customize isKeyInlined && !isValInlined
// - append new
// - clear contents
// - element options: insert, delete
// - rename keys
// - maintain key order after rename
// - display error on conflicting keys

//namespace imgui
//{
//	int32 DragDrop(const char* text, int32 index)
//	{
//		int32 source = -1;
//
//		ImGui::PushID(index);
//		ImGui::TableSetColumnIndex(0);
//		ImGui::Unindent();
//		ImGui::Button("::");
//		ImGui::Indent();
//		ImGui::SameLine();
//		if (ImGui::BeginDragDropSource())
//		{
//			ImGui::SetDragDropPayload("DRAG", &index, sizeof(int32));
//			ImGui::Text(text);
//			ImGui::EndDragDropSource();
//		}
//		if (ImGui::BeginDragDropTarget())
//		{
//			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DRAG"))
//			{
//				source = *(int32*)payload->Data;
//			}
//			ImGui::EndDragDropTarget();
//		}
//		ImGui::PopID();
//
//		return source;
//	}
//}

template<typename Descriptor, class Container>
void editor::InspectContainer(const char* text, Descriptor descriptor, Container& container)
{
	ImGui::Text("Unsupported Container Type!");
}

template<typename Descriptor, class Key, class Value>
void editor::InspectContainer(const char* text, Descriptor descriptor, std::map<Key, Value>& container)
{
	struct None { };
	struct Insert { Key key; };
	struct RemoveAll { };
	struct Rename { int32 index; Key key; };

	using Iterator = typename std::map<Key, Value>::iterator;
	using Command = std::variant<None, Insert, RemoveAll, Rename>;

	if constexpr (!refl::trait::is_reflectable<Key>::value || !refl::trait::is_reflectable<Value>::value)
		return;

	Command command = None();

	ImGui::TableSetColumnIndex(0);
	const bool isExpanded = imgui::InspectHeader(text);

	ImGui::TableSetColumnIndex(1);
	ImGui::Text("%d Elements", container.size());
	ImGui::SameLine(ImGui::GetColumnWidth() - 50.f);
	if (ImGui::Button("+"))
		command = Insert{ Key() };
	ImGui::SameLine();
	if (ImGui::Button("x"))
		command = RemoveAll();

	if (isExpanded)
	{
		ImGui::TableNextRow();

		ImGui::TableSetColumnIndex(0);
		ImGui::Indent();

		constexpr bool isKeyInlined = !std::is_class<Key>::value;
		constexpr bool isValInlined = !std::is_class<Value>::value;

		Iterator itr = container.begin();
		Iterator end = container.end();
		for (int32 i = 0, id = 0; itr != end; ++itr, ++i)
		{
			Key key = itr->first;
			Value& value = itr->second;

			// key
			ImGui::PushID(++id);
			if (isKeyInlined && isValInlined)
			{
				ImGui::TableSetColumnIndex(0);
				imgui::Bullet();
				ImGui::SetNextItemWidth(-1);
				editor::InspectType(key);
			}
			if (isKeyInlined && !isValInlined)
			{
				editor::InspectMember("Key", key, descriptor);
				ImGui::TableNextRow();
			}
			if (!isKeyInlined && isValInlined)
			{
				editor::InspectMember("Key", key, descriptor);
				ImGui::TableNextRow();
			}
			if (!isKeyInlined && !isValInlined)
			{
				editor::InspectMember("Key", key, descriptor);
				ImGui::TableNextRow();
			}
			ImGui::PopID();

			// value
			ImGui::PushID(++id);
			if (isKeyInlined && isValInlined)
			{
				ImGui::TableSetColumnIndex(1);
				editor::InspectType(value);
			}
			if (isKeyInlined && !isValInlined)
				editor::InspectMember("Value", value, descriptor);
			if (!isKeyInlined && isValInlined)
				editor::InspectMember("Value", value, descriptor);
			if (!isKeyInlined && !isValInlined)
				editor::InspectMember("Value", value, descriptor);
			ImGui::PopID();

			if (key != itr->first && ImGui::IsItemDeactivatedAfterEdit())
				command = Rename{ i, key };

			ImGui::TableNextRow();
		}

		ImGui::TableSetColumnIndex(0);
		ImGui::Unindent();
	}

	std::visit(core::VariantOverload
		{
			[&](const None& arg) {},
			[&](const Insert& arg) { container.try_emplace(arg.key); },
			[&](const RemoveAll& arg) { container.clear(); },
			[&](const Rename& arg)
			{
				if (container.count(arg.key) == 0)
				{
					Iterator itr = std::next(container.begin(), arg.index);
					auto node = container.extract(itr);
					node.key() = arg.key;
					container.insert(std::move(node));
				}
			}
		}, command);
}

template<typename Descriptor, typename Type>
void editor::InspectContainer(const char* text, Descriptor descriptor, std::vector<Type>& container)
{
	struct None { };
	struct DragDrop { int32 source, target; };
	struct Insert { int32 index; };
	struct RemoveAll { };

	using Command = std::variant<None, DragDrop, Insert, RemoveAll>;
	using Iterator = typename std::vector<Type>::iterator;

	if constexpr (!refl::trait::is_reflectable<Type>::value)
		return;

	Command command = None();

	ImGui::TableSetColumnIndex(0);
	const bool isExpanded = imgui::InspectHeader(text);

	ImGui::TableSetColumnIndex(1);
	ImGui::Text("%d Elements", container.size());
	ImGui::SameLine(ImGui::GetColumnWidth() - 50.f);
	if (ImGui::Button("+"))
		command = Insert{ static_cast<int32>(container.size()) };
	ImGui::SameLine();
	if (ImGui::Button("x"))
		command = RemoveAll();

	if (isExpanded)
	{
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::Indent();

		Iterator itr = container.begin();
		Iterator end = container.end();
		for (int32 i = 0; itr != end; ++itr, ++i)
		{
			Type& value = *itr;
			str::String label = std::to_string(i);

			//int32 source = imgui::DragDrop(label.c_str(), i);
			//if (source != -1)
			//{
			//	command = DragDrop{ source, i };
			//}

			editor::InspectMember(label.c_str(), value, descriptor);
			ImGui::TableNextRow();
		}

		ImGui::TableSetColumnIndex(0);
		ImGui::Unindent();
	}

	std::visit(core::VariantOverload
	{ 
		[&](auto& arg) {},
		[&](DragDrop& arg)
		{
			const bool isUnderneath = arg.source < arg.target;
			Iterator source = std::next(container.begin(), arg.source);
			Iterator target = std::next(container.begin(), isUnderneath ? arg.target + 1 : arg.target);
			container.insert(target, *source);

			// iterators were invalidated
			source = std::next(container.begin(), !isUnderneath ? arg.source + 1 : arg.source);
			container.erase(source);
		},
		[&](Insert& arg)		
		{ 
			Iterator itr = container.begin() + arg.index;
			container.insert(itr, Type());
		},
		[&](RemoveAll& arg)	{ container.clear(); },
	}, command);
}
