#include "EnttDebugger.h"

#include "Components/CameraComponent.h"
#include "Components/LevelComponent.h"
#include "Components/NameComponent.h"
#include "Components/SpriteComponent.h"
#include "Components/TransformComponent.h"
#include "Strings/StringHelpers.h"

#include <iostream>
#include <entt/entt.hpp>
#include <imgui/imgui.h>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Keyboard.hpp>

namespace ImGui
{
	void InputText(const char* label, std::string& string)
	{
		const size_t size = 256;
		char* buffer = new char[size];
		strncpy_s(buffer, size, string.c_str(), size);

		ImGui::InputText(label, buffer, size);
		string = buffer;
	}
}

namespace
{
	const char* DebugName(const entt::registry& registry, const entt::entity& entity)
	{
		if (entity == entt::null)
			return "null";

		if (registry.valid(entity))
		{
			if (const auto& component = registry.try_get<debug::NameComponent>(entity))
				return component->m_Name.c_str();
		}
		return "<unknown> (unknown)";
	}

	bool HasComponent(const entt::registry& registry, const entt::entity& entity, const entt::id_type& typeId)
	{
		entt::id_type types[] = { typeId };
		return registry.runtime_view(std::cbegin(types), std::cend(types)).contains(entity);
	}
}

debug::EnttDebugger::EnttDebugger()
	: m_ComponentInfo()
	, m_ComponentWidgets()
	, m_ComponentSettings()
	, m_EntitySettings()
	, m_EntityInfo()
	, m_EntityOrphans()
	, m_Selection()
	, m_IsWindowVisible(true)
{
	m_Selection.Undos.Push(entt::null);
}

debug::EnttDebugger::~EnttDebugger()
{
}

void debug::EnttDebugger::Initialize(entt::registry& registry)
{
	RegisterWidget<core::CameraComponent>([](entt::registry& registry, entt::entity& entity)
	{
		auto& component = registry.get<core::CameraComponent>(entity);
		if (ImGui::CollapsingHeader("Size"))
			ImGui::DragFloat2("", &component.m_Size.x);
	});

	RegisterWidget<core::LevelComponent>([](entt::registry& registry, entt::entity& entity)
	{
		auto& component = registry.get<core::LevelComponent>(entity);
		if (ImGui::CollapsingHeader("Name"))
			ImGui::Text("%s", component.m_Name.c_str());
		if (ImGui::CollapsingHeader("Path"))
			ImGui::Text("%s", component.m_Path.c_str());
	});

	RegisterWidget<core::TransformComponent>([](entt::registry& registry, entt::entity& entity)
	{
		auto& component = registry.get<core::TransformComponent>(entity);

		ImGui::PushID("Translate");
		if (ImGui::CollapsingHeader("Translate"))
			ImGui::DragFloat3("", &component.m_Translate.x);
		ImGui::PopID();

		ImGui::PushID("Rotate");
		if (ImGui::CollapsingHeader("Rotate"))
			ImGui::DragFloat3("", &component.m_Rotate.x);
		ImGui::PopID();

		ImGui::PushID("Scale");
		if (ImGui::CollapsingHeader("Scale"))
			ImGui::DragFloat3("", &component.m_Scale.x);
		ImGui::PopID();
	});

	RegisterWidget<debug::NameComponent>([](entt::registry& registry, entt::entity& entity)
	{
		auto& component = registry.get<debug::NameComponent>(entity);
		ImGui::Text("%s", component.m_Name.c_str());
	});
}

void debug::EnttDebugger::Destroy(entt::registry& registry)
{
}

void debug::EnttDebugger::Update(entt::registry& registry, const sf::Time& time)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F11))
		m_IsWindowVisible = !m_IsWindowVisible;

	if (m_Selection.Request)
	{
		const auto& request = m_Selection.Request.value();
		switch (request.first)
		{
		case Selection::Action::Select:
			m_Selection.Current = request.second;
			m_Selection.Undos.Push(m_Selection.Current);
			m_Selection.Redos.RemoveAll();
			break;
		case Selection::Action::Undo:
			if (m_Selection.Undos.GetCount() > 1)
			{
				m_Selection.Redos.Push(m_Selection.Current);
				m_Selection.Undos.Pop();
				m_Selection.Current = m_Selection.Undos.GetLast();
			}
			break;
		case Selection::Action::Redo:
			if (!m_Selection.Redos.IsEmpty())
			{
				m_Selection.Current = m_Selection.Redos.GetLast();
				m_Selection.Undos.Push(m_Selection.Current);
				m_Selection.Redos.Pop();
			}
			break;
		}
		m_Selection.Request.reset();
	}

	if (m_ComponentSettings.IsAutoRefreshEnabled || m_ComponentSettings.IsRefreshRequested)
	{
		m_ComponentInfo.clear();
		auto itr = registry.debug.begin();
		auto end = registry.debug.end();
		for (; itr != end; ++itr)
		{
			str::TrimLeft(itr->name, "class ");
			str::TrimLeft(itr->name, "struct ");
			m_ComponentInfo.push_back({ itr->name, itr->type_id });
		}

		std::sort(m_ComponentInfo.begin(), m_ComponentInfo.end(),
			[](const auto& a, const auto& b)
			{
				return a.Name < b.Name;
			});
	}

	if (m_EntitySettings.IsAutoRefreshEnabled || m_EntitySettings.IsRefreshRequested)
	{
		m_EntityInfo.clear();
		m_EntityOrphans.clear();

		if (!m_ComponentSettings.FilterTypes.empty())
		{
			const auto& list = m_ComponentSettings.FilterTypes;
			const auto& view = registry.runtime_view(list.cbegin(), list.cend());
			for (const entt::entity& entity : view)
			{
				m_EntityInfo.emplace_back(EntityInfo{ DebugName(registry, entity), entity });
			}
		}
		else
		{
			registry.each([&](const entt::entity& entity)
			{
				m_EntityInfo.emplace_back(EntityInfo{ DebugName(registry, entity), entity });
			});
		}

		std::sort(m_EntityInfo.begin(), m_EntityInfo.end(),
			[](const auto& a, const auto& b)
			{
				return a.Name < b.Name;
			});

		registry.orphans([&](const entt::entity& entity)
		{
			m_EntityOrphans.emplace(entity);
		});
	}
}

void debug::EnttDebugger::Render(entt::registry& registry)
{
	if (!m_IsWindowVisible)
		return;

	ImGui::Begin("Entt Debugger", &m_IsWindowVisible);
	if (ImGui::BeginChild("left", { ImGui::GetWindowWidth() * 0.5f, 0 }, false))
	{
		RenderComponents(registry);
		RenderEntities(registry);
	}
	ImGui::EndChild();

	ImGui::SameLine();

	if (ImGui::BeginChild("right", { ImGui::GetWindowWidth() * 0.5f - 23.f, 0 }, false))
	{
		RenderUndoRedo(registry);
		RenderSelected(registry);
	}
	ImGui::EndChild();
	ImGui::End();
}

void debug::EnttDebugger::Select(const entt::entity& entity)
{
	m_Selection.Request = { Selection::Action::Select, entity };
}

void debug::EnttDebugger::Undo()
{
	m_Selection.Request = { Selection::Action::Undo, entt::null };
}

void debug::EnttDebugger::Redo()
{
	m_Selection.Request = { Selection::Action::Redo, entt::null };
}

void debug::EnttDebugger::RenderComponents(entt::registry& /*registry*/)
{
	if (ImGui::BeginChild("components", { 0, ImGui::GetWindowHeight() * 0.5f }, true))
	{
		if (ImGui::BeginChild("header", { 0, 60.f }))
		{
			ImGui::Text("Components: ");

			ImGui::InputText("", m_ComponentSettings.FilterText);
			ImGui::SameLine();
			if (ImGui::Button("X"))
				m_ComponentSettings.FilterText = "";

			ImGui::Checkbox("Auto Refresh", &m_ComponentSettings.IsAutoRefreshEnabled);
			ImGui::SameLine();
			if (ImGui::Button("Refresh"))
				m_ComponentSettings.IsRefreshRequested = true;
			ImGui::SameLine();
			if (ImGui::Button("Clear"))
			{
				m_ComponentInfo.clear();
				m_ComponentSettings.FilterTypes.clear();
			}
		}
		ImGui::EndChild();

		ImGui::Separator();

		if (ImGui::BeginChild("body", { 0, 0 }, false, ImGuiWindowFlags_HorizontalScrollbar))
		{
			std::vector<std::string> substrings = str::Split(m_ComponentSettings.FilterText, " ");

			for (const auto& info : m_ComponentInfo)
			{
				if (!m_ComponentSettings.FilterText.empty() && !str::ContainsAll(info.Name, substrings))
					continue;

				bool isInList = m_ComponentSettings.FilterTypes.count(info.TypeId);
				bool isActive = isInList;

				ImGui::Checkbox(info.Name.c_str(), &isActive);

				// add
				if (!isInList && isActive)
				{
					m_ComponentSettings.FilterTypes.emplace(info.TypeId);
				}
				// remove
				else if (isInList && !isActive)
				{
					m_ComponentSettings.FilterTypes.erase(info.TypeId);
				}
			}
		}
		ImGui::EndChild();
	}
	ImGui::EndChild();
}

void debug::EnttDebugger::RenderEntities(entt::registry& registry)
{
	if (ImGui::BeginChild("entities", { 0, ImGui::GetWindowHeight() * 0.5f - 3.f }, true, ImGuiWindowFlags_HorizontalScrollbar))
	{
		if (ImGui::BeginChild("header", { 0, 60.f }))
		{
			ImGui::Text(m_EntitySettings.IsShowingOrphans ? "Orphans: " : "Entities: ");

			ImGui::InputText("", m_EntitySettings.FilterText);
			ImGui::SameLine();
			if (ImGui::Button("X"))
				m_EntitySettings.FilterText = "";

			ImGui::Checkbox("Auto Refresh", &m_EntitySettings.IsAutoRefreshEnabled);
			ImGui::SameLine();
			if (ImGui::Button("Refresh"))
				m_EntitySettings.IsRefreshRequested = true;
			ImGui::SameLine();
			if (ImGui::Button("Clear"))
				m_EntityInfo.clear();

			//ImGui::SameLine();
			ImGui::Checkbox("Show Orphans", &m_EntitySettings.IsShowingOrphans);
		}
		ImGui::EndChild();

		ImGui::Separator();

		if (ImGui::BeginChild("body", { 0, 0 }, false, ImGuiWindowFlags_HorizontalScrollbar))
		{
			std::vector<std::string> substrings = str::Split(m_EntitySettings.FilterText, " ");
			if (m_EntitySettings.IsShowingOrphans)
			{
				for (const auto& entity : m_EntityOrphans)
				{
					bool isActive = entity == m_Selection.Current;;
					ImGui::PushID(static_cast<int>(entity));
					if (ImGui::Checkbox("<unknown>", &isActive))
						Select(isActive ? entt::null : entity);
					ImGui::PopID();
				}
			}
			else
			{
				for (const auto& info : m_EntityInfo)
				{
					bool isActive = info.Entity == m_Selection.Current;
					if (!registry.valid(info.Entity))
						continue;

					if (!m_EntitySettings.FilterText.empty() && !str::ContainsAll(info.Name, substrings))
						continue;

					ImGui::PushID(static_cast<int>(info.Entity));
					if (ImGui::Checkbox(info.Name.c_str(), &isActive))
						Select(isActive ? info.Entity : entt::null);
					ImGui::PopID();
				}
			}
		}
		ImGui::EndChild();
	}
	ImGui::EndChild();
}

void debug::EnttDebugger::RenderSelected(entt::registry& registry)
{
	if (ImGui::BeginChild("selected", { 0, ImGui::GetWindowHeight() }, true))
	{
		if (registry.valid(m_Selection.Current))
		{
			ImGui::PushID(static_cast<int>(m_Selection.Current));
			ImGui::Text("Name: %s", DebugName(registry, m_Selection.Current));
			ImGui::Text("ID:   %d", static_cast<int>(m_Selection.Current));

			ImGui::Separator();

			for (const auto& info : m_ComponentInfo)
			{
				if (!HasComponent(registry, m_Selection.Current, info.TypeId))
					continue;

				ImGui::PushID(info.TypeId);
				if (ImGui::CollapsingHeader(info.Name.c_str()))
				{
					ImGui::Indent();
					if (m_ComponentWidgets.count(info.TypeId))
					{
						const auto& callback = m_ComponentWidgets[info.TypeId];
						callback(registry, m_Selection.Current);
					}
					ImGui::Unindent();
				}
				ImGui::PopID();
			}
			ImGui::PopID();
		}
	}
	ImGui::EndChild();
}

void debug::EnttDebugger::RenderUndoRedo(entt::registry& registry)
{
	ImGui::Indent(10.f);
	ImGui::BeginGroup();
	{
		const int count = m_Selection.Undos.GetCount();
		const char* items[Selection::Capacity];
		for (int i = 0; i < Selection::Capacity; ++i)
		{
			const int index = count - 1 - i;
			if (index >= 0)
			{
				const entt::entity& entity = m_Selection.Undos[index];
				items[i] = DebugName(registry, entity);
			}
			else
			{
				items[i] = "";
			}
		}

		int selected = 0;
		ImGui::PushItemWidth(ImGui::GetWindowWidth() - 122.f);
		if (ImGui::Combo("##history", &selected, items, Selection::Capacity))
		{
			const int index = count - 1 - selected;
			Select(m_Selection.Undos[index]);
		}
		ImGui::PopItemWidth();
		ImGui::SameLine();
		if (ImGui::Button("Undo"))
			Undo();
		ImGui::SameLine();
		if (ImGui::Button("Redo"))
			Redo();
	}
	ImGui::EndGroup();
	ImGui::Unindent(10.f);
}
