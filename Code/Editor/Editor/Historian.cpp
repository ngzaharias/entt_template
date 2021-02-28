#include "EditorPCH.h"
#include "Editor/Historian.h"

#include "Editor/ComponentList.h"
#include "Editor/Transaction.h"

#include <Engine/InputComponent.h>

#include <imgui/imgui.h>
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <SFML/Window/Keyboard.hpp>

#include <iostream>

namespace
{
	using Key = sf::Keyboard::Key;

	void PrintDocument(const rapidjson::Document& document)
	{
		rapidjson::StringBuffer buffer;
		rapidjson::PrettyWriter writer(buffer);
		document.Accept(writer);
		std::cout << buffer.GetString() << "\n";
	}
}

editor::Record::Record(const editor::Record& rhs)
{
	m_Entity = rhs.m_Entity;
	m_Document.CopyFrom(rhs.m_Document, m_Document.GetAllocator());
}

void editor::Record::operator=(const editor::Record& rhs) noexcept
{
	m_Entity = rhs.m_Entity;
	m_Document.CopyFrom(rhs.m_Document, m_Document.GetAllocator());
}

editor::Historian::Historian()
{
}

editor::Historian::~Historian()
{
}

void editor::Historian::Initialize(entt::registry& registry)
{
}

void editor::Historian::Destroy(entt::registry& registry)
{
}

void editor::Historian::Update(entt::registry& registry, const core::GameTime& gameTime)
{
	for (auto& entity : registry.view<core::InputComponent>())
	{
		const auto& component = registry.get<core::InputComponent>(entity);
		if (component.IsKeyHeld(Key::LControl) && component.IsKeyPressed(Key::Z))
		{
			if (component.IsKeyHeld(Key::LShift))
				RedoRecord(registry);
			else
				UndoRecord(registry);
		}
		if (component.IsKeyHeld(Key::LControl) && component.IsKeyPressed(Key::Y))
			RedoRecord(registry);
	}

	Render(registry);
}

void editor::Historian::PushRecord(const Record& recordOld, const Record& recordNew)
{
	m_Pending[recordNew.m_Entity] = recordNew;
	m_UndoRecords.Push(recordOld);
	m_RedoRecords.RemoveAll();
}

void editor::Historian::UndoRecord(entt::registry& registry)
{
	if (!m_UndoRecords.IsEmpty())
	{
		// push redo
		editor::Record& recordNew = m_UndoRecords.GetLast();
		editor::Record& recordOld = m_Pending[recordNew.m_Entity];
		m_RedoRecords.Push(recordOld);

		// apply changes
		PrintDocument(recordNew.m_Document);
		CopyToEntity(recordNew, registry, s_ComponentList);
		m_Pending[recordNew.m_Entity] = recordNew;

		// pop undo
		m_UndoRecords.Pop();
	}
}

void editor::Historian::RedoRecord(entt::registry& registry)
{
	if (!m_RedoRecords.IsEmpty())
	{
		// push undo
		editor::Record& recordNew = m_RedoRecords.GetLast();
		editor::Record& recordOld = m_Pending[recordNew.m_Entity];
		m_UndoRecords.Push(recordOld);

		// apply changes
		CopyToEntity(recordNew, registry, s_ComponentList);
		m_Pending[recordNew.m_Entity] = recordNew;

		// pop redo
		m_RedoRecords.Pop();
	}
}

void editor::Historian::Render(entt::registry& registry)
{
	if (!IsVisible())
		return;

	if (ImGui::Begin("Historian", &m_IsVisible, ImGuiWindowFlags_MenuBar))
	{
		ImGui::BeginTable("Historian", 2, ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_Resizable);
		ImGui::TableNextRow();

		// undo
		{
			ImGui::TableSetColumnIndex(0);
			if (ImGui::Button("Undo") && !m_UndoRecords.IsEmpty())
				UndoRecord(registry);
			ImGui::Separator();

			for (int32 i = 0; i < m_UndoRecords.GetCount(); ++i)
				ImGui::Text("Entity: [%d]", m_UndoRecords[i].m_Entity);
		}

		// redo
		{
			ImGui::TableSetColumnIndex(1);
			if (ImGui::Button("Redo") && !m_RedoRecords.IsEmpty())
				RedoRecord(registry);
			ImGui::Separator();

			for (int32 i = 0; i < m_RedoRecords.GetCount(); ++i)
				ImGui::Text("Entity: [%d]", m_RedoRecords[i].m_Entity);
		}

		ImGui::EndTable();
	}
	ImGui::End();
}
