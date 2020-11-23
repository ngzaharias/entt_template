#include "Editor/Inspector.h"

#include <Engine/LevelComponent.h>
#include <Engine/NameComponent.h>
#include <Engine/ResourceManager.h>
#include <Engine/TransformComponent.h>

#include <entt/entity/registry.hpp>
#include <imgui/imgui.h>
#include <SFML/System/Time.hpp>

namespace
{
	const entt::id_type s_TransformTypeId = entt::type_info<core::TransformComponent>::id();

	const char* DebugName(const entt::registry& registry, const entt::entity& entity)
	{
		if (entity == entt::null)
			return "null";

		if (registry.valid(entity))
		{
			if (const auto& component = registry.try_get<debug::NameComponent>(entity))
				return component->m_Name.c_str();
		}
		return "<unknown>";
	}

	bool HasComponent(const entt::registry& registry, const entt::entity& entity, const entt::id_type& typeId)
	{
		entt::id_type types[] = { typeId };
		return registry.runtime_view(std::cbegin(types), std::cend(types)).contains(entity);
	}
}

editor::Inspector::Inspector(core::ResourceManager& resourceManager)
	: m_ResourceManager(resourceManager)
{
}

editor::Inspector::~Inspector()
{
}

void editor::Inspector::Initialize(entt::registry& registry)
{
	m_Entity = registry.create();
	registry.emplace<core::LevelComponent>(m_Entity);
	registry.emplace<debug::NameComponent>(m_Entity);
	registry.emplace<core::TransformComponent>(m_Entity);
}

void editor::Inspector::Destroy(entt::registry& registry)
{
}

void editor::Inspector::Update(entt::registry& registry, const sf::Time& time)
{
	Render(registry);
}

void editor::Inspector::Render(entt::registry& registry)
{
	if (!m_IsVisible)
		return;

	ImGui::Begin("Inspector", &m_IsVisible, ImGuiWindowFlags_MenuBar);

	Render_MenuBar(registry);
	Render_Entity(registry);

	ImGui::End();
}

void editor::Inspector::Render_Entity(entt::registry& registry)
{
	if (ImGui::BeginChild("entity"))
	{
		if (registry.valid(m_Entity))
		{
			ImGui::PushID(static_cast<int>(m_Entity));

			if (HasComponent(registry, m_Entity, s_TransformTypeId))
			{
				auto& transform = registry.get<core::TransformComponent>(m_Entity);

				ImGui::PushID(s_TransformTypeId);
				ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_DefaultOpen);
				ImGui::Text("Translate: ");
				ImGui::SameLine();
				ImGui::DragFloat3("##t", &transform.m_Translate.x);
				ImGui::Text("Rotate:    ");
				ImGui::SameLine();
				ImGui::DragFloat3("##r", &transform.m_Rotate.x);
				ImGui::Text("Scale:     ");
				ImGui::SameLine();
				ImGui::DragFloat3("##s", &transform.m_Scale.x);
				ImGui::PopID();
			}

			auto itr = registry.debug.begin();
			auto end = registry.debug.end();
			for (; itr != end; ++itr)
			{
				if (itr->type_id == s_TransformTypeId)
					continue;

				if (!HasComponent(registry, m_Entity, itr->type_id))
					continue;

				str::TrimLeft(itr->name, "class ");
				str::TrimLeft(itr->name, "struct ");

				ImGui::PushID(itr->type_id);
				ImGui::CollapsingHeader(itr->name.c_str(), ImGuiTreeNodeFlags_Bullet);
				ImGui::PopID();
			}

			ImGui::PopID();
		}
		ImGui::EndChild();
	}
}

void editor::Inspector::Render_MenuBar(entt::registry& registry)
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("(dummy)"))
		{
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}
}
