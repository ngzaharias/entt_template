#include "Editor/EditorPCH.h"
#include "Editor/EntityBrowser.h"

#include "Editor/Inspector.h"

#include <Engine/CameraComponent.h>
#include <Engine/FlipbookComponent.h>
#include <Engine/NameComponent.h>
#include <Engine/SpriteComponent.h>
#include <Engine/SortHelpers.h>
#include <Engine/TransformComponent.h>

#include <entt/entity/registry.hpp>
#include <imgui/imgui.h>

namespace
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

bool editor::EntityEntry::operator==(const EntityEntry& rhs) const
{
	return m_Name == rhs.m_Name;
}

bool editor::EntityEntry::operator<(const EntityEntry& rhs) const
{
	return sort::AlphaNumeric(m_Name, rhs.m_Name);
}

editor::EntityBrowser::EntityBrowser(editor::Inspector& inspector)
	: m_Inspector(inspector)
{
}
editor::EntityBrowser::~EntityBrowser()
{
}

void editor::EntityBrowser::Initialize(entt::registry& registry)
{
}

void editor::EntityBrowser::Destroy(entt::registry& registry)
{
}

void editor::EntityBrowser::Update(entt::registry& registry, const core::GameTime& gameTime)
{
	registry.each([&](const entt::entity& entity)
	{
		const uint32 value = static_cast<uint32>(entity);
		str::String name;
		if (const auto* component = registry.try_get<core::NameComponent>(entity))
		{
			name = component->m_Name;
		}
		else
		{
			name = "<unknown>";
		}

		m_Entries.insert({ entity, name });
	});

	Render(registry);
}

void editor::EntityBrowser::Render(entt::registry& registry)
{
	if (!m_IsVisible)
		return;

	ImGuiWindowFlags flags = 0;
	flags |= ImGuiWindowFlags_MenuBar;
	flags |= ImGuiWindowFlags_NoCollapse;

	if (ImGui::Begin("Entity Browser", &m_IsVisible, flags))
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Create"))
			{
				if (ImGui::MenuItem("Empty"))
					Command_CreateEmpty(registry);
				ImGui::Separator();
				if (ImGui::MenuItem("Camera"))
					Command_CreateCamera(registry);
				if (ImGui::MenuItem("Flipbook"))
					Command_CreateFlipbook(registry);
				if (ImGui::MenuItem("Sprite"))
					Command_CreateSprite(registry);

				ImGui::EndMenu();
			}

			ImGui::Separator();
			ImGui::Text("?");
			ImGui::SetNextItemWidth(-1.f);
			InputText("", m_Filter);

			ImGui::EndMenuBar();
		}

		for (const auto& entry : m_Entries)
		{
			if (!str::Contains_NoCase(entry.m_Name, m_Filter))
				continue;

			if (ImGui::Selectable(entry.m_Name.c_str()))
				m_Inspector.SetEntity(entry.m_Entity);
		}
	}
	ImGui::End();
}

void editor::EntityBrowser::Command_CreateCamera(entt::registry& registry)
{
	entt::entity entity = registry.create();
	registry.emplace<core::CameraComponent>(entity);
	registry.emplace<core::NameComponent>(entity);
	registry.emplace<core::TransformComponent>(entity);
}

void editor::EntityBrowser::Command_CreateEmpty(entt::registry& registry)
{
	registry.create();
}

void editor::EntityBrowser::Command_CreateFlipbook(entt::registry& registry)
{
	entt::entity entity = registry.create();
	registry.emplace<core::NameComponent>(entity);
	registry.emplace<core::TransformComponent>(entity);
	registry.emplace<render::FlipbookComponent>(entity);
}

void editor::EntityBrowser::Command_CreateSprite(entt::registry& registry)
{
	entt::entity entity = registry.create();
	registry.emplace<core::NameComponent>(entity);
	registry.emplace<core::TransformComponent>(entity);
	registry.emplace<render::SpriteComponent>(entity);
}
