#include "Editor/AssetBrowser.h"

#include <Engine/ResourceManager.h>

#include <imgui/imgui.h>
#include <SFML/System/Time.hpp>

editor::AssetBrowser::AssetBrowser(core::ResourceManager& resourceManager)
	: m_ResourceManager(resourceManager)
{
}

editor::AssetBrowser::~AssetBrowser()
{
}

void editor::AssetBrowser::Initialize(entt::registry& registry)
{
}

void editor::AssetBrowser::Destroy(entt::registry& registry)
{
}

void editor::AssetBrowser::Update(entt::registry& registry, const sf::Time& time)
{
	Render(registry);
}

void editor::AssetBrowser::Render(entt::registry& registry)
{
	if (!m_IsVisible)
		return;

	ImGui::Begin("Asset Browser", &m_IsVisible, ImGuiWindowFlags_MenuBar);

	Render_MenuBar();

	for (auto&& [guid, entry] : m_ResourceManager.m_ResourceEntries)
	{
		ImGui::CollapsingHeader(entry.m_Filepath.ToChar(), ImGuiTreeNodeFlags_Bullet);
	}
	ImGui::End();
}

void editor::AssetBrowser::Render_MenuBar()
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Create"))
		{
			if (ImGui::BeginMenu("Physics\t"))
			{
				if (ImGui::MenuItem("Material"))
					m_ResourceManager.CreateResource<physics::MaterialResource>("Assets\\Temp\\Example.asset");
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}

		ImGui::MenuItem("Import...");

		ImGui::EndMenuBar();
	}
}
