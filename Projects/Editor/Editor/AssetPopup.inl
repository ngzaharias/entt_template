#include "Editor/EditorPCH.h"
#include "Editor/AssetPopup.h"

#include <Engine/ResourceManager.h>

#include <imgui/Custom.h>
#include <imgui/imgui.h>

template<class TResource>
void editor::AssetPopup::Resource(core::ResourceHandle<TResource>& handle)
{
	const core::ResourceManager& resourceManager = core::ResourceManager::Instance();
	const core::ResourceEntryMap& resourceEntries = resourceManager.GetEntries();
	if (resourceEntries.count(handle.m_Guid) == 0)
		return;

	const core::ResourceEntry& resourceEntry = resourceEntries.at(handle.m_Guid);
	const char* assetName = resourceEntry.m_Filepath.GetFileNameNoExtension().data();

	if (imgui::FakeCombo("", assetName))
		ImGui::OpenPopup("AssetPopup");

	ImGui::SetNextWindowPos(ImGui::GetCursorScreenPos());

	if (ImGui::BeginPopup("AssetPopup"))
	{
		for (auto&& [guid, entry] : resourceEntries)
		{
			if (entry.m_Type != resourceEntry.m_Type)
				continue;

			if (ImGui::Button(entry.m_Filepath.ToChar()))
				handle.m_Guid = entry.m_Guid;
		}

		ImGui::EndPopup();
	}
}