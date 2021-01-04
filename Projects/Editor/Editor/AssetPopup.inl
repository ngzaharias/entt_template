#include "Editor/EditorPCH.h"
#include "Editor/AssetPopup.h"

#include <Engine/AssetHelpers.h>
#include <Engine/AssetManager.h>

#include <imgui/Custom.h>
#include <imgui/imgui.h>

template<typename Type>
void editor::AssetPopup::SelectOne(core::AssetHandle<Type>& handle)
{
	core::AssetManager& assetManager = core::AssetManager::Instance();
	const core::AssetEntryMap& assetEntries = assetManager.GetEntries();
	const core::EAssetType assetType = core::ToAssetType<Type>();

	str::String assetName = "None";
	if (handle)
	{
		const core::Asset& asset = handle.get();
		assetName = asset.m_Filepath.GetFileNameNoExtension();
	}

	if (imgui::FakeCombo("", assetName.c_str()))
		ImGui::OpenPopup("AssetPopup");

	ImGui::SetNextWindowPos(ImGui::GetCursorScreenPos());

	if (ImGui::BeginPopup("AssetPopup"))
	{
		for (auto&& [guid, entry] : assetEntries)
		{
			// #todo: highlight if current asset
			if (entry.m_Type != assetType)
				continue;

			assetName = entry.m_Filepath.GetPathNoExtension();
			if (ImGui::Button(assetName.c_str()))
			{
				handle = assetManager.LoadAsset<Type>(entry.m_Guid);
				ImGui::CloseCurrentPopup();
			}
		}

		ImGui::EndPopup();
	}
}