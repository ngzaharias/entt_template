#include "Editor/EditorPCH.h"
#include "Editor/EntityBrowser.h"

#include "Editor/Inspector.h"

#include <entt/entity/registry.hpp>
#include <imgui/imgui.h>

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
	if (!m_IsVisible)
		return;

	if (ImGui::Begin("Entity Browser", &m_IsVisible, ImGuiWindowFlags_NoCollapse))
	{
		registry.each([&](const entt::entity& entity)
		{
			const uint32 value = static_cast<uint32>(entity);
			str::String name = std::to_string(value);
			if (ImGui::Selectable(name.c_str()))
				m_Inspector.SetEntity(entity);
		});
	}
	ImGui::End();
}