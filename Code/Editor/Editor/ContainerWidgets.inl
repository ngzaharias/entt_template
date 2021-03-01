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

// #todo: map:
// - customize isKeyInlined && !isValInlined
// - append new
// - clear contents
// - element options: insert, delete
// - rename keys
// - maintain key order after rename
// - display error on conflicting keys
// - save key AND value to transaction

// #todo: vector:
// - re-orderable elements
// - append new
// - clear contents
// - element options: insert, delete

//namespace
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

template<class Container>
void editor::InspectContainer(Container& container, InspectorInfo& info)
{
	const char* name = info.m_Address.GetLast();

	ImGui::TableSetColumnIndex(0);
	imgui::Bullet();
	ImGui::Text(name);

	ImGui::TableSetColumnIndex(1);
	ImGui::Text("Unsupported Container Type!");
}

template<class Key, class Value>
void editor::InspectContainer(std::map<Key, Value>& container, InspectorInfo& info)
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
	const char* name = info.m_Address.GetLast();
	const bool isExpanded = imgui::InspectHeader(name);

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
			info.m_Address.Push(std::to_string(i));
			info.m_Address.Push("Key");
			if (isKeyInlined && isValInlined)
			{
				ImGui::TableSetColumnIndex(0);
				imgui::Bullet();
				ImGui::SetNextItemWidth(-1);
				editor::InspectType(key, info);
			}
			if (isKeyInlined && !isValInlined)
			{
				editor::InspectProperty(key, info);
				ImGui::TableNextRow();
			}
			if (!isKeyInlined && isValInlined)
			{
				editor::InspectProperty(key, info);
				ImGui::TableNextRow();
			}
			if (!isKeyInlined && !isValInlined)
			{
				editor::InspectProperty(key, info);
				ImGui::TableNextRow();
			}
			info.m_Address.Pop();
			ImGui::PopID();

			// value
			ImGui::PushID(++id);
			info.m_Address.Push("Value");
			if (isKeyInlined && isValInlined)
			{
				ImGui::TableSetColumnIndex(1);
				editor::InspectType(value, info);
			}
			if (isKeyInlined && !isValInlined)
				editor::InspectProperty(value, info);
			if (!isKeyInlined && isValInlined)
				editor::InspectProperty(value, info);
			if (!isKeyInlined && !isValInlined)
				editor::InspectProperty(value, info);
			info.m_Address.Pop();
			info.m_Address.Pop();
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
			[&](const Insert& arg) 
			{ 
				container.try_emplace(arg.key); 

				//info.m_Address.Push(args.index);
				//info.m_Transactions.emplace_back(info.m_Address, json::Object());
				//info.m_Address.Pop();
			},
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

template<typename Type>
void editor::InspectContainer(std::vector<Type>& container, InspectorInfo& info)
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
	const char* name = info.m_Address.GetLast();
	const bool isExpanded = imgui::InspectHeader(name);

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
			//int32 source = imgui::DragDrop(label.c_str(), i);
			//if (source != -1)
			//{
			//	command = DragDrop{ source, i };
			//}

			info.m_Address.Push(i);

			editor::InspectProperty(itr, info);
			ImGui::TableNextRow();

			info.m_Address.Pop();
		}

		ImGui::TableSetColumnIndex(0);
		ImGui::Unindent();
	}

	std::visit(core::VariantOverload
	{ 
		[&](auto& arg) {},
		[&](DragDrop& args)
		{
			const bool isBefore = args.source < args.target;
			Iterator source = std::next(container.begin(), args.source);
			Iterator target = std::next(container.begin(), isBefore ? args.target + 1 : args.target);
			container.insert(target, *source);

			// iterators were invalidated
			source = std::next(container.begin(), !isBefore ? args.source + 1 : args.source);
			container.erase(source);
		},
		[&](Insert& args)		
		{ 
			Iterator itr = container.begin() + args.index;
			container.insert(itr, Type());

			info.m_Address.Push(args.index);
			info.m_Transactions.emplace_back(info.m_Address, json::Object());
			info.m_Address.Pop();
		},
		[&](RemoveAll& args)
		{ 
			container.clear(); 
			json::Object object; object.SetArray();
			info.m_Transactions.emplace_back(info.m_Address, object);
		},
	}, command);
}
